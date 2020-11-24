/*
 * This .cpp implements the CPU6502 emulating class.
 */
#include "CompactLogger.h"
#include "CPU6502.h"

// Mostly for dev-op functions and logger.
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

/**
 * Constructor for the CPU6502 emulating class.
 * @param bus
 */
CompactLogger logger = CompactLogger("CPU6502_log.txt");

CPU6502::CPU6502(Bus *bus) {
    /* Bus */
    this->bus = bus;

    /* Set Registers to 0 */
    this->X = 0x0;
    this->Y = 0x0;
    this->ACC = 0x0;
    this->PC = 0x0;
    this->SR = 0x00;

    this->SP = 0xFF;

    op_code = 0x0;
    addressparam = 0;
    address_rel = 0;
    setStatusFlag(B2, true);        //this unused Flag is always true;

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
 * Sets the given Status flag to given state
 */
 void CPU6502::setStatusFlag(Flags flag, bool state) {
     if(state) {
         SR |= flag;        //if bit of flag is not already 1, it will be set to 1
     } else {
         SR &= flag;        //if bit of flag is not already 0, it will be set to 0
     }
 }

 /**
  * Get the state of a given Flag
  */
  uint8_t CPU6502::getStatusFlag(Flags flag) {
      if ((SR&flag)== 0) {
          return 0;
      }
      return 1;
                    //if bit was set it will be greater than 0 otherwise it will be zero
  }


/**
 * Executes a single operation code.
 */
void CPU6502::EXC_OP() {
    (this->*OP_TABLE[op_code].x)();
    std::cout << "| Addressparameter          :"<< addressparam << " |"<< std::endl;
    std::cout << "| Addressparameter contents :"<< intToHexString(read(addressparam))  << " |"<< std::endl;
    (this->*OP_TABLE[op_code].funcP)();
    implied = false;
    std::cout << "| Addressparameter contents :"<< intToHexString(read(addressparam))  << " |"<< std::endl;
}

/*Addressing modes*/

//works

//works
void CPU6502::imp() {
    implied = true;
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
    uint8_t lo = read(PC);
    PC++;
    uint8_t hi = read(PC);
    addressparam = (hi << 8) | lo;
}
//works
void CPU6502::abx() {
    PC++;
    uint8_t lo = read(PC);
    PC++;
    uint8_t hi = read(PC);
    addressparam = (hi << 8) | lo;
    addressparam += this->X;
}
//works
void CPU6502::aby() {
    PC++;
    uint8_t lo = read(PC);
    PC++;
    uint8_t hi = read(PC);
    addressparam = (hi << 8) | lo;
    addressparam += this->Y;
}

//this addressing mode is only used by branching instructions, the relative address can range from -128 to +127 of
// the branch instruction
void CPU6502::rel() {
    PC++;
    address_rel = read(PC);     //address_rel is a signed integer, so it wraps around after 0x7F
}

void CPU6502::ind() {
    PC++;
    uint8_t lo =read(PC);
    PC++;
    uint8_t hi = read(PC);
    addressparam = read((hi << 8) | lo);
}

void CPU6502::izx() {
    PC++;
    uint8_t lo = read((read(PC)+this->X)%256);
    uint8_t hi = read((read(PC)+this->X + 1)%256);

    addressparam = (hi << 8) | lo;
}

void CPU6502::izy() {
    PC++;
    uint8_t lo = read((read(PC))%256);
    uint8_t hi = read((read(PC)+ 1)%256);


    addressparam = ((hi << 8) | lo)+ this->Y;
}



/* Bus Handling */

void CPU6502::write(uint16_t address, uint8_t data) {
    bus->busWrite(address,data);
}

uint8_t CPU6502::read(uint16_t address) {
    return bus->busRead(address);
}


/* OP functions */

void CPU6502::LDA(){
    uint8_t value = this->read(addressparam);
    this->setZero(value);
    this->setNegative(value);
    this->ACC = value;
    logger.debug(__FUNCTION__ ,
                 "loaded accumulator.");
}

void CPU6502::LDX(){
    uint8_t value = this->read(addressparam);
    this->setZero(value);
    this->setNegative(value);
    this->X = value;
    logger.debug(__FUNCTION__ ,
                 "loaded x register.");
}

void CPU6502::LDY(){
    uint8_t value = this->read(addressparam);
    this->setZero(value);
    this->setNegative(value);
    this->Y = value;
    logger.debug(__FUNCTION__ ,
                 "loaded y register.");
}

void CPU6502::STA(){
    this->write(addressparam, this->ACC);
    logger.debug(__FUNCTION__ ,
                 "stored accumulator.");
}

void CPU6502::STX(){
    this->write(addressparam, this->X);
    logger.debug(__FUNCTION__ ,
                 "stored x register.");
}

void CPU6502::STY(){
    this->write(addressparam, this->Y);
    logger.debug(__FUNCTION__ ,
                 "stored y register.");
}


//Register Transfers
void CPU6502::TAX(){
    this->setNegative(this->ACC);
    this->setZero(this->ACC);
    this->X = this->ACC;
    logger.debug(__FUNCTION__ ,
                 "set x register to accumulator.");
}

void CPU6502::TAY(){
    this->setNegative(this->ACC);
    this->setZero(this->ACC);
    this->Y = this->ACC;
    logger.debug(__FUNCTION__ ,
                 "set y register to accumulator.");
}

void CPU6502::TXA(){
    this->setNegative(this->X);
    this->setZero(this->X);
    this->ACC = this->X;
    logger.debug(__FUNCTION__ ,
                 "set accumulator to x register.");
}

void CPU6502::TYA(){
    this->setNegative(this->Y);
    this->setZero(this->Y);
    this->ACC = this->Y;
    logger.debug(__FUNCTION__ ,
                 "set accumulator to y register.");
}


//Stack Operations
void CPU6502::TSX(){
    this->setNegative(this->SP);
    this->setZero(this->SP);
    this->X = this->SP;
    logger.debug(__FUNCTION__ ,
                 "set x register to stack pointer.");
}

void CPU6502::TXS(){
    this->SP = this->X;
    logger.debug(__FUNCTION__ ,
                 "set stack pointer to x register.");
}

void CPU6502::PHA(){
    this->write(0x100 + this->SP, this->ACC);
    this->SP--;
    logger.debug(__FUNCTION__ ,
                 "push accumulator on stack.");
}

void CPU6502::PHP(){
    this->write(0x100 + this->SP, this->SR);
    this->SP--;
    logger.debug(__FUNCTION__ ,
                 "push status register on stack.");
}

void CPU6502::PLA(){
    this->SP++;
    this->ACC = read(0x100 + this->SP);
    this->setNegative(this->ACC);
    this->setZero(this->ACC);
    logger.debug(__FUNCTION__ ,
                 "pull accumulator from stack.");
}

void CPU6502::PLP(){
    this->SP++;
    this->SR = read(0x100 + this->SP);
    logger.debug(__FUNCTION__ ,
                 "pull status register from stack.");
}

//Logical

void CPU6502::AND() {
    ACC = ACC & read(addressparam);
    if (ACC == 0) {
        setStatusFlag(Z, true);
    }
    if (ACC & EIGHTH) {
        setStatusFlag(N, true);
    }

}

void CPU6502::EOR(){
    ACC = ACC ^ read(addressparam);
    if (ACC == 0) {
        setStatusFlag(Z, true);
    }
    if (ACC & EIGHTH) {
        setStatusFlag(N, true);
    }
}

void CPU6502::ORA(){
    ACC = ACC | read(addressparam);
    if (ACC == 0) {
        setStatusFlag(Z, true);
    }
    if (ACC & EIGHTH) {
        setStatusFlag(N, true);
    }
}

void CPU6502::BIT(){
    uint8_t val = read(addressparam);
    if ((ACC & val ) == 0) {
        setStatusFlag(Z, true);
    }
    if (val & SEVENTH) {
        setStatusFlag(V, true);
    }
    if (val & EIGHTH) {
        setStatusFlag(N, true);
    }
}

//Arithmetic

void CPU6502::ADC(){
    uint16_t a = (uint16_t)ACC;
    uint16_t b = (uint16_t) addressparam + getStatusFlag(C);
    uint16_t c = a + b;
    if ((~(a ^ b))&(a ^ c)&0x80) {
        setStatusFlag(V, true);
    } else {
        setStatusFlag(V, false);
    }

    setStatusFlag(Z, c == 0);

    setStatusFlag(C, c > 255);
    setStatusFlag(N, N & EIGHTH);
    ACC = c & 0xFF;
}

void CPU6502::SBC(){
    uint16_t a = (uint16_t)ACC;
    uint16_t b = (uint16_t) addressparam - (1 - getStatusFlag(C));
    uint16_t c = a - b;
    if ((~(a ^ b))&(a ^ c)&0x80) {
        setStatusFlag(V, true);
    }

    setStatusFlag(Z, c == 0);

    setStatusFlag(C, c & 0xFF00);
    setStatusFlag(N, N & EIGHTH);
    ACC = c & 0xFF;


}

void CPU6502::CMP(){
    setStatusFlag(Z ,ACC == read(addressparam));
    setStatusFlag(C, ACC >= read(addressparam));
    setStatusFlag(N, (uint8_t) (ACC - read(addressparam))&EIGHTH);
}

void CPU6502::CPX(){
    setStatusFlag(Z ,X == read(addressparam));
    setStatusFlag(C, X >= read(addressparam));
    setStatusFlag(N, (uint8_t) (X - read(addressparam)) &EIGHTH);
}

void CPU6502::CPY(){
    setStatusFlag(Z ,Y == read(addressparam));
    setStatusFlag(C, Y >= read(addressparam));
    setStatusFlag(N, (uint8_t) (Y - read(addressparam)) &EIGHTH);
}


//Increments/Decrements

void CPU6502::INC(){
    write(addressparam, read(addressparam)+1);
    setStatusFlag(Z, read(addressparam) == 0);
    setStatusFlag(N, read(addressparam) & EIGHTH);
}

void CPU6502::INX(){
    X = X + 1;
    setStatusFlag(Z, X == 0);
    setStatusFlag(N, X & EIGHTH);
}

void CPU6502::INY(){
    Y = Y + 1;
    setStatusFlag(Z, Y == 0);
    setStatusFlag(N, Y & EIGHTH);
}

void CPU6502::DEC(){
    write(addressparam, read(addressparam)-1);
    setStatusFlag(Z, read(addressparam) == 0);
    setStatusFlag(N, read(addressparam) & EIGHTH);
}

void CPU6502::DEX(){
    X = X - 1;
    setStatusFlag(Z, X == 0);
    setStatusFlag(N, X & EIGHTH);
}

void CPU6502::DEY(){
    Y = Y - 1;
    setStatusFlag(Z, Y == 0);
    setStatusFlag(N, Y & EIGHTH);
}

//Shifts
void CPU6502::ASL(){
    if(implied) {
        data = ACC;
    } else {
        data = read(addressparam);
    }
    setStatusFlag(C, data & EIGHTH);
    data = data << 1;

    setStatusFlag(N, data & EIGHTH);
    setStatusFlag(Z, data == 0);
    if(implied) {
        ACC = data;
    } else {
        write(addressparam, data);
    }

}

void CPU6502::LSR(){
    if(implied) {
        data = ACC;
    } else {
        data = read(addressparam);
    }
    setStatusFlag(C, data & FIRST);
    data = data >> 1;
    setStatusFlag(N, data & EIGHTH);
    setStatusFlag(Z, data == 0);

    if(implied) {
        ACC = data & 0x00FF;
    } else {
        write(addressparam, data & 0x00FF);
    }
}

void CPU6502::ROL(){
    if(implied){
        data = ACC << 1 | getStatusFlag(C);
    } else {
        data = (read(addressparam) << 1) | getStatusFlag(C);
    }
    setStatusFlag(C, data&EIGHTH);
    setStatusFlag(Z, (data & 0x00FF) == 0x0000);
    setStatusFlag(N, data & EIGHTH);
    if (implied) {
        ACC = data & 0x00FF;
    } else {
        write(addressparam, data & 0x00FF);
    }

}

void CPU6502::ROR(){
    if (implied) {
        data = ((getStatusFlag(C) << 7))|(ACC >> 1);
        setStatusFlag(C, ACC&FIRST);
    } else {
        data = ((getStatusFlag(C) << 7) | (read(addressparam) >> 1));
        setStatusFlag(C, read(addressparam)&FIRST);
    }
    setStatusFlag(Z, (data & 0x00FF) == 0x0000);
    setStatusFlag(N, data & EIGHTH);
    if (implied) {
        ACC = data & 0x00FF;
    } else {
        write(addressparam, data & 0x00FF);
    }
}

//Jumps/Calls
void CPU6502::JMP(){
    PC = addressparam;
    logger.debug(__FUNCTION__ ,
                 "Jump in PC");
}

void CPU6502::JSR(){
    //(Jump to Subroutine) The JSR instruction pushes the address (minus one) of the return point on to the stack (PC-1)
    // and then sets the program counter to the target memory address.
    bus->busWrite(0x0100 + SP, ((PC-1) >> 8) & 0x00FF);     //push high-byte of PC-1 to Stack
    SP--;                                                               //adjust StackPointer
    bus->busWrite(0x0100 + SP, (PC-1) & 0x00FF);            //push low-byte of PC-1 to Stack
    SP--;                                                               //adjust StackPointer to point to the next free space
    PC = addressparam;                                                  //adjust ProgramCounter
    logger.debug(__FUNCTION__ ,
                 "Jump to Subroutine");
}

void CPU6502::RTS(){
    //(Return from Subroutine) The RTS instruction is used at the end of a subroutine to return to the calling routine.
    //It pulls the program counter (minus one) from the stack.
    SP++;                                                                   //adjust StackPointer to top element
    uint16_t low = (uint16_t)bus->busRead(0x0100 + SP);                //get low-byte from stack
    SP++;                                                                   //adjust StackPointer
    uint16_t high = (uint16_t)(bus->busRead(0x0100 + SP) << 8);        //get high-byte from stack
    PC = high + low + 1;                                                    //set new ProgramCounter
    logger.debug(__FUNCTION__ ,
                 "Return from Subroutine");
}

//Branches
void CPU6502::BCC(){
    PC++;
    if (getStatusFlag(C) == false) {
        //branch is taken, so additional cycle needed
        OPcycles++;
        addressparam = PC + address_rel;
        if ((addressparam & 0xFF00) != (PC & 0xFF00)) {     //check if page was changed, if so OPcycles needs to be
            OPcycles++;                                     //incremented
        }
        PC = addressparam;
    }
    logger.debug(__FUNCTION__ ,
                 "Branch if Carry Flag clear");
}

void CPU6502::BCS(){
    PC++;
    if (getStatusFlag(C) == true) {
        //branch is taken, so additional cycle needed
        OPcycles++;
        addressparam = PC + address_rel;
        if ((addressparam & 0xFF00) != (PC & 0xFF00)) {     //check if page was changed, if so OPcycles needs to be
            OPcycles++;                                     //incremented
        }
        PC = addressparam;
    }
    logger.debug(__FUNCTION__ ,
                 "Branch if Carry Flag set");
}

void CPU6502::BEQ(){
    PC++;
    if (getStatusFlag(Z) == true) {
        //branch is taken, so additional cycle needed
        OPcycles++;
        addressparam = PC + address_rel;
        if ((addressparam & 0xFF00) != (PC & 0xFF00)) {     //check if page was changed, if so OPcycles needs to be
            OPcycles++;                                     //incremented
        }
        PC = addressparam;
    }
    logger.debug(__FUNCTION__ ,
                 "Branch if Zero Flag set (Branch if equal)");
}

void CPU6502::BMI(){
    PC++;
    if (getStatusFlag(N) == true) {
        //branch is taken, so additional cycle needed
        OPcycles++;
        addressparam = PC + address_rel;
        if ((addressparam & 0xFF00) != (PC & 0xFF00)) {     //check if page was changed, if so OPcycles needs to be
            OPcycles++;                                     //incremented
        }
        PC = addressparam;
    }
    logger.debug(__FUNCTION__ ,
                 "Branch if Negative Flag set");
}

void CPU6502::BNE(){
    PC++;
    if (getStatusFlag(Z) == false) {
        //branch is taken, so additional cycle needed
        OPcycles++;
        addressparam = PC + address_rel;
        if ((addressparam & 0xFF00) != (PC & 0xFF00)) {     //check if page was changed, if so OPcycles needs to be
            OPcycles++;                                     //incremented
        }
        PC = addressparam;
    }
    logger.debug(__FUNCTION__ ,
                 "Branch if Zero Flag clear (Branch if not equal)");
}

void CPU6502::BLP(){
    PC++;
    if (getStatusFlag(N) == false) {
        //branch is taken, so additional cycle needed
        OPcycles++;
        addressparam = PC + address_rel;
        if ((addressparam & 0xFF00) != (PC & 0xFF00)) {     //check if page was changed, if so OPcycles needs to be
            OPcycles++;                                     //incremented
        }
        PC = addressparam;
    }
    logger.debug(__FUNCTION__ ,
                 "Branch if Negative Flag clear");
}

void CPU6502::BVC(){
    PC++;
    if (getStatusFlag(V) == false) {
        //branch is taken, so additional cycle needed
        OPcycles++;
        addressparam = PC + address_rel;
        if ((addressparam & 0xFF00) != (PC & 0xFF00)) {     //check if page was changed, if so OPcycles needs to be
            OPcycles++;                                     //incremented
        }
        PC = addressparam;
    }
    logger.debug(__FUNCTION__ ,
                 "Branch if Overflow Flag clear");
}

void CPU6502::BVS(){
    PC++;
    if (getStatusFlag(V) == true) {
        //branch is taken, so additional cycle needed
        OPcycles++;
        addressparam = PC + address_rel;
        if ((addressparam & 0xFF00) != (PC & 0xFF00)) {     //check if page was changed, if so OPcycles needs to be
            OPcycles++;                                     //incremented
        }
        PC = addressparam;
    }
    logger.debug(__FUNCTION__ ,
                 "Branch if Overflow Flag set");
}


//Status Flag Changes

void CPU6502::SEC(){
    setStatusFlag(C, true);
    logger.debug(__FUNCTION__ ,
                 "Set Carry Flag");
}

void CPU6502::SED(){
    setStatusFlag(D, true);
    logger.debug(__FUNCTION__ ,
                 "Set Decimal Flag");
}

void CPU6502::SEI(){
    setStatusFlag(I, true);
    logger.debug(__FUNCTION__ ,
                 "Set Interrupt Disable Flag");
}

void CPU6502::CLC(){
    setStatusFlag(C, false);
    logger.debug(__FUNCTION__ ,
                 "Clear Carry Flag");
}

void CPU6502::CLD(){
    setStatusFlag(D, false);
    logger.debug(__FUNCTION__ ,
                 "Clear Decimal Flag");
}

void CPU6502::CLI(){
    setStatusFlag(I, false);
    logger.debug(__FUNCTION__ ,
                 "Clear Interrupt Disable Flag");
}

void CPU6502::CLV(){
    setStatusFlag(V, false);
    logger.debug(__FUNCTION__ ,
                 "Clear Overflow Flag");
}

//System Functions
void CPU6502::BRK(){
    PC++;
    setStatusFlag(B, true);
    setStatusFlag(B2, true);
    bus->busWrite(0x0100 + SP, (PC >> 8) & 0x00FF);  //push high byte of PC onto stack
    SP--;
    bus->busWrite(0x0100 + SP, PC & 0x00FF);        //push low byte of PC onto stack
    SP--;
    bus->busWrite(0x0100 + SP, SR);
    SP--;
    setStatusFlag(I, true);
    //set PC to IRQ vector

    //PC = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8); does not work yet as this address is not implemented
    // yet
}

void CPU6502::NOP(){}

void CPU6502::RTI(){
    SP++;
    SR = bus->busRead(0x0100 + SP);
    SP++;
    uint16_t low = bus->busRead(0x0100 + SP);
    SP++;
    uint16_t high = (bus->busRead(0x0100 + SP) << 0);
    PC = high + low;
    logger.debug(__FUNCTION__ ,
                 "Return From Interrupt");
}



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



void CPU6502::testInstruction(uint8_t opcode) {
    CPU6502::op_code = opcode;
    CPU6502::X = 0x07;
    CPU6502::Y = 0x05;
    ACC = 0xFF;

    //zp nimmt 0x0001, abs auch 0X0002, imm benutzt die daten in 0x0001 als argument
    CPU6502::write(0x0001, 0xFF);
    CPU6502::write(0x0002, 0x01);

    CPU6502::write(0x0014, 0x02);
    CPU6502::write(0x0015, 0x01);

    //abs
    CPU6502::write(0x0110, 0x15);
    //zp
    CPU6502::write(0x0010, 0x14);
    //zpx
    CPU6502::write(0x0017, 0x12);
    //abx
    CPU6502::write(0x0117, 0x40);
    //aby
    CPU6502::write(0x0115, 0x16);

    //izy
    CPU6502::write(0x0010, 0x14);
    CPU6502::write(0x0011, 0x01);
    CPU6502::write(0x0119, 0x64);

    //izx
    CPU6502::write(0x0017, 0x12);
    CPU6502::write(0x0018, 0x01);
    CPU6502::write(0x0112, 0x72);

    //Rel
    //PC = 0xC8;
    //write(PC+1, 0x80);


    CPU6502::displayMemoryPage(0);
    CPU6502::EXC_OP();
    CPU6502::displayMemoryPage(0);
    CPU6502::displayRegisters();
    CPU6502::diplayFlags();

}

std::string CPU6502::intToHexString(uint8_t val) {
    std::stringstream ss;
    ss  << std::hex << (val|0);
    return ss.str();
}

void CPU6502::displayMemoryRange(uint16_t start, uint16_t finish) {
    if (start > finish) {
        return;
    }
    std::cout
    << std::string(45, '-')
    << "Memory from " << start << " to " << finish
    << std::string(45, '-') << std::endl;
    int i = 0;
    while (start <= finish) {
        std::cout
        << "| 0x"
        << std::setfill('0')
        <<  intToHexString(this->read(start)).c_str()
        << " |";
        start++;
        i++;
        if (i % 16 == 0) {
            std::cout << std::endl;
        }
    }
}

void CPU6502::displayMemoryPage(int page) {
    uint16_t addr = page*0xFF;
    displayMemoryRange(addr + page, addr + 0xFF + page);
}

void CPU6502::displayRegisters() {
    std::cout
    << "| X   : " << unsigned(X)<< " |\n"
    << "| Y   : " << unsigned(Y) << " |\n"
    << "| ACC : " << unsigned(ACC) << " |\n"
    << "| PC  : " << unsigned(PC) << " |\n"
    << "| SP  : " << unsigned(SP) << " |\n"
    << std::endl;

}

void CPU6502::diplayFlags() {
    std::cout
    << "| C : " << getStatusFlag(C) << " |" << std::endl
    << "| Z : " << getStatusFlag(Z) << " |" << std::endl
    << "| I : " << getStatusFlag(I) << " |" << std::endl
    << "| D : " << getStatusFlag(D) << " |" << std::endl
    << "| V : " << getStatusFlag(V) << " |" << std::endl
    << "| N : " << getStatusFlag(N) << " |" << std::endl << std::endl;
}

void CPU6502::setNegative(uint8_t val) {
    if (val & EIGHTH) {
        this->setStatusFlag(N, true);
    } else {
        this->setStatusFlag(N, false);
    }
}

void CPU6502::setZero(uint8_t val) {
    if (val == 0) {
        this->setStatusFlag(Z, true);
    } else {
        this->setStatusFlag(Z, false);
    }
}
