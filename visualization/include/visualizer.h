#pragma once

#include <vector>
#include "../../include/model.h"
#include "model.h"

namespace ws {
namespace vis {

using ws::wm::Group;
using ws::wm::Result;

class Visualizer {
public:
  explicit Visualizer(int* totalGoals) : totalGoals(totalGoals) {};

  ~Visualizer() = default;

  void visualizeStage(const std::string &stageName, const std::vector<Result> &results);

private:
  int* totalGoals;
};

}  // namespace vis
}  // namespace ws
