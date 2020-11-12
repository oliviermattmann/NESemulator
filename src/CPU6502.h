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

    /* Main Addressing Modes */
private:
    void imp();
    void imm();
    void acc();
    void zp();
    void zpx();
    void zpy();
    void abs();
    void abx();
    void aby();
    void rel();
    void ind();
    void izx();
    void izy();

    // Suggestion:
    // if an operation requires no special indexing we can
    // point to an empty function, we avoid checking for nullptr
    // in EXC_OP
    void void_indexed() {}

    /* Operations of the CPU */


    //Load/Store Operations
    void LDA();

    void LDX();

    void LDY();

    void STA();

    void STX();

    void STY();


    //Register Transfers
    void TAX();

    void TAY();

    void TXA();

    void TYA();


    //Stack Operations
    void TSX();

    void TXS();

    void PHA();

    void PHP();

    void PLA();

    void PLP();

    //Logical

    void AND();

    void EOR();

    void ORA();

    void BIT();

    //Arithmetic

    void ADC();

    void SBC();

    void CMP();

    void CPX();

    void CPY();


    //Increments/Decrements

    void INC();

    void INX();

    void INY();

    void DEC();

    void DEX();

    void DEY();


    //Shifts

    void ASL();

    void LSR();

    void ROL();

    void ROR();


    //Jumps/Calls

    void JMP();

    void JSR();

    void RTS();


    //Branches

    void BCC();

    void BCS();

    void BEQ();

    void BMI();

    void BNE();

    void BLP();

    void BVC();

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

    void SEI();

    /**
     * Clear Carry Flag
     */
     void CLC();

     void CLD();

     void CLI();

     void CLV();

     //System Functions
     void BRK();

     void NOP();

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
