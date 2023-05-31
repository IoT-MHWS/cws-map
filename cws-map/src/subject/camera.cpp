#include "cws/subject/camera.hpp"

using namespace Subject;
using PCoordPlain = BaseCamera::PCoordPlain;

std::list<PCoordPlain> InfraredCamera::getVisibleSubjects() const {
  std::list<PCoordPlain> result;
  return result;
}

std::list<PCoordPlain> LightCamera::getVisibleSubjects() const {
  std::list<PCoordPlain> result;
  return result;
}

// namespace Subject
