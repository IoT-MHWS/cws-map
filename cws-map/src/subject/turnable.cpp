#include "cws/subject/turnable.hpp"

namespace Subject {

Obstruction Turnable::getCurLightObstruction() const {
  if (getStatus() == TurnableStatus::ON) {
    return getDefLightObstruction();
  } else {
    return offLightObstruction_;
  }
}

Obstruction Turnable::getCurWirelessObstruction() const {
  if (getStatus() == TurnableStatus::ON) {
    return getDefWirelessObstruction();
  } else {
    return offWirelessObstruction_;
  }
}

Obstruction Turnable::getCurAirObstruction() const {
  if (getStatus() == TurnableStatus::ON) {
    return getDefAirObstruction();
  } else {
    return offAirObstruction_;
  }
}

}// namespace Subject
