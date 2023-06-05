#pragma once

#include "cws/map.hpp"

enum SubjectModifyType {
  UNSPECIFIED = 0,
  INSERT = 1,
  UPDATE = 2,
  DELETE = 3,
};

struct SubjectModifyQuery final {
  SubjectModifyType queryType;
  Coordinates coordinates;
  std::unique_ptr<Subject::Plain> subject;

  SubjectModifyQuery(SubjectModifyType type, Coordinates coordinates,
                     std::unique_ptr<Subject::Plain> && subject)
      : queryType(type), coordinates(coordinates), subject(std::move(subject)) {}
};

struct SubjectSelectQuery final {
  Coordinates coordinates;
  Subject::Id id;

public:
  SubjectSelectQuery(Coordinates c, Subject::Id id) : coordinates(c), id(id) {}
};

struct AirInsertQuery final {
  Coordinates coordinates;
  std::unique_ptr<Air::Plain> air;

  AirInsertQuery(Coordinates coordinates, std::unique_ptr<Air::Plain> && air)
      : coordinates(coordinates), air(std::move(air)) {}
};

struct AirSelectQuery final {
  Coordinates coordinates;
  Air::Id id;

public:
  AirSelectQuery(Coordinates c, Air::Id id) : coordinates(c), id(id) {}
};

/*
 * Map with extended feature to update map from queries
 */
class SimulationMap : public Map {

public:
  explicit SimulationMap(Dimension dimension) : Map(dimension) {}

  virtual SimulationMap * clone() { return new SimulationMap(*this); }

  void modify(SubjectModifyQuery && query);
  const Subject::Plain * select(const SubjectSelectQuery & query) const;

  void modify(AirInsertQuery && query);
  const Air::Plain * select(const AirSelectQuery & query) const;

private:
  void modifyInsert(SubjectModifyQuery && query);
  void modifyInsert(AirInsertQuery && query);
  void modifyUpdate(SubjectModifyQuery && query);
  void modifyDelete(SubjectModifyQuery && query);
};
