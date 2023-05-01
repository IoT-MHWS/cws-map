#pragma once

#include "cws/layer/base.hpp"
#include "cws/subject/base.hpp"

#include <list>
#include <memory>

class LayerSubject : public BaseLayer {
  std::list<std::unique_ptr<BaseSubject>> subjectList;

public:
  LayerSubject() : BaseLayer() {}

  LayerSubject(LayerSubject && obj) noexcept : BaseLayer(std::move(obj)) {
    std::swap(this->subjectList, obj.subjectList);
  }

  LayerSubject(const LayerSubject & obj) noexcept : BaseLayer(obj) {
    subjectList.clear();
    for (const auto & e: obj.subjectList) {
      subjectList.push_back(std::unique_ptr<BaseSubject>(e->clone()));
    }
  }

  const std::list<std::unique_ptr<BaseSubject>> & getSubjectList() {
    return subjectList;
  }
};
