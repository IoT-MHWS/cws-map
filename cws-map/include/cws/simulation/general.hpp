#pragma once

#include "cws/simulation/state.hpp"
#include <utility>

template<typename T>
struct Optional {
private:
  T _value;
  bool _set = false;

public:
  void set(T value) {
    this->_value = value;
    this->_set = true;
  }

  void set(Optional<T> optValue) {
    if (optValue.isSet())
      this->set(optValue.get());
  }

  const T get() const { return this->_value; }

  bool isSet() const { return this->_set; };

  void reset() { this->_set = false; }
};

struct SimulationStateIn {
  Optional<SimulationType> simType;
  Optional<SimulationStatus> simStatus;
  Optional<std::size_t> currentTick, lastTick;
  Optional<double> taskFrequency;

  void set(const SimulationStateIn & in) {
    simType.set(in.simType);
    simStatus.set(in.simStatus);
    currentTick.set(in.currentTick);
    lastTick.set(in.lastTick);
    taskFrequency.set(in.taskFrequency);
  }

  void reset() {
    simType.reset();
    simStatus.reset();
    currentTick.reset();
    lastTick.reset();
    taskFrequency.reset();
  }
};
