#ifndef piperpt_h
#define piperpt_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instruction.h"
#include "regfile.h"
#include "pipereg.h"
#include "pipeline.h"

FILE *snapshot, *error_dump;

unsigned int L_REG[REG_SIZE], L_PC, L_HI, L_LO;  //Save the last changed value
int writeToRegZero, memAddOverflow, dataMisaligned, overwriteHILO, numberOverflow;

void initOutputSetting();                     //
void writeSnapshotREG(unsigned int cycles);   //
void writeSnapshotPipe(unsigned int cycles);  //
void writeError(unsigned int cycles);         //
void writeZeroError(unsigned int cycles);     //
void closeFile();                             //

#endif
