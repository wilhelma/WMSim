#include "wm.h"
#include <wm.h>
#include <algorithm>
#include <future>
#include <tbb/flow_graph.h>

using namespace tbb::flow;

namespace ws::wm {

Best16 WM::playGroupPhase()
{
  Best16 best;

//  auto futA = std::async(std::launch::async, [&] { return playGroup("GroupA", &_groupA); });
//  auto futB = std::async(std::launch::async, [&] { return playGroup("GroupB", &_groupB); });
//  auto futC = std::async(std::launch::async, [&] { return playGroup("GroupC", &_groupC); });
//  auto futD = std::async(std::launch::async, [&] { return playGroup("GroupD", &_groupD); });
//  auto futE = std::async(std::launch::async, [&] { return playGroup("GroupE", &_groupE); });
//  auto futF = std::async(std::launch::async, [&] { return playGroup("GroupF", &_groupF); });
//  auto futG = std::async(std::launch::async, [&] { return playGroup("GroupG", &_groupG); });
//  auto futH = std::async(std::launch::async, [&] { return playGroup("GroupH", &_groupH); });
//
//  best.resultA = futA.get();
//  best.resultB = futB.get();
//  best.resultC = futC.get();
//  best.resultD = futD.get();
//  best.resultE = futE.get();
//  best.resultF = futF.get();
//  best.resultG = futG.get();
//  best.resultH = futH.get();

  graph g;

  broadcast_node<continue_msg> init(g);
  continue_node<continue_msg> playA(g, [&](continue_msg m) { best.resultA = playGroup("groupA", &_groupA); });
  continue_node<continue_msg> playB(g, [&](continue_msg m) { best.resultB = playGroup("groupB", &_groupB); });
  continue_node<continue_msg> playC(g, [&](continue_msg m) { best.resultC = playGroup("groupC", &_groupC); });
  continue_node<continue_msg> playD(g, [&](continue_msg m) { best.resultD = playGroup("groupD", &_groupD); });
  continue_node<continue_msg> playE(g, [&](continue_msg m) { best.resultE = playGroup("groupE", &_groupE); });
  continue_node<continue_msg> playF(g, [&](continue_msg m) { best.resultF = playGroup("groupF", &_groupF); });
  continue_node<continue_msg> playG(g, [&](continue_msg m) { best.resultG = playGroup("groupG", &_groupG); });
  continue_node<continue_msg> playH(g, [&](continue_msg m) { best.resultH = playGroup("groupH", &_groupH); });

  make_edge(init, playA);
  make_edge(init, playB);
  make_edge(init, playC);
  make_edge(init, playD);
  make_edge(init, playE);
  make_edge(init, playF);
  make_edge(init, playG);
  make_edge(init, playH);

  init.try_put(continue_msg());

  g.wait_for_all();

  return best;
}

Quarter WM::playRoundOfBest16(const Best16 &best)
{
  Quarter quarter;

  quarter.result1 = _simulator->playMatch(best.resultC.winner, best.resultD.second);
  quarter.result2 = _simulator->playMatch(best.resultA.winner, best.resultB.second);
  quarter.result3 = _simulator->playMatch(best.resultB.winner, best.resultA.second);
  quarter.result4 = _simulator->playMatch(best.resultD.winner, best.resultC.second);
  quarter.result5 = _simulator->playMatch(best.resultE.winner, best.resultF.second);
  quarter.result6 = _simulator->playMatch(best.resultG.winner, best.resultH.second);
  quarter.result7 = _simulator->playMatch(best.resultF.winner, best.resultE.second);
  quarter.result8 = _simulator->playMatch(best.resultH.winner, best.resultG.second);

  std::vector<Result> results{quarter.result1,
                              quarter.result2,
                              quarter.result3,
                              quarter.result4,
                              quarter.result5,
                              quarter.result6,
                              quarter.result7,
                              quarter.result8};
  _visualizer->visualizeStage("Round Of Best 16", results);

  return quarter;
}

Semi WM::playQuarterFinals(const Quarter &quarter)
{
  Semi semi;

  semi.result1 = _simulator->playMatch(quarter.result1.winner, quarter.result2.winner);
  semi.result2 = _simulator->playMatch(quarter.result5.winner, quarter.result6.winner);
  semi.result3 = _simulator->playMatch(quarter.result7.winner, quarter.result8.winner);
  semi.result4 = _simulator->playMatch(quarter.result3.winner, quarter.result4.winner);

  std::vector<Result> results{semi.result1, semi.result2, semi.result3, semi.result4};
  _visualizer->visualizeStage("Quarter Finals", results);

  return semi;
}

Final WM::playSemiFinals(const Semi &semi)
{
  Final final;

  final.result1 = _simulator->playMatch(semi.result1.winner, semi.result2.winner);
  final.result2 = _simulator->playMatch(semi.result4.winner, semi.result3.winner);

  std::vector<Result> results{final.result1, final.result2};
  _visualizer->visualizeStage("Semi Finals", results);

  return final;
}

Team *WM::playFinal(const Final &final)
{
  std::vector result{_simulator->playMatch(final.result1.winner, final.result2.winner)};
  _visualizer->visualizeStage("Final", result);
  return result.at(0).winner;
}

Result WM::playGroup(const std::string &groupName, Group *group)
{
  std::vector<Result> results(6);

    results[0] = _simulator->playMatch(group->team1.get(), group->team2.get());
    results[1] = _simulator->playMatch(group->team3.get(), group->team4.get());
    results[2] = _simulator->playMatch(group->team1.get(), group->team3.get());
    results[3] = _simulator->playMatch(group->team4.get(), group->team2.get());
    results[4] = _simulator->playMatch(group->team4.get(), group->team1.get());
    results[5] = _simulator->playMatch(group->team2.get(), group->team3.get());

  _visualizer->visualizeStage(groupName, results);

  std::vector<Team *> teams{
    group->team1.get(), group->team2.get(), group->team3.get(), group->team4.get()};
  std::sort(std::begin(teams), std::end(teams), [](Team *a, Team *b) {
    return (a->points.get() > b->points.get() ||
            (a->points.get() == b->points.get() &&
             (a->scored.get() - a->got.get()) > (b->scored.get() - b->got.get())));
  });

  Result result;
  result.winner = teams.at(0);
  result.second = teams.at(1);

  return result;
}

Team* WM::playWM()
{
  _groupA = createGroup("Russia", "SaudiArabia", "Egypt", "Uruguay");
  _groupB = createGroup("Portugal", "Spain", "Morocco", "Iran");
  _groupC = createGroup("France", "Australia", "Peru", "Denmark");
  _groupD = createGroup("Argentinia", "Iceland", "Croatia", "Nigeria");
  _groupE = createGroup("Brazil", "Switzerland", "CostaRica", "Serbia");
  _groupF = createGroup("Germany", "Mexico", "Sweden", "SouthKorea");
  _groupG = createGroup("Belgium", "Panama", "Tunisia", "England");
  _groupH = createGroup("Poland", "Senegal", "Colombia", "Japan");

  auto best16 = playGroupPhase();
  auto quarter = playRoundOfBest16(best16);
  auto semi = playQuarterFinals(quarter);
  auto final = playSemiFinals(semi);
  auto winner = playFinal(final);

  return winner;
}

Group WM::createGroup(std::string team1,
                      std::string team2,
                      std::string team3,
                      std::string team4) const
{
  Group group;
  group.team1 = _simulator->getTeam(TeamName(team1));
  group.team2 = _simulator->getTeam(TeamName(team2));
  group.team3 = _simulator->getTeam(TeamName(team3));
  group.team4 = _simulator->getTeam(TeamName(team4));
  return group;
}

}  // namespace ws::wm
