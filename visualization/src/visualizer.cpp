#include "visualizer.h"
#include <model.h>
#include <iostream>
#include <vector>

using std::cout;

namespace {

void replaceAll(std::string &str, const std::string &from, const std::string &to)
{
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();  // Handles case where 'to' is a substring of 'from'
  }
}

std::string transform(const ws::TeamName &name)
{
  std::string tmp{name.get()};
  replaceAll(tmp, "ä", "ae");
  replaceAll(tmp, "ö", "oe");
  replaceAll(tmp, "ü", "ue");
  replaceAll(tmp, "Ä", "Ae");
  replaceAll(tmp, "Ö", "Oe");
  replaceAll(tmp, "Ü", "Ue");
  replaceAll(tmp, "ß", "ss");
  tmp.resize(11, ' ');
  return tmp;
}

std::string transform(const ws::NumGoals &goals)
{
  return ((goals.get() < 10) ? " " : "") + std::to_string(goals.get());
}

void visualizeGames(const std::vector<ws::wm::Result> &results)
{
  constexpr char limiter[] = "------------------";

  for (auto i{0}; i < results.size(); ++i) { cout << limiter << ' '; }
  cout << '\n';
  for (const auto &result : results) {
    cout << "| " << transform(result.winner->name) << ' ' << transform(result.winnerGoals) << " | ";
  }
  cout << '\n';
  for (const auto &result : results) {
    cout << "| " << transform(result.second->name) << ' ' << transform(result.secondGoals) << " | ";
  }
  cout << '\n';
  for (auto i{0}; i < results.size(); ++i) { cout << limiter << ' '; }
  cout << '\n';
}

}  // namespace

namespace ws::vis {

void Visualizer::visualizeStage(const std::string &stageName, const std::vector<Result> &results)
{
  cout << stageName << '\n';
  visualizeGames(results);
}

}  // namespace ws::vis
