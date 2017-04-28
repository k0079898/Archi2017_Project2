#ifndef pipeline_h
#define pipeline_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "memory.h"
#include "regfile.h"

unsigned int branchAddress;
bool halt;

void IFprocess();
void IDprocess();
void EXprocess();
void DMprocess();
void WBprocess();

#endif
