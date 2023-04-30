#pragma once

#include "cws/general.hpp"
#include "cws/layer/temperature.hpp"
#include "cws/layer_state/base.hpp"
#include "cws/map_layer.hpp"

/*
 * Logic only for temperature layer
 */
class MapLayerTemperature : public MapLayerState<LayerTemperature> {
public:
  MapLayerTemperature(Dimension dimension)
      : MapLayerState<LayerTemperature>(dimension) {}

  void buildNextState(const LayersState & state,
                      const Structures & structures) override;
};
