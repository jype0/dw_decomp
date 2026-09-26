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
extern int8_t TRAINING_ANIM_IDS[][2];

void createCameraMovement(VECTOR *pos, int32_t speed);
void storeMapObjectPosition(int16_t *outX, int16_t *outY, int16_t a, int16_t count);
void loadMapObjectPosition(int16_t *xData, int16_t *yData, int16_t startIndex, int16_t count);
int32_t moveMapObjectsWithLimit(int16_t startIndex, int16_t count, int16_t dx, int16_t dy, int16_t limitX, int16_t limitY);
void setCameraFollowPlayer(void);
void unsetCameraFollowPlayer(void);
void setMapObjectsFlag(int16_t start, int16_t count, int32_t flag);
void createParticleFX(uint8_t kind, int32_t count, void *arg2, Entity *entity, int32_t arg4);
void resetMapObjectAnimation(int16_t startIndex, int32_t count);
void TRN_func_800888A0(int8_t arg);
void TRN_tickOffenseTraining(int32_t instanceId);

static void *trn_off_functions[] = {
	TRN_tickOffenseTraining,
	TRN_setupOffenseTraining,
};

int16_t MAIN_D_8013537E;

void TRN_setupOffenseTraining(int32_t arg)
{
	switch (arg) {
	case 0x70:
		TRN_D_8008F330.vx = -0x12f;
		TRN_D_8008F330.vy = 0;
		TRN_D_8008F330.vz = -0xa1a;
		TRN_D_8008F320.vx = -0x3d6;
		TRN_D_8008F320.vy = 0;
		TRN_D_8008F320.vz = -0xa14;
		TRN_D_8008F340[0].vx = -0x44c;
		TRN_D_8008F340[0].vy = -0x258;
		TRN_D_8008F340[0].vz = -0xb22;
		TRN_D_8008F340[1].vx = -0x44c;
		TRN_D_8008F340[1].vy = -0x1f4;
		TRN_D_8008F340[1].vz = -0xb22;
		TRN_D_8008F340[2].vx = -0x44c;
		TRN_D_8008F340[2].vy = -0x190;
		TRN_D_8008F340[2].vz = -0xb22;
		MAIN_D_8013536C = 0x64;
		MAIN_D_8013536E = 8;
		if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level < 3) {
			PARTNER_PARA.upgradeOffCounter++;
		}
		addObject(0xfac, 0, (TickFunction)TRN_tickOffenseTraining, NULL);
		break;
	case 0x4e:
		TRN_D_8008F330.vx = -0x527;
		TRN_D_8008F330.vy = 0;
		TRN_D_8008F330.vz = 0xb85;
		TRN_D_8008F320.vx = -0x527;
		TRN_D_8008F320.vy = 0;
		TRN_D_8008F320.vz = 0xcd9;
		TRN_D_8008F340[0].vx = -0x4b0;
		TRN_D_8008F340[0].vy = -0xc8;
		TRN_D_8008F340[0].vz = 0xdac;
		TRN_D_8008F340[1].vx = -0x514;
		TRN_D_8008F340[1].vy = -0xc8;
		TRN_D_8008F340[1].vz = 0xdac;
		TRN_D_8008F340[2].vx = -0x578;
		TRN_D_8008F340[2].vy = -0xc8;
		MAIN_D_8013536C = 0x2b;
		TRN_D_8008F340[2].vz = 0xdac;
		MAIN_D_8013536E = 0x1e;
		addObject(0xfac, 6, (TickFunction)TRN_tickOffenseTraining, NULL);
		break;
	}

	TRAINING_COMPLETE = 0;
	MAIN_D_80135370 = readPStat(0xf6);
	TRN_saveTrainingStartTime();
	MAIN_D_80135371 = 0;
}

void TRN_tickOffenseTraining(int32_t instanceId)
{
	int32_t r;

	switch (MAIN_D_80135371) {
	case 0:
		storeMapObjectPosition(TRN_D_8008F368, TRN_D_8008F388, MAIN_D_8013536C, MAIN_D_8013536E);
		setTamerState(8);
		unsetCameraFollowPlayer();
		setPartnerState(10);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
		createCameraMovement(&TRN_D_8008F320, 10);
		playSound(8, 9);
		MAIN_D_8013537A = 0;
		MAIN_D_80135371 = 1;
		TRN_startSlotSessionIfEnabled(2);
		break;
	case 1:
		TRN_func_800888A0(4);
		if (tickEntityWalkTo(0xfc, 0xff, TRN_D_8008F320.vx, TRN_D_8008F320.vz, 0) == 1) {
			startAnimation(ENTITY_TABLE[1], TRAINING_ANIM_IDS[PARTNER_ENTITY.digimonEntity.entity.type][0]);
			MAIN_D_8013537E = 0;
			PARTNER_ENTITY.digimonEntity.entity.anim.animFlag |= 2;
			if (instanceId == 0) {
				PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x200;
				MAIN_D_80135371 = 2;
			} else if (instanceId == 6) {
				PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x800;
				MAIN_D_80135371 = 7;
			}
			if (MAIN_D_80135370 == 1) {
				TRN_startSlotSpin();
			}
		}
		break;
	case 2:
		MAIN_D_8013537E++;
		MAIN_D_8013537A++;
		PARTNER_ENTITY.digimonEntity.entity.anim.animFlag |= 2;
		if (MAIN_D_8013537E == TRAINING_ANIM_IDS[PARTNER_ENTITY.digimonEntity.entity.type][1]) {
			createParticleFX(0, 0, &TRN_D_8008F340[0], NULL, 0);
			createParticleFX(0, 0, &TRN_D_8008F340[1], NULL, 0);
			createParticleFX(0, 0, &TRN_D_8008F340[2], NULL, 0);
			MAIN_D_8013537E = 0;
			playSound(8, 5);
			MAIN_D_80135371 = 3;
		}
		break;
	case 3:
		MAIN_D_8013537E++;
		MAIN_D_8013537A++;
		if (MAIN_D_8013537E < 4) {
			moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, -4, 2, 0, 0);
		} else if (MAIN_D_8013537E < 7) {
			moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, 4, -2, 0, 0);
		} else if ((MAIN_D_8013537A % 2) == 0) {
			moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, (100 - MAIN_D_8013537E) / 20, -(100 - MAIN_D_8013537E) / 50, 0, 0);
		} else {
			moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, -(100 - MAIN_D_8013537E) / 20, (100 - MAIN_D_8013537E) / 50, 0, 0);
		}
		if (MAIN_D_8013537E >= 0x29) {
			MAIN_D_8013537E = 0;
			startAnimation(ENTITY_TABLE[1], TRAINING_ANIM_IDS[PARTNER_ENTITY.digimonEntity.entity.type][0]);
			MAIN_D_80135371 = 2;
		}
		r = 10;
		if (MAIN_D_80135370 == 1) {
			r = TRN_getSlotSessionResult();
		}
		if ((((MAIN_D_8013537A >= 0x4b0) || (POLLED_INPUT & 0x10)) && (MAIN_D_80135370 == 0)) || ((r >= 0) && (MAIN_D_80135370 == 1))) {
			TRN_awardOffenseTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, 0, r);
			startAnimation(ENTITY_TABLE[1], 0x12);
			loadMapObjectPosition(TRN_D_8008F368, TRN_D_8008F388, MAIN_D_8013536C, MAIN_D_8013536E);
			playSound(8, 0xa);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
			MAIN_D_80135371 = 4;
		}
		break;
	case 4:
		if (TRN_statGainsAreZero() == 1) {
			MAIN_D_8013537C = 0;
			MAIN_D_80135371 = 5;
		}
		break;
	case 5:
		MAIN_D_8013537C++;
		if (MAIN_D_8013537C >= 0x14) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
			MAIN_D_80135371 = 6;
		}
		break;
	case 6:
		if (tickEntityWalkTo(0xfc, 0xfd, 0, 0, 0) == 1) {
			TRN_applyBaseStats();
			TRN_closeUIBox(1);
			setPartnerState(1);
			setTamerState(0);
			setCameraFollowPlayer();
			MAIN_D_8013537A = 0;
			MAIN_D_80135371 = 0;
			removeObject(0xfac, instanceId, TRN_tickOffenseTraining, NULL);
			TRAINING_COMPLETE = 1;
		}
		break;
	case 7:
		MAIN_D_8013537E++;
		MAIN_D_8013537A++;
		if (MAIN_D_8013537E == TRAINING_ANIM_IDS[PARTNER_ENTITY.digimonEntity.entity.type][1]) {
			createParticleFX(0, 0, &TRN_D_8008F340[0], NULL, 0);
			createParticleFX(0, 0, &TRN_D_8008F340[1], NULL, 0);
			createParticleFX(0, 0, &TRN_D_8008F340[2], NULL, 0);
			playSound(8, 0x10);
			setMapObjectsFlag(0x27, 4, 1);
			setMapObjectsFlag(MAIN_D_8013536C, MAIN_D_8013536E, 0);
			resetMapObjectAnimation(MAIN_D_8013536C, MAIN_D_8013536C);
			MAIN_D_8013537E = 0;
			MAIN_D_80135371 = 8;
		}
		break;
	case 8:
		MAIN_D_8013537A++;
		MAIN_D_8013537E++;
		if (MAIN_D_8013537E == 0x2e) {
			setMapObjectsFlag(0x27, 4, 0);
			setMapObjectsFlag(MAIN_D_8013536C, MAIN_D_8013536E, 1);
			startAnimation(ENTITY_TABLE[1], TRAINING_ANIM_IDS[PARTNER_ENTITY.digimonEntity.entity.type][0]);
			MAIN_D_8013537E = 0;
			MAIN_D_80135371 = 7;
		}
		r = 10;
		if (MAIN_D_80135370 == 1) {
			r = TRN_getSlotSessionResult();
		}
		if ((((MAIN_D_8013537A >= 0x4b0) || (POLLED_INPUT & 0x10)) && (MAIN_D_80135370 == 0)) || ((r >= 0) && (MAIN_D_80135370 == 1))) {
			setMapObjectsFlag(0x27, 4, 0);
			setMapObjectsFlag(MAIN_D_8013536C, MAIN_D_8013536E, 1);
			TRN_awardOffenseTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, 6, r);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
			playSound(8, 0xa);
			MAIN_D_80135371 = 4;
			MAIN_D_8013537C = 0;
		}
		break;
	}
}
