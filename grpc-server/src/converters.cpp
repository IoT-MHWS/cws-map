#include "converters.hpp"
#include "cws/layer/subject.hpp"
#include "cws/layer/temperature.hpp"
#include "cwspb/layer/temperature.pb.h"

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

void toLayer(cws::Layer & out, const Layer & layer) {}

void toLayerSubject(cws::LayerSubject & out, const LayerSubject & layerSubject) {
  std::cout << "TODO: LayerSubject conversion is not defined" << std::endl;
}

void toLayerTemperature(cws::LayerTemperature & out,
                        const LayerTemperature & layerTemperature) {
  toLayer(*out.mutable_base(), static_cast<const Layer &>(layerTemperature));
  toTemperature(*out.mutable_temperature(), layerTemperature.getTemperature());
}

void toTemperature(cws::Temperature & out, const Temperature & temp) {
  out.set_value(temp.value);
}

void toSubject(cws::SubjectDerived & out, const Subject & subject) {
  std::cout << "TODO: Subject conversion is not defined" << std::endl;
}
