#pragma once

#include "cws/subject/extension/base.hpp"
namespace Subject {

struct TempSourceParams {
  double heatProduction;
};

class TempSourceAlt : virtual public Subject::Extensiable {
  TempSourceParams defParams_;

public:
  TempSourceAlt(TempSourceParams params) : defParams_(params) {}

  bool isTempSource() const final override { return true; }

public:
  TempSourceParams getDefTempParams() const { return defParams_; }
  virtual TempSourceParams getCurTempParams() const { return defParams_; }

  virtual void nextTemperature() = 0;
};

}// namespace Subject
