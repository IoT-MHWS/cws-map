#include "cws/subject/camera.hpp"

using namespace Subject;
using PCoordPlain = BaseCamera::PCoordPlain;

std::list<PCoordPlain>
InfraredCamera::getVisibleSubjects(const MapLayerSubject & layerSubject,
                                   const MapLayerObstruction & layerObstruction) {
  std::list<PCoordPlain> result;
  return result;
}

std::list<PCoordPlain>
getVisibleSubjects(const MapLayerSubject & layerSubject,
                   const MapLayerObstruction & layerObstruction,
                   const MapLayerIllumination & layerIllumination) {
  std::list<PCoordPlain> result;
  return result;
}

// namespace Subject
