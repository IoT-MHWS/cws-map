#pragma once

#include "cws/layer/subject.hpp"
#include "cws/map_layer/base.hpp"

/*
 * Extended logic for subject layer
 */
class MapLayerSubject : public MapLayerBase<LayerSubject> {
public:
  MapLayerSubject(Dimension dimension) : MapLayerBase<LayerSubject>(dimension) {}

  void nextState(const Layers & state) override;
};
