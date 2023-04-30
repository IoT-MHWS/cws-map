#pragma once

#include "cws/layer/base.hpp"

#include <list>

class Subject {
  int id;
};

class LayerSubject : public BaseLayer {
  std::list<Subject> subject_list;

public:
  const std::list<Subject> & getSubjectList() { return subject_list; }
};
