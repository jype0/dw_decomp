#ifndef DW_TOURNAMENT_H
#define DW_TOURNAMENT_H

#include <dw/types.h>

#define TOURNAMENT_ARRAY_SIZE	30

extern uint16_t TOURNAMENT_MEDAL_IDS[];

extern uint8_t TOURNAMENT_DATA[];
extern char *TOURNAMENT_GRADES[];

extern uint8_t *TOURNAMENT_ARRAY;
extern uint8_t TOURNAMENT_SELECTED_COLUMN;
extern uint8_t TOURNAMENT_SELECTED_ROW;

int32_t minutesOfDay(void);
uint8_t *getCupDataJumpTable(uint32_t section, uint32_t id);
uint8_t *getCupDataJumpTableEntry(uint8_t *scriptPtr, int32_t id);
void updateTournamentRegistration(void);
void scriptStartTournament(void);
void scriptCheckTournamentMedal(void);
int32_t checkTournamentMedalConditions(void);

void initTournamentSchedule(void);
void initTournamentInfo(int32_t source);
void fillEnabledTournamentTable(void);
void buildScheduleLabels(void);
void buildScheduleEntries(void);
void tickTournamentSchedule(void);
void renderTournamentSchedule(void);
void renderTournamentTextbox(void);
void tickTournamentInfo(void);
void renderTournamentInfo(void);
int32_t isTournamentEnabled(uint8_t tournament);
int32_t tournamentCheckFair(uint8_t value);
int32_t tournamentCheckEligible(uint8_t tournament);

#endif
