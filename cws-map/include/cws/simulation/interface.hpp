#pragma once

#include <cstddef>
#include <queue>

#include "cws/simulation/general.hpp"
#include "cws/simulation/map_query.hpp"
#include "cws/simulation/simulation.hpp"

class SimulationInterface {
  friend SimulationMaster;

  template<class T>
  using QueueUP = std::queue<std::unique_ptr<T>>;

private:
  SimulationMaster * master;

  struct {
    SimulationStateIn state;
    mutable std::shared_mutex stateMutex;

    QueueUP<SubjectQuery> queries;
    mutable std::mutex queueMutex;
  } in;

  struct {
    SimulationState state;
    mutable std::shared_mutex mutex;
  } out;

public:
  SimulationInterface(){};

  void setSimulationMaster(SimulationMaster * master) { this->master = master; }

  void setState(const SimulationStateIn & newState);

  SimulationState getState();

  void addQuery(std::unique_ptr<SubjectQuery> && query);

private:
  SimulationStateIn masterGetState();

  void masterSetState(const SimulationState & state);

  std::pair<std::unique_lock<std::mutex> &&, QueueUP<SubjectQuery> &>
  masterAccessQueries();

public:
  void run();
  void exit();
};
