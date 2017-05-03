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

void detectEnd()
{
    if( strcmp(prevDM_WB.inst.name, "HALT")==0 ) halt=1;
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
    memset(&errorDetect, 0, sizeof(error));
    while(halt!=1)
    {
        writeError(cycles);
        writeSnapshotREG(cycles);
        WBprocess();
        DMprocess();
        EXprocess();
        IDprocess();
        IFprocess();
        IF_ID.stall = stall;
        writeSnapshotPipe(cycles);
        if(flush!=1 && stall!=1) PC = PC + 4;
        else if(flush)
        {
            printf("%08X\n", jumpAddress);
            PC = jumpAddress;
        }
        cycles++;
        detectEnd();
    }
    writeError(cycles);
    closeFile();
    return 0;
}
