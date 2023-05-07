#pragma once

#include "converters.hpp"
#include "cws/simulation/interface.hpp"
#include "cws/simulation/state.hpp"
#include "cwspb/service/simulation.grpc.pb.h"
#include <grpcpp/support/status.h>

class SimulationService final : public cws::SimulationService::Service {
private:
  SimulationInterface & interface;

public:
  SimulationService(SimulationInterface & simulation) : interface(simulation) {}

  grpc::Status GetSimulationState(::grpc::ServerContext * context,
                                  const cws::Request * request,
                                  cws::ResponseSimulationState * response) override {
    auto stateI = interface.getState();

    auto state = response->mutable_state();
    toSimulationState(*state, stateI);

    // Response
    auto base = response->mutable_base();
    auto status = base->mutable_status();

    return grpc::Status::OK;
  }

  grpc::Status SetSimulationState(::grpc::ServerContext * context,
                                  const cws::RequestSimulationState * request,
                                  cws::Response * response) override {
    const auto & stateI = request->state();

    SimulationStateIn state = fromSimulationState(stateI);

    interface.setState(state);

    return grpc::Status::OK;
  }

private:
};
