#include "cws/simulation/simulation_map.hpp"

void SimulationMap::update(SubjectQuerySet && query) {
  switch (query.queryType) {
  case SubjectQueryType::INSERT:
    updateInsert(std::move(query));
    break;
  case SubjectQueryType::UPDATE:
    updateUpdate(std::move(query));
    break;
  case SubjectQueryType::DELETE:
    updateDelete(std::move(query));
    break;
  default:
    break;
  }
}

void SimulationMap::updateInsert(SubjectQuerySet && query) {
  auto subjectLayer = layers.accessSubjectLayer();
  auto cell = subjectLayer.getCell(query.coordinates);
  cell.accessElement().accessSubjectList().push_back(std::move(query.subject));
}

void SimulationMap::updateUpdate(SubjectQuerySet && query) {
  auto subjectLayer = layers.accessSubjectLayer();
  auto cell = subjectLayer.getCell(query.coordinates);

  auto & subjectList = cell.accessElement().accessSubjectList();
  for (auto it = subjectList.begin(); it != subjectList.end(); ++it) {
    if (**it == *query.subject) {
      subjectList.erase(it);
      subjectList.insert(it, std::move(query.subject));
      break;
    }
  }
}
void SimulationMap::updateDelete(SubjectQuerySet && query) {
  auto subjectLayer = layers.accessSubjectLayer();
  auto cell = subjectLayer.getCell(query.coordinates);

  auto & subjectList = cell.accessElement().accessSubjectList();
  for (auto it = subjectList.begin(); it != subjectList.end(); ++it) {
    if (**it == *query.subject) {
      subjectList.erase(it);
      break;
    }
  }
}
