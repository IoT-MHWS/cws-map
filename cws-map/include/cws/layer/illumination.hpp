#pragma once

#include "cws/general.hpp"
#include "cws/layer/base.hpp"

class LayerIllumination : public Layer {
  Illumination illumination_;

public:
  LayerIllumination() {}
  LayerIllumination(Illumination i) : illumination_(i) {}

  Illumination getIllumination() const { return illumination_; }
  void setIllumination(Illumination illumination) { illumination_ = illumination; }
};

class LayerAbsorption : public Layer {
  Absorption absorption_;

public:
  Absorption getAbsorption() const { return absorption_; }
  void setAbsorption(Absorption absorption) { absorption_ = absorption; }
};
