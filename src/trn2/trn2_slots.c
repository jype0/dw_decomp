#include <stdlib.h>

#include <libgpu.h>
#include <libgs.h>

#include <dw/garbage.h>
#include <dw/input.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/training.h>
#include <dw/trn2.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/version.h>

extern int8_t TRN2_D_8008DAA8[3][13];
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern uint8_t CURRENT_SCREEN;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;

int32_t TRN2_getTrainingSpotScreenPos(int32_t key, int32_t sub, SVECTOR *out);
void TRN2_tickSlotMachine(int32_t arg);
void TRN2_renderSlotMachine(int32_t arg);
void TRN2_chooseReelStop(int16_t i, SlotMachine *p);

static void *trn2_slots_functions[] = {
	TRN2_startSlotSpin,
	TRN2_getSlotMachineResult,
	TRN2_createSlotMachineBox,
	TRN2_getSlotSessionResult,
	TRN2_startSlotSession,
	TRN2_chooseReelStop,
	TRN2_renderSlotMachine,
	TRN2_tickSlotMachine,
};

RECT MAIN_D_80134BE8 = { -82, -87, 164, 90 };
RECT MAIN_D_80134BF0 = { -8, -8, 16, 16 };
RECT MAIN_D_80134BF8 = { -8, -8, 16, 16 };

// clang-format off
int8_t TRN2_D_8008DBC8[10] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
	0x01, 0x02,
};

GsSPRITE TRN2_SLOT_SPRITE1 = {
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

GsSPRITE TRN2_SLOT_SPRITE2 = {
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

GARBAGE_ARRAY(TRN2_tickSlotMachine, TRN2_D_8008DC54, 8, 16);

void TRN2_tickSlotMachine(arg)
	int16_t arg;
{
	SlotMachine *p;
	int32_t i;
	int32_t t;
	RECT rect;

	rect = MAIN_D_80134BF8;
	p = &TRN2_SLOT_MACHINE;
	switch (p->state) {
	case 0:
		if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CONFIRM_BUTTON) || (p->autoStart >= 0)) {
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
#if VERSION_REGION_IS(NTSCJ)
			if ((p->state - 1 == i) && ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CONFIRM_BUTTON)) {
				TRN2_chooseReelStop(p->state - 1, p);
			}
#else
			{
				int32_t k = p->state - 1;

				if ((k == i) && ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CONFIRM_BUTTON)) {
					TRN2_chooseReelStop(k, p);
				}
			}
#endif
			t = p->scrollY[i] + (p->spinSpeed[i] >> 6);
			if (t == 0x20) {
				t--;
			}
			if (t >= 0x20) {
				if ((i < (p->state - 1)) && (p->stopSteps[i] <= 0)) {
					p->settling[i]++;
				} else if ((i == (p->state - 1)) && (p->stopSteps[i] <= 0) && ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CONFIRM_BUTTON)) {
					p->settling[i]++;
				}
			}
		}
		if ((p->state < 4) && ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CONFIRM_BUTTON)) {
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
			if ((TRN2_D_8008DAA8[0][p->reelPos[0]] == TRN2_D_8008DAA8[1][p->reelPos[1]]) && (TRN2_D_8008DAA8[0][p->reelPos[0]] == TRN2_D_8008DAA8[2][p->reelPos[2]])) {
				if (TRN2_D_8008DAA8[0][p->reelPos[0]] == 7) {
					playSound(8, 0xc);
					p->payout = 100;
				} else if ((TRN2_D_8008DAA8[0][p->reelPos[0]] - 1) == p->stat) {
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
		} else if ((p->resultTimer >= 0x1e) || ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & CONFIRM_BUTTON)) {
			p->state++;
		}
		p->resultTimer++;
		break;
	case 6:
		if (TRN2_getTrainingSpotScreenPos(CURRENT_SCREEN, p->stat, (SVECTOR *)&rect) == 0) {
			rect.x -= (int16_t)(0xa8 - DRAWING_OFFSET_X);
			rect.y -= (int16_t)(0x7e - DRAWING_OFFSET_Y);
		}
		removeAnimatedUIBox(arg, &rect);
		closeBox(0);
		p->result = p->payout;
		break;
	}
}

void TRN2_renderSlotMachine(arg)
	int16_t arg;
{
	int32_t i = 0;
	SlotMachine *p;
	int32_t y;
	int32_t depth;
	int32_t k;
	int32_t c;

	depth = 6 - arg;
	p = &TRN2_SLOT_MACHINE;
	TRN2_SLOT_SPRITE1.x = 0x45;
	TRN2_SLOT_SPRITE1.y = -0x1a;
	TRN2_SLOT_SPRITE1.v = 0;
	TRN2_SLOT_SPRITE1.h = 0x20;
	for (i = 0; i < 3; i++) {
		TRN2_SLOT_SPRITE2.x = (41 * i) - 41;
		GsSortSprite(&TRN2_SLOT_SPRITE2, ACTIVE_ORDERING_TABLE, depth);
		TRN2_SLOT_SPRITE1.x = TRN2_SLOT_SPRITE2.x;
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
			c = (p->reelPos[i] + (11 + k)) % 13;
			TRN2_SLOT_SPRITE1.u = (TRN2_D_8008DAA8[i][c] - 1) << 5;
			if (y < -0x4a) {
				TRN2_SLOT_SPRITE1.y = -0x4a;
				TRN2_SLOT_SPRITE1.v = 0x8e - y;
				TRN2_SLOT_SPRITE1.h = 0x20 - (TRN2_SLOT_SPRITE1.v - 0xd8);
			} else if ((y + 0x20) >= -9) {
				TRN2_SLOT_SPRITE1.y = y;
				TRN2_SLOT_SPRITE1.v = 0xd8;
				TRN2_SLOT_SPRITE1.h = 0x20 - (y + 0x2a);
			} else {
				TRN2_SLOT_SPRITE1.y = y;
				TRN2_SLOT_SPRITE1.v = 0xd8;
				TRN2_SLOT_SPRITE1.h = 0x20;
			}
			if ((y < -9) && ((y + 0x20) >= -0x49)) {
				GsSortSprite(&TRN2_SLOT_SPRITE1, ACTIVE_ORDERING_TABLE, depth);
			}
			y += 0x20;
		}
	}
}

void TRN2_chooseReelStop(int16_t i, SlotMachine *p)
{
	int32_t j;

	switch (i) {
	case 0:
		switch (p->assist) {
		case 0:
			p->targetSymbol[i] = TRN2_D_8008DAA8[i][(p->reelPos[i] + 12) % 13];
			break;
		case 1:
shift:
			if (p->scrollY[i] == 0) {
				j = 0;
			} else {
				j = 1;
			}
			p->targetSymbol[i] = TRN2_D_8008DAA8[i][(p->reelPos[i] + 13 - j) % 13];
			break;
		case 2:
scan:
			for (j = 1; j < 3; j++) {
				if (TRN2_D_8008DAA8[i][(p->reelPos[i] + 13 - j) % 13] == 7) {
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
			p->targetSymbol[i] = TRN2_D_8008DAA8[i][(p->reelPos[i] + 12) % 13];
			break;
		case 1:
			if (p->targetSymbol[0] == 7) {
				goto shift;
			}
			for (j = 1; j < 3; j++) {
				if (TRN2_D_8008DAA8[i][(p->reelPos[i] + 13 - j) % 13] == p->targetSymbol[0]) {
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
			if (p->targetSymbol[0] != TRN2_D_8008DAA8[i][(p->reelPos[i] + 12) % 13]) {
				break;
			}
			p->stopSteps[i] = (rand() % 2) + 1;
			break;
		case 1:
			if (p->targetSymbol[0] != p->targetSymbol[1]) {
				break;
			}
			if (p->targetSymbol[0] != 7) {
				for (j = 1; j < 3; j++) {
					if (TRN2_D_8008DAA8[i][(p->reelPos[i] + 13 - j) % 13] == p->targetSymbol[0]) {
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
			} else if (TRN2_D_8008DAA8[i][(p->reelPos[i] + 12) % 13] == 7) {
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

int32_t TRN2_startSlotSession(int32_t arg)
{
	int16_t *p = MAIN_D_801353E0;

	p[0] = -1;
	p[1] = 0;
	p[2] = arg;
	addObject(0xfdc, 0, (TickFunction)TRN2_tickSlotSession, (RenderFunction)TRN2_renderSlotSession);
	return 0;
}

int16_t TRN2_getSlotSessionResult(void)
{
	return MAIN_D_801353E0[0];
}

void TRN2_createSlotMachineBox(int32_t arg)
{
	SlotMachine *st;
	int32_t i;
	int32_t id;
	RECT startPos;

	st = &TRN2_SLOT_MACHINE;
	id = 3;
	startPos = MAIN_D_80134BF0;
	st->result = -1;
	st->payout = -1;
	st->state = 0;
	st->stat = arg;
	st->autoStart = -1;
	st->assist = TRN2_D_8008DBC8[rand() % 10];
	for (i = 0; i < 3; i++) {
		st->reelPos[i] = rand() % 13;
		st->scrollY[i] = 0;
		st->spinSpeed[i] = 0;
		st->settling[i] = 0;
	}

	if (TRN2_getTrainingSpotScreenPos(CURRENT_SCREEN, st->stat, (SVECTOR *)&startPos) == 0) {
		startPos.x -= (int16_t)(0xa8 - DRAWING_OFFSET_X);
		startPos.y -= (int16_t)(0x7e - DRAWING_OFFSET_Y);
	}

	createAnimatedUIBox(id, 0, 2, &MAIN_D_80134BE8, &startPos, (TickFunction)TRN2_tickSlotMachine, (RenderFunction)TRN2_renderSlotMachine);
}

int32_t TRN2_getSlotMachineResult(void)
{
	return TRN2_SLOT_MACHINE.result;
}

void TRN2_startSlotSpin(void)
{
	TRN2_SLOT_MACHINE.autoStart = 0;
}
