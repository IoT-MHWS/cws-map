#include "cws/map_layer/air.hpp"

/*
 * subTr = subSurface * deltaT * airCoef
 * sum(airTr) + sum(subTr) = 0
 * deltaT = subTr - airTr
 */
void MapLayerAir::nextConvection(MapLayerSubject & subjectLayer) {
  Dimension dim = getDimension();

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      nextConvection(subjectLayer, c);
    }
  }
}

void MapLayerAir::nextConvection(MapLayerSubject & subjectLayer, Coordinates c) {
  auto & airContainer = accessCell(c).accessElement().accessAirContainer();
  // if no air then there is no convection
  if (!airContainer.hasAir()) {
    return;
  }

  airContainer.getHeatTransferCoef();

  auto & subList = subjectLayer.accessCell(c).accessElement().accessSubjectList();

  auto airTemp = airContainer.getTemperature();
  auto airCoef = airContainer.getHeatTransferCoef();
  double totalHeatTransfer = 0;

  for (auto & sub : subList) {
    auto deltaTemp = sub->getTemperature().get() - airTemp.get();
    auto subSurf = sub->getSurfaceArea();
    double heatTransfer = subSurf * deltaTemp * airCoef;
    sub->updateTemperature(heatTransfer);
    totalHeatTransfer -= heatTransfer;
  }

  airContainer.updateTemperature(totalHeatTransfer);
}
