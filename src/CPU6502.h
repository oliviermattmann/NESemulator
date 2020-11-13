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


    OP_CODE OP_TABLE[256]{};

    /**
    * Executes the saved Operation Code.
    */
    void EXC_OP();

    /**
     *  To check on a specific bit. E.g.
     *  if (address & THIRD)
     *  Improves readability.
     */
    enum BitMask {
        FIRST = 1 << 0,
        SECOND = 1 << 1,
        THIRD = 1 << 2,
        FOURTH = 1 << 3,
        FIFTH = 1 << 4,
        SIXED = 1 << 5,
        SEVENTH = 1 << 6,
        EIGHTH = 1 << 7
    };

    /* Main Addressing Modes */
private:
    /**
     * Implied
     */
    void imp();

    /**
     * Immediate
     */
    void imm();

    /**
     * From Accumulator
     */
    void acc();

    /**
     * Zero Page
     */
    void zp();

    /**
     * Zero page X-Indexed
     */
    void zpx();

    /**
     * Zero page Y-Indexed
     */
    void zpy();

    /**
     * Absolute addressing
     */
    void abs();

    /**
     * Absolut,X
     */
    void abx();

    /**
     * Absolut,Y
     */
    void aby();

    /**
     * Relative
     */
    void rel();

    /**
     * Indirect
     */
    void ind();

    /**
     * Indexed indirect
     */
    void izx();

    /**
     * Indirect Indexed
     */
    void izy();

    // Suggestion:
    // if an operation requires no special indexing we can
    // point to an empty function, we avoid checking for nullptr
    // in EXC_OP
    void void_indexed() {}

    /* Operations of the CPU */


    //Load/Store Operations
    /**
     * Load Accumulator
     */
    void LDA();

    /**
     * Load X Register
     */
    void LDX();

    /**
     * Load Y Register
     */
    void LDY();

    /**
     * Store Accumulator
     */
    void STA();

    /**
     * Store X Register
     */
    void STX();

    /**
     * Store Y Register
     */
    void STY();


    //Register Transfers
    /**
     * Transfer Accumulator to X
     */
    void TAX();

    /**
     * Transfer Accumulator to Y
     */
    void TAY();

    /**
     * Transfer X to Accumulator
     */
    void TXA();

    /**
     * Transfer Y to Accumulator
     */
    void TYA();


    //Stack Operations
    /**
     * Transfer Stack Pointer to X
     */
    void TSX();

    /**
     * Transfer X to Stack Pointer
     */
    void TXS();

    /**
     * Push Accumulator on stack
     */
    void PHA();

    /**
     * Push Processor Status on Stack
     */
    void PHP();

    /**
     * Pull Accumulator from Stack
     */
    void PLA();

    /**
     * Pull Processor Status from Stack
     */
    void PLP();

    //Logical
    /**
     * Logical AND
     */
    void AND();

    /**
     * Exclusive OR
     */
    void EOR();

    /**
     * Logical Inclusive OR
     */
    void ORA();

    /**
     * Bit Test
     */
    void BIT();

    //Arithmetic
    /**
     * Add with Carry
     */
    void ADC();

    /**
     * Subtract with Carry
     */
    void SBC();

    /**
     * Compare Accumulator
     */
    void CMP();

    /**
     * Compare X Register
     */
    void CPX();

    /**
     * Compare Y Register
     */
    void CPY();


    //Increments/Decrements
    /**
     * Increment a memory location
     */
    void INC();

    /**
     * Increment the X Register
     */
    void INX();

    /**
     * Increment the Y Register
     */
    void INY();

    /**
     * Decrement a memory location
     */
    void DEC();

    /**
     * Decrement the X Register
     */
    void DEX();

    /**
     * Decrement the Y Register
     */
    void DEY();


    //Shifts
    /**
     * Arithmetic Shift Left
     */
    void ASL();

    /**
     * Arithmetic Shift Right
     */
    void LSR();

    /**
     * Rotate Left
     */
    void ROL();

    /**
     * Rotate Right
     */
    void ROR();


    //Jumps/Calls
    /**
     * Jump to another location
     */
    void JMP();

    /**
     * Jump to a subroutine
     */
    void JSR();

    /**
     * Return from subroutine
     */
    void RTS();


    //Branches
    /**
     * Branch if Carry Flag clear
     */
    void BCC();

    /**
     * Branch if Carry Flag set
     */
    void BCS();

    /**
     * Branch if Zero Flag set
     */
    void BEQ();

    /**
     * Branch if Negative Flag set
     */
    void BMI();

    /**
     * Branch if Zero Flag clear
     */
    void BNE();

    /**
     * Branch if Negative Flag clear
     */
    void BLP();

    /**
     * Branch if Overflow Flag clear
     */
    void BVC();

    /**
     * Branch if Overflow Flag set
     */
    void BVS();


    //Status Flag Changes
    /**
     * Set Carry Flag
     */
    void SEC();

    /**
     * Set Decimal Flag
     */
    void SED();

    /**
     * Set Interrupt Disable Flag
     */
    void SEI();

    /**
     * Clear Carry Flag
     */
     void CLC();

     /**
      * Clear Decimal Flag
      */
     void CLD();

     /**
      * Clear Interrupt Disable Flag
      */
     void CLI();

     /**
      * Clear overflow Flag
      */
     void CLV();

     //System Functions
     /**
      * Force an Interrupt
      */
     void BRK();

     /**
      * No Operation
      */
     void NOP();

     /**
      * Return from Interrupt
      */
     void RTI();


     //other
    void ISC();

    void KIL();

    void DCP();


    void AXS();
    void LAX();
    void SHX();
    void SHY();
    void TAS();
    void SAX();
    void AHX();
    void XAA();
    void ARR();
    void SRE();
    void ANC();
    void SLO();
    void ALC();
    void BPL();
    void LAS();
    void ALR();
    void RRA();
    void RLA();
};



#endif //NESEMULATOR_CPU6502_H
