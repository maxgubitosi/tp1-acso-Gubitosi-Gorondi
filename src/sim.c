#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"

#define DEBUG 1        // 1 para debuggear

typedef enum {
    ADDS = 1,
    ADDSI,
    HLT,
    SUBS,
    SUBSI,
    CMP,
    CMPI,
    ANDS,
    EOR,
    ORR,
    LSL,
    LSR,
    STUR,
    STURB,
    STURH,
    LDUR,
    LDURH,
    LDURB,
    MOVZ,
    B,
    BR,
    BEQ,
    BNE,
    BGT,
    BLT,
    BGE,
    BLE
} instr_t;

uint32_t curr_instr;  // current instruction
void fetch() {
    curr_instr = mem_read_32(CURRENT_STATE.PC);
    if (DEBUG == 1) {
    printf("fetch\n");
    printf("comentario\n");
    }
}

// INSTRUCCIONES A IMPLEMENTAR:
/*
    1. ADDS --
    2. SUBS --
    3. HLT --
    4. CMP --
    5. ANDS --
    6. EOR --
    7. ORR --
    8. B
    9. BR
    10. B.COND
        - BEQ
        - BNE
        - BGT
        - BLT
        - BGE
        - BLE
    11. LSL --
    12. LSR --
    13. STUR --
    14. STURB --
    15. STURH --
    16. LDUR --
    17. LDURH --
    18. LDURB --
    19. MOVZ --
*/



instr_t instr_name=0;
uint32_t rd;
uint32_t rt;
uint32_t rn;
uint32_t rm;
uint32_t imm;
uint32_t shift;
uint32_t immr;

int64_t result;                     // RECIEN CAMBIE RESULT, AUX1 y AUX2 A INT64_T (ERAN UINT64) PARA QUE NO SE ROMPA CUANDO HAGO LA RESTA, CHEQUEAR QUE NO HAYA ROTO OTRAS COSAS !?!?!!!!????
int64_t aux1;
int64_t aux2;
int64_t imm26;

/* IDEA: hacer un struct con los instructions:
    typedef struct instruction {
        char *name;
        uint32_t opcode;
        char *type: // R, I, D, B, CB, IW;
        uint32_t rd;
        uint32_t rn;
        uint32_t rm;
    } instruction_t;

*/



// ------------------------- EXECUTE -------------------------

// EXECUTE SPECIFIC INSTRUCTIONS
void execute_ADDS_ext() {
    if (DEBUG == 1) {printf("execute_ADDS\n");}
    uint64_t op1 = CURRENT_STATE.REGS[rn];
    uint64_t op2 = CURRENT_STATE.REGS[rm];
    result = op1 + op2;
    NEXT_STATE.REGS[rd] = result; 
}

void execute_SUBS_ext() {
    if (DEBUG == 1) {printf("execute_SUBS\n");}
    uint64_t op1 = CURRENT_STATE.REGS[rn];
    uint64_t op2 = CURRENT_STATE.REGS[rm];
    result = op1 - op2;
    NEXT_STATE.REGS[rd] = result; 
}
void execute_ADDS_imm() {
    if (DEBUG == 1) {printf("execute_ADDS\n");}
    uint64_t op1 = CURRENT_STATE.REGS[rn];
    uint64_t op2 = imm;
    result = op1 + op2;
    NEXT_STATE.REGS[rd] = result; 
}

void execute_SUBS_imm() {
    if (DEBUG == 1) {printf("execute_SUBSI\n");}
    uint64_t op1 = CURRENT_STATE.REGS[rn];
    uint64_t op2 = imm;
    result = op1 - op2;                            // ACA RESTA Y COMO SON UINT EL FLAG N NUSNCA VA VER QUE ES NEGATIVO. ASIQUE VOY A CAMBIAR LO DE FLAG A QUE MIRE EL PRIMER INT Y LISTO.
    NEXT_STATE.REGS[rd] = result;
}

void execute_HLT() {
    if (DEBUG == 1) {printf("execute_HLT\n");}
    RUN_BIT = FALSE;
}

void execute_CMP() {
    if (DEBUG == 1) {printf("execute_CMP\n");}
    uint64_t op1 = CURRENT_STATE.REGS[rn];
    uint64_t op2 = CURRENT_STATE.REGS[rm];
    result = op1 - op2;
    NEXT_STATE.REGS[0b11111] = result;   //chequear esto resetear?????????? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void execute_CMP_imm() {
    if (DEBUG == 1) {printf("execute_CMPI\n");}
    uint64_t op1 = CURRENT_STATE.REGS[rn];
    uint64_t op2 = imm;
    result = op1 - op2;
    NEXT_STATE.REGS[0b11111] = result;
}

void execute_ANDS() {
    if (DEBUG == 1) {printf("execute_ANDS\n");}
    uint64_t op1 = CURRENT_STATE.REGS[rn];
    uint64_t op2 = CURRENT_STATE.REGS[rm];
    uint64_t aux = op2;

    switch (shift) {
    case 0b00:        // LSL
        op2 = op2 << imm;
        break;
    case 0b01:       // LSR
        op2 = op2 >> imm;
        break;
    case 0b10:      // ASR
        if (op2>>63 == 0b1) {              // ver que onda uint aca si tamos usando bien. si es lo mismo ver si es neg que si el primer bit es 1 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            op2 = op2 >> imm;
            op2 = op2 | ~(0xFFFFFFFFFFFFFFFF >> imm); //  A CHEQUAAAARRRRRRR !!!!!!!!!!!!
        } else {
            op2 = op2 >> imm;
        }
        break;
    case 0b11:
        op2 = op2 >> imm;
        op2 = op2 | (aux << (64-imm));
        break;
    default:
        printf("Error en execute_ANDS\n");    
        break;
    }
    result = op1 & op2;  // asi modifica los flags
    NEXT_STATE.REGS[rd] = result;
}

void execute_EOR() {
    printf("curr_instr: %d\n", curr_instr);
    if (DEBUG == 1) {printf("execute_EOR\n");}
    uint64_t op1 = CURRENT_STATE.REGS[rn];
    uint64_t op2 = CURRENT_STATE.REGS[rm];
    uint64_t aux = op2;
    printf("shift: %d, imm: %d\n", shift, imm);
    switch (shift) {
    case 0b00:        // LSL
        printf("entra en LSL\n");
        op2 = op2 << imm;
        break;
    case 0b01:       // LSR
        printf("entra en LSR\n");
        op2 = op2 >> imm;
        break;
    case 0b10:      // ASR
        printf("entra en ASR\n");
        if (op2>>63 == 0b1) {              // ver que onda uint aca si tamos usando bien. si es lo mismo ver si es neg que si el primer bit es 1 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            op2 = op2 >> imm;
            op2 = op2 | ~(0xFFFFFFFFFFFFFFFF >> imm); //  A CHEQUAAAARRRRRRR !!!!!!!!!!!!
        } else {
            op2 = op2 >> imm;
        }
        break;
    case 0b11:   // ROR
    printf("entra en ROR\n");
        op2 = op2 >> imm;
        op2 = op2 | (aux << (64-imm));
        break;
    default:
        printf("Error en execute_EOR\n");    
        break;
    }
    NEXT_STATE.REGS[rd] = op1 ^ op2;   // devuelve el resultado directo de la operacion para no modificar el result y que cambien los flags

}

void execute_ORR() {
    if (DEBUG == 1) {printf("execute_ORR\n");}
    uint64_t op1 = CURRENT_STATE.REGS[rn];
    uint64_t op2 = CURRENT_STATE.REGS[rm];
    uint64_t aux = op2;

    switch (shift) {
    case 0b00:        // LSL
        op2 = op2 << imm;
        break;
    case 0b01:       // LSR
        op2 = op2 >> imm;
        break;
    case 0b10:      // ASR
        if (op2>>63 == 0b1) {              // ver que onda uint aca si tamos usando bien. si es lo mismo ver si es neg que si el primer bit es 1 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            op2 = op2 >> imm;
            op2 = op2 | ~(0xFFFFFFFFFFFFFFFF >> imm); //  A CHEQUAAAARRRRRRR !!!!!!!!!!!!
        } else {
            op2 = op2 >> imm;
        }
        break;
    case 0b11:
        op2 = op2 >> imm;
        op2 = op2 | (aux << (64-imm));
        break;
    default:
        printf("Error en execute_ORR\n");    
        break;
    }
    NEXT_STATE.REGS[rd] = op1 | op2;  // no setea flags

}

void execute_LSL() {                                    // QUE HAGO CON EL immr? HAY QUE ROTAR? !!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (DEBUG == 1) {
        printf("execute_LSL\n");
        printf("immr: %x\n", immr);
        printf("imms: %x\n", imm);
        printf("curr: %x\n", curr_instr);
    }
    uint64_t op1 = CURRENT_STATE.REGS[rn];
    NEXT_STATE.REGS[rd] = op1 << (63 - imm);
}

void execute_LSR() {
    if (DEBUG == 1) {
        printf("execute_LSR\n");
        printf("immr: %x\n", immr);
        printf("imms: %x\n", imm);
        printf("curr: %x\n", curr_instr);
    }
    uint64_t op1 = CURRENT_STATE.REGS[rn];
    NEXT_STATE.REGS[rd] = op1 >> immr;
}

void execute_MOVZ() {
    if (DEBUG == 1) {printf("execute_MOVZ\n");}
    NEXT_STATE.REGS[rd] = imm;
}

void execute_STUR() {
    if (DEBUG == 1) {printf("execute_STUR\n");}
    uint64_t base_address = CURRENT_STATE.REGS[rn] + imm;
    aux1 = CURRENT_STATE.REGS[rt];
    aux2 = CURRENT_STATE.REGS[rt] >> 32;
    mem_write_32(base_address, aux1);
    mem_write_32(base_address + 4, aux2);
}

void execute_STURB() {
    if (DEBUG == 1) {printf("execute_STURB\n");}
    uint32_t byte1 = CURRENT_STATE.REGS[rt];
    uint64_t base_address = CURRENT_STATE.REGS[rn] + imm;
    byte1 = byte1 & 0xff; 
    aux1 = mem_read_32(base_address);
    result = (0xffffff00 & aux1) | byte1;
    mem_write_32(base_address, result); 
}

void execute_STURH() {
    if (DEBUG == 1) {printf("execute_STURH\n");}
    uint32_t half_word = CURRENT_STATE.REGS[rt];
    uint64_t base_address = CURRENT_STATE.REGS[rn] + imm;
    half_word = half_word & 0xffff; 
    aux1 = mem_read_32(base_address);
    result = (0xffff0000 & aux1) | half_word;
    mem_write_32(base_address, result); 
}

void execute_LDUR() {
    if (DEBUG == 1) {printf("execute_LDUR\n");}
    uint64_t base_address = CURRENT_STATE.REGS[rn] + imm;
    aux1 = mem_read_32(base_address);
    aux2 = mem_read_32(base_address + 4);
    NEXT_STATE.REGS[rt] = (aux2 << 32) | aux1;
}

void execute_LDURH() {
    if (DEBUG == 1) {printf("execute_LDURH\n");}
    uint64_t base_address = CURRENT_STATE.REGS[rn] + imm;
    aux1 = mem_read_32(base_address);
    NEXT_STATE.REGS[rt] = aux1 & 0x0000ffff;
}

void execute_LDURB() {
    if (DEBUG == 1) {printf("execute_LDURB\n");}
    uint64_t base_address = CURRENT_STATE.REGS[rn] + imm;
    aux1 = mem_read_32(base_address);
    NEXT_STATE.REGS[rt] = aux1 & 0x000000ff;
}

void execute_B() {
    if (DEBUG == 1) {printf("execute_B\n");}
    NEXT_STATE.PC += imm26;
}

void execute_BR() {
    if (DEBUG == 1) {printf("execute_BR\n");}
    NEXT_STATE.PC = CURRENT_STATE.REGS[rn];
}

void execute_BEQ() {
    if (DEBUG == 1) {printf("execute_BEQ\n");}
    if (CURRENT_STATE.FLAG_Z == 1) {
        NEXT_STATE.PC += imm26;
    }
}

void execute_BNE() {
    if (DEBUG == 1) {printf("execute_BNE\n");}
    if (CURRENT_STATE.FLAG_Z == 0) {
        NEXT_STATE.PC += imm26;
    }
}

void execute_BGT() {
    if (DEBUG == 1) {printf("execute_BGT\n");}
    if (CURRENT_STATE.FLAG_Z == 0 && CURRENT_STATE.FLAG_N == 0) {
        NEXT_STATE.PC += imm26;
    }
}

void execute_BLT() {
    if (DEBUG == 1) {printf("execute_BLT\n");}
    if (CURRENT_STATE.FLAG_N != 0) {
        NEXT_STATE.PC += imm26;
    }
}

void execute_BGE() {
    if (DEBUG == 1) {printf("execute_BGE\n");}
    if (CURRENT_STATE.FLAG_N == 0) {
        NEXT_STATE.PC += imm26;
    }
}

void execute_BLE() {
    if (DEBUG == 1) {printf("execute_BLE\n");}
    if (!(CURRENT_STATE.FLAG_Z == 0 && CURRENT_STATE.FLAG_N == 0)) {
        NEXT_STATE.PC += imm26;
    }
}



// GENERAL EXECUTE FUNCTIONS
void set_flags() {
    if (DEBUG == 1) {printf("entra en función set_flags\n");}

    if (result < 0){
        NEXT_STATE.FLAG_N = 1;
    }else{
        NEXT_STATE.FLAG_N = 0;
    }
    if (result == 0){
        NEXT_STATE.FLAG_Z = 1;
    }else{
        NEXT_STATE.FLAG_Z = 0;
    }
}

void execute() {
    if (DEBUG == 1) {printf("execute_funcion\n");}
    uint8_t FLAGS = 0;
    switch (instr_name) {
        case ADDS:
            execute_ADDS_ext(); FLAGS = 1; 
            break;
        case ADDSI:
            execute_ADDS_imm(); FLAGS = 1;
            break;
        case HLT:
            execute_HLT();
            break;
        case SUBS:
            execute_SUBS_ext(); FLAGS = 1;
            break;
        case SUBSI:
            execute_SUBS_imm(); FLAGS = 1;
            break;
        case CMP:
            execute_CMP(); FLAGS = 1;
            break;
        case CMPI:
            execute_CMP_imm(); FLAGS = 1;
            break;
        case ANDS:
            execute_ANDS(); FLAGS = 1;
            break;
        case EOR:
            execute_EOR();
            break;
        case ORR:
            execute_ORR();
            break;
        case LSL:
            execute_LSL();
            break;
        case LSR:
            execute_LSR();
            break;
        case STUR:
            execute_STUR();
            break;
        case STURB:
            execute_STURB();
            break;
        case STURH:
            execute_STURH();
            break;
        case LDUR:
            execute_LDUR();
            break;
        case LDURH:
            execute_LDURH();
            break;
        case LDURB:
            execute_LDURB();
            break;
        case MOVZ:
            execute_MOVZ();
            break;
        case B:
            execute_B();
            break;
        case BR:
            execute_BR();
            break;
        case BEQ:
            execute_BEQ();
            break;
        case BNE:
            execute_BNE();
            break;
        case BGT:
            execute_BGT();
            break;
        case BLT:
            execute_BLT();
            break;
        case BGE:
            execute_BGE();
            break;
        case BLE:
            execute_BLE();
            break;    
        default:
            if (DEBUG == 1) {printf("Error en execute: case not found\n");}
            break;
    }
    if (FLAGS) {set_flags();}
    NEXT_STATE.PC += 4;
}



// ------------------------- DECODE -------------------------

void decode()
{
    if (DEBUG == 1) {
        printf("decode\n");
        printf("curr_instr: %u\n", curr_instr); // imprimo instuccion actual para debug
    }
    
    // 1. identify opcode format:
    
    uint32_t opcode = curr_instr >> 21;              // opcode size: 11 bits

    // HLT
    if (opcode == 0b11010100010) {
        instr_name = HLT;
    }

    //  ADDS EXTENDED
    if (opcode == 0b10101011000) { // quizas es 0b10101011000 segun tp
        if (DEBUG == 1) {printf("ADDS EXTENDED \n opcode: %u\n", opcode);}
        // ADDS (extended register)
        rd = curr_instr & 0x0000001f;   // mask bits 0-4
        rn = curr_instr & 0x000003e0;   // mask bits 5-9
        rn = rn >> 5;
        rm = curr_instr & 0x001f0000;  // mask bits 16-20
        rm = rm >> 16;
        if (DEBUG == 1) {printf("if\n");}
        instr_name = ADDS;
    }

    // SUBS EXTENDED
    if (opcode == 0b11101011000) { // era 0b11101011001
        if (DEBUG == 1) {printf("SUBS EXTENDED \n opcode: %u\n", opcode);}
        // SUBS (extended register)
        rd = curr_instr & 0x0000001f; 
        rn = curr_instr & 0x000003e0;
        rn = rn >> 5;
        rm = curr_instr & 0x001f0000;
        rm = rm >> 16;
        if (DEBUG == 1) {printf("if\n");}
        instr_name = SUBS;
    }

    // CMP
    if (opcode == 0b11101011001) {
        if (DEBUG == 1) {printf("CMP \n opcode: %u\n", opcode);}
        // CMP
        rd = curr_instr & 0x0000001f;   // mask bits 0-4
        rn = curr_instr & 0x000003e0;   // mask bits 5-9
        rn = rn >> 5;
        rm = curr_instr & 0x001f0000;   // mask bits 16-20
        rm = rm >> 16;                  // shift 16 bits 
        instr_name = CMP;
    }

    // ANDS(shifted register)
    if (opcode == 0b11101010000 || opcode == 0b11101010010 || opcode == 0b11101010100 || opcode == 0b11101010110) {
        if (DEBUG == 1) {printf("ANDS \n opcode: %u\n", opcode);}
        // ANDS (shifted register)
        rd = curr_instr & 0x0000001f;   // mask bits 0-4
        rn = curr_instr & 0x000003e0;   // mask bits 5-9
        rn = rn >> 5;
        rm = curr_instr & 0x001f0000;   // mask bits 16-20
        rm = rm >> 16;
        imm = curr_instr & 0x0000fc00;  // mask bits 10-15 (imm6)
        imm = imm >> 10;
        shift = curr_instr & 0x00c00000;  // mask bits 22-23
        shift = shift >> 22;
        instr_name = ANDS;
    }

    // EOR(shifted register)
    if (opcode == 0b11001010000 || opcode == 0b11001010010 || opcode == 0b11001010100 || opcode == 0b11001010110) {
        if (DEBUG == 1) {printf("EOR \n opcode: %u\n", opcode);}
        // EOR (shifted register)
        rd = curr_instr & 0x0000001f;   // mask bits 0-4
        rn = curr_instr & 0x000003e0;   // mask bits 5-9
        rn = rn >> 5;
        rm = curr_instr & 0x001f0000;   // mask bits 16-20
        rm = rm >> 16;
        imm = curr_instr & 0x0000fc00;  // mask bits 10-15 (imm6)
        imm = imm >> 10;
        shift = curr_instr & 0x00c00000;  // mask bits 22-23
        shift = shift >> 22;
        instr_name = EOR;
    }

    // ORR(shifted register)
    if (opcode == 0b10101010000  || opcode == 0b10101010010 || opcode == 0b10101010100 || opcode == 0b10101010110) {
        if (DEBUG == 1) {printf("ORR \n opcode: %u\n", opcode);}
        // OOR (shifted register)
        rd = curr_instr & 0x0000001f;   // mask bits 0-4
        rn = curr_instr & 0x000003e0;   // mask bits 5-9
        rn = rn >> 5;
        rm = curr_instr & 0x001f0000;   // mask bits 16-20
        rm = rm >> 16;
        imm = curr_instr & 0x0000fc00;  // mask bits 10-15 (imm6)
        imm = imm >> 10;
        shift = curr_instr & 0x00c00000;  // mask bits 22-23
        shift = shift >> 22;
        instr_name = ORR;
    }

    // MOVZ
    if (opcode == 0b11010010100) {
        if (DEBUG == 1) {printf("MOVZ \n opcode: %u\n", opcode);}
        rd = curr_instr & 0x0000001f;   // mask bits 0-4
        imm = curr_instr & 0x001fffe0;  // mask bits 5-20
        imm = imm >> 5;
        instr_name = MOVZ;
    }

    // STUR
    if (opcode == 0b11111000000) {
        if (DEBUG == 1) {printf("STUR \n opcode: %u\n", opcode);}
        rt = curr_instr & 0x0000001f;   // mask bits 0-4
        rn = curr_instr & 0x000003e0;   // mask bits 5-9
        rn = rn >> 5;
        imm = curr_instr & 0x001ff000;  // mask bits 12-20
        imm = imm >> 12;
        instr_name = STUR;
    }

    // STURB
    if (opcode == 0b00111000000) {
        if (DEBUG == 1) {printf("STURB \n opcode: %u\n", opcode);}
        rt = curr_instr & 0x0000001f;   // mask bits 0-4
        rn = curr_instr & 0x000003e0;   // mask bits 5-9
        rn = rn >> 5;
        imm = curr_instr & 0x001ff000;  // mask bits 12-20
        imm = imm >> 12;
        instr_name = STURB;
    }

    // STURH
    if (opcode == 0b01111000000) {
        if (DEBUG == 1) {printf("STURH \n opcode: %u\n", opcode);}
        rt = curr_instr & 0x0000001f;   // mask bits 0-4
        rn = curr_instr & 0x000003e0;   // mask bits 5-9
        rn = rn >> 5;
        imm = curr_instr & 0x001ff000;  // mask bits 12-20
        imm = imm >> 12;
        instr_name = STURH;
    }

    // LDUR
    if (opcode == 0b11111000010) {
        if (DEBUG == 1) {printf("LDUR \n opcode: %u\n", opcode);}
        rt = curr_instr & 0x0000001f;   // mask bits 0-4
        rn = curr_instr & 0x000003e0;   // mask bits 5-9
        rn = rn >> 5;
        imm = curr_instr & 0x001ff000;  // mask bits 12-20
        imm = imm >> 12;
        instr_name = LDUR;
    }

    // LDURB
    if (opcode == 0b00111000010) {
        if (DEBUG == 1) {printf("LDURB \n opcode: %u\n", opcode);}
        rt = curr_instr & 0x0000001f;   // mask bits 0-4
        rn = curr_instr & 0x000003e0;   // mask bits 5-9
        rn = rn >> 5;
        imm = curr_instr & 0x001ff000;  // mask bits 12-20
        imm = imm >> 12;
        instr_name = LDURB;
    }

    // LDURH
    if (opcode == 0b01111000010) {
        if (DEBUG == 1) {printf("LDURH \n opcode: %u\n", opcode);}
        rt = curr_instr & 0x0000001f;   // mask bits 0-4
        rn = curr_instr & 0x000003e0;   // mask bits 5-9
        rn = rn >> 5;
        imm = curr_instr & 0x001ff000;  // mask bits 12-20
        imm = imm >> 12;
        instr_name = LDURH;
    }

    // BR
    if (opcode == 0b11010110000) {
        if (DEBUG == 1) {printf("BR \n opcode: %u\n", opcode);}
        rn = curr_instr & 0x000003e0;   // mask bits 5-9
        rn = rn >> 5;
        instr_name = BR;
    }

    opcode = opcode >> 1; // shiftea total 22       // opcode size: 10 bits
    // ADDS IMMEDIATE
    if (opcode == 0b1011000100 || opcode == 0b1011000101) { 
        if (DEBUG == 1) {printf("ADDS IMMEDIATE \n opcode: %u\n", opcode);}

        // ADDS (extended register)
        // mask bits 0-4
        rd = curr_instr & 0x0000001f;
        // mask bits 5-9 bits
        rn = curr_instr & 0x000003e0;
        rn = rn >> 5;
        // mask bits 10-21
        imm = curr_instr & 0x003ffc00; //imm12
        uint32_t shift = opcode & 0b0000000001;
        if (shift == 0b0000000000) {
            imm = imm >> 10;
        } else if (shift == 0b0000000001) {
            // pass
        } 
        instr_name = ADDSI;
    }

    // SUBS IMMEDIATE
    if (opcode == 0b1111000100 || opcode == 0b1111000101) { 
        if (DEBUG == 1) {printf("SUBS IMMEDIATE \n opcode: %u\n", opcode);}
        rd = curr_instr & 0x0000001f;  // mask bits 0-4
        if (rd == 0b11111 && DEBUG == 1) {printf("caso CMPI\n");} 
        rn = curr_instr & 0x000003e0;  // mask bits 5-9 bits
        rn = rn >> 5; 
        imm = curr_instr & 0x003ffc00; // mask bits 10-21 bits (12)
        uint32_t shift = opcode & 0b0000000001;
        if (shift == 0b0000000000) {
            if (DEBUG == 1) {printf("No shift\n");}
            imm = imm >> 10;
        } else if (shift == 0b0000000001) {
            if (DEBUG == 1) {printf("shift\n");}
            imm = imm << 2;
        } 
        instr_name = SUBSI;
    }

    // // CMP IMMEDIATE
    // if (opcode == 0b1111000100 || opcode == 0b1111000101) { 
    //     if (DEBUG == 1) {printf("CMP IMMEDIATE \n opcode: %d\n", opcode);}
    //     rn = curr_instr & 0x000001f0;  // mask bits 5-9 bits
    //     rn = rn >> 5; 
    //     imm = curr_instr & 0x003ffc00; // mask bits 10-21 bits (12)
    //     uint32_t shift = opcode & 0b0000000001;
    //     if (shift == 0b0000000000) {
    //         if (DEBUG == 1) {printf("No shift\n");}
    //         imm = imm >> 10;
    //     } else if (shift == 0b0000000001) {
    //         if (DEBUG == 1) {printf("Shift\n");}
    //         imm = imm << 2;
    //     } 
    //     instr_name = CMPI;
    // }

    // LSL y LSR (immediate)
    if (opcode == 0b1101001101) {
        if (DEBUG == 1) {printf("LSL \n opcode: %u\n", opcode);}
        rd = curr_instr & 0x0000001f;   // mask bits 0-4
        rn = curr_instr & 0x000003e0;   // mask bits 5-9
        rn = rn >> 5;
        imm = curr_instr & 0x0000fc00;  // mask bits 10-15 (imm6)
        imm = imm >> 10;
        immr = curr_instr & 0x003f0000;              // mask bits 16-21
        immr = immr >> 16;
        if (imm == 0b111111) {
            if (DEBUG == 1) {printf("LSL con imm == 0b111111\n Entonces es LSR!!\n");}
            instr_name = LSR;
        }
        else {instr_name = LSL;}
    }

    opcode = opcode >> 2;                             // opcode size: 8 bits
    // B.COND
    if (opcode == 0b01010100) {
        if (DEBUG == 1) {printf("B.COND \n opcode: %u\n", opcode);}
        aux1 = curr_instr & 0x0000000f;   // mask bits 0-3
        imm26 = curr_instr & 0x00ffffe0;  // mask bits 5-23 (imm19 pero uso la variable global definida)
        imm26 = imm26 >> 5;
        if (imm26 >> 18 == 0b1) {         // si es negativo   
            imm26 = imm26 | 0xfffffffffff80000;  // sign extend: hace que siga siendo el mismo numero pero con 64 bits
        }
        imm26 = imm26 << 2;  // shift left 2 bits (lo mismo que multiplicar por 4)

        // sub casos del aux1
        switch (aux1) {
        case 000: // BEQ
            instr_name = BEQ;
            break;
        case 001: // BNE
            instr_name = BNE;
            break;
        case 100: // BGT
            instr_name = BGT;
            break;
        case 011: // BLT
            instr_name = BLT;
            break;
        case 010: // BGE
            instr_name = BGE;
            break;
        case 101: // BLE
            instr_name = BLE;
            break;
        default:
            printf("Error en B.COND: case not found\n");
            break;
        }
    }

    opcode = opcode >> 2;                             // opcode size: 6 bits
    // B
    if (opcode == 0b000101) {
        if (DEBUG == 1) {printf("B \n opcode: %u\n", opcode);}
        imm26 = curr_instr & 0x03ffffff;  // mask bits 0-25
        if (imm26 >> 25 == 0b1) {         // si es negativo   
            imm26 = imm26 | 0xfffffffffc000000;  // sign extend: hace que siga siendo el mismo numero pero con 64 bits
        }
        imm26 = imm26 << 2;  // shift left 2 bits (lo mismo que multiplicar por 4)
        instr_name = B;
    }

}



void process_instruction()
{
    fetch();
    decode();
    execute();
}
