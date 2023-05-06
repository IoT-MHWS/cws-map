#pragma once

#include "cws/simulation/interface.hpp"
#include "cwspb/service/map.grpc.pb.h"
#include "cwspb/service/map.pb.h"
#include <grpcpp/support/status.h>

class MapService final : public cws::MapService::Service {
private:
  SimulationInterface & interface;

public:
  MapService(SimulationInterface & interface) : interface(interface) {}

  ::grpc::Status GetMapDimension(::grpc::ServerContext * context,
                                 const ::cws::Request * request,
                                 ::cws::ResponseDimension * response) override {
    auto dimension = interface.getDimension();

    if (!dimension.isSet()) {
      auto res = response->mutable_base();
      auto resStatus = res->mutable_status();
      resStatus->set_text("map is not created");
      resStatus->set_type(cws::ErrorType::ERROR_TYPE_BAD_REQUEST);
      return grpc::Status::OK;
    }

    auto rDimension = response->mutable_dimension();
    rDimension->set_width(dimension.get().width);
    rDimension->set_height(dimension.get().height);

    return grpc::Status::OK;
  }

  ::grpc::Status CreateMap(::grpc::ServerContext * context,
                           const ::cws::RequestDimension * request,
                           ::cws::Response * response) override {

    if (!request->has_dimension()) {
      auto rStatus = response->mutable_status();
      rStatus->set_text("dimension is not specified");
      rStatus->set_type(cws::ErrorType::ERROR_TYPE_BAD_REQUEST);
      return grpc::Status::OK;
    }
    Dimension dimension{
        .width = request->dimension().width(),
        .height = request->dimension().height(),
    };

    interface.setDimension(dimension);

    return grpc::Status::OK;
  }

  ::grpc::Status GetCell(::grpc::ServerContext * context,
                         const ::cws::RequestCell * request,
                         ::cws::ResponseCell * response) override {

    return grpc::Status::OK;
  }

  ::grpc::Status GetMap(::grpc::ServerContext * context, const ::cws::Request * request,
                        ::grpc::ServerWriter<::cws::ResponseCell> * writer) override {

    return grpc::Status::OK;
  }
};
