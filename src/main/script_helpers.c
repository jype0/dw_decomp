#include <dw/clock.h>
#include <dw/doo2.h>
#include <dw/font.h>
#include <dw/map_object.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/tournament.h>
#include <dw/trigger.h>


void unsetCameraFollowPlayer(void);
extern int32_t MAIN_func_800D8E64(int32_t param_1, int32_t param_2,
				  int32_t param_3);
extern int32_t tickRemoveMist(void);
extern int32_t MAIN_func_801138B0(void);
extern int32_t MAIN_func_80113A20(void);
extern int32_t isTrainingComplete(void);
extern void setCameraFollowPlayer(void);

extern uint8_t MAIN_D_80134FE4;
extern uint8_t MAIN_D_80134FE9;
extern int32_t MAIN_D_80134FF0;
extern uint8_t MAIN_D_801BE6B5[];
extern uint8_t MAIN_D_801BE6B6[];
extern uint8_t *SCRIPT_HEADER_PTR;
extern uint8_t *SCRIPT_DATA_PTR;
extern char MAIN_D_80130388[];
extern int8_t MAIN_STATE;
extern int16_t SCRIPT_MAP_CHANGE_STATE;

extern uint8_t MAIN_D_801BE6B4[];

static void *script_helpers_functions[] = {
	writePStat,
	isTriggerSet,
	MAIN_func_801062F8,
	readPStat,
};

uint8_t readPStat(int32_t index)
{
	return SCRIPT_STATE_PTR->pstats[index];
}

void MAIN_func_801062F8(int32_t owner)
{
	int32_t entityId;
	uint8_t speaker;
	int32_t idx;

	if (MAIN_D_80134FF0 == 0) {
		MAIN_D_80134FF0 = 1;
		setMovementEnabled(0, 1);
		setMovementEnabled(1, 1);
		unsetCameraFollowPlayer();
		writePStat(0, 3);
		clearTextArea();
		stopGameTime();

		if (MAIN_D_80134FE0 != 0 && isTriggerSet(TRIGGER_44) == 0) {
			entityId = scriptIdToEntityId(MAIN_D_80134FE4) & 0xff;
			if (entityId != 0xff) {
				MAIN_D_801BE6B4[entityId * 0xc] = 0;
				idx = entityId * 0xc;
				speaker = MAIN_D_80134FE4;
				MAIN_D_801BE6B5[idx] = speaker;
				MAIN_D_801BE6B6[idx] = 0xfd;
				MAIN_D_801BE6B4[0] = 0;
				MAIN_D_801BE6B5[0] = 0xfd;
				MAIN_D_801BE6B6[0] = speaker;
			}
		}
	}

	unsetTrigger(TRIGGER_44);

	if ((uint32_t)owner < 0xc8) {
		owner = scriptIdToEntityId(owner) & 0xff;
		if (owner != 0xff) {
			setMovementEnabled(owner, 1);
		}
	}
}

int32_t isTriggerSet(uint16_t trigger)
{
	uint8_t *ptr;
	uint8_t mask;

	getTriggerOffset(trigger, &ptr, &mask);

	return (*ptr & mask) != 0;
}

void writePStat(int32_t index, uint8_t value)
{
	uint8_t *ptr;

	ptr = &SCRIPT_STATE_PTR->pstats[index];
	*ptr = value;
}
