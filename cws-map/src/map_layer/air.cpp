#include "cws/map_layer/air.hpp"
#include "cws/common.hpp"
#include <cassert>

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
 *
 * How:
 *
 * Dependance of air density on temperature:
 *
 * rho = (P * M / (R * T)), where: rho - density, M - molar mass, P - absolute
 * pressure, R - universal (8.314), T - temp Kelvin (273 + cels)
 *
 * m = rho * V, where: m - mass, V - volume
 *
 * F = P * S, where: F - force, S - area (on which force is applied)
 *
 * A = F * S_move, where: A - work
 *
 * Develop:
 *
 * rho = m / V
 *
 * rho = P * M / (R * T)
 *
 * m / V = P * M / (R * T)
 *
 * P = (m / V) / (M / (R * T)) = m * R * T / (M * V)
 *
 * R - const, we accept that V and M are also const.
 *
 * So air movement depends on m and T equally. And let's dive into S_move -> if cell is
 * corner cell, so work for it will be more, but we accept that A for each cell should
 * be equal. So air movement depends on whese parameters:
 *
 * ~ (m * T) / S_move
 *
 * For each cell we should make make m * T equal, but we have a problem, when we have
 * different masses and temperature for neighbour cells, but we need to make mass and
 * temperature same for all cells as a result of oncoming. Or not? Well, let's on first
 * iteration approach up formula for each cell (flow of air) and then approach
 * temperature of neighbour cells (convection) using 'smart window', that take into
 * account coef on convection (in case of walls it could be a problem).
 *
 */

void MapLayerAir::nextCirculation(const MapLayerAir & curLayerAir,
                                  const MapLayerObstruction & obstructionLayer) {
  nextCirculationMassTemp(curLayerAir, obstructionLayer);
  nextCirculationTemp(curLayerAir, obstructionLayer);
}

// Update state using mass and temp (upper formula)
void MapLayerAir::nextCirculationMassTemp(
    const MapLayerAir & curLayerAir, const MapLayerObstruction & obstructionLayer) {
  Dimension dim = getDimension();
  assert(obstructionLayer.getDimension() == dim);

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      nextCirculationCellMassTemp(curLayerAir, obstructionLayer, c);
    }
  }
}

// Update state only using temperature to approach all params to medium
void MapLayerAir::nextCirculationTemp(const MapLayerAir & curLayerAir,
                                      const MapLayerObstruction & obstructionLayer) {
  Dimension dim = getDimension();
  assert(obstructionLayer.getDimension() == dim);

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      nextCirculationCellTemp(curLayerAir, obstructionLayer, c);
    }
  }
}

void MapLayerAir::nextCirculationCellMassTemp(
    const MapLayerAir & curLayerAir, const MapLayerObstruction & obstructionLayer,
    Coordinates c) {

  const auto neiList = getNeighbours(this->getDimension(), c);
  auto & airContainer = accessAirContainer(c);
}

void MapLayerAir::nextCirculationCellTemp(const MapLayerAir & curLayerAir,
                                          const MapLayerObstruction & obstructionLayer,
                                          Coordinates c) {}
