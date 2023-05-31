#pragma once

#include "cws/layer/base.hpp"
#include "cws/network/container.hpp"
#include <list>
#include <memory>

class LayerNetwork : public Layer {
public:
  using ContainerUPTR = std::unique_ptr<Network::Container>;

private:
  std::list<ContainerUPTR> containerList;

public:
  LayerNetwork() = default;

  LayerNetwork(LayerNetwork && obj) noexcept : Layer(std::move(obj)) {
    this->containerList = std::move(obj.containerList);
  }

  LayerNetwork(const LayerNetwork & obj) noexcept : Layer(obj) {
    containerList.clear();
    for (const auto & e : obj.containerList) {
      containerList.push_back(std::unique_ptr<Network::Container>(e->clone()));
    }
  }

  LayerNetwork & operator=(LayerNetwork && obj) noexcept {
    if (this == &obj) {
      return *this;
    }

    Layer::operator=(std::move(obj));

    this->containerList = std::move(obj.containerList);

    return *this;
  }

  LayerNetwork & operator=(const LayerNetwork & obj) noexcept {
    if (this == &obj) {
      return *this;
    }

    Layer::operator=(obj);

    containerList.clear();
    for (const auto & e : obj.containerList) {
      containerList.push_back(std::unique_ptr<Network::Container>(e->clone()));
    }

    return *this;
  }

  const std::list<ContainerUPTR> & getContainerList() const { return containerList; }

  std::list<ContainerUPTR> & getContainerList() { return containerList; }
};
