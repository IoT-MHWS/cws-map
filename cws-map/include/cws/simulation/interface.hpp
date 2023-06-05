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
  using Queue = std::queue<T>;

private:
  SimulationMaster * master;

  struct {
    SimulationStateIn state;
    mutable std::mutex stateMutex;

    Queue<SubjectModifyQuery> subQueries;
    mutable std::mutex subQMutex;

    Queue<AirInsertQuery> airQueries;
    mutable std::mutex airQMutex;

    Queue<std::unique_ptr<SubjectCallbackQ>> callbQueries;
    mutable std::mutex callbMutex;

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

  void addModifyQuery(SubjectModifyQuery && query);
  void addModifyQuery(AirInsertQuery && query);
  void addModifyQuery(std::unique_ptr<SubjectCallbackQ> && query);

private:
  SimulationStateIn masterGetState();

  Optional<Dimension> masterGetDimension();

  std::pair<std::unique_lock<std::mutex> &&, Queue<SubjectModifyQuery> &>
  masterAccessSubjectMQs();

  std::pair<std::unique_lock<std::mutex> &&, Queue<AirInsertQuery> &>
  masterAccessAirMQs();

  std::pair<std::unique_lock<std::mutex> &&, Queue<std::unique_ptr<SubjectCallbackQ>> &>
  masterAccessCallbackMQs();

  void masterSet(const SimulationState & state, const SimulationMap * map);
  void masterSet(const SimulationState & state);

public:
  void run();
  void exit();
};
