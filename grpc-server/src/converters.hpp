#pragma once

// To
#include "cws/simulation/general.hpp"
#include "cws/simulation/state.hpp"
#include "cwspb/service/sv_simulation.pb.h"

cwspb::SimulationStatus toSimulationStatus(const SimulationStatus status);
cwspb::SimulationType toSimulationType(const SimulationType type);
void toSimulationState(cwspb::SimulationState & out, const struct SimulationState & in);

// void toCell(cwspb::Cell & out, const Cell & cell);

// void toCoordinates(cwspb::Coordinates & out, const Coordinates & coord);
// void toTemperature(cwspb::Temperature & out, const Temperature & temp);

// void toLayer(cwspb::Layer & out, const Layer & layer);
// void toLayerSubject(cwspb::LayerSubject & out, const LayerSubject & layerSubject);

// cwspb::SubjectType toSubjectType(Subject::Type in);
// void toSubjectId(cwspb::SubjectId & out, const Id & id);
// void toSubjectDerived(cwspb::SubjectDerived & out, const Subject * subject);
// void toSubject(cwspb::Subject & out, const Subject * subject);

// void toSubjectPlain(cwspb::SubjectPlain & out, const SubjectPlain * plain);

// cwspb::InteractionStateType toInteractionStateType(const InteractionStateType type);
// void toSubjectInteractive(cwspb::SubjectInteractive & out,
//                           const SubjectInteractive * subject);

// void toSubjectSensorDerived(cwspb::SubjectSensorDerived & out,
//                             const SubjectSensor * subject);
// void toSubjectSensor(cwspb::SubjectSensor & out, const SubjectSensor * subject);
// void toSensorTemperature(cwspb::SensorTemperature & out,
//                          const SensorTemperature * sensor);

// From
SimulationStatus fromSimulationStatus(const cwspb::SimulationStatus status);
SimulationType fromSimulationType(const cwspb::SimulationType type);
SimulationStateIn fromSimulationState(const cwspb::SimulationState & in);

// Coordinates fromCoordinates(const cwspb::Coordinates & coord);
// Temperature fromTemperature(const cwspb::Temperature & in);

// Subject::Type fromSubjectType(cwspb::SubjectType in);
// Subject::Type fromSubjectType(const cwspb::SubjectDerived & in);
// Id fromSubjectId(const cwspb::SubjectId & id, cwspb::SubjectType type);
// PlainParams fromSubjectParameters(const cwspb::Subject & in);
// std::unique_ptr<Subject> fromSubjectDerived(const cwspb::SubjectDerived & in);
// std::unique_ptr<Subject> fromSubject(const cwspb::Subject & in, SubjectType type);

// std::unique_ptr<SubjectPlain> fromSubjectPlain(const cwspb::SubjectPlain & in);

// InteractionStateType fromInteractionStateType(const cwspb::InteractionStateType type);
// std::unique_ptr<SubjectInteractive>
// fromSubjectInteractive(const cwspb::SubjectInteractive & in);

// SensorType fromSensorType(const cwspb::SubjectSensorDerived & in);
// std::unique_ptr<SubjectSensor>
// fromSubjectSensorDerived(const cwspb::SubjectSensorDerived & in);
// std::unique_ptr<SubjectSensor> fromSubjectSensor(const cwspb::SubjectSensor & in,
//                                                  SensorType type);
// std::unique_ptr<SensorTemperature>
// fromSensorTemperature(const cwspb::SensorTemperature & in);

// SubjectQueryType fromSubjectQueryType(cwspb::SubjectQueryType type);
