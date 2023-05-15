#pragma once

#include "cws/air/plain.hpp"
#include "cws/layer/base.hpp"
#include <list>
#include <memory>

class LayerAir : public Layer {
  std::list<std::unique_ptr<Air::Plain>> airList;

public:
  LayerAir() {}

  LayerAir(LayerAir && obj) noexcept : Layer(std::move(obj)) {
    std::swap(this->airList, obj.airList);
  }

  LayerAir(const LayerAir & obj) noexcept : Layer(obj) {
    airList.clear();
    for (const auto & e : obj.airList) {
      airList.push_back(std::unique_ptr<Air::Plain>(e->clone()));
    }
  }

  const std::list<std::unique_ptr<Air::Plain>> & getAirList() const { return airList; }
  std::list<std::unique_ptr<Air::Plain>> & accessAirList() { return airList; }
};
