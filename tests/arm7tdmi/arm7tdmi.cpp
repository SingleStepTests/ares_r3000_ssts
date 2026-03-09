#include <nall/nall.hpp>
#include <nall/string/markup/json.hpp>
using namespace nall;

#include <nall/main.hpp>

#include <ares/ares.hpp>
#include <ps1/cpu/cpu.hpp>

auto nall::main(Arguments arguments) -> void {
  CPU cpu;
  cpu.instruction();
  printf("\nYO!");
}
