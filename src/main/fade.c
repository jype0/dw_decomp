#include <dw/fade.h>

#include "common.h"

 int16_t FADE_OUT_TARGET; 
 int16_t FADE_IN_TARGET; 
 int16_t FADE_OUT_CURRENT; 
 int16_t FADE_IN_CURRENT; 
 uint8_t FADE_PROGRESS; 
 uint8_t FADE_MODE; 
 int32_t FADE_PROTECTION; 
 int8_t MAIN_D_80134CBC; 


void force_fade_sbss_order();
void force_fade_sbss_order(void) {
	FADE_OUT_TARGET = 0; 
	FADE_IN_TARGET = 0;
	FADE_OUT_CURRENT = 0;
	FADE_IN_CURRENT = 0;
	FADE_PROGRESS = 0;
 	FADE_MODE = 0;
 	FADE_PROTECTION = 0; 
 	MAIN_D_80134CBC = 0;
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
}

INCLUDE_ASM("asm/main/nonmatchings/fade", fadeToBlack);

INCLUDE_ASM("asm/main/nonmatchings/fade", renderFadeOut);

INCLUDE_ASM("asm/main/nonmatchings/fade", fadeFromBlack);

INCLUDE_ASM("asm/main/nonmatchings/fade", renderFadeIn);

INCLUDE_ASM("asm/main/nonmatchings/fade", renderFade);

INCLUDE_ASM("asm/main/nonmatchings/fade", fadeToWhite);

INCLUDE_ASM("asm/main/nonmatchings/fade", fadeFromWhite);
