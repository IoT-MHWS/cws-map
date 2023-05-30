#include "cws/common.hpp"
#include "cws/map_layer/air.hpp"
#include "cws/map_layer/illumination.hpp"
#include "cws/map_layer/obstruction.hpp"
#include "cws/subject/light_emitter.hpp"
#include "gtest/gtest.h"

int test_google(int argc, char ** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

int test() {
  Dimension dim{3, 3};

  MapLayerAir curLayerAir(dim);
  auto & curContainer = curLayerAir.accessAirContainer({1, 1});

  curContainer.add(std::make_unique<Air::Plain>(Physical(10, 400, {300}, {}),
                                                Air::Type::PLAIN, 0.30));
  curContainer.add(std::make_unique<Air::Plain>(Physical(20, 600, {300}, {}),
                                                Air::Type::UNSPECIFIED, 0.30));
  MapLayerAir nextLayerAir(curLayerAir);

  MapLayerSubject subject(dim);

  MapLayerObstruction obstruction(dim);
  std::vector<std::vector<double>> obs_layer{
      {1., 0.2, 0.4},
      {0.2, 0.2, 0.0},
      {0.99, 0.2, 0.0},
  };
  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x)
    for (c.y = 0; c.y < dim.height; ++c.y)
      obstruction.setAirObstruction(c, Obstruction{obs_layer[c.x][c.y]});

  for (int i = 0; i < 4; ++i) {
    nextLayerAir.nextCirculationMassTemp(curLayerAir, obstruction);
    std::cout << "ITERATION: " << i << std::endl;
    // displayAirMap(nextLayerAir);
    curLayerAir = MapLayerAir(nextLayerAir);
  }
  return 0;
}

int main(int argc, char ** argv) {
  // return test();
  return test_google(argc, argv);
}
