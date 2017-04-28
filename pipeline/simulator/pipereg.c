#include "pipereg.h"

void initPipeReg()
{
    instructionIF();
    IF_ID.PC = PC;
    IF_ID.instruction = IF.inst;
    IF_ID.stall = false;
    IF_ID.flush = false;
}
