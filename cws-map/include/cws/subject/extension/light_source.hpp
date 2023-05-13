#pragma once

#include "cws/subject/extension/base.hpp"

namespace Subject {

struct LightSourceParams {
  double emittedLight_;
};

class LightSource : virtual public Subject::Extensiable {
  LightSourceParams params_;

public:
  LightSource(LightSourceParams params) : params_(params) {}

  LightSourceParams getParams() const { return params_; }

  void setParams(LightSourceParams params) { params_ = params; }

  bool isLightSource() const final override { return true; }
};

}// namespace Subject
