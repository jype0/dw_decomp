#include <libgs.h>
#include <dw/clock.h>
#include <dw/params.h>
#include <dw/pstat.h>
#include <dw/world_object.h>

#include "common.h"


extern GsSPRITE CLOCK_SPRITE;

extern int16_t LAST_HANDLED_FRAME;
extern int8_t GAME_STATE;
extern uint8_t MAP_LAYER_ENABLED;
extern uint8_t CLOCK_TEXCOORD_U[2][4];
extern uint8_t CLOCK_TEXCOORD_V[2][4];
extern GsOT *ACTIVE_ORDERING_TABLE;

extern uint8_t CLOCK_HOUR_X[24];
extern uint8_t CLOCK_HOUR_Y[24];

void renderRectPolyFT4(int32_t posX, int32_t posY,
		       int32_t width, int32_t height,
		       int32_t texX, int32_t texY,
		       int32_t texturePage, int32_t clut,
		       int32_t zIndex, int32_t flag);

void dailyPStatTrigger(void);
void updateTimeOfDay(void);
int32_t isFishing();
int32_t readPStat(int32_t id);
void writePStat(int32_t id, int32_t value);
void addTamerLevel(int32_t chance, int32_t amount);
void updateBGM();
void* clock_functions[] = {
	startGameTime,
	stopGameTime,
	updateMinuteHand,
	advanceToTime,
	updatePlaytime,
	renderGameClock,
	tickGameClock,
	addClock,
	initializeClockData
};

INCLUDE_ASM("asm/main/nonmatchings/clock", initializeClockData);

void addClock(void)
{
	addObject(0xfa2, 0, tickGameClock, renderGameClock);
	addObject(0xfb9, 0, updatePlaytime, 0);
}

void tickGameClock(int32_t instanceId)
{
	uint8_t timeSpeed;

	LAST_HANDLED_FRAME = CURRENT_FRAME;

	if ((GAME_STATE != 0) ||
	    (isFishing() == 1) ||
	    (MAP_LAYER_ENABLED == 0) ||
	    (IS_GAMETIME_RUNNING == 0)) {
		return;
	}

	if ((IS_GAMETIME_RUNNING == 1) &&
	    (readPStat(PSTAT_TIME_SPEED) == 3)) {
		writePStat(PSTAT_TIME_SPEED, 0);
	}

	timeSpeed = readPStat(PSTAT_TIME_SPEED);
	if (timeSpeed == 3) {
		return;
	}

	if (timeSpeed == 0) {
		++CURRENT_FRAME;
	} else {
		if (timeSpeed == 1) {
			uint32_t even;
			uint32_t frame;

			CURRENT_FRAME += 2U;
			frame = CURRENT_FRAME;
			even = frame & 1;
			if (((int32_t)frame < 0)) {
				if (even != 0) {
					even -= 2;
				}
			}
			if (even != 0) {
				--CURRENT_FRAME;
			}
		} else {
			if (timeSpeed == 2) {
				uint32_t even;
				uint32_t subframeCount;
				++SUBFRAME_COUNT;
				subframeCount = SUBFRAME_COUNT;
				even = subframeCount & 1;
				if ((int32_t)subframeCount < 0) {
					if (even != 0) {
						even -= 2;
					}
				}
				if (even == 0) {
					++CURRENT_FRAME;
				}
			}
		}
	}

	if ((timeSpeed == 2) || ((CURRENT_FRAME % 20) != 0)) {
		uint32_t even;
		uint32_t subframeCount;

		if (timeSpeed != 2) {
			goto out;
		}
		subframeCount = SUBFRAME_COUNT;
		even = subframeCount & 1;
		if ((int32_t)subframeCount < 0) {
			if ((even) != 0) {
				even -= 2U;
			}
		}
		if ((even != 0) || ((CURRENT_FRAME % 20) != 0)) {
			goto out;
		}
	}

	++MINUTE;
	if (MINUTE == 60) {
		++HOUR;
		MINUTE = 0;
		++PARTNER_PARA.evoTimer;
		--PARTNER_PARA.remainingLifetime;
		SUBFRAME_COUNT = 0;
		if (HOUR == 24) {
			HOUR = 0;
			++PARTNER_PARA.age;
			CURRENT_FRAME = 0;
			++DAY;
			dailyPStatTrigger();
			if (PARTNER_PARA.remainingLifetime < 0) {
				PARTNER_PARA.remainingLifetime = 0;
			}
			if (DAY > 29) {
				++YEAR;
				DAY = 0;
				if (PARTNER_PARA.happiness == 100) {
					addTamerLevel(5, 1);
				}
				else if (PARTNER_PARA.happiness < 0) {
					addTamerLevel(10, -1);
				}
			}
		}
	}
out:
	if (YEAR > 99) {
		YEAR = 0;
	}

	CLOCK_SPRITE.rotate = MINUTE * 0x6000;

	updateBGM();
}

INCLUDE_ASM("asm/main/nonmatchings/clock", renderGameClock);

void updatePlaytime(int32_t instanceId)
{
	++PLAYTIME_FRAMES;
	if ((PLAYTIME_FRAMES % 1200) == 0) {
		++PLAYTIME_MINUTES;
		if (PLAYTIME_MINUTES > 59) {
			PLAYTIME_MINUTES = 0;
			++PLAYTIME_HOURS;
			PLAYTIME_FRAMES = 0;
			if (PLAYTIME_HOURS > 999) {
				PLAYTIME_HOURS = 999;
			}
		}
	}

	if (PLAYTIME_HOURS == 999) {
		PLAYTIME_MINUTES = 59;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/clock", advanceToTime);

void updateMinuteHand(int32_t hour, int32_t minute)
{
	CLOCK_SPRITE.rotate = minute * 0x6000;
}

void stopGameTime(void)
{
	IS_GAMETIME_RUNNING = 0;
}

void startGameTime(void)
{
	IS_GAMETIME_RUNNING = 1;
}
