#pragma once

#include "converters.hpp"
#include "cws/simulation/interface.hpp"
#include "cwspb/service/sv_map.grpc.pb.h"
#include "service/verify.hpp"
#include <grpcpp/support/status.h>

class MapService final : public cwspb::MapService::Service {
private:
  SimulationInterface & interface;

private:
public:
  MapService(SimulationInterface & interface) : interface(interface) {}

  grpc::Status GetMapDimension(::grpc::ServerContext * context,
                               const cwspb::Request * request,
                               cwspb::ResponseDimension * response) override {
    auto map = interface.getMap();
    if (!verifyMapCreated(map, *response->mutable_base())) {
      return grpc::Status::OK;
    }

    auto dim = map->getDimension();
    auto out_dim = response->mutable_dimension();
    toDimension(*out_dim, dim);
    return grpc::Status::OK;
  }

  grpc::Status CreateMap(::grpc::ServerContext * context,
                         const cwspb::RequestDimension * request,
                         cwspb::Response * response) override {

    auto & status = *response->mutable_status();

    if (!request->has_dimension()) {
      status.set_text("dimension is not specified");
      status.set_type(cwspb::ErrorType::ERROR_TYPE_BAD_REQUEST);
      return grpc::Status::OK;
    }

    Dimension dimension = fromDimension(request->dimension());
    interface.setDimension(dimension);

    return grpc::Status::OK;
  }

  grpc::Status GetCell(::grpc::ServerContext * context,
                       const cwspb::RequestCell * request,
                       cwspb::ResponseCell * response) override {
    auto map = interface.getMap();
    if (!verifyMapCreated(map, *response->mutable_base())) {
      return grpc::Status::OK;
    }

    auto dimension = map->getDimension();

    Coordinates coord = fromCoordinates(request->coordinates());
    if (!verifyCoordinates(coord, dimension, *response->mutable_base())) {
      return grpc::Status::OK;
    }

    const auto & layers = map->getLayers();
    toCell(*response->mutable_cell(), map->getLayers(), coord);

    return grpc::Status::OK;
  }

  grpc::Status GetMap(::grpc::ServerContext * context, const cwspb::Request * request,
                      ::cwspb::ResponseMap * response) override {
    grpc::WriteOptions options;

    auto map = interface.getMap();
    if (!verifyMapCreated(map, *response->mutable_base())) {
      return grpc::Status::OK;
    }

    auto dim = map->getDimension();
    const auto & layers = map->getLayers();

    toMap(*response->mutable_map(), layers, dim);
    return grpc::Status::OK;
  }

  grpc::Status
  GetMapCells(::grpc::ServerContext * context, const cwspb::Request * request,
              grpc::ServerWriter<::cwspb::ResponseCell> * writer) override {
    grpc::WriteOptions options;
    cwspb::ResponseCell response;

    auto map = interface.getMap();
    if (!verifyMapCreated(map, *response.mutable_base())) {
      writer->WriteLast(response, options);
      return grpc::Status::OK;
    }

    auto dimension = map->getDimension();
    const auto & layers = map->getLayers();

    Coordinates c;
    for (c.x = 0; c.x < dimension.width; ++c.x) {
      for (c.y = 0; c.y < dimension.height; ++c.y) {
        cwspb::ResponseCell response;
        toCell(*response.mutable_cell(), layers, c);
        writer->Write(response);
      }
    }
    return grpc::Status::OK;
  }

  grpc::Status GetSubject(::grpc::ServerContext * context,
                          const cwspb::RequestSelectSubject * request,
                          cwspb::ResponseSelectSubject * response) override {
    auto & respBase = *response->mutable_base();

    auto map = interface.getMap();
    if (!verifyMapCreated(map, respBase)) {
      return grpc::Status::OK;
    }

    auto dimension = map->getDimension();

    Coordinates coord = fromCoordinates(request->id().coordinates());
    if (!verifyCoordinates(coord, dimension, respBase)) {
      return grpc::Status::OK;
    }

    Subject::Id id = fromSubjectId(request->id().id());

    SubjectSelectQuery query(coord, id);
    auto res = map->select(std::move(query));

    if (res == nullptr) {
      auto status = respBase.mutable_status();
      status->set_text("subject doesn't exist");
      status->set_type(cwspb::ErrorType::ERROR_TYPE_BAD_REQUEST);
    } else {
      toSubjectAny(*response->mutable_subject(), *res);
    }

    return grpc::Status::OK;
  }

  grpc::Status SetSubject(::grpc::ServerContext * context,
                          const cwspb::RequestModifySubject * request,
                          cwspb::Response * response) override {

    auto map = interface.getMap();
    if (!verifyMapCreated(map, *response)) {
      return grpc::Status::OK;
    }

    SubjectModifyType queryType = fromSubjectModifyType(request->modify_type());

    Subject::Id id;
    Coordinates coordinates;
    fromSubjectId(id, coordinates, request->id());

    if (!verifyCoordinates(coordinates, map->getDimension(), *response)) {
      return grpc::Status::OK;
    }

    auto subject = fromSubjectAny(request->subject());
    interface.addModifyQuery(
        SubjectModifyQuery(queryType, coordinates, std::move(subject)));

    return grpc::Status::OK;
  }

  grpc::Status GetAir(::grpc::ServerContext * context,
                      const cwspb::RequestSelectAir * request,
                      cwspb::ResponseSelectAir * response) override {
    auto & respBase = *response->mutable_base();

    auto map = interface.getMap();
    if (!verifyMapCreated(map, respBase)) {
      return grpc::Status::OK;
    }

    auto dimension = map->getDimension();

    Coordinates coord = fromCoordinates(request->id().coordinates());
    if (!verifyCoordinates(coord, dimension, respBase)) {
      return grpc::Status::OK;
    }

    Air::Id id = fromAirId(request->id().id());

    AirSelectQuery query(coord, id);
    auto res = map->select(std::move(query));

    if (res == nullptr) {
      auto status = respBase.mutable_status();
      status->set_text("subject doesn't exist");
      status->set_type(cwspb::ErrorType::ERROR_TYPE_BAD_REQUEST);
    } else {
      toAirPlain(*response->mutable_air(), *res);
    }

    return grpc::Status::OK;
  }

  grpc::Status InsertAir(::grpc::ServerContext * context,
                         const cwspb::RequestInsertAir * request,
                         cwspb::Response * response) override {

    auto map = interface.getMap();
    if (!verifyMapCreated(map, *response)) {
      return grpc::Status::OK;
    }

    Coordinates coordinates = fromCoordinates(request->coordinates());
    if (!verifyCoordinates(coordinates, map->getDimension(), *response)) {
      return grpc::Status::OK;
    }

    auto air = fromAirPlain(request->air());
    std::cout << air->getId() << std::endl;

    interface.addModifyQuery(AirInsertQuery(coordinates, std::move(air)));

    return grpc::Status::OK;
  }

private:
};
