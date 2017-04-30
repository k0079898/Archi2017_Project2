#include "pipeline.h"

void IFprocess()
{
        if(IF_ID.stall!=1)
        {
            getIF();
            IF_ID.PC = PC;
            IF_ID.instruction = IF.inst;
            IF_ID.opcode = IF.opcode;
            IF_ID.instructionType = IF.type;
            IF_ID.rs = IF.rs;
            IF_ID.rt = IF.rt;
            IF_ID.rd = IF.rd;
            IF_ID.C  = IF.C;
            IF_ID.funct = IF.funct;
            strcpy(IF_ID.instName, IF.name);
        }
}

void IDprocess()
{
    if(stall == 1)
    {
		    ID_EX.REG_rs = REG[ID_EX.rs];
		    ID_EX.REG_rt = REG[ID_EX.rt];
	  }else if(flush) {
        memset(&ID_EX, 0, sizeof(IDtoEX));
        ID_EX.NOP = 1;
        strcpy(ID_EX.instName, "NOP");
    }else {
        ID_EX.PC = IF_ID.PC;
        ID_EX.instruction = IF_ID.instruction;
        ID_EX.opcode = IF_ID.opcode;
        ID_EX.instructionType = IF_ID.instructionType;
        if( ID_EX.instructionType == 'R' )
        {
            ID_EX.rs     = IF_ID.rs;
            ID_EX.rt     = IF_ID.rt;
            ID_EX.rd     = IF_ID.rd;
            ID_EX.C      = IF_ID.C;
            ID_EX.funct  = IF_ID.funct;
            ID_EX.REG_rs = REG[ID_EX.rs];
            ID_EX.REG_rt = REG[ID_EX.rt];
            strcpy(ID_EX.instName, IF_ID.instName);
        }else if( ID_EX.instructionType == 'I' )
        {
            ID_EX.rs     = IF_ID.rs;
            ID_EX.rt     = IF_ID.rt;
            ID_EX.C      = (short int)IF_ID.C;
            ID_EX.REG_rs = REG[ID_EX.rs];
    		    ID_EX.REG_rt = REG[ID_EX.rt];
            strcpy(ID_EX.instName, IF_ID.instName);
        }else if( ID_EX.instructionType == 'J' )
        {
            ID_EX.C = IF.C;
            strcpy(ID_EX.instName, IF_ID.instName);
        }else if( ID_EX.instructionType == 'S' )
        {
            strcpy(ID_EX.instName, IF_ID.instName);
        }
        if( strcmp(ID_EX.instName, "NOP")==0 ) ID_EX.NOP = 1;
        else ID_EX.NOP = 0;
    }
    //handle stall
    stall = 0;
    flush = 0;
    ID_EX.fwd.forward = 0;
    EX_DM.fwd.forward = 0;
    isRSinEXDM = 0;
    isRSinDMWB = 0;
    isRTinEXDM = 0;
    isRTinDMWB = 0;
    canFWD_EXDM = 0;
    canFWD_DMWB = 0;
    if( (EX_DM.instructionType=='R' && (EX_DM.funct!=0x08 || EX_DM.funct!=0x18 || EX_DM.funct!=0x19) && ID_EX.rs==EX_DM.rd) || (EX_DM.opcode>=0x08 && EX_DM.opcode<=0x25 && ID_EX.rs==EX_DM.rt) )
    {
        if(ID_EX.rs != 0 && ((IF_ID.instructionType=='R' && (IF_ID.funct!=0x10 || IF_ID.funct!=0x12)) || (IF_ID.instructionType=='I' && (IF_ID.funct!=0x0F)))) isRSinEXDM = 1;
    }
    if( (DM_WB.instructionType=='R' && (DM_WB.funct!=0x08 || DM_WB.funct!=0x18 || DM_WB.funct!=0x19) && ID_EX.rs==DM_WB.rd) || (DM_WB.opcode>=0x08 && DM_WB.opcode<=0x25 && ID_EX.rs==DM_WB.rt) )
    {
        if(ID_EX.rs != 0 && ((IF_ID.instructionType=='R' && (IF_ID.funct!=0x10 || IF_ID.funct!=0x12)) || (IF_ID.instructionType=='I' && (IF_ID.funct!=0x0F)))) isRSinDMWB = 1;
    }
    if( (EX_DM.instructionType=='R' && (EX_DM.funct!=0x08 || EX_DM.funct!=0x18 || EX_DM.funct!=0x19) && ID_EX.rt==EX_DM.rd) || (EX_DM.opcode>=0x08 && EX_DM.opcode<=0x25 && ID_EX.rt==EX_DM.rt) )
    {
        if(ID_EX.rt != 0 && ((IF_ID.instructionType=='R' && (IF_ID.funct!=0x10 || IF_ID.funct!=0x12)) || (IF_ID.instructionType=='I' && (IF_ID.funct!=0x0F)))) isRTinEXDM = 1;
    }
    if( (DM_WB.instructionType=='R' && (DM_WB.funct!=0x08 || DM_WB.funct!=0x18 || DM_WB.funct!=0x19) && ID_EX.rt==DM_WB.rd) || (DM_WB.opcode>=0x08 && DM_WB.opcode<=0x25 && ID_EX.rt==DM_WB.rt) )
    {
        if(ID_EX.rt != 0 && ((IF_ID.instructionType=='R' && (IF_ID.funct!=0x10 || IF_ID.funct!=0x12)) || (IF_ID.instructionType=='I' && (IF_ID.funct!=0x0F)))) isRTinDMWB = 1;
    }
    if( ((EX_DM.instructionType=='R' && (EX_DM.funct!=0x08 || EX_DM.funct!=0x18 || EX_DM.funct!=0x19)) ||  (EX_DM.opcode>=0x08 && EX_DM.opcode<=0x0F)) ) canFWD_EXDM = 1;
    if( (DM_WB.instructionType=='R' && (DM_WB.funct!=0x08 || DM_WB.funct!=0x18 || DM_WB.funct!=0x19)) ||  ((DM_WB.opcode>=0x08 && DM_WB.opcode<=0x0F) || (DM_WB.opcode>=0x20 && DM_WB.opcode<=0x25))) canFWD_DMWB = 1;
    //printf("RS: %d %d / RT: %d %d / %d %d\n", isRSinEXDM, isRSinDMWB, isRTinEXDM, isRTinDMWB, canFWD_EXDM, canFWD_DMWB);
    if( (ID_EX.instructionType=='R' && ID_EX.funct>=0x18 && ID_EX.funct<=0x2A) || (ID_EX.opcode==0x04 || ID_EX.opcode==0x05 || ID_EX.opcode==0x2B || ID_EX.opcode==0x28 || ID_EX.opcode==0x29) )
    {
        if( (isRSinEXDM==1 && isRTinEXDM==1) || (isRSinDMWB==1 && isRTinDMWB==1) )
        {
            if(isRSinEXDM==1)
            {
                if(canFWD_EXDM==1 && (ID_EX.opcode!=0x04 && ID_EX.opcode!=0x05))
                {
                    EX_DM.fwd.forward = 1;
                    EX_DM.fwd.rs = 1;
                    EX_DM.fwd.rt = 1;
                    stall = 0;
                }else if(ID_EX.opcode==0x04 || ID_EX.opcode==0x05)
                {
                    stall = 1;
                }else stall = 1;
            }else if(isRSinDMWB==1)
            {
                if(canFWD_DMWB==1 && (ID_EX.opcode!=0x04 && ID_EX.opcode!=0x05))
                {
                    EX_DM.fwd.forward = 1;
                    EX_DM.fwd.rs = 2;
                    EX_DM.fwd.rt = 2;
                    stall = 0;
                }else if(canFWD_DMWB==1 && (ID_EX.opcode==0x04 || ID_EX.opcode==0x05) && DM_WB.opcode<0x20)
                {
                    ID_EX.fwd.forward = 1;
                    ID_EX.fwd.rs = 1;
                    ID_EX.fwd.rt = 1;
                    ID_EX.REG_rs = DM_WB.ALUresult;
                    ID_EX.REG_rt = DM_WB.ALUresult;
                    stall = 0;
                }else stall = 1;
            }
        }else if(isRSinEXDM==1 && isRTinDMWB==1)
        {
            if(canFWD_EXDM==1 && canFWD_DMWB==1 && (ID_EX.opcode!=0x04 && ID_EX.opcode!=0x05))
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 1;
                EX_DM.fwd.rt = 2;
                stall = 0;
            }else if(canFWD_DMWB==1 && (ID_EX.opcode==0x04 || ID_EX.opcode==0x05) && DM_WB.opcode<0x20)
            {
                ID_EX.fwd.forward = 1;
                ID_EX.fwd.rs = 0;
                ID_EX.fwd.rt = 1;
                ID_EX.REG_rt = DM_WB.ALUresult;
                stall = 0;
            }else stall = 1;
        }else if(isRSinDMWB==1 && isRTinEXDM==1)
        {
            if(canFWD_EXDM==1 && canFWD_DMWB==1 && (ID_EX.opcode!=0x04 && ID_EX.opcode!=0x05))
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 2;
                EX_DM.fwd.rt = 1;
                stall = 0;
            }else if(canFWD_DMWB==1 && (ID_EX.opcode==0x04 || ID_EX.opcode==0x05) && DM_WB.opcode<0x20)
            {
                ID_EX.fwd.forward = 1;
                ID_EX.fwd.rs = 1;
                ID_EX.fwd.rt = 0;
                ID_EX.REG_rs = DM_WB.ALUresult;
                stall = 0;
            }else stall = 1;
        }else if(isRSinEXDM==1)
        {
            if(canFWD_EXDM==1 && (ID_EX.opcode!=0x04 && ID_EX.opcode!=0x05))
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 1;
                EX_DM.fwd.rt = 0;
                stall = 0;
            }else if(ID_EX.opcode==0x04 || ID_EX.opcode==0x05)
            {
                stall = 1;
            }else stall = 1;
        }else if(isRTinEXDM==1)
        {
            if(canFWD_EXDM==1 && (ID_EX.opcode!=0x04 && ID_EX.opcode!=0x05))
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 0;
                EX_DM.fwd.rt = 1;
                stall = 0;
            }else if(ID_EX.opcode==0x04 || ID_EX.opcode==0x05)
            {
                stall = 1;
            }else stall = 1;
        }else if(isRSinDMWB==1)
        {
            if(canFWD_DMWB==1 && (ID_EX.opcode!=0x04 && ID_EX.opcode!=0x05))
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 2;
                EX_DM.fwd.rt = 0;
                stall = 0;
            }else if(canFWD_DMWB==1 && (ID_EX.opcode==0x04 || ID_EX.opcode==0x05) && DM_WB.opcode<0x20)
            {
                ID_EX.fwd.forward = 1;
                ID_EX.fwd.rs = 1;
                ID_EX.fwd.rt = 0;
                ID_EX.REG_rs = DM_WB.ALUresult;
                stall = 0;
            }else stall = 1;
        }else if(isRTinDMWB==1)
        {
            if(canFWD_DMWB==1 && (ID_EX.opcode!=0x04 && ID_EX.opcode!=0x05))
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 0;
                EX_DM.fwd.rt = 2;
                stall = 0;
            }else if( canFWD_DMWB==1 && (ID_EX.opcode==0x04 || ID_EX.opcode==0x05) && DM_WB.opcode<0x20 )
            {
                ID_EX.fwd.forward = 1;
                ID_EX.fwd.rs = 0;
                ID_EX.fwd.rt = 1;
                ID_EX.REG_rt = DM_WB.ALUresult;
                stall = 0;
            }else stall = 1;
        }else stall = 0;
    }else if( ID_EX.instructionType=='R' && (ID_EX.funct==0x00 || ID_EX.funct==0x02 || ID_EX.funct==0x03) )
    {
        if(isRTinEXDM==1)
        {
            if(canFWD_EXDM==1)
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 0;
                EX_DM.fwd.rt = 1;
                stall = 0;
            }else stall = 1;
        }else if(isRTinDMWB==1)
        {
            if(canFWD_DMWB==1)
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 0;
                EX_DM.fwd.rt = 2;
                stall = 0;
            }else stall = 1;
        }else stall = 0;
    }else if( (ID_EX.instructionType=='R' && ID_EX.funct==0x08) || (ID_EX.opcode!=0x0F && ID_EX.opcode!=0x04 && ID_EX.opcode!=0x05 || ID_EX.opcode!=0x2B || ID_EX.opcode!=0x28 || ID_EX.opcode!=0x29) )
    {
        if(isRSinEXDM==1)
        {
            if(canFWD_EXDM==1 && ((ID_EX.instructionType=='R' && ID_EX.funct!=0x08) || ID_EX.opcode!=0x07) )
            {
                EX_DM.fwd.forward = 1;
                EX_DM.fwd.rs = 1;
                EX_DM.fwd.rt = 0;
                stall = 0;
            }else if(canFWD_EXDM==1 && ((ID_EX.instructionType=='R' && ID_EX.funct==0x08) || ID_EX.opcode==0x07 ))
            {
                stall = 1;
            }else stall = 1;
        }else if(isRSinDMWB==1)
        {
            if(canFWD_DMWB==1)
            {
                if(canFWD_DMWB==1 && ((ID_EX.instructionType=='R' && ID_EX.funct!=0x08) || ID_EX.opcode!=0x07) )
                {
                    EX_DM.fwd.forward = 1;
                    EX_DM.fwd.rs = 2;
                    EX_DM.fwd.rt = 0;
                    stall = 0;
                }else if(canFWD_DMWB==1 && ((ID_EX.instructionType=='R' && ID_EX.funct==0x08) || ID_EX.opcode==0x07 ))
                {
                    ID_EX.fwd.forward = 1;
                    ID_EX.fwd.rs = 1;
                    ID_EX.fwd.rt = 0;
                    ID_EX.REG_rs = DM_WB.ALUresult;
                    stall = 0;
                }else stall = 1;
            }else stall = 0;
        }else stall = 0;
    }
    //handle flush
    if(stall == 0)
    {
        if(ID_EX.instructionType=='R' && ID_EX.funct==0x08) //jr
        {
            flush = 1;
            jumpAddress = ID_EX.REG_rs;
        }else if(ID_EX.opcode == 0x04)  //beq
        {
            if(ID_EX.REG_rs == ID_EX.REG_rt)
            {
                flush = 1;
                jumpAddress = (ID_EX.PC + 4) + (4 * ID_EX.C);
            }
        }else if(ID_EX.opcode == 0x05)  //bne
        {
            if(ID_EX.REG_rs != ID_EX.REG_rt)
            {
                flush = 1;
                jumpAddress = (ID_EX.PC + 4) + (4 * ID_EX.C);
            }
        }else if(ID_EX.opcode == 0x07)  //bgtz
        {
            if((int32_t)ID_EX.REG_rs > 0)
            {
                flush = 1;
                jumpAddress = (ID_EX.PC + 4) + (4 * ID_EX.C);
            }
        }else if(ID_EX.opcode == 0x02)  //j
        {
            flush = 1;
            jumpAddress = ((ID_EX.PC+4) >> 28 << 28) | (4*ID_EX.C);
        }else if(ID_EX.opcode == 0x03)  //jal
        {
            flush = 1;
            jumpAddress = ((ID_EX.PC+4) >> 28 << 28) | (4*ID_EX.C);
        }else flush = 0;
    }
}

void EXprocess()
{
    if(stall)
    {
        memset(&EX_DM, 0, sizeof(EXtoDM));
        EX_DM.NOP = 1;
        strcpy(EX_DM.instName, "NOP");
        return;
    }
    if( strcmp(ID_EX.instName, "HALT")==0 )
    {
        memset(&EX_DM, 0, sizeof(EXtoDM));
        EX_DM.NOP = 1;
        strcpy(EX_DM.instName, "HALT");
        return;
    }
    if( ID_EX.NOP != 1 )
    {
        EX_DM.prev_fwd = EX_DM.fwd;
        if (EX_DM.fwd.forward == 1)
        {
            if(EX_DM.fwd.rs == 1)
            {
                ID_EX.REG_rs = DM_WB.ALUresult;
            }else if(EX_DM.fwd.rs == 2)
            {
                ID_EX.REG_rs = prevDM_WB.ALUresult;
            }

            if(EX_DM.fwd.rt == 1)
            {
                ID_EX.REG_rt = EX_DM.ALUresult;
            }else if(EX_DM.fwd.rt == 2)
            {
                ID_EX.REG_rt = prevDM_WB.ALUresult;
            }
        }

        if(ID_EX.instructionType == 'R')
        {
            switch(ID_EX.funct){
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
                    EX_DM.ALUresult = ID_EX.REG_rt << ID_EX.C;
                    break;
                case 0x02:  //srl
                    EX_DM.ALUresult = ID_EX.REG_rt >> ID_EX.C;
                    break;
                case 0x03:  //sra
                    EX_DM.ALUresult = (int32_t)ID_EX.REG_rt >> ID_EX.C;
                    break;
                case 0x08:  //jr
                    break;
                case 0x18:  //mult
                    if(need_mfHILO==1) overwriteHILO = 1;
                    int64_t R_rs = (int32_t)ID_EX.REG_rs;
                    int64_t R_rt = (int32_t)ID_EX.REG_rt;
                    uint64_t result = R_rs * R_rt;
                    HI = result >> 32;
                    LO = result << 32 >> 32;
                    need_mfHILO = 1;
                    EX_DM.ALUresult = 0;
                    break;
                case 0x19:  //myluu
                    if(need_mfHILO==1) overwriteHILO = 1;
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
            switch (ID_EX.opcode) {
                case 0x08:  //addi
                    EX_DM.ALUresult = (int32_t)ID_EX.REG_rs + (int32_t)ID_EX.C;
                    NumberOverflowDetection(ID_EX.REG_rs, ID_EX.C, EX_DM.ALUresult);
                    break;
                case 0x09:  //addiu
                    EX_DM.ALUresult = ID_EX.REG_rs + ID_EX.C;
                    break;
                case 0x23:  //lw
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.C;
                    NumberOverflowDetection(ID_EX.REG_rs, ID_EX.C, EX_DM.ALUresult);
                    break;
                case 0x21:  //lh
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.C;
                    NumberOverflowDetection(ID_EX.REG_rs, ID_EX.C, EX_DM.ALUresult);
                    break;
                case 0x25:  //lhu
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.C;
                    NumberOverflowDetection(ID_EX.REG_rs, ID_EX.C, EX_DM.ALUresult);
                    break;
                case 0x20:  //lb
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.C;
                    NumberOverflowDetection(ID_EX.REG_rs, ID_EX.C, EX_DM.ALUresult);
                    break;
                case 0x24:  //lbu
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.C;
                    NumberOverflowDetection(ID_EX.REG_rs, ID_EX.C, EX_DM.ALUresult);
                    break;
                case 0x2B:  //sw
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.C;
                    NumberOverflowDetection(ID_EX.REG_rs, ID_EX.C, EX_DM.ALUresult);
                    break;
                case 0x29:  //sh
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.C;
                    NumberOverflowDetection(ID_EX.REG_rs, ID_EX.C, EX_DM.ALUresult);
                    break;
                case 0x28:  //sb
                    EX_DM.ALUresult = ID_EX.REG_rs + (int32_t)ID_EX.C;
                    NumberOverflowDetection(ID_EX.REG_rs, ID_EX.C, EX_DM.ALUresult);
                    break;
                case 0x0F:  //lui
                    EX_DM.ALUresult = ID_EX.C << 16;
                    break;
                case 0x0C:  //andi
                    EX_DM.ALUresult = ID_EX.REG_rs & (uint16_t)ID_EX.C;
                    break;
                case 0x0D:  //ori
                    EX_DM.ALUresult = ID_EX.REG_rs | (uint16_t)ID_EX.C;
                    break;
                case 0x0E:  //nori
                    EX_DM.ALUresult = ~(ID_EX.REG_rs | (uint16_t)ID_EX.C);
                    break;
                case 0x0A:  //tlti
                    EX_DM.ALUresult = ((int32_t)ID_EX.REG_rs < (int32_t)ID_EX.C) ? 1:0;
                    break;
                case 0x03:  //jal
                    EX_DM.ALUresult = ID_EX.PC + 4;
                    break;
                case 0x3F:  //halt
                    break;
            }
        }
    }
    EX_DM.PC = ID_EX.PC;
    EX_DM.instruction = ID_EX.instruction;
    EX_DM.opcode = ID_EX.opcode;
    EX_DM.instructionType = ID_EX.instructionType;
    EX_DM.rs = ID_EX.rs;
    EX_DM.rt = ID_EX.rt;
    EX_DM.REG_rt = ID_EX.REG_rt;
    EX_DM.rd = ID_EX.rd;
    EX_DM.funct = ID_EX.funct;
    EX_DM.NOP = ID_EX.NOP;
    strcpy(EX_DM.instName, ID_EX.instName);
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
    DM_WB.rd = EX_DM.rd;
    DM_WB.rt = EX_DM.rt;
    DM_WB.funct = EX_DM.funct;
    DM_WB.ALUresult = EX_DM.ALUresult;
    DM_WB.NOP = EX_DM.NOP;
}

void WBprocess()
{
    if( DM_WB.NOP != 1 )
    {
        if( DM_WB.instructionType == 'R' && strcmp(DM_WB.instName, "JR")!=0 && strcmp(DM_WB.instName, "MULT")!=0 && strcmp(DM_WB.instName, "MULTU")!=0 )
        {
            if(DM_WB.rd == 0) writeToRegZero = 1;
            else
            {
                REG[DM_WB.rd] = DM_WB.ALUresult;
                prevDM_WB.ALUresult =  DM_WB.ALUresult;
            }
        }else if( DM_WB.instructionType == 'I' )
        {
            if( DM_WB.opcode==0x23 || DM_WB.opcode==0x21 || DM_WB.opcode==0x25 || DM_WB.opcode==0x20 || DM_WB.opcode==0x24 )
            {
                if(DM_WB.rt == 0) writeToRegZero = 1;
                else
                {
                    REG[DM_WB.rt] = DM_WB.memData;
                    prevDM_WB.memData =  DM_WB.memData;
                    prevDM_WB.ALUresult =  DM_WB.memData;
                }
            }else if( DM_WB.opcode==0x08 || DM_WB.opcode==0x09 || DM_WB.opcode==0x0F || DM_WB.opcode==0x0C || DM_WB.opcode==0x0D || DM_WB.opcode==0x0E || DM_WB.opcode==0x0A )
            {
                if(DM_WB.rt == 0) writeToRegZero = 1;
                else
                {
                    REG[DM_WB.rt] = DM_WB.ALUresult;
                    prevDM_WB.ALUresult =  DM_WB.ALUresult;
                }
            }
        }else if( DM_WB.instructionType == 'J' && DM_WB.opcode==0x03 )
        {
            REG[31] = DM_WB.PC + 4;
            prevDM_WB.ALUresult =  DM_WB.PC + 4;
        }
    }
    prevDM_WB.PC = DM_WB.PC;
    prevDM_WB.instruction = DM_WB.instruction;
    prevDM_WB.instructionType = DM_WB.instructionType;
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
