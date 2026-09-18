#ifndef DW_TRN_H
#define DW_TRN_H

#include <libgpu.h>
#include <libgte.h>

#include <dw/training.h>
#include <dw/types.h>

extern VECTOR TRN_D_8008F320;
extern VECTOR TRN_D_8008F330;
extern SVECTOR TRN_D_8008F340[];
extern VECTOR TRN_D_8008F358;
extern int16_t TRN_D_8008F368[];
extern int16_t TRN_D_8008F388[];
extern SlotMachine TRN_SLOT_MACHINE;

extern char MAIN_D_80134BC0[7];
extern RECT MAIN_D_80134BC8;
extern RECT MAIN_D_80134BD0;
extern RECT MAIN_D_80134BD8;

extern int16_t MAIN_D_8013536C;
extern int16_t MAIN_D_8013536E;
extern int8_t MAIN_D_80135370;
extern int8_t MAIN_D_80135371;
extern int16_t MAIN_D_80135372;
extern int16_t MAIN_D_80135374;
extern int16_t MAIN_D_80135376;
extern int16_t MAIN_D_80135378;
extern int16_t MAIN_D_8013537A;
extern int16_t MAIN_D_8013537C;
extern int16_t MAIN_D_8013537E;
extern int16_t MAIN_D_80135380;
extern uint32_t MAIN_D_80135384;
extern int32_t MAIN_D_80135388;
extern uint16_t MAIN_D_8013538C;
extern int16_t MAIN_D_8013538E;
extern int16_t MAIN_D_80135390;
extern uint8_t MAIN_D_80135392;
extern int32_t MAIN_D_80135394;
extern int8_t MAIN_D_80135398[6];
extern int16_t MAIN_D_8013539E;
extern int16_t MAIN_D_801353A0[4];

void TRN_saveTrainingStartTime(void);
void TRN_startSlotSessionIfEnabled(int16_t arg);
void TRN_createCloudFXLine(int16_t a, int16_t b, int16_t x, int16_t z, int16_t dx, int16_t dz, int8_t n);
void TRN_awardHpTrainingGains(int32_t a, int16_t b, int32_t c);
int32_t TRN_statGainsAreZero(void);
void TRN_awardOffenseTrainingGains(int32_t a, int32_t b, int32_t c);
void TRN_awardSpeedTrainingGains(int32_t a, int32_t b, int32_t c);
void TRN_awardDefenseTrainingGains(int32_t a, int16_t b, int32_t c);
void TRN_awardMpTrainingGains(int32_t a, int16_t b, int32_t c);
void TRN_awardBrainsTrainingGains(int32_t a, int32_t b, int32_t c);
void TRN_setupHpTraining(int32_t mapId);
void TRN_setupMpTraining(int32_t mapId);
void TRN_setupOffenseTraining(int32_t mapId);
void TRN_setupDefenseTraining(int32_t mapId);
void TRN_setupSpeedTraining(int32_t mapId);
void TRN_setupBrainsTraining(int32_t mapId);
void TRN_saveBaseStats(void);
void TRN_applyBaseStats(void);
void TRN_createPostTrainingStatsBox(void);
void TRN_closeUIBox(int32_t id);
void TRN_tickSlotSession(void);
void TRN_renderSlotSession(void);
int32_t TRN_startSlotSession(int32_t arg);
int16_t TRN_getSlotSessionResult(void);
void TRN_createSlotMachineBox(int16_t arg);
int16_t TRN_getSlotMachineResult(void);
void TRN_startSlotSpin(void);

#endif
