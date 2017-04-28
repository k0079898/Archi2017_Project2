#include "pipeline.h"

void IFprocess()
{
    if(ID_EX.stall != 1)
    {
        IF_ID.PC = PC;
        IF_ID.instruction = IF.inst;
        IF_ID.flush = ID_EX.flushIF;
        if(IF_ID.flush != 1) PC = PC + 4;
        else PC = jumpAddress;
    }else IF_ID.stall = ID_EX.stall;
}

void IDprocess()
{
    if(ID_EX.stall == 1)
    {
		    ID_EX.REG_rs = REG[ID_EX.rs];
		    ID_EX.REG_rt = REG[ID_EX.rt];
	  }else if(IF_ID.flush) {
        memset(&ID_EX, 0, sizeof(IDtoEX));
        ID_EX.NOP = 1;
        strcpy(ID_EX.instName, "NOP");
    }else {
        ID_EX.PC = IF_ID.PC;
        ID_EX.instruction = IF_ID.instruction;
        ID_EX.opcode = IF.opcode;
        ID_EX.instructionType = IF.type;
        if( ID_EX.instructionType == 'R' )
        {
            ID_EX.rs     = IF.rs;
            ID_EX.rt     = IF.rt;
            ID_EX.rd     = IF.rd;
            ID_EX.C      = IF.C;
            ID_EX.funct  = IF.funct;
            ID_EX.REG_rs = REG[ID_EX.rs];
    		    ID_EX.REG_rt = REG[ID_EX.rt];
            strcpy(ID_EX.instName, IF.name);
        }else if( ID_EX.instructionType == 'I' )
        {
            ID_EX.rs     = IF.rs;
            ID_EX.rt     = IF.rt;
            ID_EX.C      = (short int)IF.C;
            ID_EX.REG_rs = REG[ID_EX.rs];
    		    ID_EX.REG_rt = REG[ID_EX.rt];
            strcpy(ID_EX.instName, IF.name);
        }else if( ID_EX.instructionType == 'J' )
        {
            ID_EX.C = IF.C;
            strcpy(ID_EX.instName, IF.name);
        }
        if( strcmp(ID_EX.instName, "NOP")==0 ) ID_EX.NOP = 1;
    }

}
void EXprocess()
{
  if(ID_EX.stall == 1)
  {
      memset(&EX_DM, 0, sizeof(EXtoDM));
      EX_DM.NOP = 1;
      strcpy(ID_EX.instName, "NOP");
      return;
  }
}
void DMprocess()
{
    if( EX_DM.NOP != 1 )
    {
        unsigned int addr = EX_DM.ALUresult;
        if( EX_DM.instructionType == 'I' )  //detect error
        {
            if( EX_DM.opcode==0x23 || EX_DM.opcode==0x2B )  //lw & sw
            {
                MemAddOverflowDetection(addr, 3);
                DataMisalignedDetection(addr, 4);
            }else if( EX_DM.opcode==0x21 || EX_DM.opcode==0x25 ||  EX_DM.opcode==0x29 )  //lh & lhu & sh
            {
                MemAddOverflowDetection(addr, 1);
                DataMisalignedDetection(addr, 2);
            }else if( EX_DM.opcode==0x20 || EX_DM.opcode==0x24 ||  EX_DM.opcode==0x28 )  //lb & lbu & sb
            {
                MemAddOverflowDetection(addr, 0);
            }
        }
        if(halt != 1)
        {
            if( EX_DM.opcode==0x23 )  //lw
            {
                DM_WB.memData = (int32_t)(dMem[addr] << 24 | dMem[addr+1] << 16 | dMem[addr+2] << 8 | dMem[addr+3]);
            }else if( EX_DM.opcode==0x21 )  //lh
            {
                DM_WB.memData = (int16_t)(dMem[addr] << 8 | dMem[addr+1]);
            }else if( EX_DM.opcode==0x25 )  //lhu
            {
                DM_WB.memData = (uint16_t)(dMem[addr] << 8 | dMem[addr+1]);
            }else if( EX_DM.opcode==0x20 )  //lb
            {
                DM_WB.memData = (int8_t)dMem[addr];
            }else if( EX_DM.opcode==0x24 )  //lbu
            {
                DM_WB.memData = (uint8_t)dMem[addr];
            }else if( EX_DM.opcode==0x2B )  //sw
            {
                dMem[addr]   = EX_DM.REG_rt >> 24;
                dMem[addr+1] = EX_DM.REG_rt >> 16;
                dMem[addr+2] = EX_DM.REG_rt >> 8;
                dMem[addr+3] = EX_DM.REG_rt;
            }else if( EX_DM.opcode==0x29 )  //sh
            {
                dMem[addr]   = (EX_DM.REG_rt >> 8) & 0xFF;
                dMem[addr+1] = EX_DM.REG_rt & 0xFF;
            }else if( EX_DM.opcode==0x28 )  //sb
            {
                dMem[addr] = EX_DM.REG_rt & 0xFF;
            }
        }
    }
    DM_WB.PC = EX_DM.PC;
    DM_WB.instruction = EX_DM.instruction;
    DM_WB.opcode = EX_DM.opcode;
    DM_WB.instructionType = EX_DM.instructionType;
    strcpy(DM_WB.instName, EX_DM.instName);
    DM_WB.ALUresult = EX_DM.ALUresult;
    DM_WB.NOP = EX_DM.NOP;
}
void WBprocess()
{
    if( DM_WB.NOP != 1 )
    {
        if( DM_WB.instructionType == 'R' && strcmp(DM_WB.instName, "JR")!=0 )
        {
            if(DM_WB.rd == 0) writeToRegZero = 1;
            else REG[DM_WB.rd] = DM_WB.ALUresult;
        }else if( DM_WB.instructionType == 'I' )
        {
            if( DM_WB.opcode==0x23 || DM_WB.opcode==0x21 || DM_WB.opcode==0x25 || DM_WB.opcode==0x20 || DM_WB.opcode==0x24 )
            {
                if(DM_WB.rd == 0) writeToRegZero = 1;
                else REG[DM_WB.rd] = DM_WB.memData;
            }else if( DM_WB.opcode==0x08 || DM_WB.opcode==0x09 || DM_WB.opcode==0x0F || DM_WB.opcode==0x0C || DM_WB.opcode==0x0D || DM_WB.opcode==0x0E || DM_WB.opcode==0x0A )
            {
                if(DM_WB.rd == 0) writeToRegZero = 1;
                else REG[DM_WB.rd] = DM_WB.ALUresult;
            }
        }else if( DM_WB.instructionType == 'J' && DM_WB.opcode==0x03 )
        {
            REG[31] = DM_WB.PC + 4;
        }
    }
    prevDM_WB.PC = DM_WB.PC;
    prevDM_WB.instruction = DM_WB.instruction;
    prevDM_WB.opcode = DM_WB.opcode;
    strcpy(prevDM_WB.instName, DM_WB.instName);
    prevDM_WB.NOP = DM_WB.NOP;
}

void NumberOverflowDetection(int in1, int in2, int out)
{
    if((in1>=0 && in2>=0 && out<0) || (in1<0 && in2<0 && out>=0)) numberOverflow = 1;
}

void MemAddOverflowDetection(int addr, int size)
{
    if(addr < 0 || addr >= MEM_SIZE || addr+size >= MEM_SIZE)
    {
        memAddOverflow = 1;
        halt = 1;
    }
}

void DataMisalignedDetection(int words, int size)
{
    if(words%size!=0)
    {
        dataMisaligned = 1;
        halt = 1;
      }
}
