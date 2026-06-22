#include <string.h>

#include <libcd.h>

#include <dw/clock.h>
#include <dw/math.h>
#include <dw/script.h>
#include <dw/ui.h>

void setInventorySize(uint8_t v);

static void *script_instr10_functions[] = {
	scriptInstruction10to27,
};

void scriptInstruction10to27(int32_t op)
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
		MAIN_D_80134FDC =
			(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR + shortArg);
		break;
	case 0x14:
		skipOneReadTwoShort(&shortArg, &offset);
		entry.scriptPtr = MAIN_D_80134FDC;
		entry.scriptId = ACTIVE_MAP_SCRIPT;
		entry.smth[0] = 1;
		pushScriptStack(&entry);
		CURRENT_SCRIPT_PTR = (int32_t)getScript(shortArg);
		MAIN_D_80134FDC = getScriptSection(
			(uint8_t *)CURRENT_SCRIPT_PTR, offset);
		break;
	case 0x15:
		MAIN_D_80134FDC++;
		popScriptStack(&entry);
		CURRENT_SCRIPT_PTR = (int32_t)getScript(entry.scriptId);
		MAIN_D_80134FDC = (uint8_t *)entry.scriptPtr;
		break;
	case 0x16:
		skipOneReadOneUShort(&shortArg);
		MAIN_D_80134FDC =
			(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR + shortArg);
		break;
	case 0x17:
		skipOneReadTwoShort(&shortArg, &offset);
		CURRENT_SCRIPT_PTR = (int32_t)getScript(shortArg);
		MAIN_D_80134FDC = getScriptSection(
			(uint8_t *)CURRENT_SCRIPT_PTR, offset);
		break;
	case 0x18:
		pollOneUByteOneUShort(&pstat, &shortArg);
		value = readPStat(pstat);
		if (value >= shortArg) {
			value = (shortArg - 1) & 0xff;
		}
		shortArg = *(uint16_t *)(MAIN_D_80134FDC + (value << 1));
		MAIN_D_80134FDC =
			(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR + shortArg);
		break;
	case 0x19:
		MAIN_D_80134FDC++;
		MAIN_func_801050C0();
		break;
	case 0x1a:
		MAIN_D_80134FDC++;
		if (MAIN_D_80135000 == 2) {
			showTextbox(0, 0xff);
		} else {
			showTextbox(0, MAIN_D_80134FE6);
		}
		longjmp(SCRIPT_JMP_BUF, 2);
	case 0x1b:
		pollNextScriptUByte(&pstat);
		MAIN_D_80135000 = 0;
		if (UI_BOX_DATA[0].state != 1) {
			MAIN_D_80134FE6 = pstat - 1;
		}
		setDialogueOwner(pstat);
		break;
	case 0x1c:
		skipOneReadOneUShort(&shortArg);
		setTrigger(shortArg);
		break;
	case 0x1d:
		skipOneReadOneUShort(&shortArg);
		unsetTrigger(shortArg);
		break;
	case 0x1e:
		skipOnePollTwoScriptBytes(&pstat, &value);
		writePStat(pstat, value);
		break;
	case 0x1f:
		skipOnePollTwoScriptBytes(&pstat, &value);
		newValue = readPStat(pstat) + value;
		if (newValue >= 0x100) newValue = 0xff;
		writePStat(pstat, newValue);
		break;
	case 0x20:
		skipOnePollTwoScriptBytes(&pstat, &value);
		newValue = readPStat(pstat) - value;
		if (newValue < 0) newValue = 0;
		writePStat(pstat, newValue);
		break;
	case 0x21:
		pollNextScriptUByte(&pstat);
		writePStat(pstat, MAIN_D_80134FFE);
		break;
	case 0x22:
		pollNextScriptUByte(&pstat);
		value = PARTNER_ENTITY.digimonEntity.entity.type;
		writePStat(pstat, value);
		break;
	case 0x23:
		pollNextScriptUByte(&pstat);
		setInventorySize(pstat);
		break;
	case 0x24:
		skipOnePollTwoScriptBytes(&pstat, &value);
		writePStat(pstat, random(value + 1));
		break;
	case 0x25:
		pollNextScriptUByte(&pstat);
		writePStat(pstat, YEAR);
		writePStat((pstat + 1) & 0xff, DAY);
		writePStat((pstat + 2) & 0xff, HOUR);
		writePStat((pstat + 3) & 0xff, MINUTE);
		break;
	case 0x26:
		MAIN_func_801062F8(0xff);
		MAIN_D_80135000 = 2;
		scriptSetTextboxSize();
		break;
	case 0x27:
		pollNextScriptUByte(&pstat);
		closeBox(pstat);
		longjmp(SCRIPT_JMP_BUF, 2);
	}

	longjmp(SCRIPT_JMP_BUF, 1);
}
