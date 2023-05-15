#include "cws/layer/illumination.hpp"
#include "cws/map_layer/illumination.hpp"
#include <algorithm>
#include <cassert>

static Absorption FULL_ABSORPTION{.value = 1};

Absorption calcCellAbsorption(std::vector<Absorption> & absVector) {
  std::sort(
      absVector.begin(), absVector.end(),
      [](const Absorption & lsv, const Absorption & rsv) -> bool { return lsv > rsv; });

  Absorption res = FULL_ABSORPTION;

  for (const auto & abs : absVector) {
    res = res - res * abs;
  }

  return FULL_ABSORPTION - res;
}

void MapLayerAbsorption::updateAbsorption(const MapLayerSubject & layerSubject) {
  auto dimension = getDimension();
  assert(dimension == layerSubject.getDimension());

  Coordinates c;

  for (c.x = 0; c.x < dimension.width; ++c.x) {
    for (c.y = 0; c.y < dimension.height; ++c.y) {
      auto & absCell = getCell(c);

      auto & subList = layerSubject.getSubjectList(c);
      std::vector<Absorption> absVector(subList.size());

      for (auto & sub : subList) {
        absVector.push_back(sub->getCurLightAbsorption());
      }

      setAbsorption(c, calcCellAbsorption(absVector));
    }
  }
}
