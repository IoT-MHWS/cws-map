#include "cws/subject/base.hpp"

bool operator==(const SubjectId & lsv, const SubjectId & rsv) {
  return lsv.idx == rsv.idx && lsv.type == rsv.type;
}

bool operator==(const Subject & lsv, const Subject & rsv) { return lsv.id == rsv.id; }
