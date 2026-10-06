#ifndef DW_BTL_H
#define DW_BTL_H

#include <libgs.h>
#include <libgte.h>

#include <dw/battle.h>
#include <dw/efe.h>
#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/types.h>

typedef struct {
	long frame;
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
extern int16_t BATTLE_FRAME_COUNT;
extern int16_t FLEE_TIMER;
extern Entity *FINISHING_ENTITY;
extern int32_t HAS_TAKEN_DAMAGE;
extern int32_t NO_AI_FLAG;
extern int32_t FLEE_DISABLED[2];
extern uint8_t BTL_SAVED_CHARGE_MODE;
extern long BTL_FINISHER_AURA_ID;
extern int32_t BTL_FINISHER_TIMER;
extern int8_t BTL_COMMAND_MENU_ACTIVE;
extern StatsGains INITIAL_COMBAT_STATS[];
extern int8_t GAME_STATE;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern char *MOVE_NAMES[];

extern int16_t BTL_CARDINAL_ROTATIONS[4];
extern int8_t BTL_COMMAND_MENU_CLOSED_FRAMES;
extern uint8_t BTL_COMMAND_LABEL_U[5];
extern uint8_t BTL_COMMAND_LABEL_W[8];

#if defined(VERSION_JP)
extern int8_t BTL_SHOUT_HOP_OFFSETS[20];
extern int8_t BTL_SHOUT_DROP_OFFSETS[20];
extern uint8_t BTL_COMMAND_MENU_LAYOUTS[6][10];
extern int16_t BTL_COMMAND_MENU_SLIDE_Y[8];
extern int16_t BTL_COMMAND_MENU_SLIDE_X[8];
extern int16_t BTL_BATTLE_START_TEXT_POSITIONS[155][2];
extern int16_t BTL_STATUS_BAR_X[8];
extern int32_t BTL_FINISHER_PULSE[12];
extern BarSprite BTL_STATUS_BAR_SPRITES[6];
#else
extern const int8_t BTL_SHOUT_HOP_OFFSETS[20];
extern const int8_t BTL_SHOUT_DROP_OFFSETS[20];
extern const uint8_t BTL_COMMAND_MENU_LAYOUTS[6][10];
extern const int16_t BTL_COMMAND_MENU_SLIDE_Y[8];
extern const int16_t BTL_COMMAND_MENU_SLIDE_X[8];
extern const int16_t BTL_BATTLE_START_TEXT_POSITIONS[155][2];
extern const int16_t BTL_STATUS_BAR_X[8];
extern const int32_t BTL_FINISHER_PULSE[12];
extern const BarSprite BTL_STATUS_BAR_SPRITES[6];
#endif
extern GsSPRITE BTL_DEATH_COUNTDOWN_SPRITE;
extern BtlDeathCountdownRaw BTL_DEATH_COUNTDOWN;
extern char BTL_END_BOX_TEXTBUFFER[1024];
extern uint8_t BTL_BATTLE_START_TEXT_PIECES[155][20];
extern void (*BTL_EFE_SUB_OPCODE_HANDLERS[97])(void);
extern EfePoisonBubble BTL_POISON_BUBBLES[12];
extern EfePoison BTL_POISON_EFFECTS[4];
extern EfeConfusion BTL_CONFUSION_EFFECTS[4];
extern EfeStun BTL_STUN_EFFECTS[5];
extern EfeStunSpark BTL_STUN_SUB_EFFECTS[25];
extern EfeFinisherAura BTL_FINISHER_AURAS[2];
extern SVECTOR BTL_FINISHER_AURA_SPARKS[20];
extern BtlParticleVelocity BTL_ITEM_PARTICLE_VELOCITIES[20];
extern BtlParticleDrag BTL_ITEM_PARTICLE_DRAG[20];
extern BtlItemParticleEffect BTL_ITEM_PARTICLE_EFFECTS[4];
extern EfeBuffDisk BTL_BUFF_DISKS[1];
extern EfeTrailEffect BTL_BUFF_TRAILS[1];
extern int16_t BTL_BUFF_TRAIL_A_X[8];
extern int16_t BTL_BUFF_TRAIL_A_Z[8];
extern int16_t BTL_BUFF_TRAIL_B_X[8];
extern int16_t BTL_BUFF_TRAIL_B_Z[8];
extern EfeBuffRings BTL_BUFF_RINGS[1];
extern SVECTOR BTL_BUFF_RING_SPARKS[32];

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
