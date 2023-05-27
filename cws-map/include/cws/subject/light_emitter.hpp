#pragma once

#include "cws/subject/extension/light_source.hpp"
#include "cws/subject/extension/temp_source.hpp"
#include "cws/subject/extension/turnable.hpp"
#include "cws/subject/plain.hpp"
#include "cws/subject/temp_emitter.hpp"

namespace Subject {

using namespace Subject;

/*
 * Always turned on light emitted
 */
class LightEmitter : public TempEmitter, public LightSourceAlt {
public:
  LightEmitter(Plain && plain, LightSourceParams && light, TempSourceParams && temp)
      : TempEmitter(std::move(plain), std::move(temp)),
        LightSourceAlt(std::move(light)) {

    setType(Type::LIGHT_EMITTER);
  }

  LightEmitter * clone() const override { return new LightEmitter(*this); }
};

/*
 * Light emitter that can be switched off
 */
class TurnableLightEmitter : public LightEmitter, public Turnable {
  LightSourceParams offLightParams_;
  TempSourceParams offTempParams_;

public:
  TurnableLightEmitter(LightEmitter && lightEmitter, TurnableStatus status,
                       LightSourceParams offLightParams, TempSourceParams offTempParams)
      : LightEmitter(std::move(lightEmitter)), Turnable(status),
        offLightParams_(offLightParams), offTempParams_(offTempParams) {

    setType(Type::TURNABLE_LIGHT_EMITTER);
  }

  TurnableLightEmitter * clone() const override {
    return new TurnableLightEmitter(*this);
  }

  LightSourceParams getCurLightParams() const override;
  TempSourceParams getCurTempParams() const override;
};

}// namespace Subject
