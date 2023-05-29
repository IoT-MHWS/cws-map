#include "cws/map_layer/obstruction.hpp"
#include "cws/general.hpp"
#include "cws/map_layer/subject.hpp"
#include "gtest/gtest.h"

TEST(MapLayerObstruction, updateAirObstruction) {
  Dimension dim{1, 1};

  MapLayerObstruction obstruction(dim);

  MapLayerSubject layerSubject(dim);
  auto & subjectList =
      layerSubject.accessCell({0, 0}).accessElement().accessSubjectList();

  subjectList.emplace_back(std::make_unique<Subject::Plain>(
      Physical(10, 1000, {60}, {}), Subject::Id{.idx = 1}, 0.40, Obstruction{0.40}));
  subjectList.emplace_back(std::make_unique<Subject::Plain>(
      Physical(10, 1000, {60}, {}), Subject::Id{.idx = 1}, 0.60, Obstruction{0.40}));

  obstruction.updateAirObstruction(layerSubject);

  ASSERT_EQ(Obstruction{0.64}.get(), obstruction.getAirObstruction({0, 0}).get());
}

TEST(MapLayerObstruction, updateLightObstruction) {
  Dimension dim{1, 1};

  MapLayerObstruction obstruction(dim);

  MapLayerSubject layerSubject(dim);
  auto & subjectList =
      layerSubject.accessCell({0, 0}).accessElement().accessSubjectList();

  subjectList.emplace_back(std::make_unique<Subject::Plain>(
      Physical(10, 1000, {60}, {0.5}), Subject::Id{.idx = 1}, 0.40, Obstruction{0.40}));
  subjectList.emplace_back(std::make_unique<Subject::Plain>(
      Physical(10, 1000, {60}, {0.5}), Subject::Id{.idx = 1}, 0.60, Obstruction{0.40}));

  obstruction.updateLightObstruction(layerSubject);

  ASSERT_EQ(Obstruction{0.75}.get(), obstruction.getLightObstruction({0, 0}).get());
}
