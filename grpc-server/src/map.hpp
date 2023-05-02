#pragma once

#include "cwspb/service/map.pb.h"
#include "cwspb/service/map.grpc.pb.h"
#include <grpcpp/support/status.h>

class MapService final: public cws::MapService::Service {

};

