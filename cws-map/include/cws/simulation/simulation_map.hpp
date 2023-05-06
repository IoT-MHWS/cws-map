#pragma once

#include "cws/map.hpp"

enum SubjectQueryType {
  UNSPECIFIED = 0,
  INSERT = 1,
  UPDATE = 2,
  DELETE = 3,
  SELECT = 4
};

struct SubjectQuery {
  SubjectQueryType queryType;
  Coordinates coordinates;
};

struct SubjectQuerySelect : public SubjectQuery {
  SubjectId selectId;
};

struct SubjectQuerySet : public SubjectQuery {
  std::unique_ptr<Subject> subject;
};

/*
 * Map with extended feature to update map from queries
 */
class SimulationMap : public Map {

public:
  explicit SimulationMap(Dimension dimension) : Map(dimension) {}

  void update(SubjectQuerySet && query);

private:
  void updateInsert(SubjectQuerySet && query);
  void updateUpdate(SubjectQuerySet && query);
  void updateDelete(SubjectQuerySet && query);
};
