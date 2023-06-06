#include "converters.hpp"

#include "cws/simulation/simulation_map.hpp"
#include "cws/subject/camera.hpp"
#include "cws/subject/light_emitter.hpp"
#include "cws/subject/network.hpp"
#include "cws/subject/sensor.hpp"
#include "cws/subject/temp_emitter.hpp"
#include "cws/subject/turnable.hpp"

namespace pb = cwspb;

int toSimulationStatus(const SimulationStatus status) {
  return static_cast<int>(status);
}

int toSimulationType(const SimulationType type) { return static_cast<int>(type); }

void toSimulationState(pb::SimulationState & out, const struct SimulationState & in) {
  out.set_status(toSimulationStatus(in.status));
  out.set_type(toSimulationType(in.type));
  out.set_task_frequency(in.taskFrequency);
  out.set_current_tick(in.currentTick);
  out.set_last_tick(in.lastTick);
}

void toDimension(pb::Dimension & out, const Dimension & dim) {
  out.set_width(dim.width);
  out.set_height(dim.height);
}

void toCoordinates(pb::Coordinates & out, const Coordinates & coord) {
  out.set_x(coord.x);
  out.set_y(coord.y);
}

void toTemperature(pb::Temperature & out, const Temperature & in) {
  out.set_value(in.value);
}

void toIllumination(pb::Illumination & out, const Illumination & in) {
  out.set_value(in.get());
}

void toObstruction(pb::Obstruction & out, const Obstruction & in) {
  out.set_value(in.get());
}

void toCell(pb::Cell & out, const Layers & layers, const Coordinates c) {
  auto & mapLayerWireless = layers.networkWireless;
  auto & airLayer = layers.airLayer.getCell(c).getElement();
  auto & illuminationLayer = layers.illuminationLayer.getCell(c).getElement();
  auto & obstructionLayer = layers.obstructionLayer.getCell(c).getElement();
  auto & subjectLayer = layers.subjectLayer.getCell(c).getElement();

  toCoordinates(*out.mutable_coordinates(), c);
  toLayerAir(*out.mutable_air(), airLayer);
  toLayerIllumination(*out.mutable_illumination(), illuminationLayer);
  toLayerNetworkWireless(*out.mutable_wireless_network(),
                         mapLayerWireless.getTransmittableLayer(c),
                         mapLayerWireless.getReceivableLayer(c));
  toLayerObstruction(*out.mutable_obstruction(), obstructionLayer);
  toLayerSubject(*out.mutable_subject(), subjectLayer);
}

void toMap(cwspb::Map & out, const Layers & layers, Dimension dim) {
  toDimension(*out.mutable_dimension(), dim);
  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x) {
    for (c.y = 0; c.y < dim.height; ++c.y) {
      toCell(*out.add_cells(), layers, c);
    }
  }
}

void toLayerAir(pb::layer::Air & out, const LayerAir & in) {
  for (const auto & air : in.getAirContainer().getList()) {
    toAirPlain(*out.add_airs(), *air);
  }
}

void toLayerIllumination(pb::layer::Illumination & out, const LayerIllumination & in) {
  toIllumination(*out.mutable_illumination(), in.getIllumination());
}

void toLayerObstruction(pb::layer::Obstruction & out, const LayerObstruction & in) {
  toObstruction(*out.mutable_light_obstruction(), in.getLightObstruction());
  toObstruction(*out.mutable_air_obstruction(), in.getAirObstruction());
  toObstruction(*out.mutable_wireless_obstruction(), in.getWirelessObstruction());
}

void toLayerSubject(pb::layer::Subject & out, const LayerSubject & in) {
  for (const auto & sub : in.getSubjectList()) {
    toSubjectAny(*out.add_subjects(), *sub);
  }
}

void toLayerNetworkWireless(pb::layer::WirelessNetwork & out,
                            const LayerNetwork & transmitables,
                            const LayerNetwork & receivables) {
  for (auto & cont : transmitables.getContainerList()) {
    toWirelessContainer(*out.add_transmitables(),
                        static_cast<Network::WirelessContainer &>(*cont));
  }

  for (const auto & cont : receivables.getContainerList()) {
    toWirelessContainer(*out.add_receivables(),
                        static_cast<Network::WirelessContainer &>(*cont));
  }
}

void toWirelessContainer(pb::network::WirelessContainer & out,
                         const Network::WirelessContainer & in) {
  toPacket(*out.mutable_packet(), *in.getPacket());
  out.set_signal_power(in.getSignalPower());
}

void toPacket(pb::network::Packet & out, const Network::Packet & in) {
  std::string content(reinterpret_cast<const char *>(in.getContent().data()),
                      in.getContent().size());
  out.set_content(std::move(content));
}

void toPhysical(pb::Physical & out, const Physical & in) {
  out.set_weight(in.getWeight());
  out.set_heat_capacity(in.getHeatCapacity());
  toTemperature(*out.mutable_temperature(), in.getTemperature());
  toObstruction(*out.mutable_light_obstruction(), in.getDefLightObstruction());
  toObstruction(*out.mutable_wireless_obstruction(), in.getDefWirelessObstruction());
}

void toAirPlain(pb::air::Plain & out, const Air::Plain & in) {
  toPhysical(*out.mutable_base(), static_cast<const Physical &>(in));
  toAirId(*out.mutable_id(), in.getId());
  out.set_heat_transfer_coef(in.getHeatTransferCoef());
}

void toAirId(pb::air::Id & out, const Air::Id & in) {
  out.set_type(toAirType(in.type));
  out.set_idx(in.idx);
}

void toAirId(pb::AirId & out, const Air::Id & id, Coordinates c) {
  toAirId(*out.mutable_id(), id);
  toCoordinates(*out.mutable_coordinates(), c);
}

int toAirType(Air::Type in) { return static_cast<int>(in); }

void toSubject(pb::subject::Plain & out, const Subject::Plain & in) {
  toPhysical(*out.mutable_base(), in);
  toSubjectId(*out.mutable_id(), in.getId());
  out.set_surface_area(in.getSurfaceArea());
  toObstruction(*out.mutable_air_obstruction(), in.getDefAirObstruction());
}

void toTempSourceParams(pb::subject::TempSourceParams & out,
                        const Subject::TempSourceParams & in) {
  out.set_heat_production(in.heatProduction);
}

void toSubject(pb::subject::TempEmitter & out, const Subject::TempEmitter & in) {
  toSubject(*out.mutable_base(), in);
  toTempSourceParams(*out.mutable_temp_params(), in.getDefTempParams());
}

int toTurnableStatus(Subject::TurnableStatus in) { return static_cast<int>(in); }

void toSubject(pb::subject::TurnableTempEmitter & out,
               const Subject::TurnableTempEmitter & in) {
  toSubject(*out.mutable_base(), in);
  out.set_turnable_status(toTurnableStatus(in.getStatus()));
  toTempSourceParams(*out.mutable_off_temp_params(), in.getOffTempParams());
}

void toLightSourceParams(pb::subject::LightSourceParams & out,
                         const Subject::LightSourceParams & in) {
  toIllumination(*out.mutable_raw_illumination(), in.rawIllumination);
}

void toSubject(pb::subject::LightEmitter & out, const Subject::LightEmitter & in) {
  toSubject(*out.mutable_base(), in);
  toLightSourceParams(*out.mutable_light_params(), in.getDefLightParams());
}

void toSubject(pb::subject::TurnableLightEmitter & out,
               const Subject::TurnableLightEmitter & in) {
  toSubject(*out.mutable_base(), in);
  out.set_turnable_status(toTurnableStatus(in.getStatus()));
  toTempSourceParams(*out.mutable_off_temp_params(), in.getOffTempParams());
  toLightSourceParams(*out.mutable_off_light_params(), in.getOffLightParams());
}

void toSubject(pb::subject::BaseCamera & out, const Subject::BaseCamera & in) {
  toSubject(*out.mutable_base(), in);
  out.set_power(in.getPower());
  out.set_power_threshold(in.getPowerThreshold());
}

void toSubject(pb::subject::InfraredCamera & out, const Subject::InfraredCamera & in) {
  toSubject(*out.mutable_base(), in);
}

void toSubject(pb::subject::LightCamera & out, const Subject::LightCamera & in) {
  toSubject(*out.mutable_base(), in);
  out.set_light_threshold(in.getLightThreshold());
}

void toSubject(pb::subject::Turnable & out, const Subject::Turnable & in) {
  toSubject(*out.mutable_base(), in);
  out.set_turnable_status(toTurnableStatus(in.getStatus()));
  toObstruction(*out.mutable_off_light_obs(), in.getOffLightObstruction());
  toObstruction(*out.mutable_off_wireless_obs(), in.getOffWirelessObstruction());
  toObstruction(*out.mutable_off_air_obs(), in.getOffAirObstruction());
}

void toSubject(pb::subject::SensorAirTemperature & out,
               const Subject::SensorAirTemperature & in) {
  toSubject(*out.mutable_base(), in);
  toTemperature(*out.mutable_air_temperature(), in.getAirTemperature());
}

void toSubject(pb::subject::SensorIllumination & out,
               const Subject::SensorIllumination & in) {
  toSubject(*out.mutable_base(), in);
  toIllumination(*out.mutable_cell_illumination(), in.getCellIllumination());
}

void toSubject(pb::subject::WirelessNetworkDevice & out,
               const Subject::WirelessNetworkDevice & in) {
  toSubject(*out.mutable_base(), in);
  for (const auto & packet : in.getTransmitPackets()) {
    toPacket(*out.add_transmit_packets(), *packet);
  }
  for (const auto & packet : in.getReceivedPackets()) {
    toPacket(*out.add_received_packets(), *packet);
  }
  out.set_transmit_power(in.getTransmitPower());
  out.set_receive_threshold(in.getReceiveThresh());
}

void toSubjectAny(pb::subject::Any & out, const Subject::Plain & in) {
  using namespace Subject;

  auto type = in.getId().type;
  switch (type) {
  case Type::PLAIN:
    return toSubject(*out.mutable_plain(), in);
  case Type::TEMP_EMITTER:
    return toSubject(*out.mutable_temp_emitter(), static_cast<const TempEmitter &>(in));
  case Type::TURNABLE_TEMP_EMITTER:
    return toSubject(*out.mutable_turnable_temp_emitter(),
                     static_cast<const TurnableTempEmitter &>(in));
  case Type::LIGHT_EMITTER:
    return toSubject(*out.mutable_light_emitter(),
                     static_cast<const LightEmitter &>(in));
  case Type::TURNABLE_LIGHT_EMITTER:
    return toSubject(*out.mutable_turnable_light_emitter(),
                     static_cast<const TurnableLightEmitter &>(in));
  case Type::WIRELESS_NETWORK_DEVICE:
    return toSubject(*out.mutable_wireless_network_device(),
                     static_cast<const WirelessNetworkDevice &>(in));
  case Type::INFRARED_CAMERA:
    return toSubject(*out.mutable_infrared_camera(),
                     static_cast<const InfraredCamera &>(in));
  case Type::LIGHT_CAMERA:
    return toSubject(*out.mutable_light_camera(), static_cast<const LightCamera &>(in));
  case Type::TURNABLE:
    return toSubject(*out.mutable_turnable(), static_cast<const Turnable &>(in));
  case Type::AIR_TEMPERATURE_SENSOR:
    return toSubject(*out.mutable_sensor_air_temperature(),
                     static_cast<const SensorAirTemperature &>(in));
  case Type::ILLUMINATION_SENSOR:
    return toSubject(*out.mutable_sensor_illumination(),
                     static_cast<const SensorIllumination &>(in));
  default:
    return;
  }
}

void toSubjectId(pb::subject::Id & out, const Subject::Id & id) {
  out.set_type(toSubjectType(id.type));
  out.set_idx(id.idx);
}

void toSubjectId(cwspb::SubjectId & out, const Subject::Id & id, Coordinates c) {
  toSubjectId(*out.mutable_id(), id);
  toCoordinates(*out.mutable_coordinates(), c);
}

int toSubjectType(Subject::Type in) { return static_cast<int>(in); }

// From
SimulationStatus fromSimulationStatus(const int status) {
  return static_cast<SimulationStatus>(status);
}

SimulationType fromSimulationType(const int type) {
  return static_cast<SimulationType>(type);
}

SimulationStateIn fromSimulationState(const pb::SimulationState & in) {
  SimulationStateIn out;
  if (in.has_type())
    out.simType.set(fromSimulationType(in.type()));
  if (in.has_status())
    out.simStatus.set(fromSimulationStatus(in.status()));
  if (in.has_current_tick())
    out.currentTick.set(in.current_tick());
  if (in.has_last_tick())
    out.lastTick.set(in.last_tick());
  if (in.has_task_frequency())
    out.taskFrequency.set(in.task_frequency());
  return out;
}

Dimension fromDimension(const pb::Dimension & in) {
  Dimension out;
  out.width = in.width();
  out.height = in.height();
  return out;
}

Coordinates fromCoordinates(const pb::Coordinates & coord) {
  Coordinates out;
  out.x = coord.x();
  out.y = coord.y();
  return out;
}

Temperature fromTemperature(const pb::Temperature & in) {
  Temperature out;
  out.value = in.value();
  return out;
}

Obstruction fromObstruction(const pb::Obstruction & in) {
  Obstruction out;
  out.value = in.value();
  return out;
}

Illumination fromIllumination(const pb::Illumination & in) {
  Illumination out;
  out.value = in.value();
  return out;
}

Subject::Id fromSubjectId(const pb::subject::Id & in) {
  Subject::Id out;
  out.idx = in.idx();
  out.type = fromSubjectType(in.type());
  return out;
}

void fromSubjectId(Subject::Id & outId, Coordinates & outC,
                   const cwspb::SubjectId & id) {
  outId = fromSubjectId(id.id());
  outC = fromCoordinates(id.coordinates());
}

Subject::Type fromSubjectType(int in) { return static_cast<Subject::Type>(in); }

// NOTE: may pass to proto type as separate field
Subject::Type fromSubjectType(const cwspb::subject::Any & in) {
  if (in.has_plain())
    return Subject::Type::PLAIN;
  if (in.has_temp_emitter())
    return Subject::Type::TEMP_EMITTER;
  if (in.has_turnable_temp_emitter())
    return Subject::Type::TURNABLE_TEMP_EMITTER;
  if (in.has_light_emitter())
    return Subject::Type::LIGHT_EMITTER;
  if (in.has_turnable_light_emitter())
    return Subject::Type::TURNABLE_LIGHT_EMITTER;
  if (in.has_wireless_network_device())
    return Subject::Type::WIRELESS_NETWORK_DEVICE;
  if (in.has_infrared_camera())
    return Subject::Type::INFRARED_CAMERA;
  if (in.has_light_camera())
    return Subject::Type::LIGHT_CAMERA;
  if (in.has_turnable())
    return Subject::Type::TURNABLE;
  if (in.has_sensor_air_temperature())
    return Subject::Type::AIR_TEMPERATURE_SENSOR;
  if (in.has_sensor_illumination())
    return Subject::Type::ILLUMINATION_SENSOR;
  return Subject::Type::UNSPECIFIED;
}

std::unique_ptr<Physical> fromPhysical(const pb::Physical & in) {
  return std::make_unique<Physical>(in.weight(), in.heat_capacity(),
                                    fromTemperature(in.temperature()),
                                    fromObstruction(in.light_obstruction()),
                                    fromObstruction(in.wireless_obstruction()));
}

std::unique_ptr<Subject::Plain> fromSubject(const pb::subject::Plain & in) {
  auto base = fromPhysical(in.base());
  auto id = fromSubjectId(in.id());
  return std::make_unique<Subject::Plain>(std::move(*base), id.idx, in.surface_area(),
                                          fromObstruction(in.air_obstruction()));
}

Subject::TempSourceParams
fromTempSourceParams(const pb::subject::TempSourceParams & in) {
  return {.heatProduction = in.heat_production()};
}

Subject::LightSourceParams
fromLightSourceParams(const pb::subject::LightSourceParams & in) {
  return {.rawIllumination = fromIllumination(in.raw_illumination())};
}

std::unique_ptr<Subject::TempEmitter> fromSubject(const pb::subject::TempEmitter & in) {
  auto base = fromSubject(in.base());
  auto tempSP = fromTempSourceParams(in.temp_params());
  return std::make_unique<Subject::TempEmitter>(std::move(*base), tempSP);
}

Subject::TurnableStatus fromTurnableStatus(int in) {
  return static_cast<Subject::TurnableStatus>(in);
}

std::unique_ptr<Subject::TurnableTempEmitter>
fromSubject(const pb::subject::TurnableTempEmitter & in) {
  auto base = fromSubject(in.base());
  auto turnS = fromTurnableStatus(in.turnable_status());
  auto offTempSP = fromTempSourceParams(in.off_temp_params());
  return std::make_unique<Subject::TurnableTempEmitter>(std::move(*base), turnS,
                                                        offTempSP);
}

std::unique_ptr<Subject::LightEmitter>
fromSubject(const pb::subject::LightEmitter & in) {
  auto base = fromSubject(in.base());
  auto tempSP = fromTempSourceParams(in.temp_params());
  auto lightSP = fromLightSourceParams(in.light_params());
  return std::make_unique<Subject::LightEmitter>(std::move(*base), tempSP, lightSP);
}

std::unique_ptr<Subject::TurnableLightEmitter>
fromSubject(const pb::subject::TurnableLightEmitter & in) {
  auto base = fromSubject(in.base());
  auto turnS = fromTurnableStatus(in.turnable_status());
  auto tempSP = fromTempSourceParams(in.off_temp_params());
  auto lightSP = fromLightSourceParams(in.off_light_params());
  return std::make_unique<Subject::TurnableLightEmitter>(std::move(*base), turnS,
                                                         lightSP, tempSP);
}

std::unique_ptr<Subject::WirelessNetworkDevice>
fromSubject(const pb::subject::WirelessNetworkDevice & in) {
  auto base = fromSubject(in.base());
  auto transP = in.transmit_power();
  auto receivTH = in.receive_threshold();
  return std::make_unique<Subject::WirelessNetworkDevice>(std::move(*base), transP,
                                                          receivTH);
}

std::unique_ptr<Subject::InfraredCamera>
fromSubject(const pb::subject::InfraredCamera & in) {
  auto base = fromSubject(in.base().base());
  auto power = in.base().power();
  auto powerTH = in.base().power_threshold();
  return std::make_unique<Subject::InfraredCamera>(std::move(*base), power, powerTH);
}

std::unique_ptr<Subject::LightCamera> fromSubject(const pb::subject::LightCamera & in) {
  auto base = fromSubject(in.base().base());
  auto power = in.base().power();
  auto powerTH = in.base().power_threshold();
  auto lightTH = in.light_threshold();
  return std::make_unique<Subject::LightCamera>(std::move(*base), power, powerTH,
                                                lightTH);
}

std::unique_ptr<Subject::Turnable> fromSubject(const pb::subject::Turnable & in) {
  auto base = fromSubject(in.base());
  auto status = fromTurnableStatus(in.turnable_status());
  auto lightObs = fromObstruction(in.off_light_obs());
  auto wirelessObs = fromObstruction(in.off_wireless_obs());
  auto airObs = fromObstruction(in.off_air_obs());
  return std::make_unique<Subject::Turnable>(std::move(*base), status, lightObs,
                                             wirelessObs, airObs);
}

std::unique_ptr<Subject::SensorAirTemperature>
fromSubject(const pb::subject::SensorAirTemperature & in) {
  auto base = fromSubject(in.base());
  return std::make_unique<Subject::SensorAirTemperature>(std::move(*base));
}

std::unique_ptr<Subject::SensorIllumination>
fromSubject(const pb::subject::SensorIllumination & in) {
  auto base = fromSubject(in.base());
  return std::make_unique<Subject::SensorIllumination>(std::move(*base));
}

std::unique_ptr<Subject::Plain> fromSubjectAny(const pb::subject::Any & in) {
  using namespace Subject;

  auto outST = fromSubjectType(in);

  switch (outST) {
  case Type::PLAIN:
    return fromSubject(in.plain());
  case Type::TEMP_EMITTER:
    return fromSubject(in.temp_emitter());
  case Type::TURNABLE_TEMP_EMITTER:
    return fromSubject(in.turnable_temp_emitter());
  case Type::LIGHT_EMITTER:
    return fromSubject(in.light_emitter());
  case Type::TURNABLE_LIGHT_EMITTER:
    return fromSubject(in.turnable_light_emitter());
  case Type::WIRELESS_NETWORK_DEVICE:
    return fromSubject(in.wireless_network_device());
  case Type::INFRARED_CAMERA:
    return fromSubject(in.infrared_camera());
  case Type::LIGHT_CAMERA:
    return fromSubject(in.light_camera());
  case Type::TURNABLE:
    return fromSubject(in.turnable());
  case Type::AIR_TEMPERATURE_SENSOR:
    return fromSubject(in.sensor_air_temperature());
  case Type::ILLUMINATION_SENSOR:
    return fromSubject(in.sensor_illumination());
  default:
    return std::unique_ptr<Subject::Plain>();
  }
}

SubjectModifyType fromSubjectModifyType(pb::SubjectModifyType type) {
  switch (type) {
  case pb::SubjectModifyType::SUBJECT_MODIFY_TYPE_DELETE:
    return SubjectModifyType::DELETE;
  case pb::SubjectModifyType::SUBJECT_MODIFY_TYPE_INSERT:
    return SubjectModifyType::INSERT;
  case pb::SubjectModifyType::SUBJECT_MODIFY_TYPE_UPDATE:
    return SubjectModifyType::UPDATE;
  default:
    return SubjectModifyType::UNSPECIFIED;
  }
}

Air::Type fromAirType(int in) { return static_cast<Air::Type>(in); }

Air::Id fromAirId(const cwspb::air::Id & id) {
  Air::Id out;
  out.idx = id.idx();
  out.type = fromAirType(id.type());
  return out;
}
void fromAirId(Air::Id & outId, Coordinates & outC, const cwspb::AirId & id) {
  outId = fromAirId(id.id());
  outC = fromCoordinates(id.coordinates());
}

std::unique_ptr<Air::Plain> fromAirPlain(const pb::air::Plain & in) {
  auto base = fromPhysical(in.base());
  auto id = fromAirId(in.id());
  return std::make_unique<Air::Plain>(std::move(*base), id.idx,
                                      in.heat_transfer_coef());
}

std::unique_ptr<Network::Packet> fromPacket(const cwspb::network::Packet & in) {
  auto & inStr = in.content();
  const std::byte * byteA = reinterpret_cast<const std::byte *>(inStr.data());
  std::vector<std::byte> byteV(byteA, byteA + inStr.size());
  return std::make_unique<Network::Packet>(std::move(byteV));
}
