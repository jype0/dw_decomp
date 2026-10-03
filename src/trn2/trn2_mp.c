#include <dw/anim.h>
#include <dw/entity.h>
#include <dw/input.h>
#include <dw/partner.h>
#include <dw/sound.h>
#include <dw/training.h>
#include <dw/trn2.h>
#include <dw/types.h>
#include <dw/world_object.h>

extern int32_t TRAINING_COMPLETE;
extern uint32_t POLLED_INPUT;

void createCameraMovement(VECTOR *pos, int32_t speed);
void tamerSetState(int8_t state);
int32_t tickEntityWalkTo();

static void *trn2_mp_functions[] = {
	TRN2_tickMpTraining,
};

void TRN2_tickMpTraining(instanceId)
int16_t instanceId;
{
	int32_t r;

	switch (MAIN_D_801353BD) {
	case 0:
		tamerSetState(8);
		partnerSetState(10);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
		createCameraMovement(&TRN2_D_8008DC1C, 10);
		playSound(8, 9);
		MAIN_D_801353C2 = 0;
		MAIN_D_801353BD = 1;
		TRN2_startSlotSessionIfEnabled(1);
		break;
	case 1:
		if (tickEntityWalkTo(0xfc, 0xff, TRN2_D_8008DC2C.vx, TRN2_D_8008DC2C.vz, 0) == 1) {
			MAIN_D_801353BD = 2;
		}
		break;
	case 2:
		if (tickEntityWalkTo(0xfc, 0xff, TRN2_D_8008DC1C.vx, TRN2_D_8008DC1C.vz, 0) == 1) {
			PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x400;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0xc);
			MAIN_D_801353BD = 3;
			if (MAIN_D_801353BC == 1) {
				TRN2_startSlotSpin();
			}
		}
		break;
	case 3:
		MAIN_D_801353C2++;
		r = 10;
		if (PARTNER_ENTITY.digimonEntity.entity.anim.animFrame <= PARTNER_ENTITY.digimonEntity.entity.anim.frameCount) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0xc);
		}
		if (MAIN_D_801353BC == 1) {
			r = TRN2_getSlotSessionResult();
		}
		if ((((MAIN_D_801353C2 >= 0x4b0) || (POLLED_INPUT & CANCEL_BUTTON)) && (MAIN_D_801353BC == 0)) || ((MAIN_D_801353BC == 1) && (r >= 0))) {
			playSound(8, 0xa);
			TRN2_awardMpTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, 8, r);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
			MAIN_D_801353BD = 4;
			MAIN_D_801353C4 = 0;
		}
		break;
	case 4:
		if (tickEntityWalkTo(0xfc, 0xff, TRN2_D_8008DC2C.vx, TRN2_D_8008DC2C.vz, 0) == 1) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			MAIN_D_801353BD = 5;
		}
		break;
	case 5:
		if (TRN2_statGainsAreZero() == 1) {
			MAIN_D_801353C4 = 0;
			MAIN_D_801353BD = 6;
		}
		break;
	case 6:
		MAIN_D_801353C4++;
		if (MAIN_D_801353C4 >= 0x14) {
			TRN2_applyBaseStats();
			TRN2_closeUIBox(1);
			tamerSetState(0);
			partnerSetState(1);
			MAIN_D_801353C2 = 0;
			MAIN_D_801353BD = 0;
			/*
			 * BUG: reference to TRN_tickMpTraining in TRN
			 * overlay. The function only takes two parameters so
			 * the parameter is unused.
			 */
			removeObject(0xfaf, instanceId, TRN_tickMpTraining, NULL);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}
