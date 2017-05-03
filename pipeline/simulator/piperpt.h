#ifndef piperpt_h
#define piperpt_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regfile.h"
#include "pipereg.h"
#include "pipeline.h"

typedef struct _error {
    int writeToRegZero;  //Write to register $0
    int memAddOverflow;  //D-Memory address overflow
    int dataMisaligned;  //D-Memory miss align error
    int overwriteHILO;   //Overwrite HI-LO registers
    int numberOverflow;  //Number overflow
} error;

FILE *snapshot, *error_dump;                     //File pointer
unsigned int L_REG[REG_SIZE], L_PC, L_HI, L_LO;  //Save the last changed value
error errorDetect;                               //To save the error detection

void initOutputSetting();                     //Open file
void writeSnapshotREG(unsigned int cycles);   //Write the REG part
void writeSnapshotPipe(unsigned int cycles);  //Write the Pipeline part
void writeError(unsigned int cycles);         //Write the ERROR
void closeFile();                             //Close file

#endif
