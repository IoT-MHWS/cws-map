#pragma once

#include "cws/common.hpp"
#include "cws/map.hpp"
#include "cws/simulation/general.hpp"
#include "cws/simulation/simulation_map.hpp"
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <thread>

class SimulationMaster;
class SimulationInterface;

class SimulationSlave final {
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
class SimulationMaster final {
  friend SimulationSlave;

  SimulationState state;

  SimulationInterface & interface;
  SimulationSlave slave;

  std::jthread worker;

  // shared with slave
  std::condition_variable cv;
  std::mutex msMutex;

  std::unique_ptr<SimulationMap> currMap;
  std::unique_ptr<SimulationMap> nextMap;

  bool runReady = false;
  bool runProcessed = false;
  bool runDoExit = false;

public:
  SimulationMaster(SimulationInterface & interface)
      : interface(interface), slave(*this) {}

  void run();
  void wait();
  void exit();

  bool mapsExist();

private:
  void execute(std::stop_token stoken);

  bool processStopRequest(const std::stop_token & stoken);

  void updateState();
  void updateMap();
  void prepareMapsNextIter();
  void notifySlaveReady();
  void waitSlaveProcess();
  void waitDurationExceeds(
      const SimulationState & state,
      const std::chrono::time_point<std::chrono::high_resolution_clock> & start);
};
