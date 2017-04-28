#ifndef pipeline_h
#define pipeline_h

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "piperpt.h"
#include "memory.h"
#include "regfile.h"

unsigned int jumpAddress;
int halt;

void IFprocess();
void IDprocess();
void EXprocess();
void DMprocess();
void WBprocess();
void NumberOverflowDetection(int in1, int in2, int out);  //Detect number overflow
void MemAddOverflowDetection(int addr, int size);         //Detect memory address overflow
void DataMisalignedDetection(int words, int size);        //Detect data misaligned

#endif
