#pragma once

#include "cws/general.hpp"
#include "cws/subject/extension/base.hpp"

namespace Subject {

struct LightSourceParams {
  Illumination rawIllumination;
};

class LightSourceAlt : virtual public Subject::Extensiable {
  LightSourceParams defParams_;

public:
  LightSourceAlt(LightSourceParams params) : defParams_(params) {}

  bool isLightSource() const final override { return true; }

public:
  LightSourceParams getDefLightParams() const { return defParams_; }
  virtual LightSourceParams getCurLightParams() const {
    return defParams_;
  }
};

} // namespace Subject
