#ifndef pipereg_h
#define pipereg_h

#include <stdio.h>
#include "instruction.h"
#include "regfile.h"

typedef struct forwarding {
	int forward;
	int rs;  //0: no forward, 1: forward form EX/DM, 2:forward form DM/WB
	int rt;  //0: no forward, 1: forward form EX/DM, 2:forward form DM/WB
} forwarding;

typedef struct IFtoID {
    unsigned int PC;
    unsigned int instruction;
    unsigned int opcode;
    char instructionType;
    unsigned int rs;
    unsigned int rt;
    unsigned int rd;
    unsigned int C;
    unsigned int funct;
    char instName[5];
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
    unsigned int funct;
    char instName[5];
    int NOP;
    forwarding fwd;
} IDtoEX;

typedef struct EXtoDM {
    unsigned int PC;
    unsigned int instruction;
    unsigned int opcode;
    char instructionType;
    unsigned int rs;
    unsigned int rt;
    unsigned int REG_rt;
    unsigned int rd;
    unsigned int funct;
    char instName[5];
    unsigned int ALUresult;
    int NOP;
    forwarding prev_fwd;
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
    unsigned int funct;
    unsigned int memData;
    unsigned int ALUresult;
    int NOP;
} DMtoWB;

typedef struct prevDMtoWB {
    unsigned int PC;
    unsigned int instruction;
    char instructionType;
    unsigned int opcode;
    char instName[5];
    unsigned int memData;
    unsigned int ALUresult;
    int NOP;
} prevDMtoWB;

IFtoID      IF_ID;
IDtoEX      ID_EX;
EXtoDM      EX_DM;
DMtoWB      DM_WB;
prevDMtoWB  prevDM_WB;

void initPipeReg();

#endif
