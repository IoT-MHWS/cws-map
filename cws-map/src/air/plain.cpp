#include "cws/air/plain.hpp"
#include <cassert>
#include <iostream>

using namespace Air;

namespace Air {

template<typename T>
T proportion(T lhs, double lhsRatio, T rhs, double rhsRatio) {
  return (lhs * lhsRatio + rhs * rhsRatio) / (lhsRatio + rhsRatio);
}

bool operator==(const Air::Id & lhs, const Air::Id & rhs) {
  return (lhs.type == rhs.type) && lhs.idx == rhs.idx;
}

std::ostream & operator<<(std::ostream &out, const Id & rhs) {
  out << "{" << "t:" << (int)rhs.type << ",idx:" << rhs.idx << "}";
  return out;
}

// create same object but reset it's weight
Plain * Plain::cloneWithWeight(double weight) const {
  auto eptr = this->clone();
  eptr->setWeight(weight);
  return eptr;
}

Plain Plain::operator+(const Plain & rhs) {
  assert(this->getId() == rhs.getId());

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

  return Plain(Physical(resW, resHC, resT, resA), getId(), resHTC);
}

}// namespace Air
