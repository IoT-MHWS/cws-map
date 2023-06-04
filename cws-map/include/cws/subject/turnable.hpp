#pragma once

#include "cws/subject/extension/turnable.hpp"
#include "cws/subject/plain.hpp"

namespace Subject {

// like door, window, etc.
class Turnable : public Plain, public ExtTurnable {
  TurnableStatus turnableStatus_;
  Obstruction offLightObstruction_;
  Obstruction offWirelessObstruction_;
  Obstruction offAirObstruction_;

public:
  Turnable(Plain && plain, TurnableStatus status, Obstruction offLightObstruction,
           Obstruction offWirelessObstruction, Obstruction offAirObstruction)
      : Plain(std::move(plain)), turnableStatus_(status),
        offLightObstruction_(offLightObstruction),
        offWirelessObstruction_(offWirelessObstruction),
        offAirObstruction_(offAirObstruction) {

    setType(Type::TURNABLE);
  }

  Turnable * clone() const override { return new Turnable(*this); }

  TurnableStatus getStatus() const final override { return turnableStatus_; }
  void setStatus(TurnableStatus status) final override { turnableStatus_ = status; }

  Obstruction getOffLightObstruction() const { return offLightObstruction_; }
  Obstruction getOffWirelessObstruction() const { return offWirelessObstruction_; }
  Obstruction getOffAirObstruction() const { return offAirObstruction_; }

  Obstruction getCurLightObstruction() const override;
  Obstruction getCurWirelessObstruction() const override;
  Obstruction getCurAirObstruction() const override;
};

}// namespace Subject
