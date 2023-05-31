#pragma once

#include "cws/map_layer/illumination.hpp"
#include "cws/map_layer/obstruction.hpp"
#include "cws/subject/extension/camera.hpp"
#include "cws/subject/plain.hpp"

namespace Subject {

class BaseCamera : public Plain {
public:
  using PCoordPlain = std::pair<Coordinates, Plain &>;

private:
  double power_;
  double powerThreshold_;

public:
  BaseCamera(Plain && plain, double power, double powerThreshold)
      : Plain(std::move(plain)), power_(power), powerThreshold_(powerThreshold) {}

  BaseCamera * clone() const override = 0;
};

class InfraredCamera : public BaseCamera, public ExtInfraredCamera {

public:
  InfraredCamera(Plain && plain, double power, double powerThreshold)
      : BaseCamera(std::move(plain), power, powerThreshold) {}

  InfraredCamera * clone() const override { return new InfraredCamera(*this); }

  // maybe store ptrs to layers inside?
  std::list<PCoordPlain>
  getVisibleSubjects(const MapLayerSubject & layerSubject,
                     const MapLayerObstruction & layerObstruction) override;
};

class LightCamera : public BaseCamera, public ExtLightCamera {
private:
  double lightThreshold_;

public:
  LightCamera(Plain && plain, double power, double powerThreshold,
              double lightThreshold)
      : BaseCamera(std::move(plain), power, powerThreshold),
        lightThreshold_(lightThreshold) {}

  LightCamera * clone() const override { return new LightCamera(*this); }

  // maybe store ptrs to layers inside?
  std::list<PCoordPlain>
  getVisibleSubjects(const MapLayerSubject & layerSubject,
                     const MapLayerObstruction & layerObstruction,
                     const MapLayerIllumination & layerIllumination) override;
};

}// namespace Subject
