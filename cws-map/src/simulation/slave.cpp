#include "cws/simulation/simulation.hpp"

/* ExecutorSlave */
void SimulationSlave::run() {
  std::thread worker2(&SimulationSlave::execute, this);
  worker.swap(worker2);
}

void SimulationSlave::wait() { worker.join(); }

// called by master
void SimulationSlave::exit() {
  {
    std::unique_lock lock(master.run_mutex);
    master.runDoExit = true;
  }
  master.cv.notify_one();
  this->wait();
}

void SimulationSlave::execute() {
  auto & mutex = master.run_mutex;
  auto & cv = master.cv;
  auto & runReady = master.runReady;
  auto & runProcessed = master.runProcessed;
  auto & doExit = master.runDoExit;

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

void SimulationSlave::updateSimulationMap() {
  // const auto &[lock, map] = 
  std::cout << "slave: Simulation map next state generated." << std::endl;
}
