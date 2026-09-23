#include <dw/anim.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/tamer.h>
#include <dw/trn.h>
#include <dw/types.h>
#include <dw/world_object.h>

extern uint32_t POLLED_INPUT;
extern int32_t TRAINING_COMPLETE;

void createCameraMovement(VECTOR *pos, int32_t speed);
void removeAllCloudFX(void);
void storeMapObjectPosition(int16_t *outX, int16_t *outY, int16_t a, int16_t count);
void loadMapObjectPosition(int16_t *xData, int16_t *yData, int16_t startIndex, int16_t count);
int32_t moveMapObjectsWithLimit(int16_t startIndex, int16_t count, int16_t dx, int16_t dy, int16_t limitX, int16_t limitY);
void TRN_func_800888A0(int8_t arg);
void TRN_tickHpTraining(int32_t instanceId);

static void *trn_hp_functions[] = {
	TRN_tickHpTraining,
	TRN_setupHpTraining,
};

int16_t MAIN_D_8013536C;
int16_t MAIN_D_8013536E;
int8_t MAIN_D_80135370;
int8_t MAIN_D_80135371;

static void *trn_hp_sbss_order[] = {
	&MAIN_D_80135371,
	&MAIN_D_80135370,
	&MAIN_D_8013536E,
	&MAIN_D_8013536C,
};

static void trn_hp__garbage__(void)
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

void TRN_setupHpTraining(int32_t arg)
{
	if (arg == 0x70) {
		TRN_D_8008F320.vx = 0x58c;
		TRN_D_8008F320.vy = 0;
		TRN_D_8008F320.vz = -0x6d0;
		MAIN_D_8013536C = 0x53;
		MAIN_D_8013536E = 0x10;
		addObject(0xfab, 0, (TickFunction)TRN_tickHpTraining, NULL);
		MAIN_D_80135370 = readPStat(0xf6);
		if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level < 3) {
			PARTNER_PARA.upgradeHPcounter++;
		}
		TRAINING_COMPLETE = 0;
	}

	TRN_saveTrainingStartTime();
	MAIN_D_80135371 = 0;
}

void TRN_tickHpTraining(int32_t instanceId)
{
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
		TRN_startSlotSessionIfEnabled(0);
		break;
	case 1:
		TRN_func_800888A0(4);
		if (tickEntityWalkTo(0xfc, 0xff, TRN_D_8008F320.vx, TRN_D_8008F320.vz, 0) == 1) {
			startAnimation(ENTITY_TABLE[1], 0x1b);
			PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0xa00;
			if (MAIN_D_80135370 == 1) {
				TRN_startSlotSpin();
			}
			playSound(8, 0);
			MAIN_D_80135371 = 2;
		}
		break;
	case 2:
		PARTNER_ENTITY.digimonEntity.entity.anim.animFlag |= 2;
		if (((MAIN_D_8013537A % 0x78) == 0) && (MAIN_D_8013537A < 0x4b1)) {
			PARTNER_ENTITY.digimonEntity.entity.anim.animFlag &= 0xfd;
			moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, 1, -2, 0, 0);
		}
		if ((MAIN_D_8013537A % 2) == 0) {
			moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, 2, 0, 0, 0);
		} else {
			moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, -2, 0, 0, 0);
		}
		if ((MAIN_D_8013537A % 10) == 0) {
			playSound(8, 0);
		}
		TRN_createCloudFXLine(MAIN_D_8013537A, 0xf, 0x550, -0x604, 0x64, -0x32, 8);
		TRN_createCloudFXLine(MAIN_D_8013537A, 0x14, 0x5b5, -0x667, 0x32, -0x32, 6);
		TRN_createCloudFXLine(MAIN_D_8013537A, 0x12, 0x514, -0x525, 0x14, -0x32, 4);
		MAIN_D_8013537A++;
		r = 10;
		if (MAIN_D_80135370 == 1) {
			r = TRN_getSlotSessionResult();
		}
		if ((((MAIN_D_8013537A >= 0x4b0) || (POLLED_INPUT & 0x10)) && (MAIN_D_80135370 == 0)) || ((MAIN_D_80135370 == 1) && (0 <= r))) {
			MAIN_D_8013537A %= 0x4b0;
			playSound(8, 0xa);
			MAIN_D_80135371 = 3;
			MAIN_D_8013537A = MAIN_D_8013537A / 120;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			TRN_awardHpTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, 0, r);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
		}
		break;
	case 3:
		if (TRN_statGainsAreZero() == 1) {
			MAIN_D_80135371 = 4;
			MAIN_D_8013537C = 0;
		}
		break;
	case 4:
		MAIN_D_8013537C++;
		if (MAIN_D_8013537C >= 0x14) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
			MAIN_D_80135371 = 5;
		}
		break;
	case 5:
		MAIN_D_8013537A--;
		if (MAIN_D_8013537A > 0) {
			moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, -1, 2, 0, 0);
		}
		if ((tickEntityWalkTo(0xfc, 0xfd, 0, 0, 0) == 1) && (MAIN_D_8013537A <= 0)) {
			TRN_applyBaseStats();
			TRN_closeUIBox(1);
			setPartnerState(1);
			loadMapObjectPosition(TRN_D_8008F368, TRN_D_8008F388, MAIN_D_8013536C, MAIN_D_8013536E);
			setTamerState(0);
			removeAllCloudFX();
			MAIN_D_8013537A = 0;
			MAIN_D_80135371 = 0;
			removeObject(0xfab, instanceId, TRN_tickHpTraining, NULL);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}
