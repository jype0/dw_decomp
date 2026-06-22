#include <dw/partner.h>
#include <dw/script.h>

#include "common.h"

void setActiveAnim(int32_t state);

static void *script_anim_functions[] = {
	scriptStartAnimation,
	MAIN_func_80105464,
	MAIN_func_801053EC,
	scriptUpdateEnergyBoundaries,
	MAIN_func_801050C0,
};

INCLUDE_ASM("asm/main/nonmatchings/script_anim", scriptUpdateEnergyBoundaries);

void MAIN_func_801053EC(void)
{
	int32_t state = readPStat(6) & 0xff;

	switch (state) {
	case 0:
		MAIN_D_80134FA0 = (MAIN_D_80134FE0 != 0) ^ 1;
		break;
	case 1:
		MAIN_D_80134FA0 = 1;
		break;
	case 2:
		MAIN_D_80134FA0 = 0;
		break;
	}

	writePStat(6, 0);
}

void MAIN_func_80105464(uint8_t actorId, int32_t animationId)
{
	if (actorId == 0xfd) {
		startAnimationTamer((int16_t)(animationId + 2));
		setTamerState(0xa);
	} else if (actorId == 0xfc) {
		startPartnerAnimation((animationId + 1) << 1);
		setPartnerState(0xc);
	} else {
		startNPCAnimation(actorId, (animationId + 1) << 1);
		setActiveAnim(0xc);
	}
}

void scriptStartAnimation(uint8_t actorId, int32_t animationId)
{
	if (actorId == 0xfd) {
		startAnimationTamer(animationId);
		setTamerState(0xa);
	} else if (actorId == 0xfc) {
		startPartnerAnimation(animationId);
		setPartnerState(0xc);
	} else {
		startNPCAnimation(actorId, animationId);
		setActiveAnim(0xc);
	}
}

void MAIN_func_801050C0(void)
{
	uint8_t condOp;
	uint32_t one;
	uint8_t pstatValue;
	uint8_t comparand;
	uint16_t shortArg;
	int32_t cond;
	int32_t result;

	result = 0;
	for (;;) {
		pollNextScriptUByte(&condOp);
		MAIN_D_80134FDC++;
		if (condOp == 0x19) {
			longjmp(SCRIPT_JMP_BUF, 1);
		}
		switch (condOp & 0x38) {
		case 0:
			pollNextScriptUShort(&shortArg);
			cond = isTriggerSet(shortArg);
			if ((condOp & 7) == 0) {
				cond ^= (one = 1);
				cond = (cond == 0);
				break;
			} else {
				cond = (cond == 0);
				break;
			}
		case (1 << 3):
			pollNextTwoScriptBytes(&pstatValue, &comparand);
			pstatValue = readPStat(pstatValue);
			cond = scriptCompareValues(condOp, pstatValue,
						   comparand);
			break;
		case (2 << 3):
			pollNextScriptUShort(&shortArg);
			if (result != 1) {
				continue;
			}
			MAIN_D_80134FDC =
				(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR +
					    shortArg);
			longjmp(SCRIPT_JMP_BUF, 1);
		case (3 << 3):
			pollNextScriptUShort(&shortArg);
			if (result != 0) {
				continue;
			}

			MAIN_D_80134FDC =
				(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR +
					    shortArg);
			longjmp(SCRIPT_JMP_BUF, 1);
		case (4 << 3):
			switch (condOp & 7) {
			case 0:
				cond = MAIN_func_801022FC();
				break;
			case 1:
				cond = MAIN_func_801024CC();
				break;
			case 2:
				cond = MAIN_func_80102514();
				break;
			case 3:
				cond = MAIN_func_80102564();
				break;
			case 4:
				cond = MAIN_func_801025E8();
				break;
			case 5:
				cond = MAIN_func_80102630();
				break;
			}
			break;
		}

		if (condOp & 0x80) {
			int32_t combined = 0;
			if (result == 1 && cond == 1) combined = 1;
			result = combined;
		} else if (condOp & 0x40) {
			int32_t combined = 1;
			if (result != 1 && cond != 1) combined = 0;
			result = combined;
		} else {
			result = cond;
		}
	}
}
