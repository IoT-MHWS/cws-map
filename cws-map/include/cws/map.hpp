#pragma once

#include <memory>

#include "cws/layer_state/temperature.hpp"
#include "cws/layer_structure/subject.hpp"

/*
 * Stays same while state changes
 */
class Structures {
  MapLayerSubject subjects;

public:
  Structures(Dimension dimension) : subjects(dimension) {}

  const MapLayerSubject & getSubjectLayer() const { return subjects; }
};

/*
 * Changes every call for the next state
 */
class LayersState {
  MapLayerTemperature tempLayer;

public:
  LayersState(Dimension dimension) : tempLayer(dimension){};

  const MapLayerTemperature & getTemperatureLayer() const { return tempLayer; }

  void buildNextState(const LayersState & state, const Structures & structures);
};

/*
 * Saved states of map
 * NOTE: may be will have to use shared_pointers for multithreading
 */
class States {
  LayersState stateCurrent;

public:
  States(Dimension dimension) : stateCurrent(dimension) {}

  const LayersState & getCurrentState() const { return stateCurrent; }

  void nextState(const Structures & structures, const Dimension & dimension);
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

  void nextState();
};
