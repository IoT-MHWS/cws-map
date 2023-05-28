#include "gtest/gtest.h"

#include "cws/subject/light_emitter.hpp"

TEST(Subject, TurnableLightEmitter) {
  using namespace Subject;

  Plain plain({}, Id{.idx = 2}, 0);

  TurnableLightEmitter emitter(LightEmitter(Plain({}, Id{.idx = 1}, 0), {}, {}),
                               TurnableStatus::OFF, {}, {});

  std::cout << (int)emitter.getSubjectId().type << std::endl;
  std::cout << emitter.isTempSource() << std::endl;
  std::cout << plain.isTempSource() << std::endl;
}
