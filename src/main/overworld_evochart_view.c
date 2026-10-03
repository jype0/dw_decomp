#include <dw/clock.h>

typedef struct {
	int16_t posX;
	int16_t posY;
	uint8_t u;
	uint8_t v;
	uint8_t clut;
	uint8_t pad;
} EvoChartEntry;

typedef struct {
	int16_t m[5];
} EvoClutTable;

extern int8_t MENU_STATE;
extern EvoChartEntry MAIN_D_80124544[];
extern EvoClutTable MAIN_D_80123E1C;
extern int16_t MAIN_D_80134D40;
extern int16_t MAIN_D_80134D42;
extern int16_t MAIN_D_80134D44;

int32_t drawEvoChartStrings(int32_t arg);
void renderRectPolyFT4(int16_t posX, int16_t posY, int32_t width,
		       int32_t height, uint8_t texX, uint8_t texY,
		       int16_t texturePage, int16_t clut, int32_t zIndex,
		       int8_t flag);
void renderBorderBox(int16_t x, int16_t y, int16_t w, int16_t h,
		     int32_t c1, int32_t c2, uint8_t r, uint8_t g, uint8_t b,
		     int32_t a10);
void renderString();
int32_t hasDigimonRaised(int32_t id);
void renderEvoChartView(void);

void renderEvoChartView(void)
{
	int32_t i;
	int16_t x;
	int8_t shift;
	EvoClutTable cluts;

	cluts = MAIN_D_80123E1C;
	switch (MENU_STATE) {
	case 0:
		if (drawEvoChartStrings(0) == 1) {
			MENU_STATE = 1;
		}
		break;
	case 2:
	case 3:
	case 4:
		if (MAIN_D_80134D42 < 3) {
			x = (int16_t)(MAIN_D_80134D42 * 0x25 + 0x1c);
		} else if (MAIN_D_80134D42 < 7) {
			x = (int16_t)((MAIN_D_80134D42 - 3) * 0x18 + 0x8b);
		} else {
			x = (int16_t)((MAIN_D_80134D42 - 7) * 0x18 + 0xf8);
		}
		renderRectPolyFT4((int16_t)(x - 0xa2),
			(int16_t)(MAIN_D_80134D44 * 0x13 - 0x4e), 0x18, 0x14,
			0, 0xe8, 0x18, 0x7dc7, 5, 0);
		/* fall through */
	case 1:
		for (i = 0; i < 0x3e; i++) {
			if (hasDigimonRaised((i + 1) & 0xffff)) {
				shift = 0;
				if ((i + 1 == MAIN_D_80134D40) && (1 < MENU_STATE) &&
				    ((PLAYTIME_FRAMES % 10) < 5)) {
					shift = 0x10;
				}
				renderRectPolyFT4(
					(int16_t)(MAIN_D_80124544[i].posX - 0xa0),
					(int16_t)(MAIN_D_80124544[i].posY - 0x78),
					0x10, 0x10,
					(uint8_t)(shift + MAIN_D_80124544[i].u),
					MAIN_D_80124544[i].v, 0x18,
					cluts.m[MAIN_D_80124544[i].clut],
					5, 0);
			}
		}
		for (i = 0; i < 0x3e; i++) {
			renderBorderBox((MAIN_D_80124544[i].posX - 1),
				(MAIN_D_80124544[i].posY - 1), 0x12, 0x12,
				0xbebebe, 0x3c3c3c, 0x87, 0x87, 0x87, 5);
		}
		renderBorderBox(0x15, 0x27, 0x22, 0x9f, 0xaaa0c8, 0x5a3c8c,
				0xa5, 0x5a, 0x73, 5);
		renderBorderBox(0x3a, 0x27, 0x22, 0x9f, 0xa078a5, 0x46144b,
				0x6e, 0x2d, 0x5a, 5);
		renderBorderBox(0x5f, 0x27, 0x22, 0xb0, 0xc88250, 0x5f370a,
				0xf, 0x50, 0x82, 5);
		renderBorderBox(0x84, 0x27, 0x6a, 0x9f, 0x6eaf5a, 0x285a00,
				0, 0x87, 0x41, 5);
		renderBorderBox(0xf1, 0x27, 0x38, 0x9f, 0x6996d2, 0x1e4178,
				0xaf, 0x64, 0x2d, 5);
#if defined(VERSION_JP)
		renderString(0xe, -0x1a, 0x54, 0xc, 0xc, 0, 0xc, 5, 1);
#else
		renderString(0xf, -0x1a, 0x54, 0xc, 0xc, 0, 0xc, 5, 1);
#endif
		renderString(0, -0xe, 0x54, 0x3c, 0xc, 0xc, 0xc, 5, 1);
#if defined(VERSION_JP)
		renderString(0xf, 0x3a, 0x54, 0xc, 0xc, 0x48, 0xc, 5, 1);
#else
		renderString(7, 0x3a, 0x54, 0xc, 0xc, 0x48, 0xc, 5, 1);
#endif
		renderString(0, 0x46, 0x54, 0x3c, 0xc, 0x54, 0xc, 5, 1);
	}
}
