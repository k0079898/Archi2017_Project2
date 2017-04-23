#include <stdio.h>
#include "piperpt.h"
#include "memory.h"
#include "regfile.h"

void initialSetting()
{
  initOutputSetting();
  initMem();
  initREG();
}

int main()
{
  //Initial the simlator
  unsigned int cycles, halt;
  initialSetting();
  readBin();
  writeMem();
  cycles = 0;
  writeInitSnapshot();
  //Do something
  closeFile();
  return 0;
}
