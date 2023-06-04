#include "converters.hpp"

#include "cws/subject/camera.hpp"
#include "cws/subject/light_emitter.hpp"
#include "cws/subject/network.hpp"
#include "cws/subject/sensor.hpp"
#include "cws/subject/temp_emitter.hpp"
#include "cws/subject/turnable.hpp"

namespace pb = cwspb;

// To
pb::SimulationStatus toSimulationStatus(const SimulationStatus status) {
  switch (status) {
  case SimulationStatus::RUNNING:
    return pb::SimulationStatus::SIMULATION_STATUS_RUNNING;
  case SimulationStatus::STOPPED:
    return pb::SimulationStatus::SIMULATION_STATUS_STOPPED;
  default:
    return pb::SimulationStatus::SIMULATION_STATUS_UNSPECIFIED;
  }
}

pb::SimulationType toSimulationType(const SimulationType type) {
  switch (type) {
  case SimulationType::LIMITED:
    return pb::SimulationType::SIMULATION_TYPE_LIMITED;
  case SimulationType::INFINITE:
    return pb::SimulationType::SIMULATION_TYPE_INFINITE;
  default:
    return pb::SimulationType::SIMULATION_TYPE_UNSPECIFIED;
  }
}

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

pb::air::Type toAirType(Air::Type in) {
  switch (in) {
  case Air::Type::PLAIN:
    return pb::air::TYPE_PLAIN;
  default:
    return pb::air::TYPE_UNSPECIFIED;
  }
}

void toSubject(pb::subject::Plain & out, const Subject::Plain & in) {
  toPhysical(*out.mutable_base(), in);
  toSubjectId(*out.mutable_id(), in.getSubjectId());
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

pb::subject::TurnableStatus toTurnableStatus(Subject::TurnableStatus in) {
  using namespace pb::subject;
  switch (in) {
  case Subject::TurnableStatus::ON:
    return TurnableStatus::TURNABLE_STATUS_ON;
  case Subject::TurnableStatus::OFF:
    return TurnableStatus::TURNABLE_STATUS_OFF;
  default:
    return TurnableStatus::TURNABLE_STATUS_UNSPECIFIED;
  }
}

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

void toSubject(pb::subject::NetworkDevice & out, const Subject::NetworkDevice & in) {
  toSubject(*out.mutable_base(), in);
  for (const auto & packet : in.getTransmitPackets()) {
    toPacket(*out.add_transmit_packets(), *packet);
  }
  for (const auto & packet : in.getReceivedPackets()) {
    toPacket(*out.add_received_packets(), *packet);
  }
}

void toSubject(pb::subject::BaseCamera & out, const Subject::BaseCamera & in) {
  toSubject(*out.mutable_base(), in);
  out.set_power(in.getPower());
  out.set_power(in.getPowerThreshold());
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
  out.set_transmit_power(in.getTransmitPower());
  out.set_receive_threshold(in.getReceiveThresh());
}

void toSubjectAny(pb::subject::Any & out, const Subject::Plain & in) {
  using namespace Subject;
  namespace pbs = pb::subject;

  auto type = toSubjectType(in.getSubjectId().type);
  switch (type) {
  case pbs::Type::TYPE_PLAIN:
    return toSubject(*out.mutable_plain(), in);
  case pbs::Type::TYPE_TEMP_EMITTER:
    return toSubject(*out.mutable_temp_emitter(), static_cast<const TempEmitter &>(in));
  case pbs::Type::TYPE_TURNABLE_TEMP_EMITTER:
    return toSubject(*out.mutable_turnable_temp_emitter(),
                     static_cast<const TurnableTempEmitter &>(in));
  case pbs::Type::TYPE_LIGHT_EMITTER:
    return toSubject(*out.mutable_light_emitter(),
                     static_cast<const LightEmitter &>(in));
  case pbs::Type::TYPE_TURNABLE_LIGHT_EMITTER:
    return toSubject(*out.mutable_turnable_light_emitter(),
                     static_cast<const TurnableLightEmitter &>(in));
  case pbs::Type::TYPE_NETWORK_DEVICE:
    return toSubject(*out.mutable_network_device(),
                     static_cast<const NetworkDevice &>(in));
  case pbs::Type::TYPE_INFRARED_CAMERA:
    return toSubject(*out.mutable_infrared_camera(),
                     static_cast<const InfraredCamera &>(in));
  case pbs::Type::TYPE_LIGHT_CAMERA:
    return toSubject(*out.mutable_light_camera(), static_cast<const LightCamera &>(in));
  case pbs::Type::TYPE_TURNABLE:
    return toSubject(*out.mutable_turnable(), static_cast<const Turnable &>(in));
  case pbs::Type::TYPE_AIR_TEMPERATURE_SENSOR:
    return toSubject(*out.mutable_sensor_air_temperature(),
                     static_cast<const SensorAirTemperature &>(in));
  case pbs::Type::TYPE_ILLUMINATION_SENSOR:
    return toSubject(*out.mutable_sensor_illumination(),
                     static_cast<const SensorIllumination &>(in));
  case pbs::Type::TYPE_WIRELESS_NETWORK_DEVICE:
    return toSubject(*out.mutable_wireless_network_device(),
                     static_cast<const WirelessNetworkDevice &>(in));
  default:
    return;
  }
}

void toSubjectId(pb::subject::Id & out, const Subject::Id & id) {
  out.set_type(toSubjectType(id.type));
  out.set_idx(id.idx);
}

pb::subject::Type toSubjectType(Subject::Type in) {
  using namespace pb::subject;

  switch (in) {
  case Subject::Type::PLAIN:
    return Type::TYPE_PLAIN;
  case Subject::Type::TEMP_EMITTER:
    return Type::TYPE_TEMP_EMITTER;
  case Subject::Type::TURNABLE_TEMP_EMITTER:
    return Type::TYPE_TURNABLE_TEMP_EMITTER;
  case Subject::Type::LIGHT_EMITTER:
    return Type::TYPE_LIGHT_EMITTER;
  case Subject::Type::TURNABLE_LIGHT_EMITTER:
    return Type::TYPE_TURNABLE_LIGHT_EMITTER;
  case Subject::Type::NETWORK_DEVICE:
    return Type::TYPE_NETWORK_DEVICE;
  case Subject::Type::INFRARED_CAMERA:
    return Type::TYPE_INFRARED_CAMERA;
  case Subject::Type::LIGHT_CAMERA:
    return Type::TYPE_LIGHT_CAMERA;
  case Subject::Type::TURNABLE:
    return Type::TYPE_TURNABLE;
  case Subject::Type::AIR_TEMPERATURE_SENSOR:
    return Type::TYPE_AIR_TEMPERATURE_SENSOR;
  case Subject::Type::ILLUMINATION_SENSOR:
    return Type::TYPE_ILLUMINATION_SENSOR;
  case Subject::Type::WIRELESS_NETWORK_DEVICE:
    return Type::TYPE_WIRELESS_NETWORK_DEVICE;
  default:
    return Type::TYPE_UNSPECIFIED;
  }
}

// void toSubjectDerived(pb::SubjectDerived & out, const Subject::Plain * subject) {
//   switch (subject->getSubjectId().type) {
//   case SubjectType::PLAIN:
//     return toSubjectPlain(*out.mutable_plain(),
//                           static_cast<const SubjectPlain *>(subject));
//   case SubjectType::SENSOR:
//     return toSubjectSensorDerived(*out.mutable_sensor(),
//                                   static_cast<const SubjectSensor *>(subject));
//   case SubjectType::INTERACTIVE:
//     return toSubjectInteractive(*out.mutable_interactive(),
//                                 static_cast<const SubjectInteractive *>(subject));
//   case SubjectType::UNSPECIFIED:
//   default:
//     return toSubject(*out.mutable_base(), subject);
//   }
// }

// void toSubject(pb::Subject & out, const Subject * subject) {
//   toSubjectId(*out.mutable_id(), subject->getSubjectId());

//   auto params = subject->getSubjectParameters();
//   // out.set_weight(params.weight);
//   // out.set_heat_capacity(params.heatCapacity);
//   // toPercentage(*out.mutable_heat_transmission(), params.heatTransmission);
//   // toTemperature(*out.mutable_temperature(), params.temperature);
//   // toPercentage(*out.mutable_light_transmission(), params.lightTransmission);
//   // toPercentage(*out.mutable_humidity_transmission(), params.humidityTransmission);
// }

// void toSubjectSensorDerived(pb::SubjectSensorDerived & out,
//                             const SubjectSensor * subject) {
//   switch (subject->getSensorType()) {
//   case SensorType::TEMPERATURE:
//     return toSensorTemperature(*out.mutable_temperature(),
//                                static_cast<const SensorTemperature *>(subject));
//   default:
//     return toSubjectSensor(*out.mutable_base(), subject);
//   }
// }

// pb::InteractionStateType toInteractionStateType(const InteractionStateType type) {
//   switch (type) {
//   case InteractionStateType::ENABLED:
//     return pb::InteractionStateType::INTERACTION_STATE_TYPE_OPENED;
//   case InteractionStateType::DISABLED:
//     return pb::InteractionStateType::INTERACTION_STATE_TYPE_CLOSED;
//   case InteractionStateType::UNSPECIFIED:
//   default:
//     return pb::InteractionStateType::INTERACTION_STATE_TYPE_UNSPECIFIED;
//   }
// }

// void toSubjectInteractive(pb::SubjectInteractive & out,
//                           const SubjectInteractive * subject) {
//   toSubject(*out.mutable_base(), subject);
//   out.set_interaction_state_type(
//       toInteractionStateType(subject->getInteractionState().type));
// }

// void toSubjectPlain(pb::SubjectPlain & out, const SubjectPlain * plain) {
//   toSubject(*out.mutable_base(), plain);
// }

// void toSubjectSensor(pb::SubjectSensor & out, const SubjectSensor * subject) {
//   toSubject(*out.mutable_base(), subject);
// }

// void toSensorTemperature(pb::SensorTemperature & out,
//                          const SensorTemperature * sensor) {
//   toSubjectSensor(*out.mutable_base(), sensor);
//   toTemperature(*out.mutable_temperature(), sensor->getTemperature());
// }

// From
SimulationStatus fromSimulationStatus(const pb::SimulationStatus status) {
  switch (status) {
  case pb::SimulationStatus::SIMULATION_STATUS_RUNNING:
    return SimulationStatus::RUNNING;
  case pb::SimulationStatus::SIMULATION_STATUS_STOPPED:
    return SimulationStatus::STOPPED;
  default:
    return SimulationStatus::STOPPED;
  }
}

SimulationType fromSimulationType(const pb::SimulationType type) {
  switch (type) {
  case pb::SimulationType::SIMULATION_TYPE_LIMITED:
    return SimulationType::LIMITED;
  case pb::SimulationType::SIMULATION_TYPE_INFINITE:
    return SimulationType::INFINITE;
  default:
    return SimulationType::LIMITED;
  }
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

// SubjectType fromSubjectType(pb::SubjectType in) {
//   switch (in) {
//   case pb::SubjectType::SUBJECT_TYPE_PLAIN:
//     return SubjectType::PLAIN;
//   case pb::SubjectType::SUBJECT_TYPE_INTERACTIVE:
//     return SubjectType::INTERACTIVE;
//   case pb::SubjectType::SUBJECT_TYPE_SENSOR:
//     return SubjectType::SENSOR;
//   case pb::SubjectType::SUBJECT_TYPE_UNSPECIFIED:
//   default:
//     return SubjectType::UNSPECIFIED;
//   }
// }

// SubjectType fromSubjectType(const pb::SubjectDerived & in) {
//   if (in.has_plain())
//     return SubjectType::PLAIN;
//   else if (in.has_interactive())
//     return SubjectType::INTERACTIVE;
//   else if (in.has_sensor())
//     return SubjectType::SENSOR;
//   else
//     return SubjectType::UNSPECIFIED;
// }

// Id fromSubjectId(const pb::SubjectId & id, pb::SubjectType type) {
//   Id out;
//   out.type = fromSubjectType(type);
//   out.idx = id.id();
//   return out;
// }

// SubjectParameters fromSubjectParameters(const pb::Subject & in) {
//   return {
//       // .weight = in.weight(),
//       // .heatCapacity = in.heat_capacity(),
//       // .heatTransmission = fromPercentage(in.heat_transmission()),
//       // .temperature = fromTemperature(in.temperature()),
//       // .lightTransmission = fromPercentage(in.light_transmission()),
//       // .humidityTransmission = fromPercentage(in.humidity_transmission()),
//   };
// }

// std::unique_ptr<Subject> fromSubjectDerived(const pb::SubjectDerived & in) {
//   SubjectType outST = fromSubjectType(in);
//   switch (outST) {
//   case SubjectType::PLAIN:
//     return fromSubjectPlain(in.plain());
//   case SubjectType::INTERACTIVE:
//     return fromSubjectInteractive(in.interactive());
//   case SubjectType::SENSOR:
//     return fromSubjectSensorDerived(in.sensor());
//   case SubjectType::UNSPECIFIED:
//   default:
//     if (in.has_base()) {
//       return fromSubject(in.base(), outST);
//     } else {
//       return std::unique_ptr<Subject>();
//     }
//   }
// }

// std::unique_ptr<Subject> fromSubject(const pb::Subject & in, SubjectType type) {
//   auto params = fromSubjectParameters(in);
//   auto id = fromSubjectId(in.id(), toSubjectType(type));
//   return std::make_unique<Subject>(id, params);
// }

// std::unique_ptr<SubjectPlain> fromSubjectPlain(const pb::SubjectPlain & in) {
//   auto idx = in.base().id().id();
//   auto params = fromSubjectParameters(in.base());
//   return std::make_unique<SubjectPlain>(idx, params);
// }

// InteractionStateType fromInteractionStateType(const pb::InteractionStateType type)
// {
//   switch (type) {
//   case pb::InteractionStateType::INTERACTION_STATE_TYPE_CLOSED:
//     return InteractionStateType::DISABLED;
//   case pb::InteractionStateType::INTERACTION_STATE_TYPE_OPENED:
//     return InteractionStateType::ENABLED;
//   case pb::InteractionStateType::INTERACTION_STATE_TYPE_UNSPECIFIED:
//   default:
//     return InteractionStateType::UNSPECIFIED;
//   }
// }

// std::unique_ptr<SubjectInteractive>
// fromSubjectInteractive(const pb::SubjectInteractive & in) {
//   auto idx = in.base().id().id();
//   auto params = fromSubjectParameters(in.base());
//   auto ist = fromInteractionStateType(in.interaction_state_type());
//   InteractiveState is{.type = ist};
//   return std::make_unique<SubjectInteractive>(idx, params, is);
// }

// SensorType fromSensorType(const pb::SubjectSensorDerived & in) {
//   if (in.has_temperature()) {
//     return SensorType::TEMPERATURE;
//   } else if (in.has_base()) {
//     return SensorType::UNSPECIFIED;
//   }
//   return SensorType::UNSPECIFIED;
// }

// std::unique_ptr<SubjectSensor>
// fromSubjectSensorDerived(const pb::SubjectSensorDerived & in) {
//   SensorType type = fromSensorType(in);
//   switch (type) {
//   case SensorType::TEMPERATURE:
//     return fromSensorTemperature(in.temperature());
//   case SensorType::UNSPECIFIED:
//   default:
//     if (in.has_base()) {
//       return fromSubjectSensor(in.base(), type);
//     } else {
//       return std::unique_ptr<SubjectSensor>();
//     }
//   }
// }
// std::unique_ptr<SubjectSensor> fromSubjectSensor(const pb::SubjectSensor & in,
//                                                  SensorType type) {
//   auto idx = in.base().id().id();
//   auto params = fromSubjectParameters(in.base());
//   return std::make_unique<SubjectSensor>(type, idx, params);
// }

// std::unique_ptr<SensorTemperature>
// fromSensorTemperature(const pb::SensorTemperature & in) {
//   auto idx = in.base().base().id().id();
//   auto params = fromSubjectParameters(in.base().base());
//   auto temp = fromTemperature(in.temperature());
//   return std::make_unique<SensorTemperature>(idx, params, temp);
// }

// SubjectQueryType fromSubjectQueryType(pb::SubjectQueryType type) {
//   switch (type) {
//   case pb::SubjectQueryType::SUBJECT_QUERY_TYPE_DELETE:
//     return SubjectQueryType::DELETE;
//   case pb::SubjectQueryType::SUBJECT_QUERY_TYPE_INSERT:
//     return SubjectQueryType::INSERT;
//   case pb::SubjectQueryType::SUBJECT_QUERY_TYPE_UPDATE:
//     return SubjectQueryType::UPDATE;
//   case pb::SubjectQueryType::SUBJECT_QUERY_TYPE_UNSPECIFIED:
//     // case pb::SubjectQueryType::SUBJECT_QUERY_TYPE_SELECT:
//     return SubjectQueryType::SELECT;
//   default:
//     return SubjectQueryType::UNSPECIFIED;
//   }
// }
