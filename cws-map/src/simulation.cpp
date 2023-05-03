#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

#include <cws/simulation.hpp>

/* Simulation */
void Simulation::run() { master.run(); }

void Simulation::exit() { master.exit(); }

/* ExecutorMaster */
void Simulation::ExecutorMaster::run() {
  std::jthread worker2(std::bind_front(&Simulation::ExecutorMaster::execute, this));
  worker.swap(worker2);
}

void Simulation::ExecutorMaster::wait() { worker.join(); }

// called by outer thread
void Simulation::ExecutorMaster::exit() {
  worker.request_stop();
  this->wait();
}

void Simulation::ExecutorMaster::execute(std::stop_token stoken) {
  this->slave.run();

  while (true) {
    auto clockStart = std::chrono::high_resolution_clock::now();

    if (processStopRequest(stoken)) {
      return;
    }

    updateSimulationState();
    updateSimulationMap();

    auto simState = simulation.getSimulationState();

    // if not running -> increases load on polling for simulation update
    if (simState.currentTick <= simState.lastTick) {
      notifySlaveReady();
      waitSlaveProcess();
      waitDurationExceeds(simState, clockStart);
    }
  }
}

bool Simulation::ExecutorMaster::processStopRequest(const std::stop_token & stoken) {
  bool requested = stoken.stop_requested();
  if (requested) {
    slave.exit();
  }
  return requested;
}

void Simulation::ExecutorMaster::updateSimulationState() {
  SimulationState simulationState = simulation.getSimulationState();
  simulationState.currentTick += 1;
  simulation.setSimulationState(std::move(simulationState));
}

void Simulation::ExecutorMaster::updateSimulationMap() {
  std::cout << "Simulation map updating..." << std::endl;
}

void Simulation::ExecutorMaster::notifySlaveReady() {
  {
    std::unique_lock lock(mutex);
    runReady = true;
  }
  cv.notify_one();
}

void Simulation::ExecutorMaster::waitSlaveProcess() {
  std::unique_lock lock(mutex);
  cv.wait(lock, [this] { return runProcessed; });
  runProcessed = false;
}

void Simulation::ExecutorMaster::waitDurationExceeds(
    const SimulationState & state,
    const std::chrono::time_point<std::chrono::high_resolution_clock> & start) {

  auto duration = static_cast<int>(1'000'000'000 / state.taskFrequency);
  std::chrono::nanoseconds taskMaxDuration(duration);

  auto leadTime = std::chrono::high_resolution_clock::now() - start;
  auto waitTime = std::max(taskMaxDuration - leadTime, std::chrono::nanoseconds(0));

  std::this_thread::sleep_for(waitTime);
}

/* ExecutorSlave */
void Simulation::ExecutorSlave::run() {
  std::thread worker2(&Simulation::ExecutorSlave::execute, this);
  worker.swap(worker2);
}

void Simulation::ExecutorSlave::wait() { worker.join(); }

// called by master
void Simulation::ExecutorSlave::exit() {
  {
    std::unique_lock lock(master.mutex);
    master.doExit = true;
  }
  master.cv.notify_one();
  this->wait();
}

void Simulation::ExecutorSlave::execute() {
  auto & mutex = master.mutex;
  auto & cv = master.cv;
  auto & runReady = master.runReady;
  auto & runProcessed = master.runProcessed;
  auto & doExit = master.doExit;

  while (true) {
    std::unique_lock lock(mutex);

    cv.wait(lock, [&runReady, &doExit] { return runReady || doExit; });

    if (runReady) {
      runReady = false;
      updateSimulationMap();
      runProcessed = true;
    }

    lock.unlock();
    cv.notify_one();

    if (doExit) {
      break;
    }
  }
}

void Simulation::ExecutorSlave::updateSimulationMap() {
  std::cout << "Simulation map updating..." << std::endl;
}
