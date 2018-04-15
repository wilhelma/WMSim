#pragma once

#include <string>

namespace ws {

template <typename T>
class NamedType
{
 public:
  explicit NamedType(T const &value) : value_(value) {}

  explicit NamedType(T &&value) : value_(std::move(value)) {}

  T &get() { return value_; }

  T const &get() const { return value_; }

 private:
  T value_;
};

using TeamName          = NamedType<std::string>;
using TeamId            = NamedType<int>;
using PlayerId          = NamedType<int>;
using MatchId           = NamedType<int>;
using NumParticipations = NamedType<int>;
using NumPoints         = NamedType<int>;
using NumGames          = NamedType<int>;
using NumGoals          = NamedType<int>;
using Year              = NamedType<int>;
enum class Round : int { FirstStage = 0, FinalRound = 1 };

struct Player {
  PlayerId id;
  NumGames totalGames, finalGames;
  NumGoals totalGoals, finalGoals;

  Player(
    PlayerId id, NumGames totalGames, NumGames finalGames, NumGoals totalGoals, NumGoals finalGoals)
    : id(id),
      totalGames(totalGames),
      finalGames(finalGames),
      totalGoals(totalGoals),
      finalGoals(finalGoals)
  {}
};

struct Team {
  TeamName name;
  TeamId id;
  NumParticipations participated;
  NumPoints points;
  NumGames total, won, draw, lost;
  NumGoals scored, got;

  Team(TeamName name,
       TeamId id,
       NumParticipations participated,
       NumGames total,
       NumGames won,
       NumGames draw,
       NumGames lost,
       NumGoals scored,
       NumGoals got)
    : name(name),
      id(id),
      participated(participated),
      points(0),
      total(total),
      won(won),
      draw(draw),
      lost(lost),
      scored(scored),
      got(got)
  {}

  NumPoints getPoints() const { return NumPoints(won.get() * 2 + draw.get()); }
};

struct Match {
  MatchId id;
  Round stage;
  Year wm;
  NumGoals t1Goals, t2Goals;
  TeamId t1Id, t2Id;

  Match(
    MatchId id, Round stage, Year wm, NumGoals t1Goals, NumGoals t2Goals, TeamId t1Id, TeamId t2Id)
    : id(id), stage(stage), wm(wm), t1Goals(t1Goals), t2Goals(t2Goals), t1Id(t1Id), t2Id(t2Id)
  {}
};

namespace wm {
struct Result {
  Team *winner, *second;
  NumGoals winnerGoals, secondGoals;
  explicit Result(Team *winner, Team *second, NumGoals winnerGoals, NumGoals secondGoals)
    : winner(winner), second(second), winnerGoals(winnerGoals), secondGoals(secondGoals)
  {}
  Result() : winner(nullptr), second(nullptr), winnerGoals(NumGoals(0)), secondGoals(NumGoals(0)) {}
  ~Result() = default;
};

struct Group {
  std::unique_ptr<Team> team1, team2, team3, team4;
};

struct Best16 {
  Result resultA, resultB, resultC, resultD, resultE, resultF, resultG, resultH;
};

struct Quarter {
  Result result1, result2, result3, result4, result5, result6, result7, result8;
};

struct Semi {
  Result result1, result2, result3, result4;
};

struct Final {
  Result result1, result2;
};
}  // namespace wm

}  // namespace ws
