#pragma once

#include "cws/map.hpp"

enum SubjectQueryType {
  UNSPECIFIED = 0,
  INSERT = 1,
  UPDATE = 2,
  DELETE = 3,
  SELECT = 4
};

struct SubjectQuery final {
  SubjectQueryType queryType;
  Coordinates coordinates;
  std::unique_ptr<Subject::Plain> subject;

  SubjectQuery(SubjectQueryType type, Coordinates coordinates,
               std::unique_ptr<Subject::Plain> && subject)
      : queryType(type), coordinates(coordinates), subject(std::move(subject)) {}
};

/*
 * Map with extended feature to update map from queries
 */
class SimulationMap : public Map {

public:
  explicit SimulationMap(Dimension dimension) : Map(dimension) {}

  virtual SimulationMap * clone() { return new SimulationMap(*this); }

  void setQuery(SubjectQuery && query);
  const Subject::Plain * getQuery(const SubjectQuery & query) const;

private:
  void setQueryInsert(SubjectQuery && query);
  void setQueryUpdate(SubjectQuery && query);
  void setQueryDelete(SubjectQuery && query);
};
