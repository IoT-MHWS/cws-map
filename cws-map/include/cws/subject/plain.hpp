#pragma once

#include "cws/physical.hpp"
#include "cws/subject/extension/base.hpp"
#include "cws/subject/type.hpp"

namespace Subject {

/*
 * Identifier of subject
 */
struct Id {
  SubjectType type;
  int idx;

protected:
  friend bool operator==(const Id &, const Id &);
};

class Plain : public Physical, virtual public Extensiable {
  Id id_;
  double surfaceArea_;

  double defAirConduction_;

public:
  Plain(Physical &&params, Id id, double surfaceArea)
      : Physical(std::move(params)), id_(id), surfaceArea_(surfaceArea) {

    setType(SubjectType::PLAIN);
  }

  virtual Plain *clone() const { return new Plain(*this); }

public:
  Id getSubjectId() const { return id_; }
  double getSurfaceArea() const { return surfaceArea_; }

  double getDefAirConduction() const { return defAirConduction_; }
  virtual double getCurAirConduction() const { return defAirConduction_; }

protected:
  void setType(SubjectType type) { id_.type = type; }

public:
  friend bool operator==(const Plain &lhs, const Plain &rhs);
};

} // namespace Subject
