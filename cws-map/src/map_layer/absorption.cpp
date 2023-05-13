#include "cws/layer/illumination.hpp"
#include "cws/map_layer/illumination.hpp"
#include <algorithm>
#include <cassert>

Absorption calcCellAbsorption(std::vector<Absorption> & absVector) {
  std::sort(
      absVector.begin(), absVector.end(),
      [](const Absorption & lsv, const Absorption & rsv) -> bool { return lsv > rsv; });
  Absorption res{.value = 1};
  for (const auto & abs : absVector) {
    res = res - res * abs;
  }
  return Absorption{.value = 1} - res;
}

void MapLayerAbsorption::update(const MapLayerSubject & layerSubject) {
  auto dimension = getDimension();
  assert(dimension == layerSubject.getDimension());

  Coordinates c;

  for (c.x = 0; c.x < dimension.width; ++c.x) {
    for (c.y = 0; c.y < dimension.height; ++c.y) {
      auto & absCell = getCell(c);

      auto & subList = layerSubject[c].getElement().getSubjectList();
      std::vector<Absorption> absVector(subList.size());

      for (auto & sub : subList) {
        absVector.push_back(sub->getSubjectParameters().lightAbsorption);
      }

      setAbsorption(c, calcCellAbsorption(absVector));
    }
  }
}
