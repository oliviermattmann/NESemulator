//
// Created by matia on 10/11/2020.
//

#include "CPU6502.h"
#include "Bus.h"



CPU6502::CPU6502() {
    this->X = 0x0;
    this->Y = 0x0;
    this->Y = 0x0;
    this->PC = 0x0;
    this->C = 0x0;
    /*
    void (CPU6502::*function())(void) = &CPU6502::SEC;
    OP_CODE a = {
            , 0
    };
    this->OP_TABLE[0] =
     */
}

void CPU6502::SEC() {
    this->C = true;
}

void CPU6502::SED() {
    this->D = true;
}

void CPU6502::EXC_OP(int8_t op_code) {
    unsigned char lo = op_code & 0x0f;
    unsigned char hi = (byte >> 4) & 0x0f;
    CPU6502::OP_TABLE[hi][lo]->op();
}

void CPU6502::run(Bus bus) {
    while (1) {
        int8_t instrc = bus.busRead(CPU6502::PC);
        CPU6502::EXC_OP(instrc);
        printf("%d", this->C)
        break;
    }
}
