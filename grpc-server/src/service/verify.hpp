#pragma once

#include "cws/common.hpp"
#include "cws/map.hpp"
#include "cwspb/service/general.pb.h"
#include <memory>

bool verifyMapCreated(const std::shared_ptr<const Map> & map,
                      cwspb::Response & response);

bool verifyCoordinates(const Coordinates & coord, const Dimension & dimension,
                       cwspb::Response & response);

bool verifySubjectExists(const void * subject, cwspb::Response & response);
