#include "calculate.h"

/* 
    計算系の命令は計算
    じゃないのは抜粋して渡す
*/
EX_MEM caluculate(ID_EX idExReg) {
    EX_MEM res;
    uint32_t alu = idExReg.alu;
    uint32_t data_rs = idExReg.data_rs;
    uint32_t data_rt = idExReg.data_rt;
    uint32_t shamt = idExReg.shamt;
    uint32_t w_reg = idExReg.w_reg;
    uint32_t imm = idExReg.imm;

    res.zero = 2; //初期化 
    res.result_address = -1; //初期化
    res.result = -1; //初期化
    res.w_reg = -1; //初期化
    res.mem_op = 0; //初期化

    switch(alu) {
        case 0x20: // add
            res.result = data_rs + data_rt;
            res.w_reg = w_reg;
            printf("add: %u + %u = %u\n", data_rs, data_rt, res.result);
            break;
        case 0x00: // sll
            res.result = data_rt << shamt;
            res.w_reg = w_reg;
            printf("sll: %u << %u = %u\n", data_rt, shamt, res.result);
            break;
        case 0x27: // nor
            res.result = ~(data_rs | data_rt);
            res.w_reg = w_reg;
            printf("nor: ~(%u | %u) = %u\n", data_rs, data_rt, res.result);
            break;
        case 0x2A: // slt
            res.result = (data_rs < data_rt) ? 1 : 0;
            res.w_reg = w_reg;
            printf("slt: %u < %u = %u\n", data_rs, data_rt, res.result);
            break;
        case 0x0B: // sltiu
            res.result = (data_rs < imm) ? 1 : 0;
            res.w_reg = w_reg;
            printf("sltiu: %u < %u = %u\n", data_rs, imm, res.result);
            break;
        case 0x02: // srl
            res.result = data_rt >> shamt;
            res.w_reg = w_reg;
            printf("srl: %u >> %u = %u\n", data_rt, shamt, res.result);
            break;
        case 0x08: // addi
            res.result = data_rs + imm;
            res.w_reg = w_reg;
            printf("addi: %u + %u = %u\n", data_rs, imm, res.result);
            break;
        /*それ以外の命令はアドレス計算だけどか？*/
        case 0x23: // lw
            res.result_address = data_rs + imm; //アドレス計算
            res.w_reg = w_reg;
            res.mem_op = 1; //メモリ操作の種類をlwに設定
            printf("lw: アドレス計算 %u + %u = %u\n", data_rs, imm, res.result_address);
            break;
        case 0x2B: // sw
            res.result_address = data_rs + imm; //アドレス計算
            res.w_reg = w_reg;
            res.mem_op = 2; //メモリ操作の種類をswに設定
            printf("sw: アドレス計算 %u + %u = %u\n", data_rs, imm, res.result_address);
            break;
        case 0x04: // beq
            data_rt = Reg[w_reg];
            res.result_address = PC + imm*4 + 4; //アドレス計算
            res.zero = (data_rs - data_rt) == 0 ? 1 : 0; //ゼロ判定
            printf("beq: %u - %u = %u\n", data_rs, data_rt, data_rs - data_rt);
            break;
    }   

    return res;
}