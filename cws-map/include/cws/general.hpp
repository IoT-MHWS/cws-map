#pragma once

#include <ostream>

/*
 * Coordinates of a cell on the map
 */
struct Coordinates {
  int x;
  int y;

  friend std::ostream & operator<<(std::ostream & out, const Coordinates & value);
};

struct Dimension {
  int width;
  int height;

  friend std::ostream & operator<<(std::ostream & out, const Dimension & value);
  friend bool operator==(const Dimension & lhs, const Dimension & rhs);
};

struct Temperature {
  double value;

  double get() const { return value; }

  friend Temperature operator+(const Temperature & lhs, const Temperature & rhs);
  friend std::ostream & operator<<(std::ostream & out, const Temperature & value);
  friend bool operator==(const Temperature & lhs, const Temperature & rhs);
};

struct Obstruction {
  double value;

  double get() const { return value; }

  friend bool operator>(const Obstruction & lhs, const Obstruction & rhs);
  friend Obstruction operator+(const Obstruction & lhs, const Obstruction & rhs);
  friend Obstruction operator-(const Obstruction & lhs, const Obstruction & rhs);
  friend Obstruction operator*(const Obstruction & lhs, const Obstruction & rhs);
};

struct Illumination {
  int value;

  int get() const { return value; }

  Illumination getActualIllumination(const Obstruction & abs);

  friend Illumination operator+(const Illumination & lhs, const Illumination & rhs);
  friend bool operator==(const Illumination & lhs, const Illumination & rhs);
};
