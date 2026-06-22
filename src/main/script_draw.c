#include <string.h>

#include <dw/params.h>
#include <dw/pstat.h>
#include <dw/script.h>
#include <dw/trigger.h>
#include <dw/ui.h>

uint16_t convertAsciiToJis(int32_t input);
int32_t swapShortBytes(int32_t input);
int32_t isAsciiEncoded(const char *value);

void initializeFontCLUT(void);
void clearTextArea(void);
int32_t drawGlyph(uint16_t glyph, int32_t x, int32_t y);
void drawString(char *str, uint16_t x, int32_t y);

extern uint8_t COLORCODE_LOWBITS;
extern uint8_t COLORCODE_HIGHBITS;
extern u_long MAIN_D_80130FE8[];
extern int32_t MAIN_D_8013502C;
extern uint8_t MAIN_D_80134F90;
extern char MAIN_D_80134607[1];
extern uint16_t CHAR_TO_GLYPH_TABLE[];
extern uint16_t GLYPH_DATA[];
extern uint16_t MAIN_D_8013087C[];
extern uint8_t MAIN_D_801BE958[];

static void *script_draw_functions[] = {
	drawString,
	drawGlyph,
	setTextColor,
	clearTextSubArea,
	clearTextArea,
	initializeFontCLUT,
	newGameStateMachine,
	initializeNamingBuffer,
	MAIN_func_8010C4B0,
	MAIN_func_8010C28C,
	openJukebox,
	MAIN_func_8010BF68,
	MAIN_func_8010BC10,
	MAIN_func_8010BB0C,
	MAIN_func_8010B9D8,
	rollCardPack,
	MAIN_func_8010B648,
};

void MAIN_func_8010C28C(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 0xc, 6, 0xd2, 0x18, 6,
				      0x5a);
		MAIN_func_80107AB8();
		showMapHeadTextbox(4, owner, 0, 0x4d6);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&MAIN_D_80134F68);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		MAIN_func_800FC968(1);
		MAIN_func_80107B98();
		setInputRepeatMask(0x5000);
		SELECTION_MENU_STATE = 1;
		break;
	case 4:
		setInputRepeatMask(0);
		showMapheadSelection(7, 0xfd, 2, &selection, 0x4d6);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 5;
		SCRIPT_STATE_3 = 2;
		break;
	case 5:
		{
			uint8_t idx;

			triggerBoxCloseFlag(2);
			triggerBoxCloseFlag(1);

			idx = MAIN_D_80134F68->buf[(MAIN_D_80134F68->topRow + MAIN_D_80134F68->cursor) * 2] & 0x7f;
			writePStat(PSTAT_247, MAIN_D_8013024C[idx].mapId);
			writePStat(PSTAT_248, MAIN_D_8013024C[idx].unk_0x1);
			CURRENT_SCRIPT_PTR = (int32_t)getScript(0);
			MAIN_D_80134FDC = getScriptSection((uint8_t *)CURRENT_SCRIPT_PTR, 0x4e3);
			MONEY -= MAIN_D_8013500C;
			SELECTION_MENU_STATE = 2;
			break;
		}
	case 6:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(2);
		triggerBoxCloseFlag(1);
		showMapHeadTextbox(5, owner, 0, 0x4d6);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	}
}

void MAIN_func_8010B648(void)
{
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0:
		{
			int32_t hasItems;
			initializeItemMenuBox(&MAIN_D_80134F68, 0x9c, 6, 0xd2,
					      0x18, 6, 0x5a);
			hasItems = MAIN_func_80106D28();
			MAIN_D_80134F70 = 0;
			MAIN_D_80134F74 = 0;

			if (hasItems != 0) {
				showShopkeeperTextbox(0, owner, 0);
				SELECTION_MENU_STATE = 1;
				SCRIPT_STATE_4 = 3;
				SCRIPT_STATE_3 = 1;
			} else {
				showShopkeeperTextbox(1, owner, 0);
				SELECTION_MENU_STATE = 1;
				SCRIPT_STATE_4 = 2;
				SCRIPT_STATE_3 = 1;
			}
			break;
		}
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&MAIN_D_80134F68);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		MAIN_func_800FC968(1);
		MAIN_func_800FCA14(3, 0xfd, 2, &MAIN_D_80134F70);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 4;
		SCRIPT_STATE_3 = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000);
		MAIN_D_80135011 = 2;
		MAIN_func_800FCA3C();
		showShopkeeperTextbox(8, owner, 0);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 5:
	case 6:
		triggerBoxCloseFlag(2);
		if (MAIN_D_80134F74 != 0) {
			showShopkeeperTextbox(4, owner, 0);
		} else {
			showShopkeeperTextbox(5, owner, 0);
		}
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	case 7:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		if (isPartnerBaby() != 0) {
			showShopkeeperTextbox(0xd, owner, 0);
		} else {
			showShopkeeperTextbox(10, owner, 0);
		}
		MAIN_D_80134F74 = 1;
		if (MAIN_func_80106D28() != 0) {
			SCRIPT_STATE_4 = 3;
		} else {
			SCRIPT_STATE_4 = 9;
		}
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 1;
		break;
	case 8:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		showShopkeeperTextbox(0xb, owner, 0);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	case 9:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		showShopkeeperTextbox(0xc, owner, 0);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 6;
		SCRIPT_STATE_3 = 1;
		break;
	}
}

void rollCardPack(void)
{
	switch (SELECTION_MENU_STATE) {
	case 0:
		writePStat(PSTAT_249, rollCard());
		showCardTextbox();
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	case 1:
		break;
	case 2:
		writePStat(PSTAT_249, rollCard());
		showCardTextbox();
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	case 3:
		writePStat(PSTAT_249, rollCard());
		showCardTextbox();
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 4;
		SCRIPT_STATE_3 = 1;
		break;
	case 4:
		ACTIVE_INSTRUCTION = 0;
		break;
	}
}

void MAIN_func_8010B9D8(void)
{
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 0xc, 6, 0xb2, 0x18, 6,
				      0x5a);
		if (MAIN_func_80107000() != 0) {
			SELECTION_MENU_STATE = 3;
			SCRIPT_STATE_3 = 0;
		} else {
			showMapHeadTextbox(1, owner, 0, 0x4d3);
			SELECTION_MENU_STATE = 1;
			SCRIPT_STATE_4 = 2;
			SCRIPT_STATE_3 = 1;
		}
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&MAIN_D_80134F68);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0x5000);
		MAIN_D_80135011 = 3;
		MAIN_func_80107110();
		showMapHeadTextbox(0, owner, 0, 0x4d3);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 4:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		SELECTION_MENU_STATE = 2;
		break;
	}
}

void MAIN_func_8010BB0C(void)
{
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 0x84, 6, 0xd2, 0x18,
				      6, 0x5a);
		MAIN_func_80107200();
		SELECTION_MENU_STATE = 3;
		SCRIPT_STATE_3 = 0;
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&MAIN_D_80134F68);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0x5000);
		MAIN_D_80135011 = 4;
		MAIN_func_80107110();
		showMapHeadTextbox(8, owner, 0, 0x4d3);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 4:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		SELECTION_MENU_STATE = 2;
		break;
	}
}

void MAIN_func_8010BC10(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 0x84, 6, 0x9a, 0x18,
				      6, 0x5a);
		initializeItemMenuBox(&MAIN_D_80134F6C, 0x100, 6, 0xb2, 0x18,
				      6, 0x5a);
		showMapHeadTextbox(0, owner, 0, 0x4d4);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&MAIN_D_80134F6C);
		destroyItemMenuBox(&MAIN_D_80134F68);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0);
		MAIN_func_800FC968(0);
		showMapheadSelection(1, 0xfd, 3, &selection, 0x4d4);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 4;
		SCRIPT_STATE_3 = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000);
		MAIN_D_80135011 = 6;
		MAIN_func_80107200();
		MAIN_func_80107110();
		showMapHeadTextbox(5, owner, 0, 0x4d4);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 5:
		setInputRepeatMask(0x5000);
		MAIN_D_80135011 = 7;
		MAIN_func_801072C4();
		MAIN_func_800FCA3C();
		showMapHeadTextbox(6, owner, 0, 0x4d4);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 6:
		triggerBoxCloseFlag(2);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	case 7:
		setInputRepeatMask(0);
		showMapheadSelection(2, 0xfd, 2, &selection, 0x4d4);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 8;
		SCRIPT_STATE_3 = 2;
		break;
	case 8:
		triggerBoxCloseFlag(1);
		MERIT += MAIN_D_8013500C;
		if (MERIT > 9999) MERIT = 9999;
		MAIN_D_80134F84 = 1;
		owner = getCardAmount(MAIN_D_80134F78);
		owner = (owner - 1u);
		setCardAmount(MAIN_D_80134F78, owner);
		SELECTION_MENU_STATE = 3;
		SCRIPT_STATE_3 = 0;
		MAIN_func_800FDFB4();
		return;
	case 10:
		setInputRepeatMask(0);
		/* fall through */
	case 9:
	case 0xd:
		triggerBoxCloseFlag(1);
		SELECTION_MENU_STATE = 3;
		SCRIPT_STATE_3 = 0;
		break;
	case 0xb:
		setInputRepeatMask(0);
		showMapheadSelection(3, 0xfd, 2, &selection, 0x4d4);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 0xc;
		SCRIPT_STATE_3 = 2;
		break;
	case 0xc:
		triggerBoxCloseFlag(1);
		MERIT -= MAIN_D_8013500C;
		MAIN_D_80134F84 = 1;
		giveItem(MAIN_D_80134F78, 1);
		SELECTION_MENU_STATE = 3;
		SCRIPT_STATE_3 = 0;
		MAIN_func_800FDFB4();
		break;
	}
}

void MAIN_func_8010BF68(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 0x100, 5, 0x88, 0x28,
				      6, 0x4a);
		initializeItemMenuBox(&MAIN_D_80134F6C,
				      INVENTORY_SIZE[0] << 1, 5, 0x88, 0x28,
				      6, 0x4a);
		showMapHeadTextbox(0, owner, 0, 0x4d5);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&MAIN_D_80134F6C);
		destroyItemMenuBox(&MAIN_D_80134F68);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		MAIN_D_80134F90 = 0;
		showMapheadSelection(1, 0xfd, 2, &selection, 0x4d5);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 4;
		SCRIPT_STATE_3 = 2;
		break;
	case 4:
		setInputRepeatMask(0x5060);
		MAIN_D_80135011 = 5;
		MAIN_func_80107444();
		MAIN_func_80107660();
		showMapHeadTextbox(3, owner, 0, 0x4d5);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 6:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		triggerBoxCloseFlag(2);
		SELECTION_MENU_STATE = 5;
		break;
	case 5:
		showMapHeadTextbox(2, owner, 0, 0x4d5);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	}
}

void openJukebox(void)
{
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 0x7e, 6, 0xd2, 0x18,
				      6, 0x5a);
		MAIN_func_80107784();
		showMapHeadTextbox(3, owner, 0, 0x4d6);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&MAIN_D_80134F68);
		playBGM(ACTIVE_BGM_FONT);
		readMapTFS(MAIN_D_80134FFE);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0x5000);
		MAIN_func_801078F4();
		showMapHeadTextbox(0, owner, 0, 0x4d6);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 4:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		showMapHeadTextbox(1, owner, 0, 0x4d6);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	}
}

void MAIN_func_8010C4B0(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 6, 3, 0, 0, 0, 0);
		initializeItemMenuBox(&MAIN_D_80134F6C, 6, 3, 0, 0, 0, 0);
		MAIN_func_80107C4C();
		writePStat(PSTAT_249, 255);
		showMapHeadTextbox(8, owner, 0, 0x4d6);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&MAIN_D_80134F6C);
		destroyItemMenuBox(&MAIN_D_80134F68);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0x5000);
		MAIN_func_80107D54();
		showMapHeadTextbox(9, owner, 0, 0x4d6);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 4:
		setInputRepeatMask(0);
		showMapheadSelection(0xc, 0xfd, 2, &selection, 0x4d6);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 5;
		SCRIPT_STATE_3 = 2;
		break;
	case 5:
	{
		int32_t giveResult;

		triggerBoxCloseFlag(1);

		giveResult = giveItem(MAIN_D_80134F6C->buf[
			(MAIN_D_80134F68->topRow +
			 MAIN_D_80134F68->cursor) * 2], 1);
		if (giveResult != 0) {
			uint8_t itemId = readPStat(PSTAT_249);
			removeItem(itemId, 1);
			MAIN_func_80107DFC();
			showMapHeadTextbox(0xd, owner, 0, 0x4d6);
		} else {
			showMapHeadTextbox(0xe, owner, 0, 0x4d6);
		}

		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		MAIN_func_800FDFB4();
		break;
	}
	case 6:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		showMapHeadTextbox(10, owner, 0, 0x4d6);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	}
}

void initializeNamingBuffer(uint8_t flags)
{
	if ((flags & 2) == 0) {
		MAIN_D_80134F8E = 0;
		SELECTION_MENU_STATE = 0;
		MAIN_D_801B1D1C[0] = 0;
	} else {
		closeBox(0);
		SELECTION_MENU_STATE = 0x14;
		MAIN_D_80134F8E = flags;

		if ((flags & 1) == 0) {
			strcpy(MAIN_D_801B1D1C, DIGIMON_DATA[0].name);
		} else {
			strcpy(MAIN_D_801B1D1C, PARTNER_ENTITY.name);
		}
	}

	ACTIVE_INSTRUCTION = 0x64;
	SCRIPT_STATE_3 = 0;
}

int32_t newGameStateMachine(void)
{
	int32_t i;

	switch (SELECTION_MENU_STATE) {
	case 0:
		setTrigger(TRIGGER_49);
		strcpy(DIGIMON_DATA[0].name, MAIN_D_80134607);
		setupNewGameDialogueBox();
		showNewgameDialogue(0x10, 2);
		MAIN_D_80134F98 = 0;
		break;
	case 2:
		showNewgameDialogue(0x11, 3);
		return 0;
	case 3:
		showNewgameSelection(0x12, 4);
		return 0;
	case 4:
		showNewgameDialogue(0x13, 6);
		return 0;
	case 5:
		showNewgameDialogue(0x16, 10);
		return 0;
	case 6:
		showNewgameDialogue(0x14, 7);
		return 0;
	case 7:
		showNewgameSelection(0x15, 8);
		return 0;
	case 8:
		writePStat(PSTAT_254, 0);
		SELECTION_MENU_STATE = 0x11;
		break;
	case 9:
		writePStat(PSTAT_254, 1);
		SELECTION_MENU_STATE = 0x11;
		break;
	case 10:
		showNewgameDialogue(0x17, 0xb);
		return 0;
	case 0xb:
		showNewgameSelection(0x18, 8);
		return 0;
	case 0x11:
		showNewgameDialogue(0x19, 0x12);
		return 0;
	case 0x12:
		showNewgameDialogue(0x1a, 0x13);
		return 0;
	case 0x13:
		closeBox(0);
		SELECTION_MENU_STATE = 0x14;
		break;
	case 0x14:
		setInputRepeatMask(0xf000);
		setupNameSelectorBox();
		setupNameDisplayBox();
		MAIN_D_80134F98 = 1;
		SELECTION_MENU_STATE = 1;
		break;
	case 0x15:
		setInputRepeatMask(0);

		if ((MAIN_D_80134F8E & 1) == 0) {
			strcpy(DIGIMON_DATA[0].name, MAIN_D_801B1D1C);
		} else {
			strcpy(PARTNER_ENTITY.name, MAIN_D_801B1D1C);
		}

		if ((MAIN_D_80134F8E & 2) != 0) {
			triggerBoxCloseFlag(1);
			triggerBoxCloseFlag(2);
			MAIN_D_80134F98 = 0;
			SELECTION_MENU_STATE = 0x16;
		} else {
			triggerBoxCloseFlag(1);
			triggerBoxCloseFlag(2);
			MAIN_D_80134F98 = 0;
			SELECTION_MENU_STATE = 0x16;
		}
		break;
	case 0x16:
		if ((MAIN_D_80134F8E & 2) != 0) {
			for (i = 0; i < 6; i++) {
				if (UI_BOX_DATA[1].state != 0) {
					return 0;
				}
			}

			return 1;
		}

		setupNewGameDialogueBox();

		if ((MAIN_D_80134F8E & 1) == 0) {
			showNewgameDialogue(0x1b, 0x17);
		} else {
			showNewgameDialogue(0x1e, 0x1b);
		}
		break;
	case 0x17:
		showNewgameSelection(0x1c, 0x18);
		return 0;
	case 0x18:
		showNewgameDialogue(0x1d, 0x1a);
		return 0;
	case 0x19:
		SELECTION_MENU_STATE = 0x13;
		break;
	case 0x1a:
		MAIN_D_80134F8E |= 1;
		MAIN_D_801B1D1C[0] = 0;
		SELECTION_MENU_STATE = 0x13;
		break;
	case 0x1b:
		showNewgameSelection(0x1c, 0x1c);
		return 0;
	case 0x1c:
		showNewgameDialogue(0x1f, 0x1e);
		return 0;
	case 0x1d:
		SELECTION_MENU_STATE = 0x13;
		break;
	case 0x1e:
		showNewgameDialogue(0x20, 0x1f);
		return 0;
	case 0x1f:
		closeBox(0);
		unsetTrigger(TRIGGER_49);
		return 1;
	}

	return 0;
}

void initializeFontCLUT(void)
{
	RECT rect;

	clearTextArea();
	rect.x = 208;
	rect.y = 488;
	rect.w = 16;
	rect.h = 1;
	LoadImage(&rect, MAIN_D_80130FE8);
	MAIN_D_8013502C = 0;
}

void clearTextArea(void)
{
	RECT rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = 256;
	rect.h = 256;
	clearTextSubArea(&rect);
}

void clearTextSubArea(RECT *rect)
{
	rect->x = (rect->x / 4) + 704;
	rect->y += 256;
	rect->w = rect->w >> 2;
	ClearImage(rect, 0, 0, 0);
}

void setTextColor(int32_t color)
{
	COLORCODE_LOWBITS = color & 0xf;
	COLORCODE_HIGHBITS = COLORCODE_LOWBITS << 4;
}

int32_t drawGlyph(uint16_t glyph, int32_t x, int32_t y)
{
	RECT rect;
	uint16_t *g;
	uint8_t *out;
	uint16_t bits;
	uint8_t px;
	int32_t n;
	int32_t off;
	int32_t i;

	if ((uint32_t)x >= 0xf4) {
		return 0;
	}

	if ((uint32_t)y >= 0xf4) {
		return 0;
	}

	glyph = ((glyph & 0xff) << 8) + ((glyph & 0xff00) >> 8);
	MAIN_D_8013087C[0] = glyph;

	for (n = 0; ; ++n) {
		if (glyph == CHAR_TO_GLYPH_TABLE[n]) {
			break;
		}
	}

	g = &GLYPH_DATA[n * 12];
	MAIN_D_8013502C = (MAIN_D_8013502C + 1) % 0x32;
	out = MAIN_D_801BE958 + MAIN_D_8013502C * 72;
	for (i = 0; i < 0xb; i++) {
		bits = *g++;

		px = 0;
		if ((bits & 0x8000) == 0) {
			px |= COLORCODE_LOWBITS;
		}

		if ((bits & 0x4000) == 0) {
			px |= COLORCODE_HIGHBITS;
		}

		*out++ = px;

		px = 0;
		if ((bits & 0x2000) == 0) {
			px |= COLORCODE_LOWBITS;
		}

		if ((bits & 0x1000) == 0) {
			px |= COLORCODE_HIGHBITS;
		}

		*out++ = px;

		px = 0;
		if ((bits & 0x800) == 0) {
			px |= COLORCODE_LOWBITS;
		}

		if ((bits & 0x400) == 0) {
			px |= COLORCODE_HIGHBITS;
		}

		*out++ = px;

		px = 0;
		if ((bits & 0x200) == 0) {
			px |= COLORCODE_LOWBITS;
		}

		if ((bits & 0x100) == 0) {
			px |= COLORCODE_HIGHBITS;
		}

		*out++ = px;

		px = 0;
		if ((bits & 0x80) == 0) {
			px |= COLORCODE_LOWBITS;
		}

		if ((bits & 0x40) == 0) {
			px |= COLORCODE_HIGHBITS;
		}

		*out++ = px;

		px = 0;
		if ((bits & 0x20) == 0) {
			px |= COLORCODE_LOWBITS;
		}

		if ((bits & 0x10) == 0) {
			px |= COLORCODE_HIGHBITS;
		}

		*out++ = px;
	}

	rect.x = x / 4 + 0x2c0;
	rect.y = y + 0x100;
	rect.w = 3;
	rect.h = 0xb;
	LoadImage(&rect, (u_long *)(MAIN_D_801BE958 + MAIN_D_8013502C * 72));

	return *g;
}

void drawString(char *str, uint16_t x, int32_t y)
{
	uint16_t glyph;
	uint16_t hi;
	uint16_t lo;

	while (*str != 0) {
		if (isAsciiEncoded(str) != 0) {
			if (*str == 0x2e) {
				glyph = 0x8142;
			} else if (*str == 0x27) {
				glyph = 0x8175;
			} else {
				glyph = convertAsciiToJis(*str);
			}

			glyph = swapShortBytes(glyph);
			str++;
		} else {
			lo = *str;
			str++;
			hi = *str;
			lo = (uint8_t)lo;
			hi = (uint8_t)hi;
			lo = lo << 8;
			str++;
			glyph = swapShortBytes((uint16_t)(lo | hi));
		}

		x += drawGlyph(glyph, x, y);
	}
}
