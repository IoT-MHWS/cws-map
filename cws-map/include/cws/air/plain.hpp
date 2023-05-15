#pragma once

#include "cws/physical.hpp"

namespace Air {

class Plain : public Physical {
  double heatTransferCoef_;

public:
  Plain(Physical &&params, double heatTransferCoef)
      : Physical(std::move(params)), heatTransferCoef_(heatTransferCoef) {}

  virtual Plain *clone() const { return new Plain(*this); }

  double getHeatTransferCoef() const { return heatTransferCoef_; }
};

}; // namespace Air
