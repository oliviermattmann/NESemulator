/*
 *  This header defines the CPU6502 emulating class.
 */


#ifndef NESEMULATOR_CPU6502_H
#define NESEMULATOR_CPU6502_H
#include <cstdint>
#include "Bus.h"

/**
 * Central processing unit emulating class.
 */
class CPU6502 {

    /**
     * Constructor for the CPU6502 emulating class.
     * @param bus
     */
public: CPU6502(Bus *bus);

    /**
     * Runs the CPU6502 Module.
     */
    void run();

    /* Main Registers */
public:
    int8_t X;       // X-Register
    int8_t Y;       // Y-Register
    int8_t ACC;     // Accumulator
    int16_t PC;     // Program Counter
    int8_t SP;      // Stack Pointer

    /* Status Flag Register */
public:
    bool Z;  // 0-Flag
    bool C;  // Carry
    bool I;  // Interrupt Disable
    bool D;  // Unused, Decimal
    bool V;  // Overflow
    bool N;  // Negative
    bool B;  // B Flag

    /* Bus Handling */
public:

    Bus *bus; // Pointer to the bus.

    /**
     * Writes a byte to a given address.
     * @param address ,address to write to.
     * @param data ,data to write.
     */
    void write(int16_t address, int8_t data);

    /**
     * Reads a byte from a given address.
     * @param address ,address to read from.
     * @return byte from given address.
     */
    int8_t read(int16_t address);

    /* Operation-Code Handling */
private:
    /**
     *  OP_CODE struct emulates an entry in the
     *  Operation-Code-Matrix and contains within
     *  it function pointers to the operations themselves,
     *  addressing mode functions and number of clock-cycles.
     */
    struct OP_CODE {
        void (CPU6502::*funcP)();
        void (CPU6502::*x)(void) = nullptr;
        int8_t cycles;
    };

    int8_t op_code;
    int16_t addressparam;


    OP_CODE OP_TABLE[16][16]{};

    /**
    * Executes the saved Operation Code.
    */
    void EXC_OP();

    /* Main Addressing Modes */
private:
    void zero_page_indexed_x();
    void zero_page_indexed_y();
    void absolute_indexed_x();
    void absolute_indexed_y();
    void indexed_indirect_x();
    void indirect_indexed_y();

    // Suggestion:
    // if an operation requires no special indexing we can
    // point to an empty function, we avoid checking for nullptr
    // in EXC_OP
    void void_indexed() {}

    /* Operations of the CPU */

    /**
     * Set Carry Flag
     */
    void SEC();

    /**
     * Set Decimal Flag
     */
    void SED();

    /**
     * Subtracts one from the value held at a specified memory location setting the
     * zero and negative flags as appropriate.
     */
    void DEC();

    /**
     * Clear Carry Flag, Implicit instruction.
     * (= No further operand needs to be specified.)
     */
     void CLC();


};



#endif //NESEMULATOR_CPU6502_H
