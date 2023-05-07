#pragma once

#include "cws/layer/temperature.hpp"
#include "cws/map_layer/base.hpp"

/*
 * Extended logic for temperature layer
 */
class MapLayerTemperature : public MapLayerBase<LayerTemperature> {
public:
  MapLayerTemperature(Dimension dimension)
      : MapLayerBase<LayerTemperature>(dimension) {}

  void nextState(const Layers & state) override;

  // friend std::ostream & operator<<(std::ostream & out,
  //                                  const MapLayerTemperature * layer);
};
