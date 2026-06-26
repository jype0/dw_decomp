#include <stdlib.h>

#include <libgpu.h>
#include <libgs.h>

#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/item.h>
#include <dw/main.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

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

typedef struct {
	uint32_t id;
	uint32_t flags;
	uint32_t nobj;
} TMDModel;

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
void MAIN_func_800DADDC(void);
void initializeInventory(void);
void MAIN_func_800DF804(void);
void initializeInventoryModules(void);

void startAnimation(Entity *entity, uint8_t animId);
void removeItem(int32_t type, int32_t amount);

void initializeEFE(void);
void initializeParticleFX(void);
void initializeEntityParticleFX(void);
void initializeCloudFXData(void);
void initializeHealingParticles(void);

void BTL_buffStats(Entity *entity, int32_t entityId, int32_t amount,
		   int16_t* valuePtr, uint8_t color, uint8_t icon);
void BTL_addBuffDiskEffect(Entity *entity);

INCLUDE_ASM("asm/main/nonmatchings/main", main);

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

INCLUDE_ASM("asm/main/nonmatchings/main", recalculatePPandArena);

INCLUDE_ASM("asm/main/nonmatchings/main", gameLoop);

INCLUDE_ASM("asm/main/nonmatchings/main", applyHUDOffset);

INCLUDE_ASM("asm/main/nonmatchings/main", pollInputGame);

INCLUDE_ASM("asm/main/nonmatchings/main", pollInputMenu);

INCLUDE_ASM("asm/main/nonmatchings/main", renderPressStartToContinue);

INCLUDE_ASM("asm/main/nonmatchings/main", renderMainMenuBackground);

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
	MAIN_func_800DADDC();
	initializeInventory();
	MAIN_func_800DF804();
}

INCLUDE_ASM("asm/main/nonmatchings/main", addThrownItem);

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

INCLUDE_ASM("asm/main/nonmatchings/main", tickNewGameJijimon);

INCLUDE_ASM("asm/main/nonmatchings/main", loadNewgameScene);

void unloadNewGameScene(void)
{
	removeEntity(0x75, 2);
	thunkUnloadModel(0x75, 0);
}

INCLUDE_ASM("asm/main/nonmatchings/main", tickBuffModelObject);

void MAIN_func_800F1794(void)
{
}

INCLUDE_ASM("asm/main/nonmatchings/main", MAIN_func_800F179C);

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
