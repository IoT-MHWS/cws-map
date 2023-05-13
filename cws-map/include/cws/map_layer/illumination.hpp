#pragma once

#include "cws/layer/illumination.hpp"
#include "cws/layer/subject.hpp"
#include "cws/map_layer/base.hpp"
#include "cws/map_layer/subject.hpp"

class MapLayerAbsorption : public MapLayerBase<LayerAbsorption> {
public:
  MapLayerAbsorption(Dimension dimension) : MapLayerBase<LayerAbsorption>(dimension) {}

  /*
   * Sync AbsorptionLayer with LayerSubject
   */
  void setState(const MapLayerSubject & layerSubject);
};

class MapLayerIllumination : public MapLayerBase<LayerIllumination> {

public:
  MapLayerIllumination(Dimension dimension)
      : MapLayerBase<LayerIllumination>(dimension) {}

  void setState(const MapLayerAbsorption & absorptionLayer,
                const MapLayerSubject & subjectLayer);
};
