//
// Created by matia on 10/11/2020.
//

#ifndef NESEMULATOR_CPU6502_H
#define NESEMULATOR_CPU6502_H
#include <cstdint>

class CPU6502 {
    int8_t X;
    int8_t Y;
    int8_t ACC;
    //Programm counter
    int16_t PC;
    //Stack Pointer
    int8_t SP;
    //Flags
    // Zero
    bool Z;
    // Carry
    bool C;
    // Interrupt disable
    bool I;
    // Decimal (not used)
    bool D;
    // Overflow
    bool V;
    // Negative
    bool N;
    // B Flag
    bool B;



};


#endif //NESEMULATOR_CPU6502_H
