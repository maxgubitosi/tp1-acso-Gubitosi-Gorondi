#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"

#define DEBUG 1        // 1 para debuggear

#define ADDS 1
#define ADDSI 2
#define HLT 3
#define SUBS 4
#define SUBSI 5
#define CMP 6
#define CMPI 7
#define ANDS 8
#define EOR 9
#define ORR 10
#define LSL 11
#define LSR 12
#define STUR 13
#define STURB 14
#define STURH 15
#define LDUR 16
#define LDURH 17
#define LDURB 18
#define MOVZ 19


// probar enummerate


uint32_t curr_instr;  // current instruction
void fetch() {
    curr_instr = mem_read_32(CURRENT_STATE.PC);
    printf("fetch\n");
    printf("comentario\n");
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
    11. LSL
    12. LSR
    13. STUR
    14. STURB
    15. STURH
    16. LDUR
    17. LDURH
    18. LDURB
    19. MOVZ
*/



int instr_name=0;
uint32_t rd;
uint32_t rn;
uint32_t rm;
uint32_t imm;
uint32_t shift;
uint64_t result;
uint32_t immr;

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
    result = op1 - op2;
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
    NEXT_STATE.REGS[0b1111] = result;   //chequear esto resetear?????????? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void execute_CMP_imm() {
    if (DEBUG == 1) {printf("execute_CMPI\n");}
    uint64_t op1 = CURRENT_STATE.REGS[rn];
    uint64_t op2 = imm;
    result = op1 - op2;
    NEXT_STATE.REGS[0b1111] = result;
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
            // execute_STUR();
            break;
        case STURB:
            // execute_STURB();
            break;
        case STURH:
            // execute_STURH();
            break;
        case LDUR:
            // execute_LDUR();
            break;
        case LDURH:
            // execute_LDURH();
            break;
        case LDURB:
            // execute_LDURB();
            break;
        case MOVZ:
            execute_MOVZ();
            break;
        
        default:
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
        printf("curr_instr: %d\n", curr_instr); // imprimo instuccion actual para debug
    }
    
    // 1. identify opcode format:
    
    uint32_t opcode = curr_instr >> 21;              // opcode size: 11 bits

    // HLT
    if (opcode == 0b11010100010) {
        instr_name = HLT;
    }

    //  ADDS EXTENDED
    if (opcode == 0b10101011000) { // quizas es 0b10101011000 segun tp
        if (DEBUG == 1) {printf("ADDS EXTENDED \n opcode: %d\n", opcode);}
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
        if (DEBUG == 1) {printf("SUBS EXTENDED \n opcode: %d\n", opcode);}
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
        if (DEBUG == 1) {printf("CMP \n opcode: %d\n", opcode);}
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
        if (DEBUG == 1) {printf("ANDS \n opcode: %d\n", opcode);}
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
        if (DEBUG == 1) {printf("EOR \n opcode: %d\n", opcode);}
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
        if (DEBUG == 1) {printf("ORR \n opcode: %d\n", opcode);}
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
        if (DEBUG == 1) {printf("MOVZ \n opcode: %d\n", opcode);}
        rd = curr_instr & 0x0000001f;   // mask bits 0-4
        imm = curr_instr & 0x001fffe0;  // mask bits 5-20
        imm = imm >> 5;
        instr_name = MOVZ;
    }

    // STUR
    // if (opcode == 0b11111000000) {
    //     if (DEBUG == 1) {printf("STUR \n opcode: %d\n", opcode);}
    //     // STUR
    //     rd = curr_instr & 0x0000001f;   // mask bits 0-4
    //     rn = curr_instr & 0x000003e0;   // mask bits 5-9
    //     rn = rn >> 5;
    //     imm = curr_instr & 0x003ffc00;  // mask bits 10-21
    //     imm = imm >> 10;
    //     instr_name = STUR;
    // }

    opcode = opcode >> 1; // shiftea total 22       // opcode size: 10 bits
    // ADDS IMMEDIATE
    if (opcode == 0b1011000100 || opcode == 0b1011000101) { 
        if (DEBUG == 1) {printf("ADDS IMMEDIATE \n opcode: %d\n", opcode);}

        // ADDS (extended register)
        // mask bits 0-4
        rd = curr_instr & 0x0000000f;
        // mask bits 5-9 bits
        rn = curr_instr & 0x000001f0;
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
        if (DEBUG == 1) {printf("SUBS IMMEDIATE \n opcode: %d\n", opcode);}
        rd = curr_instr & 0x0000000f;  // mask bits 0-4
        rn = curr_instr & 0x000001f0;  // mask bits 5-9 bits
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

    // CMP IMMEDIATE
    if (opcode == 0b1111000100 || opcode == 0b1111000101) { 
        if (DEBUG == 1) {printf("CMP IMMEDIATE \n opcode: %d\n", opcode);}
        rn = curr_instr & 0x000001f0;  // mask bits 5-9 bits
        rn = rn >> 5; 
        imm = curr_instr & 0x003ffc00; // mask bits 10-21 bits (12)
        uint32_t shift = opcode & 0b0000000001;
        if (shift == 0b0000000000) {
            if (DEBUG == 1) {printf("No shift\n");}
            imm = imm >> 10;
        } else if (shift == 0b0000000001) {
            if (DEBUG == 1) {printf("Shift\n");}
            imm = imm << 2;
        } 
        instr_name = CMPI;
    }

    // LSL y LSR (immediate)
    if (opcode == 0b1101001101) {
        if (DEBUG == 1) {printf("LSL \n opcode: %d\n", opcode);}
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

    // // case CB:  
    // opcode = curr_instr >> 24;

    // // B.COND (pag 549)    
    // if (opcode == 0b01010100) {
    //     // execute_B_COND();
    // } // chequear sub casos (BEQ, BNE, BGT, BLT, BGE, BLE) mirar desp del opcode

    // // case I:
    // opcode = curr_instr >> 22;

    // // CMP
    // if (opcode == 0b1101000100) {
    //     // execute_CMP();
    // }


    // LSL

    // LSR

    // case I:
    // case D:
    // case R:
    // case IW:
}



void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. 
     * 
     * Sugerencia: hagan una funcion para decode()
     *             y otra para execute()
     * 
     * */
    fetch();
    decode();
    execute();

}
