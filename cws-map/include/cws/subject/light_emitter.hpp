#pragma once

#include "cws/subject/extension/light_source.hpp"
#include "cws/subject/extension/temp_source.hpp"
#include "cws/subject/extension/turnable.hpp"
#include "cws/subject/plain.hpp"

namespace Subject {

using namespace Subject;

/*
 * Always turned on light emitted
 */
class LightEmitter : public Plain, public LightSource, public TempSource {

public:
  LightEmitter(Id id, PlainParams plain, LightSourceParams light, TempSourceParams temp)
      : Plain(id, plain), LightSource(light), TempSource(temp) {}

  virtual Plain * clone() const { return new LightEmitter(*this); }
};

/*
 * Light emitter that can be switched off
 */
class TurnableLightEmitter : public LightEmitter, public Turnable {
public:
  TurnableLightEmitter(LightEmitter lightEmitter, TurnableStatus status)
      : LightEmitter(lightEmitter), Turnable(status) {}

  TurnableLightEmitter(LightEmitter lightEmitter)
      : LightEmitter(std::move(lightEmitter)) {}

  virtual Plain * clone() const { return new TurnableLightEmitter(*this); }
};

}// namespace Subject
