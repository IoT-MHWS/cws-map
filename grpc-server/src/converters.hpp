#pragma once

// To
#include "cws/common.hpp"
#include "cws/map.hpp"
#include "cws/simulation/general.hpp"
#include "cws/simulation/state.hpp"
#include "cwspb/common.pb.h"
#include "cwspb/map.pb.h"
#include "cwspb/service/sv_simulation.pb.h"

cwspb::SimulationStatus toSimulationStatus(const SimulationStatus status);
cwspb::SimulationType toSimulationType(const SimulationType type);
void toSimulationState(cwspb::SimulationState & out, const struct SimulationState & in);

void toDimension(cwspb::Dimension & out, const Dimension & in);

void toCell(cwspb::Cell & out, const Layers & layers, const Coordinates c);
void toMap(cwspb::Map & out, const Layers & layers, Dimension dim);

void toCoordinates(cwspb::Coordinates & out, const Coordinates & in);
void toTemperature(cwspb::Temperature & out, const Temperature & temp);
void toIllumination(cwspb::Illumination & out, const Illumination & in);

void toLayerAir(cwspb::layer::Air & out, const LayerAir & in);
void toLayerIllumination(cwspb::layer::Illumination & out,
                         const LayerIllumination & in);
void toLayerObstruction(cwspb::layer::Obstruction & out, const LayerObstruction & in);
void toLayerSubject(cwspb::layer::Subject & out, const LayerSubject & in);
void toLayerNetworkWireless(cwspb::layer::WirelessNetwork & out,
                            const LayerNetwork & transmitables,
                            const LayerNetwork & receivables);

void toPacket(cwspb::network::Packet & out, const Network::Packet & in);

void toWirelessContainer(cwspb::network::WirelessContainer & out,
                         const Network::WirelessContainer & in);

void toAirPlain(cwspb::air::Plain & out, const Air::Plain & in);
void toAirId(cwspb::air::Id & out, const Air::Id & in);
cwspb::air::Type toAirType(Air::Type in);

void toSubjectAny(cwspb::subject::Any & out, const Subject::Plain & in);
void toSubjectId(cwspb::subject::Id & out, const Subject::Id & id);
cwspb::subject::Type toSubjectType(Subject::Type in);

// From
SimulationStatus fromSimulationStatus(const cwspb::SimulationStatus status);
SimulationType fromSimulationType(const cwspb::SimulationType type);
SimulationStateIn fromSimulationState(const cwspb::SimulationState & in);

Dimension fromDimension(const cwspb::Dimension & in);

Coordinates fromCoordinates(const cwspb::Coordinates & coord);
Temperature fromTemperature(const cwspb::Temperature & in);

// Subject::Type fromSubjectType(cwspb::SubjectType in);
// Subject::Type fromSubjectType(const cwspb::SubjectDerived & in);
// Id fromSubjectId(const cwspb::SubjectId & id, cwspb::SubjectType type);
// PlainParams fromSubjectParameters(const cwspb::Subject & in);
// std::unique_ptr<Subject> fromSubjectDerived(const cwspb::SubjectDerived & in);
// std::unique_ptr<Subject> fromSubject(const cwspb::Subject & in, SubjectType type);

// std::unique_ptr<SubjectPlain> fromSubjectPlain(const cwspb::SubjectPlain & in);

// InteractionStateType fromInteractionStateType(const cwspb::InteractionStateType
// type); std::unique_ptr<SubjectInteractive> fromSubjectInteractive(const
// cwspb::SubjectInteractive & in);

// SensorType fromSensorType(const cwspb::SubjectSensorDerived & in);
// std::unique_ptr<SubjectSensor>
// fromSubjectSensorDerived(const cwspb::SubjectSensorDerived & in);
// std::unique_ptr<SubjectSensor> fromSubjectSensor(const cwspb::SubjectSensor & in,
//                                                  SensorType type);
// std::unique_ptr<SensorTemperature>
// fromSensorTemperature(const cwspb::SensorTemperature & in);

// SubjectQueryType fromSubjectQueryType(cwspb::SubjectQueryType type);
