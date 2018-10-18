#include "wm.h"
#include <wm.h>
#include <algorithm>
#include <future>
#include <tbb/flow_graph.h>
#include <tbb/task_scheduler_init.h>

using namespace tbb::flow;

namespace ws {
namespace wm {

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

//    tbb::task_scheduler_init sched(4);

    _groupA = createGroup("Frankreich", "Rumänien", "Albanien", "Schweiz");
    _groupB = createGroup("England", "Rußland", "Wales", "Slowakei");
    _groupC = createGroup("Deutschland", "Ukraine", "Polen", "Nordirland");
    _groupD = createGroup("Spanien", "Tschechoslowakei", "Türkei", "Kroatien");
    _groupE = createGroup("Belgien", "Italien", "Irland", "Schweden");
    _groupF = createGroup("Portugal", "Island", "Österreich", "Ungarn");
    _groupG = createGroup("Belgien", "Italien", "Irland", "Schweden");
    _groupH = createGroup("Portugal", "Island", "Österreich", "Ungarn");

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
    Result finalResult;
    function_node<tuple<Result, Result>, Result>
    playFinalNode(g, 1, [&](const tuple<Result, Result> &result) {
      finalResult = _simulator->playMatch(std::get<0>(result).winner, std::get<1>(result).winner);
      return finalResult;
    });

    make_edge(playSemi_1, input_port<0>(join1));
    make_edge(playSemi_2, input_port<1>(join1));
    make_edge(join1, playFinalNode);

    init.try_put(continue_msg());

    g.wait_for_all();

    return finalResult.winner;
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

}  // namespace wm
}  // namespace ws
