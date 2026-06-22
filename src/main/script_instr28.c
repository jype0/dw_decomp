#include <dw/clock.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sound.h>

void skipHours(int32_t hours);
void scriptSetDigimon(int32_t a0, int32_t a1, int32_t a2);
void scriptUnloadEntity(int32_t a0);
void resetEntityOrigin(int32_t a0);
void setMapObjectsFlag(int32_t a0, int32_t a1, int32_t a2);

extern uint8_t MAIN_D_801BE6B4[];

static void *script_instr28_functions[] = {
	scriptInstruction5Ato5F,
	scriptInstruction46to58,
	scriptInstruction28to3F,
};

static void scriptInstruction28to3F__garbage__(int32_t op)
{
	StackEntry entry;
	uint16_t shortArg;
	uint16_t offset;
	uint8_t pstat;
	uint8_t value;
	uint8_t unusedByte;
	int32_t newValue;

	switch (op) {
	case 0x10:
		MAIN_D_80135000 = 1;
		scriptShowSelection();
		longjmp(SCRIPT_JMP_BUF, 2);
		break;
	case 0x13:
		skipOneReadOneUShort(&shortArg);
		entry.scriptPtr = MAIN_D_80134FDC;
		entry.scriptId = ACTIVE_MAP_SCRIPT;
		entry.smth[0] = 1;
		pushScriptStack(&entry);
		MAIN_D_80134FDC = (uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR + shortArg);
		break;
	}
}

void scriptInstruction28to3F(int32_t op)
{
	int16_t *statPtr;
	uint32_t value32;
	int32_t intArg;
	uint16_t value;
	uint8_t byteArg1;
	uint8_t byteArg2;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint32_t sec;

	switch (op) {
	case 0x28:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		if (giveItem(byteArg1, byteArg2) != 0) {
			unsetTrigger(0);
		} else {
			setTrigger(0);
		}
		break;
	case 0x29:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		removeItem(byteArg1, byteArg2);
		break;
	case 0x2a:
		skipOneReadInteger(&intArg);
		MONEY += intArg;
		if (MONEY >= 0xf4240) {
			MONEY = 0xf423f;
		}
		break;
	case 0x2b:
		skipOneReadInteger(&intArg);
		MONEY -= intArg;
		if (MONEY < 0) {
			MONEY = 0;
		}
		break;
	case 0x2c:
		scriptCompareDate();
		break;
	case 0x2d:
		pollNextScriptUByte(&byteArg1);
		scriptLearnMove(byteArg1);
		break;
	case 0x2e:
		pollNextScriptUByte(&byteArg1);
		break;
	case 0x2f:
		pollNextScriptUByte(&byteArg1);
		byteArg2 = getCardAmount(byteArg1);
		if (byteArg2 < 9) {
			byteArg2 = byteArg2 + 1u;
			setCardAmount(byteArg1, byteArg2);
		}
		break;
	case 0x30:
		pollNextScriptUByte(&byteArg1);
		byteArg2 = getCardAmount(byteArg1);
		if (byteArg2 != 0) {
			byteArg2 = byteArg2 - 1u;
			setCardAmount(byteArg1, byteArg2);
		}
		break;
	case 0x31:
		skipOneReadOneUShort(&value);
		MERIT = value;
		if (MERIT >= 0x2710) {
			MERIT = 0x270f;
		}
		break;
	case 0x32:
		skipOneReadOneUShort(&value);
		MERIT += value;
		if (MERIT >= 0x2710) {
			MERIT = 0x270f;
		}
		break;
	case 0x33:
		skipOneReadOneUShort(&value);
		MERIT = -value;
		if (MERIT < 0) {
			MERIT = 0;
		}
		break;
	case 0x34:
		pollOneUByteOneUShort(&byteArg1, &value);
		statPtr = getStatsPointer(byteArg1);
		value32 = value;
		*statPtr = enforceStatsLimits(byteArg1, (int16_t)value32);
		scriptUpdateEnergyBoundaries(byteArg1, *statPtr);
		if (byteArg1 == 0x15) {
			TAMER_ENTITY.tamerLevel = MAIN_D_80135002;
		}
		if (byteArg1 == 0x16) {
			PARTNER_ENTITY.lives = MAIN_D_80135004;
		}
		break;
	case 0x35:
		pollOneUByteOneUShort(&byteArg1, &value);
		statPtr = getStatsPointer(byteArg1);
		value32 = value;
		*statPtr += value32;
		*statPtr = enforceStatsLimits(byteArg1, *statPtr);
		scriptUpdateEnergyBoundaries(byteArg1, *statPtr);
		if (byteArg1 == 0x15) {
			TAMER_ENTITY.tamerLevel = MAIN_D_80135002;
		}
		if (byteArg1 == 0x16) {
			PARTNER_ENTITY.lives = MAIN_D_80135004;
		}
		break;
	case 0x36:
		pollOneUByteOneUShort(&byteArg1, &value);
		statPtr = getStatsPointer(byteArg1);
		intArg = *statPtr - (int16_t)value;
		if (byteArg1 != 9) {
			if (intArg < 0) intArg = 0;
		} else {
			if (intArg < -0x64) intArg = -0x64;
		}
		*statPtr = intArg;
		scriptUpdateEnergyBoundaries(byteArg1, *statPtr);
		if (byteArg1 == 0x15) {
			TAMER_ENTITY.tamerLevel = MAIN_D_80135002;
		}
		if (byteArg1 == 0x16) {
			PARTNER_ENTITY.lives = MAIN_D_80135004;
		}
		break;
	case 0x37:
		pollNextScriptUByte(&byteArg1);
		byteArg2 = readPStat(byteArg1);
		day = readPStat((byteArg1 + 1) & 0xff);
		hour = readPStat((byteArg1 + 2) & 0xff);
		minute = readPStat((byteArg1 + 3) & 0xff);
		sec = dateToSeconds(byteArg2, day, hour, minute);
		{
			uint32_t cur_sec = dateToSeconds(YEAR, DAY & 0xff,
							 HOUR & 0xff, MINUTE &
							 0xff);
			if (cur_sec < sec) {
				skipHours((sec - cur_sec) / 60);
			}
		}
		YEAR = byteArg2;
		{
			int16_t prev_day = DAY;
			DAY = day;
			HOUR = hour;
			MINUTE = minute;
			CURRENT_FRAME = HOUR * 1200 + MINUTE * 20;
			if (DAY != prev_day) {
				dailyPStatTrigger();
			}
		}
		break;
	case 0x38:
	case 0x39:
		pollNextScriptUByte(&byteArg1);
		pollNextInt(&intArg);
		byteArg2 = readPStat(byteArg1);
		day = readPStat((byteArg1 + 1) & 0xff);
		hour = readPStat((byteArg1 + 2) & 0xff);
		minute = readPStat((byteArg1 + 3) & 0xff);
		sec = dateToSeconds(byteArg2, day, hour, minute);
		if (op == 0x38) {
			intArg = sec + intArg;
		} else {
			intArg = sec - intArg;
			if (intArg < 0) intArg = 0;
		}
		MAIN_func_8010692C(intArg, &byteArg2, &day, &hour, &minute);
		writePStat(byteArg1, byteArg2);
		writePStat((byteArg1 + 1) & 0xff, day);
		writePStat((byteArg1 + 2) & 0xff, hour);
		writePStat((byteArg1 + 3) & 0xff, minute);
		break;
	case 0x3a:
	case 0x3b:
	case 0x3c:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		break;
	case 0x3d:
	case 0x3e:
		pollNextScriptUByte(&byteArg1);
		break;
	case 0x3f:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		byteArg1 = readPStat(byteArg1);
		byteArg1 = DIGIMON_DATA[byteArg1].type;
		writePStat(byteArg2, byteArg1);
		break;
	}

	longjmp(SCRIPT_JMP_BUF, 1);
}

void scriptInstruction46to58(int32_t op)
{
	StackEntry entry;
	int16_t posX;
	int16_t posY;
	uint8_t byteArg1;
	uint8_t byteArg2;
	uint8_t byteArg3;
	uint8_t entityId;
	uint8_t *b;

	switch (op) {
	case 0x46:
		pollNextScriptUByte(&byteArg1);
		scriptLoadModel(byteArg1);
		break;
	case 0x47:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		scriptSetDigimon(byteArg1, byteArg2, byteArg3);
		break;
	case 0x48:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		scriptUnloadEntity(byteArg1);
		break;
	case 0x49:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		callDigimonRoutine(byteArg1);
		break;
	case 0x4a:
		pollNextScriptUByte(&byteArg1);
		ACTIVE_INSTRUCTION = 0x4a;
		if (byteArg1 == 0xff) {
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xc8) {
			byteArg1 = 0xa;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xca) {
			byteArg1 = 0xb;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xcb) {
			byteArg1 = 0xc;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xcc) {
			byteArg1 = 0xd;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xcd) {
			byteArg1 = 0xe;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xce) {
			byteArg1 = 0xf;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xcf) {
			byteArg1 = 0x10;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xd0) {
			byteArg1 = 0x11;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xd1) {
			byteArg1 = 0x12;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xd2) {
			byteArg1 = 0x13;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xd3) {
			byteArg1 = 0x14;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xd4) {
			byteArg1 = 0x15;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xc9) {
			byteArg1 = 0x19;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xd5) {
			byteArg1 = 0x19;
			SOME_SCRIPT_SYNC_BIT = 0;
			goto wait_for_entity_end;
		}
		if (byteArg1 == 0xd6) {
			byteArg1 = 0x1a;
			goto wait_for_entity_end;
		}
		byteArg1 = scriptIdToEntityId(byteArg1);
	wait_for_entity_end:
		MAIN_D_80134FA4 = byteArg1;
		longjmp(SCRIPT_JMP_BUF, 2);
	case 0x4b:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		MAIN_D_80134FF8 = byteArg1;
		SELECTION_MENU_STATE = byteArg2;
		MAIN_func_801053EC();
		entry.smth[0] = 4;
		entry.smth[1] = byteArg3;
		pushScriptStack(&entry);
		longjmp(SCRIPT_JMP_BUF, 3);
	case 0x4c:
		MAIN_func_801062F8(0xff);
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) break;
		b = MAIN_D_801BE6B4 + entityId * 0xc;
		b[0] = 0;
		b[1] = byteArg1;
		b[2] = byteArg2;
		break;
	case 0x4d:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextScriptShort(&posX);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) break;
		b = MAIN_D_801BE6B4 + entityId * 0xc;
		b[0] = 1;
		b[1] = byteArg1;
		*(int16_t *)(b + 4) = posX;
		break;
	case 0x4e:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		MAIN_func_80105464(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) break;
		b = MAIN_D_801BE6B4 + entityId * 0xc;
		b[0] = 2;
		b[1] = byteArg1;
		*(int16_t *)(b + 4) = posX;
		*(int16_t *)(b + 6) = posY;
		break;
	case 0x4f:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		b = (uint8_t *)&MAIN_D_801BE72C;
		b[0] = 6;
		*(int16_t *)(b + 4) = posX;
		*(int16_t *)(b + 6) = posY;
		b[3] = byteArg1;
		break;
	case 0x50:
		MAIN_func_801062F8(0xff);
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		b = (uint8_t *)&MAIN_D_801BE72C;
		b[0] = 7;
		b[1] = byteArg1;
		b[3] = byteArg2;
		break;
	case 0x51:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		MAIN_func_80105464(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) break;
		b = MAIN_D_801BE6B4 + entityId * 0xc;
		b[0] = 3;
		b[1] = byteArg1;
		b[2] = byteArg3;
		break;
	case 0x52:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		MAIN_func_80105464(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) break;
		b = MAIN_D_801BE6B4 + entityId * 0xc;
		b[0] = 4;
		b[1] = byteArg1;
		*(int16_t *)(b + 4) = posX;
		*(int16_t *)(b + 6) = posY;
		break;
	case 0x53:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		MAIN_func_80105464(byteArg1, byteArg2);
		entityId = scriptIdToEntityId(byteArg1);
		if (entityId == 0xff) break;
		b = MAIN_D_801BE6B4 + entityId * 0xc;
		b[0] = 5;
		b[1] = byteArg1;
		b[2] = byteArg3;
		break;
	case 0x54:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		resetEntityOrigin(byteArg1);
		break;
	case 0x55:
		MAIN_D_80134FDC++;
		pollNextTwoScriptShorts(&MAIN_D_80134FD2, &MAIN_D_80134FD4);
		pollNextScriptShort(&MAIN_D_80134FD6);
		break;
	case 0x56:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		scriptStartAnimation(byteArg1, byteArg2);
		break;
	case 0x57:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		setMapObjectsFlag(byteArg1, 1, byteArg2);
		break;
	case 0x58:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		MAIN_D_80134FF8 = readPStat(byteArg1);
		SELECTION_MENU_STATE = readPStat((byteArg1 + 1) & 0xff);
		MAIN_func_801053EC();
		entry.smth[0] = 4;
		entry.smth[1] = 0xff;
		pushScriptStack(&entry);
		longjmp(SCRIPT_JMP_BUF, 3);
	}

	longjmp(SCRIPT_JMP_BUF, 1);
}

void scriptInstruction5Ato5F(int32_t op)
{
	uint8_t byteArg1;
	uint8_t byteArg2;

	switch (op) {
	case 0x5a:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		playSound(byteArg1, byteArg2);
		break;
	case 0x5b:
		pollNextScriptUByte(&byteArg1);
		break;
	case 0x5c:
		pollNextScriptUByte(&byteArg1);
		break;
	case 0x5d:
		pollNextScriptUByte(&byteArg1);
		playBGM(byteArg1);
		break;
	case 0x5e:
		pollNextScriptUByte(&byteArg1);
		resetBGM();
		break;
	case 0x5f:
		pollNextScriptUByte(&byteArg1);
		break;
	}

	longjmp(SCRIPT_JMP_BUF, 1);
}
