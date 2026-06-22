#ifndef DW_SCRIPT_H
#define DW_SCRIPT_H

#define LANGUAGE_C
#include <setjmp.h>

#include <libgpu.h>

#include <dw/entity.h>
#include <dw/types.h>

typedef struct {
	uint8_t *scriptPtr;
	uint16_t scriptId;
	uint8_t smth[2];
} StackEntry;

typedef struct {
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
} BattleEntry;

extern BattleEntry MAIN_D_8013024C[];

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

extern uint16_t MAIN_D_80134FFC;
extern uint8_t MAIN_D_80135010;
extern ScriptState *SCRIPT_STATE_PTR;
extern uint8_t ACTIVE_BGM_FONT;
extern uint8_t *TEXT_BUFFERS_PTR;
extern uint8_t *MAIN_D_80134FDC;
extern uint8_t *MAPHEAD_DATA_PTR;
extern int32_t CURRENT_SCRIPT_PTR;
extern int32_t LOADED_DIGIMON_MODELS[];
extern ItemMenuBox *MAIN_D_80134F68;
extern ItemMenuBox *MAIN_D_80134F6C;
extern int8_t TRN_LOADING_COMPLETE;
extern uint8_t MAIN_D_80134FE6;
extern uint16_t MAIN_D_80134FC6;
extern int16_t MAIN_D_8013027C[];
extern uint16_t CURRENT_SCRIPT_ID;
extern uint16_t MAIN_D_80134FFE;
extern int32_t MAIN_D_80134FEC;
extern jmp_buf SCRIPT_JMP_BUF;
extern uint8_t MAIN_D_80134FE5;
extern uint8_t MAIN_D_80134FE7;
extern int32_t SOME_SCRIPT_SYNC_BIT;
extern int32_t IS_SCRIPT_PAUSED;
extern int16_t MAIN_D_80134F9C;
extern uint16_t ACTIVE_MAP_SCRIPT;
extern int16_t MERIT;
extern int16_t MAIN_D_80134FC8;
extern int16_t MAIN_D_80134FCA;
extern int16_t MAIN_D_80134FCC;
extern int16_t TOURNAMENTS_LOST;
extern int16_t MAIN_D_80134FD0;
extern int16_t MAIN_D_80134FD2;
extern int16_t MAIN_D_80134FD4;
extern int16_t MAIN_D_80134FD6;
extern uint16_t MAIN_D_80134F8C;
extern uint8_t MAIN_D_80134F78;
extern int16_t MAIN_D_80135002;
extern int16_t MAIN_D_80135004;
extern int32_t MAIN_D_80134F70;
extern int32_t MAIN_D_80134F74;
extern uint8_t INVENTORY_SIZE[];
extern uint8_t MAIN_D_8012FE78[];
extern int32_t MAIN_D_8013500C;
extern uint16_t SELECTION_MENU_STATE;
extern uint16_t SCRIPT_STATE_4;
extern uint8_t SCRIPT_STATE_3;
extern int32_t MAIN_D_80134FA0;
extern int32_t MAIN_D_80134FE0;
extern int32_t MONEY;
extern char MAIN_D_801B1D1C[];
extern int32_t MAIN_D_80134F84;
extern uint8_t ACTIVE_INSTRUCTION;
extern uint8_t MAIN_D_80135011;
extern uint8_t MAIN_D_80135000;
extern uint8_t MAIN_D_80134FA4;
extern uint16_t MAIN_D_80134FF8;
extern ScriptCameraMovement MAIN_D_801BE72C;
extern uint8_t MAIN_D_80134F8E;
extern int8_t MAIN_D_80134F98;
int32_t scriptIdToEntityId(int32_t scriptId);
uint32_t showTextbox(int32_t boxId, uint32_t speakerId);
void closeBox(int32_t boxId);
void MAIN_func_801062F8(int32_t owner);
void MAIN_func_800FCCFC(ItemMenuBox *box, int32_t startRow, int32_t style);
void startAnimationTamer(int32_t animId);
void setTamerState(int8_t state);
void tickScriptDialogueBox(void);
void renderScriptDialogueBox(void);
void returnFromScriptFile(void);
int32_t getEntityScreenPos(Entity *entity, int32_t flag, int16_t *outPos);
void readFileSection(char *filename, void *dest, uint32_t offset,
		     uint32_t size);
void dailyPStatTrigger(void);
void showMapheadSelection(int32_t idx, int32_t owner, int32_t x,
			  int32_t *outSel, uint16_t section);
int32_t setCardAmount(int32_t cardId, int32_t value);
int32_t popScriptStack(StackEntry *entry);
void MAIN_func_801050C0(void);
void scriptCompareDate(void);
int32_t giveItem(uint32_t type, uint8_t amount);
void removeItem(int32_t type, int32_t amount);
void readMapTFS(int32_t mapId);
void handleItemLoss(void);
void setMovementEnabled(int32_t a0, int32_t a1);
void startNPCAnimation(uint32_t scriptId, int32_t animId);
uint8_t rollCard(void);
void showCardTextbox(void);
void MAIN_func_80107C4C(void);
void renderNameDisplayBox(void);
void initialKeyInputs(void);
void setInputRepeatMask(uint32_t mask);
int32_t isPartnerBaby(void);
void MAIN_func_800FC508(void);
void initializeItemMenuBox(ItemMenuBox **box, int32_t bufSize, int32_t rows,
			   int32_t x, uint8_t y, uint8_t w, uint8_t h);
void showShopkeeperTextbox(int32_t idx, int32_t owner, int32_t boxId);
void destroyItemMenuBox(ItemMenuBox **box);
void MAIN_func_800FC968(int32_t showBits);
void MAIN_func_800FCA14(int32_t idx, int32_t owner, int32_t boxId,
			int32_t *outSelection);
void MAIN_func_800FCA3C(void);
void MAIN_func_800FCB3C(void);
ItemMenuBox *MAIN_func_800FCC40(void);
void MAIN_func_800FCC98(ItemMenuBox *box, int32_t boxId, int32_t startRow);
void MAIN_func_800FDFB4(void);
void showMapHeadTextbox(int32_t idx, int32_t owner, int32_t boxId,
			int32_t section);
void setDialogueOwner(int32_t owner);
void createMonochromonMoodBubble(void);
void MAIN_func_800FF9AC(void);
void MAIN_func_8010020C(void);
void MAIN_func_8010064C(void);
void createTextbox(int32_t boxId, int32_t flags, RECT *rect, RECT *origin,
		   void *tick, void *render);
void triggerBoxCloseFlag(int32_t boxId);
void registerTextbox(int32_t boxId, int32_t row, int32_t rows,
		     int32_t doubleBuffer, int32_t mode);
int32_t setupBoxOrigin(int32_t ownerId, RECT *origin);
void scriptShowSelection(void);
void scriptSetTextboxSize(void);
int32_t MAIN_func_801022FC(void);
int16_t *getStatsPointer(int32_t stat);
int32_t MAIN_func_801024CC(void);
int32_t MAIN_func_80102514(void);
int32_t MAIN_func_80102564(void);
int32_t MAIN_func_801025E8(void);
int32_t MAIN_func_80102630(void);
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
void MAIN_func_80105464(uint8_t actorId, int32_t animationId);
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
uint32_t dateToSeconds(uint32_t years, uint32_t days, uint32_t hours,
		       uint32_t minutes);
void pollNextInt(int32_t *out);
void MAIN_func_8010692C(uint32_t totalMinutes, uint8_t *outYear,
			uint8_t *outDay, uint8_t *outHour,
			uint8_t *outMinute);
void scriptLoadModel(int32_t modelId);
void pollNextScriptShort(int16_t *out);
void pollNextTwoScriptShorts(int16_t *out1, int16_t *out2);
void playBGM(int16_t bgmId);
void scriptUnloadModel(int16_t modelId);
void getTriggerOffset(int32_t trigger, uint8_t **outPtr, uint8_t *outMask);
int32_t MAIN_func_80106D1C(int32_t moveId);
int32_t MAIN_func_80106D28(void);
int32_t MAIN_func_80107000(void);
void MAIN_func_80107110(void);
int32_t MAIN_func_80107200(void);
int32_t MAIN_func_801072C4(void);
void MAIN_func_80107444(void);
void MAIN_func_80107660(void);
void MAIN_func_80107784(void);
void MAIN_func_801078F4(void);
void MAIN_func_80107AB8(void);
void MAIN_func_80107B98(void);
void MAIN_func_80107D54(void);
void MAIN_func_80107DFC(void);
void setupNewGameDialogueBox(void);
void showNewgameDialogue(int32_t textId, int16_t nextState);
void showNewgameSelection(int32_t textId, int16_t nextState);
void setupNameSelectorBox(void);
void setupNameDisplayBox(void);
void MAIN_func_8010B648(void);
void rollCardPack(void);
void MAIN_func_8010B9D8(void);
void MAIN_func_8010BB0C(void);
void MAIN_func_8010BC10(void);
void MAIN_func_8010BF68(void);
void openJukebox(void);
void MAIN_func_8010C28C(void);
void MAIN_func_8010C4B0(void);
void initializeNamingBuffer(uint8_t flags);
int32_t newGameStateMachine(void);
void clearTextSubArea(RECT *rect);
void setTextColor(int32_t color);
int16_t *getStatsPointer(int32_t stat);
uint8_t *getScript(int32_t mapId);
uint8_t *getScriptSection(uint8_t *script, int32_t section);
extern void MAIN_func_80100258(int32_t a0);
extern void lostAllLives(void);

#endif
