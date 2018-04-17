#include <array>

#include "simulator.h"

namespace {

using ws::NumGoals;
using ws::Player;

NumGoals getGoalsPerGame(const std::vector<std::unique_ptr<Player>> players)
{
  NumGoals goalsPerGame{0};

  for (const auto &player : players) {
    if (player->totalGames.get()) {
      goalsPerGame.get() += player->totalGoals.get() / player->totalGames.get();
    }
  }

  return goalsPerGame;
}

}  // namespace

namespace ws {
namespace sim {

  Result Simulator::playMatch(Team *team1, Team *team2)
{
  auto matches = _loader->getMatchesBetween(*team1, *team2);
  NumGoals goals1{0};
  NumGoals goals2{0};

  for (auto &match : matches) {
    std::vector<std::unique_ptr<Player>> players1, players2;
    std::tie(players1, players2) = _loader->getPlayersOfMatch(match);
    goals1.get() += getGoalsPerGame(std::move(players1)).get() + match.t1Goals.get();
    goals2.get() += getGoalsPerGame(std::move(players2)).get() + match.t2Goals.get();

    if (match.t1Goals.get() > match.t2Goals.get()) {
      team1->points.get() += 3;
    } else if (match.t2Goals.get() > match.t1Goals.get()) {
      team2->points.get() += 3;

    } else {
      team1->points.get() += 1;
      team2->points.get() += 1;
    }
  }

  if (goals1.get() >= goals2.get())
    return Result(team1, team2, goals1, goals2);
  else
    return Result(team2, team1, goals2, goals1);
}

std::unique_ptr<Team> Simulator::getTeam(TeamName name) {
    return _loader->getTeam(name);
  }

}  // namespace sim
}  // namespace ws
