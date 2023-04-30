#pragma once

#include "cws/layer/base.hpp"
#include "cws/subject/base.hpp"

#include <list>

class LayerSubject : public BaseLayer {
  std::list<Subject> subject_list;

public:
  const std::list<Subject> & getSubjectList() { return subject_list; }
};
