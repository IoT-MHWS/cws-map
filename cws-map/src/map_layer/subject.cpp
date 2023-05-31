#include "cws/map_layer/subject.hpp"
#include "cws/map_layer/network.hpp"
#include "cws/subject/camera.hpp"
#include "cws/subject/extension/temp_source.hpp"

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
      auto & cellSubs = this->accessCell(c).accessElement().accessSubjectList();
      for (auto & sub : cellSubs) {
        if (auto tempSub = dynamic_cast<ExtTempSource *>(sub.get())) {
          tempSub->nextTemperature();
        }
      }
    }
  }
}

void MapLayerSubject::setupSubject(Subject::Plain & subject,
                                   const MapLayerObstruction & obstructionLayer,
                                   const MapLayerIllumination & illuminationLayer) {
  using namespace Subject;

  switch (subject.getSubjectId().type) {
  case Type::INFRARED_CAMERA:
    return static_cast<InfraredCamera &>(subject).setup(*this, obstructionLayer);
  case Type::LIGHT_CAMERA:
    return static_cast<LightCamera &>(subject).setup(*this, obstructionLayer,
                                                     illuminationLayer);
  default:
    break;
  }
}

// Setup complex devices to be ready to process user requests
void MapLayerSubject::setupSubjects(const MapLayerObstruction & obstructionLayer,
                                    const MapLayerIllumination & illuminationLayer) {
  Dimension dim = getDimension();

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      auto & cellSubs = this->accessCell(c).accessElement().accessSubjectList();
      for (auto & sub : cellSubs) {
        setupSubject(*sub, obstructionLayer, illuminationLayer);
      }
    }
  }
}

void MapLayerSubject::receivePackets(const MapLayerNetwork & networkLayer) {
  Dimension dim = getDimension();

  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      auto & cellSubs = this->accessCell(c).accessElement().accessSubjectList();
      for (auto & sub : cellSubs) {
        if (auto netSub = dynamic_cast<Subject::ExtReceiver *>(sub.get())) {
          netSub->placeNetworkPackets(networkLayer.getReceivablePackages(c),
                                      networkLayer.getNetworkType());
        }
      }
    }
  }
}
