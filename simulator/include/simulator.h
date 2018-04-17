#pragma once

#include "dbloader.h"

namespace ws {
namespace sim {

  using ws::wm::Result;
  using ws::db::Loader;

  class Simulator
{
  public:
  explicit Simulator(Loader *loader) : _loader(loader) {}
  ~Simulator() = default;

  Result playMatch(Team *team1, Team *team2);
  std::unique_ptr<Team> getTeam(TeamName name);

  private:
  Loader *_loader;
};

}  // namespace sim
}  // namespace ws
