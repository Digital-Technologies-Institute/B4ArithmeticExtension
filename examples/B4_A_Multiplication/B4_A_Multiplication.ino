 #include <B4ArithmeticExtension.h>
 
B4 myB4;
/*  
 *  3x2
 */

  uint8_t DataRAMContent[] = {
    B0000, B1111, B1101, B1111,
    B1110, B1100, B1110, B0000,
    B1111, B0000, B0000, B0000,
    B1111, B0010, B0011, B0000,
  };

  uint8_t ProgramRAMContent[] = {
    B0000, B1111, B1110, B1100,
    B1111, B1110, B1100, B1010,
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
