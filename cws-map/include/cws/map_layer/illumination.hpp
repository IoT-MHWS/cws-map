#pragma once

#include "cws/layer/illumination.hpp"
#include "cws/layer/obstruction.hpp"
#include "cws/layer/subject.hpp"
#include "cws/map_layer/base.hpp"
#include "cws/map_layer/obstruction.hpp"
#include "cws/map_layer/subject.hpp"

class MapLayerIllumination : public MapLayerBase<LayerIllumination> {

public:
  MapLayerIllumination(Dimension dimension)
      : MapLayerBase<LayerIllumination>(dimension) {}

  MapLayerIllumination(Dimension dimension, Illumination base)
      : MapLayerBase<LayerIllumination>(dimension, base) {}

  void updateIllumination(const MapLayerObstruction & obstructionLayer,
                          const MapLayerSubject & subjectLayer);

  void setIllumination(Coordinates coord, Illumination illum) {
    accessCell(coord).accessElement().setIllumination(illum);
  }

  Illumination getIllumination(Coordinates coord) const {
    return getCell(coord).getElement().getIllumination();
  }
};
