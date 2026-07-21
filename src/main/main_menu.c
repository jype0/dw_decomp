#include <libgs.h>

#include <libmcrd.h>
#include <stdio.h>
#include <string.h>

#include <dw/entity.h>
#include <dw/params.h>

#include <dw/types.h>

#include "common.h"

extern int32_t TARGET_MENU;
extern int32_t CURRENT_MENU;
extern int32_t MEMORY_CARD_ID;
extern int32_t MEMORY_CARD_SLOT;
extern int32_t CHECKED_MEMORY_CARD;
extern int8_t MAIN_STATE;
extern uint8_t CURRENT_SCREEN;
extern int32_t MAIN_D_80135040;
extern int32_t MAIN_D_8013506C;
extern char MAIN_D_801346CC;
extern int32_t MAIN_D_80135030;
extern int32_t MAIN_D_80135050;
extern int32_t MAIN_D_80135060;
extern uint8_t MAIN_D_80131B2C[];
extern uint8_t MAIN_D_8013192C[];
long MemCardExist();
void clearTextSubArea(RECT *area);
void drawString(char *text, int32_t color, int32_t pos);
extern char MAIN_D_80131658[];
extern char MAIN_D_801BF768[];
extern int32_t CHANGED_INPUT;
void playSound(int32_t vabId, uint32_t note);

typedef struct {
	int8_t pos;
	int8_t count;
	int8_t unk2;
	int8_t scroll;
	int8_t max;
} MenuCursor;


extern char MAIN_D_801346C0;
extern struct DIRENTRY *volatile MAIN_D_801346D0;

#include <dw/entity.h>
#include <dw/params.h>

int8_t getFileCityTopMap(void);
extern GsOT *ACTIVE_ORDERING_TABLE;
extern int32_t MAIN_D_80135034;
extern int32_t MAIN_D_80135038;
extern int8_t MAIN_D_8013172B;
extern char *MOVE_NAMES[];
extern TamerEntity TAMER_ENTITY;
extern uint8_t INVENTORY_ITEM_TYPES[30];
extern int16_t HOUR;
extern int16_t MINUTE;
extern int16_t DAY;
extern uint8_t YEAR;
extern uint16_t CURRENT_FRAME;
extern uint16_t LAST_HANDLED_FRAME;
extern uint16_t PLAYTIME_FRAMES;
extern uint16_t PLAYTIME_HOURS;
extern uint16_t PLAYTIME_MINUTES;
extern int32_t MONEY;
extern uint8_t PREVIOUS_SCREEN;
extern uint8_t CURRENT_EXIT;
extern uint8_t PREVIOUS_EXIT;
extern int16_t MERIT;
extern int16_t MAIN_D_80134FC8;
extern int16_t MAIN_D_80134FCA;
extern int16_t MAIN_D_80134FCC;
extern int16_t MAIN_D_80134FD0;
extern int16_t TOURNAMENTS_LOST;
extern void *MAIN_D_80134FB8;
extern char WORLD_POOP;
extern char TAMER_WAYPOINT_X;
extern char TAMER_WAYPOINT_Y;
extern int8_t TAMER_PREVIOUS_TILE_X;
extern int8_t TAMER_PREVIOUS_TILE_Y;
extern int8_t TAMER_WAYPOINT_CURRENT;
extern int8_t TAMER_WAYPOINT_COUNT;
extern int8_t TAMER_START_TILE_X;
extern int8_t TAMER_START_TILE_Y;
extern int8_t TAMER_WAYPOINT_ACTIVE;
void recalculatePPandArena(void);
extern int32_t SAVE_TAMER_POS[];
extern int32_t MAIN_D_801555D4[];
extern int32_t MAIN_D_801555D8[];
extern int32_t MAIN_D_801555DC[];
extern int32_t SAVE_PARTNER_POS[];
extern int32_t MAIN_D_801555E4[];
extern int32_t MAIN_D_801555E8[];
extern int32_t MAIN_D_801555EC[];
extern int32_t MAIN_D_80155670[];
extern int32_t MAIN_D_80155674[];
extern int16_t SAVE_STATS[];
extern int32_t SAVE_PARTNER_PARA[];
extern int8_t MAIN_D_8015571C[];
extern uint8_t MAIN_D_8015571D[];
extern uint8_t MAIN_D_80155725[];
extern uint8_t SAVE_PREVIOUS_SCREEN[];
extern uint8_t SAVE_CURRENT_EXIT[];
extern uint8_t SAVE_PREVIOUS_EXIT[];
extern char SAVE_TAMER_WAYPOINT_X[];
extern char SAVE_TAMER_WAYPOINT_Y[];
extern int8_t SAVE_TAMER_PREVIOUS_TILE_X[];
extern int8_t SAVE_TAMER_PREVIOUS_TILE_Y[];
extern int8_t SAVE_TAMER_WAYPOINT_CURRENT[];
extern int8_t SAVE_TAMER_WAYPOINT_COUNT[];
extern int8_t SAVE_TAMER_START_TILE_X[];
extern int8_t SAVE_TAMER_START_TILE_Y[];
extern int8_t SAVE_TAMER_WAYPOINT_ACTIVE[];
int32_t readPStat(int32_t id);
void writePStat(int32_t id, int32_t value);
void setTextColor(int32_t color);
extern uint8_t MAIN_D_80131638[];
extern uint8_t MAIN_D_801315F8[];
extern uint8_t MAIN_D_80131618[];
extern char MAIN_D_801346C4;
extern char MAIN_D_8013468C;
extern uint8_t MAIN_D_80131639[];
extern uint8_t MAIN_D_8013163A[];
void renderUIBoxBorder(int16_t *rect, int32_t flag);
void MAIN_func_80092B9C(POLY_G4 *prim);

void MAIN_func_8010D034();
void renderText(POLY_FT4 *prim, int32_t x, int32_t y, uint8_t u, int32_t v,
		int32_t w, int32_t h, int32_t textColor);
void renderMenuBox(int32_t x, int32_t y, int16_t w, int16_t h);
void MAIN_func_8010D3E0();
void MAIN_func_8010D554();
void MAIN_func_8010D694(void);
void MAIN_func_8010D70C();
void renderSaveSlotBox(int32_t slot, int32_t x, int32_t y);
void MAIN_func_8010DA44();
void renderContinueSaveSelection();
void MAIN_func_8010DEBC(void);
void MAIN_func_8010DFAC();
void MAIN_func_8010E0C8(void);
void MAIN_func_8010E16C();
void MAIN_func_8010E350();
void MAIN_func_8010E4B8();
void MAIN_func_8010E638(void);
void MAIN_func_8010E73C();
void MAIN_func_8010E8C0(void);
void MAIN_func_8010E938(void);
void MAIN_func_8010EA1C(void);
void drawMainMenuStrings();
void drawSaveSlotText(int32_t slot, int32_t row);
char *MAIN_func_8010FB7C(int32_t value, char *buf);
void MAIN_func_8010FBB0();
void MAIN_func_8010FC48();
void updateMemoryCardState();
void tickMainMenu();
int32_t MAIN_func_8011239C(MenuCursor *cursor, int32_t which);
int32_t MAIN_func_80112524(int32_t menu);
void setMemoryCardReadError(int32_t id, int32_t slot);
int32_t MAIN_func_801125A8(int32_t unused, int32_t mode);
int32_t MAIN_func_80112628(int32_t channel, int32_t slot);
void loadSaveSlotData();
int32_t MAIN_func_8011296C(MenuCursor *cursor, int32_t which);
void initializeDefaultSavegame();
char *_strncpy(char *dst, char *src, int32_t n);
int32_t createSavegameChecksum(int32_t slot);
void loadSavegame(uint8_t *sv);
void writeSavegame(uint8_t *sv);
int32_t MAIN_func_8011341C(uint8_t *p);
void renderMainMenu();
void registerBattleData();
typedef struct {
	int8_t pos;
	int8_t pad[5];
	int16_t rowHeight;
	int16_t x;
	int16_t y;
	int16_t w;
	int16_t h;
} MenuHighlight;
void MAIN_func_801136C8(MenuHighlight *b);
int32_t createByteSum(uint8_t *data, int32_t len);
void openSaveMachine(void);
int32_t MAIN_func_801138B0(void);
void awardMachinedramonData();
void gameClearSave();
int32_t MAIN_func_80113A20(void);

void *main_menu_order_anchor[] = {
	MAIN_func_80113A20,
	gameClearSave,
	awardMachinedramonData,
	MAIN_func_801138B0,
	openSaveMachine,
	createByteSum,
	MAIN_func_801136C8,
	registerBattleData,
	renderMainMenu,
	MAIN_func_8011341C,
	writeSavegame,
	loadSavegame,
	createSavegameChecksum,
	_strncpy,
	initializeDefaultSavegame,
	MAIN_func_8011296C,
	loadSaveSlotData,
	MAIN_func_80112628,
	MAIN_func_801125A8,
	setMemoryCardReadError,
	MAIN_func_80112524,
	MAIN_func_8011239C,
	tickMainMenu,
	updateMemoryCardState,
	MAIN_func_8010FC48,
	MAIN_func_8010FBB0,
	MAIN_func_8010FB7C,
	drawSaveSlotText,
	drawMainMenuStrings,
	MAIN_func_8010EA1C,
	MAIN_func_8010E938,
	MAIN_func_8010E8C0,
	MAIN_func_8010E73C,
	MAIN_func_8010E638,
	MAIN_func_8010E4B8,
	MAIN_func_8010E350,
	MAIN_func_8010E16C,
	MAIN_func_8010E0C8,
	MAIN_func_8010DFAC,
	MAIN_func_8010DEBC,
	renderContinueSaveSelection,
	MAIN_func_8010DA44,
	renderSaveSlotBox,
	MAIN_func_8010D70C,
	MAIN_func_8010D694,
	MAIN_func_8010D554,
	MAIN_func_8010D3E0,
	renderMenuBox,
	renderText,
	MAIN_func_8010D034,
};

INCLUDE_ASM("asm/main/nonmatchings/main_menu", MAIN_func_8010D034);

void renderText(POLY_FT4 *prim, int32_t x, int32_t y, uint8_t u, int32_t v,
		int32_t w, int32_t h, int32_t textColor)
{
	x -= 0xA0;
	y -= 0x78;
	SetPolyFT4(prim);
	prim->tpage = 0x1B;
	prim->clut = GetClut(0xD0, 0x1E8);
	prim->r0 = MAIN_D_80131638[textColor * 4];
	prim->g0 = MAIN_D_80131639[textColor * 4];
	prim->b0 = MAIN_D_8013163A[textColor * 4];
	prim->x0 = x;
	prim->y0 = y;
	prim->x1 = x + w;
	prim->y1 = y;
	prim->x2 = x;
	prim->y2 = y + h;
	prim->x3 = x + w;
	prim->y3 = y + h;
	prim->u0 = u;
	prim->v0 = v;
	prim->u1 = u + w;
	prim->v1 = v;
	prim->u2 = u;
	prim->v2 = v + h;
	prim->u3 = u + w;
	prim->v3 = v + h;
	AddPrim(ACTIVE_ORDERING_TABLE->org, prim);
}

void renderMenuBox(int32_t x, int32_t y, int16_t w, int16_t h)
{
	int16_t rect[4];
	POLY_G4 *prim;
	x -= 0xA0;
	y -= 0x78;
	rect[0] = x;
	rect[1] = y;
	rect[2] = w;
	rect[3] = h;
	renderUIBoxBorder(rect, 0);
	prim = (POLY_G4 *)GsGetWorkBase();
	MAIN_func_80092B9C(prim);
	prim->r0 = 0;
	prim->g0 = 0;
	prim->b0 = 0x70;
	prim->r1 = 0;
	prim->g1 = 0;
	prim->b1 = 0x50;
	prim->r2 = 0;
	prim->g2 = 0;
	prim->b2 = 0x30;
	prim->r3 = 0;
	prim->g3 = 0;
	prim->b3 = 0x10;
	prim->x0 = x + 3;
	prim->y0 = y + 3;
	prim->x1 = (x + 3) + (w - 6);
	prim->y1 = y + 3;
	prim->x2 = x + 3;
	prim->y2 = (y + 3) + (h - 6);
	prim->x3 = (x + 3) + (w - 6);
	prim->y3 = (y + 3) + (h - 6);
	AddPrim(ACTIVE_ORDERING_TABLE->org, prim++);
	GsSetWorkBase((PACKET *)prim);
}

void MAIN_func_8010D3E0(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
	if (MAIN_D_80135030 & 1) {
		renderText(cur++, 0x4B, 0x37, 0, 0, 0xBE, 0xC, 0);
	} else {
		renderText(cur++, 0x4B, 0x37, 0, 0, 0xBE, 0xC, 1);
	}
	if (MAIN_D_80135030 & 0x10) {
		renderText(cur++, 0x4B, 0x43, 0, 0xC, 0xBE, 0xC, 0);
	} else {
		renderText(cur++, 0x4B, 0x43, 0, 0xC, 0xBE, 0xC, 1);
	}
	renderText(cur++, 0x4B, 0x4F, 0, 0x18, 0xBE, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x41, 0x32, 0xBE, 0x2E);
}

void MAIN_func_8010D554(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
	if (MAIN_D_80135030 & 1) {
		renderText(cur++, 0x46, 0x37, 0, 0, 0xBE, 0xC, 0);
	} else {
		renderText(cur++, 0x46, 0x37, 0, 0, 0xBE, 0xC, 1);
	}
	if (MAIN_D_80135030 & 0x10) {
		renderText(cur++, 0x46, 0x43, 0, 0xC, 0xBE, 0xC, 0);
	} else {
		renderText(cur++, 0x46, 0x43, 0, 0xC, 0xBE, 0xC, 1);
	}
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3C, 0x32, 0xB7, 0x22);
}

void MAIN_func_8010D694(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x40, 0x37, 0, 0, 0xC0, 0x24, 2);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x36, 0x32, 0xD4, 0x2E);
}

void MAIN_func_8010D70C(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x3A, 0x18, 0, 0, 0xBE, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x30, 0x13, 0xBE, 0x16);
	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x22, 0x5A, 0, 0xC, 0xFC, 0x3C, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x18, 0x55, 0x110, 0x46);
	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x6E, 0x37, 0, 0xF0, 0x1C, 0xC, 0);
	renderText(cur++, 0x6E, 0x43, 0x1C, 0xF0, 0x1C, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x64, 0x32, 0x38, 0x22);
}

void renderSaveSlotBox(int32_t slot, int32_t x, int32_t y)
{
	POLY_FT4 *cur;
	int32_t hl;
	int32_t v;
	int32_t idx;
	v = ((slot % 7) * 0x18) + 0xC;
	cur = (POLY_FT4 *)GsGetWorkBase();
	idx = slot * 0x11;
	if ((((int32_t *)MAIN_D_801BF768)[idx] != 0 && MAIN_D_80135034 != 0) ||
	    (((int32_t *)MAIN_D_801BF768)[idx] == 0 && MAIN_D_80135034 == 0)) {
		hl = 0;
	} else {
		hl = 1;
	}
	renderText(cur++, x + 0xA, y + 5, 0, v, 0x18, 0xC, hl);
	if (((int32_t *)MAIN_D_801BF768)[idx] != 0) {
		renderText(cur++, x + 0x28, y + 5, 0x18, v, 0x48, 0xC, hl);
		renderText(cur++, x + 0x76, y + 5, 0x6C, v, 0x60, 0xC, hl);
		renderText(cur++, x + 0x28, y + 0x13, 0x18, v + 0xC, 0xE0, 0xC, hl);
	}
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(x, y, 0xE0, 0x24);
}

INCLUDE_ASM("asm/main/nonmatchings/main_menu", MAIN_func_8010DA44);

void renderContinueSaveSelection(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x3A, 0x18, 0, 0, 0x90, 0xC, 0);
	renderText(cur++, 0x3A, 0x2E, 0, 0xC, 0x18, 0xC, 0);
	if (((int32_t *)MAIN_D_801BF768)[MEMORY_CARD_SLOT * 0x11] != 0) {
		renderText(cur++, 0x58, 0x2E, 0x18, 0xC, 0x48, 0xC, 0);
		renderText(cur++, 0xA6, 0x2E, 0x6C, 0xC, 0x60, 0xC, 0);
		renderText(cur++, 0x58, 0x3C, 0, 0x18, 0xE0, 0xC, 0);
	} else {
		renderText(cur++, 0x58, 0x2E, 0x90, 0, 0x6C, 0xC, 0);
	}
	renderText(cur++, 0x3A, 0x52, 0, 0xF0, 0x1C, 0xC, 0);
	renderText(cur++, 0x3A, 0x5E, 0x1C, 0xF0, 0x1C, 0xC, 0);
	renderText(cur++, 0x3A, 0x74, 0x14, 0x30, 0xCC, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x30, 0x13, 0xB0, 0x16);
	renderMenuBox(0x30, 0x29, 0xE0, 0x24);
	renderMenuBox(0x30, 0x4D, 0x38, 0x22);
	renderMenuBox(0x30, 0x6F, 0xE0, 0x16);
}

void MAIN_func_8010DEBC(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x4C, 0x37, 0, 0, 0xA8, 0x24, 2);
	renderText(cur++, 0x4C, 0x65, 0, 0xF0, 0x1C, 0xC, 0);
	renderText(cur++, 0x4C, 0x71, 0x1C, 0xF0, 0x1C, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x42, 0x32, 0xBC, 0x2E);
	renderMenuBox(0x42, 0x60, 0x38, 0x22);
}

void MAIN_func_8010DFAC(void)
{
	POLY_FT4 *cur;

	if ((MAIN_D_80135040 == 1) || (MAIN_D_80135040 == 3) || (MAIN_D_80135040 == 4) ||
	    (MAIN_D_80135040 == 5) || (MAIN_D_80135040 == 7)) {
		cur = (POLY_FT4 *)GsGetWorkBase();
		renderText(cur++, 0x4C, 0x37, 0, 0, 0xA8, 0x18, 3);
		GsSetWorkBase((PACKET *)cur);
		renderMenuBox(0x42, 0x32, 0xBC, 0x22);
		return;
	}
	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x40, 0x37, 0, 0xC, 0xC0, 0xC, 3);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x36, 0x32, 0xD4, 0x16);
}

void MAIN_func_8010E0C8(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x76, 0x6A, 0, 0, 0x54, 0xC, 0);
	renderText(cur++, 0x76, 0x7A, 0, 0xC, 0x54, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x6C, 0x65, 0x68, 0x26);
}

void MAIN_func_8010E16C(void)
{
	POLY_FT4 *cur;
	int32_t mask;

	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x40, 0x38, 0, 0, 0xD4, 0x18, 0);
	if (MEMORY_CARD_ID == 0) {
		mask = 1;
	} else {
		mask = 0x10;
	}
	if (!(MAIN_D_80135030 & mask)) {
		MAIN_D_80135040 = 1;
		renderText(cur++, 0x40, 0x5A, 0, 0xF0, 0x1C, 0xC, 1);
	} else {
		if (MAIN_D_80135040 == 1) {
			MAIN_D_80135040 = -1;
		}
		renderText(cur++, 0x40, 0x5A, 0, 0xF0, 0x1C, 0xC, 0);
	}
	renderText(cur++, 0x40, 0x66, 0x1C, 0xF0, 0x1C, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x36, 0x33, 0xE8, 0x22);
	renderMenuBox(0x36, 0x55, 0x38, 0x22);
	if (MAIN_D_80135040 != -1) {
		cur = (POLY_FT4 *)GsGetWorkBase();
		renderText(cur++, 0x4C, 0x82, 0, 0x24, 0xA8, 0x18, 2);
		GsSetWorkBase((PACKET *)cur);
		renderMenuBox(0x42, 0x7D, 0xBC, 0x22);
	}
}

void MAIN_func_8010E350(void)
{
	POLY_FT4 *cur;
	int32_t mask;
	int32_t hl;

	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x46, 0x21, 0, 0, 0xD6, 0xC, 0);
	renderText(cur++, 0x46, 0x37, 0, 0xC, 0xDC, 0x24, 0);
	if (MEMORY_CARD_ID == 0) {
		mask = 1;
	} else {
		mask = 0x10;
	}
	if (MAIN_D_80135030 & mask) {
		hl = 0;
	} else {
		hl = 1;
	}
	renderText(cur++, 0x46, 0x65, 0, 0xF0, 0x1C, 0xC, hl);
	renderText(cur++, 0x46, 0x71, 0x1C, 0xF0, 0x1C, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3C, 0x1C, 0xE0, 0x16);
	renderMenuBox(0x3C, 0x32, 0xE6, 0x2E);
	renderMenuBox(0x3C, 0x60, 0x38, 0x22);
}

void MAIN_func_8010E4B8(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x32, 0x21, 0, 0, 0xE2, 0xC, 0);
	renderText(cur++, 0x32, 0x37, 0, 0xC, 0xD6, 0x6C, 0);
	renderText(cur++, 0x46, 0xAD, 0, 0x78, 0xB4, 0xC, 0);
	renderText(cur++, 0x46, 0xC3, 0, 0x84, 0x24, 0xC, 0);
	renderText(cur++, 0x46, 0xCF, 0, 0x90, 0x24, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x28, 0x1C, 0xF6, 0x16);
	renderMenuBox(0x28, 0x32, 0xE0, 0x76);
	renderMenuBox(0x3C, 0xA8, 0xC8, 0x16);
	renderMenuBox(0x3C, 0xBE, 0x38, 0x22);
}

void MAIN_func_8010E638(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x32, 0x21, 0, 0, 0xE2, 0xC, 0);
	renderText(cur++, 0x46, 0x37, 0, 0xC, 0xE0, 0x78, 0);
	renderText(cur++, 0x46, 0xB9, 0, 0x84, 0xCA, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x28, 0x1C, 0xF6, 0x16);
	renderMenuBox(0x3C, 0x32, 0xE0, 0x82);
	renderMenuBox(0x3C, 0xB4, 0xD4, 0x16);
}

void MAIN_func_8010E73C(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
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
}

void MAIN_func_8010E8C0(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x22, 0x46, 0, 0, 0xFC, 0x30, 3);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x18, 0x40, 0x110, 0x3A);
}

void MAIN_func_8010E938(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x46, 0x3C, 0, 0, 0xA8, 0xC, 0);
	renderText(cur++, 0x46, 0x57, 0, 0xC, 0x1C, 0xC, 0);
	renderText(cur++, 0x46, 0x63, 0x1C, 0xC, 0x1C, 0xC, 0);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x3C, 0x32, 0xBC, 0x20);
	renderMenuBox(0x3C, 0x52, 0x38, 0x22);
}

void MAIN_func_8010EA1C(void)
{
	POLY_FT4 *cur;

	cur = (POLY_FT4 *)GsGetWorkBase();
	renderText(cur++, 0x2E, 0x45, 0, 0, 0xE4, 0x24, 3);
	GsSetWorkBase((PACKET *)cur);
	renderMenuBox(0x24, 0x40, 0xF8, 0x2E);
}

INCLUDE_ASM("asm/main/nonmatchings/main_menu", drawMainMenuStrings);

void drawSaveSlotText(int32_t slot, int32_t row)
{
	RECT area;

	if (slot >= 0 && slot < 0xF) {
		row *= 0x18;
		area.x = 0;
		area.y = row + 0xC;
		area.w = 0xE0;
		area.h = 0x18;
		clearTextSubArea(&area);
		drawString(&MAIN_D_80131658[(slot + 1) * 6], 0, row + 0xC);
		drawString(&MAIN_D_801BF768[slot * 0x44] + 4, 0x18, row + 0xC);
		DrawSync(0);
		drawString(&MAIN_D_801BF768[slot * 0x44] + 0x18, 0x6C, row + 0xC);
		DrawSync(0);
		drawString(&MAIN_D_801BF768[slot * 0x44] + 0x2C, 0x18, row + 0x18);
		DrawSync(0);
	}
}

char *MAIN_func_8010FB7C(int32_t value, char *buf)
{
	sprintf(buf, &MAIN_D_801346CC, value);
	return buf;
}

void MAIN_func_8010FBB0(int32_t type, int32_t anim, int32_t color, int32_t pos)
{
	int16_t move;

	if (anim >= 0x2E && anim < 0x3E) {
		move = DIGIMON_DATA[type].moves[anim - 0x2E];
		if (move >= 0 && move < 0x79) {
			drawString(MOVE_NAMES[move], color, pos);
		}
	}
}

INCLUDE_ASM("asm/main/nonmatchings/main_menu", MAIN_func_8010FC48);

void updateMemoryCardState(void)
{
	unsigned long result;
	unsigned long cmd;
	int32_t mask;

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
				if (CHECKED_MEMORY_CARD == 0x10) {
					mask = 0x10;
				} else {
					mask = 1;
				}
				MAIN_D_80135050 |= mask;
			case 0:
				if (CHECKED_MEMORY_CARD == 0x10) {
					mask = 0x10;
				} else {
					mask = 1;
				}
				MAIN_D_80135030 |= mask;
				CHECKED_MEMORY_CARD ^= 0x10;
				break;
			case 2:
			case 1:
			default:
				if (CHECKED_MEMORY_CARD == 0x10) {
					mask = -0x11;
				} else {
					mask = -2;
				}
				MAIN_D_80135030 &= mask;
				CHECKED_MEMORY_CARD ^= 0x10;
				break;
			}
		}
		return;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/main_menu", tickMainMenu);

int32_t MAIN_func_8011239C(MenuCursor *cursor, int32_t which)
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
	if (input == 0x10) {
		playSound(0, 4);
		return 2;
	}
	if (input == 0x40) {
		playSound(0, 3);
		return 1;
	}
	return 0;
}

int32_t MAIN_func_80112524(int32_t menu)
{
	int32_t mask;
	int32_t mask2;

	if (MEMORY_CARD_ID == 0) {
		mask = 1;
	} else {
		mask = 0x10;
	}
	if (!(MAIN_D_80135030 & mask)) {
		return menu;
	}
	if (MEMORY_CARD_ID == 0) {
		mask2 = 1;
	} else {
		mask2 = 0x10;
	}
	if (MAIN_D_80135050 & mask2) {
		return menu;
	}
	return TARGET_MENU;
}

void setMemoryCardReadError(int32_t id, int32_t slot)
{
	TARGET_MENU = 3;
	MAIN_D_80135040 = id;
	MAIN_D_8013506C = slot;
}

int32_t MAIN_func_801125A8(int32_t unused, int32_t mode)
{
	unsigned long cmd;
	unsigned long result;

	if ((mode == 0) || (mode == 3)) {
		return 0;
	}

	MemCardExist();
	MemCardSync(0, &cmd, &result);
	if ((result == 0) && (mode == 4)) {
		result = 4;
	}

	return result;
}

INCLUDE_ASM("asm/main/nonmatchings/main_menu", MAIN_func_80112628);

INCLUDE_ASM("asm/main/nonmatchings/main_menu", loadSaveSlotData);

int32_t MAIN_func_8011296C(MenuCursor *cursor, int32_t which)
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
	if (input == 0x10) {
		playSound(0, 4);
		return 2;
	}
	if (input == 0x40) {
		playSound(0, 3);
		return 1;
	}
	return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/main_menu", initializeDefaultSavegame);

char *_strncpy(char *dst, char *src, int32_t n)
{
	char c;
	int32_t i;

	i = 0;
	while (i < n) {
		c = *src;
		if (c == 0) {
			break;
		}

		src++;
		*dst++ = c;
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

INCLUDE_ASM("asm/main/nonmatchings/main_menu", createSavegameChecksum);

INCLUDE_ASM("asm/main/nonmatchings/main_menu", loadSavegame);

INCLUDE_ASM("asm/main/nonmatchings/main_menu", writeSavegame);

int32_t MAIN_func_8011341C(uint8_t *p)
{
	int32_t count;
	int32_t i;

	count = 0;
	i = 0;
	while (i < 0x28) {
		if (p[0x1C] != 0) {
			count++;
		}
		p = (uint8_t *)((uint32_t)p + 0x40);
		i++;
	}
	return count;
}

INCLUDE_ASM("asm/main/nonmatchings/main_menu", renderMainMenu);

void registerBattleData(void)
{
	int16_t *rec;

	rec = (int16_t *)(&MAIN_D_8013192C[MAIN_D_80135060 << 6] + 0x700);
	rec[0] = PARTNER_ENTITY.digimonEntity.stats.base.hp;
	rec[1] = PARTNER_ENTITY.digimonEntity.stats.base.mp;
	rec[2] = PARTNER_ENTITY.digimonEntity.stats.base.off;
	rec[3] = PARTNER_ENTITY.digimonEntity.stats.base.def;
	rec[4] = PARTNER_ENTITY.digimonEntity.stats.base.speed;
	rec[5] = PARTNER_ENTITY.digimonEntity.stats.base.brain;
	rec[6] = PARTNER_PARA.discipline;
	((char *)rec)[0x1C] = PARTNER_ENTITY.digimonEntity.entity.type;
	memcpy((char *)rec + 0xE, PARTNER_ENTITY.name, 0xE);
	memcpy((char *)rec + 0x1D, PARTNER_ENTITY.digimonEntity.stats.base.moves, 3);
}

void MAIN_func_801136C8(MenuHighlight *b)
{
	LINE_F3 *prim;
	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;

	x = b->x - 1;
	y = (b->y + (b->rowHeight * b->pos)) - 2;
	x -= 0xA0;
	y -= 0x78;
	w = b->w + 1;
	h = b->h + 2;
	prim = (LINE_F3 *)GsGetWorkBase();
	SetLineF3(prim);
	prim->r0 = 0xA0;
	prim->g0 = 0xA0;
	prim->b0 = 0;
	prim->x0 = x;
	prim->y0 = y;
	prim->x1 = x + w;
	prim->y1 = y;
	prim->x2 = x + w;
	prim->y2 = y + h;
	AddPrim(ACTIVE_ORDERING_TABLE->org, prim++);
	SetLineF3(prim);
	prim->r0 = 0xA0;
	prim->g0 = 0xA0;
	prim->b0 = 0;
	prim->x0 = x + w;
	prim->y0 = y + h;
	prim->x1 = x;
	prim->y1 = y + h;
	prim->x2 = x;
	prim->y2 = y;
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

int32_t MAIN_func_801138B0(void)
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
	int16_t *rec;

	rec = (int16_t *)&MAIN_D_80131B2C[0xEC0];
	rec[0] = PARTNER_ENTITY.digimonEntity.stats.base.hp;
	rec[1] = PARTNER_ENTITY.digimonEntity.stats.base.mp;
	rec[2] = PARTNER_ENTITY.digimonEntity.stats.base.off;
	rec[3] = PARTNER_ENTITY.digimonEntity.stats.base.def;
	rec[4] = PARTNER_ENTITY.digimonEntity.stats.base.speed;
	rec[5] = PARTNER_ENTITY.digimonEntity.stats.base.brain;
	rec[6] = PARTNER_PARA.discipline;
	((char *)rec)[0x1C] = 0x73;
	memcpy((char *)rec + 0xE, PARTNER_ENTITY.name, 0xE);
	((char *)rec)[0x1D] = 0x2E;
	((char *)rec)[0x1E] = 0x30;
	((char *)rec)[0x1F] = 0x32;
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

int32_t MAIN_func_80113A20(void)
{
	tickMainMenu();
	renderMainMenu();
	if (CURRENT_MENU == -1) {
		return 1;
	}

	return 0;
}
