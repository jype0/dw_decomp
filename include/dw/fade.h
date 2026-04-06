#ifndef DW_FADE_H
#define DW_FADE_H

#include <dw/types.h>

#pragma section sbss begin
extern int16_t FADE_OUT_TARGET;
extern int16_t FADE_IN_TARGET;
extern int16_t FADE_OUT_CURRENT;
extern int16_t FADE_IN_CURRENT;
extern uint8_t FADE_PROGRESS;
extern uint8_t FADE_MODE;
extern int32_t FADE_PROTECTION;
#pragma section sbss end

void initializeFadeData(void);

#endif
