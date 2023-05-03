#pragma once

template<typename T>
struct ModifCheck {
  T value;
  bool modified = false;
};
