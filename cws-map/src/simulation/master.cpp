#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
// #include <sys/prctl.h>
#include <thread>

#include "cws/simulation/interface.hpp"
#include "cws/simulation/simulation.hpp"
#include "cws/simulation/simulation_map.hpp"

void SimulationMaster::run() {
  std::jthread tmp(std::bind_front(&SimulationMaster::execute, this));
  worker.swap(tmp);
}

void SimulationMaster::wait() { worker.join(); }

void SimulationMaster::exit() {
  worker.request_stop();
  this->wait();
}

void SimulationMaster::execute(std::stop_token stoken) {
  // prctl(PR_SET_NAME, "sim-master", 0, 0, 0);

  this->slave.run();

  while (true) {
    auto clockStart = std::chrono::high_resolution_clock::now();

    if (processStopRequest(stoken)) {
      return;
    }

#ifndef NDEBUG
    std::cout << "------------------------------------------" << std::endl;
#endif

    updateState();

#ifndef NDEBUG
    std::cout << "master: " << state << std::endl
              << "  curMap: " << currMap.get() << std::endl
              << "  newMap: " << nextMap.get() << std::endl;
#endif

    bool isStatusRunning = state.status == SimulationStatus::RUNNING;
    bool isNotLastTick = state.currentTick < state.lastTick;
    bool isSimTypeINF = state.type == SimulationType::INFINITE;
    bool isRunning = isStatusRunning && (isSimTypeINF || isNotLastTick);
    bool doSlave = isRunning && mapsExist();

    if (isRunning) {
      updateMap();
    }

    if (doSlave) {
      notifySlaveReady();
    }

    if (isRunning) {
      interface.masterSet(state, currMap.get());
#ifndef NDEBUG
      std::cout << "master->interface: state set" << std::endl
                << "  state: " << state << std::endl
                << "  map: " << &*currMap << std::endl;
#endif
    } else {
      interface.masterSet(state);
#ifndef NDEBUG
      std::cout << "master->interface: state set" << std::endl
                << "  state: " << state << std::endl;
#endif
    }

    if (isRunning) {
      state.currentTick += 1;
    }

    if (!isSimTypeINF && state.currentTick == state.lastTick) {
      state.status = SimulationStatus::STOPPED;
    }

    if (doSlave) {
      waitSlaveProcess();
    }

    if (isRunning) {
      prepareMapsNextIter();
    }

#ifndef NDEBUG
    std::cout << "master: map prepared" << std::endl
              << "  curMap: " << currMap.get() << std::endl
              << "  newMap: " << nextMap.get() << std::endl;
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

void SimulationMaster::updateState() {
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

void SimulationMaster::updateMap() {
  // to be synced with slave
  std::scoped_lock<std::mutex> lock(msMutex);

  Optional<Dimension> dimension = interface.masterGetDimension();

  if (dimension.isSet()) {
    currMap.reset(new SimulationMap(dimension.get()));
    nextMap.reset(new SimulationMap(dimension.get()));
  }

  if (!mapsExist()) {
    return;
  }

  auto [qlock, subMQs] = interface.masterAccessSubjectMQs();

  while (!subMQs.empty()) {
    const auto & subMQ = subMQs.front();
    nextMap->setModifyQuery(std::move(*subMQ));
    subMQs.pop();
#ifndef NDEBUG
    std::cout << "master: update query processed." << std::endl;
#endif
  }
}

// makes current and next state map the same
void SimulationMaster::prepareMapsNextIter() {
  if (mapsExist()) {
    *currMap = *nextMap;
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

  auto duration = static_cast<long>((long)1'000'000'000 / state.taskFrequency);
  std::chrono::nanoseconds taskMaxDuration(duration);

  auto leadTime = std::chrono::high_resolution_clock::now() - start;
  auto waitTime = std::max(taskMaxDuration - leadTime, std::chrono::nanoseconds(0));

  std::this_thread::sleep_for(waitTime);
}

bool SimulationMaster::mapsExist() { return currMap.get() && nextMap.get(); }
