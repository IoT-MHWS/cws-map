/*
 * Contains layers of map
 */

#pragma once

#include <iostream>
#include <vector>

#include "cws/general.hpp"
#include "cws/layer/base.hpp"
#include "cws/util.hpp"

/*
 * Just a normal cell that contains element in it
 */
template<Derived<Layer> T>
class CellLayer {
  T element;

public:
  CellLayer(){};

  CellLayer(T && element) {
#ifndef NDEBUG
    std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    this->element = std::move(element);
  }

  const T & getElement() const { return this->element; }
  T & accessElement() { return this->element; }

  void setElement(T && member) { return this->element = std::move(element); }
};

/*
 * Is a 2-d array of cells of type T
 */
template<Derived<Layer> T>
class MapLayer {
private:
  Dimension dimension_;
  std::vector<std::vector<CellLayer<T>>> field_;

public:
  MapLayer(Dimension dimension)
      : dimension_(dimension),
        field_(std::vector<std::vector<CellLayer<T>>>(
            dimension.width, std::vector<CellLayer<T>>(dimension.height))) {}

  const CellLayer<T> & getCell(Coordinates coordinates) const {
    return field_[coordinates.x][coordinates.y];
  }

  CellLayer<T> & accessCell(Coordinates coordinates) {
    return field_[coordinates.x][coordinates.y];
  }

  void setCell(CellLayer<T> cell, Coordinates coordinates) {
    field_[coordinates.x][coordinates.y] = cell;
  }

  Dimension getDimension() const {
    return dimension_;
  }
};
