#include "cws/general.hpp"

std::ostream & operator<<(std::ostream & out, const Coordinates * value) {
  if (value) {
    out << "{x:" << value->x<< ",y:" << value->y << "}";
  }
  return out;
}

std::ostream & operator<<(std::ostream & out, const Dimension * value) {
  if (value) {
    out << "{width:" << value->width << ",height=" << value->height << "}";
  }
  return out;
}
