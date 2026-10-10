#include <dw/anim.h>
#include <dw/entity.h>
#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/training.h>
#include <dw/trn.h>
#include <dw/types.h>
#include <dw/version.h>
#include <dw/world_object.h>

extern uint32_t POLLED_INPUT;
extern int32_t TRAINING_COMPLETE;

void createCameraMovement(VECTOR *pos, int32_t speed);
void storeMapObjectPosition();
void loadMapObjectPosition();
void setMapObjectsFlag(int16_t start, int16_t count, int32_t flag);
void moveMapObjects(int32_t startIndex, int32_t count, int32_t dx, int32_t dy);
void getDrawPosition(SVECTOR *worldPos, int16_t *outX, int16_t *outY);
void tamerSetState(int8_t state);
int32_t tickEntityWalkTo();

static void *trn_mp_functions[] = {
#if VERSION_IS(EU)
	TRN_setupMpTraining,
	TRN_tickMpTraining,
#else
	TRN_tickMpTraining,
	TRN_setupMpTraining,
#endif
};

int16_t MAIN_D_80135372;
int16_t MAIN_D_80135374;
int16_t MAIN_D_80135376;
int16_t MAIN_D_80135378;
int16_t MAIN_D_8013537A;
int16_t MAIN_D_8013537C;

static void *trn_mp_sbss_order[] = {
	&MAIN_D_8013537C,
	&MAIN_D_8013537A,
#if VERSION_IS(EU)
	&MAIN_D_80135372,
	&MAIN_D_80135374,
	&MAIN_D_80135376,
	&MAIN_D_80135378,
#else
	&MAIN_D_80135378,
	&MAIN_D_80135376,
	&MAIN_D_80135374,
	&MAIN_D_80135372,
#endif
};

void TRN_tickMpTraining(instanceId)
int16_t instanceId;
{
	SVECTOR svec;
	SVECTOR screen;
	int32_t r;

	switch (TRAINING_STATE) {
	case 0:
		storeMapObjectPosition(TRN_D_8008F368, TRN_D_8008F388, MAIN_D_8013536C, MAIN_D_8013536E);
		tamerSetState(8);
		partnerSetState(10);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
		createCameraMovement(&TRN_D_8008F320, 10);
		playSound(8, 9);
		MAIN_D_8013537A = 0;
		TRAINING_STATE = 1;
		TRN_startSlotSessionIfEnabled(1);
		break;
	case 1:
		if (tickEntityWalkTo(0xfc, 0xff, TRN_D_8008F330.vx, TRN_D_8008F330.vz, 0) == 1) {
			TRAINING_STATE = 2;
		}
		break;
	case 2:
		if (tickEntityWalkTo(0xfc, 0xff, TRN_D_8008F320.vx, TRN_D_8008F320.vz, 0) == 1) {
			setMapObjectsFlag(MAIN_D_80135372, MAIN_D_80135374, 1);
			setMapObjectsFlag(MAIN_D_80135376, MAIN_D_80135378, 0);
			svec.vx = PARTNER_ENTITY.digimonEntity.entity.posData->location.vx;
			svec.vy = PARTNER_ENTITY.digimonEntity.entity.posData->location.vy - DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].height;
			svec.vz = PARTNER_ENTITY.digimonEntity.entity.posData->location.vz;
			getDrawPosition(&svec, &screen.vx, &screen.vy);
			moveMapObjects(MAIN_D_8013536C, MAIN_D_8013536E, 0, screen.vy - 0x14);
			PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
			MAIN_D_80135384 = playSound2(8, 2);
			TRAINING_STATE = 3;
			if (MAIN_D_80135370 == 1) {
				TRN_startSlotSpin();
			}
		}
		break;
	case 3:
		MAIN_D_8013537A++;
		r = 10;
		if (MAIN_D_80135370 == 1) {
			r = TRN_getSlotSessionResult();
		}
		if ((((MAIN_D_8013537A >= 0x4b0) || (POLLED_INPUT & CANCEL_BUTTON)) && (MAIN_D_80135370 == 0)) || ((MAIN_D_80135370 == 1) && (r >= 0))) {
			TRN_awardMpTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, 0, r);
			setMapObjectsFlag(MAIN_D_80135372, MAIN_D_80135374, 0);
			setMapObjectsFlag(MAIN_D_80135376, MAIN_D_80135378, 1);
			loadMapObjectPosition(TRN_D_8008F368, TRN_D_8008F388, MAIN_D_8013536C, MAIN_D_8013536E);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
			playSound(8, 0xa);
			thunkStopSoundMask(MAIN_D_80135384);
			TRAINING_STATE = 4;
		}
		break;
	case 4:
		if (tickEntityWalkTo(0xfc, 0xff, TRN_D_8008F330.vx, TRN_D_8008F330.vz, 0) == 1) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			TRAINING_STATE = 5;
		}
		break;
	case 5:
		if (TRN_statGainsAreZero() == 1) {
			MAIN_D_8013537C = 0;
			TRAINING_STATE = 6;
		}
		break;
	case 6:
		MAIN_D_8013537C++;
		if (MAIN_D_8013537C >= 0x14) {
			TRN_applyBaseStats();
			TRN_closeUIBox(1);
			tamerSetState(0);
			partnerSetState(1);
			MAIN_D_8013537A = 0;
			TRAINING_STATE = 0;
			removeObject(0xfaf, instanceId, TRN_tickMpTraining, NULL);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}

GARBAGE(TRN_setupMpTraining, 14);

void TRN_setupMpTraining(arg)
int16_t arg;
{
	if (arg == 0x70) {
		TRN_D_8008F330.vx = 0x290;
		TRN_D_8008F330.vy = 0;
		TRN_D_8008F330.vz = 0x2ee;
		TRN_D_8008F320.vx = 0x2ac;
		TRN_D_8008F320.vy = 0;
		TRN_D_8008F320.vz = 0x868;
		MAIN_D_8013536C = 0xa4;
		MAIN_D_8013536E = 8;
		MAIN_D_80135372 = 0x93;
		MAIN_D_80135374 = 0x10;
		MAIN_D_80135376 = 0xa3;
		MAIN_D_80135378 = 0x18;
		addObject(0xfaf, 0, (TickFunction)TRN_tickMpTraining, NULL);
		if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level < 3) {
			PARTNER_PARA.upgradeMPcounter++;
		}
		TRAINING_COMPLETE = 0;
	}

	MAIN_D_80135370 = readPStat(0xf6);
	TRN_saveTrainingStartTime();
	TRAINING_STATE = 0;
}
