#pragma once

#include "cws/general.hpp"
#include "cws/layer/temperature.hpp"
#include <cstddef>

class Layers;

enum class SubjectType {
  UNSPECIFIED = 0,
  INTERACTIVE = 1,
  PLAIN = 2,
  SENSOR = 3,
};

struct SubjectId {
  Dimension dimension;
  std::size_t idx;
};

struct SubjectParameters {
  double weight;

  double heatCapacity;
  Percentage heatTransmission;
  Temperature temperature;

  Percentage lightTransmission;

  Percentage humidityTransmission;
};

class Subject {
protected:
  SubjectType type;
  SubjectParameters parameters;

public:
  Subject() : type(SubjectType::UNSPECIFIED) {}

  Subject(SubjectType type, SubjectParameters parameters)
      : type(type), parameters(parameters) {}

  virtual Subject * clone() const { return new Subject(*this); }

  SubjectType getSubjectType() const { return type; }
  SubjectParameters getSubjectParameters() const { return parameters; }

  virtual void nextState(SubjectId subjectId, const Layers & layers);
};
