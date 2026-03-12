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
  if constexpr(Size == 2) {
    if(unlikely(address & 1)) {
      exception.address<Read>(address);  //nop
      return 0;
    }
  }
  if constexpr(Size == 4) {
    if(unlikely(address & 3)) {
      exception.address<Read>(address);
      return 0;  //nop
    }
  }

  u32 v = test_read(tptr, address, Size);

  accruedCycles++;
  return v;
}

//write data to the bus
template<u32 Size>
inline auto CPU::write(u32 address, u32 data) -> void {
  //printf("\nCPU write!");
  if constexpr(Size == 2) {
    if(unlikely(address & 1)) {
      exception.address<Write>(address);  //nop
      return;
    }
  }
  if constexpr(Size == 4) {
    if(unlikely(address & 3)) {
      exception.address<Write>(address);
      return;  //nop
    }
  }
  test_write(tptr, address, Size, data);
  accruedCycles++;
}
