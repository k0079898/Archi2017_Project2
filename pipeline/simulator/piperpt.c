#include "piperpt.h"

void initOutputSetting()
{
  snapshot = fopen("snapshot.rpt", "w");
	error_dump = fopen("error_dump.rpt", "w");
}

void writeSnapshotREG(unsigned int cycles)
{
    if(cycles==0)
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
    }else
    {
        fprintf(snapshot, "cycle %d\n", cycles);
        printf("cycle %d\n", cycles);
        for(int i=0; i<REG_SIZE; i++)
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
    }
}

void writeSnapshotPipe(unsigned int cycles)
{
  fprintf(snapshot, "PC: 0x%08X\n", PC);
  printf("PC: 0x%08X\n", PC);
  fprintf(snapshot, "IF: 0x%08X", IF.inst);
  printf("IF: 0x%08X", IF.inst);
  if(stall==1)
  {
      fprintf(snapshot, " to_be_stalled");
      printf(" to_be_stalled");
  }else if(flush==1)
  {
      fprintf(snapshot, " to_be_flushed");
      printf(" to_be_flushed");
  }

  fprintf(snapshot, "\nID: %s", ID_EX.instName);
  printf("\nID: %s", ID_EX.instName);
  if(stall==1)
  {
      fprintf(snapshot, " to_be_stalled");
      printf(" to_be_stalled");
  }
  if(ID_EX.fwd.forward==1)
  {
      if(ID_EX.fwd.rs==1)
      {
          fprintf(snapshot, " fwd_EX-DM_rs_$%d", ID_EX.rs);
          printf(" fwd_EX-DM_rs_$%d", ID_EX.rs);
      }

      if(ID_EX.fwd.rt==1)
      {
          fprintf(snapshot, " fwd_EX-DM_rt_$%d", ID_EX.rt);
          printf(" fwd_EX-DM_rt_$%d", ID_EX.rt);
      }
  }
  fprintf(snapshot, "\nEX: %s", EX_DM.instName);
  printf("\nEX: %s", EX_DM.instName);
  if(EX_DM.prev_fwd.forward==1)
  {
      if(EX_DM.prev_fwd.rs==1)
      {
          fprintf(snapshot, " fwd_EX-DM_rs_$%d", EX_DM.rs);
          printf(" fwd_EX-DM_rs_$%d", EX_DM.rs);
      }if(EX_DM.prev_fwd.rs==2)
      {
          fprintf(snapshot, " fwd_DM-WB_rs_$%d", EX_DM.rs);
          printf(" fwd_DM-WB_rs_$%d", EX_DM.rs);
      }

      if(EX_DM.prev_fwd.rt==1)
      {
          fprintf(snapshot, " fwd_EX-DM_rt_$%d", EX_DM.rt);
          printf(" fwd_EX-DM_rt_$%d", EX_DM.rt);
      }if(EX_DM.prev_fwd.rt==2)
      {
          fprintf(snapshot, " fwd_DM-WB_rt_$%d", EX_DM.rt);
          printf(" fwd_DM-WB_rt_$%d", EX_DM.rt);
      }
  }
  fprintf(snapshot, "\nDM: %s\n", DM_WB.instName);
  printf("\nDM: %s\n", DM_WB.instName);
  fprintf(snapshot, "WB: %s\n\n\n", prevDM_WB.instName);
  printf("WB: %s\n\n\n", prevDM_WB.instName);
}


void writeError(unsigned int cycles)
{
  if(overwriteHILO==1)
  {
    fprintf(error_dump, "In cycle %d: Overwrite HI-LO registers\n", cycles);
    printf("In cycle %d: Overwrite HI-LO registers\n", cycles);
    overwriteHILO = 0;
  }
  if(memAddOverflow==1)
	{
		fprintf(error_dump, "In cycle %d: Address Overflow\n", cycles);
		printf("In cycle %d: Address Overflow\n", cycles);
    memAddOverflow = 0;
	}
	if(dataMisaligned==1)
	{
		fprintf(error_dump, "In cycle %d: Misalignment Error\n", cycles);
		printf("In cycle %d: Misalignment Error\n", cycles);
    dataMisaligned = 0;
	}
  if(numberOverflow==1)
	{
		fprintf(error_dump, "In cycle %d: Number Overflow\n", cycles);
		printf("In cycle %d: Number Overflow\n", cycles);
    numberOverflow = 0;
	}
}

void writeZeroError(unsigned int cycles)
{
    if(writeToRegZero==1)
    {
        fprintf(error_dump, "In cycle %d: Write $0 Error\n", cycles);
        printf("In cycle %d: Write $0 Error\n", cycles);
        writeToRegZero = 0;
    }
}

void closeFile()
{
    fclose(snapshot);
  	fclose(error_dump);
}
