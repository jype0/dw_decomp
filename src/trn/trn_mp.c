#include <dw/anim.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/tamer.h>
#include <dw/training.h>
#include <dw/trn.h>
#include <dw/types.h>
#include <dw/world_object.h>

extern uint32_t POLLED_INPUT;
extern int32_t TRAINING_COMPLETE;

void createCameraMovement(VECTOR *pos, int32_t speed);
void storeMapObjectPosition(int16_t *outX, int16_t *outY, int16_t a, int16_t count);
void loadMapObjectPosition(int16_t *xData, int16_t *yData, int16_t startIndex, int16_t count);
void setMapObjectsFlag(int16_t start, int16_t count, int32_t flag);
void moveMapObjects(int32_t startIndex, int32_t count, int32_t dx, int32_t dy);
void getDrawPosition(SVECTOR *worldPos, int16_t *outX, int16_t *outY);

static void *trn_mp_functions[] = {
	TRN_tickMpTraining,
	TRN_setupMpTraining,
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
	&MAIN_D_80135378,
	&MAIN_D_80135376,
	&MAIN_D_80135374,
	&MAIN_D_80135372,
};

void TRN_tickMpTraining(int32_t instanceId)
{
	SVECTOR svec;
	SVECTOR screen;
	int32_t r;

	switch (MAIN_D_80135371) {
	case 0:
		storeMapObjectPosition(TRN_D_8008F368, TRN_D_8008F388, MAIN_D_8013536C, MAIN_D_8013536E);
		setTamerState(8);
		setPartnerState(10);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
		createCameraMovement(&TRN_D_8008F320, 10);
		playSound(8, 9);
		MAIN_D_8013537A = 0;
		MAIN_D_80135371 = 1;
		TRN_startSlotSessionIfEnabled(1);
		break;
	case 1:
		if (tickEntityWalkTo(0xfc, 0xff, TRN_D_8008F330.vx, TRN_D_8008F330.vz, 0) == 1) {
			MAIN_D_80135371 = 2;
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
			MAIN_D_80135371 = 3;
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
		if ((((MAIN_D_8013537A >= 0x4b0) || (POLLED_INPUT & 0x10)) && (MAIN_D_80135370 == 0)) || ((MAIN_D_80135370 == 1) && (r >= 0))) {
			TRN_awardMpTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, 0, r);
			setMapObjectsFlag(MAIN_D_80135372, MAIN_D_80135374, 0);
			setMapObjectsFlag(MAIN_D_80135376, MAIN_D_80135378, 1);
			loadMapObjectPosition(TRN_D_8008F368, TRN_D_8008F388, MAIN_D_8013536C, MAIN_D_8013536E);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
			playSound(8, 0xa);
			stopSoundMask(MAIN_D_80135384);
			MAIN_D_80135371 = 4;
		}
		break;
	case 4:
		if (tickEntityWalkTo(0xfc, 0xff, TRN_D_8008F330.vx, TRN_D_8008F330.vz, 0) == 1) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			MAIN_D_80135371 = 5;
		}
		break;
	case 5:
		if (TRN_statGainsAreZero() == 1) {
			MAIN_D_8013537C = 0;
			MAIN_D_80135371 = 6;
		}
		break;
	case 6:
		MAIN_D_8013537C++;
		if (MAIN_D_8013537C >= 0x14) {
			TRN_applyBaseStats();
			TRN_closeUIBox(1);
			setTamerState(0);
			setPartnerState(1);
			MAIN_D_8013537A = 0;
			MAIN_D_80135371 = 0;
			removeObject(0xfaf, instanceId, TRN_tickMpTraining, NULL);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}

static void trn_mp__garbage__(void)
{
	int32_t v0;
	int32_t v1;
	int32_t v2;
	int32_t v3;
	int32_t v4;
	int32_t v5;
	int32_t v6;
	int32_t v7;
	int32_t v8;
	int32_t v9;
	int32_t v10;
	int32_t v11;
	int32_t v12;
	int32_t v13;
	int32_t v14;
	int32_t v15;
	int32_t v16;
	int32_t v17;
	int32_t v18;
	int32_t v19;

	v0 = TRN_D_8008F368[0] + 0;
	v1 = TRN_D_8008F368[1] + 1;
	v2 = TRN_D_8008F368[2] + 2;
	v3 = TRN_D_8008F368[3] + 3;
	v4 = TRN_D_8008F368[4] + 4;
	v5 = TRN_D_8008F368[5] + 5;
	v6 = TRN_D_8008F368[6] + 6;
	v7 = TRN_D_8008F368[7] + 7;
	v8 = TRN_D_8008F368[0] + 8;
	v9 = TRN_D_8008F368[1] + 9;
	v10 = TRN_D_8008F368[2] + 10;
	v11 = TRN_D_8008F368[3] + 11;
	v12 = TRN_D_8008F368[4] + 12;
	v13 = TRN_D_8008F368[5] + 13;
	v14 = TRN_D_8008F368[6] + 14;
	v15 = TRN_D_8008F368[7] + 15;
	v16 = TRN_D_8008F368[0] + 16;
	v17 = TRN_D_8008F368[1] + 17;
	v18 = TRN_D_8008F368[2] + 18;
	v19 = TRN_D_8008F368[3] + 19;
	TRN_D_8008F368[0] = (v0 * v1) + v2;
	TRN_D_8008F368[1] = (v1 * v2) + v3;
	TRN_D_8008F368[2] = (v2 * v3) + v4;
	TRN_D_8008F368[3] = (v3 * v4) + v5;
	TRN_D_8008F368[4] = (v4 * v5) + v6;
	TRN_D_8008F368[5] = (v5 * v6) + v7;
	TRN_D_8008F368[6] = (v6 * v7) + v8;
	TRN_D_8008F368[7] = (v7 * v8) + v9;
	TRN_D_8008F368[0] = (v8 * v9) + v10;
	TRN_D_8008F368[1] = (v9 * v10) + v11;
	TRN_D_8008F368[2] = (v10 * v11) + v12;
	TRN_D_8008F368[3] = (v11 * v12) + v13;
	TRN_D_8008F368[4] = (v12 * v13) + v14;
	TRN_D_8008F368[5] = (v13 * v14) + v15;
	TRN_D_8008F368[6] = (v14 * v15) + v16;
	TRN_D_8008F368[7] = (v15 * v16) + v17;
	TRN_D_8008F368[0] = (v16 * v17) + v18;
	TRN_D_8008F368[1] = (v17 * v18) + v19;
	TRN_D_8008F368[2] = (v18 * v19) + v0;
	TRN_D_8008F368[3] = (v19 * v0) + v1;
}

void TRN_setupMpTraining(int32_t arg)
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
	MAIN_D_80135371 = 0;
}
