#include <nall/nall.hpp>
#include <nall/string/markup/json.hpp>
using namespace nall;

#include <nall/main.hpp>

#include "ps1_stub.h"

#include <ares/ares.hpp>

namespace ares::PlayStation {
#include <ps1/cpu/cpu.hpp>
}

auto nall::main(Arguments arguments) -> void {

  ares::PlayStation::CPU cpu;

  cpu.power(false);

  for(int i = 0; i < 10; i++) {
    cpu.instruction();
  }

  print("CPU ran!\n");
}