#pragma once

#include "cws/layer/base.hpp"
#include "cws/util.hpp"
#include "cws/map_layer.hpp"

class LayersState;
class Structures;

template<Derived<BaseLayer> T>
class MapLayerState : public MapLayer<T> {
public:
  MapLayerState(Dimension dimension) : MapLayer<T>(dimension) {}
  virtual void buildNextState(const LayersState & state,
                              const Structures & structures) = 0;
};
