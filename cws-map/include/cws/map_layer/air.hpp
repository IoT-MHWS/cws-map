#pragma once

#include "cws/layer/air.hpp"
#include "cws/map_layer/base.hpp"
#include "cws/map_layer/obstruction.hpp"
#include "cws/map_layer/subject.hpp"

/*
 * Extended logic for subject layer
 */
class MapLayerAir : public MapLayerBase<LayerAir> {
public:
  MapLayerAir(Dimension dimension) : MapLayerBase<LayerAir>(dimension) {}

  const Air::Container & getAirContainer(Coordinates c) const {
    return getCell(c).getElement().getAirContainer();
  }

  Air::Container & accessAirContainer(Coordinates c) {
    return accessCell(c).accessElement().accessAirContainer();
  }

  void nextConvection(MapLayerSubject & subjectLayer);

  void nextCirculation(const MapLayerAir & curLayerAir,
                       const MapLayerObstruction & obstructionLayer);
  void nextCirculationMassTemp(const MapLayerAir & curLayerAir,
                               const MapLayerObstruction & obstructionLayer);
  void nextCirculationTemp(const MapLayerAir & curLayerAir,
                           const MapLayerObstruction & obstructionLayer);

private:
  void nextConvection(MapLayerSubject & subjectLayer, Coordinates c);

  void nextCirculationCellMassTemp(const MapLayerAir & curLayerAir,
                                   const MapLayerObstruction & obstructionLayer,
                                   Coordinates c);
  void nextCirculationCellTemp(const MapLayerAir & curLayerAir,
                               const MapLayerObstruction & obstructionLayer,
                               Coordinates c);
};
