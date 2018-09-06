#include "dbloader.h"
#include <future>

namespace {
sqlite3 *openConnection(const char *database)
{
  sqlite3 *connection{};
  int rc = sqlite3_open_v2(database, &connection, SQLITE_OPEN_READONLY | SQLITE_OPEN_NOMUTEX, NULL);
  if (rc) {
    sqlite3_close(connection);
    throw std::runtime_error(std::string("Error while opening database") + std::string(database));
  }

  return connection;
}
}  // namespace

namespace ws {
namespace db {

Loader::~Loader()
{
  for (auto &it : _connections) { sqlite3_close(it.second); }
}

sqlite3 *Loader::getConnection()
{
  auto threadId = std::this_thread::get_id();
  auto connIt   = _connections.find(threadId);
  if (connIt == std::end(_connections)) {
    _connections[threadId] = openConnection(_database);
    connIt                 = _connections.find(threadId);
  }

  return connIt->second;
}

std::unique_ptr<Team> Loader::getTeam(const TeamName &teamName)
{
  sqlite3_stmt *stmt;

  std::string select{"SELECT * FROM laender WHERE Land=\"" + teamName.get() + "\""};

  if (sqlite3_prepare_v2(getConnection(), select.c_str(), -1, &stmt, NULL) != SQLITE_OK ||
      sqlite3_step(stmt) != SQLITE_ROW) {
    throw std::runtime_error("No result returned from " + select);
  }

  auto team = std::make_unique<Team>(teamName,
                                     TeamId(sqlite3_column_int(stmt, 3)),
                                     NumParticipations(sqlite3_column_int(stmt, 13)),
                                     NumGames(sqlite3_column_int(stmt, 15)),
                                     NumGames(sqlite3_column_int(stmt, 16)),
                                     NumGames(sqlite3_column_int(stmt, 17)),
                                     NumGames(sqlite3_column_int(stmt, 18)),
                                     NumGoals(sqlite3_column_int(stmt, 19)),
                                     NumGoals(sqlite3_column_int(stmt, 20)));

  sqlite3_finalize(stmt);

  return team;
}

Player Loader::getPlayer(PlayerId id)
{
  sqlite3_stmt *stmt;

  std::string select{"SELECT * FROM spieler WHERE nr=" + std::to_string(id.get())};

  if (sqlite3_prepare_v2(getConnection(), select.c_str(), -1, &stmt, NULL) != SQLITE_OK ||
      sqlite3_step(stmt) != SQLITE_ROW) {
    throw std::runtime_error("No result returned from " + select);
  }

  Player player(id,
                NumGames(sqlite3_column_int(stmt, 3)),
                NumGames(sqlite3_column_int(stmt, 7)),
                NumGoals(sqlite3_column_int(stmt, 4)),
                NumGoals(sqlite3_column_int(stmt, 8)));

  sqlite3_finalize(stmt);

  return player;
}

std::vector<Match> Loader::getMatchesBetween(const Team &team1, const Team &team2)
{
  std::vector<Match> matches;

  auto selectMatches = [&](const Team &t1, const Team &t2) {
    auto connection = getConnection();
    sqlite3_stmt *stmt;
    std::string select{"SELECT * FROM Paarungen WHERE Heim=\"" + t1.name.get() + "\" AND Gast=\"" +
                       t2.name.get() + "\""};

    if (sqlite3_prepare_v2(connection, select.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
      while (sqlite3_step(stmt) == SQLITE_ROW) {
        matches.emplace_back(
          Match(MatchId(sqlite3_column_int(stmt, 13)),
                Round(sqlite3_column_int(stmt, 16)),
                Year(sqlite3_column_int(stmt, 0)),
                NumGoals(sqlite3_column_int(stmt, (t1.id.get() == team1.id.get()) ? 3 : 4)),
                NumGoals(sqlite3_column_int(stmt, (t1.id.get() == team1.id.get()) ? 4 : 3)),
                TeamId(sqlite3_column_int(stmt, (t1.id.get() == team1.id.get()) ? 11 : 12)),
                TeamId(sqlite3_column_int(stmt, (t1.id.get() == team1.id.get()) ? 12 : 11))));
      }
      sqlite3_finalize(stmt);
    }
  };

  selectMatches(team1, team2);
  selectMatches(team2, team1);

  return matches;
}

std::tuple<std::vector<std::unique_ptr<Player>>, std::vector<std::unique_ptr<Player>>>
Loader::getPlayersOfMatch(const Match &match)
{
  auto getPlayersOfTeam = [&](TeamId id) {
    std::vector<std::unique_ptr<Player>> players;
    sqlite3_stmt *stmt;
    auto connection = getConnection();

    std::string select{"SELECT * FROM aufstellung WHERE SpielNr=" + std::to_string(match.id.get()) +
                       " AND LandNr=" + std::to_string(id.get())};

    if (sqlite3_prepare_v2(connection, select.c_str(), -1, &stmt, NULL) == SQLITE_OK) {
      int numPlayers{0};
      std::vector<std::future<void>> futures;
      while (sqlite3_step(stmt) == SQLITE_ROW) {
        futures.emplace_back(std::async(std::launch::async, [&] {
          players.emplace_back(fillPlayer(PlayerId(sqlite3_column_int(stmt, 2))));
        }));
      }
      for (auto &f : futures) { f.get(); }
      sqlite3_finalize(stmt);
    }

    return std::move(players);
  };

  return std::make_tuple(getPlayersOfTeam(match.t1Id), getPlayersOfTeam(match.t2Id));
}

std::unique_ptr<Player> Loader::fillPlayer(PlayerId id)
{
  sqlite3_stmt *stmt;
  std::string select{"SELECT * FROM spieler WHERE nr=" + std::to_string(id.get())};
  auto connection = getConnection();

  if (sqlite3_prepare_v2(connection, select.c_str(), -1, &stmt, NULL) != SQLITE_OK ||
      sqlite3_step(stmt) != SQLITE_ROW) {
    throw std::runtime_error("No result returned from " + select);
  }

  sqlite3_finalize(stmt);

  return std::make_unique<Player>(id,
                                  NumGames(sqlite3_column_int(stmt, 3)),
                                  NumGames(sqlite3_column_int(stmt, 7)),
                                  NumGoals(sqlite3_column_int(stmt, 4)),
                                  NumGoals(sqlite3_column_int(stmt, 8)));
}

}  // namespace db
}  // namespace ws
