#include <dw/anim.h>
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
void loadMapObjectPosition();
void setMapObjectsFlag(int16_t start, int16_t count, int32_t flag);
void setCameraFollowPlayer(void);
void createParticleFX(uint8_t kind, int32_t count, SVECTOR *pos, Entity *entity, int32_t lifetime);
void resetMapObjectAnimation(int16_t startIndex, int16_t count);
void entityLookAtLocation(Entity *entity, VECTOR *pos);
void tamerSetState(int8_t state);
int32_t tickEntityWalkTo();

static void *trn2_def_map108_functions[] = {
	TRN2_setupSpeedTraining,
	TRN2_tickDefenseTrainingMap108,
};

void TRN2_tickDefenseTrainingMap108(int32_t instanceId)
{
	int32_t r;

	switch (MAIN_D_801353BD) {
	case 0:
		tamerSetState(8);
		partnerSetState(10);
		startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
		createCameraMovement(&TRN2_D_8008DC1C, 10);
		MAIN_D_801353C2 = 0;
		MAIN_D_801353BD = 1;
		TRN2_startSlotSessionIfEnabled(3);
		break;
	case 1:
		if (tickEntityWalkTo(0xfc, 0xff, TRN2_D_8008DC1C.vx, TRN2_D_8008DC1C.vz, 0) == 1) {
			PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy = 0x800;
			setMapObjectsFlag(MAIN_D_801353B8, MAIN_D_801353BA, 1);
			setMapObjectsFlag(MAIN_D_801353BE, MAIN_D_801353C0, 0);
			resetMapObjectAnimation(MAIN_D_801353BE, MAIN_D_801353C0);
			MAIN_D_801353CC = 0;
			MAIN_D_801353BD = 2;
			if (MAIN_D_801353BC == 1) {
				TRN2_startSlotSpin();
			}
		}
		break;
	case 2:
		MAIN_D_801353C2++;
		MAIN_D_801353CC++;
		if (MAIN_D_801353CC == 4) {
			startAnimation(ENTITY_TABLE[1], 0x25);
			createParticleFX(0, 0, &TRN2_D_8008DC3C[0], NULL, 0);
			createParticleFX(0, 0, &TRN2_D_8008DC3C[1], NULL, 0);
			createParticleFX(0, 0, &TRN2_D_8008DC3C[2], NULL, 0);
			playSound(8, 4);
			MAIN_D_801353CC = 0;
			MAIN_D_801353BD = 3;
		}
		break;
	case 3:
		MAIN_D_801353CC++;
		if (MAIN_D_801353CC >= 0x1e) {
			resetMapObjectAnimation(MAIN_D_801353BE, MAIN_D_801353C0);
			MAIN_D_801353CC = 0;
			MAIN_D_801353BD = 2;
		}
		if (POLLED_INPUT & CANCEL_BUTTON) {
			MAIN_D_801353C2 = 0x4b0;
		}
		r = 10;
		MAIN_D_801353C2++;
		if (MAIN_D_801353BC == 1) {
			r = TRN2_getSlotSessionResult();
		}
		if (((MAIN_D_801353C2 >= 0x4b0) && (MAIN_D_801353BC == 0)) || ((MAIN_D_801353BC == 1) && (r >= 0))) {
			setMapObjectsFlag(MAIN_D_801353B8, MAIN_D_801353BA, 0);
			setMapObjectsFlag(MAIN_D_801353BE, MAIN_D_801353C0, 1);
			TRN2_awardDefenseTrainingGains(PARTNER_ENTITY.digimonEntity.entity.type, instanceId, r);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0x12);
			createCameraMovement(&TAMER_ENTITY.entity.posData->location, 10);
			playSound(8, 0xa);
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
			entityLookAtLocation(ENTITY_TABLE[1], &TAMER_ENTITY.entity.posData->location);
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 4);
			MAIN_D_801353BD = 6;
		}
		break;
	case 6:
		if (tickEntityWalkTo(0xfc, 0xfd, 0, 0, 0) == 1) {
			TRN2_applyBaseStats();
			TRN2_closeUIBox(1);
			loadMapObjectPosition(TRN2_D_8008DC54, TRN2_D_8008DC74, MAIN_D_801353B4, MAIN_D_801353B6);
			tamerSetState(0);
			setCameraFollowPlayer();
			partnerSetState(1);
			MAIN_D_801353C2 = 0;
			MAIN_D_801353BD = 0;
			removeObject(0xfae, instanceId);
			TRAINING_COMPLETE = 1;
		}
		break;
	}
}

GARBAGE(TRN2_setupSpeedTraining, 10);

void TRN2_setupSpeedTraining(arg)
int16_t arg;
{
	if (arg == 0x6c) {
		TRN2_D_8008DC2C.vx = 0xf7;
		TRN2_D_8008DC2C.vy = 0;
		TRN2_D_8008DC2C.vz = 0x226;
		TRN2_D_8008DC1C.vx = 0x372;
		TRN2_D_8008DC1C.vy = 0;
		TRN2_D_8008DC1C.vz = 0x3c5;
		MAIN_D_801353B8 = 0x38;
		MAIN_D_801353BA = 3;
		addObject(0xfad, 1, (TickFunction)TRN2_tickSpeedTraining, NULL);
	}

	MAIN_D_801353BC = readPStat(0xf6);
	TRAINING_COMPLETE = 0;
	TRN2_saveTrainingStartTime();
	MAIN_D_801353BD = 0;
}
