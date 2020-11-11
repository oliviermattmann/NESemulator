//
// Created by matia on 10/11/2020.
//

#include "CPU6502.h"
#include <iostream>



CPU6502::CPU6502(Bus *bus) {
    this->bus = bus;
    this->X = 0x00;
    this->Y = 0x0;
    this->ACC = 0x0;
    this->PC = 0x0;
    this->C = 0x0;
    this->D = 0x0;
    op_code = 0x0;
    addressparam = 0;


//  OP_TABLE[0][0] = {&CPU6502::SEC,, 1};
    OP_TABLE[1][0] = {&CPU6502::DEC,&CPU6502::zero_page_indexed_x, 1};


}



void CPU6502::EXC_OP() {
    unsigned char lo = op_code & 0x0f;
    unsigned char hi = (op_code >> 4) & 0x0f;
    (this->*OP_TABLE[hi][lo].x)();
    (this->*OP_TABLE[hi][lo].funcP)();
    PC++;
}

void CPU6502::run() {
    while (1) {
        op_code = read(CPU6502::PC);
        CPU6502::EXC_OP();
        /*op_code = read(CPU6502::PC);
        CPU6502::EXC_OP();
        std::cout << this->D << "\n";*/
        break;
    }
}

/*Addressing modes*/
//works
void CPU6502::zero_page_indexed_x() {
    PC++;
    addressparam = read(PC);
    addressparam = (addressparam + this->X)%256;
}
//works
void CPU6502::zero_page_indexed_y() {
    PC++;
    addressparam = read(PC);
    addressparam = (addressparam + this->Y)%256;
}
//works
void CPU6502::absolute_indexed_x() {
    PC++;
    int8_t lo = read(PC);
    PC++;
    int8_t hi = read(PC);
    addressparam = (hi << 8) | lo;
    addressparam += this->X;
}
//works
void CPU6502::absolute_indexed_y() {
    PC++;
    int8_t lo = read(PC);
    PC++;
    int8_t hi = read(PC);
    addressparam = (hi << 8) | lo;
    addressparam += this->Y;
}
//Doesnt work yet
void CPU6502::indexed_indirect_x() {
    PC++;
    addressparam = read(read((read(PC) + this->X)%256) + read((read(PC) + this->X + 1)%256)*256);
}
//Doesnt work yet
void CPU6502::indirect_indexed_y() {
    PC++;
    addressparam = read(read((read(PC) + this->Y)%256) + read((read(PC) + this->Y + 1)%256)*256);
}



/*OP functions*/
void CPU6502::SEC() {
    this->C = true;
}

void CPU6502::SED() {
    this->D = true;
}

void CPU6502::DEC() {
    printf("%d\n", read(addressparam));
    write(addressparam, read(addressparam)-1);
    printf("%d\n", read(addressparam));
}

void CPU6502::write(int16_t address, int8_t data) {
    bus->busWrite(address,data);
}

int8_t CPU6502::read(int16_t address) {
    return bus->busRead(address);
}
