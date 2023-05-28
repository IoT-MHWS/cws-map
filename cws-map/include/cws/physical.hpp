#pragma once

#include "cws/general.hpp"

/*
 * Parameters can be alterable and always same, it depends on object state. To
 * implement this methods that return current values are marked virtual to
 * override them further
 */
class Physical {
private:
  double weight_;
  int heatCapacity_;
  Temperature temperature_;
  Absorption defLightAbsorption_;

public:
  Physical(double weight = 0, int heatCapacity = 0, Temperature temp = {},
           Absorption lightAbs = {})
      : weight_(weight), heatCapacity_(heatCapacity), temperature_(temp),
        defLightAbsorption_(lightAbs) {}

  virtual ~Physical() = default;

  double getWeight() const { return weight_; }
  int getHeatCapacity() const { return heatCapacity_; }
  Temperature getTemperature() const { return temperature_; }
  void setTemperature(Temperature temp) { temperature_ = temp; }

  Absorption getDefLightAbsorption() const { return defLightAbsorption_; }
  virtual Absorption getCurLightAbsorption() const { return defLightAbsorption_; }

public:
  void updateTemperature(double heatProd);
};
