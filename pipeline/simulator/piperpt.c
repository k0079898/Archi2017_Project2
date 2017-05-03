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
        //printf("cycle 0\n");
        for(int i=0; i<REG_SIZE; i++)
        {
          fprintf(snapshot, "$%02d: ", i);
          fprintf(snapshot, "0x%08X\n", REG[i]);
          //printf("$%02d: ", i);
          //printf("0x%08X\n", REG[i]);
          L_REG[i] = REG[i];
        }
        fprintf(snapshot, "$HI: 0x%08X\n", HI);
        //printf("$HI: 0x%08X\n", HI);
        L_HI = HI;
        fprintf(snapshot, "$LO: 0x%08X\n", LO);
        //printf("$LO: 0x%08X\n", LO);
        L_LO = LO;
    }else
    {
        fprintf(snapshot, "cycle %d\n", cycles);
        //printf("cycle %d\n", cycles);
        for(int i=0; i<REG_SIZE; i++)
        {
            if(L_REG[i] != REG[i])
            {
                fprintf(snapshot, "$%02d: ", i);
                fprintf(snapshot, "0x%08X\n", REG[i]);
                //printf("$%02d: ", i);
                //printf("0x%08X\n", REG[i]);
                L_REG[i] = REG[i];
            }
        }
        if(L_HI != HI)
        {
            fprintf(snapshot, "$HI: 0x%08X\n", HI);
            //printf("$HI: 0x%08X\n", HI);
            L_HI = HI;
        }
        if(L_LO != LO)
        {
            fprintf(snapshot, "$LO: 0x%08X\n", LO);
            //printf("$LO: 0x%08X\n", LO);
            L_LO = LO;
        }
    }
}

void writeSnapshotPipe(unsigned int cycles)
{
  fprintf(snapshot, "PC: 0x%08X\n", PC);
  //printf("PC: 0x%08X\n", PC);
  fprintf(snapshot, "IF: 0x%08X", IF_ID.inst.inst);
  //printf("IF: 0x%08X", IF_ID.inst.inst);
  if(stall==1)
  {
      fprintf(snapshot, " to_be_stalled");
      //printf(" to_be_stalled");
  }else if(flush==1)
  {
      fprintf(snapshot, " to_be_flushed");
      //printf(" to_be_flushed");
  }

  fprintf(snapshot, "\nID: %s", ID_EX.inst.name);
  //printf("\nID: %s", ID_EX.inst.name);
  if(stall==1)
  {
      fprintf(snapshot, " to_be_stalled");
      //printf(" to_be_stalled");
  }
  if(ID_EX.fwd.forward==1)
  {
      if(ID_EX.fwd.rs==1)
      {
          fprintf(snapshot, " fwd_EX-DM_rs_$%d", ID_EX.inst.rs);
          //printf(" fwd_EX-DM_rs_$%d", ID_EX.inst.rs);
      }

      if(ID_EX.fwd.rt==1)
      {
          fprintf(snapshot, " fwd_EX-DM_rt_$%d", ID_EX.inst.rt);
          //printf(" fwd_EX-DM_rt_$%d", ID_EX.inst.rt);
      }
  }
  fprintf(snapshot, "\nEX: %s", EX_DM.inst.name);
  //printf("\nEX: %s", EX_DM.inst.name);
  if(EX_DM.prev_fwd.forward==1)
  {
      if(EX_DM.prev_fwd.rs==1)
      {
          fprintf(snapshot, " fwd_EX-DM_rs_$%d", EX_DM.inst.rs);
          //printf(" fwd_EX-DM_rs_$%d", EX_DM.inst.rs);
      }if(EX_DM.prev_fwd.rs==2)
      {
          fprintf(snapshot, " fwd_DM-WB_rs_$%d", EX_DM.inst.rs);
          //printf(" fwd_DM-WB_rs_$%d", EX_DM.inst.rs);
      }

      if(EX_DM.prev_fwd.rt==1)
      {
          fprintf(snapshot, " fwd_EX-DM_rt_$%d", EX_DM.inst.rt);
          //printf(" fwd_EX-DM_rt_$%d", EX_DM.inst.rt);
      }if(EX_DM.prev_fwd.rt==2)
      {
          fprintf(snapshot, " fwd_DM-WB_rt_$%d", EX_DM.inst.rt);
          //printf(" fwd_DM-WB_rt_$%d", EX_DM.inst.rt);
      }
  }
  fprintf(snapshot, "\nDM: %s\n", DM_WB.inst.name);
  //printf("\nDM: %s\n", DM_WB.inst.name);
  fprintf(snapshot, "WB: %s\n\n\n", prevDM_WB.inst.name);
  //printf("WB: %s\n\n\n", prevDM_WB.inst.name);
}


void writeError(unsigned int cycles)
{
  if(errorDetect.writeToRegZero==1)
  {
      fprintf(error_dump, "In cycle %d: Write $0 Error\n", cycles);
      //printf("In cycle %d: Write $0 Error\n", cycles);
      errorDetect.writeToRegZero = 0;
  }
  if(errorDetect.memAddOverflow==1)
	{
		fprintf(error_dump, "In cycle %d: Address Overflow\n", cycles);
		//printf("In cycle %d: Address Overflow\n", cycles);
    errorDetect.memAddOverflow = 0;
	}
	if(errorDetect.dataMisaligned==1)
	{
		fprintf(error_dump, "In cycle %d: Misalignment Error\n", cycles);
		//printf("In cycle %d: Misalignment Error\n", cycles);
    errorDetect.dataMisaligned = 0;
	}
  if(errorDetect.overwriteHILO==1)
  {
    fprintf(error_dump, "In cycle %d: Overwrite HI-LO registers\n", cycles);
    //printf("In cycle %d: Overwrite HI-LO registers\n", cycles);
    errorDetect.overwriteHILO = 0;
  }
  if(errorDetect.numberOverflow==1)
	{
		fprintf(error_dump, "In cycle %d: Number Overflow\n", cycles);
		//printf("In cycle %d: Number Overflow\n", cycles);
    errorDetect.numberOverflow = 0;
	}
}

void closeFile()
{
    fclose(snapshot);
  	fclose(error_dump);
}
