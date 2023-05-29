#pragma once

#include "cws/general.hpp"
#include "cws/layer/obstruction.hpp"
#include "cws/map_layer/subject.hpp"

class MapLayerObstruction : public MapLayerBase<LayerObstruction> {
public:
  MapLayerObstruction(Dimension dimension)
      : MapLayerBase<LayerObstruction>(dimension) {}

  void updateObstruction(const MapLayerSubject & layerSubject);

  Obstruction getLightObstruction(Coordinates c) const {
    return getCell(c).getElement().getLightObstruction();
  }

  void setLightObstruction(Coordinates c, Obstruction abs) {
    accessCell(c).accessElement().setLightObstruction(abs);
  }

  Obstruction getAirObstruction(Coordinates c) const {
    return getCell(c).getElement().getAirObstruction();
  }

  void setAirObstruction(Coordinates c, Obstruction abs) {
    accessCell(c).accessElement().setAirObstruction(abs);
  }
};
