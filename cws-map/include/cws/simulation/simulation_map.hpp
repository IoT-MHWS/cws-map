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

/*
 * Map with extended feature to update map from queries
 */
class SimulationMap : public Map {

public:
  explicit SimulationMap(Dimension dimension) : Map(dimension) {}

  virtual SimulationMap * clone() { return new SimulationMap(*this); }

  void setModifyQuery(SubjectModifyQuery && query);
  const Subject::Plain * selectSubject(const SubjectSelectQuery & query) const;

private:
  void modifyQueryInsert(SubjectModifyQuery && query);
  void modifyQueryUpdate(SubjectModifyQuery && query);
  void modifyQueryDelete(SubjectModifyQuery && query);
};
