#include <dw/anim.h>
#include <dw/entity.h>
#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/trn2.h>
#include <dw/types.h>
#include <dw/world_object.h>

extern int32_t TRAINING_COMPLETE;
extern uint32_t POLLED_INPUT;

void createCameraMovement(VECTOR *pos, int32_t speed);
void storeMapObjectPosition();
void setMapObjectsFlag(int16_t start, int16_t count, int32_t flag);
void setCameraFollowPlayer(void);
void unsetCameraFollowPlayer(void);
void createParticleFX(uint8_t kind, int32_t count, void *arg2, Entity *entity, int32_t arg4);
void resetMapObjectAnimation(int16_t startIndex, int16_t count);
void TRN2_tickOffenseTraining(int32_t instanceId);
void TRN2_func_8008AA84(int8_t arg);
void tamerSetState(int8_t state);
int32_t tickEntityWalkTo();

static void *trn2_off_functions[] = {
	TRN2_setupDefenseTraining,
	TRN2_tickOffenseTraining,
	TRN2_setupOffenseTraining,
};

int16_t MAIN_D_801353C6;

// clang-format off
int8_t TRN2_D_8008DA20[67][2] = {
	{ 0x0, 0x0 }, { 0x2e, 0xa }, { 0x2e, 0xa }, { 0x33, 0xc },
	{ 0x2e, 0x14 }, { 0x35, 0x14 }, { 0x2e, 0xa }, { 0x2e, 0xa },
	{ 0x2e, 0xa }, { 0x35, 0xd }, { 0x2e, 0xa }, { 0x2e, 0x14 },
	{ 0x31, 0xa }, { 0x2e, 0x8 }, { 0x2e, 0xa }, { 0x2e, 0xa },
	{ 0x2e, 0xa }, { 0x2e, 0x14 }, { 0x2e, 0xa }, { 0x2e, 0xa },
	{ 0x2e, 0xe }, { 0x2e, 0xf }, { 0x2e, 0xa }, { 0x2e, 0xa },
	{ 0x2e, 0xa }, { 0x2e, 0xa }, { 0x2e, 0xa }, { 0x2e, 0x8 },
	{ 0x33, 0xa }, { 0x2e, 0x14 }, { 0x2e, 0x14 }, { 0x2e, 0xa },
	{ 0x2e, 0xe }, { 0x2e, 0x14 }, { 0x31, 0xa }, { 0x2e, 0xa },
	{ 0x35, 0x14 }, { 0x2e, 0x14 }, { 0x2e, 0xa }, { 0x2e, 0x14 },
	{ 0x2e, 0xd }, { 0x2f, 0x1e }, { 0x2e, 0xf }, { 0x2e, 0xf },
	{ 0x2e, 0xa }, { 0x35, 0x17 }, { 0x2e, 0xa }, { 0x33, 0xa },
	{ 0x2e, 0xa }, { 0x2e, 0xa }, { 0x34, 0xa }, { 0x2e, 0xa },
	{ 0x2e, 0xa }, { 0x2e, 0xa }, { 0x2e, 0xa }, { 0x2e, 0xa },
	{ 0x30, 0x1e }, { 0x2e, 0xa }, { 0x2e, 0xa }, { 0x2e, 0x19 },
	{ 0x30, 0xa }, { 0x2e, 0xa }, { 0x2e, 0xa }, { 0x2e, 0xa },
	{ 0x2e, 0xa }, { 0x2e, 0xa }, { 0x2e, 0xa },
};
// clang-format on

GARBAGE_ARRAY(TRN2_setupOffenseTraining, TRN2_D_8008DC54, 8, 16);

void TRN2_setupOffenseTraining(arg)
int16_t arg;
{
	if (arg == 0x6b) {
		TRN2_D_8008DC1C.vx = -0x586;
		TRN2_D_8008DC1C.vy = 0;
		TRN2_D_8008DC1C.vz = -0x59;
		TRN2_D_8008DC3C[0].vx = -0x640;
		TRN2_D_8008DC3C[0].vy = -0xc8;
		TRN2_D_8008DC3C[0].vz = 0;
		TRN2_D_8008DC3C[1].vx = -0x640;
		TRN2_D_8008DC3C[1].vy = -0xc8;
		TRN2_D_8008DC3C[1].vz = 0x64;
		TRN2_D_8008DC3C[2].vx = -0x640;
		TRN2_D_8008DC3C[2].vy = -0xc8;
		TRN2_D_8008DC3C[2].vz = -0x64;
		MAIN_D_801353B4 = 0xc;
		MAIN_D_801353B6 = 3;
		MAIN_D_801353B8 = 0xc;
		MAIN_D_801353BA = 3;
		MAIN_D_801353BE = 0xf;
		MAIN_D_801353C0 = 1;
		addObject(0xfac, 1, (TickFunction)TRN2_tickOffenseTraining, NULL);
	}

	TRAINING_COMPLETE = 0;
	MAIN_D_801353BC = readPStat(0xf6);
	TRN2_saveTrainingStartTime();
	MAIN_D_801353BD = 0;
}

void TRN2_tickOffenseTraining(instanceId)
int16_t instanceId;
{
	int32_t r;

	switch (MAIN_D_801353BD) {
	case 0:
		storeMapObjectPosition(TRN2_D_8008DC54, TRN2_D_8008DC74, MAIN_D_801353B4, MAIN_D_801353B6);
		tamerSetState(8);
		unsetCameraFollowPlayer();
		partnerSetState(10);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
		createCameraMovement(&TRN2_D_8008DC1C, 10);
		playSound(8, 9);
		MAIN_D_801353C2 = 0;
		MAIN_D_801353BD = 1;
		TRN2_startSlotSessionIfEnabled(2);
		break;
	case 1:
		TRN2_func_8008AA84(4);
		if (tickEntityWalkTo(0xfc, 0xff, TRN2_D_8008DC1C.vx, TRN2_D_8008DC1C.vz, 0) == 1) {
			startAnimation(ENTITY_TABLE[1], TRN2_D_8008DA20[PARTNER_ENTITY.digimonEntity.entity.type][0]);
			PARTNER_ENTITY.digimonEntity.entity.anim.animFlag |= 2;
			MAIN_D_801353C6 = 0;
			PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x400;
			if (MAIN_D_801353BC == 1) {
				TRN2_startSlotSpin();
			}
			MAIN_D_801353BD = 2;
		}
		break;
	case 2:
		MAIN_D_801353C6++;
		MAIN_D_801353C2++;
		PARTNER_ENTITY.digimonEntity.entity.anim.animFlag |= 2;
		if (MAIN_D_801353C6 == TRN2_D_8008DA20[PARTNER_ENTITY.digimonEntity.entity.type][1]) {
			createParticleFX(0, 0, &TRN2_D_8008DC3C[0], NULL, 0);
			createParticleFX(0, 0, &TRN2_D_8008DC3C[1], NULL, 0);
			createParticleFX(0, 0, &TRN2_D_8008DC3C[2], NULL, 0);
			playSound(8, 0xf);
			setMapObjectsFlag(MAIN_D_801353B4, MAIN_D_801353B6, 0);
			setMapObjectsFlag(MAIN_D_801353BE, MAIN_D_801353C0, 1);
			resetMapObjectAnimation(MAIN_D_801353B4, MAIN_D_801353B4);
			MAIN_D_801353C6 = 0;
			MAIN_D_801353BD = 3;
		}
		break;
	case 3:
		MAIN_D_801353C2++;
		MAIN_D_801353C6++;
		if (MAIN_D_801353C6 == 0x2e) {
			setMapObjectsFlag(0x27, 4, 0);
			setMapObjectsFlag(MAIN_D_801353B4, MAIN_D_801353B6, 1);
			startAnimation(ENTITY_TABLE[1], TRN2_D_8008DA20[PARTNER_ENTITY.digimonEntity.entity.type][0]);
			MAIN_D_801353C6 = 0;
			MAIN_D_801353BD = 2;
		}
		r = 10;
		if (MAIN_D_801353BC == 1) {
			r = TRN2_getSlotSessionResult();
		}
		if ((((MAIN_D_801353C2 >= 0x4b0) || (POLLED_INPUT & CANCEL_BUTTON)) && (MAIN_D_801353BC == 0)) || ((r >= 0) && (MAIN_D_801353BC == 1))) {
			playSound(8, 0xa);
			setMapObjectsFlag(0x27, 4, 0);
			setMapObjectsFlag(MAIN_D_801353B4, MAIN_D_801353B6, 1);
			TRN2_awardOffenseTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, 1, r);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
			MAIN_D_801353BD = 4;
		}
		break;
	case 4:
		if (TRN2_statGainsAreZero() == 1) {
			MAIN_D_801353C4 = 0;
			MAIN_D_801353BD = 5;
		}
		break;
	case 5:
		MAIN_D_801353C4++;
		if (MAIN_D_801353C4 >= 0x14) {
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 2);
			MAIN_D_801353BD = 6;
		}
		break;
	case 6:
		if (tickEntityWalkTo(0xfc, 0xfd, 0, 0, 0) == 1) {
			TRN2_applyBaseStats();
			TRN2_closeUIBox(1);
			partnerSetState(1);
			tamerSetState(0);
			setCameraFollowPlayer();
			MAIN_D_801353C2 = 0;
			MAIN_D_801353BD = 0;
			removeObject(0xfac, instanceId);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}

void TRN2_setupDefenseTraining(arg)
int16_t arg;
{
	switch (arg) {
	case 0x6c:
		TRN2_D_8008DC1C.vx = -0x3c6;
		TRN2_D_8008DC1C.vy = 0;
		TRN2_D_8008DC1C.vz = 0x3f8;
		TRN2_D_8008DC3C[0].vx = -0x3c6;
		TRN2_D_8008DC3C[0].vy = -DIGIMON_DATA[ENTITY_TABLE[1]->type].height;
		TRN2_D_8008DC3C[0].vz = 0x3f8;
		TRN2_D_8008DC3C[1].vx = -0x3b6;
		TRN2_D_8008DC3C[1].vy = -DIGIMON_DATA[ENTITY_TABLE[1]->type].height;
		TRN2_D_8008DC3C[1].vz = 0x3e8;
		TRN2_D_8008DC3C[2].vx = -0x3a2;
		TRN2_D_8008DC3C[2].vy = -DIGIMON_DATA[ENTITY_TABLE[1]->type].height;
		TRN2_D_8008DC3C[2].vz = 0x3d4;
		MAIN_D_801353B8 = 4;
		MAIN_D_801353BA = 3;
		MAIN_D_801353BE = 7;
		MAIN_D_801353C0 = 0x31;
		addObject(0xfae, 1, (TickFunction)TRN2_tickDefenseTrainingMap108, NULL);
		break;
	case 0x63:
		TRN2_D_8008DC1C.vx = 0x63e;
		TRN2_D_8008DC1C.vy = 0;
		TRN2_D_8008DC1C.vz = -0x3bf;
		TRN2_D_8008DC3C[0].vx = 0x63e;
		TRN2_D_8008DC3C[0].vy = -DIGIMON_DATA[ENTITY_TABLE[1]->type].height;
		TRN2_D_8008DC3C[0].vz = -0x3bf;
		MAIN_D_801353B4 = 0xe;
		MAIN_D_801353B6 = 1;
		MAIN_D_801353B8 = 0xe;
		MAIN_D_801353BA = 1;
		MAIN_D_801353BE = 0x21;
		MAIN_D_801353C0 = 1;
		addObject(0xfae, 3, (TickFunction)TRN2_tickDefenseTrainingMap99, NULL);
		break;
	}

	TRAINING_COMPLETE = 0;
	MAIN_D_801353BC = readPStat(0xf6);
	TRN2_saveTrainingStartTime();
	MAIN_D_801353BD = 0;
}
