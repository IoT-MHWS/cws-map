#pragma once

#include "cws/air/type.hpp"
#include "cws/physical.hpp"
#include <memory>

namespace Air {

class Plain : public Physical {
  Type type_;
  double heatTransferCoef_;

public:
  Plain(Physical && params, double heatTransferCoef)
      : Plain(std::move(params), Type::PLAIN, heatTransferCoef) {}

  Plain(Physical && params, Type type, double heatTransferCoef)
      : Physical(std::move(params)), type_(type), heatTransferCoef_(heatTransferCoef) {}

  virtual Plain * clone() const { return new Plain(*this); }

  Plain * cloneWithWeight(double weight) const;

  double getHeatTransferCoef() const { return heatTransferCoef_; }

  Type getType() const { return type_; }

  virtual Plain operator+(const Plain & rsv);
};

};// namespace Air
