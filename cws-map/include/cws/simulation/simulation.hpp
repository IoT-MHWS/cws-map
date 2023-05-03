#pragma once

#include "cws/general.hpp"
#include "cws/map.hpp"
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <thread>

class SimulationMaster;
class SimulationInterface;

enum class SimulationType { LIMITED = 0, INFINITE = 1 };

struct SimulationState {
  SimulationType simulationType;
  std::size_t currentTick, lastTick;
  double taskFrequency;
};

class SimulationSlave {
  SimulationMaster & master;
  std::thread worker;

public:
  SimulationSlave(SimulationMaster & master) : master(master) {}

  void run();
  void wait();
  void exit();

private:
  void execute();

  void waitMasterProcess();
  void updateSimulationMap();
  void notifyMasterReady();
};

/* Manages map and passed it to interface */
class SimulationMaster {
  friend SimulationSlave;

  Map map;
  SimulationState state;

  SimulationInterface & interface;

  SimulationSlave slave;

  std::jthread worker;

  std::condition_variable cv;
  std::mutex mutex;
  bool runReady = false;
  bool runProcessed = false;
  bool doExit = false;

public:
  SimulationMaster(SimulationInterface & interface)
      : map(Dimension{10, 10}), interface(interface), slave(*this) {}

  void run();
  void wait();
  void exit();

public:
  // std::pair<std::unique_lock<std::shared_mutex> &&, Map &> workersGetMap() {
  //   std::unique_lock lock(map_mutex);
  //   return std::make_pair(std::move(lock), std::ref(map));
  // }

private:
  void execute(std::stop_token stoken);

  bool processStopRequest(const std::stop_token & stoken);

  void updateSimulationState();
  void updateSimulationMap();
  void notifySlaveReady();
  void waitSlaveProcess();
  void waitDurationExceeds(
      const SimulationState & state,
      const std::chrono::time_point<std::chrono::high_resolution_clock> & start);
};
