#pragma once

#include "cws/simulation/interface.hpp"
#include "cws/simulation/state.hpp"
#include "cwspb/service/simulation.grpc.pb.h"
#include <grpcpp/support/status.h>

class SimulationService final : public cws::SimulationService::Service {
private:
  SimulationInterface & interface;

public:
  SimulationService(SimulationInterface & simulation) : interface(simulation) {}

  ::grpc::Status
  GetSimulationState(::grpc::ServerContext * context, const ::cws::Request * request,
                     ::cws::ResponseSimulationState * response) override {
    auto stateI = interface.getState();

    auto state = response->mutable_state();
    toProtoState(*state, stateI);

    // Response
    auto base = response->mutable_base();
    auto status = base->mutable_status();

    return grpc::Status::OK;
  }

  ::grpc::Status SetSimulationState(::grpc::ServerContext * context,
                                    const ::cws::RequestSimulationState * request,
                                    ::cws::Response * response) override {
    const auto & stateI = request->state();

    SimulationStateIn state;
    fromProtoState(state, stateI);

    interface.setState(state);

    return grpc::Status::OK;
  }

private:
  void toProtoState(cws::SimulationState & out, const struct SimulationState & in) {
    out.set_status(toProtoStatus(in.status));
    out.set_type(toProtoType(in.type));
    out.set_task_frequency(in.taskFrequency);
    out.set_current_tick(in.currentTick);
    out.set_last_tick(in.lastTick);
  }

  void fromProtoState(SimulationStateIn & out, const cws::SimulationState & in) {
    if (in.has_status())
      out.simStatus.set(fromProtoStatus(in.status()));
    if (in.has_type())
      out.simType.set(fromProtoType(in.type()));
    if (in.has_current_tick())
      out.currentTick.set(in.current_tick());
    if (in.has_last_tick())
      out.lastTick.set(in.last_tick());
    if (in.has_task_frequency())
      out.taskFrequency.set(in.task_frequency());
  }

  cws::SimulationStatus toProtoStatus(const SimulationStatus status) {
    switch (status) {
    case SimulationStatus::RUNNING:
      return cws::SimulationStatus::SIMULATION_STATUS_RUNNING;
    case SimulationStatus::STOPPED:
      return cws::SimulationStatus::SIMULATION_STATUS_STOPPED;
    default:
      return cws::SimulationStatus::SIMULATION_STATUS_UNSPECIFIED;
    }
  }

  SimulationStatus fromProtoStatus(const cws::SimulationStatus status) {
    switch (status) {
    case cws::SimulationStatus::SIMULATION_STATUS_RUNNING:
      return SimulationStatus::RUNNING;
    case cws::SimulationStatus::SIMULATION_STATUS_STOPPED:
      return SimulationStatus::STOPPED;
    default:
      return SimulationStatus::STOPPED;
    }
  }

  cws::SimulationType toProtoType(const SimulationType type) {
    switch (type) {
    case SimulationType::LIMITED:
      return cws::SimulationType::SIMULATION_TYPE_LIMITED;
    case SimulationType::INFINITE:
      return cws::SimulationType::SIMULATION_TYPE_INFINITE;
    default:
      return cws::SimulationType::SIMULATION_TYPE_UNSPECIFIED;
    }
  }

  SimulationType fromProtoType(const cws::SimulationType type) {
    switch (type) {
    case cws::SimulationType::SIMULATION_TYPE_LIMITED:
      return SimulationType::LIMITED;
    case cws::SimulationType::SIMULATION_TYPE_INFINITE:
      return SimulationType::INFINITE;
    default:
      return SimulationType::LIMITED;
    }
  }
};
