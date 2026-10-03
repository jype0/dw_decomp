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
	openMojyamonShop,
	tickBirdraTransport,
	openJukebox,
	tickItemKeeper,
	openMeritShop,
	openSellCardMenu,
	openBuyCardMenu,
	rollCardPack,
	openRecycleShop,
};

void openRecycleShop(void)
{
	uint8_t owner = readPStat(PSTAT_254);
	int32_t hasItems;

	switch (SELECTION_MENU_STATE) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0x9c, 6, 0xd2,
		                    0x18, 6, 0x5a);
		hasItems = fillRecycleShopItemList();
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
		/* fall through */
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&ITEM_MENU_LEFT);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		createShopBitsBox(1);
		showShopkeepSelection(3, 0xfd, 2, &MAIN_D_80134F70);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 4;
		SCRIPT_STATE_3 = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 2;
		createItemMenu();
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
		hasItems = fillRecycleShopItemList();
		if (hasItems != 0) {
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

void openBuyCardMenu(void)
{
	uint8_t owner = readPStat(PSTAT_254);
	int32_t hasItems;

	switch (SELECTION_MENU_STATE) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0xc, 6, 0xb2, 0x18, 6,
		                    0x5a);
		hasItems = hasAnyCardToBuy();
		if (hasItems != 0) {
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
		createCardMenu();
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

void openSellCardMenu(void)
{
	uint8_t owner = readPStat(PSTAT_254);
	int32_t hasItems;

	switch (SELECTION_MENU_STATE) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0x84, 6, 0xd2, 0x18,
		                    6, 0x5a);
		hasItems = shopFillSellCardList();
		SELECTION_MENU_STATE = 3;
		SCRIPT_STATE_3 = 0;
		/* fall through */
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&ITEM_MENU_LEFT);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 4;
		createCardMenu();
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

void openMeritShop(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0x84, 6, 0x9a, 0x18,
		                    6, 0x5a);
		allocateItemMenuBox(&ITEM_MENU_RIGHT, 0x100, 6, 0xb2, 0x18,
		                    6, 0x5a);
		showMapHeadTextbox(0, owner, 0, 0x4d4);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		/* fall through */
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&ITEM_MENU_RIGHT);
		destroyItemMenuBox(&ITEM_MENU_LEFT);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0);
		createShopBitsBox(0);
		showMapheadSelection(1, 0xfd, 3, &selection, 0x4d4);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 4;
		SCRIPT_STATE_3 = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 6;
		shopFillSellCardList();
		createCardMenu();
		showMapHeadTextbox(5, owner, 0, 0x4d4);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 5:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 7;
		shopFillMeritItemList();
		createItemMenu();
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
		owner -= 1;
		setCardAmount(SHOP_ITEM_TYPE, owner);
		SELECTION_MENU_STATE = 3;
		SCRIPT_STATE_3 = 0;
		playShopSoundOnlyInSavannah();
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
		playShopSoundOnlyInSavannah();
		break;
	}
}

void tickItemKeeper(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0x100, 5, 0x88, 0x28,
		                    6, 0x4a);
		allocateItemMenuBox(&ITEM_MENU_RIGHT,
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
#if defined(VERSION_JP)
		setInputRepeatMask(0x5030);
#else
		setInputRepeatMask(0x5060);
#endif
		ITEM_MENU_TYPE = 5;
		itemKeeperFillItemList();
		createItemKeepWindow();
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
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0x7e, 6, 0xd2, 0x18,
		                    6, 0x5a);
		jukeboxFillTitleList();
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
		createJukeboxMenu();
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

void tickBirdraTransport(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0xc, 6, 0xd2, 0x18, 6,
		                    0x5a);
		birdraFillTargetList();
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
		createShopBitsBox(1);
		createBirdraTransportMenu();
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
		int32_t row;

		triggerBoxCloseFlag(2);
		triggerBoxCloseFlag(1);

		row = (ITEM_MENU_LEFT->topRow + ITEM_MENU_LEFT->cursor) * 2;
		idx = ITEM_MENU_LEFT->buf[row];
		idx &= 0x7f;
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

void openMojyamonShop(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_254);

	switch (SELECTION_MENU_STATE) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 6, 3, 0, 0, 0, 0);
		allocateItemMenuBox(&ITEM_MENU_RIGHT, 6, 3, 0, 0, 0, 0);
		mojyaTradeFillItemList();
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
		createMojyaTradeMenu();
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
		uint8_t itemId;
		int32_t row;

		triggerBoxCloseFlag(1);

		row = (ITEM_MENU_LEFT->topRow + ITEM_MENU_LEFT->cursor) * 2;
		itemId = ITEM_MENU_RIGHT->buf[row];
		if (giveItem(itemId, 1) != 0) {
			itemId = readPStat(PSTAT_249);
			removeItem(itemId, 1);
			setMojyaItemTradedTrigger();
			showMapHeadTextbox(0xd, owner, 0, 0x4d6);
		} else {
			showMapHeadTextbox(0xe, owner, 0, 0x4d6);
		}

		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		playShopSoundOnlyInSavannah();
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
		MAIN_D_80134F8E = flags;
		SELECTION_MENU_STATE = 0x14;

		if ((MAIN_D_80134F8E & 1) == 0) {
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
#if defined(VERSION_JP)
		strcpy(DIGIMON_DATA[0].name, "？？？");
#else
		strcpy(DIGIMON_DATA[0].name, &MAIN_D_80134600[7]);
#endif
		setupNewGameDialogueBox();
		showNewGameDialogue(0x10, 2);
#if !defined(VERSION_JP)
		MAIN_D_80134F98 = 0;
#endif
		break;
#if defined(VERSION_JP)
	case 1:
		break;
#endif
	case 2:
		showNewGameDialogue(0x11, 3);
		break;
	case 3:
		showNewGameSelection(0x12, 4);
		break;
	case 4:
		showNewGameDialogue(0x13, 6);
		break;
	case 5:
		showNewGameDialogue(0x16, 10);
		break;
	case 6:
		showNewGameDialogue(0x14, 7);
		break;
	case 7:
		showNewGameSelection(0x15, 8);
		break;
	case 8:
		writePStat(PSTAT_254, 0);
		SELECTION_MENU_STATE = 0x11;
		break;
	case 9:
		writePStat(PSTAT_254, 1);
		SELECTION_MENU_STATE = 0x11;
		break;
	case 10:
		showNewGameDialogue(0x17, 0xb);
		break;
	case 0xb:
		showNewGameSelection(0x18, 8);
		break;
	case 0x11:
		showNewGameDialogue(0x19, 0x12);
		break;
	case 0x12:
		showNewGameDialogue(0x1a, 0x13);
		break;
	case 0x13:
		closeBox(0);
		SELECTION_MENU_STATE = 0x14;
		break;
	case 0x14:
		setInputRepeatMask(0xf000);
		setupNameSelectorBox();
		setupNameDisplayBox();
#if !defined(VERSION_JP)
		MAIN_D_80134F98 = 1;
#endif
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
#if !defined(VERSION_JP)
			MAIN_D_80134F98 = 0;
#endif
			SELECTION_MENU_STATE = 0x16;
		} else {
			triggerBoxCloseFlag(1);
			triggerBoxCloseFlag(2);
#if !defined(VERSION_JP)
			MAIN_D_80134F98 = 0;
#endif
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
			showNewGameDialogue(0x1b, 0x17);
		} else {
			showNewGameDialogue(0x1e, 0x1b);
		}
		break;
	case 0x17:
		showNewGameSelection(0x1c, 0x18);
		break;
	case 0x18:
		showNewGameDialogue(0x1d, 0x1a);
		break;
	case 0x19:
		SELECTION_MENU_STATE = 0x13;
		break;
	case 0x1a:
		MAIN_D_80134F8E |= 1;
		MAIN_D_801B1D1C[0] = 0;
		SELECTION_MENU_STATE = 0x13;
		break;
	case 0x1b:
		showNewGameSelection(0x1c, 0x1c);
		break;
	case 0x1c:
		showNewGameDialogue(0x1f, 0x1e);
		break;
	case 0x1d:
		SELECTION_MENU_STATE = 0x13;
		break;
	case 0x1e:
		showNewGameDialogue(0x20, 0x1f);
		break;
	case 0x1f:
		closeBox(0);
		unsetTrigger(TRIGGER_49);
		return 1;
	}

	return 0;
}
