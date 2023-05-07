#include "cws/subject/base.hpp"
#include <iostream>

void Subject::nextState(SubjectId subjectId, const Layers & layers) {
  std::cout << "WARNING: calling nextState() for base Subject" << this <<  std::endl;
}

bool operator==(const SubjectId & lsv, const SubjectId & rsv) {
  return lsv.idx == rsv.idx && lsv.type == rsv.type;
}

bool operator==(const Subject & lsv, const Subject & rsv) { return lsv.id == rsv.id; }
