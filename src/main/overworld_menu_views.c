#include <dw/graphics.h>
#include <dw/move.h>
#include <dw/ui.h>

typedef struct {
	int16_t posX;
	int16_t posY;
	uint8_t width;
	uint8_t height;
	uint8_t texX;
	uint8_t texY;
} IconRect;

typedef struct {
	int16_t posX;
	int16_t posY;
	int16_t uvWidth;
	uint8_t uvX;
	uint8_t uvY;
} StringRect;

extern int8_t MENU_STATE;
extern int8_t SELECTED_CARD;
extern uint8_t MAIN_D_80134234[4];
extern int16_t MAIN_D_80134D38;
extern int16_t MAIN_D_80134D3A;
extern int16_t TECH_VIEW_LINES1[];
extern int16_t TECH_VIEW_LINES2[];
extern int16_t TECH_VIEW_LINES3[];
extern IconRect MAIN_D_801241CC[];
extern IconRect MAIN_D_80124044[];
extern StringRect MAIN_D_80124234[];
extern IconRect MAIN_D_80124294[];
extern char MAIN_D_80123E78[];

int32_t drawDigimonMovesText(void);
u_short GetTPage(int32_t tp, int32_t abr, int32_t x, int32_t y);
u_short GetClut(int32_t x, int32_t y);
void renderRectPolyFT4(int16_t posX, int16_t posY, int32_t width,
		       int32_t height, uint8_t texX, uint8_t texY,
		       int16_t texturePage, int16_t clut, int32_t zIndex,
		       int8_t flag);
void renderString();
void renderBoxBar(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t r,
		  uint8_t g, uint8_t b, uint8_t flags, int32_t otz);
void renderDigimonMoveBox(void);
void renderLinePrimitive(int32_t color, int32_t x0, int32_t y0, int32_t x1,
			 int32_t y1, int32_t otz, int32_t flag);
int32_t getCardAmount(uint8_t card);
int32_t loadStackedTIMEntry(char *path, void *buffer, int32_t offset,
			     int32_t sectors);
void renderNumber(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
		  int32_t f);
void renderSeperatorLines(int16_t *lines, int32_t b, ...);
void renderDigimonMovesSelected(int32_t arg);
int32_t drawMoveViewHelpStrings(void);
void renderDigimonMovesView(void);
int32_t loadCardImage(int32_t id);
void renderCardImage(void);
void renderCardCount(void);
void renderDigimonStatsView(void);
void renderMenuTab(void);
void renderPlayerInfoView(void);

static void *overworld_menu_views_functions[] = {
	renderPlayerInfoView,
	renderMenuTab,
	renderDigimonMovesView,
	renderDigimonStatsView,
	renderCardCount,
	renderCardImage,
	loadCardImage,
};

int32_t loadCardImage(int32_t id)
{
	return loadStackedTIMEntry(MAIN_D_80123E78, (uint8_t *)0x80088800,
				    id * 0xE, 0xE);
}

void renderCardImage(void)
{
	int16_t tpage;
	int16_t clut;

	tpage = GetTPage(1, 0, 0x240, 0x100);
	clut = GetClut(0x1c0, 0x1ff);
	renderRectPolyFT4(-0x4b, -0x54, 0x96, 0xb4, 0, 0, tpage, clut, 4, 0);
}

void renderCardCount(void)
{
	renderNumber(0, 0x59, 0x4b, 1, getCardAmount(SELECTED_CARD), 3);
	renderString(0, 0x6b, 0x4c, 0xc, 0xc, 0xd8, 0xc, 0);
}

INCLUDE_ASM("asm/main/nonmatchings/overworld_menu_views", renderDigimonStatsView);

void renderDigimonMovesView(void)
{
	extern uint8_t MAIN_D_80134237;
	uint8_t uVar1;
	uint16_t clut;
	int32_t i;
	int32_t iVar3;
	int32_t iVar6;
	int32_t j;
	int32_t iVar7;
	Move *mv;
	IconRect *icon;
	StringRect *sr;
	if (MENU_STATE > 0 && MENU_STATE < 7) {
		for (i = 0; i < 0xd; i++) {
			icon = &MAIN_D_801241CC[i];
			renderRectPolyFT4((int16_t)icon->posX,
					  (int16_t)icon->posY,
					  icon->width,
					  icon->height,
					  icon->texX,
					  icon->texY + 0x80, 5, 0x7b06, 5, 0);
		}
		renderString(3, -0x8e, -0x57, 0x3c, 0xc, 0, 0x48, 5, 1);
		j = 0;
		while (j < 3) {
			uVar1 = MAIN_D_80134234[j];
			if (uVar1 != 0xff) {
				mv = &MOVE_DATA[MAIN_D_80134234[j]];
				renderString(0, -0x7c, j * 0xf - 0x42, 0x78, 0xc, 0, j * 0xc + 0x18, 5, 1);
				renderNumber(0, 0x10, j * 0xf - 0x43, 3, mv->power, 5);
				renderNumber(0, 0x39, j * 0xf - 0x43, 3, mv->mpCost * 3, 5);
				if (mv->range != 0) {
					renderString(0, 0x65, j * 0xf - 0x42, 0xc, 0xc,
						     (mv->range - 1) * 0x24, 0x78, 5, 1);
				}
				if (mv->status != 0) {
					renderRectPolyFT4(0x7d, j * 0xf - 0x43, 0xc, 0xc,
							  (mv->status - 1) * 0xc, 0x8c, 5, 0x7a06, 5, 0);
				}
			}
			j++;
		}
		renderSeperatorLines(TECH_VIEW_LINES1, 3, 3);
	}
	switch (MENU_STATE) {
	case 0:
		if (drawDigimonMovesText() == 1) {
			MENU_STATE = 1;
		}
		break;
	case 1:
		renderString(3, -0x8e, -0xf, 0x24, 0xc, 0x3c, 0x48, 5, 1);
		for (i = 0; i < 3; i++) {
			icon = &MAIN_D_801241CC[i];
			renderRectPolyFT4((int16_t)icon->posX, -9,
					  icon->width,
					  icon->height,
					  icon->texX,
					  icon->texY + 0x80, 5, 0x7b06, 5, 0);
		}
		renderRectPolyFT4(0x75, 4, 4, 4, 0x78, 0x8c, 5, 0x7b06, 5, 0);
		renderSeperatorLines(TECH_VIEW_LINES2, 3, 5);
		if (MAIN_D_80134237 != 0xff) {
			renderString(3, -0x8e, -0xf, 0x24, 0xc, 0x3c, 0x48, 5, 1);
			renderString(0, -0x7c, 1, 0x84, 0xc, 0, 0x3c, 5, 1);
			renderNumber(0, 0x10, 0, 3, MOVE_DATA[MAIN_D_80134237].power, 5);
			renderString(0, 0x65, 1, 0xc, 0xc,
				     (MOVE_DATA[MAIN_D_80134237].range - 1) * 0xc + 0x48, 0x6c, 5, 1);
			if (MOVE_DATA[MAIN_D_80134237].status != 0) {
				renderRectPolyFT4(0x7d, 0, 0xc, 0xc,
						  (MOVE_DATA[MAIN_D_80134237].status - 1) * 0xc, 0x8c, 5,
						  0x7a06, 5, 0);
			}
		}
		renderString(0xf, -0x82, 0x19, 0xc, 0xc, 0, 0x54, 5, 1);
		renderString(0, -0x76, 0x19, 0xa8, 0xc, 0xc, 0x54, 5, 1);
		renderString(0, -0x76, 0x27, 0x84, 0xc, 0xc, 0x60, 5, 1);
		renderString(7, -0x82, 0x27, 0xc, 0xc, 0, 0x60, 5, 1);
		renderString(0, 0xe, 0x27, 0x18, 0xc, 0x90, 0x60, 5, 1);
		break;
	case 2:
	case 4:
		UI_BOX_DATA[1].finalPos.h = UI_BOX_DATA[1].finalPos.h - 0x27;
		if (UI_BOX_DATA[1].finalPos.h < 0x4a) {
			++MENU_STATE;
		}
		break;
	case 3:
	case 5:
		UI_BOX_DATA[1].finalPos.h = UI_BOX_DATA[1].finalPos.h + 0x27;
		if (0xbd < UI_BOX_DATA[1].finalPos.h) {
			MENU_STATE = (MENU_STATE == 3) ? 6 : 1;
		}
		break;
	case 6:
		renderString(3, -0x8e, -0xf, 0x3c, 0xc, 0x60, 0x48, 5, 1);
		renderSeperatorLines(TECH_VIEW_LINES3, 0xc, 5);
		for (i = 0; i < 4; i++) {
			icon = &MAIN_D_80124044[i];
			renderRectPolyFT4((int16_t)icon->posX,
					  (int16_t)icon->posY,
					  icon->width,
					  icon->height,
					  icon->texX,
					  icon->texY + 0x80, 5, 0x7b06, 5, 0);
		}
		renderString(0, -0x87, 0x20, 0x30, 0xc, 0x3c, 0, 5, 1);
		renderString(0, -0x81, 0x2e, 0x24, 0xc, 0, 0x6c, 5, 1);
		renderString(0xc, -0x74, 0x4c, 0x18, 0xc, 0x30, 0x6c, 5, 1);
		renderRectPolyFT4(-0x74, 0x3c, 10, 10, 0x80, 0x8c, 5, 0x7b06, 5, 0);
		j = 0;
		while (j < 7) {
			renderRectPolyFT4(-0x43, j * 0xf - 8, 0x14, 10, j * 0x14, 0x98, 5, 0x7b06, 5, 0);
			renderRectPolyFT4(100, j * 0xf - 8, 0x14, 10, j * 0x14, 0x98, 5, 0x7b06, 5, 0);
			j++;
		}
		j = 0;
		while (j < 8) {
			renderRectPolyFT4(j * 0x12 - 0x26, -0xf, 4, 5, j * 4 + 0x94, 0x8c, 5, 0x7b06, 5, 0);
			j++;
		}
		renderRectPolyFT4(MAIN_D_80134D3A - 0xa0,
				  MAIN_D_80134D38 - 0x78, 0x12, 0x10, 0xc0, 0x8c, 5, 0x7b06, 5, 0);
		iVar3 = 0;
		iVar6 = 0;
		while (iVar3 < 7) {
			j = 0;
			while (j < 8) {
				iVar7 = iVar6 - 7;
				renderBoxBar((int16_t)(j * 0x12 - 0x2a), (int16_t)iVar7, 0xc, 0xc,
					     0x4e, 0x60, 0x6e, 0x80, 5);
				j++;
			}
			iVar3++;
			iVar6 += 0xf;
		}
		if (0xa8 < MAIN_D_80134D3A) {
			renderDigimonMovesSelected(0);
		} else {
			renderDigimonMovesSelected(1);
		}
		renderDigimonMoveBox();
		break;
	case 7:
		if (drawMoveViewHelpStrings() == 1) {
			MENU_STATE = 8;
		}
		break;
	case 8:
		renderLinePrimitive(0x20202, -0x92, -4, 0x92, -4, 5, 0);
		renderLinePrimitive(0xfad990, -0x93, -3, 0x93, -3, 5, 0);
		renderLinePrimitive(0x20202, -0x92, -2, 0x92, -2, 5, 0);
		iVar7 = 1;
		renderString(3, -0x8e, -0x52, 0xa0, 0xc, 0, 0x30, 5, iVar7);
		renderString(0xe, -0x72, -0x32, 0xc, 0xc, 0xcc, 0x30, 5, iVar7);
		renderString(0xe, 6, -0x21, 0xc, 0xc, 0xcc, 0x30, 5, 1);
		renderString(0xe, -0x2a, -0x13, 0xc, 0xc, 0xcc, 0x30, 5, 1);
		for (i = 0; i < 0xc; i++) {
			sr = &MAIN_D_80124234[i];
			renderString(0, sr->posX,
				     sr->posY + 1,
				     sr->uvWidth, 0xc,
				     sr->uvX,
				     sr->uvY, 5, iVar7);
		}
		renderString(0, 0x42, 0x33, 0x3c, 0xc, 0x78, 0x48, 5, 1);
		renderString(0, 0x42, 0x43, 0x3c, 0xc, 0x84, 0x54, 5, iVar7);
		renderRectPolyFT4(-0x81, -0x41, 0xc, 0xc, 0xb4, 0x8c, 5, 0x7b06, 5, 0);
		renderRectPolyFT4(-0x81, -0x22, 0xc, 0xc, 0xb4, 0x8c, 5, 0x7b06, 5, 0);
		renderRectPolyFT4(-0x7b, 0x50, 0x12, 0x10, 0xc0, 0x8c, 5, 0x7b06, 5, 0);
		for (i = 0; i < 7; i++) {
			icon = &MAIN_D_80124294[i];
			if (i != 2) {
				if (i == 5) {
					clut = 0x7a46;
				} else {
					clut = 0x7a06;
				}
				renderRectPolyFT4((int16_t)icon->posX,
						  (int16_t)icon->posY,
						  icon->width,
						  icon->height,
						  icon->texX,
						  icon->texY + 0x80, 5, clut, 5, 0x80);
			}
		}
		renderBoxBar(0x52, -0x22, 0xc, 0xc, 200, 0, 0x28, 0x81, 4);
		renderBoxBar(-0x78, 2, 0xc, 0xc, 200, 0, 0x28, 0x81, 4);
		renderBoxBar(-0x78, 0x22, 0xc, 0xc, 0x4e, 0x60, 0x6e, 0x80, 4);
		renderBoxBar(-0x78, 0x32, 0xc, 0xc, 0x68, 0x68, 0x68, 0x83, 4);
		renderBoxBar(-0x78, 0x42, 0xc, 0xc, 0x68, 0x68, 0x68, 0x83, 4);
		break;
	case 9:
		if (drawDigimonMovesText() == 1) {
			MENU_STATE = 6;
		}
	}
}

INCLUDE_ASM("asm/main/nonmatchings/overworld_menu_views", renderMenuTab);

INCLUDE_ASM("asm/main/nonmatchings/overworld_menu_views", renderPlayerInfoView);
