#pragma once

#include "cws/general.hpp"
#include "cws/subject/base.hpp"

enum class SensorType {
  UNKNOWN = 0,
  TEMPERATURE = 1,
  HUMIDITY = 2,
  LUMINOSITY = 3,
};

struct SubjectSensor : public BaseSubject {
  int id;
  SensorType sensorType;

public:
  SubjectSensor(SensorType sensorType, int id, SubjectParameters parameters)
      : BaseSubject(SubjectType::SENSOR, parameters), sensorType(sensorType), id(id) {}

  SubjectSensor * clone() const override { return new SubjectSensor(*this); }

  int getId() const { return id; }
  SensorType getSensorType() const { return sensorType; }
};

/*
 * Temperature sensor
 */
struct SensorTemperature : public SubjectSensor {
  Temperature sensorTemp;

public:
  SensorTemperature(int id, SubjectParameters subjectParameters, Temperature sensorTemp)
      : SubjectSensor(SensorType::TEMPERATURE, id, subjectParameters),
        sensorTemp(sensorTemp) {}

  SensorTemperature * clone() const override { return new SensorTemperature(*this); }

  void nextState(SubjectId subjectId, const Layers & layers) override;
};
