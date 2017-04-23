#include "regfile.h"

void initREG() //Initial the register
{
  for(int i=0; i<REG_SIZE; i++) REG[i] = 0;
  PC = 0;
  HI = 0;
  LO = 0;
}
