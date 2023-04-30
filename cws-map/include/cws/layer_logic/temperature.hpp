#pragma once

#include "cws/layer/temperature.hpp"
#include "cws/layer_logic/base.hpp"

/*
 * Extended logic for temperature layer
 */
class MapLayerTemperature : public MapLayerBase<LayerTemperature> {
public:
  MapLayerTemperature(Dimension dimension)
      : MapLayerBase<LayerTemperature>(dimension) {}

  void nextState(const Layers & state) override;
};
