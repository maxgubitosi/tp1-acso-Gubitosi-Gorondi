#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "shell.h"

uint32_t curr_instr;  // current instruction
void fetch() {
    curr_instr = mem_read_32(CURRENT_STATE.PC);
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

uint32_t rd;
uint32_t rn;
uint32_t rm;

// ------------------------- DECODE -------------------------
void decode()
{
    fetch();

    // 1. identify opcode format:
    
    // case B: B or BR
    uint32_t opcode = curr_instr >> 26;
    
    opcode = curr_instr >> 21;
    if (curr_instr==0b10101011001) { // quizas es 0b10101011000 segun tp
        // ADDS (extended register)
        rd = curr_instr & 0x0000001f; 
        rn = curr_instr & 0x000003e0;
        rn = rn >> 5;
        // uint32_t shamt = curr_instr & 0x0000fc00;
        // shamt = shamt >> 10;
        rm = curr_instr & 0x001f0000;
        rm = rm >> 16;
        execute_ADDS();
    }

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

void execute_ADDS() {
    int64_t op1 = CURRENT_STATE.REGS[rn];
    int64_t op2 = CURRENT_STATE.REGS[rm];
    // exec_ALU(op1, op2, &N, &Z, &V, &C, &result, instr_name);
    int64_t result = op1 + op2;
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

    NEXT_STATE.REGS[rd] = result; 
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

}
