#pragma once

#include "cws/general.hpp"
#include "cws/layer/base.hpp"

class LayerTemperature : public Layer {
  Temperature temperature;

public:
  LayerTemperature() {}

  explicit LayerTemperature(Temperature t) : temperature(t) {}

  Temperature getTemperature() const { return temperature; }
};
