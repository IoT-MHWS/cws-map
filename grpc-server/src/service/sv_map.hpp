#pragma once

#include "converters.hpp"
#include "cws/simulation/interface.hpp"
#include "cwspb/service/sv_map.grpc.pb.h"
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

    auto status = response->mutable_status();

    if (!request->has_dimension()) {
      status->set_text("dimension is not specified");
      status->set_type(cwspb::ErrorType::ERROR_TYPE_BAD_REQUEST);
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

  // grpc::Status GetSubject(::grpc::ServerContext * context,
  //                         const cwspb::RequestQuerySubject * request,
  //                         cwspb::ResponseSelectSubject * response) override {
  //   auto map = interface.getMap();

  //   if (!verifyMapSet(map, *response->mutable_base())) {
  //     return grpc::Status::OK;
  //   }

  //   auto queryType = fromSubjectQueryType(request->query_type());

  //   auto dimension = map->getDimension();

  //   Coordinates coord = fromCoordinates(request->coordinates());

  //   if (!verifyCoordinates(coord, dimension, *response->mutable_base())) {
  //     return grpc::Status::OK;
  //   }

  //   SubjectQuery query(queryType, coord, fromSubjectDerived(request->subject()));
  //   auto res = map->getQuery(std::move(query));

  //   if (res == nullptr) {
  //     auto respBase = response->mutable_base();
  //     auto status = respBase->mutable_status();
  //     status->set_text("element doesn't exist");
  //     status->set_type(cwspb::ErrorType::ERROR_TYPE_BAD_REQUEST);
  //   } else {
  //     auto subject = response->mutable_derived();
  //     toSubjectDerived(*subject, res);
  //     auto coordinates = response->mutable_coordinates();
  //     toCoordinates(*coordinates, coord);
  //   }

  //   return grpc::Status::OK;
  // }

  // grpc::Status SetSubject(::grpc::ServerContext * context,
  //                         const cwspb::RequestQuerySubject * request,
  //                         cwspb::Response * response) override {

  //   auto map = interface.getMap();

  //   if (!verifyMapSet(map, *response)) {
  //     return grpc::Status::OK;
  //   }

  //   SubjectQueryType queryType = fromSubjectQueryType(request->query_type());
  //   Coordinates coordinates = fromCoordinates(request->coordinates());

  //   if (!verifyCoordinates(coordinates, map->getDimension(), *response)) {
  //     return grpc::Status::OK;
  //   }

  //   auto subject = fromSubjectDerived(request->subject());

  //   interface.addQuerySet(
  //       std::make_unique<SubjectQuery>(queryType, coordinates, std::move(subject)));

  //   return grpc::Status::OK;
  // }

private:
  bool verifyMapCreated(const std::shared_ptr<const Map> & map,
                        cwspb::Response & response) {
    bool bad = !map;
    auto status = response.mutable_status();

    if (bad) {
      status->set_text("map is not created");
      status->set_type(cwspb::ErrorType::ERROR_TYPE_BAD_REQUEST);
    }
    return !bad;
  }

  bool verifyCoordinates(const Coordinates & coord, const Dimension & dimension,
                         cwspb::Response & response) {
    bool bad = (coord.x >= dimension.width || coord.x < 0 ||
                coord.y >= dimension.height || coord.y < 0);
    auto status = response.mutable_status();

    if (bad) {
      status->set_text("coordinates out of bounds");
      status->set_type(cwspb::ErrorType::ERROR_TYPE_BAD_REQUEST);
    }
    return !bad;
  }
};
