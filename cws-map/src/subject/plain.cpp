#include "cws/subject/plain.hpp"

namespace Subject {

bool operator==(const Id & lsv, const Id & rsv) {
  return lsv.idx == rsv.idx && lsv.type == rsv.type;
}

bool operator==(const Plain & lhs, const Plain & rhs) { return lhs.id_ == rhs.id_; }

}// namespace Subject
