#pragma once

// To
#include "cws/common.hpp"
#include "cws/map.hpp"
#include "cws/simulation/general.hpp"
#include "cws/simulation/simulation_map.hpp"
#include "cws/simulation/state.hpp"
#include "cws/subject/extension/turnable.hpp"
#include "cwspb/common.pb.h"
#include "cwspb/map.pb.h"
#include "cwspb/service/common.pb.h"
#include "cwspb/service/sv_map.pb.h"
#include "cwspb/service/sv_simulation.pb.h"

int toSimulationStatus(const SimulationStatus status);
int toSimulationType(const SimulationType type);
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
void toAirId(cwspb::AirId & out, const Air::Id & in, Coordinates c);
int toAirType(Air::Type in);

void toSubjectAny(cwspb::subject::Any & out, const Subject::Plain & in);
void toSubjectId(cwspb::subject::Id & out, const Subject::Id & id);
void toSubjectId(cwspb::SubjectId & out, const Subject::Id & id, Coordinates c);
int toSubjectType(Subject::Type in);

// From
SimulationStatus fromSimulationStatus(const int status);
SimulationType fromSimulationType(const int type);
SimulationStateIn fromSimulationState(const cwspb::SimulationState & in);

Dimension fromDimension(const cwspb::Dimension & in);

Coordinates fromCoordinates(const cwspb::Coordinates & coord);
Temperature fromTemperature(const cwspb::Temperature & in);

Subject::Type fromSubjectType(int in);
Subject::Type fromSubjectType(const cwspb::subject::Any & in);
Subject::Id fromSubjectId(const cwspb::subject::Id & id);
void fromSubjectId(Subject::Id & outId, Coordinates & outC,
                   const cwspb::SubjectId & id);
Subject::TurnableStatus fromTurnableStatus(int in);
std::unique_ptr<Subject::Plain> fromSubjectAny(const cwspb::subject::Any & in);

SubjectModifyType fromSubjectModifyType(cwspb::SubjectModifyType type);

Air::Id fromAirId(const cwspb::air::Id & id);
void fromAirId(Air::Id & outId, Coordinates & outC, const cwspb::AirId & id);
std::unique_ptr<Air::Plain> fromAirPlain(const cwspb::air::Plain & in);

std::unique_ptr<Network::Packet> fromPacket(const cwspb::network::Packet & in);
