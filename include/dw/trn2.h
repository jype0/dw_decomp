#ifndef DW_TRN2_H
#define DW_TRN2_H

#include <libgpu.h>
#include <libgte.h>

#include <dw/training.h>
#include <dw/types.h>

extern VECTOR TRN2_D_8008DC1C;
extern VECTOR TRN2_D_8008DC2C;
extern SVECTOR TRN2_D_8008DC3C[];
extern int16_t TRN2_D_8008DC54[];
extern int16_t TRN2_D_8008DC74[];
extern SlotMachine TRN2_SLOT_MACHINE;

extern char MAIN_D_80134BE0[7];
extern RECT MAIN_D_80134BE8;
extern RECT MAIN_D_80134BF0;
extern RECT MAIN_D_80134BF8;

extern int16_t MAIN_D_801353B4;
extern int16_t MAIN_D_801353B6;
extern int16_t MAIN_D_801353B8;
extern int16_t MAIN_D_801353BA;
extern int8_t MAIN_D_801353BC;
extern int8_t MAIN_D_801353BD;
extern int16_t MAIN_D_801353BE;
extern int16_t MAIN_D_801353C0;
extern int16_t MAIN_D_801353C2;
extern int16_t MAIN_D_801353C4;
extern int16_t MAIN_D_801353C6;
extern uint32_t MAIN_D_801353C8;
extern int16_t MAIN_D_801353CC;
extern uint16_t MAIN_D_801353CE;
extern int16_t MAIN_D_801353D0;
extern int16_t MAIN_D_801353D2;
extern int32_t MAIN_D_801353D4;
extern int8_t MAIN_D_801353D8[6];
extern int16_t MAIN_D_801353DE;
extern int16_t MAIN_D_801353E0[4];

void TRN2_tickHpTrainingMap99(int32_t instanceId);
void TRN2_tickDefenseTrainingMap99(int32_t instanceId);
void TRN2_setupHpTraining(int32_t mapId);
void TRN2_tickHpTrainingMap107(int32_t instanceId);
void TRN2_saveTrainingStartTime(void);
void TRN2_setupMpTraining(int32_t mapId);
void TRN2_tickMpTraining(int32_t instanceId);
void TRN2_setupOffenseTraining(int32_t mapId);
void TRN2_setupDefenseTraining(int32_t mapId);
void TRN2_tickDefenseTrainingMap108(int32_t instanceId);
void TRN2_setupSpeedTraining(int32_t mapId);
void TRN2_tickSpeedTraining(int32_t instanceId);
void TRN2_startSlotSessionIfEnabled(int16_t arg);
void TRN2_awardHpTrainingGains(int32_t a, int32_t b, int32_t c);
int32_t TRN2_statGainsAreZero(void);
void TRN2_awardMpTrainingGains(int32_t a, int16_t b, int32_t c);
void TRN2_awardOffenseTrainingGains(int32_t a, int16_t b, int32_t c);
void TRN2_awardDefenseTrainingGains(int32_t a, int32_t b, int32_t c);
int16_t TRN2_calculateTrainingMultiplier(int32_t type, int32_t mode);
void TRN2_advanceTrainingTime(int16_t tiredGain, int16_t energyLoss, int16_t happyLoss);
void TRN2_saveBaseStats(void);
void TRN2_applyBaseStats(void);
void TRN2_createPostTrainingStatsBox(void);
void TRN2_closeUIBox(int32_t id);
void TRN2_tickSlotSession(void);
void TRN2_renderSlotSession(void);
int32_t TRN2_startSlotSession(int32_t arg);
int16_t TRN2_getSlotSessionResult(void);
void TRN2_createSlotMachineBox(int16_t arg);
int16_t TRN2_getSlotMachineResult(void);
void TRN2_startSlotSpin(void);

#endif
