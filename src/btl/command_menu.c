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

int16_t MAIN_D_80135084;
int16_t MAIN_D_80135086;
uint8_t MAIN_D_80135088;
uint8_t MAIN_D_80135089;
uint8_t MAIN_D_8013508A;
uint8_t MAIN_D_8013508B;
uint8_t MAIN_D_8013508C;

static void *command_menu_sbss_order[] = {
	&MAIN_D_8013508C,
	&MAIN_D_8013508B,
	&MAIN_D_8013508A,
	&MAIN_D_80135089,
	&MAIN_D_80135088,
	&MAIN_D_80135086,
	&MAIN_D_80135084,
};

void BTL_initializeCommandMenu(void)
{
	MAIN_D_80135084 = -0x98;
	MAIN_D_80135086 = -0x78;
	MAIN_D_80135094 = 1;
	MAIN_D_80135088 = 0;
	MAIN_D_80135089 = 0;
	MAIN_D_8013508A = 0;
	MAIN_D_8013508B = 0;

	switch (COMBAT_DATA_PTR->player.numCommands[0]) {
	case 3:
		MAIN_D_8013508C = 0;
		break;
	case 4:
		MAIN_D_8013508C = 1;
		break;
	case 5:
		MAIN_D_8013508C = 2;
		break;
	case 6:
		break;
	case 7:
		MAIN_D_8013508C = 3;
		break;
	case 8:
		MAIN_D_8013508C = 4;
		break;
	case 9:
		MAIN_D_8013508C = 5;
		break;
	}

	MAIN_D_801346F8 = 0;
	addObject(0x198, 0, (TickFunction)BTL_tickCommandMenu, BTL_renderCommandMenu);
}

void BTL_tickCommandMenu(void)
{
	MAIN_D_80135089++;

	if (MAIN_D_80135094 == 1) {
		if (MAIN_D_8013508B < 7) {
			MAIN_D_8013508B++;
		}
		if ((MAIN_D_8013508B == 7) && (MAIN_D_8013508A < 7)) {
			MAIN_D_8013508A++;
		}
		MAIN_D_80135086 = BTL_D_80072EB8[MAIN_D_8013508B];
		MAIN_D_80135084 = BTL_D_80072EC8[MAIN_D_8013508A];
		if ((GAME_STATE == 1) && ((MAIN_D_80135089 % 8) == 0)) {
			MAIN_D_80135088 = (MAIN_D_80135088 + 1) & 1;
		}
	} else {
		if (MAIN_D_8013508A != 0) {
			MAIN_D_8013508A--;
		}
		if ((MAIN_D_8013508A == 0) && (MAIN_D_8013508B != 0)) {
			MAIN_D_8013508B--;
		}
		MAIN_D_80135086 = BTL_D_80072EB8[MAIN_D_8013508B];
		MAIN_D_80135084 = BTL_D_80072EC8[MAIN_D_8013508A];
	}
}

void BTL_renderCommandMenu(int32_t arg0)
{
	POLY_FT4 *prim;
	int32_t i;
	int32_t off;
	int16_t base;
	int16_t count;
	int16_t x;
	int32_t y;
	int16_t rowY;
	int16_t width;

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
			if ((COMBAT_DATA_PTR->player.hoveredCommand[0] % 2) == 0) {
				y = -0x64;
			} else {
				y = -0x5a;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		} else {
			if ((COMBAT_DATA_PTR->player.hoveredCommand[0] % 2) == 1) {
				y = -0x64;
			} else {
				y = -0x5a;
			}
			setPosDataPolyFT4(prim, x - 3, y, 0x16, 0x16);
		}
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	for (i = 1, off = 0xe; i < COMBAT_DATA_PTR->player.numCommands[0]; i++, off += 0xe) {
		SetPolyFT4(prim);
		prim->tpage = getTPage(0, 0, 960, 256);
		setClut(prim, 272, 496);
		setRGB0(prim, 0x80, 0x80, 0x80);
		BTL_setCommandIconUV((DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[arg0]], prim, COMBAT_DATA_PTR->player.availableCommands[0][i]);
		x = base - off;
		if ((count % 2) == 0) {
			setXY4(prim, x, ((i % 2) == 0) ? MAIN_D_80135084 + 3 : MAIN_D_80135084 + 0xd, x + 0x10, ((i % 2) == 0) ? MAIN_D_80135084 + 3 : MAIN_D_80135084 + 0xd, x, ((i % 2) == 0) ? MAIN_D_80135086 - 0xd : MAIN_D_80135086 - 3, x + 0x10, ((i % 2) == 0) ? MAIN_D_80135086 - 0xd : MAIN_D_80135086 - 3);
		} else {
			setXY4(prim, x, ((i % 2) == 1) ? MAIN_D_80135084 + 3 : MAIN_D_80135084 + 0xd, x + 0x10, ((i % 2) == 1) ? MAIN_D_80135084 + 3 : MAIN_D_80135084 + 0xd, x, ((i % 2) == 1) ? MAIN_D_80135086 - 0xd : MAIN_D_80135086 - 3, x + 0x10, ((i % 2) == 1) ? MAIN_D_80135086 - 0xd : MAIN_D_80135086 - 3);
		}
		if ((i == COMBAT_DATA_PTR->player.hoveredCommand[0]) && (MAIN_D_80135088 == 1)) {
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
		setUVWH(prim, MAIN_D_80134728[BTL_D_80072E7C[MAIN_D_8013508C][i]], 0xe0, MAIN_D_80134730[BTL_D_80072E7C[MAIN_D_8013508C][i]], 31);
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
		setXY4(prim, rowY - 0x8f, MAIN_D_80135084, (rowY - 0x8f) + width, MAIN_D_80135084, rowY - 0x8f, MAIN_D_80135086, (rowY - 0x8f) + width, MAIN_D_80135086);
		AddPrim(ACTIVE_ORDERING_TABLE->org + 7, prim++);
	}

	GsSetWorkBase((PACKET *)prim);
}

void BTL_removeCommandMenu(void)
{
	if (MAIN_D_80135094 != -1) {
		MAIN_D_801346F8 = 0;
		MAIN_D_80135094 = -1;
		removeObject(0x198, 0);
	}
}

int32_t BTL_isCommandMenuClosed(void)
{
	if ((MAIN_D_8013508A == 0) && (MAIN_D_8013508B == 0)) {
		BTL_removeCommandMenu();
		MAIN_D_801346F8++;
		if (MAIN_D_801346F8 >= 4) {
			return 1;
		}
	}

	return 0;
}
