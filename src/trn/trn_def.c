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
void createCloudFX(int16_t *pos);
void storeMapObjectPosition(int16_t *outX, int16_t *outY, int16_t a, int16_t count);
void loadMapObjectPosition(int16_t *xData, int16_t *yData, int16_t startIndex, int16_t count);
int32_t moveMapObjectsWithLimit(int16_t startIndex, int16_t count, int16_t dx, int16_t dy, int16_t limitX, int16_t limitY);
void setCameraFollowPlayer(void);
void unsetCameraFollowPlayer(void);
void createParticleFX(uint8_t kind, int32_t count, void *arg2, Entity *entity, int32_t arg4);
void TRN_tickDefenseTraining(int32_t instanceId);

static void *trn_def_functions[] = {
	TRN_tickDefenseTraining,
	TRN_setupDefenseTraining,
};

int16_t MAIN_D_80135380;
uint32_t MAIN_D_80135384;
int32_t MAIN_D_80135388;
uint16_t MAIN_D_8013538C;
int16_t MAIN_D_8013538E;
int16_t MAIN_D_80135390;
uint8_t MAIN_D_80135392;

static void *trn_def_sbss_order[] = {
	&MAIN_D_80135392,
	&MAIN_D_80135390,
	&MAIN_D_8013538E,
	&MAIN_D_8013538C,
	&MAIN_D_80135388,
	&MAIN_D_80135384,
	&MAIN_D_80135380,
};

static void trn_def__garbage__(void)
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

void TRN_setupDefenseTraining(int32_t arg)
{
	if (arg == 0x70) {
		TRN_D_8008F330.vx = -0x26b;
		TRN_D_8008F330.vy = 0;
		TRN_D_8008F330.vz = -0x602;
		TRN_D_8008F320.vx = -0x33b;
		TRN_D_8008F320.vy = 0;
		TRN_D_8008F320.vz = -0x662;
		TRN_D_8008F340[0].vx = -0x384;
		TRN_D_8008F340[0].vy = -0x1f4;
		TRN_D_8008F340[0].vz = -0x7d0;
		TRN_D_8008F340[1].vx = -0x3b6;
		TRN_D_8008F340[1].vy = -0x1f4;
		TRN_D_8008F340[1].vz = -0x76c;
		TRN_D_8008F340[2].vx = -0x3b6;
		TRN_D_8008F340[2].vy = -0x1f4;
		TRN_D_8008F340[2].vz = -0x834;
		MAIN_D_8013536C = 0x47;
		MAIN_D_8013536E = 0xd;
		addObject(0xfae, 0, (TickFunction)TRN_tickDefenseTraining, NULL);
		if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level < 3) {
			PARTNER_PARA.upgradeDefenseCounter++;
		}
		TRAINING_COMPLETE = 0;
	}

	MAIN_D_80135370 = readPStat(0xf6);
	TRN_saveTrainingStartTime();
	MAIN_D_80135371 = 0;
}

void TRN_tickDefenseTraining(int32_t instanceId)
{
	int16_t pos[3];
	VECTOR *loc;
	int32_t r;

	loc = &PARTNER_ENTITY.digimonEntity.entity.posData->location;
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
		TRN_startSlotSessionIfEnabled(3);
		break;
	case 1:
		if (tickEntityWalkTo(0xfc, 0xff, TRN_D_8008F320.vx, TRN_D_8008F320.vz, 0) == 1) {
			PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x400;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x25);
			MAIN_D_80135371 = 2;
			MAIN_D_80135380 = 0;
			if (MAIN_D_80135370 == 1) {
				TRN_startSlotSpin();
			}
		}
		break;
	case 2:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & 0x10) {
			MAIN_D_8013537A = 0x4b0;
		}
		if (moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, 0x32, 0, TRN_D_8008F368[0] + 0x64, 0) == 1) {
			createParticleFX(0, 0, &TRN_D_8008F340[0], NULL, 0);
			createParticleFX(0, 0, &TRN_D_8008F340[1], NULL, 0);
			createParticleFX(0, 0, &TRN_D_8008F340[2], NULL, 0);
			MAIN_D_80135384 = playSound2(8, 4);
			MAIN_D_80135371 = 3;
		}
		break;
	case 3:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & 0x10) {
			MAIN_D_8013537A = 0x4b0;
		}
		loc->vx += 0x64;
		PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x400;
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x25);
		pos[0] = loc->vx;
		pos[1] = 0;
		pos[2] = loc->vz - 0x64;
		createCloudFX(pos);
		if (moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, -0x32, 0, TRN_D_8008F368[0] + 0x32, 0) == 1) {
			MAIN_D_80135371 = 4;
		}
		break;
	case 4:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & 0x10) {
			MAIN_D_8013537A = 0x4b0;
		}
		loc->vx += 0x64;
		PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x400;
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x25);
		pos[0] = loc->vx;
		pos[1] = 0;
		pos[2] = loc->vz - 0x64;
		createCloudFX(pos);
		if (moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, 0x32, 0, TRN_D_8008F368[0] + 0x5a, 0) == 1) {
			MAIN_D_80135371 = 5;
		}
		break;
	case 5:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & 0x10) {
			MAIN_D_8013537A = 0x4b0;
		}
		if (moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, -0x1e, 0, TRN_D_8008F368[0] + 0x3c, 0) == 1) {
			MAIN_D_80135371 = 6;
		}
		break;
	case 6:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & 0x10) {
			MAIN_D_8013537A = 0x4b0;
		}
		if (moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, 0x14, 0, TRN_D_8008F368[0] + 0x5a, 0) == 1) {
			MAIN_D_80135371 = 7;
		}
		break;
	case 7:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & 0x10) {
			MAIN_D_8013537A = 0x4b0;
		}
		if (moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, -0xa, 0, TRN_D_8008F368[0] + 0x46, 0) == 1) {
			MAIN_D_80135371 = 8;
		}
		break;
	case 8:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & 0x10) {
			MAIN_D_8013537A = 0x4b0;
		}
		if (moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, 5, 0, TRN_D_8008F368[0] + 0x5a, 0) == 1) {
			MAIN_D_80135371 = 9;
		}
		break;
	case 9:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & 0x10) {
			MAIN_D_8013537A = 0x4b0;
		}
		if (moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, -4, 0, TRN_D_8008F368[0], 0) == 1) {
			loadMapObjectPosition(TRN_D_8008F368, TRN_D_8008F388, MAIN_D_8013536C, MAIN_D_8013536E);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
			MAIN_D_80135371 = 0xa;
		}
		break;
	case 10:
		if (POLLED_INPUT & 0x10) {
			MAIN_D_8013537A = 0x4b0;
		}
		if (tickEntityWalkTo(0xfc, 0xff, TRN_D_8008F320.vx, TRN_D_8008F320.vz, 0) == 1) {
			r = 10;
			if (MAIN_D_80135370 == 1) {
				r = TRN_getSlotSessionResult();
			}
			if (((MAIN_D_8013537A >= 0x4b0) && (MAIN_D_80135370 == 0)) || ((MAIN_D_80135370 == 1) && (r >= 0))) {
				TRN_awardDefenseTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, 0, r);
				startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
				playSound(8, 0xa);
				stopSoundMask(MAIN_D_80135384);
				createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
				MAIN_D_80135371 = 0xc;
			} else {
				PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x400;
				startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x25);
				MAIN_D_80135371 = 2;
			}
		}
		break;
	case 12:
		if (TRN_statGainsAreZero() == 1) {
			MAIN_D_8013537C = 0;
			MAIN_D_80135371 = 0xd;
		}
		break;
	case 13:
		MAIN_D_8013537C++;
		if (MAIN_D_8013537C >= 0x14) {
			MAIN_D_8013537C = 0;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
			MAIN_D_80135371 = 0xe;
		}
		break;
	case 14:
		if (tickEntityWalkTo(0xfc, 0xfd, 0, 0, 0) == 1) {
			TRN_applyBaseStats();
			TRN_closeUIBox(1);
			loadMapObjectPosition(TRN_D_8008F368, TRN_D_8008F388, MAIN_D_8013536C, MAIN_D_8013536E);
			setTamerState(0);
			setCameraFollowPlayer();
			setPartnerState(1);
			MAIN_D_8013537A = 0;
			MAIN_D_80135371 = 0;
			removeObject(0xfae, instanceId, TRN_tickDefenseTraining, NULL);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}
