#pragma once

#include "cws/subject/extension/base.hpp"
namespace Subject {

enum class TurnableStatus {
  UNSPECIFIED = 0,
  ON = 1,
  OFF = 2,
};

class Turnable : virtual public Subject::Extensiable {
  TurnableStatus status_;

public:
  Turnable() : status_(TurnableStatus::OFF) {}

  Turnable(TurnableStatus status) { status_ = status; }

  bool isTurnable() const final override { return true; }

public:
  TurnableStatus getStatus() const { return status_; }

  void setStatus(TurnableStatus status) { status_ = status; }
};

}// namespace Subject
