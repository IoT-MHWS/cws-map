#pragma once

#include "cws/subject/extension/temp_source.hpp"
#include "cws/subject/extension/turnable.hpp"
#include "cws/subject/plain.hpp"

namespace Subject {

/*
 * Always turned on temp emitted
 */
class TempEmitter : public Plain, public ExtTempSource {
  TempSourceParams tempParams_;

public:
  TempEmitter(Plain && plain, TempSourceParams tempParams)
      : Plain(std::move(plain)), tempParams_(tempParams) {

    setType(Type::TEMP_EMITTER);
  }

  TempEmitter * clone() const override { return new TempEmitter(*this); }

  void nextTemperature() final override;

  virtual TempSourceParams getDefTempParams() const final override {
    return tempParams_;
  };
  virtual TempSourceParams getCurTempParams() const override { return tempParams_; }
};

/*
 * Temp emitter that can be switched off
 */
class TurnableTempEmitter : public TempEmitter, public ExtTurnable {
  TurnableStatus turnableStatus_;
  TempSourceParams offTempParams_;

public:
  TurnableTempEmitter(TempEmitter && emitter, TurnableStatus status,
                      TempSourceParams offTempParams)
      : TempEmitter(std::move(emitter)), turnableStatus_(status),
        offTempParams_(offTempParams) {

    setType(Type::TURNABLE_TEMP_EMITTER);
  }

  TurnableTempEmitter * clone() const override {
    return new TurnableTempEmitter(*this);
  }

  TurnableStatus getStatus() const final override { return turnableStatus_; }
  void setStatus(TurnableStatus status) final override { turnableStatus_ = status; }

  TempSourceParams getOffTempParams() const { return offTempParams_; }

  TempSourceParams getCurTempParams() const final override;
};

}// namespace Subject
