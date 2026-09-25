#include <string.h>

#include <libetc.h>
#include <libgs.h>

#include <dw/file_queue.h>
#include <dw/font.h>
#include <dw/item.h>
#include <dw/map.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/pstat.h>
#include <dw/script.h>
#include <dw/sjis.h>
#include <dw/sound.h>
#include <dw/tamer.h>
#include <dw/text.h>
#include <dw/trigger.h>
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

extern ScriptCameraMovement SCRIPT_MOVEMENTS[];
extern TextBoxTable TEXT_BOX_TABLE;
extern uint8_t TEXTBOX_OPEN_TIMER;
extern uint32_t INPUT_REPEAT_MASK;
extern int32_t WAIT_CROSS_RELEASE;
extern uint32_t ARRAY_STACK_TOP;
extern uint8_t *MAIN_D_801345B0;
extern int16_t MAIN_D_801345C0[1];
extern uint16_t MAIN_D_801345C2;
extern int16_t MOOD_BUBBLE_TIMER;
extern int32_t INPUT_ACCEPTED;
extern int32_t INPUT_RELEASED;
extern uint16_t INPUT_REPEAT_TIMER;
extern int32_t POPUP_TEXT_ROW;
extern uint32_t POLLED_INPUT;
extern int32_t TEXTBOX_SHOW_NEXT_ARROW;
extern uint16_t CHOICE_CURSOR[];
extern uint16_t CHOICE_CURSOR_Y[];
extern uint16_t CHOICE_CURSOR_WIDTH[];
extern BabyTypeTable MAIN_D_801345B4;
extern int32_t MONEY_BOX_SHOWS_BITS;
extern int16_t MAIN_D_801345BC[2];
extern uint32_t POLLED_INPUT_PREVIOUS;
extern uint16_t CHOICE_COUNT[];
extern int32_t CHOICE_TARGETS[];
extern int32_t CHOICE_CANCEL_PC[];
extern uint8_t MAIN_D_80134F81;
extern uint8_t MAIN_D_80134F82;
extern uint8_t QUANTITY_MAX;
extern int32_t SELECTED_ITEM_PRICE;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern char *MOVE_NAMES[];
extern char *MAP_NAME_PTR[];
extern char *ITEM_DESC_PTR[];
extern int8_t TEXT_MONOSPACE;
extern int32_t SCRIPT_FILE_POSITION;
extern char FILE_VERSION_SUFFIX[3];
extern char CUP_NAME_GRADE_D[];
extern char CUP_NAME_GRADE_C[];
extern char CUP_NAME_GRADE_B[];
extern char CUP_NAME_GRADE_A[];
extern char CUP_NAME_GRADE_S[];
extern char CUP_NAME_GRADE_R[];
extern char CUP_LETTERS_D_C[2][2];
extern char CUP_LETTERS_B_A[2][2];
extern char CUP_LETTERS_S_R[2][2];
extern char CUP_LETTERS_H_I[2][2];
extern char CUP_LETTERS_J_K[2][2];
extern char CUP_LETTERS_L_F[2][2];
extern char CUP_LETTERS_G_W[2][2];
extern char CUP_LETTERS_O_N[2][2];
extern char CUP_LETTERS_M_T[2][2];
extern char CUP_LETTERS_Y_Z[2][2];
extern char CUP_LETTERS_X_Q[2][2];
extern char SPEAKER_NAME_SIGN[];
extern char SPEAKER_NAME_BOX[];
extern char SPEAKER_NAME_BETAMON[];
extern char SPEAKER_NAME_TANEMON[];
extern char SPEAKER_NAME_PALMON[];
extern int32_t CURRENT_SCRIPT_PTR;

void renderSelectionCursor(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);
int32_t scriptCompareSigned(uint8_t op, uint32_t lhs, uint32_t rhs);
void renderItemSprite(int32_t itemId, int32_t x, int32_t y, int32_t depth);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t xPos, int32_t yPos, int32_t width, int32_t height);
void renderString(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8);
int32_t tickTextboxPageFlip(int32_t boxId);
int32_t hasMove(int32_t moveId);
void unlearnMove(int32_t moveId);
void loadMap(uint16_t mapId);
void renderMonochromonMoodBubble(int32_t instanceId);
void translateConditionFXToEntity(Entity *entity, SVECTOR *out);
int32_t worldPosToScreenPos(SVECTOR *worldPos, DVECTOR *screenPos);
void closeTextbox(int32_t boxId, RECT *target);
void redrawItemMenuRow(ItemMenuBox *box, int32_t style);
void renderMoneyBox(void);
void tickItemMenu(void);
void renderItemMenuBox(void);
void itemMenuCursorDown(ItemMenuBox *box, int32_t style);
int32_t openItemDescriptionBox(ItemMenuBox *box, RECT *origin, int32_t uiBoxId);
void renderSizedTextbox(void);
int32_t shopFillBuyItemList(void);
int32_t fillInventoryItemList(void);
int32_t pickMeritItem(void);
void tickItemDescriptionBox(void);
void renderItemDescriptionBox(void);
void tickMoneyBox(void);
uint8_t *getShopText(int32_t idx);
uint8_t *getMapHeadText(int32_t section, int32_t idx);
void processInput(void);
int32_t isKeyDown(uint32_t key);
void waitForCrossRelease(void);
int32_t isXPressedAfterDialogue(void);
void *allocateArray(uint32_t size);
void freeArray(uint32_t *array);
uint8_t getRecycleId(uint8_t value);
int32_t tickItemMenuPageFlip(ItemMenuBox *box);
void itemMenuJumpToFirst(ItemMenuBox *box, int32_t startRow, int32_t style);
void itemMenuCursorUp(ItemMenuBox *box, int32_t style);
void itemMenuJumpToLast(ItemMenuBox *box, int32_t startRow, int32_t style);
void reservePopupTextRow(void);
int32_t flipMenuPage(uint8_t a);
void itemMenuSelectLast(ItemMenuBox *box);
void itemMenuSelectFirst(ItemMenuBox *box);
uint8_t *getItemMenuRowBuffer(ItemMenuBox *box, int32_t index);
void terminateString(uint8_t *str, int32_t flag);
uint8_t *padWithSpaces(uint8_t *str, int32_t width, int32_t used);
void setDigimonRaised(int32_t digimonId);
int32_t hasDigimonRaised(int32_t digimonId);
void unlockMedal(uint16_t medal);
int32_t hasMedal(uint16_t medal);
void MAIN_func_800FF900(void);
void checkShopMap(int32_t mapId);
void checkArenaMap(int32_t mapId);
void releaseTextboxRows(int32_t boxId);
int32_t tickDialogueAdvance(void);
void renderDialogueChoiceCursor(int32_t x, int32_t y);
void tickSizedTextbox(void);
void getVRAMModeCoords(int32_t mode, int32_t *outX, int32_t *outClut);
int32_t advanceTextbox(int32_t boxId);
void setupDialogueBox(uint8_t owner);
int32_t showTextboxFinalPage(int32_t boxId, int32_t speakerId);
void layoutQuantityBox(void);
void tickItemConfirmBox(void);
void renderItemConfirmBox(void);
int32_t openQuantityBox(RECT *origin);
int32_t openItemConfirmBox(RECT *origin);
void renderItemMenuHeader(int32_t boxId, int32_t idx, int16_t x, int16_t y);
void renderItemMenuScrollbar(ItemMenuBox *box);
void renderItemMenuRows(ItemMenuBox *box, int16_t x1, int16_t y1, int16_t x2, int16_t y2, int32_t flag);
void renderInsetFrame(int32_t boxId, int16_t x, int16_t y, int32_t w, int16_t h);
void renderTrianglePrimitive(int32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t otz, int32_t flag);
void renderCardIcon(int32_t spriteId, int16_t x, int16_t y, int32_t depth);
void layoutItemRow(ItemMenuBox *box, uint8_t row, int32_t isLast);
void layoutCardRow(ItemMenuBox *box, uint8_t row, int32_t isLast);
void layoutBgmTrackRow(ItemMenuBox *box, uint8_t row, int32_t isLast);
void layoutDestinationRow(ItemMenuBox *box, uint8_t row, int32_t isLast);
void layoutTradeRow(ItemMenuBox *box, uint8_t row, int32_t isLast);
void renderRaisedFrame(uint8_t boxId, int16_t x, int16_t y, int32_t w, int16_t h);
int32_t drawTextboxRows(int32_t boxId, int32_t flag);
int32_t tickDialogueChoice(void);
int32_t isDialogueFinished(void);
int32_t drawTextRow(uint8_t *str, int16_t x, int16_t y, int32_t flag);
int32_t getSpeakerName(int32_t speakerId, uint8_t *buf);
uint8_t *intToStringSJIS(uint8_t *buf, int32_t value, uint8_t digits, int32_t flag);
void renderHorizontalLine(uint8_t boxId, int16_t x, int16_t y, int32_t w);
void renderLinePrimitive(uint32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t order, uint32_t mode);
void renderSellItemBox(void);
void renderTextboxNextArrow(int32_t boxId);
void renderVerticalLine(int32_t boxId, int16_t x, int16_t y, int32_t h);
void tickSellItemBox(void);

static void *script_common_text_order[] = {
	scriptInstructionFBtoFF,
	tickScriptedMovements,
	readFileSection,
	enforceStatsLimits,
	scriptTestMoney,
	scriptTestItemCount,
	scriptTestPartnerCondition,
	scriptTestHasMove,
	scriptTestCardAmount,
	getStatsPointer,
	scriptTestStat,
	scriptCompareDate,
	scriptIdToEntityId,
	intToStringSJIS,
	getSpeakerName,
	showTextboxFinalPage,
	scriptSetTextboxSize,
	showTextbox,
	scriptShowSelection,
	renderTextboxNextArrow,
	renderScriptDialogueBox,
	tickScriptDialogueBox,
	setupBoxOrigin,
	setupDialogueBox,
	advanceTextbox,
	tickTextboxPageFlip,
	drawTextRow,
	registerTextbox,
	triggerBoxCloseFlag,
	createTextbox,
	closeBox,
	closeAllTextboxes,
	closeTextbox,
	getVRAMModeCoords,
	tickTextboxes,
	resetTextboxes,
	renderSizedTextbox,
	tickSizedTextbox,
	renderDialogueChoiceCursor,
	isDialogueFinished,
	tickDialogueAdvance,
	tickDialogueChoice,
	releaseTextboxRows,
	drawTextboxRows,
	checkArenaMap,
	loadShopLibrary,
	checkShopMap,
	MAIN_func_800FF900,
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
	getItemMenuRowBuffer,
	renderRaisedFrame,
	itemMenuSelectFirst,
	itemMenuSelectLast,
	redrawItemMenuRow,
	layoutTradeRow,
	layoutDestinationRow,
	layoutBgmTrackRow,
	layoutCardRow,
	layoutItemRow,
	flipMenuPage,
	lostAllLives,
	showMapHeadTextbox,
	renderCardIcon,
	renderInsetFrame,
	renderHorizontalLine,
	MAIN_func_800FDFB4,
	reservePopupTextRow,
	renderItemMenuRows,
	renderItemMenuScrollbar,
	renderItemMenuHeader,
	openItemDescriptionBox,
	itemMenuCursorDown,
	itemMenuJumpToLast,
	itemMenuCursorUp,
	itemMenuJumpToFirst,
	openItemConfirmBox,
	openQuantityBox,
	tickItemMenuPageFlip,
	layoutItemMenu,
	openItemMenu,
	getActiveItemMenu,
	tickPickItemMenu,
	openItemMenuBox,
	showShopkeeperSelection,
	openMoneyBox,
	destroyItemMenuBox,
	showShopkeeperTextbox,
	initializeItemMenuBox,
	tickItemShop,
	isPartnerBaby,
	dailyPStatTrigger,
	getRecycleId,
	freeArray,
	allocateArray,
	handleItemLoss,
	setInputRepeatMask,
	isXPressedAfterDialogue,
	waitForCrossRelease,
	isKeyDown,
	processInput,
	initialKeyInputs,
	renderMonochromonMoodBubble,
	getMapHeadText,
	getShopText,
	renderMoneyBox,
	tickMoneyBox,
	renderItemConfirmBox,
	tickItemConfirmBox,
	layoutQuantityBox,
	renderSellItemBox,
	tickSellItemBox,
	renderItemDescriptionBox,
	tickItemDescriptionBox,
	pickMeritItem,
	renderItemMenuBox,
	tickItemMenu,
	fillInventoryItemList,
	shopFillBuyItemList,
};

// clang-format off
char BGM_NAME_NON_BEWILDERING_FOREST[] = "Non Bewildering Forest Theme";
char BGM_NAME_NON_BEWILDERING_FOREST_NIGHT[] = "Non Bewildering Forest Night Theme";
char BGM_NAME_TROPICAL[] = "Tropical Theme";
char BGM_NAME_TROPICAL_NIGHT[] = "Tropical Night Theme";
char BGM_NAME_MT_PANORAMA[] = "Mt. Panorama Theme";
char BGM_NAME_MT_PANORAMA_NIGHT[] = "Mt. Panorama Night Theme";
char BGM_NAME_DRILL_TUNNEL[] = "Drill Tunnel Theme";
char BGM_NAME_OGRE_FORTRESS[20] = "Ogre Fortress Theme";
char BGM_NAME_OVERDELL_CEMETERY[24] = "Overdell Cemetary Theme";
char BGM_NAME_CANYON[] = "Canyon Theme";
char BGM_NAME_OGREMON_2[20] = "Ogremon Theme No. 2";
char BGM_NAME_EVERYTHING_SHOP[] = "Everything Shop Theme";
char BGM_NAME_OGREMON_3[] = "Ogremon ThemeNo. 3";
char BGM_NAME_LAVA_CAVE[16] = "Lava Cave Theme";
char BGM_NAME_DARK_ARISTOCRATS_MANSION[] = "Dark Aristcrat's Mansion Theme";
char BGM_NAME_UNDERGROUND_LAB[] = "Underground Lab Theme";
char BGM_NAME_GEAR_SAVANNA[] = "Gear Savanna Theme";
char BGM_NAME_GEAR_SAVANNA_NIGHT[] = "Gear Savanna Night Theme";
char BGM_NAME_LEOMON[] = "Leomon Theme";
char BGM_NAME_AMIDA_FOREST[] = "Amida Forest Theme";
char BGM_NAME_AMIDA_FOREST_NIGHT[] = "Amida Forest Night Theme";
char BGM_NAME_DINO_SPEEDY_TIME_ZONE[] = "The Ancient Region of Dino Speedy Time Zone Theme";
char BGM_NAME_DINO_SPEEDY_TIME_ZONE_NIGHT[56] = "The Ancient Region of Dino Speedy Time Zone Night Theme";
char BGM_NAME_DINO_GLACIAL_TIME_ZONE[] = "The Ancient Region of Dino Glacial Time Zone Theme";
char BGM_NAME_DINO_GLACIAL_TIME_ZONE_NIGHT[] = "The Ancient Region of Dino Glacial Time Zone Night Theme";
char BGM_NAME_FREEZELAND[] = "Freezeland Theme";
char BGM_NAME_FREEZELAND_NIGHT[] = "Freezeland Night Theme";
char BGM_NAME_IGLOO[12] = "Igloo Theme";
char BGM_NAME_CURLING[] = "Curling Theme";
char BGM_NAME_SANCTUARY[16] = "Sanctuary Theme";
char BGM_NAME_SANCTUARY_BELOW[] = "Sanctuary Below Theme";
char BGM_NAME_GECKO_SWAMP[] = "Gecko Swamp Theme";
char BGM_NAME_GECKO_SWAMP_NIGHT[24] = "Gecko Swamp Night Theme";
char BGM_NAME_MISTY_TREES[] = "Misty Trees Theme";
char BGM_NAME_MISTY_TREES_NIGHT[24] = "Misty Trees Night Theme";
char BGM_NAME_WARUMONZAEMON[20] = "WaruMonzaemon Theme";
char BGM_NAME_TOY_TOWN[] = "Toy Town Theme";
char BGM_NAME_FACTORIAL[] = "Theme of Factorial";
char BGM_NAME_FACTORIAL_NIGHT[] = "Factorial Night Theme";
char BGM_NAME_SEWER[12] = "Sewer Theme";
char BGM_NAME_TRASH_MOUNTAIN[] = "Trash Mountain Theme";
char BGM_NAME_TRASH_MOUNTAIN_NIGHT[] = "Trash Mountain Night Theme";
char BGM_NAME_BEATLAND[] = "Beatland Theme";
char BGM_NAME_BEATLAND_NIGHT[] = "Beatland Night Theme";
char BGM_NAME_SECRET_BEACH_CAVE[24] = "Secret Beach Cave Theme";
char BGM_NAME_LAST_ROOM[16] = "Last Room Theme";
char BGM_NAME_FILE_CITY[16] = "File City Theme";
char BGM_NAME_FILE_CITY_NIGHT[] = "File City Night Theme";
char BGM_NAME_TOURNAMENT_OPENING[24] = "Tornament Opening Theme";
char BGM_NAME_TOURNAMENT_PROGRESS[] = "Tornament Progress Theme";
char BGM_NAME_TOURNAMENT_CHAMPIONSHIP[] = "Tornament Championship Theme";
char BGM_NAME_PARTNERS_ENTRANCE[] = "Partner's Entrance Theme";
char BGM_NAME_COMPETITION_BATTLE_OPPONENTS_ENTRANCE[] = "Competition Battle Opponent's Entrance Theme";
char BGM_NAME_ARENA_BATTLE_1[] = "Arena Battle Theme No. 1";
char BGM_NAME_PARTNERS_WIN[20] = "Partner's Win Theme";
char BGM_NAME_PARTNERS_LOSS[] = "Partner's Loss Theme";
char BGM_NAME_ARENA_BATTLE_2[] = "Arena Battle Theme No. 2";
char BGM_NAME_ARENA_BATTLE_3[] = "Arena Battle Theme No. 3";
char BGM_NAME_EVENT_BATTLE[] = "Event Battle Theme";
char BGM_NAME_NORMAL_BATTLE[20] = "Normal Battle Theme";
char BGM_NAME_NORMAL_BATTLE_2[] = "Normal Battle  Theme No.2";
char BGM_NAME_LAST_BATTLE[] = "Last Battle Theme";
char CUP_NAME_VERSION_1[] = "Version 1 Cup";
char CUP_NAME_VERSION_2[] = "Version 2 Cup";
char CUP_NAME_VERSION_3[] = "Version 3 Cup";
char CUP_NAME_VERSION_4[] = "Version 4 Cup";
char CUP_NAME_VERSION_0[] = "Version 0 Cup";
char CUP_NAME_FIRE[] = "Fire Cup";
char CUP_NAME_GRAPPLE[12] = "Grapple Cup";
char CUP_NAME_THUNDER_WIND[] = "Thunder Wind Cup";
char CUP_NAME_COOL[] = "Cool Cup";
char CUP_NAME_NATURE[] = "Nature Cup";
char CUP_NAME_METALLIC[12] = "Metalic Cup";
char CUP_NAME_FILTH[] = "Filth Cup";
char CUP_NAME_DINO[] = "Dino Cup";
char CUP_NAME_WING[] = "Wing Cup";
char CUP_NAME_ANIMAL[] = "Animal Cup";
char CUP_NAME_HUMAN[] = "Human Cup";
char CUP_NAME_BEETLE[] = "Beetle Cup";

GsSPRITE MOOD_BUBBLE_SPRITE = {
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

MenuTextLayout QUANTITY_BOX_LAYOUT = {
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

ShopkeeperIdTable SHOPKEEPER_DIGIMON = {
	{ 0x97, 0x99, 0xa3, 0xa5, 0x82, 0x89, 0x87, 0xaa, 0x76, 0x80, 0xff },
};

int16_t MAIN_D_8012FE68[8] = {
	0x0083, 0x0085, 0x0086, 0x008d, 0x00cf, 0x00d3, 0x00d7, 0x00da,
};

uint8_t LOST_ITEM_IDS[78] = {
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
	BGM_NAME_NON_BEWILDERING_FOREST,
	BGM_NAME_NON_BEWILDERING_FOREST_NIGHT,
	BGM_NAME_TROPICAL,
	BGM_NAME_TROPICAL_NIGHT,
	BGM_NAME_MT_PANORAMA,
	BGM_NAME_MT_PANORAMA_NIGHT,
	BGM_NAME_DRILL_TUNNEL,
	BGM_NAME_OGRE_FORTRESS,
	BGM_NAME_OVERDELL_CEMETERY,
	BGM_NAME_CANYON,
	BGM_NAME_OGREMON_2,
	BGM_NAME_EVERYTHING_SHOP,
	BGM_NAME_OGREMON_3,
	BGM_NAME_LAVA_CAVE,
	BGM_NAME_DARK_ARISTOCRATS_MANSION,
	BGM_NAME_UNDERGROUND_LAB,
	BGM_NAME_GEAR_SAVANNA,
	BGM_NAME_GEAR_SAVANNA_NIGHT,
	BGM_NAME_LEOMON,
	BGM_NAME_AMIDA_FOREST,
	BGM_NAME_AMIDA_FOREST_NIGHT,
	BGM_NAME_DINO_SPEEDY_TIME_ZONE,
	BGM_NAME_DINO_SPEEDY_TIME_ZONE_NIGHT,
	BGM_NAME_DINO_GLACIAL_TIME_ZONE,
	BGM_NAME_DINO_GLACIAL_TIME_ZONE_NIGHT,
	BGM_NAME_FREEZELAND,
	BGM_NAME_FREEZELAND_NIGHT,
	BGM_NAME_IGLOO,
	BGM_NAME_CURLING,
	BGM_NAME_SANCTUARY,
	BGM_NAME_SANCTUARY_BELOW,
	BGM_NAME_GECKO_SWAMP,
	BGM_NAME_GECKO_SWAMP_NIGHT,
	BGM_NAME_MISTY_TREES,
	BGM_NAME_MISTY_TREES_NIGHT,
	BGM_NAME_WARUMONZAEMON,
	BGM_NAME_TOY_TOWN,
	BGM_NAME_FACTORIAL,
	BGM_NAME_FACTORIAL_NIGHT,
	BGM_NAME_SEWER,
	BGM_NAME_TRASH_MOUNTAIN,
	BGM_NAME_TRASH_MOUNTAIN_NIGHT,
	BGM_NAME_BEATLAND,
	BGM_NAME_BEATLAND_NIGHT,
	BGM_NAME_SECRET_BEACH_CAVE,
	BGM_NAME_MT_PANORAMA,
	BGM_NAME_LAST_ROOM,
	BGM_NAME_FILE_CITY,
	BGM_NAME_FILE_CITY_NIGHT,
	BGM_NAME_TOURNAMENT_OPENING,
	BGM_NAME_TOURNAMENT_PROGRESS,
	BGM_NAME_TOURNAMENT_CHAMPIONSHIP,
	BGM_NAME_PARTNERS_ENTRANCE,
	BGM_NAME_COMPETITION_BATTLE_OPPONENTS_ENTRANCE,
	BGM_NAME_ARENA_BATTLE_1,
	BGM_NAME_PARTNERS_WIN,
	BGM_NAME_PARTNERS_LOSS,
	BGM_NAME_ARENA_BATTLE_2,
	BGM_NAME_ARENA_BATTLE_3,
	BGM_NAME_EVENT_BATTLE,
	BGM_NAME_NORMAL_BATTLE,
	BGM_NAME_NORMAL_BATTLE_2,
	BGM_NAME_LAST_BATTLE,
};

int32_t CARD_PRICES[5] = {
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

char *TOURNAMENT_CUP_NAMES[23] = {
	CUP_NAME_GRADE_D,
	CUP_NAME_GRADE_C,
	CUP_NAME_GRADE_B,
	CUP_NAME_GRADE_A,
	CUP_NAME_GRADE_S,
	CUP_NAME_GRADE_R,
	CUP_NAME_VERSION_1,
	CUP_NAME_VERSION_2,
	CUP_NAME_VERSION_3,
	CUP_NAME_VERSION_4,
	CUP_NAME_VERSION_0,
	CUP_NAME_FIRE,
	CUP_NAME_GRAPPLE,
	CUP_NAME_THUNDER_WIND,
	CUP_NAME_COOL,
	CUP_NAME_NATURE,
	CUP_NAME_METALLIC,
	CUP_NAME_FILTH,
	CUP_NAME_DINO,
	CUP_NAME_WING,
	CUP_NAME_ANIMAL,
	CUP_NAME_HUMAN,
	CUP_NAME_BEETLE,
};

char *TOURNAMENT_GRADES[23] = {
	CUP_LETTERS_D_C[0],
	CUP_LETTERS_D_C[1],
	CUP_LETTERS_B_A[0],
	CUP_LETTERS_B_A[1],
	CUP_LETTERS_S_R[0],
	CUP_LETTERS_S_R[1],
	CUP_LETTERS_H_I[0],
	CUP_LETTERS_H_I[1],
	CUP_LETTERS_J_K[0],
	CUP_LETTERS_J_K[1],
	CUP_LETTERS_L_F[0],
	CUP_LETTERS_L_F[1],
	CUP_LETTERS_G_W[0],
	CUP_LETTERS_G_W[1],
	CUP_LETTERS_O_N[0],
	CUP_LETTERS_O_N[1],
	CUP_LETTERS_M_T[0],
	CUP_LETTERS_M_T[1],
	CUP_LETTERS_Y_Z[0],
	CUP_LETTERS_G_W[1],
	CUP_LETTERS_Y_Z[1],
	CUP_LETTERS_X_Q[0],
	CUP_LETTERS_X_Q[1],
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

TransportDestination TRANSPORT_DESTINATIONS[6] = {
	{ 0x26, 0x09, 0x00dd, 0x000003e8 },
	{ 0x46, 0x09, 0x00be, 0x000003e8 },
	{ 0x4f, 0x09, 0x00bc, 0x000005dc },
	{ 0x5d, 0x09, 0x015f, 0x000007d0 },
	{ 0x77, 0x09, 0x0093, 0x000009c4 },
	{ 0x69, 0x00, 0x00d2, 0x000009c4 },
};

int16_t ITEM_MENU_RECTS[32] = {
	0xffc9, 0xff9e, 0x00be, 0x0081, 0xffb9, 0xff9e, 0x00de, 0x0081,
	0xffb9, 0xff9e, 0x00de, 0x0081, 0xffc9, 0xff9e, 0x00be, 0x0081,
	0xffb9, 0xff9e, 0x00de, 0x0081, 0xffdc, 0xff9e, 0x00a6, 0x0081,
	0xffdc, 0xff9e, 0x00a6, 0x0081, 0xffc9, 0xff9e, 0x00be, 0x0081,
};

RECT ITEM_MENU_POPUP_ORIGINS[8] = {
	{ 5, 17, 170, 18 },
	{ 5, 17, 202, 18 },
	{ 5, 17, 202, 18 },
	{ 5, 17, 170, 18 },
	{ 5, 17, 202, 18 },
	{ 5, 17, 146, 18 },
	{ 5, 17, 146, 18 },
	{ 5, 17, 170, 18 },
};

int16_t ITEM_MENU_CURSOR_WIDTHS[8] = {
	0x00aa, 0x00ca, 0x00ca, 0x00aa, 0x00ca, 0x0092, 0x0092, 0x00aa,
};

char SPEAKER_NAME_COELAMON[] = "Coelamon";

int16_t STAT_LIMITS[22] = {
	0x03e7, 0x03e7, 0x03e7, 0x03e7, 0x270f, 0x270f, 0x270f, 0x270f,
	0x0064, 0x0064, 0x03e7, 0x03e7, 0x03e7, 0x03e7, 0x270f, 0x270f,
	0x270f, 0x03e7, 0x03e7, 0x03e7, 0x0063, 0x000a,
};

Pow10Table POWERS_OF_TEN = {
	{
		0x00000001, 0x0000000a, 0x00000064, 0x000003e8,
		0x00002710, 0x000186a0,
	},
};

char *SPECIAL_SPEAKER_NAMES[6] = {
	SPEAKER_NAME_SIGN,
	SPEAKER_NAME_BOX,
	SPEAKER_NAME_BETAMON,
	SPEAKER_NAME_COELAMON,
	SPEAKER_NAME_TANEMON,
	SPEAKER_NAME_PALMON,
};

char MAPHEAD_FILE_PATH[] = "\\SCN\\MAPHEAD.SCN";
char SCRIPT_FILE_PATH[12] = "\\SCN\\DG.SCN";
char PATH_ETCHI_BOSS_EFE_TMD[20] = "\\ETCHI\\BOSS_EFE.TMD";
char PATH_ETCHI_OP_TIM[] = "\\ETCHI\\OP.TIM";

uint8_t JUKEBOX_TRACKS[128] = {
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

uint8_t TRADE_WANTED_ITEMS[12] = {
	0x2c, 0x29, 0x45, 0x27, 0x41, 0x11, 0x09, 0x01,
	0x3e, 0x00, 0x00, 0x00,
};

uint8_t TRADE_GIVEN_ITEMS[12] = {
	0x01, 0x09, 0x61, 0x16, 0x0b, 0x0e, 0x13, 0x14,
	0x15, 0x00, 0x00, 0x00,
};

char NAMING_ROW_HIRAGANA_A[] = "あいうえお";

/* sic: と instead of こ in the original */
char NAMING_ROW_HIRAGANA_KA[] = "かきくけと";

char NAMING_ROW_HIRAGANA_SA[] = "さしすせそ";

char NAMING_ROW_HIRAGANA_TA[] = "たちつてと";

char NAMING_ROW_HIRAGANA_NA[] = "なにぬねの";

char NAMING_ROW_HIRAGANA_HA[] = "はひふへほ";

char NAMING_ROW_HIRAGANA_MA[] = "まみむめも";

char NAMING_ROW_HIRAGANA_YA[] = "や　ゆ　よ";

char NAMING_ROW_HIRAGANA_RA[] = "らりるれろ";

char *NAMING_PAGE_HIRAGANA[9] = {
	NAMING_ROW_HIRAGANA_A,
	NAMING_ROW_HIRAGANA_KA,
	NAMING_ROW_HIRAGANA_SA,
	NAMING_ROW_HIRAGANA_TA,
	NAMING_ROW_HIRAGANA_NA,
	NAMING_ROW_HIRAGANA_HA,
	NAMING_ROW_HIRAGANA_MA,
	NAMING_ROW_HIRAGANA_YA,
	NAMING_ROW_HIRAGANA_RA,
};

char NAMING_ROW_HIRAGANA_WA[] = "わ　を　ん";

char NAMING_ROW_HIRAGANA_GA[] = "がぎぐげご";

char NAMING_ROW_HIRAGANA_ZA[] = "ざじずぜぞ";

char NAMING_ROW_HIRAGANA_DA[] = "だぢづでど";

char NAMING_ROW_HIRAGANA_BA[] = "ばびぶべぼ";

char NAMING_ROW_HIRAGANA_PA[] = "ぱぴぷぺぽ";

char NAMING_ROW_HIRAGANA_SMALL_A[] = "ぁぃぅぇぉ";

char NAMING_ROW_HIRAGANA_SMALL_TU[] = "っゃゅょー";

char NAMING_BLANK_ROW[] = "　　　　　";

char *NAMING_PAGE_HIRAGANA_2[9] = {
	NAMING_ROW_HIRAGANA_WA,
	NAMING_ROW_HIRAGANA_GA,
	NAMING_ROW_HIRAGANA_ZA,
	NAMING_ROW_HIRAGANA_DA,
	NAMING_ROW_HIRAGANA_BA,
	NAMING_ROW_HIRAGANA_PA,
	NAMING_ROW_HIRAGANA_SMALL_A,
	NAMING_ROW_HIRAGANA_SMALL_TU,
	NAMING_BLANK_ROW,
};

char NAMING_ROW_KATAKANA_A[] = "アイウエオ";

char NAMING_ROW_KATAKANA_KA[] = "カキクケコ";

char NAMING_ROW_KATAKANA_SA[] = "サシスセソ";

char NAMING_ROW_KATAKANA_TA[] = "タチツテト";

char NAMING_ROW_KATAKANA_NA[] = "ナニヌネノ";

char NAMING_ROW_KATAKANA_HA[] = "ハヒフヘホ";

char NAMING_ROW_KATAKANA_MA[] = "マミムメモ";

char NAMING_ROW_KATAKANA_YA[] = "ヤ　ユ　ヨ";

char NAMING_ROW_KATAKANA_RA[] = "ラリルレロ";

char *NAMING_PAGE_KATAKANA[9] = {
	NAMING_ROW_KATAKANA_A,
	NAMING_ROW_KATAKANA_KA,
	NAMING_ROW_KATAKANA_SA,
	NAMING_ROW_KATAKANA_TA,
	NAMING_ROW_KATAKANA_NA,
	NAMING_ROW_KATAKANA_HA,
	NAMING_ROW_KATAKANA_MA,
	NAMING_ROW_KATAKANA_YA,
	NAMING_ROW_KATAKANA_RA,
};

char NAMING_ROW_KATAKANA_WA[] = "ワ　ヲ　ン";

char NAMING_ROW_KATAKANA_GA[] = "ガギグゲゴ";

char NAMING_ROW_KATAKANA_ZA[] = "ザジズゼゾ";

char NAMING_ROW_KATAKANA_DA[] = "ダヂヅデド";

char NAMING_ROW_KATAKANA_BA[] = "バビブベボ";

char NAMING_ROW_KATAKANA_PA[] = "パピプペポ";

char NAMING_ROW_KATAKANA_SMALL_A[] = "ァィゥェォ";

char NAMING_ROW_KATAKANA_SMALL_TU[] = "ッャュョー";

char *NAMING_PAGE_KATAKANA_2[9] = {
	NAMING_ROW_KATAKANA_WA,
	NAMING_ROW_KATAKANA_GA,
	NAMING_ROW_KATAKANA_ZA,
	NAMING_ROW_KATAKANA_DA,
	NAMING_ROW_KATAKANA_BA,
	NAMING_ROW_KATAKANA_PA,
	NAMING_ROW_KATAKANA_SMALL_A,
	NAMING_ROW_KATAKANA_SMALL_TU,
	NAMING_BLANK_ROW,
};

char NAMING_ROW_UPPER_A[] = "ＡＢＣＤＥ";

char NAMING_ROW_UPPER_F[] = "ＦＧＨＩＪ";

char NAMING_ROW_UPPER_K[] = "ＫＬＭＮＯ";

char NAMING_ROW_UPPER_P[] = "ＰＱＲＳＴ";

char NAMING_ROW_UPPER_U[] = "ＵＶＷＸＹ";

char NAMING_ROW_UPPER_Z[] = "Ｚ　　　　";

char *NAMING_PAGE_UPPERCASE[9] = {
	NAMING_ROW_UPPER_A,
	NAMING_ROW_UPPER_F,
	NAMING_ROW_UPPER_K,
	NAMING_ROW_UPPER_P,
	NAMING_ROW_UPPER_U,
	NAMING_ROW_UPPER_Z,
	NAMING_BLANK_ROW,
	NAMING_BLANK_ROW,
	NAMING_BLANK_ROW,
};

char NAMING_ROW_LOWER_a[] = "ａｂｃｄｅ";

char NAMING_ROW_LOWER_f[] = "ｆｇｈｉｊ";

char NAMING_ROW_LOWER_k[] = "ｋｌｍｎｏ";

char NAMING_ROW_LOWER_p[] = "ｐｑｒｓｔ";

char NAMING_ROW_LOWER_u[] = "ｕｖｗｘｙ";

char NAMING_ROW_LOWER_z[] = "ｚ　　　　";

char NAMING_ROW_DIGITS_0[] = "０１２３４";

char NAMING_ROW_DIGITS_5[] = "５６７８９";

char *NAMING_PAGE_LOWERCASE[9] = {
	NAMING_ROW_LOWER_a,
	NAMING_ROW_LOWER_f,
	NAMING_ROW_LOWER_k,
	NAMING_ROW_LOWER_p,
	NAMING_ROW_LOWER_u,
	NAMING_ROW_LOWER_z,
	NAMING_ROW_DIGITS_0,
	NAMING_ROW_DIGITS_5,
	NAMING_BLANK_ROW,
};

char **NAMING_CHAR_PAGES[6] = {
	NAMING_PAGE_UPPERCASE,
	NAMING_PAGE_LOWERCASE,
	NAMING_PAGE_KATAKANA,
	NAMING_PAGE_KATAKANA_2,
	NAMING_PAGE_HIRAGANA,
	NAMING_PAGE_HIRAGANA_2,
};

int16_t NAMING_LABEL_LAYOUT[10] = {
	0x000e, 0x0006, 0x0030, 0x000e, 0x005a, 0x0030, 0x000e, 0x0076,
	0x0018, 0x0000,
};

uint16_t MAIN_D_801307A0[10] = {
	0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000,
	0x8000, 0x0000,
};

uint8_t ITEM_KEEPER_HAND_LABEL[12] = "In hand";

uint8_t ITEM_KEEPER_STORED_LABEL[12] = "Keeping";

char TRADE_HEADER_LABELS[20] = "You have Will trade";
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
	itemList = MAIN_D_80134F68->buf;
	canBuyAnything = 0;
	hasSpace = 0;
	MAIN_D_80134F68->itemCount = 0;

	for (slotId = 0; slotId < inventorySize; slotId++) {
		if (INVENTORY.types.array[slotId] == 0xff) {
			hasSpace = 1;

			break;
		}
	}

	for (itemId = 0; itemId < 128; itemId++) {
		if (!isTriggerSet(TRIGGER_ITEM_IN_SHOPS + itemId)) {
			continue;
		}

		MAIN_D_80134F68->itemCount++;
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

int32_t fillInventoryItemList(void)
{
	int32_t i;
	uint8_t *buf;
	int32_t result;
	int32_t type;
	int32_t amount;

	result = 0;
	buf = MAIN_D_80134F6C->buf;
	MAIN_D_80134F6C->itemCount = 0;

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

		MAIN_D_80134F6C->itemCount++;
		*buf++ = type;
		*buf++ = amount;
	}

	return result;
}

/*
 * Cross picks the row, triangle leaves, up and down move (to the ends with
 * R1 held) and start shows the description of the item.
 */
void tickItemMenu(void)
{
	ItemMenuBox *box;
	RECT rect;
	RECT *src;

	box = getActiveItemMenu();

	if (tickItemMenuPageFlip(box) != 0) {
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

	if (SCRIPT_PRICE != 0) {
		return;
	}

	if (SELECTION_MENU_STATE != 1) {
		return;
	}

	if (isKeyDown(PADRdown)) {
		src = &ITEM_MENU_POPUP_ORIGINS[ITEM_MENU_MODE];
		setRECT(&rect, src->x, src->y, src->w, src->h);

		switch (ITEM_MENU_MODE) {
		default:
			openQuantityBox(&rect);
			break;
		case ITEM_MENU_MERIT_ITEM:
			pickMeritItem();
			break;
		case ITEM_MENU_PICK_ITEM:
			openItemConfirmBox(&rect);
			break;
		}
	} else if (isKeyDown(PADRup)) {
		if (ITEM_MENU_MODE == ITEM_MENU_MERIT_ITEM) {
			SELECTION_MENU_STATE = 0xa;
		} else if (ITEM_MENU_MODE == ITEM_MENU_PICK_ITEM) {
			if (isTriggerSet(3) != 0) {
				writePStat(PSTAT_BUILTIN_ARG, 0xff);
				unsetTrigger(3);
				SELECTION_MENU_STATE = 4;
			}
		} else {
			SELECTION_MENU_STATE = 8;
		}

		playSound(0, 4);
	} else if (isKeyDown(PADLup)) {
		if (POLLED_INPUT & PADR1) {
			itemMenuJumpToFirst(box, 9, 0);
		} else {
			itemMenuCursorUp(box, 0);
		}
	} else if (isKeyDown(PADLdown)) {
		if (POLLED_INPUT & PADR1) {
			itemMenuJumpToLast(box, 9, 0);
		} else {
			itemMenuCursorDown(box, 0);
		}
	} else if (isKeyDown(PADstart)) {
		src = &ITEM_MENU_POPUP_ORIGINS[ITEM_MENU_MODE];
		setRECT(&rect, src->x, src->y, src->w, src->h);
		openItemDescriptionBox(box, &rect, 1);
		playSound(0, 3);
	}
}

int32_t pickMeritItem(void)
{
	ItemMenuBox *box;
	int32_t idx;

	box = getActiveItemMenu();
	idx = (box->topRow + box->cursor) * 2;
	SELECTED_ITEM = box->buf[idx];

	if (SELECTED_ITEM != 0xff && box->buf[idx + 1] != 0) {
		SCRIPT_PRICE = ITEM_PARA[SELECTED_ITEM].meritValue;
		SELECTION_MENU_STATE = 0xb;
		SCRIPT_STATE_3 = 0;
		playSound(0, 3);
		return 1;
	}

	playSound(0, 0xb);

	return 0;
}

void tickItemDescriptionBox(void)
{
	if (UI_BOX_DATA[3].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (isKeyDown(PADRdown | PADRup | PADstart) == 0) {
		return;
	}

	triggerBoxCloseFlag(3);
	playSound(0, 3);
}

void renderItemDescriptionBox(void)
{
	renderString(0, (int16_t)(UI_BOX_DATA[3].finalPos.x + 6),
	             (int16_t)(UI_BOX_DATA[3].finalPos.y + 5), 0xfc, 0xc, 0,
	             (int16_t)(TEXT_BOX_DATA[3].vramRow * 12), 3, 1);
}

void renderSellItemBox(void)
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

	layout = QUANTITY_BOX_LAYOUT;
	rowPx = TEXT_BOX_DATA[3].vramRow * 12;
	x = UI_BOX_DATA[3].finalPos.x;
	y = UI_BOX_DATA[3].finalPos.y;
	renderHorizontalLine(3, 4, 0x17, 0x7a);
	renderHorizontalLine(3, 0xc, 0x3c, 0x6a);
	renderInsetFrame(3, 0x55, 0x2a, 0x1a, 0xe);
	sx = x + 8;
	sy = y + 5;

	if (ITEM_MENU_MODE < ITEM_MENU_BUY_CARD) {
		renderItemSprite(SELECTED_ITEM, sx, sy, 3);
	} else {
		card = (CardData *)(SELECTED_ITEM * 4);
		card = (CardData *)((uint32_t)card + (uint32_t)CARD_DATA);
		sx += 2;
		sy += 2;
		renderCardIcon(card->spriteId, sx, sy, 3);
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

void renderItemMenuBox(void)
{
	int16_t bx;
	int16_t by;
	int16_t cy;
	ItemMenuBox *box;

	bx = UI_BOX_DATA[1].finalPos.x;
	by = UI_BOX_DATA[1].finalPos.y;
	renderItemMenuHeader(1, 0, bx + 8, by + 5);
	if (ITEM_MENU_MODE != ITEM_MENU_PICK_ITEM) {
		if (ITEM_MENU_MODE != ITEM_MENU_MERIT_ITEM) {
			renderItemMenuHeader(1, 1, bx + 0x80, by + 5);
			if (ITEM_MENU_MODE != ITEM_MENU_BUY) {
				renderItemMenuHeader(1, 3, bx + 0xb6, by + 5);
			}
		} else {
			renderItemMenuHeader(1, 2, bx + 0x80, by + 5);
		}
	} else {
		renderItemMenuHeader(1, 3, bx + 0x80, by + 5);
	}
	box = getActiveItemMenu();
	renderItemMenuScrollbar(box);
	cy = by + box->cursor * 0x12 + 0x11;
draw:
	renderSelectionCursor(bx + 5, cy, ITEM_MENU_CURSOR_WIDTHS[ITEM_MENU_MODE], 0x12, 5);
	renderItemMenuRows(box, bx + 0x1a, by + 0x13, bx + 8, by + 0x12, 0);
}

/* Lays out the name, unit price, quantity and total of the quantity box. */
void layoutQuantityBox(void)
{
	uint8_t *out;
	char *name;
	uint32_t total;

	out = TEXT_BUFFERS_PTR + (POPUP_TEXT_ROW << 6);

	if (ITEM_MENU_MODE < ITEM_MENU_BUY_CARD) {
		strcpy(out, ITEM_PARA[SELECTED_ITEM].name);
		out += strlen(ITEM_PARA[SELECTED_ITEM].name);
	} else {
		strcpy(out, name = DIGIMON_DATA[CARD_DATA[SELECTED_ITEM].digimonId].name);
		out += strlen(name);
	}

	*out++ = TEXT_COLUMN_96;
	*out++ = 0;
	out = intToStringSJIS(out, SELECTED_ITEM_PRICE, 5, 0);
	*out++ = TEXT_COLUMN_156;
	*out++ = 0;
	out = intToStringSJIS(out, MAIN_D_80134F81, 2, 0);
	*out++ = TEXT_COLUMN_180;
	*out++ = 0;

	total = SELECTED_ITEM_PRICE * MAIN_D_80134F81;
	if (total >= 1000000) {
		total = 999999;
	}

	out = intToStringSJIS(out, total, 6, 0);
	*out++ = 0;
	*out = 0;
	TEXT_BOX_DATA[3].pageReady = 1;
	TEXT_BOX_DATA[3].writeCount++;
}

/* The "Is it OK? Yes No" box; MAIN_D_80134F81 is 0 for Yes. */
void tickItemConfirmBox(void)
{
	ItemMenuBox *box;
	uint8_t amount;

	if (UI_BOX_DATA[3].state != 1) {
		return;
	}

	if (isXPressedAfterDialogue() == 0) {
		return;
	}

	if (isKeyDown(PADRup)) {
		triggerBoxCloseFlag(3);
		playSound(0, 4);
		return;
	}

	if (isKeyDown(PADLleft)) {
		MAIN_D_80134F81 = 0;
		playSound(0, 2);
		return;
	}

	if (isKeyDown(PADLright)) {
		MAIN_D_80134F81 = 1;
		playSound(0, 2);
		return;
	}

	if (isKeyDown(PADRdown)) {
		triggerBoxCloseFlag(3);
		if (MAIN_D_80134F81 == 0) {
			if (ITEM_MENU_MODE != ITEM_MENU_PICK_ITEM) {
				box = getActiveItemMenu();
				amount = getCardAmount(SELECTED_ITEM);
				amount = amount + 1u;
				setCardAmount(SELECTED_ITEM, amount);
				MONEY -= CARD_PRICES[CARD_DATA[SELECTED_ITEM].spriteId];
				SCRIPT_STATE_PTR->smth[box->cursor] = 0xff;
				MONEY_BOX_DIRTY = 1;
				SELECTION_MENU_STATE = 4;
				MAIN_func_800FDFB4();
			} else {
				writePStat(PSTAT_BUILTIN_ARG, SELECTED_ITEM);
				unsetTrigger(3);
				SELECTION_MENU_STATE = 4;
				playSound(0, 3);
			}
		} else {
			playSound(0, 4);
		}
	}
}

void renderItemConfirmBox(void)
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
	renderSelectionCursor(x + 8 + MAIN_D_80134F81 * 47, y + 0x12, 0x28,
	                      0xe, 3);
}

void tickMoneyBox(void)
{
	int32_t line;
	uint8_t saved;

	if (MONEY_BOX_DIRTY != 0) {
		MONEY_BOX_DIRTY = 0;

		if (MONEY_BOX_SHOWS_BITS != 0) {
			line = SHOP_TEXT_BITS;
		} else {
			line = SHOP_TEXT_POINTS;
		}

		saved = ACTIVE_INSTRUCTION;
		showShopkeeperTextbox(line, SPEAKER_NONE, 2);
		ACTIVE_INSTRUCTION = saved;
	}
}

/* Returns message idx of the shop run by the NPC in pstat 254. */
uint8_t *getShopText(int32_t idx)
{
	ShopkeeperIdTable shopkeeperIds;
	int32_t section;
	int32_t pstat;
	int32_t i;
	int32_t npcType;

	shopkeeperIds = SHOPKEEPER_DIGIMON;
	pstat = readPStat(PSTAT_BUILTIN_ARG) & 0xff;

	if (pstat == 0xff) {
		section = MAPHEAD_TEXT_COELAMON_SHOP;
	} else {
		pstat = scriptIdToEntityId(pstat) & 0xff;
		if (pstat != 0xff) {
			npcType =
				*(int32_t *)(&NPC_ENTITIES[pstat - 2]) & 0xff;
			for (i = 0; (uint32_t)i < 0xb; i++) {
				if (npcType == shopkeeperIds.b[i]) {
					section = (i + MAPHEAD_TEXT_SHOP) & 0xffff;
					goto done;
				}
			}
		}
		section = MAPHEAD_TEXT_SHOP;
	}
done:
	return getMapHeadText(section, idx);
}

/*
 * Returns message idx of a MAPHEAD_TEXT_* table. The table is a list of jumps
 * to the text instructions, and this returns the text after the opcode.
 */
uint8_t *getMapHeadText(int32_t section, int32_t idx)
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

	offset = 0x10 - (MOOD_BUBBLE_TIMER >> 1);
	scale = offset << 8;
	MOOD_BUBBLE_SPRITE.x = screen.vx;
	MOOD_BUBBLE_SPRITE.y = screen.vy - offset;
	MOOD_BUBBLE_SPRITE.scalex = scale;
	MOOD_BUBBLE_SPRITE.scaley = scale;
	GsSortSprite(&MOOD_BUBBLE_SPRITE, ACTIVE_ORDERING_TABLE, depth >> 4);

	if (MOOD_BUBBLE_TIMER != 0) {
		MOOD_BUBBLE_TIMER--;
	}
}

void initialKeyInputs(void)
{
	INPUT_ACCEPTED = 0;
	INPUT_RELEASED = -1;
	INPUT_REPEAT_MASK = 0;
	INPUT_REPEAT_TIMER = 0;
}

void processInput(void)
{
	uint32_t held;

	INPUT_RELEASED |= ~POLLED_INPUT;
	held = POLLED_INPUT & INPUT_REPEAT_MASK;
	if (held == 0) {
		INPUT_REPEAT_TIMER = 0;
	}

	if (INPUT_REPEAT_MASK != 0) {
		if (held == (POLLED_INPUT_PREVIOUS & INPUT_REPEAT_MASK)) {
			if (INPUT_REPEAT_TIMER == 8) {
				INPUT_REPEAT_TIMER = 6;
			} else if (INPUT_REPEAT_TIMER != 6) {
				held = 0;
			}

			INPUT_REPEAT_TIMER += 1;
		} else {
			INPUT_REPEAT_TIMER = 0;
			INPUT_RELEASED |= INPUT_REPEAT_MASK;
			held = 0;
		}
	}

	INPUT_ACCEPTED = held | (POLLED_INPUT & INPUT_RELEASED);
}

int32_t isKeyDown(uint32_t key)
{
	if ((INPUT_ACCEPTED & key) == 0) {
		return 0;
	}

	INPUT_RELEASED &= ~key;

	return 1;
}

void waitForCrossRelease(void)
{
	WAIT_CROSS_RELEASE = 1;
}

int32_t isXPressedAfterDialogue(void)
{
	if (WAIT_CROSS_RELEASE != 0) {
		if ((POLLED_INPUT & 0x40) != 0) {
			return 0;
		}

		WAIT_CROSS_RELEASE = 0;
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

		slot = pool[random(count)];
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
		if (value == LOST_ITEM_IDS[i]) {
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
		r = random(0x40);
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

/*
 * The quantity box of the shops. Up and down change the quantity (by ten with
 * circle held), square picks the most, cross buys or sells.
 */
void tickSellItemBox(void)
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

	if (MAIN_D_80134F81 != QUANTITY_MAX && isKeyDown(PADLup)) {
		if (POLLED_INPUT & PADRright) {
			MAIN_D_80134F81 += 10;
			if (MAIN_D_80134F81 > QUANTITY_MAX) {
				MAIN_D_80134F81 -= 10;
			}
		} else {
			MAIN_D_80134F81 += 1;
		}

		playSound(0, 2);
		layoutQuantityBox();

		return;
	}
	if (MAIN_D_80134F81 != 1 && isKeyDown(PADLdown)) {
		if (POLLED_INPUT & PADRright) {
			q = MAIN_D_80134F81;
			q -= 10;
			if (q <= 0) {
				q += 10;
			}
			MAIN_D_80134F81 = q;
		} else {
			MAIN_D_80134F81 -= 1;
		}

		playSound(0, 2);
		layoutQuantityBox();

		return;
	}

	if (isKeyDown(PADRup)) {
		triggerBoxCloseFlag(3);
		playSound(0, 4);
	} else if (isKeyDown(PADRleft)) {
		MAIN_D_80134F81 = QUANTITY_MAX;
		playSound(0, 3);
	} else if (isKeyDown(PADRdown)) {
		if (ITEM_MENU_MODE < ITEM_MENU_BUY_CARD) {
			if (ITEM_MENU_MODE == ITEM_MENU_SELL) {
				MONEY += MAIN_D_80134F81 * SELECTED_ITEM_PRICE;
				if (MONEY >= 1000000) {
					MONEY = 999999;
				}

				removeItem(SELECTED_ITEM, MAIN_D_80134F81);
				owner = readPStat(PSTAT_BUILTIN_ARG);
			} else {
				unitPrice = SELECTED_ITEM_PRICE;
				/* Babies get 10% off. */
				savings = isPartnerBaby();
				if (savings != 0) {
					unitPrice = unitPrice * 90 / 100;
					savings = SELECTED_ITEM_PRICE;
					savings -= unitPrice;
					SCRIPT_PRICE =
						MAIN_D_80134F81 * savings;
				}

				qty = MAIN_D_80134F81;
				MONEY -= qty * unitPrice;
				giveItem(SELECTED_ITEM, qty);

				if (ITEM_MENU_MODE == ITEM_MENU_BUY_LOST) {
					owner = getRecycleId(SELECTED_ITEM);
					SCRIPT_STATE_PTR->smth[owner + 6] -=
						MAIN_D_80134F81;
				}

				owner = 0xfd;
			}
			SELECTION_MENU_STATE = 7;
		} else {
			MONEY += MAIN_D_80134F81 * SELECTED_ITEM_PRICE;
			if (MONEY >= 1000000) {
				MONEY = 999999;
			}

			owner = getCardAmount(SELECTED_ITEM);
			owner = (uint32_t)owner - MAIN_D_80134F81;
			setCardAmount(SELECTED_ITEM, owner);
			owner = readPStat(PSTAT_BUILTIN_ARG);
			SELECTION_MENU_STATE = 4;
		}

		setupBoxOrigin(owner, &origin);
		triggerBoxCloseFlag(3);
		closeTextbox(3, &origin);
		MONEY_BOX_DIRTY = 1;
		MAIN_func_800FDFB4();
	}

	layoutQuantityBox();
}

/*
 * Lays out a row of the collector's trades: the item he wants, in red if the
 * player does not have it, and the item he gives for it.
 */
void layoutTradeRow(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	int32_t idx;
	int32_t row0;
	int32_t item;
	int32_t len;

	out = getItemMenuRowBuffer(box, row);
	*out++ = TEXT_COLOR;
	*out++ = TEXT_COLOR_YELLOW;

	row0 = row;
	row0 = row0;
	idx = ITEM_MENU_MODE + ((MAIN_D_80134F68->topRow + 5) + row);

	/* "－" for trades already done, "　" otherwise. */
	if (isTriggerSet(idx) != 0) {
		*out++ = 0x81;
		*out++ = 0x7c;
	} else {
		*out++ = 0x81;
		*out++ = 0x40;
	}

	idx = (box->topRow + row0) * 2;
	item = MAIN_D_80134F68->buf[idx];
	*out++ = TEXT_COLOR;

	if ((item & 0x80) != 0) {
		*out++ = TEXT_COLOR_WHITE;
	} else {
		*out++ = TEXT_COLOR_RED;
	}

	item = (uint8_t)(item & 0x7f);
	strcpy((char *)out, ITEM_PARA[item].name);
	len = strlen(ITEM_PARA[item].name);
	out += len;
	out = padWithSpaces(out, 8, len);
	*out++ = TEXT_HALF_SPACE;
	*out++ = 0;
	*out++ = TEXT_COLOR;
	*out++ = TEXT_COLOR_WHITE;

	item = MAIN_D_80134F6C->buf[idx];
	strcpy((char *)out, ITEM_PARA[item].name);
	out += strlen(ITEM_PARA[item].name);
	terminateString(out, isLast);
}

void tickItemShop(void)
{
	int32_t npcId;
	int32_t i;
	int32_t trig;
	int32_t found;

	npcId = readPStat(PSTAT_BUILTIN_ARG) & 0xff;

	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F68, 0x100, 6, 0xb2, 0x18,
		                      6, 0x5a);
		initializeItemMenuBox(&MAIN_D_80134F6C,
		                      INVENTORY.size << 1, 6, 0xd2, 0x18,
		                      6, 0x5a);
		SHOP_CHOICE = 0;

		found = 0;
		for (i = 0, trig = TRIGGER_ITEM_IN_SHOPS; i < 0x80; i++, trig++) {
			if (isTriggerSet(trig)) {
				found = 1;
				break;
			}
		}

		SHOP_MADE_A_DEAL = 0;
		if (found) {
			showShopkeeperTextbox(SHOP_TEXT_WELCOME, npcId, 0);
			SELECTION_MENU_STATE = 1;
			SCRIPT_STATE_4 = 3;
			SCRIPT_STATE_3 = 1;
		} else {
			showShopkeeperTextbox(SHOP_TEXT_NOTHING_TO_SELL, npcId, 0);
			SELECTION_MENU_STATE = 1;
			SCRIPT_STATE_4 = 2;
			SCRIPT_STATE_3 = 1;
		}
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&MAIN_D_80134F6C);
		destroyItemMenuBox(&MAIN_D_80134F68);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		openMoneyBox(1);
		showShopkeeperSelection(SHOP_TEXT_BUY_SELL_LEAVE, SPEAKER_PLAYER, 3, &SHOP_CHOICE);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 4;
		SCRIPT_STATE_3 = 2;
		break;
	case 4:
		setInputRepeatMask(0x5000);
		ITEM_MENU_MODE = ITEM_MENU_BUY;
		shopFillBuyItemList();
		openItemMenuBox();
		showShopkeeperTextbox(SHOP_TEXT_WHAT_TO_BUY, npcId, 0);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 5:
		setInputRepeatMask(0x5000);
		ITEM_MENU_MODE = ITEM_MENU_SELL;
		fillInventoryItemList();
		openItemMenuBox();
		showShopkeeperTextbox(SHOP_TEXT_WHAT_TO_SELL, npcId, 0);
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 3;
		break;
	case 6:
		triggerBoxCloseFlag(2);

		if (SHOP_MADE_A_DEAL) {
			showShopkeeperTextbox(SHOP_TEXT_COME_AGAIN, npcId, 0);
		} else {
			showShopkeeperTextbox(SHOP_TEXT_BOUGHT_NOTHING, npcId, 0);
		}

		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 2;
		SCRIPT_STATE_3 = 1;
		break;
	case 7:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);

		if (ITEM_MENU_MODE == ITEM_MENU_BUY && isPartnerBaby()) {
			showShopkeeperTextbox(SHOP_TEXT_BABY_DISCOUNT, npcId, 0);
		} else {
			showShopkeeperTextbox(SHOP_TEXT_THANKS, npcId, 0);
		}

		SCRIPT_STATE_4 = 3;
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_3 = 1;
		SHOP_MADE_A_DEAL = 1;
		break;
	case 8:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		showShopkeeperTextbox(SHOP_TEXT_ANYTHING_ELSE, npcId, 0);
		SELECTION_MENU_STATE = 9;
		break;
	case 9:
		SELECTION_MENU_STATE = 1;
		SCRIPT_STATE_4 = 3;
		SCRIPT_STATE_3 = 1;
		break;
	}
}

void initializeItemMenuBox(ItemMenuBox **box, int32_t bufSize, int32_t rows,
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
	showMapHeadTextbox(idx, owner, boxId, MAPHEAD_TEXT_CURRENT_SHOP);
}

void destroyItemMenuBox(ItemMenuBox **box)
{
	freeArray((uint32_t *)(*box)->buf);
	freeArray((uint32_t *)*box);
	*box = 0;
}

/* Opens the box with the money, or the merit points if showBits is 0. */
void openMoneyBox(int32_t showBits)
{
	RECT rect;
	RECT origin;

	MONEY_BOX_DIRTY = 1;
	MONEY_BOX_SHOWS_BITS = showBits;
	if (UI_BOX_DATA[2].state == 1) {
		return;
	}

	setupBoxOrigin(0xfd, &origin);
	setRECT(&rect, -0x98, -0x62, 0x52, 0x21);
	createTextbox(2, 0xe1, &rect, &origin, tickMoneyBox,
	              renderMoneyBox);
	registerTextbox(2, 8, 1, 0, 0);
	tickMoneyBox();
}

void showShopkeeperSelection(int32_t idx, int32_t owner, int32_t boxId,
                        int32_t *outSelection)
{
	showMapheadSelection(idx, owner, boxId, outSelection, MAPHEAD_TEXT_CURRENT_SHOP);
}

/* Opens box 1 with the list of ITEM_MENU_MODE. */
void openItemMenuBox(void)
{
	RECT rect;
	RECT origin;
	int32_t boxId;
	ItemMenuBox *result;
	int16_t *dims;

	if (ITEM_MENU_MODE == ITEM_MENU_SELL || ITEM_MENU_MODE == ITEM_MENU_PICK_ITEM) {
		boxId = 0xfd;
	} else {
		boxId = readPStat(PSTAT_BUILTIN_ARG) & 0xff;
	}

	setupBoxOrigin(boxId, &origin);
	result = getActiveItemMenu();
	dims = (int16_t *)((uint8_t *)ITEM_MENU_RECTS + ITEM_MENU_MODE * 8);
	setRECT(&rect, dims[0], dims[1], dims[2], dims[3]);
	createTextbox(1, 0xf1, &rect, &origin, tickItemMenu,
	              renderItemMenuBox);
	registerTextbox(1, 9, 6, 1, 0);
	openItemMenu(result, 1, 9);
	layoutItemMenu(result, 9, 0);
	SCRIPT_PRICE = 0;
}

void tickPickItemMenu(void)
{
	switch (SELECTION_MENU_STATE) {
	case 0:
		initializeItemMenuBox(&MAIN_D_80134F6C,
		                      INVENTORY.size << 1, 6, 0x9a, 0x18,
		                      6, 0x5a);

		if (fillInventoryItemList()) {
			SELECTION_MENU_STATE = 3;
			SCRIPT_STATE_3 = 0;
		} else {
			setTrigger(3);
			writePStat(PSTAT_BUILTIN_ARG, 0xff);
			SELECTION_MENU_STATE = 2;
			SCRIPT_STATE_3 = 0;
		}
		break;
	case 1:
		break;
	case 2:
		destroyItemMenuBox(&MAIN_D_80134F6C);
		ACTIVE_INSTRUCTION = 0;
		break;
	case 3:
		setInputRepeatMask(0x5000);
		ITEM_MENU_MODE = ITEM_MENU_PICK_ITEM;
		openItemMenuBox();
		SELECTION_MENU_STATE = 1;
		break;
	case 4:
		setInputRepeatMask(0);
		triggerBoxCloseFlag(1);
		SELECTION_MENU_STATE = 2;
		break;
	}
}

/* Returns the list that ITEM_MENU_MODE shows. */
ItemMenuBox *getActiveItemMenu(void)
{
	switch (ITEM_MENU_MODE) {
	case ITEM_MENU_BUY:
	case ITEM_MENU_BUY_LOST:
	case ITEM_MENU_BUY_CARD:
	case ITEM_MENU_SELL_CARD:
	case ITEM_MENU_MERIT_CARD:
		return MAIN_D_80134F68;
	case ITEM_MENU_SELL:
	case ITEM_MENU_PICK_ITEM:
	case ITEM_MENU_MERIT_ITEM:
		return MAIN_D_80134F6C;
	}

	return 0;
}

/* Starts a list at its first row, drawn from row startRow of the buffer. */
void openItemMenu(ItemMenuBox *box, int32_t boxId, int32_t startRow)
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

/* Lays out the visible rows of the menu with the layout*Row() of style. */
void layoutItemMenu(ItemMenuBox *box, int32_t startRow, int32_t style)
{
	TextBoxData *entry;
	uint8_t *p;
	int32_t rows;
	int32_t last;
	int32_t i;

	entry = &TEXT_BOX_TABLE.box[box->boxId];
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
					layoutItemRow(box, i, 1);
				} else if (style == 1) {
					layoutCardRow(box, i, 1);
				} else if (style == 2) {
					layoutBgmTrackRow(box, i, 1);
				} else if (style == 3) {
					layoutDestinationRow(box, i, 1);
				} else {
					layoutTradeRow(box, i, 1);
				}
			} else {
				if (style == 0) {
					layoutItemRow(box, i, 0);
				} else if (style == 1) {
					layoutCardRow(box, i, 0);
				} else if (style == 2) {
					layoutBgmTrackRow(box, i, 0);
				} else if (style == 3) {
					layoutDestinationRow(box, i, 0);
				} else {
					layoutTradeRow(box, i, 0);
				}
			}

			i++;
		}
	} else {
		/* Nothing to list. */
		p = TEXT_BUFFERS_PTR + (startRow << 6);
		if (entry->vramMode == 2) {
			p += TEXT_ROW_SIZE / 2;
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

int32_t tickItemMenuPageFlip(ItemMenuBox *box)
{
	if (box == 0) {
		return 1;
	}

	return flipMenuPage(box->boxId);
}

/*
 * Opens the box to choose how many of the selected item to buy or sell, up to
 * what the player can pay for, carry or has. Returns 0 if it cannot be picked.
 */
int32_t openQuantityBox(RECT *origin)
{
	ItemMenuBox *box;
	RECT rect;
	int32_t idx;
	int16_t boxY;
	uint8_t id;
	uint8_t amount;
	uint8_t room;

	box = getActiveItemMenu();
	idx = (box->topRow + box->cursor) * 2;
	SELECTED_ITEM = box->buf[idx];
	if (SELECTED_ITEM == 0xff) {
		goto fail;
	}

	id = SELECTED_ITEM;
	amount = box->buf[idx + 1];

	if (ITEM_MENU_MODE < ITEM_MENU_BUY_CARD) {
		SELECTED_ITEM_PRICE = ITEM_PARA[id].value;
		if (ITEM_MENU_MODE == ITEM_MENU_SELL) {
			if ((amount & 0x80) == 0) {
				goto fail;
			}

			SELECTED_ITEM_PRICE >>= 1;
			QUANTITY_MAX = amount & 0x7f;
		} else {
			if (ITEM_MENU_MODE == ITEM_MENU_BUY) {
				if (amount == 0) {
					goto fail;
				}
				QUANTITY_MAX = 0x63;
			} else {
				if ((amount & 0x80) == 0) {
					goto fail;
				}
				QUANTITY_MAX = amount & 0x7f;
			}

			room = 0x63 - getItemCount(id);
			if (room < QUANTITY_MAX) {
				QUANTITY_MAX = room;
			}

			idx = SELECTED_ITEM_PRICE;
			if (QUANTITY_MAX * idx > MONEY) {
				QUANTITY_MAX = MONEY / SELECTED_ITEM_PRICE;
			}
		}
	} else {
		SELECTED_ITEM_PRICE =
			CARD_PRICES[CARD_DATA[id].spriteId] >> 1;
		QUANTITY_MAX = amount;
	}

	MAIN_D_80134F81 = 1;
	MAIN_D_80134F82 = 1;
	reservePopupTextRow();
	boxY = UI_BOX_DATA[1].finalPos.y;
	origin->x += UI_BOX_DATA[1].finalPos.x;
	origin->y += (boxY + box->cursor * 18);
	setRECT(&rect, -0x41, -0x2a, 0x82, 0x53);
	createTextbox(3, 0xc1, &rect, origin, tickSellItemBox,
	              renderSellItemBox);
	registerTextbox(3, POPUP_TEXT_ROW, 1, 0, 0);
	layoutQuantityBox();
	playSound(0, 3);

	return 1;
fail:
	playSound(0, 0xb);

	return 0;
}

/* Asks to confirm the item picked in ITEM_MENU_PICK_ITEM. */
int32_t openItemConfirmBox(RECT *origin)
{
	ItemMenuBox *box;
	RECT rect;
	int32_t idx;
	int16_t boxY;
	uint8_t amount;

	box = getActiveItemMenu();
	idx = (box->topRow + box->cursor) * 2;
	SELECTED_ITEM = box->buf[idx];
	if (SELECTED_ITEM == 0xff) {
		goto fail;
	}

	amount = box->buf[idx + 1];
	if (ITEM_MENU_MODE == ITEM_MENU_PICK_ITEM) {
		if ((amount & 0x80) == 0) {
			goto fail;
		}
	} else {
		if (amount == 0) {
			goto fail;
		}
	}

	reservePopupTextRow();
	boxY = UI_BOX_DATA[1].finalPos.y;
	origin->x += UI_BOX_DATA[1].finalPos.x;
	origin->y += (boxY + box->cursor * 18);
	setRECT(&rect, -0x38, -0x15, 0x70, 0x2a);
	createTextbox(3, 0xc1, &rect, origin, tickItemConfirmBox,
	              renderItemConfirmBox);
	registerTextbox(3, POPUP_TEXT_ROW, 2, 0, 0);
	/* "Is it OK? / Yes No" */
	showMapHeadTextbox(0, SPEAKER_NONE, 3, MAPHEAD_TEXT_SYSTEM);
	MAIN_D_80134F81 = 0;
	playSound(0, 3);

	return 1;
fail:
	playSound(0, 0xb);

	return 0;
}

void itemMenuJumpToFirst(ItemMenuBox *box, int32_t startRow, int32_t style)
{
	itemMenuSelectFirst(box);
	layoutItemMenu(box, startRow, style);
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
			redrawItemMenuRow(box, style);
		} else {
			box->cursor -= 1;
		}
		playSound(0, 2);
	} else {
		playSound(0, 0xb);
	}
}

void itemMenuJumpToLast(ItemMenuBox *box, int32_t startRow, int32_t style)
{
	itemMenuSelectLast(box);
	layoutItemMenu(box, startRow, style);
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
			redrawItemMenuRow(box, style);
		}
		playSound(0, 2);
	} else {
		box->cursor -= 1;
		playSound(0, 0xb);
	}
}

/* Shows the description of the selected item in a box next to the row. */
int32_t openItemDescriptionBox(ItemMenuBox *box, RECT *origin, int32_t uiBoxId)
{
	RECT rect;
	uint8_t *out;
	uint8_t item;
	int16_t boxY;

	item = box->buf[(box->topRow + box->cursor) * 2];
	if (item == 0xff) {
		return 0;
	}

	reservePopupTextRow();
	boxY = UI_BOX_DATA[uiBoxId].finalPos.y;
	origin->x += UI_BOX_DATA[uiBoxId].finalPos.x;
	origin->y += (boxY + box->cursor * 18);
	setRECT(&rect, -0x84, -0xb, 0x108, 0x16);
	createTextbox(3, 0xc1, &rect, origin, tickItemDescriptionBox,
	              renderItemDescriptionBox);
	registerTextbox(3, POPUP_TEXT_ROW, 1, 0, 0);
	strcpy((out = TEXT_BUFFERS_PTR + (POPUP_TEXT_ROW << 6), out),
	       ITEM_DESC_PTR[item]);
	out += strlen(ITEM_DESC_PTR[item]);
	*out++ = 0;
	*out = 0;
	TEXT_BOX_DATA[3].pageReady = 1;
	TEXT_BOX_DATA[3].writeCount++;

	return 1;
}

void renderItemMenuHeader(int32_t boxId, int32_t idx, int16_t x, int16_t y)
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

static void renderItemMenuScrollbar__garbage__(void)
{
	int16_t a;
	int16_t b;

	a = MOOD_BUBBLE_TIMER;
	b = MAIN_D_80134F82;
	a = (a * 100) / (b + 0);
	a = (a * 100) / (b + 1);
	a = (a * 100) / (b + 2);
	a = (a * 100) / (b + 3);
	a = (a * 100) / (b + 4);
	MOOD_BUBBLE_TIMER = a;
}

/* Draws the scroll bar on the left of a list. */
void renderItemMenuScrollbar(ItemMenuBox *box)
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
	renderTrianglePrimitive(0x20202, x + w + 1, y, x, y, x, bottom + offset, boxId, 0);
	renderTrianglePrimitive(0xa08769, x, y + h + 1, x + w + 1, y + h + 1, x + w + 1, y + 1, boxId, 0);
	renderTrianglePrimitive(0xa08769, x + offset + w * scale, thumbY, x + 1, thumbY, x + 1, thumbY + track - 2, boxId, 0);
	renderTrianglePrimitive(0x20202, x + 1, thumbY + track - 1, x + offset + w * scale, thumbY + track - 1, x + offset + w * scale, thumbY + 1, boxId, 0);

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

/*
 * Draws the icons (mode 0: items, 1: cards, 2: none) and the text of the
 * visible rows.
 */
void renderItemMenuRows(ItemMenuBox *box, int16_t x1, int16_t y1, int16_t x2,
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
	tbox = &TEXT_BOX_TABLE.box[boxId];
	if (mode != 2) {
		if (tickItemMenuPageFlip(box) != 0) {
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
					renderCardIcon(spriteId, x2 + 2, yy2, order);
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

/*
 * Popups over a list use the rows of the back page of the dialogue box, which
 * is not flipping while the list is open.
 */
void reservePopupTextRow(void)
{
	uint8_t *box = (uint8_t *)TEXT_BOX_DATA;

	/* vramRow, then (backPage ^ 1) * vramRows; the fields do not match. */
	POPUP_TEXT_ROW = *(int32_t *)(box + 0x20);
	POPUP_TEXT_ROW = (*(int32_t *)(box + 0x18) ^ 1) * *(int32_t *)(box + 0x24);
}

void MAIN_func_800FDFB4(void)
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

	renderLinePrimitive(0x20202, x, yc, (x + w) - 1, yc, boxId, 0);
	y++;
	renderLinePrimitive(0xa08769, x, y, (x + w) - 1, y, boxId, 0);
	y++;
	renderLinePrimitive(0x20202, x, y, (x + w) - 1, y, boxId, 0);
}

static int32_t renderInsetFrame__garbage__(int32_t seed)
{
	int32_t t0 = 1;
	int32_t t1 = 2;
	int32_t t2 = 3;
	int32_t t3 = 4;
	int32_t t4 = 5;
	int32_t t5 = 6;
	int32_t t6 = 7;
	int32_t t7 = 8;
	int32_t t8 = 9;
	int32_t t9 = 10;
	int32_t t10 = 11;
	int32_t t11 = 12;
	int32_t t12 = 13;
	int32_t t13 = 14;
	int32_t t14 = 15;
	int32_t t15 = 16;
	int32_t t16 = 17;
	int32_t t17 = 18;
	int32_t t18 = 19;
	int32_t t19 = 20;
	int32_t t20 = 21;
	int32_t t21 = 22;
	int32_t t22 = 23;
	int32_t t23 = 24;
	int32_t t24 = 25;
	int32_t t25 = 26;
	int32_t t26 = 27;
	int32_t t27 = 28;
	int32_t t28 = 29;
	int32_t t29 = 30;
	int32_t t30 = 31;
	int32_t t31 = 32;
	int32_t t32 = 33;
	int32_t t33 = 34;
	int32_t t34 = 35;
	int32_t t35 = 36;
	int32_t t36 = 37;
	int32_t t37 = 38;
	int32_t t38 = 39;
	int32_t t39 = 40;

	return seed + t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8 + t9 + t10 + t11 + t12 + t13 + t14 + t15 + t16 + t17 + t18 + t19 + t20 + t21 + t22 + t23 + t24 + t25 + t26 + t27 + t28 + t29 + t30 + t31 + t32 + t33 + t34 + t35 + t36 + t37 + t38 + t39;
}

void renderInsetFrame(int32_t boxId, int16_t x, int16_t y, int32_t w, int16_t h)
{
	uint8_t order;

	x = x + UI_BOX_DATA[boxId].finalPos.x;
	y = y + UI_BOX_DATA[boxId].finalPos.y;
	order = 6 - boxId;
	renderTrianglePrimitive(0xa08769, x + 1, (y + h) - 1, (x + w) - 1, (y + h) - 1, (x + w) - 1, y, order, 0);
	renderTrianglePrimitive(0x20202, (x + w) - 1, y, x, y, x, (y + h) - 1, order, 0);
}

void renderCardIcon(int32_t spriteId, int16_t x, int16_t y, int32_t depth)
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

/*
 * Shows message idx of the MAPHEAD_TEXT_* table section in boxId, said by
 * owner, in the middle of whatever the script is doing.
 */
void showMapHeadTextbox(int32_t idx, int32_t owner, int32_t boxId,
                        int32_t section)
{
	uint8_t *savedCursor;

	if (boxId == 0 && owner != SPEAKER_NARRATOR) {
		setDialogueOwner(owner);
	}

	savedCursor = SCRIPT_PC;

	if (section == MAPHEAD_TEXT_CURRENT_SHOP) {
		SCRIPT_PC = getShopText(idx);
	} else {
		SCRIPT_PC = getMapHeadText(section, idx);
	}

	if (owner == SPEAKER_NARRATOR) {
		owner = SPEAKER_NONE;
	}

	showTextboxFinalPage(boxId, owner);
	ACTIVE_INSTRUCTION = SCRIPT_OP_BUILTIN;
	SCRIPT_PC = savedCursor;
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

	writePStat(PSTAT_TEXT_ARG_2, count);
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

	pick = random(j);
	pick = pool[pick];
	unlearnMove(pick);
	writePStat(PSTAT_TEXT_ARG_1, pick);
	freeArray((uint32_t *)pool);
	/* "<move>was not passed on." */
	showMapHeadTextbox(7, SPEAKER_NONE, 0, MAPHEAD_TEXT_SYSTEM);
	SELECTION_MENU_STATE = 1;
	SCRIPT_STATE_4 = 3;
	SCRIPT_STATE_3 = 1;

	return;
state3:
	pick = readPStat(PSTAT_TEXT_ARG_2);
	pick -= 1;
	if (pick == 0) {
		closeBox(0);
		ACTIVE_INSTRUCTION = 0;
		return;
	}

	writePStat(PSTAT_TEXT_ARG_2, pick);
	SELECTION_MENU_STATE = 2;
}

int32_t flipMenuPage(uint8_t a)
{
	TextBoxData *entry;

	entry = &TEXT_BOX_TABLE.box[a];

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

/*
 * Lays out a row of an item list: the name, in red if it cannot be picked,
 * then the price and the amount, depending on ITEM_MENU_MODE.
 */
void layoutItemRow(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	char *name;
	uint8_t type;
	uint8_t amount;
	int32_t value;
	int32_t sum;
	int32_t idx;
	int32_t i;

	out = getItemMenuRowBuffer(box, row);
	sum = box->topRow + row;
	i = sum * 2;
	idx = i;
	type = box->buf[idx];
	amount = box->buf[idx + 1];

	if (type != 0xff) {
		*out++ = TEXT_COLOR;
		if (ITEM_MENU_MODE == ITEM_MENU_BUY || ITEM_MENU_MODE == ITEM_MENU_MERIT_ITEM) {
			if (amount != 0) {
				*out++ = TEXT_COLOR_WHITE;
			} else {
				*out++ = TEXT_COLOR_RED;
			}
		} else {
			if ((amount & 0x80) != 0) {
				*out++ = TEXT_COLOR_WHITE;
			} else {
				*out++ = TEXT_COLOR_RED;
			}
		}

		strcpy(out, name = ITEM_PARA[type].name);
		out += strlen(name);
		*out++ = TEXT_HALF_SPACE;
		*out++ = 0;

		if (ITEM_MENU_MODE != ITEM_MENU_PICK_ITEM) {
			*out++ = TEXT_COLUMN_105_ALT;
			*out++ = 0;
			if (ITEM_MENU_MODE != ITEM_MENU_MERIT_ITEM) {
				value = ITEM_PARA[type].value;
				if (ITEM_MENU_MODE == ITEM_MENU_SELL) {
					if ((amount & 0x80) != 0) {
						value >>= 1;
					} else {
						/* Cannot be sold: "－－－－" */
						for (i = 0; i < 4; i++) {
							*out++ = 0x81;
							*out++ = 0x7c;
						}
						*out++ = TEXT_HALF_SPACE;
						*out++ = 0;
						goto amountPart;
					}
				}
			} else {
				value = ITEM_PARA[type].meritValue;
			}

			out = intToStringSJIS(out, value, 4, 0);
			*out++ = TEXT_HALF_SPACE;
			*out++ = 0;
		}
amountPart:
		if (ITEM_MENU_MODE == ITEM_MENU_SELL) {
			out = intToStringSJIS(out, amount & 0x7f, 2, 0);
		}
		if ((ITEM_MENU_MODE != ITEM_MENU_BUY) && (ITEM_MENU_MODE != ITEM_MENU_MERIT_ITEM) &&
		    (ITEM_MENU_MODE != ITEM_MENU_SELL)) {
			*out++ = TEXT_COLUMN_100;
			*out++ = 0;
			out = intToStringSJIS(out, amount & 0x7f, 2, 0);
		}
	}

	terminateString(out, isLast);
}

/* Lays out a row of a card list: the Digimon, the price and the amount. */
void layoutCardRow(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	char *name;
	uint8_t type;
	uint8_t amount;
	int32_t idx;
	int32_t value;

	out = getItemMenuRowBuffer(box, row);
	idx = (box->topRow + row) * 2;
	type = box->buf[idx];
	amount = box->buf[idx + 1];

	if (type != 0xff) {
		*out++ = TEXT_COLOR;
		if (ITEM_MENU_MODE == ITEM_MENU_BUY_CARD) {
			if (amount != 0) {
				/* Cards the player does not have yet. */
				if (getCardAmount(type) == 0) {
					*out++ = TEXT_COLOR_YELLOW;
				} else {
					*out++ = TEXT_COLOR_WHITE;
				}
			} else {
				*out++ = TEXT_COLOR_RED;
			}
		} else {
			*out++ = TEXT_COLOR_WHITE;
		}

		strcpy(out, name = DIGIMON_DATA[CARD_DATA[type].digimonId].name);
		out += strlen(name);
		*out++ = TEXT_COLUMN_105;
		*out++ = 0;
		*out++ = TEXT_HALF_SPACE;
		*out++ = 0;
		value = CARD_PRICES[CARD_DATA[type].spriteId];

		if (ITEM_MENU_MODE == ITEM_MENU_BUY_CARD) {
			out = intToStringSJIS(out, value, 4, 0);
		} else {
			if (ITEM_MENU_MODE != ITEM_MENU_MERIT_CARD) {
				out = intToStringSJIS(out, value >> 1, 4, 0);
				*out++ = TEXT_HALF_SPACE;
				*out++ = 0;
			}
			out = intToStringSJIS(out, amount, 2, 0);
		}
	}

	terminateString(out, isLast);
}

/* Lays out a row of the jukebox: the number and name of the track. */
void layoutBgmTrackRow(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	uint8_t type;
	int32_t len;

	out = getItemMenuRowBuffer(box, row);
	type = box->buf[(box->topRow + row) * 2];
	*out++ = TEXT_COLOR;

	/* The track that is playing. */
	if (type == readPStat(PSTAT_SELECTED)) {
		*out++ = TEXT_COLOR_LIGHT_BLUE;
	} else {
		*out++ = TEXT_COLOR_WHITE;
	}

	out = intToStringSJIS(out, type + 1, 2, 0);
	*out++ = TEXT_HALF_SPACE;
	*out++ = 0;
	strcpy(out, BGM_TRACK_NAMES[type]);
	len = strlen(BGM_TRACK_NAMES[type]);
	out += len;
	out = padWithSpaces(out, 0xc, len);
	terminateString(out, isLast);
}

/* Lays out a row of the transport menu: the map and what the trip costs. */
void layoutDestinationRow(ItemMenuBox *box, uint8_t row, int32_t isLast)
{
	uint8_t *out;
	uint8_t raw;
	uint8_t nameId;
	int32_t len;

	out = getItemMenuRowBuffer(box, row);
	raw = box->buf[(box->topRow + row) * 2];
	*out++ = TEXT_COLOR;

	if ((raw & 0x80) != 0) {
		*out++ = TEXT_COLOR_WHITE;
	} else {
		*out++ = TEXT_COLOR_RED;
	}

	raw &= 0x7f;
	nameId = MAP_ENTRIES[TRANSPORT_DESTINATIONS[raw].mapId].loadingName;
	strcpy(out, MAP_NAME_PTR[nameId]);
	len = strlen(MAP_NAME_PTR[nameId]);
	out += len;
	out = padWithSpaces(out, 0xc, len);
	*out++ = TEXT_HALF_SPACE;
	*out++ = 0;
	*out++ = TEXT_COLUMN_160;
	*out++ = 0;
	out = intToStringSJIS(out, TRANSPORT_DESTINATIONS[raw].cost, 4, 0);
	terminateString(out, isLast);
}

/* Draws the row that scrolled in straight into VRAM, without flipping. */
void redrawItemMenuRow(ItemMenuBox *box, int32_t style)
{
	TextBoxData *entry;
	uint8_t *p;
	int16_t row;
	int32_t outX;
	int32_t outClut;

	/* Lay out into the front page. */
	entry = &TEXT_BOX_TABLE.box[box->boxId];
	entry->backPage ^= 1;

	row = box->cursor;

	if (style == 0) {
		layoutItemRow(box, row, 1);
	} else if (style == 1) {
		layoutCardRow(box, row, 1);
	} else if (style == 2) {
		layoutBgmTrackRow(box, row, 1);
	} else if (style == 3) {
		layoutDestinationRow(box, row, 1);
	} else {
		layoutTradeRow(box, row, 1);
	}

	entry->backPage ^= 1;

	row = box->itemRow[row];
	if (entry->doubleBuffered == 1) {
		row = row + (int16_t)((int32_t)entry->backPage * (int32_t)entry->vramRows);
	}

	getVRAMModeCoords(entry->vramMode, &outX, &outClut);

	p = TEXT_BUFFERS_PTR + (row << 6);
	if (outX != 0) {
		p += 0x20;
	}

	drawTextRow(p, outX, row * 12, 1);
}

void renderRaisedFrame(uint8_t boxId, int16_t x, int16_t y, int32_t w, int16_t h)
{
	x = x + UI_BOX_DATA[boxId].finalPos.x;
	y = y + UI_BOX_DATA[boxId].finalPos.y;
	boxId = 6 - boxId;
	renderTrianglePrimitive(0xa08769, (x + w) - 1, y, x, y, x, (y + h) - 1, boxId, 0);
	renderTrianglePrimitive(0x20202, x, (y + h) - 1, (x + w) - 1, (y + h) - 1, (x + w) - 1, y, boxId, 0);
}

void itemMenuSelectLast(ItemMenuBox *data)
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

void itemMenuSelectFirst(ItemMenuBox *data)
{
	data->prevTopRow = data->topRow;
	data->prevCursor = data->cursor;
	data->topRow = 0;
	data->cursor = 0;
	playSound(0, 2);
}

/* Returns the row of the text buffer that holds row index of the menu. */
uint8_t *getItemMenuRowBuffer(ItemMenuBox *box, int32_t index)
{
	TextBoxData *entry;
	uint8_t *row;

	entry = &TEXT_BOX_TABLE.box[box->boxId];
	row = TEXT_BUFFERS_PTR + box->itemRow[index] * TEXT_ROW_SIZE;

	if (entry->vramMode == 2) {
		row += TEXT_ROW_SIZE / 2;
	}

	if (entry->doubleBuffered == 1) {
		row = (uint8_t *)(row + (((entry->backPage ^ 1) * entry->vramRows) << 6));
	}

	return row;
}

/* Resets the color and ends the text, or only the row if flag is 0. */
void terminateString(uint8_t *str, int32_t flag)
{
	*str++ = TEXT_COLOR;
	*str++ = TEXT_COLOR_WHITE;

	if (flag != 0) {
		*str++ = TEXT_END;
		*str = 0;
	} else {
		*str++ = TEXT_NEWLINE;
		*str = 0;
	}
}

/* Pads text that is used bytes long to width full-width characters. */
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

	renderLinePrimitive(0x20202, xc, y, xc, (y + h) - 1, order, 0);
	x++;
	renderLinePrimitive(0xa08769, x, y, x, (y + h) - 1, order, 0);
	x++;
	renderLinePrimitive(0x20202, x, y, x, (y + h) - 1, order, 0);
}

/* Changes who is talking, reopening the box next to them. */
void setDialogueOwner(int32_t owner)
{
	if (DIALOGUE_SPEAKER != owner) {
		DIALOGUE_SPEAKER = owner;
		beginScriptEvent(owner);
		setupDialogueBox(DIALOGUE_SPEAKER);
	}
}

void showMapheadSelection(int32_t idx, int32_t owner, int32_t x,
                          int32_t *outSel, uint16_t section)
{
	uint8_t *saved;
	uint16_t rows;

	if (owner != SPEAKER_NARRATOR) {
		setDialogueOwner(owner);
	} else {
		DIALOGUE_SPEAKER = SPEAKER_NONE;
		owner = SPEAKER_PLAYER;
	}

	saved = SCRIPT_PC;

	if (section == MAPHEAD_TEXT_CURRENT_SHOP) {
		SCRIPT_PC = getShopText(idx);
	} else {
		SCRIPT_PC = getMapHeadText(section, idx);
	}

	CHOICE_COUNT[0] = x;

	if (*outSel == 0) {
		*outSel = 1;
		CHOICE_CURSOR[0] = 0;
	}

	CHOICE_CURSOR_WIDTH[0] = showTextboxFinalPage(0, DIALOGUE_SPEAKER);
	CHOICE_CURSOR_WIDTH[0] = CHOICE_CURSOR_WIDTH[0] * TEXT_GLYPH_WIDTH + 2;

	/* Skip the row with the name of the speaker. */
	if (owner != SPEAKER_NONE) {
		CHOICE_CURSOR_Y[0] = 0xd;
	} else {
		CHOICE_CURSOR_Y[0] = 0;
	}

	ACTIVE_INSTRUCTION = SCRIPT_OP_BUILTIN;
	SCRIPT_PC = saved;
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

	if (ACTIVE_INSTRUCTION == SCRIPT_OP_CHOICE) {
		renderDialogueChoiceCursor(x, ySave);
	} else if (ACTIVE_INSTRUCTION == SCRIPT_OP_BUILTIN && SCRIPT_STATE_3 == 2) {
		renderDialogueChoiceCursor(x, ySave);
	}

	if (TEXTBOX_SHOW_NEXT_ARROW != 0) {
		renderTextboxNextArrow(0);
	}
}

void setDigimonRaised(int32_t digimonId)
{
	if ((uint32_t)digimonId < 0x3f) {
		setTrigger(digimonId + TRIGGER_DIGIMON_RAISED);
	}
}

int32_t hasDigimonRaised(int32_t digimonId)
{
	return isTriggerSet(digimonId + TRIGGER_DIGIMON_RAISED);
}

void unlockMedal(uint16_t medal)
{
	if (medal < 0x13) {
		setTrigger(medal + TRIGGER_MEDAL);
	}
}

int32_t hasMedal(uint16_t medal)
{
	return isTriggerSet(medal + TRIGGER_MEDAL);
}

void createMonochromonMoodBubble(void)
{
	int32_t a;
	int32_t b;

	a = readPStat(0xf7) & 0xff;
	b = readPStat(0xf8) & 0xff;

	if ((uint32_t)b < 5) {
		if (scriptIdToEntityId(a) != 0xff) {
			MOOD_BUBBLE_TIMER = 0x1e;
			MOOD_BUBBLE_SPRITE.u = b * 32;
			addObject(0x1b1, 0, 0, renderMonochromonMoodBubble);
		}
	} else {
		removeObject(0x1b1, 0);
	}
}

void MAIN_func_800FF900(void)
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

/*
 * Draws the rows laid out by showTextbox() into VRAM, one row per frame for a
 * full-width box and two for a half-width one. Returns 0 if there was nothing
 * left to draw.
 */
int32_t drawTextboxRows(int32_t boxId, int32_t flag)
{
	TextBoxData *box;
	uint32_t x;
	int32_t clut;
	int16_t px;
	int16_t row;
	uint8_t *buf;
	int32_t done;

	box = &TEXT_BOX_TABLE.box[boxId];
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
		done = drawTextRow(buf, px, row, flag);
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

/* Gives the rows of the text buffer used by the box back. */
void releaseTextboxRows(int32_t boxId)
{
	TextBoxData *entry;
	int32_t rows;

	entry = &TEXT_BOX_TABLE.box[boxId];

	rows = entry->vramRows;
	if (entry->doubleBuffered == 1) {
		rows <<= 1;
	}

	entry->vramRows = 0;
	entry->registered = 1;
	TEXT_BOX_TABLE.usedRows -= rows;
}

/*
 * Moves the cursor of a choice and returns the option picked, or 0xffff while
 * the player has not picked one. Triangle picks the last option, or leaves the
 * choice if it is part of the script (CHOICE_CANCEL_PC).
 */
int32_t tickDialogueChoice(void)
{
	if (tickTextboxPageFlip(0) != 0) {
		return 0xffff;
	}

	if (isXPressedAfterDialogue() == 0) {
		return 0xffff;
	}

	if (isKeyDown(PADRdown)) {
		advanceTextbox(0);
		playSound(0, 3);

		return CHOICE_CURSOR[0];
	}

	if (isKeyDown(PADRup)) {
		if (isTriggerSet(TRIGGER_CHOICE_CANT_CANCEL) == 0) {
			if (ACTIVE_INSTRUCTION != SCRIPT_OP_BUILTIN) {
				advanceTextbox(0);
				SCRIPT_PC =
					(uint8_t *)CHOICE_CANCEL_PC[0];
				playSound(0, 4);

				return 0xffff;
			}

			advanceTextbox(0);
			CHOICE_CURSOR[0] = CHOICE_COUNT[0] - 1;
			playSound(0, 4);

			return CHOICE_CURSOR[0];
		}

		playSound(0, 0xb);

		return 0xffff;
	}

	if (isKeyDown(PADLup)) {
		if (CHOICE_CURSOR[0] == 0) {
			CHOICE_CURSOR[0] = CHOICE_COUNT[0] - 1;
		} else {
			CHOICE_CURSOR[0] -= 1;
		}

		playSound(0, 2);

		return 0xffff;
	}

	if (isKeyDown(PADLdown)) {
		CHOICE_CURSOR[0] += 1;
		if (CHOICE_CURSOR[0] == CHOICE_COUNT[0]) {
			CHOICE_CURSOR[0] = 0;
		}

		playSound(0, 2);
	}
	return 0xffff;
}

/*
 * Waits for the player to read the page, depending on TEXT_ADVANCE_MODE: 0
 * waits for cross, 1 goes on at once and 2 after TEXT_AUTO_ADVANCE_TIMER.
 * Returns 1 when the page is done.
 */
int32_t tickDialogueAdvance(void)
{
	if (tickTextboxPageFlip(0) != 0) {
		return 0;
	}

	if (TEXT_ADVANCE_MODE == 0) {
		TEXTBOX_SHOW_NEXT_ARROW = 1;
	}

	if (isXPressedAfterDialogue() == 0) {
		return 0;
	}

	switch (TEXT_ADVANCE_MODE) {
	case 0:
		if (isKeyDown(PADRdown) == 0) {
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
		if (TEXT_AUTO_ADVANCE_TIMER != 0) {
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

/* Draws the cursor over the selected option of a choice. */
void renderDialogueChoiceCursor(int32_t x, int32_t y)
{
	if (TEXT_BOX_DATA[0].idle == 1) {
		return;
	}

	renderSelectionCursor(x - 1,
	                      y + CHOICE_CURSOR_Y[0] + CHOICE_CURSOR[0] * 13 - 2,
	                      CHOICE_CURSOR_WIDTH[0], 0xd, 6);
}

void tickSizedTextbox(void)
{
	if (tickTextboxPageFlip(0) != 0) {
		return;
	}

	if (!isXPressedAfterDialogue()) {
		return;
	}

	if (isKeyDown(PADRdown)) {
		if (UI_BOX_DATA[0].state != 1) {
			return;
		}

		if (advanceTextbox(0) != 0) {
			triggerBoxCloseFlag(0);
		}

		playSound(0, 3);

		return;
	}

	if (!isKeyDown(PADRup)) {
		return;
	}

	/* Triangle skips the rest of the message. */
	if (UI_BOX_DATA[0].state == 1) {
		while (TEXT_BOX_DATA[0].pageReady == 0) {
			showTextbox(0, SPEAKER_NONE);
		}

		ACTIVE_INSTRUCTION = 0;
		TEXT_BOX_DATA[0].idle = 1;
		triggerBoxCloseFlag(0);
		playSound(0, 4);
	}
}

void renderSizedTextbox(void)
{
	TextBoxData *box;
	int16_t rowPx;
	int16_t pagePx;
	int16_t x;
	int16_t y;
	int32_t i;

	box = TEXT_BOX_TABLE.box;
	rowPx = box->vramRow * 12;
	pagePx = box->vramRows * 12;
	rowPx = rowPx + (int16_t)(box->backPage * pagePx);
	x = UI_BOX_DATA[0].finalPos.x + 5;
	y = UI_BOX_DATA[0].finalPos.y + 4;

	for (i = 0; i < box->vramRows; i++, rowPx += 0xc, y += 0xd) {
		renderString(0, x, y, 0xfc, 0xc, 0, rowPx, 6, 1);
	}
}

/* Frees every textbox and the rows of the text buffer. */
void resetTextboxes(void)
{
	TextBoxData *box;
	int32_t i;

	for (i = 0, box = TEXT_BOX_DATA; i < 6; i++, box++) {
		box->flags = 0;
		box->vramRows = 0;
		box->registered = 1;
	}

	TEXT_BOX_TABLE.usedRows = 0;
	SCRIPT_WAIT_TIMER = 0;
	TEXT_AUTO_ADVANCE_TIMER = 0;
	TEXTBOX_OPEN_TIMER = 0;
}

/*
 * Runs once per frame from tickScript(): clears the back page of the boxes
 * with a new page, draws their text and opens or closes their UI boxes.
 */
void tickTextboxes(int32_t flag)
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
					drew = drawTextboxRows(i & 0xff, flag);
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
		if (SCRIPT_WAIT_TIMER != 0) {
			SCRIPT_WAIT_TIMER--;
		}
		if (TEXT_AUTO_ADVANCE_TIMER != 0) {
			TEXT_AUTO_ADVANCE_TIMER--;
		}
		TEXTBOX_SHOW_NEXT_ARROW = 0;
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

/*
 * Mode 0 uses the whole width of the text area, 1 its left half and 2 its
 * right half. outClut is really the width in pixels.
 */
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
void closeTextbox(int32_t boxId, RECT *target)
{
	uint32_t b;

	b = boxId;

	if (UI_BOX_DATA[boxId].state != 0 && UI_BOX_DATA[boxId].state != 3) {
		if ((TEXT_BOX_DATA[b].flags & 0x40) == 0) {
			removeStaticUIBox(boxId);
		} else {
			removeAnimatedUIBox(boxId, target);
		}

		releaseTextboxRows(boxId);
	}
}

/*
 * Closes every open textbox. If one closes with an animation (flag 0x40), the
 * script ends once the animation is over, see tickTextboxes().
 */
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

void createTextbox(int32_t boxId, int32_t flags, RECT *rect, RECT *origin,
                   void *tick, void *render)
{
	TextBoxData *entry;

	entry = &TEXT_BOX_TABLE.box[boxId];

	if ((entry->flags & 0xf) == 1) {
		closeTextbox(boxId, 0);
	}

	entry->flags = flags;
	entry->rect = *rect;
	entry->origin = *origin;
	entry->tick = tick;
	entry->render = render;

	waitForCrossRelease();
}

void triggerBoxCloseFlag(int32_t boxId)
{
	TextBoxData *e = &TEXT_BOX_TABLE.box[boxId];
	uint8_t val = e->flags;
	uint32_t v;

	if ((val & 0xf) != 0) {
		v = val & 0xf0;
		v &= 0xff;
		e->flags = v;
	}
}

/*
 * Reserves rows of the text buffer for the box, twice as many if it is double
 * buffered, and clears their area of VRAM. mode picks which part of the text
 * area the rows use, see getVRAMModeCoords().
 */
void registerTextbox(int32_t boxId, int32_t row, int32_t rows,
                     int32_t doubleBuffer, int32_t mode)
{
	TextBoxData *entry;
	int32_t usedRows;
	RECT rect;
	int32_t vramX;
	int32_t vramW;

	entry = &TEXT_BOX_TABLE.box[boxId];
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

	TEXT_BOX_TABLE.usedRows += usedRows;
	setTextColor(1);
	getVRAMModeCoords(entry->vramMode, &vramX, &vramW);
	setRECT(&rect, vramX, entry->vramRow * 12, vramW, usedRows * 12);
	clearTextSubArea(&rect);
}

/*
 * Draws one row of laid out text into the text area of VRAM. Returns 1 at the
 * end of the message and 0 at the end of the row.
 */
int32_t drawTextRow(uint8_t *str, int16_t x, int16_t y, int32_t flag)
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
		case TEXT_END:
			return 1;
		case TEXT_SKIP_3:
			str += 3;
			break;
		case TEXT_SKIP_1:
			str++;
			break;
		case TEXT_COLOR:
			ch = *str++;
			setTextColor(ch);
			break;
		case TEXT_TAB_8:
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
		case TEXT_COLUMN_105_ALT:
			str++;
			setRECT(&rect, x + pos - 6, y, 105, 0xc);
			clearTextSubArea(&rect);
			pos = 105;
			break;
		case TEXT_COLUMN_105:
			str++;
			setRECT(&rect, x + pos, y, 105, 0xc);
			clearTextSubArea(&rect);
			pos = 105;
			break;
		case TEXT_COLUMN_160:
			str++;
			setRECT(&rect, x + pos, y, 160, 0xc);
			clearTextSubArea(&rect);
			pos = 160;
			break;
		case TEXT_COLUMN_100:
			str++;
			setRECT(&rect, x + pos, y, 100, 0xc);
			clearTextSubArea(&rect);
			pos = 100;
			break;
		case TEXT_COLUMN_96:
			str++;
			setRECT(&rect, x + pos, y, 96, 0xc);
			clearTextSubArea(&rect);
			pos = 96;
			break;
		case TEXT_COLUMN_156:
			str++;
			setRECT(&rect, x + pos, y, 156, 0xc);
			clearTextSubArea(&rect);
			pos = 156;
			break;
		case TEXT_COLUMN_180:
			str++;
			setRECT(&rect, x + pos, y, 180, 0xc);
			clearTextSubArea(&rect);
			pos = 180;
			break;
		case TEXT_TAB_11:
			str++;
			rem = pos / 12 % 11;
			save = pos;
			if (rem != 0) {
				rem = (11 - rem) * 12;
				setRECT(&rect, x + save, y, rem, 0xc);
				clearTextSubArea(&rect);
				pos = save + rem;
			}
			break;
		case TEXT_HALF_SPACE:
			str++;
			setRECT(&rect, x + pos, y, 6, 0xc);
			clearTextSubArea(&rect);
			pos += 6;
			break;
		case TEXT_NEWLINE:
			return 0;
		default:
			/* Half-width ASCII is drawn with the full-width glyphs. */
			if (isAsciiEncoded((char *)&ch) != 0) {
				glyph = swapShortBytes(convertAsciiToJis(ch));
			} else {
				glyph = ch + (*str++ << 8);
			}

			/* "　", read little-endian. */
			if (glyph == 0x4081) {
				setRECT(&rect, x + pos, y, 0xc, 0xc);
				clearTextSubArea(&rect);
			} else {
				y2 = y;
				adv = drawGlyph(glyph, x + pos, y2);
				if (TEXT_MONOSPACE != 0) {
					adv = 0xc;
				}
			}

			pos += adv;
			break;
		}
	}
}

/*
 * Shows the back page of a double buffered box once it has been drawn, and
 * lets the script lay out the page after it. Returns 1 while the box is busy
 * and the player cannot advance yet.
 */
int32_t tickTextboxPageFlip(int32_t boxId)
{
	TextBoxData *entry;

	entry = &TEXT_BOX_TABLE.box[boxId];

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

/* Returns 1 once the last page of the dialogue has been shown. */
int32_t isDialogueFinished(void)
{
	TextBoxData *entry;

	entry = TEXT_BOX_DATA;

	if (tickTextboxPageFlip(0) != 0) {
		return 0;
	}

	if (entry->renderCount == entry->flipCount) {
		entry->idle = 1;
		entry->pageReady = 0;
		return 1;
	}

	return 0;
}
/*
 * Goes to the next page of the box. Returns 1 if the page shown was the last
 * one (pageReady marks the last page of a message).
 */
int32_t advanceTextbox(int32_t boxId)
{
	TextBoxData *entry;

	entry = &TEXT_BOX_TABLE.box[boxId];
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

/* Opens the dialogue box, growing out of owner if it is on screen. */
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

/*
 * Gets the point on screen that the box of ownerId grows out of. Returns 0 if
 * the box has no owner on screen.
 */
int32_t setupBoxOrigin(int32_t ownerId, RECT *origin)
{
	int16_t pos[2];

	if (ownerId == SPEAKER_NONE) {
		return 0;
	}

	if (TEXTBOX_ORIGIN_X != -0x270f) {
		worldPosToScreenPos2(&TEXTBOX_ORIGIN_X, &TEXTBOX_ORIGIN_Y,
		                     &TEXTBOX_ORIGIN_Z);
		pos[0] = TEXTBOX_ORIGIN_X - 5;
		pos[1] = TEXTBOX_ORIGIN_Y - 5;
		TEXTBOX_ORIGIN_X = -0x270f;
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

/* Waits for the player to read or pick an option in the dialogue box. */
void tickScriptDialogueBox(void)
{
	int32_t sel;

	if (ACTIVE_INSTRUCTION == SCRIPT_OP_CHOICE) {
		sel = tickDialogueChoice();
		if (sel != 0xffff) {
			SCRIPT_PC =
				(uint8_t *)((uint32_t)CHOICE_TARGETS[0] +
			                    CHOICE_CURSOR[0] * 2);
			SCRIPT_PC =
				(uint8_t *)((uint32_t)CURRENT_SCRIPT_PTR +
			                    *(uint16_t *)SCRIPT_PC);
		}
	} else if (ACTIVE_INSTRUCTION == SCRIPT_OP_TEXT) {
		tickDialogueAdvance();
	} else if (ACTIVE_INSTRUCTION == SCRIPT_OP_BUILTIN) {
		switch (SCRIPT_STATE_3) {
		case 0:
			break;
		case 1:
			if (tickDialogueAdvance() == 1) {
				SCRIPT_STATE_3 = 0;
				SELECTION_MENU_STATE = SCRIPT_STATE_4;
				ACTIVE_INSTRUCTION = SCRIPT_OP_BUILTIN;
			}
			break;
		case 2:
			sel = tickDialogueChoice();
			if (sel != 0xffff) {
				SCRIPT_STATE_3 = 0;
				sel = CHOICE_CURSOR[0];
				SELECTION_MENU_STATE =
					SCRIPT_STATE_4 + (uint16_t)sel;
				ACTIVE_INSTRUCTION = SCRIPT_OP_BUILTIN;
			}
			break;
		case 3:
			if (isDialogueFinished() == 1) {
				ACTIVE_INSTRUCTION = SCRIPT_OP_BUILTIN;
				SCRIPT_STATE_3 = 0;
			}
			break;
		}
	}
}

/* Draws the blinking arrow in the corner of a box that waits for cross. */
void renderTextboxNextArrow(int32_t boxId)
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

/*
 * Script instruction 0x10: a choice. It is followed by the number of options,
 * the offsets to jump to for each option, the text with one option per row and
 * the code that runs if the player cancels.
 */
void scriptShowSelection(void)
{
	uint8_t optionCount;
	uint16_t height;

	pollNextScriptUByte(&optionCount);

	CHOICE_COUNT[0] = optionCount;
	CHOICE_CURSOR[0] = 0;
	CHOICE_TARGETS[0] = (int32_t)SCRIPT_PC;
	SCRIPT_PC += (optionCount + 1) * 2;
	CHOICE_CURSOR_WIDTH[0] = showTextbox(0, DIALOGUE_SPEAKER);
	CHOICE_CURSOR_WIDTH[0] = CHOICE_CURSOR_WIDTH[0] * TEXT_GLYPH_WIDTH + 2;
	height = CHOICE_CURSOR_WIDTH[0];

	if (height > 0xf0) {
		CHOICE_CURSOR_WIDTH[0] = 0xf0;
	}

	SCRIPT_PC += 2;
	CHOICE_CANCEL_PC[0] = (int32_t)SCRIPT_PC;

	if (DIALOGUE_SPEAKER != SPEAKER_NONE) {
		CHOICE_CURSOR_Y[0] = 0xd;
	} else {
		CHOICE_CURSOR_Y[0] = 0;
	}

	ACTIVE_INSTRUCTION = SCRIPT_OP_CHOICE;
}

static void showTextbox__garbage__(void)
{
	uint8_t moveSlot;
	uint8_t objectId;
	uint8_t speed;
	uint8_t targetId;
	uint8_t pad;
	int16_t posX;
	int16_t posY;
	ScriptCameraMovement *slot;

	beginScriptEvent(SPEAKER_NONE);
	pollNextScriptUByte(&moveSlot);
	pollNextTwoScriptBytes(&objectId, &speed);
	pollNextTwoScriptBytes(&targetId, &pad);
	pollNextTwoScriptShorts(&posX, &posY);
	moveSlot += 0xc;
	slot = &SCRIPT_MOVEMENTS[moveSlot];
	slot->type = 0xd;
	slot->entityId = objectId;
	slot->speed = speed;
	slot->target = targetId;
	slot->targetX = posX;
	slot->targetY = posY;
}

/*
 * Lays out the next page of the message at SCRIPT_PC into the back page of
 * the textbox, with the name of the speaker on the first row, and returns the
 * width of the longest row in glyphs. The page ends at the end of the message
 * or at an empty row.
 */
uint32_t showTextbox(int32_t boxId, uint32_t speakerId)
{
	TextBoxData *entry;
	uint8_t *base;
	uint8_t *out;
	uint32_t col;
	uint16_t maxCol;
	uint32_t row;
	uint32_t rowOffset;
	int32_t len;
	uint8_t ctrl;

	entry = &TEXT_BOX_TABLE.box[boxId];
	base = TEXT_BUFFERS_PTR + (entry->vramRow << 6);
	if (entry->vramMode == 2) {
		base += TEXT_ROW_SIZE / 2;
	}
	if (entry->doubleBuffered == 1) {
		base = (uint8_t *)(base + (((entry->backPage ^ 1) * entry->vramRows) << 6));
	}
	out = base;
	col = maxCol = 0;
	row = 0;
	if (speakerId != SPEAKER_NONE) {
		*out++ = TEXT_COLOR;
		if (speakerId == SPEAKER_PLAYER) {
			*out++ = TEXT_COLOR_LIGHT_BLUE;
		} else if (speakerId == SPEAKER_PARTNER) {
			*out++ = TEXT_COLOR_ORANGE;
		} else if (speakerId >= SPEAKER_SPECIAL) {
			*out++ = TEXT_COLOR_GREEN;
		} else {
			*out++ = TEXT_COLOR_YELLOW;
		}
		out += getSpeakerName(speakerId, out);
		*out++ = TEXT_COLOR;
		*out++ = TEXT_COLOR_WHITE;
		*out++ = TEXT_NEWLINE;
		*out = TEXT_END;
		row++;
		out = base + (row << 6);
	}
	rowOffset = row << 6;
top: {
	ctrl = *SCRIPT_PC++;
	{
		switch (ctrl) {
		case TEXT_SKIP_3:
			*out++ = ctrl;
			*out++ = *SCRIPT_PC++;
			*out++ = *SCRIPT_PC++;
			*out++ = *SCRIPT_PC++;
			goto top;
		case TEXT_PSTAT_NUMBER:
			ctrl = *SCRIPT_PC++;
			ctrl = readPStat(ctrl);
			out = intToStringSJIS(out, ctrl, 3, 1);
			goto top;
		case TEXT_PLAYER_NAME:
			SCRIPT_PC++;
			len = getSpeakerName(SPEAKER_PLAYER, out);
			out += len;
			col = (col + ((len >> 1) &
			              0xffff)) &
			      0xffff;
			goto top;
		case TEXT_PARTNER_NAME:
			SCRIPT_PC++;
			len = getSpeakerName(SPEAKER_PARTNER, out);
			out += len;
			col = (col + ((len >> 1) &
			              0xffff)) &
			      0xffff;
			goto top;
		case TEXT_DIGIMON_NAME:
			ctrl = *SCRIPT_PC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out,
				       (const char *)&DIGIMON_DATA[ctrl]);
				len = strlen(
					(const char *)&DIGIMON_DATA[ctrl]);
				out += len;
				col = (col + ((len >> 1) &
				              0xffff)) &
				      0xffff;
			}
			goto top;
		case TEXT_MOVE_NAME:
			ctrl = *SCRIPT_PC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out, MOVE_NAMES[ctrl]);
				len = strlen(MOVE_NAMES[ctrl]);
				out += len;
				col = (col + ((len >> 1) &
				              0xffff)) &
				      0xffff;
			}
			goto top;
		case TEXT_ITEM_NAME:
			ctrl = *SCRIPT_PC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out,
				       (const char *)&ITEM_PARA[ctrl]);
				len = strlen(
					(const char *)&ITEM_PARA[ctrl]);
				out += len;
				col = (col + ((len >> 1) &
				              0xffff)) &
				      0xffff;
			}
			goto top;
		case TEXT_MONEY:
			SCRIPT_PC++;
			out = intToStringSJIS(out, MONEY, 6, 0);
			goto top;
		case TEXT_MERIT:
			SCRIPT_PC++;
			out = intToStringSJIS(out, MERIT, 4, 0);
			goto top;
		case TEXT_PRICE:
			SCRIPT_PC++;
			out = intToStringSJIS(out, SCRIPT_PRICE, 5, 1);
			goto top;
		case TEXT_TOURNAMENT_TITLES:
			SCRIPT_PC++;
			out = intToStringSJIS(out, TOURNAMENT_TITLES, 3, 1);
			goto top;
		case TEXT_TOURNAMENT_WINS:
			SCRIPT_PC++;
			out = intToStringSJIS(out, TOURNAMENT_WINS, 3, 1);
			goto top;
		case TEXT_TOURNAMENT_LOSSES:
			SCRIPT_PC++;
			out = intToStringSJIS(out, TOURNAMENT_LOSSES, 3, 1);
			goto top;
		case TEXT_BGM_NAME:
			ctrl = *SCRIPT_PC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out,
				       BGM_TRACK_NAMES[ctrl]);
				len = strlen(BGM_TRACK_NAMES[ctrl]);
				out += len;
				col = (col + ((len >> 1) & 0xffff)) & 0xffff;
			}
			goto top;
		case TEXT_CUP_NAME:
			ctrl = *SCRIPT_PC++;
			{
				ctrl = readPStat(ctrl);
				strcpy(out,
				       TOURNAMENT_CUP_NAMES[ctrl]);
				len = strlen(TOURNAMENT_CUP_NAMES[ctrl]);
				out += len;
				col = (col + ((len >> 1) & 0xffff)) & 0xffff;
			}
			goto top;
		case TEXT_TAB_8:
			*out++ = ctrl;
			*out++ = *SCRIPT_PC++;
			goto top;
		case TEXT_TAB_11:
			*out++ = ctrl;
			*out++ = *SCRIPT_PC++;
			goto top;
		case TEXT_NEWLINE:
			*out++ = ctrl;
			*out++ = *SCRIPT_PC++;
			if (maxCol < col) {
				maxCol = col;
			}
			col = 0;
			ctrl = *SCRIPT_PC;
			if (ctrl == TEXT_END) {
				/* End of the message: drop the newline. */
				SCRIPT_PC += 2;
				out -= 2;
				*out = ctrl;
				/* Another text instruction continues the dialogue. */
				ctrl = *SCRIPT_PC;
				if (ctrl == SCRIPT_OP_TEXT) {
					goto done;
				}
				entry->pageReady = 1;
				goto done;
			}
			rowOffset += TEXT_ROW_SIZE;
			row++;
			len = rowOffset;
			out = base + len;
			goto top;
		case TEXT_COLOR:
		case TEXT_SKIP_1:
			*out++ = ctrl;
			*out++ = *SCRIPT_PC++;
			goto top;
		case TEXT_HALF_SPACE:
		default:
			/* A Shift JIS character. */
			*out++ = ctrl;
			*out++ = *SCRIPT_PC++;
			col = (col + 1) & 0xffff;
			goto top;
		}
	}
}
done:
	entry->writeCount++;
	ACTIVE_INSTRUCTION = SCRIPT_OP_TEXT;

	return maxCol;
}

/*
 * Script instruction 0x26: opens a centered box that is cols glyphs wide and
 * rows lines high, for text without a speaker.
 */
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
	createTextbox(0, boxFlags, &rect1, &origin, tickSizedTextbox,
	              renderSizedTextbox);
	registerTextbox(0, 0, rows, 1, 0);
}

/* Lays out text that is not followed by more text, as MAPHEAD text is. */
int32_t showTextboxFinalPage(int32_t boxId, int32_t speakerId)
{
	TEXT_BOX_DATA[boxId].pageReady = 1;

	return showTextbox(boxId, speakerId);
}

/* Copies the name of the speaker into buf and returns its length. */
int32_t getSpeakerName(int32_t speakerId, uint8_t *buf)
{
	if (speakerId == SPEAKER_NONE) {
		return 0;
	}

	/* The name of the player is kept as the name of Digimon 0. */
	if (speakerId == SPEAKER_PLAYER) {
		speakerId = 0;
		goto digimon;
	}

	if (speakerId == SPEAKER_PARTNER) {
		strcpy((char *)buf, PARTNER_ENTITY.name);

		return strlen(PARTNER_ENTITY.name);
	}

	if ((uint32_t)speakerId < SPEAKER_SPECIAL) {
		speakerId = scriptIdToEntityId(speakerId) & 0xff;
		speakerId = ENTITY_TABLE[speakerId]->type & 0xff;
		goto digimon;
	}

	speakerId = (speakerId - SPEAKER_SPECIAL) & 0xff;
	strcpy((char *)buf, SPECIAL_SPEAKER_NAMES[speakerId]);

	return strlen(SPECIAL_SPEAKER_NAMES[speakerId]);

digimon:
	strcpy((char *)buf, DIGIMON_DATA[speakerId].name);

	return strlen(DIGIMON_DATA[speakerId].name);
}

/*
 * Writes value with full-width digits. Leading zeros become spaces, or are
 * left out if flag is set.
 */
uint8_t *intToStringSJIS(uint8_t *buf, int32_t value, uint8_t digits, int32_t flag)
{
	Pow10Table divs;
	uint16_t base;
	int32_t started;
	uint16_t c;
	int32_t hi;
	int32_t lo;

	divs = POWERS_OF_TEN;
	base = SJIS_DIGIT_ZERO;
	started = 0;
	while (digits != 0) {
		c = value / divs.v[digits - 1];
		c = base + c;
		value = value % divs.v[digits - 1];
		if (digits != 1) {
			if (c == SJIS_DIGIT_ZERO) {
				if (started == 0) {
					if (flag != 0) {
						goto skip;
					}
					c = SJIS_SPACE;
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

	if (scriptId == SPEAKER_PLAYER) {
		return 0;
	}

	if (scriptId == SPEAKER_PARTNER) {
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

	SCRIPT_PC = (uint8_t *)(SCRIPT_PC + 1);
	v1 = readPStat(statIdx);
	v2 = readPStat((uint8_t)(statIdx + 1));
	v3 = readPStat((uint8_t)(statIdx + 2));
	now = dateToMinutes(v1, v2, v3, readPStat((uint8_t)(statIdx + 3)));

	if (scriptCompareValues(op, now, dateToMinutes(years, days, hours, minutes)) != 0) {
		setTrigger(trigger);
	} else {
		unsetTrigger(trigger);
	}
}

int32_t scriptTestStat(void)
{
	uint16_t u;
	int16_t s;
	uint8_t b1;
	uint8_t b2;
	uint16_t stat;
	int16_t stat2;

	pollNextTwoScriptBytes(&b1, &b2);

	/* Happiness is signed. */
	if (b1 != SCRIPT_STAT_HAPPINESS) {
		stat = *getStatsPointer(b1);
		pollNextScriptUShort(&u);

		return scriptCompareValues(b2, stat, u);
	}

	stat2 = *getStatsPointer(b1);
	pollNextScriptShort(&s);

	return scriptCompareSigned(b2, stat2, s);
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
	case SCRIPT_STAT_HP:
		return &PARTNER_ENTITY.digimonEntity.stats.current.currentHP;
	case SCRIPT_STAT_MP:
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
	case SCRIPT_STAT_BATTLES_FOUGHT:
		return &BATTLES_FOUGHT;
	case SCRIPT_STAT_BATTLES_WON:
		return &BATTLES_WON;
	case SCRIPT_STAT_TOURNAMENT_TITLES:
		return &TOURNAMENT_TITLES;
	case SCRIPT_STAT_TOURNAMENT_WINS:
		return &TOURNAMENT_WINS;
	case SCRIPT_STAT_TOURNAMENT_LOSSES:
		return &TOURNAMENT_LOSSES;
	case SCRIPT_STAT_WEIGHT:
		return &PARTNER_PARA.weight;
	case SCRIPT_STAT_TAMER_LEVEL:
		STAT_TAMER_LEVEL_VALUE = TAMER_ENTITY.tamerLevel;
		return &STAT_TAMER_LEVEL_VALUE;
	case SCRIPT_STAT_LIVES:
		STAT_LIVES_VALUE = PARTNER_ENTITY.lives;
		return &STAT_LIVES_VALUE;
	}
}

void *allocateArray(uint32_t size)
{
	uint32_t oldTop;

	oldTop = ARRAY_STACK_TOP;
	size = ((size + 3) >> 2) << 2;
	*(uint32_t *)(MAIN_D_801345B0 + oldTop) = size;
	ARRAY_STACK_TOP += (size + 4);

	return MAIN_D_801345B0 + (oldTop + 4);
}

void freeArray(uint32_t *array)
{
	ARRAY_STACK_TOP -= array[-1] + 4;
}

void renderMoneyBox(void)
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

int32_t scriptTestCardAmount(void)
{
	uint16_t value;
	uint8_t cardId;
	uint8_t op;

	pollNextTwoScriptBytes(&cardId, &op);
	pollNextScriptUShort(&value);

	return scriptCompareValues(op, (uint8_t)getCardAmount(cardId), value);
}

int32_t scriptTestHasMove(void)
{
	uint8_t moveId;
	uint8_t negate;
	int32_t res;

	pollNextTwoScriptBytes(&moveId, &negate);
	res = scriptHasMove(moveId);
	if (negate == 0) {
		return res;
	}

	return (res != 0) ^ 1;
}

int32_t scriptTestPartnerCondition(void)
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

int32_t scriptTestItemCount(void)
{
	uint16_t value;
	uint8_t itemId;
	uint8_t op;

	pollNextTwoScriptBytes(&itemId, &op);
	pollNextScriptUShort(&value);

	return scriptCompareValues(op, (uint8_t)getItemCount(itemId), value);
}

void readFileSection(char *filename, void *dest, uint32_t offset,
                     uint32_t size)
{
	CdlFILE file;
	char path[64];
	uint8_t mode;

	mode = 0x80;

	if (SCRIPT_FILE_POSITION == 0) {
		path[0] = '\\';
		strcpy(&path[1], filename);
		strcat(path, FILE_VERSION_SUFFIX);
		if (CdSearchFile(&file, path) == 0) {
			return;
		}

		while (CdControl(0xe, &mode, 0) == 0)
			;

		SCRIPT_FILE_POSITION = CdPosToInt(&file.pos);
	} else {
		while (CdControl(0xe, &mode, 0) == 0)
			;
	}

	file.pos = *CdIntToPos(SCRIPT_FILE_POSITION + (offset >> 11), &file.pos);

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
		if (SCRIPT_MOVEMENTS[i].type != 0xff) {
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

	SCRIPT_WARPING = 1;

	resetBGM();
	loadMap(CURRENT_MAP_ID);
	longjmp(SCRIPT_JMP_BUF, 1);
}

int32_t enforceStatsLimits(int32_t stat, int32_t value)
{
	int32_t cap;

	if (stat == SCRIPT_STAT_HP) {
		cap = PARTNER_ENTITY.digimonEntity.stats.base.hp;
	} else if (stat == SCRIPT_STAT_MP) {
		cap = PARTNER_ENTITY.digimonEntity.stats.base.mp;
	} else {
		cap = STAT_LIMITS[stat];
	}

	if (cap < value) {
		return cap;
	}

	return value;
}

int32_t scriptTestMoney(void)
{
	int32_t value;
	uint8_t op;

	SCRIPT_PC = (uint8_t *)(SCRIPT_PC + 1);

	pollNextScriptUByte(&op);
	pollNextInt(&value);

	return scriptCompareValues(op, MONEY, value);
}
