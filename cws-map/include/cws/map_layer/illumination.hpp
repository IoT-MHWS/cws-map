#pragma once

#include "cws/layer/illumination.hpp"
#include "cws/layer/subject.hpp"
#include "cws/map_layer/base.hpp"
#include "cws/map_layer/subject.hpp"

class MapLayerAbsorption : public MapLayerBase<LayerAbsorption> {
public:
  MapLayerAbsorption(Dimension dimension) : MapLayerBase<LayerAbsorption>(dimension) {}

  void update(const MapLayerSubject & layerSubject);

  Absorption getAbsorption(Coordinates c) const {
    return getCell(c).getElement().getAbsorption();
  }

  void setAbsorption(Coordinates c, Absorption abs) {
    accessCell(c).accessElement().setAbsorption(abs);
  }
};

class MapLayerIllumination : public MapLayerBase<LayerIllumination> {

public:
  MapLayerIllumination(Dimension dimension)
      : MapLayerBase<LayerIllumination>(dimension) {}

  MapLayerIllumination(Dimension dimension, Illumination base)
      : MapLayerBase<LayerIllumination>(dimension, base) {}

  void update(const MapLayerAbsorption & absorptionLayer,
              const MapLayerSubject & subjectLayer);

  void setIllumination(Coordinates coord, Illumination illum) {
    accessCell(coord).accessElement().setIllumination(illum);
  }

  Illumination getIllumination(Coordinates coord) const {
    return getCell(coord).getElement().getIllumination();
  }
};
