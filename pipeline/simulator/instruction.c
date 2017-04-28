#include "instruction.h"

void getIF()
{
    for(int i=0; i<4; i++) IF.inst = (IF.inst<<8) + (unsigned char)iMem[PC+i];
    decodeIF();
}

void decodeIF()
{
    IF.opcode = IF.inst >> 26;
    IF.rs = IF.inst << 6 >> 27;
    IF.rt = IF.inst << 11 >> 27;
    IF.rd = IF.inst << 16 >> 27;
    switch (IF.opcode) {
    case 0x00: //R-Type
        IF.type = 'R';
        IF.C  = IF.inst << 21 >> 27;
        IF.funct = IF.inst << 26 >> 26;
        switch (IF.funct) {
          case 0x20:
              strcpy(IF.name, "ADD");
              break;
          case 0x21:
              strcpy(IF.name, "ADDU");
              break;
          case 0x22:
              strcpy(IF.name, "SUB");
              break;
          case 0x24:
              strcpy(IF.name, "AND");
              break;
          case 0x25:
              strcpy(IF.name, "OR");
              break;
          case 0x26:
              strcpy(IF.name, "XOR");
              break;
          case 0x27:
              strcpy(IF.name, "NOR");
              break;
          case 0x28:
              strcpy(IF.name, "NAND");
              break;
          case 0x2A:
              strcpy(IF.name, "SLT");
              break;
          case 0x00:
              strcpy(IF.name, "SLL");
              break;
          case 0x02:
              strcpy(IF.name, "SRL");
              break;
          case 0x03:
              strcpy(IF.name, "SRA");
              break;
          case 0x08:
              strcpy(IF.name, "JR");
              break;
          case 0x18:
              strcpy(IF.name, "MULT");
              break;
          case 0x19:
              strcpy(IF.name, "MULTU");
              break;
          case 0x10:
              strcpy(IF.name, "MFHI");
              break;
          case 0x12:
              strcpy(IF.name, "MFLO");
              break;
        }
        break;
    //I-Type
    case 0x08:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "ADDI");
        break;
    case 0x09:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "ADDIU");
        break;
    case 0x23:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "LW");
        break;
    case 0x21:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "LH");
        break;
    case 0x25:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "LHU");
        break;
    case 0x20:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "LB");
        break;
    case 0x24:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "LBU");
        break;
    case 0x2B:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "SW");
        break;
    case 0x29:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "SH");
        break;
    case 0x28:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "SB");
        break;
    case 0x0F:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "LUI");
        break;
    case 0x0C:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "ANDI");
        break;
    case 0x0D:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "ORI");
        break;
    case 0x0E:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "NORI");
        break;
    case 0x0A:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "SLTI");
        break;
    case 0x04:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "BEQ");
        break;
    case 0x05:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "BNE");
        break;
    case 0x07:
        IF.type = 'I';
        IF.C  = (short int)IF.inst << 16 >> 16;
        strcpy(IF.name, "BGTZ");
        break;
    //J-Type
    case 0x02:
        IF.type = 'J';
        IF.C = IF.inst << 6 >> 6;
        strcpy(IF.name, "J");
        break;
    case 0x03:
        IF.type = 'J';
        IF.C = IF.inst << 6 >> 6;
        strcpy(IF.name, "JAL");
        break;
    //S-Type
    case 0x3F:
        IF.type = 'S';
        strcpy(IF.name, "HALT");
        break;
    }
    if(strcmp(IF.name, "SLL")==0)
    {
        if (IF.rt == 0 && IF.rd == 0 && IF.C == 0) strcpy(IF.name, "NOP");
    }
}
