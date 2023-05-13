#pragma once

#include <ostream>

/*
 * Coordinates of a cell on the map
 */
struct Coordinates {
  int x;
  int y;

  friend std::ostream & operator<<(std::ostream & out, const Coordinates * value);
};

struct Dimension {
  int width;
  int height;

  friend std::ostream & operator<<(std::ostream & out, const Dimension * value);

  friend bool operator==(const Dimension& lhs, const Dimension& rhs);
};

struct Temperature {
  double value;
};

struct Illumination {
  int value;
};

struct Absorption {
  double value;

  friend bool operator>(const Absorption &lhs, const Absorption &rhs);

  friend Absorption operator+(const Absorption &lhs, const Absorption &rhs);
  friend Absorption operator-(const Absorption &lhs, const Absorption &rhs);
  friend Absorption operator*(const Absorption &lhs, const Absorption &rhs);
};
