#include <dw/fade.h>

#include "common.h"

#pragma optimization_level 4

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
