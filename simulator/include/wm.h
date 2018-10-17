#pragma once

#include <vector>
#include "model.h"
#include "simulator.h"
#include "visualizer.h"
#include <tbb/flow_graph.h>

namespace ws::wm {

  using ws::sim::Simulator;
  using ws::vis::Visualizer;

class WM
{
 public:
  explicit WM(Simulator *simulator) : _simulator(simulator) {}
  ~WM() = default;

  Team* playWM();

 private:
  Simulator *_simulator;
  Visualizer *_visualizer;

  std::vector<std::unique_ptr<Team>> _teams;
  Group _groupA, _groupB, _groupC, _groupD, _groupE, _groupF, _groupG, _groupH;

  Result playGroup(const std::string &groupName, Group*);
  Group createGroup(std::string team1, std::string team2, std::string team3, std::string team4) const;
};

}  // namespace ws::wm
