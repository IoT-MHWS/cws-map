#pragma once

namespace Subject {

struct TempSourceParams {
  double heatProduction;
};

class ExtTempSource {
public:
  virtual TempSourceParams getDefTempParams() const = 0;
  virtual TempSourceParams getCurTempParams() const = 0;
  virtual void nextTemperature() = 0;
};

}// namespace Subject
