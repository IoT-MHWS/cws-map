#include "cws/map_layer/subject.hpp"
#include "cws/subject/extension/temp_source.hpp"

using namespace Subject;

std::list<std::pair<Coordinates, const ExtLightSource *>>
MapLayerSubject::getActiveLightSources() const {

  Dimension dim = getDimension();
  std::list<std::pair<Coordinates, const ExtLightSource *>> srcs;

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      auto & cellSrcs = this->getCell(c).getElement().getActiveLightSources();
      for (const auto & src : cellSrcs) {
        srcs.emplace_back(c, src);
      }
    }
  }

  return srcs;
}

void MapLayerSubject::nextTemperature() {
  Dimension dim = getDimension();

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      auto & cellSubs = this->accessCell(c).accessElement().accessSubjectList();
      for (auto & sub : cellSubs) {
        if (auto tempSub = dynamic_cast<ExtTempSource *>(sub.get())) {
          tempSub->nextTemperature();
        }
      }
    }
  }
}
