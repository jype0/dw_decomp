#ifndef DW_STD_H
#define DW_STD_H

#include <libgs.h>
#include <libgte.h>

#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/graphics.h>
#include <dw/types.h>

typedef struct {
	uint8_t modelCount[2];
	uint8_t timCount[2];
} StdArenaCfg;

typedef struct {
	int16_t unk0;
	uint8_t pad2[0xe];
	uint8_t unk10;
	uint8_t pad11[0x7];
} StdSrcA598;

typedef struct {
	int32_t opcode;
	void (*handler)(void);
} StdEfeSubOpcode;

typedef struct {
	int16_t x;
	int16_t y;
	uint8_t owner;
	uint8_t flag;
} StdUnkBB64;

typedef struct {
	int16_t unk0;
	int16_t unk2;
	Entity *target;
	Entity *entity;
	uint8_t unkc[16];
} StdUnkB9D0;

typedef struct {
	int16_t unk0;
	int16_t unk2;
	uint8_t unk4;
	uint8_t unk5;
	uint8_t unk6;
	uint8_t unk7;
	uint8_t unk8;
	uint8_t unk9;
	uint8_t unkA[2];
	uint8_t unkC;
	uint8_t unkD;
} StdUnkBAF4;

extern uint8_t STD_D_80052AE0[];
extern int16_t STD_D_8007AA40[];
extern char STD_D_8007AAB0[];
extern int32_t STD_D_8007AA10[];

extern GsOT_TAG STD_D_8007B664[];
extern GsOT_TAG STD_D_8007B674[];
extern GsOT STD_D_8007B684[];
extern GsOT STD_D_8007B6AC[];
extern GsOT_TAG STD_D_8007B6D4[];
extern GsOT_TAG STD_D_8007B6E4[];
extern VECTOR STD_D_8007B6F4;
extern VECTOR STD_D_8007B704;
extern GsOT STD_D_8007B714[];
extern GsOT_TAG STD_D_8007B73C[];
extern GsOT_TAG STD_D_8007B7BC[];
extern GsCOORDINATE2Raw STD_D_8007B83C[];
extern GsDOBJ2 STD_D_8007B97C[];
extern int16_t STD_D_8007B9BC[];
extern StdUnkB9D0 STD_D_8007B9D0;
extern int32_t STD_D_8007B9EC[4];
extern int32_t STD_D_8007B9FC[];
extern int32_t STD_D_8007BA44[22];
extern int32_t STD_D_8007BA9C[4];
extern int32_t STD_D_8007BAAC[];
extern StdUnkBAF4 STD_D_8007BAF4[8];
extern StdUnkBB64 STD_D_8007BB64[8];
extern uint8_t STD_D_8007BB94[155][20];
extern PositionDataRaw STD_D_8007C7B0[4];
extern int32_t STD_D_8007CAD0[];
extern int32_t STD_D_8007CB58[];
extern int32_t STD_D_8007CBE0[];
extern int16_t STD_D_8007CC50[];
extern int32_t STD_D_8007CC68[];
extern int16_t STD_D_8007CC78[];
extern int16_t STD_D_8007CCA4[];
extern int16_t STD_D_8007CCD0[];
extern int16_t STD_D_8007CCFC[];
extern char STD_D_8007CD28[];
extern PositionDataRaw STD_D_8007F528[8];
extern int16_t STD_D_8007F968[];
extern int16_t STD_D_8007F9B8[];
extern int16_t STD_D_8007FA08[];
extern int16_t STD_D_8007FA1C[];
extern char STD_D_8007FA5C[];
extern void (*STD_jtbl_8007FA7C[])(void);
extern MATRIXRaw STD_D_8007FC00;
extern int16_t STD_D_8007FC20[12][6];
extern int16_t STD_D_8007FCB0[][4];
extern int16_t STD_D_8007FCD0[][8];
extern int16_t STD_D_8007FD10[][6];
extern int16_t STD_D_8007FD4C[][4];
extern int16_t STD_D_8007FE14[2][4];
extern SVECTOR STD_D_8007FE24[];

extern StdArenaCfg MAIN_D_801347FC;
extern uint8_t MAIN_D_80134800[4];
extern uint8_t MAIN_D_80134804[4];
extern char MAIN_D_80134808[5];
extern char MAIN_D_80134810[5];
extern SVECTOR MAIN_D_80134818;
extern SVECTOR MAIN_D_80134820;
extern SVECTOR MAIN_D_80134828;
extern SVECTOR MAIN_D_80134830;
extern SVECTOR MAIN_D_80134838;
extern SVECTOR MAIN_D_80134840;
extern SVECTOR MAIN_D_80134848;
extern SVECTOR MAIN_D_80134850;
extern SVECTOR MAIN_D_80134858;
extern SVECTOR MAIN_D_80134860;
extern SVECTOR MAIN_D_80134868;
extern int16_t MAIN_D_80134870[4];
extern char MAIN_D_80134878[7];
extern uint8_t MAIN_D_80134880[5];
extern uint8_t MAIN_D_80134888[4];
extern uint8_t MAIN_D_8013488C[4];
extern uint8_t MAIN_D_80134890[4];
extern uint8_t MAIN_D_801348B0[5];
extern uint8_t MAIN_D_801348B8[5];
extern uint8_t MAIN_D_801348C0[8];
extern uint8_t MAIN_D_801348C8[8];
extern uint8_t MAIN_D_801348D0[8];
extern char *MAIN_D_801348D8;
extern int16_t MAIN_D_801348DC[4];
extern char MAIN_D_801348E4[4];
extern int8_t MAIN_D_801348E8[4];
extern int8_t MAIN_D_801348EC[4];
extern int8_t MAIN_D_801348F0[8];
extern int8_t MAIN_D_801348F8[8];
extern int8_t MAIN_D_80134900[8];
extern int32_t MAIN_D_80134908;
extern uint8_t MAIN_D_8013490C[8];
extern int8_t MAIN_D_80134914[6];
extern SVECTOR MAIN_D_8013491C;
extern int16_t MAIN_D_80134924[4];
extern SVECTOR MAIN_D_8013492C;
extern RGB8 MAIN_D_80134934;
extern SVECTOR MAIN_D_80134938;
extern SVECTOR MAIN_D_80134940;
extern SVECTOR MAIN_D_80134948;
extern SVECTOR MAIN_D_80134950;

int16_t STD_func_800579D8(uint8_t *arg);
void STD_tickNPCTournament(int32_t instanceId);
void STD_tickPartnerTournament(int32_t instanceId);
void STD_tickTamerTournament(int32_t instanceId);

#endif
