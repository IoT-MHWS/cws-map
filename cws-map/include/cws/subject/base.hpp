#pragma once

enum class SubjectType {
  UNKNOWN = 0,
  PLAIN = 1,
  FLOOR = 2,
  INTERACTIVE = 3,
  SENSOR = 4
};

class Subject {
  SubjectType subjectType;
};
