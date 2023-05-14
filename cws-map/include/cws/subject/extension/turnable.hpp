#pragma once

#include "cws/subject/extension/base.hpp"
#include "cws/util.hpp"

namespace Subject {

enum class TurnableStatus {
  OFF = 0,
  ON = 1,
};

class Turnable : virtual public Subject::Extensiable {
  TurnableStatus status_;

public:
  Turnable(TurnableStatus status) : status_(TurnableStatus::OFF) {}

  bool isTurnable() const final override { return true; }

public:
  TurnableStatus getStatus() const { return status_; }
  void setStatus(TurnableStatus status) { status_ = status; }
};

} // namespace Subject
