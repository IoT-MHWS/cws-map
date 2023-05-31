#include "gtest/gtest.h"

#include "cws/map_layer/subject.hpp"
#include "cws/physical.hpp"
#include "cws/subject/camera.hpp"
#include "cws/subject/light_emitter.hpp"

TEST(Subject, TurnableLightEmitter) {
  using namespace Subject;

  Plain plain({}, Id{.idx = 2}, 0, {});

  TurnableLightEmitter emitter(
      LightEmitter(Plain(Physical(1, 2, {3}, {4}), Id{.idx = 1}, 5, {0.5}),
                   TempSourceParams{.heatProduction = 400},
                   LightSourceParams{.rawIllumination = Illumination{10}}),
      TurnableStatus::OFF, {}, {});

  EXPECT_EQ(1, emitter.getWeight());
  EXPECT_EQ(2, emitter.getHeatCapacity());
  EXPECT_EQ(3, emitter.getTemperature().get());
  EXPECT_EQ(4, emitter.getDefLightObstruction().get());
  EXPECT_EQ(1, emitter.getSubjectId().idx);
  EXPECT_EQ(emitter.getSubjectId().type, Type::TURNABLE_LIGHT_EMITTER);
  EXPECT_EQ(5, emitter.getSurfaceArea());
  EXPECT_EQ(0.5, emitter.getDefAirObstruction().get());
  EXPECT_EQ(10, emitter.getDefLightParams().rawIllumination.get());
  EXPECT_EQ(400, emitter.getDefTempParams().heatProduction);

  EXPECT_NE(nullptr, dynamic_cast<ExtLightSource *>(&emitter));
  EXPECT_NE(nullptr, dynamic_cast<ExtTempSource *>(&emitter));
}

TEST(MapLayerSubject, createTemplate) { MapLayerSubject layer({4, 4}); }

TEST(Subject, updateTemperature) {
  using namespace Subject;

  TurnableLightEmitter emitter(
      LightEmitter(Plain(Physical(10, 400, Temperature{30}, Obstruction{0.5}),
                         Id{.idx = 1}, 1, {}),
                   TempSourceParams{.heatProduction = 4000}, {}),
      TurnableStatus::ON, {}, TempSourceParams{.heatProduction = 0});

  Temperature before = emitter.getTemperature();

  EXPECT_EQ(TurnableStatus::ON, emitter.getStatus());
  emitter.nextTemperature();

  Temperature after = emitter.getTemperature();
  EXPECT_EQ(Temperature{.value = 1}.value, after.value - before.value);

  emitter.setStatus(TurnableStatus::OFF);

  Temperature afterOff = emitter.getTemperature();
  EXPECT_EQ(after, afterOff);
}

TEST(SubjectInfraredCamera, getVisibleSubjects) {
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

  MapLayerSubject layerSubject(dim);

  layerSubject.accessSubjectList({1, 1}).push_back(
      std::make_unique<InfraredCamera>(Plain(Physical(), 1, 0, {}), 100, 20));
}

TEST(SubjectLightCamera, getVisibleSubjects) { using namespace Subject; }
