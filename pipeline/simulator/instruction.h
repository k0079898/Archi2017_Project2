#ifndef instruction_h
#define instruction_h

#include <stdio.h>
#include <string.h>
#include "memory.h"

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
} instIF;

instIF IF;

void getIF();
void decodeIF();

#endif
