#include "pipeline.h"

void NumberOverflowDetection(int in1, int in2, int out)
{
    if((in1>=0 && in2>=0 && out<0) || (in1<0 && in2<0 && out>=0)) errorDetect.numberOverflow = 1;
}

void MemAddOverflowDetection(int addr, int size)
{
    if(addr < 0 || addr >= MEM_SIZE || addr+size >= MEM_SIZE)
    {
        errorDetect.memAddOverflow = 1;
        halt = 1;
    }
}

void DataMisalignedDetection(int words, int size)
{
    if(words%size!=0)
    {
        errorDetect.dataMisaligned = 1;
        halt = 1;
      }
}

void IFprocess()
{
        if(IF_ID.stall!=1)
        {
            IF_ID.PC = PC;
            getInstruction();
        }
}

void IDprocess()
{
    if(stall == 1)
    {
		    ID_EX.REG_rs = REG[ID_EX.inst.rs];
		    ID_EX.REG_rt = REG[ID_EX.inst.rt];
	  }else if(flush) {
        memset(&ID_EX, 0, sizeof(IDtoEX));
        strcpy(ID_EX.inst.name, "NOP");
        flush = 0;
        ID_EX.fwd.forward = 0;
        EX_DM.fwd.forward = 0;
        return;
    }else {
        ID_EX.PC = IF_ID.PC;
        ID_EX.inst = IF_ID.inst;
        if( ID_EX.inst.type == 'R' )
        {
            ID_EX.REG_rs = REG[ID_EX.inst.rs];
            ID_EX.REG_rt = REG[ID_EX.inst.rt];
        }else if( ID_EX.inst.type == 'I' )
        {
            ID_EX.REG_rs = REG[ID_EX.inst.rs];
    		    ID_EX.REG_rt = REG[ID_EX.inst.rt];
        }
    }
    //handle stall detection and forwarding detection
    stall = 0;
    flush = 0;
    ID_EX.fwd.forward = 0;
    EX_DM.fwd.forward = 0;
    fwdD.isRSinEXDM = 0;
    fwdD.isRSinDMWB = 0;
    fwdD.isRTinEXDM = 0;
    fwdD.isRTinDMWB = 0;
    fwdD.canFWD_EXDM = 0;
    fwdD.canFWD_DMWB = 0;
    //printf("%s %d %d %d\n", ID_EX.inst.name, ID_EX.inst.rs, ID_EX.inst.rt, ID_EX.inst.rd);
    if( (EX_DM.inst.type=='R' && (EX_DM.inst.funct!=0x08 && EX_DM.inst.funct!=0x18 && EX_DM.inst.funct!=0x19) && ID_EX.inst.rs==EX_DM.inst.rd) || (EX_DM.inst.opcode>=0x08 && EX_DM.inst.opcode<=0x25 && ID_EX.inst.rs==EX_DM.inst.rt) )
    {
        if(ID_EX.inst.rs != 0 && ((ID_EX.inst.type=='R' && (ID_EX.inst.funct!=0x10 && ID_EX.inst.funct!=0x12)) || (ID_EX.inst.type=='I' && (ID_EX.inst.funct!=0x0F)))) fwdD.isRSinEXDM = 1;
    }
    if( (DM_WB.inst.type=='R' && (DM_WB.inst.funct!=0x08 && DM_WB.inst.funct!=0x18 && DM_WB.inst.funct!=0x19) && ID_EX.inst.rs==DM_WB.inst.rd) || (DM_WB.inst.opcode>=0x08 && DM_WB.inst.opcode<=0x25 && ID_EX.inst.rs==DM_WB.inst.rt) || (DM_WB.inst.opcode==0x03 && ID_EX.inst.rs==31) )
    {
        if(ID_EX.inst.rs != 0 && ((ID_EX.inst.type=='R' && (ID_EX.inst.funct!=0x10 && ID_EX.inst.funct!=0x12)) || (ID_EX.inst.type=='I' && (ID_EX.inst.funct!=0x0F)))) fwdD.isRSinDMWB = 1;
    }
    if( (EX_DM.inst.type=='R' && (EX_DM.inst.funct!=0x08 && EX_DM.inst.funct!=0x18 && EX_DM.inst.funct!=0x19) && ID_EX.inst.rt==EX_DM.inst.rd) || (EX_DM.inst.opcode>=0x08 && EX_DM.inst.opcode<=0x25 && ID_EX.inst.rt==EX_DM.inst.rt) )
    {
        if(ID_EX.inst.rt != 0 && ((ID_EX.inst.type=='R' && (ID_EX.inst.funct!=0x10 && ID_EX.inst.funct!=0x12)) || (ID_EX.inst.type=='I' && (ID_EX.inst.funct!=0x0F)))) fwdD.isRTinEXDM = 1;
    }
    if( (DM_WB.inst.type=='R' && (DM_WB.inst.funct!=0x08 && DM_WB.inst.funct!=0x18 && DM_WB.inst.funct!=0x19) && ID_EX.inst.rt==DM_WB.inst.rd) || (DM_WB.inst.opcode>=0x08 && DM_WB.inst.opcode<=0x25 && ID_EX.inst.rt==DM_WB.inst.rt) || (DM_WB.inst.opcode==0x03 && ID_EX.inst.rt==31) )
    {
        if(ID_EX.inst.rt != 0 && ((ID_EX.inst.type=='R' && (ID_EX.inst.funct!=0x10 && ID_EX.inst.funct!=0x12)) || (ID_EX.inst.type=='I' && (ID_EX.inst.funct!=0x0F)))) fwdD.isRTinDMWB = 1;
    }
    if( ((EX_DM.inst.type=='R' && (EX_DM.inst.funct!=0x08 || EX_DM.inst.funct!=0x18 || EX_DM.inst.funct!=0x19)) || (EX_DM.inst.opcode>=0x08 && EX_DM.inst.opcode<=0x0F)) ) fwdD.canFWD_EXDM = 1;
    if( (DM_WB.inst.type=='R' && (DM_WB.inst.funct!=0x08 || DM_WB.inst.funct!=0x18 || DM_WB.inst.funct!=0x19)) || ((DM_WB.inst.opcode>=0x08 && DM_WB.inst.opcode<=0x0F) || (DM_WB.inst.opcode>=0x20 && DM_WB.inst.opcode<=0x25)) || (DM_WB.inst.opcode==0x03)) fwdD.canFWD_DMWB = 1;
    //printf("RS: %d %d / RT: %d %d / %d %d\n", fwdD.isRSinEXDM, fwdD.isRSinDMWB, fwdD.isRTinEXDM, fwdD.isRTinDMWB, fwdD.canFWD_EXDM, fwdD.canFWD_DMWB);
    if( (ID_EX.inst.type=='R' && ID_EX.inst.funct>=0x18 && ID_EX.inst.funct<=0x2A) || (ID_EX.inst.type=='I' &&  (ID_EX.inst.opcode==0x04 || ID_EX.inst.opcode==0x05 || ID_EX.inst.opcode==0x2B || ID_EX.inst.opcode==0x28 || ID_EX.inst.opcode==0x29)) )
    {
        if( (fwdD.isRSinEXDM==1 && fwdD.isRTinEXDM==1) || (fwdD.isRSinDMWB==1 && fwdD.isRTinDMWB==1) )
        {
            if(fwdD.isRSinEXDM==1)
            {
                if(fwdD.canFWD_EXDM==1 && (ID_EX.inst.opcode!=0x04 && ID_EX.inst.opcode!=0x05))
                {
                    EX_DM.fwd.forward = 1;
                    EX_DM.fwd.rs = 1;
                    EX_DM.fwd.rt = 1;
                    stall = 0;
                }else if(ID_EX.inst.opcode==0x04 || ID_EX.inst.opcode==0x05)
                {
                    stall = 1;
                }else stall = 1;
            }else if(fwdD.isRSinDMWB==1)
            {
                if(fwdD.canFWD_DMWB==1 && (ID_EX.inst.opcode!=0x04 && ID_EX.inst.opcode!=0x05))
                {
                    EX_DM.fwd.forward = 1;
                    EX_DM.fwd.rs = 2;
                    EX_DM.fwd.rt = 2;
                    stall = 0;
                }else if(fwdD.canFWD_DMWB==1 && (ID_EX.inst.opcode==0x04 || ID_EX.inst.opcode==0x05) && DM_WB.inst.opcode<0x20)
                {
                    ID_EX.fwd.forward = 1;
                    ID_EX.fwd.rs = 1;
                    ID_EX.fwd.rt = 1;
                    ID_EX.REG_rs = DM_WB.ALUresult;
                    ID_EX.REG_rt = DM_WB.ALUresult;
                    stall = 0;
                }else stall = 1;
            }
        }else if(fwdD.isRSinEXDM==1 && fwdD.isRTinDMWB==1)
        {
            if(fwdD.canFWD_EXDM==1 && fwdD.canFWD_DMWB==1 && (ID_EX.inst.opcode!=0x04 && ID_EX.inst.opcode!=0x05))
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 1;
                EX_DM.fwd.rt = 2;
                stall = 0;
            }else stall = 1;
        }else if(fwdD.isRSinDMWB==1 && fwdD.isRTinEXDM==1)
        {
            if(fwdD.canFWD_EXDM==1 && fwdD.canFWD_DMWB==1 && (ID_EX.inst.opcode!=0x04 && ID_EX.inst.opcode!=0x05))
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 2;
                EX_DM.fwd.rt = 1;
                stall = 0;
            }else stall = 1;
        }else if(fwdD.isRSinEXDM==1)
        {
            if(fwdD.canFWD_EXDM==1 && (ID_EX.inst.opcode!=0x04 && ID_EX.inst.opcode!=0x05))
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 1;
                EX_DM.fwd.rt = 0;
                stall = 0;
            }else stall = 1;
        }else if(fwdD.isRTinEXDM==1)
        {
            if(fwdD.canFWD_EXDM==1 && (ID_EX.inst.opcode!=0x04 && ID_EX.inst.opcode!=0x05))
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 0;
                EX_DM.fwd.rt = 1;
                stall = 0;
            }else stall = 1;
        }else if(fwdD.isRSinDMWB==1)
        {
            if(fwdD.canFWD_DMWB==1 && (ID_EX.inst.opcode!=0x04 && ID_EX.inst.opcode!=0x05))
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 2;
                EX_DM.fwd.rt = 0;
                stall = 0;
            }else if(fwdD.canFWD_DMWB==1 && (ID_EX.inst.opcode==0x04 || ID_EX.inst.opcode==0x05) && DM_WB.inst.opcode<0x20)
            {
                ID_EX.fwd.forward = 1;
                ID_EX.fwd.rs = 1;
                ID_EX.fwd.rt = 0;
                ID_EX.REG_rs = DM_WB.ALUresult;
                stall = 0;
            }else stall = 1;
        }else if(fwdD.isRTinDMWB==1)
        {
            if(fwdD.canFWD_DMWB==1 && (ID_EX.inst.opcode!=0x04 && ID_EX.inst.opcode!=0x05))
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 0;
                EX_DM.fwd.rt = 2;
                stall = 0;
            }else if( fwdD.canFWD_DMWB==1 && (ID_EX.inst.opcode==0x04 || ID_EX.inst.opcode==0x05) && DM_WB.inst.opcode<0x20 )
            {
                ID_EX.fwd.forward = 1;
                ID_EX.fwd.rs = 0;
                ID_EX.fwd.rt = 1;
                ID_EX.REG_rt = DM_WB.ALUresult;
                stall = 0;
            }else stall = 1;
        }else stall = 0;
    }else if( ID_EX.inst.type=='R' && (ID_EX.inst.funct==0x00 || ID_EX.inst.funct==0x02 || ID_EX.inst.funct==0x03) )
    {
        if(fwdD.isRTinEXDM==1)
        {
            if(fwdD.canFWD_EXDM==1)
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 0;
                EX_DM.fwd.rt = 1;
                stall = 0;
            }else stall = 1;
        }else if(fwdD.isRTinDMWB==1)
        {
            if(fwdD.canFWD_DMWB==1)
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 0;
                EX_DM.fwd.rt = 2;
                stall = 0;
            }else stall = 1;
        }else stall = 0;
    }else if( (ID_EX.inst.type=='R' && ID_EX.inst.funct==0x08) || (ID_EX.inst.type=='I' && (ID_EX.inst.opcode!=0x0F && ID_EX.inst.opcode!=0x04 && ID_EX.inst.opcode!=0x05 && ID_EX.inst.opcode!=0x2B && ID_EX.inst.opcode!=0x28 && ID_EX.inst.opcode!=0x29)) )
    {
        if(fwdD.isRSinEXDM==1)
        {
            if(fwdD.canFWD_EXDM==1 && ((ID_EX.inst.type=='R' && ID_EX.inst.funct!=0x08) || (ID_EX.inst.type=='I' && ID_EX.inst.opcode!=0x07)) )
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 1;
                EX_DM.fwd.rt = 0;
                stall = 0;
            }else stall = 1;
        }else if(fwdD.isRSinDMWB==1)
        {
            if(fwdD.canFWD_DMWB==1 && ((ID_EX.inst.type=='R' && ID_EX.inst.funct!=0x08) || (ID_EX.inst.type=='I' && ID_EX.inst.opcode!=0x07)) )
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 2;
                EX_DM.fwd.rt = 0;
                stall = 0;
            }else if(fwdD.canFWD_DMWB==1 && ((ID_EX.inst.type=='R' && ID_EX.inst.funct==0x08) || (ID_EX.inst.type=='I' && ID_EX.inst.opcode==0x07)) && DM_WB.inst.opcode<0x20)
            {
                ID_EX.fwd.forward = 1;
                ID_EX.fwd.rs = 1;
                ID_EX.fwd.rt = 0;
                ID_EX.REG_rs = DM_WB.ALUresult;
                stall = 0;
            }else stall = 1;
        }else stall = 0;
    }
    //handle flush detection
    if(stall == 0)
    {
        if(ID_EX.inst.type=='R' && ID_EX.inst.funct==0x08) //jr
        {
            flush = 1;
            jumpAddress = ID_EX.REG_rs;
        }else if(ID_EX.inst.opcode == 0x04)  //beq
        {
            if(ID_EX.REG_rs == ID_EX.REG_rt)
            {
                flush = 1;
                jumpAddress = (ID_EX.PC + 4) + (4 * (int32_t)ID_EX.inst.C);
            }
        }else if(ID_EX.inst.opcode == 0x05)  //bne
        {
            if(ID_EX.REG_rs != ID_EX.REG_rt)
            {
                flush = 1;
                jumpAddress = (ID_EX.PC + 4) + (4 * (int32_t)ID_EX.inst.C);
            }
        }else if(ID_EX.inst.opcode == 0x07)  //bgtz
        {
            if((int32_t)ID_EX.REG_rs > 0)
            {
                flush = 1;
                jumpAddress = (ID_EX.PC + 4) + (4 * (int32_t)ID_EX.inst.C);
            }
        }else if(ID_EX.inst.opcode == 0x02)  //j
        {
            flush = 1;
            jumpAddress = ((ID_EX.PC+4) >> 28 << 28) | (4*ID_EX.inst.C);
        }else if(ID_EX.inst.opcode == 0x03)  //jal
        {
            flush = 1;
            jumpAddress = ((ID_EX.PC+4) >> 28 << 28) | (4*ID_EX.inst.C);
        }else flush = 0;
    }
}

void EXprocess()
{
    if(stall)
    {
        memset(&EX_DM, 0, sizeof(EXtoDM));
        strcpy(EX_DM.inst.name, "NOP");
        return;
    }
    if(  strcmp(ID_EX.inst.name, "NOP")!=0 && strcmp(ID_EX.inst.name, "HALT")!=0  )
    {
        EX_DM.prev_fwd = EX_DM.fwd;
        if (EX_DM.fwd.forward == 1)  //EX_DM now in DM_WB, DM_WB now in prevDM_WB !!
        {
            if(EX_DM.fwd.rs == 1)
            {
                ID_EX.REG_rs = DM_WB.ALUresult;
            }else if(EX_DM.fwd.rs == 2)
            {
                ID_EX.REG_rs = prevDM_WB.result;
            }

            if(EX_DM.fwd.rt == 1)
            {
                ID_EX.REG_rt = EX_DM.ALUresult;
            }else if(EX_DM.fwd.rt == 2)
            {
                ID_EX.REG_rt = prevDM_WB.result;
            }
        }

        if(ID_EX.inst.type == 'R')
        {
            switch(ID_EX.inst.funct){
                case 0x20:  //addr
                    EX_DM.ALUresult = (int32_t)ID_EX.REG_rs + (int32_t)ID_EX.REG_rt;
                    NumberOverflowDetection(ID_EX.REG_rs, ID_EX.REG_rt, EX_DM.ALUresult);
                    break;
                case 0x21:  //addu
                    EX_DM.ALUresult = ID_EX.REG_rs + ID_EX.REG_rt;
                    break;
                case 0x22:  //sub
                    EX_DM.ALUresult = (int32_t)ID_EX.REG_rs - (int32_t)ID_EX.REG_rt;
                    NumberOverflowDetection(ID_EX.REG_rs, ID_EX.REG_rt*(-1), EX_DM.ALUresult);
                    break;
                case 0x24:  //and
                    EX_DM.ALUresult = ID_EX.REG_rs & ID_EX.REG_rt;
                    break;
                case 0x25:   //or
                    EX_DM.ALUresult = ID_EX.REG_rs | ID_EX.REG_rt;
                    break;
                case 0x26:  //xor
                    EX_DM.ALUresult = ID_EX.REG_rs ^ ID_EX.REG_rt;
                    break;
                case 0x27:  //nor
                    EX_DM.ALUresult = ~(ID_EX.REG_rs | ID_EX.REG_rt);
                    break;
                case 0x28:  //nand
                    EX_DM.ALUresult = ~(ID_EX.REG_rs & ID_EX.REG_rt);
                    break;
                case 0x2A:  //slt
                    EX_DM.ALUresult = ((int32_t)ID_EX.REG_rs < (int32_t)ID_EX.REG_rt) ? 1:0;
                    break;
                case 0x00:  //sll
                    EX_DM.ALUresult = ID_EX.REG_rt << ID_EX.inst.C;
                    break;
                case 0x02:  //srl
                    EX_DM.ALUresult = ID_EX.REG_rt >> ID_EX.inst.C;
                    break;
                case 0x03:  //sra
                    EX_DM.ALUresult = (int32_t)ID_EX.REG_rt >> ID_EX.inst.C;
                    break;
                case 0x08:  //jr
                    break;
                case 0x18:  //mult
                    if(need_mfHILO==1) errorDetect.overwriteHILO = 1;
                    int64_t R_rs = (int32_t)ID_EX.REG_rs;
                    int64_t R_rt = (int32_t)ID_EX.REG_rt;
                    uint64_t result = R_rs * R_rt;
                    HI = result >> 32;
                    LO = result << 32 >> 32;
                    need_mfHILO = 1;
                    EX_DM.ALUresult = 0;
                    break;
                case 0x19:  //myluu
                    if(need_mfHILO==1) errorDetect.overwriteHILO = 1;
                    uint64_t uR_rs = (uint32_t)ID_EX.REG_rs;
                    uint64_t uR_rt = (uint32_t)ID_EX.REG_rt;
                    uint64_t u_result = uR_rs * uR_rt;
                    HI = u_result >> 32;
                    LO = u_result << 32 >> 32;
                    need_mfHILO = 1;
                    EX_DM.ALUresult = 0;
                    break;
                case 0x10:  //mfhi
                    need_mfHILO = 0;
                    EX_DM.ALUresult = HI;
                    break;
                case 0x12:  //mflo
                    need_mfHILO = 0;
                    EX_DM.ALUresult = LO;
                    break;
            }
        }else
        {
            switch (ID_EX.inst.opcode) {
                case 0x08:  //addi
                    EX_DM.ALUresult = (int32_t)ID_EX.REG_rs + (int32_t)ID_EX.inst.C;
                    NumberOverflowDetection((int32_t)ID_EX.REG_rs, (int32_t)ID_EX.inst.C, EX_DM.ALUresult);
                    break;
                case 0x09:  //addiu
                    EX_DM.ALUresult = ID_EX.REG_rs + ID_EX.inst.C;
                    break;
                case 0x23:  //lw
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.inst.C;
                    NumberOverflowDetection(ID_EX.REG_rs, (int32_t)ID_EX.inst.C, EX_DM.ALUresult);
                    break;
                case 0x21:  //lh
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.inst.C;
                    NumberOverflowDetection(ID_EX.REG_rs, (int32_t)ID_EX.inst.C, EX_DM.ALUresult);
                    break;
                case 0x25:  //lhu
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.inst.C;
                    NumberOverflowDetection(ID_EX.REG_rs, (int32_t)ID_EX.inst.C, EX_DM.ALUresult);
                    break;
                case 0x20:  //lb
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.inst.C;
                    NumberOverflowDetection(ID_EX.REG_rs, (int32_t)ID_EX.inst.C, EX_DM.ALUresult);
                    break;
                case 0x24:  //lbu
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.inst.C;
                    NumberOverflowDetection(ID_EX.REG_rs, (int32_t)ID_EX.inst.C, EX_DM.ALUresult);
                    break;
                case 0x2B:  //sw
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.inst.C;
                    NumberOverflowDetection(ID_EX.REG_rs, (int32_t)ID_EX.inst.C, EX_DM.ALUresult);
                    break;
                case 0x29:  //sh
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.inst.C;
                    NumberOverflowDetection(ID_EX.REG_rs, (int32_t)ID_EX.inst.C, EX_DM.ALUresult);
                    break;
                case 0x28:  //sb
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.inst.C;
                    NumberOverflowDetection(ID_EX.REG_rs, (int32_t)ID_EX.inst.C, EX_DM.ALUresult);
                    break;
                case 0x0F:  //lui
                    EX_DM.ALUresult = ID_EX.inst.C << 16;
                    break;
                case 0x0C:  //andi
                    EX_DM.ALUresult = ID_EX.REG_rs & (uint16_t)ID_EX.inst.C;
                    break;
                case 0x0D:  //ori
                    EX_DM.ALUresult = ID_EX.REG_rs | (uint16_t)ID_EX.inst.C;
                    break;
                case 0x0E:  //nori
                    EX_DM.ALUresult = ~(ID_EX.REG_rs | (uint16_t)ID_EX.inst.C);
                    break;
                case 0x0A:  //tlti
                    EX_DM.ALUresult = ((int32_t)ID_EX.REG_rs < (int32_t)ID_EX.inst.C) ? 1:0;
                    break;
                case 0x03:  //jal
                    EX_DM.ALUresult = ID_EX.PC + 4;
                    break;
            }
        }
    }else memset(&EX_DM.prev_fwd, 0, sizeof(forwarding));
    EX_DM.PC = ID_EX.PC;
    EX_DM.inst = ID_EX.inst;
    EX_DM.REG_rt = ID_EX.REG_rt;
}

void DMprocess()
{
    if( strcmp(EX_DM.inst.name, "NOP")!=0 && strcmp(EX_DM.inst.name, "HALT")!=0 )
    {
        unsigned int addr = EX_DM.ALUresult;
        if( EX_DM.inst.type == 'I' )  //detect error
        {
            if( EX_DM.inst.opcode==0x23 || EX_DM.inst.opcode==0x2B )  //lw & sw
            {
                MemAddOverflowDetection(addr, 3);
                DataMisalignedDetection(addr, 4);
            }else if( EX_DM.inst.opcode==0x21 || EX_DM.inst.opcode==0x25 ||  EX_DM.inst.opcode==0x29 )  //lh & lhu & sh
            {
                MemAddOverflowDetection(addr, 1);
                DataMisalignedDetection(addr, 2);
            }else if( EX_DM.inst.opcode==0x20 || EX_DM.inst.opcode==0x24 ||  EX_DM.inst.opcode==0x28 )  //lb & lbu & sb
            {
                MemAddOverflowDetection(addr, 0);
            }
        }
        if(halt != 1)  //If not error need to halt, than write in memory
        {
            if( EX_DM.inst.opcode==0x23 )  //lw
            {
                DM_WB.memData = (int32_t)(dMem[addr] << 24 | dMem[addr+1] << 16 | dMem[addr+2] << 8 | dMem[addr+3]);
            }else if( EX_DM.inst.opcode==0x21 )  //lh
            {
                DM_WB.memData = (int16_t)(dMem[addr] << 8 | dMem[addr+1]);
            }else if( EX_DM.inst.opcode==0x25 )  //lhu
            {
                DM_WB.memData = (uint16_t)(dMem[addr] << 8 | dMem[addr+1]);
            }else if( EX_DM.inst.opcode==0x20 )  //lb
            {
                DM_WB.memData = (int8_t)dMem[addr];
            }else if( EX_DM.inst.opcode==0x24 )  //lbu
            {
                DM_WB.memData = (uint8_t)dMem[addr];
            }else if( EX_DM.inst.opcode==0x2B )  //sw
            {
                dMem[addr]   = EX_DM.REG_rt >> 24;
                dMem[addr+1] = EX_DM.REG_rt >> 16;
                dMem[addr+2] = EX_DM.REG_rt >> 8;
                dMem[addr+3] = EX_DM.REG_rt;
            }else if( EX_DM.inst.opcode==0x29 )  //sh
            {
                dMem[addr]   = (EX_DM.REG_rt >> 8) & 0xFF;
                dMem[addr+1] = EX_DM.REG_rt & 0xFF;
            }else if( EX_DM.inst.opcode==0x28 )  //sb
            {
                dMem[addr] = EX_DM.REG_rt & 0xFF;
            }
        }
    }
    DM_WB.PC = EX_DM.PC;
    DM_WB.inst = EX_DM.inst;
    DM_WB.ALUresult = EX_DM.ALUresult;
}

void WBprocess()
{
    if( strcmp(DM_WB.inst.name, "NOP")!=0 && strcmp(DM_WB.inst.name, "HALT")!=0 )
    {
        if( DM_WB.inst.type == 'R' && strcmp(DM_WB.inst.name, "JR")!=0 && strcmp(DM_WB.inst.name, "MULT")!=0 && strcmp(DM_WB.inst.name, "MULTU")!=0 )
        {  //rType without jr, mult, multu
            if(DM_WB.inst.rd == 0) errorDetect.writeToRegZero = 1;
            else
            {
                REG[DM_WB.inst.rd] = DM_WB.ALUresult;
                prevDM_WB.result   =  DM_WB.ALUresult;
            }
        }else if( DM_WB.inst.type == 'I' )
        {  //iType without sw, sh, sb, beq, bne, bgtz
            if( DM_WB.inst.opcode==0x23 || DM_WB.inst.opcode==0x21 || DM_WB.inst.opcode==0x25 || DM_WB.inst.opcode==0x20 || DM_WB.inst.opcode==0x24 )
            {  //lw, lh, lhu, lb, lbu
                if(DM_WB.inst.rt == 0) errorDetect.writeToRegZero = 1;
                else
                {
                    REG[DM_WB.inst.rt] = DM_WB.memData;
                    prevDM_WB.result   =  DM_WB.memData;
                }
            }else if( DM_WB.inst.opcode==0x08 || DM_WB.inst.opcode==0x09 || DM_WB.inst.opcode==0x0F || DM_WB.inst.opcode==0x0C || DM_WB.inst.opcode==0x0D || DM_WB.inst.opcode==0x0E || DM_WB.inst.opcode==0x0A )
            {  //addi, addiu, lui, andi, ori, nori, slti
                if(DM_WB.inst.rt == 0) errorDetect.writeToRegZero = 1;
                else
                {
                    REG[DM_WB.inst.rt] = DM_WB.ALUresult;
                    prevDM_WB.result   =  DM_WB.ALUresult;
                }
            }
        }else if( DM_WB.inst.type == 'J' && DM_WB.inst.opcode==0x03 )
        {  //jal
            REG[31] = DM_WB.PC + 4;
            prevDM_WB.result =  DM_WB.PC + 4;
        }
    }
    prevDM_WB.inst = DM_WB.inst;
}
