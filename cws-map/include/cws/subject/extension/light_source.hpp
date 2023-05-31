#pragma once

#include "cws/common.hpp"

namespace Subject {

struct LightSourceParams {
  Illumination rawIllumination;
};

class ExtLightSource {
public:
  virtual LightSourceParams getDefLightParams() const = 0;
  virtual LightSourceParams getCurLightParams() const = 0;
};

}// namespace Subject
