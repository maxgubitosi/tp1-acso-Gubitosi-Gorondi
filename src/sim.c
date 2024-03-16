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

// ------------------------- DECODE -------------------------





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
ddd

}
