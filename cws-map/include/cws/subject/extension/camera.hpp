#pragma once

#include "cws/common.hpp"
#include "cws/map_layer/illumination.hpp"
#include "cws/map_layer/obstruction.hpp"
#include "cws/map_layer/subject.hpp"
#include "cws/subject/plain.hpp"
#include <list>

namespace Subject {

using PCoordPlain = std::pair<Coordinates, Subject::Plain &>;

class ExtCamera {
public:
  virtual std::list<PCoordPlain> getVisibleSubjects() const = 0;
};

}// namespace Subject
