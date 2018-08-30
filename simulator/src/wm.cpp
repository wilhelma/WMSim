#include "wm.h"
#include <wm.h>
#include <algorithm>
#include <future>

namespace ws::wm {

Best16 WM::playGroupPhase()
{
  Best16 best;

  auto futA = std::async(std::launch::async, [&] { return playGroup("GroupA", &_groupA); });
  auto futB = std::async(std::launch::async, [&] { return playGroup("GroupB", &_groupB); });
  auto futC = std::async(std::launch::async, [&] { return playGroup("GroupC", &_groupC); });
  auto futD = std::async(std::launch::async, [&] { return playGroup("GroupD", &_groupD); });
  auto futE = std::async(std::launch::async, [&] { return playGroup("GroupE", &_groupE); });
  auto futF = std::async(std::launch::async, [&] { return playGroup("GroupF", &_groupF); });
  auto futG = std::async(std::launch::async, [&] { return playGroup("GroupG", &_groupG); });
  auto futH = std::async(std::launch::async, [&] { return playGroup("GroupH", &_groupH); });

  best.resultA = futA.get();
  best.resultB = futB.get();
  best.resultC = futC.get();
  best.resultD = futD.get();
  best.resultE = futE.get();
  best.resultF = futF.get();
  best.resultG = futG.get();
  best.resultH = futH.get();

  //  best.resultA = playGroup("Group A", &_groupA);
  //  best.resultB = playGroup("Group B", &_groupB);
  //  best.resultC = playGroup("Group C", &_groupC);
  //  best.resultD = playGroup("Group D", &_groupD);
  //  best.resultE = playGroup("Group E", &_groupE);
  //  best.resultF = playGroup("Group F", &_groupF);
  //  best.resultG = playGroup("Group G", &_groupG);
  //  best.resultH = playGroup("Group H", &_groupH);

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
  std::vector<Result> results;

  results.push_back(_simulator->playMatch(group->team1.get(), group->team2.get()));
  results.push_back(_simulator->playMatch(group->team3.get(), group->team4.get()));
  results.push_back(_simulator->playMatch(group->team1.get(), group->team3.get()));
  results.push_back(_simulator->playMatch(group->team4.get(), group->team2.get()));
  results.push_back(_simulator->playMatch(group->team4.get(), group->team1.get()));
  results.push_back(_simulator->playMatch(group->team2.get(), group->team3.get()));

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
  _groupA = createGroup("Frankreich", "Rumänien", "Albanien", "Schweiz");
  _groupB = createGroup("England", "Rußland", "Wales", "Slowakei");
  _groupC = createGroup("Deutschland", "Ukraine", "Polen", "Nordirland");
  _groupD = createGroup("Spanien", "Tschechien", "Türkei", "Kroatien");
  _groupE = createGroup("Belgien", "Italien", "Irland", "Schweden");
  _groupF = createGroup("Portugal", "Island", "Österreich", "Ungarn");
  _groupG = createGroup("Bulgarien", "Dänemark", "Finnland", "Griechenland");
  _groupH = createGroup("Niederlande", "Schottland", "Israel", "Lettland");

  auto best16  = playGroupPhase();
  auto quarter = playRoundOfBest16(best16);
  auto semi    = playQuarterFinals(quarter);
  auto final   = playSemiFinals(semi);
  auto winner  = playFinal(final);

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
