/*
 * Contains layers of map
 */

#pragma once

#include <iostream>
#include <vector>

#include "cws/general.hpp"
#include "layers/base.hpp"
#include "util.hpp"

/*
 * Just a normal cell that contains element in it
 */
template<Derived<Layer> T>
class CellLayer {
  T element;

public:
  CellLayer(){};

  CellLayer(T && element) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    this->element = std::move(element);
  }

  const T & getElement() { return this->element; }

  T setElement(T && member) { return this->element; }
};

/*
 * Is a 2-d array of cells of type T
 */
template<Derived<Layer> T>
class MapLayer {
private:
  std::vector<std::vector<CellLayer<T>>> field;

public:
  MapLayer(Dimension dimension) {
    field = std::vector<std::vector<CellLayer<T>>>(
        dimension.width, std::vector<CellLayer<T>>(dimension.height));
  }
};
