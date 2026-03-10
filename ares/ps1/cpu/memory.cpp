//read code from the bus
inline auto CPU::fetch(u32 address) -> u32 {
  //printf("\nCPU fetch!");
  u32 v = test_fetch(tptr, address, 4, true);
  accruedCycles++;
  return v;
}

//peek at the next instruction, does not consume cycles
inline auto CPU::peek(u32 address) -> u32 {
  u32 v = test_fetch(tptr, address, 4, false);
  //printf("\nCPU peek!");
  return v;
}

//read data from the bus
template<u32 Size>
inline auto CPU::read(u32 address) -> u32 {
  //printf("\nCPU read %d!", Size);
  u32 v = test_read(tptr, address, Size);
  accruedCycles++;
  return v;
}

//write data to the bus
template<u32 Size>
inline auto CPU::write(u32 address, u32 data) -> void {
  //printf("\nCPU write!");
  test_write(tptr, address, Size, data);
  accruedCycles++;
}
