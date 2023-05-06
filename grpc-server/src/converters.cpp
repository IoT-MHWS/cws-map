#include "converters.hpp"
#include "cws/general.hpp"
#include "cws/layer/subject.hpp"
#include "cws/layer/temperature.hpp"
#include "cwspb/general.pb.h"
#include "cwspb/layer/temperature.pb.h"
#include "cwspb/subject/interactive.pb.h"

void toSimulationState(cws::SimulationState & out, const struct SimulationState & in) {
  out.set_status(toSimulationStatus(in.status));
  out.set_type(toSimulationType(in.type));
  out.set_task_frequency(in.taskFrequency);
  out.set_current_tick(in.currentTick);
  out.set_last_tick(in.lastTick);
}

void fromSimulationState(SimulationStateIn & out, const cws::SimulationState & in) {
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
}

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

void toCell(cws::Cell & out, const Cell & cell) {
  toCoordinates(*out.mutable_coordinates(), cell.coordinates);
  toLayerSubject(*out.mutable_subject(), cell.subject);
  toLayerTemperature(*out.mutable_temperature(), cell.temp);
}

void toCoordinates(cws::Coordinates & out, const Coordinates & coord) {
  out.set_x(coord.x);
  out.set_y(coord.y);
}

void fromCoordinates(Coordinates & out, const cws::Coordinates & coord) {
  out.x = coord.x();
  out.y = coord.y();
}

void toTemperature(cws::Temperature & out, const Temperature & temp) {
  out.set_value(temp.value);
}

void toPercentage(cws::Percentage & out, const Percentage & percentage) {
  out.set_value(percentage.value);
}

void toLayer(cws::Layer & out, const Layer & layer) {}

void toLayerSubject(cws::LayerSubject & out, const LayerSubject & layerSubject) {

  std::cout << "TODO: LayerSubject conversion is not defined" << std::endl;
}

void toLayerTemperature(cws::LayerTemperature & out,
                        const LayerTemperature & layerTemperature) {
  toLayer(*out.mutable_base(), static_cast<const Layer &>(layerTemperature));
  toTemperature(*out.mutable_temperature(), layerTemperature.getTemperature());
}

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

void toSubjectInteractive(cws::SubjectInteractive & out,
                          const SubjectInteractive * subject) {
  toSubject(*out.mutable_base(), subject);
  out.set_interaction_state_type(
      toInteractionStateType(subject->getInteractionState().type));
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

void toSubjectPlain(cws::SubjectPlain & out, const SubjectPlain * plain) {
  toSubject(*out.mutable_base(), plain);
}

void toSubjectSensorDerived(cws::SubjectSensorDerived & out,
                            const SubjectSensor * subject) {
  switch (subject->sensorType) {
  case SensorType::TEMPERATURE:
    return toSensorTemperature(*out.mutable_temperature(),
                               static_cast<const SensorTemperature *>(subject));
  default:
    return toSubjectSensor(*out.mutable_base(), subject);
  }
}

void toSubjectSensor(cws::SubjectSensor & out, const SubjectSensor * subject) {
  toSubject(*out.mutable_base(), subject);
}

void toSensorTemperature(cws::SensorTemperature & out,
                         const SensorTemperature * sensor) {
  toSubjectSensor(*out.mutable_base(), sensor);
  toTemperature(*out.mutable_temperature(), sensor->sensorTemp);
}
