#include <libgs.h>

#include <dw/clock.h>
#include <dw/fade.h>

#include "common.h"

int16_t FADE_DATA;
int16_t FADE_IN_TARGET;
int16_t FADE_OUT_CURRENT;
int16_t FADE_IN_CURRENT;
uint8_t FADE_PROGRESS;
uint8_t FADE_MODE;
int32_t FADE_PROTECTION;
uint8_t FADE_OUT_IN_PROGRESS;

int32_t addObject(int16_t objectId, int16_t instanceId, void *tick, void *render);
int32_t removeObject(int16_t objectId, int16_t instanceId);
void setPosDataPolyFT4(POLY_FT4 *prim, int16_t posX, int16_t posY, int16_t width, int16_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int16_t xPos, int16_t yPos, int16_t width, int16_t height);
extern GsOT *ACTIVE_ORDERING_TABLE;

// Garbage function to force sbss symbol order and ensure
// correct codegen for renderFadeOut()
static void __garbage__()
{
	FADE_DATA /= 7;
	FADE_IN_TARGET /= 7;
	FADE_OUT_CURRENT = 0;
	FADE_IN_CURRENT = 0;
	FADE_PROGRESS = 0;
	FADE_MODE = 0;
	FADE_PROTECTION = 0;
	FADE_OUT_IN_PROGRESS = 0;
}

void initializeFadeData(void)
{
	FADE_DATA = 0;
	FADE_OUT_CURRENT = 0;
	FADE_IN_TARGET = 0;
	FADE_IN_CURRENT = 0;
	FADE_PROGRESS = 0;
	FADE_MODE = 2;
	FADE_PROTECTION = 0;
}

void fadeToBlack(int16_t frames)
{
	if (FADE_OUT_CURRENT || FADE_IN_CURRENT) {
		removeObject(4005, 0);
	}

	FADE_DATA = frames + 1;
	FADE_OUT_CURRENT = 1;
	FADE_MODE = 2;
	addObject(4005, 0, 0, renderFadeOut);
	stopGameTime();
	FADE_PROTECTION = 1;
}

void fadeFromBlack(int16_t frames)
{
	removeObject(4005, 0);
	FADE_OUT_CURRENT = 0;
#if defined(VERSION_JP) && !defined(VERSION_JP_REV1)
	removeObject(4005, 0);
#endif

	FADE_IN_TARGET = frames + 1;
	FADE_IN_CURRENT = 1;
	FADE_MODE = 2;
	addObject(4005, 0, 0, renderFadeIn);
}

void renderFadeIn(int16_t instanceId)
{
	int16_t temp;
	uint8_t progress;

	if (FADE_IN_CURRENT < FADE_IN_TARGET) {
		temp = FADE_IN_CURRENT * (160 / FADE_IN_TARGET);

		if (temp < 1) {
			temp = FADE_IN_CURRENT;
		}

		FADE_PROGRESS = progress = 160 - temp;

		if (progress < 0) {
			FADE_PROGRESS = progress = 0;
		}

		renderFade(progress);

		FADE_IN_CURRENT++;
	} else {
		FADE_IN_CURRENT = 0;
		removeObject(4005, instanceId);
		FADE_PROTECTION = 0;
	}
}

void renderFadeOut(void)
{
	uint8_t next;

	next = FADE_PROGRESS += 160 / FADE_DATA;

	if (160 < next) {
		FADE_PROGRESS = next = 160;
	}

	renderFade(next);

	FADE_OUT_CURRENT++;
}

void renderFade(uint8_t progress)
{
	POLY_FT4 *prim;

	prim = (POLY_FT4 *)GsGetWorkBase();

	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->tpage = GetTPage(0, FADE_MODE, 896, 448);
	prim->clut = GetClut(256, 502);
	setPosDataPolyFT4(prim, -160, -120, 320, 240);
	setUVDataPolyFT4(prim, 250, 509, 2, 2);
	
	setRGB0(prim, progress, progress, progress);

	if (FADE_MODE == 2) {
		AddPrim(&ACTIVE_ORDERING_TABLE->org[1], prim);
	} else {
		AddPrim(&ACTIVE_ORDERING_TABLE->org[10], prim);
	}

	prim++;

	GsSetWorkBase((PACKET*)(prim));
}

void fadeToWhite(int16_t frames)
{
#if !defined(VERSION_JP) || defined(VERSION_JP_REV1)
	if (FADE_OUT_CURRENT || FADE_IN_CURRENT) {
		removeObject(4005, 0);
	}
#endif

	FADE_DATA = frames + 1;
	FADE_OUT_CURRENT = 1;
	FADE_MODE = 1;

#if defined(VERSION_JP) && !defined(VERSION_JP_REV1)
	if (!FADE_OUT_IN_PROGRESS) {
		addObject(4005, 0, 0, renderFadeOut);
	}

	FADE_OUT_IN_PROGRESS = 1;
#else
	addObject(4005, 0, 0, renderFadeOut);
#endif
}

void fadeFromWhite(int16_t frames)
{
	removeObject(4005, 0);
	FADE_OUT_CURRENT = 0;
	FADE_IN_TARGET = frames + 1;
	FADE_IN_CURRENT = 1;
	FADE_MODE = 1;
	FADE_OUT_IN_PROGRESS = 0;
	addObject(4005, 0, 0, renderFadeIn);
}
