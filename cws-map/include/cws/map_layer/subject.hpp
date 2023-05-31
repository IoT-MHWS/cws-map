#pragma once

#include "cws/layer/subject.hpp"
#include "cws/map_layer/base.hpp"

class MapLayerNetwork;
class MapLayerObstruction;
class MapLayerIllumination;

/*
 * Extended logic for subject layer
 */
class MapLayerSubject : public MapLayerBase<LayerSubject> {
public:
  MapLayerSubject(Dimension dimension) : MapLayerBase<LayerSubject>(dimension) {}

  std::list<std::pair<Coordinates, const Subject::ExtLightSource *>>
  getActiveLightSources() const;

  const std::list<std::unique_ptr<Subject::Plain>> &
  getSubjectList(Coordinates c) const {
    return getCell(c).getElement().getSubjectList();
  }

  std::list<std::unique_ptr<Subject::Plain>> & accessSubjectList(Coordinates c) {
    return accessCell(c).accessElement().accessSubjectList();
  }

  void nextTemperature();
  void setupSubjects(const MapLayerObstruction & obstructionLayer,
                     const MapLayerIllumination & illuminationLayer);

  void receivePackets(const MapLayerNetwork & networkLayer);

private:
  void setupSubject(Subject::Plain & subject,
                    const MapLayerObstruction & obstructionLayer,
                    const MapLayerIllumination & illuminationLayer);
};
