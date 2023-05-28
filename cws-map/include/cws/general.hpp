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

  friend bool operator==(const Dimension & lhs, const Dimension & rhs);
};

struct Temperature {
  double value;
  friend Temperature operator+(const Temperature & lhs, const Temperature & rhs);

  friend std::ostream & operator<<(std::ostream & out, const Temperature * value);

  double get() const { return value; }
};

struct Absorption {
  double value;

  friend bool operator>(const Absorption & lhs, const Absorption & rhs);

  friend Absorption operator+(const Absorption & lhs, const Absorption & rhs);
  friend Absorption operator-(const Absorption & lhs, const Absorption & rhs);
  friend Absorption operator*(const Absorption & lhs, const Absorption & rhs);

  double get() const { return value; }
};

struct Illumination {
  int value;

  friend Illumination operator+(const Illumination & lhs, const Illumination & rhs);
  friend bool operator==(const Illumination & lhs, const Illumination & rhs);

  Illumination getActualIllumination(const Absorption & abs);

  int get() const { return value; }
};
