#pragma once

#include "cws/air/plain.hpp"
#include "cws/physical.hpp"
#include <list>
#include <memory>

namespace Air {

/*
 * Air is moved in container to maintain certain constraints:
 * 1) temperature of everything should be equal
 */
class Container {
public:
  using PlainUPTR = std::unique_ptr<Plain>;

private:
  std::list<PlainUPTR> airList;

public:
  Container() = default;

  Container(Container && obj) noexcept { std::swap(this->airList, obj.airList); }

  Container(const Container & obj) noexcept {
    airList.clear();
    for (const auto & e : obj.airList) {
      airList.push_back(PlainUPTR(e->clone()));
    }
  }

  const std::list<PlainUPTR> & getList() const;

  bool empty() const;
  void add(PlainUPTR && plain);
  void add(std::list<PlainUPTR> && list);
  std::list<PlainUPTR>::const_iterator erase(std::list<PlainUPTR>::const_iterator & it);

  // nullable
  Plain * findOrNull(const Plain & plain) const;

  double getHeatTransferCoef() const;
  Temperature getTemperature() const;
  double getWeight() const;
  void updateTemperature(double heatAirTransfer);

private:
  void normalizeTemperature();
  void getHeatTransferAndTotalWeight(double * totalWeight, double * transferCoef) const;
  void addNotNormalize(PlainUPTR && plain);
};

};// namespace Air
