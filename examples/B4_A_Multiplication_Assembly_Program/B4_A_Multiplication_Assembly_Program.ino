#include <B4ArithmeticExtension.h>
/*  
 *  3x2
 */

B4 myB4;
String assemblyProgram = "NOP(0);LOAD_A(15);ADD_A(13);WRT_A(15);LOAD_A(14);SUB_A(12);WRT_A(14);JNZ(0);JMP(15);NOP(0);NOP(0);NOP(0);NOP(1);NOP(2);NOP(3);NOP(0);";
void setup()
{
  Serial.begin(9600);
  myB4.assembler(&assemblyProgram);
  myB4.programB4();
}

void loop()
{
}
