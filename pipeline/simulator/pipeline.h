#ifndef pipeline_h
#define pipeline_h

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "instruction.h"
#include "piperpt.h"
#include "memory.h"
#include "regfile.h"

//From project 1
unsigned int jumpAddress;
int need_mfHILO;  //For the HI & LO overwrite detection

void NumberOverflowDetection(int in1, int in2, int out);  //Detect number overflow
void MemAddOverflowDetection(int addr, int size);         //Detect memory address overflow
void DataMisalignedDetection(int words, int size);        //Detect data misaligned

//New things
typedef struct _forwardingDetect {  //Use to detect the forwarding
    int isRSinEXDM;
    int isRSinDMWB;
    int isRTinEXDM;
    int isRTinDMWB;
    int canFWD_EXDM;
    int canFWD_DMWB;
} fwdDetect;

int halt, stall, flush;  //PC state
fwdDetect fwdD;          //Use to detect the forwarding

void IFprocess();
void IDprocess();
void EXprocess();
void DMprocess();
void WBprocess();

#endif
