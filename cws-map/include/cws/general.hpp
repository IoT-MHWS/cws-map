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
};

struct Percentage {
  double value;
};

struct Temperature {
  double value;
};
