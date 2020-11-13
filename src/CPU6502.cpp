/*
 * This .cpp implements the CPU6502 emulating class.
 */

#include "CPU6502.h"
#include <iostream>

/**
 * Constructor for the CPU6502 emulating class.
 * @param bus
 */
CPU6502::CPU6502(Bus *bus) {
    /* Bus */
    this->bus = bus;

    /* Set Registers to 0 */
    this->X = 0x0;
    this->Y = 0x0;
    this->ACC = 0x0;
    this->PC = 0x0;
    this->C = 0x0;
    this->D = 0x0;

    op_code = 0x0;
    addressparam = 0;

    //Initializing OP_TABLE
    //KIL if op_code not set

    OP_TABLE[0x00] = {&CPU6502::BRK, &CPU6502::imp, 7};
    OP_TABLE[0x01] = {&CPU6502::ORA, &CPU6502::izx, 6};
    OP_TABLE[0x02] = {&CPU6502::KIL, &CPU6502::imp, 0};
    OP_TABLE[0x03] = {&CPU6502::SLO, &CPU6502::izx, 8};
    OP_TABLE[0x04] = {&CPU6502::NOP, &CPU6502::zp, 3};
    OP_TABLE[0x05] = {&CPU6502::ORA, &CPU6502::zp, 3};
    OP_TABLE[0x06] = {&CPU6502::ASL, &CPU6502::zp, 5};
    OP_TABLE[0x07] = {&CPU6502::SLO, &CPU6502::zp, 5};
    OP_TABLE[0x08] = {&CPU6502::PHP, &CPU6502::imp, 3};
    OP_TABLE[0x09] = {&CPU6502::ORA, &CPU6502::imm, 2};
    OP_TABLE[0x0A] = {&CPU6502::ASL, &CPU6502::imp, 2};
    OP_TABLE[0x0B] = {&CPU6502::ANC, &CPU6502::imm, 2};
    OP_TABLE[0x0C] = {&CPU6502::NOP, &CPU6502::abs, 4};
    OP_TABLE[0x0D] = {&CPU6502::ORA, &CPU6502::abs, 4};
    OP_TABLE[0x0E] = {&CPU6502::ASL, &CPU6502::abs, 6};
    OP_TABLE[0x0F] = {&CPU6502::SLO, &CPU6502::abs, 6};
    OP_TABLE[0x10] = {&CPU6502::BPL, &CPU6502::rel, 2};//
    OP_TABLE[0x11] = {&CPU6502::ORA, &CPU6502::izy, 5};//
    OP_TABLE[0x12] = {&CPU6502::KIL, &CPU6502::imp, 0};
    OP_TABLE[0x13] = {&CPU6502::SLO, &CPU6502::izy, 8};
    OP_TABLE[0x14] = {&CPU6502::NOP, &CPU6502::zpx, 4};
    OP_TABLE[0x15] = {&CPU6502::ORA, &CPU6502::zpx, 4};
    OP_TABLE[0x16] = {&CPU6502::ASL, &CPU6502::zpx, 6};
    OP_TABLE[0x17] = {&CPU6502::SLO, &CPU6502::zpx, 6};
    OP_TABLE[0x18] = {&CPU6502::CLC, &CPU6502::imp, 2};
    OP_TABLE[0x19] = {&CPU6502::ORA, &CPU6502::aby, 4};//
    OP_TABLE[0x1A] = {&CPU6502::NOP, &CPU6502::imp, 2};
    OP_TABLE[0x1B] = {&CPU6502::SLO, &CPU6502::aby, 7};
    OP_TABLE[0x1C] = {&CPU6502::NOP, &CPU6502::abx, 4};//
    OP_TABLE[0x1D] = {&CPU6502::ORA, &CPU6502::abx, 4};//
    OP_TABLE[0x1E] = {&CPU6502::ASL, &CPU6502::abx, 7};
    OP_TABLE[0x1F] = {&CPU6502::SLO, &CPU6502::abx, 7};
    OP_TABLE[0x20] = {&CPU6502::JSR, &CPU6502::abs, 6};
    OP_TABLE[0x21] = {&CPU6502::AND, &CPU6502::izx,6};
    OP_TABLE[0x22] = {&CPU6502::KIL, &CPU6502::imp, 0};
    OP_TABLE[0x23] = {&CPU6502::RLA, &CPU6502::izx, 8};
    OP_TABLE[0x24] = {&CPU6502::BIT, &CPU6502::zp, 3};
    OP_TABLE[0x25] = {&CPU6502::AND, &CPU6502::zp, 3};
    OP_TABLE[0x26] = {&CPU6502::ROL, &CPU6502::zp, 5};
    OP_TABLE[0x27] = {&CPU6502::RLA, &CPU6502::zp, 5};
    OP_TABLE[0x28] = {&CPU6502::PLP, &CPU6502::imp, 4};
    OP_TABLE[0x29] = {&CPU6502::AND, &CPU6502::imm, 2};
    OP_TABLE[0x2A] = {&CPU6502::ROL, &CPU6502::imp, 2};
    OP_TABLE[0x2B] = {&CPU6502::ANC, &CPU6502::imm, 2};
    OP_TABLE[0x2C] = {&CPU6502::BIT, &CPU6502::abs, 4};
    OP_TABLE[0x2D] = {&CPU6502::AND, &CPU6502::abs, 4};
    OP_TABLE[0x2E] = {&CPU6502::ROL, &CPU6502::abs, 6};
    OP_TABLE[0x2F] = {&CPU6502::RLA, &CPU6502::abs, 6};
    OP_TABLE[0x30] = {&CPU6502::BMI, &CPU6502::rel, 2};//
    OP_TABLE[0x31] = {&CPU6502::AND, &CPU6502::izy, 5};//
    OP_TABLE[0x32] = {&CPU6502::KIL, &CPU6502::imp, 0};
    OP_TABLE[0x33] = {&CPU6502::RLA, &CPU6502::izy, 8};
    OP_TABLE[0x34] = {&CPU6502::NOP, &CPU6502::zpx, 4};
    OP_TABLE[0x35] = {&CPU6502::AND, &CPU6502::zpx, 4};
    OP_TABLE[0x36] = {&CPU6502::ROL, &CPU6502::zpx, 6};
    OP_TABLE[0x37] = {&CPU6502::RLA, &CPU6502::zpx, 6};
    OP_TABLE[0x38] = {&CPU6502::SEC, &CPU6502::imp, 2};
    OP_TABLE[0x39] = {&CPU6502::AND, &CPU6502::aby, 4};//
    OP_TABLE[0x3A] = {&CPU6502::NOP, &CPU6502::imp, 2};
    OP_TABLE[0x3B] = {&CPU6502::RLA, &CPU6502::aby, 7};
    OP_TABLE[0x3C] = {&CPU6502::NOP, &CPU6502::abx, 4};//
    OP_TABLE[0x3D] = {&CPU6502::AND, &CPU6502::abx, 4};
    OP_TABLE[0x3E] = {&CPU6502::ROL, &CPU6502::abx, 7};
    OP_TABLE[0x3F] = {&CPU6502::RLA, &CPU6502::abx, 7};
    OP_TABLE[0x40] = {&CPU6502::RTI, &CPU6502::imp, 6};
    OP_TABLE[0x41] = {&CPU6502::EOR, &CPU6502::izx, 6};
    OP_TABLE[0x42] = {&CPU6502::KIL, &CPU6502::imp, 0};
    OP_TABLE[0x43] = {&CPU6502::SRE, &CPU6502::izx, 8};
    OP_TABLE[0x44] = {&CPU6502::NOP, &CPU6502::zp, 3};
    OP_TABLE[0x45] = {&CPU6502::EOR, &CPU6502::zp, 3};
    OP_TABLE[0x46] = {&CPU6502::LSR, &CPU6502::zp, 5};
    OP_TABLE[0x47] = {&CPU6502::SRE, &CPU6502::zp, 5};
    OP_TABLE[0x48] = {&CPU6502::PHA, &CPU6502::imp, 3};
    OP_TABLE[0x49] = {&CPU6502::EOR, &CPU6502::imm, 2};
    OP_TABLE[0x4A] = {&CPU6502::LSR, &CPU6502::imp, 2};
    OP_TABLE[0x4B] = {&CPU6502::ALR, &CPU6502::imm, 2};
    OP_TABLE[0x4C] = {&CPU6502::JMP, &CPU6502::abs, 3};
    OP_TABLE[0x4D] = {&CPU6502::EOR, &CPU6502::abs, 4};
    OP_TABLE[0x4E] = {&CPU6502::LSR, &CPU6502::abs, 6};
    OP_TABLE[0x4F] = {&CPU6502::SRE, &CPU6502::abs, 6};
    OP_TABLE[0x50] = {&CPU6502::BVC, &CPU6502::rel, 2};//
    OP_TABLE[0x51] = {&CPU6502::EOR, &CPU6502::izy, 5};//
    OP_TABLE[0x52] = {&CPU6502::KIL, &CPU6502::imp, 0};
    OP_TABLE[0x53] = {&CPU6502::SRE, &CPU6502::izy, 8};
    OP_TABLE[0x54] = {&CPU6502::NOP, &CPU6502::zpx, 4};
    OP_TABLE[0x55] = {&CPU6502::EOR, &CPU6502::zpx, 4};
    OP_TABLE[0x56] = {&CPU6502::LSR, &CPU6502::zpx, 6};
    OP_TABLE[0x57] = {&CPU6502::SRE, &CPU6502::zpx, 6};
    OP_TABLE[0x58] = {&CPU6502::CLI, &CPU6502::imp, 2};
    OP_TABLE[0x59] = {&CPU6502::EOR, &CPU6502::aby, 4};//
    OP_TABLE[0x5A] = {&CPU6502::NOP, &CPU6502::imp, 2};
    OP_TABLE[0x5B] = {&CPU6502::SRE, &CPU6502::aby, 7};
    OP_TABLE[0x5C] = {&CPU6502::NOP, &CPU6502::abx, 4};//
    OP_TABLE[0x5D] = {&CPU6502::EOR, &CPU6502::abx, 4};//
    OP_TABLE[0x5E] = {&CPU6502::LSR, &CPU6502::abx, 7};
    OP_TABLE[0x5F] = {&CPU6502::SRE, &CPU6502::abx, 7};
    OP_TABLE[0x60] = {&CPU6502::RTS, &CPU6502::imp, 6};
    OP_TABLE[0x61] = {&CPU6502::ADC, &CPU6502::izx, 6};
    OP_TABLE[0x62] = {&CPU6502::KIL, &CPU6502::imp, 0};
    OP_TABLE[0x63] = {&CPU6502::RRA, &CPU6502::izx, 8};
    OP_TABLE[0x64] = {&CPU6502::NOP, &CPU6502::zp, 3};
    OP_TABLE[0x65] = {&CPU6502::ADC, &CPU6502::zp, 3};
    OP_TABLE[0x66] = {&CPU6502::ROR, &CPU6502::zp, 5};
    OP_TABLE[0x67] = {&CPU6502::RRA, &CPU6502::zp, 5};
    OP_TABLE[0x68] = {&CPU6502::PLA, &CPU6502::imp, 4};
    OP_TABLE[0x69] = {&CPU6502::ADC, &CPU6502::imm, 2};
    OP_TABLE[0x6A] = {&CPU6502::ROR, &CPU6502::imp, 2};
    OP_TABLE[0x6B] = {&CPU6502::ARR, &CPU6502::imm, 2};
    OP_TABLE[0x6C] = {&CPU6502::JMP, &CPU6502::ind, 5};
    OP_TABLE[0x6D] = {&CPU6502::ADC, &CPU6502::abs, 4};
    OP_TABLE[0x6E] = {&CPU6502::ROR, &CPU6502::abs, 6};
    OP_TABLE[0x6F] = {&CPU6502::RRA, &CPU6502::abs, 6};
    OP_TABLE[0x70] = {&CPU6502::BVS, &CPU6502::rel, 2};//
    OP_TABLE[0x71] = {&CPU6502::ADC, &CPU6502::izy, 5};//
    OP_TABLE[0x72] = {&CPU6502::KIL, &CPU6502::imp, 0};
    OP_TABLE[0x73] = {&CPU6502::RRA, &CPU6502::izy, 8};
    OP_TABLE[0x74] = {&CPU6502::NOP, &CPU6502::zpx, 4};
    OP_TABLE[0x75] = {&CPU6502::ADC, &CPU6502::zpx, 4};
    OP_TABLE[0x76] = {&CPU6502::ROR, &CPU6502::zpx, 6};
    OP_TABLE[0x77] = {&CPU6502::RRA, &CPU6502::zpx, 6};
    OP_TABLE[0x78] = {&CPU6502::SEI, &CPU6502::imp, 2};
    OP_TABLE[0x79] = {&CPU6502::ADC, &CPU6502::aby, 4};//
    OP_TABLE[0x7A] = {&CPU6502::NOP, &CPU6502::imp, 2};
    OP_TABLE[0x7B] = {&CPU6502::RRA, &CPU6502::aby, 7};
    OP_TABLE[0x7C] = {&CPU6502::NOP, &CPU6502::abx, 4};//
    OP_TABLE[0x7D] = {&CPU6502::ADC, &CPU6502::abx, 4};//
    OP_TABLE[0x7E] = {&CPU6502::ROR, &CPU6502::abx, 7};
    OP_TABLE[0x7F] = {&CPU6502::RRA, &CPU6502::abx, 7};
    OP_TABLE[0x80] = {&CPU6502::NOP, &CPU6502::imm, 2};
    OP_TABLE[0x81] = {&CPU6502::STA, &CPU6502::izx, 6};
    OP_TABLE[0x82] = {&CPU6502::KIL, &CPU6502::imp, 2};
    OP_TABLE[0x83] = {&CPU6502::SAX, &CPU6502::izx, 6};
    OP_TABLE[0x84] = {&CPU6502::STY, &CPU6502::zp, 3};
    OP_TABLE[0x85] = {&CPU6502::STA, &CPU6502::zp, 3};
    OP_TABLE[0x86] = {&CPU6502::STX, &CPU6502::zp, 3};
    OP_TABLE[0x87] = {&CPU6502::SAX, &CPU6502::zp, 3};
    OP_TABLE[0x88] = {&CPU6502::DEY, &CPU6502::imp, 2};
    OP_TABLE[0x89] = {&CPU6502::NOP, &CPU6502::imm, 2};
    OP_TABLE[0x8A] = {&CPU6502::TXA, &CPU6502::imp, 2};
    OP_TABLE[0x8B] = {&CPU6502::XAA, &CPU6502::imm, 2};
    OP_TABLE[0x8C] = {&CPU6502::STY, &CPU6502::abs, 4};
    OP_TABLE[0x8D] = {&CPU6502::STA, &CPU6502::abs, 4};
    OP_TABLE[0x8E] = {&CPU6502::STX, &CPU6502::abs, 4};
    OP_TABLE[0x8F] = {&CPU6502::SAX, &CPU6502::abs, 4};
    OP_TABLE[0x90] = {&CPU6502::BCC, &CPU6502::rel, 2};//
    OP_TABLE[0x91] = {&CPU6502::STA, &CPU6502::izy, 6};
    OP_TABLE[0x92] = {&CPU6502::KIL, &CPU6502::imp, 2};
    OP_TABLE[0x93] = {&CPU6502::AHX, &CPU6502::izy, 6};
    OP_TABLE[0x94] = {&CPU6502::STY, &CPU6502::zpx, 4};
    OP_TABLE[0x95] = {&CPU6502::STA, &CPU6502::zpx, 4};
    OP_TABLE[0x96] = {&CPU6502::STX, &CPU6502::zpy, 4};
    OP_TABLE[0x97] = {&CPU6502::SAX, &CPU6502::zpy, 4};
    OP_TABLE[0x98] = {&CPU6502::TYA, &CPU6502::imp, 2};
    OP_TABLE[0x99] = {&CPU6502::STA, &CPU6502::aby, 5};
    OP_TABLE[0x9A] = {&CPU6502::TXS, &CPU6502::imp, 2};
    OP_TABLE[0x9B] = {&CPU6502::TAS, &CPU6502::aby, 5};
    OP_TABLE[0x9C] = {&CPU6502::SHY, &CPU6502::abx, 5};
    OP_TABLE[0x9D] = {&CPU6502::STA, &CPU6502::abx, 5};
    OP_TABLE[0x9E] = {&CPU6502::SHX, &CPU6502::aby, 5};
    OP_TABLE[0x9F] = {&CPU6502::AHX, &CPU6502::aby, 5};
    OP_TABLE[0xA0] = {&CPU6502::LDY, &CPU6502::imm, 2};
    OP_TABLE[0xA1] = {&CPU6502::LDA, &CPU6502::izx, 6};
    OP_TABLE[0xA2] = {&CPU6502::LDX, &CPU6502::imm, 2};
    OP_TABLE[0xA3] = {&CPU6502::LAX, &CPU6502::izx, 6};
    OP_TABLE[0xA4] = {&CPU6502::LDY, &CPU6502::zp, 3};
    OP_TABLE[0xA5] = {&CPU6502::LDA, &CPU6502::zp, 3};
    OP_TABLE[0xA6] = {&CPU6502::LDX, &CPU6502::zp, 3};
    OP_TABLE[0xA7] = {&CPU6502::LAX, &CPU6502::zp, 3};
    OP_TABLE[0xA8] = {&CPU6502::TAY, &CPU6502::imp, 2};
    OP_TABLE[0xA9] = {&CPU6502::LDA, &CPU6502::imm, 2};
    OP_TABLE[0xAA] = {&CPU6502::TAX, &CPU6502::imp, 2};
    OP_TABLE[0xAB] = {&CPU6502::LAX, &CPU6502::imm, 2};
    OP_TABLE[0xAC] = {&CPU6502::LDY, &CPU6502::abs, 4};
    OP_TABLE[0xAD] = {&CPU6502::LDA, &CPU6502::abs, 4};
    OP_TABLE[0xAE] = {&CPU6502::LDX, &CPU6502::abs, 4};
    OP_TABLE[0xAF] = {&CPU6502::LAX, &CPU6502::abs, 4};
    OP_TABLE[0xB0] = {&CPU6502::BCS, &CPU6502::rel, 2};//
    OP_TABLE[0xB1] = {&CPU6502::LDA, &CPU6502::izy, 5};//
    OP_TABLE[0xB2] = {&CPU6502::KIL, &CPU6502::imp, 0};
    OP_TABLE[0xB3] = {&CPU6502::LAX, &CPU6502::izy, 5};//
    OP_TABLE[0xB4] = {&CPU6502::LDY, &CPU6502::zpx, 4};
    OP_TABLE[0xB5] = {&CPU6502::LDA, &CPU6502::zpx, 4};
    OP_TABLE[0xB6] = {&CPU6502::LDX, &CPU6502::zpy, 4};
    OP_TABLE[0xB7] = {&CPU6502::LAX, &CPU6502::zpy, 4};
    OP_TABLE[0xB8] = {&CPU6502::CLV, &CPU6502::imp, 2};
    OP_TABLE[0xB9] = {&CPU6502::LDA, &CPU6502::aby, 4};//
    OP_TABLE[0xBA] = {&CPU6502::TSX, &CPU6502::imp, 2};
    OP_TABLE[0xBB] = {&CPU6502::LAS, &CPU6502::aby, 4};//
    OP_TABLE[0xBC] = {&CPU6502::LDY, &CPU6502::abx, 4};//
    OP_TABLE[0xBD] = {&CPU6502::LDA, &CPU6502::abx, 4};//
    OP_TABLE[0xBE] = {&CPU6502::LDX, &CPU6502::aby, 4};//
    OP_TABLE[0xBF] = {&CPU6502::LAX, &CPU6502::aby, 4};//
    OP_TABLE[0xC0] = {&CPU6502::CPY, &CPU6502::imm, 2};
    OP_TABLE[0xC1] = {&CPU6502::CMP, &CPU6502::izx, 6};
    OP_TABLE[0xC2] = {&CPU6502::NOP, &CPU6502::imm, 2};
    OP_TABLE[0xC3] = {&CPU6502::DCP, &CPU6502::izx, 8};
    OP_TABLE[0xC4] = {&CPU6502::CPY, &CPU6502::zp, 3};
    OP_TABLE[0xC5] = {&CPU6502::CMP, &CPU6502::zp, 3};
    OP_TABLE[0xC6] = {&CPU6502::DEC, &CPU6502::zp, 5};
    OP_TABLE[0xC7] = {&CPU6502::DCP, &CPU6502::zp, 5};
    OP_TABLE[0xC8] = {&CPU6502::INY, &CPU6502::imp, 2};
    OP_TABLE[0xC9] = {&CPU6502::CMP, &CPU6502::imm, 2};
    OP_TABLE[0xCA] = {&CPU6502::DEX, &CPU6502::imp, 2};
    OP_TABLE[0xCB] = {&CPU6502::AXS, &CPU6502::imm, 2};
    OP_TABLE[0xCC] = {&CPU6502::CPY, &CPU6502::abs, 4};
    OP_TABLE[0xCD] = {&CPU6502::CMP, &CPU6502::abs, 4};
    OP_TABLE[0xCE] = {&CPU6502::DEC, &CPU6502::abs, 6};
    OP_TABLE[0xCF] = {&CPU6502::DCP, &CPU6502::abs, 6};
    OP_TABLE[0xD0] = {&CPU6502::BNE, &CPU6502::rel, 2};//
    OP_TABLE[0xD1] = {&CPU6502::CMP, &CPU6502::izy, 5};//
    OP_TABLE[0xD2] = {&CPU6502::KIL, &CPU6502::imp, 0};
    OP_TABLE[0xD3] = {&CPU6502::DCP, &CPU6502::izy, 8};
    OP_TABLE[0xD4] = {&CPU6502::NOP, &CPU6502::zpx, 4};
    OP_TABLE[0xD5] = {&CPU6502::CMP, &CPU6502::zpx, 4};
    OP_TABLE[0xD6] = {&CPU6502::DEC, &CPU6502::zpx,6};
    OP_TABLE[0xD7] = {&CPU6502::DCP, &CPU6502::zpx, 6};
    OP_TABLE[0xD8] = {&CPU6502::CLD, &CPU6502::imp, 2};
    OP_TABLE[0xD9] = {&CPU6502::CMP, &CPU6502::aby, 4};//
    OP_TABLE[0xDA] = {&CPU6502::NOP, &CPU6502::abx, 2};
    OP_TABLE[0xDB] = {&CPU6502::DCP, &CPU6502::aby, 7};
    OP_TABLE[0xDC] = {&CPU6502::NOP, &CPU6502::abx, 4};//
    OP_TABLE[0xDD] = {&CPU6502::CMP, &CPU6502::abx, 4};//
    OP_TABLE[0xDE] = {&CPU6502::DEC, &CPU6502::abx, 7};
    OP_TABLE[0xDF] = {&CPU6502::DCP, &CPU6502::abx, 7};
    OP_TABLE[0xE0] = {&CPU6502::CPX, &CPU6502::imm, 2};
    OP_TABLE[0xE1] = {&CPU6502::SBC, &CPU6502::izx, 6};
    OP_TABLE[0xE2] = {&CPU6502::NOP, &CPU6502::imm, 2};
    OP_TABLE[0xE3] = {&CPU6502::ISC, &CPU6502::izx, 8};
    OP_TABLE[0xE4] = {&CPU6502::CPX, &CPU6502::zp, 3};
    OP_TABLE[0xE5] = {&CPU6502::SBC, &CPU6502::zp, 3};
    OP_TABLE[0xE6] = {&CPU6502::INC, &CPU6502::zp, 5};
    OP_TABLE[0xE7] = {&CPU6502::ISC, &CPU6502::zp, 5};
    OP_TABLE[0xE8] = {&CPU6502::INX, &CPU6502::imp, 2};
    OP_TABLE[0xE9] = {&CPU6502::SBC, &CPU6502::imm, 2};
    OP_TABLE[0xEA] = {&CPU6502::NOP, &CPU6502::imp, 2};
    OP_TABLE[0xEB] = {&CPU6502::SBC, &CPU6502::imm, 2};
    OP_TABLE[0xEC] = {&CPU6502::CPX, &CPU6502::abs, 4};
    OP_TABLE[0xED] = {&CPU6502::SBC, &CPU6502::abs, 4};
    OP_TABLE[0xEE] = {&CPU6502::INC, &CPU6502::abs, 6};
    OP_TABLE[0xEF] = {&CPU6502::ISC, &CPU6502::abs, 6};
    OP_TABLE[0xF0] = {&CPU6502::BEQ, &CPU6502::rel, 2};//
    OP_TABLE[0xF1] = {&CPU6502::SBC, &CPU6502::izy, 5};//
    OP_TABLE[0xF2] = {&CPU6502::KIL, &CPU6502::imp, 0};
    OP_TABLE[0xF3] = {&CPU6502::ISC, &CPU6502::izy, 8};
    OP_TABLE[0xF4] = {&CPU6502::NOP, &CPU6502::zpx, 4};
    OP_TABLE[0xF5] = {&CPU6502::SBC, &CPU6502::zpx, 4};
    OP_TABLE[0xF6] = {&CPU6502::INC, &CPU6502::zpx, 6};
    OP_TABLE[0xF7] = {&CPU6502::ISC, &CPU6502::zpx, 6};
    OP_TABLE[0xF8] = {&CPU6502::SED, &CPU6502::imp, 2};
    OP_TABLE[0xF9] = {&CPU6502::SBC, &CPU6502::aby, 4};//
    OP_TABLE[0xFA] = {&CPU6502::NOP, &CPU6502::imp, 2};
    OP_TABLE[0xFB] = {&CPU6502::ISC, &CPU6502::aby, 7};
    OP_TABLE[0xFC] = {&CPU6502::NOP, &CPU6502::abx, 4};//
    OP_TABLE[0xFD] = {&CPU6502::SBC, &CPU6502::abx, 4};//
    OP_TABLE[0xFE] = {&CPU6502::INC, &CPU6502::abx, 7};
    OP_TABLE[0xFF] = {&CPU6502::ISC, &CPU6502::abx, 7};


}

/**
 *  Runs the CPU6052.
 */
void CPU6502::run() {
    while (1) {
        op_code = read(CPU6502::PC);
        CPU6502::EXC_OP();


        break;
    }
}

/**
 * Executes a single operation code.
 */
void CPU6502::EXC_OP() {
    (this->*OP_TABLE[op_code].x)();
    (this->*OP_TABLE[op_code].funcP)();
    PC++;
}

/*Addressing modes*/

//works

//works
void CPU6502::imp() {

}
void CPU6502::imm() {
    PC++;
    addressparam = PC;
}
void CPU6502::acc() {
}

void CPU6502::zp() {
    PC++;
    addressparam = read(PC);
}

void CPU6502::zpx() {
    PC++;
    addressparam = read(PC);
    addressparam = (addressparam + this->X)%256;
}

void CPU6502::zpy() {
    PC++;
    addressparam = read(PC);
    addressparam = (addressparam + this->Y)%256;
}

void CPU6502::abs() {
    PC++;
    int8_t lo = read(PC);
    PC++;
    int8_t hi = read(PC);
    addressparam = (hi << 8) | lo;
}
//works
void CPU6502::abx() {
    PC++;
    int8_t lo = read(PC);
    PC++;
    int8_t hi = read(PC);
    addressparam = (hi << 8) | lo;
    addressparam += this->X;
}
//works
void CPU6502::aby() {
    PC++;
    int8_t lo = read(PC);
    PC++;
    int8_t hi = read(PC);
    addressparam = (hi << 8) | lo;
    addressparam += this->Y;
}

void CPU6502::rel() {
    PC++;
    addressparam = PC + read(PC);
}

void CPU6502::ind() {
    PC++;
    char lo =read(PC);
    PC++;
    char hi = read(PC);
    addressparam = read((hi << 8) | lo);
}
//Doesnt work yet
void CPU6502::izx() {
    PC++;
    char lo = read((read(PC)+this->X)%256);
    char hi = read((read(PC)+this->X + 1)%256);

    addressparam = (hi << 8) | lo;
}
//Doesnt work yet
void CPU6502::izy() {
    PC++;
    char lo = read((read(PC))%256);
    char hi = read((read(PC)+ 1)%256);


    addressparam = ((hi << 8) | lo)+ this->Y;
}



/* Bus Handling */

void CPU6502::write(int16_t address, int8_t data) {
    bus->busWrite(address,data);
}

int8_t CPU6502::read(int16_t address) {
    return bus->busRead(address);
}


/* OP functions */

void CPU6502::LDA(){}

void CPU6502::LDX(){}

void CPU6502::LDY(){}

void CPU6502::STA(){}

void CPU6502::STX(){}

void CPU6502::STY(){}


//Register Transfers
void CPU6502::TAX(){}

void CPU6502::TAY(){}

void CPU6502::TXA(){}

void CPU6502::TYA(){}


//Stack Operations
void CPU6502::TSX(){}

void CPU6502::TXS(){}

void CPU6502::PHA(){}

void CPU6502::PHP(){}

void CPU6502::PLA(){}

void CPU6502::PLP(){}

//Logical

void CPU6502::AND(){}

void CPU6502::EOR(){}

void CPU6502::ORA(){}

void CPU6502::BIT(){}

//Arithmetic

void CPU6502::ADC(){}

void CPU6502::SBC(){}

void CPU6502::CMP(){}

void CPU6502::CPX(){}

void CPU6502::CPY(){}


//Increments/Decrements

void CPU6502::INC(){}

void CPU6502::INX(){}

void CPU6502::INY(){}

void CPU6502::DEC(){
    printf("%d\n", read(addressparam));
    write(addressparam, read(addressparam)-1);
    printf("%d\n", read(addressparam));
}

void CPU6502::DEX(){}

void CPU6502::DEY(){}


//Shifts

void CPU6502::ASL(){}

void CPU6502::LSR(){}

void CPU6502::ROL(){}

void CPU6502::ROR(){}


//Jumps/Calls

void CPU6502::JMP(){}

void CPU6502::JSR(){}

void CPU6502::RTS(){}


//Branches

void CPU6502::BCC(){}

void CPU6502::BCS(){}

void CPU6502::BEQ(){}

void CPU6502::BMI(){}

void CPU6502::BNE(){}

void CPU6502::BLP(){}

void CPU6502::BVC(){}

void CPU6502::BVS(){}


//Status Flag Changes

void CPU6502::SEC(){
    this->C = true;
}


void CPU6502::SED(){
    this->D = true;
}

void CPU6502::SEI(){
    this->I = true;
}


void CPU6502::CLC(){
    this->C = false;
}

void CPU6502::CLD(){}

void CPU6502::CLI(){}

void CPU6502::CLV(){}

//System Functions
void CPU6502::BRK(){}

void CPU6502::NOP(){}

void CPU6502::RTI(){}



//other
void CPU6502::ISC(){
}

void CPU6502::KIL(){}

void CPU6502::DCP(){}


void CPU6502::AXS(){}
void CPU6502::LAX(){}
void CPU6502::SHX(){}
void CPU6502::SHY(){}
void CPU6502::TAS(){}
void CPU6502::SAX(){}
void CPU6502::AHX(){}
void CPU6502::XAA(){}
void CPU6502::ARR(){}
void CPU6502::SRE(){}
void CPU6502::ANC(){}
void CPU6502::SLO(){}
void CPU6502::ALC(){}
void CPU6502::BPL(){}
void CPU6502::LAS(){}
void CPU6502::ALR(){}
void CPU6502::RRA(){}
void CPU6502::RLA(){}