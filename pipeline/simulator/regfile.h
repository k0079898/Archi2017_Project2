#ifndef regfile_h
#define regfile_h

#include <stdio.h>
#include "memory.h"

#define REG_SIZE 32

unsigned int initPC;  //Save the initial PC address

//Register
unsigned int REG[REG_SIZE], PC, HI, LO;

void initREG();  //Initial all the register

#endif
