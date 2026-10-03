#include <string.h>

#include <libgs.h>

#include <dw/file_queue.h>
#include <dw/font.h>
#include <dw/garbage.h>
#include <dw/item.h>
#include <dw/main.h>
#include <dw/map.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/script.h>
#include <dw/sjis.h>
#include <dw/sound.h>
#include <dw/tamer.h>
#include <dw/ui.h>
#include <dw/utils.h>

typedef struct {
	uint8_t b[11];
} ShopkeeperIdTable;

typedef struct {
	uint8_t b[8];
} BabyTypeTable;

typedef struct {
	int16_t v[15];
} MenuTextLayout;

typedef struct {
	int16_t v[28];
} BoxUVTable;

typedef struct {
	uint32_t usedRows;
	TextBoxData box[6];
} TextBoxTable;

typedef struct {
	int32_t v[6];
} Pow10Table;

extern ScriptCameraMovement MAIN_D_801BE6B4[];
extern TextBoxTable MAIN_D_801BE80C;
extern uint8_t TEXTBOX_OPEN_TIMER;
extern uint32_t MAIN_D_8013501C;
extern int32_t MAIN_D_80135028;
extern uint32_t MAIN_D_80134F64;
extern int16_t MAIN_D_80134F60;
extern int32_t MAIN_D_80135024;
extern int32_t MAIN_D_80135020;
extern uint16_t MAIN_D_80135016;
extern int32_t ITEM_MENU_SUB_TEXTBOX_LINE;
extern uint32_t POLLED_INPUT;
extern int32_t MAIN_D_80134F94;
extern uint16_t MAIN_D_801BE952[];
extern uint16_t MAIN_D_801BE954[];
extern uint16_t MAIN_D_801BE956[];
extern int32_t BIT_BOX_SHOW_BITS;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern uint16_t MAIN_D_801BE950[];
extern int32_t MAIN_D_801BE948[];
extern int32_t MAIN_D_801BE94C[];
extern uint8_t SHOP_AMOUNT;
extern uint8_t MAIN_D_80134F82;
extern uint8_t MAX_SHOP_AMOUNT;
extern int32_t SHOP_ITEM_PRICE;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern char *MOVE_NAMES[];
extern char *MAP_NAME_PTR[];
extern char *ITEM_DESC_PTR[];
extern int8_t MAIN_D_80134F98;
extern int32_t MAIN_D_80134FA8;
extern int32_t CURRENT_SCRIPT_PTR;

void renderSelectionCursor(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);
int32_t scriptCompareSignedValue(uint8_t op, uint32_t lhs, uint32_t rhs);
void renderItemSprite(int32_t itemId, int32_t x, int32_t y, int32_t depth);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t xPos, int32_t yPos, int32_t width, int32_t height);
void renderString(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8);
int32_t flipIdleTextboxPage(int32_t boxId);
int32_t hasMove(int32_t moveId);
void unlearnMove(int32_t moveId);
void loadMap(uint16_t mapId);
void renderMonochromonMoodBubble(int32_t instanceId);
void translateConditionFXToEntity(Entity *entity, SVECTOR *out);
int32_t worldPosToScreenPos(SVECTOR *worldPos, DVECTOR *screenPos);
void closeTextbox(int32_t boxId, RECT *target);
void updateItemMenuLine(ItemMenuBox *box, int32_t style);
void renderShopBitsBox(void);
void tickItemMenu(void);
void renderItemMenu(void);
void itemMenuCursorDown(ItemMenuBox *box, int32_t style);
int32_t createItemMenuDescriptionBox(ItemMenuBox *box, RECT *origin, int32_t uiBoxId);
void renderCustomSizedTextbox(void);
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
uint8_t *getTextboxLine(uint8_t *data, int32_t index);
void terminateString(uint8_t *str, int32_t flag);
uint8_t *padWithSpaces(uint8_t *str, int32_t width, int32_t used);
void setDigimonRaised(int32_t digimonId);
int32_t hasDigimonRaised(int32_t digimonId);
void unlockMedal(uint16_t medal);
int32_t hasMedal(uint16_t medal);
void triggerSeadramonCutscene(void);
void checkShopMap(int32_t mapId);
void checkArenaMap(int32_t mapId);
void clearTextboxLineCount(int32_t boxId);
int32_t tickConfirmDialogue(void);
void renderDialogueSelectionCursor(int32_t x, int32_t y);
void tickCustomSizedTextbox(void);
void getVRAMModeCoords(int32_t mode, int32_t *outX, int32_t *outClut);
int32_t advanceTextbox(int32_t boxId);
void setupDialogueBox(uint8_t owner);
int32_t showTextboxReady(int32_t boxId, int32_t speakerId);
void updateItemMenuAmountBoxString(void);
void tickSingleCardShop(void);
void renderSingleCardShop(void);
int32_t createItemMenuAmountBox(RECT *origin);
int32_t createSingleCardShopMenu(RECT *origin);
void renderItemMenuSprite(int32_t boxId, int32_t idx, int16_t x, int16_t y);
void renderItemMenuScrollBar(ItemMenuBox *box);
void renderItemMenuItemList(ItemMenuBox *box, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int32_t flag);
void renderInsetWithoutBox(int32_t boxId, int16_t x, int16_t y, int32_t w, int16_t h);
void drawLine3P(int32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t otz, int32_t flag);
void renderCardSprite(int32_t spriteId, int16_t x, int16_t y, int32_t depth);
void calculateItemMenuStrings(ItemMenuBox *box, uint8_t row, int32_t isLast);
void calculateCardMenuStrings(ItemMenuBox *box, uint8_t row, int32_t isLast);
void calculateMusicMenuStrings(ItemMenuBox *box, uint8_t row, int32_t isLast);
void calculateBirdramonMenuStrings(ItemMenuBox *box, uint8_t row, int32_t isLast);
void calculateItemListStrings(ItemMenuBox *box, uint8_t row, int32_t isLast);
void renderKeeperTableBox(uint8_t boxId, int16_t x, int16_t y, int32_t w, int16_t h);
int32_t drawTextboxStrings(int32_t boxId, int32_t flag);
int32_t tickSelectionDialogue(void);
int32_t tickBackgroundDialogue(void);
int32_t drawString2(uint8_t *str, int16_t x, int16_t y, int32_t flag);
int32_t getSpeakerName(int32_t speakerId, uint8_t *buf);
uint8_t *intToStringSJIS(uint8_t *buf, int32_t value, uint8_t digits, int32_t flag);
void renderHorizontalLine(uint8_t boxId, int16_t x, int16_t y, int32_t w);
void drawLine2P(uint32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t order, uint32_t mode);
void renderItemMenuAmountBox(void);
void renderUIBox(int32_t boxId);
void renderVerticalLine(int32_t boxId, int16_t x, int16_t y, int32_t h);
void tickItemMenuAmountBox(void);

static void *script_common_text_order[] = {
	scriptInstructionFBtoFF,
	tickScriptedMovements,
	readFileSection,
	enforceStatsLimits,
	scriptCompareMoney,
	scriptCompareItemCount,
	scriptCompareCondition,
	scriptCompareMove,
	scriptCompareCard,
	getStatsPointer,
	scriptCompareStat,
	scriptCompareDate,
	scriptIdToEntityId,
	intToStringSJIS,
	getSpeakerName,
	showTextboxReady,
	scriptSetTextboxSize,
	showTextbox,
	scriptShowSelection,
	renderUIBox,
	renderScriptDialogueBox,
	tickScriptDialogueBox,
	setupBoxOrigin,
	setupDialogueBox,
	advanceTextbox,
	flipIdleTextboxPage,
	drawString2,
	registerTextbox,
	triggerBoxCloseFlag,
	createTextbox,
	closeBox,
	closeAllTextboxes,
	closeTextbox,
	getVRAMModeCoords,
	tickTextboxHandling,
	initializeTextbox,
	renderCustomSizedTextbox,
	tickCustomSizedTextbox,
	renderDialogueSelectionCursor,
	tickBackgroundDialogue,
	tickConfirmDialogue,
	tickSelectionDialogue,
	clearTextboxLineCount,
	drawTextboxStrings,
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
	shopFillSellItemList,
	shopFillBuyItemList,
};

// clang-format off
char STR_TOURNAMENT_NAME_GRADE_D[8] = "Grade D";

char STR_TOURNAMENT_NAME_GRADE_C[8] = "Grade C";

char STR_TOURNAMENT_NAME_GRADE_B[8] = "Grade B";

char STR_TOURNAMENT_NAME_GRADE_A[8] = "Grade A";

char STR_TOURNAMENT_NAME_GRADE_S[8] = "Grade S";

char STR_TOURNAMENT_NAME_GRADE_R[8] = "Grade R";

char STR_TOURNAMENT_GRADE_D[] = "D";

char STR_TOURNAMENT_GRADE_C[] = "C";

char STR_TOURNAMENT_GRADE_B[] = "B";

char STR_TOURNAMENT_GRADE_A[] = "A";

char STR_TOURNAMENT_GRADE_S[] = "S";

char STR_TOURNAMENT_GRADE_R[] = "R";

char STR_TOURNAMENT_GRADE_H[] = "H";

char STR_TOURNAMENT_GRADE_I[] = "I";

char STR_TOURNAMENT_GRADE_J[] = "J";

char STR_TOURNAMENT_GRADE_K[] = "K";

char STR_TOURNAMENT_GRADE_L[] = "L";

char STR_TOURNAMENT_GRADE_F[] = "F";

char STR_TOURNAMENT_GRADE_G[] = "G";

char STR_TOURNAMENT_GRADE_W[] = "W";

char STR_TOURNAMENT_GRADE_O[] = "O";

char STR_TOURNAMENT_GRADE_N[] = "N";

char STR_TOURNAMENT_GRADE_M[] = "M";

char STR_TOURNAMENT_GRADE_T[] = "T";

char STR_TOURNAMENT_GRADE_Y[] = "Y";

char STR_TOURNAMENT_GRADE_Z[] = "Z";

char STR_TOURNAMENT_GRADE_X[] = "X";

char STR_TOURNAMENT_GRADE_Q[] = "Q";

uint8_t *MAIN_D_801345B0 = TEXTURE_BUFFER;

BabyTypeTable MAIN_D_801345B4 = { {
	0x01, 0x02, 0x0f, 0x10, 0x1d, 0x1e, 0x2b, 0x2c,
} };

int16_t MAIN_D_801345BC[2] = {
	0x00d0, 0x00df,
};

int16_t MAIN_D_801345C0[1] = {
	0x0083,
};

uint16_t MAIN_D_801345C2 = 0x1700;

int32_t MAP_LIGHT_UPDATE_DATA[2] = {
	0x003101b7, 0xffff0002,
};

char MAIN_D_801345CC[] = "Sign";

char MAIN_D_801345D4[4] = "Box";

char MAIN_D_801345D8[8] = "Betamon";

char MAIN_D_801345E0[8] = "Tanemon";

char MAIN_D_801345E8[] = "Palmon";

char MAIN_D_801345F0[] = ";1";

char STR_BGM_TRACK_NON_BEWILDERING_FOREST_THEME[] = "Non Bewildering Forest Theme";
char STR_BGM_TRACK_NON_BEWILDERING_FOREST_NIGHT_THEME[] = "Non Bewildering Forest Night Theme";
char STR_BGM_TRACK_TROPICAL_THEME[] = "Tropical Theme";
char STR_BGM_TRACK_TROPICAL_NIGHT_THEME[] = "Tropical Night Theme";
char STR_BGM_TRACK_MT_PANORAMA_THEME[] = "Mt. Panorama Theme";
char STR_BGM_TRACK_MT_PANORAMA_NIGHT_THEME[] = "Mt. Panorama Night Theme";
char STR_BGM_TRACK_DRILL_TUNNEL_THEME[] = "Drill Tunnel Theme";
char STR_BGM_TRACK_OGRE_FORTRESS_THEME[20] = "Ogre Fortress Theme";
char STR_BGM_TRACK_OVERDELL_CEMETARY_THEME[24] = "Overdell Cemetary Theme";
char STR_BGM_TRACK_CANYON_THEME[] = "Canyon Theme";
char STR_BGM_TRACK_OGREMON_THEME_NO_2[20] = "Ogremon Theme No. 2";
char STR_BGM_TRACK_EVERYTHING_SHOP_THEME[] = "Everything Shop Theme";
char STR_BGM_TRACK_OGREMON_THEMENO_3[] = "Ogremon ThemeNo. 3";
char STR_BGM_TRACK_LAVA_CAVE_THEME[16] = "Lava Cave Theme";
char STR_BGM_TRACK_DARK_ARISTCRATS_MANSION_THEME[] = "Dark Aristcrat's Mansion Theme";
char STR_BGM_TRACK_UNDERGROUND_LAB_THEME[] = "Underground Lab Theme";
char STR_BGM_TRACK_GEAR_SAVANNA_THEME[] = "Gear Savanna Theme";
char STR_BGM_TRACK_GEAR_SAVANNA_NIGHT_THEME[] = "Gear Savanna Night Theme";
char STR_BGM_TRACK_LEOMON_THEME[] = "Leomon Theme";
char STR_BGM_TRACK_AMIDA_FOREST_THEME[] = "Amida Forest Theme";
char STR_BGM_TRACK_AMIDA_FOREST_NIGHT_THEME[] = "Amida Forest Night Theme";
char STR_BGM_TRACK_THE_ANCIENT_REGION_OF_DINO_SPEEDY_TIME_ZONE_THEME[] = "The Ancient Region of Dino Speedy Time Zone Theme";
char STR_BGM_TRACK_THE_ANCIENT_REGION_OF_DINO_SPEEDY_TIME_ZONE_NIGHT_THEME[56] = "The Ancient Region of Dino Speedy Time Zone Night Theme";
char STR_BGM_TRACK_THE_ANCIENT_REGION_OF_DINO_GLACIAL_TIME_ZONE_THEME[] = "The Ancient Region of Dino Glacial Time Zone Theme";
char STR_BGM_TRACK_THE_ANCIENT_REGION_OF_DINO_GLACIAL_TIME_ZONE_NIGHT_THEME[] = "The Ancient Region of Dino Glacial Time Zone Night Theme";
char STR_BGM_TRACK_FREEZELAND_THEME[] = "Freezeland Theme";
char STR_BGM_TRACK_FREEZELAND_NIGHT_THEME[] = "Freezeland Night Theme";
char STR_BGM_TRACK_IGLOO_THEME[12] = "Igloo Theme";
char STR_BGM_TRACK_CURLING_THEME[] = "Curling Theme";
char STR_BGM_TRACK_SANCTUARY_THEME[16] = "Sanctuary Theme";
char STR_BGM_TRACK_SANCTUARY_BELOW_THEME[] = "Sanctuary Below Theme";
char STR_BGM_TRACK_GECKO_SWAMP_THEME[] = "Gecko Swamp Theme";
char STR_BGM_TRACK_GECKO_SWAMP_NIGHT_THEME[24] = "Gecko Swamp Night Theme";
char STR_BGM_TRACK_MISTY_TREES_THEME[] = "Misty Trees Theme";
char STR_BGM_TRACK_MISTY_TREES_NIGHT_THEME[24] = "Misty Trees Night Theme";
char STR_BGM_TRACK_WARUMONZAEMON_THEME[20] = "WaruMonzaemon Theme";
char STR_BGM_TRACK_TOY_TOWN_THEME[] = "Toy Town Theme";
char STR_BGM_TRACK_THEME_OF_FACTORIAL[] = "Theme of Factorial";
char STR_BGM_TRACK_FACTORIAL_NIGHT_THEME[] = "Factorial Night Theme";
char STR_BGM_TRACK_SEWER_THEME[12] = "Sewer Theme";
char STR_BGM_TRACK_TRASH_MOUNTAIN_THEME[] = "Trash Mountain Theme";
char STR_BGM_TRACK_TRASH_MOUNTAIN_NIGHT_THEME[] = "Trash Mountain Night Theme";
char STR_BGM_TRACK_BEATLAND_THEME[] = "Beatland Theme";
char STR_BGM_TRACK_BEATLAND_NIGHT_THEME[] = "Beatland Night Theme";
char STR_BGM_TRACK_SECRET_BEACH_CAVE_THEME[24] = "Secret Beach Cave Theme";
char STR_BGM_TRACK_LAST_ROOM_THEME[16] = "Last Room Theme";
char STR_BGM_TRACK_FILE_CITY_THEME[16] = "File City Theme";
char STR_BGM_TRACK_FILE_CITY_NIGHT_THEME[] = "File City Night Theme";
char STR_BGM_TRACK_TORNAMENT_OPENING_THEME[24] = "Tornament Opening Theme";
char STR_BGM_TRACK_TORNAMENT_PROGRESS_THEME[] = "Tornament Progress Theme";
char STR_BGM_TRACK_TORNAMENT_CHAMPIONSHIP_THEME[] = "Tornament Championship Theme";
char STR_BGM_TRACK_PARTNERS_ENTRANCE_THEME[] = "Partner's Entrance Theme";
char STR_BGM_TRACK_COMPETITION_BATTLE_OPPONENTS_ENTRANCE_THEME[] = "Competition Battle Opponent's Entrance Theme";
char STR_BGM_TRACK_ARENA_BATTLE_THEME_NO_1[] = "Arena Battle Theme No. 1";
char STR_BGM_TRACK_PARTNERS_WIN_THEME[20] = "Partner's Win Theme";
char STR_BGM_TRACK_PARTNERS_LOSS_THEME[] = "Partner's Loss Theme";
char STR_BGM_TRACK_ARENA_BATTLE_THEME_NO_2[] = "Arena Battle Theme No. 2";
char STR_BGM_TRACK_ARENA_BATTLE_THEME_NO_3[] = "Arena Battle Theme No. 3";
char STR_BGM_TRACK_EVENT_BATTLE_THEME[] = "Event Battle Theme";
char STR_BGM_TRACK_NORMAL_BATTLE_THEME[20] = "Normal Battle Theme";
char STR_BGM_TRACK_NORMAL_BATTLE__THEME_NO2[] = "Normal Battle  Theme No.2";
char STR_BGM_TRACK_LAST_BATTLE_THEME[] = "Last Battle Theme";
char STR_TOURNAMENT_NAME_VERSION_1_CUP[] = "Version 1 Cup";
char STR_TOURNAMENT_NAME_VERSION_2_CUP[] = "Version 2 Cup";
char STR_TOURNAMENT_NAME_VERSION_3_CUP[] = "Version 3 Cup";
char STR_TOURNAMENT_NAME_VERSION_4_CUP[] = "Version 4 Cup";
char STR_TOURNAMENT_NAME_VERSION_0_CUP[] = "Version 0 Cup";
char STR_TOURNAMENT_NAME_FIRE_CUP[] = "Fire Cup";
char STR_TOURNAMENT_NAME_GRAPPLE_CUP[12] = "Grapple Cup";
char STR_TOURNAMENT_NAME_THUNDER_WIND_CUP[] = "Thunder Wind Cup";
char STR_TOURNAMENT_NAME_COOL_CUP[] = "Cool Cup";
char STR_TOURNAMENT_NAME_NATURE_CUP[] = "Nature Cup";
char STR_TOURNAMENT_NAME_METALIC_CUP[12] = "Metalic Cup";
char STR_TOURNAMENT_NAME_FILTH_CUP[] = "Filth Cup";
char STR_TOURNAMENT_NAME_DINO_CUP[] = "Dino Cup";
char STR_TOURNAMENT_NAME_WING_CUP[] = "Wing Cup";
char STR_TOURNAMENT_NAME_ANIMAL_CUP[] = "Animal Cup";
char STR_TOURNAMENT_NAME_HUMAN_CUP[] = "Human Cup";
char STR_TOURNAMENT_NAME_BEETLE_CUP[] = "Beetle Cup";

GsSPRITE MAIN_D_8012FDC0 = {
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

MenuTextLayout AMOUNT_BOX_LAYOUT = {
	{
		0x001a, 0x0007, 0x0008, 0x004a,
		0x001b, 0x0005, 0x0059, 0x002b,
		0x0002, 0x0040, 0x0041, 0x0006,
		0x0016, 0x002b, 0x0001,
	},
};

MenuTextLayout CONFIRM_BOX_LAYOUT = {
	{
		0x0000, 0x0000, 0x0004, 0x0002,
		0x0060, 0x0000, 0x000c, 0x000e,
		0x0012, 0x0024, 0x0024, 0x000c,
		0x0044, 0x0012, 0x0024,
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

ShopkeeperIdTable MAIN_D_8012FE5C = {
	{ 0x97, 0x99, 0xa3, 0xa5, 0x82, 0x89, 0x87, 0xaa, 0x76, 0x80, 0xff },
};

int16_t MAIN_D_8012FE68[8] = {
	0x0083, 0x0085, 0x0086, 0x008d, 0x00cf, 0x00d3, 0x00d7, 0x00da,
};

uint8_t MAIN_D_8012FE78[78] = {
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
	STR_BGM_TRACK_OGREMON_THEMENO_3,
	STR_BGM_TRACK_LAVA_CAVE_THEME,
	STR_BGM_TRACK_DARK_ARISTCRATS_MANSION_THEME,
	STR_BGM_TRACK_UNDERGROUND_LAB_THEME,
	STR_BGM_TRACK_GEAR_SAVANNA_THEME,
	STR_BGM_TRACK_GEAR_SAVANNA_NIGHT_THEME,
	STR_BGM_TRACK_LEOMON_THEME,
	STR_BGM_TRACK_AMIDA_FOREST_THEME,
	STR_BGM_TRACK_AMIDA_FOREST_NIGHT_THEME,
	STR_BGM_TRACK_THE_ANCIENT_REGION_OF_DINO_SPEEDY_TIME_ZONE_THEME,
	STR_BGM_TRACK_THE_ANCIENT_REGION_OF_DINO_SPEEDY_TIME_ZONE_NIGHT_THEME,
	STR_BGM_TRACK_THE_ANCIENT_REGION_OF_DINO_GLACIAL_TIME_ZONE_THEME,
	STR_BGM_TRACK_THE_ANCIENT_REGION_OF_DINO_GLACIAL_TIME_ZONE_NIGHT_THEME,
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
	STR_BGM_TRACK_MT_PANORAMA_THEME,
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
	STR_BGM_TRACK_NORMAL_BATTLE__THEME_NO2,
	STR_BGM_TRACK_LAST_BATTLE_THEME,
};

int32_t MAIN_D_8012FFC4[5] = {
	0x00001388, 0x000005dc, 0x000001f4, 0x00000064,
	0x00000032,
};

CardData CARD_DATA[66] = {
	{ 0x00, 0x05, 0x00, 0x00 },
	{ 0x3b, 0x00, 0x64, 0x00 },
	{ 0x3c, 0x00, 0x64, 0x00 },
	{ 0x3d, 0x00, 0x64, 0x00 },
	{ 0x77, 0x00, 0x64, 0x00 },
	{ 0x42, 0x00, 0x64, 0x00 },
	{ 0x0c, 0x01, 0x1e, 0x00 },
	{ 0x0d, 0x01, 0x1e, 0x00 },
	{ 0x0e, 0x01, 0x1e, 0x00 },
	{ 0x1a, 0x01, 0x1e, 0x00 },
	{ 0x1b, 0x01, 0x1e, 0x00 },
	{ 0x1c, 0x01, 0x1e, 0x00 },
	{ 0x28, 0x01, 0x1e, 0x00 },
	{ 0x29, 0x01, 0x1e, 0x00 },
	{ 0x2a, 0x01, 0x1e, 0x00 },
	{ 0x36, 0x01, 0x1e, 0x00 },
	{ 0x37, 0x01, 0x1e, 0x00 },
	{ 0x38, 0x01, 0x1e, 0x00 },
	{ 0x6e, 0x01, 0x1e, 0x00 },
	{ 0x46, 0x01, 0x1e, 0x00 },
	{ 0x75, 0x01, 0x1e, 0x00 },
	{ 0x78, 0x01, 0x1e, 0x00 },
	{ 0x79, 0x01, 0x1e, 0x00 },
	{ 0x5c, 0x01, 0x1e, 0x00 },
	{ 0x7a, 0x01, 0x1e, 0x00 },
	{ 0x7e, 0x01, 0x1e, 0x00 },
	{ 0x05, 0x02, 0x0a, 0x00 },
	{ 0x06, 0x02, 0x0a, 0x00 },
	{ 0x07, 0x02, 0x0a, 0x00 },
	{ 0x08, 0x02, 0x0a, 0x00 },
	{ 0x09, 0x02, 0x0a, 0x00 },
	{ 0x0a, 0x02, 0x0a, 0x00 },
	{ 0x13, 0x02, 0x0a, 0x00 },
	{ 0x14, 0x02, 0x0a, 0x00 },
	{ 0x15, 0x02, 0x0a, 0x00 },
	{ 0x16, 0x02, 0x0a, 0x00 },
	{ 0x17, 0x02, 0x0a, 0x00 },
	{ 0x18, 0x02, 0x0a, 0x00 },
	{ 0x21, 0x02, 0x0a, 0x00 },
	{ 0x22, 0x02, 0x0a, 0x00 },
	{ 0x23, 0x02, 0x0a, 0x00 },
	{ 0x24, 0x02, 0x0a, 0x00 },
	{ 0x25, 0x02, 0x0a, 0x00 },
	{ 0x26, 0x02, 0x0a, 0x00 },
	{ 0x2f, 0x02, 0x0a, 0x00 },
	{ 0x30, 0x02, 0x0a, 0x00 },
	{ 0x31, 0x02, 0x0a, 0x00 },
	{ 0x32, 0x02, 0x0a, 0x00 },
	{ 0x33, 0x02, 0x0a, 0x00 },
	{ 0x34, 0x02, 0x0a, 0x00 },
	{ 0x3a, 0x02, 0x0a, 0x00 },
	{ 0x39, 0x03, 0x05, 0x00 },
	{ 0x6d, 0x03, 0x05, 0x00 },
	{ 0x6f, 0x03, 0x05, 0x00 },
	{ 0x43, 0x03, 0x05, 0x00 },
	{ 0x44, 0x03, 0x05, 0x00 },
	{ 0x45, 0x03, 0x05, 0x00 },
	{ 0x54, 0x03, 0x05, 0x00 },
	{ 0x7f, 0x03, 0x05, 0x00 },
	{ 0x4c, 0x03, 0x05, 0x00 },
	{ 0x50, 0x03, 0x05, 0x00 },
	{ 0x0b, 0x04, 0x01, 0x00 },
	{ 0x19, 0x04, 0x01, 0x00 },
	{ 0x27, 0x04, 0x01, 0x00 },
	{ 0x35, 0x04, 0x01, 0x00 },
	{ 0x00, 0x05, 0x00, 0x00 },
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
};

uint8_t TOURNAMENT_DATA[180] = {
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

BattleEntry MAIN_D_8013024C[6] = {
	{ 0x26, 0x09, 0x00dd, 0x000003e8 },
	{ 0x46, 0x09, 0x00be, 0x000003e8 },
	{ 0x4f, 0x09, 0x00bc, 0x000005dc },
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

RECT ITEM_MENU_DESCRIPTION_RECTS[8] = {
	{ 5, 17, 170, 18 },
	{ 5, 17, 202, 18 },
	{ 5, 17, 202, 18 },
	{ 5, 17, 170, 18 },
	{ 5, 17, 202, 18 },
	{ 5, 17, 146, 18 },
	{ 5, 17, 146, 18 },
	{ 5, 17, 170, 18 },
};

int16_t SELECTION_CURSOR_WIDTHS[8] = {
	0x00aa, 0x00ca, 0x00ca, 0x00aa, 0x00ca, 0x0092, 0x0092, 0x00aa,
};

char MAIN_D_8013030C[] = "Coelamon";

int16_t MAIN_D_80130318[22] = {
	0x03e7, 0x03e7, 0x03e7, 0x03e7, 0x270f, 0x270f, 0x270f, 0x270f,
	0x0064, 0x0064, 0x03e7, 0x03e7, 0x03e7, 0x03e7, 0x270f, 0x270f,
	0x270f, 0x03e7, 0x03e7, 0x03e7, 0x0063, 0x000a,
};

Pow10Table MAIN_D_80130344 = {
	{
		0x00000001, 0x0000000a, 0x00000064, 0x000003e8,
		0x00002710, 0x000186a0,
	},
};

char *MAIN_D_8013035C[6] = {
	MAIN_D_801345CC,
	MAIN_D_801345D4,
	MAIN_D_801345D8,
	MAIN_D_8013030C,
	MAIN_D_801345E0,
	MAIN_D_801345E8,
};
// clang-format on

int32_t shopFillBuyItemList()
{
	uint8_t itemId;
	uint8_t slotId;
	int32_t hasSpace;
	uint8_t *itemList;
	uint32_t inventorySize;
	int32_t canBuyAnything;
	inventorySize = INVENTORY.size;
	itemList = ITEM_MENU_LEFT->buf;
	canBuyAnything = 0;
	hasSpace = 0;
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
	int32_t type;
	int32_t amount;

	result = 0;
	buf = ITEM_MENU_RIGHT->buf;
	ITEM_MENU_RIGHT->itemCount = 0;

	for (i = 0; i < INVENTORY.size; i++) {
		type = INVENTORY.types.array[i];
		amount = INVENTORY.amounts.array[i];

		if (type != 0xff) {
			if (ITEM_PARA[type].droppable != 0) {
				amount = (amount | 0x80) & 0xff;
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
	RECT *src;

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

	if (MAIN_D_8013500C != 0) {
		return;
	}

	if (SELECTION_MENU_STATE != 1) {
		return;
	}

	if (isKeyDown(0x40)) {
		src = &ITEM_MENU_DESCRIPTION_RECTS[ITEM_MENU_TYPE];
		setRECT(&rect, src->x, src->y, src->w, src->h);

		switch (ITEM_MENU_TYPE) {
		default:
			createItemMenuAmountBox(&rect);
			break;
		case 7:
			readSelectedItemMerit();
			break;
		case 5:
			createSingleCardShopMenu(&rect);
			break;
		}
	} else if (isKeyDown(0x10)) {
		if (ITEM_MENU_TYPE == 7) {
			SELECTION_MENU_STATE = 0xa;
		} else if (ITEM_MENU_TYPE == 5) {
			if (isTriggerSet(3) != 0) {
				writePStat(0xfe, 0xff);
				unsetTrigger(3);
				SELECTION_MENU_STATE = 4;
			}
		} else {
			SELECTION_MENU_STATE = 8;
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
		src = &ITEM_MENU_DESCRIPTION_RECTS[ITEM_MENU_TYPE];
		setRECT(&rect, src->x, src->y, src->w, src->h);
		createItemMenuDescriptionBox(box, &rect, 1);
		playSound(0, 3);
	}
}

int32_t readSelectedItemMerit(void)
{
	ItemMenuBox *box;
	int32_t idx;

	box = getItemMenuFromType();
	idx = (box->topRow + box->cursor) * 2;
	SHOP_ITEM_TYPE = box->buf[idx];

	if (SHOP_ITEM_TYPE != 0xff && box->buf[idx + 1] != 0) {
		MAIN_D_8013500C = ITEM_PARA[SHOP_ITEM_TYPE].meritValue;
		SELECTION_MENU_STATE = 0xb;
		SCRIPT_STATE_3 = 0;
		playSound(0, 3);
		return 1;
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

	if (isKeyDown(0x850) == 0) {
		return;
	}

	triggerBoxCloseFlag(3);
	playSound(0, 3);
}

void renderItemMenuDescriptionBox(void)
{
	renderString(0, (int16_t)(UI_BOX_DATA[3].finalPos.x + 6),
	             (int16_t)(UI_BOX_DATA[3].finalPos.y + 5), 0xfc, 0xc, 0,
	             (int16_t)(TEXT_BOX_DATA[3].vramRow * 12), 3, 1);
}

void renderItemMenuAmountBox(void)
{
	MenuTextLayout layout;
	int32_t srcCol;
	int16_t *entry;
	CardData *card;
	int32_t i;
	int16_t rowPx;
	int16_t y;
	int16_t x;
	int16_t sx;
	int16_t sy;

	layout = AMOUNT_BOX_LAYOUT;
	rowPx = TEXT_BOX_DATA[3].vramRow * 12;
	x = UI_BOX_DATA[3].finalPos.x;
	y = UI_BOX_DATA[3].finalPos.y;
	renderHorizontalLine(3, 4, 0x17, 0x7a);
	renderHorizontalLine(3, 0xc, 0x3c, 0x6a);
	renderInsetWithoutBox(3, 0x55, 0x2a, 0x1a, 0xe);
	sx = x + 8;
	sy = y + 5;

	if (ITEM_MENU_TYPE < 3) {
		renderItemSprite(SHOP_ITEM_TYPE, sx, sy, 3);
	} else {
		card = (CardData *)(SHOP_ITEM_TYPE * 4);
		card = (CardData *)((uint32_t)card + (uint32_t)CARD_DATA);
		sx += 2;
		sy += 2;
		renderCardSprite(card->spriteId, sx, sy, 3);
	}

	entry = layout.v;
	i = 0;
	srcCol = 0;
	while (i < 4) {
		renderString(0, entry[0] + x, entry[1] + y, entry[2] * 12,
		             0xc, srcCol * 12, rowPx, 3, 1);
		i++;
		srcCol = (int16_t)(srcCol + entry[2]);
		entry += 3;
	}
}

void renderItemMenu(void)
{
	int16_t bx;
	int16_t by;
	int16_t cy;
	ItemMenuBox *box;

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
	cy = by + box->cursor * 0x12 + 0x11;
draw:
	renderSelectionCursor(bx + 5, cy, SELECTION_CURSOR_WIDTHS[ITEM_MENU_TYPE], 0x12, 5);
	renderItemMenuItemList(box, bx + 0x1a, by + 0x13, bx + 8, by + 0x12, 0);
}

void updateItemMenuAmountBoxString(void)
{
	uint8_t *out;
	char *name;
	uint32_t total;

	out = TEXT_BUFFERS_PTR + (ITEM_MENU_SUB_TEXTBOX_LINE << 6);

	if (ITEM_MENU_TYPE < 3) {
		strcpy(out, ITEM_PARA[SHOP_ITEM_TYPE].name);
		out += strlen(ITEM_PARA[SHOP_ITEM_TYPE].name);
	} else {
		strcpy(out, name = DIGIMON_DATA[CARD_DATA[SHOP_ITEM_TYPE].digimonId].name);
		out += strlen(name);
	}

	*out++ = 0x18;
	*out++ = 0;
	out = intToStringSJIS(out, SHOP_ITEM_PRICE, 5, 0);
	*out++ = 0x19;
	*out++ = 0;
	out = intToStringSJIS(out, SHOP_AMOUNT, 2, 0);
	*out++ = 0x1a;
	*out++ = 0;

	total = SHOP_ITEM_PRICE * SHOP_AMOUNT;
	if (total >= 0xf4240) {
		total = 0xf423f;
	}

	out = intToStringSJIS(out, total, 6, 0);
	*out++ = 0;
	*out = 0;
	TEXT_BOX_DATA[3].pageReady = 1;
	TEXT_BOX_DATA[3].writeCount++;
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

	if (isKeyDown(0x10)) {
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

	if (isKeyDown(0x40)) {
		triggerBoxCloseFlag(3);
		if (SHOP_AMOUNT == 0) {
			if (ITEM_MENU_TYPE != 5) {
				box = getItemMenuFromType();
				amount = getCardAmount(SHOP_ITEM_TYPE);
				amount = amount + 1u;
				setCardAmount(SHOP_ITEM_TYPE, amount);
				MONEY -= MAIN_D_8012FFC4[CARD_DATA[SHOP_ITEM_TYPE].spriteId];
				SCRIPT_STATE_PTR->smth[box->cursor] = 0xff;
				UPDATE_SHOP_BIT_BOX = 1;
				SELECTION_MENU_STATE = 4;
				playShopSoundOnlyInSavannah();
			} else {
				writePStat(0xfe, SHOP_ITEM_TYPE);
				unsetTrigger(3);
				SELECTION_MENU_STATE = 4;
				playSound(0, 3);
			}
		} else {
			playSound(0, 4);
		}
	}
}

void renderSingleCardShop(void)
{
	MenuTextLayout layout;
	int16_t *entry;
	int16_t rowPx;
	int16_t x;
	int16_t y;
	int32_t i;

	layout = CONFIRM_BOX_LAYOUT;
	entry = layout.v;
	rowPx = TEXT_BOX_DATA[3].vramRow * 12;
	x = UI_BOX_DATA[3].finalPos.x + 4;
	y = UI_BOX_DATA[3].finalPos.y + 3;
	i = 0;
	while (i < 3) {
		renderString(0, x + entry[2], y + entry[3], entry[4], 0xc,
		             entry[0], rowPx + entry[1], 3, 1);
		i++;
		entry += 5;
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
	int32_t section;
	int32_t pstat;
	int32_t i;
	int32_t npcType;

	shopkeeperIds = MAIN_D_8012FE5C;
	pstat = readPStat(0xfe) & 0xff;

	if (pstat == 0xff) {
		section = 0x4ce;
	} else {
		pstat = scriptIdToEntityId(pstat) & 0xff;
		if (pstat != 0xff) {
			npcType =
				*(int32_t *)(&NPC_ENTITIES[pstat - 2]) & 0xff;
			for (i = 0; (uint32_t)i < 0xb; i++) {
				if (npcType == shopkeeperIds.b[i]) {
					section = (i + 0x4c4) & 0xffff;
					goto done;
				}
			}
		}
		section = 0x4c4;
	}
done:
	return resolveMapHeadEntry(section, idx);
}

uint8_t *resolveMapHeadEntry(int32_t section, int32_t idx)
{
	uint8_t *script;
	uint8_t *sectionPtr;
	uint8_t *offsetPtr;

	script = getScript(0);
	sectionPtr = getScriptSection(script, section);
	offsetPtr = sectionPtr + idx * 4;
	offsetPtr = (uint8_t *)(offsetPtr + 2);

	return script + *(uint16_t *)offsetPtr + 2;
}

void renderMonochromonMoodBubble(int32_t instanceId)
{
	SVECTOR pos;
	DVECTOR screen;
	uint8_t entityId;
	int32_t depth;
	int16_t offset;
	int16_t scale;

	entityId = readPStat(0xf7);
	readPStat(0xf8);
	entityId = scriptIdToEntityId(entityId);
	if (entityId == 0xff) {
		return;
	}

	translateConditionFXToEntity(ENTITY_TABLE[entityId], &pos);
	depth = worldPosToScreenPos(&pos, &screen);

	offset = 0x10 - (MAIN_D_80134F60 >> 1);
	scale = offset << 8;
	MAIN_D_8012FDC0.x = screen.vx;
	MAIN_D_8012FDC0.y = screen.vy - offset;
	MAIN_D_8012FDC0.scalex = scale;
	MAIN_D_8012FDC0.scaley = scale;
	GsSortSprite(&MAIN_D_8012FDC0, ACTIVE_ORDERING_TABLE, depth >> 4);

	if (MAIN_D_80134F60 != 0) {
		MAIN_D_80134F60--;
	}
}

void inputInit(void)
{
	MAIN_D_80135024 = 0;
	MAIN_D_80135020 = -1;
	MAIN_D_8013501C = 0;
	MAIN_D_80135016 = 0;
}

void processInput(void)
{
	uint32_t held;

	MAIN_D_80135020 |= ~POLLED_INPUT;
	held = POLLED_INPUT & MAIN_D_8013501C;
	if (held == 0) {
		MAIN_D_80135016 = 0;
	}

	if (MAIN_D_8013501C != 0) {
		if (held == (POLLED_INPUT_PREVIOUS & MAIN_D_8013501C)) {
			if (MAIN_D_80135016 == 8) {
				MAIN_D_80135016 = 6;
			} else if (MAIN_D_80135016 != 6) {
				held = 0;
			}

			MAIN_D_80135016 += 1;
		} else {
			MAIN_D_80135016 = 0;
			MAIN_D_80135020 |= MAIN_D_8013501C;
			held = 0;
		}
	}

	MAIN_D_80135024 = held | (POLLED_INPUT & MAIN_D_80135020);
}

int32_t isKeyDown(uint32_t key)
{
	if ((MAIN_D_80135024 & key) == 0) {
		return 0;
	}

	MAIN_D_80135020 &= ~key;

	return 1;
}

void setFreshDialogue(void)
{
	MAIN_D_80135028 = 1;
}

int32_t isXPressedAfterDialogue(void)
{
	if (MAIN_D_80135028 != 0) {
		if ((POLLED_INPUT & 0x40) != 0) {
			return 0;
		}

		MAIN_D_80135028 = 0;
	}
	return 1;
}

void setInputRepeatMask(uint32_t mask)
{
	MAIN_D_8013501C = mask;
}

void handleItemLoss(void)
{
	uint8_t *pool;
	uint8_t size;
	uint8_t k;
	int32_t count;
	int32_t lose;
	uint8_t n;
	uint8_t slot;
	uint8_t recycleId;
	int32_t idx;

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
			idx = recycleId + 6;
			if (SCRIPT_STATE_PTR->smth[idx] >= 0x64) {
				SCRIPT_STATE_PTR->smth[idx] = 0x63;
			}
		}

		removeItem(INVENTORY.types.array[slot], 0x63);
	}

	freeArray((uint32_t *)pool);
}

uint8_t getRecycleId(uint8_t value)
{
	uint8_t i;

	for (i = 0; i < 0x4e; i++) {
		if (value == MAIN_D_8012FE78[i]) {
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
		v = readPStat(i & 0xff);
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
	int32_t partnerType;

	babyTypes = MAIN_D_801345B4;
	partnerType = PARTNER_ENTITY.digimonEntity.entity.type & 0xff;

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
	uint8_t qty;
	int32_t unitPrice;
	int32_t savings;
	int8_t q;

	if (UI_BOX_DATA[3].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (SHOP_AMOUNT != MAX_SHOP_AMOUNT && isKeyDown(0x1000)) {
		if (POLLED_INPUT & 0x20) {
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
		if (POLLED_INPUT & 0x20) {
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

	if (isKeyDown(0x10)) {
		triggerBoxCloseFlag(3);
		playSound(0, 4);
	} else if (isKeyDown(0x80)) {
		SHOP_AMOUNT = MAX_SHOP_AMOUNT;
		playSound(0, 3);
	} else if (isKeyDown(0x40)) {
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
				savings = isPartnerBaby();
				if (savings != 0) {
					unitPrice = unitPrice * 90 / 100;
					savings = SHOP_ITEM_PRICE;
					savings -= unitPrice;
					MAIN_D_8013500C =
						SHOP_AMOUNT * savings;
				}

				qty = SHOP_AMOUNT;
				MONEY -= qty * unitPrice;
				giveItem(SHOP_ITEM_TYPE, qty);

				if (ITEM_MENU_TYPE == 2) {
					owner = getRecycleId(SHOP_ITEM_TYPE);
					SCRIPT_STATE_PTR->smth[owner + 6] -=
						SHOP_AMOUNT;
				}

				owner = 0xfd;
			}
			SELECTION_MENU_STATE = 7;
		} else {
			MONEY += SHOP_AMOUNT * SHOP_ITEM_PRICE;
			if (MONEY >= 0xf4240) {
				MONEY = 0xf423f;
			}

			owner = getCardAmount(SHOP_ITEM_TYPE);
			owner = (uint32_t)owner - SHOP_AMOUNT;
			setCardAmount(SHOP_ITEM_TYPE, owner);
			owner = readPStat(0xfe);
			SELECTION_MENU_STATE = 4;
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
	int32_t idx;
	int32_t row0;
	int32_t item;
	int32_t len;

	out = getTextboxLine((uint8_t *)box, row);
	*out++ = 1;
	*out++ = 7;

	row0 = row;
	row0 = row0;
	idx = ITEM_MENU_TYPE + ((ITEM_MENU_LEFT->topRow + 5) + row);

	if (isTriggerSet(idx) != 0) {
		*out++ = 0x81;
		*out++ = 0x7c;
	} else {
		*out++ = 0x81;
		*out++ = 0x40;
	}

	idx = (box->topRow + row0) * 2;
	item = ITEM_MENU_LEFT->buf[idx];
	*out++ = 1;

	if ((item & 0x80) != 0) {
		*out++ = 1;
	} else {
		*out++ = 3;
	}

	item = (uint8_t)(item & 0x7f);
	strcpy((char *)out, ITEM_PARA[item].name);
	len = strlen(ITEM_PARA[item].name);
	out += len;
	out = padWithSpaces(out, 8, len);
	*out++ = 0xf;
	*out++ = 0;
	*out++ = 1;
	*out++ = 1;

	item = ITEM_MENU_RIGHT->buf[idx];
	strcpy((char *)out, ITEM_PARA[item].name);
	out += strlen(ITEM_PARA[item].name);
	terminateString(out, isLast);
}

void openShop(void)
{
	int32_t npcId;
	int32_t i;
	int32_t trig;
	int32_t found;

	npcId = readPStat(0xfe) & 0xff;

	switch (SELECTION_MENU_STATE) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_LEFT, 0x100, 6, 0xb2, 0x18,
		                    6, 0x5a);
		allocateItemMenuBox(&ITEM_MENU_RIGHT,
		                    INVENTORY.size << 1, 6, 0xd2, 0x18,
		                    6, 0x5a);
		MAIN_D_80134F70 = 0;

		found = 0;
		for (i = 0, trig = 0x180; i < 0x80; i++, trig++) {
			if (isTriggerSet(trig)) {
				found = 1;
				break;
			}
		}

		MAIN_D_80134F74 = 0;
		if (found) {
			showShopkeeperTextbox(0, npcId, 0);
			SELECTION_MENU_STATE = 1;
			SCRIPT_STATE_4 = 3;
			SCRIPT_STATE_3 = 1;
		} else {
			showShopkeeperTextbox(1, npcId, 0);
			SELECTION_MENU_STATE = 1;
			SCRIPT_STATE_4 = 2;
			SCRIPT_STATE_3 = 1;
		}
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&ITEM_MENU_RIGHT);
		destroyItemMenuBox(&ITEM_MENU_LEFT);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		createShopBitsBox(1);
		showShopkeepSelection(2, 0xfd, 3, &MAIN_D_80134F70);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 4;
		SCRIPT_STATE_3 = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 0;
		shopFillBuyItemList();
		createItemMenu();
		showShopkeeperTextbox(8, npcId, 0);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 5:
		setInputRepeatMask(0x5000);
		ITEM_MENU_TYPE = 1;
		shopFillSellItemList();
		createItemMenu();
		showShopkeeperTextbox(9, npcId, 0);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 6:
		triggerBoxCloseFlag(2);

		if (MAIN_D_80134F74) {
			showShopkeeperTextbox(4, npcId, 0);
		} else {
			showShopkeeperTextbox(5, npcId, 0);
		}

		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	case 7:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);

		if (ITEM_MENU_TYPE == 0 && isPartnerBaby()) {
			showShopkeeperTextbox(0xd, npcId, 0);
		} else {
			showShopkeeperTextbox(0xa, npcId, 0);
		}

		SCRIPT_STATE_4 = 3;
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 1;
		MAIN_D_80134F74 = 1;
		break;
	case 8:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		showShopkeeperTextbox(0xb, npcId, 0);
		SELECTION_MENU_STATE = 9;
		break;
	case 9:
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	}
}

void allocateItemMenuBox(ItemMenuBox **box, int32_t bufSize, int32_t rows,
                         int32_t x, uint8_t y, uint8_t w, uint8_t h)
{
	*box = (ItemMenuBox *)allocateArray(0x20);
	(*box)->buf = allocateArray(bufSize);
	(*box)->isOpen = 0;
	(*box)->visibleRows = rows;
	(*box)->rect.x = x;
	(*box)->rect.y = y;
	setWH(&(*box)->rect, w, h);
}

void showShopkeeperTextbox(int32_t idx, int32_t owner, int32_t boxId)
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

	UPDATE_SHOP_BIT_BOX = 1;
	BIT_BOX_SHOW_BITS = showBits;
	if (UI_BOX_DATA[2].state == 1) {
		return;
	}

	setupBoxOrigin(0xfd, &origin);
	setRECT(&rect, -0x98, -0x62, 0x52, 0x21);
	createTextbox(2, 0xe1, &rect, &origin, tickShopBitsBox,
	              renderShopBitsBox);
	registerTextbox(2, 8, 1, 0, 0);
	tickShopBitsBox();
}

void showShopkeepSelection(int32_t idx, int32_t owner, int32_t boxId,
                           int32_t *outSelection)
{
	showMapheadSelection(idx, owner, boxId, outSelection, 0xff);
}

void createItemMenu(void)
{
	RECT rect;
	RECT origin;
	int32_t boxId;
	ItemMenuBox *result;
	int16_t *dims;

	if (ITEM_MENU_TYPE == 1 || ITEM_MENU_TYPE == 5) {
		boxId = 0xfd;
	} else {
		boxId = readPStat(0xfe) & 0xff;
	}

	setupBoxOrigin(boxId, &origin);
	result = getItemMenuFromType();
	dims = (int16_t *)((uint8_t *)ITEM_MENU_POS + ITEM_MENU_TYPE * 8);
	setRECT(&rect, dims[0], dims[1], dims[2], dims[3]);
	createTextbox(1, 0xf1, &rect, &origin, tickItemMenu,
	              renderItemMenu);
	registerTextbox(1, 9, 6, 1, 0);
	initItemMenuBox(result, 1, 9);
	updateItemMenuStrings(result, 9, 0);
	MAIN_D_8013500C = 0;
}

void openDiscardItem(void)
{
	switch (SELECTION_MENU_STATE) {
	case 0:
		allocateItemMenuBox(&ITEM_MENU_RIGHT,
		                    INVENTORY.size << 1, 6, 0x9a, 0x18,
		                    6, 0x5a);

		if (shopFillSellItemList()) {
			SELECTION_MENU_STATE = 3;
			SCRIPT_STATE_3 = 0;
		} else {
			setTrigger(3);
			writePStat(0xfe, 0xff);
			SELECTION_MENU_STATE = 2;
			SCRIPT_STATE_3 = 0;
		}
		break;
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
		SELECTION_MENU_STATE = 1;
		break;
	case 4:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		SELECTION_MENU_STATE = 2;
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

void initItemMenuBox(ItemMenuBox *box, int32_t boxId, int32_t startRow)
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
	uint8_t *p;
	int32_t rows;
	int32_t last;
	int32_t i;

	entry = &MAIN_D_801BE80C.box[box->boxId];
	rows = box->itemCount - box->topRow;
	if (box->visibleRows < rows) {
		rows = box->visibleRows;
	}

	if (rows != 0) {
		i = 0;
		last = startRow + rows - 1;
		while (i < rows) {
			if (box->itemRow[i] == last) {
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

			i++;
		}
	} else {
		p = TEXT_BUFFERS_PTR + (startRow << 6);
		if (entry->vramMode == 2) {
			p += 0x20;
		}

		if (entry->doubleBuffered == 1) {
			p = (uint8_t *)(p + (((entry->backPage ^ 1) * entry->vramRows) << 6));
		}

		*p++ = 0;
		*p = 0;
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
	RECT rect;
	int32_t idx;
	int16_t boxY;
	uint8_t id;
	uint8_t amount;
	uint8_t room;

	box = getItemMenuFromType();
	idx = (box->topRow + box->cursor) * 2;
	SHOP_ITEM_TYPE = box->buf[idx];
	if (SHOP_ITEM_TYPE == 0xff) {
		goto fail;
	}

	id = SHOP_ITEM_TYPE;
	amount = box->buf[idx + 1];

	if (ITEM_MENU_TYPE < 3) {
		SHOP_ITEM_PRICE = ITEM_PARA[id].value;
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

			room = 0x63 - getItemCount(id);
			if (room < MAX_SHOP_AMOUNT) {
				MAX_SHOP_AMOUNT = room;
			}

			idx = SHOP_ITEM_PRICE;
			if (MAX_SHOP_AMOUNT * idx > MONEY) {
				MAX_SHOP_AMOUNT = MONEY / SHOP_ITEM_PRICE;
			}
		}
	} else {
		SHOP_ITEM_PRICE =
			MAIN_D_8012FFC4[CARD_DATA[id].spriteId] >> 1;
		MAX_SHOP_AMOUNT = amount;
	}

	SHOP_AMOUNT = 1;
	MAIN_D_80134F82 = 1;
	updateItemMenuSubTextboxLine();
	boxY = UI_BOX_DATA[1].finalPos.y;
	origin->x += UI_BOX_DATA[1].finalPos.x;
	origin->y += (boxY + box->cursor * 18);
	setRECT(&rect, -0x41, -0x2a, 0x82, 0x53);
	createTextbox(3, 0xc1, &rect, origin, tickItemMenuAmountBox,
	              renderItemMenuAmountBox);
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
	RECT rect;
	int32_t idx;
	int16_t boxY;
	uint8_t amount;

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
	boxY = UI_BOX_DATA[1].finalPos.y;
	origin->x += UI_BOX_DATA[1].finalPos.x;
	origin->y += (boxY + box->cursor * 18);
	setRECT(&rect, -0x38, -0x15, 0x70, 0x2a);
	createTextbox(3, 0xc1, &rect, origin, tickSingleCardShop,
	              renderSingleCardShop);
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
	int32_t d;

	box->cursor += 1;

	if (box->topRow + box->cursor < box->itemCount) {
		if (box->cursor == box->visibleRows) {
			box->topRow += 1;
			box->cursor -= 1;
			k = 1;
			first = box->itemRow[0];
			d = 0;

			while (k < box->visibleRows) {
				box->itemRow[d] = box->itemRow[k];
				k++;
				d++;
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

int32_t createItemMenuDescriptionBox(ItemMenuBox *box, RECT *origin, int32_t uiBoxId)
{
	RECT rect;
	uint8_t *out;
	uint8_t item;
	int16_t boxY;

	item = box->buf[(box->topRow + box->cursor) * 2];
	if (item == 0xff) {
		return 0;
	}

	updateItemMenuSubTextboxLine();
	boxY = UI_BOX_DATA[uiBoxId].finalPos.y;
	origin->x += UI_BOX_DATA[uiBoxId].finalPos.x;
	origin->y += (boxY + box->cursor * 18);
	setRECT(&rect, -0x84, -0xb, 0x108, 0x16);
	createTextbox(3, 0xc1, &rect, origin, tickItemMenuDescriptionBox,
	              renderItemMenuDescriptionBox);
	registerTextbox(3, ITEM_MENU_SUB_TEXTBOX_LINE, 1, 0, 0);
	strcpy((out = TEXT_BUFFERS_PTR + (ITEM_MENU_SUB_TEXTBOX_LINE << 6), out),
	       ITEM_DESC_PTR[item]);
	out += strlen(ITEM_DESC_PTR[item]);
	*out++ = 0;
	*out = 0;
	TEXT_BOX_DATA[3].pageReady = 1;
	TEXT_BOX_DATA[3].writeCount++;

	return 1;
}

void renderItemMenuSprite(int32_t boxId, int32_t idx, int16_t x, int16_t y)
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

	a = MAIN_D_80134F60;
	b = MAIN_D_80134F82;
	a = (a * 100) / (b + 0);
	a = (a * 100) / (b + 1);
	a = (a * 100) / (b + 2);
	a = (a * 100) / (b + 3);
	a = (a * 100) / (b + 4);
	MAIN_D_80134F60 = a;
}

void renderItemMenuScrollBar(ItemMenuBox *box)
{
	int16_t track;
	POLY_F4 *prim;
	int16_t boxId;
	int16_t h;
	int16_t x;
	int16_t rows;
	int16_t y;
	GsOT_TAG *otp;
	int16_t thumbY;
	int32_t bottom;
	int32_t offset = 0;
	int32_t scale = 1;
	int16_t w;

	boxId = box->boxId;
	x = box->rect.x + UI_BOX_DATA[boxId].finalPos.x;
	y = box->rect.y + UI_BOX_DATA[boxId].finalPos.y;
	boxId = 6 - boxId;
	otp = ACTIVE_ORDERING_TABLE->org + boxId;
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

	bottom = y + h;
	drawLine3P(0x20202, x + w + 1, y, x, y, x, bottom + offset, boxId, 0);
	drawLine3P(0xa08769, x, y + h + 1, x + w + 1, y + h + 1, x + w + 1, y + 1, boxId, 0);
	drawLine3P(0xa08769, x + offset + w * scale, thumbY, x + 1, thumbY, x + 1, thumbY + track - 2, boxId, 0);
	drawLine3P(0x20202, x + 1, thumbY + track - 1, x + offset + w * scale, thumbY + track - 1, x + offset + w * scale, thumbY + 1, boxId, 0);

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

void renderItemMenuItemList(ItemMenuBox *box, int16_t x1, int16_t y1, int16_t x2,
                            int16_t y2, int32_t mode)
{
	TextBoxData *tbox;
	int16_t boxId;
	int32_t off;
	int32_t outX;
	int32_t outW;
	int32_t i;
	int32_t count;
	int32_t top;
	int32_t order;
	int32_t order2;
	int32_t yy2;
	int16_t texY;
	uint8_t item;
	uint8_t row;

	boxId = box->boxId;
	tbox = &MAIN_D_801BE80C.box[boxId];
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
		off = top * 2;
		order = 6 - boxId;
		for (i = 0, yy2 = y2 + 2; i < count; i++, off += 2, top++, y2 += 0x12, yy2 += 0x12) {
			item = box->buf[off];
			if (item != 0xff) {
				if (mode == 0) {
					renderItemSprite(item, x2, y2, order);
				} else if (mode == 1) {
					uint8_t spriteId = CARD_DATA[item].spriteId;
					renderCardSprite(spriteId, x2 + 2, yy2, order);
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
	for (i = 0, order2 = 6 - boxId; i < count; i++, y1 += 0x12) {
		if (box->buf[(box->topRow + i) * 2] != 0xff) {
			row = box->itemRow[i];
			renderString(0, x1, y1, outW, 0xc, outX, (int16_t)(texY + row * 12), order2, 1);
		}
	}
}

void updateItemMenuSubTextboxLine(void)
{
	uint8_t *box = (uint8_t *)TEXT_BOX_DATA;

	ITEM_MENU_SUB_TEXTBOX_LINE = *(int32_t *)(box + 0x20);
	ITEM_MENU_SUB_TEXTBOX_LINE = (*(int32_t *)(box + 0x18) ^ 1) * *(int32_t *)(box + 0x24);
}

void playShopSoundOnlyInSavannah(void)
{
	uint16_t mapId;
	int32_t i;

	mapId = CURRENT_MAP_ID;

	for (i = 0; i <= 0; i++) {
		if (mapId == MAIN_D_801345C0[i]) {
			i = i * 2;
			playSound(((uint8_t *)&MAIN_D_801345C2)[i],
			          ((uint8_t *)&MAIN_D_801345C2)[i + 1]);

			return;
		}
	}
}

void renderHorizontalLine(uint8_t boxId, int16_t x, int16_t y, int32_t w)
{
	int32_t yc;

	x = x + UI_BOX_DATA[boxId].finalPos.x;
	y = y + UI_BOX_DATA[boxId].finalPos.y;
	boxId = 6 - boxId;
	yc = y;

	drawLine2P(0x20202, x, yc, (x + w) - 1, yc, boxId, 0);
	y++;
	drawLine2P(0xa08769, x, y, (x + w) - 1, y, boxId, 0);
	y++;
	drawLine2P(0x20202, x, y, (x + w) - 1, y, boxId, 0);
}

GARBAGE(renderInsetWithoutBox, 1);

void renderInsetWithoutBox(int32_t boxId, int16_t x, int16_t y, int32_t w, int16_t h)
{
	uint8_t order;

	x = x + UI_BOX_DATA[boxId].finalPos.x;
	y = y + UI_BOX_DATA[boxId].finalPos.y;
	order = 6 - boxId;
	drawLine3P(0xa08769, x + 1, (y + h) - 1, (x + w) - 1, (y + h) - 1, (x + w) - 1, y, order, 0);
	drawLine3P(0x20202, (x + w) - 1, y, x, y, x, (y + h) - 1, order, 0);
}

void renderCardSprite(int32_t spriteId, int16_t x, int16_t y, int32_t depth)
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

void showMapHeadTextbox(int32_t idx, int32_t owner, int32_t boxId,
                        int32_t section)
{
	uint8_t *savedCursor;

	if (boxId == 0 && owner != 0xfe) {
		setDialogueOwner(owner);
	}

	savedCursor = MAIN_D_80134FDC;

	if (section == 0xff) {
		MAIN_D_80134FDC = getShopkeeperLine(idx);
	} else {
		MAIN_D_80134FDC = resolveMapHeadEntry(section, idx);
	}

	if (owner == 0xfe) {
		owner = 0xff;
	}

	showTextboxReady(boxId, owner);
	ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
	MAIN_D_80134FDC = savedCursor;
}

void lostAllLives(void)
{
	uint8_t *pool;
	int16_t i;
	int16_t count;
	int16_t pick;
	int32_t j;

	switch (SELECTION_MENU_STATE) {
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
	count = 0;
	while (i < 0x3a) {
		if (hasMove(i) != 0) {
			count += 1;
		}
		i++;
	}

	count = (10 - TAMER_ENTITY.tamerLevel) * 2 * (count * 100) / 100;
	if (count % 100 >= 0x32) {
		count += 100;
	}

	count = count / 100;
	if (count == 0) {
		ACTIVE_INSTRUCTION = 0;
		return;
	}

	writePStat(0xf4, count);
	SELECTION_MENU_STATE = 2;

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

	pick = randomLimit(j);
	pick = pool[pick];
	unlearnMove(pick);
	writePStat(0xf3, pick);
	freeArray((uint32_t *)pool);
	showMapHeadTextbox(7, 0xff, 0, 0x4d8);
	SELECTION_MENU_STATE = 1;
	SCRIPT_STATE_4 = 3;
	SCRIPT_STATE_3 = 1;

	return;
state3:
	pick = readPStat(0xf4);
	pick -= 1;
	if (pick == 0) {
		closeBox(0);
		ACTIVE_INSTRUCTION = 0;
		return;
	}

	writePStat(0xf4, pick);
	SELECTION_MENU_STATE = 2;
}

int32_t flipTextboxPage(uint8_t a)
{
	TextBoxData *entry;

	entry = &MAIN_D_801BE80C.box[a];

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
	char *name;
	uint8_t type;
	uint8_t amount;
	int32_t value;
	int32_t sum;
	int32_t idx;
	int32_t i;

	out = getTextboxLine((uint8_t *)box, row);
	sum = box->topRow + row;
	i = sum * 2;
	idx = i;
	type = box->buf[idx];
	amount = box->buf[idx + 1];

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

		strcpy(out, name = ITEM_PARA[type].name);
		out += strlen(name);
		*out++ = 0xf;
		*out++ = 0;

		if (ITEM_MENU_TYPE != 5) {
			*out++ = 0x16;
			*out++ = 0;
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
		}
amountPart:
		if (ITEM_MENU_TYPE == 1) {
			out = intToStringSJIS(out, amount & 0x7f, 2, 0);
		}
		if ((ITEM_MENU_TYPE != 0) && (ITEM_MENU_TYPE != 7) &&
		    (ITEM_MENU_TYPE != 1)) {
			*out++ = 0x1c;
			*out++ = 0;
			out = intToStringSJIS(out, amount & 0x7f, 2, 0);
		}
	}

	terminateString(out, isLast);
}

void calculateCardMenuStrings(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	char *name;
	uint8_t type;
	uint8_t amount;
	int32_t idx;
	int32_t value;

	out = getTextboxLine((uint8_t *)box, row);
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

		strcpy(out, name = DIGIMON_DATA[CARD_DATA[type].digimonId].name);
		out += strlen(name);
		*out++ = 0x17;
		*out++ = 0;
		*out++ = 0xf;
		*out++ = 0;
		value = MAIN_D_8012FFC4[CARD_DATA[type].spriteId];

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
	int32_t len;

	out = getTextboxLine((uint8_t *)box, row);
	type = box->buf[(box->topRow + row) * 2];
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
	out = padWithSpaces(out, 0xc, len);
	terminateString(out, isLast);
}

void calculateBirdramonMenuStrings(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	uint8_t raw;
	uint8_t nameId;
	int32_t len;

	out = getTextboxLine((uint8_t *)box, row);
	raw = box->buf[(box->topRow + row) * 2];
	*out++ = 1;

	if ((raw & 0x80) != 0) {
		*out++ = 1;
	} else {
		*out++ = 3;
	}

	raw &= 0x7f;
	nameId = MAP_ENTRIES[MAIN_D_8013024C[raw].mapId].loadingName;
	strcpy(out, MAP_NAME_PTR[nameId]);
	len = strlen(MAP_NAME_PTR[nameId]);
	out += len;
	out = padWithSpaces(out, 0xc, len);
	*out++ = 0xf;
	*out++ = 0;
	*out++ = 0x1b;
	*out++ = 0;
	out = intToStringSJIS(out, MAIN_D_8013024C[raw].cost, 4, 0);
	terminateString(out, isLast);
}

void updateItemMenuLine(ItemMenuBox *box, int32_t style)
{
	TextBoxData *entry;
	uint8_t *p;
	int16_t row;
	int32_t outX;
	int32_t outClut;

	entry = &MAIN_D_801BE80C.box[box->boxId];
	((int32_t *)entry)[6] ^= 1;

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

	((int32_t *)entry)[6] ^= 1;

	row = box->itemRow[row];
	if (entry->doubleBuffered == 1) {
		row = row + (int16_t)(((int32_t *)entry)[6] *
		                      ((int32_t *)entry)[9]);
	}

	getVRAMModeCoords(((int32_t *)entry)[7], &outX, &outClut);

	p = TEXT_BUFFERS_PTR + (row << 6);
	if (outX != 0) {
		p += 0x20;
	}

	drawString2(p, outX, row * 12, 1);
}

void renderKeeperTableBox(uint8_t boxId, int16_t x, int16_t y, int32_t w, int16_t h)
{
	x = x + UI_BOX_DATA[boxId].finalPos.x;
	y = y + UI_BOX_DATA[boxId].finalPos.y;
	boxId = 6 - boxId;
	drawLine3P(0xa08769, (x + w) - 1, y, x, y, x, (y + h) - 1, boxId, 0);
	drawLine3P(0x20202, x, (y + h) - 1, (x + w) - 1, (y + h) - 1, (x + w) - 1, y, boxId, 0);
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

uint8_t *getTextboxLine(uint8_t *data, int32_t index)
{
	TextBoxData *entry;
	uint8_t *row;

	entry = &MAIN_D_801BE80C.box[data[0xc]];
	row = TEXT_BUFFERS_PTR + (data + index)[0x18] * 0x40;

	if (((int32_t *)entry)[7] == 2) {
		row += 0x20;
	}

	if (entry->doubleBuffered == 1) {
		row = (uint8_t *)(row +
		                  (((((int32_t *)entry)[6] ^ 1) *
		                    ((int32_t *)entry)[9])
		                   << 6));
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
	while (used != 0) {
		*str++ = 0x81;
		*str++ = 0x40;
		used--;
	}

	return str;
}

void renderVerticalLine(int32_t boxId, int16_t x, int16_t y, int32_t h)
{
	int32_t xc;
	uint8_t order;

	x = x + UI_BOX_DATA[boxId].finalPos.x;
	y = y + UI_BOX_DATA[boxId].finalPos.y;
	order = 6 - boxId;
	xc = x;

	drawLine2P(0x20202, xc, y, xc, (y + h) - 1, order, 0);
	x++;
	drawLine2P(0xa08769, x, y, x, (y + h) - 1, order, 0);
	x++;
	drawLine2P(0x20202, x, y, x, (y + h) - 1, order, 0);
}

void setDialogueOwner(int32_t owner)
{
	if (MAIN_D_80134FE6 != owner) {
		MAIN_D_80134FE6 = owner;
		scriptPauseGame(owner);
		setupDialogueBox(MAIN_D_80134FE6);
	}
}

void showMapheadSelection(int32_t idx, int32_t owner, int32_t x,
                          int32_t *outSel, uint16_t section)
{
	uint8_t *saved;
	uint16_t rows;

	if (owner != 0xfe) {
		setDialogueOwner(owner);
	} else {
		MAIN_D_80134FE6 = 0xff;
		owner = 0xfd;
	}

	saved = MAIN_D_80134FDC;

	if (section == 0xff) {
		MAIN_D_80134FDC = getShopkeeperLine(idx);
	} else {
		MAIN_D_80134FDC = resolveMapHeadEntry(section, idx);
	}

	MAIN_D_801BE950[0] = x;

	if (*outSel == 0) {
		*outSel = 1;
		MAIN_D_801BE952[0] = 0;
	}

	MAIN_D_801BE956[0] = showTextboxReady(0, MAIN_D_80134FE6);
	MAIN_D_801BE956[0] = MAIN_D_801BE956[0] * 12 + 2;

	if (owner != 0xff) {
		MAIN_D_801BE954[0] = 0xd;
	} else {
		MAIN_D_801BE954[0] = 0;
	}

	ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
	MAIN_D_80134FDC = saved;
}

void renderScriptDialogueBox(void)
{
	int16_t rowPx;
	int16_t x;
	int16_t y;
	int16_t ySave;
	int32_t i;

	rowPx = TEXT_BOX_DATA[0].vramRow * 12;
	rowPx += (TEXT_BOX_DATA[0].backPage * 48);
	x = UI_BOX_DATA[0].finalPos.x + 5;
	y = UI_BOX_DATA[0].finalPos.y + 4;
	ySave = y;

	i = 0;
	while (i < 4) {
		renderString(0, x, y, 0xfc, 0xc, 0, rowPx, 6, 1);
		i++;
		rowPx += 0xc;
		y += 0xd;
	}

	if (ACTIVE_INSTRUCTION == SCRIPT_OP_SET_SELECTION) {
		renderDialogueSelectionCursor(x, ySave);
	} else if (ACTIVE_INSTRUCTION == SCRIPT_OP_CALL_ROUTINE && SCRIPT_STATE_3 == 2) {
		renderDialogueSelectionCursor(x, ySave);
	}

	if (MAIN_D_80134F94 != 0) {
		renderUIBox(0);
	}
}

void setDigimonRaised(int32_t digimonId)
{
	if ((uint32_t)digimonId < 0x3f) {
		setTrigger(digimonId + 0x200);
	}
}

int32_t hasDigimonRaised(int32_t digimonId)
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
			MAIN_D_80134F60 = 0x1e;
			MAIN_D_8012FDC0.u = b * 32;
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

void checkShopMap(int32_t mapId)
{
	uint8_t local;
	int32_t i;

	if (isTriggerSet(0x29) == 1) {
		unsetTrigger(0x29);
		return;
	}
	for (i = 0; i < 8; i++) {
		if (mapId == MAIN_D_8012FE68[i]) {
			loadDynamicLibrary(SHOP_REL, &local, 0, 0, 0);
			return;
		}
	}
}

void loadShopLibrary(void)
{
	loadDynamicLibrary(SHOP_REL, (uint8_t *)&TRN_LOADING_COMPLETE, 1, 0, 0);
}

void checkArenaMap(int32_t mapId)
{
	uint8_t local;
	int32_t i;

	for (i = 0; i < 2; i++) {
		if (mapId == MAIN_D_801345BC[i]) {
			loadDynamicLibrary(DGET_REL, &local, 0, 0, 0);
			return;
		}
	}
}

int32_t drawTextboxStrings(int32_t boxId, int32_t flag)
{
	TextBoxData *box;
	uint32_t x;
	int32_t clut;
	int16_t px;
	int16_t row;
	uint8_t *buf;
	int32_t done;

	box = &MAIN_D_801BE80C.box[boxId];
	if (box->writeCount == box->renderCount) {
		return 0;
	}

	getVRAMModeCoords(box->vramMode, (int32_t *)&x, &clut);
	px = x;
	row = box->vramRow + box->writeRow;
	buf = TEXT_BUFFERS_PTR + (row << 6);
	if (x != 0) {
		buf += 0x20;
	}
	row = row * 12;
	if (box->vramMode == 0) {
		x = 1;
	} else {
		x = 2;
	}

	while (x != 0) {
		done = drawString2(buf, px, row, flag);
		box->writeRow++;
		if (done != 0) {
			box->writeCount = 1;
			box->renderCount = 1;
			box->flipCount = 0;
			box->registered = 1;
			break;
		}
		row += 12;
		buf += 0x40;
		x--;
	}

	return 1;
}

void clearTextboxLineCount(int32_t boxId)
{
	TextBoxData *entry;
	int32_t rows;

	entry = &MAIN_D_801BE80C.box[boxId];

	rows = ((int32_t *)entry)[9];
	if (entry->doubleBuffered == 1) {
		rows <<= 1;
	}

	((int32_t *)entry)[9] = 0;
	entry->registered = 1;
	MAIN_D_801BE80C.usedRows -= rows;
}

int32_t tickSelectionDialogue(void)
{
	if (flipIdleTextboxPage(0) != 0) {
		return 0xffff;
	}

	if (isXPressedAfterDialogue() == 0) {
		return 0xffff;
	}

	if (isKeyDown(0x40)) {
		advanceTextbox(0);
		playSound(0, 3);

		return MAIN_D_801BE952[0];
	}

	if (isKeyDown(0x10)) {
		if (isTriggerSet(0x31) == 0) {
			if (ACTIVE_INSTRUCTION != SCRIPT_OP_CALL_ROUTINE) {
				advanceTextbox(0);
				MAIN_D_80134FDC =
					(uint8_t *)MAIN_D_801BE94C[0];
				playSound(0, 4);

				return 0xffff;
			}

			advanceTextbox(0);
			MAIN_D_801BE952[0] = MAIN_D_801BE950[0] - 1;
			playSound(0, 4);

			return MAIN_D_801BE952[0];
		}

		playSound(0, 0xb);

		return 0xffff;
	}

	if (isKeyDown(0x1000)) {
		if (MAIN_D_801BE952[0] == 0) {
			MAIN_D_801BE952[0] = MAIN_D_801BE950[0] - 1;
		} else {
			MAIN_D_801BE952[0] -= 1;
		}

		playSound(0, 2);

		return 0xffff;
	}

	if (isKeyDown(0x4000)) {
		MAIN_D_801BE952[0] += 1;
		if (MAIN_D_801BE952[0] == MAIN_D_801BE950[0]) {
			MAIN_D_801BE952[0] = 0;
		}

		playSound(0, 2);
	}
	return 0xffff;
}

int32_t tickConfirmDialogue(void)
{
	if (flipIdleTextboxPage(0) != 0) {
		return 0;
	}

	if (MAIN_D_80134FE5 == 0) {
		MAIN_D_80134F94 = 1;
	}

	if (isXPressedAfterDialogue() == 0) {
		return 0;
	}

	switch (MAIN_D_80134FE5) {
	case 0:
		if (isKeyDown(0x40) == 0) {
			goto ret0;
		}

		if (UI_BOX_DATA[0].state != 1) {
			goto ret0;
		}

		if (TEXT_BOX_DATA[0].doubleBuffered == 1) {
			advanceTextbox(0);
			playSound(0, 3);
			return 1;
		} else {
			playSound(0, 3);
			ACTIVE_INSTRUCTION = 0;
			return 1;
		}
	case 2:
		if (MAIN_D_80135010 != 0) {
			goto ret0;
		}
		/* fall through */
	case 1:
		if (TEXT_BOX_DATA[0].doubleBuffered == 1) {
			advanceTextbox(0);
			return 1;
		} else {
			ACTIVE_INSTRUCTION = 0;
			return 1;
		}
	}
ret0:
	return 0;
}

void renderDialogueSelectionCursor(int32_t x, int32_t y)
{
	if (TEXT_BOX_DATA[0].idle == 1) {
		return;
	}

	renderSelectionCursor(x - 1,
	                      y + MAIN_D_801BE954[0] + MAIN_D_801BE952[0] * 13 - 2,
	                      MAIN_D_801BE956[0], 0xd, 6);
}

void tickCustomSizedTextbox(void)
{
	if (flipIdleTextboxPage(0) != 0) {
		return;
	}

	if (!isXPressedAfterDialogue()) {
		return;
	}

	if (isKeyDown(0x40)) {
		if (UI_BOX_DATA[0].state != 1) {
			return;
		}

		if (advanceTextbox(0) != 0) {
			triggerBoxCloseFlag(0);
		}

		playSound(0, 3);

		return;
	}

	if (!isKeyDown(0x10)) {
		return;
	}

	if (UI_BOX_DATA[0].state == 1) {
		while (TEXT_BOX_DATA[0].pageReady == 0) {
			showTextbox(0, 0xff);
		}

		ACTIVE_INSTRUCTION = 0;
		TEXT_BOX_DATA[0].idle = 1;
		triggerBoxCloseFlag(0);
		playSound(0, 4);
	}
}

void renderCustomSizedTextbox(void)
{
	TextBoxData *box;
	int16_t rowPx;
	int16_t pagePx;
	int16_t x;
	int16_t y;
	int32_t i;

	box = MAIN_D_801BE80C.box;
	rowPx = box->vramRow * 12;
	pagePx = box->vramRows * 12;
	rowPx = rowPx + (int16_t)(box->backPage * pagePx);
	x = UI_BOX_DATA[0].finalPos.x + 5;
	y = UI_BOX_DATA[0].finalPos.y + 4;

	for (i = 0; i < box->vramRows; i++, rowPx += 0xc, y += 0xd) {
		renderString(0, x, y, 0xfc, 0xc, 0, rowPx, 6, 1);
	}
}

void initializeTextbox(void)
{
	TextBoxData *box;
	int32_t i;

	for (i = 0, box = TEXT_BOX_DATA; i < 6; i++, box++) {
		box->flags = 0;
		box->vramRows = 0;
		box->registered = 1;
	}

	MAIN_D_801BE80C.usedRows = 0;
	MAIN_D_80134FFC = 0;
	MAIN_D_80135010 = 0;
	TEXTBOX_OPEN_TIMER = 0;
}

void tickTextboxHandling(int32_t flag)
{
	RECT area;
	int32_t x;
	int32_t clut;
	TextBoxData *box;
	int32_t i;
	int32_t drew;
	uint8_t flags;
	int16_t mode;
	uint8_t features;
	uint8_t color;

	drew = 0;
	if (ACTIVE_INSTRUCTION != 0xff) {
		box = TEXT_BOX_DATA;
		for (i = 0; i < 6; i++, box++) {
			if (box->vramRows != 0) {
				if (box->registered == 1 && box->writeCount != box->renderCount) {
					if (box->doubleBuffered == 1) {
						box->writeRow = (box->backPage ^ 1) * box->vramRows;
						getVRAMModeCoords(box->vramMode, &x, &clut);
						area.x = x;
						area.y = (box->vramRow + box->writeRow) * 12;
						area.w = clut;
						area.h = box->vramRows * 12;
						clearTextSubArea(&area);
					} else {
						box->writeRow = 0;
					}
					box->registered = 0;
				}
				if (drew == 0 && box->registered == 0) {
					drew = drawTextboxStrings(i & 0xff, flag);
				}
				flags = box->flags;
				mode = flags & 0xf;
				if (mode != UI_BOX_DATA[i].state) {
					if ((flags & 0xf) == 1) {
						if (box->registered == 1 && UI_BOX_DATA[i].state == 0) {
							features = (flags >> 4) & 3;
							if (features == 0) {
								color = 1;
							} else {
								color = 0;
							}
							if ((flags & 0x80) == 0) {
								createStaticUIBox((int16_t)i, color, features, &box->rect, box->tick, box->render);
							} else {
								createAnimatedUIBox((int16_t)i, color, features, &box->rect, &box->origin, box->tick, box->render);
							}
						}
					} else if (UI_BOX_DATA[i].state == 1) {
						closeTextbox(i & 0xff, 0);
					}
				}
			}
		}
		if (MAIN_D_80134FFC != 0) {
			MAIN_D_80134FFC--;
		}
		if (MAIN_D_80135010 != 0) {
			MAIN_D_80135010--;
		}
		MAIN_D_80134F94 = 0;
		TEXTBOX_OPEN_TIMER++;
		return;
	}
	for (i = 0; i < 6; i++) {
		if (UI_BOX_DATA[i].state != 0) {
			break;
		}
		setTextColor(1);
		IS_SCRIPT_PAUSED = 1;
	}
}

void getVRAMModeCoords(int32_t mode, int32_t *outX, int32_t *outClut)
{
	if (mode == 0) {
		*outX = 0;
		*outClut = 0xff;
	} else if (mode == 1) {
		*outX = 0;
		*outClut = 0x7f;
	} else {
		*outX = 0x80;
		*outClut = 0x7f;
	}
}
// clang-format off
void closeTextbox(boxId, target)
	int16_t boxId;
	RECT *target;
// clang-format on
{
	uint32_t b;

	b = boxId;

	if (UI_BOX_DATA[boxId].state != 0 && UI_BOX_DATA[boxId].state != 3) {
		if ((TEXT_BOX_DATA[b].flags & 0x40) == 0) {
			removeStaticUIBox(boxId);
		} else {
			removeAnimatedUIBox(boxId, target);
		}

		clearTextboxLineCount(boxId);
	}
}

void closeAllTextboxes(void)
{
	int32_t i;
	uint8_t flags;

	for (i = 0; i < 6; i++) {
		flags = TEXT_BOX_DATA[i].flags;
		if ((flags & 0xf) != 0) {
			if (flags & 0x40) {
				ACTIVE_INSTRUCTION = 0xff;
				IS_SCRIPT_PAUSED = 0;
			}

			closeBox(i & 0xff);
		}
	}
}

void createTextbox(int32_t boxId, uint8_t flags, RECT *rect, RECT *origin,
                   void *tick, void *render)
{
	TextBoxData *entry;

	entry = &MAIN_D_801BE80C.box[boxId];

	if ((entry->flags & 0xf) == 1) {
		closeTextbox(boxId, 0);
	}

	entry->flags = flags;
	entry->rect = *rect;
	entry->origin = *origin;
	entry->tick = tick;
	entry->render = render;

	setFreshDialogue();
}

void triggerBoxCloseFlag(int32_t boxId)
{
	TextBoxData *e = &MAIN_D_801BE80C.box[boxId];
	uint8_t val = e->flags;
	uint32_t v;

	if ((val & 0xf) != 0) {
		v = val & 0xf0;
		v &= 0xff;
		e->flags = v;
	}
}

void registerTextbox(int32_t boxId, int32_t row, int32_t rows,
                     int32_t doubleBuffer, int32_t mode)
{
	TextBoxData *entry;
	int32_t usedRows;
	RECT rect;
	int32_t vramX;
	int32_t vramW;

	entry = &MAIN_D_801BE80C.box[boxId];
	entry->vramRow = row;
	entry->vramRows = rows;
	entry->writeRow = 0;
	entry->vramMode = mode;
	entry->backPage = 0;
	entry->doubleBuffered = doubleBuffer;
	entry->writeCount = 0;
	entry->renderCount = 0;
	entry->flipCount = 0;
	entry->registered = 1;
	entry->pageReady = 0;
	entry->idle = 1;

	usedRows = entry->vramRows;
	if (entry->doubleBuffered == 1) {
		usedRows = usedRows << 1;
	}

	MAIN_D_801BE80C.usedRows += usedRows;
	setTextColor(1);
	getVRAMModeCoords(entry->vramMode, &vramX, &vramW);
	setRECT(&rect, vramX, entry->vramRow * 12, vramW, usedRows * 12);
	clearTextSubArea(&rect);
}

int32_t drawString2(uint8_t *str, int16_t x, int16_t y, int32_t flag)
{
	int32_t y2;
	RECT rect;
	uint8_t ch;
	int16_t pos;
	int16_t rem;
	uint16_t adv;
	int32_t save;
	uint16_t glyph;

	pos = 0;
	for (;;) {
		ch = *str++;
		switch (ch) {
		case 0:
			return 1;
		case 3:
			str += 3;
			break;
		case 2:
			str++;
			break;
		case 1:
			ch = *str++;
			setTextColor(ch);
			break;
		case 0xc:
			str++;
			rem = pos / 12 % 8;
			save = pos;
			if (rem != 0) {
				rem = (8 - rem) * 12;
				setRECT(&rect, x + save, y, rem, 0xc);
				clearTextSubArea(&rect);
				pos = save + rem;
			}
			break;
		case 0x16:
			str++;
			setRECT(&rect, x + pos - 6, y, 0x69, 0xc);
			clearTextSubArea(&rect);
			pos = 0x69;
			break;
		case 0x17:
			str++;
			setRECT(&rect, x + pos, y, 0x69, 0xc);
			clearTextSubArea(&rect);
			pos = 0x69;
			break;
		case 0x1b:
			str++;
			setRECT(&rect, x + pos, y, 0xa0, 0xc);
			clearTextSubArea(&rect);
			pos = 0xa0;
			break;
		case 0x1c:
			str++;
			setRECT(&rect, x + pos, y, 0x64, 0xc);
			clearTextSubArea(&rect);
			pos = 0x64;
			break;
		case 0x18:
			str++;
			setRECT(&rect, x + pos, y, 0x60, 0xc);
			clearTextSubArea(&rect);
			pos = 0x60;
			break;
		case 0x19:
			str++;
			setRECT(&rect, x + pos, y, 0x9c, 0xc);
			clearTextSubArea(&rect);
			pos = 0x9c;
			break;
		case 0x1a:
			str++;
			setRECT(&rect, x + pos, y, 0xb4, 0xc);
			clearTextSubArea(&rect);
			pos = 0xb4;
			break;
		case 0xe:
			str++;
			rem = pos / 12 % 0xb;
			save = pos;
			if (rem != 0) {
				rem = (0xb - rem) * 12;
				setRECT(&rect, x + save, y, rem, 0xc);
				clearTextSubArea(&rect);
				pos = save + rem;
			}
			break;
		case 0xf:
			str++;
			setRECT(&rect, x + pos, y, 6, 0xc);
			clearTextSubArea(&rect);
			pos += 6;
			break;
		case 0xd:
			return 0;
		default:
			if (isAsciiEncoded((char *)&ch) != 0) {
				glyph = swapShortBytes(convertAsciiToJis(ch));
			} else {
				glyph = ch + (*str++ << 8);
			}

			if (glyph == 0x4081) {
				setRECT(&rect, x + pos, y, 0xc, 0xc);
				clearTextSubArea(&rect);
			} else {
				y2 = y;
				adv = drawGlyph(glyph, x + pos, y2);
				if (MAIN_D_80134F98 != 0) {
					adv = 0xc;
				}
			}

			pos += adv;
			break;
		}
	}
}

int32_t flipIdleTextboxPage(int32_t boxId)
{
	TextBoxData *entry;

	entry = &MAIN_D_801BE80C.box[boxId];

	if (entry->doubleBuffered == 0) {
		return 0;
	}

	if (entry->registered == 0) {
		return 1;
	}

	if (entry->writeCount != entry->renderCount) {
		return 1;
	}

	if (entry->idle == 0) {
		return 0;
	}

	if (entry->renderCount == 0) {
		return 1;
	}

	entry->idle = 0;
	entry->backPage ^= 1;
	entry->flipCount++;

	if (entry->pageReady == 0) {
		ACTIVE_INSTRUCTION = 0;
	}

	TEXTBOX_OPEN_TIMER = 0;

	return 1;
}

void closeBox(int32_t boxId)
{
	triggerBoxCloseFlag(boxId);
	closeTextbox(boxId, 0);
}

int32_t tickBackgroundDialogue(void)
{
	TextBoxData *entry;

	entry = TEXT_BOX_DATA;

	if (flipIdleTextboxPage(0) != 0) {
		return 0;
	}

	if (entry->renderCount == entry->flipCount) {
		entry->idle = 1;
		entry->pageReady = 0;
		return 1;
	}

	return 0;
}
int32_t advanceTextbox(int32_t boxId)
{
	TextBoxData *entry;

	entry = &MAIN_D_801BE80C.box[boxId];
	if (entry->registered == 0) {
		return 0;
	}

	if (entry->pageReady == 1) {
		if (entry->renderCount != entry->flipCount) {
			entry->backPage ^= 1;
		} else {
			entry->idle = 1;
			entry->pageReady = 0;
			ACTIVE_INSTRUCTION = 0;
			return 1;
		}
	} else {
		entry->backPage ^= 1;
		ACTIVE_INSTRUCTION = 0;
	}

	entry->flipCount++;
	TEXTBOX_OPEN_TIMER = 0;

	return 0;
}

void setupDialogueBox(uint8_t owner)
{
	RECT rect;
	RECT origin;
	uint8_t flags;

	flags = 0x21;
	if (setupBoxOrigin(owner, &origin) != 0) {
		flags |= 0x80;
	}

	setRECT(&rect, -0x82, 0x2a, 0x106, 0x3b);
	createTextbox(0, flags, &rect, &origin, tickScriptDialogueBox,
	              renderScriptDialogueBox);
	registerTextbox(0, 0, 4, 1, 0);
}

int32_t setupBoxOrigin(int32_t ownerId, RECT *origin)
{
	int16_t pos[2];

	if (ownerId == 0xff) {
		return 0;
	}

	if (MAIN_D_80134FD2 != -0x270f) {
		worldPosToScreenPos2(&MAIN_D_80134FD2, &MAIN_D_80134FD4,
		                     &MAIN_D_80134FD6);
		pos[0] = MAIN_D_80134FD2 - 5;
		pos[1] = MAIN_D_80134FD4 - 5;
		MAIN_D_80134FD2 = -0x270f;
	} else {
		int32_t entityId = scriptIdToEntityId(ownerId) & 0xff;
		if (entityId == 0xff) {
			return 0;
		}

		getEntityScreenPos(ENTITY_TABLE[entityId], 1, pos);
	}

	setRECT(origin, pos[0], pos[1], 0xa, 0xa);

	return 1;
}

void tickScriptDialogueBox(void)
{
	int32_t sel;

	if (ACTIVE_INSTRUCTION == SCRIPT_OP_SET_SELECTION) {
		sel = tickSelectionDialogue();
		if (sel != 0xffff) {
			MAIN_D_80134FDC =
				(uint8_t *)((uint32_t)MAIN_D_801BE948[0] +
			                    MAIN_D_801BE952[0] * 2);
			MAIN_D_80134FDC =
				(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR +
			                    *(uint16_t *)MAIN_D_80134FDC);
		}
	} else if (ACTIVE_INSTRUCTION == SCRIPT_OP_SHOW_TEXTBOX) {
		tickConfirmDialogue();
	} else if (ACTIVE_INSTRUCTION == SCRIPT_OP_CALL_ROUTINE) {
		switch (SCRIPT_STATE_3) {
		case 0:
			break;
		case 1:
			if (tickConfirmDialogue() == 1) {
				SCRIPT_STATE_3 = 0;
				SELECTION_MENU_STATE = SCRIPT_STATE_4;
				ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
			}
			break;
		case 2:
			sel = tickSelectionDialogue();
			if (sel != 0xffff) {
				SCRIPT_STATE_3 = 0;
				sel = MAIN_D_801BE952[0];
				SELECTION_MENU_STATE =
					SCRIPT_STATE_4 + (uint16_t)sel;
				ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
			}
			break;
		case 3:
			if (tickBackgroundDialogue() == 1) {
				ACTIVE_INSTRUCTION = SCRIPT_OP_CALL_ROUTINE;
				SCRIPT_STATE_3 = 0;
			}
			break;
		}
	}
}

void renderUIBox(int32_t boxId)
{
	POLY_FT4 poly;
	int16_t x;
	int16_t y;
	int16_t u;

	x = UI_BOX_DATA[boxId].finalPos.x + 0xef;
	y = UI_BOX_DATA[boxId].finalPos.y + 0x2b;
	u = TEXTBOX_OPEN_TIMER / 4 % 3 * 16 + 0x4c;
	SetPolyFT4(&poly);
	SetSemiTrans(&poly, 1);
	poly.tpage = GetTPage(0, 1, 320, 128);
	poly.clut = GetClut(96, 500);
	setRGB0(&poly, 0x80, 0x80, 0x80);
	setUVDataPolyFT4(&poly, u, 0xb7, 0x10, 0xc);
	setPosDataPolyFT4(&poly, x, y, 0x10, 0xc);

	GsSortPoly(&poly, ACTIVE_ORDERING_TABLE, (6 - boxId));
}

void scriptShowSelection(void)
{
	uint8_t optionCount;
	uint16_t height;

	pollNextScriptUByte(&optionCount);

	MAIN_D_801BE950[0] = optionCount;
	MAIN_D_801BE952[0] = 0;
	MAIN_D_801BE948[0] = (int32_t)MAIN_D_80134FDC;
	MAIN_D_80134FDC += (optionCount + 1) * 2;
	MAIN_D_801BE956[0] = showTextbox(0, MAIN_D_80134FE6);
	MAIN_D_801BE956[0] = MAIN_D_801BE956[0] * 12 + 2;
	height = MAIN_D_801BE956[0];

	if (height > 0xf0) {
		MAIN_D_801BE956[0] = 0xf0;
	}

	MAIN_D_80134FDC += 2;
	MAIN_D_801BE94C[0] = (int32_t)MAIN_D_80134FDC;

	if (MAIN_D_80134FE6 != 0xff) {
		MAIN_D_801BE954[0] = 0xd;
	} else {
		MAIN_D_801BE954[0] = 0;
	}

	ACTIVE_INSTRUCTION = SCRIPT_OP_SET_SELECTION;
}

GARBAGE(showTextbox, 1);

uint32_t showTextbox(int32_t boxId, uint32_t speakerId)
{
	TextBoxData *entry;
	uint8_t *base;
	uint8_t *out;
	uint32_t col;
	uint16_t maxCol;
	uint32_t row;
	uint32_t rowOffset;
	int32_t lines;
	uint8_t ctrl;

	entry = &MAIN_D_801BE80C.box[boxId];
	base = TEXT_BUFFERS_PTR + (entry->vramRow << 6);
	if (entry->vramMode == 2) {
		base += 0x20;
	}
	if (entry->doubleBuffered == 1) {
		base = (uint8_t *)(base + (((entry->backPage ^ 1) * entry->vramRows) << 6));
	}
	out = base;
	col = maxCol = 0;
	row = 0;
	if (speakerId != 0xff) {
		*out++ = 1;
		if (speakerId == 0xfd) {
			*out++ = 6;
		} else if (speakerId == 0xfc) {
			*out++ = 0xa;
		} else if (speakerId >= 0xc8) {
			*out++ = 5;
		} else {
			*out++ = 7;
		}
		out += getSpeakerName(speakerId, out);
		*out++ = 1;
		*out++ = 1;
		*out++ = 0xd;
		*out = 0;
		row++;
		out = base + (row << 6);
	}
	rowOffset = row << 6;
top: {
	ctrl = *MAIN_D_80134FDC++;
	{
		switch (ctrl) {
		case 3:
			*out++ = ctrl;
			*out++ = *MAIN_D_80134FDC++;
			*out++ = *MAIN_D_80134FDC++;
			*out++ = *MAIN_D_80134FDC++;
			goto top;
		case 4:
			ctrl = *MAIN_D_80134FDC++;
			ctrl = readPStat(ctrl);
			out = intToStringSJIS(out, ctrl, 3, 1);
			goto top;
		case 5:
			MAIN_D_80134FDC++;
			lines = getSpeakerName(0xfd, out);
			out += lines;
			col = (col + ((lines >> 1) &
			              0xffff)) &
			      0xffff;
			goto top;
		case 6:
			MAIN_D_80134FDC++;
			lines = getSpeakerName(0xfc, out);
			out += lines;
			col = (col + ((lines >> 1) &
			              0xffff)) &
			      0xffff;
			goto top;
		case 7:
			ctrl = *MAIN_D_80134FDC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out,
				       (const char *)&DIGIMON_DATA[ctrl]);
				lines = strlen(
					(const char *)&DIGIMON_DATA[ctrl]);
				out += lines;
				col = (col + ((lines >> 1) &
				              0xffff)) &
				      0xffff;
			}
			goto top;
		case 8:
			ctrl = *MAIN_D_80134FDC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out, MOVE_NAMES[ctrl]);
				lines = strlen(MOVE_NAMES[ctrl]);
				out += lines;
				col = (col + ((lines >> 1) &
				              0xffff)) &
				      0xffff;
			}
			goto top;
		case 9:
			ctrl = *MAIN_D_80134FDC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out,
				       (const char *)&ITEM_PARA[ctrl]);
				lines = strlen(
					(const char *)&ITEM_PARA[ctrl]);
				out += lines;
				col = (col + ((lines >> 1) &
				              0xffff)) &
				      0xffff;
			}
			goto top;
		case 10:
			MAIN_D_80134FDC++;
			out = intToStringSJIS(out, MONEY, 6, 0);
			goto top;
		case 11:
			MAIN_D_80134FDC++;
			out = intToStringSJIS(out, MERIT, 4, 0);
			goto top;
		case 16:
			MAIN_D_80134FDC++;
			out = intToStringSJIS(out, MAIN_D_8013500C, 5, 1);
			goto top;
		case 19:
			MAIN_D_80134FDC++;
			out = intToStringSJIS(out, MAIN_D_80134FCC, 3, 1);
			goto top;
		case 20:
			MAIN_D_80134FDC++;
			out = intToStringSJIS(out, TOURNAMENTS_LOST, 3, 1);
			goto top;
		case 21:
			MAIN_D_80134FDC++;
			out = intToStringSJIS(out, MAIN_D_80134FD0, 3, 1);
			goto top;
		case 17:
			ctrl = *MAIN_D_80134FDC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out, BGM_TRACK_NAMES[ctrl]);
				lines = strlen(BGM_TRACK_NAMES[ctrl]);
				out += lines;
				col = (col + ((lines >> 1) & 0xffff)) & 0xffff;
			}
			goto top;
		case 18:
			ctrl = *MAIN_D_80134FDC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out, TOURNAMENT_NAMES[ctrl]);
				lines = strlen(TOURNAMENT_NAMES[ctrl]);
				out += lines;
				col = (col + ((lines >> 1) & 0xffff)) & 0xffff;
			}
			goto top;
		case 12:
			*out++ = ctrl;
			*out++ = *MAIN_D_80134FDC++;
			goto top;
		case 14:
			*out++ = ctrl;
			*out++ = *MAIN_D_80134FDC++;
			goto top;
		case 13:
			*out++ = ctrl;
			*out++ = *MAIN_D_80134FDC++;
			if (maxCol < col) {
				maxCol = col;
			}
			col = 0;
			ctrl = *MAIN_D_80134FDC;
			if (ctrl == 0) {
				MAIN_D_80134FDC += 2;
				out -= 2;
				*out = ctrl;
				ctrl = *MAIN_D_80134FDC;
				if (ctrl == SCRIPT_OP_SHOW_TEXTBOX) {
					goto done;
				}
				entry->pageReady = 1;
				goto done;
			}
			rowOffset += 0x40;
			row++;
			lines = rowOffset;
			out = base + lines;
			goto top;
		case 1:
		case 2:
			*out++ = ctrl;
			*out++ = *MAIN_D_80134FDC++;
			goto top;
		case 15:
		default:
			*out++ = ctrl;
			*out++ = *MAIN_D_80134FDC++;
			col = (col + 1) & 0xffff;
			goto top;
		}
	}
}
done:
	entry->writeCount++;
	ACTIVE_INSTRUCTION = SCRIPT_OP_SHOW_TEXTBOX;

	return maxCol;
}

void scriptSetTextboxSize(void)
{
	uint8_t originId;
	uint8_t cols;
	uint8_t rows;
	uint8_t boxFlags;
	RECT rect1;
	RECT origin;
	uint16_t width;
	uint16_t height;

	pollNextScriptUByte(&originId);
	pollNextTwoScriptBytes(&cols, &rows);

	boxFlags = 0x21;
	if (setupBoxOrigin(originId, &origin) != 0) {
		boxFlags |= 0x80;
	}

	width = cols * 12 + 10;
	height = rows * 13 + 7;
	setRECT(&rect1, (0x140 - width) / 2 - 0xa0, (0xf0 - height) / 2 - 0x78, width, height);
	createTextbox(0, boxFlags, &rect1, &origin, tickCustomSizedTextbox,
	              renderCustomSizedTextbox);
	registerTextbox(0, 0, rows, 1, 0);
}

int32_t showTextboxReady(int32_t boxId, int32_t speakerId)
{
	TEXT_BOX_DATA[boxId].pageReady = 1;

	return showTextbox(boxId, speakerId);
}

int32_t getSpeakerName(int32_t speakerId, uint8_t *buf)
{
	if (speakerId == 0xff) {
		return 0;
	}

	if (speakerId == 0xfd) {
		speakerId = 0;
		goto digimon;
	}

	if (speakerId == 0xfc) {
		strcpy((char *)buf, PARTNER_ENTITY.name);

		return strlen(PARTNER_ENTITY.name);
	}

	if ((uint32_t)speakerId < 0xc8) {
		speakerId = scriptIdToEntityId(speakerId) & 0xff;
		speakerId = ENTITY_TABLE[speakerId]->type & 0xff;
		goto digimon;
	}

	speakerId = (speakerId - 0xc8) & 0xff;
	strcpy((char *)buf, MAIN_D_8013035C[speakerId]);

	return strlen(MAIN_D_8013035C[speakerId]);

digimon:
	strcpy((char *)buf, DIGIMON_DATA[speakerId].name);

	return strlen(DIGIMON_DATA[speakerId].name);
}

uint8_t *intToStringSJIS(uint8_t *buf, int32_t value, uint8_t digits, int32_t flag)
{
	Pow10Table divs;
	uint16_t base;
	int32_t started;
	uint16_t c;
	int32_t hi;
	int32_t lo;

	divs = MAIN_D_80130344;
	base = 0x824f;
	started = 0;
	while (digits != 0) {
		c = value / divs.v[digits - 1];
		c = base + c;
		value = value % divs.v[digits - 1];
		if (digits != 1) {
			if (c == 0x824f) {
				if (started == 0) {
					if (flag != 0) {
						goto skip;
					}
					c = 0x8140;
				}
			} else {
				started = 1;
			}
		}
		hi = c >> 8;
		lo = c;
		*buf++ = hi;
		*buf++ = lo;
skip:
		digits--;
	}

	return buf;
}

int32_t scriptIdToEntityId(int32_t scriptId)
{
	uint8_t i;

	if (scriptId == 0xfd) {
		return 0;
	}

	if (scriptId == 0xfc) {
		return 1;
	}

	for (i = 0; i < 8; i++) {
		if (ENTITY_TABLE[i + 2] != 0 && scriptId == NPC_ENTITIES[i].scriptId) {
			return (uint8_t)(i + 2);
		}
	}

	return 0xff;
}

void scriptCompareDate(void)
{
	uint16_t trigger;
	uint8_t statIdx;
	uint8_t op;
	uint8_t years;
	uint8_t days;
	uint8_t hours;
	uint8_t minutes;
	uint32_t now;
	uint32_t v1;
	uint32_t v2;
	uint32_t v3;

	pollNextScriptUByte(&statIdx);
	pollNextScriptUShort(&trigger);
	pollNextTwoScriptBytes(&op, &years);
	pollNextTwoScriptBytes(&days, &hours);
	pollNextScriptUByte(&minutes);

	MAIN_D_80134FDC = (uint8_t *)(MAIN_D_80134FDC + 1);
	v1 = readPStat(statIdx);
	v2 = readPStat((uint8_t)(statIdx + 1));
	v3 = readPStat((uint8_t)(statIdx + 2));
	now = dateToSeconds(v1, v2, v3, readPStat((uint8_t)(statIdx + 3)));

	if (scriptCompareValue(op, now, dateToSeconds(years, days, hours, minutes)) != 0) {
		setTrigger(trigger);
	} else {
		unsetTrigger(trigger);
	}
}

int32_t scriptCompareStat(void)
{
	uint16_t u;
	int16_t s;
	uint8_t b1;
	uint8_t b2;
	uint16_t stat;
	int16_t stat2;

	pollNextTwoScriptBytes(&b1, &b2);

	if (b1 != 9) {
		stat = *getStatsPointer(b1);
		pollNextScriptUShort(&u);

		return scriptCompareValue(b2, stat, u);
	}

	stat2 = *getStatsPointer(b1);
	pollNextScriptShort(&s);

	return scriptCompareSignedValue(b2, stat2, s);
}

int16_t *getStatsPointer(int32_t stat)
{
	switch (stat) {
	case SCRIPT_STAT_OFFENSE:
		return &PARTNER_ENTITY.digimonEntity.stats.base.off;
	case SCRIPT_STAT_DEFENSE:
		return &PARTNER_ENTITY.digimonEntity.stats.base.def;
	case SCRIPT_STAT_SPEED:
		return &PARTNER_ENTITY.digimonEntity.stats.base.speed;
	case SCRIPT_STAT_BRAINS:
		return &PARTNER_ENTITY.digimonEntity.stats.base.brain;
	case SCRIPT_STAT_MAX_HP:
		return &PARTNER_ENTITY.digimonEntity.stats.base.hp;
	case SCRIPT_STAT_MAX_MP:
		return &PARTNER_ENTITY.digimonEntity.stats.base.mp;
	case SCRIPT_STAT_CURRENT_HP:
		return &PARTNER_ENTITY.digimonEntity.stats.current.currentHP;
	case SCRIPT_STAT_CURRENT_MP:
		return &PARTNER_ENTITY.digimonEntity.stats.current.currentMP;
	case SCRIPT_STAT_TIREDNESS:
		return &PARTNER_PARA.tiredness;
	case SCRIPT_STAT_HAPPINESS:
		return &PARTNER_PARA.happiness;
	case SCRIPT_STAT_DISCIPLINE:
		return &PARTNER_PARA.discipline;
	case SCRIPT_STAT_ENERGY:
		return &PARTNER_PARA.energyLevel;
	case SCRIPT_STAT_VIRUS:
		return &PARTNER_PARA.virusBar;
	case SCRIPT_STAT_LIFETIME:
		return &PARTNER_PARA.remainingLifetime;
	case SCRIPT_STAT_MERIT:
		return &MERIT;
	case SCRIPT_STAT_STARTED_BATTLES:
		return &MAIN_D_80134FC8;
	case SCRIPT_STAT_FLED_BATTLES:
		return &MAIN_D_80134FCA;
	case SCRIPT_STAT_TOURNAMENTS_WON:
		return &MAIN_D_80134FCC;
	case SCRIPT_STAT_TOURNAMENT_WINS:
		return &TOURNAMENTS_LOST;
	case SCRIPT_STAT_TOURNAMENTS_LOST:
		return &MAIN_D_80134FD0;
	case SCRIPT_STAT_WEIGHT:
		return &PARTNER_PARA.weight;
	case SCRIPT_STAT_TAMER_LEVEL:
		MAIN_D_80135002 = TAMER_ENTITY.tamerLevel;
		return &MAIN_D_80135002;
	case SCRIPT_STAT_LIVES:
		MAIN_D_80135004 = PARTNER_ENTITY.lives;
		return &MAIN_D_80135004;
	}
}

void *allocateArray(uint32_t size)
{
	uint32_t oldTop;

	oldTop = MAIN_D_80134F64;
	size = ((size + 3) >> 2) << 2;
	*(uint32_t *)(MAIN_D_801345B0 + oldTop) = size;
	MAIN_D_80134F64 += (size + 4);

	return MAIN_D_801345B0 + (oldTop + 4);
}

void freeArray(uint32_t *array)
{
	MAIN_D_80134F64 -= array[-1] + 4;
}

void renderShopBitsBox(void)
{
	int16_t rowPx;
	int16_t x;
	int16_t y;

	rowPx = TEXT_BOX_DATA[2].vramRow * 12;
	x = UI_BOX_DATA[2].finalPos.x + 6;
	y = UI_BOX_DATA[2].finalPos.y + 4;

	renderString(0, x + 0x10, y, 0x30, 0xc, 0, rowPx, 4, 1);
	renderString(0, x + 0x10, y + 0xd, 0x3c, 0xc, 0x30, rowPx, 4, 1);
}

int32_t scriptCompareCard(void)
{
	uint16_t value;
	uint8_t cardId;
	uint8_t op;

	pollNextTwoScriptBytes(&cardId, &op);
	pollNextScriptUShort(&value);

	return scriptCompareValue(op, (uint8_t)getCardAmount(cardId), value);
}

int32_t scriptCompareMove(void)
{
	uint8_t moveId;
	uint8_t negate;
	int32_t res;

	pollNextTwoScriptBytes(&moveId, &negate);
	res = _hasMove(moveId);
	if (negate == 0) {
		return res;
	}

	return (res != 0) ^ 1;
}

int32_t scriptCompareCondition(void)
{
	uint8_t mask;
	uint8_t negate;

	pollNextTwoScriptBytes(&mask, &negate);

	if (negate == 0) {
		if (PARTNER_PARA.condition & mask) {
			return 1;
		}

		return 0;
	} else {
		if ((PARTNER_PARA.condition & mask) == 0) {
			return 1;
		}

		return 0;
	}
}

int32_t scriptCompareItemCount(void)
{
	uint16_t value;
	uint8_t itemId;
	uint8_t op;

	pollNextTwoScriptBytes(&itemId, &op);
	pollNextScriptUShort(&value);

	return scriptCompareValue(op, (uint8_t)getItemCount(itemId), value);
}

void readFileSection(char *filename, void *dest, uint32_t offset,
                     uint32_t size)
{
	CdlFILE file;
	char path[64];
	uint8_t mode;

	mode = 0x80;

	if (MAIN_D_80134FA8 == 0) {
		path[0] = '\\';
		strcpy(&path[1], filename);
		strcat(path, MAIN_D_801345F0);
		if (CdSearchFile(&file, path) == 0) {
			return;
		}

		while (CdControl(0xe, &mode, 0) == 0)
			;

		MAIN_D_80134FA8 = CdPosToInt(&file.pos);
	} else {
		while (CdControl(0xe, &mode, 0) == 0)
			;
	}

	file.pos = *CdIntToPos(MAIN_D_80134FA8 + (offset >> 11), &file.pos);

	while (CdControl(2, (u_char *)&file.pos, 0) == 0)
		;
	while (CdRead(size >> 11, dest, mode) == 0)
		;
	while (CdReadSync(0, 0) > 0)
		;
}

void tickScriptedMovements(void)
{
	int32_t i;

	for (i = 0; i < 0x16; i++) {
		if (MAIN_D_801BE6B4[i].type != 0xff) {
			tickScriptedMovement(i);
		}
	}
}

void scriptInstructionFBtoFF(int32_t op)
{
	StackEntry entry;

	if (op != 0xfb) {
		if (op == 0xfe || op == 0xff) {
			returnFromScriptFile();
		}
		return;
	}

	skipOneReadTwoShort(&CURRENT_SCRIPT_ID, &CURRENT_MAP_ID);

	entry.smth[0] = 3;
	pushScriptStack(&entry);

	MAIN_D_80134FEC = 1;

	resetBGM();
	loadMap(CURRENT_MAP_ID);
	longjmp(SCRIPT_JMP_BUF, 1);
}

int16_t enforceStatsLimits(int32_t stat, int16_t value)
{
	int16_t cap;

	if (stat == SCRIPT_STAT_CURRENT_HP) {
		cap = PARTNER_ENTITY.digimonEntity.stats.base.hp;
	} else if (stat == SCRIPT_STAT_CURRENT_MP) {
		cap = PARTNER_ENTITY.digimonEntity.stats.base.mp;
	} else {
		cap = MAIN_D_80130318[stat];
	}

	if (cap < value) {
		return cap;
	}

	return value;
}

int32_t scriptCompareMoney(void)
{
	int32_t value;
	uint8_t op;

	MAIN_D_80134FDC = (uint8_t *)(MAIN_D_80134FDC + 1);

	pollNextScriptUByte(&op);
	pollNextInt(&value);

	return scriptCompareValue(op, MONEY, value);
}
