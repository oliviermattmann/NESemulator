//
// Created by matia on 10/11/2020.
//

#ifndef NESEMULATOR_CPU6502_H
#define NESEMULATOR_CPU6502_H
#include <cstdint>



class CPU6502 {

public:
    CPU6502();

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

    /* https://wiki.nesdev.com/w/index.php/CPU_addressing_modes */

    // Indexed addressing

    int8_t zero_page_indexed_x(int8_t address);

    int8_t zero_page_indexed_y(int8_t address);

    int8_t absolute_indexed_x(int8_t address);

    int8_t absolute_indexed_y(int8_t address);

    int8_t indexed_indirect_x(int8_t address);

    int8_t indirect_indexed_y(int8_t address);

    // Operations

    /**
     * Set Carry Flag
     */
    void SEC();

    /**
     * Set Decimal Flag
     */
    void SED();

    /**
     * Executes an Operation Code.
     */
    void EXC_OP(int8_t op_code);

    void run();

    OP_CODE OP_TABLE[256]{};

};

struct OP_CODE {
    void* f; //pointer to function
    void (CPU6502::*x)(void) = nullptr;
    int8_t cycles;
};

#endif //NESEMULATOR_CPU6502_H
