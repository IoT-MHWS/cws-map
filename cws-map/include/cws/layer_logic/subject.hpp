#pragma once

#include "cws/layer/subject.hpp"
#include "cws/layer_logic/base.hpp"

/*
 * Extended logic for subject layer
 */
class MapLayerSubject : public MapLayerBase<LayerSubject> {
public:
  MapLayerSubject(Dimension dimension) : MapLayerBase<LayerSubject>(dimension) {}

  void nextState(const Layers & state) override;
};
