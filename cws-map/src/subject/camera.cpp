#include "cws/subject/camera.hpp"
#include "cws/common.hpp"
#include <cassert>
#include <deque>
#include <queue>

using namespace Subject;
using PCoordPlain = BaseCamera::PCoordPlain;

using IsCellFitFn = bool (*)(double cellPower, const BaseCamera & baseCamera);

using GetResidualPowerFn = double (*)(Coordinates, double cellPower,
                                      const BaseCamera & baseCamera);

using FilterCellFn = bool (*)(Coordinates, const BaseCamera & baseCamera);

std::list<PCoordPlain> getVisibleSubjectsGen(const BaseCamera & camera,
                                             IsCellFitFn isCellFit,
                                             GetResidualPowerFn getResidualPower,
                                             FilterCellFn filterCell) {
  std::list<PCoordPlain> result;

  auto subjectLayer = camera.getLayerSubject();
  auto cameraCoords = camera.getCameraCoords();
  auto cameraPower = camera.getPower();

  Dimension dim = subjectLayer->getDimension();

  assert(subjectLayer != nullptr);

  // -2 - not touched, -1 - in queue, >=0 - calculated
  std::vector<std::vector<double>> infoV(dim.width,
                                         std::vector<double>(dim.height, -2));

  std::queue<Coordinates> queue;

  auto addNeighboursQV = [&](Coordinates c) -> void {
    for (const auto & neigh : getNeighbours(dim, c)) {
      auto & infoVN = infoV[neigh.x][neigh.y];
      if (infoVN == -2) {
        queue.push(neigh);
        infoVN = -1;
      }
    }
  };

  auto addSubjectsRes = [&](Coordinates c) -> void {
    for (const auto & sub : subjectLayer->getSubjectList(c)) {
      result.emplace_back(c, *sub.get());
    }
  };

  // process camera cell
  {
    double cellPower = getResidualPower(cameraCoords, cameraPower, camera);
    infoV[cameraCoords.x][cameraCoords.y] = cellPower;
    if (isCellFit(cellPower, camera)) {
      addNeighboursQV(cameraCoords);
      if (filterCell(cameraCoords, camera)) {
        addSubjectsRes(cameraCoords);
      }
    }
  }

  while (!queue.empty()) {
    Coordinates u = queue.front();
    queue.pop();

    auto uNeighs = getNeighbours(dim, u);
    auto v_ucam = getVector(u, cameraCoords);

    Coordinates best_n;
    auto best_sm = -1;
    auto best_d_un = 0;

    for (const auto & n : uNeighs) {
      auto v_un = getVector(u, n);
      auto sm_n = getScalarMultiplication(v_ucam, v_un);
      auto d_un = getDistanceSquare(v_un);

      if (sm_n > best_sm || (sm_n == best_sm && d_un < best_d_un)) {
        best_n = n;
        best_sm = sm_n;
        best_d_un = d_un;
      }
    }

    double uResPower = getResidualPower(u, infoV[best_n.x][best_n.y], camera);
    infoV[u.x][u.y] = uResPower;
    if (isCellFit(uResPower, camera)) {
      addNeighboursQV(u);
      if (filterCell(u, camera)) {
        addSubjectsRes(u);
      }
    }
  }

  return result;
}

std::list<PCoordPlain> InfraredCamera::getVisibleSubjects() const {

  auto isCellFit = [](double cellPower, const BaseCamera & baseCamera) -> bool {
    return cellPower >= baseCamera.getPowerThreshold();
  };

  auto getResidualPower = [](Coordinates c, double cellPower,
                             const BaseCamera & baseCamera) -> double {
    auto camera = static_cast<const InfraredCamera &>(baseCamera);
    auto cellObstruction = camera.getLayerObstruction()->getLightObstruction(c);
    return cellPower * std::max(1 - cellObstruction.get(), 0.);
  };

  auto cellFilter = [](Coordinates, const BaseCamera & camera) -> bool { return true; };

  return getVisibleSubjectsGen(*this, isCellFit, getResidualPower, cellFilter);
}

std::list<PCoordPlain> LightCamera::getVisibleSubjects() const {

  auto isCellFit = [](double cellPower, const BaseCamera & baseCamera) -> bool {
    return cellPower >= baseCamera.getPowerThreshold();
  };

  auto getResidualPower = [](Coordinates c, double cellPower,
                             const BaseCamera & baseCamera) -> double {
    auto camera = static_cast<const LightCamera &>(baseCamera);
    auto cellObstruction = camera.getLayerObstruction()->getLightObstruction(c);
    return cellPower * std::max(1 - cellObstruction.get(), 0.);
  };

  auto cellFilter = [](Coordinates c, const BaseCamera & baseCamera) -> bool {
    auto camera = static_cast<const LightCamera &>(baseCamera);
    return camera.getLayerIllumination()->getIllumination(c).get() >=
           camera.getLightThreshold();
  };

  return getVisibleSubjectsGen(*this, isCellFit, getResidualPower, cellFilter);
}

// namespace Subject
