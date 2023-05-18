#include "cws/map_layer/air.hpp"

void MapLayerAir::nextConvection(MapLayerSubject & subjectLayer) {
  Dimension dim = getDimension();

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      auto & airContainer = accessCell(c).accessElement().accessAirContainer();
      // if no air then there is no convection
      if (!airContainer.hasAir()) {
        continue;
      }

      airContainer.getHeatTransferCoef();

      auto & subList = subjectLayer.accessCell(c).accessElement().accessSubjectList();

      auto airCoef = airContainer.getHeatTransferCoef();
      double totalHeatTransfer = 0;

      for (auto & sub : subList) {
        auto subTemp = sub->getTemperature();
        auto subSurf = sub->getSurfaceArea();
        double heatTransfer = subSurf * subTemp.get() * airCoef;
        sub->updateTemperature(heatTransfer);
        totalHeatTransfer -= heatTransfer;
      }

      airContainer.updateTemperature(totalHeatTransfer);
    }
  }
}
