#include "cws/map_layer/subject.hpp"
#include "cws/map_layer/air.hpp"
#include "cws/map_layer/network.hpp"
#include "cws/subject/camera.hpp"
#include "cws/subject/extension/temp_source.hpp"
#include "cws/subject/sensor.hpp"

using namespace Subject;

std::list<std::pair<Coordinates, const ExtLightSource *>>
MapLayerSubject::getActiveLightSources() const {

  Dimension dim = getDimension();
  std::list<std::pair<Coordinates, const ExtLightSource *>> srcs;

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      auto & cellSrcs = this->getCell(c).getElement().getActiveLightSources();
      for (const auto & src : cellSrcs) {
        srcs.emplace_back(c, src);
      }
    }
  }

  return srcs;
}

void MapLayerSubject::nextTemperature() {
  Dimension dim = getDimension();

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      auto & cellSubs = this->accessSubjectList(c);
      for (auto & sub : cellSubs) {
        if (auto tempSub = dynamic_cast<ExtTempSource *>(sub.get())) {
          tempSub->nextTemperature();
        }
      }
    }
  }
}

void MapLayerSubject::setupSubject(Subject::Plain & subject, Coordinates c,
                                   const MapLayerAir & airLayer,
                                   const MapLayerObstruction & obstructionLayer,
                                   const MapLayerIllumination & illuminationLayer) {
  using namespace Subject;

  switch (subject.getId().type) {
  case Type::INFRARED_CAMERA: {
    static_cast<InfraredCamera &>(subject).setup(c, *this, obstructionLayer);
    break;
  }
  case Type::LIGHT_CAMERA: {
    return static_cast<LightCamera &>(subject).setup(c, *this, obstructionLayer,
                                                     illuminationLayer);
    break;
  }
  case Type::AIR_TEMPERATURE_SENSOR: {
    const auto & airContainer = airLayer.getAirContainer(c);
    if (!airContainer.empty()) {
      static_cast<SensorAirTemperature &>(subject).setup(airContainer.getTemperature());
    }
    break;
  }
  case Type::ILLUMINATION_SENSOR: {
    static_cast<SensorIllumination &>(subject).setup(
        illuminationLayer.getIllumination(c));
    break;
  }
  default:
    break;
  }
}

// Setup complex devices to be ready to process user requests
void MapLayerSubject::setupSubjects(const MapLayerAir & airLayer,
                                    const MapLayerObstruction & obstructionLayer,
                                    const MapLayerIllumination & illuminationLayer) {
  Dimension dim = getDimension();

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      auto & cellSubs = this->accessSubjectList(c);
      for (auto & sub : cellSubs) {
        setupSubject(*sub, c, airLayer, obstructionLayer, illuminationLayer);
      }
    }
  }
}

void MapLayerSubject::receiveContainers(const MapLayerNetwork & networkLayer) {
  Dimension dim = getDimension();

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      auto & cellSubs = this->accessSubjectList(c);
      for (auto & sub : cellSubs) {
        if (auto netSub = dynamic_cast<Subject::ExtReceiver *>(sub.get())) {
          netSub->placeNetworkContainers(networkLayer.getReceivableContainers(c),
                                         networkLayer.getNetworkType());
        }
      }
    }
  }
}

void MapLayerSubject::clearNetworkBuffers() {
  Dimension dim = getDimension();

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      auto & cellSubs = this->accessSubjectList(c);
      for (auto & sub : cellSubs) {
        if (auto trans = dynamic_cast<Subject::ExtTransmitter *>(sub.get())) {
          trans->clearTransmitBuffer();
        }
        if (auto receiv = dynamic_cast<Subject::ExtReceiver *>(sub.get())) {
          receiv->clearReceiveBuffer();
        }
      }
    }
  }
}
