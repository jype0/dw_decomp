#include <dw/anim.h>
#include <dw/entity.h>
#include <dw/input.h>
#include <dw/model.h>
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
void tamerSetState(int8_t state);
int32_t tickEntityWalkTo();
void setCameraFollowPlayer(void);
void unsetCameraFollowPlayer(void);
void TRN_func_800888A0(int8_t arg);
void TRN_tickSpeedTraining(int32_t instanceId);

static void *trn_speed_functions[] = {
	TRN_tickSpeedTraining,
	TRN_setupSpeedTraining,
};

void TRN_setupSpeedTraining(arg)
int16_t arg;
{
	switch (arg) {
	case 0x70:
		TRN_D_8008F320.vx = 0x2d;
		TRN_D_8008F320.vy = 0;
		TRN_D_8008F320.vz = -0x184;
		if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level < 3) {
			PARTNER_PARA.upgradeSpeedCounter++;
		}
		addObject(0xfad, 0, (TickFunction)TRN_tickSpeedTraining, NULL);
		break;
	case 0x4e:
		TRN_D_8008F320.vx = 0x32;
		TRN_D_8008F320.vy = 0;
		TRN_D_8008F320.vz = -0x11f;
		addObject(0xfad, 6, (TickFunction)TRN_tickSpeedTraining, NULL);
		break;
	}

	MAIN_D_80135370 = readPStat(0xf6);
	TRAINING_COMPLETE = 0;
	TRN_saveTrainingStartTime();
	MAIN_D_80135371 = 0;
}

void TRN_tickSpeedTraining(int32_t instanceId)
{
	int32_t r;

	switch (MAIN_D_80135371) {
	case 0:
		tamerSetState(8);
		unsetCameraFollowPlayer();
		partnerSetState(10);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
		createCameraMovement(&TRN_D_8008F320, 10);
		playSound(8, 9);
		MAIN_D_8013537A = 0;
		MAIN_D_80135371 = 1;
		TRN_startSlotSessionIfEnabled(4);
		break;
	case 1:
		TRN_func_800888A0(4);
		if (tickEntityWalkTo(0xfc, 0xff, TRN_D_8008F320.vx, TRN_D_8008F320.vz, 0) == 1) {
			if (instanceId == 0) {
				PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0xc00;
			} else if (instanceId == 6) {
				PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0;
			}
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
			MAIN_D_80135371 = 2;
			if (MAIN_D_80135370 == 1) {
				TRN_startSlotSpin();
			}
		}
		break;
	case 2:
		MAIN_D_8013537A++;
		if ((PARTNER_ENTITY.digimonEntity.entity.isOnScreen == 0) && ((MAIN_D_8013537A % 0x3c) == 0)) {
			if (instanceId == 0) {
				PARTNER_ENTITY.digimonEntity.entity.posData->location.vx = -0xbb8;
			} else if (instanceId == 6) {
				PARTNER_ENTITY.digimonEntity.entity.posData->location.vz = 0x7d0;
			}
			startAnimation(ENTITY_TABLE[1], 4);
			setupEntityMatrix(1);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
		}
		r = 10;
		if (MAIN_D_80135370 == 1) {
			r = TRN_getSlotSessionResult();
		}
		if ((((MAIN_D_8013537A >= 0x4b0) || (POLLED_INPUT & CANCEL_BUTTON)) && (MAIN_D_80135370 == 0)) || ((MAIN_D_80135370 == 1) && (r >= 0))) {
			TRN_awardSpeedTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, instanceId, r);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			playSound(8, 0xa);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
			MAIN_D_80135371 = 3;
		}
		break;
	case 3:
		if (TRN_statGainsAreZero() == 1) {
			MAIN_D_8013537C = 0;
			MAIN_D_80135371 = 4;
		}
		break;
	case 4:
		MAIN_D_8013537C++;
		if (MAIN_D_8013537C >= 0x14) {
			MAIN_D_8013537C = 0;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
			MAIN_D_80135371 = 5;
		}
		break;
	case 5:
		if (tickEntityWalkTo(0xfc, 0xfd, 0, 0, 0) == 1) {
			TRN_applyBaseStats();
			TRN_closeUIBox(1);
			partnerSetState(1);
			setCameraFollowPlayer();
			MAIN_D_8013537A = 0;
			MAIN_D_80135371 = 0;
			removeObject(0xfad, instanceId, TRN_tickSpeedTraining, NULL);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}
