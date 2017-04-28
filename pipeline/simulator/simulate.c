#include <stdio.h>
#include "piperpt.h"
#include "memory.h"
#include "regfile.h"
#include "pipereg.h"
#include "pipeline.h"

void initialSetting()
{
  initOutputSetting();
  initMem();
  initREG();
}

int main()
{
  //Initial the simlator
  unsigned int cycles;
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
