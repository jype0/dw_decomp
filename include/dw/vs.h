#ifndef DW_VS_H
#define DW_VS_H

#include <libgte.h>

#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/types.h>

extern uint8_t VS_D_8006FF20[][14];
extern uint8_t VS_D_800716A8[];
extern uint8_t VS_D_800716AD[];
extern uint8_t VS_D_800716B2[];
extern uint8_t VS_D_800716B3[];
extern VECTOR VS_D_80071744;

int32_t VS_addAuraProjectile(Entity *e);
void VS_addCommandMenu(uint8_t index);
void VS_addFighterCounter(uint8_t arg);
void VS_addFighterStatusBars(int32_t id);
int32_t VS_addFinisherAura(int32_t arg, int32_t val);
void VS_addFinisherProgress(FighterData *fighter, int16_t amount);
void VS_addTargetCursor(void);
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
void VS_playMoveEffect(void);
void VS_queueRandomMove(void);
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
int32_t VS_selectMoveByMpCost(int32_t arg0, int16_t *flags);
int32_t VS_selectMoveByPower(int32_t arg0, int16_t *flags);
int32_t VS_selectMoveTarget(Entity *entity, FighterData *fighter);
void VS_selectPartnerMove(void);
void VS_selectRandomCamera(void);
void VS_setRandomViewpoint(Entity *entity, int32_t idx);
void VS_setupQueuedMove(DigimonEntity *digimon, FighterData *fighter, int16_t arg2, int32_t moveIndex);
void VS_startCameraChase(void);
void VS_startFighterMove(void);
void VS_tickFrame(void);
void VS_unloadAllEFESlots(void);

#endif
