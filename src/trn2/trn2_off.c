#include <dw/anim.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/tamer.h>
#include <dw/trn2.h>
#include <dw/types.h>
#include <dw/world_object.h>

extern int32_t TRAINING_COMPLETE;
extern uint32_t POLLED_INPUT;

void createCameraMovement(VECTOR *pos, int32_t speed);
void storeMapObjectPosition(int16_t *outX, int16_t *outY, int16_t a, int16_t count);
void setMapObjectsFlag(int16_t start, int16_t count, int32_t flag);
void setCameraFollowPlayer(void);
void unsetCameraFollowPlayer(void);
void createParticleFX(uint8_t kind, int32_t count, void *arg2, Entity *entity, int32_t arg4);
void resetMapObjectAnimation(int16_t startIndex, int16_t count);
void TRN2_tickOffenseTraining(int32_t instanceId);
void TRN2_func_8008AA84(int8_t arg);

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

static void trn2_off__garbage__(void)
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

	v0 = TRN2_D_8008DC54[0] + 0;
	v1 = TRN2_D_8008DC54[1] + 1;
	v2 = TRN2_D_8008DC54[2] + 2;
	v3 = TRN2_D_8008DC54[3] + 3;
	v4 = TRN2_D_8008DC54[4] + 4;
	v5 = TRN2_D_8008DC54[5] + 5;
	v6 = TRN2_D_8008DC54[6] + 6;
	v7 = TRN2_D_8008DC54[7] + 7;
	v8 = TRN2_D_8008DC54[0] + 8;
	v9 = TRN2_D_8008DC54[1] + 9;
	v10 = TRN2_D_8008DC54[2] + 10;
	v11 = TRN2_D_8008DC54[3] + 11;
	v12 = TRN2_D_8008DC54[4] + 12;
	v13 = TRN2_D_8008DC54[5] + 13;
	v14 = TRN2_D_8008DC54[6] + 14;
	v15 = TRN2_D_8008DC54[7] + 15;
	v16 = TRN2_D_8008DC54[0] + 16;
	v17 = TRN2_D_8008DC54[1] + 17;
	v18 = TRN2_D_8008DC54[2] + 18;
	v19 = TRN2_D_8008DC54[3] + 19;
	TRN2_D_8008DC54[0] = (v0 * v1) + v2;
	TRN2_D_8008DC54[1] = (v1 * v2) + v3;
	TRN2_D_8008DC54[2] = (v2 * v3) + v4;
	TRN2_D_8008DC54[3] = (v3 * v4) + v5;
	TRN2_D_8008DC54[4] = (v4 * v5) + v6;
	TRN2_D_8008DC54[5] = (v5 * v6) + v7;
	TRN2_D_8008DC54[6] = (v6 * v7) + v8;
	TRN2_D_8008DC54[7] = (v7 * v8) + v9;
	TRN2_D_8008DC54[0] = (v8 * v9) + v10;
	TRN2_D_8008DC54[1] = (v9 * v10) + v11;
	TRN2_D_8008DC54[2] = (v10 * v11) + v12;
	TRN2_D_8008DC54[3] = (v11 * v12) + v13;
	TRN2_D_8008DC54[4] = (v12 * v13) + v14;
	TRN2_D_8008DC54[5] = (v13 * v14) + v15;
	TRN2_D_8008DC54[6] = (v14 * v15) + v16;
	TRN2_D_8008DC54[7] = (v15 * v16) + v17;
	TRN2_D_8008DC54[0] = (v16 * v17) + v18;
	TRN2_D_8008DC54[1] = (v17 * v18) + v19;
	TRN2_D_8008DC54[2] = (v18 * v19) + v0;
	TRN2_D_8008DC54[3] = (v19 * v0) + v1;
}

void TRN2_setupOffenseTraining(int32_t arg)
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

void TRN2_tickOffenseTraining(int32_t instanceId)
{
	int32_t r;

	switch (MAIN_D_801353BD) {
	case 0:
		storeMapObjectPosition(TRN2_D_8008DC54, TRN2_D_8008DC74, MAIN_D_801353B4, MAIN_D_801353B6);
		setTamerState(8);
		unsetCameraFollowPlayer();
		setPartnerState(10);
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
			MAIN_D_801353C6 = 0;
			PARTNER_ENTITY.digimonEntity.entity.anim.animFlag |= 2;
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
		if ((((MAIN_D_801353C2 >= 0x4b0) || (POLLED_INPUT & 0x10)) && (MAIN_D_801353BC == 0)) || ((r >= 0) && (MAIN_D_801353BC == 1))) {
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
			setPartnerState(1);
			setTamerState(0);
			setCameraFollowPlayer();
			MAIN_D_801353C2 = 0;
			MAIN_D_801353BD = 0;
			removeObject(0xfac, instanceId);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}

void TRN2_setupDefenseTraining(int32_t arg)
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
