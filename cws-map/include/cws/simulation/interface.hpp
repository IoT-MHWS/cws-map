#pragma once

#include <cstddef>
#include <queue>

#include "cws/simulation/general.hpp"
#include "cws/simulation/map_query.hpp"
#include "cws/simulation/simulation.hpp"

class SimulationInterface {
  friend SimulationMaster;

private:
  SimulationMaster * master;

  ModifCheck<SimulationState> currentState;
  mutable std::shared_mutex state_mutex;

  template<class T>
  using QueueUP = std::queue<std::unique_ptr<T>>;

  QueueUP<SubjectQuery> queries;
  mutable std::mutex queue_mutex;

public:
  SimulationInterface(){};

  void setSimulationMaster(SimulationMaster * master) { this->master = master; }

  void setSimulationState(const SimulationState & newState) {
    std::unique_lock lock(state_mutex);
    this->currentState.value = newState;
    this->currentState.modified = true;
  }

  void addUpdateQuery(std::unique_ptr<SubjectQuery> && query) {
    std::unique_lock lock(queue_mutex);
    queries.push(std::move(query));
  }

private:
  ModifCheck<SimulationState> masterGetSimulationState() {
    std::shared_lock lock(state_mutex);

    auto prevCurrentState = currentState;
    currentState.modified = false;

    return prevCurrentState;
  }

  std::pair<std::unique_lock<std::mutex> &&, QueueUP<SubjectQuery> &>
  masterAccessQueries() {
    std::unique_lock lock(queue_mutex);
    return std::make_pair(std::move(lock), std::ref(queries));
  }

public:
  void run();
  void exit();
};
