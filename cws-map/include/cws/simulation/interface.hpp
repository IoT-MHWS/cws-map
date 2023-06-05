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

    QueueUP<SubjectModifyQuery> subQueries;
    mutable std::mutex subQMutex;

    QueueUP<AirInsertQuery> airQueries;
    mutable std::mutex airQMutex;

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

  void addModifyQuery(std::unique_ptr<SubjectModifyQuery> && query);
  void addModifyQuery(std::unique_ptr<AirInsertQuery> && query);

private:
  SimulationStateIn masterGetState();

  Optional<Dimension> masterGetDimension();

  std::pair<std::unique_lock<std::mutex> &&, QueueUP<SubjectModifyQuery> &>
  masterAccessSubjectMQs();

  std::pair<std::unique_lock<std::mutex> &&, QueueUP<AirInsertQuery> &>
  masterAccessAirMQs();

  void masterSet(const SimulationState & state, const SimulationMap * map);
  void masterSet(const SimulationState & state);

public:
  void run();
  void exit();
};
