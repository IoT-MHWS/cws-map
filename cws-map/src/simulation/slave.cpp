#include "cws/simulation/simulation.hpp"

// #include <sys/prctl.h>

void SimulationSlave::run() {
  std::thread worker2(&SimulationSlave::execute, this);
  worker.swap(worker2);
}

void SimulationSlave::wait() { worker.join(); }

// called by master
void SimulationSlave::exit() {
  {
    std::unique_lock lock(master.msMutex);
    master.runDoExit = true;
  }
  master.cv.notify_one();
  this->wait();
}

void SimulationSlave::execute() {
  // prctl(PR_SET_NAME, "sim-slave", 0, 0, 0);

  auto & msMutex = master.msMutex;
  auto & cv = master.cv;
  auto & runReady = master.runReady;
  auto & runProcessed = master.runProcessed;
  auto & doExit = master.runDoExit;

  while (true) {
    std::unique_lock lock(msMutex);

    cv.wait(lock, [&runReady, &doExit] { return runReady || doExit; });

    if (doExit) {
      break;
    }

    if (runReady) {
      runReady = false;
      updateSimulationMap();
    }

    runProcessed = true;
    lock.unlock();

    cv.notify_one();
  }
}

void SimulationSlave::updateSimulationMap() {
  if (master.newMap) {
    master.newMap->next(*master.curMap);
  }
  std::cout << "slave: "
            << "map updated" << std::endl;
}
