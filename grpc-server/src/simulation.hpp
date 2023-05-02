#pragma once

#include "cwspb/service/simulation.grpc.pb.h"
#include <grpcpp/support/status.h>

class SimulationService final : public cws::SimulationService::Service {
  ::grpc::Status
  GetSimulationStatus(::grpc::ServerContext * context, const ::cws::Request * request,
                      ::cws::ResponseSimulationStatus * response) override {
    return grpc::Status::OK;
  }

  ::grpc::Status SetSimulationState(::grpc::ServerContext * context,
                                    const ::cws::RequestSimulationState * request,
                                    ::cws::Response * response) override {
    return grpc::Status::OK;
  }

  ::grpc::Status
  GetSimulationState(::grpc::ServerContext * context, const ::cws::Request * request,
                     ::cws::ResponseSimulationState * response) override {
    return grpc::Status::OK;
  }

  ::grpc::Status SetSimulationStatus(::grpc::ServerContext * context,
                                     const ::cws::RequestSimulationStatus * request,
                                     ::cws::Response * response) override {
    return grpc::Status::OK;
  }
};
