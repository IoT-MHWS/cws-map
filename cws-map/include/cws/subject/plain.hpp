#pragma once

#include "cws/subject/base.hpp"

struct SubjectPlain : public BaseSubject {
  int id;

public:
  SubjectPlain(int id, SubjectParameters parameters)
      : BaseSubject(SubjectType::PLAIN, parameters), id(id) {}

  SubjectPlain * clone() const override { return new SubjectPlain(*this); }

  int getId() const { return id; }

  void nextState(SubjectId subjectId, const Layers & layers) override;
};
