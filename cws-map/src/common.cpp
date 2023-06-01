#include "cws/common.hpp"
#include <list>

// Coordinates
std::ostream & operator<<(std::ostream & out, const Coordinates & value) {
  out << "{x:" << value.x << ",y:" << value.y << "}";
  return out;
}

bool operator<(const Coordinates & lhs, const Coordinates & rhs) {
  if (lhs.x == rhs.x) {
    return lhs.y < rhs.y;
  } else {
    return lhs.x < rhs.x;
  }
}

bool operator==(const Coordinates & lhs, const Coordinates & rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
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

std::list<Coordinates> getNeighbours(Dimension dim, Coordinates p) {

  std::list<Coordinates> n;

  for (int x = p.x - 1; x <= p.x + 1; ++x) {
    for (int y = p.y - 1; y <= p.y + 1; ++y) {
      if (x < 0 || x >= dim.width || y < 0 || y >= dim.height ||
          (x == p.x && y == p.y)) {
        continue;
      }
      n.emplace_back(x, y);
    }
  }
  return n;
}

std::pair<int, int> getVector(Coordinates p1, Coordinates p2) {
  return std::make_pair(p2.x - p1.x, p2.y - p1.y);
}

int getScalarMultiplication(std::pair<int, int> v1, std::pair<int, int> v2) {
  return v1.first * v2.first + v1.second * v2.second;
}

int getDistanceSquare(std::pair<int, int> v) {
  return v.first * v.first + v.second * v.second;
}
