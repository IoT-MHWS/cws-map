/*
 * Contains layers of map
 */

#pragma once

#include <iostream>
#include <vector>

#include "cws/common.hpp"
#include "cws/layer/base.hpp"
#include "cws/util.hpp"

/*
 * Just a normal cell that contains element in it
 */
template<Derived<Layer> T>
class CellLayer {
  T element_;

public:
  CellLayer(){};

  CellLayer(const T & element) : element_(element) {}

  CellLayer(T && element) : element_(std::move(element)) {}

  const T & getElement() const { return this->element_; }
  T & accessElement() { return this->element_; }

  void setElement(T && member) { return this->element_ = std::move(element_); }
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

  MapLayer(Dimension dimension, T base)
      : dimension_(dimension),
        field_(std::vector<std::vector<CellLayer<T>>>(
            dimension.width,
            std::vector<CellLayer<T>>(dimension.height, CellLayer<T>(base)))) {}

  Dimension getDimension() const { return dimension_; }

public:
  const CellLayer<T> & operator[](Coordinates c) const { return field_[c.x][c.y]; }

  CellLayer<T> & operator[](Coordinates c) { return field_[c.x][c.y]; }

public:
  const CellLayer<T> & getCell(Coordinates c) const { return (*this)[c]; }

  CellLayer<T> & accessCell(Coordinates c) { return (*this)[c]; }
};
