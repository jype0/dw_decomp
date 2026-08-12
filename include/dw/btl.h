#ifndef DW_BTL_H
#define DW_BTL_H

#include <libgs.h>
#include <libgte.h>

#include <dw/entity.h>
#include <dw/math.h>
#include <dw/script.h>
#include <dw/types.h>

extern int16_t ENEMY_COUNT;
extern int32_t TAMER_ITEM[];
extern int16_t MAIN_D_80134D66;
extern int16_t MAIN_D_80134D68;
extern Entity *MAIN_D_80134D60;
extern int32_t MAIN_D_80134D70;
extern int32_t MAIN_D_80134D74;
extern int32_t MAIN_D_80134D7C[2];
extern uint8_t MAIN_D_80135078;
extern int32_t MAIN_D_8013507C;
extern int32_t MAIN_D_80135080;
extern StatsGains INITIAL_COMBAT_STATS[];
extern int8_t GAME_STATE;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern char *MOVE_NAMES[];
extern uint8_t MAIN_D_80134730[8];
extern int32_t MAIN_D_80135098;
extern int16_t MAIN_D_801350AE;
extern int16_t MAIN_D_801350B0;
extern int16_t MAIN_D_801350B2;
extern int16_t MAIN_D_801350B4;
extern int16_t MAIN_D_801350B6;
extern int16_t MAIN_D_801350B8;

void BTL_initializeDeathCountdown(void);
void BTL_initializePartnerTile(void);
void BTL_initializeEnemyHPBarSprites(void);
void BTL_addEnemyHPBars(void);
void BTL_initializePartnerStatusBars(void);
void BTL_initializeCommandMenu(void);
void BTL_addDeathCountdown(Entity *entity);
void startAnimation(Entity *entity, int32_t animId);
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
void drawString(char *text, int32_t color, int32_t pos);
void BTL_drawHoveredCommandName(void);

#endif
