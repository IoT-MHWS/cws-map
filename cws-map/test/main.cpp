#include "cws/common.hpp"
#include "cws/map_layer/illumination.hpp"
#include "cws/map_layer/obstruction.hpp"
#include "cws/subject/light_emitter.hpp"
#include "gtest/gtest.h"

int test_google(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

int test() { return 0; }

int main(int argc, char ** argv) {
  // return test();
  return test_google(argc, argv);
}
