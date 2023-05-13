#include "cws/general.hpp"

std::ostream & operator<<(std::ostream & out, const Coordinates * value) {
  if (value) {
    out << "{x:" << value->x << ",y:" << value->y << "}";
  }
  return out;
}

std::ostream & operator<<(std::ostream & out, const Dimension * value) {
  if (value) {
    out << "{width:" << value->width << ",height=" << value->height << "}";
  }
  return out;
}

bool operator==(const Dimension & lhs, const Dimension & rhs) {
  return lhs.width == rhs.width && lhs.height == rhs.height;
}

bool operator>(const Absorption & lhs, const Absorption & rhs) {
  return lhs.value > rhs.value;
}

Absorption operator+(const Absorption & lhs, const Absorption & rhs) {
  return {.value = lhs.value + rhs.value};
}
Absorption operator-(const Absorption & lhs, const Absorption & rhs) {
  return {.value = lhs.value - rhs.value};
}
Absorption operator*(const Absorption & lhs, const Absorption & rhs) {
  return {.value = lhs.value * rhs.value};
}
