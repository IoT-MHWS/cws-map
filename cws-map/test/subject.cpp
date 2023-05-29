#include "gtest/gtest.h"

#include "cws/map_layer/subject.hpp"
#include "cws/physical.hpp"
#include "cws/subject/light_emitter.hpp"

TEST(Subject, TurnableLightEmitter) {
  using namespace Subject;

  Plain plain({}, Id{.idx = 2}, 0, {});

  TurnableLightEmitter emitter(LightEmitter(Plain({}, Id{.idx = 1}, 0, {}), {}, {}),
                               TurnableStatus::OFF, {}, {});

  EXPECT_EQ(emitter.getSubjectId().type, Type::TURNABLE_LIGHT_EMITTER);
  EXPECT_TRUE(emitter.isTempSource());
  EXPECT_FALSE(plain.isTempSource());
}

TEST(MapLayerSubject, createTemplate) { MapLayerSubject layer({4, 4}); }

TEST(Subject, updateTemperature) {
  using namespace Subject;

  TurnableLightEmitter emitter(
      LightEmitter(Plain(Physical(10, 400, Temperature{30}, Obstruction{0.5}),
                         Id{.idx = 1}, 1, {}),
                   {}, TempSourceParams{.heatProduction = 4000}),
      TurnableStatus::ON, {}, TempSourceParams{.heatProduction = 0});

  EXPECT_TRUE(static_cast<Plain *>(&emitter)->isTempSource());

  Temperature before = emitter.getTemperature();

  EXPECT_EQ(TurnableStatus::ON, emitter.getStatus());
  emitter.nextTemperature();

  Temperature after = emitter.getTemperature();
  EXPECT_EQ(Temperature{.value = 1}.value, after.value - before.value);

  emitter.setStatus(TurnableStatus::OFF);

  Temperature afterOff = emitter.getTemperature();
  EXPECT_EQ(after, afterOff);
}
