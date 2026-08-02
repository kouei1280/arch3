#pragma once
#include "../MemoryModule/MemoryModule.h"

/* 型 */
typedef struct {
	/*  members */
	uint32_t pc; // PCの更新後の値(32bit)
	uint32_t inst; // 読みだした命令(32bit)
} IF_ID;

typedef struct {
	/*  members */
	uint32_t imm; // 即値フィールドの値
	uint32_t data_rs; // レジスタに格納されている値
	uint32_t data_rt; // レジスタに格納されている値
	uint32_t shamt; // シフト量
	uint32_t w_reg; // WBにおける書き込み/読み込み先のレジスタ番号(rd)
	uint32_t alu; // 計算の種類
	uint32_t pc; // IF/IDレジスタから引き継いだPCの値
} ID_EX;

typedef struct {
	/*  members */
	uint32_t result; // 算術演算の結果
	uint32_t result_address; // アドレス計算の結果
	//uint32_t data; // レジスタに書く用されている値
	uint32_t w_reg; // WBにおける書き込み/読み込み先のレジスタ番号
	uint32_t zero; //分岐命令のためのゼロ判定の結果
	uint32_t mem_op; //メモリ操作の種類(0:なし, 1:lw, 2:sw, 3:beq, 4: 分岐しない)
//  uint32_t pc; // ID/EXレジスタから日気づいたPCの値
} EX_MEM;

typedef struct {
	/*  members */
	uint32_t data; // メモリから読み出したデータ
	uint32_t result; // 算術演算の結果
	uint32_t result_address; // アドレス計算の結果
	uint32_t w_reg; // WBにおける書き込み先のレジスタ番号
	uint32_t alu; // 計算の種類（R形式: 0, lw: 1, sw: 2, beq: 3）
} MEM_WB;

/* 大域変数 */
extern MEMORY Memory[];

extern uint32_t PC;
extern uint32_t Reg[32];


extern IF_ID IfIdReg;
extern ID_EX IdExReg;
extern EX_MEM ExMemReg;
extern MEM_WB MemWbReg;

/* 関数 */
void IF(void);
void ID(void);
void EX(void);
void MEM(void);
void WB(void);
