#include <stdio.h>
#include <string.h>
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

void detectSimpleEnd()
{
    if( strcmp(prevDM_WB.instName, "HALT")==0 ) halt=1;
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
    writeToRegZero = 0;
    overwriteHILO = 0;
    memAddOverflow = 0;
    dataMisaligned = 0;
    numberOverflow = 0;
    while(halt!=1)
    {
        writeZeroError(cycles);
        writeError(cycles);
        writeSnapshotREG(cycles);
        WBprocess();
        DMprocess();
        EXprocess();
        IDprocess();
        IFprocess();
        writeSnapshotPipe(cycles);
        if(flush!=1 && stall!=1) PC = PC + 4;
        else if(flush) PC = jumpAddress;
        cycles++;
        detectSimpleEnd();
    }
    closeFile();
    return 0;
}
