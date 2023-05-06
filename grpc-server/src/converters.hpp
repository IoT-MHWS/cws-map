#pragma once

#include "cws/general.hpp"
#include "cws/simulation/general.hpp"
#include "cws/simulation/interface.hpp"
#include "cws/subject/interactive.hpp"
#include "cws/subject/plain.hpp"
#include "cws/subject/sensor.hpp"
#include "cwspb/general.pb.h"
#include "cwspb/service/map.grpc.pb.h"
#include "cwspb/service/simulation.pb.h"

struct Cell {
  const Coordinates & coordinates;
  const LayerTemperature & temp;
  const LayerSubject & subject;
};

void toSimulationState(cws::SimulationState & out, const struct SimulationState & in);
void fromSimulationState(SimulationStateIn & out, const cws::SimulationState & in);

cws::SimulationStatus toSimulationStatus(const SimulationStatus status);
SimulationStatus fromSimulationStatus(const cws::SimulationStatus status);

cws::SimulationType toSimulationType(const SimulationType type);
SimulationType fromSimulationType(const cws::SimulationType type);

void toCell(cws::Cell & out, const Cell & cell);

void toCoordinates(cws::Coordinates & out, const Coordinates & coord);
void fromCoordinates(Coordinates & out, const cws::Coordinates & coord);

void toLayer(cws::Layer & out, const Layer & layer);
void toLayerSubject(cws::LayerSubject & out, const LayerSubject & layerSubject);
void toLayerTemperature(cws::LayerTemperature & out,
                        const LayerTemperature & layerTemperature);

void toTemperature(cws::Temperature & out, const Temperature & temp);
void toPercentage(cws::Percentage & out, const Percentage & percentage);

void toSubjectDerived(cws::SubjectDerived & out, const Subject & subject);
void toSubjectId(cws::SubjectId &out, const SubjectId & id);
cws::SubjectType toSubjectType(SubjectType in);
void toSubject(cws::Subject & out, const Subject * subject);

void toSubjectInteractive(cws::SubjectInteractive & out,
                          const SubjectInteractive * subject);
cws::InteractionStateType toInteractionStateType(const InteractionStateType type);

void toSubjectPlain(cws::SubjectPlain & out, const SubjectPlain * plain);
void toSubjectSensorDerived(cws::SubjectSensorDerived & out,
                            const SubjectSensor * subject);
void toSubjectSensor(cws::SubjectSensor & out, const SubjectSensor * subject);
void toSensorTemperature(cws::SensorTemperature & out,
                         const SensorTemperature * sensor);
