#pragma once

#include <vector>
#include "model.h"
#include "simulator.h"
#include "visualizer.h"

namespace ws {
namespace wm {

using ws::sim::Simulator;
using ws::vis::Visualizer;

class WM {
public:
  explicit WM(Simulator *simulator) : _totalGoals(0), _simulator(simulator), _visualizer(&_totalGoals) {}

  ~WM() = default;

  void playWM();

  Best16 playGroupPhase();

  Quarter playRoundOfBest16(const Best16 &best);

  Semi playQuarterFinals(const Quarter &quarter);

  Final playSemiFinals(const Semi &semi);

  Team *playFinal(const Final &final);

  int _totalGoals;

private:
  Simulator *_simulator;
  Visualizer _visualizer;

  std::vector<std::unique_ptr<Team>> _teams;
  Group _groupA, _groupB, _groupC, _groupD, _groupE, _groupF, _groupG, _groupH;

  Result playGroup(const std::string &groupName, Group *);

  Group createGroup(TeamName team1, TeamName team2, TeamName team3, TeamName team4) const;
};

}  // namespace wm
}  // namespace ws
