#pragma once

#include "cws/general.hpp"
#include "cws/subject/base.hpp"

enum class SensorType {
  UNSPECIFIED = 0,
  TEMPERATURE = 1,
  HUMIDITY = 2,
  LUMINOSITY = 3,
};

class SubjectSensor : public Subject {
  SensorType sensorType;

public:
  SubjectSensor(SensorType sensorType, int idx, SubjectParameters parameters)
      : Subject(SubjectId{SubjectType::SENSOR, idx}, parameters),
        sensorType(sensorType) {}

  SubjectSensor * clone() const override { return new SubjectSensor(*this); }

  SensorType getSensorType() const { return sensorType; }
};

/*
 * Temperature sensor
 */
class SensorTemperature : public SubjectSensor {
  Temperature sensorTemp;

public:
  SensorTemperature(int idx, SubjectParameters subjectParameters,
                    Temperature sensorTemp)
      : SubjectSensor(SensorType::TEMPERATURE, idx, subjectParameters),
        sensorTemp(sensorTemp) {}

  SensorTemperature(int idx) : SensorTemperature(idx, {}, {}) {}

  SensorTemperature * clone() const override { return new SensorTemperature(*this); }

  Temperature getTemperature() const { return sensorTemp; }

  void nextState(SubjectId subjectId, const Layers & layers) override;
};
