#pragma once

#include "cws/subject/base.hpp"

struct SubjectPlain : public Subject {
public:
  SubjectPlain(int idx, SubjectParameters parameters)
      : Subject(SubjectId{SubjectType::PLAIN, idx}, parameters) {}

  SubjectPlain * clone() const override { return new SubjectPlain(*this); }

  void nextState(SubjectId subjectId, const Layers & layers) override;
};
