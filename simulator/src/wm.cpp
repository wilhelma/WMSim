#include "wm.h"
#include <wm.h>
#include <algorithm>

namespace ws::wm {

Best16 WM::playGroupPhase()
{
  Best16 best;

  best.resultA = playGroup("Group A", &_groupA);
  best.resultB = playGroup("Group B", &_groupB);
  best.resultC = playGroup("Group C", &_groupC);
  best.resultD = playGroup("Group D", &_groupD);
  best.resultE = playGroup("Group E", &_groupE);
  best.resultF = playGroup("Group F", &_groupF);
  best.resultG = playGroup("Group G", &_groupG);
  best.resultH = playGroup("Group H", &_groupH);

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

void WM::playWM()
{
  _groupA = createGroup(
    TeamName("Frankreich"), TeamName("Rumänien"), TeamName("Albanien"), TeamName("Schweiz"));
  _groupB =
    createGroup(TeamName("England"), TeamName("Rußland"), TeamName("Wales"), TeamName("Slowakei"));
  _groupC = createGroup(
    TeamName("Deutschland"), TeamName("Ukraine"), TeamName("Polen"), TeamName("Nordirland"));
  _groupD = createGroup(
    TeamName("Spanien"), TeamName("Tschechien"), TeamName("Türkei"), TeamName("Kroatien"));
  _groupE =
    createGroup(TeamName("Belgien"), TeamName("Italien"), TeamName("Irland"), TeamName("Schweden"));
  _groupF = createGroup(
    TeamName("Portugal"), TeamName("Island"), TeamName("Österreich"), TeamName("Ungarn"));
  _groupG = createGroup(
    TeamName("Bulgarien"), TeamName("Dänemark"), TeamName("Finnland"), TeamName("Griechenland"));
  _groupH = createGroup(
    TeamName("Niederlande"), TeamName("Schottland"), TeamName("Israel"), TeamName("Lettland"));

  auto best16  = playGroupPhase();
  auto quarter = playRoundOfBest16(best16);
  auto semi    = playQuarterFinals(quarter);
  auto final   = playSemiFinals(semi);
  auto winner  = playFinal(final);
}

Group WM::createGroup(TeamName team1, TeamName team2, TeamName team3, TeamName team4) const
{
  Group group;
  group.team1 = _simulator->getTeam(team1);
  group.team2 = _simulator->getTeam(team2);
  group.team3 = _simulator->getTeam(team3);
  group.team4 = _simulator->getTeam(team4);
  return group;
}

}  // namespace ws::wm
