#include "cws/map_layer/illumination.hpp"
#include "cws/common.hpp"
#include "cws/map_layer/obstruction.hpp"
#include "cws/subject/light_emitter.hpp"
#include "gtest/gtest.h"

TEST(Illumination, updateIllumination) {
  using namespace Subject;

  Dimension dim{4, 4};

  std::vector<std::vector<double>> obs_layer{
      {0.2, 0.2, 0.4, 0.1},
      {0.2, 0.2, 0.0, 0.1},
      {0.2, 0.2, 0.0, 0.1},
      {0.2, 0.0, 0.4, 0.5},
  };
  MapLayerObstruction obstruction(dim);
  Coordinates c;
  for (c.x = 0; c.x < dim.width; ++c.x)
    for (c.y = 0; c.y < dim.height; ++c.y)
      obstruction.setLightObstruction(c, Obstruction{obs_layer[c.x][c.y]});

  MapLayerSubject subject(dim);

  LightEmitter emitter(Plain(Physical(0, 0, {}, {}), Id{.idx = 1}, 0, {}), {},
                       LightSourceParams{.rawIllumination = Illumination{200}});

  subject.accessSubjectList({1, 1}).push_back(std::make_unique<LightEmitter>(emitter));

  MapLayerIllumination illumination(dim);
  illumination.updateIllumination(obstruction, subject);

  std::vector<std::vector<int>> expected{
      {128, 128, 96, 144},
      {128, 160, 160, 144},
      {128, 128, 160, 144},
      {102, 128, 76, 80},
  };

  for (c.x = 0; c.x < dim.width; ++c.x)
    for (c.y = 0; c.y < dim.height; ++c.y)
      EXPECT_EQ(expected[c.x][c.y], illumination.getIllumination(c).get());
}
