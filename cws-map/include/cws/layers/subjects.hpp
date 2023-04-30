#pragma once

#include "cws/layers/base.hpp"
#include <list>

class Subject {
  int id;
};

class LayerSubject : public Layer {
  std::list<Subject> subject_list;

public:
  const std::list<Subject> & getSubjectList() { return subject_list; }
};
