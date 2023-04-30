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
template<Derived<BaseLayer> T>
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

  T setElement(T && member) { return this->element; }
};

/*
 * Is a 2-d array of cells of type T
 */
template<Derived<BaseLayer> T>
class MapLayer {
private:
  std::vector<std::vector<CellLayer<T>>> field;

public:
  MapLayer(Dimension dimension) {
    field = std::vector<std::vector<CellLayer<T>>>(
        dimension.width, std::vector<CellLayer<T>>(dimension.height));
  }

  const CellLayer<T> & getCell(Coordinates coordinates) const {
    return field[coordinates.x][coordinates.y];
  }

  void setCell(CellLayer<T> cell, Coordinates coordinates) {
    field[coordinates.x][coordinates.y] = cell;
  }
};
