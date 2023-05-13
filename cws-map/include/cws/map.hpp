#pragma once

#include <memory>

#include "cws/map_layer/illumination.hpp"
#include "cws/map_layer/subject.hpp"
#include "cws/map_layer/temperature.hpp"

class Layers {
  MapLayerTemperature temperatureLayer;
  MapLayerIllumination illuminationLayer;
  MapLayerSubject subjectLayer;

public:
  explicit Layers(Dimension dimension)
      : temperatureLayer(dimension), illuminationLayer(dimension),
        subjectLayer(dimension) {}

  const MapLayerTemperature & getTemperatureLayer() const { return temperatureLayer; }
  MapLayerTemperature & accessTemperatureLayer() { return temperatureLayer; }

  const MapLayerSubject & getSubjectLayer() const { return subjectLayer; }
  MapLayerSubject & accessSubjectLayer() { return subjectLayer; }

  friend std::ostream & operator<<(std::ostream & out, const Layers * map);
};

class Map {
protected:
  Layers layers;
  Dimension dimension;

public:
  explicit Map(Dimension dimension) : layers(dimension), dimension(dimension) {}

  const Layers & getLayers() const { return layers; }

  Dimension getDimension() const { return dimension; }

  void nextState(const Map & cur);

  friend std::ostream & operator<<(std::ostream & out, const Map * map);
};
