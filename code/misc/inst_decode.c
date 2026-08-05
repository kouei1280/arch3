// 命令の解釈
# include "inst_decode.h"

ID_EX R(Instruction instraction) {
    ID_EX res = {0};
    switch(instraction.funct) {
        case 0x20: // add
            res.data_rs = Reg[instraction.rs];
            res.data_rt = Reg[instraction.rt];
            res.w_reg = instraction.rd;
            res.alu = 0x20; // +
            break;
        case 0x00: // sll
            res.data_rt= Reg[instraction.rt];
            res.w_reg = instraction.rd;
            res.alu = 0x00; // <<
            res.shamt = instraction.shamt;
            break;
        case 0x27: // nor
            res.data_rs = Reg[instraction.rs];
            res.data_rt = Reg[instraction.rt];
            res.w_reg = instraction.rd;
            res.alu = 0x27; // nor
            break;
        case 0x2A: // slt
            res.data_rs = Reg[instraction.rs];
            res.data_rt = Reg[instraction.rt];
            res.w_reg = instraction.rd;
            res.alu = 0x2A; // <
            break;
        case 0x02: // srl
            res.data_rt= Reg[instraction.rt];
            res.w_reg = instraction.rd;
            res.alu = 0x02; // >>
            res.shamt = instraction.shamt;
            break;
    }
    return res;
}

ID_EX I(Instruction instraction, uint32_t funct) {
    ID_EX res = {0};
    res.data_rs = Reg[instraction.rs];
    res.w_reg = instraction.rt;
    res.alu = funct; // 0x08 + (即値), 0x23 lw, 0x2B sw, 0x04 beq
    res.imm = instraction.imm;
    return res;
}

ID_EX J(Instruction instraction, uint32_t alu) {
    ID_EX res = {0};
    res.alu = alu; // 0x11 j
    PC -= instraction.addr * 4 + 4; // PCの更新
    return res;
}

ID_EX inst_decode(uint32_t inst){
    Instruction instraction;
    instraction.opcode = (inst >> 26) & 0x3F;
    instraction.rs = (inst >> 21) & 0x1F;
    instraction.rt = (inst >> 16) & 0x1F;
    instraction.rd = (inst >> 11) & 0x1F;
    instraction.shamt = (inst >> 6) & 0x1F;
    instraction.funct = inst & 0x3F;
    instraction.imm = (int16_t)(inst & 0xFFFF);
    instraction.addr = inst & 0x3FFFFF;
    
    ID_EX res = {0};
    uint32_t funct;
    res.is_break = 0; // break命令のフラグを初期化
    if (inst == 0x0000000C){
        res.is_break = 1; // break命令のフラグを立てる
        return res;
    }

    switch(instraction.opcode) {
        case 0x00: // R形式(add, sll, nor, slt, srl)
            res = R(instraction);
            break; 
        case 0x08: // addi I
            funct=0x08;
            res = I(instraction, funct);
            break;
        case 0x23: // lw I
            funct=0x23;
            res = I(instraction, funct);
            break;
        case 0x2B: // sw I
            funct=0x2B;
            res = I(instraction, funct);
            break;
        case 0x0B: // sltiu I
            funct=0x0B;
            res = I(instraction, funct);
            break;
        case 0x04: // beq I
            funct=0x04;
            res = I(instraction, funct);
            break;
        case 0x02: // j J
            printf("j命令を実行します\n");
            funct = 0x11; // srlとalu値が被るため別の値に変更
            res = J(instraction, funct);
            break;
    }

    return res;
}