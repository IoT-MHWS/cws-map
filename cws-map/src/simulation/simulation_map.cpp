#include "cws/simulation/simulation_map.hpp"
#include "cws/subject/plain.hpp"

void SimulationMap::setModifyQuery(SubjectModifyQuery && query) {
  switch (query.queryType) {
  case SubjectModifyType::INSERT:
    modifyQueryInsert(std::move(query));
    break;
  case SubjectModifyType::UPDATE:
    modifyQueryUpdate(std::move(query));
    break;
  case SubjectModifyType::DELETE:
    modifyQueryDelete(std::move(query));
    break;
  default:
    break;
  }
}

const Subject::Plain *
SimulationMap::selectSubject(const SubjectSelectQuery & query) const {
  auto & subjectLayer = layers.subjectLayer;
  auto & cell = subjectLayer.getCell(query.coordinates);
  auto & subjectList = cell.getElement().getSubjectList();

  for (auto it = subjectList.begin(); it != subjectList.end(); ++it) {
    if ((*it)->getSubjectId() == query.id) {
      return it->get();
    }
  }
  return nullptr;
}

void SimulationMap::modifyQueryInsert(SubjectModifyQuery && query) {
  auto & subjectLayer = layers.subjectLayer;
  auto & cell = subjectLayer.accessCell(query.coordinates);
  cell.accessElement().accessSubjectList().push_front(std::move(query.subject));
}

void SimulationMap::modifyQueryUpdate(SubjectModifyQuery && query) {
  auto & subjectLayer = layers.subjectLayer;
  auto & cell = subjectLayer.accessCell(query.coordinates);
  auto & subjectList = cell.accessElement().accessSubjectList();

  for (auto it = subjectList.begin(); it != subjectList.end(); ++it) {
    if (**it == *query.subject) {
      *it = std::move(query.subject);
      break;
    }
  }
}
void SimulationMap::modifyQueryDelete(SubjectModifyQuery && query) {
  auto & subjectLayer = layers.subjectLayer;
  auto & cell = subjectLayer.accessCell(query.coordinates);
  auto & subjectList = cell.accessElement().accessSubjectList();

  for (auto it = subjectList.begin(); it != subjectList.end(); ++it) {
    if (**it == *query.subject) {
      subjectList.erase(it);
      break;
    }
  }
}
