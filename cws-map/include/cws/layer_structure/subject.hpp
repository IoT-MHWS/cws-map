#pragma once

#include "cws/general.hpp"
#include "cws/layer/subject.hpp"
#include "cws/map_layer.hpp"

/*
 * Extended logic for subject layer
 */
class MapLayerSubject : public MapLayer<LayerSubject> {
public:
  MapLayerSubject(Dimension dimension) : MapLayer<LayerSubject>(dimension){};
};
