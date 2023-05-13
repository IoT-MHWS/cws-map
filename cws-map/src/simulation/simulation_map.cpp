#include "cws/simulation/simulation_map.hpp"
#include "cws/subject/plain.hpp"

void SimulationMap::setQuery(SubjectQuery && query) {
  switch (query.queryType) {
  case SubjectQueryType::INSERT:
    setQueryInsert(std::move(query));
    break;
  case SubjectQueryType::UPDATE:
    setQueryUpdate(std::move(query));
    break;
  case SubjectQueryType::DELETE:
    setQueryDelete(std::move(query));
    break;
  default:
    break;
  }
}

const Subject::Plain * SimulationMap::getQuery(const SubjectQuery & query) const {
  if (query.queryType == SubjectQueryType::SELECT) {
    auto & subjectLayer = layers.getSubjectLayer();
    auto & cell = subjectLayer.getCell(query.coordinates);
    auto & subjectList = cell.getElement().getSubjectList();

    for (auto it = subjectList.begin(); it != subjectList.end(); ++it) {
      if ((*it)->getSubjectId() == query.subject->getSubjectId()) {
        return it->get();
      }
    }
  }
  return nullptr;
}

void SimulationMap::setQueryInsert(SubjectQuery && query) {
  auto & subjectLayer = layers.accessSubjectLayer();
  auto & cell = subjectLayer.accessCell(query.coordinates);
  cell.accessElement().accessSubjectList().push_back(std::move(query.subject));
}

void SimulationMap::setQueryUpdate(SubjectQuery && query) {
  auto & subjectLayer = layers.accessSubjectLayer();
  auto & cell = subjectLayer.accessCell(query.coordinates);
  auto & subjectList = cell.accessElement().accessSubjectList();

  for (auto it = subjectList.begin(); it != subjectList.end(); ++it) {
    if (**it == *query.subject) {
      *it = std::move(query.subject);
      break;
    }
  }
}
void SimulationMap::setQueryDelete(SubjectQuery && query) {
  auto & subjectLayer = layers.accessSubjectLayer();
  auto & cell = subjectLayer.accessCell(query.coordinates);
  auto & subjectList = cell.accessElement().accessSubjectList();

  for (auto it = subjectList.begin(); it != subjectList.end(); ++it) {
    if (**it == *query.subject) {
      subjectList.erase(it);
      break;
    }
  }
}
