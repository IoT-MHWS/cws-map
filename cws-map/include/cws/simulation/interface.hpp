#pragma once

#include <cstddef>
#include <mutex>
#include <queue>
#include <shared_mutex>

#include "cws/simulation/general.hpp"
#include "cws/simulation/simulation_map.hpp"

class SimulationMaster;

class SimulationInterface final {
  friend SimulationMaster;

  template<class T>
  using QueueUP = std::queue<std::unique_ptr<T>>;

private:
  SimulationMaster * master;

  struct {
    SimulationStateIn state;
    mutable std::mutex stateMutex;

    QueueUP<SubjectQuery> queries;
    mutable std::mutex queueMutex;

    Optional<Dimension> dimension;// if set then new map creation request
    mutable std::mutex dimensionMutex;
  } in;

  struct {
    SimulationState state;
    std::shared_ptr<const SimulationMap> map;
    mutable std::shared_mutex mutex;
  } out;

public:
  SimulationInterface(){};

  void setSimulationMaster(SimulationMaster * master) { this->master = master; }

  SimulationState getState() const;
  void setState(const SimulationStateIn & newState);

  void setDimension(const Dimension & dimension);

  std::shared_ptr<const SimulationMap> getMap() const;

  void addQuerySet(std::unique_ptr<SubjectQuery> && query);

private:
  SimulationStateIn masterGetState();

  Optional<Dimension> masterGetDimension();

  std::pair<std::unique_lock<std::mutex> &&, QueueUP<SubjectQuery> &>
  masterAccessQueries();

  void masterSet(const SimulationState & state, const SimulationMap * map);

public:
  void run();
  void exit();
};
