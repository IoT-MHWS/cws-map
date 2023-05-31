#pragma once

#include "cws/util.hpp"

namespace Subject {

enum class TurnableStatus {
  OFF = 0,
  ON = 1,
};

class ExtTurnable {
public:
  virtual TurnableStatus getStatus() const = 0;
  virtual void setStatus(TurnableStatus status) = 0;
};

}// namespace Subject
