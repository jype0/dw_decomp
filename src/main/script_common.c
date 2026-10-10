#include <string.h>

#include <libgs.h>

#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/item.h>
#include <dw/main.h>
#include <dw/map.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/ui.h>
#include <dw/utils.h>
#include <dw/version.h>
#include <text/main/script_common.h>

typedef struct {
	uint8_t b[11];
} ShopkeeperIdTable;

typedef struct {
	uint8_t b[8];
} BabyTypeTable;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t chars;
} MenuTextField;

typedef struct {
	MenuTextField v[5];
} AmountBoxLayout;

typedef struct {
	int16_t srcX;
	int16_t srcY;
	int16_t x;
	int16_t y;
	int16_t width;
} MenuTextSprite;

typedef struct {
	MenuTextSprite v[3];
} ConfirmBoxLayout;

typedef struct {
	int16_t v[28];
} BoxUVTable;

extern uint32_t INPUT_REPEAT_MASK;
extern int32_t MAIN_D_80135028;
extern int32_t INPUT_PENDING_MASK;
extern uint32_t INPUT_FRESH_MASK;
extern uint16_t INPUT_REPEAT_COUNTER;
extern int32_t ITEM_MENU_SUB_TEXTBOX_LINE;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern char *MAP_NAME_PTR[];
extern char *ITEM_DESCS[];

void renderSelectionCursor(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);
void renderItemSprite(int32_t itemId, int32_t x, int32_t y, int32_t depth);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t xPos, int32_t yPos, int32_t width, int32_t height);
void renderString(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8);
int32_t hasMove(int32_t moveId);
void unlearnMove(int32_t moveId);
void renderMonochromonMoodBubble(int32_t instanceId);
void translateConditionFXToEntity(Entity *entity, SVECTOR *out);
int32_t worldPosToScreenPos(SVECTOR *worldPos, DVECTOR *screenPos);
void closeTextbox(int32_t boxId, RECT *target);
void updateItemMenuLine(ItemMenuBox *box, int32_t style);
void renderShopBitsBox(void);
void tickItemMenu(void);
void renderItemMenu(void);
void itemMenuCursorDown(ItemMenuBox *box, int32_t style);
int32_t createItemMenuDescriptionBox(ItemMenuBox *box, RECT *origin, uint8_t uiBoxId);
int32_t shopFillBuyItemList(void);
int32_t shopFillSellItemList(void);
int32_t readSelectedItemMerit(void);
void tickItemMenuDescriptionBox(void);
void renderItemMenuDescriptionBox(void);
void tickShopBitsBox(void);
uint8_t *getShopkeeperLine(int32_t idx);
uint8_t *resolveMapHeadEntry(int32_t section, int32_t idx);
void processInput(void);
int32_t isKeyDown(uint32_t key);
void setFreshDialogue(void);
int32_t isXPressedAfterDialogue(void);
void *allocateArray(uint32_t size);
void freeArray(uint32_t *array);
uint8_t getRecycleId(uint8_t value);
int32_t isItemMenuBoxBusy(ItemMenuBox *box);
void itemMenuCursorTop(ItemMenuBox *box, int32_t startRow, int32_t style);
void itemMenuCursorUp(ItemMenuBox *box, int32_t style);
void itemMenuCursorBottom(ItemMenuBox *box, int32_t startRow, int32_t style);
void updateItemMenuSubTextboxLine(void);
int32_t flipTextboxPage(uint8_t a);
void itemMenuCursorMoveToBottom(ItemMenuBox *box);
void itemMenuCursorMoveToTop(ItemMenuBox *box);
uint8_t *getTextboxLine(ItemMenuBox *box, uint8_t index);
void terminateString(uint8_t *str, int32_t flag);
uint8_t *padWithSpaces(uint8_t *str, int32_t width, int32_t used);
void setDigimonRaised(int32_t digimonId);
int32_t hasDigimonRaised(int32_t digimonId);
void unlockMedal(uint16_t medal);
int32_t hasMedal(uint16_t medal);
void triggerSeadramonCutscene(void);
void checkShopMap(int32_t mapId);
void checkArenaMap(int32_t mapId);
void getVRAMModeCoords(int32_t mode, int32_t *outX, int32_t *outClut);
void setupDialogueBox(uint8_t owner);
uint16_t showTextboxReady(uint8_t boxId, uint8_t speakerId);
void updateItemMenuAmountBoxString(void);
void tickSingleCardShop(void);
void renderSingleCardShop(void);
int32_t createItemMenuAmountBox(RECT *origin);
int32_t createSingleCardShopMenu(RECT *origin);
void renderItemMenuSprite(uint8_t boxId, uint8_t idx, int16_t x, int16_t y);
void renderItemMenuScrollBar(ItemMenuBox *box);
void renderItemMenuItemList(ItemMenuBox *box, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int32_t flag);
void renderInsetWithoutBox(uint8_t boxId, int16_t x, int16_t y, int16_t w, int16_t h);
void drawLine3P(int32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t otz, int32_t flag);
void renderCardSprite(uint8_t spriteId, int16_t x, int16_t y, int32_t depth);
void calculateItemMenuStrings(ItemMenuBox *box, uint8_t row, int32_t isLast);
void calculateCardMenuStrings(ItemMenuBox *box, uint8_t row, int32_t isLast);
void calculateMusicMenuStrings(ItemMenuBox *box, uint8_t row, int32_t isLast);
void calculateBirdramonMenuStrings(ItemMenuBox *box, uint8_t row, int32_t isLast);
void calculateItemListStrings(ItemMenuBox *box, uint8_t row, int32_t isLast);
void renderKeeperTableBox(uint8_t boxId, int16_t x, int16_t y, int16_t w, int16_t h);
uint8_t *intToStringSJIS(uint8_t *buf, int32_t value, uint8_t digits, int32_t flag);
void renderHorizontalLine(uint8_t boxId, int16_t x, int16_t y, int16_t w);
void drawLine2P(uint32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t order, uint32_t mode);
void renderItemMenuAmountBox(void);
void renderVerticalLine(uint8_t boxId, int16_t x, int16_t y, int16_t h);
void tickItemMenuAmountBox(void);
void createTextbox(int32_t boxId, uint8_t flags, RECT *rect, RECT *origin, void *tick, void *render);
void showMapHeadTextbox(int32_t idx, uint8_t owner, uint8_t boxId, int32_t section);
uint8_t *getScriptSection(uint8_t *script, uint16_t section);

static void *script_common_text_order[] = {
	checkArenaMap,
	loadShopLibrary,
	checkShopMap,
	triggerSeadramonCutscene,
	createMonochromonMoodBubble,
	hasMedal,
	unlockMedal,
	hasDigimonRaised,
	setDigimonRaised,
	showMapheadSelection,
	setDialogueOwner,
	renderVerticalLine,
	padWithSpaces,
	terminateString,
	getTextboxLine,
	renderKeeperTableBox,
	itemMenuCursorMoveToTop,
	itemMenuCursorMoveToBottom,
	updateItemMenuLine,
	calculateItemListStrings,
	calculateBirdramonMenuStrings,
	calculateMusicMenuStrings,
	calculateCardMenuStrings,
	calculateItemMenuStrings,
	flipTextboxPage,
	lostAllLives,
	showMapHeadTextbox,
	renderCardSprite,
	renderInsetWithoutBox,
	renderHorizontalLine,
	playShopSoundOnlyInSavannah,
	updateItemMenuSubTextboxLine,
	renderItemMenuItemList,
	renderItemMenuScrollBar,
	renderItemMenuSprite,
	createItemMenuDescriptionBox,
	itemMenuCursorDown,
	itemMenuCursorBottom,
	itemMenuCursorUp,
	itemMenuCursorTop,
	createSingleCardShopMenu,
	createItemMenuAmountBox,
	isItemMenuBoxBusy,
	updateItemMenuStrings,
	initItemMenuBox,
	getItemMenuFromType,
	openDiscardItem,
	createItemMenu,
	showShopkeepSelection,
	createShopBitsBox,
	destroyItemMenuBox,
	showShopkeeperTextbox,
	allocateItemMenuBox,
	openShop,
	isPartnerBaby,
	dailyPStatTrigger,
	getRecycleId,
	freeArray,
	allocateArray,
	handleItemLoss,
	setInputRepeatMask,
	isXPressedAfterDialogue,
	setFreshDialogue,
	isKeyDown,
	processInput,
	inputInit,
	renderMonochromonMoodBubble,
	resolveMapHeadEntry,
	getShopkeeperLine,
#if VERSION_IS(EU)
	tickShopBitsBox,
	renderShopBitsBox,
	tickSingleCardShop,
	renderSingleCardShop,
	updateItemMenuAmountBoxString,
	tickItemMenuAmountBox,
	renderItemMenuAmountBox,
	tickItemMenuDescriptionBox,
	renderItemMenuDescriptionBox,
	readSelectedItemMerit,
	tickItemMenu,
	renderItemMenu,
#else
	renderShopBitsBox,
	tickShopBitsBox,
	renderSingleCardShop,
	tickSingleCardShop,
	updateItemMenuAmountBoxString,
	renderItemMenuAmountBox,
	tickItemMenuAmountBox,
	renderItemMenuDescriptionBox,
	tickItemMenuDescriptionBox,
	readSelectedItemMerit,
	renderItemMenu,
	tickItemMenu,
#endif
	shopFillSellItemList,
	shopFillBuyItemList,
};

// clang-format off
BGM_TRACK_NAMES_TEXT
TOURNAMENT_NAMES_TEXT
TOURNAMENT_GRADES_TEXT

uint8_t *ARRAY_SECTION_START = TEXTURE_BUFFER;

BabyTypeTable BABY_IDS = { {
	0x01, 0x02, 0x0f, 0x10, 0x1d, 0x1e, 0x2b, 0x2c,
} };

int16_t ARENA_MAPS[2] = {
	0x00d0, 0x00df,
};

int16_t MAIN_D_801345C0[1] = {
	0x0083,
};

uint16_t MAIN_D_801345C2 = 0x1700;

MapLightUpdateData MAP_LIGHT_UPDATE_DATA[1] = {
	{ 0xb7, 0x01, 0x0031, 0x0002, 0xffff },
};

GsSPRITE MONOCHROMON_BUBBLE_SPRITE = {
	0x50000000,			/* attribute */
	0,				/* x */
	0,				/* y */
	32,				/* w */
	32,				/* h */
	getTPage(0, 0, 640, 0),		/* tpage */
	128,				/* u */
	0,				/* v */
	0,				/* cx */
	486,				/* cy */
	0x80,				/* r */
	0x80,				/* g */
	0x80,				/* b */
	16,				/* mx */
	16,				/* my */
	0x1000,				/* scalex */
	0x1000,				/* scaley */
	0,				/* rotate */
};

AmountBoxLayout AMOUNT_BOX_LAYOUT = {
	{
#if VERSION_IS(EU)
		{ 0x001a, 0x0007, 0x000d },
		{ 0x0046, 0x001b, 0x0005 },
		{ 0x005e, 0x002b, 0x0002 },
		{ 0x003e, 0x0041, 0x0006 },
		{ 0x001a, 0x002b, 0x0001 },
#elif !VERSION_IS(US)
		{ 0x001a, 0x0007, 0x0008 },
		{ 0x0032, 0x001b, 0x0005 },
		{ 0x0056, 0x002b, 0x0002 },
		{ 0x0026, 0x0041, 0x0006 },
		{ 0x0016, 0x002b, 0x0001 },
#else
		{ 0x001a, 0x0007, 0x0008 },
		{ 0x004a, 0x001b, 0x0005 },
		{ 0x0059, 0x002b, 0x0002 },
		{ 0x0040, 0x0041, 0x0006 },
		{ 0x0016, 0x002b, 0x0001 },
#endif
	},
};

ConfirmBoxLayout CONFIRM_BOX_LAYOUT = {
	{
#if !VERSION_IS(US)
		{ 0x0000, 0x0000, 0x0004, 0x0002, 0x0060 },
		{ 0x0000, 0x000c, 0x0010, 0x0012, 0x0018 },
		{ 0x0018, 0x000c, 0x003a, 0x0012, 0x0024 },
#else
		{ 0x0000, 0x0000, 0x0004, 0x0002, 0x0060 },
		{ 0x0000, 0x000c, 0x000e, 0x0012, 0x0024 },
		{ 0x0024, 0x000c, 0x0044, 0x0012, 0x0024 },
#endif
	},
};

BoxUVTable ITEM_MENU_HEADER_UVS = {
	{
		0x0200, 0x01a2, 0x0014, 0x0007,
		0x0214, 0x01a2, 0x0014, 0x0007,
		0x0250, 0x01b0, 0x0014, 0x0007,
		0x0264, 0x01b0, 0x0016, 0x0007,
		0x0228, 0x01a2, 0x000c, 0x0007,
		0x0234, 0x01a2, 0x0014, 0x0007,
		0x0238, 0x01b0, 0x0014, 0x0007,
	},
};

ShopkeeperIdTable SHOPKEEP_DIGIMON_TYPES = {
	{ 0x97, 0x99, 0xa3, 0xa5, 0x82, 0x89, 0x87, 0xaa, 0x76, 0x80, 0xff },
};

int16_t SHOP_MAPS[8] = {
	0x0083, 0x0085, 0x0086, 0x008d, 0x00cf, 0x00d3, 0x00d7, 0x00da,
};

uint8_t RECYCLABLE_ITEMS[78] = {
	0x0b, 0x0c, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c,
	0x1d, 0x1e, 0x1f, 0x21, 0x23, 0x24, 0x2e, 0x32,
	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x43, 0x46,
	0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4e, 0x4f,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x54, 0x55, 0x56,
	0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e,
	0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
	0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e,
	0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
	0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c,
};

char *BGM_TRACK_NAMES[63] = {
	STR_BGM_TRACK_NON_BEWILDERING_FOREST_THEME,
	STR_BGM_TRACK_NON_BEWILDERING_FOREST_NIGHT_THEME,
	STR_BGM_TRACK_TROPICAL_THEME,
	STR_BGM_TRACK_TROPICAL_NIGHT_THEME,
	STR_BGM_TRACK_MT_PANORAMA_THEME,
	STR_BGM_TRACK_MT_PANORAMA_NIGHT_THEME,
	STR_BGM_TRACK_DRILL_TUNNEL_THEME,
	STR_BGM_TRACK_OGRE_FORTRESS_THEME,
	STR_BGM_TRACK_OVERDELL_CEMETARY_THEME,
	STR_BGM_TRACK_CANYON_THEME,
	STR_BGM_TRACK_OGREMON_THEME_NO_2,
	STR_BGM_TRACK_EVERYTHING_SHOP_THEME,
	STR_BGM_TRACK_OGREMON_THEME_NO_3,
	STR_BGM_TRACK_LAVA_CAVE_THEME,
	STR_BGM_TRACK_DARK_ARISTCRATS_MANSION_THEME,
	STR_BGM_TRACK_UNDERGROUND_LAB_THEME,
	STR_BGM_TRACK_GEAR_SAVANNA_THEME,
	STR_BGM_TRACK_GEAR_SAVANNA_NIGHT_THEME,
	STR_BGM_TRACK_LEOMON_THEME,
	STR_BGM_TRACK_AMIDA_FOREST_THEME,
	STR_BGM_TRACK_AMIDA_FOREST_NIGHT_THEME,
	STR_BGM_TRACK_THE_ANCIENT_REGION_OF_DINO_SPEEDY_TIME_ZONE_THEME,
	STR_BGM_TRACK_THE_ANCIENT_REGION_OF_DINO_SPEEDY_TIME_ZONE_NIGHT,
	STR_BGM_TRACK_THE_ANCIENT_REGION_OF_DINO_GLACIAL_TIME_ZONE_THEME,
	STR_BGM_TRACK_THE_ANCIENT_REGION_OF_DINO_GLACIAL_TIME_ZONE_NIGHT,
	STR_BGM_TRACK_FREEZELAND_THEME,
	STR_BGM_TRACK_FREEZELAND_NIGHT_THEME,
	STR_BGM_TRACK_IGLOO_THEME,
	STR_BGM_TRACK_CURLING_THEME,
	STR_BGM_TRACK_SANCTUARY_THEME,
	STR_BGM_TRACK_SANCTUARY_BELOW_THEME,
	STR_BGM_TRACK_GECKO_SWAMP_THEME,
	STR_BGM_TRACK_GECKO_SWAMP_NIGHT_THEME,
	STR_BGM_TRACK_MISTY_TREES_THEME,
	STR_BGM_TRACK_MISTY_TREES_NIGHT_THEME,
	STR_BGM_TRACK_WARUMONZAEMON_THEME,
	STR_BGM_TRACK_TOY_TOWN_THEME,
	STR_BGM_TRACK_THEME_OF_FACTORIAL,
	STR_BGM_TRACK_FACTORIAL_NIGHT_THEME,
	STR_BGM_TRACK_SEWER_THEME,
	STR_BGM_TRACK_TRASH_MOUNTAIN_THEME,
	STR_BGM_TRACK_TRASH_MOUNTAIN_NIGHT_THEME,
	STR_BGM_TRACK_BEATLAND_THEME,
	STR_BGM_TRACK_BEATLAND_NIGHT_THEME,
	STR_BGM_TRACK_SECRET_BEACH_CAVE_THEME,
#if !VERSION_IS(US)
	STR_BGM_TRACK_MT_INFINITY,
#else
	STR_BGM_TRACK_MT_PANORAMA_THEME,
#endif
	STR_BGM_TRACK_LAST_ROOM_THEME,
	STR_BGM_TRACK_FILE_CITY_THEME,
	STR_BGM_TRACK_FILE_CITY_NIGHT_THEME,
	STR_BGM_TRACK_TORNAMENT_OPENING_THEME,
	STR_BGM_TRACK_TORNAMENT_PROGRESS_THEME,
	STR_BGM_TRACK_TORNAMENT_CHAMPIONSHIP_THEME,
	STR_BGM_TRACK_PARTNERS_ENTRANCE_THEME,
	STR_BGM_TRACK_COMPETITION_BATTLE_OPPONENTS_ENTRANCE_THEME,
	STR_BGM_TRACK_ARENA_BATTLE_THEME_NO_1,
	STR_BGM_TRACK_PARTNERS_WIN_THEME,
	STR_BGM_TRACK_PARTNERS_LOSS_THEME,
	STR_BGM_TRACK_ARENA_BATTLE_THEME_NO_2,
	STR_BGM_TRACK_ARENA_BATTLE_THEME_NO_3,
	STR_BGM_TRACK_EVENT_BATTLE_THEME,
	STR_BGM_TRACK_NORMAL_BATTLE_THEME,
	STR_BGM_TRACK_NORMAL_BATTLE_THEME_NO_2,
	STR_BGM_TRACK_LAST_BATTLE_THEME,
};

int32_t CARD_PRICES[5] = {
	0x00001388, 0x000005dc, 0x000001f4, 0x00000064,
	0x00000032,
};

CardData CARD_DATA[66] = {
	{ 0x00, 0x05, 0x0000 },
	{ 0x3b, 0x00, 0x0064 },
	{ 0x3c, 0x00, 0x0064 },
	{ 0x3d, 0x00, 0x0064 },
	{ 0x77, 0x00, 0x0064 },
	{ 0x42, 0x00, 0x0064 },
	{ 0x0c, 0x01, 0x001e },
	{ 0x0d, 0x01, 0x001e },
	{ 0x0e, 0x01, 0x001e },
	{ 0x1a, 0x01, 0x001e },
	{ 0x1b, 0x01, 0x001e },
	{ 0x1c, 0x01, 0x001e },
	{ 0x28, 0x01, 0x001e },
	{ 0x29, 0x01, 0x001e },
	{ 0x2a, 0x01, 0x001e },
	{ 0x36, 0x01, 0x001e },
	{ 0x37, 0x01, 0x001e },
	{ 0x38, 0x01, 0x001e },
	{ 0x6e, 0x01, 0x001e },
	{ 0x46, 0x01, 0x001e },
	{ 0x75, 0x01, 0x001e },
	{ 0x78, 0x01, 0x001e },
	{ 0x79, 0x01, 0x001e },
	{ 0x5c, 0x01, 0x001e },
	{ 0x7a, 0x01, 0x001e },
	{ 0x7e, 0x01, 0x001e },
	{ 0x05, 0x02, 0x000a },
	{ 0x06, 0x02, 0x000a },
	{ 0x07, 0x02, 0x000a },
	{ 0x08, 0x02, 0x000a },
	{ 0x09, 0x02, 0x000a },
	{ 0x0a, 0x02, 0x000a },
	{ 0x13, 0x02, 0x000a },
	{ 0x14, 0x02, 0x000a },
	{ 0x15, 0x02, 0x000a },
	{ 0x16, 0x02, 0x000a },
	{ 0x17, 0x02, 0x000a },
	{ 0x18, 0x02, 0x000a },
	{ 0x21, 0x02, 0x000a },
	{ 0x22, 0x02, 0x000a },
	{ 0x23, 0x02, 0x000a },
	{ 0x24, 0x02, 0x000a },
	{ 0x25, 0x02, 0x000a },
	{ 0x26, 0x02, 0x000a },
	{ 0x2f, 0x02, 0x000a },
	{ 0x30, 0x02, 0x000a },
	{ 0x31, 0x02, 0x000a },
	{ 0x32, 0x02, 0x000a },
	{ 0x33, 0x02, 0x000a },
	{ 0x34, 0x02, 0x000a },
	{ 0x3a, 0x02, 0x000a },
	{ 0x39, 0x03, 0x0005 },
	{ 0x6d, 0x03, 0x0005 },
	{ 0x6f, 0x03, 0x0005 },
	{ 0x43, 0x03, 0x0005 },
	{ 0x44, 0x03, 0x0005 },
	{ 0x45, 0x03, 0x0005 },
	{ 0x54, 0x03, 0x0005 },
	{ 0x7f, 0x03, 0x0005 },
	{ 0x4c, 0x03, 0x0005 },
	{ 0x50, 0x03, 0x0005 },
	{ 0x0b, 0x04, 0x0001 },
	{ 0x19, 0x04, 0x0001 },
	{ 0x27, 0x04, 0x0001 },
	{ 0x35, 0x04, 0x0001 },
	{ 0x00, 0x05, 0x0000 },
};

char *TOURNAMENT_NAMES[23] = {
	STR_TOURNAMENT_NAME_GRADE_D,
	STR_TOURNAMENT_NAME_GRADE_C,
	STR_TOURNAMENT_NAME_GRADE_B,
	STR_TOURNAMENT_NAME_GRADE_A,
	STR_TOURNAMENT_NAME_GRADE_S,
	STR_TOURNAMENT_NAME_GRADE_R,
	STR_TOURNAMENT_NAME_VERSION_1_CUP,
	STR_TOURNAMENT_NAME_VERSION_2_CUP,
	STR_TOURNAMENT_NAME_VERSION_3_CUP,
	STR_TOURNAMENT_NAME_VERSION_4_CUP,
	STR_TOURNAMENT_NAME_VERSION_0_CUP,
	STR_TOURNAMENT_NAME_FIRE_CUP,
	STR_TOURNAMENT_NAME_GRAPPLE_CUP,
	STR_TOURNAMENT_NAME_THUNDER_WIND_CUP,
	STR_TOURNAMENT_NAME_COOL_CUP,
	STR_TOURNAMENT_NAME_NATURE_CUP,
	STR_TOURNAMENT_NAME_METALIC_CUP,
	STR_TOURNAMENT_NAME_FILTH_CUP,
	STR_TOURNAMENT_NAME_DINO_CUP,
	STR_TOURNAMENT_NAME_WING_CUP,
	STR_TOURNAMENT_NAME_ANIMAL_CUP,
	STR_TOURNAMENT_NAME_HUMAN_CUP,
	STR_TOURNAMENT_NAME_BEETLE_CUP,
};

char *TOURNAMENT_GRADES[23] = {
#if !VERSION_IS(US)
	STR_TOURNAMENT_GRADE_D,
	STR_TOURNAMENT_GRADE_C,
	STR_TOURNAMENT_GRADE_B,
	STR_TOURNAMENT_GRADE_A,
	STR_TOURNAMENT_GRADE_S,
	STR_TOURNAMENT_GRADE_R,
	STR_TOURNAMENT_GRADE_V1,
	STR_TOURNAMENT_GRADE_V2,
	STR_TOURNAMENT_GRADE_V3,
	STR_TOURNAMENT_GRADE_V4,
	STR_TOURNAMENT_GRADE_VO,
	STR_TOURNAMENT_GRADE_FR,
	STR_TOURNAMENT_GRADE_GP,
	STR_TOURNAMENT_GRADE_TW,
	STR_TOURNAMENT_GRADE_CO,
	STR_TOURNAMENT_GRADE_NT,
	STR_TOURNAMENT_GRADE_MT,
	STR_TOURNAMENT_GRADE_DT,
	STR_TOURNAMENT_GRADE_DY,
	STR_TOURNAMENT_GRADE_WI,
	STR_TOURNAMENT_GRADE_AN,
	STR_TOURNAMENT_GRADE_HU,
	STR_TOURNAMENT_GRADE_BT,
#else
	STR_TOURNAMENT_GRADE_D,
	STR_TOURNAMENT_GRADE_C,
	STR_TOURNAMENT_GRADE_B,
	STR_TOURNAMENT_GRADE_A,
	STR_TOURNAMENT_GRADE_S,
	STR_TOURNAMENT_GRADE_R,
	STR_TOURNAMENT_GRADE_H,
	STR_TOURNAMENT_GRADE_I,
	STR_TOURNAMENT_GRADE_J,
	STR_TOURNAMENT_GRADE_K,
	STR_TOURNAMENT_GRADE_L,
	STR_TOURNAMENT_GRADE_F,
	STR_TOURNAMENT_GRADE_G,
	STR_TOURNAMENT_GRADE_W,
	STR_TOURNAMENT_GRADE_O,
	STR_TOURNAMENT_GRADE_N,
	STR_TOURNAMENT_GRADE_M,
	STR_TOURNAMENT_GRADE_T,
	STR_TOURNAMENT_GRADE_Y,
	STR_TOURNAMENT_GRADE_W,
	STR_TOURNAMENT_GRADE_Z,
	STR_TOURNAMENT_GRADE_X,
	STR_TOURNAMENT_GRADE_Q,
#endif
};

uint8_t TOURNAMENT_SCHEDULE[180] = {
	0x00, 0x01, 0x02, 0x03, 0x12, 0xff, 0x00, 0x01,
	0x02, 0x0b, 0xff, 0xff, 0x00, 0x01, 0x03, 0x06,
	0xff, 0xff, 0x00, 0x01, 0x02, 0x0c, 0xff, 0xff,
	0x00, 0x02, 0x03, 0x07, 0xff, 0xff, 0x01, 0x0d,
	0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x02, 0x03,
	0x04, 0x08, 0x00, 0x01, 0x02, 0x0f, 0xff, 0xff,
	0x00, 0x01, 0x03, 0x09, 0xff, 0xff, 0x00, 0x02,
	0x13, 0xff, 0xff, 0xff, 0x00, 0x01, 0x02, 0x03,
	0x0e, 0xff, 0x01, 0x0a, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x01, 0x02, 0x03, 0x10, 0xff, 0x00, 0x01,
	0x02, 0x04, 0x11, 0xff, 0x00, 0x03, 0xff, 0xff,
	0xff, 0xff, 0x00, 0x01, 0x02, 0x0b, 0xff, 0xff,
	0x00, 0x01, 0x02, 0x03, 0x06, 0xff, 0x01, 0x0c,
	0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x02, 0x03,
	0x07, 0xff, 0x00, 0x02, 0x14, 0xff, 0xff, 0xff,
	0x00, 0x01, 0x03, 0x04, 0x0d, 0xff, 0x00, 0x01,
	0x02, 0x08, 0xff, 0xff, 0x00, 0x01, 0x02, 0x03,
	0x0f, 0xff, 0x01, 0x09, 0xff, 0xff, 0xff, 0xff,
	0x00, 0x02, 0x03, 0x15, 0xff, 0xff, 0x00, 0x01,
	0x02, 0x0e, 0xff, 0xff, 0x00, 0x01, 0x03, 0x0a,
	0xff, 0xff, 0x00, 0x01, 0x02, 0x04, 0x10, 0xff,
	0x00, 0x01, 0x02, 0x03, 0x11, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff,
};

BattleEntry BIRDRA_TRANSPORT_TARGETS[6] = {
	{ 0x26, 0x09, 0x00dd, 0x000003e8 },
	{ 0x46, 0x09, 0x00be, 0x000003e8 },
#if VERSION_IS(JP)
	{ 0x4f, 0x09, 0x0051, 0x000005dc },
#else
	{ 0x4f, 0x09, 0x00bc, 0x000005dc },
#endif
	{ 0x5d, 0x09, 0x015f, 0x000007d0 },
	{ 0x77, 0x09, 0x0093, 0x000009c4 },
	{ 0x69, 0x00, 0x00d2, 0x000009c4 },
};

int16_t ITEM_MENU_POS[32] = {
	0xffc9, 0xff9e, 0x00be, 0x0081, 0xffb9, 0xff9e, 0x00de, 0x0081,
	0xffb9, 0xff9e, 0x00de, 0x0081, 0xffc9, 0xff9e, 0x00be, 0x0081,
	0xffb9, 0xff9e, 0x00de, 0x0081, 0xffdc, 0xff9e, 0x00a6, 0x0081,
	0xffdc, 0xff9e, 0x00a6, 0x0081, 0xffc9, 0xff9e, 0x00be, 0x0081,
};

int16_t ITEM_MENU_DESCRIPTION_RECTS[32] = {
	5, 17, 170, 18, 5, 17, 202, 18, 5, 17, 202, 18, 5, 17, 170, 18,
	5, 17, 202, 18, 5, 17, 146, 18, 5, 17, 146, 18, 5, 17, 170, 18,
};

int16_t SELECTION_CURSOR_WIDTHS[8] = {
	0x00aa, 0x00ca, 0x00ca, 0x00aa, 0x00ca, 0x0092, 0x0092, 0x00aa,
};
// clang-format on

int16_t MONOCHROMON_BUBBLE_TIMER;
uint32_t ARRAY_SECTION_OFFSET;
ItemMenuBox *ITEM_MENU_LEFT;
ItemMenuBox *ITEM_MENU_RIGHT;
int32_t SHOP_ACTION_SELECTED;
int32_t MAIN_D_80134F74;
uint8_t SHOP_ITEM_TYPE;
int32_t SHOP_ITEM_PRICE;
uint8_t MAX_SHOP_AMOUNT;
uint8_t SHOP_AMOUNT;
uint8_t NAMING_CURRENT_LETTER;
int32_t UPDATE_SHOP_BIT_BOX;
int32_t BIT_BOX_SHOW_BITS;
uint16_t NAMING_SELECTOR;
uint8_t NAMING_BOX_FLAG;
uint8_t MAIN_D_80134F8F;
uint8_t MAIN_D_80134F90;

static void *script_common_sbss_order[] = {
	&MAIN_D_80134F90,
	&MAIN_D_80134F8F,
	&NAMING_BOX_FLAG,
	&NAMING_SELECTOR,
	&BIT_BOX_SHOW_BITS,
	&UPDATE_SHOP_BIT_BOX,
#if VERSION_IS(EU)
	&SHOP_ITEM_PRICE,
	&MAX_SHOP_AMOUNT,
	&SHOP_AMOUNT,
	&NAMING_CURRENT_LETTER,
	&SHOP_ITEM_TYPE,
	&SHOP_ACTION_SELECTED,
	&MAIN_D_80134F74,
	&ITEM_MENU_RIGHT,
	&ITEM_MENU_LEFT,
	&MONOCHROMON_BUBBLE_TIMER,
	&ARRAY_SECTION_OFFSET,
#else
	&NAMING_CURRENT_LETTER,
	&SHOP_AMOUNT,
	&MAX_SHOP_AMOUNT,
	&SHOP_ITEM_PRICE,
	&SHOP_ITEM_TYPE,
	&MAIN_D_80134F74,
	&SHOP_ACTION_SELECTED,
	&ITEM_MENU_RIGHT,
	&ITEM_MENU_LEFT,
	&ARRAY_SECTION_OFFSET,
	&MONOCHROMON_BUBBLE_TIMER,
#endif
};

char NAMING_BUFFER[20];

int32_t shopFillBuyItemList()
{
	uint8_t itemId;
	uint8_t slotId;
	int32_t canBuyAnything;
	int32_t hasSpace;
	uint8_t *itemList;
	uint8_t inventorySize;

	canBuyAnything = 0;
	hasSpace = 0;
	inventorySize = INVENTORY.size;
	itemList = ITEM_MENU_LEFT->buf;
	ITEM_MENU_LEFT->itemCount = 0;

	for (slotId = 0; slotId < inventorySize; slotId++) {
		if (INVENTORY.types.array[slotId] == 0xff) {
			hasSpace = 1;

			break;
		}
	}

	for (itemId = 0; itemId < 128; itemId++) {
		if (!isTriggerSet(0x180 + itemId)) {
			continue;
		}

		ITEM_MENU_LEFT->itemCount++;
		*itemList++ = itemId;

		if (ITEM_PARA[itemId].value > MONEY) {
			*itemList++ = 0;

			continue;
		}

		if (hasSpace) {
			for (slotId = 0; slotId < inventorySize; slotId++) {
				if (INVENTORY.types.array[slotId] == itemId &&
				    INVENTORY.amounts.array[slotId] == 99) {
					*itemList++ = 0;

					goto end;
				}
			}
			*itemList++ = 1;

			canBuyAnything = 1;

		} else {
			for (slotId = 0; slotId < inventorySize; slotId++) {
				if (INVENTORY.types.array[slotId] == itemId &&
				    INVENTORY.amounts.array[slotId] != 99) {
					*itemList++ = 1;

					canBuyAnything = 1;
					goto end;
				}
			}
			*itemList++ = 0;
		}

end:
		continue;
	}

	return canBuyAnything;
}

int32_t shopFillSellItemList(void)
{
	int32_t i;
	uint8_t *buf;
	int32_t result;
	uint8_t type;
	uint8_t amount;

	result = 0;
	buf = ITEM_MENU_RIGHT->buf;
	ITEM_MENU_RIGHT->itemCount = 0;

	for (i = 0; i < INVENTORY.size; i++) {
		type = INVENTORY.types.array[i];
		amount = INVENTORY.amounts.array[i];

		if (type != 0xff) {
			if (ITEM_PARA[type].droppable != 0) {
				amount |= 0x80;
				result = 1;
			}
		} else {
			type = 0xff;
		}

		ITEM_MENU_RIGHT->itemCount++;
		*buf++ = type;
		*buf++ = amount;
	}

	return result;
}

void tickItemMenu(void)
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

	if (SCRIPT_STATE_2 != 1) {
		return;
	}

	if (isKeyDown(CONFIRM_BUTTON)) {
		src = &ITEM_MENU_DESCRIPTION_RECTS[ITEM_MENU_TYPE * 4];
		setRECT(&rect, src[0], src[1], src[2], src[3]);

		if (ITEM_MENU_TYPE != 5) {
			if (ITEM_MENU_TYPE != 7) {
				createItemMenuAmountBox(&rect);
			} else {
				readSelectedItemMerit();
			}
		} else {
			createSingleCardShopMenu(&rect);
		}
	} else if (isKeyDown(CANCEL_BUTTON)) {
		if (ITEM_MENU_TYPE == 7) {
			SCRIPT_STATE_2 = 0xa;
		} else if (ITEM_MENU_TYPE == 5) {
			if (isTriggerSet(3) != 0) {
				writePStat(0xfe, 0xff);
				unsetTrigger(3);
				SCRIPT_STATE_2 = 4;
			}
		} else {
			SCRIPT_STATE_2 = 8;
		}

		playSound(0, 4);
	} else if (isKeyDown(0x1000)) {
		if (POLLED_INPUT & 8) {
			itemMenuCursorTop(box, 9, 0);
		} else {
			itemMenuCursorUp(box, 0);
		}
	} else if (isKeyDown(0x4000)) {
		if (POLLED_INPUT & 8) {
			itemMenuCursorBottom(box, 9, 0);
		} else {
			itemMenuCursorDown(box, 0);
		}
	} else if (isKeyDown(0x800)) {
		src = &ITEM_MENU_DESCRIPTION_RECTS[ITEM_MENU_TYPE * 4];
		setRECT(&rect, src[0], src[1], src[2], src[3]);
		createItemMenuDescriptionBox(box, &rect, 1);
		playSound(0, 3);
	}
}

int32_t readSelectedItemMerit(void)
{
	ItemMenuBox *box;
	int32_t idx;
	uint8_t amount;

	box = getItemMenuFromType();
	idx = (box->topRow + box->cursor) * 2;
	SHOP_ITEM_TYPE = box->buf[idx];

	if (SHOP_ITEM_TYPE != 0xff) {
		amount = box->buf[idx + 1];
		if (amount != 0) {
			SHOP_VARIABLE = ITEM_PARA[SHOP_ITEM_TYPE].meritValue;
			SCRIPT_STATE_2 = 0xb;
			SCRIPT_TEXTBOX_MODE = 0;
			playSound(0, 3);
			return 1;
		}
	}

	playSound(0, 0xb);

	return 0;
}

void tickItemMenuDescriptionBox(void)
{
	if (UI_BOX_DATA[3].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (isKeyDown(CONFIRM_BUTTON | CANCEL_BUTTON | 0x800) == 0) {
		return;
	}

	triggerBoxCloseFlag(3);
	playSound(0, 3);
}

void renderItemMenuDescriptionBox(void)
{
	int16_t rowPx;
	int16_t x;
	int16_t y;

	rowPx = TEXTBOX_DATA.box[3].vramRow * 12;
	x = UI_BOX_DATA[3].finalPos.x + 6;
	y = UI_BOX_DATA[3].finalPos.y + 5;
#if VERSION_IS(EU)
	renderString(0, x, y, 0xfc, 0x18, 0, rowPx, 3, 1);
#else
	renderString(0, x, y, 0xfc, 0xc, 0, rowPx, 3, 1);
#endif
}

void renderItemMenuAmountBox(void)
{
	AmountBoxLayout layout;
	MenuTextField *entry;
	int32_t i;
	int16_t x;
	int16_t y;
	int16_t sx;
	int16_t sy;
	int16_t rowPx;
	int16_t srcCol;

	layout = AMOUNT_BOX_LAYOUT;
	rowPx = TEXTBOX_DATA.box[3].vramRow * 12;
	x = UI_BOX_DATA[3].finalPos.x;
	y = UI_BOX_DATA[3].finalPos.y;
#if VERSION_IS(EU)
	renderHorizontalLine(3, 4, 0x17, 0x82);
	renderHorizontalLine(3, 0xc, 0x3c, 0x72);
#else
	renderHorizontalLine(3, 4, 0x17, 0x7a);
	renderHorizontalLine(3, 0xc, 0x3c, 0x6a);
#endif
	renderInsetWithoutBox(3, 0x55, 0x2a, 0x1a, 0xe);
	sx = x + 8;
	sy = y + 5;

	if (ITEM_MENU_TYPE < 3) {
		renderItemSprite(SHOP_ITEM_TYPE, sx, sy, 3);
	} else {
		renderCardSprite(CARD_DATA[SHOP_ITEM_TYPE].spriteId, sx + 2, sy + 2, 3);
	}

	entry = layout.v;
	i = 0;
	srcCol = 0;
	while (i < 4) {
		renderString(0, entry->x + x, entry->y + y, entry->chars * GLYPH_WIDTH, 0xc, srcCol * GLYPH_WIDTH, rowPx, 3, 1);
		srcCol += entry->chars;
		i++;
		entry += 1;
	}
#if !VERSION_IS(US)
	renderString(0, entry->x + x, entry->y + y, entry->chars * GLYPH_WIDTH, 0xc, 12 * GLYPH_WIDTH, 0x60, 3, 1);
#endif
}

void renderItemMenu(void)
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
	if (ITEM_MENU_TYPE != 5) {
		if (ITEM_MENU_TYPE != 7) {
			renderItemMenuSprite(1, 1, bx + 0x80, by + 5);
			if (ITEM_MENU_TYPE != 0) {
				renderItemMenuSprite(1, 3, bx + 0xb6, by + 5);
			}
		} else {
			renderItemMenuSprite(1, 2, bx + 0x80, by + 5);
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
	renderItemMenuItemList(box, x, y, x2, y2, 0);
}

void updateItemMenuAmountBoxString(void)
{
	uint8_t *out;
	uint32_t total;
	uint8_t id;
	uint32_t len;

	out = TEXTBOX_LINES_PTR + ITEM_MENU_SUB_TEXTBOX_LINE * TEXTBOX_LINE_SIZE;

	if (ITEM_MENU_TYPE < 3) {
		strcpy(out, ITEM_NAME(SHOP_ITEM_TYPE));
		len = strlen(ITEM_NAME(SHOP_ITEM_TYPE));
	} else {
		id = CARD_DATA[SHOP_ITEM_TYPE].digimonId;
		strcpy(out, DIGIMON_NAME(id));
		len = strlen(DIGIMON_NAME(id));
	}

	out += len;
#if !VERSION_IS(US)
	*out++ = 0xc;
	*out++ = 0;
	out = intToStringSJIS(out, SHOP_ITEM_PRICE, 5, 0);
	out = intToStringSJIS(out, SHOP_AMOUNT, 2, 0);
#else
	*out++ = 0x18;
	*out++ = 0;
	out = intToStringSJIS(out, SHOP_ITEM_PRICE, 5, 0);
	*out++ = 0x19;
	*out++ = 0;
	out = intToStringSJIS(out, SHOP_AMOUNT, 2, 0);
	*out++ = 0x1a;
	*out++ = 0;
#endif

	total = SHOP_ITEM_PRICE * SHOP_AMOUNT;
	if (total >= 0xf4240) {
		total = 0xf423f;
	}

	out = intToStringSJIS(out, total, 6, 0);
	*out++ = 0;
	*out = 0;
	TEXTBOX_DATA.box[3].pageReady = 1;
	TEXTBOX_DATA.box[3].writeCount++;
}

void tickSingleCardShop(void)
{
	ItemMenuBox *box;
	uint8_t amount;

	if (UI_BOX_DATA[3].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (isKeyDown(CANCEL_BUTTON)) {
		triggerBoxCloseFlag(3);
		playSound(0, 4);
		return;
	}

	if (isKeyDown(0x8000)) {
		SHOP_AMOUNT = 0;
		playSound(0, 2);
		return;
	}

	if (isKeyDown(0x2000)) {
		SHOP_AMOUNT = 1;
		playSound(0, 2);
		return;
	}

	if (isKeyDown(CONFIRM_BUTTON)) {
		triggerBoxCloseFlag(3);
		if (SHOP_AMOUNT == 0) {
			if (ITEM_MENU_TYPE != 5) {
				box = getItemMenuFromType();
				amount = getCardAmount(SHOP_ITEM_TYPE);
				amount++;
				setCardAmount(SHOP_ITEM_TYPE, amount);
				amount = CARD_DATA[SHOP_ITEM_TYPE].spriteId;
				MONEY -= CARD_PRICES[amount];
				SCRIPT_STATE_PTR->smth[box->cursor] = 0xff;
				UPDATE_SHOP_BIT_BOX = 1;
				SCRIPT_STATE_2 = 4;
				playShopSoundOnlyInSavannah();
			} else {
				writePStat(0xfe, SHOP_ITEM_TYPE);
				unsetTrigger(3);
				SCRIPT_STATE_2 = 4;
				playSound(0, 3);
			}
		} else {
			playSound(0, 4);
		}
	}
}

void renderSingleCardShop(void)
{
	ConfirmBoxLayout layout;
	MenuTextSprite *entry;
	int32_t i;
	int32_t unused;
	int16_t rowPx;
	int16_t x;
	int16_t y;

	layout = CONFIRM_BOX_LAYOUT;
	rowPx = TEXTBOX_DATA.box[3].vramRow * 12;
	x = UI_BOX_DATA[3].finalPos.x + 4;
	y = UI_BOX_DATA[3].finalPos.y + 3;
	entry = layout.v;
	i = 0;
	unused = 0;
	while (i < 3) {
		renderString(0, x + entry->x, y + entry->y, entry->width, 0xc,
		             entry->srcX, rowPx + entry->srcY, 3, 1);
		i++;
		entry += 1;
	}
	renderSelectionCursor(x + 8 + SHOP_AMOUNT * 47, y + 0x12, 0x28,
	                      0xe, 3);
}

void tickShopBitsBox(void)
{
	int32_t line;
	uint8_t saved;

	if (UPDATE_SHOP_BIT_BOX != 0) {
		UPDATE_SHOP_BIT_BOX = 0;

		if (BIT_BOX_SHOW_BITS != 0) {
			line = 6;
		} else {
			line = 7;
		}

		saved = ACTIVE_INSTRUCTION;
		showShopkeeperTextbox(line, 0xff, 2);
		ACTIVE_INSTRUCTION = saved;
	}
}

uint8_t *getShopkeeperLine(int32_t idx)
{
	ShopkeeperIdTable shopkeeperIds;
	uint16_t section;
	uint8_t pstat;
	int32_t i;

	shopkeeperIds = SHOPKEEP_DIGIMON_TYPES;
	pstat = readPStat(0xfe);

	if (pstat == 0xff) {
		section = 0x4ce;
	} else {
		pstat = scriptIdToEntityId(pstat);
		if (pstat != 0xff) {
			pstat = *(int32_t *)(&NPC_ENTITIES[pstat - 2]);
			for (i = 0; (uint32_t)i < 0xb; i++) {
				if (pstat == shopkeeperIds.b[i]) {
					section = i + 0x4c4;
					goto done;
				}
			}
		}
		section = 0x4c4;
	}
done:
	return resolveMapHeadEntry(section, idx);
}

// clang-format off
uint8_t *resolveMapHeadEntry(section, idx)
	uint16_t section;
	int32_t idx;
// clang-format on
{
	uint8_t *script;
	uint8_t *ptr;

	script = getScript(0);
	ptr = getScriptSection(script, section);
	ptr = ptr + idx * 4 + 2;
	ptr = script + *(uint16_t *)ptr + 2;

	return ptr;
}

void renderMonochromonMoodBubble(int32_t instanceId)
{
	SVECTOR pos;
	int32_t depth;
	DVECTOR screen;
	uint8_t entityId;
	int16_t offset;
	int16_t scale;
	uint8_t mood;

	entityId = readPStat(0xf7);
	mood = readPStat(0xf8);
	entityId = scriptIdToEntityId(entityId);
	if (entityId == 0xff) {
		return;
	}

	translateConditionFXToEntity(ENTITY_TABLE[entityId], &pos);
	depth = worldPosToScreenPos(&pos, &screen);

	offset = 0x10 - (MONOCHROMON_BUBBLE_TIMER >> 1);
	scale = offset << 8;
	MONOCHROMON_BUBBLE_SPRITE.x = screen.vx;
	MONOCHROMON_BUBBLE_SPRITE.y = screen.vy - offset;
	MONOCHROMON_BUBBLE_SPRITE.scalex = scale;
	MONOCHROMON_BUBBLE_SPRITE.scaley = scale;
	GsSortSprite(&MONOCHROMON_BUBBLE_SPRITE, ACTIVE_ORDERING_TABLE, depth >> 4);

	if (MONOCHROMON_BUBBLE_TIMER != 0) {
		MONOCHROMON_BUBBLE_TIMER--;
	}
}

void inputInit(void)
{
	INPUT_PENDING_MASK = 0;
	INPUT_FRESH_MASK = -1;
	INPUT_REPEAT_MASK = 0;
	INPUT_REPEAT_COUNTER = 0;
}

void processInput(void)
{
	uint32_t held;
	uint32_t prev;
	uint32_t released;

	released = ~POLLED_INPUT;
	INPUT_FRESH_MASK |= released;
	held = POLLED_INPUT & INPUT_REPEAT_MASK;
	if (held == 0) {
		INPUT_REPEAT_COUNTER = 0;
	}

	if (INPUT_REPEAT_MASK != 0) {
		prev = POLLED_INPUT_PREVIOUS & INPUT_REPEAT_MASK;
		if (held == prev) {
			if (INPUT_REPEAT_COUNTER == 8) {
				INPUT_REPEAT_COUNTER = 6;
			} else if (INPUT_REPEAT_COUNTER != 6) {
				held = 0;
			}

			INPUT_REPEAT_COUNTER += 1;
		} else {
			INPUT_REPEAT_COUNTER = 0;
			INPUT_FRESH_MASK |= INPUT_REPEAT_MASK;
			held = 0;
		}
	}

	INPUT_PENDING_MASK = held | (POLLED_INPUT & INPUT_FRESH_MASK);
}

int32_t isKeyDown(uint32_t key)
{
	if ((INPUT_PENDING_MASK & key) == 0) {
		return 0;
	}

	INPUT_FRESH_MASK &= ~key;

	return 1;
}

void setFreshDialogue(void)
{
	MAIN_D_80135028 = 1;
}

int32_t isXPressedAfterDialogue(void)
{
	if (MAIN_D_80135028 != 0) {
		if ((POLLED_INPUT & CONFIRM_BUTTON) != 0) {
			return 0;
		}

		MAIN_D_80135028 = 0;
	}
	return 1;
}

void setInputRepeatMask(uint32_t mask)
{
	INPUT_REPEAT_MASK = mask;
}

void handleItemLoss(void)
{
	uint8_t *pool;
	uint8_t k;
	int32_t count;
	int32_t lose;
	uint8_t n;
	uint8_t slot;
	uint8_t recycleId;
	uint8_t size;

	size = INVENTORY.size;
	pool = allocateArray(size);
	k = 0;
	count = 0;
	while (k < size) {
		if (INVENTORY.types.array[k] != 0xff) {
			pool[count] = k;
			count += 1;
		}
		k++;
	}

	lose = count * 30 / 100;
	writePStat(0xc8, lose);
	if (lose == 0) {
		freeArray((uint32_t *)pool);
		return;
	}

	for (n = 0; n < lose; n++) {
		k = 0;
		count = 0;
		while (k < size) {
			if (INVENTORY.types.array[k] != 0xff) {
				pool[count] = k;
				count += 1;
			}
			k++;
		}

		slot = pool[randomLimit(count)];
		recycleId = INVENTORY.types.array[slot];
		recycleId = getRecycleId(recycleId);
		if (recycleId != 0xff) {
			SCRIPT_STATE_PTR->smth[recycleId + 6] +=
				INVENTORY.amounts.array[slot];
			if (SCRIPT_STATE_PTR->smth[recycleId + 6] >= 0x64) {
				SCRIPT_STATE_PTR->smth[recycleId + 6] = 0x63;
			}
		}

		recycleId = INVENTORY.types.array[slot];
		removeItem(recycleId, 0x63);
	}

	freeArray((uint32_t *)pool);
}

uint8_t getRecycleId(uint8_t value)
{
	uint8_t i;

	for (i = 0; i < 0x4e; i++) {
		if (value == RECYCLABLE_ITEMS[i]) {
			return i;
		}
	}

	return 0xff;
}

void dailyPStatTrigger(void)
{
	ScriptState *st;
	int32_t i;
	int32_t j;
	uint8_t r;
	uint8_t v;

	st = SCRIPT_STATE_PTR;
	for (i = 0; i < 6; i++) {
		st->smth[i] = 0xff;
	}
	for (i = 0; i < 6; i++) {
retry:
		r = randomLimit(0x40);
		r++;
		if (r == 4) {
			goto retry;
		}
		for (j = 0; j < 6; j++) {
			if (r == st->smth[i]) {
				goto retry;
			}
		}
		st->smth[i] = r;
	}
	for (i = 0x1c; i < 0x20; i++) {
		v = readPStat(i);
		if (v != 0xff) {
			v++;
			writePStat((uint8_t)i, v);
		}
	}
	v = readPStat(2);
	if (v != 0xff) {
		v &= 0x7f;
		writePStat(2, v);
	}
}

int32_t isPartnerBaby(void)
{
	BabyTypeTable babyTypes;
	int32_t i;
	uint8_t partnerType;

	babyTypes = BABY_IDS;
	partnerType = PARTNER_ENTITY.digimonEntity.entity.type;

	for (i = 0; (uint32_t)i < 8; i++) {
		if (partnerType == babyTypes.b[i]) {
			return 1;
		}
	}

	return 0;
}

void tickItemMenuAmountBox(void)
{
	RECT origin;
	uint8_t owner;
	int8_t q;
	int32_t unitPrice;

	if (UI_BOX_DATA[3].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (SHOP_AMOUNT != MAX_SHOP_AMOUNT && isKeyDown(0x1000)) {
		if (POLLED_INPUT & ALT_BUTTON) {
			SHOP_AMOUNT += 10;
			if (SHOP_AMOUNT > MAX_SHOP_AMOUNT) {
				SHOP_AMOUNT -= 10;
			}
		} else {
			SHOP_AMOUNT += 1;
		}

		playSound(0, 2);
		updateItemMenuAmountBoxString();

		return;
	}
	if (SHOP_AMOUNT != 1 && isKeyDown(0x4000)) {
		if (POLLED_INPUT & ALT_BUTTON) {
			q = SHOP_AMOUNT;
			q -= 10;
			if (q <= 0) {
				q += 10;
			}
			SHOP_AMOUNT = q;
		} else {
			SHOP_AMOUNT -= 1;
		}

		playSound(0, 2);
		updateItemMenuAmountBoxString();

		return;
	}

	if (isKeyDown(CANCEL_BUTTON)) {
		triggerBoxCloseFlag(3);
		playSound(0, 4);
	} else if (isKeyDown(0x80)) {
		SHOP_AMOUNT = MAX_SHOP_AMOUNT;
		playSound(0, 3);
	} else if (isKeyDown(CONFIRM_BUTTON)) {
		if (ITEM_MENU_TYPE < 3) {
			if (ITEM_MENU_TYPE == 1) {
				MONEY += SHOP_AMOUNT * SHOP_ITEM_PRICE;
				if (MONEY >= 0xf4240) {
					MONEY = 0xf423f;
				}

				removeItem(SHOP_ITEM_TYPE, SHOP_AMOUNT);
				owner = readPStat(0xfe);
			} else {
				unitPrice = SHOP_ITEM_PRICE;
				if (isPartnerBaby() != 0) {
					unitPrice = unitPrice * 90 / 100;
					SHOP_VARIABLE = (int16_t)SHOP_AMOUNT * (SHOP_ITEM_PRICE - unitPrice);
				}

				MONEY -= SHOP_AMOUNT * unitPrice;
				giveItem(SHOP_ITEM_TYPE, SHOP_AMOUNT);

				if (ITEM_MENU_TYPE == 2) {
					owner = getRecycleId(SHOP_ITEM_TYPE);
					SCRIPT_STATE_PTR->smth[owner + 6] -= SHOP_AMOUNT;
				}

				owner = 0xfd;
			}
			SCRIPT_STATE_2 = 7;
		} else {
			MONEY += SHOP_AMOUNT * SHOP_ITEM_PRICE;
			if (MONEY >= 0xf4240) {
				MONEY = 0xf423f;
			}

			owner = getCardAmount(SHOP_ITEM_TYPE);
			owner -= SHOP_AMOUNT;
			setCardAmount(SHOP_ITEM_TYPE, owner);
			owner = readPStat(0xfe);
			SCRIPT_STATE_2 = 4;
		}

		setupBoxOrigin(owner, &origin);
		triggerBoxCloseFlag(3);
		closeTextbox(3, &origin);
		UPDATE_SHOP_BIT_BOX = 1;
		playShopSoundOnlyInSavannah();
	}

	updateItemMenuAmountBoxString();
}

void calculateItemListStrings(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	uint8_t item;
	int32_t idx;
	uint32_t len;

	out = getTextboxLine(box, row);
	*out++ = 1;
	*out++ = 7;

	idx = ITEM_MENU_TYPE + ((ITEM_MENU_LEFT->topRow + 5) + row);
	if (isTriggerSet(idx) != 0) {
#if !VERSION_IS(US)
		*out++ = 0x82;
		*out++ = 0x85;
#else
		*out++ = 0x81;
		*out++ = 0x7c;
#endif
	} else {
		*out++ = 0x81;
		*out++ = 0x40;
	}

	idx = (box->topRow + row) * 2;
	item = ITEM_MENU_LEFT->buf[idx];
	*out++ = 1;

	if ((item & 0x80) != 0) {
		*out++ = 1;
	} else {
		*out++ = 3;
	}

	item &= 0x7f;
	strcpy(out, ITEM_NAME(item));
	len = strlen(ITEM_NAME(item));
	out += len;
#if VERSION_IS(EU)
	out = padWithSpaces(out, 13, len);
#else
	out = padWithSpaces(out, 8, len);
#endif
	*out++ = 0xf;
	*out++ = 0;
	*out++ = 1;
	*out++ = 1;

	item = ITEM_MENU_RIGHT->buf[idx];
	strcpy(out, ITEM_NAME(item));
	len = strlen(ITEM_NAME(item));
	out += len;
	terminateString(out, isLast);
}

void openShop(void)
{
	uint8_t npcId;
	int32_t i;
	int32_t found;

	npcId = readPStat(0xfe);

	switch (SCRIPT_STATE_2) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0x100, 6, 0xb2, 0x18,
		                    6, 0x5a);
		allocateItemMenuBox(&ITEM_MENU_RIGHT,
		                    INVENTORY.size << 1, 6, 0xd2, 0x18,
		                    6, 0x5a);
		SHOP_ACTION_SELECTED = 0;

		found = 0;
		for (i = 0; i < 0x80; i++) {
			if (isTriggerSet(i + 0x180)) {
				found = 1;
				break;
			}
		}

		MAIN_D_80134F74 = 0;
		if (found) {
			showShopkeeperTextbox(0, npcId, 0);
			SCRIPT_STATE_2 = 1;
			SCRIPT_NEXT_STATE_2 = 3;
			SCRIPT_TEXTBOX_MODE = 1;
		} else {
			showShopkeeperTextbox(1, npcId, 0);
			SCRIPT_STATE_2 = 1;
			SCRIPT_NEXT_STATE_2 = 2;
			SCRIPT_TEXTBOX_MODE = 1;
		}
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&ITEM_MENU_RIGHT);
		destroyItemMenuBox(&ITEM_MENU_LEFT);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		createShopBitsBox(1);
		showShopkeepSelection(2, 0xfd, 3, &SHOP_ACTION_SELECTED);
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 4;
		SCRIPT_TEXTBOX_MODE = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 0;
		shopFillBuyItemList();
		createItemMenu();
		showShopkeeperTextbox(8, npcId, 0);
		SCRIPT_STATE_2 = 1;
		SCRIPT_TEXTBOX_MODE = 3;
		break;
	case 5:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 1;
		shopFillSellItemList();
		createItemMenu();
		showShopkeeperTextbox(9, npcId, 0);
		SCRIPT_STATE_2 = 1;
		SCRIPT_TEXTBOX_MODE = 3;
		break;
	case 6:
		triggerBoxCloseFlag(2);

		if (MAIN_D_80134F74) {
			showShopkeeperTextbox(4, npcId, 0);
		} else {
			showShopkeeperTextbox(5, npcId, 0);
		}

		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 2;
		SCRIPT_TEXTBOX_MODE = 1;
		break;
	case 7:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);

		if (ITEM_MENU_TYPE == 0 && isPartnerBaby()) {
			showShopkeeperTextbox(0xd, npcId, 0);
		} else {
			showShopkeeperTextbox(0xa, npcId, 0);
		}

		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 3;
		SCRIPT_TEXTBOX_MODE = 1;
		MAIN_D_80134F74 = 1;
		break;
	case 8:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		showShopkeeperTextbox(0xb, npcId, 0);
		SCRIPT_STATE_2 = 9;
		break;
	case 9:
		SCRIPT_STATE_2 = 1;
		SCRIPT_NEXT_STATE_2 = 3;
		SCRIPT_TEXTBOX_MODE = 1;
		break;
	}
}

void allocateItemMenuBox(ItemMenuBox **box, int32_t bufSize, uint8_t rows, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	*box = (ItemMenuBox *)allocateArray(0x20);
	(*box)->buf = allocateArray(bufSize);
	(*box)->isOpen = 0;
	(*box)->visibleRows = rows;
	(*box)->rect.x = x;
	(*box)->rect.y = y;
	setWH(&(*box)->rect, w, h);
}

// clang-format off
void showShopkeeperTextbox(idx, owner, boxId)
	int32_t idx;
	uint8_t owner;
	uint8_t boxId;
// clang-format on
{
	showMapHeadTextbox(idx, owner, boxId, 0xff);
}

void destroyItemMenuBox(ItemMenuBox **box)
{
	freeArray((uint32_t *)(*box)->buf);
	freeArray((uint32_t *)*box);
	*box = 0;
}

void createShopBitsBox(int32_t showBits)
{
	RECT rect;
	RECT origin;
	uint8_t flags;

	flags = 0xe1;
	BIT_BOX_SHOW_BITS = showBits;
	UPDATE_SHOP_BIT_BOX = 1;
	if (UI_BOX_DATA[2].state == 1) {
		return;
	}

	setupBoxOrigin(0xfd, &origin);
	setRECT(&rect, -0x98, -0x62, 0x52, 0x21);
	createTextbox(2, flags, &rect, &origin, tickShopBitsBox, renderShopBitsBox);
	registerTextbox(2, 8, 1, 0, 0);
	tickShopBitsBox();
}

// clang-format off
void showShopkeepSelection(idx, owner, boxId, outSelection)
	int32_t idx;
	uint8_t owner;
	uint16_t boxId;
	int32_t *outSelection;
// clang-format on
{
	showMapheadSelection(idx, owner, boxId, outSelection, 0xff);
}

void createItemMenu(void)
{
	RECT rect;
	RECT origin;
	uint8_t flags;
	ItemMenuBox *result;
	uint8_t boxId;
	int16_t *dims;

	flags = 0xf1;
	if (ITEM_MENU_TYPE == 1 || ITEM_MENU_TYPE == 5) {
		boxId = 0xfd;
	} else {
		boxId = readPStat(0xfe);
	}

	setupBoxOrigin(boxId, &origin);
	result = getItemMenuFromType();
	dims = &ITEM_MENU_POS[ITEM_MENU_TYPE * 4];
	setRECT(&rect, dims[0], dims[1], dims[2], dims[3]);
	createTextbox(1, flags, &rect, &origin, tickItemMenu, renderItemMenu);
	registerTextbox(1, 9, 6, 1, 0);
	initItemMenuBox(result, 1, 9);
	updateItemMenuStrings(result, 9, 0);
	SHOP_VARIABLE = 0;
}

void openDiscardItem(void)
{
	int32_t found;

	switch (SCRIPT_STATE_2) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_RIGHT,
		                    INVENTORY.size << 1, 6, 0x9a, 0x18,
		                    6, 0x5a);

		found = shopFillSellItemList();
		if (found) {
			SCRIPT_STATE_2 = 3;
			SCRIPT_TEXTBOX_MODE = 0;
			break;
		}

		setTrigger(3);
		writePStat(0xfe, 0xff);
		SCRIPT_STATE_2 = 2;
		SCRIPT_TEXTBOX_MODE = 0;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&ITEM_MENU_RIGHT);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 5;
		createItemMenu();
		SCRIPT_STATE_2 = 1;
		break;
	case 4:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		SCRIPT_STATE_2 = 2;
		break;
	}
}

ItemMenuBox *getItemMenuFromType(void)
{
	switch (ITEM_MENU_TYPE) {
	case 0:
	case 2:
	case 3:
	case 4:
	case 6:
		return ITEM_MENU_LEFT;
	case 1:
	case 5:
	case 7:
		return ITEM_MENU_RIGHT;
	}

	return 0;
}

// clang-format off
void initItemMenuBox(box, boxId, startRow)
	ItemMenuBox *box;
	uint8_t boxId;
	int32_t startRow;
// clang-format on
{
	int32_t i;

	if (box->isOpen == 0) {
		box->isOpen = 1;
		box->boxId = boxId;
		box->topRow = 0;
		box->cursor = 0;
		box->prevTopRow = 0;
		box->prevCursor = 0;
		for (i = 0; i < box->visibleRows; i++) {
			box->itemRow[i] = startRow + i;
		}
	}
}

void updateItemMenuStrings(ItemMenuBox *box, int32_t startRow, int32_t style)
{
	TextBoxData *entry;
	int32_t i;
	uint8_t *p;
	int32_t rows;

	entry = &TEXTBOX_DATA.box[box->boxId];
	rows = box->itemCount - box->topRow;
	if (rows > box->visibleRows) {
		rows = box->visibleRows;
	}

	if (rows != 0) {
		for (i = 0; i < rows; i++) {
			if (box->itemRow[i] == startRow + rows - 1) {
				if (style == 0) {
					calculateItemMenuStrings(box, i, 1);
				} else if (style == 1) {
					calculateCardMenuStrings(box, i, 1);
				} else if (style == 2) {
					calculateMusicMenuStrings(box, i, 1);
				} else if (style == 3) {
					calculateBirdramonMenuStrings(box, i, 1);
				} else {
					calculateItemListStrings(box, i, 1);
				}
			} else {
				if (style == 0) {
					calculateItemMenuStrings(box, i, 0);
				} else if (style == 1) {
					calculateCardMenuStrings(box, i, 0);
				} else if (style == 2) {
					calculateMusicMenuStrings(box, i, 0);
				} else if (style == 3) {
					calculateBirdramonMenuStrings(box, i, 0);
				} else {
					calculateItemListStrings(box, i, 0);
				}
			}
		}
	} else {
		p = TEXTBOX_LINES_PTR + startRow * TEXTBOX_LINE_SIZE;
		if (entry->vramMode == 2) {
			p += TEXTBOX_LINE_SIZE / 2;
		}

		if (entry->doubleBuffered == 1) {
			p = (uint8_t *)(p + ((entry->backPage ^ 1) * entry->vramRows) * TEXTBOX_LINE_SIZE);
		}

		*p++ = 0;
		*p++ = 0;
	}

	entry->pageReady = 1;
	entry->writeCount++;
}

int32_t isItemMenuBoxBusy(ItemMenuBox *box)
{
	if (box == 0) {
		return 1;
	}

	return flipTextboxPage(box->boxId);
}

int32_t createItemMenuAmountBox(RECT *origin)
{
	ItemMenuBox *box;
	uint8_t amount;
	uint8_t n;
	RECT rect;
	int32_t idx;
	int32_t cost;
	int16_t boxX;
	int16_t boxY;
	uint8_t flags;

	flags = 0xc1;
	box = getItemMenuFromType();
	idx = (box->topRow + box->cursor) * 2;
	SHOP_ITEM_TYPE = box->buf[idx];
	if (SHOP_ITEM_TYPE == 0xff) {
		goto fail;
	}

	amount = box->buf[idx + 1];
	if (ITEM_MENU_TYPE < 3) {
		SHOP_ITEM_PRICE = ITEM_PARA[SHOP_ITEM_TYPE].value;
		if (ITEM_MENU_TYPE == 1) {
			if ((amount & 0x80) == 0) {
				goto fail;
			}

			SHOP_ITEM_PRICE >>= 1;
			MAX_SHOP_AMOUNT = amount & 0x7f;
		} else {
			if (ITEM_MENU_TYPE == 0) {
				if (amount == 0) {
					goto fail;
				}
				MAX_SHOP_AMOUNT = 0x63;
			} else {
				if ((amount & 0x80) == 0) {
					goto fail;
				}
				MAX_SHOP_AMOUNT = amount & 0x7f;
			}

			n = 0x63 - getItemCount(SHOP_ITEM_TYPE);
			if (n < MAX_SHOP_AMOUNT) {
				MAX_SHOP_AMOUNT = n;
			}

			cost = MAX_SHOP_AMOUNT * SHOP_ITEM_PRICE;
			if (cost > MONEY) {
				MAX_SHOP_AMOUNT = MONEY / SHOP_ITEM_PRICE;
			}
		}
	} else {
		n = CARD_DATA[SHOP_ITEM_TYPE].spriteId;
		SHOP_ITEM_PRICE = CARD_PRICES[n] >> 1;
		MAX_SHOP_AMOUNT = amount;
	}

	SHOP_AMOUNT = 1;
	NAMING_CURRENT_LETTER = 1;
	updateItemMenuSubTextboxLine();
	boxX = UI_BOX_DATA[1].finalPos.x;
	boxY = UI_BOX_DATA[1].finalPos.y;
	origin->x += boxX;
	origin->y += (boxY + box->cursor * 18);
#if VERSION_IS(EU)
	setRECT(&rect, -0x41, -0x2a, 0x8a, 0x53);
#else
	setRECT(&rect, -0x41, -0x2a, 0x82, 0x53);
#endif
	createTextbox(3, flags, &rect, origin, tickItemMenuAmountBox, renderItemMenuAmountBox);
	registerTextbox(3, ITEM_MENU_SUB_TEXTBOX_LINE, 1, 0, 0);
	updateItemMenuAmountBoxString();
	playSound(0, 3);

	return 1;
fail:
	playSound(0, 0xb);

	return 0;
}

int32_t createSingleCardShopMenu(RECT *origin)
{
	ItemMenuBox *box;
	int32_t idx;
	RECT rect;
	int16_t boxX;
	int16_t boxY;
	uint8_t flags;
	uint8_t amount;

	flags = 0xc1;
	box = getItemMenuFromType();
	idx = (box->topRow + box->cursor) * 2;
	SHOP_ITEM_TYPE = box->buf[idx];
	if (SHOP_ITEM_TYPE == 0xff) {
		goto fail;
	}

	amount = box->buf[idx + 1];
	if (ITEM_MENU_TYPE == 5) {
		if ((amount & 0x80) == 0) {
			goto fail;
		}
	} else {
		if (amount == 0) {
			goto fail;
		}
	}

	updateItemMenuSubTextboxLine();
	boxX = UI_BOX_DATA[1].finalPos.x;
	boxY = UI_BOX_DATA[1].finalPos.y;
	origin->x += boxX;
	origin->y += (boxY + box->cursor * 18);
	setRECT(&rect, -0x38, -0x15, 0x70, 0x2a);
	createTextbox(3, flags, &rect, origin, tickSingleCardShop, renderSingleCardShop);
	registerTextbox(3, ITEM_MENU_SUB_TEXTBOX_LINE, 2, 0, 0);
	showMapHeadTextbox(0, 0xff, 3, 0x4d8);
	SHOP_AMOUNT = 0;
	playSound(0, 3);

	return 1;
fail:
	playSound(0, 0xb);

	return 0;
}

void itemMenuCursorTop(ItemMenuBox *box, int32_t startRow, int32_t style)
{
	itemMenuCursorMoveToTop(box);
	updateItemMenuStrings(box, startRow, style);
}

void itemMenuCursorUp(ItemMenuBox *box, int32_t style)
{
	int32_t j;
	int32_t k;
	uint8_t last;

	if (box->topRow + box->cursor != 0) {
		if (box->cursor == 0) {
			box->topRow -= 1;
			j = box->visibleRows - 1;
			last = box->itemRow[j];

			k = j;
			while (k != 0) {
				box->itemRow[k] = box->itemRow[k - 1];
				k--;
			}

			box->itemRow[0] = last;
			updateItemMenuLine(box, style);
		} else {
			box->cursor -= 1;
		}
		playSound(0, 2);
	} else {
		playSound(0, 0xb);
	}
}

void itemMenuCursorBottom(ItemMenuBox *box, int32_t startRow, int32_t style)
{
	itemMenuCursorMoveToBottom(box);
	updateItemMenuStrings(box, startRow, style);
}

void itemMenuCursorDown(ItemMenuBox *box, int32_t style)
{
	int32_t k;
	uint8_t first;

	box->cursor += 1;

	if (box->topRow + box->cursor < box->itemCount) {
		if (box->cursor == box->visibleRows) {
			box->topRow += 1;
			box->cursor -= 1;
			first = box->itemRow[0];

			for (k = 1; k < box->visibleRows; k++) {
				box->itemRow[k - 1] = box->itemRow[k];
			}

			box->itemRow[box->visibleRows - 1] = first;
			updateItemMenuLine(box, style);
		}
		playSound(0, 2);
	} else {
		box->cursor -= 1;
		playSound(0, 0xb);
	}
}

int32_t createItemMenuDescriptionBox(ItemMenuBox *box, RECT *origin, uint8_t uiBoxId)
{
	uint8_t *out;
	RECT rect;
	uint32_t len;
	int16_t boxX;
	int16_t boxY;
	uint8_t flags;
	uint8_t item;

	flags = 0xc1;
	item = box->buf[(box->topRow + box->cursor) * 2];
	if (item == 0xff) {
		return 0;
	}

	updateItemMenuSubTextboxLine();
	boxX = UI_BOX_DATA[uiBoxId].finalPos.x;
	boxY = UI_BOX_DATA[uiBoxId].finalPos.y;
	origin->x += boxX;
	origin->y += (boxY + box->cursor * 18);
#if VERSION_IS(EU)
	setRECT(&rect, -0x84, -0xb, 0x108, 0x22);
	createTextbox(3, flags, &rect, origin, tickItemMenuDescriptionBox, renderItemMenuDescriptionBox);
	registerTextbox(3, ITEM_MENU_SUB_TEXTBOX_LINE, 2, 0, 0);
	out = TEXTBOX_LINES_PTR + ITEM_MENU_SUB_TEXTBOX_LINE * 80;
	strcpy(out, ITEM_DESCS[item * 2]);
	len = strlen(ITEM_DESCS[item * 2]);
	out += len;
	*out++ = 0xd;
	*out = 0;
	out = TEXTBOX_LINES_PTR + (ITEM_MENU_SUB_TEXTBOX_LINE + 1U) * 80;
	strcat((char *)out, ITEM_DESCS[item * 2 + 1]);
	len = strlen(ITEM_DESCS[item * 2 + 1]);
	out += len;
	*out++ = 0;
	*out = 0;
#else
	setRECT(&rect, -0x84, -0xb, 0x108, 0x16);
	createTextbox(3, flags, &rect, origin, tickItemMenuDescriptionBox, renderItemMenuDescriptionBox);
	registerTextbox(3, ITEM_MENU_SUB_TEXTBOX_LINE, 1, 0, 0);
#if !VERSION_IS(US)
	out = TEXTBOX_LINES_PTR + ITEM_MENU_SUB_TEXTBOX_LINE * TEXTBOX_LINE_SIZE;
	strcpy(out, ITEM_DESCS[item]);
#else
	strcpy((out = TEXTBOX_LINES_PTR + ITEM_MENU_SUB_TEXTBOX_LINE * TEXTBOX_LINE_SIZE, out), ITEM_DESCS[item]);
#endif
	len = strlen(ITEM_DESCS[item]);
	out += len;
	*out++ = 0;
	*out = 0;
#endif
	TEXTBOX_DATA.box[3].pageReady = 1;
	TEXTBOX_DATA.box[3].writeCount++;

	return 1;
}

void renderItemMenuSprite(uint8_t boxId, uint8_t idx, int16_t x, int16_t y)
{
	BoxUVTable uvs;
	POLY_FT4 poly;
	int16_t *e;

	uvs = ITEM_MENU_HEADER_UVS;
	e = &uvs.v[idx * 4];
	SetPolyFT4(&poly);
	poly.tpage = getTPage(0, 0, 320, 0);
	poly.clut = GetClut(96, 492);
	setRGB0(&poly, 0x80, 0x80, 0x80);
	setUVDataPolyFT4(&poly, e[0], e[1], e[2], e[3]);
	setPosDataPolyFT4(&poly, x, y, e[2], e[3]);

	GsSortPoly(&poly, ACTIVE_ORDERING_TABLE, (6 - boxId));
}

static void MAIN_func_800FD8D4__garbage__(void)
{
	int16_t a;
	int16_t b;

	a = MONOCHROMON_BUBBLE_TIMER;
	b = NAMING_CURRENT_LETTER;
	a = (a * 100) / (b + 0);
	a = (a * 100) / (b + 1);
	a = (a * 100) / (b + 2);
	a = (a * 100) / (b + 3);
	a = (a * 100) / (b + 4);
	MONOCHROMON_BUBBLE_TIMER = a;
}

void renderItemMenuScrollBar(ItemMenuBox *box)
{
	POLY_F4 *prim;
	int16_t x;
	int32_t shadow;
	int32_t highlight;
	GsOT_TAG *otp;
	int16_t boxId;
	int16_t thumbY;
	int16_t y;
	int16_t w;
	int16_t h;
	int16_t track;
	int16_t rows;

	boxId = box->boxId;
	x = box->rect.x + UI_BOX_DATA[boxId].finalPos.x;
	y = box->rect.y + UI_BOX_DATA[boxId].finalPos.y;
	boxId = 6 - boxId;
	otp = ACTIVE_ORDERING_TABLE->org + boxId;
	shadow = 0x20202;
	highlight = 0xa08769;
	w = box->rect.w;
	h = box->rect.h;
	if (box->itemCount < box->visibleRows) {
		rows = box->visibleRows;
	} else {
		rows = box->itemCount;
	}
	track = (h * 100) / rows;
	thumbY = y + 1 + ((track * box->topRow) / 100);
	track = (track * box->visibleRows) / 100;

	drawLine3P(shadow, x + w + 1, y, x, y, x, y + h + 1 - 1, boxId, 0);
	drawLine3P(highlight, x, y + h + 1, x + w + 1, y + h + 1, x + w + 1, y + 1, boxId, 0);
	drawLine3P(highlight, x + 1 - 1 + w, thumbY, x + 1, thumbY, x + 1, thumbY + track - 2, boxId, 0);
	drawLine3P(shadow, x + 1, thumbY + track - 1, x + 1 - 1 + w, thumbY + track - 1, x + 1 - 1 + w, thumbY + 1, boxId, 0);

	prim = (POLY_F4 *)GsGetWorkBase();
	SetPolyF4(prim);
	setRGB0(prim, 0x5b, 0x70, 0x80);
	setXYWH(prim, x + 2, thumbY + 1, w - 2, track - 2);
	AddPrim(otp, prim++);
	SetPolyF4(prim);
	setRGB0(prim, 0x35, 0x4b, 0x5c);
	setXYWH(prim, x + 1, y + 1, w, h);
	AddPrim(otp, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void renderItemMenuItemList(ItemMenuBox *box, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int32_t mode)
{
	int32_t count;
	int32_t i;
	TextBoxData *tbox;
	int32_t top;
	int32_t outX;
	int32_t outW;
	int16_t boxId;
	int16_t texY;
	int16_t rowY;
	uint8_t item;

	boxId = box->boxId;
	tbox = &TEXTBOX_DATA.box[boxId];
	if (mode != 2) {
		if (isItemMenuBoxBusy(box) != 0) {
			top = box->prevTopRow;
			count = box->itemCount - top;
			if (box->visibleRows < count) {
				count = box->visibleRows;
			}
		} else {
			top = box->topRow;
			count = box->itemCount - top;
			if (box->visibleRows < count) {
				count = box->visibleRows;
			}
		}

		for (i = 0; i < count; i++, top++, y2 += 0x12) {
			item = box->buf[top * 2];
			if (item != 0xff) {
				if (mode == 0) {
					renderItemSprite(item, x2, y2, 6 - boxId);
				} else if (mode == 1) {
					item = CARD_DATA[item].spriteId;
					renderCardSprite(item, x2 + 2, y2 + 2, 6 - boxId);
				}
			}
		}
	}

	getVRAMModeCoords(tbox->vramMode, &outX, &outW);
	texY = tbox->backPage * tbox->vramRows * 12;
	count = box->itemCount - box->topRow;
	if (box->visibleRows < count) {
		count = box->visibleRows;
	}

	for (i = 0; i < count; i++, y1 += 0x12) {
		item = box->buf[(box->topRow + i) * 2];
		if (item != 0xff) {
			rowY = texY + box->itemRow[i] * 12;
			renderString(0, x1, y1, outW, 0xc, outX, rowY, 6 - boxId, 1);
		}
	}
}

void updateItemMenuSubTextboxLine(void)
{
	TextBoxData *box = (TextBoxData *)TEXTBOX_DATA.box;

	ITEM_MENU_SUB_TEXTBOX_LINE = box->vramRow;
	ITEM_MENU_SUB_TEXTBOX_LINE = (box->backPage ^ 1) * box->vramRows;
}

void playShopSoundOnlyInSavannah(void)
{
	int32_t i;

	for (i = 0; i <= 0; i++) {
		if (CURRENT_SCREEN_ID == MAIN_D_801345C0[i]) {
			i = i * 2;
			playSound(((uint8_t *)&MAIN_D_801345C2)[i],
			          ((uint8_t *)&MAIN_D_801345C2)[i + 1]);

			return;
		}
	}
}

void renderHorizontalLine(uint8_t boxId, int16_t x, int16_t y, int16_t w)
{
	uint32_t color;

	x += UI_BOX_DATA[boxId].finalPos.x;
	y += UI_BOX_DATA[boxId].finalPos.y;
	boxId = 6 - boxId;
	color = 0x20202;
	drawLine2P(color, x, y, (x + w) - 1, y, boxId, 0);
	y++;
	color = 0xa08769;
	drawLine2P(color, x, y, (x + w) - 1, y, boxId, 0);
	y++;
	color = 0x20202;
	drawLine2P(color, x, y, (x + w) - 1, y, boxId, 0);
}

void renderInsetWithoutBox(uint8_t boxId, int16_t x, int16_t y, int16_t w, int16_t h)
{
	int32_t color;

	x += UI_BOX_DATA[boxId].finalPos.x;
	y += UI_BOX_DATA[boxId].finalPos.y;
	boxId = 6 - boxId;
	color = 0xa08769;
	drawLine3P(color, x + 1, (y + h) - 1, (x + w) - 1, (y + h) - 1, (x + w) - 1, y, boxId, 0);
	color = 0x20202;
	drawLine3P(color, (x + w) - 1, y, x, y, x, (y + h) - 1, boxId, 0);
}

void renderCardSprite(uint8_t spriteId, int16_t x, int16_t y, int32_t depth)
{
	POLY_FT4 *prim;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	prim->tpage = getTPage(0, 0, 320, 0);
	setClut(prim, 96, 493);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setUVDataPolyFT4(prim, spriteId * 12 + 0x200, 0x1c0, 0xc, 0xc);
	setPosDataPolyFT4(prim, x, y, 0xc, 0xc);
	AddPrim(ACTIVE_ORDERING_TABLE->org + depth, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void showMapHeadTextbox(int32_t idx, uint8_t owner, uint8_t boxId, int32_t section)
{
	uint8_t *savedCursor;

	if (boxId == 0 && owner != 0xfe) {
		setDialogueOwner(owner);
	}

	savedCursor = SCRIPT_POINTER;

	if (section == 0xff) {
		SCRIPT_POINTER = getShopkeeperLine(idx);
	} else {
		SCRIPT_POINTER = resolveMapHeadEntry(section, idx);
	}

	if (owner == 0xfe) {
		owner = 0xff;
	}

	showTextboxReady(boxId, owner);
	ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
	SCRIPT_POINTER = savedCursor;
}

void lostAllLives(void)
{
	uint8_t *pool;
	int16_t i;
	int16_t n;
	int32_t j;

	switch (SCRIPT_STATE_2) {
	case 0:
		break;
	case 1:
		return;
	case 2:
		goto state2;
	case 3:
		goto state3;
	default:
		return;
	}

	i = 0;
	n = 0;
	while (i < 0x3a) {
		if (hasMove(i) != 0) {
			n += 1;
		}
		i++;
	}

	n = (10 - TAMER_ENTITY.tamerLevel) * 2 * (n * 100) / 100;
	if (n % 100 >= 0x32) {
		n += 100;
	}

	n = n / 100;
	if (n == 0) {
		ACTIVE_INSTRUCTION = 0;
		return;
	}

	writePStat(0xf4, n);
	SCRIPT_STATE_2 = 2;

	return;
state2:
	pool = allocateArray(0x3a);
	j = 0;
	i = 0;
	while (i < 0x3a) {
		if (hasMove(i) != 0) {
			pool[j] = i;
			j += 1;
		}
		i++;
	}

	n = randomLimit(j);
	n = pool[n];
	unlearnMove(n);
	writePStat(0xf3, n);
	freeArray((uint32_t *)pool);
	showMapHeadTextbox(7, 0xff, 0, 0x4d8);
	SCRIPT_STATE_2 = 1;
	SCRIPT_NEXT_STATE_2 = 3;
	SCRIPT_TEXTBOX_MODE = 1;

	return;
state3:
	n = readPStat(0xf4);
	n -= 1;
	if (n == 0) {
		closeBox(0);
		ACTIVE_INSTRUCTION = 0;
		return;
	}

	writePStat(0xf4, n);
	SCRIPT_STATE_2 = 2;
}

int32_t flipTextboxPage(uint8_t a)
{
	TextBoxData *entry;

	entry = &TEXTBOX_DATA.box[a];

	if (entry->doubleBuffered == 0) {
		return 0;
	}

	if (entry->registered == 0) {
		return 1;
	}

	if (entry->writeCount != entry->renderCount) {
		return 1;
	}

	if (entry->pageReady == 0) {
		return 0;
	}

	entry->backPage ^= 1;
	entry->flipCount++;
	entry->pageReady = 0;
}

void calculateItemMenuStrings(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	int32_t i;
	uint32_t len;
	int32_t value;
	uint8_t type;
	uint8_t amount;

	out = getTextboxLine(box, row);
	i = (box->topRow + row) * 2;
	type = box->buf[i];
	amount = box->buf[i + 1];

	if (type != 0xff) {
		*out++ = 1;
		if (ITEM_MENU_TYPE == 0 || ITEM_MENU_TYPE == 7) {
			if (amount != 0) {
				*out++ = 1;
			} else {
				*out++ = 3;
			}
		} else {
			if ((amount & 0x80) != 0) {
				*out++ = 1;
			} else {
				*out++ = 3;
			}
		}

		strcpy(out, ITEM_NAME(type));
		len = strlen(ITEM_NAME(type));
		out += len;
#if !VERSION_IS(US)
		*out++ = 0xc;
		*out++ = 0;
#endif
#if !VERSION_IS(EU)
		*out++ = 0xf;
		*out++ = 0;
#endif

		if (ITEM_MENU_TYPE != 5) {
#if VERSION_IS(EU)
			*out++ = 0xf;
			*out++ = 0;
#endif
#if VERSION_IS(US)
			*out++ = 0x16;
			*out++ = 0;
#endif
			if (ITEM_MENU_TYPE != 7) {
				value = ITEM_PARA[type].value;
				if (ITEM_MENU_TYPE == 1) {
					if ((amount & 0x80) != 0) {
						value >>= 1;
					} else {
						for (i = 0; i < 4; i++) {
							*out++ = 0x81;
							*out++ = 0x7c;
						}
						*out++ = 0xf;
						*out++ = 0;
						goto amountPart;
					}
				}
			} else {
				value = ITEM_PARA[type].meritValue;
			}

			out = intToStringSJIS(out, value, 4, 0);
			*out++ = 0xf;
			*out++ = 0;
#if VERSION_IS(EU)
			*out++ = 0xf;
			*out++ = 0;
#endif
		}
amountPart:
#if !VERSION_IS(US)
		if (ITEM_MENU_TYPE != 0 && ITEM_MENU_TYPE != 7) {
			out = intToStringSJIS(out, amount & 0x7f, 2, 0);
#if VERSION_IS(EU)
			*out++ = 0xf;
			*out++ = 0;
#endif
		}
#else
		if (ITEM_MENU_TYPE == 1) {
			out = intToStringSJIS(out, amount & 0x7f, 2, 0);
		}
		if ((ITEM_MENU_TYPE != 0) && (ITEM_MENU_TYPE != 7) &&
		    (ITEM_MENU_TYPE != 1)) {
			*out++ = 0x1c;
			*out++ = 0;
			out = intToStringSJIS(out, amount & 0x7f, 2, 0);
		}
#endif
	}

	terminateString(out, isLast);
}

void calculateCardMenuStrings(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	uint8_t type;
	uint8_t id;
	uint8_t amount;
	int32_t idx;
	uint32_t len;
	int32_t value;

	out = getTextboxLine(box, row);
	idx = (box->topRow + row) * 2;
	type = box->buf[idx];
	amount = box->buf[idx + 1];

	if (type != 0xff) {
		*out++ = 1;
		if (ITEM_MENU_TYPE == 3) {
			if (amount != 0) {
				if (getCardAmount(type) == 0) {
					*out++ = 7;
				} else {
					*out++ = 1;
				}
			} else {
				*out++ = 3;
			}
		} else {
			*out++ = 1;
		}

		id = CARD_DATA[type].digimonId;
		strcpy(out, DIGIMON_NAME(id));
		len = strlen(DIGIMON_NAME(id));
		out += len;
#if !VERSION_IS(US)
		*out++ = 0xc;
#else
		*out++ = 0x17;
#endif
		*out++ = 0;
		*out++ = 0xf;
		*out++ = 0;
		id = CARD_DATA[type].spriteId;
		value = CARD_PRICES[id];

		if (ITEM_MENU_TYPE == 3) {
			out = intToStringSJIS(out, value, 4, 0);
		} else {
			if (ITEM_MENU_TYPE != 6) {
				out = intToStringSJIS(out, value >> 1, 4, 0);
				*out++ = 0xf;
				*out++ = 0;
			}
			out = intToStringSJIS(out, amount, 2, 0);
		}
	}

	terminateString(out, isLast);
}

void calculateMusicMenuStrings(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	uint8_t type;
	int32_t idx;
	uint32_t len;

	out = getTextboxLine(box, row);
	idx = (box->topRow + row) * 2;
	type = box->buf[idx];
	*out++ = 1;

	if (type == readPStat(0xf9)) {
		*out++ = 6;
	} else {
		*out++ = 1;
	}

	out = intToStringSJIS(out, type + 1, 2, 0);
	*out++ = 0xf;
	*out++ = 0;
	strcpy(out, BGM_TRACK_NAMES[type]);
	len = strlen(BGM_TRACK_NAMES[type]);
	out += len;
#if VERSION_IS(EU)
	out = padWithSpaces(out, 0x12, len);
#else
	out = padWithSpaces(out, 0xc, len);
#endif
	terminateString(out, isLast);
}

void calculateBirdramonMenuStrings(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	uint8_t raw;
	uint8_t nameId;
	int32_t idx;
	uint32_t len;

	out = getTextboxLine(box, row);
	idx = (box->topRow + row) * 2;
	raw = box->buf[idx];
	*out++ = 1;

	if ((raw & 0x80) != 0) {
		*out++ = 1;
	} else {
		*out++ = 3;
	}

	raw &= 0x7f;
	nameId = BIRDRA_TRANSPORT_TARGETS[raw].mapId;
	nameId = MAP_ENTRIES[nameId].loadingName;
	strcpy(out, MAP_NAME_PTR[nameId]);
	len = strlen(MAP_NAME_PTR[nameId]);
	out += len;
#if VERSION_IS(EU)
	out = padWithSpaces(out, 0x12, len);
#else
	out = padWithSpaces(out, 0xc, len);
#endif
	*out++ = 0xf;
	*out++ = 0;
#if VERSION_IS(US)
	*out++ = 0x1b;
	*out++ = 0;
#endif
	out = intToStringSJIS(out, BIRDRA_TRANSPORT_TARGETS[raw].cost, 4, 0);
	terminateString(out, isLast);
}

void updateItemMenuLine(ItemMenuBox *box, int32_t style)
{
	TextBoxData *entry;
	int16_t row;
	int32_t outX;
	int32_t outClut;
	uint8_t *p;

	entry = &TEXTBOX_DATA.box[box->boxId];
	entry->backPage ^= 1;

	row = box->cursor;

	if (style == 0) {
		calculateItemMenuStrings(box, row, 1);
	} else if (style == 1) {
		calculateCardMenuStrings(box, row, 1);
	} else if (style == 2) {
		calculateMusicMenuStrings(box, row, 1);
	} else if (style == 3) {
		calculateBirdramonMenuStrings(box, row, 1);
	} else {
		calculateItemListStrings(box, row, 1);
	}

	entry->backPage ^= 1;

	row = box->itemRow[row];
	if (entry->doubleBuffered == 1) {
		row += entry->backPage * entry->vramRows;
	}

	getVRAMModeCoords(entry->vramMode, &outX, &outClut);

	p = TEXTBOX_LINES_PTR + row * TEXTBOX_LINE_SIZE;
	if (outX != 0) {
		p += TEXTBOX_LINE_SIZE / 2;
	}

#if !VERSION_IS(US)
	drawString2(p, outX, row * 12);
#else
	drawString2(p, outX, row * 12, 1);
#endif
}

void renderKeeperTableBox(uint8_t boxId, int16_t x, int16_t y, int16_t w, int16_t h)
{
	int32_t color;

	x += UI_BOX_DATA[boxId].finalPos.x;
	y += UI_BOX_DATA[boxId].finalPos.y;
	boxId = 6 - boxId;
	color = 0xa08769;
	drawLine3P(color, (x + w) - 1, y, x, y, x, (y + h) - 1, boxId, 0);
	color = 0x20202;
	drawLine3P(color, x, (y + h) - 1, (x + w) - 1, (y + h) - 1, (x + w) - 1, y, boxId, 0);
}

void itemMenuCursorMoveToBottom(ItemMenuBox *data)
{
	int32_t diff;

	data->prevTopRow = data->topRow;
	data->prevCursor = data->cursor;
	diff = data->itemCount - data->visibleRows;

	if (diff <= 0) {
		data->topRow = 0;
		data->cursor = data->itemCount - 1;
	} else {
		data->topRow = diff;
		data->cursor = data->visibleRows - 1;
	}

	playSound(0, 2);
}

void itemMenuCursorMoveToTop(ItemMenuBox *data)
{
	data->prevTopRow = data->topRow;
	data->prevCursor = data->cursor;
	data->topRow = 0;
	data->cursor = 0;
	playSound(0, 2);
}

uint8_t *getTextboxLine(ItemMenuBox *box, uint8_t index)
{
	TextBoxData *entry;
	uint8_t *row;

	entry = &TEXTBOX_DATA.box[box->boxId];
	row = TEXTBOX_LINES_PTR + box->itemRow[index] * TEXTBOX_LINE_SIZE;

	if (entry->vramMode == 2) {
		row += TEXTBOX_LINE_SIZE / 2;
	}

	if (entry->doubleBuffered == 1) {
		row = (uint8_t *)(row + ((entry->backPage ^ 1) * entry->vramRows) * TEXTBOX_LINE_SIZE);
	}

	return row;
}

void terminateString(uint8_t *str, int32_t flag)
{
	*str++ = 1;
	*str++ = 1;

	if (flag != 0) {
		*str++ = 0;
		*str = 0;
	} else {
		*str++ = 0xd;
		*str = 0;
	}
}

uint8_t *padWithSpaces(uint8_t *str, int32_t width, int32_t used)
{
	used = width - (used >> 1);
#if VERSION_IS(EU)
	if (used < 0) {
		str -= used * 2;
	} else {
		while (used != 0) {
			*str++ = 0x81;
			*str++ = 0x40;
			used--;
		}
#else
	while (used != 0) {
		*str++ = 0x81;
		*str++ = 0x40;
		used--;
#endif
	}

	return str;
}

void renderVerticalLine(uint8_t boxId, int16_t x, int16_t y, int16_t h)
{
	uint32_t color;

	x += UI_BOX_DATA[boxId].finalPos.x;
	y += UI_BOX_DATA[boxId].finalPos.y;
	boxId = 6 - boxId;
	color = 0x20202;
	drawLine2P(color, x, y, x, (y + h) - 1, boxId, 0);
	x++;
	color = 0xa08769;
	drawLine2P(color, x, y, x, (y + h) - 1, boxId, 0);
	x++;
	color = 0x20202;
	drawLine2P(color, x, y, x, (y + h) - 1, boxId, 0);
}

// clang-format off
void setDialogueOwner(owner)
	uint8_t owner;
// clang-format on
{
	if (CURRENT_DIALOGUE_OWNER != owner) {
		CURRENT_DIALOGUE_OWNER = owner;
		scriptPauseGame(CURRENT_DIALOGUE_OWNER);
		setupDialogueBox(CURRENT_DIALOGUE_OWNER);
	}
}

void showMapheadSelection(int32_t idx, uint8_t owner, uint16_t x, int32_t *outSel, uint16_t section)
{
	uint8_t *saved;
	uint16_t rows;

	if (owner != 0xfe) {
		setDialogueOwner(owner);
	} else {
		CURRENT_DIALOGUE_OWNER = 0xff;
		owner = 0xfd;
	}

	saved = SCRIPT_POINTER;

	if (section == 0xff) {
		SCRIPT_POINTER = getShopkeeperLine(idx);
	} else {
		SCRIPT_POINTER = resolveMapHeadEntry(section, idx);
	}

	DIALOGUE_SELECTION.count = x;

	if (*outSel == 0) {
		*outSel = 1;
		DIALOGUE_SELECTION.current = 0;
	}

	DIALOGUE_SELECTION.cursorWidth = showTextboxReady(0, CURRENT_DIALOGUE_OWNER);
	DIALOGUE_SELECTION.cursorWidth = DIALOGUE_SELECTION.cursorWidth * GLYPH_WIDTH + 2;

	if (owner != 0xff) {
		DIALOGUE_SELECTION.cursorOffsetY = 0xd;
	} else {
		DIALOGUE_SELECTION.cursorOffsetY = 0;
	}

	ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
	SCRIPT_POINTER = saved;
}

void setDigimonRaised(int32_t digimonId)
{
	if ((uint32_t)digimonId < 0x3f) {
		setTrigger(digimonId + 0x200);
	}
}

// clang-format off
int32_t hasDigimonRaised(digimonId)
	uint16_t digimonId;
// clang-format on
{
	return isTriggerSet(digimonId + 0x200);
}

void unlockMedal(uint16_t medal)
{
	if (medal < 0x13) {
		setTrigger(medal + 0x16c);
	}
}

int32_t hasMedal(uint16_t medal)
{
	return isTriggerSet(medal + 0x16c);
}

void createMonochromonMoodBubble(void)
{
	int32_t a;
	int32_t b;

	a = readPStat(0xf7) & 0xff;
	b = readPStat(0xf8) & 0xff;

	if ((uint32_t)b < 5) {
		if (scriptIdToEntityId(a) != 0xff) {
			MONOCHROMON_BUBBLE_TIMER = 0x1e;
			MONOCHROMON_BUBBLE_SPRITE.u = b * 32;
			addObject(0x1b1, 0, 0, renderMonochromonMoodBubble);
		}
	} else {
		removeObject(0x1b1, 0);
	}
}

void triggerSeadramonCutscene(void)
{
	callScriptSection(0, 0x4e4, 0);
}

// clang-format off
void checkShopMap(mapId)
	int16_t mapId;
// clang-format on
{
	uint8_t local;
	int32_t i;

	if (isTriggerSet(0x29) == 1) {
		unsetTrigger(0x29);
		return;
	}
	for (i = 0; i < 8; i++) {
		if (mapId == SHOP_MAPS[i]) {
			loadDynamicLibrary(SHOP_REL, &local, 0, 0, 0);
			return;
		}
	}
}

void loadShopLibrary(void)
{
	loadDynamicLibrary(SHOP_REL, (uint8_t *)&TRN_LOADING_COMPLETE, 1, 0, 0);
}

// clang-format off
void checkArenaMap(mapId)
	int16_t mapId;
// clang-format on
{
	uint8_t local;
	int32_t i;

	for (i = 0; i < 2; i++) {
		if (mapId == ARENA_MAPS[i]) {
			loadDynamicLibrary(DGET_REL, &local, 0, 0, 0);
			return;
		}
	}
}

void *allocateArray(uint32_t size)
{
	uint32_t oldTop;

	oldTop = ARRAY_SECTION_OFFSET;
	size = ((size + 3) >> 2) << 2;
	*(uint32_t *)(ARRAY_SECTION_START + oldTop) = size;
	ARRAY_SECTION_OFFSET += (size + 4);

	return ARRAY_SECTION_START + (oldTop + 4);
}

void freeArray(uint32_t *array)
{
	uint32_t *header;

	header = array - 1;
	ARRAY_SECTION_OFFSET -= *header + 4;
}

void renderShopBitsBox(void)
{
	int16_t rowPx;
	int16_t x;
	int16_t y;

	rowPx = TEXTBOX_DATA.box[2].vramRow * 12;
	x = UI_BOX_DATA[2].finalPos.x + 6;
	y = UI_BOX_DATA[2].finalPos.y + 4;

#if !VERSION_IS(US)
	renderString(0, x, y, 0x48, 0xc, 0, rowPx, 4, 1);
	renderString(0, x, y + 0xd, 0x48, 0xc, 0x48, rowPx, 4, 1);
#else
	renderString(0, x + 0x10, y, 0x30, 0xc, 0, rowPx, 4, 1);
	renderString(0, x + 0x10, y + 0xd, 0x3c, 0xc, 0x30, rowPx, 4, 1);
#endif
}
