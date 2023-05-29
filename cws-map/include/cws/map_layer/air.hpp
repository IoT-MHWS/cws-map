#pragma once

#include "cws/layer/air.hpp"
#include "cws/map_layer/base.hpp"
#include "cws/map_layer/subject.hpp"

/*
 * Extended logic for subject layer
 */
class MapLayerAir : public MapLayerBase<LayerAir> {
public:
  MapLayerAir(Dimension dimension) : MapLayerBase<LayerAir>(dimension) {}

  void nextConvection(MapLayerSubject & subjectLayer);
  void nextConvection(MapLayerSubject & subjectLayer, Coordinates c);

  void nextMixing();
};
