#include "pipereg.h"

void initPipeReg()
{
    getIF();
    memset(&IF_ID, 0, sizeof(IFtoID));
    memset(&ID_EX, 0, sizeof(IDtoEX));
    memset(&EX_DM, 0, sizeof(EXtoDM));
    memset(&DM_WB, 0, sizeof(DMtoWB));
    memset(&prevDM_WB, 0, sizeof(prevDMtoWB));
    IF_ID.PC = PC;
    IF_ID.instruction = IF.inst;
    ID_EX.NOP = 1;
    strcpy(ID_EX.instName, "NOP");
    EX_DM.NOP = 1;
    strcpy(EX_DM.instName, "NOP");
    DM_WB.NOP = 1;
    strcpy(DM_WB.instName, "NOP");
    prevDM_WB.NOP = 1;
    strcpy(prevDM_WB.instName, "NOP");
}
