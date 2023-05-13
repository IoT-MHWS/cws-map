#pragma once

namespace Subject {

class Extensiable {
  virtual bool isLightSource() const { return false; }
  virtual bool isTempSource() const { return false; }
  virtual bool isTurnable() const { return false; }
};

};// namespace Subject
