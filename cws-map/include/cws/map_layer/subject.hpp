#pragma once

#include "cws/layer/subject.hpp"
#include "cws/map_layer/base.hpp"

/*
 * Extended logic for subject layer
 */
class MapLayerSubject : public MapLayerBase<LayerSubject> {
public:
  MapLayerSubject(Dimension dimension) : MapLayerBase<LayerSubject>(dimension) {}

  std::list<std::pair<Coordinates, const Subject::LightSourceAlt *>>
  getActiveLightSources() const;

  const std::list<std::unique_ptr<Subject::Plain>> &
  getSubjectList(Coordinates c) const {
    return getCell(c).getElement().getSubjectList();
  }
};
