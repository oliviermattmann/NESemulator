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
    addressparam = 2;
    OP_TABLE[0][0] = {&CPU6502::SEC, &CPU6502::void_indexed, 1};
    OP_TABLE[1][0] = {&CPU6502::DEC,&CPU6502::indexed_indirect_x, 1};
    OP_TABLE[2][0] = {&CPU6502::SED, &CPU6502::void_indexed, 1};
    OP_TABLE[3][0] = {&CPU6502::CLC, &CPU6502::void_indexed, 1};

}

/**
 *  Runs the CPU6052.
 */
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

/**
 * Executes a single operation code.
 */
void CPU6502::EXC_OP() {
    unsigned char lo = op_code & 0x0f;
    unsigned char hi = (op_code >> 4) & 0x0f;
    (this->*OP_TABLE[hi][lo].x)();
    (this->*OP_TABLE[hi][lo].funcP)();
    PC++;
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
    char lo = read((read(PC)+this->X)%256);
    char hi = read((read(PC)+this->X + 1)%256);


    addressparam = (hi << 8) | lo;
}
//Doesnt work yet
void CPU6502::indirect_indexed_y() {
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

void CPU6502::CLC() {
    this->C = false;
}


