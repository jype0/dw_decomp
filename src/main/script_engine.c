#include <string.h>

#include <dw/file.h>
#include <dw/map_object.h>
#include <dw/partner.h>
#include <dw/script.h>

int32_t tickLookAtEntity(uint32_t scriptId1, uint32_t scriptId2);
int32_t tickEntitySetRotation(uint32_t scriptId, int16_t rotationY);
int32_t tickEntityWalkTo(uint32_t scriptId1, uint32_t scriptId2,
			 int32_t targetX, int32_t targetZ,
			 int32_t withCamera);
int32_t tickMoveCameraTo(int32_t x, int32_t y, uint8_t speed);
int32_t tickMoveCameraToEntity(uint32_t scriptId, uint8_t speed);
int32_t tickEntityMoveTo(uint32_t scriptId1, uint32_t scriptId2,
			 int32_t targetX, int32_t targetZ, int32_t speed,
			 int32_t withCamera);
int32_t tickMoveObjectTo(uint32_t scriptId1, uint32_t scriptId2,
			 int32_t angle, int32_t targetX, int32_t targetY);
int32_t tickEntityMoveToAxis(uint32_t scriptId, int32_t target, int32_t axis,
			     int32_t speed, int32_t withCamera);

void initializeScripts(void);
void initializeLoadedNPCModels(void);
void runMapHeadScript(int32_t section);

extern ScriptCameraMovement MAIN_D_801BE6B4[];
extern uint8_t MAIN_D_80134FE4;
extern int8_t TALKED_TO_ENTITY;
extern uint8_t MAIN_D_80134FE9;
extern int32_t MAIN_D_80134FF0;
extern uint8_t MAPHEAD_DATA[];
extern uint8_t SCRIPT_HEADER[];
extern uint8_t SCRIPT_DATA[];
extern ScriptState SCRIPT_STATE;
extern uint8_t TEXT_BUFFERS[];
extern uint8_t *SCRIPT_HEADER_PTR;
extern uint8_t *SCRIPT_DATA_PTR;
extern char MAIN_D_80130374[];
extern char MAIN_D_80130388[];

static void *script_engine_functions[] = {
	callScriptSection,
	runMapHeadScript,
	initializeLoadedNPCModels,
	initializeScripts,
	tickScriptedMovement,
};

void initializeScripts(void)
{
	MAPHEAD_DATA_PTR = MAPHEAD_DATA;
	SCRIPT_HEADER_PTR = SCRIPT_HEADER;
	SCRIPT_DATA_PTR = SCRIPT_DATA;
	SCRIPT_STATE_PTR = &SCRIPT_STATE;
	TEXT_BUFFERS_PTR = TEXT_BUFFERS;

	readFile(MAIN_D_80130374, MAPHEAD_DATA_PTR);
	readFileSection(MAIN_D_80130388, SCRIPT_HEADER_PTR, 0, 0x2000);
	memset((void *)SCRIPT_STATE_PTR, 0, sizeof(*SCRIPT_STATE_PTR));

	CURRENT_SCRIPT_ID = 0xffff;
	ACTIVE_MAP_SCRIPT = 0xffff;
	MERIT = 0;
	MAIN_D_80134FC6 = 0;
	MAIN_D_80134FC8 = 0;
	MAIN_D_80134FCA = 0;
	MAIN_D_80134FCC = 0;
	TOURNAMENTS_LOST = 0;
	MAIN_D_80134FD0 = 0;
	MAIN_D_80134FD2 = -0x270f;
	MAIN_D_80134FD4 = -0x270f;
	MAIN_D_80134FD6 = -0x270f;

	initialKeyInputs();
	dailyPStatTrigger();
	initializeLoadedNPCModels();
}

void initializeLoadedNPCModels(void)
{
	int32_t i;

	for (i = 0; i < 8; i++) {
		LOADED_DIGIMON_MODELS[i] = -1;
	}
}

void runMapHeadScript(int32_t section)
{
	callScriptSection(0, section, 1);
	tickScript();
}

void callScriptSection(int32_t scriptId, int32_t section, int32_t param)
{
	int32_t i;

	CURRENT_SCRIPT_PTR = (int32_t)getScript(scriptId);
	MAIN_D_80134FDC =
		getScriptSection((uint8_t *)CURRENT_SCRIPT_PTR,
					    section);
	MAIN_D_80134FE0 = param;
	MAIN_D_80134FE4 = section;
	MAIN_D_80134FE5 = 0;
	MAIN_D_80134FE6 = 0xfd;
	MAIN_D_80134FE7 = readPStat(0);
	SOME_SCRIPT_SYNC_BIT = 1;
	ACTIVE_INSTRUCTION = 0;
	MAIN_D_80134FE9 = 0;
	MAIN_D_80134FEC = 0;
	MAIN_D_80134FF0 = 0;
	IS_SCRIPT_PAUSED = 0;
	MAIN_D_80134F9C = TALKED_TO_ENTITY;
	for (i = 0; i < 0x16; i++) {
		MAIN_D_801BE6B4[i].type = 0xff;
	}

	MAIN_func_8010020C();
}

void tickScriptedMovement(int32_t slot)
{
	ScriptCameraMovement *movement = &MAIN_D_801BE6B4[slot];
	int32_t done;

	switch (movement->type) {
	case 0:
		done = tickLookAtEntity(movement->entityId, movement->target);
		break;
	case 1:
		done = tickEntitySetRotation(movement->entityId,
					     movement->posX);
		break;
	case 2:
		done = tickEntityWalkTo(movement->entityId, 0xff,
					movement->posX, movement->posY, 0);
		break;
	case 3:
		done = tickEntityWalkTo(movement->entityId, movement->target,
					0, 0, 0);
		break;
	case 4:
		done = tickEntityWalkTo(movement->entityId, 0xff,
					movement->posX, movement->posY, 1);
		break;
	case 5:
		done = tickEntityWalkTo(movement->entityId, movement->target,
					0, 0, 1);
		break;
	case 6:
		done = tickMoveCameraTo(movement->posX,
					movement->posY,
					movement->speed);
		break;
	case 7:
		done = tickMoveCameraToEntity(movement->entityId,
					      movement->speed);
		break;
	case 8:
		done = tickEntityMoveTo(movement->entityId, 0xff,
					movement->posX, movement->posY,
					movement->speed, 0);
		break;
	case 9:
		done = tickEntityMoveTo(movement->entityId, movement->target,
					0, 0, movement->speed, 0);
		break;
	case 10:
		done = tickEntityMoveTo(movement->entityId, 0xff,
					movement->posX, movement->posY,
					movement->speed, 1);
		break;
	case 0xb:
		done = tickEntityMoveTo(movement->entityId, movement->target,
					0, 0, movement->speed, 1);
		break;
	case 0xc:
		{
			int32_t instanceId;
			int32_t target;

			instanceId = movement->entityId;
			target = movement->target;

			done = tickRotateDoor(instanceId, target);
		}
		break;
	case 0xd:
		done = tickMoveObjectTo(movement->entityId,
					((slot & 0xff) - 0xc) & 0xff,
					(int8_t)movement->target,
					movement->targetX, movement->targetY);
		break;
	case 0xe:
		done = tickEntityMoveToAxis(movement->entityId,
					    movement->posX,
					    movement->target,
					    movement->speed, 0);
		break;
	case 0xf:
		done = tickEntityMoveToAxis(movement->entityId,
					    movement->posX,
					    movement->target,
					    movement->speed, 1);
		break;
	}

	if (done != 0) {
		if (slot < 10 && movement->type < 8) {
			if (slot == 0) {
				startAnimationTamer(0);
			} else if (slot == 1) {
				startPartnerAnimation(0);
			} else if (slot < 10) {
				startNPCAnimation(movement->entityId, 0);
			}
		}

		movement->type = 0xff;
	}
}
