#include "pipereg.h"

void initPipeReg()
{
    memset(&IF_ID, 0, sizeof(IFtoID));
    memset(&ID_EX, 0, sizeof(IDtoEX));
    memset(&EX_DM, 0, sizeof(EXtoDM));
    memset(&DM_WB, 0, sizeof(DMtoWB));
    memset(&prevDM_WB, 0, sizeof(prevDMtoWB));
    strcpy(IF_ID.inst.name, "NOP");
    strcpy(ID_EX.inst.name, "NOP");
    strcpy(EX_DM.inst.name, "NOP");
    strcpy(DM_WB.inst.name, "NOP");
    strcpy(prevDM_WB.inst.name, "NOP");
}
