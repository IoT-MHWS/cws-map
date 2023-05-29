#include "cws/map_layer/air.hpp"

void MapLayerAir::nextConvection(MapLayerSubject & subjectLayer) {
  Dimension dim = getDimension();
  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      nextConvection(subjectLayer, c);
    }
  }
}

/*
 * subTr = subSurface * deltaT * airCoef
 * sum(airTr) + sum(subTr) = 0
 * deltaT = subTr - airTr
 */
void MapLayerAir::nextConvection(MapLayerSubject & subjectLayer, Coordinates c) {
  auto & airContainer = accessCell(c).accessElement().accessAirContainer();
  // if no air then there is no convection
  if (airContainer.empty()) {
    return;
  }

  airContainer.getHeatTransferCoef();

  auto & subList = subjectLayer.accessCell(c).accessElement().accessSubjectList();

  auto airTemp = airContainer.getTemperature();
  auto airCoef = airContainer.getHeatTransferCoef();
  double totalHeatTransfer = 0;

  for (auto & sub : subList) {
    auto deltaTemp = airTemp.get() - sub->getTemperature().get();
    auto subSurf = sub->getSurfaceArea();
    double heatTransfer = subSurf * deltaTemp * airCoef;
    sub->updateTemperature(heatTransfer);
    totalHeatTransfer -= heatTransfer;
  }

  airContainer.updateTemperature(totalHeatTransfer);
}

/*
 * Need:
 * 1) Weight of each type of air should approach to medium
 * 2) Temperature of air should mix and then approach to medium
 */
void MapLayerAir::nextMixing() {

}
