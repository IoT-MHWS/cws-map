#pragma once

#include "cws/common.hpp"
#include "cws/layer/base.hpp"

class LayerObstruction : public Layer {
  Obstruction lightObstruction_;
  Obstruction airObstruction_;
  Obstruction wirelessObstruction_;

public:
  Obstruction getLightObstruction() const { return lightObstruction_; }
  void setLightObstruction(Obstruction obstruction) { lightObstruction_ = obstruction; }

  Obstruction getAirObstruction() const { return airObstruction_; }
  void setAirObstruction(Obstruction obstruction) { airObstruction_ = obstruction; }

  Obstruction getWirelessObstruction() const { return wirelessObstruction_; }
  void setWirelessObstruction(Obstruction obstruction) {
    wirelessObstruction_ = obstruction;
  }
};
