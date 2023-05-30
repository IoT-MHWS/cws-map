#include "cws/map_layer/air.hpp"
#include "cws/common.hpp"
#include <cassert>
#include <cmath>
#include <map>

using namespace Air;
using PlainUPTR = std::unique_ptr<Air::Plain>;

static const double MASS_TEMP_ITER_COEF = 0.1;
static const double TEMP_ITER_COEF = 10;

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
  // MapLayerAir interLayerAir(*this); // road to 0 fps
  // nextCirculationTemp(interLayerAir, obstructionLayer);
  nextCirculationTemp(curLayerAir, obstructionLayer);
}

// Update state using mass and temp (upper formula)
void MapLayerAir::nextCirculationMassTemp(
    const MapLayerAir & curLayerAir, const MapLayerObstruction & obstructionLayer) {
  Dimension dim = getDimension();
  assert(this != &curLayerAir);
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
  assert(this != &curLayerAir);
  assert(obstructionLayer.getDimension() == dim);

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      nextCirculationCellTemp(curLayerAir, obstructionLayer, c);
    }
  }
}

double cellMassTempValue(const Air::Plain * air) {
  if (air == nullptr) {
    return 0;
  }
  return air->getTemperature().get() * air->getWeight();
}

// works only for for < 9 neighbours
double getNeighDistance(Coordinates a, Coordinates b) {
  if (a.x == b.x || a.y == b.y)
    return 1;
  else
    return pow(2, 0.5);
}

PlainUPTR cloneWithValueMassTemp(const Plain & source, double value) {
  return std::unique_ptr<Plain>(
      source.cloneWithWeight(value / source.getTemperature().get()));
}

std::map<Coordinates, std::list<PlainUPTR>>
getMapToAddAir(const std::list<Coordinates> & neighList, const Coordinates & c) {
  std::map<Coordinates, std::list<PlainUPTR>> airToAdd;
  airToAdd.emplace(c, std::list<PlainUPTR>());
  for (const auto & neigh : neighList) {
    airToAdd.emplace(neigh, std::list<PlainUPTR>());
  }
  return airToAdd;
}

void moveToLayerFromAirMap(MapLayerAir & layer,
                           std::map<Coordinates, std::list<PlainUPTR>> && airMap) {
  for (auto & [key, value] : airMap) {
    auto & container = layer.accessAirContainer(key);
    container.add(std::move(value));
  }
}

void MapLayerAir::nextCirculationCellMassTemp(
    const MapLayerAir & curLayerAir, const MapLayerObstruction & obstructionLayer,
    Coordinates c) {

  const auto neighList = getNeighbours(this->getDimension(), c);
  auto airMap = getMapToAddAir(neighList, c);

  for (const auto & curAir : curLayerAir.getAirContainer(c).getList()) {
    double curAirValue = cellMassTempValue(curAir.get());
    // also store nulls
    std::list<std::pair<Coordinates, Plain *>> suitableAirs;
    // Select neighbours to change
    for (const auto & neigh : neighList) {
      auto neighAir = curLayerAir.getAirContainer(neigh).findOrNull(*curAir.get());
      if (cellMassTempValue(neighAir) >= curAirValue) {
        continue;
      }
      suitableAirs.emplace_back(neigh, neighAir);
    }
    // Calculate air value change
    double neighCoef = std::pow(suitableAirs.size(), 0.5);
    double iterCoef = MASS_TEMP_ITER_COEF;
    for (const auto & [nc, nair] : suitableAirs) {
      double curTrans = std::max(1 - obstructionLayer.getAirObstruction(c).get(), 0.);
      double neiTrans = std::max(1 - obstructionLayer.getAirObstruction(nc).get(), 0.);

      double valueDiff = curAirValue - cellMassTempValue(nair);
      double normCoef = (iterCoef) / (neighCoef * getNeighDistance(c, nc));
      double normValueDiff = normCoef * curTrans * neiTrans * valueDiff;
      // create air with weight
      airMap[nc].push_back(cloneWithValueMassTemp(*curAir, normValueDiff));
      airMap[c].push_back(cloneWithValueMassTemp(*curAir, -normValueDiff));
    }
  }
  moveToLayerFromAirMap(*this, std::move(airMap));
}

// logic almost the same as with convection in-cell
void MapLayerAir::nextCirculationCellTemp(const MapLayerAir & curLayerAir,
                                          const MapLayerObstruction & obstructionLayer,
                                          Coordinates c) {
  auto & curAirCon = curLayerAir.getAirContainer(c);

  if (curAirCon.empty()) {
    return;
  }

  auto curAirTemp = curAirCon.getTemperature();
  auto curAirCoef = curAirCon.getHeatTransferCoef();

  double totalHeatTransfer = 0;
  const auto neighList = getNeighbours(this->getDimension(), c);

  for (const auto & nc : neighList) {
    auto & curNeighCon = curLayerAir.getAirContainer(nc);
    if (curNeighCon.empty()) {
      break;
    }
    auto curNeighTemp = curNeighCon.getTemperature();

    double deltaTemp = curAirTemp.get() - curNeighTemp.get();

    if (deltaTemp > 0) {
      double curTrans = std::max(1 - obstructionLayer.getAirObstruction(c).get(), 0.);
      double neiTrans = std::max(1 - obstructionLayer.getAirObstruction(nc).get(), 0.);
      double normCoef = TEMP_ITER_COEF / getNeighDistance(c, nc);

      double heatTransfer = normCoef * curAirCon.getWeight() * curNeighCon.getWeight() *
                            curTrans * neiTrans * deltaTemp * curAirCoef;

      this->accessAirContainer(nc).updateTemperature(heatTransfer);
      totalHeatTransfer -= heatTransfer;
    }
  }
  this->accessAirContainer(c).updateTemperature(totalHeatTransfer);
}
