#pragma once

#include <memory>

#include "map_layer/subject.hpp"
#include "map_layer/temperature.hpp"

/*
 * Changes every call for the next state
 */
class Layers {
  MapLayerTemperature temperatureLayer;
  MapLayerSubject subjectLayer;

public:
  explicit Layers(Dimension dimension) : temperatureLayer(dimension), subjectLayer(dimension) {}

  const MapLayerTemperature & getTemperatureLayer() const { return temperatureLayer; }

  const MapLayerSubject & getSubjectLayer() const { return subjectLayer; }

  void nextState();
};

/*
 * Non-concurrent map
 */
class Map {
  Layers layers;
  Dimension dimension;

public:
  explicit Map(Dimension dimension) : layers(dimension), dimension(dimension) {}

  const Layers & getLayers() const { return layers; }

  Dimension getDimension() const { return dimension; }

  void nextState();
};
