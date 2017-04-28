#ifndef pipereg_h
#define pipereg_h

#include <stdio.h>
#include <stdbool.h>
#include "instruction.h"
#include "memory.h"
#include "regfile.h"

typedef struct forwarding {
	bool forward;
	bool rs;
	bool rt;
} forwarding;

typedef struct IFtoID {
    unsigned int PC;
    unsigned int instruction;
    bool stall;
    bool flush;
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
    char name[5];
    bool stall;
    bool NOP;
    forwarding fwd;
} IDtoEX;

typedef struct EXtoDM {
    unsigned int PC;
    unsigned int instruction;
    unsigned int opcode;
    char instructionType;
    unsigned ALUresult;
    unsigned int rd;
    unsigned int rt;
    char name[5];
    bool NOP;
    forwarding fwd;
} EXtoDM;

typedef struct DMtoWB {
    unsigned int PC;
    unsigned int instruction;
    unsigned int opcode;
    char instructionType;
    char name[5];
    unsigned int data;
    unsigned ALUresult;
} DMtoWB;

IFtoID IF_ID;
IDtoEX ID_EX;
EXtoDM EX_DM;
DMtoWB DM_WB;

void initPipeReg();

#endif
