#ifndef DW_VS_H
#define DW_VS_H

#include <libgs.h>
#include <libgte.h>

#include <dw/combat.h>
#include <dw/efe.h>
#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/types.h>
#include <dw/version.h>

#define VS_FINISHER_TIM	((char *)0x80052ae0)
#define VS_FINISHER_MODEL	((char *)0x80053800)
#define VS_CONFUSION_MODEL	((char *)0x80054838)
#define VS_STUN_MODEL		((char *)0x80054d00)
#define VS_BUFF_MODEL		((TMDModel *)0x80055328)

typedef struct {
	int16_t hp;
	int16_t mp;
	int16_t offense;
	int16_t defense;
	int16_t speed;
	int16_t brains;
	int16_t discipline;
	char name[14];
	uint8_t digimonId;
	uint8_t moves[3];
	uint8_t unk20[32];
} RegisteredDigimon;

typedef struct {
	uint8_t fighters[2][5];
	uint8_t stage;
	uint8_t battleCount;
} VsBattleSetup;

/* Hack to match overlay .bss */
typedef union {
	uint8_t raw[12];
	int8_t flags[2][5];
} VsRoundFlagsRaw;

typedef struct {
	uint8_t slotIds[40];
	uint8_t selectionState;
	uint8_t listPage;
	uint8_t listPageCount;
	uint8_t slotCount;
	uint8_t detailPage;
	uint8_t unk1;
	uint8_t unk2;
	uint8_t detailScrollDir;
	uint8_t unk4;
	uint8_t unk5;
	uint8_t unk6;
	uint8_t unk7;
	uint8_t selectedSlot;
	uint8_t selectedMask;
	uint8_t unk9;
	uint8_t unk10[24];
	uint8_t lastSlot;
} VsSelectDigimonData;

typedef struct {
	int32_t opcode;
	void (*handler)(void);
} VsEfeSubOpcode;

typedef struct {
	int16_t timer;
	int8_t phase;
	int8_t side;
} CameraChase;

extern RegisteredDigimon *VS_DIGIMON_P1_PTR;
extern RegisteredDigimon *VS_DIGIMON_P2_PTR;
extern Entity *VS_FOCUSED_ENTITY;
extern char VS_STR_WIN[];
extern char VS_STR_LOSE[];
extern uint8_t VS_ARENA_MODEL_COUNTS[4];
extern uint8_t VS_ARENA_TIM_COUNTS[4];
extern SVECTOR VS_INTRO_CAMERA_STAGE1_POS;
extern SVECTOR VS_INTRO_CAMERA_STAGE1_ROT;
extern SVECTOR VS_CAMERA_CHASE_OFFSET;
extern SVECTOR VS_CAMERA_CHASE_ROTATION;
extern SVECTOR MAIN_D_80134A84;
extern SVECTOR MAIN_D_80134A8C;
extern SVECTOR VS_CAMERA_INTRO_OFFSET;
extern SVECTOR VS_CAMERA_INTRO_ROTATION;
extern SVECTOR VS_INTRO_CAMERA_STAGE2_POS;
extern SVECTOR VS_INTRO_CAMERA_STAGE2_ROT;
extern SVECTOR MAIN_D_80134AB4;
extern uint8_t VS_YOUR_CALL_POWER_PRIO[4];
extern uint8_t VS_YOUR_CALL_MP_PRIO[4];
extern uint8_t VS_YOUR_CALL_WIDE_PRIO[4];
extern int8_t MAIN_D_80134AC8[2];
extern uint8_t VS_COMMAND_LABEL_U[5];
extern uint8_t VS_COMMAND_LABEL_W[5];
extern uint8_t VS_FINISHER_SEGMENT_U[8];
extern uint8_t VS_FINISHER_SEGMENT_W[8];
extern uint8_t VS_FINISHER_SEGMENT_X[8];
extern char *VS_VERSUS_MODEL_PATH;
extern int16_t VS_VERSUS_MODEL_TARGET_X[4];
extern char VS_FMT_D[4];
extern int8_t VS_LINE_OFFSET_X[4];
extern int8_t VS_LINE_OFFSET_Y[4];
extern int8_t VS_WIREFRAME_BOX_SIGN_X[8];
extern int8_t VS_WIREFRAME_BOX_SIGN_Y[8];
extern int8_t VS_WIREFRAME_BOX_SIGN_Z[8];
extern int32_t VS_RADIAL_WAVE_COLOR;
extern uint8_t VS_RIBBON_UVS[8];
extern int8_t VS_POISON_BUBBLE_FRAME_U[6];
extern SVECTOR VS_STUN_FX_ROTATION;
extern int16_t VS_FINISHER_AURA_OBJECTS[3];
extern SVECTOR VS_FINISHER_AURA_ROTATION;
extern RGB8 VS_FINISHER_AURA_COLOR;
extern SVECTOR VS_AURA_PROJECTILE_VERTEX_0;
extern SVECTOR VS_AURA_PROJECTILE_VERTEX_1;
extern SVECTOR VS_AURA_PROJECTILE_VERTEX_2;
extern SVECTOR VS_AURA_PROJECTILE_VERTEX_3;
extern uint16_t VS_FONT_CHARS[];
extern int16_t VS_FONT_GLYPHS[];
extern uint8_t VS__INTRO_DIGIMON_NAMES[][14];

extern VsRoundFlagsRaw VS_ROUND_WON;
extern VsRoundFlagsRaw VS_ROUND_LOST;
extern VsBattleSetup VS_BATTLE_SETUP;
extern GsOT_TAG VS_D_800716B4[];
extern GsOT_TAG VS_D_800716C4[];
extern GsOT VS_D_800716D4[];
extern GsOT VS_D_800716FC[];
extern GsOT_TAG VS_D_80071724[];
extern GsOT_TAG VS_D_80071734[];
extern VECTOR VS_INTRO_TARGET_POS;
extern VECTOR VS_CAMERA_CHASE_LAST_POS;
extern GsOT VS_MODEL_SCENE_ORDERING_TABLE[];
extern GsOT_TAG VS_MODEL_SCENE_OT_TAGS_0[];
extern GsOT_TAG VS_MODEL_SCENE_OT_TAGS_1[];
extern GsCOORDINATE2Raw VS_ARENA_COORDS[];
extern GsDOBJ2 VS_ARENA_OBJECTS[];
extern int16_t VS_D_80071A0C[];
extern Entity *VS_D_80071A18[];
extern int32_t VS_D_80071A30[4];
extern int32_t VS_D_80071A40[];
extern int32_t VS_D_80071A88[22];
extern int32_t VS_D_80071AE0[4];
extern int32_t VS_D_80071AF0[];
extern uint8_t VS_BATTLE_START_TEXT_PIECES[155][20];
extern PositionDataRaw VS_VERSUS_MODEL_OBJECTS[4];
extern int16_t VS_RESULT_ROTATION_X[];
extern int16_t VS_RESULT_ROTATION_Y[];
extern int16_t VS_D_800729CC[];
extern int16_t VS_D_800729F8[];
extern int16_t VS_RESULT_LAST_X[];
extern PositionDataRaw VS_RESULT_MODEL_OBJECTS[7];
extern int16_t VS_RESULT_MODEL_STEPS[10];
extern void (*VS_EFE_SUB_OPCODE_HANDLERS[])(void);
extern MATRIXRaw VS_SAVED_WS_MATRIX;
extern EfePoisonBubble VS_POISON_BUBBLES[12];
extern EfePoison VS_POISON_EFFECTS[4];
extern EfeConfusion VS_CONFUSION_EFFECTS[4];
extern EfeStun VS_STUN_EFFECTS[5];
extern EfeStunSpark VS_STUN_SUB_EFFECTS[25];
extern EfeFinisherAura VS_FINISHER_AURAS[2];
extern SVECTOR VS_FINISHER_AURA_SPARKS[];

void VS__initialize(RegisteredDigimon *fightersP1, RegisteredDigimon *fightersP2);
#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
void VS_playDemo(void);
void VS_initializeTrialBattle(RegisteredDigimon *fightersP1, RegisteredDigimon *fightersP2);
#endif
int32_t VS_addAuraProjectile(Entity *e);
void VS_addCommandMenu(uint8_t index);
void VS_addFighterCounter(int32_t arg);
void VS_addFighterStatusBars(int32_t id);
int32_t VS_addFinisherAura(Entity *entity, int32_t duration);
void VS_addFinisherProgress(FighterData *fighter, int16_t amount);
void VS_addTargetCursor(/* int16_t id, int32_t tech */);
void VS_addVersusModelScene(void);
void VS_applyChargeRequirement(DigimonEntity *digimon, FighterData *fighter, int16_t tech);
void VS_applyMoveResult(void);
int32_t VS_getDistanceSquared(Entity *a, Entity *b);
void VS_initializeBattleStartText(void);
void VS_initializeBattleStartTextBurst(void);
char *VS_initializeEFEEngine(char *base);
void VS_initializeVS(void);
int32_t VS_isBattleStartTextFinished(void);
int32_t VS_isMoveUsable(DigimonEntity *digimon, FighterData *fighter, int16_t slot);
int32_t VS_isVersusModelSceneFinished(void);
void VS_loadMoveEFE(int16_t *moves, int16_t *effectIds, int8_t *isLoaded);
void VS_loadVersusSceneModel(void);
void VS_playMoveEffect(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void VS_queueRandomMove(DigimonEntity *digimon, FighterData *fighter, int32_t tech);
void VS_removeAllAuraProjectiles(void);
void VS_removeAllFinisherAuras(void);
void VS_removeAllPoisonEffects(void);
void VS_removeAllStunEffects(void);
void VS_removeBattleStartText(void);
void VS_removeBattleStartTextBurst(void);
void VS_removeCommandMenu(int32_t i);
void VS_removeEFEEngine(void);
void VS_removeFighterCounter(void);
void VS_removeFighterStatusBars(int32_t i);
void VS_removeMoveEffect(DigimonEntity *digimon, FighterData *fighter);
void VS_removeResultModelScene(void);
void VS_removeStatusEffects(DigimonEntity *digimon, FighterData *fighter);
void VS_removeVersusModelScene(void);
void VS_resetFighterAction(FighterData *fighter);
void VS_resolveAttack(void);
int32_t VS_selectMoveByMpCost(/* int32_t arg0, int16_t *flags */);
int32_t VS_selectMoveByPower(/* int32_t arg0, int16_t *flags */);
int32_t VS_selectMoveTarget(Entity *entity, FighterData *fighter);
void VS_selectPartnerMove(DigimonEntity *digimon, FighterData *fighter, int16_t index);
void VS_selectRandomCamera(DigimonEntity *entity, int32_t mode, int32_t sub);
void VS_setRandomViewpoint(Entity *entity, int32_t idx);
void VS_setupQueuedMove(DigimonEntity *digimon, FighterData *fighter, int16_t arg2, uint8_t moveIndex);
void VS_startCameraChase(Entity *entity, int32_t dx, int32_t side);
void VS_startFighterMove(DigimonEntity *digimon, DigimonEntity *target, FighterData *fighter);
void VS_tickFrame(void);
void VS_unloadAllEFESlots(void);

#endif
