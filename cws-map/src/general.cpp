#include "cws/general.hpp"

// Coordinates
std::ostream & operator<<(std::ostream & out, const Coordinates * value) {
  if (value) {
    out << "{x:" << value->x << ",y:" << value->y << "}";
  }
  return out;
}

// Dimension
std::ostream & operator<<(std::ostream & out, const Dimension * value) {
  if (value) {
    out << "{width:" << value->width << ",height=" << value->height << "}";
  }
  return out;
}

bool operator==(const Dimension & lhs, const Dimension & rhs) {
  return lhs.width == rhs.width && lhs.height == rhs.height;
}

// Temperature
std::ostream & operator<<(std::ostream & out, const Temperature & value) {
  out << "{value: " << value.value << "}";
  return out;
}

Temperature operator+(const Temperature & lhs, const Temperature & rhs) {
  return Temperature{.value = lhs.value + rhs.value};
}

bool operator==(const Temperature & lhs, const Temperature & rhs) {
  return lhs.value == rhs.value;
}

// Obstruction
bool operator>(const Obstruction & lhs, const Obstruction & rhs) {
  return lhs.value > rhs.value;
}

Obstruction operator+(const Obstruction & lhs, const Obstruction & rhs) {
  return {.value = lhs.value + rhs.value};
}
Obstruction operator-(const Obstruction & lhs, const Obstruction & rhs) {
  return {.value = lhs.value - rhs.value};
}
Obstruction operator*(const Obstruction & lhs, const Obstruction & rhs) {
  return {.value = lhs.value * rhs.value};
}

// Illumination
Illumination operator+(const Illumination & lhs, const Illumination & rhs) {
  return {.value = lhs.value + rhs.value};
}

bool operator==(const Illumination & lhs, const Illumination & rhs) {
  return lhs.value == rhs.value;
}

Illumination Illumination::getActualIllumination(const Obstruction & abs) {
  return {.value = static_cast<int>(this->value * (1 - abs.value))};
}
