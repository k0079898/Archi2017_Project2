#ifndef instruction_h
#define instruction_h

#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "pipereg.h"

void getInstruction();     //To get the PC instrction
void decodeInstruction();  //Decode the instrction

#endif
