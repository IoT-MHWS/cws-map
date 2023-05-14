#include "cws/map_layer/subject.hpp"

std::list<std::pair<Coordinates, const Subject::LightSourceAlt *>>
MapLayerSubject::getActiveLightSources() const {

  Dimension dim = getDimension();
  std::list<std::pair<Coordinates, const Subject::LightSourceAlt *>> srcs;

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      auto cellSrcs = (*this)[c].getElement().getActiveLightSources();
      for (const auto & src : cellSrcs) {
        srcs.emplace_back(c, src);
      }
    }
  }

  return srcs;
}
