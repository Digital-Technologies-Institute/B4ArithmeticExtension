 #include <B4ArithmeticExtension.h>

B4 myB4;
/*  
 *  15/3
 */
 
String assemblyProgram = "NOP(0);LOAD_A(15);ADD_A(12);WRT_A(15);LOAD_A(13);SUB_A(14);WRT_A(13);JNZ(0);JMP(15);NOP(0);NOP(0);NOP(0);NOP(1);NOP(15);NOP(3);NOP(0);";

void setup()
{
  Serial.begin(9600);
  myB4.assembler(&assemblyProgram);
  myB4.programB4();
}

void loop()
{
}
