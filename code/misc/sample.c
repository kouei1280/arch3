# include "mips.h"
# include "inst_decode.h"
# include "calculate.h"

/* メモリ */
# define MEMORY_SIZE	100
MEMORY Memory[MEMORY_SIZE];

/* レジスタ */
uint32_t PC;
uint32_t Reg[32];

IF_ID IfIdReg;
ID_EX IdExReg;
EX_MEM ExMemReg;
MEM_WB MemWbReg;

/* ステージ */
void IF(void) {
	/* body */
    uint32_t inst;
    inst = MMreadWord(Memory, PC);
    IfIdReg.inst = inst;
    PC += 4;
    IfIdReg.pc = PC;
}

void ID(void) {
	/* body */
    uint32_t inst;
    inst = IfIdReg.inst;

    /* 命令の解釈 */
    IdExReg = inst_decode(inst);
    IdExReg.pc = PC;
}

void EX(void) {
    /* body */
    ExMemReg = caluculate(IdExReg);
    //ExMemReg.pc = PC;
    ExMemReg.is_break = IdExReg.is_break; // break命令のフラグを引き継ぐ
}

void MEM(void) {
	/* body */
    uint32_t address;
    address = ExMemReg.result_address;
    MemWbReg.w_reg = ExMemReg.w_reg;
    MemWbReg.result_address = address;
    MemWbReg.alu = 0; //初期化
    MemWbReg.is_break = ExMemReg.is_break; // break命令のフラグを引き継ぐ

    if (address != -1) { // I形式
        if (ExMemReg.zero == 2) {
            if (ExMemReg.mem_op == 1) { // lw
                MemWbReg.data = MMreadWord(Memory, address);
                MemWbReg.alu = 1; // lw
            } else if (ExMemReg.mem_op == 2) { // sw
                MMwriteWord(Memory, address, Reg[ExMemReg.w_reg]);
                MemWbReg.alu = 2; // sw
                printf("Reg[%d]の値%uをMemory[%u]に書き込む\n", ExMemReg.w_reg, Reg[ExMemReg.w_reg], address);
            }
        }else if (ExMemReg.zero == 1) { // beq 分岐
            PC = address;
            MemWbReg.result_address = PC;
            MemWbReg.alu = 3; // beq
        }else{
            MemWbReg.alu = 4; // 分岐しない
        }
    } else { // R形式
        MemWbReg.result = ExMemReg.result;
    }
}

void WB(void) {
	/* body */
    uint32_t data, result, result_address, w_reg, alu;
    //なにこれdata = MemWbReg.data;
    result = MemWbReg.result;
    result_address = MemWbReg.result_address;
    w_reg = MemWbReg.w_reg;
    alu = MemWbReg.alu;
    if (result_address == PC) {
        printf("分岐命令:PCを%uに更新した\n", result_address);
    }else {
        if (alu == 0) { // R形式
            printf("Reg[%d]に%uを書き込む\n", w_reg, result);
            Reg[w_reg] = result;
        } else if (alu == 1) { // lw
            printf("Reg[%d]に%uを書き込む\n", w_reg, MemWbReg.data);
            Reg[w_reg] = MemWbReg.data;
        } else if (alu == 4) {
            printf("分岐せずに続けます\n");
        }
    }
}

static uint32_t *load_program(const char *path, size_t *out_count) {
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        fprintf(stderr, "ファイルを開けません: %s\n", path);
        exit(EXIT_FAILURE);
    }

    size_t cap = 64;
    size_t count = 0;
    uint32_t *inst = malloc(cap * sizeof(*inst));
    if (inst == NULL) {
        fprintf(stderr, "メモリを確保できません\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    unsigned char bytes[4];
    while (fread(bytes, 1, sizeof(bytes), fp) == sizeof(bytes)) {
        if (count == cap) {
            cap *= 2;
            uint32_t *tmp = realloc(inst, cap * sizeof(*inst));
            if (tmp == NULL) {
                fprintf(stderr, "メモリを拡張できません\n");
                free(inst);
                fclose(fp);
                exit(EXIT_FAILURE);
            }
            inst = tmp;
        }

        uint32_t word =
            ((uint32_t)bytes[0] << 24) |
            ((uint32_t)bytes[1] << 16) |
            ((uint32_t)bytes[2] << 8) |
            (uint32_t)bytes[3];
        inst[count++] = word;
    }

    if (ferror(fp)) {
        fprintf(stderr, "ファイル読み込み中にエラーが発生しました\n");
        free(inst);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    fclose(fp);
    *out_count = count;
    return inst;
}

int main(int argc, char *argv[]) {
    const char *program_path = (argc > 1) ? argv[1] : "../Assembler/Sample_sltiu.bin";
    size_t inst_count = 0;
    uint32_t *inst = load_program(program_path, &inst_count);

    if (inst_count == 0) {
        fprintf(stderr, "命令がありません\n");
        return 1;
    }

    MMloadProgram(Memory, 0, inst, inst_count);
    printf("プログラムをロードしました: %s (%zu命令)\n", program_path, inst_count);
    while (PC < inst_count * 4 - 4) {
        printf("PCの値: %u", PC);
        IF();
        printf("->%u\n", PC);
        ID();
        if (IfIdReg.pc != IdExReg.pc) {
            printf("PC: %u->%uにジャンプします\n", IfIdReg.pc, IdExReg.pc);
            continue;
        }     
        
        EX();
        MEM();
        printf("PCの値: %u, inst_count: %u\n", PC, inst_count * 4);
        //sleep(1);
        WB();

        if (MemWbReg.is_break) {
            printf("break命令を検知しました。プログラムを終了します。\n");
            break;
        }
    }

    free(inst);
    printf("プログラムを終了します\n");
    return 0;
}