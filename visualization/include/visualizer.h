#pragma once

#include "model.h"
#include "../../include/model.h"
#include <vector>

namespace ws::vis {

  using ws::wm::Group;
  using ws::wm::Result;

  class Visualizer {
  public:
    Visualizer() = default;
    ~Visualizer() = default;

    void visualizeStage(const std::string &stageName, const std::vector<Result> &results);
  };

}

