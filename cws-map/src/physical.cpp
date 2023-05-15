#include "cws/physical.hpp"

void Physical::updateTemperature(double heatProd) {
  temperature_ =
      temperature_ + Temperature{.value = heatProd / (weight_ * heatCapacity_)};
}
