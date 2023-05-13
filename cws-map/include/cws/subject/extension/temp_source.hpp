#pragma once

#include "cws/subject/extension/base.hpp"
namespace Subject {

struct TempSourceParams {
  double emittedPower_;
  double amperage_;

  double getRadiatedHeat() const { return emittedPower_ * amperage_; }
};

class TempSource : virtual public Subject::Extensiable {
  TempSourceParams params_;

public:
  TempSource(TempSourceParams params) : params_(params) {}

  bool isTempSource() const final override { return true; }

public:
  TempSourceParams getParams() const { return params_; }

  void setParams(TempSourceParams params) { params_ = params; }
};

}// namespace Subject
