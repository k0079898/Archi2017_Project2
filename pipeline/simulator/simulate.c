#include <stdio.h>
#include "piperpt.h"
#include "memory.h"
#include "regfile.h"
#include "pipereg.h"

void initialSetting()
{
  initOutputSetting();
  initMem();
  initREG();
  initPipeReg();
}

int main()
{
  //Initial the simlator
  unsigned int cycles, halt;
  initialSetting();
  readBin();
  writeMem();
  cycles = 0;
  initPipeReg();
  writeInitSnapshot();
  //Do something
  closeFile();
  return 0;
}
