#pragma once

#include "cws/layer/base.hpp"
#include "cws/map_layer.hpp"
#include "cws/util.hpp"

template<Derived<Layer> T>
class MapLayerBase : public MapLayer<T> {
public:
  MapLayerBase(Dimension dimension) : MapLayer<T>(dimension) {}

  MapLayerBase(Dimension dimension, T base) : MapLayer<T>(dimension, base) {}
};
