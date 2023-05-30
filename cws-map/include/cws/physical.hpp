#pragma once

#include "cws/common.hpp"

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
  Obstruction defLightObstruction_;

public:
  Physical(double weight = 0, int heatCapacity = 0, Temperature temp = {},
           Obstruction lightAbs = {})
      : weight_(weight), heatCapacity_(heatCapacity), temperature_(temp),
        defLightObstruction_(lightAbs) {}

  virtual ~Physical() = default;

  double getWeight() const { return weight_; }

  int getHeatCapacity() const { return heatCapacity_; }

  Temperature getTemperature() const { return temperature_; }
  void setTemperature(Temperature temp) { temperature_ = temp; }

  Obstruction getDefLightObstruction() const { return defLightObstruction_; }
  virtual Obstruction getCurLightObstruction() const { return defLightObstruction_; }

public:
  void updateTemperature(double heatProd);
};
