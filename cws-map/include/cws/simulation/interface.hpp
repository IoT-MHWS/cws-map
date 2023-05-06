#pragma once

#include <cstddef>
#include <queue>

#include "cws/simulation/general.hpp"
#include "cws/simulation/simulation.hpp"
#include "cws/simulation/simulation_map.hpp"

class SimulationInterface {
  friend SimulationMaster;

  template<class T>
  using QueueUP = std::queue<std::unique_ptr<T>>;

private:
  SimulationMaster * master;

  struct {
    SimulationStateIn state;
    mutable std::mutex stateMutex;

    QueueUP<SubjectQuerySet> queries;
    mutable std::mutex queueMutex;

    Optional<Dimension> dimension;// if set then new map creation request
    mutable std::mutex dimensionMutex;
  } in;

  struct {
    SimulationState state;
    std::shared_ptr<Map> map;
    mutable std::shared_mutex mutex;
  } out;

public:
  SimulationInterface(){};

  void setSimulationMaster(SimulationMaster * master) { this->master = master; }

  SimulationState getState() const;
  void setState(const SimulationStateIn & newState);

  void setDimension(const Dimension & dimension);

  std::shared_ptr<const Map> getMap() const;

  void addQuery(std::unique_ptr<SubjectQuerySet> && query);

private:
  SimulationStateIn masterGetState();

  Optional<Dimension> masterGetDimension();

  std::pair<std::unique_lock<std::mutex> &&, QueueUP<SubjectQuerySet> &>
  masterAccessQueries();

  void masterSet(SimulationState state, std::shared_ptr<Map> atomicMap);

public:
  void run();
  void exit();
};
