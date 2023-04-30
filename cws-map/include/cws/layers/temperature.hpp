#pragma once

#include "base.hpp"

struct Temperature {
  double temp;
};

class LayerTemperature : public BaseLayer {
  Temperature temperature;

public:
  LayerTemperature() {}

  explicit LayerTemperature(Temperature t) : temperature(t) {}

  Temperature getTemperature() { return temperature; }
};
