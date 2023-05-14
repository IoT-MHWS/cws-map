#pragma once

#include "cws/general.hpp"

/*
 * Parameters can be alterable and always same, it depends on object state. To
 * implement this methods that return current values are marked virtual to
 * override them further
 */
struct Physical {
private:
  double weight_;
  int heatCapacity_;
  Temperature temperature_;

  Absorption defLightAbsorption_;

public:
  double getWeight() const { return weight_; }
  int getHeatCapacity() const { return heatCapacity_; }
  Temperature getTemperature() const { return temperature_; }

  Absorption getDefLightAbsorption() const { return defLightAbsorption_; }
  virtual Absorption getCurLightAbsorption() const {
    return defLightAbsorption_;
  }
};
