#include "pipeline.h"

void IFprocess()
{
    if(IF_ID.stall != true)
    {
        IF_ID.PC = PC;
        IF_ID.instruction = IF.inst;
        if(ID_EX.flush != true) PC = PC + 4;
        else PC = branchAddress;
    }
}

void IDprocess()
{
    if(ID_EX.stall)
    {
		    ID_EX.REG_rs = REG[ID_EX.rs];
		    ID_EX.REG_rt = REG[ID_EX.rt];
	  }else if(IFID.flush)
}
void EXprocess()
{

}
void DMprocess()
{

}
void WBprocess()
{

}
