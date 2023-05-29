#include "gtest/gtest.h"

#include "cws/map_layer/air.hpp"
#include "cws/map_layer/subject.hpp"

TEST(AirPlain, add) {
  using namespace Air;

  Plain lhs(Physical(1, 300, {30}, {0.3}), 0.3);
  Plain rhs(Physical(2, 600, {60}, {0.6}), 0.6);

  Plain exp(Physical(3, 500, {54}, {0.5}), 0.5);
  Plain res = lhs + rhs;

  ASSERT_EQ(exp.getType(), res.getType());
  ASSERT_EQ(exp.getWeight(), res.getWeight());
  ASSERT_EQ(exp.getDefLightObstruction().value, res.getDefLightObstruction().value);
  ASSERT_EQ(exp.getHeatCapacity(), res.getHeatCapacity());
  ASSERT_EQ(exp.getTemperature().value, res.getTemperature().value);
  ASSERT_EQ(exp.getCurLightObstruction().value, res.getCurLightObstruction().value);
}

TEST(AirContainer, addRemoveErase) {
  using namespace Air;

  Container container;
  ASSERT_TRUE(container.empty());

  container.add(std::make_unique<Plain>(Physical{}, 0.15));
  container.add(std::make_unique<Plain>(Physical{}, 0.15));
  container.add(std::make_unique<Plain>(Physical{}, Type::UNSPECIFIED, 0.15));

  ASSERT_FALSE(container.empty());

  const auto & list = container.getList();
  auto it = list.begin();

  it = container.erase(it);
  ASSERT_FALSE(container.empty());

  it = container.erase(it);
  ASSERT_TRUE(container.empty());
  ASSERT_EQ(list.end(), it);
}

TEST(AirContainer, normalize) {
  using namespace Air;

  Container container;
  container.add(std::make_unique<Plain>(Physical(10, 400, {30}, {}), 0.15));
  container.add(std::make_unique<Plain>(Physical(20, 400, {60}, {}), 0.15));

  for (auto & eptr : container.getList()) {
    ASSERT_EQ(Temperature{50}, eptr->getTemperature());
  }

  for (auto it = container.getList().begin(); it != container.getList().end();) {
    it = container.erase(it);
  }
  ASSERT_TRUE(container.empty());

  container.add(std::make_unique<Plain>(Physical(10, 400, {30}, {}), 0.15));
  container.add(std::make_unique<Plain>(Physical(10, 800, {60}, {}), 0.15));
  for (auto & eptr : container.getList()) {
    ASSERT_EQ(Temperature{50}, eptr->getTemperature());
  }
}

TEST(AirContainer, getHeatTransferCoef) {
  using namespace Air;

  Container container;
  container.add(std::make_unique<Plain>(Physical(10, 400, {30}, {}), 0.3));
  container.add(std::make_unique<Plain>(Physical(20, 400, {60}, {}), 0.15));

  ASSERT_EQ(0.2, container.getHeatTransferCoef());
}

TEST(MapLayerAir, nextConvectionCellNoSubjects) {
  Dimension dim{1, 1};

  MapLayerAir layerAir(dim);

  auto & container = layerAir.accessCell({0, 0}).accessElement().accessAirContainer();
  container.add(std::make_unique<Air::Plain>(Physical(10, 400, {30}, {}), 3));
  container.add(std::make_unique<Air::Plain>(Physical(20, 400, {30}, {}),
                                             Air::Type::UNSPECIFIED, 1.5));

  MapLayerSubject layerSubject(dim);
  auto & subjectList =
      layerSubject.accessCell({0, 0}).accessElement().accessSubjectList();

  layerAir.nextConvection(layerSubject);

  for (auto & elptr : container.getList()) {
    ASSERT_EQ(Temperature{30}, elptr->getTemperature());
  }
}

TEST(MapLayerAir, nextConvectionCellNoAir) {
  Dimension dim{1, 1};

  MapLayerAir layerAir(dim);

  auto & container = layerAir.accessCell({0, 0}).accessElement().accessAirContainer();

  MapLayerSubject layerSubject(dim);
  auto & subjectList =
      layerSubject.accessCell({0, 0}).accessElement().accessSubjectList();

  subjectList.emplace_back(std::make_unique<Subject::Plain>(
      Physical(10, 1000, {60}, {}), Subject::Id{.idx = 1}, 10, Obstruction{}));
  subjectList.emplace_back(std::make_unique<Subject::Plain>(
      Physical(10, 1000, {60}, {}), Subject::Id{.idx = 1}, 5, Obstruction{}));

  layerAir.nextConvection(layerSubject);

  for (auto & elptr : subjectList) {
    ASSERT_EQ(Temperature{60}, elptr->getTemperature());
  }
}

// TESTING USING PRINTS
TEST(MapLayerAir, nextConvectionCellValues) {
  Dimension dim{1, 1};

  MapLayerAir layerAir(dim);

  auto & container = layerAir.accessCell({0, 0}).accessElement().accessAirContainer();
  container.add(std::make_unique<Air::Plain>(Physical(10, 400, {30}, {}), 3));
  container.add(std::make_unique<Air::Plain>(Physical(20, 400, {30}, {}),
                                             Air::Type::UNSPECIFIED, 1.5));

  MapLayerSubject layerSubject(dim);
  auto & subjectList =
      layerSubject.accessCell({0, 0}).accessElement().accessSubjectList();

  subjectList.emplace_back(std::make_unique<Subject::Plain>(
      Physical(10, 1000, {60}, {}), Subject::Id{.idx = 1}, 10, Obstruction{}));
  subjectList.emplace_back(std::make_unique<Subject::Plain>(
      Physical(10, 1000, {60}, {}), Subject::Id{.idx = 1}, 5, Obstruction{}));

  for (int i = 0; i < 1000; ++i) {
    layerAir.nextConvection(layerSubject);

    if (i == 0 || i == 1 || i == 998 || i == 999) {
      std::cout << "{" << std::endl;
      std::cout << "  AIR" << std::endl;
      for (auto & eptr : container.getList()) {
        std::cout << "  " << eptr->getTemperature() << std::endl;
      }

      std::cout << "  SUBJECT" << std::endl;
      for (auto & eptr : subjectList) {
        std::cout << "  " << eptr->getTemperature() << std::endl;
      }
      std::cout << "}" << std::endl;
    }
  }
}
