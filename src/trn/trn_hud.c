#include <libgpu.h>
#include <libgs.h>

#include <dw/entity.h>
#include <dw/font.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/training.h>
#include <dw/trn.h>
#include <dw/types.h>
#include <dw/ui.h>

extern int16_t INITIAL_COMBAT_STATS[][6];
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern int16_t STATS_GAINS[6];
extern GsOT *ACTIVE_ORDERING_TABLE;
extern char MAIN_D_80124C0C[];
extern char MAIN_D_80124C54[];

void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void renderLinePrimitive(uint32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t order, uint32_t mode);
void renderNumber(int32_t a, int32_t x, int32_t y, int32_t digits, int32_t value, int32_t layer);
void renderTrianglePrimitive(int32_t color, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t otz, int32_t flag);
void setEntityTextDigit(POLY_FT4 *poly, int32_t x, int32_t y);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t x, int32_t y, int32_t w, int32_t h);
void setUVDataPolyFT4(POLY_FT4 *p, int32_t u, int32_t v, int32_t w, int32_t h);
void worldPosToScreenPos(TrainingSpot *item, SVECTOR *out);
void TRN_tickPostTrainingStatsBox();
void TRN_renderPostTrainingStatsBox(uint8_t depth);
int32_t TRN_getTrainingSpotScreenPos(int32_t key, int16_t sub, SVECTOR *out);

static void *trn_hud_functions[] = {
	TRN_getTrainingSpotScreenPos,
	TRN_renderSlotSession,
	TRN_tickSlotSession,
	TRN_closeUIBox,
	TRN_renderPostTrainingStatsBox,
	TRN_tickPostTrainingStatsBox,
	TRN_createPostTrainingStatsBox,
	TRN_applyBaseStats,
	TRN_saveBaseStats,
};

/* "所持金" (money held) */
char MAIN_D_80134BC0[] = "\x8f\x8a\x8e\x9d\x8b\xe0";

int32_t MAIN_D_80135394;
int8_t MAIN_D_80135398[6];
int16_t MAIN_D_8013539E;
int16_t MAIN_D_801353A0[4];

static void *trn_hud_sbss_order[] = {
	&MAIN_D_801353A0,
	&MAIN_D_8013539E,
	&MAIN_D_80135398,
	&MAIN_D_80135394,
};

// clang-format off
int8_t TRN_D_8008F1AC[3][13] = {
	{ 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x1, 0x3, 0x5, 0x2, 0x4, 0x6 },
	{ 0x6, 0x4, 0x2, 0x5, 0x3, 0x1, 0x2, 0x5, 0x3, 0x6, 0x1, 0x4, 0x7 },
	{ 0x7, 0x6, 0x5, 0x4, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1, 0x3, 0x2, 0x1 },
};

TrainingSpot TRN_D_8008F1D4[7] = {
	{ 0x3f2, 0x0, -0x3cf, 0x0 },
	{ -0x1fa, 0x0, -0x791, 0x2 },
	{ -0x315, 0x0, -0x36b, 0x3 },
	{ 0x598, 0x0, 0x4fc, 0x1 },
	{ 0xc, 0x0, 0x1bd, 0x4 },
	{ -0x6f3, 0x0, 0x824, 0x5 },
	{ 0x0, 0x0, 0x0, -0x1 },
};

TrainingSpot TRN_D_8008F20C[3] = {
	{ -0x33d, 0x0, 0x1003, 0x2 },
	{ 0x2fe, 0x0, 0x6a, 0x4 },
	{ 0x0, 0x0, 0x0, -0x1 },
};

TrainingSpot TRN_D_8008F224[2] = {
	{ 0x59a, 0x0, -0x11b, 0x1 },
	{ 0x0, 0x0, 0x0, -0x1 },
};

TrainingSpot TRN_D_8008F234[3] = {
	{ -0x52b, 0x0, 0x540, 0x2 },
	{ -0x228, 0x0, 0x6be, 0x0 },
	{ 0x0, 0x0, 0x0, -0x1 },
};

TrainingSpot TRN_D_8008F24C[3] = {
	{ 0x2ee, 0x0, 0x7f8, 0x4 },
	{ -0xcc, 0x0, 0x962, 0x3 },
	{ 0x0, 0x0, 0x0, -0x1 },
};

TrainingSpot TRN_D_8008F264[3] = {
	{ 0x5e9, 0x0, 0x5b, 0x2 },
	{ 0x0, 0x0, 0x3e8, 0x0 },
	{ 0x0, 0x0, 0x0, -0x1 },
};

TrainingSpot TRN_D_8008F27C[2] = {
	{ -0x7e1, 0x0, 0x562, 0x5 },
	{ 0x0, 0x0, 0x0, -0x1 },
};

MapTrainingSpots TRN_TRAINING_SPOTS[8] = {
	{ 0x70, TRN_D_8008F1D4 },
	{ 0x4e, TRN_D_8008F20C },
	{ 0xa5, TRN_D_8008F224 },
	{ 0x6b, TRN_D_8008F234 },
	{ 0x6c, TRN_D_8008F24C },
	{ 0x63, TRN_D_8008F264 },
	{ 0x77, TRN_D_8008F27C },
	{ -0x1, NULL },
};
// clang-format on

void TRN_saveBaseStats(void)
{
	INITIAL_COMBAT_STATS[0][0] = PARTNER_ENTITY.digimonEntity.stats.base.hp;
	INITIAL_COMBAT_STATS[0][1] = PARTNER_ENTITY.digimonEntity.stats.base.mp;
	INITIAL_COMBAT_STATS[0][2] = PARTNER_ENTITY.digimonEntity.stats.base.off;
	INITIAL_COMBAT_STATS[0][3] = PARTNER_ENTITY.digimonEntity.stats.base.def;
	INITIAL_COMBAT_STATS[0][4] = PARTNER_ENTITY.digimonEntity.stats.base.speed;
	INITIAL_COMBAT_STATS[0][5] = PARTNER_ENTITY.digimonEntity.stats.base.brain;
}

void TRN_applyBaseStats(void)
{
	PARTNER_ENTITY.digimonEntity.stats.base.hp = INITIAL_COMBAT_STATS[0][0];
	PARTNER_ENTITY.digimonEntity.stats.base.mp = INITIAL_COMBAT_STATS[0][1];
	PARTNER_ENTITY.digimonEntity.stats.base.off = INITIAL_COMBAT_STATS[0][2];
	PARTNER_ENTITY.digimonEntity.stats.base.def = INITIAL_COMBAT_STATS[0][3];
	PARTNER_ENTITY.digimonEntity.stats.base.speed = INITIAL_COMBAT_STATS[0][4];
	PARTNER_ENTITY.digimonEntity.stats.base.brain = INITIAL_COMBAT_STATS[0][5];
}

void TRN_createPostTrainingStatsBox(void)
{
	int32_t i;
	int32_t row;
	int32_t y;
	int16_t screenPos[2];
	RECT finalPos;
	RECT startPos;

	MAIN_D_80135394 = 0;
	clearTextArea();

	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] == 0) {
			MAIN_D_80135398[i] = 0;
		} else {
			MAIN_D_80135398[i] = 1;
		}
	}

	for (i = 0; i < 4; i++) {
		y = i * 12;
		row = i * 2;

		if (i < 3) {
			drawString(&MAIN_D_80124C0C[row * 12], 0, y * 2);
			drawString(&MAIN_D_80124C0C[(row + 1) * 12], 0, (row + 1) * 12);
		}
		if (i == 3) {
			drawString(MAIN_D_80134BC0, 0, 84);
			drawString(MAIN_D_80124C54, 0, 240);
		}
		DrawSync(0);
	}

	MAIN_D_8013539E = 100;

	setRECT(&finalPos, -94, -78, 188, 96);

	getEntityScreenPos(ENTITY_TABLE[1], 1, screenPos);

	setRECT(&startPos, screenPos[0] - 5, screenPos[1] - 5, 10, 10);
	createAnimatedUIBox(1, 0, 2, &finalPos, &startPos, TRN_tickPostTrainingStatsBox, (RenderFunction)TRN_renderPostTrainingStatsBox);
}

void TRN_tickPostTrainingStatsBox(void)
{
	int32_t i;
	int16_t gain;

	if (MAIN_D_8013539E > 0) {
		MAIN_D_8013539E--;
	}

	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] != 0) {
			break;
		}
	}

	if ((POLLED_INPUT == 0x40) || (POLLED_INPUT == 0x10)) {
		if (!(POLLED_INPUT_PREVIOUS & 0x40) && !(POLLED_INPUT_PREVIOUS & 0x10)) {
			MAIN_D_80135394 = 1;
		}
	}

	if (MAIN_D_80135394 != 1) {
		return;
	}

	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] != 0) {
			gain = STATS_GAINS[i];
			INITIAL_COMBAT_STATS[0][i] += gain;

			if (gain > 0) {
				if (i < 2) {
					if (INITIAL_COMBAT_STATS[0][i] >= 10000) {
						INITIAL_COMBAT_STATS[0][i] = 9999;
					}
				} else {
					if (INITIAL_COMBAT_STATS[0][i] >= 1000) {
						INITIAL_COMBAT_STATS[0][i] = 999;
					}
				}
			}
			STATS_GAINS[i] = 0;
		}
	}

	MAIN_D_8013539E = 0;
}

void TRN_renderPostTrainingStatsBox(uint8_t depth)
{
	RECT *box = &UI_BOX_DATA[1].finalPos;
	GsBOXF rect;
	int32_t i;
	int32_t first;
	int16_t y;
	int32_t hasStatGain;

	first = 1;
	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] == 0) {
			continue;
		}
		if ((MAIN_D_8013539E == 0) && (STATS_GAINS[i] > 0) && (first == 1)) {
			playSound(0, 0x16);
			STATS_GAINS[i] = STATS_GAINS[i] - 1;
			INITIAL_COMBAT_STATS[0][i] += 1;

			if (i < 2) {
				if (INITIAL_COMBAT_STATS[0][i] >= 10000) {
					INITIAL_COMBAT_STATS[0][i] = 9999;
				}
			} else {
				if (INITIAL_COMBAT_STATS[0][i] >= 1000) {
					INITIAL_COMBAT_STATS[0][i] = 999;
				}
			}
			first = 0;
		}
		if (STATS_GAINS[i] != 0) {
			POLY_FT4 *prim = (POLY_FT4 *)GsGetWorkBase();
			setEntityTextDigit(prim, 256, 491);
			setRGB0(prim, 0x80, 0x80, 0x80);
			setUVDataPolyFT4(prim, 96, 180, 12, 12);
			setPosDataPolyFT4(prim, box->x + 130, (box->y + 9) + (i * 13), 12, 12);
			AddPrim((ACTIVE_ORDERING_TABLE->org + 6) - depth, prim++);
			GsSetWorkBase((PACKET *)prim);
		}
		renderNumber(5, box->x + 142, (box->y + 9) + (i * 13), 3, STATS_GAINS[i], 6 - depth);
	}

	for (i = 0; i < 6; i++) {
		renderNumber(0, box->x + 68, (box->y + 9) + (i * 13), 4, INITIAL_COMBAT_STATS[0][i], 6 - depth);
	}

	renderLinePrimitive(0xfad990, box->x + 122, box->y + 2, box->x + 122, (box->y + box->h) - 3, 6 - depth, 0);
	renderLinePrimitive(0x20202, box->x + 123, box->y + 2, box->x + 123, (box->y + box->h) - 3, 6 - depth, 0);

	for (i = 0; i < 6; i++) {
		renderString(4, box->x + 10, (box->y + 9) + (i * 13), 48, 12, 0, i * 12, 6 - depth, 0);
	}

	rect.attribute = 0;
	rect.x = box->x + 66;
	rect.h = 2;

	for (i = 0; i < 6; i++) {
		y = (box->y + 20) + (i * 13);
		rect.y = y - 2;

		if (i < 2) {
			rect.w = (INITIAL_COMBAT_STATS[0][i] * 50) / 9999;
		} else {
			rect.w = (INITIAL_COMBAT_STATS[0][i] * 50) / 999;
		}
		hasStatGain = MAIN_D_80135398[i];
		if (hasStatGain != 1) {
			if (hasStatGain == 0) {
				rect.r = rect.g = rect.b = 0x78;
				GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE, (uint16_t)(6 - depth));
				rect.r = rect.g = rect.b = 0x28;
			}
		} else {
			rect.r = 0x69;
			rect.g = 0xc2;
			rect.b = 0xff;
			GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE, (uint16_t)(6 - depth));
			rect.r = 0;
			rect.g = 0x5a;
			rect.b = 0x96;
		}
		rect.w = 50;

		renderTrianglePrimitive(0x20202, box->x + 64, y, box->x + 64, y - 3, box->x + 117, y - 3, 6 - depth, 0);
		renderTrianglePrimitive(0x666666, box->x + 117, y - 2, box->x + 117, y, box->x + 65, y, 6 - depth, 0);
		GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE, (uint16_t)(6 - depth));
	}
}

void TRN_closeUIBox(int32_t id)
{
	if (UI_BOX_DATA[id].state != 0) {
		removeAnimatedUIBox(id, NULL);
	}
}

void TRN_tickSlotSession(void)
{
	int16_t *p = MAIN_D_801353A0;

	switch (p[1]) {
	case 0:
		TRN_createSlotMachineBox(p[2]);
		p[1] = 1;
		break;
	case 1:
		p[3] = TRN_getSlotMachineResult();
		if (p[3] != -1) {
			p[1] = 2;
		}
		break;
	case 2:
		if (UI_BOX_DATA[3].state == 0) {
			p[0] = p[3];
			removeObject(0xfdc, 0);
		}
		break;
	}
}

void TRN_renderSlotSession(void)
{
}

int32_t TRN_getTrainingSpotScreenPos(int32_t key, int16_t sub, SVECTOR *out)
{
	int32_t i;
	int32_t id;
	TrainingSpot *p;

	i = 0;
	for (;;) {
		id = TRN_TRAINING_SPOTS[i].mapId;
		if (id == key) {
			break;
		}
		if (id == -1) {
			return -1;
		}
		i++;
	}

	p = TRN_TRAINING_SPOTS[i].spots;
	for (;;) {
		if (p->stat == sub) {
			break;
		}
		if (p->stat == -1) {
			return -1;
		}
		p++;
	}

	worldPosToScreenPos(p, out);
	return 0;
}
