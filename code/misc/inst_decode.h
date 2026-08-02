# include <stdint.h>
# include "mips.h"

extern uint32_t Reg[32];

typedef struct {
    uint32_t opcode;
    uint32_t rs;
    uint32_t rt;
    uint32_t rd;
    uint32_t shamt;
    uint32_t funct;
    uint32_t imm;
    uint32_t addr;
} Instruction;
/*
typedef struct {
    uint32_t imm;
    uint32_t data_rs;
    uint32_t data_rt;
    uint32_t shamt;
    uint32_t w_reg;
    uint32_t alu; // R(0~4), I(5~)
    uint32_t pc;
}Res;
*/
ID_EX R(Instruction instraction);
ID_EX I(Instruction instraction, uint32_t alu);
ID_EX inst_decode(uint32_t inst);