/*
 B4MultiplicationExtension.cpp - Library.
 Created by Dr. Karsten Schulz, Digital Technologies Institute April 30, 2017.
 */

#include "Arduino.h"
#include "B4ArithmeticExtension.h"

B4::B4()
{
}

void B4::programB4()
{
    //Serial.begin(9600);
    pinMode(P0, OUTPUT);
    pinMode(P1, OUTPUT);
    pinMode(P2, OUTPUT);
    pinMode(P3, OUTPUT);
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(programCounterReset, OUTPUT);
    pinMode(programCounterSet, OUTPUT);
    pinMode(selectDataRAMPCSource, OUTPUT);
    pinMode(latchReset, OUTPUT);
    pinMode(writeProgramRAM, OUTPUT);
    pinMode(writeDataRAM, OUTPUT);
    pinMode(clockPIN, OUTPUT);
    pinMode(programmerActive, OUTPUT);
    
    //set the control signals to neutral position
    digitalWrite(programCounterReset, LOW);
    digitalWrite(programCounterSet, LOW);
    digitalWrite(selectDataRAMPCSource, LOW);
    digitalWrite(latchReset, HIGH);
    digitalWrite(writeProgramRAM, HIGH);
    digitalWrite(writeDataRAM, HIGH);
    digitalWrite(clockPIN, LOW);
    digitalWrite(programmerActive, LOW);
    //Start Programming
    Serial.println("Activating Automatic Programmer");
    
    digitalWrite(programmerActive, HIGH);
    
    Serial.println("Resetting Latch to 0000");
    resetLatch();
    
    Serial.println("Clearing Data RAM");
    clearDataRAM();
    
    Serial.println("Clearing Program RAM");
    clearProgramRAM();
    
    Serial.println("Programming Data RAM");
    setData();
    
    Serial.println("Programming Program RAM");
    setProgram();
    
    
    reSetProgramCounter();
    uint8_t a=0;
    while (a<15)
    {
        clockCycle();
        a++;
    }

    Serial.println("Resetting Latch to 0000");
    resetLatch();

    
    Serial.println("Deactivating Automatic Programmer");
    //switch ports to high impedance to avoid interference with the B4's own signals
    pinMode(P0, INPUT);
    pinMode(P1, INPUT);
    pinMode(P2, INPUT);
    pinMode(P3, INPUT);
    pinMode(D0, INPUT);
    pinMode(D1, INPUT);
    pinMode(D2, INPUT);
    pinMode(D3, INPUT);
    pinMode(programCounterSet, INPUT);
    pinMode(programCounterReset, INPUT);
    pinMode(selectDataRAMPCSource, INPUT);
    pinMode(latchReset, INPUT);
    pinMode(writeProgramRAM, INPUT);
    pinMode(writeDataRAM, INPUT);
    pinMode(clockPIN, INPUT);
    digitalWrite(programmerActive, LOW);
    
    Serial.println("Programming Complete");
    Serial.println("You can now run your program on the B4 by pressing the 'Enter' button on the Program Counter.");
    //the programming is now done
    
}

void B4::setData()
{
    reSetProgramCounter();
    while (programCounterValue < 16)
    {
        digitalWrite(D3, HIGH && (myData[programCounterValue] & B1000));
        digitalWrite(D2, HIGH && (myData[programCounterValue] & B0100));
        digitalWrite(D1, HIGH && (myData[programCounterValue] & B0010));
        digitalWrite(D0, HIGH && (myData[programCounterValue] & B0001));
        writeRAM(writeDataRAM);
        clockCycle();
    }
}
void B4::setProgram()
{
    reSetProgramCounter();
    while (programCounterValue < 16)
    {
        digitalWrite(P3, HIGH && (myProgram[programCounterValue] & B1000));
        digitalWrite(P2, HIGH && (myProgram[programCounterValue] & B0100));
        digitalWrite(P1, HIGH && (myProgram[programCounterValue] & B0010));
        digitalWrite(P0, HIGH && (myProgram[programCounterValue] & B0001));
        writeRAM(writeProgramRAM);
        clockCycle();
    }
}

void B4::clearDataRAM()
{
    reSetProgramCounter();
    while (programCounterValue < 16)
    {
        digitalWrite(D0, LOW);
        digitalWrite(D1, LOW);
        digitalWrite(D2, LOW);
        digitalWrite(D3, LOW);
        writeRAM(writeDataRAM);
        clockCycle();
    }
}

void B4::clearProgramRAM()
{
    reSetProgramCounter();
    while (programCounterValue < 16)
    {
        digitalWrite(P0, LOW);
        digitalWrite(P1, LOW);
        digitalWrite(P2, LOW);
        digitalWrite(P3, LOW);
        writeRAM(writeProgramRAM);
        clockCycle();
    }
}

void B4::reSetProgramCounter()
{
    // for HW Revision 1.0: programCounterReset is active LOW
    // for HW Revision>1.1: programCounterReset is active HIGH
    pinMode(programCounterReset, OUTPUT);
    digitalWrite(programCounterReset, HIGH); //active
    delay(100);
    digitalWrite(programCounterReset, LOW); //passive
    programCounterValue = 0;
    pinMode(programCounterReset, INPUT);
}

void B4::clockCycle()
{
    //if (programCounterValue==15)
        //delay (10000);
    digitalWrite(clockPIN, HIGH);
    delay(50);
    digitalWrite(clockPIN, LOW);
    programCounterValue++;
}

void B4::writeRAM(uint8_t port)
{
    digitalWrite(port, LOW);
    delay(50);
    digitalWrite(port, HIGH);
    delay(50);
}

void B4::resetLatch()
{
    //reset the latch.
    digitalWrite(latchReset, LOW);
    delay(100);
    digitalWrite(latchReset, HIGH);
}

void B4::loadDataAndProgram(uint8_t mylocalData[], uint8_t mylocalProgram[])
{
    Serial.println("\nI will upload the following to the B4");
    Serial.println("DRAM | PRAM");
    Serial.println("-----------");
    
    uint8_t i=0;
    while (i<16)
    {
        myData[i]=mylocalData[i];
        printBinary(myData[i],4);
        //Serial.print(myData[i],BIN);
        Serial.print(" | ");
        myProgram[i]=mylocalProgram[i];
        Serial.println(myProgram[i],BIN);
        i++;
    }
}

void B4::assembler(String * assemblerProgram)
{
    Serial.print("Assembly Program Original: ");
    Serial.println(*assemblerProgram);

    String assemblerProgramLines[16];
    String assemblerCodes[] = {"LOAD_A", "ADD_A", "SUB_A", "WRT_A", "JMP", "JNZ", "LOAD", "ADD", "SUB", "PRINT", "NOP"};
    uint8_t machineCodes[] = {B1111, B1110, B1101, B1100, B1011, B1010, B1001, B1000, B0111, B0110, B0000};
    uint8_t machineCodesCount = sizeof(machineCodes);
    String assemblerCode;
    String dataCode;
    uint8_t semicolonIndex = 0;
    uint8_t openBracketIndex = 0;
    uint8_t closingBracketIndex = 0;
    uint8_t programCounter = 0;
    uint8_t programLength = 0;
    uint8_t counter=0;
    B4 myBlueberry;
    
    uint8_t DataRAMContent[] = {
        B0000, B0000, B0000, B0000,
        B0000, B0000, B0000, B0000,
        B0000, B0000, B0000, B0000,
        B0000, B0000, B0000, B0000,
    };
    
    uint8_t ProgramRAMContent[] = {
        B0000, B0000, B0000, B0000,
        B0000, B0000, B0000, B0000,
        B0000, B0000, B0000, B0000,
        B0000, B0000, B0000, B0000,
    };
    
    //first, we break the assemblerProgram into its individual commands. The ';' separates them.
    Serial.println("\nSeparating program into individual commands ...");
    
    while ((semicolonIndex != -1) && (programCounter <= 15))
    {
        semicolonIndex = assemblerProgram->indexOf(';');
        assemblerProgramLines[programCounter] = assemblerProgram->substring(0, semicolonIndex);
        *assemblerProgram = assemblerProgram->substring(semicolonIndex + 1); // We can now remove the command from the assemblerProgram string.
        Serial.println(assemblerProgramLines[programCounter]);
        programCounter++;
        /*Serial.print("Semicolon Index: ");
        Serial.println(semicolonIndex);
        Serial.print("Assembler Program: ");
        Serial.println(assemblerProgram);*/
    }
    Serial.print("\nProgram length: ");
    Serial.println(programCounter);
    programLength=programCounter-1; //Programs start at index 0;
    
    //next, we break each program line into program and data component and translate the assembly codes into machine codes.
    
    Serial.println("\nNow breaking each program line into program and data component ...");

    programCounter=0;
    while (programCounter <= programLength)
    {
        openBracketIndex = assemblerProgramLines[programCounter].indexOf('('); // The identifier is now the opening bracket
        if (openBracketIndex!=-1)
        {
            assemblerCode=assemblerProgramLines[programCounter].substring(0, openBracketIndex);
            Serial.print(assemblerCode);
            Serial.print(" , ");

            counter=0;
            while (counter<machineCodesCount)
            {
                if (assemblerCode.equals(assemblerCodes[counter]))
                {
                    ProgramRAMContent[programCounter]=machineCodes[counter];
                    counter=machineCodesCount; //we then exit
                }
                counter++;
            }
        }
        // Next, look for the data
        assemblerProgramLines[programCounter]=assemblerProgramLines[programCounter].substring(openBracketIndex + 1);
        closingBracketIndex = assemblerProgramLines[programCounter].indexOf(')'); // The identifier is now the closing bracket
        if (closingBracketIndex!=-1)
        {
            dataCode=assemblerProgramLines[programCounter].substring(0, closingBracketIndex);
            Serial.println(dataCode);
            DataRAMContent[programCounter]=dataCode.toInt();
        }
        programCounter++;
    }
    
    Serial.println("\nTranslating mnemonics into machine code ...");
    
    /*
    programCounter=0;
    Serial.println("Program Machine Code");
    while (programCounter <= 15)
    {
        Serial.println(ProgramRAMContent[programCounter],BIN);
        programCounter++;
    }
    
    Serial.println("Data Machine Code");
    programCounter=0;
    while (programCounter <= 15)
    {
        Serial.println(DataRAMContent[programCounter],BIN);
        programCounter++;
    }
    Serial.println("--------------");*/
    
    loadDataAndProgram(DataRAMContent, ProgramRAMContent);
}

uint8_t B4::latchOutput()
{
    uint8_t output=0;
    output=output+digitalRead(D0);
    output=output+digitalRead(D1)*2;
    output=output+digitalRead(D2)*4;
    output=output+digitalRead(D3)*8;
    return output;
}

void B4::functionTest()
{
    String assemblerProgram= "LOAD(1);ADD(2);WRT();SUB(2);WRT();ADD(4);WRT();SUB(1);WRT();ADD(11);WRT();SUB(1);WRT();ADD(2);SUB(1);WRT()";
    assembler(&assemblerProgram);
    programB4();
    
    pinMode(clockPIN, OUTPUT);
    pinMode(programCounterReset, OUTPUT);

    reSetProgramCounter();
    
    while (programCounterValue<15)
    {
        //clockCycle();
        delay(500);
    }
        
    pinMode(programCounterReset, INPUT);
    pinMode(clockPIN, INPUT);

}
void B4::printBinary(uint8_t v, uint8_t num_places)
{
    uint8_t mask=0, n;
    
    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    v = v & mask;  // truncate v to specified number of places
    
    while(num_places)
    {
        
        if (v & (0x0001 << num_places-1))
        {
            Serial.print("1");
        }
        else
        {
            Serial.print("0");
        }
        
        --num_places;
        if(((num_places%4) == 0) && (num_places != 0))
        {
            Serial.print("_");
        }
    }
}

int B4::freeRAM()
{
    extern int __heap_start, *__brkval;
    int v;
    int freeRAM= (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
    Serial.print("Free RAM= ");
    Serial.println(freeRAM);
}


