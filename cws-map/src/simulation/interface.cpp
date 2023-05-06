#include "cws/simulation/interface.hpp"

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

Optional<Dimension> SimulationInterface::getDimension() const {
  Optional<Dimension> dimension;
  std::shared_lock lock(out.mutex);
  if (out.map) {
    dimension.set(out.map->getDimension());
  }
  return dimension;
}

void SimulationInterface::setDimension(const Dimension & dimension) {
  std::unique_lock lock(in.dimensionMutex);
  in.dimension.set(dimension);
}

std::shared_ptr<const Map> SimulationInterface::getMap() const {
  std::shared_lock lock(out.mutex);
  return out.map;
}

void SimulationInterface::addQuery(std::unique_ptr<SubjectQuery> && query) {
  std::unique_lock lock(in.queueMutex);
  in.queries.push(std::move(query));
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

void SimulationInterface::masterSet(SimulationState state,
                                    std::shared_ptr<Map> map) {
  std::unique_lock lock(out.mutex);
  this->out.state = state;
  this->out.map = map;
}

std::pair<std::unique_lock<std::mutex> &&, SimulationInterface::QueueUP<SubjectQuery> &>
SimulationInterface::masterAccessQueries() {
  std::unique_lock lock(in.queueMutex);
  return std::make_pair(std::move(lock), std::ref(in.queries));
}
