#pragma once

#include "cws/subject/extension/light_source.hpp"
#include "cws/subject/extension/temp_source.hpp"
#include "cws/subject/extension/turnable.hpp"
#include "cws/subject/plain.hpp"
#include "cws/subject/temp_emitter.hpp"
#include <iostream>

namespace Subject {

/*
 * Always turned on light emitted
 */
class LightEmitter : public TempEmitter, public ExtLightSource {
  LightSourceParams lightParams_;

public:
  LightEmitter(Plain && plain, TempSourceParams temp, LightSourceParams light)
      : TempEmitter(std::move(plain), temp), lightParams_(light) {

    setType(Type::LIGHT_EMITTER);
  }

  LightEmitter * clone() const override { return new LightEmitter(*this); }

  virtual LightSourceParams getDefLightParams() const final override {
    return lightParams_;
  }
  virtual LightSourceParams getCurLightParams() const override { return lightParams_; }
};

/*
 * Light emitter that can be switched off
 */
class TurnableLightEmitter : public LightEmitter, public ExtTurnable {
  TurnableStatus turnableStatus_;
  LightSourceParams offLightParams_;
  TempSourceParams offTempParams_;

public:
  TurnableLightEmitter(LightEmitter && lightEmitter, TurnableStatus status,
                       LightSourceParams offLightParams, TempSourceParams offTempParams)
      : LightEmitter(std::move(lightEmitter)), turnableStatus_(status),
        offLightParams_(offLightParams), offTempParams_(offTempParams) {

    setType(Type::TURNABLE_LIGHT_EMITTER);
  }

  TurnableLightEmitter * clone() const override {
    return new TurnableLightEmitter(*this);
  }

  TurnableStatus getStatus() const final override { return turnableStatus_; }
  void setStatus(TurnableStatus status) final override { turnableStatus_ = status; }

  TempSourceParams getOffTempParams() const { return offTempParams_; }
  LightSourceParams getOffLightParams() const { return offLightParams_; }

  LightSourceParams getCurLightParams() const override;
};

}// namespace Subject
