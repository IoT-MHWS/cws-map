#pragma once

#include "converters.hpp"
#include "cws/simulation/interface.hpp"
#include "cws/subject/camera.hpp"
#include "cws/subject/network.hpp"
#include "cws/subject/sensor.hpp"
#include "cwspb/service/sv_device.grpc.pb.h"
#include "service/sv_device_cb.hpp"
#include "service/verify.hpp"
#include <grpcpp/support/status.h>

class DeviceService final : public cwspb::DeviceService::Service {
private:
  SimulationInterface & interface;

public:
  DeviceService(SimulationInterface & interface) : interface(interface) {}

  ::grpc::Status
  GetAirTemperature(::grpc::ServerContext * context,
                    const ::cwspb::RequestDevice * request,
                    ::cwspb::ResponseSensorAirTemperature * response) override {

    auto & baseResp = *response->mutable_base();

    auto map = interface.getMap();
    if (!verifyMapCreated(map, baseResp)) {
      return grpc::Status::OK;
    }

    Subject::Id id;
    Coordinates coord;
    fromSubjectId(id, coord, request->id());
    if (!verifyCoordinates(coord, map->getDimension(), baseResp)) {
      return grpc::Status::OK;
    }

    auto subject = map->select(std::move(SubjectSelectQuery(coord, id)));

    // process
    if (auto sensor = dynamic_cast<const Subject::SensorAirTemperature *>(subject)) {
      toTemperature(*response->mutable_temp(), sensor->getAirTemperature());
      return grpc::Status::OK;
    }

    verifySubjectExists(nullptr, *response->mutable_base());
    return grpc::Status::OK;
  }

  ::grpc::Status
  GetIllumination(::grpc::ServerContext * context,
                  const ::cwspb::RequestDevice * request,
                  ::cwspb::ResponseSensorIllumination * response) override {

    auto & baseResp = *response->mutable_base();

    auto map = interface.getMap();
    if (!verifyMapCreated(map, baseResp)) {
      return grpc::Status::OK;
    }

    Subject::Id id;
    Coordinates coord;
    fromSubjectId(id, coord, request->id());
    if (!verifyCoordinates(coord, map->getDimension(), baseResp)) {
      return grpc::Status::OK;
    }

    auto subject = map->select(std::move(SubjectSelectQuery(coord, id)));

    // process
    if (auto sensor = dynamic_cast<const Subject::SensorIllumination *>(subject)) {
      toIllumination(*response->mutable_illumination(), sensor->getCellIllumination());
      return grpc::Status::OK;
    }

    verifySubjectExists(nullptr, *response->mutable_base());
    return grpc::Status::OK;
  }

  ::grpc::Status GetCameraInfo(::grpc::ServerContext * context,
                               const ::cwspb::RequestDevice * request,
                               ::cwspb::ResponseCameraInfo * response) override {

    auto & baseResp = *response->mutable_base();

    auto map = interface.getMap();
    if (!verifyMapCreated(map, baseResp)) {
      return grpc::Status::OK;
    }

    Subject::Id id;
    Coordinates coord;
    fromSubjectId(id, coord, request->id());
    if (!verifyCoordinates(coord, map->getDimension(), baseResp)) {
      return grpc::Status::OK;
    }

    auto subject = map->select(std::move(SubjectSelectQuery(coord, id)));

    // process
    if (auto camera = dynamic_cast<const Subject::BaseCamera *>(subject)) {
      for (const auto & [coord, sub] : camera->getVisibleSubjects()) {
        toSubjectId(*response->add_visible_subjects(), sub.getId(), coord);
      }
      return grpc::Status::OK;
    }

    verifySubjectExists(nullptr, *response->mutable_base());
    return grpc::Status::OK;
  }

  ::grpc::Status TransmitPacket(::grpc::ServerContext * context,
                                const ::cwspb::RequestTransmitPackets * request,
                                ::cwspb::Response * response) override {
    auto map = interface.getMap();
    if (!verifyMapCreated(map, *response)) {
      return grpc::Status::OK;
    }

    auto dimension = map->getDimension();

    Subject::Id id;
    Coordinates coord;
    fromSubjectId(id, coord, request->id());
    if (!verifyCoordinates(coord, dimension, *response)) {
      return grpc::Status::OK;
    }

    PacketList packetList;
    for (const auto & packet : request->packets()) {
      packetList.emplace_back(fromPacket(packet));
    }

    std::function<void(Subject::Plain *, void *)> callback = addPacketToTransmitQueue;

    interface.addModifyQuery(std::make_unique<SubjectCallbackQuery<PacketList>>(
        SubjectSelectQuery(coord, id), std::move(callback), std::move(packetList)));

    return grpc::Status::OK;
  }

  ::grpc::Status ReceivePackets(::grpc::ServerContext * context,
                                const ::cwspb::RequestDevice * request,
                                ::cwspb::ResponseReceivedPackets * response) override {
    auto & baseResp = *response->mutable_base();

    auto map = interface.getMap();
    if (!verifyMapCreated(map, baseResp)) {
      return grpc::Status::OK;
    }

    Subject::Id id;
    Coordinates coord;
    fromSubjectId(id, coord, request->id());
    if (!verifyCoordinates(coord, map->getDimension(), baseResp)) {
      return grpc::Status::OK;
    }

    auto subject = map->select(std::move(SubjectSelectQuery(coord, id)));

    // process
    if (auto receiver = dynamic_cast<const Subject::ExtReceiver *>(subject)) {
      for (const auto & packet : receiver->getReceivedPackets()) {
        toPacket(*response->add_packets(), *packet);
      }
      return grpc::Status::OK;
    }

    verifySubjectExists(nullptr, *response->mutable_base());
    return grpc::Status::OK;
  }

  ::grpc::Status TurnDevice(::grpc::ServerContext * context,
                            const ::cwspb::RequestTurnDevice * request,
                            ::cwspb::Response * response) override {
    auto map = interface.getMap();
    if (!verifyMapCreated(map, *response)) {
      return grpc::Status::OK;
    }

    auto dimension = map->getDimension();

    Subject::Id id;
    Coordinates coord;
    fromSubjectId(id, coord, request->id());
    if (!verifyCoordinates(coord, dimension, *response)) {
      return grpc::Status::OK;
    }

    Subject::TurnableStatus status = fromTurnableStatus(request->turnable_status());
    std::function<void(Subject::Plain *, void *)> callback = setTurnableStatus;

    interface.addModifyQuery(
        std::make_unique<SubjectCallbackQuery<Subject::TurnableStatus>>(
            SubjectSelectQuery(coord, id), std::move(callback), std::move(status)));

    return grpc::Status::OK;
  }

private:
};
