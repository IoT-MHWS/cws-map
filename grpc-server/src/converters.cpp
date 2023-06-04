#include "converters.hpp"

// To
cwspb::SimulationStatus toSimulationStatus(const SimulationStatus status) {
  switch (status) {
  case SimulationStatus::RUNNING:
    return cwspb::SimulationStatus::SIMULATION_STATUS_RUNNING;
  case SimulationStatus::STOPPED:
    return cwspb::SimulationStatus::SIMULATION_STATUS_STOPPED;
  default:
    return cwspb::SimulationStatus::SIMULATION_STATUS_UNSPECIFIED;
  }
}

cwspb::SimulationType toSimulationType(const SimulationType type) {
  switch (type) {
  case SimulationType::LIMITED:
    return cwspb::SimulationType::SIMULATION_TYPE_LIMITED;
  case SimulationType::INFINITE:
    return cwspb::SimulationType::SIMULATION_TYPE_INFINITE;
  default:
    return cwspb::SimulationType::SIMULATION_TYPE_UNSPECIFIED;
  }
}

void toSimulationState(cwspb::SimulationState & out, const struct SimulationState & in) {
  out.set_status(toSimulationStatus(in.status));
  out.set_type(toSimulationType(in.type));
  out.set_task_frequency(in.taskFrequency);
  out.set_current_tick(in.currentTick);
  out.set_last_tick(in.lastTick);
}

// void toCell(cwspb::Cell & out, const Cell & cell) {
//   toCoordinates(*out.mutable_coordinates(), cell.coordinates);
//   // toLayerSubject(*out.mutable_subject(), cell.subject);
// }

// void toCoordinates(cwspb::Coordinates & out, const Coordinates & coord) {
//   out.set_x(coord.x);
//   out.set_y(coord.y);
// }

// void toTemperature(cwspb::Temperature & out, const Temperature & temp) {
//   out.set_value(temp.value);
// }

// void toLayer(cwspb::Layer & out, const Layer & layer) {}

// void toLayerSubject(cwspb::LayerSubject & out, const LayerSubject & layerSubject) {
//   toLayer(*out.mutable_base(), static_cast<const Layer &>(layerSubject));
//   for (const auto & subject : layerSubject.getSubjectList()) {
//     auto outSubject = out.add_subjects();
//     toSubjectDerived(*outSubject, subject.get());
//   }
// }

// cwspb::SubjectType toSubjectType(SubjectType in) {
//   switch (in) {
//   case SubjectType::PLAIN:
//     return cwspb::SubjectType::SUBJECT_TYPE_PLAIN;
//   case SubjectType::INTERACTIVE:
//     return cwspb::SubjectType::SUBJECT_TYPE_INTERACTIVE;
//   case SubjectType::SENSOR:
//     return cwspb::SubjectType::SUBJECT_TYPE_SENSOR;
//   case SubjectType::UNSPECIFIED:
//   default:
//     return cwspb::SubjectType::SUBJECT_TYPE_UNSPECIFIED;
//   }
// }

// void toSubjectId(cwspb::SubjectId & out, const Id & id) { out.set_id(id.idx); }

// void toSubjectDerived(cwspb::SubjectDerived & out, const Subject::Plain * subject) {
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

// void toSubject(cwspb::Subject & out, const Subject * subject) {
//   toSubjectId(*out.mutable_id(), subject->getSubjectId());

//   auto params = subject->getSubjectParameters();
//   // out.set_weight(params.weight);
//   // out.set_heat_capacity(params.heatCapacity);
//   // toPercentage(*out.mutable_heat_transmission(), params.heatTransmission);
//   // toTemperature(*out.mutable_temperature(), params.temperature);
//   // toPercentage(*out.mutable_light_transmission(), params.lightTransmission);
//   // toPercentage(*out.mutable_humidity_transmission(), params.humidityTransmission);
// }

// void toSubjectSensorDerived(cwspb::SubjectSensorDerived & out,
//                             const SubjectSensor * subject) {
//   switch (subject->getSensorType()) {
//   case SensorType::TEMPERATURE:
//     return toSensorTemperature(*out.mutable_temperature(),
//                                static_cast<const SensorTemperature *>(subject));
//   default:
//     return toSubjectSensor(*out.mutable_base(), subject);
//   }
// }

// cwspb::InteractionStateType toInteractionStateType(const InteractionStateType type) {
//   switch (type) {
//   case InteractionStateType::ENABLED:
//     return cwspb::InteractionStateType::INTERACTION_STATE_TYPE_OPENED;
//   case InteractionStateType::DISABLED:
//     return cwspb::InteractionStateType::INTERACTION_STATE_TYPE_CLOSED;
//   case InteractionStateType::UNSPECIFIED:
//   default:
//     return cwspb::InteractionStateType::INTERACTION_STATE_TYPE_UNSPECIFIED;
//   }
// }

// void toSubjectInteractive(cwspb::SubjectInteractive & out,
//                           const SubjectInteractive * subject) {
//   toSubject(*out.mutable_base(), subject);
//   out.set_interaction_state_type(
//       toInteractionStateType(subject->getInteractionState().type));
// }

// void toSubjectPlain(cwspb::SubjectPlain & out, const SubjectPlain * plain) {
//   toSubject(*out.mutable_base(), plain);
// }

// void toSubjectSensor(cwspb::SubjectSensor & out, const SubjectSensor * subject) {
//   toSubject(*out.mutable_base(), subject);
// }

// void toSensorTemperature(cwspb::SensorTemperature & out,
//                          const SensorTemperature * sensor) {
//   toSubjectSensor(*out.mutable_base(), sensor);
//   toTemperature(*out.mutable_temperature(), sensor->getTemperature());
// }

// From
SimulationStatus fromSimulationStatus(const cwspb::SimulationStatus status) {
  switch (status) {
  case cwspb::SimulationStatus::SIMULATION_STATUS_RUNNING:
    return SimulationStatus::RUNNING;
  case cwspb::SimulationStatus::SIMULATION_STATUS_STOPPED:
    return SimulationStatus::STOPPED;
  default:
    return SimulationStatus::STOPPED;
  }
}

SimulationType fromSimulationType(const cwspb::SimulationType type) {
  switch (type) {
  case cwspb::SimulationType::SIMULATION_TYPE_LIMITED:
    return SimulationType::LIMITED;
  case cwspb::SimulationType::SIMULATION_TYPE_INFINITE:
    return SimulationType::INFINITE;
  default:
    return SimulationType::LIMITED;
  }
}

SimulationStateIn fromSimulationState(const cwspb::SimulationState & in) {
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

// Coordinates fromCoordinates(const cwspb::Coordinates & coord) {
//   Coordinates out;
//   out.x = coord.x();
//   out.y = coord.y();
//   return out;
// }

// Temperature fromTemperature(const cwspb::Temperature & in) {
//   Temperature out;
//   out.value = in.value();
//   return out;
// }

// SubjectType fromSubjectType(cwspb::SubjectType in) {
//   switch (in) {
//   case cwspb::SubjectType::SUBJECT_TYPE_PLAIN:
//     return SubjectType::PLAIN;
//   case cwspb::SubjectType::SUBJECT_TYPE_INTERACTIVE:
//     return SubjectType::INTERACTIVE;
//   case cwspb::SubjectType::SUBJECT_TYPE_SENSOR:
//     return SubjectType::SENSOR;
//   case cwspb::SubjectType::SUBJECT_TYPE_UNSPECIFIED:
//   default:
//     return SubjectType::UNSPECIFIED;
//   }
// }

// SubjectType fromSubjectType(const cwspb::SubjectDerived & in) {
//   if (in.has_plain())
//     return SubjectType::PLAIN;
//   else if (in.has_interactive())
//     return SubjectType::INTERACTIVE;
//   else if (in.has_sensor())
//     return SubjectType::SENSOR;
//   else
//     return SubjectType::UNSPECIFIED;
// }

// Id fromSubjectId(const cwspb::SubjectId & id, cwspb::SubjectType type) {
//   Id out;
//   out.type = fromSubjectType(type);
//   out.idx = id.id();
//   return out;
// }

// SubjectParameters fromSubjectParameters(const cwspb::Subject & in) {
//   return {
//       // .weight = in.weight(),
//       // .heatCapacity = in.heat_capacity(),
//       // .heatTransmission = fromPercentage(in.heat_transmission()),
//       // .temperature = fromTemperature(in.temperature()),
//       // .lightTransmission = fromPercentage(in.light_transmission()),
//       // .humidityTransmission = fromPercentage(in.humidity_transmission()),
//   };
// }

// std::unique_ptr<Subject> fromSubjectDerived(const cwspb::SubjectDerived & in) {
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

// std::unique_ptr<Subject> fromSubject(const cwspb::Subject & in, SubjectType type) {
//   auto params = fromSubjectParameters(in);
//   auto id = fromSubjectId(in.id(), toSubjectType(type));
//   return std::make_unique<Subject>(id, params);
// }

// std::unique_ptr<SubjectPlain> fromSubjectPlain(const cwspb::SubjectPlain & in) {
//   auto idx = in.base().id().id();
//   auto params = fromSubjectParameters(in.base());
//   return std::make_unique<SubjectPlain>(idx, params);
// }

// InteractionStateType fromInteractionStateType(const cwspb::InteractionStateType type) {
//   switch (type) {
//   case cwspb::InteractionStateType::INTERACTION_STATE_TYPE_CLOSED:
//     return InteractionStateType::DISABLED;
//   case cwspb::InteractionStateType::INTERACTION_STATE_TYPE_OPENED:
//     return InteractionStateType::ENABLED;
//   case cwspb::InteractionStateType::INTERACTION_STATE_TYPE_UNSPECIFIED:
//   default:
//     return InteractionStateType::UNSPECIFIED;
//   }
// }

// std::unique_ptr<SubjectInteractive>
// fromSubjectInteractive(const cwspb::SubjectInteractive & in) {
//   auto idx = in.base().id().id();
//   auto params = fromSubjectParameters(in.base());
//   auto ist = fromInteractionStateType(in.interaction_state_type());
//   InteractiveState is{.type = ist};
//   return std::make_unique<SubjectInteractive>(idx, params, is);
// }

// SensorType fromSensorType(const cwspb::SubjectSensorDerived & in) {
//   if (in.has_temperature()) {
//     return SensorType::TEMPERATURE;
//   } else if (in.has_base()) {
//     return SensorType::UNSPECIFIED;
//   }
//   return SensorType::UNSPECIFIED;
// }

// std::unique_ptr<SubjectSensor>
// fromSubjectSensorDerived(const cwspb::SubjectSensorDerived & in) {
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
// std::unique_ptr<SubjectSensor> fromSubjectSensor(const cwspb::SubjectSensor & in,
//                                                  SensorType type) {
//   auto idx = in.base().id().id();
//   auto params = fromSubjectParameters(in.base());
//   return std::make_unique<SubjectSensor>(type, idx, params);
// }

// std::unique_ptr<SensorTemperature>
// fromSensorTemperature(const cwspb::SensorTemperature & in) {
//   auto idx = in.base().base().id().id();
//   auto params = fromSubjectParameters(in.base().base());
//   auto temp = fromTemperature(in.temperature());
//   return std::make_unique<SensorTemperature>(idx, params, temp);
// }

// SubjectQueryType fromSubjectQueryType(cwspb::SubjectQueryType type) {
//   switch (type) {
//   case cwspb::SubjectQueryType::SUBJECT_QUERY_TYPE_DELETE:
//     return SubjectQueryType::DELETE;
//   case cwspb::SubjectQueryType::SUBJECT_QUERY_TYPE_INSERT:
//     return SubjectQueryType::INSERT;
//   case cwspb::SubjectQueryType::SUBJECT_QUERY_TYPE_UPDATE:
//     return SubjectQueryType::UPDATE;
//   case cwspb::SubjectQueryType::SUBJECT_QUERY_TYPE_UNSPECIFIED:
//     // case cwspb::SubjectQueryType::SUBJECT_QUERY_TYPE_SELECT:
//     return SubjectQueryType::SELECT;
//   default:
//     return SubjectQueryType::UNSPECIFIED;
//   }
// }
