#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

#include "cws/simulation/simulation_map.hpp"
#include <cws/simulation/interface.hpp>
#include <cws/simulation/simulation.hpp>

void SimulationMaster::run() {
  std::jthread worker2(std::bind_front(&SimulationMaster::execute, this));
  worker.swap(worker2);
}

void SimulationMaster::wait() { worker.join(); }

// called by outer thread
void SimulationMaster::exit() {
  worker.request_stop();
  this->wait();
}

void SimulationMaster::execute(std::stop_token stoken) {
  this->slave.run();

  while (true) {
    auto clockStart = std::chrono::high_resolution_clock::now();

    if (processStopRequest(stoken)) {
      return;
    }

#ifndef NDEBUG
    std::cout << "------------------------------------------" << std::endl;
#endif

    updateSimulationState();
    updateSimulationMap();

#ifndef NDEBUG
    // if (curMap.get()) {
    //   std::cout << "curMap: "
    //             << curMap.get()
    //                    ->getLayers()
    //                    .getSubjectLayer()
    //                    .getCell({.x = 0, .y = 0})
    //                    .getElement()
    //                    .getSubjectList()
    //                    .size()
    //             << std::endl;
    // }
    // if (newMap.get()) {
    //   std::cout << "newMap: "
    //             << newMap.get()
    //                    ->getLayers()
    //                    .getSubjectLayer()
    //                    .getCell({.x = 0, .y = 0})
    //                    .getElement()
    //                    .getSubjectList()
    //                    .size()
    //             << std::endl;
    // }
#endif

#ifndef NDEBUG
    std::cout << "master: " << state << std::endl;
#endif

    bool isStatusRunning = state.status == SimulationStatus::RUNNING;
    bool isNotLastTick = state.currentTick < state.lastTick;
    bool isSimTypeINF = state.type == SimulationType::INFINITE;

    if (isStatusRunning && (isSimTypeINF || isNotLastTick)) {
      state.currentTick += 1;
      notifySlaveReady();
      waitSlaveProcess();
    }

    if (state.type != SimulationType::INFINITE && state.currentTick == state.lastTick) {
      state.status = SimulationStatus::STOPPED;
    }

#ifndef NDEBUG
    std::cout << "master: " << std::endl
              << "  curMap: " << curMap.get() << std::endl
              << "  newMap: " << newMap.get() << std::endl;
#endif

    // Set current map as updated map
    if (mapExisted()) {
      auto newMapR = newMap.release();
      curMap = std::shared_ptr<SimulationMap>(newMapR);
    }

    interface.masterSet(state, curMap);

#ifndef NDEBUG
    std::cout << "master: "
              << "updated interface state" << std::endl;
#endif

    waitDurationExceeds(state, clockStart);
  };
}

bool SimulationMaster::processStopRequest(const std::stop_token & stoken) {
  bool requested = stoken.stop_requested();
  if (requested) {
    slave.exit();
  }
  return requested;
}

void SimulationMaster::updateSimulationState() {
  SimulationStateIn stateIn = interface.masterGetState();
  if (stateIn.simType.isSet()) {
    state.type = stateIn.simType.get();
  }
  if (stateIn.simStatus.isSet()) {
    state.status = stateIn.simStatus.get();
  }
  if (stateIn.currentTick.isSet()) {
    state.currentTick = stateIn.currentTick.get();
  }
  if (stateIn.lastTick.isSet()) {
    state.lastTick = stateIn.lastTick.get();
  }
  if (stateIn.taskFrequency.isSet()) {
    state.taskFrequency = stateIn.taskFrequency.get();
  }
}

void SimulationMaster::updateSimulationMap() {
  // to be synced with slave
  std::scoped_lock<std::mutex> lock(msMutex);

  Optional<Dimension> dimension = interface.masterGetDimension();

  bool curMapExist = !!curMap;

  /* If request on new map then create new currentMap
   * else if current map exists then just create newMap to store create next state*/
  if (dimension.isSet()) {
    curMap.reset(new SimulationMap(dimension.get()));
    newMap.reset();
  } else {
    if (curMapExist) {
      newMap.reset(new SimulationMap(*curMap));
    }
  }

  {
    auto [qlock, queries] = interface.masterAccessQueries();

    while (!queries.empty()) {
      const auto & query = queries.front();
      // if current map exists then apply changes to new map else to current
      if (curMapExist) {
        newMap->setQuery(std::move(*query));
      } else {
        curMap->setQuery(std::move(*query));
      }
      queries.pop();

#ifndef NDEBUG
      std::cout << "master: update query processed." << std::endl;
#endif
    }
  }
}

void SimulationMaster::notifySlaveReady() {
  {
    std::unique_lock lock(msMutex);
    runReady = true;
  }
  cv.notify_one();
}

void SimulationMaster::waitSlaveProcess() {
  std::unique_lock lock(msMutex);
  cv.wait(lock, [this] { return runProcessed; });
  runProcessed = false;
}

void SimulationMaster::waitDurationExceeds(
    const SimulationState & state,
    const std::chrono::time_point<std::chrono::high_resolution_clock> & start) {

  auto duration = static_cast<int>(1'000'000'000 / state.taskFrequency);
  std::chrono::nanoseconds taskMaxDuration(duration);

  auto leadTime = std::chrono::high_resolution_clock::now() - start;
  auto waitTime = std::max(taskMaxDuration - leadTime, std::chrono::nanoseconds(0));

  std::this_thread::sleep_for(waitTime);
}

bool SimulationMaster::mapExisted() { return !!newMap; }
