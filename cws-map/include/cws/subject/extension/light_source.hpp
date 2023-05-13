#pragma once

#include "cws/general.hpp"
#include "cws/subject/extension/base.hpp"

namespace Subject {

struct LightSourceParams {
  Illumination emittedRawIllumination;
};

class LightSource : virtual public Subject::Extensiable {
  LightSourceParams params_;

public:
  LightSource(LightSourceParams params) : params_(params) {}

  bool isLightSource() const final override { return true; }

public:
  LightSourceParams getParams() const { return params_; }

  void setParams(LightSourceParams params) { params_ = params; }
};

}// namespace Subject
