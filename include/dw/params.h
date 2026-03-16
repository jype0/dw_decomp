#ifndef DW_PARAMS_H
#define DW_PARAMS_H

#include <dw/types.h>

#define NUM_DIGIMON 180

typedef struct {
	char name[20];
	int32_t boneCount;
	int16_t radius;
	int16_t height;
	uint8_t type;
	uint8_t level;
	uint8_t special[3];
	uint8_t dropItem;
	uint8_t dropChance;
	int8_t moves[16];
	uint8_t padding;
} DigimonPara;

typedef struct {
	uint32_t condition;
	int16_t sleepyHour;
	int16_t sleepyMinute;
	int16_t wakeupHour;
	int16_t wakeupMinute;
	int16_t hoursAwakeDefault;
	int16_t hoursAsleepDefault;
	int16_t timeAwakeToday;
	int16_t sicknessCounter;
	int16_t missedSleepHours;
	int16_t tirednessSleepTimer;
	int16_t poopLevel;
	int16_t unused1;
	int16_t unused2;
	int16_t virusBar;
	int16_t poopingTimer;
	int16_t tiredness;
	int16_t subTiredness;
	int16_t tirednessHungerTimer;
	int16_t discipline;
	int16_t happiness;
	int16_t unused3;
	int16_t unused4;
	int16_t timesBeingSick;
	int16_t sicknessTries;
	int16_t areaEffectTimer;
	int16_t sicknessTimer;
	int16_t injuryTimer;
	int16_t nextHungerHour;
	int16_t energyLevel;
	int16_t foodLevel;
	int16_t starvationTimer;
	int16_t weight;
	int16_t refusedFavFood;
	int16_t emptyStomachTimer;
	int16_t remainingLifetime;
	int16_t age;
	uint16_t trainBoostFlag;
	uint16_t trainBoostValue;
	uint16_t trainBoostTimer;
	int16_t careMistakes;
	int16_t battles;
	int16_t evoTimer;
	int16_t unused5;
	int16_t unused6;
	int16_t unused7;
	int16_t fishCaught;
	int16_t unused8;
	int16_t unused9;
	int16_t unused10;
	int16_t upgradeHPcounter;
	int16_t upgradeMPcounter;
	int16_t upgradeOffCounter;
	int16_t buggedUpgradeBrainsCounter;
	int16_t upgradeDefenseCounter;
	int16_t upgradeSpeedCounter;
	int16_t upgradeBrainCounter;
	int16_t unused11;
	int16_t sukaBackupHP;
	int16_t sukaBackupMP;
	int16_t sukaBackupOff;
	int16_t sukaBackupDef;
	int16_t sukaBackupSpeed;
	int16_t sukaBackupBrain;
	int16_t unused12;
} PartnerPara;

extern DigimonPara DIGIMON_DATA[NUM_DIGIMON];
extern PartnerPara PARTNER_PARA;

#endif
