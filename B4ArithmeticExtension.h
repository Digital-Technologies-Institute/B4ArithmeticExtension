/*
  B4ArithmeticExtension.h - Library.
  Created by Karsten Schulz, Digital Technologies Institute April 30, 2017.
*/

#ifndef B4MultiplicationExtension_h
#define B4MultiplicationExtension_h

class B4
{
public:
    B4();
    void loadDataAndProgram(uint8_t mylocalData[], uint8_t mylocalProgram[]);
    void programB4();
    void assembler(String * assemblerProgram);
    uint8_t  latchOutput();
    void functionTest();
    int freeRAM();

  private:
    void setData();
    void setProgram();
    void clearDataRAM();
    void clearProgramRAM();
    void reSetProgramCounter();
    void clockCycle();
    void writeRAM(uint8_t port);
    void resetLatch();
    void printBinary(uint8_t v, uint8_t num_places);

  private:
    //program memory address bus (neutral position is B00XX)
    const uint8_t P3 = 5; // subtract (active HIGH)
    const uint8_t P2 = 4; // write Data RAM (active HIGH)
    const uint8_t P1 = 3; // 2-1 selector HIGH: RAM, LOW: Adder)
    const uint8_t P0 = 2; // User defined
    
    //data memory address bus
    const uint8_t D0 = 6;
    const uint8_t D1 = 7;
    const uint8_t D2 = 8;
    const uint8_t D3 = 9;
    
    //program counter reset. Connected to CLR PIN of Program Counter Module
    const uint8_t programCounterReset = A0;

    //program counter set. Connected to Set Program Counter PIN of Program Counter Module, Needed for jump instructions
    const uint8_t programCounterSet = A1;

    //selectDataRAMPCSource. LOW: Data RAM PC input is set by Program Counter Module. HIGH: Data RAM PC input is set by Latch. Needed for relative data addressing.
    
    const uint8_t selectDataRAMPCSource = A2;

    
    //control signals
    const uint8_t latchReset = 10; //active LOW
    const uint8_t writeProgramRAM = 11;
    const uint8_t writeDataRAM = 12;
    const uint8_t clockPIN = 13;
    const uint8_t programmerActive = A5;
    
    uint8_t programCounterValue = 0;
    uint8_t myData[16];
    uint8_t myProgram[16];
};

#endif

