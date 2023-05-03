#pragma once

#include "cws/general.hpp"
#include "cws/map.hpp"
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <thread>

/*
 * Thread safe Simulation class
 */
enum class SimulationType { LIMITED = 0, INFINITE = 1 };

/* Moved here to easily change and lock it */
struct SimulationState {
  SimulationType simulationType;
  std::size_t currentTick, lastTick;
  double taskFrequency;
};

class Simulation {
public:
  Simulation() : master(*this), map(Dimension{10, 10}) {}

  SimulationState getSimulationState() const {
    std::shared_lock lock(state_mutex);
    return this->state;
  }

  void setSimulationState(const SimulationState & state) {
    std::unique_lock lock(state_mutex);
    this->state = state;
  }
  void setSimulationState(SimulationState && state) { this->state = std::move(state); }

  /* Get read-only map */
  std::pair<std::shared_lock<std::shared_mutex> &&, const Map &> getMap() const {
    std::shared_lock lock(map_mutex);
    return std::make_pair(std::move(lock), map);
  }

private:
  std::pair<std::unique_lock<std::shared_mutex> &&, Map &> accessMap() {
    std::unique_lock lock(map_mutex);
    return std::make_pair(std::move(lock), std::ref(map));
  }

public:
  void run();
  void exit();

private:
  class ExecutorMaster;

  /* Redundant */
  class ExecutorSlave {
    ExecutorMaster & master;

    std::thread worker; // won't use jthread because it is a consumer

  public:
    ExecutorSlave(ExecutorMaster & master) : master(master) {}

    void run();
    void wait();
    void exit();

  private:
    void execute();

    void waitMasterProcess();
    void updateSimulationMap();
    void notifyMasterReady();
  };

  class ExecutorMaster {
    Simulation & simulation;
    ExecutorSlave slave;

    std::jthread worker;

  public:
    std::condition_variable cv;
    std::mutex mutex;
    bool runReady = false;
    bool runProcessed = false;
    bool doExit = false;

  public:
    ExecutorMaster(Simulation & simulation) : simulation(simulation), slave(*this) {}

    void run();
    void wait();
    void exit();

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

private:
  ExecutorMaster master;

  SimulationState state;
  Map map;

  mutable std::shared_mutex state_mutex;
  mutable std::shared_mutex map_mutex;
};
