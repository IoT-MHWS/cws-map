#pragma once

#include <memory>

#include "cws/map_layer/air.hpp"
#include "cws/map_layer/illumination.hpp"
#include "cws/map_layer/subject.hpp"

struct Layers {
public:
  MapLayerSubject subjectLayer;

  MapLayerAbsorption absorptionLayer;
  MapLayerIllumination illuminationLayer;

  MapLayerAir airLayer;

public:
  explicit Layers(Dimension dimension)
      : subjectLayer(dimension), absorptionLayer(dimension),
        illuminationLayer(dimension), airLayer(dimension) {}

public:
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

  void update(const Map & cur);

  friend std::ostream & operator<<(std::ostream & out, const Map * map);
};
