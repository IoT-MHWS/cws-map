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

// To
cws::SimulationStatus toSimulationStatus(const SimulationStatus status);
cws::SimulationType toSimulationType(const SimulationType type);
void toSimulationState(cws::SimulationState & out, const struct SimulationState & in);

void toCell(cws::Cell & out, const Cell & cell);

void toCoordinates(cws::Coordinates & out, const Coordinates & coord);
void toTemperature(cws::Temperature & out, const Temperature & temp);
void toPercentage(cws::Percentage & out, const Percentage & percentage);

void toLayer(cws::Layer & out, const Layer & layer);
void toLayerSubject(cws::LayerSubject & out, const LayerSubject & layerSubject);
void toLayerTemperature(cws::LayerTemperature & out,
                        const LayerTemperature & layerTemperature);

cws::SubjectType toSubjectType(SubjectType in);
void toSubjectId(cws::SubjectId & out, const SubjectId & id);
void toSubjectDerived(cws::SubjectDerived & out, const Subject * subject);
void toSubject(cws::Subject & out, const Subject * subject);

void toSubjectPlain(cws::SubjectPlain & out, const SubjectPlain * plain);

cws::InteractionStateType toInteractionStateType(const InteractionStateType type);
void toSubjectInteractive(cws::SubjectInteractive & out,
                          const SubjectInteractive * subject);

void toSubjectSensorDerived(cws::SubjectSensorDerived & out,
                            const SubjectSensor * subject);
void toSubjectSensor(cws::SubjectSensor & out, const SubjectSensor * subject);
void toSensorTemperature(cws::SensorTemperature & out,
                         const SensorTemperature * sensor);

// From
SimulationStatus fromSimulationStatus(const cws::SimulationStatus status);
SimulationType fromSimulationType(const cws::SimulationType type);
SimulationStateIn fromSimulationState(const cws::SimulationState & in);

Coordinates fromCoordinates(const cws::Coordinates & coord);
Temperature fromTemperature(const cws::Temperature & in);
Percentage fromPercentage(const cws::Percentage & in);

SubjectType fromSubjectType(cws::SubjectType in);
SubjectType fromSubjectType(const cws::SubjectDerived & in);
SubjectId fromSubjectId(const cws::SubjectId & id, cws::SubjectType type);
SubjectParameters fromSubjectParameters(const cws::Subject & in);
std::unique_ptr<Subject> fromSubjectDerived(const cws::SubjectDerived & in);
std::unique_ptr<Subject> fromSubject(const cws::Subject & in, SubjectType type);

std::unique_ptr<SubjectPlain> fromSubjectPlain(const cws::SubjectPlain & in);

InteractionStateType fromInteractionStateType(const cws::InteractionStateType type);
std::unique_ptr<SubjectInteractive>
fromSubjectInteractive(const cws::SubjectInteractive & in);

SensorType fromSensorType(const cws::SubjectSensorDerived & in);
std::unique_ptr<SubjectSensor>
fromSubjectSensorDerived(const cws::SubjectSensorDerived & in);
std::unique_ptr<SubjectSensor> fromSubjectSensor(const cws::SubjectSensor & in,
                                                 SensorType type);
std::unique_ptr<SensorTemperature>
fromSensorTemperature(const cws::SensorTemperature & in);

SubjectQueryType fromSubjectQueryType(cws::SubjectQueryType type);
