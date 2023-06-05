#include "cws/simulation/interface.hpp"
#include "cws/simulation/simulation.hpp"

template<typename T>
using Queue = SimulationInterface::Queue<T>;

void SimulationInterface::run() { master->run(); }

void SimulationInterface::exit() { master->exit(); }

SimulationState SimulationInterface::getState() const {
  std::shared_lock lock(out.mutex);
  return this->out.state;
}

void SimulationInterface::setState(const SimulationStateIn & newState) {
  std::unique_lock lock(in.stateMutex);
  this->in.state.set(newState);
}

void SimulationInterface::setDimension(const Dimension & dimension) {
  std::unique_lock lock(in.dimensionMutex);
  in.dimension.set(dimension);
}

std::shared_ptr<const SimulationMap> SimulationInterface::getMap() const {
  std::shared_lock lock(out.mutex);
  return out.map;
}

void SimulationInterface::addModifyQuery(SubjectModifyQuery && query) {
  std::unique_lock lock(in.subQMutex);
  in.subQueries.push(std::move(query));
}

void SimulationInterface::addModifyQuery(AirInsertQuery && query) {
  std::unique_lock lock(in.airQMutex);
  in.airQueries.push(std::move(query));
}

void SimulationInterface::addModifyQuery(std::unique_ptr<SubjectCallbackQ> && query) {
  std::unique_lock lock(in.callbMutex);
  in.callbQueries.push(std::move(query));
}

SimulationStateIn SimulationInterface::masterGetState() {
  std::unique_lock lock(in.stateMutex);
  auto prev = this->in.state;
  this->in.state.reset();
  return prev;
}

Optional<Dimension> SimulationInterface::masterGetDimension() {
  std::unique_lock lock(in.dimensionMutex);
  auto prev = this->in.dimension;
  this->in.dimension.reset();
  return prev;
}

void SimulationInterface::masterSet(const SimulationState & state,
                                    const SimulationMap * map) {
  std::unique_lock lock(out.mutex);

  this->out.state = state;

  // NOTE: may do it only when input requests are waiting
  //  may results of requirests being blocked
  if (map == nullptr) {
    this->out.map.reset();
  } else {
    this->out.map = std::make_shared<SimulationMap>(*map);
  }
}

void SimulationInterface::masterSet(const SimulationState & state) {
  std::unique_lock lock(out.mutex);

  this->out.state = state;
}

std::pair<std::unique_lock<std::mutex> &&, Queue<SubjectModifyQuery> &>
SimulationInterface::masterAccessSubjectMQs() {
  std::unique_lock lock(in.subQMutex);
  return std::make_pair(std::move(lock), std::ref(in.subQueries));
}

std::pair<std::unique_lock<std::mutex> &&, Queue<AirInsertQuery> &>
SimulationInterface::masterAccessAirMQs() {
  std::unique_lock lock(in.airQMutex);
  return std::make_pair(std::move(lock), std::ref(in.airQueries));
}

std::pair<std::unique_lock<std::mutex> &&, Queue<std::unique_ptr<SubjectCallbackQ>> &>
SimulationInterface::masterAccessCallbackMQs() {
  std::unique_lock lock(in.callbMutex);
  return std::make_pair(std::move(lock), std::ref(in.callbQueries));
}
