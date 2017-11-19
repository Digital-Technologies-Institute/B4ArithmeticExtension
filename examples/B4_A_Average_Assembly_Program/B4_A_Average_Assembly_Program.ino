 #include <B4ArithmeticExtension.h>
/*  
 *  (6+8)/2=7
 */
 
B4 myB4;
//String assemblyProgram = "LOAD_A(11);ADD_A(12);WRT_A(13);NOP(0);LOAD_A(15);ADD(1);WRT_A(15);LOAD_A(13);SUB_A(14);WRT(13);JNZ(3);NOP(6);NOP(8);NOP(0);NOP(2);NOP(0);";
String assemblyProgram = "LOAD(5);ADD(5);WRT_A(13);NOP(0);LOAD_A(15);ADD_A(12);WRT_A(15);LOAD_A(13);SUB_A(14);WRT_A(13);JNZ(3);JMP(15);NOP(1);NOP(0);NOP(2);NOP(0);";

void setup()
{
  Serial.begin(9600);
  myB4.assembler(&assemblyProgram);
  myB4.programB4();
}

void loop()
{
}

