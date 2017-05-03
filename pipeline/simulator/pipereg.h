#ifndef pipereg_h
#define pipereg_h

#include <stdio.h>
#include <string.h>
#include "instruction.h"

typedef struct _instruction {
  unsigned int inst;
  unsigned int opcode;
  char type;
  unsigned int rs;
  unsigned int rt;
  unsigned int rd;
  unsigned int C;
  unsigned int funct;
  char name[5];
} instruct;

typedef struct _forwarding {
	int forward;
	int rs;  //0: no forward, 1: forward form EX/DM, 2:forward form DM/WB
	int rt;  //0: no forward, 1: forward form EX/DM, 2:forward form DM/WB
} forwarding;

typedef struct _IFtoID {
    unsigned int PC;
		instruct inst;
    int stall;
} IFtoID;

typedef struct _IDtoEX {
    unsigned int PC;
    instruct inst;
		unsigned int REG_rs;
    unsigned int REG_rt;
    forwarding fwd;
} IDtoEX;

typedef struct _EXtoDM {
    unsigned int PC;
    instruct inst;
		unsigned int REG_rt;
    unsigned int ALUresult;
    forwarding prev_fwd;
    forwarding fwd;
} EXtoDM;

typedef struct _DMtoWB {
    unsigned int PC;
    instruct inst;
    unsigned int memData;
    unsigned int ALUresult;
} DMtoWB;

typedef struct prevDMtoWB {
    instruct inst;
    unsigned int result;
} prevDMtoWB;

IFtoID      IF_ID;      //IF/ID pipeline
IDtoEX      ID_EX;      //ID/EX pipeline
EXtoDM      EX_DM;      //EX/EM pipeline
DMtoWB      DM_WB;      //DM/WB pipeline
prevDMtoWB  prevDM_WB;  //The previous DM/WB pipeline

void initPipeReg();     //Initial the pipeline to "NOP"

#endif
