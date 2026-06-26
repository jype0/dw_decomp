#ifndef DW_COMBAT_H
#define DW_COMBAT_H

#include <libgte.h>

#include <dw/types.h>

typedef struct {
	int32_t statusFxId;
	int32_t effectSlot[4];
	int32_t unk11;
	int16_t finisherGoal;
	int16_t finisherProgress;
	int16_t poisonTimer;
	int16_t confusionTimer;
	int16_t stunTimer;
	int16_t flatTimer;
	int16_t flatAttackTimer;
	int16_t invulnerableTimer;
	int16_t cooldown;
	int16_t senileTimer;
	int16_t unk15;
	int16_t hpDamageBuffer;
	int16_t mpDamageBuffer;
	int16_t speedBuffer;
	int16_t flags;
	uint8_t moveRange;
	uint8_t targetId;
	uint8_t queuedAnim;
	uint8_t buffsRemaining;
	uint8_t buffPrioTimer;
	uint8_t unk16;
	uint8_t table1[150];
	uint8_t table2[150];
} FighterData;

typedef struct {
	int32_t unk1;
	DVECTOR pos;
	int16_t unk2;
	int16_t unk3;
	int16_t unk4;
	uint8_t unk5;
	uint8_t unk6;
	int16_t unk7;
	int16_t unk8;
	uint8_t unk9;
	uint8_t unk10;
	uint8_t unk11;
	uint8_t unk12;
	uint8_t unk13;
	uint8_t unk14;
	uint8_t unk15;
	uint8_t unk16;
	uint8_t unk17;
	uint8_t unk18;
	uint8_t unk19;
	uint8_t unk20;
	uint8_t unk21;
	uint8_t unk22;
	uint8_t unk23;
	uint8_t unk24;
	uint8_t unk25;
	uint8_t unk26;
	uint8_t unk27;
	uint8_t unk28;
} PlayerDataSub;

typedef struct {
	PlayerDataSub unk1[4];
	int16_t hitCount;
	int16_t blockedCount;
	int16_t statusedCount;
	int16_t unk2;
	int16_t startingHP;
	int16_t commandDelay[2];
	uint8_t currentCommand[2];
	uint8_t buffereCommand[2];
	uint8_t hoveredCommand[2];
	uint8_t availableCommands[2][9];
	uint8_t numCommands[2];
	uint8_t finisherChargeup[2];
	uint8_t remainingChargeupTime[2];
	uint8_t entityIds[4];
	uint8_t unk4;
	uint8_t unk5;
	uint8_t unk6;
	uint8_t unk7;
	uint8_t changeTarget;
	uint8_t usedMoves[12];
	uint8_t unk8;
	uint8_t unk9;
	uint8_t unk10;
} PlayerData;

typedef struct {
	FighterData fighter[4];
	PlayerData player;
} CombatData;

extern CombatData COMBAT_DATA;
extern CombatData *COMBAT_DATA_PTR;

#endif
