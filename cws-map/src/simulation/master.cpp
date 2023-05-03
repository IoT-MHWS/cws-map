#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

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

    updateSimulationState();
    updateSimulationMap();

#ifndef NDEBUG
    std::cout << "master: "
              << "(" << state.currentTick << "/" << state.lastTick << ")" << std::endl;
#endif

    if (state.currentTick < state.lastTick) {
      state.currentTick += 1;

      notifySlaveReady();
      waitSlaveProcess();
    }
    waitDurationExceeds(state, clockStart);
  }
}

bool SimulationMaster::processStopRequest(const std::stop_token & stoken) {
  bool requested = stoken.stop_requested();
  if (requested) {
    slave.exit();
  }
  return requested;
}

// Read request change queue and apply it to this state
void SimulationMaster::updateSimulationState() {
  // SimulationState simulationState = simulation.getSimulationState();
  // simulation.setSimulationState(std::move(simulationState));
  std::cout << "master: Simulation state updated." << std::endl;
}

// Read request change queue and apply it to this state (same)
void SimulationMaster::updateSimulationMap() {
  std::cout << "master: Simulation map updated." << std::endl;
}

void SimulationMaster::notifySlaveReady() {
  {
    std::unique_lock lock(mutex);
    runReady = true;
  }
  cv.notify_one();
}

void SimulationMaster::waitSlaveProcess() {
  std::unique_lock lock(mutex);
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
