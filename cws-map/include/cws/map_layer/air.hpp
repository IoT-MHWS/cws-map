#pragma once

#include "cws/layer/air.hpp"
#include "cws/map_layer/base.hpp"

/*
 * Extended logic for subject layer
 */
class MapLayerAir : public MapLayerBase<LayerAir> {
public:
  MapLayerAir(Dimension dimension) : MapLayerBase<LayerAir>(dimension) {}

  const std::list<std::unique_ptr<Air::Plain>> & getAirList(Coordinates c) const {
    return getCell(c).getElement().getAirList();
  }
};
