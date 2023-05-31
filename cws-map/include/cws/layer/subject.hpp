#pragma once

#include "cws/layer/base.hpp"
#include "cws/subject/extension/light_source.hpp"
#include "cws/subject/plain.hpp"

#include <list>
#include <memory>

class LayerSubject : public Layer {
  std::list<std::unique_ptr<Subject::Plain>> subjectList;

public:
  LayerSubject() : Layer() {}

  LayerSubject(LayerSubject && obj) noexcept : Layer(std::move(obj)) {
    this->subjectList = std::move(obj.subjectList);
  }

  LayerSubject(const LayerSubject & obj) noexcept : Layer(obj) {
    subjectList.clear();
    for (const auto & e : obj.subjectList) {
      subjectList.push_back(std::unique_ptr<Subject::Plain>(e->clone()));
    }
  }

  LayerSubject & operator=(LayerSubject && obj) noexcept {
    if (this == &obj) {
      return *this;
    }

    Layer::operator=(std::move(obj));

    this->subjectList = std::move(obj.subjectList);

    return *this;
  }

  LayerSubject & operator=(const LayerSubject & obj) noexcept {
    if (this == &obj) {
      return *this;
    }

    Layer::operator=(obj);

    subjectList.clear();
    for (const auto & e : obj.subjectList) {
      subjectList.push_back(std::unique_ptr<Subject::Plain>(e->clone()));
    }

    return *this;
  }

  const std::list<std::unique_ptr<Subject::Plain>> & getSubjectList() const {
    return subjectList;
  }
  std::list<std::unique_ptr<Subject::Plain>> & accessSubjectList() {
    return subjectList;
  }

  const std::list<const Subject::ExtLightSource *> getActiveLightSources() const;
};
