#ifndef DW_CLOCK_H
#define DW_CLOCK_H

#include <dw/types.h>

#pragma section sbss begin
extern int8_t IS_GAMETIME_RUNNING;
extern uint16_t CURRENT_FRAME;
extern int16_t MINUTE;
extern int16_t HOUR;
extern int16_t DAY;
extern uint8_t YEAR;
extern uint16_t PLAYTIME_FRAMES;
extern uint16_t PLAYTIME_MINUTES;
extern uint16_t PLAYTIME_HOURS;
extern uint16_t SUBFRAME_COUNT;
extern int16_t CLOCK_OFFSET_X;
#pragma section sbss end

void initializeClockData(void);
void addClock(void);
void tickGameClock(int32_t instanceId);
void renderGameClock(int32_t instanceId);
void updatePlaytime(int32_t instanceId);
void advanceToTime(int32_t hour, int32_t minute);
void updateMinuteHand(int32_t hour, int32_t minute);
void stopGameTime(void);
void startGameTime(void);

#endif
