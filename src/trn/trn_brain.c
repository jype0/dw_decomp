#include <libgpu.h>

#include <dw/anim.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/tamer.h>
#include <dw/trn.h>
#include <dw/types.h>
#include <dw/ui.h>

extern uint32_t POLLED_INPUT;
extern int32_t TRAINING_COMPLETE;

void createCameraMovement(VECTOR *pos, int32_t speed);
void entityLookAtLocation(Entity *entity, VECTOR *pos);
void TRN_tickBrainsTraining(int32_t instanceId);

static void *trn_brain_functions[] = {
	TRN_tickBrainsTraining,
	TRN_setupBrainsTraining,
};

static void trn_brain__garbage__(void)
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

void TRN_setupBrainsTraining(int32_t arg)
{
	switch (arg) {
	case 0x70:
		TRN_D_8008F358.vx = -0x5ac;
		TRN_D_8008F358.vy = 0;
		TRN_D_8008F358.vz = 0x61b;
		TRN_D_8008F320.vx = -0x4e1;
		TRN_D_8008F320.vy = 0;
		TRN_D_8008F320.vz = 0x3f3;
		addObject(0xfb0, 0, (TickFunction)TRN_tickBrainsTraining, NULL);
		if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level < 3) {
			PARTNER_PARA.upgradeBrainCounter++;
		}
		TRAINING_COMPLETE = 0;
		break;
	case 0x77:
		TRN_D_8008F358.vx = -0x564;
		TRN_D_8008F358.vy = 0;
		TRN_D_8008F358.vz = 0x3c4;
		TRN_D_8008F320.vx = -0x5e3;
		TRN_D_8008F320.vy = 0;
		TRN_D_8008F320.vz = 0x2ce;
		addObject(0xfb0, 9, (TickFunction)TRN_tickBrainsTraining, NULL);
		TRAINING_COMPLETE = 0;
		break;
	}

	MAIN_D_80135370 = readPStat(0xf6);
	TRN_saveTrainingStartTime();
	MAIN_D_80135371 = 0;
}

void TRN_tickBrainsTraining(int32_t instanceId)
{
	RECT rect;
	DVECTOR pos;
	int32_t r;

	switch (MAIN_D_80135371) {
	case 0:
		setTamerState(-1);
		startAnimation(&TAMER_ENTITY.entity, 2);
		setPartnerState(10);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
		playSound(8, 9);
		createCameraMovement(&TRN_D_8008F358, 10);
		MAIN_D_8013537A = 0;
		MAIN_D_80135371 = 1;
		TRN_startSlotSessionIfEnabled(5);
		break;
	case 1:
		if (tickEntityWalkTo(0xfd, 0xff, TRN_D_8008F358.vx, TRN_D_8008F358.vz, 0) == 1) {
			startAnimation(ENTITY_TABLE[0], 0);
			entityLookAtLocation(&TAMER_ENTITY.entity, &PARTNER_ENTITY.digimonEntity.entity.posData->location);
			startAnimation(ENTITY_TABLE[1], 2);
			MAIN_D_80135371 = 2;
		}
		break;
	case 2:
		if (tickEntityWalkTo(0xfc, 0xff, TRN_D_8008F320.vx, TRN_D_8008F320.vz, 0) == 1) {
			if (instanceId == 0) {
				entityLookAtLocation(&TAMER_ENTITY.entity, &PARTNER_ENTITY.digimonEntity.entity.posData->location);
				entityLookAtLocation(&PARTNER_ENTITY.digimonEntity.entity, &TAMER_ENTITY.entity.posData->location);
				startAnimation(&TAMER_ENTITY.entity, 9);
				startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x16);
			}
			if (instanceId == 9) {
				entityLookAtLocation(&TAMER_ENTITY.entity, &PARTNER_ENTITY.digimonEntity.entity.posData->location);
				entityLookAtLocation(&PARTNER_ENTITY.digimonEntity.entity, &NPC_ENTITIES[0].digimonEntity.entity.posData->location);
				entityLookAtLocation(&NPC_ENTITIES[0].digimonEntity.entity, &PARTNER_ENTITY.digimonEntity.entity.posData->location);
				startAnimation(ENTITY_TABLE[1], 0x16);
				startAnimation(ENTITY_TABLE[2], 0x1e);
			}
			MAIN_D_80135371 = 3;
			if (MAIN_D_80135370 == 1) {
				TRN_startSlotSpin();
			}
		}
		break;
	case 3:
		MAIN_D_8013537A++;
		if (instanceId == 0) {
			if ((MAIN_D_8013537A % 0x1e) == 0) {
				playSound(8, 3);
				startAnimation(ENTITY_TABLE[0], 9);
			}
			TAMER_ENTITY.entity.anim.animFlag |= 2;
		}
		if (instanceId == 9) {
			if (NPC_ENTITIES[0].digimonEntity.entity.anim.animFrame >= NPC_ENTITIES[0].digimonEntity.entity.anim.frameCount) {
				startAnimation(&NPC_ENTITIES[0].digimonEntity.entity, 0x1e);
			}
			if ((MAIN_D_8013537A % 0x3c) == 0) {
				playSound(8, 0x12);
			}
		}
		if ((MAIN_D_8013537A % 0x23) == 0) {
			startAnimation(ENTITY_TABLE[1], 0x16);
		}
		r = 10;
		if (MAIN_D_80135370 == 1) {
			r = TRN_getSlotSessionResult();
		}
		if ((((MAIN_D_8013537A >= 0x4b0) || (POLLED_INPUT & 0x10)) && (MAIN_D_80135370 == 0)) || ((MAIN_D_80135370 == 1) && (r >= 0))) {
			TRN_awardBrainsTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, 0, r);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
			startAnimation(&NPC_ENTITIES[0].digimonEntity.entity, 0);
			MAIN_D_80135371 = 4;
			playSound(8, 0xa);
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
			TRN_applyBaseStats();
			TRN_closeUIBox(1);
			if (UI_BOX_DATA[2].state == 1) {
				getEntityScreenPos(ENTITY_TABLE[0], 0, (int16_t *)&pos);
				setRECT(&rect, pos.vx - 5, pos.vy - 5, 10, 10);
				removeAnimatedUIBox(2, &rect);
			}
			setTamerState(0);
			setPartnerState(1);
			MAIN_D_8013537A = 0;
			MAIN_D_80135371 = 0;
			removeObject(0xfb0, instanceId, TRN_tickBrainsTraining, NULL);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}
