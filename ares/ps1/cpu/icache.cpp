//used by the interpreter to fully emulate the instruction cache
inline auto CPU::InstructionCache::fetch(u32 address) -> u32 {
  printf("\nWARN ICACHE FETCH!");
  return 0;
}

inline auto CPU::InstructionCache::read(u32 address) -> u32 {
  printf("\nWARN ICACHE READ!");
  return 0;
}

inline auto CPU::InstructionCache::invalidate(u32 address) -> void {
  auto& line = lines[address >> 4 & 0xff];
  line.tag |= 1;  //mark tag as invalid
}

auto CPU::InstructionCache::enable(bool enable) -> void {
  if(!enable) {
    for(auto& line : lines) line.tag |=  2;  //mark tag as disabled
  } else {
    for(auto& line : lines) line.tag &= ~2;  //mark tag as enabled
  }
}

auto CPU::InstructionCache::power(bool reset) -> void {
  for(auto& line : lines) {
    line.tag = 1;  //mark tag as invalid
    for(auto& word : line.words) word = random();
  }
}
