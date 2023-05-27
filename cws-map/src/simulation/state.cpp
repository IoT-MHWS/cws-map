
#include "cws/simulation/state.hpp"

std::ostream & operator<<(std::ostream & out, const SimulationStatus & status) {
  switch (status) {
  case SimulationStatus::RUNNING:
    out << "RUNNING";
    break;
  case SimulationStatus::STOPPED:
    out << "STOPPED";
    break;
  default:
    out << "UNKNOWN";
  }
  return out;
}

std::ostream & operator<<(std::ostream & out, const SimulationType & status) {
  switch (status) {
  case SimulationType::INFINITE:
    out << "INFINITE";
    break;
  case SimulationType::LIMITED:
    out << "LIMITED";
    break;
  default:
    out << "UNKNOWN";
  }
  return out;
}

std::ostream & operator<<(std::ostream & out, const SimulationState & state) {
  out << "type: " << state.type << ", "
      << "status:" << state.status << ", "
      << "(" << state.currentTick << "/" << state.lastTick << "), "
      << "freq: " << state.taskFrequency;
  return out;
}
