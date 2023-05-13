#pragma once

#include "converters.hpp"
#include "cws/simulation/interface.hpp"
#include "cwspb/service/general.pb.h"
#include "cwspb/service/map.grpc.pb.h"
#include "cwspb/service/map.pb.h"
#include <grpcpp/support/status.h>

class MapService final : public cws::MapService::Service {
private:
  SimulationInterface & interface;

private:
public:
  MapService(SimulationInterface & interface) : interface(interface) {}

  grpc::Status GetMapDimension(::grpc::ServerContext * context,
                               const cws::Request * request,
                               cws::ResponseDimension * response) override {
    auto map = interface.getMap();

    if (!verifyMapSet(map, *response->mutable_base())) {
      return grpc::Status::OK;
    }

    auto dimension = map->getDimension();

    auto rDimension = response->mutable_dimension();
    rDimension->set_width(dimension.width);
    rDimension->set_height(dimension.height);

    return grpc::Status::OK;
  }

  grpc::Status CreateMap(::grpc::ServerContext * context,
                         const cws::RequestDimension * request,
                         cws::Response * response) override {

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

  grpc::Status GetCell(::grpc::ServerContext * context,
                       const cws::RequestCell * request,
                       cws::ResponseCell * response) override {
    auto map = interface.getMap();

    if (!verifyMapSet(map, *response->mutable_base())) {
      return grpc::Status::OK;
    }

    auto dimension = map->getDimension();

    Coordinates coord = fromCoordinates(request->coordinates());

    if (!verifyCoordinates(coord, dimension, *response->mutable_base())) {
      return grpc::Status::OK;
    }

    const auto & layers = map->getLayers();
    const auto & tempLayer = layers.temperatureLayer;
    const auto & subjectLayer = layers.subjectLayer;

    const auto cell = Cell{.coordinates = coord,
                           .temp = tempLayer.getCell(coord).getElement(),
                           .subject = subjectLayer.getCell(coord).getElement()};

    toCell(*response->mutable_cell(), cell);
    return grpc::Status::OK;
  }

  grpc::Status GetMap(::grpc::ServerContext * context, const cws::Request * request,
                      grpc::ServerWriter<::cws::ResponseCell> * writer) override {
    grpc::WriteOptions options;

    auto map = interface.getMap();

    cws::ResponseCell response;

    if (!verifyMapSet(map, *response.mutable_base())) {
      writer->WriteLast(response, options);
      return grpc::Status::OK;
    }

    auto dimension = map->getDimension();

    const auto & layers = map->getLayers();
    const auto & tempLayer = layers.temperatureLayer;
    const auto & subjectLayer = layers.subjectLayer;

    for (int x = 0; x < dimension.width; ++x) {
      for (int y = 0; y < dimension.height; ++y) {
        cws::ResponseCell response;

        Coordinates coord{.x = x, .y = y};

        const auto cell = Cell{.coordinates = coord,
                               .temp = tempLayer.getCell(coord).getElement(),
                               .subject = subjectLayer.getCell(coord).getElement()};

        toCell(*response.mutable_cell(), cell);
        writer->Write(response);
      }
    }
    return grpc::Status::OK;
  }

  // grpc::Status GetSubject(::grpc::ServerContext * context,
  //                         const cws::RequestQuerySubject * request,
  //                         cws::ResponseSelectSubject * response) override {
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
  //     status->set_type(cws::ErrorType::ERROR_TYPE_BAD_REQUEST);
  //   } else {
  //     auto subject = response->mutable_derived();
  //     toSubjectDerived(*subject, res);
  //     auto coordinates = response->mutable_coordinates();
  //     toCoordinates(*coordinates, coord);
  //   }

  //   return grpc::Status::OK;
  // }

  // grpc::Status SetSubject(::grpc::ServerContext * context,
  //                         const cws::RequestQuerySubject * request,
  //                         cws::Response * response) override {

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
  bool verifyMapSet(const std::shared_ptr<const Map> & map, cws::Response & response) {
    bool bad = !map;

    if (bad) {
      auto resStatus = response.mutable_status();
      resStatus->set_text("map is not created");
      resStatus->set_type(cws::ErrorType::ERROR_TYPE_BAD_REQUEST);
    }
    return !bad;
  }

  bool verifyCoordinates(const Coordinates & coord, const Dimension & dimension,
                         cws::Response & response) {
    bool bad = (coord.x >= dimension.width || coord.x < 0 ||
                coord.y >= dimension.height || coord.y < 0);

    if (bad) {
      auto resStatus = response.mutable_status();
      resStatus->set_text("coordinates out of bounds");
      resStatus->set_type(cws::ErrorType::ERROR_TYPE_BAD_REQUEST);
    }
    return !bad;
  }
};
