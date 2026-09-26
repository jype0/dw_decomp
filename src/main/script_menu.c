#include <string.h>

#include <dw/item.h>
#include <dw/params.h>
#include <dw/pstat.h>
#include <dw/script.h>
#include <dw/trigger.h>
#include <dw/ui.h>

extern uint8_t MAIN_D_80134F90;
extern char MAIN_D_80134600[8];
extern int32_t CURRENT_SCRIPT_PTR;

static void *script_menu_text_order[] = {
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

void MAIN_func_8010B648(void)
{
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0: {
		int32_t hasItems;
		initializeItemMenuBox(&ITEM_MENU_LEFT, 0x9c, 6, 0xd2,
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
		destroyItemMenuBox(&ITEM_MENU_LEFT);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		createShopBitBox(1);
		MAIN_func_800FCA14(3, 0xfd, 2, &MAIN_D_80134F70);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 4;
		SCRIPT_STATE_3 = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 2;
		createItemMenuBox();
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
		initializeItemMenuBox(&ITEM_MENU_LEFT, 0xc, 6, 0xb2, 0x18, 6,
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
		destroyItemMenuBox(&ITEM_MENU_LEFT);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 3;
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
		initializeItemMenuBox(&ITEM_MENU_LEFT, 0x84, 6, 0xd2, 0x18,
		                      6, 0x5a);
		MAIN_func_80107200();
		SELECTION_MENU_STATE = 3;
		SCRIPT_STATE_3 = 0;
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&ITEM_MENU_LEFT);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 4;
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
		initializeItemMenuBox(&ITEM_MENU_LEFT, 0x84, 6, 0x9a, 0x18,
		                      6, 0x5a);
		initializeItemMenuBox(&ITEM_MENU_RIGHT, 0x100, 6, 0xb2, 0x18,
		                      6, 0x5a);
		showMapHeadTextbox(0, owner, 0, 0x4d4);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&ITEM_MENU_RIGHT);
		destroyItemMenuBox(&ITEM_MENU_LEFT);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0);
		createShopBitBox(0);
		showMapheadSelection(1, 0xfd, 3, &selection, 0x4d4);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 4;
		SCRIPT_STATE_3 = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 6;
		MAIN_func_80107200();
		MAIN_func_80107110();
		showMapHeadTextbox(5, owner, 0, 0x4d4);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 5:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 7;
		MAIN_func_801072C4();
		createItemMenuBox();
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
		if (MERIT > 9999) {
			MERIT = 9999;
		}
		UPDATE_SHOP_BIT_BOX = 1;
		owner = getCardAmount(SHOP_ITEM_TYPE);
		owner = (owner - 1u);
		setCardAmount(SHOP_ITEM_TYPE, owner);
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
		UPDATE_SHOP_BIT_BOX = 1;
		giveItem(SHOP_ITEM_TYPE, 1);
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
		initializeItemMenuBox(&ITEM_MENU_LEFT, 0x100, 5, 0x88, 0x28,
		                      6, 0x4a);
		initializeItemMenuBox(&ITEM_MENU_RIGHT,
		                      INVENTORY.size << 1, 5, 0x88, 0x28,
		                      6, 0x4a);
		showMapHeadTextbox(0, owner, 0, 0x4d5);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&ITEM_MENU_RIGHT);
		destroyItemMenuBox(&ITEM_MENU_LEFT);
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
		ITEM_MENU_TYPE = 5;
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
		initializeItemMenuBox(&ITEM_MENU_LEFT, 0x7e, 6, 0xd2, 0x18,
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
		destroyItemMenuBox(&ITEM_MENU_LEFT);
		playBGM(ACTIVE_BGM_FONT);
		readMapTFS(CURRENT_MAP_ID);
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

void MAIN_func_8010C28C(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&ITEM_MENU_LEFT, 0xc, 6, 0xd2, 0x18, 6,
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
		destroyItemMenuBox(&ITEM_MENU_LEFT);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		createShopBitBox(1);
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
	case 5: {
		uint8_t idx;

		triggerBoxCloseFlag(2);
		triggerBoxCloseFlag(1);

		idx = ITEM_MENU_LEFT->buf[(ITEM_MENU_LEFT->topRow + ITEM_MENU_LEFT->cursor) * 2] & 0x7f;
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

void MAIN_func_8010C4B0(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&ITEM_MENU_LEFT, 6, 3, 0, 0, 0, 0);
		initializeItemMenuBox(&ITEM_MENU_RIGHT, 6, 3, 0, 0, 0, 0);
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
		destroyItemMenuBox(&ITEM_MENU_RIGHT);
		destroyItemMenuBox(&ITEM_MENU_LEFT);
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
	case 5: {
		int32_t giveResult;

		triggerBoxCloseFlag(1);

		giveResult = giveItem(ITEM_MENU_RIGHT->buf[(ITEM_MENU_LEFT->topRow +
		                                            ITEM_MENU_LEFT->cursor) *
		                                           2],
		                      1);
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

	ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
	SCRIPT_STATE_3 = 0;
}

int32_t newGameStateMachine(void)
{
	int32_t i;

	switch (SELECTION_MENU_STATE) {
	case 0:
		setTrigger(TRIGGER_49);
		strcpy(DIGIMON_DATA[0].name, &MAIN_D_80134600[7]);
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
