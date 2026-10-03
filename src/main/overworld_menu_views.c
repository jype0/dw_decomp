#include <string.h>

#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/main.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/ui.h>

#include "common.h"

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
extern int32_t MONEY;
extern int16_t MAIN_D_80124424[];
extern StringRect MAIN_D_801244EC[11];
extern RECT MAIN_D_80124494[11];
extern char *TAMER_LEVEL_TITLES[];
int32_t drawPlayerInfoStrings(void);
int32_t isTriggerSet(uint16_t trigger);
int32_t hasMedal(uint16_t medal);
void renderInsetBox(int16_t a, int16_t b, int16_t c, int16_t d, int32_t otz);
void renderDigiviceEntity(Entity *entity, int32_t entityId);
extern int8_t SELECTED_CARD;
extern uint8_t EQUIPPED_MOVES[4];
extern int16_t MAIN_D_80134D38;
extern int16_t MAIN_D_80134D3A;
extern int16_t TECH_VIEW_LINES1[];
extern int16_t TECH_VIEW_LINES3[];
extern IconRect MAIN_D_801241CC[];
extern IconRect MAIN_D_80124044[];
extern StringRect MAIN_D_80124234[];
extern IconRect MAIN_D_80124294[];
extern char MAIN_D_80123E78[];

int32_t drawDigimonMovesText(void);
u_short GetTPage(int32_t tp, int32_t abr, int32_t x, int32_t y);
u_short GetClut(int32_t x, int32_t y);
void renderRectPolyFT4(int16_t posX, int16_t posY, uint8_t width,
		       uint8_t height, uint8_t texX, uint8_t texY,
		       int16_t texturePage, int16_t clut, int32_t zIndex,
		       int8_t flag);
void renderString();
void renderBox(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t r,
	       uint8_t g, uint8_t b, uint8_t flags, int32_t otz);
void renderDigimonMoveBox(void);
void drawLine2P(int32_t color, int32_t x0, int32_t y0, int32_t x1,
		int32_t y1, int32_t otz, int32_t flag);
int32_t getCardAmount(uint8_t card);
int32_t loadStackedTIMEntry(char *path, u_long buffer, int32_t offset,
			     int32_t sectors);
void renderNumber(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
		  int32_t f);
void renderSeparatorLines(int16_t *lines, int32_t b, ...);
void renderDigimonMovesSelected(int16_t panel);
int32_t drawMoveViewHelpStrings(void);
void renderDigimonMovesView(void);
int32_t loadCardImage(int32_t id);
void renderCardImage(void);
void renderCardCount(void);
void renderDigimonStatsView(void);
void renderMenuTab(int16_t x, int8_t w, int8_t layer);
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

int32_t loadCardImage(id)
int8_t id;
{
	return loadStackedTIMEntry(MAIN_D_80123E78, (u_long)TEXTURE_BUFFER,
				    id * 0xe, 0xe);
}

void renderCardImage(void)
{
	renderRectPolyFT4(-0x4b, -0x54, 0x96, 0xb4, 0, 0, GetTPage(1, 0, 576, 256), GetClut(448, 511), 4, 0);
}

void renderCardCount(void)
{
	renderNumber(0, 0x59, 0x4b, 1, getCardAmount(SELECTED_CARD), 3);
	renderString(0, 0x6b, 0x4c, 0xc, 0xc, 0xd8, 0xc, 0);
}

typedef struct {
	uint8_t v[21];
} StatsIconClutTable;
extern StatsIconClutTable MAIN_D_80123DB8;
extern int16_t MAIN_D_80123F54[];
extern StringRect MAIN_D_80124334[];
extern IconRect MAIN_D_8012437C[];
extern RECT MAIN_D_801242CC[];
int32_t drawDigimonStatsStrings(void);
void renderDigimonStatsBar(int32_t a, int32_t b, int32_t c, int16_t d, int16_t e);
void renderDigimonStatusConditions(int32_t condition);
extern uint16_t PLAYTIME_FRAMES;

void renderDigimonStatsView(void)
{
	RECT *r;
	StringRect *sr;
	StatsIconClutTable cluts;
	IconRect *icon;
	int32_t i;
	int32_t j;
	int16_t w;
	int16_t k;
	int16_t special;
	int16_t clut;
	uint8_t frame;

	cluts = MAIN_D_80123DB8;
	switch (MENU_STATE) {
	case 0:
		if (drawDigimonStatsStrings() == 1) {
			MENU_STATE = 1;
		}
		break;
	case 1:
		renderSeparatorLines(MAIN_D_80123F54, 6, 5);
		for (j = 0; j < 9; j++) {
			sr = &MAIN_D_80124334[j];
			renderString(3, sr->posX, sr->posY + 1, sr->uvWidth, 0xc, sr->uvX, sr->uvY, 5, 1);
		}
		for (j = 0; j < 0x15; j++) {
			icon = &MAIN_D_8012437C[j];
			renderRectPolyFT4(icon->posX, icon->posY, icon->width, icon->height, icon->texX,
					  icon->texY + 0x80, 5, GetClut(0x60, cluts.v[j] + 0x1e8), 5, 0);
		}
		renderString(0, -0x6d, -0x42, 0x48, 0xc, 0, 0x3c, 5, 0);
		w = strlen(DIGIMON_DATA[ENTITY_TABLE[1]->type].name);
#if defined(VERSION_JP)
		renderString(0, -0x6d, -0x33, (w / 2) * 12, 0xc, 0x9c, 0x24, 5, 0);
#else
		w = w * 10;
		if (w >= 0x79) {
			w = 0x78;
		}
		renderString(0, -0x6c, -0x32, w, 0xc, 0, 0x48, 5, 0);
#endif
		renderNumber(0, -0x6d, -0x22, 2, PARTNER_PARA.age, 5);
#if defined(VERSION_JP)
		renderNumber(0, -0x26, -0x22, 2, PARTNER_PARA.weight, 5);
		renderNumber(0, 0x23, 3, 4, PARTNER_ENTITY.digimonEntity.stats.current.currentHP, 5);
		renderString(0, 0x53, 4, 0xc, 0xc, 0x3c, 0x30, 5, 0);
		renderNumber(0, 0x5f, 3, 4, PARTNER_ENTITY.digimonEntity.stats.base.hp, 5);
		renderNumber(0, 0x23, 0x12, 4, PARTNER_ENTITY.digimonEntity.stats.current.currentMP, 5);
		renderString(0, 0x53, 0x13, 0xc, 0xc, 0x3c, 0x30, 5, 0);
		renderNumber(0, 0x5f, 0x12, 4, PARTNER_ENTITY.digimonEntity.stats.base.mp, 5);
		renderNumber(0, 0x23, 0x20, 4, PARTNER_ENTITY.digimonEntity.stats.base.off, 5);
		renderNumber(0, 0x23, 0x30, 4, PARTNER_ENTITY.digimonEntity.stats.base.def, 5);
		renderNumber(0, 0x23, 0x3f, 4, PARTNER_ENTITY.digimonEntity.stats.base.speed, 5);
		renderNumber(0, 0x23, 0x4e, 4, PARTNER_ENTITY.digimonEntity.stats.base.brain, 5);
#else
		renderNumber(0, -0x23, -0x22, 2, PARTNER_PARA.weight, 5);
		renderNumber(0, 0x23, 1, 4, PARTNER_ENTITY.digimonEntity.stats.current.currentHP, 5);
		renderNumber(0, 0x5f, 1, 4, PARTNER_ENTITY.digimonEntity.stats.base.hp, 5);
		renderNumber(0, 0x23, 0x10, 4, PARTNER_ENTITY.digimonEntity.stats.current.currentMP, 5);
		renderNumber(0, 0x5f, 0x10, 4, PARTNER_ENTITY.digimonEntity.stats.base.mp, 5);
		renderNumber(0, 0x23, 0x1f, 4, PARTNER_ENTITY.digimonEntity.stats.base.off, 5);
		renderNumber(0, 0x23, 0x2e, 4, PARTNER_ENTITY.digimonEntity.stats.base.def, 5);
		renderNumber(0, 0x23, 0x3d, 4, PARTNER_ENTITY.digimonEntity.stats.base.speed, 5);
		renderNumber(0, 0x23, 0x4c, 4, PARTNER_ENTITY.digimonEntity.stats.base.brain, 5);
#endif
		renderDigimonStatsBar(PARTNER_ENTITY.digimonEntity.stats.base.hp, 0x270f, 0x64, 0x24, 0xc);
		renderDigimonStatsBar(PARTNER_ENTITY.digimonEntity.stats.base.mp, 0x270f, 0x64, 0x24, 0x1b);
		renderDigimonStatsBar(PARTNER_ENTITY.digimonEntity.stats.base.off, 0x3e7, 0x32, 0x24, 0x2a);
		renderDigimonStatsBar(PARTNER_ENTITY.digimonEntity.stats.base.def, 0x3e7, 0x32, 0x24, 0x39);
		renderDigimonStatsBar(PARTNER_ENTITY.digimonEntity.stats.base.speed, 0x3e7, 0x32, 0x24, 0x48);
		renderDigimonStatsBar(PARTNER_ENTITY.digimonEntity.stats.base.brain, 0x3e7, 0x32, 0x24, 0x57);
		for (i = 0, k = 0; i < 3; i++) {
			special = DIGIMON_DATA[ENTITY_TABLE[1]->type].special[i];
			if (special != 0xff) {
				clut = 0x7a06;
				if ((special == 2) || (special == 3) || (special == 4)) {
					clut = 0x7a46;
				} else if (special == 5) {
					clut = 0x7a86;
				}
				if (PLAYTIME_FRAMES % 10 < 5) {
					frame = 0;
				} else {
					frame = 0xc;
				}
				renderRectPolyFT4(k + 9, -0x33, 0xc, 0xc, frame + (special * 0x18 + 0x24), 0x80, 5, clut, 5, 0);
				k += 0xc;
			}
		}
		if (DIGIMON_DATA[ENTITY_TABLE[1]->type].type != 0) {
			renderRectPolyFT4(0x3b, -0x33, 0xc, 0xc, (DIGIMON_DATA[ENTITY_TABLE[1]->type].type - 1) * 0xc, 0x80, 5, 0x7a06, 5, 0);
		}
		if (RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].sleepCycle == 5) {
			renderString(0, 0x55, -0x33, 0x30, 0xc, 0xcc, 0x30, 5, 0);
		}
		if (RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].sleepCycle < 5) {
			renderString(0, 0x55, -0x33, 0x30, 0xc, RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].sleepCycle * 0x30, 0x18, 5, 0);
		}
		for (i = 0; i < 3; i++) {
			if (i < PARTNER_ENTITY.lives) {
				renderRectPolyFT4(i * 0xe - 0x54, 0x22, 0xc, 0xb, 0xe8, 0x8c, 5, 0x7b86, 5, 0);
			}
			renderRectPolyFT4(i * 0xe - 0x54, 0x22, 0xc, 0xb, 0xf4, 0x8c, 5, 0x7b86, 5, 0);
		}
		renderDigimonStatusConditions(PARTNER_PARA.condition);
		if (PARTNER_PARA.happiness >= 0) {
			renderRectPolyFT4(-0x8a, 0x31, 0xb, 0xb, 0, 0xf4, 5, GetClut(0x60, 0x1f2), 5, 0);
		} else {
			renderRectPolyFT4(-0x8a, 0x31, 0xb, 0xb, 0xb, 0xf4, 5, GetClut(0x60, 0x1f2), 5, 0);
		}
		if (PARTNER_PARA.happiness >= 0) {
			renderRectPolyFT4(-0x54, 0x35, (uint8_t)(0x32 - (0x32 - PARTNER_PARA.happiness / 2)), 6, 0x18, 0xf0, 0x18, GetClut(0x70, 0x1f6), 5, 0);
			renderRectPolyFT4(-0x54, 0x35, 0x32, 6, 0x4a, 0xf0, 0x18, GetClut(0x70, 0x1f5), 5, 0);
		} else {
			renderRectPolyFT4(-0x54, 0x35, (uint8_t)(0x32 - (0x32 - (PARTNER_PARA.happiness + 0x64) / 2)), 6, 0x4a, 0xf0, 0x18, GetClut(0x70, 0x1f5), 5, 0);
		}
		if (PARTNER_PARA.discipline >= 0x32) {
			renderRectPolyFT4(-0x8a, 0x40, 0xb, 0xb, 0x16, 0xf4, 5, GetClut(0x60, 0x1f2), 5, 0);
		} else {
			renderRectPolyFT4(-0x8a, 0x40, 0xb, 0xb, 0x21, 0xf4, 5, GetClut(0x60, 0x1f2), 5, 0);
		}
		if (PARTNER_PARA.discipline >= 0x32) {
			renderRectPolyFT4(-0x54, 0x44, (uint8_t)(PARTNER_PARA.discipline - 0x32), 6, 0x18, 0xf0, 0x18, GetClut(0x70, 0x1f6), 5, 0);
			renderRectPolyFT4(-0x54, 0x44, 0x32, 6, 0x4a, 0xf0, 0x18, GetClut(0x70, 0x1f5), 5, 0);
		} else {
			renderRectPolyFT4(-0x54, 0x44, (uint8_t)PARTNER_PARA.discipline, 6, 0x4a, 0xf0, 0x18, GetClut(0x70, 0x1f5), 5, 0);
		}
		renderBox(-0x54, 0x52, PARTNER_PARA.virusBar * 3, 6, 0xc8, 0xc8, 0x3c, 0, 5);
		for (j = 0; j < 0xd; j++) {
			r = &MAIN_D_801242CC[j];
			renderInsetBox(r->x, r->y, r->w, r->h, 5);
		}
		renderDigiviceEntity(ENTITY_TABLE[1], 1);
		break;
	}
}

void renderDigimonMovesView(void)
{
	extern uint8_t MAIN_D_80134237;
	Move *mv;
	StringRect *sr;
	int32_t row;
	IconRect *icon;
	int32_t i;
	int32_t j;
	if (MENU_STATE > 0 && MENU_STATE < 7) {
		for (i = 0; i < 0xd; i++) {
			icon = &MAIN_D_801241CC[i];
			renderRectPolyFT4(icon->posX,
					  icon->posY,
					  icon->width,
					  icon->height,
					  icon->texX,
					  icon->texY + 0x80, 5, 0x7b06, 5, 0);
		}
		renderString(3, -0x8e, -0x57, 0x3c, 0xc, 0, 0x48, 5, 1);
		j = 0;
		while (j < 3) {
			if (EQUIPPED_MOVES[j] != 0xff) {
				mv = &MOVE_DATA[EQUIPPED_MOVES[j]];
				renderString(0, -0x7c, j * 0xf - 0x42, 0x78, 0xc, 0, j * 0xc + 0x18, 5, 1);
				renderNumber(0, 0x10, j * 0xf - 0x43, 3, mv->power, 5);
				renderNumber(0, 0x39, j * 0xf - 0x43, 3, mv->mpCost * 3, 5);
				if (mv->range != 0) {
#if defined(VERSION_JP)
					renderString(0, 0x65, j * 0xf - 0x42, 0xc, 0xc,
						     (mv->range - 1) * 0xc + 0x48, 0x6c, 5, 1);
#else
					renderString(0, 0x65, j * 0xf - 0x42, 0xc, 0xc,
						     (mv->range - 1) * 0x24, 0x78, 5, 1);
#endif
				}
				if (mv->status != 0) {
					renderRectPolyFT4(0x7d, j * 0xf - 0x43, 0xc, 0xc,
							  (mv->status - 1) * 0xc, 0x8c, 5, 0x7a06, 5, 0);
				}
			}
			j++;
		}
		renderSeparatorLines(TECH_VIEW_LINES1, 3, 3);
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
			renderRectPolyFT4(icon->posX, -9,
					  icon->width,
					  icon->height,
					  icon->texX,
					  icon->texY + 0x80, 5, 0x7b06, 5, 0);
		}
		renderRectPolyFT4(0x75, 4, 4, 4, 0x78, 0x8c, 5, 0x7b06, 5, 0);
		renderSeparatorLines(&TECH_VIEW_LINES1[15], 3, 5);
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
#if defined(VERSION_JP)
		renderString(0xe, -0x82, 0x19, 0xc, 0xc, 0, 0x54, 5, 1);
		renderString(0, -0x76, 0x19, 0xa8, 0xc, 0xc, 0x54, 5, 1);
		renderString(0, -0x82, 0x27, 0x84, 0xc, 0, 0x60, 5, 1);
		renderString(0xf, 2, 0x27, 0xc, 0xc, 0x84, 0x60, 5, 1);
#else
		renderString(0xf, -0x82, 0x19, 0xc, 0xc, 0, 0x54, 5, 1);
		renderString(0, -0x76, 0x19, 0xa8, 0xc, 0xc, 0x54, 5, 1);
		renderString(0, -0x76, 0x27, 0x84, 0xc, 0xc, 0x60, 5, 1);
		renderString(7, -0x82, 0x27, 0xc, 0xc, 0, 0x60, 5, 1);
#endif
		renderString(0, 0xe, 0x27, 0x18, 0xc, 0x90, 0x60, 5, 1);
		break;
	case 2:
	case 4:
		UI_BOX_DATA[1].finalPos.h -= 0x27;
		if (UI_BOX_DATA[1].finalPos.h < 0x4a) {
			++MENU_STATE;
		}
		break;
	case 3:
	case 5:
		UI_BOX_DATA[1].finalPos.h += 0x27;
		if (0xbd < UI_BOX_DATA[1].finalPos.h) {
			MENU_STATE = (MENU_STATE == 3) ? 6 : 1;
		}
		break;
	case 6:
		renderString(3, -0x8e, -0xf, 0x3c, 0xc, 0x60, 0x48, 5, 1);
		renderSeparatorLines(TECH_VIEW_LINES3, 0xc, 5);
		for (i = 0; i < 4; i++) {
			icon = &MAIN_D_80124044[i];
			renderRectPolyFT4(icon->posX,
					  icon->posY,
					  icon->width,
					  icon->height,
					  icon->texX,
					  icon->texY + 0x80, 5, 0x7b06, 5, 0);
		}
		renderString(0, -0x87, 0x20, 0x30, 0xc, 0x3c, 0, 5, 1);
		renderString(0, -0x81, 0x2e, 0x24, 0xc, 0, 0x6c, 5, 1);
#if defined(VERSION_JP)
		renderString(0x10, -0x80, 0x4c, 0xc, 0xc, 0x24, 0x6c, 5, 1);
		renderString(0, -0x74, 0x4c, 0x18, 0xc, 0x30, 0x6c, 5, 1);
#else
		renderString(0xc, -0x74, 0x4c, 0x18, 0xc, 0x30, 0x6c, 5, 1);
#endif
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
		for (row = 0; row < 7; row++) {
			for (j = 0; j < 8; j++) {
				renderBox(j * 0x12 - 0x2a, row * 0xf - 7, 0xc, 0xc,
					  0x4e, 0x60, 0x6e, 0x80, 5);
			}
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
		drawLine2P(0x20202, -0x92, -4, 0x92, -4, 5, 0);
		drawLine2P(0xfad990, -0x93, -3, 0x93, -3, 5, 0);
		drawLine2P(0x20202, -0x92, -2, 0x92, -2, 5, 0);
#if defined(VERSION_JP)
		renderString(3, -0x8e, -0x52, 0x54, 0xc, 0, 0x30, 5, 1);
#else
		renderString(3, -0x8e, -0x52, 0xa0, 0xc, 0, 0x30, 5, 1);
#endif
		renderString(0xe, -0x72, -0x32, 0xc, 0xc, 0xcc, 0x30, 5, 1);
		renderString(0xe, 6, -0x21, 0xc, 0xc, 0xcc, 0x30, 5, 1);
		renderString(0xe, -0x2a, -0x13, 0xc, 0xc, 0xcc, 0x30, 5, 1);
		for (i = 0; i < 0xc; i++) {
			sr = &MAIN_D_80124234[i];
			renderString(0, sr->posX,
				     sr->posY + 1,
				     sr->uvWidth, 0xc,
				     sr->uvX,
				     sr->uvY, 5, 1);
		}
		renderString(0, 0x42, 0x33, 0x3c, 0xc, 0x78, 0x48, 5, 1);
		renderString(0, 0x42, 0x43, 0x3c, 0xc, 0x84, 0x54, 5, 1);
		renderRectPolyFT4(-0x81, -0x41, 0xc, 0xc, 0xb4, 0x8c, 5, 0x7b06, 5, 0);
		renderRectPolyFT4(-0x81, -0x22, 0xc, 0xc, 0xb4, 0x8c, 5, 0x7b06, 5, 0);
		renderRectPolyFT4(-0x7b, 0x50, 0x12, 0x10, 0xc0, 0x8c, 5, 0x7b06, 5, 0);
		for (i = 0; i < 7; i++) {
			icon = &MAIN_D_80124294[i];
#if !defined(VERSION_JP)
			if (i != 2) {
#endif
				renderRectPolyFT4(icon->posX, icon->posY, icon->width, icon->height,
						  icon->texX, icon->texY + 0x80, 5,
						  (i == 5) ? getClut(96, 489) : getClut(96, 488), 5, 0x80);
#if !defined(VERSION_JP)
			}
#endif
		}
#if defined(VERSION_JP)
		renderBox(-0x18, -0x22, 0xc, 0xc, 200, 0, 0x28, 0x81, 4);
#else
		renderBox(0x52, -0x22, 0xc, 0xc, 200, 0, 0x28, 0x81, 4);
#endif
		renderBox(-0x78, 2, 0xc, 0xc, 200, 0, 0x28, 0x81, 4);
		renderBox(-0x78, 0x22, 0xc, 0xc, 0x4e, 0x60, 0x6e, 0x80, 4);
		renderBox(-0x78, 0x32, 0xc, 0xc, 0x68, 0x68, 0x68, 0x83, 4);
		renderBox(-0x78, 0x42, 0xc, 0xc, 0x68, 0x68, 0x68, 0x83, 4);
		break;
	case 9:
		if (drawDigimonMovesText() == 1) {
			MENU_STATE = 6;
		}
	}
}

void renderMenuTab(int16_t x, int8_t w, int8_t layer)
{
	int32_t i;
	int8_t h;

	h = 0x10;
	if (layer == 1) {
		h--;
	}
	renderRectPolyFT4(x, -0x68, 7, h, 0xd4, 0x8c, 5, GetClut(0x60, 0x1ec), 5, layer);
	for (i = 0; i < (w - 0xe) / 4; i++) {
		renderRectPolyFT4(x + 7 + i * 4, -0x68, 4, h, 0xe2, 0x8c, 5, GetClut(0x60, 0x1ec), 5, layer);
	}
	renderRectPolyFT4(x + w - 9, -0x68, 7, h, 0xdb, 0x8c, 5, GetClut(0x60, 0x1ec), 5, layer);
}

void renderPlayerInfoView(void)
{
	int32_t i;
	StringRect *e;
	RECT *r;
	int32_t j;
	int32_t k;
	int32_t n;
	int8_t count;

	switch (MENU_STATE) {
	case 0:
		if (drawPlayerInfoStrings() == 1) {
			MENU_STATE = 1;
		}
		break;
	case 1:
		renderSeparatorLines(MAIN_D_80124424, 0xb, 5);

		for (i = 0; i < 0xb; i++) {
			e = &MAIN_D_801244EC[i];
			renderString(3 - ((i / 7) * 3), e->posX, e->posY + 1, e->uvWidth, 0xc, e->uvX,
				     e->uvY, 5, 1);
		}

		renderString(0, -0x54, -0x50, 0x48, 0xc, 0, 0x30, 5, 1);
		renderNumber(0, 0x35, -0x36, 2, TAMER_ENTITY.tamerLevel, 5);
#if defined(VERSION_JP)
		renderString(0, 0x28, -0x25, 0x30, 0xc, 0, (TAMER_ENTITY.tamerLevel * 12) + 0x40, 5, 1);
#else
		renderString(0, 0x28, -0x24, 0x64, 0xc, 0, (TAMER_ENTITY.tamerLevel * 12) + 0x40, 5, 1);
#endif
		n = strlen(TAMER_LEVEL_TITLES[TAMER_ENTITY.tamerLevel]) / 2;
		renderString(0, (n * 12) + 0x28, -0x25, 0x30, 0xc, 0xb4, 0x18, 5, 1);
		renderNumber(0, 0x35, -0x11, 2, TAMER_ENTITY.raisedCount, 5);
		renderNumber(0, 0x29, 2, 6, MONEY, 5);
#if defined(VERSION_JP)
		renderNumber(0, 0x29, 0x16, 3, PLAYTIME_HOURS, 5);
		renderNumber(0, 0x59, 0x16, 2, PLAYTIME_MINUTES, 5);
#else
		renderString(0, 0x32, 0x16, 0x4c, 0xc, 0, 0xe4, 5, 1);
#endif

		for (i = 0; i < 2; i++) {
			if (isTriggerSet(i + 0x2d) == 1) {
				renderRectPolyFT4(i * 0x15 + 0x34, 0x37, 0x10, 0x10, i * 0x10 + 0xb0, 0xa0, 5, 0x7bc6, 5, 0);
			}
		}

		if (isTriggerSet(0x2f) == 1) {
			renderRectPolyFT4(0x5e, 0x37, 0x10, 0x10, 0xe0, 0xa0, 5, 0x7bc6, 5, 0);
		}

		if (isTriggerSet(0x30) == 1) {
			renderRectPolyFT4(0x73, 0x37, 0x10, 0x10, 0xd0, 0xa0, 5, 0x7bc6, 5, 0);
		}

		count = 0;
		for (j = 0; j < 0x12; j++) {
			if (hasMedal(j) != 0) {
				count++;
			}
		}

		renderNumber(0, 0x35, 0x4d, 2, count, 5);
		renderDigiviceEntity(ENTITY_TABLE[0], 0);

		for (k = 0; k < 0xb; k++) {
			r = &MAIN_D_80124494[k];
			renderInsetBox(r->x, r->y, r->w, r->h, 5);
		}
		break;
	}
}
