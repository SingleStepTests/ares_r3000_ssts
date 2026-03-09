#pragma once

#include <cstdint>

namespace ares::PlayStation {

    using u8  = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;

    struct DummyScheduler {
        void step(u32 clocks) {}
    };

    struct DummyInterrupt {
        void raise(u32) {}
    };

    struct DummyBus {

        u8 memory[2 * 1024 * 1024]{};  // 2MB RAM is enough for testing

        template<typename T>
        auto read(u32 addr) -> T {
            addr &= sizeof(memory) - 1;
            return *(T*)&memory[addr];
        }

        template<typename T>
        auto write(u32 addr, T value) -> void {
            addr &= sizeof(memory) - 1;
            *(T*)&memory[addr] = value;
        }
    };

    struct DummyPS1 {
        DummyBus bus;
        DummyScheduler scheduler;
        DummyInterrupt interrupt;
    };

    extern DummyPS1 ps1;

}