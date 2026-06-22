#include <libgs.h>
#include <dw/entity.h>
#include <dw/params.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/item.h>
#include <dw/world_object.h>

#include "common.h"

extern int32_t MAIN_D_80134E04;
extern int32_t MAIN_D_80134E08;
extern uint8_t MAIN_D_80134E13;
extern uint8_t MAIN_D_80134E14;
extern uint8_t MAIN_D_80134E15;
extern int32_t MAIN_D_80134E18;
extern int32_t POLLED_INPUT;
extern int32_t POLLED_INPUT_PREVIOUS;
extern TamerItem TAMER_ITEM;
extern uint8_t INVENTORY_POINTER;
extern char MAIN_D_8012A9D8[];
void clearTextArea(void);
extern uint8_t MAIN_D_80134E11;
extern uint8_t INVENTORY_ITEM_TYPES[30];
extern uint8_t INVENTORY_ITEM_NAMES[30];
extern uint8_t MAIN_D_80134E12;
extern int8_t GAME_STATE;
extern char *COMBAT_DATA_PTR;
extern TamerEntity TAMER_ENTITY;
void addGameMenu(void);
void closeTriangleMenu(void);
void startAnimation(Entity *e, int32_t anim);
void startFeedingItem(uint8_t type);
void startThrowingItem(void);
void removeItem(int32_t type, uint32_t amount);
extern char MAIN_D_801343BC;
extern char MAIN_D_801343C4;
extern char MAIN_D_801343CC;
extern char MAIN_D_801343D4;
extern char MAIN_D_801343D8;
extern char MAIN_D_8012A9B4[];
extern int16_t MAIN_D_80134E0C;
extern int16_t MAIN_D_80134E0E;
extern int32_t MAIN_D_80134E00;
extern volatile uint8_t INVENTORY_SIZE[];
void getEntityScreenPos(Entity *e, int32_t mode, int16_t *out);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
		  int32_t f, int32_t g, int32_t h, int32_t i);
void renderSelectionCursor(int16_t x, int16_t y, int32_t w, int32_t h, int32_t layer);
extern char *ITEM_DESC_PTR[];
void clearTextSubArea(RECT *area);
void drawString(char *text, int32_t color, int32_t pos);
void swapByte(uint8_t *a, uint8_t *b);
void playSound(int32_t vabId, uint32_t note);
void sortItems(int16_t mode);

int32_t MAIN_func_800DAAF8(int32_t mode);
void drawInventoryText();
void drawInventoryTextLine(int8_t slot);
void MAIN_func_800DADDC();
void initializeInventoryObject();
void tickInventoryObject(int32_t instanceId);
void closeInventoryBoxes();
int32_t createInventoryUI(void);
void tickInventoryTop();
void renderInventoryTop();
void renderInventoryBottom(int32_t arg);
void closeInventoryBoxes2();
void MAIN_func_800DBCE8();
void MAIN_func_800DBE68();
void MAIN_func_800DBEA0();
int32_t MAIN_func_800DBFE8(void);
void MAIN_func_800DC03C();
void MAIN_func_800DC150();
void MAIN_func_800DC160();
void MAIN_func_800DC2A4();
void MAIN_func_800DC2D8();
void MAIN_func_800DC3E4();
void MAIN_func_800DC47C();
void MAIN_thunk_func_800DC2A4();
void updateItemNames();
void moveItem();
void selectSorting();
void MAIN_func_800DC75C();
void MAIN_func_800DC788();
void MAIN_func_800DC918();

void copyItemArray(uint8_t *src, uint8_t *dst, int32_t n);
void MAIN_func_800DCCD4();
int32_t MAIN_func_800DCD08(int32_t mask);
void MAIN_func_800DCD58(uint8_t *p, int32_t unused, int32_t max);

void *inventory_order_anchor[] = {
	MAIN_func_800DCD58,
	MAIN_func_800DCD08,
	MAIN_func_800DCCD4,
	copyItemArray,
	sortItems,
	MAIN_func_800DC918,
	MAIN_func_800DC788,
	MAIN_func_800DC75C,
	selectSorting,
	moveItem,
	updateItemNames,
	MAIN_thunk_func_800DC2A4,
	MAIN_func_800DC47C,
	MAIN_func_800DC3E4,
	MAIN_func_800DC2D8,
	MAIN_func_800DC2A4,
	MAIN_func_800DC160,
	MAIN_func_800DC150,
	MAIN_func_800DC03C,
	MAIN_func_800DBFE8,
	MAIN_func_800DBEA0,
	MAIN_func_800DBE68,
	MAIN_func_800DBCE8,
	closeInventoryBoxes2,
	renderInventoryBottom,
	renderInventoryTop,
	tickInventoryTop,
	createInventoryUI,
	closeInventoryBoxes,
	tickInventoryObject,
	initializeInventoryObject,
	MAIN_func_800DADDC,
	drawInventoryTextLine,
	drawInventoryText,
	MAIN_func_800DAAF8,
};

int32_t MAIN_func_800DAAF8(int32_t mode)
{
	Item *item;
	uint8_t t;

	if ((mode == 1) || (mode == 2)) {
		return 9;
	}
	t = INVENTORY_ITEM_TYPES[INVENTORY_POINTER];
	if (mode == 0) {
		if (t != 0xFF) {
			item = &ITEM_PARA[t];
			switch (GAME_STATE) {
			case 0:
				if ((item->itemColor == 0) ||
				    (item->itemColor == 2)) {
					return 9;
				}
				return 0xA;
			case 1:
			case 2:
			case 3:
				if ((item->itemColor == 1) ||
				    (item->itemColor == 2)) {
					return 9;
				}
				return 0xA;
			}
		}
		return 0xA;
	} else if (mode == 3) {
		if (GAME_STATE == 1) {
			return 0xA;
		}
		if (t != 0xFF) {
			item = &ITEM_PARA[t];
			if (item->droppable == 1) {
				return 9;
			}
			return 0xA;
		} else {
			return 0xA;
		}
	}
}

INCLUDE_ASM("asm/main/nonmatchings/inventory", drawInventoryText);

INCLUDE_ASM("asm/main/nonmatchings/inventory", drawInventoryTextLine);

void MAIN_func_800DADDC(void)
{
	MAIN_D_80134E04 = 0;
	MAIN_D_80134E08 = 0;
}

void initializeInventoryObject(void)
{
	if ((MAIN_D_80134E08 != 1) && (UI_BOX_DATA[0].state == 0) &&
	    (TAMER_ITEM.worldItem.type == 0xFF)) {
		MAIN_D_80134E08 = 1;
		MAIN_D_80134E04 = 1;
		addObject(0x1A5, 0, tickInventoryObject, 0);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/inventory", tickInventoryObject);

void closeInventoryBoxes(void)
{
  int16_t new_var;
  int i;
  if (MAIN_D_80134E08 != 0)
  {
    MAIN_D_80134E08 = 0;
    MAIN_D_80134E04 = 0;
    for (i = 0; i < 4; i++)
    {
      if (UI_BOX_DATA[i].state != 0)
      {
        removeStaticUIBox(new_var = i);
      }
    }

    removeObject(0x1A5, 0);
  }
}

INCLUDE_ASM("asm/main/nonmatchings/inventory", createInventoryUI);

INCLUDE_ASM("asm/main/nonmatchings/inventory", tickInventoryTop);

INCLUDE_ASM("asm/main/nonmatchings/inventory", renderInventoryTop);

void renderInventoryBottom(int32_t arg)
{
	if (INVENTORY_ITEM_TYPES[INVENTORY_POINTER] != 0xFF) {
		if (INVENTORY_POINTER != MAIN_D_80134E12) {
			updateItemNames();
		}
		MAIN_D_80134E12 = INVENTORY_POINTER;
		renderString(9, UI_BOX_DATA[1].finalPos.x + 0x1A,
			     UI_BOX_DATA[1].finalPos.y + 8, 0xFC, 0xC, 0, 0xB4,
			     6 - arg, 1);
	}
}

void closeInventoryBoxes2(void)
{
	int16_t xy[2];
	RECT r;
	int16_t x;

	if (UI_BOX_DATA[0].frame == 0) {
		closeInventoryBoxes();
		return;
	}
	if (UI_BOX_DATA[1].state == 1) {
		if (INVENTORY_POINTER & 1) {
			x = UI_BOX_DATA[0].finalPos.x + 0x9A;
		} else {
			x = UI_BOX_DATA[0].finalPos.x + 0xA;
		}
		r.x = x;
		r.y = UI_BOX_DATA[0].finalPos.y + 8 +
		      ((int16_t)((INVENTORY_POINTER / 2) - MAIN_D_80134E0C) * 0x12);
		r.w = 0x10;
		r.h = 0x10;
		removeAnimatedUIBox(1, &r);
	}
	if ((UI_BOX_DATA[1].frame <= 0) && (UI_BOX_DATA[0].state == 1)) {
		getEntityScreenPos(ENTITY_TABLE[0], 1, xy);
		r.x = xy[0] - 5;
		r.y = xy[1] - 5;
		r.w = 0xA;
		r.h = 0xA;
		removeAnimatedUIBox(0, &r);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/inventory", MAIN_func_800DBCE8);

void MAIN_func_800DBE68(void)
{
	if (UI_BOX_DATA[3].state == 0) {
		MAIN_func_800DCD58(&MAIN_D_80134E13, 2, 4);
	}
}

INCLUDE_ASM("asm/main/nonmatchings/inventory", MAIN_func_800DBEA0);

int32_t MAIN_func_800DBFE8(void)
{
	if (UI_BOX_DATA[2].frame == 0) {
		return 1;
	}
	if (UI_BOX_DATA[2].state == 1) {
		removeAnimatedUIBox(2, NULL);
	}
	return 0;
}

void MAIN_func_800DC03C(void)
{
	RECT r1;
	RECT r2;

	if ((UI_BOX_DATA[2].state == 1) && (UI_BOX_DATA[3].state == 0)) {
		drawString(&MAIN_D_801343BC, 0xC0, 0x30);
		drawString(&MAIN_D_801343C4, 0xC0, 0x3C);
		drawString(&MAIN_D_801343CC, 0xC0, 0x48);
		MAIN_D_80134E14 = 0;
		r1.x = UI_BOX_DATA[2].finalPos.x + UI_BOX_DATA[2].finalPos.w;
		r1.y = UI_BOX_DATA[2].finalPos.y;
		r1.w = 0x48;
		r1.h = 0x42;
		r2.x = UI_BOX_DATA[2].finalPos.x + 9;
		r2.y = UI_BOX_DATA[2].finalPos.y + 6 + (MAIN_D_80134E13 * 0x12);
		r2.w = 0x28;
		r2.h = 0x10;
		createAnimatedUIBox(3, 1, 0, &r1, &r2,
				    (TickFunction)MAIN_func_800DC150,
				    (RenderFunction)MAIN_func_800DC160);
	}
}

void MAIN_func_800DC150(void)
{
	MAIN_func_800DCD58(&MAIN_D_80134E14, 3, 3);
}

INCLUDE_ASM("asm/main/nonmatchings/inventory", MAIN_func_800DC160);

void MAIN_func_800DC2A4(void)
{
	if (UI_BOX_DATA[3].state == 1) {
		removeAnimatedUIBox(3, NULL);
	}
}

void MAIN_func_800DC2D8(void)
{
	RECT r1;
	RECT r2;

	if ((UI_BOX_DATA[2].state == 1) && (UI_BOX_DATA[3].state == 0)) {
		drawString(&MAIN_D_801343D4, 0xC5, 0x54);
		drawString(&MAIN_D_801343D8, 0xC0, 0x60);
		drawString(MAIN_D_8012A9B4, 0, 0xC0);
		r1.x = -0x40;
		r1.y = -0x26;
		r1.w = 0x80;
		r1.h = 0x35;
		r2.x = UI_BOX_DATA[2].finalPos.x + 9;
		MAIN_D_80134E15 = 1;
		r2.y = UI_BOX_DATA[2].finalPos.y + 6 + (MAIN_D_80134E13 * 0x12);
		r2.w = 0x28;
		r2.h = 0x10;
		createAnimatedUIBox(3, 1, 0, &r1, &r2,
				    (TickFunction)MAIN_func_800DC3E4,
				    (RenderFunction)MAIN_func_800DC47C);
	}
}

void MAIN_func_800DC3E4(void)
{
	MAIN_func_800DCCD4();
	if ((MAIN_func_800DCD08(0x8000) != 0) && (MAIN_D_80134E15 != 0)) {
		playSound(0, 2);
		MAIN_D_80134E15 -= 1;
	}
	if ((MAIN_func_800DCD08(0x2000) != 0) && (MAIN_D_80134E15 == 0)) {
		playSound(0, 2);
		MAIN_D_80134E15 += 1;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/inventory", MAIN_func_800DC47C);

void MAIN_thunk_func_800DC2A4(void)
{
	MAIN_func_800DC2A4();
}

void updateItemNames(void)
{
	RECT area;

	area.x = 0;
	area.y = 0xB4;
	area.w = 0xFC;
	area.h = 0xC;
	clearTextSubArea(&area);
	drawString(ITEM_DESC_PTR[INVENTORY_ITEM_TYPES[INVENTORY_POINTER]], 0, 0xB4);
	DrawSync(0);
}

void moveItem(void)
{
	uint8_t *p;

	p = &INVENTORY_ITEM_TYPES[INVENTORY_POINTER];
	swapByte(p, &INVENTORY_ITEM_TYPES[MAIN_D_80134E11]);
	swapByte(&INVENTORY_ITEM_TYPES[INVENTORY_POINTER] + 0x1E,
		 &INVENTORY_ITEM_TYPES[MAIN_D_80134E11] + 0x1E);
	swapByte(&INVENTORY_ITEM_TYPES[INVENTORY_POINTER] + 0x3C,
		 &INVENTORY_ITEM_TYPES[MAIN_D_80134E11] + 0x3C);
	if (*p != 0xFF) {
		updateItemNames();
	}
}

void selectSorting(void)
{
	if (MAIN_D_80134E04 != MAIN_D_80134E14 + 0xB) {
		sortItems(MAIN_D_80134E14 + 0xB);
		MAIN_D_80134E04 = MAIN_D_80134E14 + 0xB;
	}
}

void MAIN_func_800DC75C(void)
{
	if (MAIN_D_80134E15 == 0) {
		MAIN_D_80134E04 = 7;
	} else {
		MAIN_D_80134E04 = 0xF;
	}
}

void MAIN_func_800DC788(void)
{
	Item *item;
	uint8_t t;

	if ((MAIN_D_80134E04 != 0xB) && (MAIN_D_80134E04 != 0xC) &&
	    (MAIN_D_80134E04 != 0xD) && (UI_BOX_DATA[3].state == 0)) {
		switch (MAIN_D_80134E13) {
		case 0:
			t = INVENTORY_ITEM_TYPES[INVENTORY_POINTER];
			if (t != 0xFF) {
				item = &ITEM_PARA[INVENTORY_ITEM_TYPES[INVENTORY_POINTER]];
				switch (GAME_STATE) {
				case 0:
					if ((item->itemColor == 0) ||
					    (item->itemColor == 2)) {
						MAIN_D_80134E04 = 5;
					}
					break;
				case 1:
					if ((item->itemColor == 1) ||
					    (item->itemColor == 2)) {
						MAIN_D_80134E04 = 5;
					}
					break;
				}
			}
			break;
		case 1:
			MAIN_D_80134E04 = 6;
			MAIN_D_80134E11 = INVENTORY_POINTER;
			UI_BOX_DATA[0].state = 4;
			return;
		case 2:
			MAIN_D_80134E04 = 8;
			break;
		case 3:
			if (GAME_STATE == 0) {
				t = INVENTORY_ITEM_TYPES[INVENTORY_POINTER];
				if ((t != 0xFF) && ITEM_PARA[INVENTORY_ITEM_TYPES[INVENTORY_POINTER]].droppable == 1) {
					MAIN_D_80134E04 = 0xE;
				}
			}
			break;
		}
	}
}

void MAIN_func_800DC918(void)
{
	if ((MAIN_D_80134E04 != 0xB) && (MAIN_D_80134E04 != 0xC) &&
	    (MAIN_D_80134E04 != 0xD) && (MAIN_D_80134E04 != 5) &&
	    (UI_BOX_DATA[2].state == 0)) {
		if (UI_BOX_DATA[0].state == 0) {
			MAIN_D_80134E04 = 1;
			return;
		}
		if (MAIN_D_80134E04 == 6) {
			playSound(0, 4);
			UI_BOX_DATA[0].state = 1;
			MAIN_D_80134E04 = 0;
			moveItem();
			return;
		}
		if ((MAIN_D_80134E04 < 5) && (TAMER_ITEM.worldItem.type == 0xFF) &&
		    (UI_BOX_DATA[1].state == 1)) {
			MAIN_D_80134E04 = 3;
		}
	}
}

INCLUDE_ASM("asm/main/nonmatchings/inventory", sortItems);

void copyItemArray(uint8_t *src, uint8_t *dst, int32_t n)
{
	int32_t i;

	i = 0;
	while (i < n) {
		*dst++ = *src++;
		i += 1;
	}
}

void MAIN_func_800DCCD4(void)
{
	if (POLLED_INPUT == POLLED_INPUT_PREVIOUS) {
		MAIN_D_80134E18 += 1;
		return;
	}
	MAIN_D_80134E18 = 0;
}

int32_t MAIN_func_800DCD08(int32_t mask)
{
	if ((mask & (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS)) ||
	    ((MAIN_D_80134E18 >= 0xB) && (mask & POLLED_INPUT))) {
		return 1;
	}
	return 0;
}

void MAIN_func_800DCD58(uint8_t *p, int32_t unused, int32_t max)
{
	MAIN_func_800DCCD4();
	if ((MAIN_func_800DCD08(0x1000) != 0) && (*p != 0)) {
		playSound(0, 2);
		*p -= 1;
	}
	if ((MAIN_func_800DCD08(0x4000) != 0) && (*p < max - 1)) {
		playSound(0, 2);
		*p += 1;
	}
}

