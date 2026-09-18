#include <stdlib.h>

#include <libgpu.h>
#include <libgs.h>

#include <dw/script.h>
#include <dw/sound.h>
#include <dw/training.h>
#include <dw/trn.h>
#include <dw/types.h>
#include <dw/ui.h>

extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern uint8_t CURRENT_SCREEN;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern int8_t TRN_D_8008F1AC[3][13];

int32_t TRN_getTrainingSpotScreenPos(int32_t key, int16_t sub, SVECTOR *out);
void TRN_tickSlotMachine(int32_t arg);
void TRN_renderSlotMachine(int32_t arg);
void TRN_chooseReelStop(int16_t i, SlotMachine *p);

static void *trn_slots_functions[] = {
	TRN_startSlotSpin,
	TRN_getSlotMachineResult,
	TRN_createSlotMachineBox,
	TRN_getSlotSessionResult,
	TRN_startSlotSession,
	TRN_chooseReelStop,
	TRN_renderSlotMachine,
	TRN_tickSlotMachine,
};

// clang-format off
int8_t TRN_D_8008F2CC[12] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
	0x01, 0x02, 0x00, 0x00,
};

GsSPRITE TRN_SLOT_SPRITE1 = {
	0x50000000,	/* attribute */
	0x0,		/* x */
	0x0,		/* y */
	0x20,		/* w */
	0x20,		/* h */
	0x2b,		/* tpage */
	0x0,		/* u */
	0xd8,		/* v */
	0x30,		/* cx */
	0x1e6,		/* cy */
	0x80,		/* r */
	0x80,		/* g */
	0x80,		/* b */
	0x10,		/* mx */
	0x0,		/* my */
	0x1000,		/* scalex */
	0x1000,		/* scaley */
	0x0,		/* rotate */
};

GsSPRITE TRN_SLOT_SPRITE2 = {
	0x60000000,	/* attribute */
	0x0,		/* x */
	-0x4a,		/* y */
	0x8,		/* w */
	0x20,		/* h */
	0x4b,		/* tpage */
	0xe0,		/* u */
	0xd8,		/* v */
	0x40,		/* cx */
	0x1e6,		/* cy */
	0x40,		/* r */
	0x40,		/* g */
	0x40,		/* b */
	0x4,		/* mx */
	0x0,		/* my */
	0x4000,		/* scalex */
	0x2000,		/* scaley */
	0x0,		/* rotate */
};
// clang-format on

static void trn_slots__garbage__(void)
{
	int32_t v0;
	int32_t v1;
	int32_t v2;
	int32_t v3;
	int32_t v4;
	int32_t v5;
	int32_t v6;
	int32_t v7;
	int32_t v8;
	int32_t v9;
	int32_t v10;
	int32_t v11;
	int32_t v12;
	int32_t v13;
	int32_t v14;
	int32_t v15;
	int32_t v16;
	int32_t v17;
	int32_t v18;
	int32_t v19;

	v0 = TRN_D_8008F368[0] + 0;
	v1 = TRN_D_8008F368[1] + 1;
	v2 = TRN_D_8008F368[2] + 2;
	v3 = TRN_D_8008F368[3] + 3;
	v4 = TRN_D_8008F368[4] + 4;
	v5 = TRN_D_8008F368[5] + 5;
	v6 = TRN_D_8008F368[6] + 6;
	v7 = TRN_D_8008F368[7] + 7;
	v8 = TRN_D_8008F368[0] + 8;
	v9 = TRN_D_8008F368[1] + 9;
	v10 = TRN_D_8008F368[2] + 10;
	v11 = TRN_D_8008F368[3] + 11;
	v12 = TRN_D_8008F368[4] + 12;
	v13 = TRN_D_8008F368[5] + 13;
	v14 = TRN_D_8008F368[6] + 14;
	v15 = TRN_D_8008F368[7] + 15;
	v16 = TRN_D_8008F368[0] + 16;
	v17 = TRN_D_8008F368[1] + 17;
	v18 = TRN_D_8008F368[2] + 18;
	v19 = TRN_D_8008F368[3] + 19;
	TRN_D_8008F368[0] = (v0 * v1) + v2;
	TRN_D_8008F368[1] = (v1 * v2) + v3;
	TRN_D_8008F368[2] = (v2 * v3) + v4;
	TRN_D_8008F368[3] = (v3 * v4) + v5;
	TRN_D_8008F368[4] = (v4 * v5) + v6;
	TRN_D_8008F368[5] = (v5 * v6) + v7;
	TRN_D_8008F368[6] = (v6 * v7) + v8;
	TRN_D_8008F368[7] = (v7 * v8) + v9;
	TRN_D_8008F368[0] = (v8 * v9) + v10;
	TRN_D_8008F368[1] = (v9 * v10) + v11;
	TRN_D_8008F368[2] = (v10 * v11) + v12;
	TRN_D_8008F368[3] = (v11 * v12) + v13;
	TRN_D_8008F368[4] = (v12 * v13) + v14;
	TRN_D_8008F368[5] = (v13 * v14) + v15;
	TRN_D_8008F368[6] = (v14 * v15) + v16;
	TRN_D_8008F368[7] = (v15 * v16) + v17;
	TRN_D_8008F368[0] = (v16 * v17) + v18;
	TRN_D_8008F368[1] = (v17 * v18) + v19;
	TRN_D_8008F368[2] = (v18 * v19) + v0;
	TRN_D_8008F368[3] = (v19 * v0) + v1;
}

void TRN_tickSlotMachine(int32_t arg)
{
	RECT rect;
	SlotMachine *p;
	int32_t i;
	int32_t t;
	int32_t k;

	rect = MAIN_D_80134BD8;
	p = &TRN_SLOT_MACHINE;
	switch (TRN_SLOT_MACHINE.state) {
	case 0:
		if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40) || (p->autoStart >= 0)) {
			p->state = 1;
			p->spinSpeed[0] = 0x400;
			p->spinSpeed[1] = 0x400;
			p->spinSpeed[2] = 0x400;
			p->stopSteps[0] = 0;
			p->stopSteps[1] = 0;
			p->stopSteps[2] = 0;
			playSound(8, 6);
		}
		break;
	case 1:
	case 2:
	case 3:
	case 4:
		for (i = 0; i < 3; i++) {
			p->settling[i] = 0;
			p->scrollY[i] += p->spinSpeed[i] >> 6;
			if (p->scrollY[i] == 0x20) {
				p->scrollY[i]--;
			}
			if (p->scrollY[i] >= 0x20) {
				p->scrollY[i] -= 0x20;
				p->reelPos[i] = (p->reelPos[i] + 12) % 13;
				if (i < (p->state - 1)) {
					if (p->stopSteps[i] > 0) {
						p->stopSteps[i]--;
					} else {
						p->scrollY[i] = 0;
						p->spinSpeed[i] = 0;
						playSound(8, 8);
					}
				}
			}
			k = p->state - 1;
			if ((k == i) && ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40)) {
				TRN_chooseReelStop(k, p);
			}
			t = p->scrollY[i] + (p->spinSpeed[i] >> 6);
			if (t == 0x20) {
				t--;
			}
			if (t >= 0x20) {
				if ((i < (p->state - 1)) && (p->stopSteps[i] <= 0)) {
					p->settling[i]++;
				} else if ((i == (p->state - 1)) && (p->stopSteps[i] <= 0) && ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40)) {
					p->settling[i]++;
				}
			}
		}
		if ((p->state < 4) && ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40)) {
			playSound(8, 7);
			p->state++;
		}
		if ((p->spinSpeed[0] == 0) && (p->spinSpeed[1] == 0) && (p->spinSpeed[2] == 0)) {
			p->resultTimer = 0;
			p->state++;
		}
		break;
	case 5:
		if (p->resultTimer == 0) {
			if ((TRN_D_8008F1AC[0][p->reelPos[0]] == TRN_D_8008F1AC[1][p->reelPos[1]]) && (TRN_D_8008F1AC[0][p->reelPos[0]] == TRN_D_8008F1AC[2][p->reelPos[2]])) {
				if (TRN_D_8008F1AC[0][p->reelPos[0]] == 7) {
					playSound(8, 0xc);
					p->payout = 100;
				} else if ((TRN_D_8008F1AC[0][p->reelPos[0]] - 1) == p->stat) {
					playSound(8, 0xb);
					p->payout = 0x28;
				} else {
					playSound(8, 0xb);
					p->payout = 0x14;
				}
			} else {
				playSound(8, 0xd);
				p->payout = 5;
			}
		} else if ((p->resultTimer >= 0x1e) || ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x40)) {
			p->state++;
		}
		p->resultTimer++;
		break;
	case 6:
		if (TRN_getTrainingSpotScreenPos(CURRENT_SCREEN, p->stat, (SVECTOR *)&rect) == 0) {
			rect.x -= (int16_t)(0xa8 - DRAWING_OFFSET_X);
			rect.y -= (int16_t)(0x7e - DRAWING_OFFSET_Y);
		}
		removeAnimatedUIBox(arg, &rect);
		closeBox(0);
		p->result = p->payout;
		break;
	}
}

void TRN_renderSlotMachine(int32_t arg)
{
	SlotMachine *p;
	int32_t i;
	int32_t k;
	int32_t y;
	int32_t c;
	int32_t depth;

	depth = 6 - arg;
	p = &TRN_SLOT_MACHINE;
	TRN_SLOT_SPRITE1.x = 0x45;
	TRN_SLOT_SPRITE1.y = -0x1a;
	TRN_SLOT_SPRITE1.v = 0;
	TRN_SLOT_SPRITE1.h = 0x20;
	for (i = 0; i < 3; i++) {
		TRN_SLOT_SPRITE2.x = (41 * i) - 41;
		GsSortSprite(&TRN_SLOT_SPRITE2, ACTIVE_ORDERING_TABLE, depth);
		TRN_SLOT_SPRITE1.x = TRN_SLOT_SPRITE2.x;
		y = p->scrollY[i] - 0x7a;
		if (p->settling[i] == 0) {
			y += p->spinSpeed[i] >> 6;
			if ((p->scrollY[i] + (p->spinSpeed[i] >> 6)) == 0x20) {
				y--;
			}
		} else {
			y += 2;
		}
		for (k = 0; k < 4; k++) {
			c = TRN_D_8008F1AC[i][(p->reelPos[i] + (11 + k)) % 13];
			TRN_SLOT_SPRITE1.u = (c - 1) << 5;
			if (y < -0x4a) {
				TRN_SLOT_SPRITE1.y = -0x4a;
				TRN_SLOT_SPRITE1.v = 0x8e - y;
				TRN_SLOT_SPRITE1.h = 0x20 - (TRN_SLOT_SPRITE1.v - 0xd8);
			} else if ((y + 0x20) >= -9) {
				TRN_SLOT_SPRITE1.y = y;
				TRN_SLOT_SPRITE1.v = 0xd8;
				TRN_SLOT_SPRITE1.h = 0x20 - (y + 0x2a);
			} else {
				TRN_SLOT_SPRITE1.y = y;
				TRN_SLOT_SPRITE1.v = 0xd8;
				TRN_SLOT_SPRITE1.h = 0x20;
			}
			if ((y < -9) && ((y + 0x20) >= -0x49)) {
				GsSortSprite(&TRN_SLOT_SPRITE1, ACTIVE_ORDERING_TABLE, depth);
			}
			y += 0x20;
		}
	}
}

void TRN_chooseReelStop(int16_t i, SlotMachine *p)
{
	int32_t j;
	int8_t t;

	switch (i) {
	case 0:
		switch (p->assist) {
		case 0:
			p->targetSymbol[i] = TRN_D_8008F1AC[i][(p->reelPos[i] + 12) % 13];
			break;
		case 1:
shift:
			if (p->scrollY[i] == 0) {
				j = 0;
			} else {
				j = 1;
			}
			p->targetSymbol[i] = TRN_D_8008F1AC[i][(p->reelPos[i] + 13 - j) % 13];
			break;
		case 2:
scan:
			for (j = 1; j < 3; j++) {
				if (TRN_D_8008F1AC[i][(p->reelPos[i] + 13 - j) % 13] == 7) {
					break;
				}
			}
			if (j == 3) {
				p->stopSteps[i] = 0;
				p->targetSymbol[i] = -1;
			} else {
				p->stopSteps[i] = j - 1;
				p->targetSymbol[i] = 7;
			}
			break;
		}
		break;
	case 1:
		switch (p->assist) {
		case 0:
			p->targetSymbol[i] = TRN_D_8008F1AC[i][(p->reelPos[i] + 12) % 13];
			break;
		case 1:
			if (p->targetSymbol[0] == 7) {
				goto shift;
			}
			t = p->targetSymbol[0];
			for (j = 1; j < 3; j++) {
				if (TRN_D_8008F1AC[i][(p->reelPos[i] + 13 - j) % 13] == t) {
					break;
				}
			}
			if ((j == 3) || (j == 0)) {
				p->stopSteps[i] = 0;
				p->targetSymbol[i] = -2;
			} else {
				p->stopSteps[i] = j - 1;
				p->targetSymbol[i] = p->targetSymbol[0];
			}
			break;
		case 2:
			if (p->targetSymbol[0] == 7) {
				goto scan;
			}
			p->targetSymbol[i] = -2;
			break;
		}
		break;
	case 2:
		switch (p->assist) {
		case 0:
			if (p->targetSymbol[0] != p->targetSymbol[1]) {
				break;
			}
			t = p->targetSymbol[0];
			if (t != TRN_D_8008F1AC[i][(p->reelPos[i] + 12) % 13]) {
				break;
			}
			p->stopSteps[i] = (rand() % 2) + 1;
			break;
		case 1:
			if (p->targetSymbol[0] != p->targetSymbol[1]) {
				break;
			}
			t = p->targetSymbol[0];
			if (t != 7) {
				for (j = 1; j < 3; j++) {
					if (TRN_D_8008F1AC[i][(p->reelPos[i] + 13 - j) % 13] == t) {
						break;
					}
				}
				if ((j == 3) || (j == 0)) {
					p->stopSteps[i] = 0;
					p->targetSymbol[i] = -2;
				} else {
					p->stopSteps[i] = j - 1;
					p->targetSymbol[i] = p->targetSymbol[0];
				}
			} else if (TRN_D_8008F1AC[i][(p->reelPos[i] + 12) % 13] == 7) {
				p->stopSteps[i] = (rand() % 2) + 1;
			}
			break;
		case 2:
			if ((p->targetSymbol[0] == 7) && (p->targetSymbol[1] == 7)) {
				goto scan;
			}
			p->targetSymbol[i] = -3;
			break;
		}
		break;
	}
}

int32_t TRN_startSlotSession(int32_t arg)
{
	int16_t *p = MAIN_D_801353A0;

	p[0] = -1;
	p[1] = 0;
	p[2] = arg;
	addObject(0xfdc, 0, (TickFunction)TRN_tickSlotSession, (RenderFunction)TRN_renderSlotSession);
	return 0;
}

int16_t TRN_getSlotSessionResult(void)
{
	return MAIN_D_801353A0[0];
}

void TRN_createSlotMachineBox(int16_t arg)
{
	SlotMachine *st = &TRN_SLOT_MACHINE;
	RECT startPos;
	int32_t i;

	startPos = MAIN_D_80134BD0;
	TRN_SLOT_MACHINE.result = -1;
	TRN_SLOT_MACHINE.payout = -1;
	TRN_SLOT_MACHINE.state = 0;
	TRN_SLOT_MACHINE.stat = arg;
	TRN_SLOT_MACHINE.autoStart = -1;
	TRN_SLOT_MACHINE.assist = TRN_D_8008F2CC[rand() % 10];
	for (i = 0; i < 3; i++) {
		st->reelPos[i] = rand() % 13;
		st->scrollY[i] = 0;
		st->spinSpeed[i] = 0;
		st->settling[i] = 0;
	}

	if (TRN_getTrainingSpotScreenPos(CURRENT_SCREEN, st->stat, (SVECTOR *)&startPos) == 0) {
		startPos.x -= (int16_t)(0xa8 - DRAWING_OFFSET_X);
		startPos.y -= (int16_t)(0x7e - DRAWING_OFFSET_Y);
	}

	createAnimatedUIBox(3, 0, 2, &MAIN_D_80134BC8, &startPos, (TickFunction)TRN_tickSlotMachine, (RenderFunction)TRN_renderSlotMachine);
}

int16_t TRN_getSlotMachineResult(void)
{
	return TRN_SLOT_MACHINE.result;
}

void TRN_startSlotSpin(void)
{
	TRN_SLOT_MACHINE.autoStart = 0;
}
