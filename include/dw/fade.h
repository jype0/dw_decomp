#ifndef DW_FADE_H
#define DW_FADE_H

#include <dw/types.h>

extern int16_t FADE_DATA;
extern int16_t FADE_IN_TARGET;
extern int16_t FADE_OUT_CURRENT;
extern int16_t FADE_IN_CURRENT;
extern uint8_t FADE_PROGRESS;
extern uint8_t FADE_MODE;
extern int32_t FADE_PROTECTION;

void initializeFadeData(void);
void fadeToBlack(int16_t frames);
void renderFadeOut(void);
void fadeFromBlack(int16_t frames);
void renderFadeIn(int16_t instanceId);
void renderFade(uint8_t progress);
void fadeToWhite(int16_t frames);
void fadeFromWhite(int16_t frames);

#endif
