#include "instruction.h"

void getInstruction()
{
    for(int i=0; i<4; i++) IF_ID.inst.inst = (IF_ID.inst.inst<<8) + (unsigned char)iMem[IF_ID.PC+i];
    decodeInstruction();
}

void decodeInstruction()
{
    IF_ID.inst.opcode = IF_ID.inst.inst >> 26;
    IF_ID.inst.rs = IF_ID.inst.inst << 6 >> 27;
    IF_ID.inst.rt = IF_ID.inst.inst << 11 >> 27;
    IF_ID.inst.rd = IF_ID.inst.inst << 16 >> 27;
    switch (IF_ID.inst.opcode) {
    case 0x00: //R-Type
        IF_ID.inst.type = 'R';
        IF_ID.inst.C  = IF_ID.inst.inst << 21 >> 27;
        IF_ID.inst.funct = IF_ID.inst.inst << 26 >> 26;
        switch (IF_ID.inst.funct) {
          case 0x20:
              strcpy(IF_ID.inst.name, "ADD");
              break;
          case 0x21:
              strcpy(IF_ID.inst.name, "ADDU");
              break;
          case 0x22:
              strcpy(IF_ID.inst.name, "SUB");
              break;
          case 0x24:
              strcpy(IF_ID.inst.name, "AND");
              break;
          case 0x25:
              strcpy(IF_ID.inst.name, "OR");
              break;
          case 0x26:
              strcpy(IF_ID.inst.name, "XOR");
              break;
          case 0x27:
              strcpy(IF_ID.inst.name, "NOR");
              break;
          case 0x28:
              strcpy(IF_ID.inst.name, "NAND");
              break;
          case 0x2A:
              strcpy(IF_ID.inst.name, "SLT");
              break;
          case 0x00:
              strcpy(IF_ID.inst.name, "SLL");
              break;
          case 0x02:
              strcpy(IF_ID.inst.name, "SRL");
              break;
          case 0x03:
              strcpy(IF_ID.inst.name, "SRA");
              break;
          case 0x08:
              strcpy(IF_ID.inst.name, "JR");
              break;
          case 0x18:
              strcpy(IF_ID.inst.name, "MULT");
              break;
          case 0x19:
              strcpy(IF_ID.inst.name, "MULTU");
              break;
          case 0x10:
              strcpy(IF_ID.inst.name, "MFHI");
              break;
          case 0x12:
              strcpy(IF_ID.inst.name, "MFLO");
              break;
        }
        break;
    //I-Type
    case 0x08:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "ADDI");
        break;
    case 0x09:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "ADDIU");
        break;
    case 0x23:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "LW");
        break;
    case 0x21:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "LH");
        break;
    case 0x25:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "LHU");
        break;
    case 0x20:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "LB");
        break;
    case 0x24:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "LBU");
        break;
    case 0x2B:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "SW");
        break;
    case 0x29:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "SH");
        break;
    case 0x28:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "SB");
        break;
    case 0x0F:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "LUI");
        break;
    case 0x0C:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "ANDI");
        break;
    case 0x0D:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "ORI");
        break;
    case 0x0E:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "NORI");
        break;
    case 0x0A:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "SLTI");
        break;
    case 0x04:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "BEQ");
        break;
    case 0x05:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "BNE");
        break;
    case 0x07:
        IF_ID.inst.type = 'I';
        IF_ID.inst.C  = (short int)IF_ID.inst.inst << 16 >> 16;
        strcpy(IF_ID.inst.name, "BGTZ");
        break;
    //J-Type
    case 0x02:
        IF_ID.inst.type = 'J';
        IF_ID.inst.C = IF_ID.inst.inst << 6 >> 6;
        strcpy(IF_ID.inst.name, "J");
        break;
    case 0x03:
        IF_ID.inst.type = 'J';
        IF_ID.inst.C = IF_ID.inst.inst << 6 >> 6;
        strcpy(IF_ID.inst.name, "JAL");
        break;
    //S-Type
    case 0x3F:
        IF_ID.inst.type = 'S';
        strcpy(IF_ID.inst.name, "HALT");
        break;
    }
    if(strcmp(IF_ID.inst.name, "SLL")==0)
    {
        if (IF_ID.inst.rt == 0 && IF_ID.inst.rd == 0 && IF_ID.inst.C == 0)
        {
            memset(&IF_ID.inst, 0, sizeof(instruct));
            strcpy(IF_ID.inst.name, "NOP");
        }
    }
}
