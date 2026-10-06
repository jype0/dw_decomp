#include <libgs.h>

#include <dw/anim.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/font.h>
#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/item.h>
#include <dw/line.h>
#include <dw/params.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/world_object.h>

extern int32_t POLLED_INPUT;
extern int32_t POLLED_INPUT_PREVIOUS;
extern int8_t GAME_STATE;
extern TamerEntity TAMER_ENTITY;
extern char *ITEM_DESC_PTR[];

void addGameMenu(void);
void closeTriangleMenu(void);
void startFeedingItem(uint8_t type);
void startThrowingItem(void);
void getEntityScreenPos(Entity *e, int32_t mode, DVECTOR *out);
void renderString(int32_t color, int32_t x, int32_t y, int32_t w, int32_t h,
                  int32_t u, int32_t v, int32_t layer, int32_t shadow);
void renderSelectionCursor(int32_t x, int32_t y, int16_t w, int16_t h, int32_t layer);
void swapByte(uint8_t *a, uint8_t *b);
void playSound(int32_t vabId, uint32_t note);
void sortItems(int16_t mode);
void sortItemsById(uint8_t *data, long count);
void renderItemSprite(int32_t type, int32_t x, int32_t y, int32_t layer);
void renderItemAmount(int32_t color, int32_t n, int32_t x, int32_t y,
                      int32_t value, int32_t layer);

int32_t getItemOptionColor(int32_t mode);
void drawInventoryText(void);
void drawInventoryTextLine(int16_t startSlot);
void initializeInventoryUI();
void addInventoryUI();
void tickInventoryUI(int32_t instanceId);
void closeInventoryBoxes();
int32_t createInventoryView(void);
void tickInventoryTop();
void renderInventoryTop(int16_t boxId);
void renderInventoryBottom(int16_t boxId);
void closeInventoryBoxes2();
void createItemOption(void);
void tickItemOption();
void renderItemOption(int16_t boxId);
int32_t removeUIBox2(void);
void createSortOption();
void tickSortOption();
void renderSortOption(int16_t boxId);
void removeUIBox3();
void createDropItemConfirmBox();
void tickDropItemConfirmBox();
void renderDropItemConfirmBox(int16_t boxId);
void thunkRemoveUIBox3();
void updateItemDescription();
void moveItem();
void selectSorting();
void selectDropItem();
void selectOptionMenu();
void selectRegularInventory();

void copyItemArray(uint8_t *src, uint8_t *dst, long n);
void updateInventoryInputRepeatCounter();
int32_t isInventoryButtonPressed(int32_t mask);
void tickInventoryOptionSelector(uint8_t *cursor, int32_t unused, int16_t max);

#if defined(VERSION_JP)
char SORT_LABEL_BATTLE[] = "戦闘配置";
char SORT_LABEL_RAISE[] = "育成配置";
char SORT_LABEL_BASIC[] = "基本配置";
char CONFIRM_PROMPT[] = "本当に捨てますか？";
#else
char SORT_LABEL_BATTLE[] = "Battle";
char SORT_LABEL_RAISE[] = "Raise";
char SORT_LABEL_BASIC[] = "Basic";
char CONFIRM_PROMPT[] = "Are you sure?";
#endif
uint8_t ITEM_SORT_ORDER[3][6] = {
	{ 0x00, 0x01, 0x03, 0x02, 0x05, 0x04 },
	{ 0x02, 0x05, 0x00, 0x01, 0x04, 0x03 },
	{ 0x02, 0x00, 0x01, 0x03, 0x05, 0x04 },
};
#if defined(VERSION_JP)
char ITEM_MENU_LABELS[4][8] = { "使う", "移動", "せいり", "捨てる" };
char CONFIRM_LABEL_YES[] = "はい";
char CONFIRM_LABEL_NO[] = "いいえ";
#else
char ITEM_MENU_LABELS[4][8] = { "Use", "Move", "Sort", "Drop" };
char CONFIRM_LABEL_YES[4] = "Yes";
char CONFIRM_LABEL_NO[] = "No";
#endif

int32_t INVENTORY_UNUSED;
int32_t INVENTORY_ACTION;
int32_t IS_INVENTORY_INITIALIZED;
int16_t INVENTORY_TYPE_OFFSET;
int16_t INVENTORY_ROW_OFFSET;
uint8_t INVENTORY_POINTER;
uint8_t ITEM_MOVE_SOURCE;
uint8_t ITEM_MOVE_TARGET;
uint8_t ITEM_MENU_SELECTED;
uint8_t SORT_SELECTED;
uint8_t DROP_ITEM_SELECTED_OPTION;
int32_t INVENTORY_INPUT_REPEAT_COUNTER;

static void *inventory_sbss_order[] = {
	&INVENTORY_INPUT_REPEAT_COUNTER,
	&DROP_ITEM_SELECTED_OPTION,
	&SORT_SELECTED,
	&ITEM_MENU_SELECTED,
	&ITEM_MOVE_TARGET,
	&ITEM_MOVE_SOURCE,
	&INVENTORY_POINTER,
	&INVENTORY_ROW_OFFSET,
	&INVENTORY_TYPE_OFFSET,
	&IS_INVENTORY_INITIALIZED,
	&INVENTORY_ACTION,
	&INVENTORY_UNUSED,
};

void *inventory_text_order[] = {
	tickInventoryOptionSelector,
	isInventoryButtonPressed,
	updateInventoryInputRepeatCounter,
	copyItemArray,
	sortItems,
	selectRegularInventory,
	selectOptionMenu,
	selectDropItem,
	selectSorting,
	moveItem,
	updateItemDescription,
	thunkRemoveUIBox3,
	renderDropItemConfirmBox,
	tickDropItemConfirmBox,
	createDropItemConfirmBox,
	removeUIBox3,
	renderSortOption,
	tickSortOption,
	createSortOption,
	removeUIBox2,
	renderItemOption,
	tickItemOption,
	createItemOption,
	closeInventoryBoxes2,
	renderInventoryBottom,
	renderInventoryTop,
	tickInventoryTop,
	createInventoryView,
	closeInventoryBoxes,
	tickInventoryUI,
	addInventoryUI,
	initializeInventoryUI,
	drawInventoryTextLine,
	drawInventoryText,
	getItemOptionColor,
};

int32_t getItemOptionColor(int32_t mode)
{
	Item *item;
	uint8_t type;

	if ((mode == 1) || (mode == 2)) {
		return 9;
	}
	type = INVENTORY.types.array[INVENTORY_POINTER];
	if (mode == 0) {
		if (type != 0xff) {
			item = &ITEM_PARA[type];
			switch (GAME_STATE) {
			case 0:
				if ((item->itemColor == 0) ||
				    (item->itemColor == 2)) {
					return 9;
				}
				return 0xa;
			case 1:
			case 2:
			case 3:
				if ((item->itemColor == 1) ||
				    (item->itemColor == 2)) {
					return 9;
				}
				return 0xa;
			}
		}
		return 0xa;
	} else if (mode == 3) {
		if (GAME_STATE == 1) {
			return 0xa;
		}
		if (type != 0xff) {
			item = &ITEM_PARA[type];
			if (item->droppable == 1) {
				return 9;
			}
			return 0xa;
		} else {
			return 0xa;
		}
	}
}

void drawInventoryText(void)
{
	int32_t i;

	clearTextArea();
	for (i = 0; i < INVENTORY.size / 2; i++) {
		drawInventoryTextLine(i * 2);
	}
	for (i = 0; i < 4; i++) {
		drawString(ITEM_MENU_LABELS[i], 0xc0, i * 12);
	}
}

void createItemOption(void)
{
	int32_t startX;
	int32_t startY;
	RECT finalPos;
	RECT startPos;
	int32_t x;
	int32_t y;
	int16_t row;

	if ((UI_BOX_DATA[1].state == 1) &&
	    (UI_BOX_DATA[2].frame == 0) &&
	    (INVENTORY_ACTION != 5)) {
		UI_BOX_DATA[2].features = 0;
		ITEM_MENU_SELECTED = 0;
		if ((INVENTORY_POINTER & 1) != 0) {
			x = UI_BOX_DATA[0].finalPos.x + 0x9a;
		} else {
			x = UI_BOX_DATA[0].finalPos.x + 0xa;
		}
		row = (INVENTORY_POINTER / 2) - INVENTORY_TYPE_OFFSET;
		if (row < 5) {
			y = UI_BOX_DATA[0].finalPos.y + 0x18 + row * 0x12;
		} else {
			y = UI_BOX_DATA[0].finalPos.y + 0x20 + (row - 6) * 0x12;
		}
		setRECT(&finalPos, x, y, 0x3a, 0x54);
		startX = x - 3;
		startY = UI_BOX_DATA[0].finalPos.y + 7 + (INVENTORY_POINTER / 2 - INVENTORY_TYPE_OFFSET) * 0x12;
		setRECT(&startPos, startX, startY, 0x8a, 0x12);
		createAnimatedUIBox(2, 1, 0, &finalPos, &startPos,
		                    (TickFunction)tickItemOption,
		                    (RenderFunction)renderItemOption);
	}
}

void drawInventoryTextLine(int16_t startSlot)
{
	long i;

	for (i = 0; i < 2; i++) {
		if (INVENTORY.types.array[startSlot + i] != 0xff) {
			INVENTORY.names.array[startSlot + i] = startSlot + i;
			drawString(ITEM_PARA[INVENTORY.types.array[startSlot + i]].name,
			           ((startSlot + i) & 1) * 0x60, ((startSlot + i) / 2) * 0xc);
		}
	}
	DrawSync(0);
}

void initializeInventoryUI(void)
{
	INVENTORY_ACTION = 0;
	IS_INVENTORY_INITIALIZED = 0;
}

void addInventoryUI(void)
{
	if ((IS_INVENTORY_INITIALIZED != 1) && (UI_BOX_DATA[0].state == 0) &&
	    (TAMER_ITEM.worldItem.type == 0xff)) {
		IS_INVENTORY_INITIALIZED = 1;
		INVENTORY_ACTION = 1;
		addObject(0x1a5, 0, tickInventoryUI, 0);
	}
}

GARBAGE(tickInventoryUI, 1);

void tickInventoryUI(int32_t instanceId)
{
#ifdef __MWERKS__
	extern void removeItem(uint8_t type, uint8_t amount);
#endif
	TamerEntity *tam;

	if (COMBAT_DATA_PTR->player.currentCommand[0] != 1) {
		tam = &TAMER_ENTITY;
		if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CONFIRM_BUTTON) {
			if (UI_BOX_DATA[3].state == 1) {
				if (ITEM_MENU_SELECTED == 2) {
					selectSorting();
				} else {
					selectDropItem();
				}
			} else if (UI_BOX_DATA[2].state == 1) {
				selectOptionMenu();
			} else {
				selectRegularInventory();
			}
		}
		if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CANCEL_BUTTON) {
			if (UI_BOX_DATA[3].state == 1) {
				if (ITEM_MENU_SELECTED == 2) {
					INVENTORY_ACTION = 9;
				} else {
					INVENTORY_ACTION = 0xf;
				}
			} else if (UI_BOX_DATA[2].state == 0) {
				if (UI_BOX_DATA[0].state == 4) {
					playSound(0, 3);
					UI_BOX_DATA[0].state = 1;
					INVENTORY_ACTION = 0;
				} else if (UI_BOX_DATA[1].state == 1) {
					INVENTORY_ACTION = 2;
				}
			} else if (UI_BOX_DATA[2].state == 1) {
				INVENTORY_ACTION = 4;
			}
		}
		switch (INVENTORY_ACTION) {
		case 1:
			createInventoryView();
			if (tam->entity.anim.animId != 4) {
				startAnimation(&tam->entity, 4);
			}
			break;

		case 2:
			closeInventoryBoxes2();
			if ((GAME_STATE == 0) && (UI_BOX_DATA[0].frame == 0)) {
				closeTriangleMenu();
				addGameMenu();
				startAnimation(&(&TAMER_ENTITY)->entity, 0);
			}
			break;

		case 3:
			createItemOption();
			break;

		case 4:

		case 6:
			removeUIBox2();
			break;

		case 5:
			if (removeUIBox2() != 0) {
				closeInventoryBoxes2();
			}
			if (UI_BOX_DATA[0].state == 0) {
				if (GAME_STATE == 0) {
					startFeedingItem(INVENTORY.types.array[INVENTORY_POINTER]);
					INVENTORY_ACTION = 0;
				} else if (GAME_STATE == 1) {
					startThrowingItem();
					INVENTORY_ACTION = 0;
				}
			}
			break;

		case 7:
			if (UI_BOX_DATA[3].state != 0) {
				thunkRemoveUIBox3();
			} else if (UI_BOX_DATA[2].state != 0) {
				removeUIBox2();
			} else {
				INVENTORY_ACTION = 0;
				removeItem(INVENTORY.types.array[INVENTORY_POINTER], INVENTORY.amounts.array[INVENTORY_POINTER]);
			}
			break;

		case 8:
			createSortOption();
			break;

		case 9:
			removeUIBox3();
			break;

		case 11:

		case 12:

		case 13:
			if (UI_BOX_DATA[3].state != 0) {
				removeUIBox3();
			} else if (UI_BOX_DATA[2].state != 0) {
				removeUIBox2();
			} else {
				INVENTORY_ACTION = 0;
			}
			break;

		case 14:
			createDropItemConfirmBox();
			break;

		case 15:
			thunkRemoveUIBox3();
			break;
		}
	}
}

void closeInventoryBoxes(void)
{
	int32_t i;
	if (IS_INVENTORY_INITIALIZED != 0) {
		IS_INVENTORY_INITIALIZED = 0;
		INVENTORY_ACTION = 0;
		for (i = 0; i < 4; i++) {
			if (UI_BOX_DATA[i].state != 0) {
				removeStaticUIBox(i);
			}
		}

		removeObject(0x1a5, 0);
	}
}

int32_t createInventoryView(void)
{
	DVECTOR xy;
	RECT finalPos;
	RECT startPos;
	uint8_t features;
	UIBoxData *box;

	if (UI_BOX_DATA[1].state == 1) {
		return 1;
	}
	box = &UI_BOX_DATA[0];
	if (box->frame == 0) {
#if defined(VERSION_JP)
		INVENTORY_UNUSED = 0;
		INVENTORY_TYPE_OFFSET = 0;
		INVENTORY_ROW_OFFSET = 0;
#else
		INVENTORY_TYPE_OFFSET = 0;
		INVENTORY_ROW_OFFSET = 0;
		INVENTORY_UNUSED = 0;
#endif
		INVENTORY_POINTER = 0;
		box->rowOffset = 0;
		box->visibleRows = 9;
		box->totalRows = 5;
		features = 2;
		if (INVENTORY.size > 10) {
			features |= 4;
			box->totalRows = INVENTORY.size == 20 ? 10 : 15;
		}
		setRECT(&finalPos, -0x98, -0x68, 0x130, INVENTORY.size == 10 ? 0x6e : 0xb6);
		getEntityScreenPos(ENTITY_TABLE[0], 1, &xy);
		setRECT(&startPos, xy.vx - 5, xy.vy - 5, 10, 10);
		createAnimatedUIBox(0, 0, features, &finalPos, &startPos,
		                    (TickFunction)tickInventoryTop, (RenderFunction)renderInventoryTop);
	}
	if (box->state != 1) {
		return 0;
	}
	if (UI_BOX_DATA[1].frame == 0) {
		UI_BOX_DATA[1].features = 2;
		ITEM_MOVE_TARGET = 0xff;
		setRECT(&finalPos, -0x98, 0x4d, 0x130, 0x1c);
		setRECT(&startPos, box->finalPos.x + 8, box->finalPos.y + 0xe, 0x10, 0x10);
		createAnimatedUIBox(1, 0, 2, &finalPos, &startPos,
		                    NULL, (RenderFunction)renderInventoryBottom);
	}
	return 0;
}

void tickInventoryTop(void)
{
	if (UI_BOX_DATA[2].state == 0 && UI_BOX_DATA[1].state == 1) {
		updateInventoryInputRepeatCounter();
		if (isInventoryButtonPressed(0x1000) && INVENTORY_POINTER >= 2) {
			playSound(0, 2);
			INVENTORY_POINTER -= 2;
		}
		if (isInventoryButtonPressed(0x4000) && INVENTORY_POINTER < INVENTORY.size - 2) {
			playSound(0, 2);
			INVENTORY_POINTER += 2;
		}
		if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x8000) &&
		    (INVENTORY_POINTER & 1)) {
			playSound(0, 2);
			--INVENTORY_POINTER;
		}
		if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x2000) &&
		    !(INVENTORY_POINTER & 1)) {
			playSound(0, 2);
			++INVENTORY_POINTER;
		}
		if (INVENTORY_POINTER < INVENTORY_TYPE_OFFSET * 2) {
			--INVENTORY_TYPE_OFFSET;
		}
		if (*(volatile uint8_t *)&INVENTORY_POINTER >
		    (INVENTORY_TYPE_OFFSET + UI_BOX_DATA[0].visibleRows - 1) * 2 + 1) {
			++INVENTORY_TYPE_OFFSET;
		}
	}
}

void renderInventoryTop(int16_t boxId)
{
	UIBoxData *box;
	int32_t i;
	int32_t slot;
	int32_t count;
	int32_t row;
	uint8_t type;
	uint8_t color;
	int16_t x;
	int16_t y;

	box = &UI_BOX_DATA[boxId];
	x = box->finalPos.x;
	y = box->finalPos.y;
	slot = INVENTORY_TYPE_OFFSET * 2;
	if (INVENTORY.size == 10) {
		count = 10;
	} else {
		count = 18;
	}
	for (i = 0; i < count; ++i, ++slot) {
		type = INVENTORY.types.array[slot];
		if (type != 0xff) {
			renderItemSprite(type, (i & 1) ? x + 0x9a : x + 0xa,
			                 y + 8 + (i / 2) * 0x12, 6 - boxId);
			if (ITEM_PARA[type].itemColor == 0xff) {
				color = 8;
			} else {
				color = ITEM_PARA[type].itemColor + 5;
			}
			renderString(color, (i & 1) ? x + 0xad : x + 0x1d,
			             y + 0xa + (i / 2) * 0x12, 0x60, 0xc,
			             (INVENTORY.names.array[slot] & 1) * 0x60,
			             (INVENTORY.names.array[slot] / 2) * 0xc,
			             6 - boxId, 1);
			renderItemAmount(0, 2, (i & 1) ? x + 0x113 : x + 0x83,
			                 y + 0xc + (i / 2) * 0x12,
			                 INVENTORY.amounts.array[slot], 6 - boxId);
		}
	}
	if (box->state == 4) {
		row = ITEM_MOVE_SOURCE / 2 - INVENTORY_TYPE_OFFSET;
		if (0 <= row && row < box->visibleRows) {
			if (ITEM_MOVE_SOURCE & 1) {
				x = box->finalPos.x + 0x97;
			} else {
				x = box->finalPos.x + 7;
			}
			y = box->finalPos.y + 7 + row * 0x12;
			drawLine3P(0x5d4af1, x, y + 0x12, x, y,
			           x + 0x8a, y, 6 - boxId, 0);
			drawLine3P(0x5d4af1, x + 0x8a, y,
			           x + 0x8a, y + 0x12, x, y + 0x12,
			           6 - boxId, 0);
		}
	}
	if (INVENTORY_POINTER & 1) {
		x = box->finalPos.x + 0x97;
	} else {
		x = box->finalPos.x + 7;
	}
	y = box->finalPos.y + 7 + (INVENTORY_POINTER / 2 - INVENTORY_TYPE_OFFSET) * 0x12;
	renderSelectionCursor(x, y, 0x8a, 0x12, 6 - boxId);
	box->rowOffset = INVENTORY_TYPE_OFFSET;
	INVENTORY_ROW_OFFSET = box->rowOffset;
}

void renderInventoryBottom(int16_t boxId)
{
	if (INVENTORY.types.array[INVENTORY_POINTER] != 0xff) {
		if (INVENTORY_POINTER != ITEM_MOVE_TARGET) {
			updateItemDescription();
		}
		ITEM_MOVE_TARGET = INVENTORY_POINTER;
		renderString(9, UI_BOX_DATA[1].finalPos.x + 0x1a,
		             UI_BOX_DATA[1].finalPos.y + 8, 0xfc, 0xc, 0, 0xb4,
		             6 - boxId, 1);
	}
}

void closeInventoryBoxes2(void)
{
	DVECTOR xy;
	RECT rect;
	int16_t x;
	int16_t y;
	int16_t row;

	if (UI_BOX_DATA[0].frame == 0) {
		closeInventoryBoxes();
		return;
	}
	if (UI_BOX_DATA[1].state == 1) {
		if (INVENTORY_POINTER & 1) {
			x = UI_BOX_DATA[0].finalPos.x + 0x9a;
		} else {
			x = UI_BOX_DATA[0].finalPos.x + 0xa;
		}
		row = INVENTORY_POINTER / 2 - INVENTORY_TYPE_OFFSET;
		y = UI_BOX_DATA[0].finalPos.y + 8 + row * 0x12;
		setRECT(&rect, x, y, 0x10, 0x10);
		removeAnimatedUIBox(1, &rect);
	}
	if ((UI_BOX_DATA[1].frame <= 0) && (UI_BOX_DATA[0].state == 1)) {
		getEntityScreenPos(ENTITY_TABLE[0], 1, &xy);
		setRECT(&rect, xy.vx - 5, xy.vy - 5, 0xa, 0xa);
		removeAnimatedUIBox(0, &rect);
	}
}

void tickItemOption(void)
{
	if (UI_BOX_DATA[3].state == 0) {
		tickInventoryOptionSelector(&ITEM_MENU_SELECTED, 2, 4);
	}
}

int32_t removeUIBox2(void)
{
	if (UI_BOX_DATA[2].frame == 0) {
		return 1;
	}
	if (UI_BOX_DATA[2].state == 1) {
		removeAnimatedUIBox(2, NULL);
	}
	return 0;
}

void createSortOption(void)
{
	RECT r1;
	RECT r2;
	int16_t x;
	int16_t y;

	if ((UI_BOX_DATA[2].state == 1) && (UI_BOX_DATA[3].state == 0)) {
		drawString(SORT_LABEL_BATTLE, 0xc0, 0x30);
		drawString(SORT_LABEL_RAISE, 0xc0, 0x3c);
		drawString(SORT_LABEL_BASIC, 0xc0, 0x48);
		SORT_SELECTED = 0;
		setRECT(&r1, UI_BOX_DATA[2].finalPos.x + UI_BOX_DATA[2].finalPos.w, UI_BOX_DATA[2].finalPos.y, 0x48, 0x42);
		x = UI_BOX_DATA[2].finalPos.x + 9;
		y = UI_BOX_DATA[2].finalPos.y + 6 + ITEM_MENU_SELECTED * 0x12;
		setRECT(&r2, x, y, 0x28, 0x10);
		createAnimatedUIBox(3, 1, 0, &r1, &r2,
		                    (TickFunction)tickSortOption,
		                    (RenderFunction)renderSortOption);
	}
}

void renderItemOption(int16_t boxId)
{
	UIBoxData *box;
	int16_t x;
	int16_t y;
	int32_t i;

	box = &UI_BOX_DATA[boxId];
	x = box->finalPos.x + 9;
	y = box->finalPos.y + 6 + ITEM_MENU_SELECTED * 0x12;

	for (i = 0; i < 4; ++i) {
		renderString(getItemOptionColor(i), x + 3,
		             box->finalPos.y + 8 + i * 0x12, 0x24, 0xc,
		             0xc0, i * 0xc, 6 - boxId, 1);
	}
	renderSelectionCursor(x, y, 0x28, 0x10, 6 - boxId);
}

void tickSortOption(void)
{
	tickInventoryOptionSelector(&SORT_SELECTED, 3, 3);
}

void renderSortOption(int16_t boxId)
{
	int32_t i;
	int16_t x;
	int16_t y;
	RECT *pos;

	pos = &UI_BOX_DATA[boxId].finalPos;
	x = pos->x + 9;
	y = pos->y + 6 + SORT_SELECTED * 0x12;

	for (i = 0; i < 3; ++i) {
		renderString(9, x + 3, pos->y + 8 + i * 0x12, 0x30,
		             0xc, 0xc0, 0x30 + i * 0xc, 6 - boxId, 1);
	}
	renderSelectionCursor(x, y, 0x36, 0x10, 6 - boxId);
}

void removeUIBox3(void)
{
	if (UI_BOX_DATA[3].state == 1) {
		removeAnimatedUIBox(3, NULL);
	}
}

void createDropItemConfirmBox(void)
{
	RECT r1;
	RECT r2;
	int16_t x;
	int16_t y;

	if ((UI_BOX_DATA[2].state == 1) && (UI_BOX_DATA[3].state == 0)) {
		drawString(CONFIRM_LABEL_YES, 0xc5, 0x54);
		drawString(CONFIRM_LABEL_NO, 0xc0, 0x60);
		drawString(CONFIRM_PROMPT, 0, 0xc0);
		DROP_ITEM_SELECTED_OPTION = 1;
		setRECT(&r1, -0x40, -0x26, 0x80, 0x35);
		x = UI_BOX_DATA[2].finalPos.x + 9;
		y = UI_BOX_DATA[2].finalPos.y + 6 + ITEM_MENU_SELECTED * 0x12;
		setRECT(&r2, x, y, 0x28, 0x10);
		createAnimatedUIBox(3, 1, 0, &r1, &r2,
		                    (TickFunction)tickDropItemConfirmBox,
		                    (RenderFunction)renderDropItemConfirmBox);
	}
}

void tickDropItemConfirmBox(void)
{
	updateInventoryInputRepeatCounter();
	if ((isInventoryButtonPressed(0x8000) != 0) && (DROP_ITEM_SELECTED_OPTION != 0)) {
		playSound(0, 2);
		--DROP_ITEM_SELECTED_OPTION;
	}
	if ((isInventoryButtonPressed(0x2000) != 0) && (DROP_ITEM_SELECTED_OPTION == 0)) {
		playSound(0, 2);
		++DROP_ITEM_SELECTED_OPTION;
	}
}

void renderDropItemConfirmBox(int16_t boxId)
{
	int32_t i;
	int16_t x;
	int16_t y;
	UIBoxData *box;

	box = &UI_BOX_DATA[boxId];
	x = box->finalPos.x + 0x12 + DROP_ITEM_SELECTED_OPTION * 0x35;
	y = box->finalPos.y + 0x1e;

	renderString(9, -0x34, -0x1e, 0x6c, 0xc, 0, 0xc0, 6 - boxId, 1);
	for (i = 0; i < 2; ++i) {
		renderString(9, (int32_t)(box->finalPos.x + 0x12 + i * 0x34) + 5,
		             y + 2, 0x24, 0xc, 0xc0, 0x54 + i * 0xc,
		             6 - boxId, 1);
	}
	renderSelectionCursor(x, y, 0x2a, 0x10, 6 - boxId);
}

void thunkRemoveUIBox3(void)
{
	removeUIBox3();
}

void updateItemDescription(void)
{
	RECT area;

	setRECT(&area, 0, 0xb4, 0xfc, 0xc);
	clearTextSubArea(&area);
	drawString(ITEM_DESC_PTR[INVENTORY.types.array[INVENTORY_POINTER]], 0, 0xb4);
	DrawSync(0);
}

void moveItem(void)
{
	uint8_t *p;
	uint8_t *src;

	p = &INVENTORY.types.array[INVENTORY_POINTER];
	src = &INVENTORY.types.array[ITEM_MOVE_SOURCE];
	swapByte(p, src);
	swapByte(&INVENTORY.amounts.array[INVENTORY_POINTER], &INVENTORY.amounts.array[ITEM_MOVE_SOURCE]);
	swapByte(&INVENTORY.names.array[INVENTORY_POINTER], &INVENTORY.names.array[ITEM_MOVE_SOURCE]);
	if (*p != 0xff) {
		updateItemDescription();
	}
}

void selectSorting(void)
{
	if (INVENTORY_ACTION != SORT_SELECTED + 0xb) {
		sortItems(SORT_SELECTED + 0xb);
		INVENTORY_ACTION = SORT_SELECTED + 0xb;
	}
}

void selectDropItem(void)
{
	if (DROP_ITEM_SELECTED_OPTION == 0) {
		INVENTORY_ACTION = 7;
	} else {
		INVENTORY_ACTION = 0xf;
	}
}

void selectOptionMenu(void)
{
	Item *item;

	if ((INVENTORY_ACTION != 0xb) && (INVENTORY_ACTION != 0xc) &&
	    (INVENTORY_ACTION != 0xd) && (UI_BOX_DATA[3].state == 0)) {
		switch (ITEM_MENU_SELECTED) {
		case 0:
			if (INVENTORY.types.array[INVENTORY_POINTER] != 0xff) {
				item = &ITEM_PARA[INVENTORY.types.array[INVENTORY_POINTER]];
				switch (GAME_STATE) {
				case 0:
					if ((item->itemColor == 0) ||
					    (item->itemColor == 2)) {
						INVENTORY_ACTION = 5;
					}
					break;
				case 1:
					if ((item->itemColor == 1) ||
					    (item->itemColor == 2)) {
						INVENTORY_ACTION = 5;
					}
					break;
				}
			}
			break;
		case 1:
			INVENTORY_ACTION = 6;
			ITEM_MOVE_SOURCE = INVENTORY_POINTER;
			UI_BOX_DATA[0].state = 4;
			return;
		case 2:
			INVENTORY_ACTION = 8;
			break;
		case 3:
			if (GAME_STATE == 0) {
				if ((INVENTORY.types.array[INVENTORY_POINTER] != 0xff) && ITEM_PARA[INVENTORY.types.array[INVENTORY_POINTER]].droppable == 1) {
					INVENTORY_ACTION = 0xe;
				}
			}
			break;
		}
	}
}

void selectRegularInventory(void)
{
	if ((INVENTORY_ACTION != 0xb) && (INVENTORY_ACTION != 0xc) &&
	    (INVENTORY_ACTION != 0xd) && (INVENTORY_ACTION != 5) &&
	    (UI_BOX_DATA[2].state == 0)) {
		if (UI_BOX_DATA[0].state == 0) {
			INVENTORY_ACTION = 1;
			return;
		}
		if (INVENTORY_ACTION == 6) {
			playSound(0, 4);
			UI_BOX_DATA[0].state = 1;
			INVENTORY_ACTION = 0;
			moveItem();
			return;
		}
		if ((INVENTORY_ACTION < 5) && (TAMER_ITEM.worldItem.type == 0xff) &&
		    (UI_BOX_DATA[1].state == 1)) {
			INVENTORY_ACTION = 3;
		}
	}
}

void sortItems(int16_t mode)
{
	long counts[6];
	uint8_t desired[30];
	uint8_t categories[6][30];
	int32_t i;
	int32_t j;
	uint8_t *order;

	ITEM_MOVE_TARGET = 0xff;
	for (i = 0; i < INVENTORY.size; i++) {
		desired[i] = 0xff;
	}
	for (i = 0; i < 6; i++) {
		counts[i] = 0;
		for (j = 0; j < INVENTORY.size; j++) {
			if (INVENTORY.types.array[j] != 0xff) {
				if (i == ITEM_PARA[INVENTORY.types.array[j]].sortingValue) {
					categories[i][counts[i]] = INVENTORY.types.array[j];
					counts[i]++;
				}
			}
		}
		sortItemsById(categories[i], counts[i]);
	}
	i = 0;
	order = ITEM_SORT_ORDER[mode - 11];
	for (j = 0; j < 6; j++) {
		copyItemArray(categories[*order], &desired[i], counts[*order]);
		i += counts[*order];
		order++;
	}
	for (i = 0; i < INVENTORY.size; i++) {
		if (desired[i] == 0xff) {
			break;
		}
		if (INVENTORY.types.array[i] != desired[i]) {
			for (j = 0; j < INVENTORY.size; j++) {
				if (INVENTORY.types.array[j] == desired[i]) {
					break;
				}
			}
			swapByte(&INVENTORY.types.array[i], &INVENTORY.types.array[j]);
			swapByte(&INVENTORY.amounts.array[i], &INVENTORY.amounts.array[j]);
			swapByte(&INVENTORY.names.array[i], &INVENTORY.names.array[j]);
		}
	}
}

void copyItemArray(uint8_t *src, uint8_t *dst, long n)
{
	int32_t i;

	for (i = 0; i < n; ++i) {
		*dst++ = *src++;
	}
}

void updateInventoryInputRepeatCounter(void)
{
	if (POLLED_INPUT == POLLED_INPUT_PREVIOUS) {
		++INVENTORY_INPUT_REPEAT_COUNTER;
		return;
	}
	INVENTORY_INPUT_REPEAT_COUNTER = 0;
}

int32_t isInventoryButtonPressed(int32_t mask)
{
	if ((mask & (POLLED_INPUT & ~POLLED_INPUT_PREVIOUS)) ||
	    ((INVENTORY_INPUT_REPEAT_COUNTER >= 0xb) && (mask & POLLED_INPUT))) {
		return 1;
	}
	return 0;
}

void tickInventoryOptionSelector(uint8_t *cursor, int32_t unused, int16_t max)
{
	updateInventoryInputRepeatCounter();
	if ((isInventoryButtonPressed(0x1000) != 0) && (*cursor != 0)) {
		playSound(0, 2);
		--*cursor;
	}
	if ((isInventoryButtonPressed(0x4000) != 0) && (*cursor < max - 1)) {
		playSound(0, 2);
		++*cursor;
	}
}
