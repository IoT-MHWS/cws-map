#pragma once

#include "cws/common.hpp"
#include "cws/map_layer/illumination.hpp"
#include "cws/map_layer/obstruction.hpp"
#include "cws/map_layer/subject.hpp"
#include "cws/subject/plain.hpp"
#include <list>

namespace Subject {

using PCoordPlain = std::pair<Coordinates, Subject::Plain &>;

class ExtInfraredCamera {
public:
  virtual std::list<PCoordPlain>
  getVisibleSubjects(const MapLayerSubject & layerSubject,
                     const MapLayerObstruction & layerObstruction) = 0;
};

class ExtLightCamera {
  virtual std::list<PCoordPlain>
  getVisibleSubjects(const MapLayerSubject & layerSubject,
                     const MapLayerObstruction & layerObstruction,
                     const MapLayerIllumination & layerIllumination) = 0;
};

}
