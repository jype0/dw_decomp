#include <dw/anim.h>
#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/trn2.h>
#include <dw/types.h>
#include <dw/world_object.h>

extern int32_t TRAINING_COMPLETE;
extern uint32_t POLLED_INPUT;

void createCameraMovement(VECTOR *pos, int32_t speed);
void removeAllCloudFX(void);
void storeMapObjectPosition();
void loadMapObjectPosition();
int32_t moveMapObjectsWithLimit();
void tamerSetState(int8_t state);
int32_t tickEntityWalkTo();
void TRN2_func_8008AA84(int8_t arg);

static void *trn2_hp_map107_functions[] = {
	TRN2_setupMpTraining,
	TRN2_saveTrainingStartTime,
	TRN2_tickHpTrainingMap107,
};

void TRN2_tickHpTrainingMap107(instanceId)
int16_t instanceId;
{
	int32_t r;
	int32_t done;

	switch (MAIN_D_801353BD) {
	case 0:
		storeMapObjectPosition(TRN2_D_8008DC54, TRN2_D_8008DC74, MAIN_D_801353B4, MAIN_D_801353B6);
		tamerSetState(8);
		partnerSetState(10);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
		createCameraMovement(&TRN2_D_8008DC1C, 10);
		playSound(8, 9);
		MAIN_D_801353C2 = 0;
		MAIN_D_801353BD = 1;
		TRN2_startSlotSessionIfEnabled(0);
		break;
	case 1:
		TRN2_func_8008AA84(4);
		if (tickEntityWalkTo(0xfc, 0xff, TRN2_D_8008DC1C.vx, TRN2_D_8008DC1C.vz, 0) == 1) {
			startAnimation(ENTITY_TABLE[1], 0x1b);
			PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x898;
			if (MAIN_D_801353BC == 1) {
				TRN2_startSlotSpin();
			}
			playSound(8, 0);
			MAIN_D_801353BD = 2;
		}
		break;
	case 2:
		PARTNER_ENTITY.digimonEntity.entity.anim.animFlag |= 2;
		if (((MAIN_D_801353C2 % 0x78) == 0) && (MAIN_D_801353C2 < 0x4b1)) {
			PARTNER_ENTITY.digimonEntity.entity.anim.animFlag &= 0xfd;
			moveMapObjectsWithLimit(MAIN_D_801353B4, MAIN_D_801353B6, 0, -1, 0, 0);
		}
		if ((MAIN_D_801353C2 % 2) == 0) {
			moveMapObjectsWithLimit(MAIN_D_801353B4, MAIN_D_801353B6, 2, 0, 0, 0);
		} else {
			moveMapObjectsWithLimit(MAIN_D_801353B4, MAIN_D_801353B6, -2, 0, 0, 0);
		}
		if ((MAIN_D_801353C2 % 10) == 0) {
			playSound(8, 0);
		}
		MAIN_D_801353C2++;
		r = 10;
		if (MAIN_D_801353BC == 1) {
			r = TRN2_getSlotSessionResult();
		}
		if ((((MAIN_D_801353C2 >= 0x4b0) || (POLLED_INPUT & CANCEL_BUTTON)) && (MAIN_D_801353BC == 0)) || ((MAIN_D_801353BC == 1) && (0 <= r))) {
			MAIN_D_801353C2 %= 0x4b0;
			playSound(8, 0xa);
			MAIN_D_801353C2 /= 120;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			TRN2_awardHpTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, 1, r);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
			MAIN_D_801353BD = 3;
		}
		break;
	case 3:
		if (TRN2_statGainsAreZero() == 1) {
			MAIN_D_801353C4 = 0;
			MAIN_D_801353BD = 4;
		}
		break;
	case 4:
		MAIN_D_801353C4++;
		if (MAIN_D_801353C4 >= 0x14) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
			MAIN_D_801353BD = 5;
		}
		break;
	case 5:
		MAIN_D_801353C2--;
		if (MAIN_D_801353C2 > 0) {
			moveMapObjectsWithLimit(MAIN_D_801353B4, MAIN_D_801353B6, -1, 2, 0, 0);
		}
		done = tickEntityWalkTo(0xfc, 0xfd, 0, 0, 0);
		if ((done == 1) && (MAIN_D_801353C2 <= 0)) {
			TRN2_applyBaseStats();
			TRN2_closeUIBox(1);
			partnerSetState(1);
			loadMapObjectPosition(TRN2_D_8008DC54, TRN2_D_8008DC74, MAIN_D_801353B4, MAIN_D_801353B6);
			tamerSetState(0);
			removeAllCloudFX();
			MAIN_D_801353C2 = 0;
			MAIN_D_801353BD = 0;
			removeObject(0xfab, instanceId);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}

void TRN2_saveTrainingStartTime(void)
{
	MAIN_D_801353CE = CURRENT_FRAME;
	MAIN_D_801353D0 = HOUR;
	MAIN_D_801353D2 = MINUTE;
}

GARBAGE(TRN2_setupMpTraining, 8);

void TRN2_setupMpTraining(arg)
int16_t arg;
{
	if (arg == 0xa5) {
		TRN2_D_8008DC2C.vx = 0x3e5;
		TRN2_D_8008DC2C.vy = 0;
		TRN2_D_8008DC2C.vz = 0x76;
		TRN2_D_8008DC1C.vx = 0x6f8;
		TRN2_D_8008DC1C.vy = 0;
		TRN2_D_8008DC1C.vz = 0x1d2;
		addObject(0xfaf, 8, (TickFunction)TRN2_tickMpTraining, NULL);
	}

	TRAINING_COMPLETE = 0;
	MAIN_D_801353BC = readPStat(0xf6);
	TRN2_saveTrainingStartTime();
	MAIN_D_801353BD = 0;
}
