#include <dw/anim.h>
#include <dw/entity.h>
#include <dw/partner.h>
#include <dw/sound.h>
#include <dw/tamer.h>
#include <dw/trn2.h>
#include <dw/types.h>
#include <dw/world_object.h>

extern int32_t TRAINING_COMPLETE;
extern uint32_t POLLED_INPUT;

void createCameraMovement(VECTOR *pos, int32_t speed);
void removeAllCloudFX(void);
void storeMapObjectPosition(int16_t *outX, int16_t *outY, int16_t a, int16_t count);
void loadMapObjectPosition(int16_t *xData, int16_t *yData, int16_t startIndex, int16_t count);
int32_t moveMapObjectsWithLimit(int16_t startIndex, int16_t count, int16_t dx, int16_t dy, int16_t limitX, int16_t limitY);
void setMapObjectsFlag(int16_t start, int16_t count, int32_t flag);
void TRN2_func_8008AA84(int8_t arg);

static void *trn2_hp_map99_functions[] = {
	TRN2_tickHpTrainingMap99,
};

int16_t MAIN_D_801353B4;
int16_t MAIN_D_801353B6;
int16_t MAIN_D_801353B8;
int16_t MAIN_D_801353BA;
int8_t MAIN_D_801353BC;
int8_t MAIN_D_801353BD;

static void *trn2_hp_map99_sbss_order[] = {
	&MAIN_D_801353BD,
	&MAIN_D_801353BC,
	&MAIN_D_801353BA,
	&MAIN_D_801353B8,
	&MAIN_D_801353B6,
	&MAIN_D_801353B4,
};

void TRN2_tickHpTrainingMap99(int32_t instanceId)
{
	int32_t r;

	switch (MAIN_D_801353BD) {
	case 0:
		storeMapObjectPosition(TRN2_D_8008DC54, TRN2_D_8008DC74, MAIN_D_801353B4, MAIN_D_801353B6);
		setTamerState(8);
		setPartnerState(10);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
		createCameraMovement(&TRN2_D_8008DC1C, 10);
		MAIN_D_801353C2 = 0;
		MAIN_D_801353BD = 1;
		TRN2_startSlotSessionIfEnabled(0);
		break;
	case 1:
		TRN2_func_8008AA84(4);
		if (tickEntityWalkTo(0xfc, 0xff, TRN2_D_8008DC1C.vx, TRN2_D_8008DC1C.vz, 0) == 1) {
			startAnimation(ENTITY_TABLE[1], 0x1b);
			PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x400;
			setMapObjectsFlag(MAIN_D_801353B8, MAIN_D_801353BA, 0);
			if (MAIN_D_801353BC == 1) {
				TRN2_startSlotSpin();
			}
			MAIN_D_801353BD = 2;
		}
		break;
	case 2:
		PARTNER_ENTITY.digimonEntity.entity.anim.animFlag |= 2;
		if (((MAIN_D_801353C2 % 0x78) == 0) && (MAIN_D_801353C2 < 0x4b1)) {
			PARTNER_ENTITY.digimonEntity.entity.anim.animFlag &= 0xfd;
			moveMapObjectsWithLimit(MAIN_D_801353B4, MAIN_D_801353B6, -1, -1, 0, 0);
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
		if ((((MAIN_D_801353C2 >= 0x4b0) || (POLLED_INPUT & 0x10)) && (MAIN_D_801353BC == 0)) || ((MAIN_D_801353BC == 1) && (0 <= r))) {
			MAIN_D_801353C2 %= 0x4b0;
			playSound(8, 0xa);
			MAIN_D_801353BD = 3;
			MAIN_D_801353C2 = MAIN_D_801353C2 / 120;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			TRN2_awardHpTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, 3, r);
			setMapObjectsFlag(MAIN_D_801353B8, MAIN_D_801353BA, 1);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
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
			moveMapObjectsWithLimit(MAIN_D_801353B4, MAIN_D_801353B6, 1, 1, 0, 0);
		}
		if ((tickEntityWalkTo(0xfc, 0xfd, 0, 0, 0) == 1) && (MAIN_D_801353C2 <= 0)) {
			TRN2_applyBaseStats();
			TRN2_closeUIBox(1);
			setPartnerState(1);
			loadMapObjectPosition(TRN2_D_8008DC54, TRN2_D_8008DC74, MAIN_D_801353B4, MAIN_D_801353B6);
			setTamerState(0);
			removeAllCloudFX();
			MAIN_D_801353C2 = 0;
			MAIN_D_801353BD = 0;
			removeObject(0xfab, instanceId);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}
