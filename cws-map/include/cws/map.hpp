#pragma once

#include <memory>

#include "cws/map_layer/air.hpp"
#include "cws/map_layer/illumination.hpp"
#include "cws/map_layer/network.hpp"
#include "cws/map_layer/subject.hpp"

struct Layers final {
public:
  MapLayerSubject subjectLayer;
  MapLayerObstruction obstructionLayer;
  MapLayerIllumination illuminationLayer;
  MapLayerAir airLayer;
  MapLayerNetworkWireless networkWireless;

public:
  explicit Layers(Dimension dimension)
      : subjectLayer(dimension), obstructionLayer(dimension),
        illuminationLayer(dimension), airLayer(dimension), networkWireless(dimension) {}

public:
  friend std::ostream & operator<<(std::ostream & out, const Layers * map);
};

class Map {
protected:
  Layers layers;
  Dimension dimension;

public:
  explicit Map(Dimension dimension) : layers(dimension), dimension(dimension) {}

  virtual ~Map() = default;

  const Layers & getLayers() const { return layers; }

  Dimension getDimension() const { return dimension; }

  void next(const Map & cur);

  friend std::ostream & operator<<(std::ostream & out, const Map * map);
};
