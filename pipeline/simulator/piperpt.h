#ifndef piperpt_h
#define piperpt_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regfile.h"
#include "pipereg.h"
#include "pipeline.h"

//Same as project 1
typedef struct _error {
    int writeToRegZero;
    int memAddOverflow;
    int dataMisaligned;
    int overwriteHILO;
    int numberOverflow;
} error;

FILE *snapshot, *error_dump;
unsigned int L_REG[REG_SIZE], L_PC, L_HI, L_LO;  //Save the last changed value
error errorDetect;

void initOutputSetting();                     //
void writeSnapshotREG(unsigned int cycles);   //
void writeSnapshotPipe(unsigned int cycles);  //
void writeError(unsigned int cycles);         //
void writeZeroError(unsigned int cycles);     //
void closeFile();                             //

#endif
