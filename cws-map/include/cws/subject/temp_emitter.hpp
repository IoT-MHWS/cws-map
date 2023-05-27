#pragma once

#include "cws/subject/extension/temp_source.hpp"
#include "cws/subject/extension/turnable.hpp"
#include "cws/subject/plain.hpp"

namespace Subject {

using namespace Subject;

/*
 * Always turned on temp emitted
 */
class TempEmitter : public Plain, public TempSourceAlt {
public:
  TempEmitter(Plain && plain, TempSourceParams && temp)
      : Plain(std::move(plain)), TempSourceAlt(std::move(temp)) {

    setType(Type::TEMP_EMITTER);
  }

  void nextTemperature() override;

  TempEmitter * clone() const override { return new TempEmitter(*this); }
};

/*
 * Temp emitter that can be switched off
 */
class TurnableTempEmitter : public TempEmitter, public Turnable {
  TempSourceParams offTempParams_;

public:
  TurnableTempEmitter(TempEmitter && emitter, TurnableStatus status,
                      TempSourceParams offTempParams)
      : TempEmitter(std::move(emitter)), Turnable(status),
        offTempParams_(offTempParams) {

    setType(Type::TURNABLE_TEMP_EMITTER);
  }

  TurnableTempEmitter * clone() const override {
    return new TurnableTempEmitter(*this);
  }

  TempSourceParams getCurTempParams() const override;
};

}// namespace Subject
