#include "converters.hpp"

// To
cws::SimulationStatus toSimulationStatus(const SimulationStatus status) {
  switch (status) {
  case SimulationStatus::RUNNING:
    return cws::SimulationStatus::SIMULATION_STATUS_RUNNING;
  case SimulationStatus::STOPPED:
    return cws::SimulationStatus::SIMULATION_STATUS_STOPPED;
  default:
    return cws::SimulationStatus::SIMULATION_STATUS_UNSPECIFIED;
  }
}

cws::SimulationType toSimulationType(const SimulationType type) {
  switch (type) {
  case SimulationType::LIMITED:
    return cws::SimulationType::SIMULATION_TYPE_LIMITED;
  case SimulationType::INFINITE:
    return cws::SimulationType::SIMULATION_TYPE_INFINITE;
  default:
    return cws::SimulationType::SIMULATION_TYPE_UNSPECIFIED;
  }
}

void toSimulationState(cws::SimulationState & out, const struct SimulationState & in) {
  out.set_status(toSimulationStatus(in.status));
  out.set_type(toSimulationType(in.type));
  out.set_task_frequency(in.taskFrequency);
  out.set_current_tick(in.currentTick);
  out.set_last_tick(in.lastTick);
}

void toCell(cws::Cell & out, const Cell & cell) {
  toCoordinates(*out.mutable_coordinates(), cell.coordinates);
  toLayerSubject(*out.mutable_subject(), cell.subject);
  toLayerTemperature(*out.mutable_temperature(), cell.temp);
}

void toCoordinates(cws::Coordinates & out, const Coordinates & coord) {
  out.set_x(coord.x);
  out.set_y(coord.y);
}

void toTemperature(cws::Temperature & out, const Temperature & temp) {
  out.set_value(temp.value);
}

void toPercentage(cws::Percentage & out, const Percentage & percentage) {
  out.set_value(percentage.value);
}

void toLayer(cws::Layer & out, const Layer & layer) {}

void toLayerSubject(cws::LayerSubject & out, const LayerSubject & layerSubject) {
  toLayer(*out.mutable_base(), static_cast<const Layer &>(layerSubject));
  for (const auto & subject : layerSubject.getSubjectList()) {
    auto outSubject = out.add_subjects();
    toSubjectDerived(*outSubject, subject.get());
  }
}

void toLayerTemperature(cws::LayerTemperature & out,
                        const LayerTemperature & layerTemperature) {
  toLayer(*out.mutable_base(), static_cast<const Layer &>(layerTemperature));
  toTemperature(*out.mutable_temperature(), layerTemperature.getTemperature());
}

cws::SubjectType toSubjectType(SubjectType in) {
  switch (in) {
  case SubjectType::PLAIN:
    return cws::SubjectType::SUBJECT_TYPE_PLAIN;
  case SubjectType::INTERACTIVE:
    return cws::SubjectType::SUBJECT_TYPE_INTERACTIVE;
  case SubjectType::SENSOR:
    return cws::SubjectType::SUBJECT_TYPE_SENSOR;
  case SubjectType::UNSPECIFIED:
  default:
    return cws::SubjectType::SUBJECT_TYPE_UNSPECIFIED;
  }
}

void toSubjectId(cws::SubjectId & out, const SubjectId & id) { out.set_id(id.idx); }

void toSubjectDerived(cws::SubjectDerived & out, const Subject * subject) {
  switch (subject->getSubjectId().type) {
  case SubjectType::PLAIN:
    return toSubjectPlain(*out.mutable_plain(),
                          static_cast<const SubjectPlain *>(subject));
  case SubjectType::SENSOR:
    return toSubjectSensorDerived(*out.mutable_sensor(),
                                  static_cast<const SubjectSensor *>(subject));
  case SubjectType::INTERACTIVE:
    return toSubjectInteractive(*out.mutable_interactive(),
                                static_cast<const SubjectInteractive *>(subject));
  case SubjectType::UNSPECIFIED:
  default:
    return toSubject(*out.mutable_base(), subject);
  }
}

void toSubject(cws::Subject & out, const Subject * subject) {
  toSubjectId(*out.mutable_id(), subject->getSubjectId());

  auto params = subject->getSubjectParameters();
  out.set_weight(params.weight);
  out.set_heat_capacity(params.heatCapacity);
  toPercentage(*out.mutable_heat_transmission(), params.heatTransmission);
  toTemperature(*out.mutable_temperature(), params.temperature);
  toPercentage(*out.mutable_light_transmission(), params.lightTransmission);
  toPercentage(*out.mutable_humidity_transmission(), params.humidityTransmission);
}

void toSubjectSensorDerived(cws::SubjectSensorDerived & out,
                            const SubjectSensor * subject) {
  switch (subject->getSensorType()) {
  case SensorType::TEMPERATURE:
    return toSensorTemperature(*out.mutable_temperature(),
                               static_cast<const SensorTemperature *>(subject));
  default:
    return toSubjectSensor(*out.mutable_base(), subject);
  }
}

cws::InteractionStateType toInteractionStateType(const InteractionStateType type) {
  switch (type) {
  case InteractionStateType::OPENED:
    return cws::InteractionStateType::INTERACTION_STATE_TYPE_OPENED;
  case InteractionStateType::CLOSED:
    return cws::InteractionStateType::INTERACTION_STATE_TYPE_CLOSED;
  case InteractionStateType::UNSPECIFIED:
  default:
    return cws::InteractionStateType::INTERACTION_STATE_TYPE_UNSPECIFIED;
  }
}

void toSubjectInteractive(cws::SubjectInteractive & out,
                          const SubjectInteractive * subject) {
  toSubject(*out.mutable_base(), subject);
  out.set_interaction_state_type(
      toInteractionStateType(subject->getInteractionState().type));
}

void toSubjectPlain(cws::SubjectPlain & out, const SubjectPlain * plain) {
  toSubject(*out.mutable_base(), plain);
}

void toSubjectSensor(cws::SubjectSensor & out, const SubjectSensor * subject) {
  toSubject(*out.mutable_base(), subject);
}

void toSensorTemperature(cws::SensorTemperature & out,
                         const SensorTemperature * sensor) {
  toSubjectSensor(*out.mutable_base(), sensor);
  toTemperature(*out.mutable_temperature(), sensor->getTemperature());
}

// From
SimulationStatus fromSimulationStatus(const cws::SimulationStatus status) {
  switch (status) {
  case cws::SimulationStatus::SIMULATION_STATUS_RUNNING:
    return SimulationStatus::RUNNING;
  case cws::SimulationStatus::SIMULATION_STATUS_STOPPED:
    return SimulationStatus::STOPPED;
  default:
    return SimulationStatus::STOPPED;
  }
}

SimulationType fromSimulationType(const cws::SimulationType type) {
  switch (type) {
  case cws::SimulationType::SIMULATION_TYPE_LIMITED:
    return SimulationType::LIMITED;
  case cws::SimulationType::SIMULATION_TYPE_INFINITE:
    return SimulationType::INFINITE;
  default:
    return SimulationType::LIMITED;
  }
}

SimulationStateIn fromSimulationState(const cws::SimulationState & in) {
  SimulationStateIn out;
  if (in.has_status())
    out.simStatus.set(fromSimulationStatus(in.status()));
  if (in.has_type())
    out.simType.set(fromSimulationType(in.type()));
  if (in.has_current_tick())
    out.currentTick.set(in.current_tick());
  if (in.has_last_tick())
    out.lastTick.set(in.last_tick());
  if (in.has_task_frequency())
    out.taskFrequency.set(in.task_frequency());
  return out;
}

Coordinates fromCoordinates(const cws::Coordinates & coord) {
  Coordinates out;
  out.x = coord.x();
  out.y = coord.y();
  return out;
}

Temperature fromTemperature(const cws::Temperature & in) {
  Temperature out;
  out.value = in.value();
  return out;
}

Percentage fromPercentage(const cws::Percentage & in) {
  Percentage out;
  out.value = in.value();
  return out;
}

SubjectType fromSubjectType(cws::SubjectType in) {
  switch (in) {
  case cws::SubjectType::SUBJECT_TYPE_PLAIN:
    return SubjectType::PLAIN;
  case cws::SubjectType::SUBJECT_TYPE_INTERACTIVE:
    return SubjectType::INTERACTIVE;
  case cws::SubjectType::SUBJECT_TYPE_SENSOR:
    return SubjectType::SENSOR;
  case cws::SubjectType::SUBJECT_TYPE_UNSPECIFIED:
  default:
    return SubjectType::UNSPECIFIED;
  }
}

SubjectType fromSubjectType(const cws::SubjectDerived & in) {
  if (in.has_plain())
    return SubjectType::PLAIN;
  else if (in.has_interactive())
    return SubjectType::INTERACTIVE;
  else if (in.has_sensor())
    return SubjectType::SENSOR;
  else
    return SubjectType::UNSPECIFIED;
}

SubjectId fromSubjectId(const cws::SubjectId & id, cws::SubjectType type) {
  SubjectId out;
  out.type = fromSubjectType(type);
  out.idx = id.id();
  return out;
}

SubjectParameters fromSubjectParameters(const cws::Subject & in) {
  return {
      .weight = in.weight(),
      .heatCapacity = in.heat_capacity(),
      .heatTransmission = fromPercentage(in.heat_transmission()),
      .temperature = fromTemperature(in.temperature()),
      .lightTransmission = fromPercentage(in.light_transmission()),
      .humidityTransmission = fromPercentage(in.humidity_transmission()),
  };
}

std::unique_ptr<Subject> fromSubjectDerived(const cws::SubjectDerived & in) {
  SubjectType outST = fromSubjectType(in);
  switch (outST) {
  case SubjectType::PLAIN:
    return fromSubjectPlain(in.plain());
  case SubjectType::INTERACTIVE:
    return fromSubjectInteractive(in.interactive());
  case SubjectType::SENSOR:
    return fromSubjectSensorDerived(in.sensor());
  case SubjectType::UNSPECIFIED:
  default:
    if (in.has_base()) {
      return fromSubject(in.base(), outST);
    } else {
      return std::unique_ptr<Subject>();
    }
  }
}

std::unique_ptr<Subject> fromSubject(const cws::Subject & in, SubjectType type) {
  auto params = fromSubjectParameters(in);
  auto id = fromSubjectId(in.id(), toSubjectType(type));
  return std::make_unique<Subject>(id, params);
}

std::unique_ptr<SubjectPlain> fromSubjectPlain(const cws::SubjectPlain & in) {
  auto idx = in.base().id().id();
  auto params = fromSubjectParameters(in.base());
  return std::make_unique<SubjectPlain>(idx, params);
}

InteractionStateType fromInteractionStateType(const cws::InteractionStateType type) {
  switch (type) {
  case cws::InteractionStateType::INTERACTION_STATE_TYPE_CLOSED:
    return InteractionStateType::CLOSED;
  case cws::InteractionStateType::INTERACTION_STATE_TYPE_OPENED:
    return InteractionStateType::OPENED;
  case cws::InteractionStateType::INTERACTION_STATE_TYPE_UNSPECIFIED:
  default:
    return InteractionStateType::UNSPECIFIED;
  }
}

std::unique_ptr<SubjectInteractive>
fromSubjectInteractive(const cws::SubjectInteractive & in) {
  auto idx = in.base().id().id();
  auto params = fromSubjectParameters(in.base());
  auto ist = fromInteractionStateType(in.interaction_state_type());
  InteractiveState is{.type = ist};
  return std::make_unique<SubjectInteractive>(idx, params, is);
}

SensorType fromSensorType(const cws::SubjectSensorDerived & in) {
  if (in.has_temperature()) {
    return SensorType::TEMPERATURE;
  } else if (in.has_base()) {
    return SensorType::UNSPECIFIED;
  }
  return SensorType::UNSPECIFIED;
}

std::unique_ptr<SubjectSensor>
fromSubjectSensorDerived(const cws::SubjectSensorDerived & in) {
  SensorType type = fromSensorType(in);
  switch (type) {
  case SensorType::TEMPERATURE:
    return fromSensorTemperature(in.temperature());
  case SensorType::UNSPECIFIED:
  default:
    if (in.has_base()) {
      return fromSubjectSensor(in.base(), type);
    } else {
      return std::unique_ptr<SubjectSensor>();
    }
  }
}
std::unique_ptr<SubjectSensor> fromSubjectSensor(const cws::SubjectSensor & in,
                                                 SensorType type) {
  auto idx = in.base().id().id();
  auto params = fromSubjectParameters(in.base());
  return std::make_unique<SubjectSensor>(type, idx, params);
}

std::unique_ptr<SensorTemperature>
fromSensorTemperature(const cws::SensorTemperature & in) {
  auto idx = in.base().base().id().id();
  auto params = fromSubjectParameters(in.base().base());
  auto temp = fromTemperature(in.temperature());
  return std::make_unique<SensorTemperature>(idx, params, temp);
}

SubjectQueryType fromSubjectQueryType(cws::SubjectQueryType type) {
  switch (type) {
  case cws::SubjectQueryType::SUBJECT_QUERY_TYPE_DELETE:
    return SubjectQueryType::DELETE;
  case cws::SubjectQueryType::SUBJECT_QUERY_TYPE_INSERT:
    return SubjectQueryType::INSERT;
  case cws::SubjectQueryType::SUBJECT_QUERY_TYPE_UPDATE:
    return SubjectQueryType::UPDATE;
  case cws::SubjectQueryType::SUBJECT_QUERY_TYPE_UNSPECIFIED:
  case cws::SubjectQueryType::SUBJECT_QUERY_TYPE_SELECT:
    return SubjectQueryType::SELECT;
  default:
    return SubjectQueryType::UNSPECIFIED;
  }
}
