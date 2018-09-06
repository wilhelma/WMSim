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

  auto fut1 = std::async(std::launch::async, [&] {
    return _simulator->playMatch(best.resultC.winner, best.resultD.second);
  });
  auto fut2 = std::async(std::launch::async, [&] {
    return _simulator->playMatch(best.resultA.winner, best.resultB.second);
  });
  auto fut3 = std::async(std::launch::async, [&] {
    return _simulator->playMatch(best.resultB.winner, best.resultA.second);
  });
  auto fut4 = std::async(std::launch::async, [&] {
    return _simulator->playMatch(best.resultD.winner, best.resultC.second);
  });
  auto fut5 = std::async(std::launch::async, [&] {
    return _simulator->playMatch(best.resultE.winner, best.resultF.second);
  });
  auto fut6 = std::async(std::launch::async, [&] {
    return _simulator->playMatch(best.resultG.winner, best.resultH.second);
  });
  auto fut7 = std::async(std::launch::async, [&] {
    return _simulator->playMatch(best.resultF.winner, best.resultE.second);
  });
  auto fut8 = std::async(std::launch::async, [&] {
    return _simulator->playMatch(best.resultH.winner, best.resultG.second);
  });

  quarter.result1 = fut1.get();
  quarter.result2 = fut2.get();
  quarter.result3 = fut3.get();
  quarter.result4 = fut4.get();
  quarter.result5 = fut5.get();
  quarter.result6 = fut6.get();
  quarter.result7 = fut7.get();
  quarter.result8 = fut8.get();

  //  quarter.result1 = _simulator->playMatch(best.resultC.winner, best.resultD.second;
  //  quarter.result2 = _simulator->playMatch(best.resultA.winner, best.resultB.second);
  //  quarter.result3 = _simulator->playMatch(best.resultB.winner, best.resultA.second);
  //  quarter.result4 = _simulator->playMatch(best.resultD.winner, best.resultC.second);
  //  quarter.result5 = _simulator->playMatch(best.resultE.winner, best.resultF.second);
  //  quarter.result6 = _simulator->playMatch(best.resultG.winner, best.resultH.second);
  //  quarter.result7 = _simulator->playMatch(best.resultF.winner, best.resultE.second);
  //  quarter.result8 = _simulator->playMatch(best.resultH.winner, best.resultG.second);

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

  auto fut1 = std::async(std::launch::async, [&] {
    return _simulator->playMatch(quarter.result1.winner, quarter.result2.winner);
  });
  auto fut2 = std::async(std::launch::async, [&] {
    return _simulator->playMatch(quarter.result5.winner, quarter.result6.winner);
  });
  auto fut3 = std::async(std::launch::async, [&] {
    return _simulator->playMatch(quarter.result7.winner, quarter.result8.winner);
  });
  auto fut4 = std::async(std::launch::async, [&] {
    return _simulator->playMatch(quarter.result3.winner, quarter.result4.winner);
  });

  semi.result1 = fut1.get();
  semi.result2 = fut2.get();
  semi.result3 = fut3.get();
  semi.result4 = fut4.get();

  //  semi.result1 = _simulator->playMatch(quarter.result1.winner, quarter.result2.winner);
  //  semi.result2 = _simulator->playMatch(quarter.result5.winner, quarter.result6.winner);
  //  semi.result3 = _simulator->playMatch(quarter.result7.winner, quarter.result8.winner);
  //  semi.result4 = _simulator->playMatch(quarter.result3.winner, quarter.result4.winner);

  std::vector<Result> results{semi.result1, semi.result2, semi.result3, semi.result4};
  _visualizer->visualizeStage("Quarter Finals", results);

  return semi;
}

Final WM::playSemiFinals(const Semi &semi)
{
  Final final;

  auto fut1 = std::async(std::launch::async, [&] {
    return _simulator->playMatch(semi.result1.winner, semi.result2.winner);
  });
  auto fut2 = std::async(std::launch::async, [&] {
    return _simulator->playMatch(semi.result4.winner, semi.result3.winner);
  });

  final.result1 = fut1.get();
  final.result2 = fut2.get();

  //  final.result1 = _simulator->playMatch(semi.result1.winner, semi.result2.winner);
  //  final.result2 = _simulator->playMatch(semi.result4.winner, semi.result3.winner);

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

  graph h;
  broadcast_node<continue_msg> init(h);

  continue_node<continue_msg>
      playGame1(h, [&](continue_msg msg) {
    results[0] = _simulator->playMatch(group->team1.get(), group->team2.get());
  }),
      playGame2(h, [&](continue_msg msg) {
    results[1] = _simulator->playMatch(group->team3.get(), group->team4.get());
  }),
      playGame3(h, [&](continue_msg msg) {
    results[2] = _simulator->playMatch(group->team1.get(), group->team3.get());
  }),
      playGame4(h, [&](continue_msg msg) {
    results[3] = _simulator->playMatch(group->team4.get(), group->team2.get());
  }),
      playGame5(h, [&](continue_msg msg) {
    results[4] = _simulator->playMatch(group->team4.get(), group->team1.get());
  }),
      playGame6(h, [&](continue_msg msg) {
    results[5] = _simulator->playMatch(group->team2.get(), group->team3.get());
  });

  make_edge(init, playGame1);
  make_edge(init, playGame2);
  make_edge(playGame1, playGame3);
  make_edge(playGame1, playGame4);
  make_edge(playGame2, playGame3);
  make_edge(playGame2, playGame4);
  make_edge(playGame3, playGame5);
  make_edge(playGame3, playGame6);
  make_edge(playGame4, playGame5);
  make_edge(playGame4, playGame6);

  init.try_put(continue_msg());
  h.wait_for_all();

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

#define GROUP(name, var) tbb::flow::function_node<continue_msg, Result> name(g, 1, [&](continue_msg m) { return playGroup("name", &var); });

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

  graph g;
  broadcast_node<continue_msg> init(g);

  // group phase
  function_node<continue_msg, Result> playA(g, 1, [&](continue_msg m) { return playGroup("groupA", &_groupA); });
  function_node<continue_msg, Result> playB(g, 1, [&](continue_msg m) { return playGroup("groupB", &_groupB); });
  function_node<continue_msg, Result> playC(g, 1, [&](continue_msg m) { return playGroup("groupC", &_groupC); });
  function_node<continue_msg, Result> playD(g, 1, [&](continue_msg m) { return playGroup("groupD", &_groupD); });
  function_node<continue_msg, Result> playE(g, 1, [&](continue_msg m) { return playGroup("groupE", &_groupE); });
  function_node<continue_msg, Result> playF(g, 1, [&](continue_msg m) { return playGroup("groupF", &_groupF); });
  function_node<continue_msg, Result> playG(g, 1, [&](continue_msg m) { return playGroup("groupG", &_groupG); });
  function_node<continue_msg, Result> playH(g, 1, [&](continue_msg m) { return playGroup("groupH", &_groupH); });

  make_edge(init, playA);
  make_edge(init, playB);
  make_edge(init, playC);
  make_edge(init, playD);
  make_edge(init, playE);
  make_edge(init, playF);
  make_edge(init, playG);
  make_edge(init, playH);

  // round of best 16
  join_node<tuple<Result, Result>> join_1(g), join_2(g), join_3(g), join_4(g), join_5(g), join_6(g), join_7(g), join_8(g);
  function_node<tuple<Result, Result>, Result>
      playBest16_1(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).second);
  }),
      playBest16_2(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).second);
  }),
      playBest16_3(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).second);
  }),
      playBest16_4(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).second);
  }),
      playBest16_5(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).second);
  }),
      playBest16_6(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).second);
  }),
      playBest16_7(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).second);
  }),
      playBest16_8(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).second);
  });

  make_edge(playC, input_port<0>(join_1));
  make_edge(playD, input_port<1>(join_1));
  make_edge(join_1, playBest16_1);

  make_edge(playA, input_port<0>(join_2));
  make_edge(playB, input_port<1>(join_2));
  make_edge(join_2, playBest16_2);

  make_edge(playB, input_port<0>(join_3));
  make_edge(playA, input_port<1>(join_3));
  make_edge(join_3, playBest16_3);

  make_edge(playD, input_port<0>(join_4));
  make_edge(playC, input_port<1>(join_4));
  make_edge(join_4, playBest16_4);

  make_edge(playE, input_port<0>(join_5));
  make_edge(playF, input_port<1>(join_5));
  make_edge(join_5, playBest16_5);

  make_edge(playG, input_port<0>(join_6));
  make_edge(playH, input_port<1>(join_6));
  make_edge(join_6, playBest16_6);

  make_edge(playF, input_port<0>(join_7));
  make_edge(playE, input_port<1>(join_7));
  make_edge(join_7, playBest16_7);

  make_edge(playH, input_port<0>(join_8));
  make_edge(playG, input_port<1>(join_8));
  make_edge(join_8, playBest16_8);

  // quarter final
  join_node<tuple<Result, Result>> join4_1(g), join4_2(g), join4_3(g), join4_4(g);
  function_node<tuple<Result, Result>, Result>
      playQuarter_1(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).winner);
  }),
      playQuarter_2(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).winner);
  }),
      playQuarter_3(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).winner);
  }),
      playQuarter_4(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).winner);
  });

  make_edge(playBest16_1, input_port<0>(join4_1));
  make_edge(playBest16_2, input_port<1>(join4_1));
  make_edge(join4_1, playQuarter_1);

  make_edge(playBest16_5, input_port<0>(join4_2));
  make_edge(playBest16_6, input_port<1>(join4_2));
  make_edge(join4_2, playQuarter_2);

  make_edge(playBest16_7, input_port<0>(join4_3));
  make_edge(playBest16_8, input_port<1>(join4_3));
  make_edge(join4_3, playQuarter_3);

  make_edge(playBest16_3, input_port<0>(join4_4));
  make_edge(playBest16_4, input_port<1>(join4_4));
  make_edge(join4_4, playQuarter_4);

  // semi final
  join_node<tuple<Result, Result>> join2_1(g), join2_2(g);
  function_node<tuple<Result, Result>, Result>
      playSemi_1(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).winner);
  }),
      playSemi_2(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).winner);
  });

  make_edge(playQuarter_1, input_port<0>(join2_1));
  make_edge(playQuarter_2, input_port<1>(join2_1));
  make_edge(join2_1, playSemi_1);

  make_edge(playQuarter_4, input_port<0>(join2_2));
  make_edge(playQuarter_3, input_port<1>(join2_2));
  make_edge(join2_2, playSemi_2);

  // final
  join_node<tuple<Result, Result>> join1(g);
  function_node<tuple<Result, Result>, Result>
      playFinalNode(g, 1, [&](const tuple<Result, Result> &result) {
    return _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).winner);
  });

  make_edge(playSemi_1, input_port<0>(join1));
  make_edge(playSemi_2, input_port<1>(join1));
  make_edge(join1, playFinalNode);

  init.try_put(continue_msg());

  g.wait_for_all();

  Result result;

  playFinalNode.try_get(result);

  return result.winner;
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
