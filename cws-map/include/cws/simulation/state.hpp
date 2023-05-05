#pragma once

#include <cstddef>
#include <ostream>

enum class SimulationStatus { RUNNING = 0, STOPPED = 1 };

std::ostream & operator<<(std::ostream & out, const SimulationStatus & status);

enum class SimulationType { LIMITED = 0, INFINITE = 1 };

std::ostream & operator<<(std::ostream & out, const SimulationType & type);

struct SimulationState {
  SimulationType type;
  SimulationStatus status;
  std::size_t currentTick, lastTick;
  double taskFrequency;

  friend std::ostream & operator<<(std::ostream & out, const SimulationState & state);
};
