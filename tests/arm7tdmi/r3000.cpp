#include <nall/nall.hpp>
#include <cstdlib>
#include <nall/string/markup/json.hpp>
#include "myrandom.h"
using namespace nall;


#include <nall/main.hpp>

#include "ps1_stub.h"
#define NUMTESTS 1000

#include <ares/ares.hpp>

namespace ares::PlayStation {
#include <ps1/cpu/cpu.hpp>
}



ares::PlayStation::CPU cpu{};
sfc32_state rstate{};

struct test_state {
    struct {
        u32 R[32];
        u32 hi{}, lo{};
        u32 EPC{}, TAR{}, CAUSE{};
        u32 PC{};

        struct {
            struct {
                i32 target;
                u32 val;
            } load;

            struct {
                u32 slot{}; // Is a delay slot?
                u32 take{}; // Are we taking it?
                u32 target{}; // What is the target?
            } branch;
        } delay;
    } cpu;
};

struct testitem {
    char name[100];
    u32 first_op;
    u32 second_op;
};

testitem testitems[] = {
(testitem) { .name="BCondZ", .first_op=0x01, .second_op=0x00 },
(testitem) { .name="J", .first_op=0x02, .second_op=0x00 },
(testitem) { .name="JAL", .first_op=0x03, .second_op=0x00 },
(testitem) { .name="BEQ", .first_op=0x04, .second_op=0x00 },
(testitem) { .name="BNE", .first_op=0x05, .second_op=0x00 },
(testitem) { .name="BLEZ", .first_op=0x06, .second_op=0x00 },
(testitem) { .name="BGTZ", .first_op=0x07, .second_op=0x00 },
(testitem) { .name="ADDI", .first_op=0x08, .second_op=0x00 },
(testitem) { .name="ADDIU", .first_op=0x09, .second_op=0x00 },
(testitem) { .name="SLTI", .first_op=0x0A, .second_op=0x00 },
(testitem) { .name="SLTIU", .first_op=0x0B, .second_op=0x00 },
(testitem) { .name="ANDI", .first_op=0x0C, .second_op=0x00 },
(testitem) { .name="ORI", .first_op=0x0D, .second_op=0x00 },
(testitem) { .name="XORI", .first_op=0x0E, .second_op=0x00 },
(testitem) { .name="LUI", .first_op=0x0F, .second_op=0x00 },
// COP0-3
(testitem) { .name="LB", .first_op=0x20, .second_op=0x00 },
(testitem) { .name="LH", .first_op=0x21, .second_op=0x00 },
(testitem) { .name="LWL", .first_op=0x22, .second_op=0x00 },
(testitem) { .name="LW", .first_op=0x23, .second_op=0x00 },
(testitem) { .name="LBU", .first_op=0x24, .second_op=0x00 },
(testitem) { .name="LHU", .first_op=0x25, .second_op=0x00 },
(testitem) { .name="LWR", .first_op=0x26, .second_op=0x00 },
(testitem) { .name="SB", .first_op=0x28, .second_op=0x00 },
(testitem) { .name="SHL", .first_op=0x29, .second_op=0x00 },
(testitem) { .name="SWL", .first_op=0x2A, .second_op=0x00 },
(testitem) { .name="SW", .first_op=0x2B, .second_op=0x00 },
(testitem) { .name="SWR", .first_op=0x2E, .second_op=0x00 },
(testitem) { .name="SLL", .first_op=0x00, .second_op=0x00 },
(testitem) { .name="SRL", .first_op=0x00, .second_op=0x02 },
(testitem) { .name="SRA", .first_op=0x00, .second_op=0x03 },
(testitem) { .name="SLLV", .first_op=0x00, .second_op=0x04 },
(testitem) { .name="SRLV", .first_op=0x00, .second_op=0x06 },
(testitem) { .name="SRAV", .first_op=0x00, .second_op=0x07 },
(testitem) { .name="JR", .first_op=0x00, .second_op=0x08 },
(testitem) { .name="JALR", .first_op=0x00, .second_op=0x09 },
(testitem) { .name="SYSCALL", .first_op=0x00, .second_op=0x0C },
(testitem) { .name="BREAK", .first_op=0x00, .second_op=0x0D },
(testitem) { .name="MFHI", .first_op=0x00, .second_op=0x10 },
(testitem) { .name="MTHI", .first_op=0x00, .second_op=0x11 },
(testitem) { .name="MFLO", .first_op=0x00, .second_op=0x12 },
(testitem) { .name="MTLO", .first_op=0x00, .second_op=0x13 },
(testitem) { .name="MULT", .first_op=0x00, .second_op=0x18 },
(testitem) { .name="MULTU", .first_op=0x00, .second_op=0x19 },
(testitem) { .name="DIV", .first_op=0x00, .second_op=0x1A },
(testitem) { .name="DIVU", .first_op=0x00, .second_op=0x1B },
(testitem) { .name="ADD", .first_op=0x00, .second_op=0x20 },
(testitem) { .name="ADDU", .first_op=0x00, .second_op=0x21 },
(testitem) { .name="SUB", .first_op=0x00, .second_op=0x22 },
(testitem) { .name="SUBU", .first_op=0x00, .second_op=0x23 },
(testitem) { .name="AND", .first_op=0x00, .second_op=0x24 },
(testitem) { .name="OR", .first_op=0x00, .second_op=0x25 },
(testitem) { .name="XOR", .first_op=0x00, .second_op=0x26 },
(testitem) { .name="NOR", .first_op=0x00, .second_op=0x27 },
(testitem) { .name="SLT", .first_op=0x00, .second_op=0x2A },
(testitem) { .name="SLTU", .first_op=0x00, .second_op=0x2B },
};

enum : u64 { NOACTION = 0, READ = 1, WRITE = 2, FETCH = 4};

struct test_cycle {
    i64 addr{}; // 3
    u32 actions{}; // 2
    u32 sz{}; // 4
    i64 val{}; // 1
};

struct R3000_test {
    char name[500];
    u32 opcode;
    u32 opcode_addr{};
    test_cycle cycles[50];
    u32 num_cycles{};
    test_state initial{}, final{};
};

R3000_test test;

void random_initial_state(test_state &s) {
    s.cpu.R[0] = 0; // R0=0 in MIPS
    for (u32 i = 1; i < 32; i++) {
        s.cpu.R[i] = sfc32(rstate);
    }
    s.cpu.hi = sfc32(rstate);
    s.cpu.lo = sfc32(rstate);
    s.cpu.EPC = sfc32(rstate);
    s.cpu.TAR = sfc32(rstate);
    /*
    data.bit( 2, 6) = scc.cause.exceptionCode;
    data.bit( 8,15) = scc.cause.interruptPending;
    data.bit(28,29) = scc.cause.coprocessorError;
    data.bit(30)    = scc.cause.branchTaken;
    data.bit(31)    = scc.cause.branchDelay;
    */
    s.cpu.CAUSE = sfc32(rstate) & 0b11110000000000001111111101111100;
    s.cpu.PC = test.opcode_addr;

    // Now look into the pipeline and opcodes!!!
    // 1/4 chance for a register delay, we won't put in a branch-branch-delay on purpose...
    auto &b = s.cpu.delay.branch;
    b.slot = false;
    b.take = false;
    b.target = 0;

    auto &t = s.cpu.delay.load;
    t.target = -1;
    t.val = 0;

    // 1/4 chance for a load delay
    if ((sfc32(rstate) & 3) == 0) {
        auto &rt = s.cpu.delay.load;
        rt.target = 0;
        while (rt.target == 0) {
            rt.target = sfc32(rstate) & 31;
        }
        rt.val = sfc32(rstate);
    }

    // 1/16 chance for a branch delay slot
    if ((sfc32(rstate) & 15) == 0) {
        auto &rb = s.cpu.delay.branch;
        rb.slot = true;
        rb.take = true;
        rb.target = sfc32(rstate) & 0xEFFFFFFC;
    }
}

void copy_state_from_cpu(test_state &s) {
    test.num_cycles = cpu.accruedCycles;
    for (u32 i = 0; i < 32; i++) {
        s.cpu.R[i] = cpu.ipu.r[i];
    }
    s.cpu.R[0] = 0;
    s.cpu.hi = cpu.ipu.hi;
    s.cpu.lo = cpu.ipu.lo;
    s.cpu.EPC = cpu.scc.epc;
    s.cpu.TAR = cpu.scc.targetAddress;
    u32 o = cpu.scc.cause.exceptionCode << 2;
    o |= cpu.scc.cause.interruptPending << 8;
    o |= cpu.scc.cause.coprocessorError << 28;
    o |= cpu.scc.cause.branchTaken << 30;
    o |= cpu.scc.cause.branchDelay << 31;
    s.cpu.CAUSE = o;
    s.cpu.PC = cpu.ipu.pc;
    auto &b = s.cpu.delay.branch;
    b.target = cpu.delay.branch[0].address;
    b.slot = cpu.delay.branch[0].slot;
    b.take = cpu.delay.branch[0].take;

    auto &l = s.cpu.delay.load;
    l.target = -1;
    l.val = 0;
    if (cpu.delay.load[0].target != nullptr) {
        for (u32 i = 0; i < 32; i++) {
            if (cpu.delay.load[0].target == &cpu.ipu.r[i]) {
                l.target = i;
                break;
            }
        }
        l.val = cpu.delay.load[0].source;
    }
}

void copy_state_to_cpu(test_state &s) {
    cpu.accruedCycles = 0;
    for (u32 i = 0; i < 32; i++) {
        cpu.ipu.r[i] = s.cpu.R[i];
    }
    cpu.ipu.hi = s.cpu.hi;
    cpu.ipu.lo = s.cpu.lo;
    cpu.scc.epc = s.cpu.EPC;
    cpu.scc.targetAddress = s.cpu.TAR;
    cpu.scc.cause.exceptionCode = (s.cpu.CAUSE >> 2) & 0x1F;
    cpu.scc.cause.interruptPending = (s.cpu.CAUSE >> 8) & 0xFF;
    cpu.scc.cause.coprocessorError = (s.cpu.CAUSE >> 28) & 3;
    cpu.scc.cause.branchTaken = (s.cpu.CAUSE >> 30) & 1;
    cpu.scc.cause.branchDelay = (s.cpu.CAUSE >> 31) & 1;
    cpu.ipu.pb  = s.cpu.PC - 4; // prev
    cpu.ipu.pc = s.cpu.PC;
    cpu.ipu.pd = s.cpu.PC + 4;
    //printf("\nSET PC TO %08x vs. %08x", cpu.ipu.pc, test.opcode_addr);
    for (auto &b : cpu.delay.branch) {
        b.address = 0;
        b.slot = false;
        b.take = false;
    }
    for (auto &b : cpu.delay.load) {
        b.target = nullptr;
        b.source = 0;
    }

    if (s.cpu.delay.branch.take) {
        cpu.delay.branch[0].address = s.cpu.delay.branch.target;
        cpu.delay.branch[0].slot = s.cpu.delay.branch.slot;
        cpu.delay.branch[0].take = s.cpu.delay.branch.take;
        cpu.ipu.pd = s.cpu.delay.branch.target;
    }
    if (s.cpu.delay.load.target != -1) {
        cpu.delay.load[0].target = &cpu.ipu.r[s.cpu.delay.load.target];
        cpu.delay.load[0].source = s.cpu.delay.load.val;
    }
}


u32 do_fetch(void *p, u32 addr, u8 sz, bool use_cycles) {
    // Fetch opcode
    //if (addr != test.cycles[test.cpu.cycle_number].fetch_addr]): alert issue
    //num = (addr - test.base_addr) / 2;
    //if ((num >= 0) && (num <= 3)): return test.opcodes[num];
    //return test.opcodes[4];
    //if (addr < test.base_addr)
    u32 v;
    //printf("\nFETCH from %08x (TEST:%08x)", addr, test.opcode_addr);
    if (addr == test.opcode_addr) v = test.opcode;
    else {
        printf("\nWARN BAD OPCODE FETCH!?");
        v = 0;
    }
    if (use_cycles) {
        auto &t = test.cycles[cpu.accruedCycles];
        if (t.actions != NOACTION) printf("\nWARN DUPE CYCLE");
        t.actions |= FETCH;
        t.addr = addr;
        t.sz = sz;
        t.val = v;
    }
    return v;
}

u32 do_read(void *p, u32 addr, u8 sz) {
    // Do read
    //if test.read_addr != addr: alert issue
    //test.did_read = true;
    //return test.read_val
    u32 v;
    if (addr == test.opcode_addr) v = test.opcode;
    else v = sfc32(rstate);
    auto &t = test.cycles[cpu.accruedCycles];
    if (t.actions != NOACTION) printf("\nWARN DUPE CYCLE");
    t.actions |= READ;
    t.addr = addr;
    t.sz = sz;
    t.val = v;
    return v;
}

void do_write(void *p, u32 addr, u8 sz, u32 val) {
    // Do write
    //if test.write_addr != addr: alert issue
    //if test.write_val != val: alert issue
    //test.did_write = true
    u32 v;
    auto &t = test.cycles[cpu.accruedCycles];
    if (t.actions != NOACTION) printf("\nWARN DUPE CYCLE");
    t.actions |= WRITE;
    t.addr = addr;
    t.sz = sz;
    t.val = val;
}


u32 gen_opcode(testitem &t) {
    u32 bits;
    if (t.first_op == 1) { // BLTZ, BGEZ, BLTZAL, BGEZAL aka BCondZ
        bits = sfc32(rstate) & 0b00000011111000001111111111111111;
        bits |= (1 << 26);
        u32 bits5 = (sfc32(rstate) & 31);
        bits |= (bits5 << 16);
    }
    else {
        u32 b05 = (t.first_op == 0) ? t.second_op : (sfc32(rstate) & 63);
        bits = (sfc32(rstate) & 0b00000011111111111111111111000000) | b05 | (t.first_op << 26);
    }
    //printf("\n%s %08x", t.name, bits);
    return bits;
}

#define cW32(x) fwrite(&x, sizeof(x), 1, fp)

void write_state(test_state &s, FILE *fp) {
    for (u32 i = 0; i < 32; i++) {
        cW32(s.cpu.R[i]);
    }
    cW32(s.cpu.hi);
    cW32(s.cpu.lo);
    cW32(s.cpu.EPC);
    cW32(s.cpu.TAR);
    cW32(s.cpu.CAUSE);
    cW32(s.cpu.PC);
    cW32(s.cpu.delay.branch.target);
    cW32(s.cpu.delay.branch.slot);
    cW32(s.cpu.delay.branch.take);
    cW32(s.cpu.delay.load.target);
    cW32(s.cpu.delay.load.val);
}

void write_test(FILE *fp) {
    cW32(test.opcode);
    cW32(test.opcode_addr);
    write_state(test.initial, fp);
    write_state(test.final, fp);
    cW32(test.num_cycles);
    for (u32 i = 0; i < test.num_cycles; i++) {
        auto &c = test.cycles[i];
        cW32(c.val);
        cW32(c.actions);
        cW32(c.addr);
        cW32(c.sz);
    }
}

void make_opcode_test(testitem &t) {
    char PATH[500];
    cpu.power(true);
    printf("\n\nMAKE TEST %s", t.name);
    snprintf(PATH, sizeof(PATH), "/Users/dave/dev/r3000/v1/%s.json.bin", t.name);
    FILE *fp = fopen(PATH, "wb");
    u32 num = NUMTESTS;
    cW32(num);
    for (u32 i = 0; i < NUMTESTS; i++) {
        snprintf(test.name, sizeof(test.name), "%s $%03x", t.name, i);
        u8 ml = strlen(test.name);
        cW32(ml);
        assert(strlen(test.name) < 50);
        fwrite(test.name, 50, 1, fp);
        sfc32_seed(test.name, rstate);
        // Prime the pump!
        for (u32 i = 0; i < 20; i++) {
            sfc32(rstate);
        }
        test.opcode_addr = sfc32(rstate) & 0xEFFFFFFC;
        test.opcode = gen_opcode(t);
        random_initial_state(test.initial);

        test.num_cycles = 0;
        for (auto &c : test.cycles) {
            c.val = -1;
            c.addr = -1;
            c.actions = NOACTION;
        }
        cpu.accruedCycles = 0;
        if (strcmp(t.name, "LW") == 0) {
            int a = 4;
            a++;
        }

        copy_state_to_cpu(test.initial);

        cpu.instruction();

        copy_state_from_cpu(test.final);

        write_test(fp);
    }
    fclose(fp);
}


auto nall::main(Arguments arguments) -> void {
    //sfc32_seed(test.name, ts.rstate);
    cpu.tptr = static_cast<void *>(&test);
    cpu.test_read = &do_read;
    cpu.test_write = &do_write;
    cpu.test_fetch = &do_fetch;
    cpu.power(false);

    for (auto & t : testitems) {
      make_opcode_test(t);
    }
}