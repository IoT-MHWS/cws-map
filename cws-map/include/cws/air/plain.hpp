#pragma once

#include "cws/air/type.hpp"
#include "cws/physical.hpp"
#include <memory>

namespace Air {

struct Id {
  Type type;
  int idx;

  friend bool operator==(const Id & lhs, const Id & rhs);
  friend std::ostream & operator<<(std::ostream &, const Id & rhs);
};

class Plain : public Physical {
  Id id_;
  double heatTransferCoef_;

public:
  Plain(Physical && base, Id id, double heatTransferCoef)
      : Physical(std::move(base)), id_(id), heatTransferCoef_(heatTransferCoef) {}

  Plain(Physical && base, int idx, double heatTransferCoef)
      : Plain(std::move(base), Id{.type = Type::PLAIN, .idx = idx}, heatTransferCoef) {}

  virtual Plain * clone() const { return new Plain(*this); }

  Plain * cloneWithWeight(double weight) const;

  double getHeatTransferCoef() const { return heatTransferCoef_; }

  Id getId() const { return id_; }

  virtual Plain operator+(const Plain & rsv);
};

};// namespace Air
