#include "cws/simulation/interface.hpp"


void SimulationInterface::run() { master->run(); }

void SimulationInterface::exit() { master->exit(); }


void SimulationInterface::setState(const SimulationStateIn & newState) {
  std::unique_lock lock(in.stateMutex);
  this->in.state.set(newState);
}

SimulationState SimulationInterface::getState() {
  std::shared_lock lock(out.mutex);
  return this->out.state;
}

void SimulationInterface::addQuery(std::unique_ptr<SubjectQuery> && query) {
  std::unique_lock lock(in.queueMutex);
  in.queries.push(std::move(query));
}


SimulationStateIn SimulationInterface::masterGetState() {
  std::shared_lock lock(in.stateMutex);
  auto prev = this->in.state;
  this->in.state.reset();
  return prev;
}

void SimulationInterface::masterSetState(const SimulationState &state) {
  std::unique_lock lock(out.mutex);
  this->out.state = state;
}

std::pair<std::unique_lock<std::mutex> &&, SimulationInterface::QueueUP<SubjectQuery> &>
SimulationInterface::masterAccessQueries() {
  std::unique_lock lock(in.queueMutex);
  return std::make_pair(std::move(lock), std::ref(in.queries));
}

