#include <libgpu.h>
#include <libgs.h>

#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/world_object.h>

void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uPos, int32_t vPos, int32_t width, int32_t height);
void BTL_tickCommandMenu(void);
void BTL_renderCommandMenu(int32_t arg0);
void BTL_removeCommandMenu(void);

/*
 * command_shout.c state is defined here because it's in .rodata section but
 * it gets modified.
 */
const BtlCommandShout BTL_COMMAND_SHOUT = { -1, 0, 0, 0 };

int16_t BTL_COMMAND_MENU_X;
int16_t BTL_COMMAND_MENU_Y;
uint8_t BTL_COMMAND_MENU_BLINK;
uint8_t BTL_COMMAND_MENU_TIMER;
uint8_t BTL_COMMAND_MENU_X_STEP;
uint8_t BTL_COMMAND_MENU_Y_STEP;
uint8_t BTL_COMMAND_MENU_LAYOUT;

static void *command_menu_sbss_order[] = {
	&BTL_COMMAND_MENU_LAYOUT,
	&BTL_COMMAND_MENU_Y_STEP,
	&BTL_COMMAND_MENU_X_STEP,
	&BTL_COMMAND_MENU_TIMER,
	&BTL_COMMAND_MENU_BLINK,
	&BTL_COMMAND_MENU_Y,
	&BTL_COMMAND_MENU_X,
};

void BTL_initializeCommandMenu(void)
{
	BTL_COMMAND_MENU_ACTIVE = 1;
	BTL_COMMAND_MENU_X = -0x98;
	BTL_COMMAND_MENU_Y = -0x78;
	BTL_COMMAND_MENU_BLINK = 0;
	BTL_COMMAND_MENU_TIMER = 0;
	BTL_COMMAND_MENU_X_STEP = 0;
	BTL_COMMAND_MENU_Y_STEP = 0;

	switch (COMBAT_DATA_PTR->player.numCommands[0]) {
	case 3:
		BTL_COMMAND_MENU_LAYOUT = 0;
		break;
	case 4:
		BTL_COMMAND_MENU_LAYOUT = 1;
		break;
	case 5:
		BTL_COMMAND_MENU_LAYOUT = 2;
		break;
	case 7:
		BTL_COMMAND_MENU_LAYOUT = 3;
		break;
	case 8:
		BTL_COMMAND_MENU_LAYOUT = 4;
		break;
	case 9:
		BTL_COMMAND_MENU_LAYOUT = 5;
		break;
	}

	BTL_COMMAND_MENU_CLOSED_FRAMES = 0;
	addObject(0x198, 0, (TickFunction)BTL_tickCommandMenu, BTL_renderCommandMenu);
}

void BTL_tickCommandMenu(void)
{
	BTL_COMMAND_MENU_TIMER++;

	if (BTL_COMMAND_MENU_ACTIVE == 1) {
		if (BTL_COMMAND_MENU_Y_STEP < 7) {
			BTL_COMMAND_MENU_Y_STEP++;
		}
		if ((BTL_COMMAND_MENU_Y_STEP == 7) && (BTL_COMMAND_MENU_X_STEP < 7)) {
			BTL_COMMAND_MENU_X_STEP++;
		}
		BTL_COMMAND_MENU_Y = BTL_COMMAND_MENU_SLIDE_Y[BTL_COMMAND_MENU_Y_STEP];
		BTL_COMMAND_MENU_X = BTL_COMMAND_MENU_SLIDE_X[BTL_COMMAND_MENU_X_STEP];
		if ((GAME_STATE == 1) && ((BTL_COMMAND_MENU_TIMER % 8) == 0)) {
			BTL_COMMAND_MENU_BLINK = (BTL_COMMAND_MENU_BLINK + 1) & 1;
		}
	} else {
		if (BTL_COMMAND_MENU_X_STEP != 0) {
			BTL_COMMAND_MENU_X_STEP--;
		}
		if ((BTL_COMMAND_MENU_X_STEP == 0) && (BTL_COMMAND_MENU_Y_STEP != 0)) {
			BTL_COMMAND_MENU_Y_STEP--;
		}
		BTL_COMMAND_MENU_Y = BTL_COMMAND_MENU_SLIDE_Y[BTL_COMMAND_MENU_Y_STEP];
		BTL_COMMAND_MENU_X = BTL_COMMAND_MENU_SLIDE_X[BTL_COMMAND_MENU_X_STEP];
	}
}

void BTL_renderCommandMenu(arg0)
int16_t arg0;
{
	POLY_FT4 *prim;
	int32_t i;
	int16_t x;
	int16_t rowY;
	int16_t width;
	int16_t base;
	int16_t count;

	base = ((COMBAT_DATA_PTR->player.numCommands[0] - 1) * 0xe) - 0x8c;
	if (GAME_STATE == 1) {
		BTL_drawHoveredCommandName();
	}

	prim = (POLY_FT4 *)GsGetWorkBase();
	count = COMBAT_DATA_PTR->player.numCommands[0] - 1;

	if (GAME_STATE == 1) {
		x = base - (COMBAT_DATA_PTR->player.hoveredCommand[0] * 0xe);
		SetPolyFT4(prim);
		prim->tpage = getTPage(0, 0, 960, 256);
		setClut(prim, 282, 497);
		setRGB0(prim, 0x80, 0x80, 0x80);
		setUVDataPolyFT4(prim, 0x3d, 0xe0, 0x16, 0x16);
		if ((count % 2) == 0) {
			int32_t y;

			if ((COMBAT_DATA_PTR->player.hoveredCommand[0] % 2) == 0) {
				y = -0x64;
			} else {
				y = -0x5a;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		} else {
			int32_t y;

			if ((COMBAT_DATA_PTR->player.hoveredCommand[0] % 2) == 1) {
				y = -0x64;
			} else {
				y = -0x5a;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	for (i = 1; i < COMBAT_DATA_PTR->player.numCommands[0]; i++) {
		SetPolyFT4(prim);
		prim->tpage = getTPage(0, 0, 960, 256);
		setClut(prim, 272, 496);
		setRGB0(prim, 0x80, 0x80, 0x80);
		BTL_setCommandIconUV((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[arg0]], prim, COMBAT_DATA_PTR->player.availableCommands[0][i]);
		if (i > 0) {
			rowY = ((i - 1) * 0xe) + 0xb;
		} else {
			rowY = 0;
		}
		if ((i == 0) || (i == (COMBAT_DATA_PTR->player.numCommands[0] - 1))) {
			width = 0xb;
		} else {
			width = 0xe;
		}
		x = (int16_t)base - (i * 0xe);
		if ((count % 2) == 0) {
			setXY4(prim, x, ((i % 2) == 0) ? BTL_COMMAND_MENU_X + 3 : BTL_COMMAND_MENU_X + 0xd, x + 0x10, ((i % 2) == 0) ? BTL_COMMAND_MENU_X + 3 : BTL_COMMAND_MENU_X + 0xd, x, ((i % 2) == 0) ? BTL_COMMAND_MENU_Y - 0xd : BTL_COMMAND_MENU_Y - 3, x + 0x10, ((i % 2) == 0) ? BTL_COMMAND_MENU_Y - 0xd : BTL_COMMAND_MENU_Y - 3);
		} else {
			setXY4(prim, x, ((i % 2) == 1) ? BTL_COMMAND_MENU_X + 3 : BTL_COMMAND_MENU_X + 0xd, x + 0x10, ((i % 2) == 1) ? BTL_COMMAND_MENU_X + 3 : BTL_COMMAND_MENU_X + 0xd, x, ((i % 2) == 1) ? BTL_COMMAND_MENU_Y - 0xd : BTL_COMMAND_MENU_Y - 3, x + 0x10, ((i % 2) == 1) ? BTL_COMMAND_MENU_Y - 0xd : BTL_COMMAND_MENU_Y - 3);
		}
		if ((i == COMBAT_DATA_PTR->player.hoveredCommand[0]) && (BTL_COMMAND_MENU_BLINK == 1)) {
			prim->u0 += 0x10;
			prim->u1 += 0x10;
			prim->u2 += 0x10;
			prim->u3 += 0x10;
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	for (i = 0; i < COMBAT_DATA_PTR->player.numCommands[0]; i++) {
		SetPolyFT4(prim);
		setSemiTrans(prim, 1);
		setTPage(prim, 0, 0, 960, 256);
		setClut(prim, 272, 497);
		setRGB0(prim, 0x80, 0x80, 0x80);
		setUVWH(prim, BTL_COMMAND_LABEL_U[BTL_COMMAND_MENU_LAYOUTS[BTL_COMMAND_MENU_LAYOUT][i]], 0xe0, BTL_COMMAND_LABEL_W[BTL_COMMAND_MENU_LAYOUTS[BTL_COMMAND_MENU_LAYOUT][i]], 31);
		if (i > 0) {
			rowY = ((i - 1) * 0xe) + 0xb;
		} else {
			rowY = 0;
		}
		if ((i == 0) || (i == (COMBAT_DATA_PTR->player.numCommands[0] - 1))) {
			width = 0xb;
		} else {
			width = 0xe;
		}
		setXY4(prim, rowY - 0x8f, BTL_COMMAND_MENU_X, (rowY - 0x8f) + width, BTL_COMMAND_MENU_X, rowY - 0x8f, BTL_COMMAND_MENU_Y, (rowY - 0x8f) + width, BTL_COMMAND_MENU_Y);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void BTL_removeCommandMenu(void)
{
	if (BTL_COMMAND_MENU_ACTIVE != -1) {
		BTL_COMMAND_MENU_CLOSED_FRAMES = 0;
		BTL_COMMAND_MENU_ACTIVE = -1;
		removeObject(0x198, 0);
	}
}

int32_t BTL_isCommandMenuClosed(void)
{
	if ((BTL_COMMAND_MENU_X_STEP == 0) && (BTL_COMMAND_MENU_Y_STEP == 0)) {
		BTL_removeCommandMenu();
		BTL_COMMAND_MENU_CLOSED_FRAMES++;
		if (BTL_COMMAND_MENU_CLOSED_FRAMES >= 4) {
			return 1;
		}
	}

	return 0;
}
