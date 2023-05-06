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

  SubjectQuery(SubjectQueryType type, Coordinates coordinates)
      : queryType(type), coordinates(coordinates) {}
};

struct SubjectQuerySelect : public SubjectQuery {
  SubjectId id;
  SubjectQuerySelect(Coordinates coordinates, SubjectId id)
      : SubjectQuery(SubjectQueryType::SELECT, coordinates), id(id) {}
};

struct SubjectQuerySet : public SubjectQuery {
  std::unique_ptr<Subject> subject;
  SubjectQuerySet(SubjectQueryType type, Coordinates coordinates,
                  std::unique_ptr<Subject> && subject)
      : SubjectQuery(type, coordinates), subject(std::move(subject)) {}
};

/*
 * Map with extended feature to update map from queries
 */
class SimulationMap : public Map {

public:
  explicit SimulationMap(Dimension dimension) : Map(dimension) {}

  void setQuery(SubjectQuerySet && query);
  const Subject * getQuery(const SubjectQuerySelect & query) const;

private:
  void setQueryInsert(SubjectQuerySet && query);
  void setQueryUpdate(SubjectQuerySet && query);
  void setQueryDelete(SubjectQuerySet && query);
};
