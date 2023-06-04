#pragma once

#include "cws/subject/plain.hpp"

namespace Subject {

class SensorAirTemperature : public Plain {
private:
  Temperature airTemperature_;

public:
  SensorAirTemperature(Plain && plain) : Plain(std::move(plain)) {
    setType(Type::AIR_TEMPERATURE_SENSOR);
  }

  SensorAirTemperature * clone() const override {
    return new SensorAirTemperature(*this);
  }

  Temperature getAirTemperature() const { return airTemperature_; }

  void setup(Temperature airTemperature) { airTemperature_ = airTemperature; }
};

class SensorIllumination : public Plain {
private:
  Illumination cellIllumination_;

public:
  SensorIllumination(Plain && plain) : Plain(std::move(plain)) {
    setType(Type::ILLUMINATION_SENSOR);
  }

  SensorIllumination * clone() const override { return new SensorIllumination(*this); }

  Illumination getCellIllumination() const { return cellIllumination_; }

  void setup(Illumination cellIllumination) { cellIllumination_ = cellIllumination; }
};

}// namespace Subject
