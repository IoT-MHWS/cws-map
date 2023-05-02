#pragma once

#include "cws/concurrent_map.hpp"
#include "cws/general.hpp"

/*
 * Number of iteration to simulate
 */
enum class SimulationType { LIMITED = 0, INFINITE = 1 };

class Simulation {
  std::size_t repetitions, repetitionsLeft;
  double taskFrequency;

  SimulationType simulationType;

  ConcurrentMap concurrentMap;

public:
  Simulation() : Simulation(Dimension{10, 10}) {}

  Simulation(Dimension dimension) : concurrentMap(dimension) {
    repetitions = 0;
    repetitionsLeft = 0;
    taskFrequency = 4;
    simulationType = SimulationType::LIMITED;
  }

  Simulation(Dimension dimension, SimulationType simulationType,
             std::size_t repetitionsLeft, double taskFrequency)
      : Simulation(dimension) {
    this->simulationType = simulationType;
    this->repetitionsLeft = repetitionsLeft;
    this->taskFrequency = taskFrequency;
  }

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

  // concurrent map
  const ConcurrentMap & getConcurrentMap() const { return concurrentMap; }

  void run();
};
