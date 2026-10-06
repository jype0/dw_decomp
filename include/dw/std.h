#ifndef DW_STD_H
#define DW_STD_H

#include <libgs.h>
#include <libgte.h>

#include <dw/efe.h>
#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/types.h>

typedef struct {
	uint8_t modelCount[2];
	uint8_t timCount[2];
} StdArenaCfg;

typedef struct {
	int16_t unk0[8];
	uint8_t unk10[8];
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

extern int16_t STD_LOSE_DROP_Y[];
extern char STD_PATH_STDDAT_DRAW_TMD[];
extern MATRIX STD_MODEL_SCENE_MATRIX;

extern GsOT_TAG STD_D_8007B664[];
extern GsOT_TAG STD_D_8007B674[];
extern GsOT STD_D_8007B684[];
extern GsOT STD_D_8007B6AC[];
extern GsOT_TAG STD_D_8007B6D4[];
extern GsOT_TAG STD_D_8007B6E4[];
extern VECTOR STD_INTRO_TARGET_POS;
extern VECTOR STD_CAMERA_CHASE_LAST_POS;
extern GsOT STD_MODEL_SCENE_ORDERING_TABLE[];
extern GsOT_TAG STD_MODEL_SCENE_OT_TAGS_0[];
extern GsOT_TAG STD_MODEL_SCENE_OT_TAGS_1[];
extern GsCOORDINATE2Raw STD_ARENA_COORDS[];
extern GsDOBJ2 STD_ARENA_OBJECTS[];
extern int16_t STD_D_8007B9BC[];
extern StdUnkB9D0 STD_D_8007B9D0;
extern int32_t STD_D_8007B9EC[4];
extern int32_t STD_D_8007B9FC[];
extern int32_t STD_D_8007BA44[22];
extern int32_t STD_D_8007BA9C[4];
extern int32_t STD_D_8007BAAC[];
extern StdUnkBAF4 STD_BRACKET_SLOTS[8];
extern StdUnkBB64 STD_BRACKET_PROJECTILES[8];
extern uint8_t STD_BATTLE_START_TEXT_PIECES[155][20];
extern PositionDataRaw STD_VERSUS_MODEL_OBJECTS[9];
extern int16_t STD_WIN_ROTATION_X[];
extern int16_t STD_WIN_ROTATION_Y[];
extern int16_t STD_WIN_FIRST_X[];
extern int16_t STD_WIN_LAST_X[];
extern char STD_RESULT_TMD_BUFFER[];
extern PositionDataRaw STD_RESULT_MODEL_OBJECTS[8];
extern int16_t STD_CHAMPION_ORBIT_ANGLES[];
extern int16_t STD_MODEL_SCENE_STEPS[];
extern int16_t STD_LOSE_WIPE_HEIGHTS[];
extern uint8_t STD_LOSE_WIPE_ORDER[];
extern void (*STD_EFE_SUB_OPCODE_HANDLERS[])(void);
extern MATRIXRaw STD_SAVED_WS_MATRIX;
extern EfePoisonBubble STD_POISON_BUBBLES[12];
extern EfePoison STD_POISON_EFFECTS[4];
extern EfeConfusion STD_CONFUSION_EFFECTS[4];
extern EfeStun STD_STUN_EFFECTS[5];
extern EfeStunSpark STD_STUN_SUB_EFFECTS[25];
extern EfeFinisherAura STD_FINISHER_AURAS[2];
extern SVECTOR STD_FINISHER_AURA_SPARKS[];

extern StdArenaCfg STD_ARENA_COUNTS;
extern uint8_t STD_INTRO_NAME_CHAR_SIZES[4];
extern uint8_t STD_INTRO_NAME_CHAR_OFFSETS[4];
extern char STD_STR_HP[5];
extern char STD_STR_MP[5];
extern SVECTOR STD_INTRO_CAMERA_STAGE1_POS;
extern SVECTOR STD_INTRO_CAMERA_STAGE1_ROT;
extern SVECTOR STD_CAMERA_CHASE_OFFSET;
extern SVECTOR STD_CAMERA_CHASE_ROTATION;
extern SVECTOR MAIN_D_80134838;
extern SVECTOR MAIN_D_80134840;
extern SVECTOR STD_CAMERA_INTRO_OFFSET;
extern SVECTOR STD_CAMERA_INTRO_ROTATION;
extern SVECTOR STD_INTRO_CAMERA_STAGE2_POS;
extern SVECTOR STD_INTRO_CAMERA_STAGE2_ROT;
extern SVECTOR MAIN_D_80134868;
extern int16_t STD_CARDINAL_ROTATIONS[4];
extern char STD_STR_ATAETA[7];
extern uint8_t STD_BRAIN_TO_COMMAND_MAP[5];
extern uint8_t STD_YOUR_CALL_POWER_PRIO[4];
extern uint8_t STD_YOUR_CALL_MP_PRIO[4];
extern uint8_t STD_YOUR_CALL_WIDE_PRIO[4];
extern uint8_t STD_COMMAND_LABEL_U[5];
extern uint8_t STD_COMMAND_LABEL_W[5];
extern uint8_t STD_FINISHER_SEGMENT_U[8];
extern uint8_t STD_FINISHER_SEGMENT_W[8];
extern uint8_t STD_FINISHER_SEGMENT_X[8];
extern char *STD_VERSUS_MODEL_PATH;
extern int16_t STD_VERSUS_MODEL_TARGET_X[4];
extern char STD_FMT_D[4];
extern int8_t STD_LINE_OFFSET_X[4];
extern int8_t STD_LINE_OFFSET_Y[4];
extern int8_t STD_WIREFRAME_BOX_SIGN_X[8];
extern int8_t STD_WIREFRAME_BOX_SIGN_Y[8];
extern int8_t STD_WIREFRAME_BOX_SIGN_Z[8];
extern int32_t STD_RADIAL_WAVE_COLOR;
extern uint8_t STD_RIBBON_UVS[8];
extern int8_t STD_POISON_BUBBLE_FRAME_U[6];
extern SVECTOR STD_STUN_FX_ROTATION;
extern int16_t STD_FINISHER_AURA_OBJECTS[3];
extern SVECTOR STD_FINISHER_AURA_ROTATION;
extern RGB8 STD_FINISHER_AURA_COLOR;
extern SVECTOR STD_AURA_PROJECTILE_VERTEX_0;
extern SVECTOR STD_AURA_PROJECTILE_VERTEX_1;
extern SVECTOR STD_AURA_PROJECTILE_VERTEX_2;
extern SVECTOR STD_AURA_PROJECTILE_VERTEX_3;

void STD_tickNPCTournament(int32_t instanceId);
void STD_tickPartnerTournament(int32_t instanceId);
void STD_tickTamerTournament(int32_t instanceId);

#endif
