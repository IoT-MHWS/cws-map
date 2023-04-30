#pragma once

#include "base.hpp"

struct Temperature {
  double temp;
};

class LayerTemperature : public Layer {
  Temperature temperature;

public:
  LayerTemperature(){};

  explicit LayerTemperature(Temperature t)
      : Layer(LayerType::TEMPARATURE), temperature(t){};

  Temperature getTemperature() { return temperature; }
};
