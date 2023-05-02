#pragma once

#include "cws/general.hpp"
#include "cws/map.hpp"

/*
 * Number of iteration to simulate
 */
enum class SimulationType { LIMITED = 0, INFINITE = 1 };

class Simulation {
  std::size_t repetitions, repetitionsLeft;
  double taskFrequency;
  SimulationType simulationType;

  Map map;

public:
  Simulation() : map(Dimension{10, 10}) {}

  // repetitions
  std::size_t getRepetitions() const { return repetitions; }

  void setRepetitions(std::size_t repetitions) { this->repetitions = repetitions; };

  // repetitionsLeft
  std::size_t getRepetitionsLeft() const { return repetitionsLeft; }

  void setRepetitionsLeft(std::size_t repetitionsLeft) {
    this->repetitionsLeft = repetitionsLeft;
  };

  // taskFrequency
  double getTaskFrequency() const { return taskFrequency; }

  void setTaskFrequency(double taskFrequency) { this->taskFrequency = taskFrequency; };

  // simulationType
  SimulationType getSimulationType() const { return simulationType; }

  void setSimulationType(SimulationType simulationType) {
    this->simulationType = simulationType;
  }

  void run();
};
