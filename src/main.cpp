#include <fstream>
#include <iostream>

#include "wm.h"
#include "simulator.h"
#include "dbloader.h"

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "No program argument given!\n";
    abort();
  }

  std::ifstream inputfile(argv[1]);

  if (inputfile.is_open()) {
    ws::db::Loader loader(argv[1]);
    ws::sim::Simulator simulator(&loader);
    ws::wm::WM wm(&simulator);

    wm.playWM();

/*    auto loader = std::make_unique<ws::db::Loader>(argv[1]);
    auto simulator = std::make_unique<ws::sim::Simulator>(loader.get());
    auto wm = std::make_unique<ws::wm::WM>(simulator.get());

    wm->playWM();*/
  } else {
    std::cerr << "Cannot open file " << argv[1];
    return -1;
  }

  return 0;
}