inline auto Bus::mmio(u32 address) -> Memory::Interface& {
  return unmapped;
}

template<bool isWrite, bool isDMA>
auto Bus::calcAccessTime(u32 address, u32 bytesCount) -> u32 const {
  return 1;
}

template<u32 Size>
inline auto Bus::read(u32 address) -> u32 {
  return 0;
}

template<u32 Size>
inline auto Bus::write(u32 address, u32 data) -> void {
}
