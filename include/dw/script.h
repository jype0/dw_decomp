#ifndef DW_SCRIPT_H
#define DW_SCRIPT_H

#include <setjmp.h>

#include <libgpu.h>

#include <dw/entity.h>
#include <dw/font.h>
#include <dw/types.h>

/*
 * Script opcodes. Arguments follow the opcode byte; "_" is a padding byte,
 * u8/u16/s16/s32 are little-endian values, "id" is a script id (an NPC, or a
 * SPEAKER_* value) and "pstat" the index of a pstat. Offsets are from the
 * start of the script. ACTIVE_INSTRUCTION holds the opcode the script waits
 * on, such as SCRIPT_OP_TEXT while a textbox is shown.
 */
#define SCRIPT_OP_CHOICE		0x10	/* u8 count, u16 targets[count], text */
#define SCRIPT_OP_CALL			0x13	/* _, u16 offset */
#define SCRIPT_OP_CALL_SCRIPT		0x14	/* _, u16 script, u16 section */
#define SCRIPT_OP_RETURN		0x15	/* _ */
#define SCRIPT_OP_JUMP			0x16	/* _, u16 offset */
#define SCRIPT_OP_JUMP_SCRIPT		0x17	/* _, u16 script, u16 section */
#define SCRIPT_OP_SWITCH		0x18	/* pstat, u16 count, u16 offsets[count] */
#define SCRIPT_OP_CONDITION		0x19	/* _, entries, 0x19, _ */
#define SCRIPT_OP_TEXT			0x1A	/* _, text */
#define SCRIPT_OP_SPEAKER		0x1B	/* id */
#define SCRIPT_OP_SET_TRIGGER		0x1C	/* _, u16 trigger */
#define SCRIPT_OP_UNSET_TRIGGER		0x1D	/* _, u16 trigger */
#define SCRIPT_OP_SET_PSTAT		0x1E	/* _, pstat, u8 value */
#define SCRIPT_OP_ADD_PSTAT		0x1F	/* _, pstat, u8 value */
#define SCRIPT_OP_SUB_PSTAT		0x20	/* _, pstat, u8 value */
#define SCRIPT_OP_GET_MAP		0x21	/* pstat */
#define SCRIPT_OP_GET_PARTNER_TYPE	0x22	/* pstat */
#define SCRIPT_OP_SET_INVENTORY_SIZE	0x23	/* u8 size */
#define SCRIPT_OP_RANDOM		0x24	/* _, pstat, u8 max */
#define SCRIPT_OP_GET_DATE		0x25	/* pstat: year, day, hour, minute */
#define SCRIPT_OP_SIZED_TEXTBOX		0x26	/* id origin, u8 cols, u8 rows */
#define SCRIPT_OP_CLOSE_TEXTBOX		0x27	/* u8 box */
#define SCRIPT_OP_GIVE_ITEM		0x28	/* _, u8 item, u8 amount; see TRIGGER_GIVE_ITEM_FAILED */
#define SCRIPT_OP_TAKE_ITEM		0x29	/* _, u8 item, u8 amount */
#define SCRIPT_OP_ADD_MONEY		0x2A	/* _, s32 amount */
#define SCRIPT_OP_SUB_MONEY		0x2B	/* _, s32 amount */
#define SCRIPT_OP_COMPARE_DATE		0x2C	/* pstat date, u16 trigger, u8 compare, u8 year, u8 day, u8 hour, u8 minute, _ */
#define SCRIPT_OP_LEARN_MOVE		0x2D	/* u8 move */
#define SCRIPT_OP_NOP_2E		0x2E	/* u8 */
#define SCRIPT_OP_GIVE_CARD		0x2F	/* u8 card */
#define SCRIPT_OP_TAKE_CARD		0x30	/* u8 card */
#define SCRIPT_OP_SET_MERIT		0x31	/* _, u16 value */
#define SCRIPT_OP_ADD_MERIT		0x32	/* _, u16 value */
#define SCRIPT_OP_SUB_MERIT		0x33	/* _, u16 value; sets MERIT to -value */
#define SCRIPT_OP_SET_STAT		0x34	/* u8 stat, u16 value; a SCRIPT_STAT_* */
#define SCRIPT_OP_ADD_STAT		0x35	/* u8 stat, u16 value */
#define SCRIPT_OP_SUB_STAT		0x36	/* u8 stat, u16 value */
#define SCRIPT_OP_WAIT_UNTIL_DATE	0x37	/* pstat: year, day, hour, minute */
#define SCRIPT_OP_ADD_TO_DATE		0x38	/* pstat, s32 minutes */
#define SCRIPT_OP_SUB_FROM_DATE		0x39	/* pstat, s32 minutes */
#define SCRIPT_OP_NOP_3A		0x3A	/* _, u8, u8 */
#define SCRIPT_OP_NOP_3B		0x3B	/* _, u8, u8 */
#define SCRIPT_OP_NOP_3C		0x3C	/* _, u8, u8 */
#define SCRIPT_OP_NOP_3D		0x3D	/* u8 */
#define SCRIPT_OP_NOP_3E		0x3E	/* u8 */
#define SCRIPT_OP_GET_DIGIMON_TYPE	0x3F	/* _, pstat digimon, pstat out */
#define SCRIPT_OP_LOAD_MODEL		0x46	/* u8 digimon */
#define SCRIPT_OP_SPAWN_DIGIMON		0x47	/* u8 digimon, u8 slot, u8 autotalk */
#define SCRIPT_OP_REMOVE_DIGIMON	0x48	/* id */
#define SCRIPT_OP_DIGIMON_ROUTINE	0x49	/* u8 routine, see callDigimonRoutine() */
#define SCRIPT_OP_WAIT_MOVEMENT		0x4A	/* id, or 0xFF for all */
#define SCRIPT_OP_CHANGE_SCREEN		0x4B	/* u8 screen, u8 exit, u8 section */
#define SCRIPT_OP_LOOK_AT		0x4C	/* _, id, id target */
#define SCRIPT_OP_SET_ROTATION		0x4D	/* id, s16 rotation */
#define SCRIPT_OP_WALK_TO		0x4E	/* id, s16 x, s16 z, u8 anim, u8 */
#define SCRIPT_OP_CAMERA_TO		0x4F	/* u8 speed, s16 x, s16 z */
#define SCRIPT_OP_CAMERA_TO_ENTITY	0x50	/* _, id, u8 speed */
#define SCRIPT_OP_WALK_TO_ENTITY	0x51	/* id, u8 anim, id target */
#define SCRIPT_OP_WALK_TO_CAMERA	0x52	/* as WALK_TO, the camera follows */
#define SCRIPT_OP_WALK_TO_ENTITY_CAMERA	0x53	/* as WALK_TO_ENTITY, the camera follows */
#define SCRIPT_OP_RESET_ORIGIN		0x54	/* id */
#define SCRIPT_OP_TEXTBOX_ORIGIN	0x55	/* _, s16 x, s16 y, s16 z */
#define SCRIPT_OP_ANIMATE		0x56	/* _, id, u8 anim */
#define SCRIPT_OP_SET_MAP_OBJECT	0x57	/* _, u8 object, u8 flag */
#define SCRIPT_OP_CHANGE_SCREEN_PSTAT	0x58	/* pstat: screen, exit */
#define SCRIPT_OP_PLAY_SOUND		0x5A	/* _, u8 bank, u8 sound */
#define SCRIPT_OP_NOP_5B		0x5B	/* u8 */
#define SCRIPT_OP_NOP_5C		0x5C	/* u8 */
#define SCRIPT_OP_PLAY_BGM		0x5D	/* u8 bgm */
#define SCRIPT_OP_STOP_BGM		0x5E	/* u8 */
#define SCRIPT_OP_NOP_5F		0x5F	/* u8 */
#define SCRIPT_OP_BUILTIN		0x64	/* u8 builtin; a SCRIPT_BUILTIN_* */
#define SCRIPT_OP_CURE_CONDITION	0x65	/* u8 mask of CONDITION_* */
#define SCRIPT_OP_BATTLE		0x66	/* u8 */
#define SCRIPT_OP_WAIT			0x67	/* _, u16 frames */
#define SCRIPT_OP_TEXT_ADVANCE		0x68	/* _, u8 mode, u8 frames */
#define SCRIPT_OP_HURT_PARTNER		0x69	/* pstat percent of max HP */
#define SCRIPT_OP_SET_AUTOTALK		0x6A	/* _, id, u8 autotalk */
#define SCRIPT_OP_NOP_6B		0x6B
#define SCRIPT_OP_MOVE_TO		0x6C	/* id, s16 x, s16 z, u8 speed, u8 */
#define SCRIPT_OP_MOVE_TO_ENTITY	0x6D	/* id, id target, u8 speed */
#define SCRIPT_OP_MOVE_TO_CAMERA	0x6E	/* as MOVE_TO, but uses MOVE_TO_ENTITY_CAMERA's movement */
#define SCRIPT_OP_MOVE_TO_ENTITY_CAMERA	0x6F	/* as MOVE_TO_ENTITY, the camera follows */
#define SCRIPT_OP_ROTATE_DOOR		0x70	/* u8 door, u8, u8 target */
#define SCRIPT_OP_MOVE_OBJECT		0x71	/* u8 slot, id, u8 speed, s8 angle, _, s16 x, s16 z */
#define SCRIPT_OP_MOVE_ALONG_AXIS	0x72	/* id, s16 target, u8 axis, u8 speed */
#define SCRIPT_OP_MOVE_ALONG_AXIS_CAMERA 0x73	/* as MOVE_ALONG_AXIS, the camera follows */
#define SCRIPT_OP_SPAWN_ITEM		0x74	/* u8 item, s16 x, s16 z */
#define SCRIPT_OP_SPAWN_CHEST		0x75	/* u8 item, s16 x, y, z, w, u16 trigger */
#define SCRIPT_OP_SPAWN_BOULDER		0x76	/* u8 */
#define SCRIPT_OP_MOVE_BOULDER		0x77	/* u8, s16 x, s16 z */
#define SCRIPT_OP_REMOVE_BOULDER	0x78	/* u8 */
#define SCRIPT_OP_UNLOAD_MODEL		0x79	/* u8 digimon */
#define SCRIPT_OP_COPY_PSTAT		0x7A	/* _, pstat from, pstat to */
#define SCRIPT_OP_RETURN_SECTION	0x7B	/* u8 section to run after a screen change */
#define SCRIPT_OP_SET_IMPASSABLE	0x7C	/* u8, s16 x, s16 z, u8 w, u8 h */
#define SCRIPT_OP_SPAWN_SPRITE		0x7D	/* u8 sprite, s16 x, y, z, w */
#define SCRIPT_OP_SPAWN_SPRITE_AT	0x7E	/* id, u8 node, u8 sprite */
#define SCRIPT_OP_WARP			0xFB	/* _, u16 script, u16 map */
#define SCRIPT_OP_NOP_FC		0xFC
#define SCRIPT_OP_NOP_FD		0xFD
#define SCRIPT_OP_END			0xFE
#define SCRIPT_OP_END_FF		0xFF

/* The stats that SCRIPT_OP_SET_STAT and the like change, see getStatsPointer(). */
#define SCRIPT_STAT_OFFENSE		0x00
#define SCRIPT_STAT_DEFENSE		0x01
#define SCRIPT_STAT_SPEED		0x02
#define SCRIPT_STAT_BRAINS		0x03
#define SCRIPT_STAT_MAX_HP		0x04
#define SCRIPT_STAT_MAX_MP		0x05
#define SCRIPT_STAT_HP			0x06
#define SCRIPT_STAT_MP			0x07
#define SCRIPT_STAT_TIREDNESS		0x08
#define SCRIPT_STAT_HAPPINESS		0x09
#define SCRIPT_STAT_DISCIPLINE		0x0A
#define SCRIPT_STAT_ENERGY		0x0B
#define SCRIPT_STAT_VIRUS		0x0C
#define SCRIPT_STAT_LIFETIME		0x0D
#define SCRIPT_STAT_MERIT		0x0E
#define SCRIPT_STAT_BATTLES_FOUGHT	0x0F
#define SCRIPT_STAT_BATTLES_WON		0x10
#define SCRIPT_STAT_TOURNAMENT_TITLES	0x11
#define SCRIPT_STAT_TOURNAMENT_WINS	0x12
#define SCRIPT_STAT_TOURNAMENT_LOSSES	0x13
#define SCRIPT_STAT_WEIGHT		0x14
#define SCRIPT_STAT_TAMER_LEVEL		0x15
#define SCRIPT_STAT_LIVES		0x16

/*
 * Routines of the engine that SCRIPT_OP_BUILTIN runs. Most take their
 * argument from PSTAT_BUILTIN_ARG and leave a result in PSTAT_RESULT; the
 * ones that wait for the player run from tickScript() until they are done.
 */
#define SCRIPT_BUILTIN_PICK_ITEM		0x00
#define SCRIPT_BUILTIN_MERIT_SHOP		0x01
#define SCRIPT_BUILTIN_COLLECTOR_TRADE		0x02
#define SCRIPT_BUILTIN_TOURNAMENT		0x06
#define SCRIPT_BUILTIN_TRAINING			0x07
#define SCRIPT_BUILTIN_SHOP			0x08
#define SCRIPT_BUILTIN_JUKEBOX			0x09
#define SCRIPT_BUILTIN_TRANSPORT		0x0A
#define SCRIPT_BUILTIN_ITEM_KEEPER		0x0B
#define SCRIPT_BUILTIN_TAKE_ONE_ITEM		0x0C
#define SCRIPT_BUILTIN_TAKE_ALL_OF_ITEM		0x0D
#define SCRIPT_BUILTIN_CARD_SHOP		0x0E
#define SCRIPT_BUILTIN_OPEN_MONEY_BOX		0x0F
#define SCRIPT_BUILTIN_CLOSE_MONEY_BOX		0x10
#define SCRIPT_BUILTIN_TOURNAMENT_SCHEDULE	0x12
#define SCRIPT_BUILTIN_SET_DIRT_CART		0x13
#define SCRIPT_BUILTIN_SHRINK_DIRT_PILE		0x14
#define SCRIPT_BUILTIN_RESET_MAP_OBJECT_ANIM	0x15	/* pstats 0xF7 and 0xF8 */
#define SCRIPT_BUILTIN_TOURNAMENT_MEDAL		0x16
#define SCRIPT_BUILTIN_OPEN_CHEST_TRAY		0x17
#define SCRIPT_BUILTIN_CLOSE_CHEST_TRAY		0x18
#define SCRIPT_BUILTIN_SET_TIME_SPEED		0x19
#define SCRIPT_BUILTIN_MOOD_BUBBLE		0x1C	/* over NPC pstat 0xF7, mood pstat 0xF8 */
#define SCRIPT_BUILTIN_EGG_BOX			0x1D
#define SCRIPT_BUILTIN_CLEAR_PRICE		0x1E
#define SCRIPT_BUILTIN_PRICE_FROM_TEXT_ARGS	0x1F
#define SCRIPT_BUILTIN_NAMING			0x20
#define SCRIPT_BUILTIN_LOAD_SHOP		0x23
#define SCRIPT_BUILTIN_REMOVE_MIST		0x25
#define SCRIPT_BUILTIN_FORGET_MOVES		0x2F	/* after all lives are lost */
#define SCRIPT_BUILTIN_SAVE_MACHINE		0x30
#define SCRIPT_BUILTIN_ANGEMON_PEDESTAL		0x32
#define SCRIPT_BUILTIN_GAME_CLEAR_SAVE		0x36
#define SCRIPT_BUILTIN_PRICE_TO_TEXT_ARGS	0x38

typedef struct {
	uint8_t *scriptPtr;
	uint16_t scriptId;
	uint8_t smth[2];
} StackEntry;

typedef struct {
	/*
	 * [0, 6): the cards on sale today, see dailyPStatTrigger().
	 * [6, 84): how many of each LOST_ITEM_IDS the lost item shop has.
	 * [84, 212): how many of each item the Item Keeper stores.
	 * Split into arrays, the + 6 and + 0x54 indexing no longer matches.
	 */
	uint8_t smth[212];
	uint8_t cards[33];
	uint8_t triggers[100];
	uint8_t pstats[256];
	StackEntry stack[8];
} ScriptState;

typedef struct {
	uint8_t mapId;
	uint8_t unk_0x1;
	uint16_t trigger;
	uint32_t cost;
} TransportDestination;

extern TransportDestination TRANSPORT_DESTINATIONS[];

typedef struct {
	uint8_t digimonId;
	uint8_t spriteId;
	uint8_t unk2;
	uint8_t unk3;
} CardData;

extern CardData CARD_DATA[];

typedef struct {
	uint8_t *buf;
	int32_t isOpen;
	uint8_t itemCount;
	uint8_t topRow;
	uint8_t cursor;
	uint8_t visibleRows;
	uint8_t boxId;
	uint8_t prevTopRow;
	uint8_t prevCursor;
	RECT rect;
	uint8_t itemRow[8];
} ItemMenuBox;

/* ITEM_MENU_MODE: what an item menu lists and what picking a row does. */
#define ITEM_MENU_BUY		0
#define ITEM_MENU_SELL		1
#define ITEM_MENU_BUY_LOST	2	/* buy back items lost in battle */
#define ITEM_MENU_BUY_CARD	3
#define ITEM_MENU_SELL_CARD	4
#define ITEM_MENU_PICK_ITEM	5	/* choose an item to give or store */
#define ITEM_MENU_MERIT_CARD	6	/* trade a card for merit points */
#define ITEM_MENU_MERIT_ITEM	7	/* buy an item with merit points */

typedef struct {
	uint8_t type;
	uint8_t entityId;
	uint8_t target;
	uint8_t speed;
	int16_t posX;
	int16_t posY;
	int16_t targetX;
	int16_t targetY;
} ScriptCameraMovement;

extern uint16_t SCRIPT_WAIT_TIMER;
extern uint8_t TEXT_AUTO_ADVANCE_TIMER;
extern ScriptState *SCRIPT_STATE_PTR;
extern uint8_t ACTIVE_BGM_FONT;
extern uint8_t *TEXT_BUFFERS_PTR;
extern uint8_t *SCRIPT_PC;
extern uint8_t *MAPHEAD_DATA_PTR;
extern int32_t LOADED_DIGIMON_MODELS[];
extern ItemMenuBox *MAIN_D_80134F68;
extern ItemMenuBox *MAIN_D_80134F6C;
extern int8_t TRN_LOADING_COMPLETE;
extern uint8_t DIALOGUE_SPEAKER;
extern uint16_t SCRIPT_STACK_DEPTH;
extern int16_t ITEM_MENU_RECTS[];
extern uint16_t CURRENT_SCRIPT_ID;
extern uint16_t CURRENT_MAP_ID;
extern int32_t SCRIPT_WARPING;
extern jmp_buf SCRIPT_JMP_BUF;
extern uint8_t TEXT_ADVANCE_MODE;
extern uint8_t SCRIPT_SAVED_TIME_SPEED;
extern int32_t SOME_SCRIPT_SYNC_BIT;
extern int32_t IS_SCRIPT_PAUSED;
extern int16_t SCRIPT_TALKED_ENTITY;
extern uint16_t ACTIVE_MAP_SCRIPT;
extern int16_t MERIT;
extern int16_t BATTLES_FOUGHT;
extern int16_t BATTLES_WON;
extern int16_t TOURNAMENT_TITLES;
extern int16_t TOURNAMENT_WINS;
extern int16_t TOURNAMENT_LOSSES;
extern int16_t TEXTBOX_ORIGIN_X;
extern int16_t TEXTBOX_ORIGIN_Y;
extern int16_t TEXTBOX_ORIGIN_Z;
extern uint16_t NAMING_CURSOR;
extern uint8_t SELECTED_ITEM;
extern int16_t STAT_TAMER_LEVEL_VALUE;
extern int16_t STAT_LIVES_VALUE;
extern int32_t SHOP_CHOICE;
extern int32_t SHOP_MADE_A_DEAL;
extern uint8_t LOST_ITEM_IDS[];
extern int32_t SCRIPT_PRICE;
extern uint16_t SELECTION_MENU_STATE;
extern uint16_t SCRIPT_STATE_4;
extern uint8_t SCRIPT_STATE_3;
extern int32_t MAIN_D_80134FA0;
extern int32_t SCRIPT_FROM_TALK;
extern int32_t MONEY;
extern char NAMING_BUFFER[];
extern int32_t MONEY_BOX_DIRTY;
extern uint8_t ACTIVE_INSTRUCTION;
extern uint8_t ITEM_MENU_MODE;
extern uint8_t DIALOGUE_BOX_MODE;
extern uint8_t SCRIPT_WAIT_MOVEMENT_SLOT;
extern uint16_t MAIN_D_80134FF8;
extern ScriptCameraMovement SCRIPT_CAMERA_MOVEMENT;
extern uint8_t NAMING_FLAGS;
#define NAMING_PARTNER		1	/* name the partner, not the player */
#define NAMING_FROM_SCRIPT	2	/* a rename, not the new game questions */
extern int8_t TEXT_MONOSPACE;
int32_t scriptIdToEntityId(int32_t scriptId);
uint32_t showTextbox(int32_t boxId, uint32_t speakerId);
void closeBox(int32_t boxId);
void beginScriptEvent(int32_t owner);
void layoutItemMenu(ItemMenuBox *box, int32_t startRow, int32_t style);
void startAnimationTamer(int32_t animId);
void tickScriptDialogueBox(void);
void renderScriptDialogueBox(void);
void returnFromScriptFile(void);
void getEntityScreenPos(Entity *entity, int32_t flag, int16_t *outPos);
void readFileSection(char *filename, void *dest, uint32_t offset,
		     uint32_t size);
void dailyPStatTrigger(void);
void showMapheadSelection(int32_t idx, int32_t owner, int32_t x,
			  int32_t *outSel, uint16_t section);
int32_t setCardAmount(int32_t cardId, int32_t value);
int32_t popScriptStack(StackEntry *entry);
void scriptConditionBlock(void);
void scriptCompareDate(void);
void readMapTFS(int32_t mapId);
void handleItemLoss(void);
void setMovementEnabled(int32_t a0, int32_t a1);
void startNPCAnimation(uint32_t scriptId, int32_t animId);
uint8_t rollCard(void);
void showCardTextbox(void);
void fillTradeLists(void);
void renderNameDisplayBox(void);
void initialKeyInputs(void);
void setInputRepeatMask(uint32_t mask);
int32_t isPartnerBaby(void);
void tickItemShop(void);
void initializeItemMenuBox(ItemMenuBox **box, int32_t bufSize, int32_t rows,
			   int32_t x, uint8_t y, uint8_t w, uint8_t h);
void showShopkeeperTextbox(int32_t idx, int32_t owner, int32_t boxId);
void destroyItemMenuBox(ItemMenuBox **box);
void openMoneyBox(int32_t showBits);
void showShopkeeperSelection(int32_t idx, int32_t owner, int32_t boxId,
			int32_t *outSelection);
void openItemMenuBox(void);
void tickPickItemMenu(void);
ItemMenuBox *getActiveItemMenu(void);
void openItemMenu(ItemMenuBox *box, int32_t boxId, int32_t startRow);
void MAIN_func_800FDFB4(void);
void showMapHeadTextbox(int32_t idx, int32_t owner, int32_t boxId,
			int32_t section);
void setDialogueOwner(int32_t owner);
void createMonochromonMoodBubble(void);
void loadShopLibrary(void);
void resetTextboxes(void);
void closeAllTextboxes(void);
void createTextbox(int32_t boxId, int32_t flags, RECT *rect, RECT *origin,
		   void *tick, void *render);
void triggerBoxCloseFlag(int32_t boxId);
void registerTextbox(int32_t boxId, int32_t row, int32_t rows,
		     int32_t doubleBuffer, int32_t mode);
int32_t setupBoxOrigin(int32_t ownerId, RECT *origin);
void scriptShowSelection(void);
void scriptSetTextboxSize(void);
int32_t scriptTestStat(void);
int16_t *getStatsPointer(int32_t stat);
int32_t scriptTestCardAmount(void);
int32_t scriptTestHasMove(void);
int32_t scriptTestPartnerCondition(void);
int32_t scriptTestItemCount(void);
int32_t scriptTestMoney(void);
int32_t enforceStatsLimits(int32_t stat, int32_t value);
void tickScriptedMovements(void);
void scriptInstructionFBtoFF(int32_t op);
void scriptInstruction10to27(int32_t op);
void scriptInstruction28to3F(int32_t op);
void scriptInstruction46to58(int32_t op);
void scriptInstruction5Ato5F(int32_t op);
void scriptInstruction64to7E(int32_t op);
void scriptUpdateEnergyBoundaries(int32_t a0, int32_t a1);
void MAIN_func_801053EC(void);
void scriptStartWalkAnimation(uint8_t actorId, int32_t animationId);
void scriptStartAnimation(uint8_t actorId, int32_t animationId);
void handleMusicOverride(uint8_t *outFont, uint8_t *outVariant);
void tickScriptedMovement(int32_t slot);
void callScriptSection(int32_t scriptId, int32_t section, int32_t param);
void callScriptSection(int32_t scriptId, int32_t section, int32_t param);
int32_t tickScript(void);
uint8_t *getScript(int32_t mapId);
uint8_t *getScriptSection(uint8_t *script, int32_t section);
uint8_t readPStat(int32_t index);
int32_t isTriggerSet(uint16_t trigger);
void writePStat(int32_t index, uint8_t value);
void skipOneReadTwoShort(uint16_t *out1, uint16_t *out2);
void pushScriptStack(StackEntry *entry);
void resetBGM(void);
void skipOneReadOneUShort(uint16_t *out);
void pollOneUByteOneUShort(uint8_t *outByte, uint16_t *outShort);
void pollNextScriptUByte(uint8_t *out);
void setTrigger(uint16_t trigger);
void unsetTrigger(uint16_t trigger);
void skipOnePollTwoScriptBytes(uint8_t *out1, uint8_t *out2);
void pollNextScriptUShort(uint16_t *out);
void pollNextTwoScriptBytes(uint8_t *out1, uint8_t *out2);
int32_t scriptCompareValues(uint8_t op, uint32_t lhs, uint32_t rhs);
void skipOneReadInteger(int32_t *out);
void scriptLearnMove(int32_t moveId);
int32_t getCardAmount(int32_t cardId);
uint32_t dateToMinutes(uint32_t years, uint32_t days, uint32_t hours,
		       uint32_t minutes);
void pollNextInt(int32_t *out);
void minutesToDate(uint32_t totalMinutes, uint8_t *outYear,
			uint8_t *outDay, uint8_t *outHour,
			uint8_t *outMinute);
void scriptLoadModel(int32_t modelId);
void pollNextScriptShort(int16_t *out);
void pollNextTwoScriptShorts(int16_t *out1, int16_t *out2);
void playBGM(int16_t bgmId);
void scriptUnloadModel(int16_t modelId);
void getTriggerOffset(int32_t trigger, uint8_t **outPtr, uint8_t *outMask);
int32_t scriptHasMove(int32_t moveId);
int32_t fillLostItemList(void);
int32_t fillCardShopList(void);
void openCardMenuBox(void);
int32_t fillOwnedCardList(void);
int32_t fillMeritItemList(void);
void fillItemKeeperLists(void);
void openItemKeeperBoxes(void);
void fillJukeboxList(void);
void openJukeboxMenuBox(void);
void fillTransportList(void);
void openTransportMenuBox(void);
void openTradeMenuBox(void);
void markTradeDone(void);
void setupNewGameDialogueBox(void);
void showNewgameDialogue(int32_t textId, int16_t nextState);
void showNewgameSelection(int32_t textId, int16_t nextState);
void setupNameSelectorBox(void);
void setupNameDisplayBox(void);
void tickLostItemShop(void);
void rollCardPack(void);
void tickCardShop(void);
void tickCardSellShop(void);
void tickMeritShop(void);
void tickItemKeeper(void);
void openJukebox(void);
void tickTransport(void);
void tickCollectorTrade(void);
void initializeNamingBuffer(uint8_t flags);
int32_t newGameStateMachine(void);
int16_t *getStatsPointer(int32_t stat);
uint8_t *getScript(int32_t mapId);
uint8_t *getScriptSection(uint8_t *script, int32_t section);
extern void tickTextboxes(int32_t a0);
extern void lostAllLives(void);

#endif
