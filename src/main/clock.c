#include <libgs.h>

#include <dw/clock.h>
#include <dw/params.h>
#include <dw/pstat.h>
#include <dw/script.h>
#include <dw/version.h>
#include <dw/world_object.h>

void renderRectPolyFT4(int32_t posX, int32_t posY,
		       int32_t width, int32_t height,
		       int32_t texX, int32_t texY,
		       int32_t texturePage, int32_t clut,
		       int32_t zIndex, int32_t flag);

void updateTimeOfDay();
int32_t isFishing();
void addTamerLevel(int32_t chance, int32_t amount);
void updateBGM();
void renderStatusBars(int32_t isGameTimeRunning);

extern uint16_t LAST_HANDLED_FRAME;
extern int8_t GAME_STATE;
extern uint8_t MAP_LAYER_ENABLED;
extern GsOT *ACTIVE_ORDERING_TABLE;

uint8_t CLOCK_TEXCOORD_U[2][4] = {
	{ 128, 160, 128, 160 },
	{ 160, 128, 160, 128 }
};

uint8_t CLOCK_TEXCOORD_V[2][4] = {
	{ 192, 192, 208, 192 },
	{ 208, 224, 208, 224 }
};

uint8_t HOUR_POINT_X[24] = {
	20, 23, 26, 31, 34, 36, 36, 36,
	34, 31, 26, 23, 20, 17, 14,  9,
	 6,  4,  4,  4,  6,  9, 14, 17,
};

uint8_t HOUR_POINT_Y[24] = {
	 5,  6 , 6 , 8, 12, 15, 19, 23,
	26, 30, 32, 32, 33, 32, 32, 30,
	26, 23, 19, 15, 12,  8,  6,  6,
};

uint16_t SUBFRAME_COUNT;
int16_t CLOCK_OFFSET_X;
int8_t IS_GAMETIME_RUNNING;

static void *clock_sbss_order[] = {
#if VERSION_IS(EU)
	&CLOCK_OFFSET_X,
	&IS_GAMETIME_RUNNING,
#else
	&IS_GAMETIME_RUNNING,
	&CLOCK_OFFSET_X,
#endif
	&SUBFRAME_COUNT,
};

GsSPRITE CLOCK_SPRITE;

static void *clock_text_order[] = {
	startGameTime,
	stopGameTime,
	updateMinuteHand,
	advanceToTime,
#if VERSION_IS(EU)
	addClock,
	tickGameClock,
	renderGameClock,
	tickPlaytime,
#else
	tickPlaytime,
	renderGameClock,
	tickGameClock,
	addClock,
#endif
	initializeClockData
};

static void *clock_data_order[] = {
	HOUR_POINT_Y,
	HOUR_POINT_X,
};

void addClock(void)
{
	addObject(0xfa2, 0, tickGameClock, renderGameClock);
	addObject(0xfb9, 0, tickPlaytime, 0);
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
		CURRENT_FRAME++;
	} else if (timeSpeed == 1) {
		CURRENT_FRAME += 2;
		if ((CURRENT_FRAME % 2) != 0) {
			CURRENT_FRAME--;
		}
	} else if (timeSpeed == 2) {
		SUBFRAME_COUNT++;
		if ((SUBFRAME_COUNT % 2) == 0) {
			CURRENT_FRAME++;
		}
	}

	if (((timeSpeed != 2) && ((CURRENT_FRAME % 20) == 0)) ||
	    ((timeSpeed == 2) && ((SUBFRAME_COUNT % 2) == 0) &&
	     ((CURRENT_FRAME % 20) == 0))) {
		MINUTE++;
		if (MINUTE == 60) {
			HOUR++;
			MINUTE = 0;
			PARTNER_PARA.evoTimer++;
			PARTNER_PARA.remainingLifetime--;
			SUBFRAME_COUNT = 0;
			if (HOUR == 24) {
				PARTNER_PARA.age++;
				DAY++;
				HOUR = 0;
				CURRENT_FRAME = 0;
				dailyPStatTrigger();
				if (PARTNER_PARA.remainingLifetime < 0) {
					PARTNER_PARA.remainingLifetime = 0;
				}
				if (DAY >= 30) {
					DAY = 0;
					YEAR++;
					if (PARTNER_PARA.happiness == 100) {
						addTamerLevel(5, 1);
					} else if (PARTNER_PARA.happiness < 0) {
						addTamerLevel(10, -1);
					}
				}
			}
		}
	}

	if (YEAR >= 100) {
		YEAR = 0;
	}

	CLOCK_SPRITE.rotate = MINUTE * 0x6000;

	updateBGM();
}

void tickPlaytime(int32_t instanceId)
{
	++PLAYTIME_FRAMES;
	if ((PLAYTIME_FRAMES % 1200) == 0) {
		++PLAYTIME_MINUTES;
		if (PLAYTIME_MINUTES > 59) {
			++PLAYTIME_HOURS;
			PLAYTIME_MINUTES = 0;
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

void advanceToTime(int32_t hour, int16_t minute)
{
	if ((hour < HOUR) || ((HOUR == hour) && (minute < MINUTE))) {
		DAY += 1;
		PARTNER_PARA.age += 1;
		dailyPStatTrigger();
		if (DAY >= 30) {
			DAY = 0;
			YEAR += 1;
		}
	}
	HOUR = hour % 24;
	MINUTE = minute;
	CURRENT_FRAME = HOUR * 1200 + MINUTE * 20;
	CLOCK_SPRITE.rotate = MINUTE * 0x6000;
	updateTimeOfDay();
}

void initializeClockData(void)
{
	YEAR = 0;
	DAY = 0;
	HOUR = 8;
	MINUTE = 0;
	CURRENT_FRAME = 9600;
	PLAYTIME_FRAMES = 0;
	PLAYTIME_HOURS = 0;
	PLAYTIME_MINUTES = 0;
	SUBFRAME_COUNT = 0;
	CLOCK_OFFSET_X = -135;
	CLOCK_SPRITE.attribute = 0;
	CLOCK_SPRITE.x = CLOCK_OFFSET_X + 23;
	CLOCK_SPRITE.y = -66;
	setWH(&CLOCK_SPRITE, 8, 16);
	CLOCK_SPRITE.tpage = GetTPage(0, 0, 896, 448);
	CLOCK_SPRITE.cx = 256;
	CLOCK_SPRITE.cy = 499;
	CLOCK_SPRITE.u = 120;
	CLOCK_SPRITE.v = 192;
	CLOCK_SPRITE.r = CLOCK_SPRITE.g = CLOCK_SPRITE.b = 0x80;
	CLOCK_SPRITE.mx = 3;
	CLOCK_SPRITE.my = 13;
	CLOCK_SPRITE.scalex = CLOCK_SPRITE.scaley = 0x1000;
	CLOCK_SPRITE.rotate = MINUTE * 0x6000;
	IS_GAMETIME_RUNNING = 1;
}

void renderGameClock(int32_t instanceId)
{
	uint8_t isNight;
	uint8_t frame;

	if (HOUR >= 6 && HOUR < 17) {
		isNight = 0;
	} else {
		isNight = 1;
	}

	frame = (CURRENT_FRAME % 16) / 4;

	if (IS_GAMETIME_RUNNING == 0) {
		CLOCK_OFFSET_X -= 50;
	} else {
		CLOCK_OFFSET_X += 50;
	}

	if (CLOCK_OFFSET_X > -135) {
		CLOCK_OFFSET_X = -135;
	}

	if (CLOCK_OFFSET_X < -220) {
		CLOCK_OFFSET_X = -220;
	}

	renderRectPolyFT4(CLOCK_OFFSET_X - 2, -90, 16, 16, 192, 224,
			  GetTPage(0, 0, 896, 448), GetClut(256, 497), 9, 0);

	renderRectPolyFT4(CLOCK_OFFSET_X + 34, -90, 16, 16, 192, 240,
			  GetTPage(0, 0, 896, 448), GetClut(256, 497), 9, 0);

	renderRectPolyFT4(CLOCK_OFFSET_X + 8, -100, 32, 16,
			  CLOCK_TEXCOORD_U[isNight][frame],
			  CLOCK_TEXCOORD_V[isNight][frame],
			  GetTPage(0, 0, 896, 448), GetClut(256, 498), 9, 0);

	renderRectPolyFT4(CLOCK_OFFSET_X, -88, 48, 40, 208, 215,
			  GetTPage(0, 0, 896, 448), GetClut(256, 496), 10, 0);

	CLOCK_SPRITE.x = CLOCK_OFFSET_X + 23;
	GsSortSprite(&CLOCK_SPRITE, ACTIVE_ORDERING_TABLE, 9);

	renderRectPolyFT4(CLOCK_OFFSET_X + HOUR_POINT_X[HOUR],
			  HOUR_POINT_Y[HOUR] - 88,
			  6, 6, 203, 216,
			  GetTPage(0, 0, 896, 448), GetClut(256, 497), 9, 0);

	renderStatusBars(IS_GAMETIME_RUNNING);
}

void updateMinuteHand(hour, minute)
int32_t hour;
int16_t minute;
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
