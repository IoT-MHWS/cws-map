#pragma once

#include "cws/layer/base.hpp"
#include "cws/subject/base.hpp"

#include <list>
#include <memory>

class LayerSubject : public Layer {
  std::list<std::unique_ptr<Subject>> subjectList;

public:
  LayerSubject() : Layer() {}

  LayerSubject(LayerSubject && obj) noexcept : Layer(std::move(obj)) {
    std::swap(this->subjectList, obj.subjectList);
  }

  LayerSubject(const LayerSubject & obj) noexcept : Layer(obj) {
    subjectList.clear();
    for (const auto & e : obj.subjectList) {
      subjectList.push_back(std::unique_ptr<Subject>(e->clone()));
    }
  }

  const std::list<std::unique_ptr<Subject>> & getSubjectList() const {
    return subjectList;
  }
  std::list<std::unique_ptr<Subject>> & accessSubjectList() { return subjectList; }
};
