 #include <B4ArithmeticExtension.h>
 
B4 myB4;
/*  
 *  15/3
 */

  uint8_t DataRAMContent[] = {
    B0000, B1111, B1100, B1111,
    B1101, B1110, B1101, B0000,
    B1111, B0000, B0000, B0000,
    B0001, B1111, B0011, B0000,
  };

  uint8_t ProgramRAMContent[] = {
    B0000, B1111, B1110, B1100,
    B1111, B1101, B1100, B1010,
    B1011, B0000, B0000, B0000,
    B0000, B0000, B0000, B0000,
  };

void setup()
{
  myB4.loadDataAndProgram(DataRAMContent, ProgramRAMContent);
  myB4.programB4();
}

void loop()
{
}
