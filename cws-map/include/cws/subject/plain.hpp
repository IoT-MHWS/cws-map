#pragma once

#include "cws/general.hpp"
#include "cws/layer/temperature.hpp"
#include "cws/subject/extension/base.hpp"
#include "cws/subject/type.hpp"
#include <cstddef>

class Layers;

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

/*
 * Params for all subjects
 */
struct PlainParams {
  Absorption lightAbsorption;
};

/*
 * Plain subject
 */
class Plain : virtual public Subject::Extensiable {
protected:
  Id id_;
  PlainParams params_;

protected:
  void setType(SubjectType type) { id_.type = type; }

public:
  Plain() : id_({.type = SubjectType::PLAIN}) {}

  Plain(Id id, PlainParams parameters) : id_(id), params_(parameters) {
    setType(SubjectType::PLAIN);
  }

  virtual ~Plain() {}

  virtual Plain * clone() const { return new Plain(*this); }

public:
  Id getSubjectId() const { return id_; }
  PlainParams getSubjectParameters() const { return params_; }

public:
  friend bool operator==(const Plain &, const Plain &);
};

}// namespace Subject
