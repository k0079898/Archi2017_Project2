#include "piperpt.h"

void initOutputSetting()
{
  snapshot = fopen("snapshot.rpt", "w");
	error_dump = fopen("error_dump.rpt", "w");
  for(int i=0; i<REG_SIZE; i++) L_REG[i] = 0;
  L_PC = 0;
  L_HI = 0;
  L_LO = 0;
}

void writeInitSnapshot()
{
  fprintf(snapshot, "cycle 0\n");
  printf("cycle 0\n");
  for(int i=0; i<REG_SIZE; i++)
  {
    fprintf(snapshot, "$%02d: ", i);
    fprintf(snapshot, "0x%08X\n", REG[i]);
    printf("$%02d: ", i);
    printf("0x%08X\n", REG[i]);
    L_REG[i] = REG[i];
  }
  fprintf(snapshot, "$HI: 0x%08X\n", HI);
  printf("$HI: 0x%08X\n", HI);
  L_HI = HI;
  fprintf(snapshot, "$LO: 0x%08X\n", LO);
  printf("$LO: 0x%08X\n", LO);
  L_LO = LO;
  fprintf(snapshot, "PC: 0x%08X\n", PC);
  printf("PC: 0x%08X\n", PC);
  L_PC = PC;
  fprintf(snapshot, "IF: 0x%08X\n", IF);
  printf("IF: 0x%08X\n", IF);
  fprintf(snapshot, "ID: NOP\n");
  printf("ID: NOP\n");
  fprintf(snapshot, "EX: NOP\n");
  printf("EX: NOP\n");
  fprintf(snapshot, "DM: NOP\n");
  printf("DM: NOP\n");
  fprintf(snapshot, "WB: NOP\n\n\n");
  printf("WB: NOP\n\n\n");
}

void writeSnapshot(unsigned int cycles)
{
  int i;
  fprintf(snapshot, "cycle %d\n", cycles);
  printf("cycle %d\n", cycles);
  if(cycles==0)  //Initial Print
  {
    for(i=0; i<REG_SIZE; i++)
    {
      fprintf(snapshot, "$%02d: ", i);
      fprintf(snapshot, "0x%08X\n", REG[i]);
      printf("$%02d: ", i);
      printf("0x%08X\n", REG[i]);
    }
    fprintf(snapshot, "$HI: 0x%08X\n", HI);
    printf("$HI: 0x%08X\n", HI);
    fprintf(snapshot, "$LO: 0x%08X\n", LO);
    printf("$LO: 0x%08X\n", LO);
    fprintf(snapshot, "PC: 0x%08X\n\n\n", PC);
    printf("PC: 0x%08X\n\n\n", PC);
    //print initial IF ID EX DM WB here
  }else //Simulaing Print
  {
    for(i=0; i<REG_SIZE; i++)
    {
      if(L_REG[i] != REG[i])
      {
        fprintf(snapshot, "$%02d: ", i);
        fprintf(snapshot, "0x%08X\n", REG[i]);
        printf("$%02d: ", i);
        printf("0x%08X\n", REG[i]);
        L_REG[i] = REG[i];
      }
    }
    if(L_HI != HI)
    {
      fprintf(snapshot, "$HI: 0x%08X\n", HI);
      printf("$HI: 0x%08X\n", HI);
      L_HI = HI;
    }
    if(L_LO != LO)
    {
      fprintf(snapshot, "$LO: 0x%08X\n", LO);
      printf("$LO: 0x%08X\n", LO);
      L_LO = LO;
    }
    if(L_PC != PC)
    {
      fprintf(snapshot, "PC: 0x%08X\n\n\n", PC);
      printf("PC: 0x%08X\n\n\n", PC);
      L_PC = PC;
    }
    //print initial IF ID EX DM WB here
  }
}


void writeError(unsigned int cycles)
{
	if(writeToRegZero==1)
	{
		fprintf(error_dump, "In cycle %d: Write $0 Error\n", cycles);
		printf("In cycle %d: Write $0 Error\n", cycles);
	}
  if(memAddOverflow==1)
	{
		fprintf(error_dump, "In cycle %d: Address Overflow\n", cycles);
		printf("In cycle %d: Address Overflow\n", cycles);
	}
	if(dataMisaligned==1)
	{
		fprintf(error_dump, "In cycle %d: Misalignment Error\n", cycles);
		printf("In cycle %d: Misalignment Error\n", cycles);
	}
	if(overwriteHILO==1)
	{
		fprintf(error_dump, "In cycle %d: Overwrite HI-LO registers\n", cycles);
		printf("In cycle %d: Overwrite HI-LO registers\n", cycles);
	}
  if(numberOverflow==1)
	{
		fprintf(error_dump, "In cycle %d: Number Overflow\n", cycles);
		printf("In cycle %d: Number Overflow\n", cycles);
	}
}

void closeFile()
{
  fclose(snapshot);
	fclose(error_dump);
}
