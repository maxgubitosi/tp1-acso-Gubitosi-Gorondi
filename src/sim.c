#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"

#define DEBUG 1        // 1 para debuggear

#define ADDS 1
#define ADDSI 2
#define HLT 3
#define SUBS 4
# define SUBSI 5


// probar enummerate


uint32_t curr_instr;  // current instruction
void fetch() {
    curr_instr = mem_read_32(CURRENT_STATE.PC);
    printf("fetch\n");
    printf("comentario\n");
}

// INSTRUCCIONES A IMPLEMENTAR:
/*
    1. ADDS
    2. SUBS
    3. HLT
    4. CMP
    5. ANDS
    6. EOR
    7. ORR
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
uint64_t result;

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

void execute_ADDS_ext() {
    if (DEBUG == 1) {printf("execute_ADDS\n");}
    int64_t op1 = CURRENT_STATE.REGS[rn];
    int64_t op2 = CURRENT_STATE.REGS[rm];
    result = op1 + op2;
    NEXT_STATE.REGS[rd] = result; 
}

void execute_SUBS_ext() {
    if (DEBUG == 1) {printf("execute_SUBS\n");}
    int64_t op1 = CURRENT_STATE.REGS[rn];
    int64_t op2 = CURRENT_STATE.REGS[rm];
    result = op1 - op2;
    NEXT_STATE.REGS[rd] = result; 
}
void execute_ADDS_imm() {
    if (DEBUG == 1) {printf("execute_ADDS\n");}
    int64_t op1 = CURRENT_STATE.REGS[rn];
    int64_t op2 = imm;
    result = op1 + op2;
    NEXT_STATE.REGS[rd] = result; 
}

void execute_SUBS_imm() {
    if (DEBUG == 1) {printf("execute_SUBSI\n");}
    int64_t op1 = CURRENT_STATE.REGS[rn];
    int64_t op2 = imm;
    result = op1 - op2;
    NEXT_STATE.REGS[rd] = result;
}

void execute_HLT() {
    if (DEBUG == 1) {printf("execute_HLT\n");}
    RUN_BIT = FALSE;
}

void set_flags() {
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
    if (DEBUG == 1) {printf("execute\n");}
        switch (instr_name) {
            case ADDSI:
                execute_ADDS_imm();
                break;
            case ADDS:
                execute_ADDS_ext();
                break;
            case HLT:
                execute_HLT();
                break;
            case SUBS:
                execute_SUBS_ext();
                break;
            case SUBSI:
                execute_SUBS_imm();
                break;

            default:
                break;
            
        set_flags();

    }
    NEXT_STATE.PC += 4;
}

// ------------------------- DECODE -------------------------
void decode()
{
    fetch();

    if (DEBUG == 1) {
        printf("decode\n");
        printf("curr_instr: %d\n", curr_instr); // imprimo instuccion actual para debug
    }
    
    // 1. identify opcode format:
    
    // case B: B or BR
    uint32_t opcode = curr_instr >> 26;
    
    opcode = curr_instr >> 21;
    // HLT
    if (opcode == 0b11010100010) {
        instr_name = HLT;
    }
    //  ADDS EXTENDED
    if (DEBUG == 1) {printf("opcode: %d\n", opcode);}
    if (opcode == 0b10101011000) { // quizas es 0b10101011000 segun tp
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
    if (DEBUG == 1) {printf("opcode: %d\n", opcode);}
    if (opcode == 0b11101011001) { 
        // ADDS (extended register)
        rd = curr_instr & 0x0000001f; 
        rn = curr_instr & 0x000003e0;
        rn = rn >> 5;
        rm = curr_instr & 0x001f0000;
        rm = rm >> 16;
        if (DEBUG == 1) {printf("if\n");}
        instr_name = SUBS;
    }

    opcode = opcode >> 1; // shiftea total 22
    // ADDS IMMEDIATE
    if (DEBUG == 1) {printf("opcode: %d\n", opcode);}
    if (opcode == 0b1011000100 || opcode == 0b1011000101) { 
        if (DEBUG == 1) {printf("if_ADDS_IMMEDIATE\n");}
        // ADDS (extended register)
        // mask last 4 bits
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
    if (DEBUG == 1) {printf("opcode: %d\n", opcode);}
    if (opcode == 0b1111000100 || opcode == 0b1111000101) { 
        if (DEBUG == 1) {printf("if_SUBS_IMMEDIATE\n");}
        rd = curr_instr & 0x0000000f;  // mask last 4 bits
        rn = curr_instr & 0x000001f0;  // mask bits 5-9 bits
        rn = rn >> 5; 
        imm = curr_instr & 0x003ffc00; // mask bits 16-21 bits (12)
        uint32_t shift = opcode & 0b0000000001;
        if (shift == 0b0000000000) {
            imm = imm >> 10;
        } else if (shift == 0b0000000001) {
            // pass
        } 
        instr_name = SUBSI;
    }

    // SUBS IMMEDIATE


    // // B (pag 550) 
    // if (opcode == 0b000101) {
    //     // execute_B();
    // }
    // // BR (pag 562)
    // else if (opcode == 0b110101) {
    //     // execute_BR();
    // }

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
    decode();
    execute();

}
