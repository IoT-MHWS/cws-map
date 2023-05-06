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
  SubjectType type;
  int idx;

protected:
  friend bool operator==(const SubjectId &, const SubjectId &);
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
  SubjectId id;
  SubjectParameters parameters;

public:
  Subject() : id({.type = SubjectType::UNSPECIFIED}) {}

  Subject(SubjectId id, SubjectParameters parameters)
      : id(id), parameters(parameters) {}

  virtual Subject * clone() const { return new Subject(*this); }

  SubjectId getSubjectId() const { return id; }
  SubjectParameters getSubjectParameters() const { return parameters; }

  virtual void nextState(SubjectId subjectId, const Layers & layers);

protected:
  friend bool operator==(const Subject &, const Subject &);
};
