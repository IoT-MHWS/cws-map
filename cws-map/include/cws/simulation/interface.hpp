#pragma once

#include <cstddef>
#include <queue>

#include "cws/simulation/simulation.hpp"

struct MapUpdateQuery {
  // some fields to update map
};

template<typename T>
struct ModifCheck {
  T value;
  bool modified = false;
};

class SimulationInterface {
  friend SimulationMaster;

private:
  SimulationMaster * master;

  ModifCheck<SimulationState> currentState;
  mutable std::shared_mutex state_mutex;

  std::queue<std::unique_ptr<MapUpdateQuery>> queries;
  mutable std::mutex queue_mutex;

public:
  SimulationInterface(){};

  void setSimulationMaster(SimulationMaster * master) { this->master = master; }

  void setSimulationState(const SimulationState & newState) {
    std::unique_lock lock(state_mutex);
    this->currentState.value = newState;
    this->currentState.modified = true;
  }

private:
  ModifCheck<SimulationState> masterGetSimulationState() {
    std::shared_lock lock(state_mutex);

    auto prevCurrentState = currentState;
    currentState.modified = false;

    return prevCurrentState;
  }

public:
  void run();
  void exit();
};
