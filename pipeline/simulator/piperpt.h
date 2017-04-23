#ifndef piperpt_h
#define piperpt_h

#include <stdio.h>
#include <stdlib.h>
#include "regfile.h"

FILE *snapshot, *error_dump;

unsigned int L_REG[REG_SIZE], L_PC, L_HI, L_LO;  //Save the last changed value
int writeToRegZero, memAddOverflow, dataMisaligned, overwriteHILO, numberOverflow;

void initOutputSetting();                 //
void writeInitSnapshot();                 //
void writeSnapshot(unsigned int cycles);  //
void writeError(unsigned int cycles);     //
void closeFile();                         //

#endif
