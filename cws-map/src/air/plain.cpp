#include "cws/air/plain.hpp"
#include <cassert>

using namespace Air;

template<typename T>
T proportion(T lhs, double lhsRatio, T rhs, double rhsRatio) {
  return (lhs * lhsRatio + rhs * rhsRatio) / (lhsRatio + rhsRatio);
}

Plain Plain::operator+(const Plain & rhs) {
  assert(this->getType() == rhs.getType());

  auto lhsW = getWeight();
  auto lhsHC = getHeatCapacity();
  auto lhsT = getTemperature().get();
  auto lhsA = getDefLightObstruction().get();
  auto lhsHTC = getHeatTransferCoef();

  auto rhsW = rhs.getWeight();
  auto rhsHC = rhs.getHeatCapacity();
  auto rhsT = rhs.getTemperature().get();
  auto rhsA = rhs.getDefLightObstruction().get();
  auto rhsHTC = rhs.getHeatTransferCoef();

  auto resW = lhsW + rhsW;
  auto resHC = proportion(lhsHC, lhsW, rhsHC, rhsW);
  Temperature resT = {.value = proportion(lhsT, lhsW * lhsHC, rhsT, rhsW * rhsHC)};
  Obstruction resA = {.value = proportion(lhsA, lhsW, rhsA, rhsW)};
  double resHTC = proportion(lhsHTC, lhsW, rhsHTC, rhsW);

  return Plain(Physical(resW, resHC, resT, resA), getType(), resHTC);
}
