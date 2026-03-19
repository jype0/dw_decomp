#include <dw/types.h>

#include "common.h"

#pragma optimization_level 4

#pragma section sbss begin
extern int16_t FADE_OUT_TARGET;
extern int16_t FADE_IN_TARGET;
extern int16_t FADE_OUT_CURRENT;
extern int16_t FADE_IN_CURRENT;
extern uint8_t FADE_PROGRESS;
extern uint8_t FADE_MODE;
extern uint16_t FADE_PADDING;
extern int32_t FADE_PROTECTION;
#pragma section sbss end

void initializeFadeData(void)
{
	FADE_OUT_TARGET = 0;
	FADE_OUT_CURRENT = 0;
	FADE_IN_TARGET = 0;
	FADE_IN_CURRENT = 0;
	FADE_PROGRESS = 0;
	FADE_MODE = 2;
	FADE_PROTECTION = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/fade", fadeToBlack);

INCLUDE_ASM("asm/main/nonmatchings/fade", renderFadeOut);

INCLUDE_ASM("asm/main/nonmatchings/fade", fadeFromBlack);

INCLUDE_ASM("asm/main/nonmatchings/fade", renderFadeIn);

INCLUDE_ASM("asm/main/nonmatchings/fade", renderFade);

INCLUDE_ASM("asm/main/nonmatchings/fade", fadeToWhite);

INCLUDE_ASM("asm/main/nonmatchings/fade", fadeFromWhite);
