#ifndef pipereg_h
#define pipereg_h

#include <stdio.h>
#include "instruction.h"
#include "regfile.h"

typedef struct forwarding {
	int forward;
	int rs;
	int rt;
} forwarding;

typedef struct IFtoID {
    unsigned int PC;
    unsigned int instruction;
    int stall;
    int flush;
} IFtoID;

typedef struct IDtoEX {
    unsigned int PC;
    unsigned int instruction;
    unsigned int opcode;
    char instructionType;
    unsigned int rs;
    unsigned int REG_rs;
    unsigned int rt;
    unsigned int REG_rt;
    unsigned int rd;
    unsigned int C;
    unsigned funct;
    char instName[5];
    int stall;
    int flushIF;
    int NOP;
    forwarding fwd;
} IDtoEX;

typedef struct EXtoDM {
    unsigned int PC;
    unsigned int instruction;
    unsigned int opcode;
    char instructionType;
    unsigned int rd;
    unsigned int rt;
    unsigned int REG_rt;
    char instName[5];
    unsigned int ALUresult;
    int NOP;
    forwarding fwd;
} EXtoDM;

typedef struct DMtoWB {
    unsigned int PC;
    unsigned int instruction;
    unsigned int opcode;
    char instructionType;
    char instName[5];
    unsigned int rd;
    unsigned int rt;
    unsigned int memData;
    unsigned int ALUresult;
    int NOP;
} DMtoWB;

typedef struct prevDMtoWB {
    unsigned int PC;
    unsigned int instruction;
    unsigned int opcode;
    char instName[5];
    int NOP;
} prevDMtoWB;

IFtoID      IF_ID;
IDtoEX      ID_EX;
EXtoDM      EX_DM;
DMtoWB      DM_WB;
prevDMtoWB  prevDM_WB;

void initPipeReg();

#endif
