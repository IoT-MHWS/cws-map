#include "cws/layer/illumination.hpp"
#include "cws/map_layer/illumination.hpp"
#include <algorithm>
#include <cassert>

static Obstruction FULL_OBSTRUCTION{.value = 1};

Obstruction
calcCellLightObs(const std::list<std::unique_ptr<Subject::Plain>> & subList) {

  std::vector<Obstruction> lightObsV(subList.size());
  for (auto & sub : subList)
    lightObsV.push_back(sub->getCurLightObstruction());

  std::sort(lightObsV.begin(), lightObsV.end(),
            [](const Obstruction & lsv, const Obstruction & rsv) -> bool {
              return lsv > rsv;
            });

  Obstruction res = FULL_OBSTRUCTION;

  for (const auto & obs : lightObsV) {
    res = res - res * obs;
  }

  return FULL_OBSTRUCTION - res;
}

// same algorithm
Obstruction calcCellAirObs(const std::list<std::unique_ptr<Subject::Plain>> & subList) {

  std::vector<Obstruction> airObsV(subList.size());

  for (auto & sub : subList)
    airObsV.push_back(sub->getCurAirObstruction());

  std::sort(airObsV.begin(), airObsV.end(),
            [](const Obstruction & lsv, const Obstruction & rsv) -> bool {
              return lsv > rsv;
            });

  Obstruction res = FULL_OBSTRUCTION;

  for (const auto & obs : airObsV) {
    res = res - res * obs;
  }

  return FULL_OBSTRUCTION - res;
}

void MapLayerObstruction::updateLightObstruction(const MapLayerSubject & layerSubject) {
  auto dimension = getDimension();
  assert(dimension == layerSubject.getDimension());

  Coordinates c;

  for (c.x = 0; c.x < dimension.width; ++c.x) {
    for (c.y = 0; c.y < dimension.height; ++c.y) {
      setLightObstruction(c, calcCellLightObs(layerSubject.getSubjectList(c)));
    }
  }
}

void MapLayerObstruction::updateAirObstruction(const MapLayerSubject & layerSubject) {
  auto dimension = getDimension();
  assert(dimension == layerSubject.getDimension());

  Coordinates c;

  for (c.x = 0; c.x < dimension.width; ++c.x) {
    for (c.y = 0; c.y < dimension.height; ++c.y) {
      setAirObstruction(c, calcCellAirObs(layerSubject.getSubjectList(c)));
    }
  }
}
