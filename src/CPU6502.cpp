//
// Created by matia on 10/11/2020.
//

#include "CPU6502.h"
#include <iostream>



CPU6502::CPU6502() {
    this->X = 0x0;
    this->Y = 0x0;
    this->Y = 0x0;
    this->PC = 0x0;
    this->C = 0x0;
    this->D = 0x0;


    OP_TABLE[0][0] = {&CPU6502::SEC, 1};
    OP_TABLE[1][0] = {&CPU6502::SED, 1};
}



void CPU6502::EXC_OP(int8_t op_code) {
    unsigned char lo = op_code & 0x0f;
    unsigned char hi = (op_code >> 4) & 0x0f;
    (this->*OP_TABLE[hi][lo].funcP)();
}

void CPU6502::run(Bus bus) {
    while (1) {
        int8_t instrc = bus.busRead(CPU6502::PC);
        CPU6502::EXC_OP(instrc);
        std::cout << this->C << "\n";
        PC++;
        instrc = bus.busRead(CPU6502::PC);
        CPU6502::EXC_OP(instrc);
        std::cout << this->D << "\n";
        break;
    }

}
/*OP functions*/
void CPU6502::SEC() {
    this->C = true;
}

void CPU6502::SED() {
    this->D = true;
}
