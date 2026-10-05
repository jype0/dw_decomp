#include <dw/anim.h>
#include <dw/entity.h>
#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/trn.h>
#include <dw/types.h>
#include <dw/world_object.h>

extern uint32_t POLLED_INPUT;
extern int32_t TRAINING_COMPLETE;

void createCameraMovement(VECTOR *pos, int32_t speed);
void removeAllCloudFX(void);
void storeMapObjectPosition();
void loadMapObjectPosition();
int32_t moveMapObjectsWithLimit();
void tamerSetState(int8_t state);
int32_t tickEntityWalkTo();
void TRN_func_800888A0(int8_t arg);
void TRN_tickHpTraining(int32_t instanceId);

static void *trn_hp_functions[] = {
	TRN_tickHpTraining,
	TRN_setupHpTraining,
};

int16_t MAIN_D_8013536C;
int16_t MAIN_D_8013536E;
int8_t MAIN_D_80135370;
int8_t TRAINING_STATE;

static void *trn_hp_sbss_order[] = {
	&TRAINING_STATE,
	&MAIN_D_80135370,
	&MAIN_D_8013536E,
	&MAIN_D_8013536C,
};

GARBAGE_ARRAY(TRN_setupHpTraining, TRN_D_8008F368, 8, 16);

void TRN_setupHpTraining(arg)
int16_t arg;
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
	TRAINING_STATE = 0;
}

void TRN_tickHpTraining(instanceId)
int16_t instanceId;
{
	int32_t r;
	int32_t done;

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
			TRAINING_STATE = 2;
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
		if ((((MAIN_D_8013537A >= 0x4b0) || (POLLED_INPUT & CANCEL_BUTTON)) && (MAIN_D_80135370 == 0)) || ((MAIN_D_80135370 == 1) && (0 <= r))) {
			MAIN_D_8013537A %= 0x4b0;
			playSound(8, 0xa);
			TRAINING_STATE = 3;
			MAIN_D_8013537A /= 120;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			TRN_awardHpTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, 0, r);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
		}
		break;
	case 3:
		if (TRN_statGainsAreZero() == 1) {
			TRAINING_STATE = 4;
			MAIN_D_8013537C = 0;
		}
		break;
	case 4:
		MAIN_D_8013537C++;
		if (MAIN_D_8013537C >= 0x14) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
			TRAINING_STATE = 5;
		}
		break;
	case 5:
		MAIN_D_8013537A--;
		if (MAIN_D_8013537A > 0) {
			moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, -1, 2, 0, 0);
		}
		done = tickEntityWalkTo(0xfc, 0xfd, 0, 0, 0);
		if ((done == 1) && (MAIN_D_8013537A <= 0)) {
			TRN_applyBaseStats();
			TRN_closeUIBox(1);
			partnerSetState(1);
			loadMapObjectPosition(TRN_D_8008F368, TRN_D_8008F388, MAIN_D_8013536C, MAIN_D_8013536E);
			tamerSetState(0);
			removeAllCloudFX();
			MAIN_D_8013537A = 0;
			TRAINING_STATE = 0;
			removeObject(0xfab, instanceId, TRN_tickHpTraining, NULL);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}
