#include "cws/air/container.hpp"
#include <algorithm>
#include <iostream>

namespace Air {

const std::list<Container::PlainUPTR> & Container::getList() const { return airList; }

// first check if hasAir and then do get/set stuff
bool Container::empty() const { return airList.empty(); }

void Container::add(PlainUPTR && plain) {
  addNotNormalize(std::move(plain));
  normalizeTemperature();
}

void Container::add(std::list<PlainUPTR> && list) {
  while (!list.empty()) {
    addNotNormalize(std::move(list.front()));
    list.pop_front();
  }
  normalizeTemperature();
}

std::list<Container::PlainUPTR>::const_iterator
Container::erase(std::list<PlainUPTR>::const_iterator & it) {
  return airList.erase(it);
}

// Get reference to element or return new inserted plain element in container
Plain * Container::findOrNull(const Plain & plain) const {
  const auto & it =
      std::find_if(airList.begin(), airList.end(), [&plain](const PlainUPTR & rhs) {
        return rhs->getType() == plain.getType();
      });

  if (it != airList.end()) {
    return it->get();
  } else {
    return nullptr;
  }
}

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

double Container::getWeight() const {
  double totalWeight;
  double transferCoef;
  getHeatTransferAndTotalWeight(&totalWeight, &transferCoef);
  return totalWeight;
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

void Container::normalizeTemperature() {
  double totalEnergy = 0;
  double totalWC = 0;

  for (const auto & air : airList) {
    totalWC += air->getWeight() * air->getHeatCapacity();
    totalEnergy +=
        air->getWeight() * air->getHeatCapacity() * air->getTemperature().value;
  }

  Temperature temp;
  if (std::abs(totalEnergy) > 10e-14) {
    temp = Temperature{.value = totalEnergy / totalWC};
  } else {
    temp = Temperature{.value = 0};
  }

  for (const auto & air : airList) {
    air->setTemperature(temp);
  }
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

void Container::addNotNormalize(PlainUPTR && plain) {
  bool merged = false;

  for (auto it = airList.begin(); it != airList.end(); ++it) {
    if ((*it)->getType() == plain->getType()) {
      *it->get() = *it->get() + *plain;
      merged = true;
      break;
    }
  }
  if (!merged) {
    airList.push_front(std::move(plain));
  }
}

}// namespace Air
