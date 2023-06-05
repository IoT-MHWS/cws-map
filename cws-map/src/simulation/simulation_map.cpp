#include "cws/simulation/simulation_map.hpp"
#include "cws/subject/plain.hpp"

void SimulationMap::modify(SubjectModifyQuery && query) {
  switch (query.queryType) {
  case SubjectModifyType::INSERT:
    modifyInsert(std::move(query));
    break;
  case SubjectModifyType::UPDATE:
    modifyUpdate(std::move(query));
    break;
  case SubjectModifyType::DELETE:
    modifyDelete(std::move(query));
    break;
  default:
    break;
  }
}

const Subject::Plain * SimulationMap::select(const SubjectSelectQuery & query) const {
  auto & subjectLayer = layers.subjectLayer;
  auto & subjectList = subjectLayer.getSubjectList(query.coordinates);

  for (auto it = subjectList.begin(); it != subjectList.end(); ++it) {
    if ((*it)->getId() == query.id) {
      return it->get();
    }
  }
  return nullptr;
}

void SimulationMap::modifyInsert(SubjectModifyQuery && query) {
  auto & subjectLayer = layers.subjectLayer;
  auto & subjectList = subjectLayer.accessSubjectList(query.coordinates);
  subjectList.push_front(std::move(query.subject));
}

void SimulationMap::modifyUpdate(SubjectModifyQuery && query) {
  auto & subjectLayer = layers.subjectLayer;
  auto & subjectList = subjectLayer.accessSubjectList(query.coordinates);

  for (auto it = subjectList.begin(); it != subjectList.end(); ++it) {
    if (**it == *query.subject) {
      *it = std::move(query.subject);
      break;
    }
  }
}
void SimulationMap::modifyDelete(SubjectModifyQuery && query) {
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

void SimulationMap::modify(AirInsertQuery && query) { modifyInsert(std::move(query)); }

void SimulationMap::modifyInsert(AirInsertQuery && query) {
  auto & airLayer = layers.airLayer;
  auto & container = airLayer.accessAirContainer(query.coordinates);
  container.add(std::move(query.air));
}

const Air::Plain * SimulationMap::select(const AirSelectQuery & query) const {
  auto & airLayer = layers.airLayer;
  auto & airList = airLayer.getAirContainer(query.coordinates).getList();

  for (auto it = airList.begin(); it != airList.end(); ++it) {
    if ((*it)->getId() == query.id) {
      return it->get();
    }
  }
  return nullptr;
}
