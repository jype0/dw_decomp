#include <libgs.h>

#include <dw/clock.h>
#include <dw/fade.h>
#include <dw/world_object.h>

#include "common.h"

 int16_t FADE_OUT_TARGET; 
 int16_t FADE_IN_TARGET; 
 int16_t FADE_OUT_CURRENT; 
 int16_t FADE_IN_CURRENT; 
 uint8_t FADE_PROGRESS; 
 uint8_t FADE_MODE; 
 int32_t FADE_PROTECTION; 
 int8_t MAIN_D_80134CBC; 

// Garbage function to force sbss symbol order and ensure
// correct codegen for renderFadeOut()
static void __garbage__()
{
	FADE_OUT_TARGET /= 7;
	FADE_IN_TARGET /= 7;
	FADE_OUT_CURRENT = 0;
	FADE_IN_CURRENT = 0;
	FADE_PROGRESS = 0;
 	FADE_MODE = 0;
 	FADE_PROTECTION = 0;
 	MAIN_D_80134CBC = 0;
	return;
}

void initializeFadeData(void)
{
	FADE_OUT_TARGET = 0;
	FADE_OUT_CURRENT = 0;
	FADE_IN_TARGET = 0;
	FADE_IN_CURRENT = 0;
	FADE_PROGRESS = 0;
	FADE_MODE = 2;
	FADE_PROTECTION = 0;
	return;
}

void fadeToBlack(int32_t frames)
{
	if (FADE_OUT_CURRENT || FADE_IN_CURRENT)
		removeObject(4005, 0);

	FADE_OUT_TARGET = frames + 1;
	FADE_OUT_CURRENT = 1;
	FADE_MODE = 2;
	addObject(4005, 0, 0, (RenderFunction)renderFadeOut);
	stopGameTime();
	FADE_PROTECTION = 1;
	return;
}

void renderFadeOut(void)
{
  uint8_t next;
  next = FADE_PROGRESS += 160 / FADE_OUT_TARGET;
  if (160 < next)
  {
    next = 160;
    FADE_PROGRESS = 160;
  }
  renderFade(next);
  FADE_OUT_CURRENT++;
  return;
}

void fadeFromBlack(int32_t frames)
{
	removeObject(4005, 0);
	FADE_IN_TARGET = frames + 1;
	FADE_IN_CURRENT = 1;
	FADE_OUT_CURRENT = 0;
	FADE_MODE = 2;
	addObject(4005, 0, 0, (RenderFunction)renderFadeIn);
	return;
}

void renderFadeIn(int32_t arg0)
{
    if (FADE_IN_CURRENT < FADE_IN_TARGET) {
        short temp = FADE_IN_CURRENT * (160 / FADE_IN_TARGET);
        
        if (temp < 1)
            temp = FADE_IN_CURRENT;
            
        renderFade(FADE_PROGRESS = 160 - temp);
        
        FADE_IN_CURRENT++;
    } else {
        FADE_IN_CURRENT = 0;
        removeObject(4005, arg0);
        FADE_PROTECTION = 0;
    }
    return;
}

void setPosDataPolyFT4(POLY_FT4 *prim, int16_t posX, int16_t posY, int16_t width, int16_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int16_t xPos, int16_t yPos, int16_t width, int16_t height);
extern GsOT *ACTIVE_ORDERING_TABLE;

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
	return;
}

void fadeToWhite(int32_t frames)
{
	if (FADE_OUT_CURRENT || FADE_IN_CURRENT) {
		removeObject(4005, 0);
	}
	FADE_OUT_TARGET = frames + 1;
	FADE_OUT_CURRENT = 1;
	FADE_MODE = 1;
	addObject(4005, 0, 0, (RenderFunction)renderFadeOut);
	return;
}

void fadeFromWhite(int32_t frames)
{
	removeObject(4005, 0);
	FADE_IN_TARGET = frames + 1;
	FADE_OUT_CURRENT = 0;
	FADE_IN_CURRENT = 1;
	FADE_MODE = 1;
	MAIN_D_80134CBC = 0;
	addObject(4005, 0, 0, (RenderFunction)renderFadeIn);
	return;
}
