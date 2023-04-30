#pragma once

#include <memory>

#include "cws/layers/subjects.hpp"
#include "cws/layers/temperature.hpp"
#include "general.hpp"
#include "map_layer.hpp"

/*
 * Changes every call for the next state
 */
class LayersState {
  MapLayer<LayerTemperature> tempLayer;

public:
  LayersState(Dimension dimension) : tempLayer(dimension){};
};

/*
 * Saved states of map
 */
class States {
  std::unique_ptr<LayersState> cur;
  std::unique_ptr<LayersState> next;

public:
  States(Dimension dimension) {
    cur = std::make_unique<LayersState>(dimension);
    next = std::make_unique<LayersState>(dimension);
  }
};

/*
 * Stays same while state changes
 */
class Structures {
  MapLayer<LayerSubject> subjects;

public:
  Structures(Dimension dimension) : subjects(dimension) {}
};

class Map {
  Structures structures;
  States states;

public:
  Map(Dimension dimension) : structures(dimension), states(dimension) {}
};

