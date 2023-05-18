#include "gtest/gtest.h"

#include "cws/map_layer/air.hpp"
#include "cws/map_layer/subject.hpp"

TEST(air, nextConvectionCell) {
  MapLayerAir layerAir({4, 4});
  MapLayerSubject layerSubject({4, 4});
  layerAir.nextConvection(layerSubject);
}
