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
void createCloudFX(SVECTOR *pos);
void storeMapObjectPosition();
void loadMapObjectPosition();
int32_t moveMapObjectsWithLimit(int16_t startIndex, int16_t count, int16_t dx, int16_t dy, int16_t limitX, int16_t limitY);
void setCameraFollowPlayer(void);
void unsetCameraFollowPlayer(void);
void createParticleFX(uint8_t kind, int32_t count, SVECTOR *pos, Entity *entity, int32_t lifetime);
void TRN_tickDefenseTraining(int32_t instanceId);
void tamerSetState(int8_t state);
int32_t tickEntityWalkTo();

static void *trn_def_functions[] = {
	TRN_tickDefenseTraining,
	TRN_setupDefenseTraining,
};

int16_t MAIN_D_80135380;
uint32_t MAIN_D_80135384;
int32_t MAIN_D_80135388;
uint16_t TRN_CURRENT_FRAME;
int16_t TRN_CURRENT_HOUR;
int16_t TRN_CURRENT_MINUTE;
uint8_t MAIN_D_80135392;

static void *trn_def_sbss_order[] = {
	&MAIN_D_80135392,
	&TRN_CURRENT_MINUTE,
	&TRN_CURRENT_HOUR,
	&TRN_CURRENT_FRAME,
	&MAIN_D_80135388,
	&MAIN_D_80135384,
	&MAIN_D_80135380,
};

GARBAGE_ARRAY(TRN_setupDefenseTraining, TRN_D_8008F368, 8, 16);

void TRN_setupDefenseTraining(arg)
int16_t arg;
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
	TRAINING_STATE = 0;
}

void TRN_tickDefenseTraining(instanceId)
int16_t instanceId;
{
	SVECTOR pos;
	VECTOR *loc;
	int32_t r;
	int32_t done;

	loc = &PARTNER_ENTITY.digimonEntity.entity.posData->location;
	switch (TRAINING_STATE) {
	case 0:
		storeMapObjectPosition(TRN_D_8008F368, TRN_D_8008F388, MAIN_D_8013536C, MAIN_D_8013536E);
		tamerSetState(8);
		unsetCameraFollowPlayer();
		partnerSetState(10);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
		createCameraMovement(&TRN_D_8008F320, 10);
		playSound(8, 9);
		MAIN_D_8013537A = 0;
		TRAINING_STATE = 1;
		TRN_startSlotSessionIfEnabled(3);
		break;
	case 1:
		if (tickEntityWalkTo(0xfc, 0xff, TRN_D_8008F320.vx, TRN_D_8008F320.vz, 0) == 1) {
			PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x400;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x25);
			MAIN_D_80135380 = 0;
			TRAINING_STATE = 2;
			if (MAIN_D_80135370 == 1) {
				TRN_startSlotSpin();
			}
		}
		break;
	case 2:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & CANCEL_BUTTON) {
			MAIN_D_8013537A = 0x4b0;
		}
		done = moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, 0x32, 0, TRN_D_8008F368[0] + 0x64, 0);
		if (done == 1) {
			createParticleFX(0, 0, &TRN_D_8008F340[0], NULL, 0);
			createParticleFX(0, 0, &TRN_D_8008F340[1], NULL, 0);
			createParticleFX(0, 0, &TRN_D_8008F340[2], NULL, 0);
			MAIN_D_80135384 = playSound2(8, 4);
			TRAINING_STATE = 3;
		}
		break;
	case 3:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & CANCEL_BUTTON) {
			MAIN_D_8013537A = 0x4b0;
		}
		loc->vx += 0x64;
		PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x400;
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x25);
		pos.vx = loc->vx;
		pos.vy = 0;
		pos.vz = loc->vz - 0x64;
		createCloudFX(&pos);
		done = moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, -0x32, 0, TRN_D_8008F368[0] + 0x32, 0);
		if (done == 1) {
			TRAINING_STATE = 4;
		}
		break;
	case 4:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & CANCEL_BUTTON) {
			MAIN_D_8013537A = 0x4b0;
		}
		loc->vx += 0x64;
		PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x400;
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x25);
		pos.vx = loc->vx;
		pos.vy = 0;
		pos.vz = loc->vz - 0x64;
		createCloudFX(&pos);
		done = moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, 0x32, 0, TRN_D_8008F368[0] + 0x5a, 0);
		if (done == 1) {
			TRAINING_STATE = 5;
		}
		break;
	case 5:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & CANCEL_BUTTON) {
			MAIN_D_8013537A = 0x4b0;
		}
		done = moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, -0x1e, 0, TRN_D_8008F368[0] + 0x3c, 0);
		if (done == 1) {
			TRAINING_STATE = 6;
		}
		break;
	case 6:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & CANCEL_BUTTON) {
			MAIN_D_8013537A = 0x4b0;
		}
		done = moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, 0x14, 0, TRN_D_8008F368[0] + 0x5a, 0);
		if (done == 1) {
			TRAINING_STATE = 7;
		}
		break;
	case 7:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & CANCEL_BUTTON) {
			MAIN_D_8013537A = 0x4b0;
		}
		done = moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, -0xa, 0, TRN_D_8008F368[0] + 0x46, 0);
		if (done == 1) {
			TRAINING_STATE = 8;
		}
		break;
	case 8:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & CANCEL_BUTTON) {
			MAIN_D_8013537A = 0x4b0;
		}
		done = moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, 5, 0, TRN_D_8008F368[0] + 0x5a, 0);
		if (done == 1) {
			TRAINING_STATE = 9;
		}
		break;
	case 9:
		MAIN_D_8013537A++;
		if (POLLED_INPUT & CANCEL_BUTTON) {
			MAIN_D_8013537A = 0x4b0;
		}
		done = moveMapObjectsWithLimit(MAIN_D_8013536C, MAIN_D_8013536E, -4, 0, TRN_D_8008F368[0], 0);
		if (done == 1) {
			loadMapObjectPosition(TRN_D_8008F368, TRN_D_8008F388, MAIN_D_8013536C, MAIN_D_8013536E);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
			TRAINING_STATE = 0xa;
		}
		break;
	case 10:
		if (POLLED_INPUT & CANCEL_BUTTON) {
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
				thunkStopSoundMask(MAIN_D_80135384);
				createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
				TRAINING_STATE = 0xc;
			} else {
				PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x400;
				startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x25);
				TRAINING_STATE = 2;
			}
		}
		break;
	case 12:
		if (TRN_statGainsAreZero() == 1) {
			MAIN_D_8013537C = 0;
			TRAINING_STATE = 0xd;
		}
		break;
	case 13:
		MAIN_D_8013537C++;
		if (MAIN_D_8013537C >= 0x14) {
			MAIN_D_8013537C = 0;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
			TRAINING_STATE = 0xe;
		}
		break;
	case 14:
		if (tickEntityWalkTo(0xfc, 0xfd, 0, 0, 0) == 1) {
			TRN_applyBaseStats();
			TRN_closeUIBox(1);
			loadMapObjectPosition(TRN_D_8008F368, TRN_D_8008F388, MAIN_D_8013536C, MAIN_D_8013536E);
			tamerSetState(0);
			setCameraFollowPlayer();
			partnerSetState(1);
			MAIN_D_8013537A = 0;
			TRAINING_STATE = 0;
			removeObject(0xfae, instanceId, TRN_tickDefenseTraining, NULL);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}
