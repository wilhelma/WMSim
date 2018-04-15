#pragma once

#include <model.h>
#include <sqlite3.h>
#include <fstream>
#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

namespace ws {
namespace db {

class Loader
{
 public:
  /*!
   * @param database The path to the soccer database.
   */
  explicit Loader(const char *database) : _database(database) {}
  ~Loader();

  /*!
   * @return returns the sqlite3 connection for the current thread.
   */
  sqlite3 *getConnection();

  std::unique_ptr<Team> getTeam(const TeamName &);
  Player getPlayer(PlayerId);
  std::vector<Match> getMatchesBetween(const Team &team1, const Team &team2);
  std::tuple<std::vector<std::unique_ptr<Player>>, std::vector<std::unique_ptr<Player>>>
  getPlayersOfMatch(const Match &);

 private:
  const char *_database;
  std::unordered_map<std::thread::id, sqlite3 *> _connections;

  std::unique_ptr<Player> fillPlayer(PlayerId id);
};

}  // namespace db
}  // namespace ws