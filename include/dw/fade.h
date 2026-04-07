#ifndef DW_FADE_H
#define DW_FADE_H

#include <dw/types.h>

extern int16_t FADE_OUT_TARGET;
extern int16_t FADE_IN_TARGET;
extern int16_t FADE_OUT_CURRENT;
extern int16_t FADE_IN_CURRENT;
extern uint8_t FADE_PROGRESS;
extern uint8_t FADE_MODE;
extern int32_t FADE_PROTECTION;

void initializeFadeData(void);
void fadeToBlack(int32_t frames);
void renderFadeOut(void);
void fadeFromBlack(int32_t frames);
void renderFadeIn(int32_t arg0);
void renderFade(uint8_t progress);
void fadeToWhite(int32_t frames);
void fadeFromWhite(int32_t frames);
#endif
