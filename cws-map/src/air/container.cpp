#include "cws/air/container.hpp"

namespace Air {

// first check if hasAir and then do get/set stuff
bool Container::hasAir() const { return !airList.empty(); }

double Container::getHeatTransferCoef() const {
  double totalWeight;
  double transferCoef;
  getHeatTransferAndTotalWeight(&totalWeight, &transferCoef);
  return transferCoef;
}

// temperature of all air is normalized
Temperature Container::getTemperature() const {
  return airList.front()->getTemperature();
}

// maintain temperature of all air is the same
void Container::updateTemperature(double heatAirTransfer) {
  double totalWeight;
  double transferCoef;
  getHeatTransferAndTotalWeight(&totalWeight, &transferCoef);

  double sumHeatWeight = transferCoef * totalWeight;
  double partTransfer;
  for (const auto & air : airList) {
    partTransfer = air->getHeatTransferCoef() * air->getWeight() / sumHeatWeight;
    air->updateTemperature(partTransfer * heatAirTransfer);
  }
  // then normalizeTemperature
  normalizeTemperature();
}

void Container::getHeatTransferAndTotalWeight(double * totalWeight,
                                              double * transferCoef) const {
  double weight = 0;
  double sumHeatWeight = 0;

  for (const auto & air : airList) {
    weight += air->getWeight();
    sumHeatWeight += air->getWeight() * air->getHeatTransferCoef();
  }

  *totalWeight = weight;
  *transferCoef = (sumHeatWeight / weight);
}

void Container::normalizeTemperature() {
  double totalEnergy = 0;
  double totalWC = 0;

  for (const auto & air : airList) {
    totalWC += air->getWeight() * air->getHeatCapacity();
    totalEnergy +=
        air->getWeight() * air->getHeatCapacity() * air->getTemperature().value;
  }

  Temperature temp{.value = totalEnergy / totalWC};

  for (const auto & air : airList) {
    air->setTemperature(temp);
  }
}

}// namespace Air
