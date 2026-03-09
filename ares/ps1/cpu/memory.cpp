//read code from the bus
inline auto CPU::fetch(u32 address) -> u32 {
  printf("\nCPU fetch!");
  return 0;
}

//peek at the next instruction, does not consume cycles
inline auto CPU::peek(u32 address) -> u32 {
  printf("\nCPU peek!");
  return 0;
}

//read data from the bus
template<u32 Size>
inline auto CPU::read(u32 address) -> u32 {
  printf("\nCPU read!");
  return 0;
}

//write data to the bus
template<u32 Size>
inline auto CPU::write(u32 address, u32 data) -> void {
  printf("\nCPU write!");
}
