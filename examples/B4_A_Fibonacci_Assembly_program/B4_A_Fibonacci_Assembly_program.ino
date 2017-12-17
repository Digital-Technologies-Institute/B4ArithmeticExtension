 #include <B4ArithmeticExtension.h>

B4 myB4;
/*  
 *  Fibonacci
 */
// Decimal Display positioned north of classic Latch (as per handbook)
String assemblyProgram = "LOAD_A(13);ADD_A(14);WRT_A(15);LOAD_A(14);PRINT();WRT_A(13);LOAD_A(15);WRT_A(14);LOAD_A(12);SUB(1);WRT_A(12);JNZ(15);NOP(7);NOP(0);NOP(1);NOP(0);";
void setup()
{
  Serial.begin(9600);
  myB4.assembler(&assemblyProgram);
  myB4.programB4();
}

void loop()
{
}
