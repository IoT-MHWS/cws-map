#pragma once

#include "cws/common.hpp"
#include "cws/layer/obstruction.hpp"
#include "cws/map_layer/subject.hpp"

class MapLayerObstruction : public MapLayerBase<LayerObstruction> {
public:
  MapLayerObstruction(Dimension dimension)
      : MapLayerBase<LayerObstruction>(dimension) {}

  void updateAirObstruction(const MapLayerSubject & layerSubject);

  void updateLightObstruction(const MapLayerSubject & layerSubject);

  void updateWirelessObstruction(const MapLayerSubject & layerSubject);

  Obstruction getLightObstruction(Coordinates c) const {
    return getCell(c).getElement().getLightObstruction();
  }

  void setLightObstruction(Coordinates c, Obstruction obs) {
    accessCell(c).accessElement().setLightObstruction(obs);
  }

  Obstruction getAirObstruction(Coordinates c) const {
    return getCell(c).getElement().getAirObstruction();
  }

  void setAirObstruction(Coordinates c, Obstruction obs) {
    accessCell(c).accessElement().setAirObstruction(obs);
  }

  Obstruction getWirelessObstruction(Coordinates c) const {
    return getCell(c).getElement().getWirelessObstruction();
  }

  void setWirelessObstruction(Coordinates c, Obstruction obs) {
    accessCell(c).accessElement().setWirelessObstruction(obs);
  }
};
