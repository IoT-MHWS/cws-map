#pragma once

#include <memory>

#include "cws/layers/subjects.hpp"
#include "cws/layers/temperature.hpp"
#include "general.hpp"
#include "map_layer.hpp"

/*
 * Stays same while state changes
 */
class Structures {
  MapLayer<LayerSubject> subjects;

public:
  Structures(Dimension dimension) : subjects(dimension) {}

  const MapLayer<LayerSubject> & getSubjectLayer() const { return subjects; }
};

/*
 * Changes every call for the next state
 */
class LayersState {
  MapLayer<LayerTemperature> tempLayer;

public:
  LayersState(Dimension dimension) : tempLayer(dimension){};

  const MapLayer<LayerTemperature> & getTemperatureLayer() const { return tempLayer; }

  void buildNextState(const LayersState & state, const Structures & structures);
};

/*
 * Saved states of map
 * NOTE: may be will have to use shared_pointers for multithreading
 */
class States {
  LayersState currentState;

public:
  States(Dimension dimension) : currentState(dimension) {}

  const LayersState & getCurrentState() const { return currentState; }

  void buildNextState(const Structures & structures, const Dimension & dimension);
};

/*
 * Non-concurrent map
 */
class Map {
  Structures structures;
  States states;
  Dimension dimension;

public:
  Map(Dimension dimension)
      : structures(dimension), states(dimension), dimension(dimension) {}

  const Structures & getStructures() const { return structures; }

  const States & getStates() const { return states; }

  Dimension getDimension() const { return dimension; }

  void buildNextState();
};
