#include <stdio.h>
#include <string.h>

#include <kernel.h>
#include <libgs.h>
#include <libmcrd.h>

#include <dw/entity.h>
#include <dw/font.h>
#include <dw/input.h>
#include <dw/item.h>
#include <dw/main.h>
#include <dw/map.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/sjis.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/utils.h>
#include <dw/version.h>
#include <dw/vs.h>
#include <dw/world_object.h>
#include <text/main/main_menu.h>

#include "common.h"

#if VERSION_IS(EU)
#define YES_TEXT_W 0x18
#define NO_TEXT_U 0x18
#define NO_TEXT_W 0x20
#elif !VERSION_IS(US)
#define YES_TEXT_W 0x18
#define NO_TEXT_U 0xc
#define NO_TEXT_W 0x24
#else
#define YES_TEXT_W 0x1c
#define NO_TEXT_U 0x1c
#define NO_TEXT_W 0x1c
#endif

#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
#define VS_RETURN_MENU 0x47
#else
#define VS_RETURN_MENU 0
#endif

#if VERSION_IS(JP_TRIAL)
#define SECRET_CODE_ITEM 0x7d
#elif VERSION_IS(JP_BOMBOM)
#define SECRET_CODE_ITEM 0x7e
#endif

/* MemCardSync command and result type */
#if !VERSION_IS(US)
typedef long MemCardSyncWord;
#else
typedef unsigned long MemCardSyncWord;
#endif

typedef struct {
	int8_t pos;
	int8_t count;
	int8_t defaultPos;
	int8_t scroll;
	int8_t max;
} MenuCursor;

typedef struct {
	int8_t pos;
	int8_t pad[5];
	int16_t rowHeight;
	int16_t x;
	int16_t y;
	int16_t w;
	int16_t h;
} MenuHighlight;

typedef struct {
	int32_t valid;
	char playerName[14];
	uint8_t pad12[6];
	char digimonName[18];
	uint8_t pad2A[2];
	char location[24];
} SaveSlotPreview;

typedef struct {
	int32_t mainState;
	VECTOR tamerPosition;
	VECTOR partnerPosition;
	PoopPile worldPoop[100];
	int32_t money;
	int32_t partnerType;
	uint8_t partnerData[16];
	uint16_t currentFrame;
	uint16_t lastHandledFrame;
	int16_t year;
	int16_t day;
	int16_t hour;
	int16_t minute;
	uint16_t playtimeFrames;
	uint16_t playtimeHours;
	uint16_t playtimeMinutes;
	uint8_t pad1DE[2];
	PartnerPara partnerPara;
	int16_t merit;
	int16_t battlesStarted;
	int16_t battlesFled;
	int16_t tournamentsWon;
	int16_t tournamentWins;
	int16_t tournamentsLost;
	Stats stats;
	uint8_t scriptState[0x17C];
	Inventory inventory;
	char playerName[20];
	char digimonName[20];
	int8_t partnerLives;
	int8_t tamerLevel;
	uint8_t raisedCount;
	int8_t npcMaps[8];
	uint8_t currentScreen;
	uint8_t previousScreen;
	uint8_t currentExit;
	uint8_t previousExit;
	int8_t tamerWaypointX[30];
	int8_t tamerWaypointY[30];
	int8_t tamerPreviousTileX;
	int8_t tamerPreviousTileY;
	int8_t tamerWaypointCurrent;
	int8_t tamerWaypointCount;
	int8_t tamerStartTileX;
	int8_t tamerStartTileY;
	int8_t tamerWaypointActive;
	uint8_t unknown4E1[0x1B];
	int32_t checksum;
	RegisteredDigimon battleRegistrationData[40];
} SavegamePayload;

typedef struct {
	char id[2];
	uint8_t iconDisplayFlag;
	uint8_t blockNumber;
	char title[64];
	uint8_t reserved[12];
	uint8_t pocketStation[16];
	uint16_t iconClut[16];
	uint8_t iconFrames[3][0x80];
	SavegamePayload saves[2];
} SaveFile;

uint32_t CONNECTED_CARDS;
int32_t MAIN_MENU_ACTION;
int32_t SAVE_SLOT_SCROLL_TENTHS;
int32_t MEMORY_CARD_SLOT;
int32_t MEMORY_CARD_ERROR;
int32_t MEMORY_CARD_ID;
int32_t MAIN_MENU_TICKS;
int32_t CURRENT_MENU;
int32_t NEW_CARDS;
#if !VERSION_IS(EU)
int32_t MAIN_D_80135054;
#endif
int32_t MEMORY_CARD_OPERATION;
int32_t VS_PLAYER_INDEX;
int32_t BATTLE_REGISTRATION_SLOT;
int32_t CHECKED_MEMORY_CARD;
int32_t TARGET_MENU;
int32_t MEMORY_CARD_RETURN_MENU;
int32_t MEMORY_CARD_USED_BLOCKS;
long SAVE_RETRY_RETURN_MENU;

static void *main_menu_sbss_order[] = {
#if VERSION_IS(EU)
	&MEMORY_CARD_USED_BLOCKS,
	&MEMORY_CARD_RETURN_MENU,
	&SAVE_RETRY_RETURN_MENU,
	&TARGET_MENU,
	&CHECKED_MEMORY_CARD,
	&MAIN_MENU_TICKS,
	&CURRENT_MENU,
	&NEW_CARDS,
	&MEMORY_CARD_OPERATION,
	&VS_PLAYER_INDEX,
	&BATTLE_REGISTRATION_SLOT,
#else
	&SAVE_RETRY_RETURN_MENU,
	&MEMORY_CARD_USED_BLOCKS,
	&MEMORY_CARD_RETURN_MENU,
	&TARGET_MENU,
	&CHECKED_MEMORY_CARD,
	&BATTLE_REGISTRATION_SLOT,
	&VS_PLAYER_INDEX,
	&MEMORY_CARD_OPERATION,
	&MAIN_D_80135054,
	&NEW_CARDS,
	&CURRENT_MENU,
	&MAIN_MENU_TICKS,
#endif
	&MEMORY_CARD_ID,
	&MEMORY_CARD_ERROR,
	&MEMORY_CARD_SLOT,
	&SAVE_SLOT_SCROLL_TENTHS,
	&MAIN_MENU_ACTION,
	&CONNECTED_CARDS,
};

SaveSlotPreview SAVEGAME_SLOT_INFO[15];

extern int8_t MAIN_STATE;
extern uint8_t CURRENT_SCREEN;
extern uint8_t PREVIOUS_SCREEN;
extern uint8_t CURRENT_EXIT;
extern uint8_t PREVIOUS_EXIT;
extern int8_t TAMER_WAYPOINT_X[];
extern int8_t TAMER_WAYPOINT_Y[];
extern int8_t TAMER_PREVIOUS_TILE_X;
extern int8_t TAMER_PREVIOUS_TILE_Y;
extern int8_t TAMER_WAYPOINT_CURRENT;
extern int8_t TAMER_WAYPOINT_COUNT;
extern int8_t TAMER_START_TILE_X;
extern int8_t TAMER_START_TILE_Y;
extern int8_t TAMER_WAYPOINT_ACTIVE;
extern int32_t MONEY;
extern uint16_t CURRENT_FRAME;
extern uint16_t LAST_HANDLED_FRAME;
extern uint8_t YEAR;
extern int16_t DAY;
#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
extern int16_t SECRET_CODE;
#endif
extern int16_t HOUR;
extern int16_t MINUTE;
extern uint16_t PLAYTIME_FRAMES;
extern uint16_t PLAYTIME_HOURS;
extern uint16_t PLAYTIME_MINUTES;
extern int32_t CHANGED_INPUT;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern char *MOVE_NAMES[];
#if !VERSION_IS(US)
extern char *MAP_NAME_PTR[];
#endif

int8_t getFileCityTopMap(void);
void renderUIBoxBorder(RECT *rect, int32_t flag);
void SetPolyG4(POLY_G4 *prim);
void recalculatePPandArena(void);
void renderMainMenuBackground(void);

void renderInitialMenu();
void renderText(POLY_FT4 *prim, int32_t x, int32_t y, int32_t u, int32_t v,
		int32_t w, int32_t h, int32_t textColor);
void renderMenuBox(int32_t x, int32_t y, int32_t w, int32_t h);
void renderSelectNewGameCard();
void renderSelectCardSlot();
void renderCheckingCard(void);
void renderConfirmNoCard();
void renderSaveSlotBox(int32_t slot, int32_t x, int32_t y);
void renderSelectSlot();
void renderConfirmSlotSelection();
void renderFormatMemoryCard(void);
void renderMemoryCardReadError();
void renderSleepMenu(void);
void renderConfirmOverwrite();
void renderConfirmVSSlot();
void renderRegisterDigimon();
void renderSelectRegisterSlot(void);
void renderConfirmRegister();
void renderCantRegister(void);
void renderDoYouWantToSave(void);
void renderCantRegisterBaby(void);
#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
void renderBattleModeMenu(void);
void renderInsertGameCardPrompt(void);
#endif
void drawMainMenuStrings();
void drawSaveSlotText(int32_t slot, int32_t row);
char *formatInteger(int32_t value, char *buf, int32_t digits);
void drawMoveName(long type, int32_t anim, int32_t color, int32_t pos);
void drawRegisteredDigimonSlots(int32_t slot);
void updateMemoryCardState();
void tickMainMenu(void);
int32_t tickMenuInput(MenuCursor *cursor, int32_t which);
int32_t getMenuOnCardChange(int32_t menu);
void setMemoryCardReadError(int32_t id, int32_t returnMenu);
int32_t isMemcardUnformatted(int32_t chan, MemCardSyncWord mode);
int32_t getCardUsedBlockCount(int32_t channel, int32_t returnMenu);
int32_t loadSaveSlotData(int32_t channel, char *filename, SaveSlotPreview *slots, int32_t unused);
int32_t tickSelectSlotInput(MenuCursor *cursor, int32_t which);
void initializeDefaultSavegame(void);
char *_strncpy(char *dst, char *src, int32_t n);
int32_t createSavegameChecksum(int32_t slot);
void loadSavegame(SavegamePayload *savegame);
void writeSavegame(SavegamePayload *savegame);
int32_t countRegisteredDigimon(RegisteredDigimon *p);
void renderMainMenu();
void registerBattleData();
void renderMenuSelector(MenuHighlight *b);
int32_t createByteSum(uint8_t *data, int32_t len);
#if !VERSION_IS(US)
void appendWideDigit(int32_t digit, char **dst, int32_t *started);
#endif
void openSaveMachine(void);
int32_t tickSaveMachine(void);
void awardMachinedramonData();
void gameClearSave();
int32_t scriptTickMainMenu(void);

void *main_menu_order_anchor[] = {
	scriptTickMainMenu,
	gameClearSave,
	awardMachinedramonData,
	tickSaveMachine,
	openSaveMachine,
#if !VERSION_IS(US)
	appendWideDigit,
#endif
	createByteSum,
	renderMenuSelector,
	registerBattleData,
#if !VERSION_IS(EU)
	renderMainMenu,
#endif
	countRegisteredDigimon,
	writeSavegame,
	loadSavegame,
	createSavegameChecksum,
	_strncpy,
	initializeDefaultSavegame,
	tickSelectSlotInput,
	loadSaveSlotData,
	getCardUsedBlockCount,
	isMemcardUnformatted,
	setMemoryCardReadError,
	getMenuOnCardChange,
	tickMenuInput,
#if VERSION_IS(EU)
	renderMainMenu,
#endif
	tickMainMenu,
	updateMemoryCardState,
	drawRegisteredDigimonSlots,
	drawMoveName,
	formatInteger,
	drawSaveSlotText,
	drawMainMenuStrings,
#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
	renderInsertGameCardPrompt,
	renderBattleModeMenu,
#endif
	renderCantRegisterBaby,
	renderDoYouWantToSave,
	renderCantRegister,
	renderConfirmRegister,
	renderSelectRegisterSlot,
	renderRegisterDigimon,
	renderConfirmVSSlot,
	renderConfirmOverwrite,
	renderSleepMenu,
	renderMemoryCardReadError,
	renderFormatMemoryCard,
	renderConfirmSlotSelection,
	renderSelectSlot,
	renderSaveSlotBox,
	renderConfirmNoCard,
	renderCheckingCard,
	renderSelectCardSlot,
	renderSelectNewGameCard,
	renderMenuBox,
	renderText,
	renderInitialMenu,
};

#if VERSION_IS(EU)
extern char MAIN_D_80131008[];
extern char MAIN_D_80131014[];
extern char MAIN_D_80131024[];
extern char MAIN_D_80131030[];
extern char MAIN_D_8013103C[];
extern char MAIN_D_8013106C[];
extern char MAIN_D_8013107C[];
extern char MAIN_D_80131090[];
extern char MAIN_D_801310A4[];
extern char MAIN_D_801310B4[];
extern char MAIN_D_801310CC[];
extern char MAIN_D_801310E0[];
extern char MAIN_D_801310F4[];
extern char MAIN_D_80131108[];
extern char MAIN_D_8013111C[];
extern char MAIN_D_80131134[];
extern char STR_PLEASE_REPLACE_THE_MEMORY_CARD[];
extern char MAIN_D_80131144[];
extern char MAIN_D_80131150[];
extern char STR_A_MEMORY_CARD[];
extern char MAIN_D_80131168[];
extern char STR_SLOT_1_IS_UNFORMATTED[];
extern char MAIN_D_80131178[];
extern char STR_IN_MEMORY_CARD_SLOT_1[];
extern char STR_SLOT_1_HAS_INSUFFICIENT_SPACE[];
extern char MAIN_D_8013119C[];
extern char MAIN_D_801311B4[];
extern char MAIN_D_801311C0[];
extern char STR_SLOT_2_IS_UNFORMATTED[];
extern char STR_IN_MEMORY_CARD_SLOT_2[];
extern char STR_SLOT_2_HAS_INSUFFICIENT_SPACE[];
extern char MAIN_D_801311D4[];
extern char MAIN_D_801311EC[];
extern char MAIN_D_80131200[];
extern char MAIN_D_80131218[];
extern char MAIN_D_80131228[];
extern char MAIN_D_80131254[];
extern char MAIN_D_80131278[];
extern char MAIN_D_80131290[];
extern char MAIN_D_801312A0[];
extern char MAIN_D_801312B8[];
extern char MAIN_D_801312D0[];
extern char MAIN_D_801312E4[];
extern char MAIN_D_80139DF4[];
extern char STR_CARD_IS_INSERTED_DURING_PLAY[];
extern char STR_TURNING_OFF_OR_RESETTING_THE[];
extern char STR_UNIT_WILL_ERASE_CONTENTS_OF[];
extern char STR_ADVENTURE[];
extern char MAIN_D_80139E14[];
extern char STR_IN_USE[];
extern char MAIN_D_801312FC[];
extern char STR_CARD_SLOT_1_NOT[];
extern char STR_CARD_SLOT_2_NOT[];
extern char MAIN_D_80131318[];
extern char MAIN_D_80131328[];
extern char MAIN_D_80131340[];
extern char MAIN_D_8013136C[];
extern char MAIN_D_80131378[];
extern char MAIN_D_80131390[];
extern char STR_OVERWRITTEN[];
extern char MAIN_D_801313B0[];
extern char MAIN_D_801313E4[];
extern char MAIN_D_80131400[];
extern char MAIN_D_8013141C[];
extern char STR_PLAYER_2_IN_MEMORY[];
extern char STR_CARD_SLOT_2[];
extern char STR_PLAYER_1_IN_MEMORY[];
extern char STR_CARD_SLOT_1[];
extern char MAIN_D_80134684[];
extern char MAIN_D_80131460[];
extern char MAIN_D_8013147C[];
extern char STR_SAVE_TO_BLOCK_QUESTION[];
extern char MAIN_D_80139FCC[];
extern char MAIN_D_801314B0[];
extern char MAIN_D_801314C8[];
extern char MAIN_D_801314E4[];
extern char STR_PROCESS[];
extern char MAIN_D_80131500[];
extern char MAIN_D_8013151C[];
extern char MAIN_D_80131538[];
extern char MAIN_D_8013154C[];
extern char MAIN_D_80131568[];
extern char STR_YES_NO_PADDED[];
extern char MAIN_D_80131580[];
extern char MAIN_D_801315A0[];
extern char STR_YOU_CANNOT_SAVE_IT[];
extern char STR_IN_MEMORY_CARD_FOR[];
extern char STR_MAIN_MENU_BATTLE[];
extern char MAIN_D_801315C0[];
extern char MAIN_D_801315EC[];
extern char STR_FOR_PLAYER_1_IN_MEMORY[];
extern char MAIN_D_8013A0EC[];
extern char STR_EMPTY[];
extern char STR_NUMBER_0[];
extern char STR_NUMBER_1[];
extern char MAIN_D_80134674[];
extern char STR_NUMBER_3[];
extern char STR_NUMBER_4[];
extern char STR_NUMBER_5[];
extern char STR_NUMBER_6[];
extern char STR_NUMBER_7[];
extern char STR_NUMBER_8[];
extern char STR_NUMBER_9[];
extern char STR_NUMBER_10[];
extern char STR_NUMBER_11[];
extern char STR_NUMBER_12[];
extern char STR_NUMBER_13[];
extern char STR_NUMBER_14[];
extern char STR_NUMBER_15[];
extern char STR_SPACE[];
extern char STR_SPACE_QUESTION[];
extern char MAIN_D_8013D814[];
extern char MAIN_D_80134694[];
extern char MAIN_D_80134698[];
extern char MAIN_D_8013469C[];
extern char MAIN_D_801346A4[];
extern char MAIN_D_8013143C[];
extern char MAIN_D_80131448[];
extern char MAIN_D_80131454[];
extern char MAIN_D_801346B4[];
extern char MAIN_D_801346B8[];
extern char PATH_WILDCARD[];

typedef struct {
	char *strings[16];
} NumberStrings;

extern NumberStrings NUMBERS;

static void *main_menu_data_order[] = {
	PATH_WILDCARD,
	MAIN_D_801346B8,
	MAIN_D_801346B4,
	MAIN_D_80131454,
	MAIN_D_80131448,
	MAIN_D_8013143C,
	MAIN_D_801346A4,
	MAIN_D_8013469C,
	MAIN_D_80134698,
	MAIN_D_80134694,
	MAIN_D_8013D814,
	STR_SPACE_QUESTION,
	STR_SPACE,
	STR_NUMBER_15,
	STR_NUMBER_14,
	STR_NUMBER_13,
	STR_NUMBER_12,
	STR_NUMBER_11,
	STR_NUMBER_10,
	STR_NUMBER_9,
	STR_NUMBER_8,
	STR_NUMBER_7,
	STR_NUMBER_6,
	STR_NUMBER_5,
	STR_NUMBER_4,
	STR_NUMBER_3,
	MAIN_D_80134674,
	STR_NUMBER_1,
	STR_NUMBER_0,
	STR_EMPTY,
	MAIN_D_8013A0EC,
	STR_FOR_PLAYER_1_IN_MEMORY,
	MAIN_D_801315EC,
	MAIN_D_801315C0,
	STR_MAIN_MENU_BATTLE,
	STR_IN_MEMORY_CARD_FOR,
	STR_YOU_CANNOT_SAVE_IT,
	MAIN_D_801315A0,
	MAIN_D_80131580,
	STR_YES_NO_PADDED,
	MAIN_D_80131568,
	MAIN_D_8013154C,
	MAIN_D_80131538,
	MAIN_D_8013151C,
	MAIN_D_80131500,
	STR_PROCESS,
	MAIN_D_801314E4,
	MAIN_D_801314C8,
	MAIN_D_801314B0,
	MAIN_D_80139FCC,
	STR_SAVE_TO_BLOCK_QUESTION,
	MAIN_D_8013147C,
	MAIN_D_80131460,
	MAIN_D_80134684,
	STR_CARD_SLOT_1,
	STR_PLAYER_1_IN_MEMORY,
	STR_CARD_SLOT_2,
	STR_PLAYER_2_IN_MEMORY,
	MAIN_D_8013141C,
	MAIN_D_80131400,
	MAIN_D_801313E4,
	MAIN_D_801313B0,
	STR_OVERWRITTEN,
	MAIN_D_80131390,
	MAIN_D_80131378,
	MAIN_D_8013136C,
	MAIN_D_80131340,
	MAIN_D_80131328,
	MAIN_D_80131318,
	STR_CARD_SLOT_2_NOT,
	STR_CARD_SLOT_1_NOT,
	MAIN_D_801312FC,
	STR_IN_USE,
	MAIN_D_80139E14,
	STR_ADVENTURE,
	STR_UNIT_WILL_ERASE_CONTENTS_OF,
	STR_TURNING_OFF_OR_RESETTING_THE,
	STR_CARD_IS_INSERTED_DURING_PLAY,
	MAIN_D_80139DF4,
	MAIN_D_801312E4,
	MAIN_D_801312D0,
	MAIN_D_801312B8,
	MAIN_D_801312A0,
	MAIN_D_80131290,
	MAIN_D_80131278,
	&NUMBERS,
	MAIN_D_80131254,
	MAIN_D_80131228,
	MAIN_D_80131218,
	MAIN_D_80131200,
	MAIN_D_801311EC,
	MAIN_D_801311D4,
	STR_SLOT_2_HAS_INSUFFICIENT_SPACE,
	STR_IN_MEMORY_CARD_SLOT_2,
	STR_SLOT_2_IS_UNFORMATTED,
	MAIN_D_801311C0,
	MAIN_D_801311B4,
	MAIN_D_8013119C,
	STR_SLOT_1_HAS_INSUFFICIENT_SPACE,
	STR_IN_MEMORY_CARD_SLOT_1,
	MAIN_D_80131178,
	STR_SLOT_1_IS_UNFORMATTED,
	MAIN_D_80131168,
	STR_A_MEMORY_CARD,
	MAIN_D_80131150,
	MAIN_D_80131144,
	STR_PLEASE_REPLACE_THE_MEMORY_CARD,
	MAIN_D_80131134,
	MAIN_D_8013111C,
	MAIN_D_80131108,
	MAIN_D_801310F4,
	MAIN_D_801310E0,
	MAIN_D_801310CC,
	MAIN_D_801310B4,
	MAIN_D_801310A4,
	MAIN_D_80131090,
	MAIN_D_8013107C,
	MAIN_D_8013106C,
	MAIN_D_8013103C,
	MAIN_D_80131030,
	MAIN_D_80131024,
	MAIN_D_80131014,
	MAIN_D_80131008,
};
#endif

// clang-format off
SAVE_LABELS_TEXT

#if VERSION_IS(US)
char MAIN_D_801346B8[] = " is in";
char MAIN_D_801346C0[2][2] = { ".", "*" };
#elif !VERSION_IS(EU)
char MAIN_D_801346B8[4] = "）は";
char MAIN_D_801346C0[2][2] = { "", "*" };
#endif

SAVE_PLAYER_LABEL_TEXT

#if VERSION_IS(US)
char FMT_NUMBER[] = "%i";
#endif

struct DIRENTRY *MEMCARD_DIRENTRIES = (struct DIRENTRY *)(TEXTURE_BUFFER + 0x5c00);

RegisteredDigimon *VS__REGISTERED_DIGIMON_BUFFER = (RegisteredDigimon *)(TEXTURE_BUFFER + 0x4800);

MEMCARD_MESSAGES_TEXT
SAVE_MESSAGES_TEXT

#if VERSION_IS(EU)
char PATH_WILDCARD[] = "*";

NumberStrings NUMBERS = { {
	STR_NUMBER_0,  STR_NUMBER_1,  STR_NUMBER_2,  STR_NUMBER_3,
	STR_NUMBER_4,  STR_NUMBER_5,  STR_NUMBER_6,  STR_NUMBER_7,
	STR_NUMBER_8,  STR_NUMBER_9,  STR_NUMBER_10, STR_NUMBER_11,
	STR_NUMBER_12, STR_NUMBER_13, STR_NUMBER_14, STR_NUMBER_15,
} };
#endif

InventoryTable DEFAULT_INVENTORY_AMOUNTS = {
	{
		0x28, 0x2d, 0x32, 0x1e, 0x28, 0x1e, 0x1e, 0x1e,
		0x1e, 0x1e, 0x1e, 0x32, 0x1e, 0x28, 0x1e, 0x32,
		0x32, 0x50, 0x32, 0x32, 0x62, 0x32, 0x32, 0x32,
		0x32, 0x32, 0x32, 0x32, 0x32, 0x00,
	},
};

InventoryTable DEFAULT_INVENTORY_TYPES = {
	{
		0x02, 0x09, 0x0c, 0x13, 0x14, 0x15, 0x43, 0x46,
		0x0e, 0x54, 0x5a, 0x5e, 0x5f, 0x67, 0x68, 0x7d,
		0x7e, 0x7f, 0x61, 0x64, 0x6a, 0x6c, 0x6e, 0x6f,
		0x70, 0x71, 0x77, 0x20, 0x16, 0xff,
	},
};

LABELS_TEXT

CVECTOR MAIN_D_80131638[4] = {
	{ 0x80, 0x80, 0x80, 0x00 },
	{ 0x40, 0x40, 0x40, 0x00 },
	{ 0x80, 0x80, 0x00, 0x00 },
	{ 0xc0, 0x40, 0x40, 0x00 },
};

int8_t HEX_DIGITS[16] = "0123456789ABCDEF";

#if !VERSION_IS(US)
char SAVEGAME_ID_LABEL[16][6] = {
	"　０",
	"　１",
	"　２",
	"　３",
	"　４",
	"　５",
	"　６",
	"　７",
	"　８",
	"　９",
	"１０",
	"１１",
	"１２",
	"１３",
	"１４",
	"１５",
};

#if VERSION_IS(EU)
MenuHighlight MENU_HIGHLIGHTS[22] = {
	{
		0x00,
		{ 0x04, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x005e,
		0x0037,
		0x0084,
		0x000c,
	},
	{
		0x00,
		{ 0x03, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0040,
		0x0037,
		0x00c0,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0040,
		0x0037,
		0x00c0,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0058,
		0x0037,
		0x0090,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0040,
		0x0037,
		0x00c0,
		0x000c,
	},
	{
		0xff,
		{ 0x00, 0x00, 0x00, 0x00, 0x00 },
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x006e,
		0x0037,
		0x0024,
		0x000c,
	},
	{
		0x00,
		{ 0x05, 0x00, 0x00, 0x00, 0x00 },
		0x0024,
		0x0036,
		0x002e,
		0x00d4,
		0x001b,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x003a,
		0x0052,
		0x0024,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x01, 0x00, 0x00, 0x00 },
		0x000c,
		0x004c,
		0x007d,
		0x0024,
		0x000c,
	},
	{
		0xff,
		{ 0x00, 0x00, 0x00, 0x00, 0x00 },
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x0010,
		0x0060,
		0x006a,
		0x0084,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x01, 0x00, 0x00, 0x00 },
		0x000c,
		0x0040,
		0x0072,
		0x0024,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0046,
		0x007d,
		0x0024,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0046,
		0x00c3,
		0x0024,
		0x000c,
	},
	{
		0x00,
		{ 0x0a, 0x00, 0x00, 0x1d, 0x00 },
		0x000c,
		0x0034,
		0x0037,
		0x00f0,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0046,
		0x0059,
		0x0024,
		0x000c,
	},
	{
		0xff,
		{ 0x00, 0x00, 0x00, 0x00, 0x00 },
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0046,
		0x0057,
		0x0024,
		0x000c,
	},
	{
		0xff,
		{ 0x00, 0x00, 0x00, 0x00, 0x00 },
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
	},
	{
		0x00,
		{ 0x03, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x005e,
		0x0037,
		0x0084,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0046,
		0x0065,
		0x0024,
		0x000c,
	},
};
#else
#if !VERSION_IS(JP)
MenuHighlight MENU_HIGHLIGHTS[22] = {
#else
MenuHighlight MENU_HIGHLIGHTS[19] = {
#endif
	{
		0x00,
		{ 0x04, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x005e,
		0x0037,
		0x0084,
		0x000c,
	},
	{
		0x00,
		{ 0x03, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0058,
		0x0037,
		0x0090,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0058,
		0x0037,
		0x0090,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0058,
		0x0037,
		0x0090,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0058,
		0x0037,
		0x0090,
		0x000c,
	},
	{
		0xff,
		{ 0x00, 0x00, 0x00, 0x00, 0x00 },
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x006e,
		0x0037,
		0x0024,
		0x000c,
	},
	{
		0x00,
		{ 0x05, 0x00, 0x00, 0x00, 0x00 },
		0x0024,
		0x0036,
		0x002e,
		0x00d4,
		0x001b,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x003a,
		0x0052,
		0x0024,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x01, 0x00, 0x00, 0x00 },
		0x000c,
		0x004c,
		0x0065,
		0x0024,
		0x000c,
	},
	{
		0xff,
		{ 0x00, 0x00, 0x00, 0x00, 0x00 },
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x0010,
		0x0076,
		0x006a,
		0x0054,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x01, 0x00, 0x00, 0x00 },
		0x000c,
		0x0040,
		0x005a,
		0x0024,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0046,
		0x0065,
		0x0024,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0046,
		0x00c3,
		0x0024,
		0x000c,
	},
	{
		0x00,
		{ 0x0a, 0x00, 0x00, 0x1d, 0x00 },
		0x000c,
		0x0046,
		0x0037,
		0x00cc,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0046,
		0x0059,
		0x0024,
		0x000c,
	},
	{
		0xff,
		{ 0x00, 0x00, 0x00, 0x00, 0x00 },
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0046,
		0x0057,
		0x0024,
		0x000c,
	},
#if !VERSION_IS(JP)
	{
		0xff,
		{ 0x00, 0x00, 0x00, 0x00, 0x00 },
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
	},
	{
		0x00,
		{ 0x03, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x005e,
		0x0037,
		0x0084,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0046,
		0x0065,
		0x0024,
		0x000c,
	},
#endif
};
#endif

#else
char SAVEGAME_ID_LABEL[16][6] = {
	" 0",
	" 1",
	" 2",
	" 3",
	" 4",
	" 5",
	" 6",
	" 7 ",
	" 8 ",
	" 9",
	"10",
	"11",
	"12",
	"13",
	"14",
	"15",
};

MenuHighlight MENU_HIGHLIGHTS[22] = {
	{
		0x00,
		{ 0x04, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x004f,
		0x0037,
		0x0097,
		0x000c,
	},
	{
		0x00,
		{ 0x03, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0048,
		0x0037,
		0x009c,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0043,
		0x0037,
		0x00a5,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0058,
		0x0037,
		0x0090,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0043,
		0x0037,
		0x0080,
		0x000c,
	},
	{
		0xff,
		{ 0x00, 0x00, 0x00, 0x00, 0x00 },
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x006b,
		0x0037,
		0x0021,
		0x000c,
	},
	{
		0x00,
		{ 0x05, 0x00, 0x00, 0x00, 0x00 },
		0x0024,
		0x0036,
		0x002e,
		0x00d4,
		0x001b,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0037,
		0x0052,
		0x0021,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x01, 0x00, 0x00, 0x00 },
		0x000c,
		0x0049,
		0x0065,
		0x0021,
		0x000c,
	},
	{
		0xff,
		{ 0x00, 0x00, 0x00, 0x00, 0x00 },
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x0010,
		0x0076,
		0x006a,
		0x0054,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x01, 0x00, 0x00, 0x00 },
		0x000c,
		0x003d,
		0x005a,
		0x0021,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0043,
		0x0065,
		0x0021,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0043,
		0x00c3,
		0x0021,
		0x000c,
	},
	{
		0x00,
		{ 0x0a, 0x00, 0x00, 0x1d, 0x00 },
		0x000c,
		0x0046,
		0x0037,
		0x00cc,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0043,
		0x0059,
		0x0021,
		0x000c,
	},
	{
		0xff,
		{ 0x00, 0x00, 0x00, 0x00, 0x00 },
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0043,
		0x0057,
		0x0021,
		0x000c,
	},
	{
		0xff,
		{ 0x00, 0x00, 0x00, 0x00, 0x00 },
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
	},
	{
		0x00,
		{ 0x03, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x005e,
		0x0037,
		0x0084,
		0x000c,
	},
	{
		0x00,
		{ 0x02, 0x00, 0x00, 0x00, 0x00 },
		0x000c,
		0x0046,
		0x0065,
		0x0024,
		0x000c,
	},
};

#endif

int8_t MENU_VIEWS[] = {
	0x00, 0x09, 0x05, 0x0a, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x01, 0x05, 0x05, 0x05, 0x07, 0x08,
	0x05, 0x05, 0xff, 0x06, 0x02, 0x05, 0x05, 0x07,
	0x08, 0x05, 0xff, 0xff, 0xff, 0xff, 0x04, 0x05,
	0x05, 0x07, 0x08, 0x05, 0xff, 0xff, 0xff, 0xff,
	0x0b, 0x05, 0x05, 0x05, 0xff, 0xff, 0xff, 0xff,
	0x0c, 0x05, 0x0d, 0x05, 0x07, 0x08, 0x05, 0xff,
	0xff, 0xff, 0xff, 0xff, 0x0e, 0x0f, 0x10, 0x05,
	0x05, 0x05, 0x0c, 0x05, 0x11, 0x12, 0x13,
#if VERSION_EQUAL_OR_NEWER(JP_TRIAL)
	0x14,
	0xff, 0x15, 0x05, 0x05, 0x07, 0x08, 0x05, 0x05,
#endif
};

char *TITLE_MENU_ITEMS[4] = {
	MAIN_D_80131008,
	MAIN_D_80131014,
	MAIN_D_80131024,
	MAIN_D_80131030,
};

char *SLOT_ACTION_TITLES[] = {
	MAIN_D_8013103C,
	MAIN_D_8013104C,
	MAIN_D_8013105C,
	MAIN_D_8013106C,
	MAIN_D_8013107C,
	MAIN_D_80131030,
	MAIN_D_80131090,
#if VERSION_EQUAL_OR_NEWER(JP_TRIAL)
	MAIN_D_801310A4,
#endif
};

char *MEMORY_CARD_OPERATION_MESSAGES[6] = {
	MAIN_D_801310B4,
	MAIN_D_801310CC,
	MAIN_D_801310E0,
	MAIN_D_801310F4,
	MAIN_D_80131108,
	MAIN_D_8013111C,
};

#if VERSION_IS(EU)
char *MEMORY_CARD_ERROR_MESSAGES[33] = {
	STR_EMPTY, STR_EMPTY, STR_EMPTY,
	MAIN_D_80131134, STR_PLEASE_REPLACE_THE_MEMORY_CARD, STR_EMPTY,
	MAIN_D_80131144, STR_EMPTY, STR_EMPTY,
	MAIN_D_80131150, STR_A_MEMORY_CARD, STR_EMPTY,
	MAIN_D_80131168, STR_SLOT_1_IS_UNFORMATTED, STR_EMPTY,
	MAIN_D_80131178, STR_IN_MEMORY_CARD_SLOT_1, STR_EMPTY,
	STR_EMPTY, STR_EMPTY, STR_EMPTY,
	MAIN_D_8013118C, STR_SLOT_1_HAS_INSUFFICIENT_SPACE, STR_EMPTY,
	MAIN_D_8013119C, STR_EMPTY, STR_EMPTY,
	MAIN_D_801311B4, STR_EMPTY, STR_EMPTY,
	MAIN_D_801311C0, STR_EMPTY,
};

char *MEMORY_CARD_2_ERROR_MESSAGES[33] = {
	STR_EMPTY, STR_EMPTY, STR_EMPTY,
	MAIN_D_80131134, STR_EMPTY, STR_EMPTY,
	MAIN_D_80131144, STR_EMPTY, STR_EMPTY,
	MAIN_D_80131150, STR_A_MEMORY_CARD, STR_EMPTY,
	MAIN_D_80131168, STR_SLOT_2_IS_UNFORMATTED, STR_EMPTY,
	MAIN_D_80131178, STR_IN_MEMORY_CARD_SLOT_2, STR_EMPTY,
	STR_EMPTY, STR_EMPTY, STR_EMPTY,
	MAIN_D_8013118C, STR_SLOT_2_HAS_INSUFFICIENT_SPACE, STR_EMPTY,
	MAIN_D_8013119C, STR_EMPTY, STR_EMPTY,
	MAIN_D_801311B4, STR_EMPTY, STR_EMPTY,
	MAIN_D_801311C0, STR_EMPTY,
};
#else
char *MEMORY_CARD_ERROR_MESSAGES[] = {
	STR_EMPTY,
	MAIN_D_80131134,
	MAIN_D_80131144,
	MAIN_D_80131150,
	MAIN_D_80131168,
	MAIN_D_80131178,
	STR_EMPTY,
	MAIN_D_8013118C,
	MAIN_D_8013119C,
#if VERSION_EQUAL_OR_NEWER(JP_TRIAL)
	MAIN_D_801311B4,
	MAIN_D_801311C0,
#endif
};
#endif

char *SLOT_ACTION_QUESTIONS[] = {
	MAIN_D_801311D4,
	MAIN_D_801311EC,
	MAIN_D_80131200,
	STR_EMPTY,
	STR_EMPTY,
	MAIN_D_80131218,
#if VERSION_EQUAL_OR_NEWER(JP_TRIAL)
	STR_EMPTY,
	MAIN_D_80131228,
#endif
};

#if !VERSION_IS(JP) && !VERSION_IS(US)
char *BATTLE_MODE_ITEMS[4] = {
	MAIN_D_80131234,
	MAIN_D_80131030,
	MAIN_D_80131254,
	MAIN_D_801310A4,
};
#elif VERSION_IS(US)
char *BATTLE_MODE_ITEMS[4] = {
	MAIN_D_80131234,
	MAIN_D_80131240,
	MAIN_D_80131254,
	MAIN_D_80131264,
};
#endif

#if VERSION_IS(EU)
char SAVE_FILE_NAME[32] = "BESLES-02914DMR*";

SaveFile SAVE_FILE;

char MAIN_D_8013392C[68] = "Ｄ　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　";
#elif !VERSION_IS(US)
char SAVE_FILE_NAME[32] = "BISLPS-01797DMR*";

SaveFile SAVE_FILE;

char MAIN_D_8013392C[68] = "デジモン　　　　　　　　　　　　　　　　　　　　　　　　　　　　";
#else
char SAVE_FILE_NAME[32] = "BASLUS-01032DMR*";

SaveFile SAVE_FILE = { 0 };

char MAIN_D_8013392C[68] = "Digimon";
#endif

uint16_t SAVE_ICON_CLUT[16] = {
	0x0000, 0x575f, 0x3a7a, 0x1575, 0x00cd, 0x0013, 0x8009, 0x96f7,
	0x8df0, 0xc631, 0x7ff7, 0x5294, 0x39ce, 0x2d6b, 0x1ce7, 0x8842,
};

uint8_t SAVE_ICON_FRAMES[3][128] = {
	{
		0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x0f, 0x00,
		0x00, 0x00, 0xff, 0xdc, 0xdd, 0xdd, 0xfd, 0x00,
		0x00, 0xf0, 0xdc, 0xed, 0xdd, 0xde, 0xee, 0x0f,
		0x00, 0xcf, 0xec, 0xde, 0xed, 0xdd, 0xde, 0x0f,
		0xf0, 0xff, 0xbe, 0xbb, 0xfb, 0xfe, 0xff, 0xfe,
		0xdf, 0xcf, 0xbd, 0xbd, 0xeb, 0xec, 0xfe, 0xff,
		0xcf, 0xce, 0xbd, 0xbb, 0xeb, 0xec, 0xec, 0xfe,
		0xdf, 0xee, 0xee, 0xff, 0xef, 0xee, 0xed, 0xfd,
		0xff, 0xff, 0x3f, 0xff, 0xf3, 0xff, 0xfe, 0xfe,
		0xff, 0x2f, 0x3f, 0x34, 0xf2, 0x3f, 0xff, 0xff,
		0xf0, 0x43, 0xf1, 0x1f, 0xa4, 0xff, 0xf3, 0x4f,
		0x00, 0x3f, 0x44, 0x13, 0x23, 0x4f, 0xf4, 0x42,
		0x00, 0x34, 0x32, 0x11, 0x21, 0x21, 0x43, 0x43,
		0x00, 0x24, 0x31, 0x32, 0x34, 0x32, 0x44, 0x04,
		0x00, 0x40, 0x24, 0x12, 0x21, 0x43, 0x04, 0x00,
		0x00, 0x00, 0x40, 0x44, 0x44, 0x04, 0x00, 0x00,
	},
	{
		0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x0f, 0x00,
		0x00, 0x00, 0xff, 0xdc, 0xdd, 0xdd, 0xff, 0x00,
		0x00, 0xf0, 0xdc, 0xdd, 0xdd, 0xde, 0xfe, 0x0f,
		0x00, 0xcf, 0xed, 0xde, 0xed, 0xde, 0xee, 0x0f,
		0xf0, 0xff, 0xbc, 0xbb, 0xfc, 0xfd, 0xff, 0xfe,
		0xff, 0xde, 0xc9, 0xcd, 0xec, 0xec, 0xfd, 0xff,
		0xdf, 0xde, 0xb9, 0xbb, 0xec, 0xec, 0xed, 0xfd,
		0xdf, 0xee, 0xff, 0xff, 0xef, 0xee, 0xed, 0xfd,
		0xef, 0xff, 0x4f, 0xff, 0xf3, 0xf4, 0xfe, 0xfe,
		0xf0, 0x3f, 0x3f, 0x34, 0x42, 0x41, 0xf2, 0xff,
		0xf0, 0x44, 0xfa, 0x13, 0x21, 0x11, 0xff, 0x4f,
		0x00, 0x3f, 0x43, 0x12, 0x32, 0xf4, 0xf2, 0x42,
		0x00, 0x34, 0x32, 0x11, 0x21, 0x21, 0xf3, 0x43,
		0x00, 0x24, 0x31, 0x55, 0x45, 0x32, 0x44, 0x04,
		0x00, 0x40, 0x24, 0x52, 0x25, 0x43, 0x04, 0x00,
		0x00, 0x00, 0x40, 0x44, 0x44, 0x04, 0x00, 0x00,
	},
	{
		0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0x0f, 0x00,
		0x00, 0xf0, 0xcf, 0xdd, 0xdd, 0xdd, 0x0f, 0x00,
		0x00, 0xcf, 0xdc, 0xed, 0xed, 0xde, 0xfe, 0x00,
		0x00, 0xdf, 0xbd, 0xba, 0xfc, 0xfe, 0xff, 0x0f,
		0xf0, 0xce, 0xdb, 0xbb, 0xec, 0xec, 0xfe, 0xff,
		0xf0, 0xcc, 0xbb, 0xcb, 0xec, 0xec, 0xed, 0xfe,
		0xef, 0xce, 0xfe, 0xff, 0xef, 0xed, 0xed, 0xfe,
		0xdf, 0xfe, 0xff, 0x4f, 0xf3, 0xff, 0xee, 0xfd,
		0xef, 0xff, 0x3f, 0x3f, 0xf2, 0xf1, 0xf3, 0xfe,
		0xff, 0x3f, 0x3f, 0x12, 0x21, 0x21, 0xff, 0xff,
		0xf0, 0x34, 0xfa, 0x12, 0x32, 0xf4, 0xf2, 0x42,
		0x00, 0x4f, 0x43, 0x11, 0x21, 0x21, 0xf3, 0x43,
		0x00, 0x34, 0x32, 0x55, 0x45, 0x22, 0x43, 0x04,
		0x00, 0x34, 0x21, 0x52, 0x55, 0x32, 0x44, 0x00,
		0x00, 0x40, 0x44, 0x52, 0x35, 0x43, 0x04, 0x00,
		0x00, 0x00, 0x00, 0x44, 0x44, 0x44, 0x00, 0x00,
	},
};

#if VERSION_IS(US)
uint8_t SAVE_TITLE_RESERVED[28] = { 0 };
#endif
// clang-format on

void renderText(POLY_FT4 *prim, int32_t x, int32_t y, int32_t u, int32_t v,
		int32_t w, int32_t h, int32_t textColor)
{
	x -= 0xA0;
	y -= 0x78;
	SetPolyFT4(prim);
	prim->tpage = getTPage(0, 0, 704, 256);
	prim->clut = GetClut(0xD0, 0x1E8);
	setRGB0(prim, MAIN_D_80131638[textColor].r, MAIN_D_80131638[textColor].g, MAIN_D_80131638[textColor].b);
#if !VERSION_IS(US)
	setUVWH(prim, u, v, w, h);
	setXYWH(prim, x, y, w, h);
#else
	setXYWH(prim, x, y, w, h);
	setUVWH(prim, u, v, w, h);
#endif
	AddPrim(ACTIVE_ORDERING_TABLE->org, prim);
}

void renderMenuBox(int32_t x, int32_t y, int32_t w, int32_t h)
{
	RECT rect;
	POLY_G4 *prim;
	x -= 0xA0;
	y -= 0x78;
	setRECT(&rect, x, y, w, h);
	renderUIBoxBorder(&rect, 0);
	prim = (POLY_G4 *)GsGetWorkBase();
	SetPolyG4(prim);
	setRGB0(prim, 0, 0, 0x70);
	setRGB1(prim, 0, 0, 0x50);
	setRGB2(prim, 0, 0, 0x30);
	setRGB3(prim, 0, 0, 0x10);
	setXYWH(prim, x + 3, y + 3, (w - 6), (h - 6));
	AddPrim(ACTIVE_ORDERING_TABLE->org, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void renderSelectNewGameCard(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	if (CONNECTED_CARDS & 1) {
		renderText(cur++, 0x40, 0x37, 0, 0, 0xc0, 0xc, 0);
	} else {
		renderText(cur++, 0x40, 0x37, 0, 0, 0xc0, 0xc, 1);
	}
	if (CONNECTED_CARDS & 0x10) {
		renderText(cur++, 0x40, 0x43, 0, 0xc, 0xc0, 0xc, 0);
	} else {
		renderText(cur++, 0x40, 0x43, 0, 0xc, 0xc0, 0xc, 1);
	}
	renderText(cur++, 0x40, 0x4f, 0, 0x18, 0x90, 0xc, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x36, 0x32, 0xd4, 0x2e);
#elif !VERSION_IS(US)
	if (CONNECTED_CARDS & 1) {
		renderText(cur++, 0x58, 0x37, 0, 0, 0x90, 0xc, 0);
	} else {
		renderText(cur++, 0x58, 0x37, 0, 0, 0x90, 0xc, 1);
	}
	if (CONNECTED_CARDS & 0x10) {
		renderText(cur++, 0x58, 0x43, 0, 0xc, 0x90, 0xc, 0);
	} else {
		renderText(cur++, 0x58, 0x43, 0, 0xc, 0x90, 0xc, 1);
	}
	renderText(cur++, 0x58, 0x4f, 0, 0x18, 0x90, 0xc, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x4e, 0x32, 0xa4, 0x2e);
#else
	if (CONNECTED_CARDS & 1) {
		renderText(cur++, 0x4B, 0x37, 0, 0, 0xBE, 0xC, 0);
	} else {
		renderText(cur++, 0x4B, 0x37, 0, 0, 0xBE, 0xC, 1);
	}
	if (CONNECTED_CARDS & 0x10) {
		renderText(cur++, 0x4B, 0x43, 0, 0xC, 0xBE, 0xC, 0);
	} else {
		renderText(cur++, 0x4B, 0x43, 0, 0xC, 0xBE, 0xC, 1);
	}
	renderText(cur++, 0x4B, 0x4F, 0, 0x18, 0xBE, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x41, 0x32, 0xBE, 0x2E);
#endif
}

void renderSelectCardSlot(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	if (CONNECTED_CARDS & 1) {
		renderText(cur++, 0x40, 0x37, 0, 0, 0xc0, 0xc, 0);
	} else {
		renderText(cur++, 0x40, 0x37, 0, 0, 0xc0, 0xc, 1);
	}
	if (CONNECTED_CARDS & 0x10) {
		renderText(cur++, 0x40, 0x43, 0, 0xc, 0xc0, 0xc, 0);
	} else {
		renderText(cur++, 0x40, 0x43, 0, 0xc, 0xc0, 0xc, 1);
	}
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x36, 0x32, 0xd4, 0x22);
#elif !VERSION_IS(US)
	if (CONNECTED_CARDS & 1) {
		renderText(cur++, 0x58, 0x37, 0, 0, 0x90, 0xc, 0);
	} else {
		renderText(cur++, 0x58, 0x37, 0, 0, 0x90, 0xc, 1);
	}
	if (CONNECTED_CARDS & 0x10) {
		renderText(cur++, 0x58, 0x43, 0, 0xc, 0x90, 0xc, 0);
	} else {
		renderText(cur++, 0x58, 0x43, 0, 0xc, 0x90, 0xc, 1);
	}
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x4e, 0x32, 0xa4, 0x22);
#else
	if (CONNECTED_CARDS & 1) {
		renderText(cur++, 0x46, 0x37, 0, 0, 0xBE, 0xC, 0);
	} else {
		renderText(cur++, 0x46, 0x37, 0, 0, 0xBE, 0xC, 1);
	}
	if (CONNECTED_CARDS & 0x10) {
		renderText(cur++, 0x46, 0x43, 0, 0xC, 0xBE, 0xC, 0);
	} else {
		renderText(cur++, 0x46, 0x43, 0, 0xC, 0xBE, 0xC, 1);
	}
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3C, 0x32, 0xB7, 0x22);
#endif
}

void renderCheckingCard(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	renderText(cur++, 0x40, 0x37, 0, 0, 0xc0, 0x30, 2);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x36, 0x32, 0xd4, 0x3a);
#else
	renderText(cur++, 0x40, 0x37, 0, 0, 0xC0, 0x24, 2);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x36, 0x32, 0xD4, 0x2E);
#endif
}

void renderConfirmNoCard(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if !VERSION_IS(US)
	renderText(cur++, 0x3a, 0x18, 0, 0, 0x90, 0xc, 0);
#else
	renderText(cur++, 0x3A, 0x18, 0, 0, 0xBE, 0xC, 0);
#endif
	GsSetWorkBase((PACKET *)cur);
#if !VERSION_IS(US)
	renderMenuBox(0x30, 0x13, 0xb0, 0x16);
#else
	renderMenuBox(0x30, 0x13, 0xBE, 0x16);
#endif
	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	renderText(cur++, 0x22, 0x5a, 0, 0xc, 0xfc, 0x78, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x18, 0x55, 0x110, 0x82);
#else
	renderText(cur++, 0x22, 0x5A, 0, 0xC, 0xFC, 0x3C, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x18, 0x55, 0x110, 0x46);
#endif
	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x6E, 0x37, 0, 0xF0, YES_TEXT_W, 0xC, 0);
	renderText(cur++, 0x6E, 0x43, NO_TEXT_U, 0xF0, NO_TEXT_W, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x64, 0x32, 0x38, 0x22);
}

void renderSaveSlotBox(int32_t slot, int32_t x, int32_t y)
{
	POLY_FT4 *cur;
	int32_t hl;
	int32_t v;
#if VERSION_IS(EU)
	v = ((slot % 7) * 0x18) + 0x18;
#else
	v = ((slot % 7) * 0x18) + 0xC;
#endif
	cur = (POLY_FT4 *)GsGetWorkBase();
	if (((SAVEGAME_SLOT_INFO[slot].valid != 0) && (MAIN_MENU_ACTION != 0)) ||
	    ((SAVEGAME_SLOT_INFO[slot].valid == 0) && (MAIN_MENU_ACTION == 0))) {
		hl = 0;
	} else {
		hl = 1;
	}
	renderText(cur++, x + 0xA, y + 5, 0, v, 0x18, 0xC, hl);
#if VERSION_IS(EU)
	if (SAVEGAME_SLOT_INFO[slot].valid != 0) {
		renderText(cur++, x + 0x28, y + 5, 0x18, v, 0x30, 0xc, hl);
		renderText(cur++, x + 0x28, y + 0x13, 0x18, v + 0xc, 0xa0, 0xc, hl);
	} else {
		renderText(cur++, x + 0x28, y + 5, 0, 0xc, 0x78, 0xc, hl);
	}
#else
	if (SAVEGAME_SLOT_INFO[slot].valid != 0) {
		renderText(cur++, x + 0x28, y + 5, 0x18, v, 0x48, 0xC, hl);
		renderText(cur++, x + 0x76, y + 5, 0x6C, v, 0x60, 0xC, hl);
#if !VERSION_IS(US)
		renderText(cur++, x + 0x28, y + 0x13, 0x18, v + 0xc, 0x6c, 0xc, hl);
	} else {
		renderText(cur++, x + 0x28, y + 5, 0x90, 0, 0x6c, 0xc, hl);
#else
		renderText(cur++, x + 0x28, y + 0x13, 0x18, v + 0xC, 0xE0, 0xC, hl);
#endif
	}
#endif
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(x, y, 0xE0, 0x24);
}

void renderSelectSlot(void)
{
	POLY_FT4 *ft4;
	int32_t page;
	int32_t y;
	int32_t i;

	ft4 = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	renderText(ft4++, 0x3a, 0x18, 0, 0, 0xc0, 0xc, 0);
	GsSetWorkBase((PACKET *)ft4);
	renderMenuBox(0x30, 0x13, 0xd4, 0x16);
#elif !VERSION_IS(US)
	renderText(ft4++, 0x3a, 0x18, 0, 0, 0x90, 0xc, 0);
	GsSetWorkBase((PACKET *)ft4);
	renderMenuBox(0x30, 0x13, 0xb0, 0x16);
#else
	renderText(ft4++, 0x3A, 0x18, 0, 0, 0xD6, 0xC, 0);
	GsSetWorkBase((PACKET *)ft4);
	renderMenuBox(0x30, 0x13, 0xE0, 0x16);
#endif

	if (SAVE_SLOT_SCROLL_TENTHS < (MENU_HIGHLIGHTS[7].pad[2] * 10)) {
		SAVE_SLOT_SCROLL_TENTHS += 2;
	}

	if (SAVE_SLOT_SCROLL_TENTHS > (MENU_HIGHLIGHTS[7].pad[2] * 10)) {
		SAVE_SLOT_SCROLL_TENTHS -= 2;
	}

	page = SAVE_SLOT_SCROLL_TENTHS / 10;

	for (i = 1; i < 6; ++i) {
		y = (i * 0x24) + ((page * 10 - SAVE_SLOT_SCROLL_TENTHS) * 36 / 10 + 0x29);
		if (y < 0x29) {
			y = 0x29;
		}

		if (!(y < 0xBA)) {
			y = 0xB9;
		}

		renderSaveSlotBox(page + i, 0x30, y);
	}

	y = (page * 10 - SAVE_SLOT_SCROLL_TENTHS) * 36 / 10 + 0x29;
	if (y < 0x29) {
		y = 0x29;
	}

	if (!(y < 0xBA)) {
		y = 0xB9;
	}

	renderSaveSlotBox(page, 0x30, y);
}

void renderConfirmSlotSelection(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	renderText(cur++, 0x3a, 0x18, 0, 0, 0xc0, 0xc, 0);
	renderText(cur++, 0x3a, 0x2e, 0, 0x18, 0x18, 0xc, 0);
	if (SAVEGAME_SLOT_INFO[MEMORY_CARD_SLOT].valid != 0) {
		renderText(cur++, 0x58, 0x2e, 0x18, 0x18, 0x30, 0xc, 0);
		renderText(cur++, 0x58, 0x3c, 0x18, 0x24, 0xa0, 0xc, 0);
	} else {
		renderText(cur++, 0x58, 0x2e, 0, 0xc, 0x84, 0xc, 0);
	}
	renderText(cur++, 0x3a, 0x52, 0, 0xf0, YES_TEXT_W, 0xc, 0);
	renderText(cur++, 0x3a, 0x5e, NO_TEXT_U, 0xf0, NO_TEXT_W, 0xc, 0);
	renderText(cur++, 0x32, 0x74, 0, 0x30, 0xdc, 0xc, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x30, 0x13, 0xd4, 0x16);
	renderMenuBox(0x30, 0x29, 0xe0, 0x24);
	renderMenuBox(0x30, 0x4d, 0x38, 0x22);
	renderMenuBox(0x28, 0x6f, 0xf0, 0x16);
#else
	renderText(cur++, 0x3A, 0x18, 0, 0, 0x90, 0xC, 0);
	renderText(cur++, 0x3A, 0x2E, 0, 0xC, 0x18, 0xC, 0);
	if (SAVEGAME_SLOT_INFO[MEMORY_CARD_SLOT].valid != 0) {
		renderText(cur++, 0x58, 0x2E, 0x18, 0xC, 0x48, 0xC, 0);
		renderText(cur++, 0xA6, 0x2E, 0x6C, 0xC, 0x60, 0xC, 0);
#if !VERSION_IS(US)
		renderText(cur++, 0x58, 0x3c, 0x18, 0x18, 0x6c, 0xc, 0);
#else
		renderText(cur++, 0x58, 0x3C, 0, 0x18, 0xE0, 0xC, 0);
#endif
	} else {
		renderText(cur++, 0x58, 0x2E, 0x90, 0, 0x6C, 0xC, 0);
	}
	renderText(cur++, 0x3A, 0x52, 0, 0xF0, YES_TEXT_W, 0xC, 0);
	renderText(cur++, 0x3A, 0x5E, NO_TEXT_U, 0xF0, NO_TEXT_W, 0xC, 0);
#if !VERSION_IS(US)
	renderText(cur++, 0x3a, 0x74, 0, 0x30, 0xcc, 0xc, 0);
#else
	renderText(cur++, 0x3A, 0x74, 0x14, 0x30, 0xCC, 0xC, 0);
#endif
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x30, 0x13, 0xB0, 0x16);
	renderMenuBox(0x30, 0x29, 0xE0, 0x24);
	renderMenuBox(0x30, 0x4D, 0x38, 0x22);
	renderMenuBox(0x30, 0x6F, 0xE0, 0x16);
#endif
}

void renderFormatMemoryCard(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	renderText(cur++, 0x4c, 0x37, 0, 0, 0xa8, 0x3c, 2);
	renderText(cur++, 0x4c, 0x7d, 0, 0xf0, YES_TEXT_W, 0xc, 0);
	renderText(cur++, 0x4c, 0x89, NO_TEXT_U, 0xf0, NO_TEXT_W, 0xc, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x42, 0x32, 0xbc, 0x46);
	renderMenuBox(0x42, 0x78, 0x38, 0x22);
#else
	renderText(cur++, 0x4C, 0x37, 0, 0, 0xA8, 0x24, 2);
	renderText(cur++, 0x4C, 0x65, 0, 0xF0, YES_TEXT_W, 0xC, 0);
	renderText(cur++, 0x4C, 0x71, NO_TEXT_U, 0xF0, NO_TEXT_W, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x42, 0x32, 0xBC, 0x2E);
	renderMenuBox(0x42, 0x60, 0x38, 0x22);
#endif
}

void renderMemoryCardReadError(void)
{
	POLY_FT4 *cur;

	if ((MEMORY_CARD_ERROR == 1) || (MEMORY_CARD_ERROR == 3) || (MEMORY_CARD_ERROR == 4) ||
	    (MEMORY_CARD_ERROR == 5) || (MEMORY_CARD_ERROR == 7)) {
		cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
		renderText(cur++, 0x24, 0x37, 0, 0, 0xf8, 0x24, 3);
		GsSetWorkBase((PACKET *)cur);
		renderMenuBox(0x1a, 0x32, 0x10c, 0x2e);
#else
		renderText(cur++, 0x4C, 0x37, 0, 0, 0xA8, 0x18, 3);
		GsSetWorkBase((PACKET *)cur);
		renderMenuBox(0x42, 0x32, 0xBC, 0x22);
#endif
		return;
	}
	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x40, 0x37, 0, 0xC, 0xC0, 0xC, 3);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x36, 0x32, 0xD4, 0x16);
}

void renderSleepMenu(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	renderText(cur++, 0x60, 0x6a, 0, 0, 0x84, 0xc, 0);
	renderText(cur++, 0x60, 0x7a, 0, 0xc, 0x84, 0xc, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x56, 0x65, 0x94, 0x26);
#else
	renderText(cur++, 0x76, 0x6A, 0, 0, 0x54, 0xC, 0);
	renderText(cur++, 0x76, 0x7A, 0, 0xC, 0x54, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x6C, 0x65, 0x68, 0x26);
#endif
}

void renderConfirmOverwrite(void)
{
	POLY_FT4 *cur;
	int32_t mask;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	renderText(cur++, 0x40, 0x38, 0, 0, 0xc0, 0x30, 0);
	if (MEMORY_CARD_ID == 0) {
		mask = 1;
	} else {
		mask = 0x10;
	}
	if (!(CONNECTED_CARDS & mask)) {
		MEMORY_CARD_ERROR = 1;
		renderText(cur++, 0x40, 0x72, 0, 0xf0, YES_TEXT_W, 0xc, 1);
	} else {
		if (MEMORY_CARD_ERROR == 1) {
			MEMORY_CARD_ERROR = -1;
		}
		renderText(cur++, 0x40, 0x72, 0, 0xf0, YES_TEXT_W, 0xc, 0);
	}
	renderText(cur++, 0x40, 0x7e, NO_TEXT_U, 0xf0, NO_TEXT_W, 0xc, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x36, 0x33, 0xd4, 0x3a);
	renderMenuBox(0x36, 0x6d, 0x38, 0x22);
	if (MEMORY_CARD_ERROR != -1) {
		cur = (POLY_FT4 *)GsGetWorkBase();
		renderText(cur++, 0x24, 0x9a, 0, 0x3c, 0xf8, 0x24, 2);
		GsSetWorkBase((PACKET *)cur);
		renderMenuBox(0x1a, 0x95, 0x10c, 0x2e);
	}
#else
#if !VERSION_IS(US)
	renderText(cur++, 0x40, 0x38, 0, 0, 0xc0, 0x18, 0);
#else
	renderText(cur++, 0x40, 0x38, 0, 0, 0xD4, 0x18, 0);
#endif
	if (MEMORY_CARD_ID == 0) {
		mask = 1;
	} else {
		mask = 0x10;
	}
	if (!(CONNECTED_CARDS & mask)) {
		MEMORY_CARD_ERROR = 1;
		renderText(cur++, 0x40, 0x5A, 0, 0xF0, YES_TEXT_W, 0xC, 1);
	} else {
		if (MEMORY_CARD_ERROR == 1) {
			MEMORY_CARD_ERROR = -1;
		}
		renderText(cur++, 0x40, 0x5A, 0, 0xF0, YES_TEXT_W, 0xC, 0);
	}
	renderText(cur++, 0x40, 0x66, NO_TEXT_U, 0xF0, NO_TEXT_W, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
#if !VERSION_IS(US)
	renderMenuBox(0x36, 0x33, 0xd4, 0x22);
#else
	renderMenuBox(0x36, 0x33, 0xE8, 0x22);
#endif
	renderMenuBox(0x36, 0x55, 0x38, 0x22);
	if (MEMORY_CARD_ERROR != -1) {
		cur = (POLY_FT4 *)GsGetWorkBase();
		renderText(cur++, 0x4C, 0x82, 0, 0x24, 0xA8, 0x18, 2);
		GsSetWorkBase((PACKET *)cur);
		renderMenuBox(0x42, 0x7D, 0xBC, 0x22);
	}
#endif
}

void renderConfirmVSSlot(void)
{
	POLY_FT4 *cur;
	int32_t hl;
	int32_t mask;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	renderText(cur++, 0x46, 0x21, 0, 0, 0x88, 0xc, 0);
	renderText(cur++, 0x46, 0x37, 0, 0xc, 0xb8, 0x3c, 0);
#elif !VERSION_IS(US)
	renderText(cur++, 0x46, 0x21, 0, 0, 0x6c, 0xc, 0);
	renderText(cur++, 0x46, 0x37, 0, 0xc, 0xb4, 0x24, 0);
#else
	renderText(cur++, 0x46, 0x21, 0, 0, 0xD6, 0xC, 0);
	renderText(cur++, 0x46, 0x37, 0, 0xC, 0xDC, 0x24, 0);
#endif
	if (MEMORY_CARD_ID == 0) {
		mask = 1;
	} else {
		mask = 0x10;
	}
	if (CONNECTED_CARDS & mask) {
		hl = 0;
	} else {
		hl = 1;
	}
#if VERSION_IS(EU)
	renderText(cur++, 0x46, 0x7d, 0, 0xf0, YES_TEXT_W, 0xc, hl);
	renderText(cur++, 0x46, 0x89, NO_TEXT_U, 0xf0, NO_TEXT_W, 0xc, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3c, 0x1c, 0x9c, 0x16);
	renderMenuBox(0x3c, 0x32, 0xc8, 0x46);
	renderMenuBox(0x3c, 0x78, 0x38, 0x22);
#elif !VERSION_IS(US)
	renderText(cur++, 0x46, 0x65, 0, 0xf0, 0x18, 0xc, hl);
	renderText(cur++, 0x46, 0x71, 0x18, 0xf0, 0x24, 0xc, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3c, 0x1c, 0x80, 0x16);
	renderMenuBox(0x3c, 0x32, 0xc8, 0x2e);
	renderMenuBox(0x3C, 0x60, 0x38, 0x22);
#else
	renderText(cur++, 0x46, 0x65, 0, 0xF0, YES_TEXT_W, 0xC, hl);
	renderText(cur++, 0x46, 0x71, NO_TEXT_U, 0xF0, NO_TEXT_W, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3C, 0x1C, 0xE0, 0x16);
	renderMenuBox(0x3C, 0x32, 0xE6, 0x2E);
	renderMenuBox(0x3C, 0x60, 0x38, 0x22);
#endif
}

void renderRegisterDigimon(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	renderText(cur++, 0x46, 0x21, 0, 0, 0x6c, 0xc, 0);
	renderText(cur++, 0x33, 0x37, 0, 0xc, 0xc8, 0x6c, 0);
	renderText(cur++, 0x46, 0xad, 0, 0x78, 0xb8, 0xc, 0);
	renderText(cur++, 0x46, 0xc3, 0xb8, 0x78, YES_TEXT_W, 0xc, 0);
	renderText(cur++, 0x46, 0xcf, 0xd0, 0x78, NO_TEXT_W, 0xc, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3c, 0x1c, 0x80, 0x16);
	renderMenuBox(0x29, 0x32, 0xdc, 0x76);
	renderMenuBox(0x3c, 0xa8, 0xbc, 0x16);
#elif !VERSION_IS(US)
	renderText(cur++, 0x46, 0x21, 0, 0, 0x6c, 0xc, 0);
	renderText(cur++, 0x46, 0x37, 0, 0xc, 0xa2, 0x6c, 0);
	renderText(cur++, 0x46, 0xad, 0, 0x78, 0xa8, 0xc, 0);
	renderText(cur++, 0x46, 0xc3, 0xa8, 0x78, 0x18, 0xc, 0);
	renderText(cur++, 0x46, 0xcf, 0xb4, 0x78, 0x24, 0xc, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3c, 0x1c, 0x80, 0x16);
	renderMenuBox(0x3c, 0x32, 0xb6, 0x76);
	renderMenuBox(0x3c, 0xa8, 0xbc, 0x16);
#else
	renderText(cur++, 0x32, 0x21, 0, 0, 0xE2, 0xC, 0);
	renderText(cur++, 0x32, 0x37, 0, 0xC, 0xD6, 0x6C, 0);
	renderText(cur++, 0x46, 0xAD, 0, 0x78, 0xB4, 0xC, 0);
	renderText(cur++, 0x46, 0xC3, 0, 0x84, 0x24, 0xC, 0);
	renderText(cur++, 0x46, 0xCF, 0, 0x90, 0x24, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x28, 0x1C, 0xF6, 0x16);
	renderMenuBox(0x28, 0x32, 0xE0, 0x76);
	renderMenuBox(0x3C, 0xA8, 0xC8, 0x16);
#endif
	renderMenuBox(0x3C, 0xBE, 0x38, 0x22);
}

void renderSelectRegisterSlot(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	renderText(cur++, 0x46, 0x21, 0, 0, 0x6c, 0xc, 0);
	renderText(cur++, 0x34, 0x37, 0, 0xc, 0xe0, 0x78, 0);
	renderText(cur++, 0x46, 0xb9, 0, 0x84, 0x90, 0xc, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3c, 0x1c, 0x80, 0x16);
	renderMenuBox(0x2e, 0x32, 0x100, 0x82);
	renderMenuBox(0x3c, 0xb4, 0xa4, 0x16);
#elif !VERSION_IS(US)
	renderText(cur++, 0x46, 0x21, 0, 0, 0x6c, 0xc, 0);
	renderText(cur++, 0x46, 0x37, 0, 0xc, 0xe0, 0x78, 0);
	renderText(cur++, 0x46, 0xb9, 0, 0x84, 0x90, 0xc, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3c, 0x1c, 0x80, 0x16);
	renderMenuBox(0x3c, 0x32, 0xe0, 0x82);
	renderMenuBox(0x3c, 0xb4, 0xa4, 0x16);
#else
	renderText(cur++, 0x32, 0x21, 0, 0, 0xE2, 0xC, 0);
	renderText(cur++, 0x46, 0x37, 0, 0xC, 0xE0, 0x78, 0);
	renderText(cur++, 0x46, 0xB9, 0, 0x84, 0xCA, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x28, 0x1C, 0xF6, 0x16);
	renderMenuBox(0x3C, 0x32, 0xE0, 0x82);
	renderMenuBox(0x3C, 0xB4, 0xD4, 0x16);
#endif
}

void renderConfirmRegister(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	renderText(cur++, 0x46, 0x21, 0, 0, 0x6c, 0xc, 0);
	renderText(cur++, 0x46, 0x37, 0, 0x18, 0x70, 0xc, 0);
	renderText(cur++, 0x46, 0x59, 0x70, 0x18, 0x18, 0xc, 0);
	renderText(cur++, 0x46, 0x65, 0x88, 0x18, 0x24, 0xc, 0);
	renderText(cur++, 0x46, 0x7d, 0, 0x24, 0xc0, 0x30, 2);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3c, 0x1c, 0x80, 0x16);
	renderMenuBox(0x3c, 0x32, 0xd4, 0x22);
	renderMenuBox(0x3c, 0x54, 0x38, 0x22);
	renderMenuBox(0x3c, 0x78, 0xd4, 0x3a);
#elif !VERSION_IS(US)
	renderText(cur++, 0x46, 0x21, 0, 0, 0x6c, 0xc, 0);
	renderText(cur++, 0x46, 0x37, 0, 0xc, 0xc0, 0x18, 0);
	renderText(cur++, 0x46, 0x59, 0xc0, 0x18, 0x18, 0xc, 0);
	renderText(cur++, 0x46, 0x65, 0xcc, 0x18, 0x24, 0xc, 0);
	renderText(cur++, 0x46, 0x7d, 0, 0x24, 0xc0, 0x18, 2);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3c, 0x1c, 0x80, 0x16);
	renderMenuBox(0x3c, 0x32, 0xd4, 0x22);
	renderMenuBox(0x3c, 0x54, 0x38, 0x22);
	renderMenuBox(0x3c, 0x78, 0xd4, 0x22);
#else
	renderText(cur++, 0x32, 0x21, 0, 0, 0xE2, 0xC, 0);
	renderText(cur++, 0x46, 0x37, 0, 0xC, 0xC0, 0x18, 0);
	renderText(cur++, 0x46, 0x59, 0, 0x24, 0x28, 0xC, 0);
	renderText(cur++, 0x46, 0x65, 0, 0x30, 0x28, 0xC, 0);
	renderText(cur++, 0x46, 0x7D, 0, 0x3C, 0xC0, 0x24, 2);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x28, 0x1C, 0xF6, 0x16);
	renderMenuBox(0x3C, 0x32, 0xD4, 0x22);
	renderMenuBox(0x3C, 0x54, 0x38, 0x22);
	renderMenuBox(0x3C, 0x78, 0xD4, 0x2E);
#endif
}

void renderCantRegister(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	renderText(cur++, 0x22, 0x49, 0, 0, 0xfc, 0x3c, 3);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x18, 0x40, 0x110, 0x46);
#elif !VERSION_IS(US)
	renderText(cur++, 0x22, 0x49, 0, 0, 0xfc, 0xc, 3);
	renderText(cur++, 0x22, 0x59, 0, 0xc, 0xfc, 0xc, 3);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x18, 0x40, 0x110, 0x2e);
#else
	renderText(cur++, 0x22, 0x46, 0, 0, 0xFC, 0x30, 3);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x18, 0x40, 0x110, 0x3A);
#endif
}

void renderDoYouWantToSave(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	renderText(cur++, 0x46, 0x3c, 0, 0, 0x88, 0xc, 0);
#else
	renderText(cur++, 0x46, 0x3C, 0, 0, 0xA8, 0xC, 0);
#endif
	renderText(cur++, 0x46, 0x57, 0, 0xC, YES_TEXT_W, 0xC, 0);
	renderText(cur++, 0x46, 0x63, NO_TEXT_U, 0xC, NO_TEXT_W, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3C, 0x32, 0xBC, 0x20);
	renderMenuBox(0x3C, 0x52, 0x38, 0x22);
}

void renderCantRegisterBaby(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if VERSION_IS(EU)
	renderText(cur++, 0x2e, 0x45, 0, 0, 0xe4, 0x48, 3);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x24, 0x40, 0xf8, 0x52);
#else
	renderText(cur++, 0x2E, 0x45, 0, 0, 0xE4, 0x24, 3);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x24, 0x40, 0xF8, 0x2E);
#endif
}

#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
void renderBattleModeMenu(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x5e, 0x37, 0, 0, 0x84, 0xc, 0);
	renderText(cur++, 0x5e, 0x43, 0, 0xc, 0x84, 0xc, 0);
	renderText(cur++, 0x5e, 0x4f, 0, 0x18, 0x84, 0xc, 0);
	if (SECRET_CODE != -1) {
		renderText(cur++, 0x5e, 0x5b, 0, 0x24, 0x84, 0xc, 0);
	}
	GsSetWorkBase((PACKET *)cur);
	if (SECRET_CODE != -1) {
		renderMenuBox(0x54, 0x32, 0x98, 0x3a);
	} else {
		renderMenuBox(0x54, 0x32, 0x98, 0x2e);
	}
}

void renderInsertGameCardPrompt(void)
{
	POLY_FT4 *cur;
	int32_t disabled;
	int32_t slotMask;

	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x46, 0x37, 0, 0, 0xb4, 0x24, 0);
	if (MEMORY_CARD_ID == 0) {
		slotMask = 1;
	} else {
		slotMask = 0x10;
	}
	if (CONNECTED_CARDS & slotMask) {
		disabled = 0;
	} else {
		disabled = 1;
	}
	renderText(cur++, 0x46, 0x65, 0, 0x24, YES_TEXT_W, 0xc, disabled);
	renderText(cur++, 0x46, 0x71, NO_TEXT_U, 0x24, NO_TEXT_W, 0xc, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3c, 0x32, 0xc8, 0x2e);
	renderMenuBox(0x3c, 0x60, 0x38, 0x22);
}
#endif

void drawMainMenuStrings(int32_t menu)
{
#if VERSION_IS(EU)
	int8_t view;
	int32_t i;
	char buf[0x2c];
	char question[0x44];
#elif !VERSION_IS(US)
	int8_t view;
	int32_t i;
	char buf[0x2c];
#else
	int32_t view;
	int32_t i;
	char buf[0x2C];
	int32_t type;
#endif

	MAIN_MENU_TICKS = 0;
	CURRENT_MENU = menu;
	if (CURRENT_MENU == -1) {
		return;
	}
	view = MENU_VIEWS[CURRENT_MENU];
	if (view == -1) {
		return;
	}
	MENU_HIGHLIGHTS[view].pos = MENU_HIGHLIGHTS[view].pad[1];
	MENU_HIGHLIGHTS[view].pad[2] = 0;
	SAVE_SLOT_SCROLL_TENTHS = 0;
	NEW_CARDS = 0;
	clearTextArea();
#if VERSION_IS(EU)
	switch (view) {
	case 0:
		drawString(TITLE_MENU_ITEMS[0], 0, 0);
		drawString(TITLE_MENU_ITEMS[1], 0, 0xc);
		DrawSync(0);
		drawString(TITLE_MENU_ITEMS[2], 0, 0x18);
		drawString(TITLE_MENU_ITEMS[3], 0, 0x24);
		break;
	case 1:
		drawString(SLOT_ACTION_TITLES[0], 0, 0);
		DrawSync(0);
		drawString(SLOT_ACTION_TITLES[0], 0, 0xc);
		drawString(MAIN_D_80134674, 0x88, 0xc);
		DrawSync(0);
		drawString(SLOT_ACTION_TITLES[3], 0, 0x18);
		break;
	case 2:
		drawString(SLOT_ACTION_TITLES[1], 0, 0);
		DrawSync(0);
		drawString(SLOT_ACTION_TITLES[1], 0, 0xc);
		drawString(MAIN_D_80134674, 0x88, 0xc);
		DrawSync(0);
		break;
	case 3:
		break;
	case 4:
		drawString(SLOT_ACTION_TITLES[2], 0, 0);
		DrawSync(0);
		drawString(SLOT_ACTION_TITLES[2], 0, 0xc);
		drawString(MAIN_D_80134674, 0x88, 0xc);
		DrawSync(0);
		break;
	case 5:
		drawString(MEMORY_CARD_OPERATION_MESSAGES[MEMORY_CARD_OPERATION], 0, 0);
		DrawSync(0);
		drawString(MAIN_D_80131278, 0, 0xc);
		DrawSync(0);
		drawString(MAIN_D_80131290, 0, 0x18);
		break;
	case 6:
		drawString(SLOT_ACTION_TITLES[3], 0, 0);
		DrawSync(0);
		drawString(MAIN_D_801312A0, 0, 0xc);
		DrawSync(0);
		drawString(MAIN_D_801312B8, 0, 0x18);
		DrawSync(0);
		drawString(MAIN_D_801312D0, 0, 0x24);
		DrawSync(0);
		drawString(MAIN_D_801312E4, 0, 0x30);
		DrawSync(0);
		drawString(MAIN_D_80139DF4, 0, 0x3c);
		DrawSync(0);
		drawString(STR_CARD_IS_INSERTED_DURING_PLAY, 0, 0x48);
		DrawSync(0);
		drawString(STR_TURNING_OFF_OR_RESETTING_THE, 0, 0x54);
		DrawSync(0);
		drawString(STR_UNIT_WILL_ERASE_CONTENTS_OF, 0, 0x60);
		DrawSync(0);
		drawString(STR_ADVENTURE, 0, 0x6c);
		DrawSync(0);
		drawString(MAIN_D_80139E14, 0, 0xf0);
		break;
	case 7:
		drawString(SLOT_ACTION_TITLES[MAIN_MENU_ACTION], 0, 0);
		DrawSync(0);
		if (MEMORY_CARD_ID != 0) {
			switch (MAIN_MENU_ACTION) {
			case 0:
			case 1:
			case 2:
				drawString(MAIN_D_80134674, 0x88, 0);
			}
			DrawSync(0);
		}
		drawString(STR_IN_USE, 0, 0xc);
		for (i = 0; i < 6; i++) {
			drawSaveSlotText(i, i);
		}
		break;
	case 9:
		drawString(MAIN_D_801312FC, 0, 0);
		DrawSync(0);
		if (MEMORY_CARD_ID == 0) {
			drawString(STR_CARD_SLOT_1_NOT, 0, 0xc);
		} else {
			drawString(STR_CARD_SLOT_2_NOT, 0, 0xc);
		}
		DrawSync(0);
		drawString(MAIN_D_80131318, 0, 0x18);
		DrawSync(0);
		drawString(MAIN_D_80131328, 0, 0x24);
		DrawSync(0);
		drawString(MAIN_D_80139E14, 0, 0xf0);
		break;
	case 8:
		drawString(SLOT_ACTION_TITLES[MAIN_MENU_ACTION], 0, 0);
		if (MEMORY_CARD_ID != 0) {
			switch (MAIN_MENU_ACTION) {
			case 0:
			case 1:
			case 2:
				drawString(MAIN_D_80134674, 0x88, 0);
			}
			DrawSync(0);
		}
		drawString(MAIN_D_80131340, 0, 0xc);
		DrawSync(0);
		drawString(SAVEGAME_ID_LABEL[MEMORY_CARD_SLOT + 1], 0, 0x18);
		DrawSync(0);
		drawString(SAVEGAME_SLOT_INFO[MEMORY_CARD_SLOT].playerName, 0x18, 0x18);
		DrawSync(0);
		drawString(SAVEGAME_SLOT_INFO[MEMORY_CARD_SLOT].digimonName, 0x18, 0x24);
		DrawSync(0);
		{
			NumberStrings numbers = NUMBERS;

			strcpy(question, SLOT_ACTION_QUESTIONS[MAIN_MENU_ACTION]);
			strcat(question, STR_SPACE);
			strcat(question, numbers.strings[MEMORY_CARD_SLOT + 1]);
			strcat(question, STR_SPACE_QUESTION);
		}
		drawString(question, 0, 0x30);
		DrawSync(0);
		drawString(MAIN_D_80139E14, 0, 0xf0);
		if (MAIN_MENU_ACTION == 2) {
			MENU_HIGHLIGHTS[view].pos = 1;
		}
		break;
	case 10:
		if (MEMORY_CARD_ID == 0) {
			drawString(MEMORY_CARD_ERROR_MESSAGES[MEMORY_CARD_ERROR * 3], 0, 0);
			DrawSync(0);
			drawString(MEMORY_CARD_ERROR_MESSAGES[MEMORY_CARD_ERROR * 3 + 1], 0, 0xc);
			DrawSync(0);
			drawString(MEMORY_CARD_ERROR_MESSAGES[MEMORY_CARD_ERROR * 3 + 2], 0, 0x18);
		} else {
			drawString(MEMORY_CARD_2_ERROR_MESSAGES[MEMORY_CARD_ERROR * 3], 0, 0);
			DrawSync(0);
			drawString(MEMORY_CARD_2_ERROR_MESSAGES[MEMORY_CARD_ERROR * 3 + 1], 0, 0xc);
			DrawSync(0);
			drawString(MEMORY_CARD_2_ERROR_MESSAGES[MEMORY_CARD_ERROR * 3 + 2], 0, 0x18);
		}
		break;
	case 11:
		drawString(MAIN_D_8013136C, 0, 0);
		drawString(MAIN_D_80131378, 0, 0xc);
		break;
	case 12:
		drawString(MAIN_D_80131390, 0, 0);
		DrawSync(0);
		drawString(STR_OVERWRITTEN, 0, 0xc);
		DrawSync(0);
		drawString(MAIN_D_801313B0, 0, 0x18);
		DrawSync(0);
		if (MEMORY_CARD_ERROR != -1) {
			if (MEMORY_CARD_ID == 0) {
				drawString(MEMORY_CARD_ERROR_MESSAGES[MEMORY_CARD_ERROR * 3], 0, 0x3c);
				DrawSync(0);
				drawString(MEMORY_CARD_ERROR_MESSAGES[MEMORY_CARD_ERROR * 3 + 1], 0, 0x48);
				DrawSync(0);
				drawString(MEMORY_CARD_ERROR_MESSAGES[MEMORY_CARD_ERROR * 3 + 2], 0, 0x54);
			} else {
				drawString(MEMORY_CARD_2_ERROR_MESSAGES[MEMORY_CARD_ERROR * 3], 0, 0x3c);
				DrawSync(0);
				drawString(MEMORY_CARD_2_ERROR_MESSAGES[MEMORY_CARD_ERROR * 3 + 1], 0, 0x48);
				DrawSync(0);
				drawString(MEMORY_CARD_2_ERROR_MESSAGES[MEMORY_CARD_ERROR * 3 + 2], 0, 0x54);
			}
		}
		drawString(MAIN_D_80139E14, 0, 0xf0);
		break;
	case 13:
		drawString(SLOT_ACTION_TITLES[5], 0, 0);
		DrawSync(0);
		drawString(MAIN_D_801313E4, 0, 0xc);
		DrawSync(0);
		drawString(MAIN_D_80131400, 0, 0x18);
		DrawSync(0);
		drawString(MAIN_D_8013141C, 0, 0x24);
		DrawSync(0);
		if (VS_PLAYER_INDEX == 1) {
			drawString(STR_PLAYER_2_IN_MEMORY, 0, 0x30);
			DrawSync(0);
			drawString(STR_CARD_SLOT_2, 0, 0x3c);
		} else {
			drawString(STR_PLAYER_1_IN_MEMORY, 0, 0x30);
			DrawSync(0);
			drawString(STR_CARD_SLOT_1, 0, 0x3c);
		}
		DrawSync(0);
		drawString(MAIN_D_80139F6C, 0, 0xf0);
		break;
	case 14:
		drawString(SLOT_ACTION_TITLES[6], 0, 0);
		DrawSync(0);
		drawString(MAIN_D_80134684, 0, 0xc);
		drawString(PARTNER_ENTITY.name, 0x2a, 0xc);
		DrawSync(0);
		drawString(MAIN_D_8013D814, 0, 0x18);
		drawString(DIGIMON_NAME(PARTNER_ENTITY.digimonEntity.entity.type), 0x2a, 0x18);
		DrawSync(0);
		drawString(MAIN_D_80134694, 0, 0x24);
		drawString(formatInteger(PARTNER_ENTITY.digimonEntity.stats.base.hp, buf, 4), 0x2a, 0x24);
		DrawSync(0);
		drawString(MAIN_D_80134698, 0, 0x30);
		drawString(formatInteger(PARTNER_ENTITY.digimonEntity.stats.base.mp, buf, 4), 0x2a, 0x30);
		DrawSync(0);
		drawString(MAIN_D_8013469C, 0, 0x3c);
		drawString(formatInteger(PARTNER_ENTITY.digimonEntity.stats.base.off, buf, 3), 0x32, 0x3c);
		DrawSync(0);
		drawString(MAIN_D_801346A4, 0, 0x48);
		drawString(formatInteger(PARTNER_ENTITY.digimonEntity.stats.base.def, buf, 3), 0x32, 0x48);
		DrawSync(0);
		drawString(MAIN_D_8013143C, 0, 0x54);
		drawMoveName(PARTNER_ENTITY.digimonEntity.entity.type, PARTNER_ENTITY.digimonEntity.stats.base.moves[0], 0x2a, 0x54);
		DrawSync(0);
		drawString(MAIN_D_80131448, 0, 0x60);
		drawMoveName(PARTNER_ENTITY.digimonEntity.entity.type, PARTNER_ENTITY.digimonEntity.stats.base.moves[1], 0x2a, 0x60);
		DrawSync(0);
		drawString(MAIN_D_80131454, 0, 0x6c);
		drawMoveName(PARTNER_ENTITY.digimonEntity.entity.type, PARTNER_ENTITY.digimonEntity.stats.base.moves[2], 0x2a, 0x6c);
		DrawSync(0);
		drawString(MAIN_D_80131460, 0, 0x78);
		break;
	case 15:
		drawString(SLOT_ACTION_TITLES[6], 0, 0);
		DrawSync(0);
		drawRegisteredDigimonSlots(0);
		drawString(MAIN_D_8013147C, 0, 0x84);
		break;
	case 16:
		drawString(SLOT_ACTION_TITLES[6], 0, 0);
		DrawSync(0);
		strcpy(buf, &SAVEGAME_ID_LABEL[(BATTLE_REGISTRATION_SLOT + 1) / 10][2]);
		strcpy(buf, &SAVEGAME_ID_LABEL[(BATTLE_REGISTRATION_SLOT + 1) % 10][2]);
		strcat(buf, STR_SPACE);
		strcat(buf, PARTNER_ENTITY.name);
		strcat(buf, STR_SAVE_TO_BLOCK_QUESTION);
		drawString(buf, 0, 0xc);
		DrawSync(0);
		drawString(MAIN_D_80139FCC, 0, 0x18);
		DrawSync(0);
		drawString(MAIN_D_801314B0, 0, 0x24);
		DrawSync(0);
		drawString(MAIN_D_801314C8, 0, 0x30);
		DrawSync(0);
		drawString(MAIN_D_801314E4, 0, 0x3c);
		DrawSync(0);
		drawString(STR_PROCESS, 0, 0x48);
		break;
	case 17:
		drawString(MAIN_D_80131500, 0, 0);
		DrawSync(0);
		drawString(MAIN_D_8013151C, 0, 0xc);
		DrawSync(0);
		drawString(MAIN_D_80131538, 0, 0x18);
		DrawSync(0);
		drawString(MAIN_D_8013154C, 0, 0x24);
		break;
	case 18:
		drawString(MAIN_D_80131568, 0, 0);
		DrawSync(0);
		drawString(STR_YES_NO_PADDED, 0, 0xc);
		break;
	case 19:
		strcpy(buf, PARTNER_ENTITY.name);
		strcat(buf, MAIN_D_801346B4);
		strcat(buf, DIGIMON_NAME(PARTNER_ENTITY.digimonEntity.entity.type));
		strcat(buf, MAIN_D_801346B8);
		drawString(buf, 0, 0);
		DrawSync(0);
		drawString(MAIN_D_80131580, 0, 0xc);
		DrawSync(0);
		drawString(MAIN_D_801315A0, 0, 0x18);
		DrawSync(0);
		drawString(STR_YOU_CANNOT_SAVE_IT, 0, 0x24);
		DrawSync(0);
		drawString(STR_IN_MEMORY_CARD_FOR, 0, 0x30);
		DrawSync(0);
		drawString(STR_MAIN_MENU_BATTLE, 0, 0x3c);
		break;
	case 20:
		drawString(BATTLE_MODE_ITEMS[0], 0, 0);
		DrawSync(0);
		drawString(BATTLE_MODE_ITEMS[1], 0, 0xc);
		DrawSync(0);
		drawString(BATTLE_MODE_ITEMS[2], 0, 0x18);
		drawString(BATTLE_MODE_ITEMS[3], 0, 0x24);
		break;
	case 21:
		drawString(MAIN_D_801315C0, 0, 0);
		DrawSync(0);
		drawString(MAIN_D_801315D0, 0, 0xc);
		DrawSync(0);
		drawString(MAIN_D_801315EC, 0, 0x18);
		DrawSync(0);
		drawString(STR_FOR_PLAYER_1_IN_MEMORY, 0, 0x24);
		DrawSync(0);
		drawString(STR_CARD_SLOT_1, 0, 0x30);
		DrawSync(0);
		drawString(MAIN_D_80139E14, 0, 0xf0);
		break;
	}
#elif !VERSION_IS(US)
	switch (view) {
	case 0:
		drawString(TITLE_MENU_ITEMS[0], 0, 0);
		drawString(TITLE_MENU_ITEMS[1], 0, 0xc);
		DrawSync(0);
		drawString(TITLE_MENU_ITEMS[2], 0, 0x18);
		drawString(TITLE_MENU_ITEMS[3], 0, 0x24);
		break;
	case 1:
		drawString(SLOT_ACTION_TITLES[0], 0, 0);
		DrawSync(0);
		drawString(SLOT_ACTION_TITLES[0], 0, 0xc);
		drawString(MAIN_D_80134674, 0x30, 0xc);
		DrawSync(0);
		drawString(SLOT_ACTION_TITLES[3], 0, 0x18);
		break;
	case 2:
		drawString(SLOT_ACTION_TITLES[1], 0, 0);
		DrawSync(0);
		drawString(SLOT_ACTION_TITLES[1], 0, 0xc);
		drawString(MAIN_D_80134674, 0x30, 0xc);
		break;
	case 3:
		break;
	case 4:
		drawString(SLOT_ACTION_TITLES[2], 0, 0);
		DrawSync(0);
		drawString(SLOT_ACTION_TITLES[2], 0, 0xc);
		drawString(MAIN_D_80134674, 0x30, 0xc);
		break;
	case 5:
		drawString(MEMORY_CARD_OPERATION_MESSAGES[MEMORY_CARD_OPERATION], 0, 0);
		DrawSync(0);
		drawString(MAIN_D_80131278, 0, 0xc);
		DrawSync(0);
		drawString(MAIN_D_80131290, 0, 0x18);
		break;
	case 6:
		drawString(SLOT_ACTION_TITLES[3], 0, 0);
		DrawSync(0);
		drawString(MAIN_D_801312A0, 0, 0xc);
		DrawSync(0);
		drawString(MAIN_D_801312B8, 0, 0x18);
		DrawSync(0);
		drawString(MAIN_D_801312D0, 0, 0x24);
		DrawSync(0);
		drawString(MAIN_D_801312E4, 0, 0x30);
		DrawSync(0);
		drawString(MAIN_D_80139DF4, 0, 0x3c);
		DrawSync(0);
		drawString(MAIN_D_80139E14, 0, 0xf0);
		break;
	case 7:
		drawString(SLOT_ACTION_TITLES[MAIN_MENU_ACTION], 0, 0);
		DrawSync(0);
		if (MAIN_MENU_ACTION == 0 || MAIN_MENU_ACTION == 1 || MAIN_MENU_ACTION == 2) {
			if (MEMORY_CARD_ID != 0) {
				drawString(MAIN_D_80134674, 0x30, 0);
			}
		}
		drawString(MAIN_D_80131340, 0x90, 0);
		for (i = 0; i < 6; i++) {
			drawSaveSlotText(i, i);
		}
		break;
	case 9:
		drawString(MAIN_D_801312FC, 0, 0);
		DrawSync(0);
		if (MEMORY_CARD_ID != 0) {
			drawString(MAIN_D_80134674, 0x30, 0);
		}
		DrawSync(0);
		drawString(MAIN_D_80131318, 0, 0xc);
		DrawSync(0);
		drawString(MAIN_D_80131328, 0, 0x18);
		DrawSync(0);
		drawString(MAIN_D_80139E14, 0, 0xf0);
		break;
	case 8:
		drawString(SLOT_ACTION_TITLES[MAIN_MENU_ACTION], 0, 0);
		if (MAIN_MENU_ACTION == 0 || MAIN_MENU_ACTION == 1 || MAIN_MENU_ACTION == 2) {
			if (MEMORY_CARD_ID != 0) {
				drawString(MAIN_D_80134674, 0x30, 0);
			}
		}
		DrawSync(0);
		drawString(MAIN_D_80131340, 0x90, 0);
		DrawSync(0);
		drawString(SAVEGAME_ID_LABEL[MEMORY_CARD_SLOT + 1], 0, 0xc);
		DrawSync(0);
		drawString(SAVEGAME_SLOT_INFO[MEMORY_CARD_SLOT].playerName, 0x18, 0xc);
		DrawSync(0);
		drawString(SAVEGAME_SLOT_INFO[MEMORY_CARD_SLOT].digimonName, 0x6c, 0xc);
		DrawSync(0);
		drawString(SAVEGAME_SLOT_INFO[MEMORY_CARD_SLOT].location, 0x18, 0x18);
		DrawSync(0);
		drawString(SAVEGAME_ID_LABEL[MEMORY_CARD_SLOT + 1], 0, 0x30);
		DrawSync(0);
		drawString(SLOT_ACTION_QUESTIONS[MAIN_MENU_ACTION], 0x18, 0x30);
		DrawSync(0);
		drawString(MAIN_D_80139E14, 0, 0xf0);
		if (MAIN_MENU_ACTION == 2) {
			MENU_HIGHLIGHTS[view].pos = 1;
		}
		break;
	case 10:
		drawString(MAIN_D_8013134C, 0, 0);
		DrawSync(0);
		if (MEMORY_CARD_ID != 0) {
			drawString(MAIN_D_80134674, 0x30, 0);
		}
		if (MEMORY_CARD_ERROR == 1) {
			drawString(MAIN_D_80134678, 0x3c, 0);
		}
		if (MEMORY_CARD_ERROR == 1 || MEMORY_CARD_ERROR == 4) {
			drawString(MAIN_D_8013467C, 0x9c, 0);
		}
		if (MEMORY_CARD_ERROR == 3) {
			drawString(MAIN_D_80134680, 0x9c, 0);
		}
		DrawSync(0);
		drawString(MEMORY_CARD_ERROR_MESSAGES[MEMORY_CARD_ERROR], 0, 0xc);
		break;
	case 11:
		drawString(MAIN_D_8013136C, 0, 0);
		drawString(MAIN_D_80131378, 0, 0xc);
		break;
	case 12:
		drawString(MAIN_D_80131390, 0, 0);
		DrawSync(0);
		drawString(MAIN_D_801313B0, 0, 0xc);
		DrawSync(0);
		if (MEMORY_CARD_ERROR != -1) {
			drawString(MAIN_D_8013134C, 0, 0x24);
			DrawSync(0);
			if (MEMORY_CARD_ID != 0) {
				drawString(MAIN_D_80134674, 0x30, 0x24);
			}
			if (MEMORY_CARD_ERROR == 1) {
				drawString(MAIN_D_80134678, 0x3c, 0x24);
			}
			if (MEMORY_CARD_ERROR == 1 || MEMORY_CARD_ERROR == 4) {
				drawString(MAIN_D_8013467C, 0x9c, 0x24);
			}
			if (MEMORY_CARD_ERROR == 3) {
				drawString(MAIN_D_80134680, 0x9c, 0x24);
			}
			DrawSync(0);
			drawString(MEMORY_CARD_ERROR_MESSAGES[MEMORY_CARD_ERROR], 0, 0x30);
			DrawSync(0);
		}
		drawString(MAIN_D_80139E14, 0, 0xf0);
		break;
	case 13:
		drawString(SLOT_ACTION_TITLES[5], 0, 0);
		DrawSync(0);
		drawString(MAIN_D_801313E4, 0, 0xc);
		DrawSync(0);
		drawString(MAIN_D_80131400, 0, 0x18);
		DrawSync(0);
		drawString(MAIN_D_8013141C, 0, 0x24);
		DrawSync(0);
		drawString(MAIN_D_80139F6C, 0, 0xf0);
		if (VS_PLAYER_INDEX == 1) {
			drawString(MAIN_D_80134674, 0x30, 0xc);
			drawString(MAIN_D_80134674, 0x84, 0xc);
		}
		break;
	case 14:
		drawString(SLOT_ACTION_TITLES[6], 0, 0);
		DrawSync(0);
		drawString(MAIN_D_80134684, 0, 0xc);
		drawString(PARTNER_ENTITY.name, 0x2a, 0xc);
		DrawSync(0);
		drawString(MAIN_D_8013D814, 6, 0x18);
		drawString(DIGIMON_NAME(PARTNER_ENTITY.digimonEntity.entity.type), 0x2a, 0x18);
		DrawSync(0);
		drawString(MAIN_D_80134694, 6, 0x24);
		drawString(formatInteger(PARTNER_ENTITY.digimonEntity.stats.base.hp, buf, 4), 0x2a, 0x24);
		DrawSync(0);
		drawString(MAIN_D_80134698, 6, 0x30);
		drawString(formatInteger(PARTNER_ENTITY.digimonEntity.stats.base.mp, buf, 4), 0x2a, 0x30);
		DrawSync(0);
		drawString(MAIN_D_8013469C, 0, 0x3c);
		drawString(formatInteger(PARTNER_ENTITY.digimonEntity.stats.base.off, buf, 3), 0x36, 0x3c);
		DrawSync(0);
		drawString(MAIN_D_801346A4, 0, 0x48);
		drawString(formatInteger(PARTNER_ENTITY.digimonEntity.stats.base.def, buf, 3), 0x36, 0x48);
		DrawSync(0);
		drawString(MAIN_D_8013143C, 6, 0x54);
		drawMoveName(PARTNER_ENTITY.digimonEntity.entity.type, PARTNER_ENTITY.digimonEntity.stats.base.moves[0], 0x2a, 0x54);
		DrawSync(0);
		drawString(MAIN_D_80131448, 6, 0x60);
		drawMoveName(PARTNER_ENTITY.digimonEntity.entity.type, PARTNER_ENTITY.digimonEntity.stats.base.moves[1], 0x2a, 0x60);
		DrawSync(0);
		drawString(MAIN_D_80131454, 6, 0x6c);
		drawMoveName(PARTNER_ENTITY.digimonEntity.entity.type, PARTNER_ENTITY.digimonEntity.stats.base.moves[2], 0x2a, 0x6c);
		DrawSync(0);
		drawString(MAIN_D_80131460, 0, 0x78);
		break;
	case 15:
		drawString(SLOT_ACTION_TITLES[6], 0, 0);
		DrawSync(0);
		drawRegisteredDigimonSlots(0);
		drawString(MAIN_D_8013147C, 0, 0x84);
		break;
	case 16:
		drawString(SLOT_ACTION_TITLES[6], 0, 0);
		DrawSync(0);
		drawString(&SAVEGAME_ID_LABEL[(BATTLE_REGISTRATION_SLOT + 1) / 10][2], 0, 0xc);
		DrawSync(0);
		drawString(&SAVEGAME_ID_LABEL[(BATTLE_REGISTRATION_SLOT + 1) % 10][2], 0xc, 0xc);
		DrawSync(0);
		strcpy(buf, MAIN_D_8013D854);
		strcat(buf, PARTNER_ENTITY.name);
		strcat(buf, MAIN_D_80139FBC);
		drawString(buf, 0x18, 0xc);
		DrawSync(0);
		drawString(MAIN_D_80139FCC, 0, 0x18);
		DrawSync(0);
		drawString(MAIN_D_801314B0, 0, 0x24);
		DrawSync(0);
		drawString(MAIN_D_801314C8, 0, 0x30);
		break;
	case 17:
		drawString(MAIN_D_80131500, 0, 0);
		DrawSync(0);
		drawString(MAIN_D_8013151C, 0, 0xc);
		break;
	case 18:
		drawString(MAIN_D_80131568, 0, 0);
		DrawSync(0);
		drawString(MAIN_D_80139E14, 0, 0xc);
		break;
	case 19:
		strcpy(buf, PARTNER_ENTITY.name);
		strcat(buf, MAIN_D_801346B4);
		strcat(buf, DIGIMON_NAME(PARTNER_ENTITY.digimonEntity.entity.type));
		strcat(buf, MAIN_D_801346B8);
		drawString(buf, 0, 0);
		DrawSync(0);
		drawString(MAIN_D_80131580, 0, 0xc);
		DrawSync(0);
		drawString(MAIN_D_801315A0, 0, 0x18);
		break;
#if !VERSION_IS(JP)
	case 20:
		drawString(BATTLE_MODE_ITEMS[0], 0, 0);
		DrawSync(0);
		drawString(BATTLE_MODE_ITEMS[1], 0, 0xc);
		DrawSync(0);
		drawString(BATTLE_MODE_ITEMS[2], 0, 0x18);
		drawString(BATTLE_MODE_ITEMS[3], 0, 0x24);
		break;
	case 21:
		drawString(MAIN_D_801315C0, 0, 0);
		DrawSync(0);
		drawString(MAIN_D_801315D0, 0, 0xc);
		DrawSync(0);
		drawString(MAIN_D_801315EC, 0, 0x18);
		DrawSync(0);
		drawString(MAIN_D_80139E14, 0, 0x24);
		break;
#endif
	}
#else
	switch (view) {
	case 0:
		drawString(TITLE_MENU_ITEMS[0], 0, 0);
		drawString(TITLE_MENU_ITEMS[1], 0, 0xC);
		DrawSync(0);
		drawString(TITLE_MENU_ITEMS[2], 0, 0x18);
		drawString(TITLE_MENU_ITEMS[3], 0, 0x24);
		break;
	case 1:
		drawString(SLOT_ACTION_TITLES[0], 0, 0);
		DrawSync(0);
		drawString(SLOT_ACTION_TITLES[0], 0, 0xC);
		drawString(STR_2, 0x76, 0xC);
		DrawSync(0);
		drawString(SLOT_ACTION_TITLES[3], 0, 0x18);
		break;
	case 2:
		drawString(SLOT_ACTION_TITLES[1], 0, 0);
		DrawSync(0);
		drawString(SLOT_ACTION_TITLES[1], 0, 0xC);
		drawString(STR_2, 0x9A, 0xC);
		MAIN_D_80135054 = 1;
		break;
	case 4:
		drawString(SLOT_ACTION_TITLES[2], 0, 0);
		DrawSync(0);
		drawString(SLOT_ACTION_TITLES[2], 0, 0xC);
		drawString(STR_2, 0x76, 0xC);
		MAIN_D_80135054 = 2;
		break;
	case 5:
		drawString(MEMORY_CARD_OPERATION_MESSAGES[MEMORY_CARD_OPERATION], 0, 0);
		DrawSync(0);
		drawString(MAIN_D_80131278, 0, 0xC);
		DrawSync(0);
		drawString(MAIN_D_80131290, 0, 0x18);
		break;
	case 6:
		drawString(SLOT_ACTION_TITLES[3], 0, 0);
		DrawSync(0);
		drawString(MAIN_D_801312A0, 0, 0xC);
		DrawSync(0);
		drawString(MAIN_D_801312B8, 0, 0x18);
		DrawSync(0);
		drawString(MAIN_D_801312D0, 0, 0x24);
		DrawSync(0);
		drawString(MAIN_D_801312E4, 0, 0x30);
		DrawSync(0);
		drawString(STR_YES_NO, 0, 0xF0);
		break;
	case 7:
		drawString(SLOT_ACTION_TITLES[MAIN_MENU_ACTION], 0, 0);
		DrawSync(0);
		if (MAIN_MENU_ACTION == 0 || MAIN_MENU_ACTION == 1 ||
		    MAIN_MENU_ACTION == 2) {
			if (MEMORY_CARD_ID != 0) {
				if (MAIN_D_80135054 == 1) {
					drawString(STR_2, 0x9A, 0);
				}
				if (MAIN_D_80135054 == 2) {
					drawString(STR_2, 0x76, 0);
				}
			}
		}
		for (i = 0; i < 6; i++) {
			drawSaveSlotText(i, i);
		}
		break;
	case 9:
		drawString(MAIN_D_801312FC, 0, 0);
		DrawSync(0);
		if (MEMORY_CARD_ID != 0) {
			drawString(STR_2, 0x76, 0);
		}
		DrawSync(0);
		drawString(MAIN_D_80131318, 0, 0xC);
		DrawSync(0);
		drawString(MAIN_D_80131328, 0, 0x18);
		DrawSync(0);
		drawString(STR_YES_NO_SPACED, 0, 0xF0);
		break;
	case 8:
		drawString(SLOT_ACTION_TITLES[MAIN_MENU_ACTION], 0, 0);
		if (MAIN_MENU_ACTION == 0 || MAIN_MENU_ACTION == 1 ||
		    MAIN_MENU_ACTION == 2) {
			if (MEMORY_CARD_ID != 0) {
				if (MAIN_D_80135054 == 1) {
					drawString(STR_2, 0x9A, 0);
				}
				if (MAIN_D_80135054 == 2) {
					drawString(STR_2, 0x76, 0);
				}
			}
		}
		DrawSync(0);
		drawString(MAIN_D_80131340, 0x90, 0);
		DrawSync(0);
		drawString(SAVEGAME_ID_LABEL[MEMORY_CARD_SLOT + 1], 0, 0xC);
		DrawSync(0);
		drawString(SAVEGAME_SLOT_INFO[MEMORY_CARD_SLOT].playerName, 0x18, 0xC);
		DrawSync(0);
		drawString(SAVEGAME_SLOT_INFO[MEMORY_CARD_SLOT].digimonName, 0x6C, 0xC);
		DrawSync(0);
		drawString(SAVEGAME_SLOT_INFO[MEMORY_CARD_SLOT].location, 0, 0x18);
		DrawSync(0);
		drawString(SAVEGAME_ID_LABEL[MEMORY_CARD_SLOT + 1], 0, 0x30);
		DrawSync(0);
		drawString(SLOT_ACTION_QUESTIONS[MAIN_MENU_ACTION], 0x18, 0x30);
		DrawSync(0);
		drawString(STR_YES_NO_SPACED, 0, 0xF0);
		if (MAIN_MENU_ACTION == 2) {
			MENU_HIGHLIGHTS[view].pos = 1;
		}
		break;
	case 10:
		drawString(MAIN_D_8013134C, 0, 0);
		DrawSync(0);
		if (MEMORY_CARD_ID != 0) {
			drawString(MAIN_D_80134674, 0x30, 0);
		}
		if (MEMORY_CARD_ERROR == 1) {
			drawString(MAIN_D_80134678, 0x3C, 0);
		}
		if (MEMORY_CARD_ERROR == 1 || MEMORY_CARD_ERROR == 4) {
			drawString(MAIN_D_8013467C, 0x9C, 0);
		}
		if (MEMORY_CARD_ERROR == 3) {
			drawString(MAIN_D_80134680, 0x9C, 0);
		}
		DrawSync(0);
		drawString(MEMORY_CARD_ERROR_MESSAGES[MEMORY_CARD_ERROR], 0, 0xC);
		break;
	case 11:
		drawString(MAIN_D_8013136C, 0, 0);
		drawString(MAIN_D_80131378, 0, 0xC);
		break;
	case 12:
		drawString(MAIN_D_80131390, 0, 0);
		DrawSync(0);
		drawString(MAIN_D_801313B0, 0, 0xC);
		DrawSync(0);
		if (MEMORY_CARD_ERROR != -1) {
			drawString(MAIN_D_801313D0, 0, 0x24);
			DrawSync(0);
			if (MEMORY_CARD_ID != 0) {
				drawString(&STR_EMPTY, 0x30, 0x24);
			}
			if (MEMORY_CARD_ERROR == 1) {
				drawString(&STR_EMPTY, 0x3C, 0x24);
			}
			if (MEMORY_CARD_ERROR == 1 || MEMORY_CARD_ERROR == 4) {
				drawString(&STR_EMPTY, 0x9C, 0x24);
			}
			if (MEMORY_CARD_ERROR == 3) {
				drawString(&STR_EMPTY, 0x9C, 0x24);
			}
			DrawSync(0);
			drawString(MEMORY_CARD_ERROR_MESSAGES[MEMORY_CARD_ERROR], 0, 0x30);
			DrawSync(0);
		}
		drawString(STR_YES_NO, 0, 0xF0);
		break;
	case 13:
		drawString(SLOT_ACTION_TITLES[5], 0, 0);
		DrawSync(0);
		drawString(MAIN_D_801313E4, 0, 0xC);
		DrawSync(0);
		drawString(MAIN_D_80131400, 0, 0x18);
		DrawSync(0);
		drawString(MAIN_D_8013141C, 0, 0x24);
		DrawSync(0);
		drawString(STR_YES_NO, 0, 0xF0);
		if (VS_PLAYER_INDEX == 1) {
			drawString(STR_2, 0x76, 0x24);
			drawString(STR_2, 0xB4, 0x24);
		}
		break;
	case 14:
		drawString(SLOT_ACTION_TITLES[6], 0, 0);
		DrawSync(0);
		drawString(MAIN_D_80134684, 0, 0xC);
		drawString(PARTNER_ENTITY.name, 0x64, 0xC);
		DrawSync(0);
		drawString(MAIN_D_8013468C, 0, 0x18);
		drawString(DIGIMON_NAME(PARTNER_ENTITY.digimonEntity.entity.type), 0x64, 0x18);
		DrawSync(0);
		drawString(MAIN_D_80134694, 0, 0x24);
		drawString(formatInteger(PARTNER_ENTITY.digimonEntity.stats.base.hp, buf, 4), 0x64, 0x24);
		DrawSync(0);
		drawString(MAIN_D_80134698, 0, 0x30);
		drawString(formatInteger(PARTNER_ENTITY.digimonEntity.stats.base.mp, buf, 4), 0x64, 0x30);
		DrawSync(0);
		drawString(MAIN_D_8013469C, 0, 0x3C);
		drawString(formatInteger(PARTNER_ENTITY.digimonEntity.stats.base.off, buf, 3), 0x64, 0x3C);
		DrawSync(0);
		drawString(MAIN_D_801346A4, 0, 0x48);
		drawString(formatInteger(PARTNER_ENTITY.digimonEntity.stats.base.def, buf, 3), 0x64, 0x48);
		DrawSync(0);
		drawString(MAIN_D_8013143C, 0, 0x54);
		drawMoveName((type = PARTNER_ENTITY.digimonEntity.entity.type), PARTNER_ENTITY.digimonEntity.stats.base.moves[0], 0x64, 0x54);
		DrawSync(0);
		drawString(MAIN_D_80131448, 0, 0x60);
		drawMoveName((type = PARTNER_ENTITY.digimonEntity.entity.type), PARTNER_ENTITY.digimonEntity.stats.base.moves[1], 0x64, 0x60);
		DrawSync(0);
		drawString(MAIN_D_80131454, 0, 0x6C);
		drawMoveName((type = PARTNER_ENTITY.digimonEntity.entity.type), PARTNER_ENTITY.digimonEntity.stats.base.moves[2], 0x64, 0x6C);
		DrawSync(0);
		drawString(MAIN_D_80131460, 0, 0x78);
		drawString(MAIN_D_801346AC, 0, 0x84);
		drawString(MAIN_D_801346B0, 0, 0x90);
		break;
	case 15:
		drawString(SLOT_ACTION_TITLES[6], 0, 0);
		DrawSync(0);
		drawRegisteredDigimonSlots(0);
		drawString(MAIN_D_8013147C, 0, 0x84);
		break;
	case 16:
		drawString(SLOT_ACTION_TITLES[6], 0, 0);
		DrawSync(0);
		strcpy(buf, MAIN_D_80131498);
		drawString(buf, 0, 0xC);
		DrawSync(0);
		strcpy(buf, PARTNER_ENTITY.name);
		drawString(buf, 0, 0x18);
		DrawSync(0);
		drawString(&SAVEGAME_ID_LABEL[(BATTLE_REGISTRATION_SLOT + 1) / 10][2], 0, 0xC);
		DrawSync(0);
		drawString(&SAVEGAME_ID_LABEL[(BATTLE_REGISTRATION_SLOT + 1) % 10][2], 0xC, 0xC);
		DrawSync(0);
		drawString(MAIN_D_801346AC, 0, 0x24);
		DrawSync(0);
		drawString(MAIN_D_801346B0, 0, 0x30);
		DrawSync(0);
		drawString(MAIN_D_801314B0, 0, 0x3C);
		DrawSync(0);
		drawString(MAIN_D_801314C8, 0, 0x48);
		drawString(MAIN_D_801314E4, 0, 0x54);
		break;
	case 17:
		drawString(MAIN_D_80131500, 0, 0);
		DrawSync(0);
		drawString(MAIN_D_8013151C, 0, 0xC);
		DrawSync(0);
		drawString(MAIN_D_80131538, 0, 0x18);
		drawString(MAIN_D_8013154C, 0, 0x24);
		break;
	case 18:
		drawString(MAIN_D_80131568, 0, 0);
		DrawSync(0);
		drawString(STR_YES_NO_SPACED, 0, 0xC);
		break;
	case 19:
		strcpy(buf, PARTNER_ENTITY.name);
		strcat(buf, MAIN_D_801346B4);
		strcat(buf, DIGIMON_NAME(PARTNER_ENTITY.digimonEntity.entity.type));
		strcat(buf, MAIN_D_801346B8);
		drawString(buf, 0, 0);
		DrawSync(0);
		drawString(MAIN_D_80131580, 0, 0xC);
		DrawSync(0);
		drawString(MAIN_D_801315A0, 0, 0x18);
		break;
	case 20:
		drawString(BATTLE_MODE_ITEMS[0], 0, 0);
		DrawSync(0);
		drawString(BATTLE_MODE_ITEMS[1], 0, 0xC);
		DrawSync(0);
		drawString(BATTLE_MODE_ITEMS[2], 0, 0x18);
		drawString(BATTLE_MODE_ITEMS[3], 0, 0x24);
		break;
	case 21:
		drawString(MAIN_D_801315C0, 0, 0);
		DrawSync(0);
		drawString(MAIN_D_801315D0, 0, 0xC);
		DrawSync(0);
		drawString(MAIN_D_801315EC, 0, 0x18);
		DrawSync(0);
		drawString(STR_YES_NO, 0, 0x24);
		break;
	}
#endif
}

void drawSaveSlotText(int32_t slot, int32_t row)
{
	RECT area;

	if (slot >= 0 && slot < 0xF) {
		row *= 0x18;
#if VERSION_IS(EU)
		setRECT(&area, 0, row + 0x18, 0xcc, 0x18);
		clearTextSubArea(&area);
		drawString(SAVEGAME_ID_LABEL[slot + 1], 0, row + 0x18);
		drawString(SAVEGAME_SLOT_INFO[slot].playerName, 0x18, row + 0x18);
		DrawSync(0);
		drawString(SAVEGAME_SLOT_INFO[slot].digimonName, 0x18, row + 0x24);
		DrawSync(0);
#else
#if !VERSION_IS(US)
		setRECT(&area, 0, row + 0xc, 0xcc, 0x18);
#else
		setRECT(&area, 0, row + 0xC, 0xE0, 0x18);
#endif
		clearTextSubArea(&area);
		drawString(SAVEGAME_ID_LABEL[slot + 1], 0, row + 0xC);
		drawString(SAVEGAME_SLOT_INFO[slot].playerName, 0x18, row + 0xC);
		DrawSync(0);
		drawString(SAVEGAME_SLOT_INFO[slot].digimonName, 0x6C, row + 0xC);
		DrawSync(0);
		drawString(SAVEGAME_SLOT_INFO[slot].location, 0x18, row + 0x18);
		DrawSync(0);
#endif
	}
}

char *formatInteger(int32_t value, char *buf, int32_t digits)
{
#if !VERSION_IS(US)
	int32_t started;
	char *dst;

	started = 0;
	dst = buf;
	switch (digits) {
	case 4:
		appendWideDigit(value / 1000, &dst, &started);
		value %= 1000;
	case 3:
		appendWideDigit(value / 100, &dst, &started);
		value %= 100;
	case 2:
		appendWideDigit(value / 10, &dst, &started);
		value %= 10;
	case 1:
		appendWideDigit(value, &dst, &started);
	}
	*dst = 0;
	return buf;
#else
	sprintf(buf, FMT_NUMBER, value);
	return buf;
#endif
}

void drawMoveName(long type, int32_t anim, int32_t color, int32_t pos)
{
	int16_t move;

	if (anim >= 0x2E && anim < 0x3E) {
		move = DIGIMON_DATA[type].moves[anim - 0x2E];
		if (move >= 0 && move < 0x79) {
			drawString(MOVE_NAMES[move], color, pos);
		}
	}
}

void drawRegisteredDigimonSlots(int32_t slot)
{
	RECT area;
	uint8_t type;
	int32_t i;
	int32_t y;
	int32_t currentSlot;

#if VERSION_IS(EU)
	setRECT(&area, 0, 0xc, 0xf0, 0x78);
#else
	setRECT(&area, 0, 0xC, 0xCC, 0x78);
#endif
	clearTextSubArea(&area);
	for (i = 0; i < 10; i++) {
		currentSlot = slot + i;
		y = i * 0xc + 0xc;
		drawString(&SAVEGAME_ID_LABEL[(currentSlot + 1) / 10][2], 0, y);
		drawString(&SAVEGAME_ID_LABEL[(currentSlot + 1) % 10][2], GLYPH_WIDTH, y);
		if ((type = SAVE_FILE.saves[0].battleRegistrationData[currentSlot].digimonId) != 0) {
#if VERSION_IS(EU)
			drawString(SAVE_FILE.saves[0].battleRegistrationData[currentSlot].name, 0x18, y);
			drawString(DIGIMON_NAME(type), 0x50, y);
#else
			drawString(SAVE_FILE.saves[0].battleRegistrationData[currentSlot].name, 0x1e, y);
			drawString(DIGIMON_NAME(type), 0x6C, y);
#endif
		} else {
			setTextColor(9);
#if VERSION_IS(EU)
			drawString(MAIN_D_8013A0EC, 0x18, y);
#elif !VERSION_IS(US)
			drawString(MAIN_D_8013A0EC, 0x1e, y);
#else
			drawString(MAIN_D_801346C0[0], 0x1E, y);
#endif
			setTextColor(1);
		}
		DrawSync(0);
	}
}

void updateMemoryCardState(void)
{
	MemCardSyncWord result;
	MemCardSyncWord cmd;

	switch (MemCardSync(1, &cmd, &result)) {
	case -1:
		MemCardExist(CHECKED_MEMORY_CARD);
		return;
	case 0:
		return;
	case 1:
		if (cmd == 1) {
			switch (result) {
			case 3:
				NEW_CARDS |= (CHECKED_MEMORY_CARD == 0x10) ? 0x10 : 1;
			case 0:
				CONNECTED_CARDS |= (CHECKED_MEMORY_CARD == 0x10) ? 0x10 : 1;
				CHECKED_MEMORY_CARD ^= 0x10;
				break;
			case 2:
			case 1:
			default:
				CONNECTED_CARDS &= (CHECKED_MEMORY_CARD == 0x10) ? -0x11 : -2;
				CHECKED_MEMORY_CARD ^= 0x10;
				break;
			}
		}
		return;
	}
}

void tickMainMenu(void)
{
	int32_t view;
	MemCardSyncWord command;
	MemCardSyncWord result;
	long count;
	int32_t slot;
	uint32_t combinedInput;
	int32_t lo;
	int32_t hi;
	int32_t oldScroll;
	uint8_t loadComplete;
#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
	uint8_t demoLoadComplete;
#endif
	MenuCursor *cursor;
	int32_t input;
	int32_t status;
#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
	int32_t i;
	Inventory *inventory;
#endif

	updateMemoryCardState();
	if (TARGET_MENU != CURRENT_MENU) {
		drawMainMenuStrings(TARGET_MENU);
	}
	MAIN_MENU_TICKS++;

	switch (CURRENT_MENU) {
	case 0:
#if !VERSION_IS(US)
		cursor = (MenuCursor *)MENU_HIGHLIGHTS;
		input = tickMenuInput(cursor, 1);
#else
		input = tickMenuInput(cursor = (MenuCursor *)MENU_HIGHLIGHTS, 1);
#endif
		switch (input) {
		case 1:
			switch (cursor->pos) {
			case 0:
				TARGET_MENU = 0xA;
				MAIN_MENU_ACTION = 0;
				break;
			case 1:
				if (CONNECTED_CARDS != 0) {
					TARGET_MENU = 0x14;
					MAIN_MENU_ACTION = 1;
				}
				break;
			case 2:
				if (CONNECTED_CARDS != 0) {
					TARGET_MENU = 0x1E;
					MAIN_MENU_ACTION = 2;
				}
				break;
			case 3:
				VS_PLAYER_INDEX = 0;
				TARGET_MENU = 0x32;
				MAIN_MENU_ACTION = 5;
				break;
			}
			break;
		case 2:
			break;
		}
		break;

	case 1:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[9];
		input = tickMenuInput(cursor, 1);
		switch (input) {
		case 1:
			if (cursor->pos == 0) {
				TARGET_MENU = 2;
				MEMORY_CARD_OPERATION = 5;
			} else {
				TARGET_MENU = 0;
			}
			break;
		case 2:
			TARGET_MENU = 0;
			break;
		}
		TARGET_MENU = getMenuOnCardChange(0);
		break;

	case 2:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			status = MemCardFormat(MEMORY_CARD_ID);
			switch (status) {
			case 0:
				TARGET_MENU = MEMORY_CARD_RETURN_MENU;
				MEMORY_CARD_OPERATION = 0;
				break;
			case 1:
			case 2:
				setMemoryCardReadError(status, 0);
				break;
			default:
				setMemoryCardReadError(0, 0);
				break;
			}
		}
		break;

	case 3:
#if !VERSION_IS(JP) && !VERSION_IS(US)
		if (MAIN_MENU_TICKS >= 3) {
#elif VERSION_IS(US)
		if (((CHANGED_INPUT != CANCEL_BUTTON) && (CHANGED_INPUT != CONFIRM_BUTTON)) ||
		    (MAIN_MENU_TICKS >= 3)) {
#endif
			lo = CHANGED_INPUT & 0xFFFF;
			hi = (uint32_t)(CHANGED_INPUT & 0xFFFF0000) >> 16;
			combinedInput = lo | hi;
			if ((combinedInput == CANCEL_BUTTON) || (combinedInput == CONFIRM_BUTTON)) {
				playSound(0, 3);
				TARGET_MENU = MEMORY_CARD_RETURN_MENU;
			}
			if (MAIN_MENU_TICKS >= 0x12D) {
				TARGET_MENU = MEMORY_CARD_RETURN_MENU;
			}
#if VERSION_EQUAL_OR_NEWER(JP_TRIAL)
		}
#endif
		break;

	case 9:
		if (MAIN_MENU_TICKS >= 3) {
			TARGET_MENU = -1;
		}
		break;

	case 0xA:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[1];
		input = tickMenuInput(cursor, 1);
		switch (input) {
		case 1:
			switch (cursor->pos) {
			case 0:
				if (CONNECTED_CARDS & 1) {
					MEMORY_CARD_ID = 0;
					TARGET_MENU = 0xC;
					MEMORY_CARD_OPERATION = 0;
				}
				break;
			case 1:
				if (CONNECTED_CARDS & 0x10) {
					MEMORY_CARD_ID = 0x10;
					TARGET_MENU = 0xC;
					MEMORY_CARD_OPERATION = 0;
				}
				break;
			case 2:
				TARGET_MENU = 0x13;
				break;
			}
			break;
		case 2:
			TARGET_MENU = 0;
			break;
		}
		break;

	case 0xC:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			if (MemCardAccept(MEMORY_CARD_ID) == 0) {
				setMemoryCardReadError(0, 0);
				break;
			}
			MemCardSync(0, &command, &result);
			result = isMemcardUnformatted(MEMORY_CARD_ID, result);
			switch (result) {
			case 0:
				TARGET_MENU = 0xD;
				break;
			case 4:
				TARGET_MENU = 1;
				MEMORY_CARD_RETURN_MENU = 0xC;
				break;
			default:
				setMemoryCardReadError(result, 0);
				break;
			}
		}
		break;

	case 0xD:
		TARGET_MENU = 0xE;
		if ((MEMORY_CARD_USED_BLOCKS = getCardUsedBlockCount(MEMORY_CARD_ID, 0)) != -1) {
			if (MEMORY_CARD_USED_BLOCKS >= 0xF) {
				setMemoryCardReadError(7, 0);
			} else {
				SAVE_FILE_NAME[0xF] = 0x3F;
				loadSaveSlotData(MEMORY_CARD_ID, SAVE_FILE_NAME,
						 SAVEGAME_SLOT_INFO, 0);
			}
		}
		break;

	case 0xE:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[7];
		input = tickSelectSlotInput(cursor, 1);
		slot = cursor->pos + cursor->scroll;
		if (input != 2) {
			if (input == 1) {
				if (SAVEGAME_SLOT_INFO[slot].valid == 0) {
					TARGET_MENU = 0xF;
					MEMORY_CARD_SLOT = slot;
				}
			}
		} else {
			TARGET_MENU = 0xA;
		}
		TARGET_MENU = getMenuOnCardChange(0);
		break;

	case 0xF:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[8];
		input = tickMenuInput(cursor, 1);
		if (input != 2) {
			if (input == 1) {
				if (cursor->pos == 0) {
					TARGET_MENU = 0x10;
					MEMORY_CARD_OPERATION = 1;
				} else {
					goto cancel_create;
				}
			}
		} else {
	cancel_create:
			TARGET_MENU = 0xE;
		}
		TARGET_MENU = getMenuOnCardChange(0);
		break;

	case 0x10:
		if ((MAIN_MENU_TICKS >= 3) && MemCardSync(1, &command, &result)) {
			SAVE_FILE_NAME[0xF] = HEX_DIGITS[MEMORY_CARD_SLOT];
			status = MemCardCreateFile(MEMORY_CARD_ID, SAVE_FILE_NAME, 1);
			switch (status) {
			case 0:
				TARGET_MENU = 0x11;
				break;
			case 7:
				setMemoryCardReadError(7, 0xA);
				break;
			case 4:
				TARGET_MENU = 1;
				MEMORY_CARD_RETURN_MENU = 0xC;
				break;
			default:
				setMemoryCardReadError(status, 0);
				break;
			}
		}
		break;

	case 0x11:
		MemCardSync(0, &command, &result);
		SAVE_FILE.id[0] = 0x53;
		SAVE_FILE.id[1] = 0x43;
		SAVE_FILE.iconDisplayFlag = 0x13;
		SAVE_FILE.blockNumber = 1;
		memcpy(SAVE_FILE.iconClut, SAVE_ICON_CLUT, 0x20);
		memcpy(SAVE_FILE.iconFrames[0], SAVE_ICON_FRAMES[0], 0x80);
#if VERSION_IS(EU)
		memcpy(SAVE_FILE.iconFrames[1], SAVE_ICON_FRAMES[0], 0x80);
		memcpy(SAVE_FILE.iconFrames[2], SAVE_ICON_FRAMES[0], 0x80);
#else
		memcpy(SAVE_FILE.iconFrames[1], SAVE_ICON_FRAMES[1], 0x80);
		memcpy(SAVE_FILE.iconFrames[2], SAVE_ICON_FRAMES[2], 0x80);
#endif
		initializeDefaultSavegame();
#if VERSION_IS(EU)
		strncpy(SAVE_FILE.title, MAIN_D_8013392C, 0x40);
		_strncpy(SAVE_FILE.title + 2, SAVEGAME_ID_LABEL[MEMORY_CARD_SLOT + 1], 4);
		_strncpy(SAVE_FILE.title + 8, SAVE_FILE.saves[0].playerName, 0xc);
		_strncpy(SAVE_FILE.title + 0x16, DIGIMON_NAME(SAVE_FILE.saves[0].partnerType), 0x2a);
#elif !VERSION_IS(US)
		strncpy(SAVE_FILE.title, MAIN_D_8013392C, 0x40);
		_strncpy(SAVE_FILE.title + 8, SAVEGAME_ID_LABEL[MEMORY_CARD_SLOT + 1], 4);
		_strncpy(SAVE_FILE.title + 0xe, SAVE_FILE.saves[0].playerName, 0xc);
		_strncpy(SAVE_FILE.title + 0x1c, DIGIMON_NAME(SAVE_FILE.saves[0].partnerType), 0x10);
		_strncpy(SAVE_FILE.title + 0x2e,
			 MAP_NAME_PTR[MAP_ENTRIES[SAVE_FILE.saves[0].currentScreen].loadingName], 0x12);
#else
		asciiToShiftJIS((uint8_t *)MAIN_D_8013392C,
				(uint16_t *)SAVE_FILE.title);
		_strncpy(SAVE_FILE.title + 8,
			 SAVEGAME_ID_LABEL[MEMORY_CARD_SLOT + 1], 4);
		_strncpy(SAVE_FILE.title + 0xe,
			 SAVE_FILE.saves[0].playerName, 0xD);
		_strncpy(SAVE_FILE.title + 0x1c,
			 DIGIMON_NAME(SAVE_FILE.saves[0].partnerType),
			 0x11);
		memcpy(SAVE_FILE.reserved, SAVE_TITLE_RESERVED, 0x1C);
#endif
		SAVE_FILE.saves[0].checksum = createSavegameChecksum(0);
		SAVE_FILE.saves[1] =
			SAVE_FILE.saves[0];
		status = MemCardWriteFile(MEMORY_CARD_ID, SAVE_FILE_NAME,
					 (void *)&SAVE_FILE, 0, 0x2000);
		if (status != 1) {
			setMemoryCardReadError(0, 0);
		}
		MemCardSync(0, &command, &result);
		if (result == 0) {
			TARGET_MENU = 9;
			loadSavegame(&SAVE_FILE.saves[0]);
			MAIN_STATE = 0;
		} else {
			setMemoryCardReadError(status, 0);
		}
		break;

	case 0x13:
		view = MENU_VIEWS[CURRENT_MENU];
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[view];
		input = tickMenuInput(cursor, 1);
		if (input != 2) {
			if (input == 1) {
				if (cursor->pos == 0) {
					TARGET_MENU = 9;
					MEMORY_CARD_ID = -1;
					MEMORY_CARD_SLOT = -1;
					initializeDefaultSavegame();
					loadSavegame(&SAVE_FILE.saves[0]);
				} else {
					TARGET_MENU = 0xA;
				}
			}
		} else {
			TARGET_MENU = 0xA;
		}
		break;

	case 0x14:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[2];
		input = tickMenuInput(cursor, 1);
		switch (input) {
		case 1:
			switch (cursor->pos) {
			case 0:
				if (CONNECTED_CARDS & 1) {
					MEMORY_CARD_ID = 0;
					TARGET_MENU = 0x15;
					MEMORY_CARD_OPERATION = 0;
				}
				break;
			case 1:
				if (CONNECTED_CARDS & 0x10) {
					MEMORY_CARD_ID = 0x10;
					TARGET_MENU = 0x15;
					MEMORY_CARD_OPERATION = 0;
				}
				break;
			}
			break;
		case 2:
			TARGET_MENU = 0;
			break;
		}
		break;

	case 0x15:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			if (MemCardAccept(MEMORY_CARD_ID) == 0) {
				setMemoryCardReadError(0, 0);
				break;
			}
			MemCardSync(0, &command, &result);
			result = isMemcardUnformatted(MEMORY_CARD_ID, result);
			if (result == 0) {
				TARGET_MENU = 0x16;
			} else {
				setMemoryCardReadError(result, 0);
			}
		}
		break;

	case 0x16:
		TARGET_MENU = 0x17;
		SAVE_FILE_NAME[0xF] = 0x3F;
		loadSaveSlotData(MEMORY_CARD_ID, SAVE_FILE_NAME,
				 SAVEGAME_SLOT_INFO, 0);
		break;

	case 0x17:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[7];
		input = tickSelectSlotInput(cursor, 1);
		slot = cursor->pos + cursor->scroll;
		if (input != 2) {
			if (input == 1) {
				if (SAVEGAME_SLOT_INFO[slot].valid == 1) {
					TARGET_MENU = 0x18;
					MEMORY_CARD_SLOT = slot;
				}
			}
		} else {
			TARGET_MENU = 0x14;
		}
		TARGET_MENU = getMenuOnCardChange(0);
		break;

	case 0x18:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[8];
		input = tickMenuInput(cursor, 1);
		if (input != 2) {
			if (input == 1) {
				if (cursor->pos == 0) {
					TARGET_MENU = 0x19;
					MEMORY_CARD_OPERATION = 2;
				} else {
					goto cancel_load;
				}
			}
		} else {
	cancel_load:
			TARGET_MENU = 0x17;
		}
		TARGET_MENU = getMenuOnCardChange(0);
		break;

	case 0x19:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			SAVE_FILE_NAME[0xF] = HEX_DIGITS[MEMORY_CARD_SLOT];
			status = MemCardReadFile(MEMORY_CARD_ID, SAVE_FILE_NAME,
					    (void *)&SAVE_FILE, 0, 0x2000);
			if (status != 1) {
				setMemoryCardReadError(0, 0);
			}
			MemCardSync(0, &command, &result);
			if (result == 0) {
				if (SAVE_FILE.saves[0].checksum ==
				    createSavegameChecksum(0)) {
					loadSavegame(&SAVE_FILE.saves[0]);
					TARGET_MENU = 9;
					break;
				}
				if (SAVE_FILE.saves[1].checksum ==
				    createSavegameChecksum(1)) {
					SAVE_FILE.saves[0] =
						SAVE_FILE.saves[1];
					loadSavegame(&SAVE_FILE.saves[0]);
					TARGET_MENU = 9;
					break;
				}
				result = 2;
			}
			setMemoryCardReadError(result, 0);
		}
		break;

	case 0x1E:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[4];
		input = tickMenuInput(cursor, 1);
		if (input != 2) {
			if (input == 1) {
				switch (cursor->pos) {
				case 0:
					if (CONNECTED_CARDS & 1) {
						MEMORY_CARD_ID = 0;
						TARGET_MENU = 0x1F;
						MEMORY_CARD_OPERATION = 0;
					}
					break;
				case 1:
					if (CONNECTED_CARDS & 0x10) {
						MEMORY_CARD_ID = 0x10;
						TARGET_MENU = 0x1F;
						MEMORY_CARD_OPERATION = 0;
					}
					break;
				}
			}
		} else {
			TARGET_MENU = 0;
		}
		if (CONNECTED_CARDS == 0) {
			TARGET_MENU = 0;
		}
		break;

	case 0x1F:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			if (MemCardAccept(MEMORY_CARD_ID) == 0) {
				setMemoryCardReadError(0, 0);
				break;
			}
			MemCardSync(0, &command, &result);
			result = isMemcardUnformatted(MEMORY_CARD_ID, result);
			if (result == 0) {
				TARGET_MENU = 0x20;
			} else {
				setMemoryCardReadError(result, 0);
			}
		}
		break;

	case 0x20:
		TARGET_MENU = 0x21;
		SAVE_FILE_NAME[0xF] = 0x3F;
		loadSaveSlotData(MEMORY_CARD_ID, SAVE_FILE_NAME,
				 SAVEGAME_SLOT_INFO, 0);
		break;

	case 0x21:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[7];
		input = tickSelectSlotInput(cursor, 1);
		slot = cursor->pos + cursor->scroll;
		if (input != 2) {
			if (input == 1) {
				if (SAVEGAME_SLOT_INFO[slot].valid == 1) {
					TARGET_MENU = 0x22;
					MEMORY_CARD_SLOT = slot;
				}
			}
		} else {
			TARGET_MENU = 0x1E;
		}
		TARGET_MENU = getMenuOnCardChange(0);
		break;

	case 0x22:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[8];
		input = tickMenuInput(cursor, 1);
		if (input != 2) {
			if (input == 1) {
				if (cursor->pos == 0) {
					TARGET_MENU = 0x23;
					MEMORY_CARD_OPERATION = 4;
				} else {
					goto cancel_delete;
				}
			}
		} else {
	cancel_delete:
			TARGET_MENU = 0x21;
		}
		TARGET_MENU = getMenuOnCardChange(0);
		break;

	case 0x23:
		if ((MAIN_MENU_TICKS >= 3) && MemCardSync(1, &command, &result)) {
			SAVE_FILE_NAME[0xF] = HEX_DIGITS[MEMORY_CARD_SLOT];
			status = MemCardDeleteFile(MEMORY_CARD_ID, SAVE_FILE_NAME);
			if (status == 0) {
				TARGET_MENU = 0x1F;
				MEMORY_CARD_OPERATION = 0;
			} else {
				setMemoryCardReadError(status, 0);
			}
		}
		break;

	case 0x28:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[11];
		input = tickMenuInput(cursor, 1);
		switch (input) {
		case 1:
			if (cursor->pos == 0) {
				TARGET_MENU = 9;
				MAIN_STATE = 0;
			} else {
				SAVE_RETRY_RETURN_MENU = 0x28;
				TARGET_MENU = 0x29;
				MEMORY_CARD_OPERATION = 0;
			}
			break;
		case 2:
			break;
		}
		break;

	case 0x29:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			if (MemCardExist(MEMORY_CARD_ID) == 0) {
				setMemoryCardReadError(0, 0x28);
				break;
			}
			MemCardSync(0, &command, &result);
			switch (result) {
			case 0:
				TARGET_MENU = 0x2A;
				break;
			case 3:
				MEMORY_CARD_ERROR = -1;
				TARGET_MENU = 0x30;
				break;
			case 1:
			case 2:
			case 4:
				MEMORY_CARD_ERROR = result;
				TARGET_MENU = 0x30;
				break;
			default:
				setMemoryCardReadError(0, 0x28);
				break;
			}
		}
		break;

	case 0x2A:
		MemCardSync(0, &command, &result);
		SAVE_FILE_NAME[0xF] = HEX_DIGITS[MEMORY_CARD_SLOT];
		status = MemCardGetDirentry(MEMORY_CARD_ID, SAVE_FILE_NAME,
					MEMCARD_DIRENTRIES, &count, 0, 1);
		MEMORY_CARD_ERROR = -1;
		switch (status) {
		case -1:
			setMemoryCardReadError(0, 0x28);
			break;
		case 0:
			if (count == 0) {
				MEMORY_CARD_ERROR = 5;
				TARGET_MENU = 0x30;
			} else {
				TARGET_MENU = 0x2B;
				MEMORY_CARD_OPERATION = 1;
			}
			break;
		default:
			MEMORY_CARD_ERROR = status;
			TARGET_MENU = 0x30;
			break;
		}
		break;

	case 0x2B:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			SAVE_FILE.id[0] = 0x53;
			SAVE_FILE.id[1] = 0x43;
			SAVE_FILE.iconDisplayFlag = 0x13;
			SAVE_FILE.blockNumber = 1;
			memcpy(SAVE_FILE.iconClut, SAVE_ICON_CLUT, 0x20);
			memcpy(SAVE_FILE.iconFrames[0], SAVE_ICON_FRAMES[0], 0x80);
#if VERSION_IS(EU)
			memcpy(SAVE_FILE.iconFrames[1], SAVE_ICON_FRAMES[0], 0x80);
			memcpy(SAVE_FILE.iconFrames[2], SAVE_ICON_FRAMES[0], 0x80);
#else
			memcpy(SAVE_FILE.iconFrames[1], SAVE_ICON_FRAMES[1], 0x80);
			memcpy(SAVE_FILE.iconFrames[2], SAVE_ICON_FRAMES[2], 0x80);
#endif
			writeSavegame(&SAVE_FILE.saves[0]);
#if VERSION_IS(EU)
			strncpy(SAVE_FILE.title, MAIN_D_8013392C, 0x40);
			_strncpy(SAVE_FILE.title + 2, SAVEGAME_ID_LABEL[MEMORY_CARD_SLOT + 1], 4);
			_strncpy(SAVE_FILE.title + 8, SAVE_FILE.saves[0].playerName, 0xc);
			_strncpy(SAVE_FILE.title + 0x16, DIGIMON_NAME(SAVE_FILE.saves[0].partnerType), 0x2a);
#elif !VERSION_IS(US)
			strncpy(SAVE_FILE.title, MAIN_D_8013392C, 0x40);
			_strncpy(SAVE_FILE.title + 8, SAVEGAME_ID_LABEL[MEMORY_CARD_SLOT + 1], 4);
			_strncpy(SAVE_FILE.title + 0xe, SAVE_FILE.saves[0].playerName, 0xc);
			_strncpy(SAVE_FILE.title + 0x1c, DIGIMON_NAME(SAVE_FILE.saves[0].partnerType), 0x10);
			_strncpy(SAVE_FILE.title + 0x2e,
				 MAP_NAME_PTR[MAP_ENTRIES[SAVE_FILE.saves[0].currentScreen].loadingName], 0x12);
#else
			asciiToShiftJIS((uint8_t *)MAIN_D_8013392C,
				(uint16_t *)SAVE_FILE.title);
			_strncpy(SAVE_FILE.title + 8,
				 SAVEGAME_ID_LABEL[MEMORY_CARD_SLOT + 1], 4);
			_strncpy(SAVE_FILE.title + 0xe,
				 SAVE_FILE.saves[0].playerName, 0xD);
			_strncpy(SAVE_FILE.title + 0x1c,
				 DIGIMON_NAME(SAVE_FILE.saves[0].partnerType),
				 0x11);
			memcpy(SAVE_FILE.reserved, SAVE_TITLE_RESERVED, 0x1C);
#endif
			SAVE_FILE.saves[0].checksum = createSavegameChecksum(0);
			SAVE_FILE.saves[1] =
				SAVE_FILE.saves[0];
			status = MemCardWriteFile(MEMORY_CARD_ID, SAVE_FILE_NAME,
						 (void *)&SAVE_FILE, 0, 0x2000);
			if (status != 1) {
				setMemoryCardReadError(0, 0x28);
			}
			MemCardSync(0, &command, &result);
			if (result == 0) {
				TARGET_MENU = 9;
			} else {
				setMemoryCardReadError(status, 0x28);
			}
		}
		break;

	case 0x30:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[12];
		input = tickMenuInput(cursor, 1);
		switch (input) {
		case 1:
			if (cursor->pos == 0) {
				if (CONNECTED_CARDS & (MEMORY_CARD_ID == 0 ? 1 : 0x10)) {
					TARGET_MENU = 0x31;
					MEMORY_CARD_OPERATION = 0;
				}
			} else {
				TARGET_MENU = 0x28;
			}
			break;
		case 2:
			break;
		}
		break;

	case 0x31:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			if (MemCardAccept(MEMORY_CARD_ID) == 0) {
				setMemoryCardReadError(0, 0x28);
				break;
			}
			MemCardSync(0, &command, &result);
			result = isMemcardUnformatted(MEMORY_CARD_ID, result);
			switch (result) {
			case 0:
				CONNECTED_CARDS |= MEMORY_CARD_ID == 0x10 ? 0x10 : 1;
				TARGET_MENU = 0x2A;
				break;
			case 1:
				CONNECTED_CARDS &= MEMORY_CARD_ID == 0x10 ? -0x11 : -2;
				MEMORY_CARD_ERROR = 1;
				TARGET_MENU = 0x30;
				break;
			default:
				MEMORY_CARD_ERROR = result;
				TARGET_MENU = 0x30;
				break;
			}
		}
		break;

	case 0x32:
		if (VS_PLAYER_INDEX == 0) {
			MEMORY_CARD_ID = 0;
		} else {
			MEMORY_CARD_ID = 0x10;
		}
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[13];
		input = tickMenuInput(cursor,
				      VS_PLAYER_INDEX == 0 ? 1 : 2);
		switch (input) {
		case 1:
			if (cursor->pos == 0) {
				if (CONNECTED_CARDS & (MEMORY_CARD_ID == 0 ? 1 : 0x10)) {
					TARGET_MENU = 0x33;
					MEMORY_CARD_OPERATION = 0;
				}
			} else {
				TARGET_MENU = VS_RETURN_MENU;
			}
			break;
		case 2:
			TARGET_MENU = VS_RETURN_MENU;
			break;
		}
		break;

	case 0x33:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			if (MemCardAccept(MEMORY_CARD_ID) == 0) {
				setMemoryCardReadError(0, 0x32);
				break;
			}
			MemCardSync(0, &command, &result);
			result = isMemcardUnformatted(MEMORY_CARD_ID, result);
			if (result == 0) {
				TARGET_MENU = 0x34;
			} else {
				setMemoryCardReadError(result, 0x32);
				break;
			}
			SAVE_FILE_NAME[0xF] = 0x3F;
			if (loadSaveSlotData(MEMORY_CARD_ID, SAVE_FILE_NAME,
					     SAVEGAME_SLOT_INFO,
					     0x32) != 0) {
				TARGET_MENU = 0x34;
			}
		}
		break;

	case 0x34:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[7];
		input = tickSelectSlotInput(cursor,
					    VS_PLAYER_INDEX == 0 ? 1 : 2);
		slot = cursor->pos + cursor->scroll;
		if (input != 2) {
			if (input == 1) {
				if (SAVEGAME_SLOT_INFO[slot].valid == 1) {
					TARGET_MENU = 0x35;
					MEMORY_CARD_SLOT = slot;
				}
			}
		} else {
			TARGET_MENU = 0x32;
		}
		TARGET_MENU = getMenuOnCardChange(0x32);
		break;

	case 0x35:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[8];
		input = tickMenuInput(cursor,
				      VS_PLAYER_INDEX == 0 ? 1 : 2);
		if (input != 2) {
			if (input == 1) {
				if (cursor->pos == 0) {
					TARGET_MENU = 0x36;
					MEMORY_CARD_OPERATION = 2;
				} else {
					goto cancel_vs_load;
				}
			}
		} else {
	cancel_vs_load:
			TARGET_MENU = 0x34;
		}
		TARGET_MENU = getMenuOnCardChange(0x32);
		break;

	case 0x36:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			SAVE_FILE_NAME[0xF] = HEX_DIGITS[MEMORY_CARD_SLOT];
			status = MemCardReadFile(MEMORY_CARD_ID, SAVE_FILE_NAME,
					    (void *)&SAVE_FILE, 0, 0x2000);
			if (status != 1) {
				setMemoryCardReadError(0, 0x32);
			}
			MemCardSync(0, &command, &result);
			if (result == 0) {
				if (SAVE_FILE.saves[0].checksum ==
				    createSavegameChecksum(0)) {
					memcpy(&VS__REGISTERED_DIGIMON_BUFFER[VS_PLAYER_INDEX * 0x28],
					       SAVE_FILE.saves[0].battleRegistrationData, 0xA00);
				} else if (SAVE_FILE.saves[1].checksum ==
					   createSavegameChecksum(1)) {
					memcpy(&VS__REGISTERED_DIGIMON_BUFFER[VS_PLAYER_INDEX * 0x28],
					       SAVE_FILE.saves[1].battleRegistrationData, 0xA00);
				} else {
					setMemoryCardReadError(2, 0x32);
					break;
				}
				if (countRegisteredDigimon(&VS__REGISTERED_DIGIMON_BUFFER[VS_PLAYER_INDEX * 0x28])) {
					TARGET_MENU = 0x37;
				} else {
					setMemoryCardReadError(8, 0x32);
				}
			} else {
				setMemoryCardReadError(result, 0x32);
			}
		}
		break;

	case 0x37:
		if (VS_PLAYER_INDEX == 0) {
			VS_PLAYER_INDEX = 1;
			TARGET_MENU = 0x32;
		} else {
			TARGET_MENU = 0x38;
		}
		break;

	case 0x38:
		if (MAIN_MENU_TICKS >= 3) {
			removeObject(0x1388, 0);
			removeObject(0xFA3, 0);
			loadDynamicLibrary(VS_REL, &loadComplete, 0, 0, 0);
			VS__initialize(VS__REGISTERED_DIGIMON_BUFFER,
					     &VS__REGISTERED_DIGIMON_BUFFER[0x28]);
			addObject(0xFA3, 0, NULL, (RenderFunction)renderMainMenuBackground);
			addObject(0x1388, 0, (TickFunction)tickMainMenu,
				  (RenderFunction)renderMainMenu);
			TARGET_MENU = VS_RETURN_MENU;
		}
		break;

	case 0x3C:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[14];
		input = tickMenuInput(cursor, 1);
		switch (input) {
		case 1:
			if (cursor->pos == 0) {
				TARGET_MENU = 0x3D;
				SAVE_FILE.saves[0] =
					SAVE_FILE.saves[1];
				SAVE_RETRY_RETURN_MENU = 0x3C;
				break;
			}
			/* fall through */
		case 2:
			TARGET_MENU = 0x45;
			break;
		}
		break;

	case 0x3D:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[15];
		oldScroll = cursor->scroll;
		input = tickMenuInput(cursor, 1);
		if (oldScroll != cursor->scroll) {
			drawRegisteredDigimonSlots(cursor->scroll);
		}
		if (input != 2) {
			if (input == 1) {
				BATTLE_REGISTRATION_SLOT = cursor->scroll + cursor->pos;
				TARGET_MENU = 0x3E;
			}
		} else {
			TARGET_MENU = 0x3C;
		}
		break;

	case 0x3E:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[16];
		input = tickMenuInput(cursor, 1);
		switch (input) {
		case 1:
			if (cursor->pos == 0) {
				TARGET_MENU = 0x3F;
				registerBattleData();
				MEMORY_CARD_OPERATION = 0;
			} else {
				TARGET_MENU = 0x3D;
			}
			break;
		case 2:
			TARGET_MENU = 0x3C;
			break;
		}
		break;

	case 0x3F:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			if (MemCardExist(MEMORY_CARD_ID) == 0) {
				setMemoryCardReadError(0, SAVE_RETRY_RETURN_MENU);
				break;
			}
			MemCardSync(0, &command, &result);
			switch (result) {
			case 0:
				TARGET_MENU = 0x40;
				break;
			case 3:
				MEMORY_CARD_ERROR = -1;
				TARGET_MENU = 0x42;
				break;
			case 1:
			case 2:
			case 4:
				MEMORY_CARD_ERROR = result;
				TARGET_MENU = 0x42;
				break;
			default:
				setMemoryCardReadError(0, SAVE_RETRY_RETURN_MENU);
				break;
			}
		}
		break;

	case 0x40:
		MemCardSync(0, &command, &result);
		SAVE_FILE_NAME[0xF] = HEX_DIGITS[MEMORY_CARD_SLOT];
		status = MemCardGetDirentry(MEMORY_CARD_ID, SAVE_FILE_NAME,
					MEMCARD_DIRENTRIES, &count, 0, 1);
		MEMORY_CARD_ERROR = -1;
		switch (status) {
		case -1:
			setMemoryCardReadError(0, SAVE_RETRY_RETURN_MENU);
			break;
		case 0:
			if (count == 0) {
				MEMORY_CARD_ERROR = 5;
				TARGET_MENU = 0x42;
			} else {
				TARGET_MENU = 0x41;
				MEMORY_CARD_OPERATION = 1;
			}
			break;
		default:
			MEMORY_CARD_ERROR = status;
			TARGET_MENU = 0x42;
			break;
		}
		break;

	case 0x41:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			SAVE_FILE.id[0] = 0x53;
			SAVE_FILE.id[1] = 0x43;
			SAVE_FILE.iconDisplayFlag = 0x13;
			SAVE_FILE.blockNumber = 1;
			memcpy(SAVE_FILE.iconClut, SAVE_ICON_CLUT, 0x20);
			memcpy(SAVE_FILE.iconFrames[0], SAVE_ICON_FRAMES[0], 0x80);
#if VERSION_IS(EU)
			memcpy(SAVE_FILE.iconFrames[1], SAVE_ICON_FRAMES[0], 0x80);
			memcpy(SAVE_FILE.iconFrames[2], SAVE_ICON_FRAMES[0], 0x80);
#else
			memcpy(SAVE_FILE.iconFrames[1], SAVE_ICON_FRAMES[1], 0x80);
			memcpy(SAVE_FILE.iconFrames[2], SAVE_ICON_FRAMES[2], 0x80);
#endif
			writeSavegame(&SAVE_FILE.saves[0]);
#if VERSION_IS(EU)
			strncpy(SAVE_FILE.title, MAIN_D_8013392C, 0x40);
			_strncpy(SAVE_FILE.title + 2, SAVEGAME_ID_LABEL[MEMORY_CARD_SLOT + 1], 4);
			_strncpy(SAVE_FILE.title + 8, SAVE_FILE.saves[0].playerName, 0xc);
			_strncpy(SAVE_FILE.title + 0x16, DIGIMON_NAME(SAVE_FILE.saves[0].partnerType), 0x2a);
#elif !VERSION_IS(US)
			strncpy(SAVE_FILE.title, MAIN_D_8013392C, 0x40);
			_strncpy(SAVE_FILE.title + 8, SAVEGAME_ID_LABEL[MEMORY_CARD_SLOT + 1], 4);
			_strncpy(SAVE_FILE.title + 0xe, SAVE_FILE.saves[0].playerName, 0xc);
			_strncpy(SAVE_FILE.title + 0x1c, DIGIMON_NAME(SAVE_FILE.saves[0].partnerType), 0x10);
			_strncpy(SAVE_FILE.title + 0x2e,
				 MAP_NAME_PTR[MAP_ENTRIES[SAVE_FILE.saves[0].currentScreen].loadingName], 0x12);
#else
			asciiToShiftJIS((uint8_t *)MAIN_D_8013392C,
				(uint16_t *)SAVE_FILE.title);
			_strncpy(SAVE_FILE.title + 8,
				 SAVEGAME_ID_LABEL[MEMORY_CARD_SLOT + 1], 4);
			_strncpy(SAVE_FILE.title + 0xe,
				 SAVE_FILE.saves[0].playerName, 0xD);
			_strncpy(SAVE_FILE.title + 0x1c,
				 DIGIMON_NAME(SAVE_FILE.saves[0].partnerType),
				 0x11);
			memcpy(SAVE_FILE.reserved, SAVE_TITLE_RESERVED, 0x1C);
#endif
			SAVE_FILE.saves[0].checksum = createSavegameChecksum(0);
			SAVE_FILE.saves[1] =
				SAVE_FILE.saves[0];
			status = MemCardWriteFile(MEMORY_CARD_ID, SAVE_FILE_NAME,
						 (void *)&SAVE_FILE, 0, 0x2000);
			if (status != 1) {
				setMemoryCardReadError(0, SAVE_RETRY_RETURN_MENU);
			}
			MemCardSync(0, &command, &result);
			if (result == 0) {
				TARGET_MENU = 9;
			} else {
				setMemoryCardReadError(status, SAVE_RETRY_RETURN_MENU);
			}
		}
		break;

	case 0x42:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[12];
		input = tickMenuInput(cursor, 1);
		switch (input) {
		case 1:
			if (cursor->pos == 0) {
				if (CONNECTED_CARDS & (MEMORY_CARD_ID == 0 ? 1 : 0x10)) {
					TARGET_MENU = 0x43;
					MEMORY_CARD_OPERATION = 0;
				}
			} else {
				TARGET_MENU = SAVE_RETRY_RETURN_MENU;
			}
			break;
		case 2:
			break;
		}
		break;

	case 0x43:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			if (MemCardAccept(MEMORY_CARD_ID) == 0) {
				setMemoryCardReadError(0, SAVE_RETRY_RETURN_MENU);
				break;
			}
			MemCardSync(0, &command, &result);
			result = isMemcardUnformatted(MEMORY_CARD_ID, result);
			switch (result) {
			case 0:
				CONNECTED_CARDS |= MEMORY_CARD_ID == 0x10 ? 0x10 : 1;
				TARGET_MENU = 0x40;
				break;
			case 1:
				CONNECTED_CARDS &= MEMORY_CARD_ID == 0x10 ? -0x11 : -2;
				MEMORY_CARD_ERROR = 1;
				TARGET_MENU = 0x42;
				break;
			default:
				MEMORY_CARD_ERROR = result;
				TARGET_MENU = 0x42;
				break;
			}
		}
		break;

	case 0x44:
		if ((CHANGED_INPUT == CANCEL_BUTTON) || (CHANGED_INPUT == CONFIRM_BUTTON)) {
			playSound(0, 3);
			TARGET_MENU = 9;
		}
		if (MAIN_MENU_TICKS >= 0x79) {
			TARGET_MENU = 9;
		}
		break;

	case 0x45:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[18];
		input = tickMenuInput(cursor, 1);
		switch (input) {
		case 1:
			if (cursor->pos == 0) {
				TARGET_MENU = 0x3F;
				SAVE_RETRY_RETURN_MENU = 0x45;
				MEMORY_CARD_OPERATION = 0;
			} else {
				TARGET_MENU = 9;
			}
			break;
		case 2:
			break;
		}
		break;

	case 0x46:
		if ((CHANGED_INPUT == CANCEL_BUTTON) || (CHANGED_INPUT == CONFIRM_BUTTON)) {
			playSound(0, 3);
			TARGET_MENU = 0x45;
		}
		break;
#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)

	case 0x47:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[20];
		if (SECRET_CODE != -1) {
			cursor->count = 4;
		} else {
			cursor->count = 3;
		}
		input = tickMenuInput(cursor, 1);
		switch (input) {
		case 1:
			switch (cursor->pos) {
			case 0:
				TARGET_MENU = 9;
				MEMORY_CARD_ID = -1;
				MEMORY_CARD_SLOT = -1;
				initializeDefaultSavegame();
				loadSavegame(&SAVE_FILE.saves[0]);
				break;
			case 1:
				VS_PLAYER_INDEX = 0;
				TARGET_MENU = 0x32;
				MAIN_MENU_ACTION = 5;
				break;
			case 2:
				TARGET_MENU = 0x48;
				MAIN_MENU_ACTION = 5;
				break;
			case 3:
				TARGET_MENU = 0x49;
				MEMORY_CARD_ID = 0;
				MAIN_MENU_ACTION = 7;
				break;
			}
			break;
		case 2:
			break;
		}
		break;

	case 0x48:
		if (MAIN_MENU_TICKS >= 3) {
			removeObject(0x1388, 0);
			removeObject(0xFA3, 0);
			loadDynamicLibrary(VS_REL, &demoLoadComplete, 0, 0, 0);
			VS_initializeTrialBattle(VS__REGISTERED_DIGIMON_BUFFER,
					  &VS__REGISTERED_DIGIMON_BUFFER[0x28]);
			addObject(0xFA3, 0, NULL, (RenderFunction)renderMainMenuBackground);
			addObject(0x1388, 0, (TickFunction)tickMainMenu,
				  (RenderFunction)renderMainMenu);
			TARGET_MENU = 0x47;
		}
		break;

	case 0x49:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[21];
		input = tickMenuInput(cursor, 1);
		switch (input) {
		case 1:
			switch (cursor->pos) {
			case 0:
				if (CONNECTED_CARDS & 1) {
					TARGET_MENU = 0x4A;
					MEMORY_CARD_OPERATION = 0;
				}
				break;
			case 1:
				TARGET_MENU = 0x47;
				break;
			}
			break;
		case 2:
			TARGET_MENU = 0x47;
			break;
		}
		break;

	case 0x4A:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			if (MemCardAccept(MEMORY_CARD_ID) == 0) {
				setMemoryCardReadError(0, 0x47);
				break;
			}
			MemCardSync(0, &command, &result);
			result = isMemcardUnformatted(MEMORY_CARD_ID, result);
			if (result == 0) {
				TARGET_MENU = 0x4B;
			} else {
				setMemoryCardReadError(result, 0x47);
			}
		}
		break;

	case 0x4B:
		TARGET_MENU = 0x4C;
		SAVE_FILE_NAME[0xF] = 0x3F;
		loadSaveSlotData(MEMORY_CARD_ID, SAVE_FILE_NAME,
				 SAVEGAME_SLOT_INFO, 0x47);
		break;

	case 0x4C:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[7];
		input = tickSelectSlotInput(cursor, 1);
		slot = cursor->pos + cursor->scroll;
		if (input != 2) {
			if (input == 1) {
				if (SAVEGAME_SLOT_INFO[slot].valid == 1) {
					TARGET_MENU = 0x4D;
					MEMORY_CARD_SLOT = slot;
				}
			}
		} else {
			TARGET_MENU = 0x49;
		}
		TARGET_MENU = getMenuOnCardChange(0x47);
		break;

	case 0x4D:
		cursor = (MenuCursor *)&MENU_HIGHLIGHTS[8];
		input = tickMenuInput(cursor, 1);
		if (input != 2) {
			if (input == 1) {
				if (cursor->pos == 0) {
					TARGET_MENU = 0x4E;
					MEMORY_CARD_OPERATION = 2;
				} else {
					goto cancel_add_item;
				}
			}
		} else {
	cancel_add_item:
			TARGET_MENU = 0x4C;
		}
		TARGET_MENU = getMenuOnCardChange(0x47);
		break;

	case 0x4E:
		if (MAIN_MENU_TICKS >= 3) {
			MemCardSync(0, &command, &result);
			SAVE_FILE_NAME[0xF] = HEX_DIGITS[MEMORY_CARD_SLOT];
			status = MemCardReadFile(MEMORY_CARD_ID, SAVE_FILE_NAME,
					    (void *)&SAVE_FILE, 0, 0x2000);
			if (status != 1) {
				setMemoryCardReadError(0, 0x47);
			}
			MemCardSync(0, &command, &result);
			if (result == 0) {
				if (SAVE_FILE.saves[0].checksum ==
				    createSavegameChecksum(0)) {
					loadSavegame(&SAVE_FILE.saves[0]);
					TARGET_MENU = 0x4F;
					MEMORY_CARD_OPERATION = 1;
					break;
				}
				if (SAVE_FILE.saves[1].checksum ==
				    createSavegameChecksum(1)) {
					SAVE_FILE.saves[0] =
						SAVE_FILE.saves[1];
					loadSavegame(&SAVE_FILE.saves[0]);
					TARGET_MENU = 0x4F;
					MEMORY_CARD_OPERATION = 1;
					break;
				}
				result = 2;
			}
			setMemoryCardReadError(result, 0x47);
		}
		break;

	case 0x4F:
		if (MAIN_MENU_TICKS >= 3) {
			inventory = &SAVE_FILE.saves[0].inventory;
			for (i = 0; i < inventory->size; i++) {
				if (inventory->types.array[i] == 0xff) {
					inventory->amounts.array[i] = 1;
					inventory->types.array[i] = SECRET_CODE + SECRET_CODE_ITEM;
					i = 0x63;
				}
				if ((SECRET_CODE + SECRET_CODE_ITEM) == inventory->types.array[i]) {
					i = 0x64;
				}
			}
			if (i < 0x64) {
				setMemoryCardReadError(9, 0x47);
				break;
			}
			if (i == 0x65) {
				setMemoryCardReadError(0xa, 0x47);
				break;
			}
			MemCardSync(0, &command, &result);
			SAVE_FILE.saves[0].checksum = createSavegameChecksum(0);
			SAVE_FILE.saves[1] = SAVE_FILE.saves[0];
			status = MemCardWriteFile(MEMORY_CARD_ID, SAVE_FILE_NAME,
						  (void *)&SAVE_FILE, 0, 0x2000);
			if (status != 1) {
				setMemoryCardReadError(0, 0x47);
			}
			MemCardSync(0, &command, &result);
			if (result == 0) {
				TARGET_MENU = 0x47;
				break;
			}
			setMemoryCardReadError(status, 0x47);
		}
		break;
#endif
	}
}

int32_t tickMenuInput(MenuCursor *cursor, int32_t which)
{
	int32_t input;
	int32_t lo;
	int32_t hi;

	lo = CHANGED_INPUT & 0xFFFF;
	hi = (uint32_t)(CHANGED_INPUT & 0xFFFF0000) >> 0x10;
	input = 0;
	if (which & 1) {
		input |= lo;
	}
	if (which & 2) {
		input |= hi;
	}
	if (input == 0x1000) {
		if (0 < cursor->pos) {
			playSound(0, 2);
			cursor->pos--;
		} else if (0 < cursor->scroll) {
			playSound(0, 2);
			cursor->scroll--;
		}
	}
	if (input == 0x4000) {
		if (cursor->pos < cursor->count - 1) {
			playSound(0, 2);
			cursor->pos++;
		} else if (cursor->scroll < cursor->max) {
			playSound(0, 2);
			cursor->scroll++;
		}
	}
	if (input == CANCEL_BUTTON) {
		playSound(0, 4);
		return 2;
	}
	if (input == CONFIRM_BUTTON) {
		playSound(0, 3);
		return 1;
	}
	return 0;
}

int32_t getMenuOnCardChange(int32_t menu)
{
	int32_t mask;
	int32_t mask2;

	if (MEMORY_CARD_ID == 0) {
		mask = 1;
	} else {
		mask = 0x10;
	}
	if (!(CONNECTED_CARDS & mask)) {
		return menu;
	}
	if (MEMORY_CARD_ID == 0) {
		mask2 = 1;
	} else {
		mask2 = 0x10;
	}
	if (NEW_CARDS & mask2) {
		return menu;
	}
	return TARGET_MENU;
}

void setMemoryCardReadError(int32_t id, int32_t returnMenu)
{
	TARGET_MENU = 3;
	MEMORY_CARD_ERROR = id;
	MEMORY_CARD_RETURN_MENU = returnMenu;
}

int32_t isMemcardUnformatted(int32_t chan, MemCardSyncWord mode)
{
	MemCardSyncWord cmd;
	MemCardSyncWord result;

	if ((mode == 0) || (mode == 3)) {
		return 0;
	}

	MemCardExist(chan);
	MemCardSync(0, &cmd, &result);
	if ((result == 0) && (mode == 4)) {
		result = 4;
	}

	return result;
}

int32_t getCardUsedBlockCount(int32_t channel, int32_t returnMenu)
{
	MemCardSyncWord cmd;
	MemCardSyncWord result;
	long fileCount;
	int32_t i;
	int32_t blocks;
	int32_t status;

	MemCardSync(0, &cmd, &result);
#if VERSION_IS(EU)
	status = MemCardGetDirentry(channel, PATH_WILDCARD, MEMCARD_DIRENTRIES, &fileCount, 0, 15);
#else
	status = MemCardGetDirentry(channel, MAIN_D_801346C0[1], MEMCARD_DIRENTRIES,
			&fileCount, 0, 15);
#endif
	switch (status) {
	case 0:
	case 3:
		break;
	case -1:
		setMemoryCardReadError(0, returnMenu);
		return -1;
	default:
		setMemoryCardReadError(status, returnMenu);
		return -1;
	}

	blocks = 0;
	for (i = 0; i < fileCount; i++) {
		blocks += (MEMCARD_DIRENTRIES[i].size + 0x1FFF) / 0x2000;
	}
	return blocks;
}

int32_t loadSaveSlotData(int32_t channel, char *filename, SaveSlotPreview *slots, int32_t unused)
{
	MemCardSyncWord cmd;
	MemCardSyncWord result;
	long fileCount;
	int32_t status;
	/* Retail reserves 0x200 bytes even though each header read requests 0x80. */
	uint8_t data[0x200];
	int32_t i;
	int32_t slot;

	/* The fourth caller argument is unused in retail. */
	MemCardSync(0, &cmd, &result);
	status = MemCardGetDirentry(channel, filename, MEMCARD_DIRENTRIES, &fileCount, 0, 15);
	switch (status) {
	case 0:
	case 3:
		break;
	case -1:
		setMemoryCardReadError(0, 0);
		return 0;
	default:
		setMemoryCardReadError(status, 0);
		return 0;
	}

	for (i = 0; i < 15; i++) {
		slots[i].valid = 0;
	}
	for (i = 0; i < fileCount; i++) {
		if ((MEMCARD_DIRENTRIES[i].name[15] >= '0') &&
				(MEMCARD_DIRENTRIES[i].name[15] < ':')) {
			slot = MEMCARD_DIRENTRIES[i].name[15] - '0';
		} else {
			slot = MEMCARD_DIRENTRIES[i].name[15] - '7';
		}
		status = MemCardReadFile(channel, MEMCARD_DIRENTRIES[i].name, (void *)data, 0, 0x80);
		if (status != 1) {
			setMemoryCardReadError(0, 0);
			return 0;
		}
		MemCardSync(0, &cmd, &result);
		if (result == 0) {
			slots[slot].valid = 1;
#if VERSION_IS(EU)
			strncpy(slots[slot].playerName, (char *)&data[0xc], 12);
			slots[slot].playerName[12] = '\0';
			strncpy(slots[slot].digimonName, (char *)&data[0x1a], 42);
			slots[slot].location[22] = '\0';
			slots[slot].location[0] = '\0';
#elif !VERSION_IS(US)
			strncpy(slots[slot].playerName, (char *)&data[0x12], 12);
			slots[slot].playerName[12] = '\0';
			strncpy(slots[slot].digimonName, (char *)&data[0x20], 16);
			slots[slot].digimonName[16] = '\0';
			strncpy(slots[slot].location, (char *)&data[0x32], 18);
			slots[slot].location[18] = '\0';
#else
			strncpy(slots[slot].playerName, (char *)&data[0x12], 13);
			slots[slot].playerName[13] = '\0';
			strncpy(slots[slot].digimonName, (char *)&data[0x20], 17);
			slots[slot].digimonName[17] = '\0';
			strncpy(slots[slot].location, (char *)&data[0x32], 24);
			slots[slot].location[23] = '\0';
#endif
		} else {
			setMemoryCardReadError(result, 0);
			return 0;
		}
	}
	return fileCount;
}

int32_t tickSelectSlotInput(MenuCursor *cursor, int32_t which)
{
	int32_t input;
	int32_t lo;
	int32_t hi;

	lo = CHANGED_INPUT & 0xFFFF;
	hi = (uint32_t)(CHANGED_INPUT & 0xFFFF0000) >> 0x10;
	input = 0;
	if (which & 1) {
		input |= lo;
	}
	if (which & 2) {
		input |= hi;
	}
	if (input == 0x1000) {
		if (0 < cursor->pos) {
			playSound(0, 2);
			cursor->pos--;
		} else if (0 < cursor->scroll) {
			playSound(0, 2);
			cursor->scroll--;
			drawSaveSlotText(cursor->scroll - 1, (cursor->scroll - 1) % 7);
		}
	}
	if (input == 0x4000) {
		if (cursor->pos < cursor->count - 1) {
			playSound(0, 2);
			cursor->pos++;
		} else if (cursor->scroll < 0xA) {
			playSound(0, 2);
			cursor->scroll++;
			drawSaveSlotText(cursor->scroll + 5, (cursor->scroll + 5) % 7);
		}
	}
	if (input == CANCEL_BUTTON) {
		playSound(0, 4);
		return 2;
	}
	if (input == CONFIRM_BUTTON) {
		playSound(0, 3);
		return 1;
	}
	return 0;
}

void initializeDefaultSavegame(void)
{
	InventoryTable defaultInventoryCounts;
	InventoryTable defaultInventoryItems;
	SavegamePayload *savegame;
	uint8_t *cursor;
	int32_t i;

	savegame = &SAVE_FILE.saves[0];
	cursor = (uint8_t *)savegame;
	for (i = 0; i < 0xF00; i++) {
		*cursor++ = 0;
	}
	savegame->mainState = 0;
	savegame->currentScreen = 0xCC;
	savegame->currentExit = 9;
	savegame->partnerType = 0x72;
	strcpy(savegame->playerName, MAIN_D_801346C4);
	strcpy(savegame->digimonName, MAIN_D_8013468C);

	for (i = 0; i < 100; i++) {
		savegame->worldPoop[i].map = 0xFF;
		savegame->worldPoop[i].x = 0xFF;
		savegame->worldPoop[i].y = 0xFF;
		savegame->worldPoop[i].size = 0;
	}
	savegame->money = 50000;
	savegame->tamerLevel = 0;
	savegame->raisedCount = 1;
	savegame->year = 0;
	savegame->day = 0;
	savegame->hour = 8;
	savegame->minute = 0;
	savegame->lastHandledFrame = savegame->currentFrame = 0x2580;
	savegame->partnerLives = 3;
	savegame->partnerPara.remainingLifetime = 0x120;
	savegame->partnerPara.age = 3;
	for (i = 0; i < 6; i++) {
		savegame->scriptState[i] = 0x3A + i;
	}

	defaultInventoryCounts = DEFAULT_INVENTORY_AMOUNTS;
	defaultInventoryItems = DEFAULT_INVENTORY_TYPES;
	for (i = 0; i < 30; i++) {
		savegame->inventory.types.array[i] = defaultInventoryItems.array[i];
		savegame->inventory.amounts.array[i] = defaultInventoryCounts.array[i];
		savegame->inventory.names.array[i] = i;
	}
	savegame->inventory.size = 30;
}

char *_strncpy(char *dst, char *src, int32_t n)
{
	int32_t i;

	i = 0;
	while (i < n) {
		if (*src == 0) {
			break;
		}
		*dst++ = *src++;
		i++;
	}

	return dst;
}

int32_t createByteSum(uint8_t *data, int32_t len)
{
	uint32_t sum;
	int32_t i;

	sum = 0;
	for (i = 0; i < len; i++) {
		sum += *data++;
	}
	return sum;
}

#if !VERSION_IS(US)
void appendWideDigit(int32_t digit, char **dst, int32_t *started)
{
	digit %= 10;
	if ((*started == 1) || (digit != 0)) {
		strncpy(*dst, SAVEGAME_ID_LABEL[digit] + 2, 2);
		*started = 1;
	} else {
		strncpy(*dst, SAVEGAME_ID_LABEL[0], 2);
	}
	*dst += 2;
}
#endif

int32_t createSavegameChecksum(int32_t slot)
{
	uint32_t first;
	uint32_t second;
	uint32_t sum;

	first = createByteSum((uint8_t *)&SAVE_FILE.saves[slot], 0x4E4);
	second = createByteSum((uint8_t *)SAVE_FILE.saves[slot].battleRegistrationData, 0xa00);
	sum = first + second;
	return sum;
}

void loadSavegame(SavegamePayload *savegame)
{
	int32_t i;

	SAVED_STATE.playerPos = savegame->tamerPosition;
	SAVED_STATE.money = savegame->money;
	SAVED_STATE.partnerPos = savegame->partnerPosition;
	SAVED_STATE.partnerType = savegame->partnerType;
	memcpy(&PARTNER_ENTITY.learnedMoves, savegame->partnerData, 0x10);
	CURRENT_FRAME = savegame->currentFrame;
	LAST_HANDLED_FRAME = savegame->lastHandledFrame;
	YEAR = savegame->year;
	DAY = savegame->day;
	HOUR = savegame->hour;
	MINUTE = savegame->minute;
	PLAYTIME_FRAMES = savegame->playtimeFrames;
	PLAYTIME_HOURS = savegame->playtimeHours;
	PLAYTIME_MINUTES = savegame->playtimeMinutes;
	PARTNER_ENTITY.digimonEntity.stats = savegame->stats;
	SAVED_STATE.partnerStats = savegame->stats;
	INVENTORY = savegame->inventory;
	memcpy(PARTNER_ENTITY.name, savegame->digimonName, 0x14);
	TAMER_ENTITY.tamerLevel = savegame->tamerLevel;
	TAMER_ENTITY.raisedCount = savegame->raisedCount;
	MONEY = savegame->money;
	for (i = 0; i < 8; i++) {
		SAVED_STATE.npcMaps[i] = savegame->npcMaps[i];
	}
	memcpy(DIGIMON_NAME(0), savegame->playerName, 0x14);
	memcpy(WORLD_POOP, savegame->worldPoop, 0x190);
	SAVED_STATE.partnerPara = savegame->partnerPara;
	SAVED_STATE.lives = savegame->partnerLives;
	SAVED_STATE.currentScreen = savegame->currentScreen;
	SAVED_STATE.previousScreen = savegame->previousScreen;
	SAVED_STATE.currentExit = savegame->currentExit;
	SAVED_STATE.previousExit = savegame->previousExit;
	MERIT = savegame->merit;
	BATTLES_STARTED = savegame->battlesStarted;
	BATTLES_FLED = savegame->battlesFled;
	TOURNAMENTS_WON = savegame->tournamentsWon;
	TOURNAMENT_WINS = savegame->tournamentWins;
	TOURNAMENTS_LOST = savegame->tournamentsLost;
	memcpy(SCRIPT_STATE_PTR, savegame->scriptState, 0x17C);
	MAIN_STATE = savegame->mainState;
	memcpy(SAVED_STATE.tamerWaypointX, savegame->tamerWaypointX, 0x1E);
	memcpy(SAVED_STATE.tamerWaypointY, savegame->tamerWaypointY, 0x1E);
	SAVED_STATE.tamerPreviousTileX = savegame->tamerPreviousTileX;
	SAVED_STATE.tamerPreviousTileY = savegame->tamerPreviousTileY;
	SAVED_STATE.tamerWaypointCurrent = savegame->tamerWaypointCurrent;
	SAVED_STATE.tamerWaypointCount = savegame->tamerWaypointCount;
	SAVED_STATE.tamerStartTileX = savegame->tamerStartTileX;
	SAVED_STATE.tamerStartTileY = savegame->tamerStartTileY;
	SAVED_STATE.tamerWaypointActive = savegame->tamerWaypointActive;
}

void writeSavegame(SavegamePayload *savegame)
{
	int32_t restorePStat;
	int32_t i;

#if VERSION_EQUAL_OR_NEWER(US)
	recalculatePPandArena();
#endif
	restorePStat = 0;
	if (readPStat(0) == 3) {
		writePStat(0, 0);
		restorePStat = 1;
	}
	savegame->tamerPosition = TAMER_ENTITY.entity.posData->location;
	savegame->partnerPosition = PARTNER_ENTITY.digimonEntity.entity.posData->location;
	savegame->partnerType = PARTNER_ENTITY.digimonEntity.entity.type;
	savegame->money = MONEY;
	memcpy(savegame->partnerData, &PARTNER_ENTITY.learnedMoves, 0x10);
	savegame->hour = HOUR;
	savegame->minute = MINUTE;
	savegame->currentFrame = CURRENT_FRAME;
	savegame->lastHandledFrame = LAST_HANDLED_FRAME;
	savegame->day = DAY;
	savegame->year = YEAR;
	savegame->playtimeFrames = PLAYTIME_FRAMES;
	savegame->playtimeHours = PLAYTIME_HOURS;
	savegame->playtimeMinutes = PLAYTIME_MINUTES;
	savegame->inventory = INVENTORY;
	savegame->stats = PARTNER_ENTITY.digimonEntity.stats;
	savegame->partnerLives = PARTNER_ENTITY.lives;
	memcpy(savegame->digimonName, PARTNER_ENTITY.name, 0x14);
	savegame->tamerLevel = TAMER_ENTITY.tamerLevel;
	savegame->raisedCount = TAMER_ENTITY.raisedCount;
	for (i = 0; i < 8; i++) {
		savegame->npcMaps[i] = NPC_ENTITIES[i].digimonEntity.entity.isOnMap;
	}
	memcpy(savegame->playerName, DIGIMON_NAME(0), 0x14);
	memcpy(savegame->worldPoop, WORLD_POOP, 0x190);
	savegame->partnerPara = PARTNER_PARA;
	savegame->currentScreen = CURRENT_SCREEN;
	savegame->previousScreen = PREVIOUS_SCREEN;
	savegame->currentExit = CURRENT_EXIT;
	savegame->previousExit = PREVIOUS_EXIT;
	savegame->merit = MERIT;
	savegame->battlesStarted = BATTLES_STARTED;
	savegame->battlesFled = BATTLES_FLED;
	savegame->tournamentsWon = TOURNAMENTS_WON;
	savegame->tournamentWins = TOURNAMENT_WINS;
	savegame->tournamentsLost = TOURNAMENTS_LOST;
	memcpy(savegame->scriptState, SCRIPT_STATE_PTR, 0x17C);
	memcpy(savegame->tamerWaypointX, TAMER_WAYPOINT_X, 0x1E);
	memcpy(savegame->tamerWaypointY, TAMER_WAYPOINT_Y, 0x1E);
	savegame->tamerPreviousTileX = TAMER_PREVIOUS_TILE_X;
	savegame->tamerPreviousTileY = TAMER_PREVIOUS_TILE_Y;
	savegame->tamerWaypointCurrent = TAMER_WAYPOINT_CURRENT;
	savegame->tamerWaypointCount = TAMER_WAYPOINT_COUNT;
	savegame->tamerStartTileX = TAMER_START_TILE_X;
	savegame->tamerStartTileY = TAMER_START_TILE_Y;
	savegame->tamerWaypointActive = TAMER_WAYPOINT_ACTIVE;
	savegame->mainState = MAIN_STATE;
	if (restorePStat == 1) {
		writePStat(0, 3);
	}
}

int32_t countRegisteredDigimon(RegisteredDigimon *p)
{
	int32_t i;
	int32_t count;

	count = 0;
	i = 0;
	while (i < 0x28) {
		if (p->digimonId != 0) {
			count++;
		}
		p++;
		i++;
	}
	return count;
}

void renderMainMenu(void)
{
	int32_t menu;

	if (CURRENT_MENU >= 0) {
		menu = MENU_VIEWS[CURRENT_MENU];
		if (menu != -1) {
			renderMenuSelector(&MENU_HIGHLIGHTS[menu]);
		}
		switch (menu) {
		case 0:
			renderInitialMenu();
			break;
		case 1:
			renderSelectNewGameCard();
			break;
		case 2:
		case 4:
			renderSelectCardSlot();
			break;
		case 5:
			renderCheckingCard();
			break;
		case 6:
			renderConfirmNoCard();
			break;
		case 7:
			renderSelectSlot();
			break;
		case 8:
			renderConfirmSlotSelection();
			break;
		case 9:
			renderFormatMemoryCard();
			break;
		case 10:
			renderMemoryCardReadError();
			break;
		case 11:
			renderSleepMenu();
			break;
		case 12:
			renderConfirmOverwrite();
			break;
		case 13:
			renderConfirmVSSlot();
			break;
		case 14:
			renderRegisterDigimon();
			break;
		case 15:
			renderSelectRegisterSlot();
			break;
		case 16:
			renderConfirmRegister();
			break;
		case 17:
			renderCantRegister();
			break;
		case 18:
			renderDoYouWantToSave();
			break;
		case 19:
			renderCantRegisterBaby();
			break;
#if VERSION_IS(JP_TRIAL) || VERSION_IS(JP_BOMBOM)
		case 20:
			renderBattleModeMenu();
			break;
		case 21:
			renderInsertGameCardPrompt();
			break;
#endif
		}
	}
}

void registerBattleData(void)
{
	RegisteredDigimon *rec;

	rec = &SAVE_FILE.saves[0].battleRegistrationData[BATTLE_REGISTRATION_SLOT];
	rec->hp = PARTNER_ENTITY.digimonEntity.stats.base.hp;
	rec->mp = PARTNER_ENTITY.digimonEntity.stats.base.mp;
	rec->offense = PARTNER_ENTITY.digimonEntity.stats.base.off;
	rec->defense = PARTNER_ENTITY.digimonEntity.stats.base.def;
	rec->speed = PARTNER_ENTITY.digimonEntity.stats.base.speed;
	rec->brains = PARTNER_ENTITY.digimonEntity.stats.base.brain;
	rec->discipline = PARTNER_PARA.discipline;
	rec->digimonId = PARTNER_ENTITY.digimonEntity.entity.type;
	memcpy(rec->name, PARTNER_ENTITY.name, 0xe);
	memcpy(rec->moves, PARTNER_ENTITY.digimonEntity.stats.base.moves, 3);
}

void renderMenuSelector(MenuHighlight *b)
{
	LINE_F3 *prim;
	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;

	x = b->x - 1;
	y = (b->y + (b->rowHeight * b->pos)) - 2;
	w = b->w + 1;
	h = b->h + 2;
	x -= 0xA0;
	y -= 0x78;
	prim = (LINE_F3 *)GsGetWorkBase();
	SetLineF3(prim);
	setRGB0(prim, 0xA0, 0xA0, 0);
	setXY3(prim, x, y, x + w, y, x + w, y + h);
	AddPrim(ACTIVE_ORDERING_TABLE->org, prim++);
	SetLineF3(prim);
	setRGB0(prim, 0xA0, 0xA0, 0);
	setXY3(prim, x + w, y + h, x, y + h, x, y);
	AddPrim(ACTIVE_ORDERING_TABLE->org, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void openSaveMachine(void)
{
	CHECKED_MEMORY_CARD = 0x10;
	CURRENT_MENU = -2;
	MAIN_STATE = 1;
	TARGET_MENU = 0x3C;
	if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level < 3) {
		TARGET_MENU = 0x46;
	}

	if ((MEMORY_CARD_ID == -1) || (MEMORY_CARD_SLOT == -1)) {
		TARGET_MENU = 0x44;
	}
}

int32_t tickSaveMachine(void)
{
	tickMainMenu();
	renderMainMenu();
	if (CURRENT_MENU == -1) {
		return 1;
	}

	return 0;
}

void awardMachinedramonData(void)
{
	RegisteredDigimon *rec;

	rec = &SAVE_FILE.saves[0].battleRegistrationData[39];
	rec->hp = PARTNER_ENTITY.digimonEntity.stats.base.hp;
	rec->mp = PARTNER_ENTITY.digimonEntity.stats.base.mp;
	rec->offense = PARTNER_ENTITY.digimonEntity.stats.base.off;
	rec->defense = PARTNER_ENTITY.digimonEntity.stats.base.def;
	rec->speed = PARTNER_ENTITY.digimonEntity.stats.base.speed;
	rec->brains = PARTNER_ENTITY.digimonEntity.stats.base.brain;
	rec->discipline = PARTNER_PARA.discipline;
	rec->digimonId = 0x73;
	memcpy(rec->name, PARTNER_ENTITY.name, 0xe);
	rec->moves[0] = 0x2e;
	rec->moves[1] = 0x30;
	rec->moves[2] = 0x32;
}

void gameClearSave(void)
{
	CHECKED_MEMORY_CARD = 0x10;
	CURRENT_MENU = -2;
	MAIN_STATE = 2;
	TARGET_MENU = 0x45;
	CURRENT_SCREEN = getFileCityTopMap();
	awardMachinedramonData();
	if ((MEMORY_CARD_ID == -1) || (MEMORY_CARD_SLOT == -1)) {
		TARGET_MENU = -1;
	}
}

int32_t scriptTickMainMenu(void)
{
	tickMainMenu();
	renderMainMenu();
	if (CURRENT_MENU == -1) {
		return 1;
	}

	return 0;
}

void renderInitialMenu(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
#if !VERSION_IS(US)
	renderText(cur++, 0x5e, 0x37, 0, 0, 0x84, 0xc, 0);
	if (CONNECTED_CARDS != 0) {
		renderText(cur++, 0x5e, 0x43, 0, 0xc, 0x84, 0x18, 0);
	} else {
		renderText(cur++, 0x5e, 0x43, 0, 0xc, 0x84, 0x18, 1);
	}
	renderText(cur++, 0x5e, 0x5b, 0, 0x24, 0x84, 0xc, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x54, 0x32, 0x98, 0x3a);
#else
	renderText(cur++, 0x52, 0x37, 0, 0, 0xB0, 0xC, 0);
	if (CONNECTED_CARDS != 0) {
		renderText(cur++, 0x52, 0x43, 0, 0xC, 0xB0, 0x18, 0);
	} else {
		renderText(cur++, 0x52, 0x43, 0, 0xC, 0xB0, 0x18, 1);
	}
	renderText(cur++, 0x52, 0x5B, 0, 0x24, 0xB0, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x48, 0x32, 0xB0, 0x3A);
#endif
}
