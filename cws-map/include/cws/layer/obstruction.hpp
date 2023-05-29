#pragma once

#include "cws/general.hpp"
#include "cws/layer/base.hpp"

class LayerObstruction : public Layer {
  Obstruction lightObstruction_;
  Obstruction airObstruction_;

public:
  Obstruction getLightObstruction() const { return lightObstruction_; }
  void setLightObstruction(Obstruction obstruction) { lightObstruction_ = obstruction; }

  Obstruction getAirObstruction() const { return airObstruction_; }
  void setAirObstruction(Obstruction obstruction) { airObstruction_ = obstruction; }
};
