#ifndef DW_BTL_H
#define DW_BTL_H

#include <libgs.h>
#include <libgte.h>

#include <dw/battle.h>
#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/types.h>

typedef struct {
	int32_t frame;
	int16_t x;
	int16_t y;
	uint8_t width;
} BtlCommandShout;

typedef struct {
	int32_t life;
	SVECTOR p[4];
} EfeTrail;

typedef struct {
	int16_t frame;
	int16_t pad;
	MATRIX *matrix;
	Entity *entity;
	EfeTrail trails[8];
} EfeTrailEffect;

typedef struct {
	int16_t frame;
	int16_t buffId;
	MATRIX *bone;
	SVECTOR rotation[5];
	int16_t unk30[8];
	int32_t scale;
	int32_t scaleTarget;
	int16_t unk48[16];
	RGB8 color;
	uint8_t pad;
} EfeBuffRings;

typedef struct {
	int16_t frame;
	int16_t pad;
	MATRIX *bone;
	Entity *entity;
	SVECTOR rotation;
	int32_t scaleXZ;
	int32_t scaleTargetXZ;
	int32_t scaleY;
	int32_t scaleTargetY;
} EfeBuffDisk;

typedef struct {
	int16_t velocityX;
	int16_t velocityY;
	int16_t velocityZ;
	int16_t fadeStep;
	int32_t life;
	int32_t positionX;
	int32_t positionY;
	int32_t positionZ;
	int16_t brightness;
	int16_t pad;
} BtlItemParticle;

typedef struct {
	int16_t timer;
	int16_t pad;
	BtlItemParticle particles[20];
} BtlItemParticleEffect;

typedef struct {
	int16_t vx;
	int16_t vy;
	int16_t vz;
} BtlParticleVelocity;

typedef struct {
	int16_t x;
	int16_t z;
} BtlParticleDrag;

typedef struct {
	GsSPRITE sprite;
	int16_t step;
	int16_t timer;
} BtlDeathCountdown;

/* Hack to match overlay .bss */
typedef union {
	uint8_t raw[40];
	BtlDeathCountdown data;
} BtlDeathCountdownRaw;

extern int16_t ENEMY_COUNT;
extern int16_t MAIN_D_80134D66;
extern int16_t MAIN_D_80134D68;
extern Entity *MAIN_D_80134D60;
extern int32_t MAIN_D_80134D70;
extern int32_t MAIN_D_80134D74;
extern int32_t MAIN_D_80134D7C[2];
extern uint8_t MAIN_D_80135078;
extern int32_t MAIN_D_8013507C;
extern int32_t MAIN_D_80135080;
extern int8_t MAIN_D_80135094;
extern StatsGains INITIAL_COMBAT_STATS[];
extern int8_t GAME_STATE;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern char *MOVE_NAMES[];
extern int32_t END_TEXT_TYPING;
extern int16_t END_TEXT_ROWS_SHOWN;
extern int16_t END_TEXT_TOTAL_ROWS;
extern int16_t END_TEXT_ROWS_DRAWN;
extern int16_t END_TEXT_VISIBLE_ROWS;
extern int16_t END_TEXT_WAIT_FRAMES;
extern int16_t END_TEXT_WAIT_TIMER;

extern int16_t MAIN_D_801346D8[4];
extern uint8_t MAIN_D_801346E0[5];
extern uint8_t MAIN_D_801346E8[4];
extern uint8_t MAIN_D_801346EC[4];
extern uint8_t MAIN_D_801346F0[4];
extern uint8_t MAIN_D_801346F4[4];
extern int8_t MAIN_D_801346F8;
extern uint8_t MAIN_D_80134728[5];
extern uint8_t MAIN_D_80134730[8];
extern char END_TEXT_YELLOW[4];
extern char END_TEXT_WHITE_WAIT[6];
extern char MP_BONUS_20[3];
extern char MP_BONUS_15[3];
extern char MP_BONUS_10[3];
extern char MP_BONUS_5[2];
extern char MP_BONUS_SUFFIX[5];
extern uint8_t MAIN_D_80134764[8];
extern uint8_t MAIN_D_8013476C[8];
extern uint8_t MAIN_D_80134774[8];
extern char BTL_NUMBER_FORMAT[4];
extern int8_t MAIN_D_80134780[4];
extern int8_t MAIN_D_80134784[4];
extern int8_t MAIN_D_80134788[8];
extern int8_t MAIN_D_80134790[8];
extern int8_t MAIN_D_80134798[8];
extern int32_t MAIN_D_801347A0;
extern uint8_t MAIN_D_801347A4[8];
extern int8_t MAIN_D_801347AC[6];
extern SVECTOR MAIN_D_801347B4;
extern int16_t MAIN_D_801347BC[3];
extern SVECTOR MAIN_D_801347C4;
extern RGB8 MAIN_D_801347CC;
extern SVECTOR MAIN_D_801347D0;
extern SVECTOR MAIN_D_801347D8;
extern SVECTOR MAIN_D_801347E0;
extern SVECTOR MAIN_D_801347E8;
extern uint8_t MAIN_D_801347F0[4];
extern uint8_t MAIN_D_801347F4[4];
extern uint8_t MAIN_D_801347F8[4];

extern const int8_t BTL_SHOUT_HOP_OFFSETS[20];
extern const int8_t BTL_SHOUT_DROP_OFFSETS[20];
extern const uint8_t BTL_D_80072E7C[6][10];
extern const int16_t BTL_D_80072EB8[8];
extern const int16_t BTL_D_80072EC8[8];
extern const int16_t BTL_D_80073014[155][2];
extern const int16_t BTL_D_80073280[8];
extern const int32_t BTL_D_80073290[12];
extern const BarSprite BTL_D_800732C0[6];
extern GsSPRITE BTL_D_80073E54;
extern BtlDeathCountdownRaw BTL_D_80073E78;
extern char BTL_END_BOX_TEXTBUFFER[1024];
extern uint8_t BTL_D_800742A0[155][20];
extern void (*BTL_D_80074EBC[97])(void);
extern int16_t BTL_D_80075040[12][6];
extern int16_t BTL_D_800750D0[4][4];
extern int16_t BTL_D_800750F0[4][8];
extern int16_t BTL_D_80075130[5][6];
extern int16_t BTL_D_8007516C[25][4];
extern int16_t BTL_D_80075234[2][4];
extern SVECTOR BTL_D_80075244[20];
extern BtlParticleVelocity BTL_D_800752E4[20];
extern BtlParticleDrag BTL_D_8007535C[20];
extern BtlItemParticleEffect BTL_D_800753AC[4];
extern EfeBuffDisk BTL_D_80075C7C[1];
extern EfeTrailEffect BTL_D_80075CA0[1];
extern int16_t BTL_D_80075DCC[8];
extern int16_t BTL_D_80075DDC[8];
extern int16_t BTL_D_80075DEC[8];
extern int16_t BTL_D_80075DFC[8];
extern EfeBuffRings BTL_D_80075E0C[1];
extern SVECTOR BTL_D_80075E78[32];

void BTL_initializeDeathCountdown(void);
void BTL_initializePartnerTile(void);
void BTL_initializeEnemyHPBarSprites(void);
void BTL_addEnemyHPBars(void);
void BTL_initializePartnerStatusBars(void);
void BTL_initializeCommandMenu(void);
void BTL_drawCommandShout(uint32_t command);
void BTL_addDeathCountdown(Entity *entity);
void entityLookAtLocation(Entity *entity, VECTOR *pos);
int32_t entityCheckCollision(Entity *a, Entity *entity, int32_t c, int32_t d);
int32_t entityIsOffScreen(Entity *entity, int32_t width, int32_t height);
void startBattleIdleAnimation(DigimonEntity *entity, Stats *stats, int32_t flags);
void BTL_battleTickFrame(void);
void BTL_initializeBattleStartText(void);
void BTL_removeBattleStartText(void);
void BTL_initializeBattleStartTextBurst(void);
void BTL_removeBattleStartTextBurst(void);
int32_t BTL_isBattleStartTextFinished(void);
void BTL_handleBattleIntro(void);
void BTL_initializeCombat(void);
int32_t BTL_isBattleFinished(void);
void BTL_removeDeathCountdown(void);
int32_t BTL_isCommandMenuClosed(void);
void BTL_removePartnerStatusBars(void);
void BTL_removeFinisherChargeup(void);
void BTL_removeAllFinisherAuras(void);
void BTL_removeAllPoisonEffects(void);
void BTL_removeAllStunEffects(void);
void BTL_removeAllBuffRingsEffects(void);
void BTL_removeAllBuffDiskEffects(void);
void BTL_removeAllItemParticles(void);
void BTL_removeAllAuraProjectiles(void);
void BTL_unloadAllEFESlots(void);
void BTL_removeEFEEngine(void);
void BTL_drawHoveredCommandName(void);
void BTL_setCommandIconUV(DigimonEntity *digimon, POLY_FT4 *prim, uint8_t index);

#endif
