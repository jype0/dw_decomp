#include <string.h>

#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/item.h>
#include <dw/line.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/pstat.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/trigger.h>
#include <dw/ui.h>
#include <dw/version.h>

typedef struct {
	uint8_t data[8];
} SelectionBoxUVData;

typedef struct {
	int8_t data[8];
} SelectionBoxOffsetData;

typedef struct {
	char s[12];
} BoxLabel;

extern GsOT *ACTIVE_ORDERING_TABLE;
extern uint8_t ACTIVE_BGM_TRACK;
extern uint8_t NAMING_CURRENT_LETTER;
extern uint8_t MAIN_D_80134F8F;
extern uint8_t MAIN_D_80134F90;
extern int32_t CARD_PRICES[];
extern uint8_t SHOP_AMOUNT;
extern uint32_t POLLED_INPUT;
extern int16_t ITEM_MENU_DESCRIPTION_RECTS[];
extern int16_t SELECTION_CURSOR_WIDTHS[];
extern uint8_t *ACTIVE_SCRIPT;

void renderItemMenuSprite(uint8_t boxId, int32_t idx, int16_t x, int16_t y);
void renderItemMenuScrollBar(ItemMenuBox *box);
void renderItemMenuItemList(ItemMenuBox *box, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int32_t flag);
void renderSelectionCursor(int32_t x, int32_t y, int16_t w, int16_t h, int32_t layer);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t x, int32_t y, int32_t w, int32_t h);
void *allocateArray(uint32_t size);
void freeArray(uint32_t *array);
uint8_t *padWithSpaces(uint8_t *str, int32_t width, int32_t used);
void terminateString(uint8_t *str, int32_t flag);
void itemMenuCursorMoveToTop(ItemMenuBox *box);
int32_t isXPressedAfterDialogue(void);
int32_t isKeyDown(int32_t mask);
int32_t createItemMenuAmountBox(RECT *origin);
int32_t createSingleCardShopMenu(RECT *origin);
ItemMenuBox *getItemMenuFromType(void);
int32_t createItemMenuDescriptionBox(ItemMenuBox *box, RECT *origin, uint8_t uiBoxId);
void itemMenuCursorTop(ItemMenuBox *box, int32_t startRow, int32_t style);
void itemMenuCursorBottom(ItemMenuBox *box, int32_t startRow, int32_t style);
void itemMenuCursorMoveToBottom(ItemMenuBox *box);
int32_t isItemMenuBoxBusy(ItemMenuBox *box);
void itemMenuCursorUp(ItemMenuBox *box, int32_t style);
void itemMenuCursorDown(ItemMenuBox *box, int32_t style);
static inline int32_t getActiveBGMFont(void);
static inline int32_t getActiveBGMVariant(void);
#if !VERSION_IS(US)
void renderKeeperBox(ItemMenuBox *box);
#else
void renderKeeperBox(ItemMenuBox *box, int8_t flag);
#endif
void tickMojyaTradeMenu(void);
void renderMojyaTradeMenu(void);
void tickCardMenu(void);
void renderCardMenu(void);
void tickBirdraTransportMenu(void);
void renderBirdraTransportMenu(void);
void updateNamingPreview(void);
void keeperScrollLeftToItem(uint8_t itemId);
void keeperScrollRightToItem(uint8_t itemId);
void tickJukeboxMenu(void);
void renderJukeboxMenu(void);
void tickNamingBox(void);
void renderNamingBox(void);
void fillNamingMenuStrings(void);
void tickItemKeeperWindow(void);
void renderKeeperBoxLeft(void);
void updateKeeperTextbox(int32_t boxIndex);
void renderKeeperBoxRight(void);
int32_t keeperMoveOne(void);
int32_t keeperMoveTen(void);
int32_t keeperMoveStack(void);
void updateMojyaTradeStrings(void);
void namingDeleteLast(void);
void terminateNamingBuffer(void);
void namingSelectionLeft(int16_t col, int16_t row, int16_t specialIdx);
void namingSelectionRight(int16_t col, int16_t row, int16_t specialIdx);
int32_t createMeritCardTradeDialogue(void);
void namingSelectionUp(int16_t column, int16_t row);
void namingSelectionDown(int16_t column, int16_t row);
void renderNamingUnderscore(uint8_t boxId, int16_t x, int16_t y, int16_t w);
void renderSelectionBox(void);
void renderKeeperTableBox(uint8_t boxId, int16_t x, int16_t y, int16_t w, int16_t h);
void getVRAMModeCoords(int32_t mode, int32_t *outX, int32_t *outClut);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void GsSortBoxFill(GsBOXF *bp, GsOT *otp, u_short pri);
void renderHorizontalLine(uint8_t boxId, int16_t x, int16_t y, int16_t w);
void drawString(char *str, int32_t x, int32_t y);
int32_t flipTextboxPage(uint8_t boxId);
void createTextbox(int32_t boxId, uint8_t flags, RECT *rect, RECT *origin, void *tick, void *render);
void showMapHeadTextbox(int32_t idx, uint8_t owner, uint8_t boxId, int32_t section);
uint8_t *getScriptSection(uint8_t *script, int32_t section);

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
	renderNamingUnderscore,
	renderNameDisplayBox,
	renderSelectionBox,
	namingSelectionDown,
	namingSelectionUp,
	namingSelectionRight,
	namingSelectionLeft,
	terminateNamingBuffer,
	namingDeleteLast,
	updateNamingPreview,
	fillNamingMenuStrings,
	renderNamingBox,
	tickNamingBox,
	setupNameDisplayBox,
	setupNameSelectorBox,
	showNewGameSelection,
	showNewGameDialogue,
	setupNewGameDialogueBox,
	updateMojyaTradeStrings,
	renderMojyaTradeMenu,
	tickMojyaTradeMenu,
	renderBirdraTransportMenu,
	tickBirdraTransportMenu,
	renderJukeboxMenu,
	tickJukeboxMenu,
	renderKeeperBox,
	keeperScrollRightToItem,
	keeperScrollLeftToItem,
	keeperMoveStack,
	keeperMoveTen,
	keeperMoveOne,
	renderKeeperBoxRight,
	updateKeeperTextbox,
	renderKeeperBoxLeft,
	tickItemKeeperWindow,
	createMeritCardTradeDialogue,
	renderCardMenu,
	tickCardMenu,
	setMojyaItemTradedTrigger,
	createMojyaTradeMenu,
	mojyaTradeFillItemList,
	createBirdraTransportMenu,
	birdraFillTargetList,
	createJukeboxMenu,
	jukeboxFillTitleList,
	createItemKeepWindow,
	itemKeeperFillItemList,
	shopFillMeritItemList,
	shopFillSellCardList,
	createCardMenu,
	hasAnyCardToBuy,
	showCardTextbox,
	rollCard,
	fillRecycleShopItemList,
};

// clang-format off
uint8_t MAIN_D_801303B8[128] = {
	0x01, 0x00, 0x01, 0x01, 0x02, 0x00, 0x02, 0x01,
	0x03, 0x00, 0x03, 0x01, 0x04, 0x00, 0x04, 0x01,
	0x05, 0x00, 0x06, 0x00, 0x06, 0x01, 0x07, 0x00,
	0x08, 0x00, 0x09, 0x00, 0x0a, 0x00, 0x0a, 0x01,
	0x0b, 0x00, 0x0b, 0x01, 0x0b, 0x02, 0x0c, 0x00,
	0x0c, 0x01, 0x0d, 0x00, 0x0d, 0x01, 0x0e, 0x00,
	0x0e, 0x01, 0x0f, 0x00, 0x0f, 0x01, 0x0f, 0x02,
	0x10, 0x00, 0x11, 0x00, 0x12, 0x00, 0x13, 0x00,
	0x13, 0x01, 0x14, 0x00, 0x14, 0x01, 0x15, 0x00,
	0x15, 0x01, 0x16, 0x00, 0x16, 0x01, 0x17, 0x00,
	0x18, 0x00, 0x18, 0x01, 0x19, 0x00, 0x19, 0x01,
	0x1a, 0x00, 0x1b, 0x00, 0x1b, 0x01, 0x1c, 0x00,
	0x1c, 0x01, 0x1d, 0x00, 0x1d, 0x01, 0x1d, 0x02,
	0x1e, 0x00, 0x1e, 0x01, 0x1e, 0x02, 0x1e, 0x03,
	0x1e, 0x04, 0x1f, 0x02, 0x20, 0x02, 0x21, 0x00,
	0x21, 0x01, 0x21, 0x02, 0x21, 0x03, 0x00, 0x00,
};

uint8_t MOJYAMON_ITEMS_GIVE[12] = {
	0x2c, 0x29, 0x45, 0x27, 0x41, 0x11, 0x09, 0x01,
	0x3e, 0x00, 0x00, 0x00,
};

uint8_t MOJYAMON_ITEMS_GET[12] = {
	0x01, 0x09, 0x61, 0x16, 0x0b, 0x0e, 0x13, 0x14,
	0x15, 0x00, 0x00, 0x00,
};

#if VERSION_IS(EU)
char STR_YOUR_NAME[] = "Ｙｏｕｒ　ｎａｍｅ";

char STR_DIGIMON_NAME[] = "Ｄｉｇｉｍｏｎ’ｓ　ｎａｍｅ";

char STR_NAME_ENTRY[] = "Ｎａｍｅ　　";

char STR_HIRAGANA_KATAKANA_ALPHANUMERIC_BACK_DONE[] = "ＢａｃｋＯＫ　　";
#elif !VERSION_IS(US)
char STR_YOUR_NAME[] = "おぬしの名前";

char STR_DIGIMON_NAME[] = "デジモンの名前";

char STR_NAME_ENTRY[] = "名前入力";

char STR_HIRAGANA_KATAKANA_ALPHANUMERIC_BACK_DONE[] = "かなカナ英数戻る終了";
#endif

char MAIN_D_80130450[] = "あいうえお";

#if !VERSION_IS(US)
char MAIN_D_8013045C[] = "かきくけこ";
#else
char MAIN_D_8013045C[] = "かきくけと";
#endif

char MAIN_D_80130468[] = "さしすせそ";

char MAIN_D_80130474[] = "たちつてと";

char MAIN_D_80130480[] = "なにぬねの";

char MAIN_D_8013048C[] = "はひふへほ";

char MAIN_D_80130498[] = "まみむめも";

char MAIN_D_801304A4[] = "や　ゆ　よ";

char MAIN_D_801304B0[] = "らりるれろ";

char *CHAR_PAGE3_LEFT[9] = {
	MAIN_D_80130450,
	MAIN_D_8013045C,
	MAIN_D_80130468,
	MAIN_D_80130474,
	MAIN_D_80130480,
	MAIN_D_8013048C,
	MAIN_D_80130498,
	MAIN_D_801304A4,
	MAIN_D_801304B0,
};

char MAIN_D_801304E0[] = "わ　を　ん";

char MAIN_D_801304EC[] = "がぎぐげご";

char MAIN_D_801304F8[] = "ざじずぜぞ";

char MAIN_D_80130504[] = "だぢづでど";

char MAIN_D_80130510[] = "ばびぶべぼ";

char MAIN_D_8013051C[] = "ぱぴぷぺぽ";

char MAIN_D_80130528[] = "ぁぃぅぇぉ";

char MAIN_D_80130534[] = "っゃゅょー";

char MAIN_D_80130540[] = "　　　　　";

char *CHAR_PAGE3_RIGHT[9] = {
	MAIN_D_801304E0,
	MAIN_D_801304EC,
	MAIN_D_801304F8,
	MAIN_D_80130504,
	MAIN_D_80130510,
	MAIN_D_8013051C,
	MAIN_D_80130528,
	MAIN_D_80130534,
	MAIN_D_80130540,
};

char MAIN_D_80130570[] = "アイウエオ";

char MAIN_D_8013057C[] = "カキクケコ";

char MAIN_D_80130588[] = "サシスセソ";

char MAIN_D_80130594[] = "タチツテト";

char MAIN_D_801305A0[] = "ナニヌネノ";

char MAIN_D_801305AC[] = "ハヒフヘホ";

char MAIN_D_801305B8[] = "マミムメモ";

char MAIN_D_801305C4[] = "ヤ　ユ　ヨ";

char MAIN_D_801305D0[] = "ラリルレロ";

char *CHAR_PAGE2_LEFT[9] = {
	MAIN_D_80130570,
	MAIN_D_8013057C,
	MAIN_D_80130588,
	MAIN_D_80130594,
	MAIN_D_801305A0,
	MAIN_D_801305AC,
	MAIN_D_801305B8,
	MAIN_D_801305C4,
	MAIN_D_801305D0,
};

char MAIN_D_80130600[] = "ワ　ヲ　ン";

char MAIN_D_8013060C[] = "ガギグゲゴ";

char MAIN_D_80130618[] = "ザジズゼゾ";

char MAIN_D_80130624[] = "ダヂヅデド";

char MAIN_D_80130630[] = "バビブベボ";

char MAIN_D_8013063C[] = "パピプペポ";

char MAIN_D_80130648[] = "ァィゥェォ";

char MAIN_D_80130654[] = "ッャュョー";

char *CHAR_PAGE2_RIGHT[9] = {
	MAIN_D_80130600,
	MAIN_D_8013060C,
	MAIN_D_80130618,
	MAIN_D_80130624,
	MAIN_D_80130630,
	MAIN_D_8013063C,
	MAIN_D_80130648,
	MAIN_D_80130654,
	MAIN_D_80130540,
};

char MAIN_D_80130684[] = "ＡＢＣＤＥ";

char MAIN_D_80130690[] = "ＦＧＨＩＪ";

char MAIN_D_8013069C[] = "ＫＬＭＮＯ";

char MAIN_D_801306A8[] = "ＰＱＲＳＴ";

char MAIN_D_801306B4[] = "ＵＶＷＸＹ";

char MAIN_D_801306C0[] = "Ｚ　　　　";

char *CHAR_PAGE1_LEFT[9] = {
	MAIN_D_80130684,
	MAIN_D_80130690,
	MAIN_D_8013069C,
	MAIN_D_801306A8,
	MAIN_D_801306B4,
	MAIN_D_801306C0,
	MAIN_D_80130540,
	MAIN_D_80130540,
	MAIN_D_80130540,
};

#if VERSION_IS(US)
char MAIN_D_801306F0[] = "ａｂｃｄｅ";

char MAIN_D_801306FC[] = "ｆｇｈｉｊ";

char MAIN_D_80130708[] = "ｋｌｍｎｏ";

char MAIN_D_80130714[] = "ｐｑｒｓｔ";

char MAIN_D_80130720[] = "ｕｖｗｘｙ";

char MAIN_D_8013072C[] = "ｚ　　　　";
#endif

char MAIN_D_80130738[] = "０１２３４";

char MAIN_D_80130744[] = "５６７８９";

#if !VERSION_IS(US)
char *CHAR_PAGE1_RIGHT[9] = {
	MAIN_D_80130738,
	MAIN_D_80130744,
	MAIN_D_80130540,
	MAIN_D_80130540,
	MAIN_D_80130540,
	MAIN_D_80130540,
	MAIN_D_80130540,
	MAIN_D_80130540,
	MAIN_D_80130540,
};

char **NAMING_CHAR_PAGES[6] = {
	CHAR_PAGE3_LEFT,
	CHAR_PAGE3_RIGHT,
	CHAR_PAGE2_LEFT,
	CHAR_PAGE2_RIGHT,
	CHAR_PAGE1_LEFT,
	CHAR_PAGE1_RIGHT,
};

int16_t NAMING_CTRL_BOXES[18] = {
	0x000e, 0x0006, 0x0030, 0x001a, 0x0022, 0x0018, 0x001a, 0x0030,
	0x0018, 0x001a, 0x003e, 0x0018, 0x001a, 0x005a, 0x0018, 0x001a,
	0x0076, 0x0018,
};

uint16_t NAMING_ROLLOVER_CHARS[10] = {
	0x8000, 0x8000, 0x8000, 0x8001, 0x8002, 0x8003, 0x8003, 0x8004,
	0x8004, 0x0000,
};

uint16_t NAMING_ROLLOVER_CTRL[6] = {
	0x000a, 0x000f, 0x0014, 0x001e, 0x0028, 0x0000,
};
#else
char *CHAR_PAGE1_RIGHT[9] = {
	MAIN_D_801306F0,
	MAIN_D_801306FC,
	MAIN_D_80130708,
	MAIN_D_80130714,
	MAIN_D_80130720,
	MAIN_D_8013072C,
	MAIN_D_80130738,
	MAIN_D_80130744,
	MAIN_D_80130540,
};

char **NAMING_CHAR_PAGES[6] = {
	CHAR_PAGE1_LEFT,
	CHAR_PAGE1_RIGHT,
	CHAR_PAGE2_LEFT,
	CHAR_PAGE2_RIGHT,
	CHAR_PAGE3_LEFT,
	CHAR_PAGE3_RIGHT,
};

int16_t NAMING_CTRL_BOXES[10] = {
	0x000e, 0x0006, 0x0030, 0x000e, 0x005a, 0x0030, 0x000e, 0x0076,
	0x0018, 0x0000,
};

uint16_t NAMING_ROLLOVER_CHARS[10] = {
	0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0x0000,
};

BoxLabel MAIN_D_801307B4 = { "In hand" };

BoxLabel MAIN_D_801307C0 = { "Keeping" };

char MAIN_D_801307CC[20] = "You have Will trade";
#endif

#if VERSION_IS(US)
char MAIN_D_801345F4[4] = "";

char MAIN_D_801345F8[] = "Name";

char MAIN_D_80134600[8] = "BackOK";

uint16_t NAMING_ROLLOVER_CTRL[2] = {
	0x0019, 0x0019,
};
#endif

SelectionBoxUVData NAMING_U02 = { {
	0x00, 0x04, 0x00, 0x04, 0x04, 0x04, 0x08, 0x08,
} };

SelectionBoxUVData NAMING_U13 = { {
	0x04, 0x00, 0x04, 0x00, 0x08, 0x08, 0x0c, 0x0c,
} };

SelectionBoxUVData NAMING_V01 = { {
	0xfb, 0xfb, 0xff, 0xff, 0xfb, 0xfb, 0xfb, 0xfb,
} };

SelectionBoxUVData NAMING_V23 = { {
	0xff, 0xff, 0xfb, 0xfb, 0xff, 0xff, 0xff, 0xff,
} };

SelectionBoxOffsetData NAMING_CTRL_X = { {
	0x00, 0x10, 0x00, 0x10, 0x04, 0x04, 0x00, 0x11,
} };

SelectionBoxOffsetData NAMING_Y = { {
	0x00, 0x00, 0x0e, 0x0e, 0x00, 0x0e, 0x00, 0x00,
} };

SelectionBoxOffsetData NAMING_CTRL_WIDTH = { {
	0x04, 0x04, 0x04, 0x04, 0x10, 0x10, 0x04, 0x04,
} };

SelectionBoxOffsetData NAMING_HEIGHT = { {
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0e, 0x0e,
} };

SelectionBoxOffsetData NAMING_CHAR_X = { {
#if !VERSION_IS(US)
	0x00, 0x1b, 0x00, 0x1b, 0x04, 0x04, 0x00, 0x1c,
#else
	0x00, 0x2d, 0x00, 0x2d, 0x04, 0x04, 0x00, 0x2e,
#endif
} };

SelectionBoxOffsetData NAMING_CHAR_WIDTH = { {
#if !VERSION_IS(US)
	0x04, 0x04, 0x04, 0x04, 0x1b, 0x1b, 0x04, 0x04,
#else
	0x04, 0x04, 0x04, 0x04, 0x2e, 0x2e, 0x04, 0x04,
#endif
} };
// clang-format on

#if !VERSION_IS(US)
#define NAMING_DELETE 3
#define NAMING_OK 4
#define NAMING_LABELS 5
#define NAMING_PROMPT_TAMER STR_YOUR_NAME
#define NAMING_PROMPT_DIGIMON STR_DIGIMON_NAME
#define NAMING_TITLE STR_NAME_ENTRY
#define NAMING_BUTTONS STR_HIRAGANA_KATAKANA_ALPHANUMERIC_BACK_DONE
#else
#define NAMING_DELETE 0
#define NAMING_OK 1
#define NAMING_LABELS 3
#define NAMING_PROMPT_TAMER MAIN_D_801345F4
#define NAMING_PROMPT_DIGIMON (MAIN_D_801345F4 + 1)
#define NAMING_TITLE MAIN_D_801345F8
#define NAMING_BUTTONS MAIN_D_80134600
#endif

void openRecycleShop(void)
{
	uint8_t owner = readPStat(PSTAT_254);
	int32_t hasItems;

	switch (SCRIPT_STATE_2) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0x9c, 6, 0xd2,
		                    0x18, 6, 0x5a);
		hasItems = fillRecycleShopItemList();
		SHOP_ACTION_SELECTED = 0;
		MAIN_D_80134F74 = 0;

		if (hasItems != 0) {
			showShopkeeperTextbox(0, owner, 0);
			SCRIPT_STATE_2 = 1;
			SCRIPT_NEXT_STATE_2 = 3;
			SCRIPT_TEXTBOX_MODE = 1;
		} else {
			showShopkeeperTextbox(1, owner, 0);
			SCRIPT_STATE_2 = 1;
			SCRIPT_NEXT_STATE_2 = 2;
			SCRIPT_TEXTBOX_MODE = 1;
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
		showShopkeepSelection(3, 0xfd, 2, &SHOP_ACTION_SELECTED);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 4;
		SCRIPT_TEXTBOX_MODE = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 2;
		createItemMenu();
		showShopkeeperTextbox(8, owner, 0);
		SCRIPT_STATE_2 = 1;
		SCRIPT_TEXTBOX_MODE = 3;
		break;
	case 5:
	case 6:
		triggerBoxCloseFlag(2);
		if (MAIN_D_80134F74 != 0) {
			showShopkeeperTextbox(4, owner, 0);
		} else {
			showShopkeeperTextbox(5, owner, 0);
		}
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 2;
		SCRIPT_TEXTBOX_MODE = 1;
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
			SCRIPT_NEXT_STATE_2 = 3;
		} else {
			SCRIPT_NEXT_STATE_2 = 9;
		}
		SCRIPT_STATE_2 = 1;
		SCRIPT_TEXTBOX_MODE = 1;
		break;
	case 8:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		showShopkeeperTextbox(0xb, owner, 0);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 3;
		SCRIPT_TEXTBOX_MODE = 1;
		break;
	case 9:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		showShopkeeperTextbox(0xc, owner, 0);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 6;
		SCRIPT_TEXTBOX_MODE = 1;
		break;
	}
}

void rollCardPack(void)
{
	switch (SCRIPT_STATE_2) {
	case 0:
		writePStat(PSTAT_249, rollCard());
		showCardTextbox();
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 2;
		SCRIPT_TEXTBOX_MODE = 1;
		break;
	case 1:
		break;
	case 2:
		writePStat(PSTAT_249, rollCard());
		showCardTextbox();
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 3;
		SCRIPT_TEXTBOX_MODE = 1;
		break;
	case 3:
		writePStat(PSTAT_249, rollCard());
		showCardTextbox();
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 4;
		SCRIPT_TEXTBOX_MODE = 1;
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

	switch (SCRIPT_STATE_2) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0xc, 6, 0xb2, 0x18, 6,
		                    0x5a);
		hasItems = hasAnyCardToBuy();
		if (hasItems != 0) {
			SCRIPT_STATE_2 = 3;
			SCRIPT_TEXTBOX_MODE = 0;
		} else {
			showMapHeadTextbox(1, owner, 0, 0x4d3);
			SCRIPT_STATE_2 = 1;
			SCRIPT_NEXT_STATE_2 = 2;
			SCRIPT_TEXTBOX_MODE = 1;
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
		SCRIPT_STATE_2 = 1;
		SCRIPT_TEXTBOX_MODE = 3;
		break;
	case 4:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		SCRIPT_STATE_2 = 2;
		break;
	}
}

void openSellCardMenu(void)
{
	uint8_t owner = readPStat(PSTAT_254);
	int32_t hasItems;

	switch (SCRIPT_STATE_2) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0x84, 6, 0xd2, 0x18,
		                    6, 0x5a);
		hasItems = shopFillSellCardList();
		SCRIPT_STATE_2 = 3;
		SCRIPT_TEXTBOX_MODE = 0;
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
		SCRIPT_STATE_2 = 1;
		SCRIPT_TEXTBOX_MODE = 3;
		break;
	case 4:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		SCRIPT_STATE_2 = 2;
		break;
	}
}

void openMeritShop(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_254);

	switch (SCRIPT_STATE_2) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0x84, 6, 0x9a, 0x18,
		                    6, 0x5a);
		allocateItemMenuBox(&ITEM_MENU_RIGHT, 0x100, 6, 0xb2, 0x18,
		                    6, 0x5a);
		showMapHeadTextbox(0, owner, 0, 0x4d4);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 3;
		SCRIPT_TEXTBOX_MODE = 1;
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
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 4;
		SCRIPT_TEXTBOX_MODE = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 6;
		shopFillSellCardList();
		createCardMenu();
		showMapHeadTextbox(5, owner, 0, 0x4d4);
		SCRIPT_STATE_2 = 1;
		SCRIPT_TEXTBOX_MODE = 3;
		break;
	case 5:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 7;
		shopFillMeritItemList();
		createItemMenu();
		showMapHeadTextbox(6, owner, 0, 0x4d4);
		SCRIPT_STATE_2 = 1;
		SCRIPT_TEXTBOX_MODE = 3;
		break;
	case 6:
		triggerBoxCloseFlag(2);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 2;
		SCRIPT_TEXTBOX_MODE = 1;
		break;
	case 7:
		setInputRepeatMask(0);
		showMapheadSelection(2, 0xfd, 2, &selection, 0x4d4);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 8;
		SCRIPT_TEXTBOX_MODE = 2;
		break;
	case 8:
		triggerBoxCloseFlag(1);
		MERIT += SHOP_VARIABLE;
		if (MERIT > 9999) {
			MERIT = 9999;
		}
		UPDATE_SHOP_BIT_BOX = 1;
		owner = getCardAmount(SHOP_ITEM_TYPE);
		owner -= 1;
		setCardAmount(SHOP_ITEM_TYPE, owner);
		SCRIPT_STATE_2 = 3;
		SCRIPT_TEXTBOX_MODE = 0;
		playShopSoundOnlyInSavannah();
		return;
	case 10:
		setInputRepeatMask(0);
		/* fall through */
	case 9:
	case 0xd:
		triggerBoxCloseFlag(1);
		SCRIPT_STATE_2 = 3;
		SCRIPT_TEXTBOX_MODE = 0;
		break;
	case 0xb:
		setInputRepeatMask(0);
		showMapheadSelection(3, 0xfd, 2, &selection, 0x4d4);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 0xc;
		SCRIPT_TEXTBOX_MODE = 2;
		break;
	case 0xc:
		triggerBoxCloseFlag(1);
		MERIT -= SHOP_VARIABLE;
		UPDATE_SHOP_BIT_BOX = 1;
		giveItem(SHOP_ITEM_TYPE, 1);
		SCRIPT_STATE_2 = 3;
		SCRIPT_TEXTBOX_MODE = 0;
		playShopSoundOnlyInSavannah();
		break;
	}
}

void tickItemKeeper(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_254);

	switch (SCRIPT_STATE_2) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0x100, 5, 0x88, 0x28,
		                    6, 0x4a);
		allocateItemMenuBox(&ITEM_MENU_RIGHT,
		                    INVENTORY.size << 1, 5, 0x88, 0x28,
		                    6, 0x4a);
		showMapHeadTextbox(0, owner, 0, 0x4d5);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 3;
		SCRIPT_TEXTBOX_MODE = 1;
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
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 4;
		SCRIPT_TEXTBOX_MODE = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000 | CONFIRM_BUTTON | ALT_BUTTON);
		ITEM_MENU_TYPE = 5;
		itemKeeperFillItemList();
		createItemKeepWindow();
		showMapHeadTextbox(3, owner, 0, 0x4d5);
		SCRIPT_STATE_2 = 1;
		SCRIPT_TEXTBOX_MODE = 3;
		break;
	case 6:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		triggerBoxCloseFlag(2);
		SCRIPT_STATE_2 = 5;
		break;
	case 5:
		showMapHeadTextbox(2, owner, 0, 0x4d5);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 2;
		SCRIPT_TEXTBOX_MODE = 1;
		break;
	}
}

void openJukebox(void)
{
	uint8_t owner = readPStat(PSTAT_254);

	switch (SCRIPT_STATE_2) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0x7e, 6, 0xd2, 0x18,
		                    6, 0x5a);
		jukeboxFillTitleList();
		showMapHeadTextbox(3, owner, 0, 0x4d6);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 3;
		SCRIPT_TEXTBOX_MODE = 1;
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&ITEM_MENU_LEFT);
		playBGM(ACTIVE_BGM_FONT);
		readMapTFS(CURRENT_SCREEN_ID);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0x5000);
		createJukeboxMenu();
		showMapHeadTextbox(0, owner, 0, 0x4d6);
		SCRIPT_STATE_2 = 1;
		SCRIPT_TEXTBOX_MODE = 3;
		break;
	case 4:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		showMapHeadTextbox(1, owner, 0, 0x4d6);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 2;
		SCRIPT_TEXTBOX_MODE = 1;
		break;
	}
}

void tickBirdraTransport(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_254);

	switch (SCRIPT_STATE_2) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0xc, 6, 0xd2, 0x18, 6,
		                    0x5a);
		birdraFillTargetList();
		showMapHeadTextbox(4, owner, 0, 0x4d6);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 3;
		SCRIPT_TEXTBOX_MODE = 1;
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
		SCRIPT_STATE_2 = 1;
		break;
	case 4:
		setInputRepeatMask(0);
		showMapheadSelection(7, 0xfd, 2, &selection, 0x4d6);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 5;
		SCRIPT_TEXTBOX_MODE = 2;
		break;
	case 5: {
		uint8_t idx;
		int32_t row;

		triggerBoxCloseFlag(2);
		triggerBoxCloseFlag(1);

		row = (ITEM_MENU_LEFT->topRow + ITEM_MENU_LEFT->cursor) * 2;
		idx = ITEM_MENU_LEFT->buf[row];
		idx &= 0x7f;
		writePStat(PSTAT_247, BIRDRA_TRANSPORT_TARGETS[idx].mapId);
		writePStat(PSTAT_248, BIRDRA_TRANSPORT_TARGETS[idx].unk_0x1);
		ACTIVE_SCRIPT = getScript(0);
		SCRIPT_POINTER = getScriptSection(ACTIVE_SCRIPT, 0x4e3);
		MONEY -= SHOP_VARIABLE;
		SCRIPT_STATE_2 = 2;
		break;
	}
	case 6:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(2);
		triggerBoxCloseFlag(1);
		showMapHeadTextbox(5, owner, 0, 0x4d6);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 2;
		SCRIPT_TEXTBOX_MODE = 1;
		break;
	}
}

void openMojyamonShop(void)
{
	int32_t selection = 0;
	uint8_t owner = readPStat(PSTAT_254);

	switch (SCRIPT_STATE_2) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 6, 3, 0, 0, 0, 0);
		allocateItemMenuBox(&ITEM_MENU_RIGHT, 6, 3, 0, 0, 0, 0);
		mojyaTradeFillItemList();
		writePStat(PSTAT_249, 255);
		showMapHeadTextbox(8, owner, 0, 0x4d6);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 3;
		SCRIPT_TEXTBOX_MODE = 1;
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
		SCRIPT_STATE_2 = 1;
		SCRIPT_TEXTBOX_MODE = 3;
		break;
	case 4:
		setInputRepeatMask(0);
		showMapheadSelection(0xc, 0xfd, 2, &selection, 0x4d6);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 5;
		SCRIPT_TEXTBOX_MODE = 2;
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

		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 2;
		SCRIPT_TEXTBOX_MODE = 1;
		playShopSoundOnlyInSavannah();
		break;
	}
	case 6:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		showMapHeadTextbox(10, owner, 0, 0x4d6);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 2;
		SCRIPT_TEXTBOX_MODE = 1;
		break;
	}
}

void initializeNamingBuffer(uint8_t flags)
{
	if ((flags & 2) == 0) {
		NAMING_BOX_FLAG = 0;
		SCRIPT_STATE_2 = 0;
		NAMING_BUFFER[0] = 0;
	} else {
		closeBox(0);
		NAMING_BOX_FLAG = flags;
		SCRIPT_STATE_2 = 0x14;

		if ((NAMING_BOX_FLAG & 1) == 0) {
			strcpy(NAMING_BUFFER, DIGIMON_NAME(0));
		} else {
			strcpy(NAMING_BUFFER, PARTNER_ENTITY.name);
		}
	}

	ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
	SCRIPT_TEXTBOX_MODE = 0;
}

int32_t newGameStateMachine(void)
{
	int32_t i;

	switch (SCRIPT_STATE_2) {
	case 0:
		setTrigger(TRIGGER_49);
#if !VERSION_IS(US)
		strcpy(DIGIMON_NAME(0), "？？？");
#else
		strcpy(DIGIMON_NAME(0), &MAIN_D_80134600[7]);
#endif
		setupNewGameDialogueBox();
		showNewGameDialogue(0x10, 2);
#if VERSION_IS(US)
		DRAW_STRING2_IS_FIXED_WIDTH = 0;
#endif
		break;
#if !VERSION_IS(US)
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
		SCRIPT_STATE_2 = 0x11;
		break;
	case 9:
		writePStat(PSTAT_254, 1);
		SCRIPT_STATE_2 = 0x11;
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
		SCRIPT_STATE_2 = 0x14;
		break;
	case 0x14:
		setInputRepeatMask(0xf000);
		setupNameSelectorBox();
		setupNameDisplayBox();
#if VERSION_IS(US)
		DRAW_STRING2_IS_FIXED_WIDTH = 1;
#endif
		SCRIPT_STATE_2 = 1;
		break;
	case 0x15:
		setInputRepeatMask(0);

		if ((NAMING_BOX_FLAG & 1) == 0) {
			strcpy(DIGIMON_NAME(0), NAMING_BUFFER);
		} else {
			strcpy(PARTNER_ENTITY.name, NAMING_BUFFER);
		}

		if ((NAMING_BOX_FLAG & 2) != 0) {
			triggerBoxCloseFlag(1);
			triggerBoxCloseFlag(2);
#if VERSION_IS(US)
			DRAW_STRING2_IS_FIXED_WIDTH = 0;
#endif
			SCRIPT_STATE_2 = 0x16;
		} else {
			triggerBoxCloseFlag(1);
			triggerBoxCloseFlag(2);
#if VERSION_IS(US)
			DRAW_STRING2_IS_FIXED_WIDTH = 0;
#endif
			SCRIPT_STATE_2 = 0x16;
		}
		break;
	case 0x16:
		if ((NAMING_BOX_FLAG & 2) != 0) {
			for (i = 0; i < 6; i++) {
				if (UI_BOX_DATA[1].state != 0) {
					return 0;
				}
			}

			return 1;
		}

		setupNewGameDialogueBox();

		if ((NAMING_BOX_FLAG & 1) == 0) {
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
		SCRIPT_STATE_2 = 0x13;
		break;
	case 0x1a:
		NAMING_BOX_FLAG |= 1;
		NAMING_BUFFER[0] = 0;
		SCRIPT_STATE_2 = 0x13;
		break;
	case 0x1b:
		showNewGameSelection(0x1c, 0x1c);
		break;
	case 0x1c:
		showNewGameDialogue(0x1f, 0x1e);
		break;
	case 0x1d:
		SCRIPT_STATE_2 = 0x13;
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

int32_t hasAnyCardToBuy(void)
{
	uint8_t *buf;
	uint8_t i;
	uint8_t id;
	int32_t any;
	uint8_t spriteId;

	any = 0;
	buf = ITEM_MENU_LEFT->buf;
	ITEM_MENU_LEFT->itemCount = 0;
	for (i = 0; i < 6; i++) {
		ITEM_MENU_LEFT->itemCount++;
		id = SCRIPT_STATE_PTR->smth[i];
		*buf++ = id;
		if (id != 0xff) {
			spriteId = CARD_DATA[id].spriteId;
			if ((CARD_PRICES[spriteId] <= MONEY) && ((uint32_t)getCardAmount(id) < 9)) {
				*buf++ = 1;
			} else {
				*buf++ = 0;
			}
			any = 1;
		} else {
			*buf++ = 0;
		}
	}

	return any;
}

void showCardTextbox(void)
{
	int32_t cardId;
	uint8_t n;
	uint8_t line;

	cardId = readPStat(PSTAT_249) & 0xff;
	n = getCardAmount(cardId);
	if (n == 0) {
		if (CARD_DATA[cardId].spriteId == 0) {
			line = 2;
		} else if (CARD_DATA[cardId].spriteId == 1) {
			line = 3;
		} else {
			line = 4;
		}
	} else if (n < 6) {
		line = 5;
	} else {
		line = 6;
	}

	if (n < 9) {
		n++;
		setCardAmount(cardId, n);
	}

	n = CARD_DATA[cardId].digimonId;
	writePStat(PSTAT_249, n);
	showMapHeadTextbox(line, 0xfd, 0, 0x4d3);
}

int32_t shopFillSellCardList(void)
{
	uint8_t *buf;
	int32_t any;
	uint8_t id;
	uint8_t amount;

	any = 0;
	buf = ITEM_MENU_LEFT->buf;
	ITEM_MENU_LEFT->itemCount = 0;
	for (id = 1; id < 0x41; id++) {
		ITEM_MENU_LEFT->itemCount++;
		amount = getCardAmount(id);
		if (amount != 0) {
			*buf++ = id;
			*buf++ = amount;
			any = 1;
		} else {
			*buf++ = 0xff;
			*buf++ = 0;
		}
	}

	return any;
}

int32_t fillRecycleShopItemList(void)
{
	uint8_t *buf;
	uint8_t i;
	uint8_t count;
	uint8_t itemId;

	buf = ITEM_MENU_LEFT->buf;
	ITEM_MENU_LEFT->itemCount = 0;
	for (i = 0; i < 0x4e; i++) {
		count = SCRIPT_STATE_PTR->smth[i + 6];
		if (count != 0) {
			ITEM_MENU_LEFT->itemCount++;
			itemId = RECYCLABLE_ITEMS[i];
			*buf++ = itemId;
			if (ITEM_PARA[itemId].value <= MONEY) {
				count |= 0x80;
			}
			*buf++ = count;
		}
	}

	if (ITEM_MENU_LEFT->itemCount != 0) {
		return 1;
	}

	return 0;
}

uint8_t rollCard(void)
{
	uint8_t *cards;
	uint8_t *p;
	uint8_t count;
	uint8_t i;
	uint8_t rarity;

	rarity = randomLimit(100);
	if (rarity == 0) {
		rarity = 0;
	} else if (rarity < 5) {
		rarity = 1;
	} else if (rarity < 0x14) {
		rarity = 2;
	} else if (rarity < 0x32) {
		rarity = 3;
	} else {
		rarity = 4;
	}

	cards = (uint8_t *)allocateArray(0x42);
	p = cards;
	count = 0;
	for (i = 0; i < 0x42; i++) {
		if (rarity == CARD_DATA[i].spriteId) {
			*p++ = i;
			count++;
		}
	}

	rarity = randomLimit(count);
	i = cards[rarity];
	freeArray((uint32_t *)cards);

	return i;
}

void createCardMenu(void)
{
	RECT rect;
	RECT origin;
	uint8_t flags;
	ItemMenuBox *result;
	uint8_t boxId;
	int16_t *src;

	flags = 0xf1;
	if (ITEM_MENU_TYPE == 4) {
		boxId = 0xfd;
	} else {
		boxId = readPStat(PSTAT_254);
	}

	setupBoxOrigin(boxId, &origin);
	result = getItemMenuFromType();
	src = &ITEM_MENU_POS[ITEM_MENU_TYPE * 4];
	setRECT(&rect, src[0], src[1], src[2], src[3]);
	createTextbox(1, flags, &rect, &origin, tickCardMenu, renderCardMenu);
	registerTextbox(1, 9, 6, 1, 0);
	initItemMenuBox(result, 1, 9);
	updateItemMenuStrings(result, 9, 1);
	SHOP_VARIABLE = 0;
}

int32_t shopFillMeritItemList(void)
{
	int32_t any;
	int32_t hasFreeSlot;
	uint8_t *out;
	uint8_t i;
	uint8_t item;
	uint8_t size;

	any = 0;
	hasFreeSlot = 0;
	size = INVENTORY.size;
	out = ITEM_MENU_RIGHT->buf;
	ITEM_MENU_RIGHT->itemCount = 0;

	for (i = 0; i < size; i++) {
		if (INVENTORY.types.array[i] == 0xff) {
			hasFreeSlot = 1;
			break;
		}
	}

	for (item = 0; item < 0x80; item++) {
		if (ITEM_PARA[item].meritValue == 0) {
			continue;
		}

		ITEM_MENU_RIGHT->itemCount++;
		*out++ = item;

		if (ITEM_PARA[item].meritValue > MERIT) {
			*out++ = 0;
			continue;
		}

		if (hasFreeSlot != 0) {
			*out++ = 1;
			any = 1;
			continue;
		}

		for (i = 0; i < size; i++) {
			if ((INVENTORY.types.array[i] == item) &&
			    (INVENTORY.amounts.array[i] != 0x63)) {
				*out++ = 1;
				any = 1;
				goto next;
			}
		}

		*out++ = 0;
next:;
	}

	return any;
}

void itemKeeperFillItemList(void)
{
	uint8_t i;
	int32_t hasFreeSlot;
	uint8_t *counts;
	uint8_t *out;
	uint8_t j;
	uint8_t size;
	uint8_t c;

	hasFreeSlot = 0;
	counts = &SCRIPT_STATE_PTR->smth[0x54];
	out = ITEM_MENU_LEFT->buf;
	ITEM_MENU_LEFT->itemCount = 0;
	size = INVENTORY.size;

	for (i = 0; i < size; i++) {
		if (INVENTORY.types.array[i] == 0xff) {
			hasFreeSlot = 1;
			break;
		}
	}

	for (j = 0; j < 0x80; j++) {
		c = *counts++;
		if (c == 0) {
			continue;
		}

		ITEM_MENU_LEFT->itemCount++;
		*out++ = j;

		if (hasFreeSlot != 0) {
			*out++ = c | 0x80;
			continue;
		}

		for (i = 0; i < size; i++) {
			if ((INVENTORY.types.array[i] == j) && (INVENTORY.amounts.array[i] != 0x63)) {
				*out++ = c | 0x80;
				goto next;
			}
		}

		*out++ = c;
next:;
	}

	out = ITEM_MENU_RIGHT->buf;
	ITEM_MENU_RIGHT->itemCount = 0;

	for (j = 0; j < size; j++) {
		ITEM_MENU_RIGHT->itemCount++;
		if (INVENTORY.types.array[j] != 0xff) {
			i = INVENTORY.types.array[j];
			c = INVENTORY.amounts.array[j];
			*out++ = i;
			if ((SCRIPT_STATE_PTR->smth + i)[0x54] != 0x63) {
				*out++ = c | 0x80;
			} else {
				*out++ = c;
			}
		} else {
			*out++ = 0xff;
			*out++ = 0;
		}
	}
}

void createItemKeepWindow(void)
{
	RECT rect;
	RECT origin;
	uint8_t flags;

	flags = 0xe1;
	setupBoxOrigin(readPStat(PSTAT_254), &origin);
	setRECT(&rect, -152, -98, 148, 127);
	createTextbox(1, flags, &rect, &origin, tickItemKeeperWindow, renderKeeperBoxLeft);
	registerTextbox(1, 9, 6, 1, 1);
	initItemMenuBox(ITEM_MENU_LEFT, 1, 0xa);
	updateKeeperTextbox(0);
	setupBoxOrigin(0xfd, &origin);
	setRECT(&rect, 0, -98, 148, 127);
	createTextbox(2, flags, &rect, &origin, 0, renderKeeperBoxRight);
	registerTextbox(2, 9, 6, 1, 2);
	initItemMenuBox(ITEM_MENU_RIGHT, 2, 0xa);
	updateKeeperTextbox(1);
}

static inline int32_t getActiveBGMFont(void)
{
	return ACTIVE_BGM_FONT;
}

static inline int32_t getActiveBGMVariant(void)
{
	return ACTIVE_BGM_TRACK;
}

void jukeboxFillTitleList(void)
{
	uint8_t *buf;
	uint8_t i;
	uint8_t track;
	int32_t unlocked;

	unlocked = isTriggerSet(TRIGGER_50);
	buf = ITEM_MENU_LEFT->buf;
	ITEM_MENU_LEFT->itemCount = 0;
	for (i = 0; i < 0x3f; i++) {
		if (unlocked != 0) {
			ITEM_MENU_LEFT->itemCount++;
			*buf++ = i;
			buf++;
		} else if (i != 0x3e && i != 0x3e) {
			ITEM_MENU_LEFT->itemCount++;
			*buf++ = i;
			buf++;
		}
	}

	for (i = 0; i < ITEM_MENU_LEFT->itemCount; i++) {
		track = ITEM_MENU_LEFT->buf[i * 2];
		if (MAIN_D_801303B8[track * 2] == getActiveBGMFont() && MAIN_D_801303B8[track * 2 + 1] == getActiveBGMVariant()) {
			writePStat(PSTAT_249, i);
			MAIN_D_80134F8F = i;
			return;
		}
	}

	writePStat(PSTAT_249, 0xff);
	MAIN_D_80134F8F = 0xff;
	stopBGM();
}

void createJukeboxMenu(void)
{
	RECT rect;
	RECT origin;
	int32_t i;
	int32_t scroll;
	uint8_t flags;
	uint8_t item;

	flags = 0xf1;
	setupBoxOrigin(readPStat(0xfe), &origin);
	setRECT(&rect, -0x47, -0x62, 0xde, 0x81);
	createTextbox(1, flags, &rect, &origin, tickJukeboxMenu, renderJukeboxMenu);
	registerTextbox(1, 9, 6, 1, 0);

	if (ITEM_MENU_LEFT->isOpen == 0) {
		ITEM_MENU_LEFT->isOpen = 1;
		ITEM_MENU_LEFT->boxId = 1;
		item = readPStat(0xf9);

		for (i = 0; i < ITEM_MENU_LEFT->itemCount; i++) {
			if (item == ITEM_MENU_LEFT->buf[i * 2]) {
				goto found;
			}
		}

		i = 0;

found:
		scroll = i - ITEM_MENU_LEFT->visibleRows;
		if (scroll < 0) {
			ITEM_MENU_LEFT->topRow = 0;
			ITEM_MENU_LEFT->cursor = i;
		} else {
			ITEM_MENU_LEFT->topRow = scroll + 1;
			ITEM_MENU_LEFT->cursor = ITEM_MENU_LEFT->visibleRows - 1;
		}

		ITEM_MENU_LEFT->prevTopRow = ITEM_MENU_LEFT->topRow;
		ITEM_MENU_LEFT->prevCursor = ITEM_MENU_LEFT->cursor;

		for (i = 0; i < ITEM_MENU_LEFT->visibleRows; i++) {
			ITEM_MENU_LEFT->itemRow[i] = i + 9;
		}
	}

	updateItemMenuStrings(ITEM_MENU_LEFT, 9, 2);
}

void birdraFillTargetList(void)
{
	uint8_t *buf;
	uint8_t i;

	buf = ITEM_MENU_LEFT->buf;
	ITEM_MENU_LEFT->itemCount = 0;
	for (i = 0; i < 6; i++) {
		if (isTriggerSet(BIRDRA_TRANSPORT_TARGETS[i].trigger) != 0) {
			ITEM_MENU_LEFT->itemCount++;
			if (BIRDRA_TRANSPORT_TARGETS[i].cost <= MONEY) {
				*buf++ = i | 0x80;
				buf++;
			} else {
				*buf++ = i;
				buf++;
			}
		}
	}
}

void createBirdraTransportMenu(void)
{
	RECT rect;
	RECT origin;
	uint8_t flags;

	flags = 0xf1;
	setupBoxOrigin(readPStat(PSTAT_254), &origin);
	setRECT(&rect, -71, -100, 222, 129);
	createTextbox(1, flags, &rect, &origin, tickBirdraTransportMenu, renderBirdraTransportMenu);
	registerTextbox(1, 9, 6, 1, 0);
	initItemMenuBox(ITEM_MENU_LEFT, 1, 9);
	updateItemMenuStrings(ITEM_MENU_LEFT, 9, 3);
	SHOP_VARIABLE = 0;
}

void mojyaTradeFillItemList(void)
{
	uint8_t *buf;
	uint8_t *buf2;
	uint8_t idx;
	uint8_t item;
	uint8_t i;

	ITEM_MENU_TYPE = readPStat(PSTAT_249) * 3;
	idx = ITEM_MENU_TYPE;
	buf = ITEM_MENU_LEFT->buf;
	ITEM_MENU_LEFT->itemCount = 3;
	buf2 = ITEM_MENU_RIGHT->buf;
	ITEM_MENU_RIGHT->itemCount = 3;
	for (i = 0; i < 3; i++, idx++) {
		item = MOJYAMON_ITEMS_GIVE[idx];
		if (getItemCount(item) != 0) {
			*buf++ = item | 0x80;
			buf++;
		} else {
			*buf++ = item;
			buf++;
		}
		*buf2++ = MOJYAMON_ITEMS_GET[idx];
		buf2++;
	}
}

void createMojyaTradeMenu(void)
{
	RECT rect;
	RECT origin;
	uint8_t flags;

	flags = 0xe1;
	setupBoxOrigin(readPStat(PSTAT_254), &origin);
	setRECT(&rect, -88, -80, 223, 83);
	createTextbox(1, flags, &rect, &origin, tickMojyaTradeMenu, renderMojyaTradeMenu);
	registerTextbox(1, 9, 4, 1, 0);
	initItemMenuBox(ITEM_MENU_LEFT, 1, 0xa);
	updateMojyaTradeStrings();
}

void setMojyaItemTradedTrigger(void)
{
	int32_t trigger;

	trigger = ITEM_MENU_TYPE + (ITEM_MENU_LEFT->topRow + ITEM_MENU_LEFT->cursor) + 5;
	setTrigger(trigger);
	for (trigger = 5; trigger < 0xe; trigger++) {
		if (isTriggerSet(trigger) == 0) {
			return;
		}
	}

	setTrigger(0xe);
}

void tickCardMenu(void)
{
	ItemMenuBox *box;
	RECT rect;
	int16_t *src;

	box = getItemMenuFromType();
	if (isItemMenuBoxBusy(box) != 0) {
		return;
	}

	if (UI_BOX_DATA[3].state != 0) {
		return;
	}

	if (UI_BOX_DATA[1].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (SHOP_VARIABLE != 0) {
		return;
	}

	if (isKeyDown(CONFIRM_BUTTON)) {
		src = &ITEM_MENU_DESCRIPTION_RECTS[ITEM_MENU_TYPE * 4];
		if (ITEM_MENU_TYPE == 3) {
			setRECT(&rect, src[0], src[1], src[2], src[3]);
			createSingleCardShopMenu(&rect);
		} else if (ITEM_MENU_TYPE == 6) {
			createMeritCardTradeDialogue();
		} else {
			setRECT(&rect, src[0], src[1], src[2], src[3]);
			createItemMenuAmountBox(&rect);
		}
	} else if (isKeyDown(CANCEL_BUTTON)) {
		if (ITEM_MENU_TYPE != 6) {
			SCRIPT_STATE_2 = 4;
		} else {
			SCRIPT_STATE_2 = 0xa;
		}
		playSound(0, 4);
	} else if (isKeyDown(0x1000)) {
		if (POLLED_INPUT & 8) {
			itemMenuCursorTop(box, 9, 1);
		} else {
			itemMenuCursorUp(box, 1);
		}
	} else if (isKeyDown(0x4000)) {
		if (POLLED_INPUT & 8) {
			itemMenuCursorBottom(box, 9, 1);
		} else {
			itemMenuCursorDown(box, 1);
		}
	}
}

void renderCardMenu(void)
{
	int16_t bx;
	int16_t by;
	ItemMenuBox *box;
	int16_t x;
	int16_t y;
	int16_t x2;
	int16_t y2;
	int16_t w;

	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;
	renderItemMenuSprite(1, 0, bx + 8, by + 5);
	if (ITEM_MENU_TYPE != 6) {
		renderItemMenuSprite(1, 1, bx + 0x80, by + 5);
		if (ITEM_MENU_TYPE == 4) {
			renderItemMenuSprite(1, 3, bx + 0xb6, by + 5);
		}
	} else {
		renderItemMenuSprite(1, 3, bx + 0x80, by + 5);
	}
	box = getItemMenuFromType();
	renderItemMenuScrollBar(box);
	y = by + box->cursor * 0x12 + 0x11;
draw:
	w = SELECTION_CURSOR_WIDTHS[ITEM_MENU_TYPE];
	renderSelectionCursor(bx + 5, y, w, 0x12, 5);
	x = bx + 0x1a;
	y = by + 0x13;
	x2 = bx + 8;
	y2 = by + 0x12;
	renderItemMenuItemList(box, x, y, x2, y2, 1);
}

int32_t createMeritCardTradeDialogue(void)
{
	ItemMenuBox *box;
	int32_t off;
	uint8_t kind;

	box = getItemMenuFromType();
	off = (box->topRow + box->cursor) * 2;
	SHOP_ITEM_TYPE = box->buf[off];

	if (SHOP_ITEM_TYPE != 0xff) {
		kind = box->buf[off + 1];
		if (kind != 0) {
			SHOP_VARIABLE = CARD_DATA[SHOP_ITEM_TYPE].meritValue;
			kind = CARD_DATA[SHOP_ITEM_TYPE].spriteId + 7;
			showMapHeadTextbox(kind, readPStat(0xfe), 0, 0x4d4);
			SCRIPT_STATE_2 = 1;
			SCRIPT_NEXT_STATE_2 = 7;
			SCRIPT_TEXTBOX_MODE = 1;
			playSound(0, 3);

			return 1;
		}
	}

	playSound(0, 0xb);

	return 0;
}

void tickItemKeeperWindow(void)
{
	ItemMenuBox *box;
	RECT rect;
	int32_t a;
	int32_t b;

	a = isItemMenuBoxBusy(ITEM_MENU_LEFT);
	b = isItemMenuBoxBusy(ITEM_MENU_RIGHT);
	if (a == 1) {
		return;
	}

	if (b == 1) {
		return;
	}

	if (UI_BOX_DATA[3].state != 0) {
		return;
	}

	if (UI_BOX_DATA[1].state != 1) {
		return;
	}

	if (UI_BOX_DATA[2].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (SCRIPT_STATE_2 != 1) {
		return;
	}

	if (MAIN_D_80134F90 == 0) {
		box = ITEM_MENU_LEFT;
	} else {
		box = ITEM_MENU_RIGHT;
	}

	if (isKeyDown(CANCEL_BUTTON)) {
		SCRIPT_STATE_2 = 6;
		playSound(0, 4);
	} else if (isKeyDown(0x8000)) {
		MAIN_D_80134F90 = 0;
		playSound(0, 2);
	} else if (isKeyDown(0x2000)) {
		MAIN_D_80134F90 = 1;
		playSound(0, 2);
	} else if (box->itemCount != 0) {
		if (isKeyDown(CONFIRM_BUTTON)) {
			keeperMoveOne();
			playSound(0, 3);
		} else if (isKeyDown(ALT_BUTTON)) {
			keeperMoveTen();
			playSound(0, 3);
		} else if (isKeyDown(0x80)) {
			keeperMoveStack();
			playSound(0, 3);
		} else if (isKeyDown(0x1000)) {
			if (POLLED_INPUT & 8) {
				itemMenuCursorMoveToTop(box);
				updateKeeperTextbox(MAIN_D_80134F90);
			} else {
				itemMenuCursorUp(box, 0);
			}
		} else if (isKeyDown(0x4000)) {
			if (POLLED_INPUT & 8) {
				itemMenuCursorMoveToBottom(box);
				updateKeeperTextbox(MAIN_D_80134F90);
			} else {
				itemMenuCursorDown(box, 0);
			}
		} else if (isKeyDown(0x800)) {
			setRECT(&rect, 5, 0x20, 0x80, 0x12);
			createItemMenuDescriptionBox(box, &rect,
			                             (uint8_t)(MAIN_D_80134F90 + 1));
			playSound(0, 3);
		}
	}
}

void renderKeeperBoxLeft(void)
{
#if !VERSION_IS(US)
	renderKeeperBox(ITEM_MENU_LEFT);
#else
	renderKeeperBox(ITEM_MENU_LEFT, 0);
#endif
}

void updateKeeperTextbox(int32_t boxIndex)
{
	if (boxIndex == 0) {
		showMapHeadTextbox(4, 0xff, 1, 0x4d5);
		TEXTBOX_DATA.box[1].writeCount--;
		TEXTBOX_LINES_PTR[0x246] = 0xd;
		TEXTBOX_LINES_PTR[0x3c6] = 0xd;
		updateItemMenuStrings(ITEM_MENU_LEFT, 0xa, 0);
	} else {
		showMapHeadTextbox(5, 0xff, 2, 0x4d5);
		TEXTBOX_DATA.box[2].writeCount--;
		TEXTBOX_LINES_PTR[0x266] = 0xd;
		TEXTBOX_LINES_PTR[0x3e6] = 0xd;
		updateItemMenuStrings(ITEM_MENU_RIGHT, 0xa, 0);
	}
}

void renderKeeperBoxRight(void)
{
#if !VERSION_IS(US)
	renderKeeperBox(ITEM_MENU_RIGHT);
#else
	renderKeeperBox(ITEM_MENU_RIGHT, 1);
#endif
}

int32_t keeperMoveOne(void)
{
	uint8_t item;
	int32_t off;
	uint8_t amount;

	if (MAIN_D_80134F90 == 0) {
		off = (ITEM_MENU_LEFT->topRow + ITEM_MENU_LEFT->cursor) * 2;
		item = ITEM_MENU_LEFT->buf[off];
		if (item == 0xff) {
			return 0;
		}

		amount = ITEM_MENU_LEFT->buf[off + 1];
		if ((amount & 0x80) == 0) {
			return 0;
		}

		if (getItemCount(item) >= 0x63) {
			return 0;
		}

		(SCRIPT_STATE_PTR->smth + item)[0x54] -= 1;
		giveItem(item, 1);
		itemKeeperFillItemList();
		keeperScrollLeftToItem(item);
		keeperScrollRightToItem(item);
	} else {
		off = (ITEM_MENU_RIGHT->topRow + ITEM_MENU_RIGHT->cursor) * 2;
		item = ITEM_MENU_RIGHT->buf[off];
		if (item == 0xff) {
			return 0;
		}

		amount = ITEM_MENU_RIGHT->buf[off + 1];
		if ((amount & 0x80) == 0) {
			return 0;
		}

		if (getItemCount(item) == 0) {
			return 0;
		}

		removeItem(item, 1);
		(SCRIPT_STATE_PTR->smth + item)[0x54] += 1;
		itemKeeperFillItemList();
		keeperScrollRightToItem(item);
		keeperScrollLeftToItem(item);
	}

	return 1;
}

int32_t keeperMoveTen(void)
{
	uint8_t item;
	uint8_t amount;
	int32_t off;
	uint8_t count;

	if (MAIN_D_80134F90 == 0) {
		off = (ITEM_MENU_LEFT->topRow + ITEM_MENU_LEFT->cursor) * 2;
		item = ITEM_MENU_LEFT->buf[off];
		if (item == 0xff) {
			return 0;
		}

		amount = ITEM_MENU_LEFT->buf[off + 1];
		if ((amount & 0x80) == 0) {
			return 0;
		}

		amount &= 0x7f;
		if (amount < 0xa) {
			return 0;
		}

		count = getItemCount(item);
		if (count >= 0x5a) {
			return 0;
		}

		(SCRIPT_STATE_PTR->smth + item)[0x54] -= 0xa;
		giveItem(item, 0xa);
		itemKeeperFillItemList();
		keeperScrollLeftToItem(item);
		keeperScrollRightToItem(item);
	} else {
		off = (ITEM_MENU_RIGHT->topRow + ITEM_MENU_RIGHT->cursor) * 2;
		item = ITEM_MENU_RIGHT->buf[off];
		if (item == 0xff) {
			return 0;
		}

		amount = ITEM_MENU_RIGHT->buf[off + 1];
		if ((amount & 0x80) == 0) {
			return 0;
		}

		amount &= 0x7f;
		if (amount < 0xa) {
			return 0;
		}

		count = (SCRIPT_STATE_PTR->smth + item)[0x54];
		if (count >= 0x5a) {
			return 0;
		}

		removeItem(item, 0xa);
		(SCRIPT_STATE_PTR->smth + item)[0x54] += 0xa;
		itemKeeperFillItemList();
		keeperScrollRightToItem(item);
		keeperScrollLeftToItem(item);
	}

	return 1;
}

int32_t keeperMoveStack(void)
{
	uint8_t amount;
	uint8_t item;
	int32_t off;
	uint8_t cap;

	if (MAIN_D_80134F90 == 0) {
		off = (ITEM_MENU_LEFT->topRow + ITEM_MENU_LEFT->cursor) * 2;
		item = ITEM_MENU_LEFT->buf[off];
		if (item == 0xff) {
			return 0;
		}

		amount = ITEM_MENU_LEFT->buf[off + 1];
		if ((amount & 0x80) == 0) {
			return 0;
		}

		amount &= 0x7f;
		cap = 0x63 - getItemCount(item);
		if (cap < amount) {
			amount = cap;
		}

		(SCRIPT_STATE_PTR->smth + item)[0x54] -= amount;
		giveItem(item, amount);
		itemKeeperFillItemList();
		keeperScrollLeftToItem(item);
		keeperScrollRightToItem(item);
	} else {
		off = (ITEM_MENU_RIGHT->topRow + ITEM_MENU_RIGHT->cursor) * 2;
		item = ITEM_MENU_RIGHT->buf[off];
		if (item == 0xff) {
			return 0;
		}

		amount = ITEM_MENU_RIGHT->buf[off + 1];
		if ((amount & 0x80) == 0) {
			return 0;
		}

		amount &= 0x7f;
		cap = 0x63 - (SCRIPT_STATE_PTR->smth + item)[0x54];
		if (cap < amount) {
			amount = cap;
		}

		removeItem(item, amount);
		(SCRIPT_STATE_PTR->smth + item)[0x54] += amount;
		itemKeeperFillItemList();
		keeperScrollRightToItem(item);
		keeperScrollLeftToItem(item);
	}

	return 1;
}

void keeperScrollLeftToItem(uint8_t itemId)
{
	int32_t i;
	int32_t top;
	int32_t d;
	int32_t visible;
	int32_t cursor;
	int32_t count;

	ITEM_MENU_LEFT->prevTopRow = ITEM_MENU_LEFT->topRow;
	ITEM_MENU_LEFT->prevCursor = ITEM_MENU_LEFT->cursor;
	visible = ITEM_MENU_LEFT->visibleRows;
	top = ITEM_MENU_LEFT->topRow;
	cursor = ITEM_MENU_LEFT->cursor;
	count = ITEM_MENU_LEFT->itemCount;

	for (i = 0; i < count * 2; i += 2) {
		if (itemId == ITEM_MENU_LEFT->buf[i]) {
			i >>= 1;
			goto found;
		}
	}

	if (count == 0) {
		ITEM_MENU_LEFT->topRow = 0;
		ITEM_MENU_LEFT->cursor = 0;
		goto end;
	}

	i = top + cursor;
	if (i >= count) {
		i = count - 1;
	}

	if (top + visible >= count && top != 0) {
		ITEM_MENU_LEFT->topRow--;
		goto end;
	}

found:
	if (i >= top && i < top + visible) {
		ITEM_MENU_LEFT->cursor = i - top;
		goto end;
	}

	d = i - visible;
	if (d < 0) {
		ITEM_MENU_LEFT->topRow = 0;
		ITEM_MENU_LEFT->cursor = i;
	} else {
		ITEM_MENU_LEFT->topRow = d + 1;
		ITEM_MENU_LEFT->cursor = visible - 1;
	}

end:
	updateKeeperTextbox(0);
}

void keeperScrollRightToItem(uint8_t item)
{
	int32_t i;
	int32_t scroll;

	ITEM_MENU_RIGHT->prevTopRow = ITEM_MENU_RIGHT->topRow;
	ITEM_MENU_RIGHT->prevCursor = ITEM_MENU_RIGHT->cursor;

	for (i = 0; i < ITEM_MENU_RIGHT->itemCount * 2; i += 2) {
		if (item == ITEM_MENU_RIGHT->buf[i]) {
			i >>= 1;
			if (i >= ITEM_MENU_RIGHT->topRow && i < ITEM_MENU_RIGHT->topRow + ITEM_MENU_RIGHT->visibleRows) {
				ITEM_MENU_RIGHT->cursor = i - ITEM_MENU_RIGHT->topRow;
				break;
			}

			scroll = i - ITEM_MENU_RIGHT->visibleRows;
			if (scroll < 0) {
				ITEM_MENU_RIGHT->topRow = 0;
				ITEM_MENU_RIGHT->cursor = i;
			} else {
				ITEM_MENU_RIGHT->topRow = scroll + 1;
				ITEM_MENU_RIGHT->cursor = ITEM_MENU_RIGHT->visibleRows - 1;
			}
			break;
		}
	}

	updateKeeperTextbox(1);
}

#if !VERSION_IS(US)
void renderKeeperBox(ItemMenuBox *box)
#else
void renderKeeperBox(ItemMenuBox *box, int8_t flag)
#endif
{
	GsBOXF rect;
	int32_t x;
	int32_t clut;
#if VERSION_IS(US)
	BoxLabel label1;
	BoxLabel label2;
	int32_t color;
#endif
	int16_t y;
	uint8_t boxId;
	TextBoxData *tbox;
	int16_t bx;
	int16_t by;
	int16_t x2;

	boxId = box->boxId;
#if VERSION_IS(US)
	label1 = MAIN_D_801307B4;
	label2 = MAIN_D_801307C0;
#endif
	bx = UI_BOX_DATA[boxId].finalPos.x;
	by = UI_BOX_DATA[boxId].finalPos.y;
	renderKeeperTableBox(boxId, 4, 0x15, 0x67, 0xb);
	renderItemMenuSprite(boxId, 0, bx + 8, by + 0x17);
	renderKeeperTableBox(boxId, 0x6b, 0x15, 0x25, 0xb);
	renderItemMenuSprite(boxId, 3, bx + 0x6e, by + 0x17);
	renderItemMenuScrollBar(box);
	y = by + box->cursor * 0x12 + 0x21;
draw:
	renderSelectionCursor(bx + 5, y, 0x80, 0x12, 6 - boxId);
	tbox = &TEXTBOX_DATA.box[boxId];
	getVRAMModeCoords(tbox->vramMode, &x, &clut);
	y = 0x6c;
	y += tbox->backPage * tbox->vramRows * 12;
#if !VERSION_IS(US)
	renderString(0, bx + 0x38, by + 5, 0x24, 0xc, x, y, 6 - boxId, 1);
#else
	if (flag != 0) {
		drawString(label1.s, x, y);
		color = 0x38;
	} else {
		drawString(label2.s, x, y);
		color = 0x3c;
	}
	renderString(0, bx + 0x2d, by + 7, color, 0xc, x, y, 6 - boxId, 1);
#endif
	if (MAIN_D_80134F90 == boxId - 1) {
		rect.attribute = 0x40000000;
		rect.r = rect.g = 0x80;
		rect.b = 0xff;
		rect.x = bx + 4;
		rect.y = by + 3;
		rect.w = 0x8c;
		rect.h = 0x12;
		GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE, 6 - boxId);
	}
	x2 = bx + 8;
	y = by + 0x24;
	renderItemMenuItemList(box, x2, y, 0, 0, 2);
}

void tickJukeboxMenu(void)
{
	uint8_t cur;
	int32_t offset;

	if (isItemMenuBoxBusy(ITEM_MENU_LEFT) != 0) {
		return;
	}

	if (UI_BOX_DATA[0].state != 1) {
		return;
	}

	if (UI_BOX_DATA[1].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	cur = readPStat(PSTAT_249);
	if (cur != MAIN_D_80134F8F) {
		MAIN_D_80134F8F = cur;
		stopBGM();
		playMusic(MAIN_D_801303B8[cur * 2], MAIN_D_801303B8[cur * 2 + 1]);
	}

	if (isKeyDown(CONFIRM_BUTTON)) {
		offset = (ITEM_MENU_LEFT->topRow + ITEM_MENU_LEFT->cursor) * 2;
		cur = ITEM_MENU_LEFT->buf[offset];
		writePStat(PSTAT_249, cur);
		showMapHeadTextbox(2, readPStat(PSTAT_254), 0, 0x4d6);
		SCRIPT_STATE_2 = 1;
		SCRIPT_TEXTBOX_MODE = 3;
		updateItemMenuStrings(ITEM_MENU_LEFT, 9, 2);
		playSound(0, 3);
	} else if (isKeyDown(CANCEL_BUTTON)) {
		SCRIPT_STATE_2 = 4;
		playSound(0, 4);
	} else if (isKeyDown(0x1000)) {
		if (POLLED_INPUT & 8) {
			itemMenuCursorTop(ITEM_MENU_LEFT, 9, 2);
		} else {
			itemMenuCursorUp(ITEM_MENU_LEFT, 2);
		}
	} else if (isKeyDown(0x4000)) {
		if (POLLED_INPUT & 8) {
			itemMenuCursorBottom(ITEM_MENU_LEFT, 9, 2);
		} else {
			itemMenuCursorDown(ITEM_MENU_LEFT, 2);
		}
	}
}

void renderJukeboxMenu(void)
{
	int16_t bx;
	int16_t by;
	int16_t y;
	int16_t x;

	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;
	renderItemMenuSprite(1, 4, bx + 8, by + 5);
	renderItemMenuSprite(1, 5, bx + 0x26, by + 5);
	renderItemMenuScrollBar(ITEM_MENU_LEFT);
	y = by + ITEM_MENU_LEFT->cursor * 0x12 + 0x11;
draw:
	renderSelectionCursor(bx + 5, y, 0xca, 0x12, 5);
	x = bx + 8;
	y = by + 0x13;
	renderItemMenuItemList(ITEM_MENU_LEFT, x, y, 0, 0, 2);
}

void tickBirdraTransportMenu(void)
{
	uint8_t item;
	int32_t offset;

	if (isItemMenuBoxBusy(ITEM_MENU_LEFT) != 0) {
		return;
	}

	if (UI_BOX_DATA[0].state != 1) {
		return;
	}

	if (UI_BOX_DATA[1].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (SHOP_VARIABLE != 0) {
		return;
	}

	if (isKeyDown(CONFIRM_BUTTON)) {
		offset = (ITEM_MENU_LEFT->topRow + ITEM_MENU_LEFT->cursor) * 2;
		item = ITEM_MENU_LEFT->buf[offset];
		if ((item & 0x80) != 0) {
			showMapHeadTextbox(6, readPStat(PSTAT_254), 0, 0x4d6);
			SHOP_VARIABLE = BIRDRA_TRANSPORT_TARGETS[item & 0x7f].cost;
			SCRIPT_STATE_2 = 1;
			SCRIPT_NEXT_STATE_2 = 4;
			SCRIPT_TEXTBOX_MODE = 1;
			playSound(0, 3);
		} else {
			playSound(0, 0xb);
		}
	} else if (isKeyDown(CANCEL_BUTTON)) {
		SCRIPT_STATE_2 = 6;
		playSound(0, 4);
	} else if (isKeyDown(0x1000)) {
		if (POLLED_INPUT & 8) {
			itemMenuCursorTop(ITEM_MENU_LEFT, 9, 3);
		} else {
			itemMenuCursorUp(ITEM_MENU_LEFT, 3);
		}
	} else if (isKeyDown(0x4000)) {
		if (POLLED_INPUT & 8) {
			itemMenuCursorBottom(ITEM_MENU_LEFT, 9, 3);
		} else {
			itemMenuCursorDown(ITEM_MENU_LEFT, 3);
		}
	}
}

void renderBirdraTransportMenu(void)
{
	int16_t bx;
	int16_t by;
	int16_t y;
	int16_t x;

	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;
	renderItemMenuSprite(1, 6, bx + 8, by + 5);
	renderItemMenuSprite(1, 2, bx + 0xa2, by + 5);
	renderItemMenuScrollBar(ITEM_MENU_LEFT);
	y = by + ITEM_MENU_LEFT->cursor * 0x12 + 0x11;
draw:
	renderSelectionCursor(bx + 5, y, 0xca, 0x12, 5);
	x = bx + 8;
	y = by + 0x13;
	renderItemMenuItemList(ITEM_MENU_LEFT, x, y, 0, 0, 2);
}

void tickMojyaTradeMenu(void)
{
	uint8_t item;
	int32_t offset;

	if (isItemMenuBoxBusy(ITEM_MENU_LEFT) != 0) {
		return;
	}

	if (UI_BOX_DATA[0].state != 1) {
		return;
	}

	if (UI_BOX_DATA[1].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (readPStat(PSTAT_249) != 0xff) {
		return;
	}

	if (isKeyDown(CONFIRM_BUTTON)) {
		offset = (ITEM_MENU_LEFT->topRow + ITEM_MENU_LEFT->cursor) * 2;
		item = ITEM_MENU_LEFT->buf[offset];
		if ((item & 0x80) != 0) {
			writePStat(PSTAT_249, (item & 0x7f));
			showMapHeadTextbox(0xb, readPStat(PSTAT_254), 0, 0x4d6);
			SCRIPT_STATE_2 = 1;
			SCRIPT_NEXT_STATE_2 = 4;
			SCRIPT_TEXTBOX_MODE = 1;
			playSound(0, 3);
		} else {
			playSound(0, 0xb);
		}
	} else if (isKeyDown(CANCEL_BUTTON)) {
		SCRIPT_STATE_2 = 6;
		playSound(0, 4);
	} else if (isKeyDown(0x1000)) {
		if (POLLED_INPUT & 8) {
			itemMenuCursorMoveToTop(ITEM_MENU_LEFT);
			updateMojyaTradeStrings();
		} else {
			itemMenuCursorUp(ITEM_MENU_LEFT, 4);
		}
	} else if (isKeyDown(0x4000)) {
		if (POLLED_INPUT & 8) {
			itemMenuCursorMoveToBottom(ITEM_MENU_LEFT);
			updateMojyaTradeStrings();
		} else {
			itemMenuCursorDown(ITEM_MENU_LEFT, 4);
		}
	}
}

void renderMojyaTradeMenu(void)
{
	uint32_t x;
	int32_t clut;
	int16_t bx;
	int16_t by;
	int16_t y;
	int16_t x2;
	uint8_t id;
	TextBoxData *box;

	id = ITEM_MENU_LEFT->boxId;
	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;
	renderHorizontalLine(1, 4, 0x15, 0xd7);
	box = &TEXTBOX_DATA.box[id];
	getVRAMModeCoords(box->vramMode, (int32_t *)&x, &clut);
	y = 0x6c;
	y += box->backPage * box->vramRows * 12;
#if !VERSION_IS(US)
	renderString(0, bx + 0x14, by + 5, 0x3c, 0xc, x, y, 5, 1);
	renderString(0, bx + 0x7a, by + 5, 0x3c, 0xc, x + 0x3c, y, 5, 1);
#else
	drawString(MAIN_D_801307CC, x, y);
	renderString(0, bx + 0x14, by + 5, 0x42, 0xc, x, y, 5, 1);
	renderString(0, bx + 0x7a, by + 5, 0x4e, 0xc, x + 0x42, y, 5, 1);
#endif
	y = by + ITEM_MENU_LEFT->cursor * 0x12 + 0x19;
draw:
	renderSelectionCursor(bx + 5, y, 0xd4, 0x12, 5);
	x2 = bx + 8;
	y = by + 0x1b;
	renderItemMenuItemList(ITEM_MENU_LEFT, x2, y, 0, 0, 2);
}

void updateMojyaTradeStrings(void)
{
	showMapHeadTextbox(0xf, 0xff, 1, 0x4d6);

	--TEXTBOX_DATA.box[1].writeCount;
	TEXTBOX_LINES_PTR[0x254] = 0xd;
	TEXTBOX_LINES_PTR[0x354] = 0xd;

	updateItemMenuStrings(ITEM_MENU_LEFT, 0xa, 4);
}

void setupNewGameDialogueBox(void)
{
	int32_t i;
	RECT rect2;
	RECT rect1;
	DVECTOR screenPos;
	uint8_t flags;

	flags = 0x81;
#if VERSION_IS(US)
	DRAW_STRING2_IS_FIXED_WIDTH = 0;
#endif
	for (i = 2; i < 10; i++) {
		if (ENTITY_TABLE[i]->type == 0x75) {
			getEntityScreenPos(ENTITY_TABLE[i], 1, &screenPos);
		}
	}

	setRECT(&rect1, screenPos.vx, screenPos.vy, 10, 10);
	setRECT(&rect2, -130, -78, 262, 59);
	createTextbox(0, flags, &rect2, &rect1, tickScriptDialogueBox, renderScriptDialogueBox);
	registerTextbox(0, 0, 4, 1, 0);
}

void showNewGameDialogue(int32_t textId, uint16_t nextState)
{
	showMapHeadTextbox(textId, 0xfe, 0, 0x4d6);

	SCRIPT_STATE_2 = 1;
	SCRIPT_NEXT_STATE_2 = nextState;
	SCRIPT_TEXTBOX_MODE = 1;
}

void showNewGameSelection(int32_t textId, uint16_t nextState)
{
	int32_t sel = 0;

	showMapheadSelection(textId, 0xfe, 2, &sel, 0x4d6);

	SCRIPT_STATE_2 = 1;
	SCRIPT_NEXT_STATE_2 = nextState;
	SCRIPT_TEXTBOX_MODE = 2;
}

void setupNameSelectorBox(void)
{
	int32_t i;
	uint8_t flags;
	RECT rect2;
	RECT rect1;
	DVECTOR screenPos;

#if VERSION_IS(US)
	DRAW_STRING2_IS_FIXED_WIDTH = 1;
#endif
	if ((NAMING_BOX_FLAG & 2) == 0) {
		flags = 0xc1;
	} else {
		flags = 0xe1;
	}

	for (i = 2; i < 10; i++) {
		if (ENTITY_TABLE[i]->type == 0x75) {
			getEntityScreenPos(ENTITY_TABLE[i], 1, &screenPos);
		}
	}

	setRECT(&rect1, screenPos.vx, screenPos.vy, 10, 10);
	setRECT(&rect2, -145, -91, 290, 138);
	createTextbox(1, flags, &rect2, &rect1, tickNamingBox, renderNamingBox);
	registerTextbox(1, 1, 7, 1, 0);

	SHOP_AMOUNT = 0;
	NAMING_SELECTOR = 0;
	fillNamingMenuStrings();
}

void setupNameDisplayBox(void)
{
	int32_t i;
	uint8_t flags;
	RECT rect2;
	RECT rect1;
	DVECTOR screenPos;

	if ((NAMING_BOX_FLAG & 2) == 0) {
		flags = 0xc1;
	} else {
		flags = 0xe1;
	}

	for (i = 2; i < 10; i++) {
		if (ENTITY_TABLE[i]->type == 0x75) {
			getEntityScreenPos(ENTITY_TABLE[i], 1, &screenPos);
		}
	}

	setRECT(&rect1, screenPos.vx, screenPos.vy, 10, 10);
	setRECT(&rect2, -145, 60, 149, 42);
	createTextbox(2, flags, &rect2, &rect1, 0, renderNameDisplayBox);
	registerTextbox(2, 0, 1, 0, 0);

	NAMING_CURRENT_LETTER = strlen(NAMING_BUFFER) >> 1;
	if (NAMING_CURRENT_LETTER == 6) {
		--NAMING_CURRENT_LETTER;
	}

	updateNamingPreview();
}

void tickNamingBox(void)
{
	int16_t col;
	int16_t row;
	char *str;
	char **rows;
	int16_t idx;
	uint16_t special;
	uint8_t hi;
	uint8_t lo;

	if (flipTextboxPage(1) != 0) {
		return;
	}
	if (UI_BOX_DATA[1].state != 1) {
		return;
	}
	if (UI_BOX_DATA[2].state != 1) {
		return;
	}
	if (isXPressedAfterDialogue() == 0) {
		return;
	}
	row = NAMING_SELECTOR / 5;
	col = NAMING_SELECTOR % 5;
	special = NAMING_SELECTOR & 0x7fff;
	if (isKeyDown(0x80)) {
		NAMING_BUFFER[0] = 0;
		NAMING_CURRENT_LETTER = 0;
		updateNamingPreview();
		playSound(0, 3);
		return;
	}
	if (isKeyDown(CONFIRM_BUTTON)) {
		if ((NAMING_SELECTOR & 0x8000) == 0) {
			if (row < 9) {
				rows = NAMING_CHAR_PAGES[SHOP_AMOUNT * 2];
			} else {
				row -= 9;
				rows = NAMING_CHAR_PAGES[SHOP_AMOUNT * 2 + 1];
			}
			str = rows[row];
			col *= 2;
			hi = str[col + 0];
			lo = str[col + 1];
			if (hi == 0x81 && lo == 0x40 && NAMING_CURRENT_LETTER == 0) {
				playSound(0, 0xb);
				return;
			}
			idx = NAMING_CURRENT_LETTER * 2;
			NAMING_BUFFER[idx + 0] = hi;
			NAMING_BUFFER[idx + 1] = lo;
			NAMING_BUFFER[idx + 2] = 0;
			if (NAMING_CURRENT_LETTER != 5) {
				NAMING_CURRENT_LETTER++;
			}
			updateNamingPreview();
			if (NAMING_BUFFER[10] != 0 && NAMING_CURRENT_LETTER == 5) {
				NAMING_SELECTOR = 0x8000 | NAMING_OK;
			}
			playSound(0, 3);
			return;
		}
		switch (special) {
#if !VERSION_IS(US)
		case 0:
		case 1:
		case 2:
			SHOP_AMOUNT = special;
			fillNamingMenuStrings();
			playSound(0, 3);
			break;
#endif
		case NAMING_DELETE:
			namingDeleteLast();
			break;
		case NAMING_OK:
			if (NAMING_BUFFER[0] == 0) {
				playSound(0, 0xb);
				return;
			}
			terminateNamingBuffer();
			playSound(0, 3);
			SCRIPT_STATE_2 = 0x15;
			break;
		}
	} else if (isKeyDown(ALT_BUTTON)) {
		NAMING_SELECTOR = 0x8000;
		playSound(0, 2);
	} else if (isKeyDown(CANCEL_BUTTON)) {
		namingDeleteLast();
	} else if (isKeyDown(0x800)) {
		NAMING_SELECTOR = 0x8000 | NAMING_OK;
		playSound(0, 2);
	} else if (isKeyDown(0x8000)) {
		namingSelectionLeft(col, row, special);
	} else if (isKeyDown(0x2000)) {
		namingSelectionRight(col, row, special);
	} else if (isKeyDown(0x1000)) {
		namingSelectionUp(col, row);
	} else if (isKeyDown(0x4000)) {
		namingSelectionDown(col, row);
	}
}

GARBAGE(renderNamingBox, 16);

void renderNamingBox(void)
{
	TextBoxData *box;
	int32_t k;
	int32_t i;
	int16_t bx;
	int16_t by;
	int16_t x;
	int16_t y;
	int16_t texY;
	int16_t ty;
	int16_t *lbl;
	int16_t texX;
	int32_t j;

	renderSelectionBox();
	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;
	box = &TEXTBOX_DATA.box[1];
	texY = box->backPage * box->vramRows * 12;
	texY += box->vramRow * 12;
	texX = 0;
	lbl = NAMING_CTRL_BOXES;
	for (j = 0; j < NAMING_LABELS; j++, lbl += 3) {
		renderString(0, bx + lbl[0], by + lbl[1], lbl[2], 12, texX, texY, 5, 1);
		texX += lbl[2];
	}
	if (NAMING_BUFFER[0] == 0) {
		ty = 0xd;
	} else {
		ty = 0;
	}
	renderString(ty, bx + lbl[0], by + lbl[1], lbl[2], 12, texX, texY, 5, 1);
	y = by + 6;
	ty = texY + 12;
	for (i = 0; i < 3; i++, ty += 12) {
		texX = 0;
		for (j = 0; j < 3; j++, y += 14) {
			x = bx + 0x4e;
			for (k = 0; k < 5; k++, x += 18, texX += 12) {
				renderString(0, x, y, 12, 12, texX, ty, 5, 1);
			}
		}
	}
	y = by + 6;
	for (i = 0; i < 3; i++, ty += 12) {
		texX = 0;
		for (j = 0; j < 3; j++, y += 14) {
			x = bx + 0xbe;
			for (k = 0; k < 5; k++, x += 18, texX += 12) {
				renderString(0, x, y, 12, 12, texX, ty, 5, 1);
			}
		}
	}
}

void fillNamingMenuStrings(void)
{
	uint8_t *buf;
	char **table;
	int32_t j;
	uint32_t len;
	int32_t page;
	int32_t row;
	TextBoxData *box;
	uint8_t *line;

	box = &TEXTBOX_DATA.box[1];
	buf = TEXTBOX_LINES_PTR + box->vramRow * 64;
	buf = (uint8_t *)((uint32_t)buf + (box->backPage ^ 1) * box->vramRows * 64);
	line = buf;
	*buf++ = 1;
	*buf++ = 7;
	strcpy(buf, NAMING_TITLE);
	len = strlen(NAMING_TITLE);
	buf += len;
	*buf++ = 1;
	*buf++ = 1;
	strcpy(buf, NAMING_BUTTONS);
	len = strlen(NAMING_BUTTONS);
	buf += len;
	*buf++ = 0xd;
	*buf++ = 0;
	line += 0x40;
	page = SHOP_AMOUNT * 2;
	for (row = 0; row < 2; row++) {
		table = NAMING_CHAR_PAGES[page + row];
		for (j = 0; j < 9; j += 3) {
			buf = line + row * 0xc0 + (j / 3) * 64;
			strcpy(buf, table[j]);
			len = strlen(table[j]);
			buf += len;
			strcpy(buf, table[j + 1]);
			len = strlen(table[j + 1]);
			buf += len;
			strcpy(buf, table[j + 2]);
			len = strlen(table[j + 2]);
			buf += len;
			*buf++ = 0xd;
			*buf++ = 0;
		}
	}
	buf -= 2;
	*buf = 0;
	box->pageReady = 1;
	box->writeCount++;
}

void updateNamingPreview(void)
{
	uint8_t *out;
	int32_t len;

	out = TEXTBOX_LINES_PTR;
	*out++ = 1;
	*out++ = 1;

	if ((NAMING_BOX_FLAG & 1) == 0) {
		strcpy(out, NAMING_PROMPT_TAMER);
		len = strlen(NAMING_PROMPT_TAMER);
	} else {
		strcpy(out, NAMING_PROMPT_DIGIMON);
#if !VERSION_IS(US)
		len = strlen(NAMING_PROMPT_DIGIMON);
#else
		len = strlen((int32_t)MAIN_D_801345F4 + 1);
#endif
	}

	out += len;
	strcpy(out, NAMING_BUFFER);
	len = strlen(NAMING_BUFFER);
	out += len;
	out = padWithSpaces(out, 6, len);
	terminateString(out, 1);
	out = TEXTBOX_LINES_PTR;
#if !VERSION_IS(US)
	drawString2(out, 0, 0);
#else
	drawString2(out, 0, 0, 1);
#endif
}

void namingDeleteLast(void)
{
	if (NAMING_BUFFER[0] != 0) {
		if (NAMING_CURRENT_LETTER == 0) {
			NAMING_BUFFER[0] = 0;
		} else {
#if !VERSION_IS(US)
			NAMING_BUFFER[NAMING_CURRENT_LETTER * 2] = 0;
			NAMING_CURRENT_LETTER--;
#else
			NAMING_CURRENT_LETTER--;
			NAMING_BUFFER[NAMING_CURRENT_LETTER * 2] = 0;
#endif
		}

		updateNamingPreview();
		playSound(0, 3);
	}
}

void terminateNamingBuffer(void)
{
	int32_t pos;

	for (pos = 0; pos < 0xc; pos += 2) {
		if (NAMING_BUFFER[pos + 2] == 0) {
			break;
		}
	}

	while (pos != 0) {
		if ((uint8_t)NAMING_BUFFER[pos] != 0x81) {
			break;
		}

		if ((uint8_t)NAMING_BUFFER[pos + 1] != 0x40) {
			break;
		}

		pos -= 2;
	}

	NAMING_BUFFER[pos + 2] = 0;
}

void namingSelectionLeft(int16_t col, int16_t row, int16_t specialIdx)
{
	if ((NAMING_SELECTOR & 0x8000) == 0) {
		if (row < 9) {
			if (col == 0) {
				NAMING_SELECTOR = NAMING_ROLLOVER_CHARS[row];
			} else {
				--col;
				NAMING_SELECTOR = col + (row * 5);
			}
		} else {
			if (col == 0) {
				row -= 9;
				col = 4;
			} else {
				--col;
			}

			NAMING_SELECTOR = col + (row * 5);
		}
	} else {
		NAMING_SELECTOR = NAMING_ROLLOVER_CTRL[specialIdx] + 0x31;
	}

	playSound(0, 2);
}

void namingSelectionRight(int16_t col, int16_t row, int16_t specialIdx)
{
	if ((NAMING_SELECTOR & 0x8000) == 0) {
		if (row < 9) {
			if (col == 4) {
				row += 9;
				col = 0;
			} else {
				++col;
			}

			NAMING_SELECTOR = col + (row * 5);
		} else if (col == 4) {
			NAMING_SELECTOR = NAMING_ROLLOVER_CHARS[row - 9];
		} else {
			++col;
			NAMING_SELECTOR = col + (row * 5);
		}
	} else {
		NAMING_SELECTOR = NAMING_ROLLOVER_CTRL[specialIdx];
	}

	playSound(0, 2);
}

void namingSelectionUp(int16_t column, int16_t row)
{
	if ((NAMING_SELECTOR & 0x8000) == 0) {
		if (row < 9) {
			if (row == 0) {
				row = 8;
			} else {
				--row;
			}
		} else if (row == 9) {
			row = 17;
		} else {
			--row;
		}

		NAMING_SELECTOR = column + row * 5;
	} else if (NAMING_SELECTOR == 0x8000) {
		NAMING_SELECTOR = 0x8000 | NAMING_OK;
	} else {
		--NAMING_SELECTOR;
	}

	playSound(0, 2);
}

void namingSelectionDown(int16_t column, int16_t row)
{
	if ((NAMING_SELECTOR & 0x8000) == 0) {
		if (row < 9) {
			if (row == 8) {
				row = 0;
			} else {
				++row;
			}
		} else if (row == 0x11) {
			row = 9;
		} else {
			++row;
		}

		NAMING_SELECTOR = column + row * 5;
	} else if (NAMING_SELECTOR == (0x8000 | NAMING_OK)) {
		NAMING_SELECTOR = 0x8000;
	} else {
		++NAMING_SELECTOR;
	}

	playSound(0, 2);
}

void renderSelectionBox(void)
{
	GsOT_TAG *tag;
	SelectionBoxUVData u0;
	SelectionBoxUVData u1;
	SelectionBoxUVData v0;
	SelectionBoxUVData v1;
	SelectionBoxOffsetData xOffset;
	SelectionBoxOffsetData yOffset;
	SelectionBoxOffsetData width;
	SelectionBoxOffsetData height;
	SelectionBoxOffsetData altXOffset;
	SelectionBoxOffsetData altWidth;
	uint32_t idx;
	int16_t bx;
	int16_t by;
	int16_t baseX;
	uint16_t tpage;
	uint16_t clut;
	POLY_FT4 *prim;
	int32_t i;
	int16_t baseY;

	tpage = GetTPage(0, 0, 896, 448);
	clut = GetClut(256, 508);
	u0 = NAMING_U02;
	u1 = NAMING_U13;
	v0 = NAMING_V01;
	v1 = NAMING_V23;
	xOffset = NAMING_CTRL_X;
	yOffset = NAMING_Y;
	width = NAMING_CTRL_WIDTH;
	height = NAMING_HEIGHT;
	altXOffset = NAMING_CHAR_X;
	altWidth = NAMING_CHAR_WIDTH;

	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;

	if ((NAMING_SELECTOR & 0x8000) == 0) {
		baseY = NAMING_SELECTOR / 5;
		baseX = NAMING_SELECTOR % 5;

		if (baseY < 9) {
			baseX = (bx + (baseX * 18)) + 0x4a;
			baseY = (by + (baseY * 14)) + 2;
		} else {
			baseX = (bx + (baseX * 18)) + 0xba;
			baseY = (by + ((baseY - 9) * 14)) + 2;
		}
	} else {
		idx = ((NAMING_SELECTOR & 0x7fff) * 3) + 3;
		baseX = (bx + NAMING_CTRL_BOXES[idx]) - 4;
#if !VERSION_IS(US)
		baseY = (by + NAMING_CTRL_BOXES[idx + 1]) - 4;
#else
		baseY = (by + (&NAMING_CTRL_BOXES[1])[idx]) - 4;
#endif
	}

	tag = ACTIVE_ORDERING_TABLE->org;

	for (i = 0; i < 8; i++) {
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		setUV4(prim, u0.data[i], v0.data[i], u1.data[i], v0.data[i], u0.data[i], v1.data[i], u1.data[i], v1.data[i]);

		if ((NAMING_SELECTOR & 0x8000) == 0) {
			setPosDataPolyFT4(prim, baseX + xOffset.data[i], baseY + yOffset.data[i], width.data[i], height.data[i]);
		} else {
			setPosDataPolyFT4(prim, baseX + altXOffset.data[i], baseY + yOffset.data[i], altWidth.data[i], height.data[i]);
		}

		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = tpage;
		prim->clut = clut;
		AddPrim(&tag[5], prim);
		prim++;
		GsSetWorkBase((PACKET *)prim);
	}
}

void renderNameDisplayBox(void)
{
	int16_t x;
	int16_t y;
	int16_t sx;
	int16_t v;
	int16_t i;
	int16_t y6;

	x = UI_BOX_DATA[2].finalPos.x;
	y = UI_BOX_DATA[2].finalPos.y;
	sx = x + 7;
	y6 = y + 6;
	if ((NAMING_BOX_FLAG & 1) == 0) {
		v = 0x48;
	} else {
		v = 0x54;
	}
	renderString(0, sx, y6, v, 0xc, 0, 0, 4, 1);
	sx = x + 0x3a;
	y6 = y + 0x14;
	for (i = 0; i < 6; i++, sx += 0xe, v += 0xc) {
		renderString(0, sx, y6, 0xc, 0xc, v, 0, 4, 1);
	}
#if !VERSION_IS(US)
	renderNamingUnderscore(2, NAMING_CURRENT_LETTER * 0xe + 0x3b, 0x22, 0xc);
#else
	renderNamingUnderscore(2, NAMING_CURRENT_LETTER * 0xc + 4, 0x10, 0xc);
#endif
}

void renderNamingUnderscore(uint8_t boxId, int16_t x, int16_t y, int16_t w)
{
	uint32_t color;

	x += UI_BOX_DATA[boxId].finalPos.x;
	y += UI_BOX_DATA[boxId].finalPos.y;
	boxId = 6 - boxId;
	color = 0x20202;
	drawLine2P(color, x, y, (x + w) - 1, y, boxId, 0);
	y++;
	color = 0x10c0c0;
	drawLine2P(color, x, y, (x + w) - 1, y, boxId, 0);
	y++;
	color = 0x20202;
	drawLine2P(color, x, y, (x + w) - 1, y, boxId, 0);
}
