#include <stdlib.h>

#include <libcd.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>

#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/item.h>
#include <dw/main.h>
#include <dw/model.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include <dw/params.h>

#include "common.h"

int32_t isTriggerSet(uint16_t trigger);
void unsetTrigger(uint16_t trigger);



typedef struct {
	uint32_t unk1;
	uint32_t *globalPointer;
	uint32_t sbssSize;
	uint32_t bssSize;
	uint32_t sdataSize1;
	uint32_t dataSize1;
	uint32_t rodataSize;
	uint32_t *sbssPtr;
	uint32_t *bssPtr;
	uint32_t *sdataPtr;
	uint32_t *dataPtr;
	uint32_t *rodataPtr;
	uint32_t *unk6;
	uint32_t *unk7;
	uint32_t *unk8;
	uint32_t *stackTop;
	uint32_t unk9;
	uint32_t stackFrames;
	uint32_t scratchpadSize;
	uint32_t rodataStart;
	uint32_t sbssStart;
	uint32_t bssEnd;
	uint32_t sdataStart;
	uint32_t dataStart;
	uint32_t sdataSize2;
	uint32_t dataSize2;
	uint32_t unk17;
	void *mainTextPtr;
	void *btlTextPtr;
	void *stdTextPtr;
	void *fishTextPtr;
	void *evlTextPtr;
	void *karTextPtr;
	void *vsTextPtr;
	void *movTextPtr;
	void *doo2TextPtr;
	void *dooaTextPtr;
	void *trnTextPtr;
	void *dgetTextPtr;
	void *trn2TextPtr;
	void *murdTextPtr;
	void *endiTextPtr;
	void *eabTextPtr;
} SectionData;

typedef struct {
	uint16_t shiftJIS;
	uint16_t ascii;
} Ascii2ShiftJIS;

extern SectionData SECTION_DATA;

extern GsRVIEW2 GS_VIEWPOINT;

extern TamerItem TAMER_ITEM;

extern uint8_t INVENTORY_ITEM_TYPES[30];
extern uint8_t INVENTORY_POINTER;

extern StatsGains INITIAL_COMBAT_STATS[];

extern DVECTOR MAIN_D_801344DC;
extern DVECTOR MAIN_D_801344E0;

extern GsOT_TAG GS_ORDERING_TABLE_0[];
extern GsOT_TAG GS_ORDERING_TABLE_1[];
extern GsOT GS_ORDERING_TABLE[2];

extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;

extern DR_OFFSET DRAW_OFFSETS[];

extern uint8_t MAP_LAYER_ENABLED;

extern Ascii2ShiftJIS CHAR_MAPPING_NORMAL[];
extern Ascii2ShiftJIS CHAR_MAPPING_SPECIAL[];

void GsSetNearClip(long clip);

uint16_t MAIN_func_800F19B0(int32_t param_1, int32_t param_2);

extern GsOT *ACTIVE_ORDERING_TABLE;
extern int16_t MAIN_D_80134F10;
extern GsF_LIGHT LIGHT_DATA[3];

void renderMainMenuBackground(void);
void loadNewgameScene(void);
void tickNewGameJijimon(int32_t instanceId);
void setEntityPosition(int32_t entityId, int32_t x, int32_t y, int32_t z);
void setEntityRotation(int32_t entityId, int16_t x, int16_t y, int16_t z);
void setupEntityMatrix(int32_t entityId);
void writePStat(int32_t stat, int32_t value);
void initializeDigimonObject(int32_t type, int32_t instanceId,
			     TickFunction tick);
void *thunkLoadMMD(int32_t digiType, int32_t modelType);

void initializeHeap(void);
void initializeFramebuffer(void);
void view_init(void);
void initializeEffectData(void);
void addThrownItem(int32_t type);
void startThrowingItem(void);
void tickThrownItem(int32_t instanceId);
void renderThrownItem(int32_t instanceId);
void handleBuffDisks(int32_t type);
void MAIN_func_800F1794(void);
int32_t removeBuffModelObject(void);
uint16_t convertAsciiToJis(uint8_t input);
int32_t swapShortBytes(int32_t input);
int32_t isAsciiEncoded(const char *value);
void renderOverworldItem(TamerItem *item);
void tickAnimation(Entity *entity);
void removeEntity(int32_t type, int32_t entityId);
void thunkUnloadModel(int32_t id, int32_t modelType);
void tickPartnerBattle(int32_t instanceId);
void tickNPCBattle(int32_t instanceId);
void unloadNewGameScene(void);
void initializeBuffModel(TMDModel *model);
void tickBuffModelObject(int32_t instanceId);
int32_t initializeBuffModelObject(void);

extern int32_t buffModelValue[2];
extern int32_t buffModelFrame;
extern TMDModel *BUFF_MODEL[];

void resetFlattenGlobal(void);
void initializeDroppedItems(void);
void resetInventoryFlags(void);
void initializeInventory(void);
void MAIN_func_800DF804(void);
void initializeInventoryModules(void);

void startAnimation(Entity *entity, uint8_t animId);

void initializeEFE(void);
void initializeParticleFX(void);
void initializeEntityParticleFX(void);
void initializeCloudFXData(void);
void initializeHealingParticles(void);

void BTL_buffStats(Entity *entity, int32_t entityId, int32_t amount,
		   int16_t* valuePtr, uint8_t color, uint8_t icon);
void BTL_addBuffDiskEffect(Entity *entity);

void SsInit(void);
void PadInit(int32_t mode);
long MemCardInit(long val);
long MemCardStart(void);
void initializeGsTMDMap(void);
void initializeAttackObjects(void);
void initializeFontCLUT(void);
void initializeScripts(void);
void initializeStatusObjects(void);
void initializeMusic(void);
void finalizeMusic(void);
void playMovie(int32_t movieId, dw_bool shouldPlay);
void loadStackedTIMFile(char *path);
void runLandingScreen(void);
void runMainMenu(void);
void newGameScene(void);
void MAIN_func_800EF38C(void);
void recalculatePPandArena(void);
void gameLoop(void);
void cleanupGame(void);
void initializeTamer(int32_t id, int32_t x, int32_t y, int32_t z,
		     int32_t rx, int32_t ry, int32_t rz);
void initializePartner(int32_t type, int32_t posX, int32_t posY,
		       int32_t posZ, int32_t rotX, int32_t rotY,
		       int32_t rotZ);
void setDigimonRaised(uint16_t type);
void MAIN_func_800D56E0(void);
void runMapHeadScript(int32_t scriptId);
int32_t readPStat(int32_t id);
int32_t getTamerState(void);
void setTamerState(int32_t state);
void stopGameTime(void);
void fadeFromBlack(int32_t frames);
void initializeUIBoxData(void);
void initializeClockData(void);
void initializeMedalModel(void);
void initializeChest(void);
void addClock(void);
void initializeFileReadQueue(void);
void fillEFEXTable(void);
void initializeFadeData(void);
int32_t loadTIMFile(char *path, void *buffer);

extern int8_t MAIN_STATE;
extern int32_t MAIN_D_80134EB0;
extern char MAIN_D_8012CE64[];
extern char MAIN_D_8012CE78[];
extern char MAIN_D_8012CE8C[];
extern int32_t MAIN_D_80155670[];
extern uint8_t MAIN_D_80155725[];

int32_t main(void);
void applyHUDOffset(int32_t offset);
extern int32_t POLLED_INPUT;
extern int32_t POLLED_INPUT_PREVIOUS;
extern int32_t CHANGED_INPUT;
extern int32_t MAIN_D_80134EA4;
extern int32_t MAIN_D_80134EA8;
void pollInputGame(void);
void pollInputMenu(void);
void renderPressStartToContinue(void);
void tickTamerBattle(int32_t instanceId);
void MAIN_func_800F0B2C(void);
void MAIN_func_800F1020(void);
void MAIN_func_800F179C(void *model, int32_t compIdx, int32_t color);
uint16_t asciiToShiftJIS(uint8_t input);

/* Order anchor (reversed): pins symtab/section order to address order.
 * Unreferenced; discarded by --gc-sections. */
void *main_order_anchor[] = {
	swapShortBytes,
	asciiToShiftJIS,
	convertAsciiToJis,
	isAsciiEncoded,
	removeBuffModelObject,
	initializeBuffModelObject,
	initializeBuffModel,
	MAIN_func_800F179C,
	MAIN_func_800F1794,
	tickBuffModelObject,
	unloadNewGameScene,
	loadNewgameScene,
	tickNewGameJijimon,
	tickNPCBattle,
	tickPartnerBattle,
	tickTamerBattle,
	MAIN_func_800F1020,
	MAIN_func_800F0B2C,
	handleBuffDisks,
	renderThrownItem,
	tickThrownItem,
	startThrowingItem,
	addThrownItem,
	initializeInventoryModules,
	initializeEffectData,
	view_init,
	renderMainMenuBackground,
	renderPressStartToContinue,
	pollInputMenu,
	pollInputGame,
	applyHUDOffset,
	gameLoop,
	recalculatePPandArena,
	MAIN_func_800EF38C,
	newGameScene,
	runMainMenu,
	runLandingScreen,
	initializeFramebuffer,
	initializeHeap,
	main,
};

int32_t main(void)
{
	int32_t partnerId;

	initializeHeap();
	SsInit();
	ResetCallback();
	ResetGraph(0);
	SetGraphDebug(0);
	initializeFramebuffer();
	PadInit(0);
	CdInit();
	MemCardInit(1);
	MemCardStart();
	initializeModelComponents();
	initializeWorldObjects();
	initializeGsTMDMap();
	initializeFileReadQueue();
	initializeAttackObjects();
	initializeFontCLUT();
	fillEFEXTable();
	initializeFadeData();
	initializeScripts();
	view_init();
	initializeUIBoxData();
	initializeEffectData();
	initializeInventoryModules();
	initializeMedalModel();
	initializeClockData();
	initializeStatusObjects();
	loadTIMFile(MAIN_D_8012CE64, GENERAL_BUFFER_PTR);

	for (;;) {
		loadTIMFile(MAIN_D_8012CE78, GENERAL_BUFFER_PTR);
		MAIN_D_80134EB0 = 0;
		while (MAIN_D_80134EB0 == 0) {
			playMovie(0, 1);
			initializeMusic();
			runLandingScreen();
			finalizeMusic();
		}

		loadStackedTIMFile(MAIN_D_8012CE8C);
		initializeMusic();
		runMainMenu();

		switch (MAIN_STATE) {
		case 0:
			newGameScene();
			finalizeMusic();
			playMovie(1, 1);
			initializeMusic();
			loadStackedTIMFile(MAIN_D_8012CE8C);
			initializeTamer(0, 0, 0, 0, 0, 0, 0);
			if (readPStat(0xFE) == 0) {
				partnerId = 0x3;
			} else {
				partnerId = 0x11;
			}
			initializePartner(partnerId, 0, 0, 0, 0, 0, 0);
			setDigimonRaised(partnerId);
			MAIN_func_800D56E0();
			initializeChest();
			runMapHeadScript(0xCC);
			addClock();
			stopGameTime();
			break;
		case 1:
			loadStackedTIMFile(MAIN_D_8012CE8C);
			initializeTamer(0, 0, 0, 0, 0, 0, 0);
			initializePartner(MAIN_D_80155670[0], 0, 0, 0, 0, 0, 0);
			MAIN_func_800D56E0();
			initializeChest();
			runMapHeadScript(MAIN_D_80155725[0]);
			MAIN_func_800EF38C();
			addClock();
			if (getTamerState() != 0) {
				setTamerState(0);
			}
			break;
		case 2:
			finalizeMusic();
			playMovie(2, 1);
			initializeMusic();
			loadStackedTIMFile(MAIN_D_8012CE8C);
			initializeTamer(0, 0, 0, 0, 0, 0, 0);
			initializePartner(MAIN_D_80155670[0], 0, 0, 0, 0, 0, 0);
			MAIN_func_800D56E0();
			initializeChest();
			runMapHeadScript(MAIN_D_80155725[0]);
			MAIN_func_800EF38C();
			addClock();
			if (getTamerState() != 0) {
				setTamerState(0);
			}
			break;
		}

		fadeFromBlack(0x28);
		recalculatePPandArena();
		while (MAIN_STATE != 3) {
			gameLoop();
		}
		cleanupGame();
		finalizeMusic();
		playMovie(3, 1);
	}
}

void initializeHeap(void)
{
	unsigned long heap;
	unsigned long stackTop;
	unsigned long stackFrames;

	heap = (SECTION_DATA.bssEnd & 0xfffffff0) + 0x10;
	stackTop = (unsigned long)SECTION_DATA.stackTop;
	stackFrames = SECTION_DATA.stackFrames;

	InitHeap3((unsigned long *)heap,
		  ((stackTop - (stackFrames * 0x400)) & 0xfffffff0) - heap);
}

void initializeFramebuffer(void)
{
	DVECTOR ofs0;
	DVECTOR ofs1;
	RECT rect;

	ofs0 = MAIN_D_801344DC;
	ofs1 = MAIN_D_801344E0;

	SetDispMask(0);
	GsInitGraph(320, 240, 4, 0, 0);
	GsDefDispBuff(0, 0, 0, 240);

	rect.x = 0;
	rect.y = 0;
	rect.w = 320;
	rect.h = 480;
	ClearImage(&rect, 0, 0, 0);

	GS_ORDERING_TABLE[0].length = 12;
	GS_ORDERING_TABLE[0].org = GS_ORDERING_TABLE_0;
	GS_ORDERING_TABLE[1].length = 12;
	GS_ORDERING_TABLE[1].org = GS_ORDERING_TABLE_1;

	GsInit3D();

	DRAWING_OFFSET_X = 160;
	DRAWING_OFFSET_Y = 120;

	SetDrawOffset(&DRAW_OFFSETS[0], (u_short *)&ofs1);
	SetDrawOffset(&DRAW_OFFSETS[1], (u_short *)&ofs0);

	MAP_LAYER_ENABLED = 1;

	SetDispMask(1);
}

INCLUDE_ASM("asm/main/nonmatchings/main", runLandingScreen);

INCLUDE_ASM("asm/main/nonmatchings/main", runMainMenu);

INCLUDE_ASM("asm/main/nonmatchings/main", newGameScene);

INCLUDE_ASM("asm/main/nonmatchings/main", MAIN_func_800EF38C);

void recalculatePPandArena(void)
{
	uint8_t pp;
	int32_t i;

	pp = 0;
	for (i = 3; i < 0x3B; i++) {
		if ((DIGIMON_DATA[i].level >= 3) && (isTriggerSet((uint16_t)(0xC8 + i)) != 0)) {
			if ((i == 0xB) || (i == 0x27) || (i == 0x35)) {
				pp++;
			} else {
				pp += (uint8_t)(DIGIMON_DATA[i].level - 2);
			}
		}
	}
	writePStat(1, pp);
	pp = readPStat(3);
	if (pp >= 0x17) {
		if (isTriggerSet(0x25) != 0) {
			unsetTrigger(0x25);
		}
		if (isTriggerSet(0x26) != 0) {
			unsetTrigger(0x26);
		}
		if (isTriggerSet(0x27) != 0) {
			unsetTrigger(0x27);
		}
	}
}

INCLUDE_ASM("asm/main/nonmatchings/main", gameLoop);

void applyHUDOffset(int32_t offset)
{
	AddPrim((char *)GS_ORDERING_TABLE[offset].org + 0x80,
		&DRAW_OFFSETS[offset]);
}

void pollInputGame(void)
{
	int32_t held;

	POLLED_INPUT_PREVIOUS = POLLED_INPUT;
	POLLED_INPUT = PadRead(0);
	CHANGED_INPUT = POLLED_INPUT & ~POLLED_INPUT_PREVIOUS;
	held = POLLED_INPUT & POLLED_INPUT_PREVIOUS & 0xF000F000;
	if (held != 0) {
		int32_t count;

		count = MAIN_D_80134EA4 + 1;
		MAIN_D_80134EA4 = count;
		if (count >= 5) {
			MAIN_D_80134EA4 -= 2;
		} else {
			held = 0;
		}
	} else {
		MAIN_D_80134EA4 = 0;
	}
	CHANGED_INPUT |= held;
}

void pollInputMenu(void)
{
	int32_t held;

	POLLED_INPUT_PREVIOUS = POLLED_INPUT;
	POLLED_INPUT = PadRead(0);
	CHANGED_INPUT = POLLED_INPUT & ~POLLED_INPUT_PREVIOUS;
	held = POLLED_INPUT & POLLED_INPUT_PREVIOUS & 0xF000F000;
	if (held != 0) {
		int32_t count;

		count = MAIN_D_80134EA8 + 1;
		MAIN_D_80134EA8 = count;
		if (count >= 0xF) {
			MAIN_D_80134EA8 -= 6;
		} else {
			held = 0;
		}
	} else {
		MAIN_D_80134EA8 = 0;
	}
	CHANGED_INPUT |= held;
}

INCLUDE_ASM("asm/main/nonmatchings/main", renderPressStartToContinue);

void renderMainMenuBackground(void)
{
	GsOT_TAG *ot;
	POLY_FT4 *prim;

	ot = ACTIVE_ORDERING_TABLE->org;
	prim = (POLY_FT4 *)GsGetWorkBase();

	SetPolyFT4(prim);
	prim->x0 = -0xA0;
	prim->y0 = -0x78;
	prim->x1 = 0x60;
	prim->y1 = -0x78;
	prim->x2 = -0xA0;
	prim->y2 = 0x78;
	prim->x3 = 0x60;
	prim->y3 = 0x78;
	prim->u0 = 0;
	prim->v0 = 0;
	prim->u1 = 0xFF;
	prim->v1 = 0;
	prim->u2 = 0;
	prim->v2 = 0xF0;
	prim->u3 = 0xFF;
	prim->v3 = 0xF0;
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = GetTPage(1, 0, 0x300, 0);
	prim->clut = GetClut(0, 0x1E0);
	AddPrim(ot = (GsOT_TAG *)((uint32_t)ot + 0x78), prim++);

	SetPolyFT4(prim);
	prim->x0 = 0x60;
	prim->y0 = -0x78;
	prim->x1 = 0xA0;
	prim->y1 = -0x78;
	prim->x2 = 0x60;
	prim->y2 = 0x78;
	prim->x3 = 0xA0;
	prim->y3 = 0x78;
	prim->u0 = 0;
	prim->v0 = 0;
	prim->u1 = 0x40;
	prim->v1 = 0;
	prim->u2 = 0;
	prim->v2 = 0xF0;
	prim->u3 = 0x40;
	prim->v3 = 0xF0;
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = GetTPage(1, 0, 0x380, 0);
	prim->clut = GetClut(0, 0x1E0);
	AddPrim(ot, prim++);
	GsSetWorkBase((PACKET *)prim);
}

INCLUDE_ASM("asm/main/nonmatchings/main", view_init);

void initializeEffectData(void)
{
	initializeEFE();
	initializeParticleFX();
	initializeEntityParticleFX();
	initializeCloudFXData();
	initializeHealingParticles();
}

void initializeInventoryModules(void)
{
	COMBAT_DATA_PTR = &COMBAT_DATA;
	resetFlattenGlobal();
	initializeDroppedItems();
	resetInventoryFlags();
	initializeInventory();
	MAIN_func_800DF804();
}

void addThrownItem(int32_t type)
{
	MATRIX *workm;

	if (TAMER_ITEM.worldItem.type == 0xFF) {
		workm = &TAMER_ENTITY.entity.posData[9].posMatrix.workm;
		TAMER_ITEM.worldItem.spriteLocation.vx = workm->t[0];
		TAMER_ITEM.worldItem.spriteLocation.vy = workm->t[1];
		TAMER_ITEM.worldItem.spriteLocation.vz = workm->t[2];
		TAMER_ITEM.time = 0;
		TAMER_ITEM.worldItem.type = type;
		addObject(0x194, 0, tickThrownItem, renderThrownItem);
	}
}

void startThrowingItem(void)
{
	startAnimation(ENTITY_TABLE[0], 6);
	addThrownItem(INVENTORY_ITEM_TYPES[INVENTORY_POINTER]);
	removeItem(INVENTORY_ITEM_TYPES[INVENTORY_POINTER], 1);
}

INCLUDE_ASM("asm/main/nonmatchings/main", tickThrownItem);

void renderThrownItem(int32_t instanceId)
{
	renderOverworldItem(&TAMER_ITEM);
}

void handleBuffDisks(int32_t type)
{
	BaseStats *stats;
	CurrentStats *current;
	int16_t offLimit;
	int16_t defLimit;
	int16_t speedLimit;
	int16_t gain;

	current = &PARTNER_ENTITY.digimonEntity.stats.current;

	if (current->currentHP == 0) {
		return;
	}

	offLimit = (INITIAL_COMBAT_STATS[0].offense +
		    ((INITIAL_COMBAT_STATS[0].offense * 3) / 10));
	if (offLimit > 999) {
		offLimit = 999;
	}

	defLimit = (INITIAL_COMBAT_STATS[0].defense +
		    ((INITIAL_COMBAT_STATS[0].defense * 3) / 10));
	if (defLimit > 999) {
		defLimit = 999;
	}

	speedLimit = (INITIAL_COMBAT_STATS[0].speed +
		      ((INITIAL_COMBAT_STATS[0].speed * 3) / 10));
	if (speedLimit > 999) {
		speedLimit = 999;
	}

	stats = &PARTNER_ENTITY.digimonEntity.stats.base;

	switch(type) {
	case 0xf:
		if (offLimit < (stats->off + 20)) {
			gain = offLimit - stats->off;
		}
		else {
			gain = 20;
		}
		BTL_buffStats(ENTITY_TABLE[1], 0, gain, &stats->off, 0xb, 3);
		break;
	case 0x10:
		if (defLimit < (stats->def + 20)) {
			gain = defLimit - stats->def;
		}
		else {
			gain = 20;
		}
		BTL_buffStats(ENTITY_TABLE[1], 0, gain, &stats->def, 0xb, 4);
		break;
	case 0x11:
		if (speedLimit < (stats->speed + 20)) {
			gain = speedLimit - stats->speed;
		}
		else {
			gain = 20;
		}
		BTL_buffStats(ENTITY_TABLE[1], 0, gain, &stats->speed, 0xb, 5);
		break;
	case 0x12:
		BTL_buffStats(ENTITY_TABLE[1], 0, 20, &stats->off, 0xb, 3);
		BTL_buffStats(ENTITY_TABLE[1], 0, 20, &stats->def, 0xb, 4);
		BTL_buffStats(ENTITY_TABLE[1], 0, 20, &stats->speed, 0xb, 5);
		break;
	case 0x13:
		if (offLimit < (stats->off + 50)) {
			gain = offLimit - stats->off;
		}
		else {
			gain = 50;
		}
		BTL_buffStats(ENTITY_TABLE[1], 0, gain, &stats->off, 0xb, 3);
		break;
	case 0x14:
		if (defLimit < (stats->def + 50)) {
			gain = defLimit - stats->def;
		}
		else {
			gain = 50;
		}
		BTL_buffStats(ENTITY_TABLE[1], 0, gain, &stats->def, 0xb, 4);
		break;
	case 0x15:
		if (speedLimit < (stats->speed + 50)) {
			gain = speedLimit - stats->speed;
		}
		else {
			gain = 50;
		}
		BTL_buffStats(ENTITY_TABLE[1], 0, gain, &stats->speed, 0xb, 5);
		break;
	}

	BTL_addBuffDiskEffect(ENTITY_TABLE[1]);
}

INCLUDE_ASM("asm/main/nonmatchings/main", MAIN_func_800F0B2C);

INCLUDE_ASM("asm/main/nonmatchings/main", MAIN_func_800F1020);

INCLUDE_ASM("asm/main/nonmatchings/main", tickTamerBattle);

void tickPartnerBattle(int32_t instanceId)
{
	tickAnimation(ENTITY_TABLE[instanceId]);
}

void tickNPCBattle(int32_t instanceId)
{
	Entity *entity;

	tickAnimation(ENTITY_TABLE[instanceId]);
	entity = ENTITY_TABLE[instanceId];
	if (entity->anim.animFlag & 4) {
		tickAnimation(entity);
	}
}

void tickNewGameJijimon(int32_t instanceId)
{
	int16_t *p;

	p = &MAIN_D_80134F10;
	if (*p < 0x7530) {
		*p += 1;
	}
	if (*p == 0x23) {
		setEntityRotation(2, 0, 0x71, 0);
		setupEntityMatrix(2);
		startAnimation(ENTITY_TABLE[2], 0);
		writePStat(0xF3, 0);
	}
	tickAnimation(ENTITY_TABLE[2]);
}

void loadNewgameScene(void)
{
	GsRVIEW2 view;

	thunkLoadMMD(0x75, 0);
	ENTITY_TABLE[2] = &NPC_ENTITIES[0].digimonEntity.entity;
	initializeDigimonObject(0x75, 2, tickNewGameJijimon);
	ENTITY_TABLE[2]->isOnMap = 1;
	ENTITY_TABLE[2]->isOnScreen = 1;
	MAIN_D_80134F10 = 0;
	setEntityPosition(2, 0x320, 0x96, 0);
	setEntityRotation(2, 0, 0x400, 0);
	setupEntityMatrix(2);
	startAnimation(ENTITY_TABLE[2], 2);
	GsSetProjection(0x3E8);
	view.vpx = 0;
	view.vpz = -0x1068;
	view.vpy = 0;
	view.vrx = 0;
	view.vry = 0;
	view.vrz = 0;
	view.rz = 0;
	view.super = NULL;
	GsSetRefView2(&view);
	DRAWING_OFFSET_X = 0xA0;
	DRAWING_OFFSET_Y = 0xB9;
	LIGHT_DATA[0].vx = 0x1E;
	LIGHT_DATA[0].vy = 0x64;
	LIGHT_DATA[0].vz = 0x1E;
	LIGHT_DATA[0].r = 0x40;
	LIGHT_DATA[0].g = 0x40;
	LIGHT_DATA[0].b = 0x40;
	GsSetFlatLight(0, &LIGHT_DATA[0]);
	LIGHT_DATA[1].vx = -0x1E;
	LIGHT_DATA[1].vy = 0x64;
	LIGHT_DATA[1].vz = 0;
	LIGHT_DATA[1].r = 0x28;
	LIGHT_DATA[1].g = 0x28;
	LIGHT_DATA[1].b = 0x28;
	GsSetFlatLight(1, &LIGHT_DATA[1]);
	LIGHT_DATA[2].vx = 0;
	LIGHT_DATA[2].vy = 0x64;
	LIGHT_DATA[2].vz = -0x1E;
	LIGHT_DATA[2].r = 0x26;
	LIGHT_DATA[2].g = 0x26;
	LIGHT_DATA[2].b = 0x26;
	GsSetFlatLight(2, &LIGHT_DATA[2]);
	GsSetAmbient(0x800, 0x800, 0x800);
}

void unloadNewGameScene(void)
{
	removeEntity(0x75, 2);
	thunkUnloadModel(0x75, 0);
}

void tickBuffModelObject(int32_t instanceId)
{
	MAIN_func_800F179C(BUFF_MODEL[0], 5, buffModelValue[buffModelFrame & 1]);
	buffModelFrame += 1;
}

void MAIN_func_800F1794(void)
{
}

void MAIN_func_800F179C(void *model, int32_t compIdx, int32_t color)
{
  char *p;
  int32_t new_var;
  int32_t new_var2;
  char *comp;
  int32_t i;
  int32_t attr;
  int new_var3;
  comp = (char *) model;
  comp = comp + 0xC;
  comp = comp + (compIdx * 0x1C);
  new_var = *((int32_t *) (comp + 0x14));
  new_var3 = 0x10;
  p = *((char **) (comp + new_var3));
  for (i = 0; i < new_var; i++)
  {
    attr = ((*((int32_t *) p)) >> 0x18) & 0xFF;
    if (attr & 4)
    {
      attr = ((*((int32_t *) p)) >> 0x18) & 0xFF;
      *((int16_t *) (p + 6)) = (int16_t) color;
      new_var2 = attr;
      if (new_var2 & 8)
      {
        if (!(attr & new_var3))
        {
          p += 0x20;
        }
        else
        {
          p += 0x2C;
        }
      }
      else
        if (!(attr & new_var3))
      {
        p += 0x1C;
      }
      else
      {
        p += 0x24;
      }
    }
  }

}

void initializeBuffModel(TMDModel *model)
{
	BUFF_MODEL[0] = model;
	GsMapModelingData((unsigned long *)&BUFF_MODEL[0]->flags);
	buffModelValue[0] = 0x7ACC;
	buffModelValue[1] = 0x7B0C;
}

int32_t initializeBuffModelObject(void)
{
	buffModelFrame = 0;
	return addObject(0x501, 0, tickBuffModelObject,
			 (RenderFunction)MAIN_func_800F1794);
}

int32_t removeBuffModelObject(void)
{
	return removeObject(0x501, 0);
}

int32_t isAsciiEncoded(const char *value)
{
	if ((*value >> 7) == 0) {
		return 1;
	}
	return 0;
}

uint16_t convertAsciiToJis(uint8_t input)
{
	uint8_t charType;
	uint8_t specialOffset;
	int32_t offset;

	specialOffset = 0;

	if ((input >= 0x20) && (input <= 0x2f)) {
		specialOffset = 0x1;
	}
	else if ((input >= 0x30) && (input <= 0x39)) {
		charType = 0x0;
	}
	else if ((input >= 0x3a) && (input <= 0x40)) {
		specialOffset = 0xb;
	}
	else if ((input >= 0x41) && (input <= 0x5a)) {
		charType = 0x1;
	}
	else if ((input >= 0x5b) && (input <= 0x60)) {
		specialOffset = 0x25;
	}
	else if ((input >= 0x61) && (input <= 0x7a)) {
		charType = 0x2;
	}
	else if ((input >= 0x7b) && (input <= 0x7e)) {
		specialOffset = 0x3f;
	} else {
		return 0;
	}

	if (specialOffset != 0) {
		offset = (input - 0x20) - (specialOffset - 1);
		return CHAR_MAPPING_SPECIAL[offset].shiftJIS;
	} else {
		return (input +
			CHAR_MAPPING_NORMAL[charType].shiftJIS -
			CHAR_MAPPING_NORMAL[charType].ascii);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/main", asciiToShiftJIS);

int32_t swapShortBytes(int32_t input)
{
	return ((input >> 8) | (input << 8)) & 0xFFFF;
}
