//
// Created by matia on 10/11/2020.
//

#ifndef NESEMULATOR_CPU6502_H
#define NESEMULATOR_CPU6502_H
#include <cstdint>
#include "Bus.h"



class CPU6502 {

public:

//Registers and Flags
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


    Bus *bus;
    int8_t op_code;
    int16_t addressparam;


    struct OP_CODE {
        void (CPU6502::*funcP)();
        void (CPU6502::*x)(void) = nullptr;
        int8_t cycles;
    };
    OP_CODE OP_TABLE[16][16]{};

    CPU6502(Bus *bus);

    void write(int16_t address, int8_t data);
    int8_t read(int16_t address);

    /* https://wiki.nesdev.com/w/index.php/CPU_addressing_modes */
    // Indexed addressing

    void zero_page_indexed_x();

    void zero_page_indexed_y();

    void absolute_indexed_x();

    void absolute_indexed_y();

    void indexed_indirect_x();

    void indirect_indexed_y();

    // Operations

    /**
     * Set Carry Flag
     */
    void SEC();

    /**
     * Set Decimal Flag
     */
    void SED();

    void DEC();

    /**
     * Executes the saved Operation Code.
     */
    void EXC_OP();

    void run();


};



#endif //NESEMULATOR_CPU6502_H
