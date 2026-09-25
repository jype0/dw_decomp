#include <string.h>

#include <dw/item.h>
#include <dw/params.h>
#include <dw/pstat.h>
#include <dw/script.h>
#include <dw/text.h>
#include <dw/trigger.h>
#include <dw/ui.h>

extern uint8_t ITEM_KEEPER_SIDE;
extern char NAMING_BUTTON_LABELS[8];
extern int32_t CURRENT_SCRIPT_PTR;

static void *script_menu_text_order[] = {
	newGameStateMachine,
	initializeNamingBuffer,
	tickCollectorTrade,
	tickTransport,
	openJukebox,
	tickItemKeeper,
	tickMeritShop,
	tickCardSellShop,
	tickCardShop,
	rollCardPack,
	tickLostItemShop,
};

void tickLostItemShop(void)
{
	uint8_t owner = readPStat(PSTAT_BUILTIN_ARG);

	switch (SELECTION_MENU_STATE) {
	case 0: {
		int32_t hasItems;
		initializeItemMenuBox(&MAIN_D_80134F68, 0x9c, 6, 0xd2,
		                      0x18, 6, 0x5a);
		hasItems = fillLostItemList();
		SHOP_CHOICE = 0;
		SHOP_MADE_A_DEAL = 0;

		if (hasItems != 0) {
			showShopkeeperTextbox(SHOP_TEXT_WELCOME, owner, 0);
			SELECTION_MENU_STATE = 1;
			SCRIPT_STATE_4 = 3;
			SCRIPT_STATE_3 = 1;
		} else {
			showShopkeeperTextbox(SHOP_TEXT_NOTHING_TO_SELL, owner, 0);
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
		openMoneyBox(1);
		showShopkeeperSelection(SHOP_TEXT_BUY_LEAVE, SPEAKER_PLAYER, 2, &SHOP_CHOICE);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 4;
		SCRIPT_STATE_3 = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000);
		ITEM_MENU_MODE = ITEM_MENU_BUY_LOST;
		openItemMenuBox();
		showShopkeeperTextbox(SHOP_TEXT_WHAT_TO_BUY, owner, 0);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 5:
	case 6:
		triggerBoxCloseFlag(2);
		if (SHOP_MADE_A_DEAL != 0) {
			showShopkeeperTextbox(SHOP_TEXT_COME_AGAIN, owner, 0);
		} else {
			showShopkeeperTextbox(SHOP_TEXT_BOUGHT_NOTHING, owner, 0);
		}
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	case 7:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		if (isPartnerBaby() != 0) {
			showShopkeeperTextbox(SHOP_TEXT_BABY_DISCOUNT, owner, 0);
		} else {
			showShopkeeperTextbox(SHOP_TEXT_THANKS, owner, 0);
		}
		SHOP_MADE_A_DEAL = 1;
		if (fillLostItemList() != 0) {
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
		showShopkeeperTextbox(SHOP_TEXT_ANYTHING_ELSE, owner, 0);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	case 9:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		showShopkeeperTextbox(SHOP_TEXT_NO, owner, 0);
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
		writePStat(PSTAT_SELECTED, rollCard());
		showCardTextbox();
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	case 1:
		break;
	case 2:
		writePStat(PSTAT_SELECTED, rollCard());
		showCardTextbox();
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	case 3:
		writePStat(PSTAT_SELECTED, rollCard());
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

void tickCardShop(void)
{
	uint8_t owner = readPStat(PSTAT_BUILTIN_ARG);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 0xc, 6, 0xb2, 0x18, 6,
		                      0x5a);
		if (fillCardShopList() != 0) {
			SELECTION_MENU_STATE = 3;
			SCRIPT_STATE_3 = 0;
		} else {
			/* "We don't have any more." */
			showMapHeadTextbox(1, owner, 0, MAPHEAD_TEXT_CARD_SHOP);
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
		ITEM_MENU_MODE = ITEM_MENU_BUY_CARD;
		openCardMenuBox();
		/* "Look what we got today." */
		showMapHeadTextbox(0, owner, 0, MAPHEAD_TEXT_CARD_SHOP);
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

void tickCardSellShop(void)
{
	uint8_t owner = readPStat(PSTAT_BUILTIN_ARG);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 0x84, 6, 0xd2, 0x18,
		                      6, 0x5a);
		fillOwnedCardList();
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
		ITEM_MENU_MODE = ITEM_MENU_SELL_CARD;
		openCardMenuBox();
		/* "Which card do you want / to sell?" */
		showMapHeadTextbox(8, owner, 0, MAPHEAD_TEXT_CARD_SHOP);
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

void tickMeritShop(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_BUILTIN_ARG);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 0x84, 6, 0x9a, 0x18,
		                      6, 0x5a);
		initializeItemMenuBox(&MAIN_D_80134F6C, 0x100, 6, 0xb2, 0x18,
		                      6, 0x5a);
		/* "You want 'Merit' too? / What are you going to do?" */
		showMapHeadTextbox(0, owner, 0, MAPHEAD_TEXT_MERIT_SHOP);
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
		openMoneyBox(0);
		/* "Show a Digimon card / Use Merit points / See ya" */
		showMapheadSelection(1, SPEAKER_PLAYER, 3, &selection, MAPHEAD_TEXT_MERIT_SHOP);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 4;
		SCRIPT_STATE_3 = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000);
		ITEM_MENU_MODE = ITEM_MENU_MERIT_CARD;
		fillOwnedCardList();
		openCardMenuBox();
		/* "What did you bring?" */
		showMapHeadTextbox(5, owner, 0, MAPHEAD_TEXT_MERIT_SHOP);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 5:
		setInputRepeatMask(0x5000);
		ITEM_MENU_MODE = ITEM_MENU_MERIT_ITEM;
		fillMeritItemList();
		openItemMenuBox();
		/* "What do you want?" */
		showMapHeadTextbox(6, owner, 0, MAPHEAD_TEXT_MERIT_SHOP);
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
		/* "Give it / Cancel" */
		showMapheadSelection(2, SPEAKER_PLAYER, 2, &selection, MAPHEAD_TEXT_MERIT_SHOP);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 8;
		SCRIPT_STATE_3 = 2;
		break;
	case 8:
		triggerBoxCloseFlag(1);
		MERIT += SCRIPT_PRICE;
		if (MERIT > 9999) {
			MERIT = 9999;
		}
		MONEY_BOX_DIRTY = 1;
		owner = getCardAmount(SELECTED_ITEM);
		owner = (owner - 1u);
		setCardAmount(SELECTED_ITEM, owner);
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
		/* "OK / Cancel" */
		showMapheadSelection(3, SPEAKER_PLAYER, 2, &selection, MAPHEAD_TEXT_MERIT_SHOP);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 0xc;
		SCRIPT_STATE_3 = 2;
		break;
	case 0xc:
		triggerBoxCloseFlag(1);
		MERIT -= SCRIPT_PRICE;
		MONEY_BOX_DIRTY = 1;
		giveItem(SELECTED_ITEM, 1);
		SELECTION_MENU_STATE = 3;
		SCRIPT_STATE_3 = 0;
		MAIN_func_800FDFB4();
		break;
	}
}

void tickItemKeeper(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_BUILTIN_ARG);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 0x100, 5, 0x88, 0x28,
		                      6, 0x4a);
		initializeItemMenuBox(&MAIN_D_80134F6C,
		                      INVENTORY.size << 1, 5, 0x88, 0x28,
		                      6, 0x4a);
		/* "Do you have any business / at the Item Keeper?" */
		showMapHeadTextbox(0, owner, 0, MAPHEAD_TEXT_ITEM_KEEPER);
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
		ITEM_KEEPER_SIDE = 0;
		/* "Yes / No" */
		showMapheadSelection(1, SPEAKER_PLAYER, 2, &selection, MAPHEAD_TEXT_ITEM_KEEPER);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 4;
		SCRIPT_STATE_3 = 2;
		break;
	case 4:
		setInputRepeatMask(0x5060);
		ITEM_MENU_MODE = ITEM_MENU_PICK_ITEM;
		fillItemKeeperLists();
		openItemKeeperBoxes();
		/* "Use △ to move Items. / Use ○ to move by tens / and □ to move all." */
		showMapHeadTextbox(3, owner, 0, MAPHEAD_TEXT_ITEM_KEEPER);
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
		/* "Come to this shop / when you need it." */
		showMapHeadTextbox(2, owner, 0, MAPHEAD_TEXT_ITEM_KEEPER);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	}
}

void openJukebox(void)
{
	uint8_t owner = readPStat(PSTAT_BUILTIN_ARG);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 0x7e, 6, 0xd2, 0x18,
		                      6, 0x5a);
		fillJukeboxList();
		/* "Why don't you request a / number that will make me hot?" */
		showMapHeadTextbox(3, owner, 0, MAPHEAD_TEXT_SERVICES);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&MAIN_D_80134F68);
		playBGM(ACTIVE_BGM_FONT);
		readMapTFS(CURRENT_MAP_ID);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0x5000);
		openJukeboxMenuBox();
		/* "Which one do you want?" */
		showMapHeadTextbox(0, owner, 0, MAPHEAD_TEXT_SERVICES);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 4:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		/* "Help me on my next request." */
		showMapHeadTextbox(1, owner, 0, MAPHEAD_TEXT_SERVICES);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	}
}

void tickTransport(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_BUILTIN_ARG);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 0xc, 6, 0xd2, 0x18, 6,
		                      0x5a);
		fillTransportList();
		/* "Where should I take you?" */
		showMapHeadTextbox(4, owner, 0, MAPHEAD_TEXT_SERVICES);
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
		openMoneyBox(1);
		openTransportMenuBox();
		setInputRepeatMask(0x5000);
		SELECTION_MENU_STATE = 1;
		break;
	case 4:
		setInputRepeatMask(0);
		/* "I know / Cancel" */
		showMapheadSelection(7, SPEAKER_PLAYER, 2, &selection, MAPHEAD_TEXT_SERVICES);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 5;
		SCRIPT_STATE_3 = 2;
		break;
	case 5: {
		uint8_t idx;

		triggerBoxCloseFlag(2);
		triggerBoxCloseFlag(1);

		idx = MAIN_D_80134F68->buf[(MAIN_D_80134F68->topRow + MAIN_D_80134F68->cursor) * 2] & 0x7f;
		writePStat(PSTAT_TRANSPORT_MAP, TRANSPORT_DESTINATIONS[idx].mapId);
		writePStat(PSTAT_TRANSPORT_EXIT, TRANSPORT_DESTINATIONS[idx].unk_0x1);
		CURRENT_SCRIPT_PTR = (int32_t)getScript(0);
		SCRIPT_PC = getScriptSection((uint8_t *)CURRENT_SCRIPT_PTR, 0x4e3);
		MONEY -= SCRIPT_PRICE;
		SELECTION_MENU_STATE = 2;
		break;
	}
	case 6:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(2);
		triggerBoxCloseFlag(1);
		/* "I think that going yourself / may be the best way." */
		showMapHeadTextbox(5, owner, 0, MAPHEAD_TEXT_SERVICES);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	}
}

void tickCollectorTrade(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_BUILTIN_ARG);

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 6, 3, 0, 0, 0, 0);
		initializeItemMenuBox(&MAIN_D_80134F6C, 6, 3, 0, 0, 0, 0);
		fillTradeLists();
		writePStat(PSTAT_SELECTED, 255);
		/* "I'm a collector. If you got / a thing I want, I'll trade." */
		showMapHeadTextbox(8, owner, 0, MAPHEAD_TEXT_SERVICES);
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
		openTradeMenuBox();
		/* "What will you give me?" */
		showMapHeadTextbox(9, owner, 0, MAPHEAD_TEXT_SERVICES);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 4:
		setInputRepeatMask(0);
		/* "Yeah / Well..., no" */
		showMapheadSelection(0xc, SPEAKER_PLAYER, 2, &selection, MAPHEAD_TEXT_SERVICES);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 5;
		SCRIPT_STATE_3 = 2;
		break;
	case 5: {
		int32_t giveResult;

		triggerBoxCloseFlag(1);

		giveResult = giveItem(MAIN_D_80134F6C->buf[(MAIN_D_80134F68->topRow +
		                                            MAIN_D_80134F68->cursor) *
		                                           2],
		                      1);
		if (giveResult != 0) {
			uint8_t itemId = readPStat(PSTAT_SELECTED);
			removeItem(itemId, 1);
			markTradeDone();
			/* "Thanks!" */
			showMapHeadTextbox(0xd, owner, 0, MAPHEAD_TEXT_SERVICES);
		} else {
			/* "Too many Items." */
			showMapHeadTextbox(0xe, owner, 0, MAPHEAD_TEXT_SERVICES);
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
		/* "I'll trade all the time, / so bring whatever you got." */
		showMapHeadTextbox(10, owner, 0, MAPHEAD_TEXT_SERVICES);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	}
}

void initializeNamingBuffer(uint8_t flags)
{
	if ((flags & NAMING_FROM_SCRIPT) == 0) {
		NAMING_FLAGS = 0;
		SELECTION_MENU_STATE = 0;
		NAMING_BUFFER[0] = 0;
	} else {
		closeBox(0);
		SELECTION_MENU_STATE = 0x14;
		NAMING_FLAGS = flags;

		if ((flags & NAMING_PARTNER) == 0) {
			strcpy(NAMING_BUFFER, DIGIMON_DATA[0].name);
		} else {
			strcpy(NAMING_BUFFER, PARTNER_ENTITY.name);
		}
	}

	ACTIVE_INSTRUCTION = SCRIPT_OP_BUILTIN;
	SCRIPT_STATE_3 = 0;
}

int32_t newGameStateMachine(void)
{
	int32_t i;

	switch (SELECTION_MENU_STATE) {
	case 0:
		setTrigger(TRIGGER_49);
		strcpy(DIGIMON_DATA[0].name, &NAMING_BUTTON_LABELS[7]);
		setupNewGameDialogueBox();
		/* "Jijimon: I'm Jijimon. Tell me / about yourself." */
		showNewgameDialogue(0x10, 2);
		TEXT_MONOSPACE = 0;
		break;
	case 2:
		/* "Jijimon: Do you have a Digivice?" */
		showNewgameDialogue(0x11, 3);
		return 0;
	case 3:
		/* "<player>: Yes / No" */
		showNewgameSelection(0x12, 4);
		return 0;
	case 4:
		/* "Jijimon: I see." */
		showNewgameDialogue(0x13, 6);
		return 0;
	case 5:
		/* "Jijimon: I see." */
		showNewgameDialogue(0x16, 10);
		return 0;
	case 6:
		/* "Jijimon: Next question. Which do you / like better, day or night?" */
		showNewgameDialogue(0x14, 7);
		return 0;
	case 7:
		/* "<player>: Day / Night" */
		showNewgameSelection(0x15, 8);
		return 0;
	case 8:
		writePStat(PSTAT_BUILTIN_ARG, 0);
		SELECTION_MENU_STATE = 0x11;
		break;
	case 9:
		writePStat(PSTAT_BUILTIN_ARG, 1);
		SELECTION_MENU_STATE = 0x11;
		break;
	case 10:
		/* "Jijimon: Do you have friends?" */
		showNewgameDialogue(0x17, 0xb);
		return 0;
	case 0xb:
		/* "<player>: Yes / No" */
		showNewgameSelection(0x18, 8);
		return 0;
	case 0x11:
		/* "Jijimon: Alright." */
		showNewgameDialogue(0x19, 0x12);
		return 0;
	case 0x12:
		/* "Jijimon: What's your name?" */
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
		TEXT_MONOSPACE = 1;
		SELECTION_MENU_STATE = 1;
		break;
	case 0x15:
		setInputRepeatMask(0);

		if ((NAMING_FLAGS & NAMING_PARTNER) == 0) {
			strcpy(DIGIMON_DATA[0].name, NAMING_BUFFER);
		} else {
			strcpy(PARTNER_ENTITY.name, NAMING_BUFFER);
		}

		if ((NAMING_FLAGS & NAMING_FROM_SCRIPT) != 0) {
			triggerBoxCloseFlag(1);
			triggerBoxCloseFlag(2);
			TEXT_MONOSPACE = 0;
			SELECTION_MENU_STATE = 0x16;
		} else {
			triggerBoxCloseFlag(1);
			triggerBoxCloseFlag(2);
			TEXT_MONOSPACE = 0;
			SELECTION_MENU_STATE = 0x16;
		}
		break;
	case 0x16:
		if ((NAMING_FLAGS & NAMING_FROM_SCRIPT) != 0) {
			for (i = 0; i < 6; i++) {
				if (UI_BOX_DATA[1].state != 0) {
					return 0;
				}
			}

			return 1;
		}

		setupNewGameDialogueBox();

		if ((NAMING_FLAGS & NAMING_PARTNER) == 0) {
			/* "Jijimon: <player>, right?" */
			showNewgameDialogue(0x1b, 0x17);
		} else {
			/* "Jijimon: <partner>, right?" */
			showNewgameDialogue(0x1e, 0x1b);
		}
		break;
	case 0x17:
		/* "<player>: Yes / No" */
		showNewgameSelection(0x1c, 0x18);
		return 0;
	case 0x18:
		/* "Jijimon: What's your Digimon's name?" */
		showNewgameDialogue(0x1d, 0x1a);
		return 0;
	case 0x19:
		SELECTION_MENU_STATE = 0x13;
		break;
	case 0x1a:
		NAMING_FLAGS |= NAMING_PARTNER;
		NAMING_BUFFER[0] = 0;
		SELECTION_MENU_STATE = 0x13;
		break;
	case 0x1b:
		/* "<player>: Yes / No" */
		showNewgameSelection(0x1c, 0x1c);
		return 0;
	case 0x1c:
		/* "Jijimon: Good Name." */
		showNewgameDialogue(0x1f, 0x1e);
		return 0;
	case 0x1d:
		SELECTION_MENU_STATE = 0x13;
		break;
	case 0x1e:
		/* "Jijimon: That's all the questions / I have. See ya later!" */
		showNewgameDialogue(0x20, 0x1f);
		return 0;
	case 0x1f:
		closeBox(0);
		unsetTrigger(TRIGGER_49);
		return 1;
	}

	return 0;
}
