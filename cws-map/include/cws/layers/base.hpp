#pragma once

enum class LayerType {
  UNKNOWN = 0,
  TEMPARATURE = 1,
  HUMIDITY = 2,
  LUMINOSITY = 3,
  SUBJECTS = 4
};

class Layer {
  LayerType type;

public:
  explicit Layer(LayerType t) : type(t) {};

  Layer() : Layer(LayerType::UNKNOWN) {};

  LayerType getType() { return type; }
};
