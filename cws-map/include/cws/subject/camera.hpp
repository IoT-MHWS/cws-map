#pragma once

#include "cws/map_layer/illumination.hpp"
#include "cws/map_layer/obstruction.hpp"
#include "cws/subject/extension/camera.hpp"
#include "cws/subject/plain.hpp"

namespace Subject {

class BaseCamera : public Plain, public ExtCamera {
public:
  using PCoordPlain = std::pair<Coordinates, Plain &>;

private:
  Coordinates cameraCoord_;
  const MapLayerSubject * layerSubject_;
  double power_;
  double powerThreshold_;

public:
  BaseCamera(Plain && plain, double power, double powerThreshold)
      : Plain(std::move(plain)), power_(power), powerThreshold_(powerThreshold) {}

  BaseCamera * clone() const override = 0;

  Coordinates getCameraCoords() const { return cameraCoord_; }
  const MapLayerSubject * getLayerSubject() const { return layerSubject_; }
  double getPower() const { return power_; }
  double getPowerThreshold() const { return powerThreshold_; }

  void setup(Coordinates cameraCoord, const MapLayerSubject & layerSubject) {
    cameraCoord_ = cameraCoord;
    layerSubject_ = &layerSubject;
  }
};

class InfraredCamera : public BaseCamera {
private:
  const MapLayerObstruction * layerObstruction_;

public:
  InfraredCamera(Plain && plain, double power, double powerThreshold)
      : BaseCamera(std::move(plain), power, powerThreshold) {

    setType(Type::INFRARED_CAMERA);
  }

  InfraredCamera * clone() const override { return new InfraredCamera(*this); }

  const MapLayerObstruction * getLayerObstruction() const { return layerObstruction_; }

  void setup(Coordinates cameraCoord, const MapLayerSubject & layerSubject,
             const MapLayerObstruction & layerObstruction) {
    BaseCamera::setup(cameraCoord, layerSubject);
    layerObstruction_ = &layerObstruction;
  }

  std::list<PCoordPlain> getVisibleSubjects() const override;
};

class LightCamera : public BaseCamera {
private:
  const MapLayerObstruction * layerObstruction_;
  const MapLayerIllumination * layerIllumination_;

  double lightThreshold_;

public:
  LightCamera(Plain && plain, double power, double powerThreshold,
              double lightThreshold)
      : BaseCamera(std::move(plain), power, powerThreshold),
        lightThreshold_(lightThreshold) {

    setType(Type::LIGHT_CAMERA);
  }

  LightCamera * clone() const override { return new LightCamera(*this); }

  const MapLayerObstruction * getLayerObstruction() const { return layerObstruction_; }
  const MapLayerIllumination * getLayerIllumination() const {
    return layerIllumination_;
  }
  double getLightThreshold() const { return lightThreshold_; }

  void setup(Coordinates cameraCoord, const MapLayerSubject & layerSubject,
             const MapLayerObstruction & layerObstruction,
             const MapLayerIllumination & layerIllumination) {
    BaseCamera::setup(cameraCoord, layerSubject);
    layerObstruction_ = &layerObstruction;
    layerIllumination_ = &layerIllumination;
  }

  std::list<PCoordPlain> getVisibleSubjects() const override;
};

}// namespace Subject
