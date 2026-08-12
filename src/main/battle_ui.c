#include <libgs.h>

#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/math.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/types.h>
#include <dw/ui.h>
#include <dw/world_object.h>

void BTL_tickBattleEndText();
void renderNumber(int32_t a, int32_t x, int32_t y, int32_t digits,
		  int32_t value, int32_t layer);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
		  int32_t f, int32_t g, int32_t h, int32_t i);
void BTL_renderBattleEndText(int32_t layer);
void renderLinePrimitive(uint32_t color, int32_t x0, int32_t y0, int32_t x1,
			 int32_t y1, int32_t order, uint32_t mode);
void renderTrianglePrimitive(int32_t color, int32_t x0, int32_t y0, int32_t x1,
			     int32_t y1, int32_t x2, int32_t y2, int32_t otz,
			     int32_t flag);
void initStringFT4(POLY_FT4 *p);
void setUVDataPolyFT4(POLY_FT4 *p, int32_t u, int32_t v, int32_t w, int32_t h);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t x, int32_t y, int32_t w,
		       int32_t h);
void drawString(char *text, int32_t color, int32_t pos);
void clearTextArea(void);
int32_t hasMove(int32_t moveId);
void learnMove(int32_t moveId);
void BTL_battleTickFrame(void);
void BTL_initializeBattleEndText(int32_t a, int32_t b, RECT *r);
void BTL_appendItemDroppedText(Entity *e);
void BTL_appendInjuredText(char *name);
void BTL_appendCommandLearnedText(void);
void BTL_appendMPBonusText(void);
void BTL_appendMoveLearnedText(int32_t arg0);
void BTL_drawBattleEndText(int32_t a);
int32_t BTL_isEndBoxTextFinished(void);
void spawnDroppedItems(Entity *e, int32_t item);
void setEntityTextDigit(POLY_FT4 *poly, int32_t x, int32_t y);

void initBitBox();
void battleStatsGainsAndDrops(uint8_t *droppedItems);
void handleBattleInjury();
void battleMoveLearning();
void createBitBox();
void createFinalBalanceBox();
void handleBattleEndBox();
void tickBitBox(int32_t instanceId);
void renderBitBox(uint8_t layer);
void renderFinalBalance(int32_t layer);
void resetStatsAfterCombat();
void createPostBattleStatsBox();
void MAIN_func_800EE088();
void MAIN_func_800EE1FC(uint8_t depth);
void MAIN_func_800EE7B4(int32_t id);

extern uint8_t MOVE_LEARN_CHANCES[58][3];
extern int16_t ENEMY_COUNT;
extern int32_t MAIN_D_80134D70;
extern uint16_t MAIN_D_80134E90;
extern int32_t MAIN_D_80134E9C;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern POLY_FT4 BIT_BOX;
extern int16_t INITIAL_COMBAT_STATS[][6];
extern int16_t STATS_GAINS[6];
extern int16_t MAIN_D_80134EA0;
extern int8_t BTL_END_BOX_TEXTBUFFER[];
extern int8_t HAS_STAT_GAIN[6];
extern char MAIN_D_80124C0C[];
extern char MAIN_D_80124C54[];
extern uint8_t GAME_STATE;
extern uint8_t CURRENT_SCREEN;

int8_t STAT_GAIN_FACTORS[4] = { 10, 12, 16, 0 };
char BITS_LABEL[] = "Bits";

static void *battle_ui_functions[] = {
	MAIN_func_800EE7B4,
	MAIN_func_800EE1FC,
	MAIN_func_800EE088,
	createPostBattleStatsBox,
	resetStatsAfterCombat,
	renderFinalBalance,
	renderBitBox,
	tickBitBox,
	handleBattleEndBox,
	createFinalBalanceBox,
	createBitBox,
	battleMoveLearning,
	handleBattleInjury,
	battleStatsGainsAndDrops,
	initBitBox,
};

void battleStatsGainsAndDrops(uint8_t *droppedItems)
{
	int32_t i;
	int32_t stat;
	int32_t den;
	int32_t chance;
	int32_t type;

	for (i = 0; i < 6; i++) {
		STATS_GAINS[i] = 0;
	}

	for (stat = 0; stat < 6; stat++) {
		int32_t partnerStat;
		int32_t enemyStat = INITIAL_COMBAT_STATS[1][stat];

		partnerStat = INITIAL_COMBAT_STATS[0][stat];

		for (i = 1; i <= ENEMY_COUNT; i++) {
			if (enemyStat < INITIAL_COMBAT_STATS[i][stat]) {
				enemyStat = INITIAL_COMBAT_STATS[i][stat];
			}
		}

		den = partnerStat * 10;
		if (den == 0) {
			den = 10;
		}

		if (enemyStat >= partnerStat) {
			STATS_GAINS[stat] = ((den + (enemyStat * STAT_GAIN_FACTORS[ENEMY_COUNT - 1])) - 1) / den;
		} else {
			chance = ((enemyStat * STAT_GAIN_FACTORS[ENEMY_COUNT - 1]) * 100) / den;
			if (random(100) < chance) {
				STATS_GAINS[stat] = 1;
			}
		}
	}

	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] != 0) {
			continue;
		}
		switch (i) {
		case 0:
			chance = ((COMBAT_DATA_PTR->player.startingHP - PARTNER_ENTITY.digimonEntity.stats.current.currentHP) * 100) / PARTNER_ENTITY.digimonEntity.stats.base.hp;
			break;
		case 1:
		case 2:
			chance = COMBAT_DATA_PTR->player.hitCount * 10;
			break;
		case 3:
			chance = COMBAT_DATA_PTR->player.unk2 * 10;
			break;
		case 4:
			chance = (COMBAT_DATA_PTR->player.blockedCount * 10) + (((COMBAT_DATA_PTR->player.startingHP - PARTNER_ENTITY.digimonEntity.stats.current.currentHP) * 50) / PARTNER_ENTITY.digimonEntity.stats.base.hp);
			break;
		case 5:
			chance = (COMBAT_DATA_PTR->player.hitCount * 5) + (COMBAT_DATA_PTR->player.unk2 * 5);
			break;
		}

		if (random(100) < chance) {
			STATS_GAINS[i] = 1;
		}
	}

	for (i = 0; i < 3; i++) {
		if (i < ENEMY_COUNT) {
			if (CURRENT_SCREEN == 0x8f) {
				droppedItems[i] = 0xff;
				continue;
			}

			type = ENTITY_TABLE[(COMBAT_DATA_PTR->player.entityIds + 1)[i]]->type;

			chance = DIGIMON_DATA[type].dropChance;
			if (random(100) < chance) {
				droppedItems[i] = DIGIMON_DATA[type].dropItem;
				continue;
			}
		}
		droppedItems[i] = 0xff;
	}
}

void handleBattleInjury(void)
{
	int16_t hpRatio;
	int16_t chance;
	int16_t roll;

	hpRatio = (100 * PARTNER_ENTITY.digimonEntity.stats.current.currentHP) / PARTNER_ENTITY.digimonEntity.stats.base.hp;
	chance = PARTNER_PARA.tiredness - hpRatio;
	roll = random(100);
	if (roll < chance) {
		PARTNER_PARA.condition |= 0x20;
	}
}

void battleMoveLearning(void)
{
	uint8_t learnableMoves[12];
	int32_t i;
	int32_t count;
	int32_t j;
	int16_t moveId;
	uint8_t foundIdx;
	int32_t threshold;

	count = 0;
	for (i = 0; i < 12; i++) {
		moveId = COMBAT_DATA_PTR->player.usedMoves[i];
		if (moveId == 0xff) {
			break;
		}

		if (hasMove(moveId) == 1) {
			continue;
		}

		for (j = 0; j < 3; j++) {
			if (MOVE_DATA[moveId].special == DIGIMON_DATA[ENTITY_TABLE[1]->type].special[j]) {
				foundIdx = j;
				break;
			}
		}

		if (j == 3) {
			continue;
		}

		for (j = 0; j < 16; j++) {
			if (DIGIMON_DATA[ENTITY_TABLE[1]->type].moves[j] == moveId) {
				break;
			}
		}

		if (j == 16) {
			continue;
		}

		threshold = MOVE_LEARN_CHANCES[moveId][foundIdx];
		if (random(100) < threshold) {
			learnableMoves[count++] = moveId;
		}
	}

	if (count == 0) {
		return;
	}

	moveId = learnableMoves[random(count)];
	learnMove(moveId);
	BTL_appendMoveLearnedText(moveId);
}

void createBitBox(void)
{
	int16_t screenPos[2];
	RECT finalPos;
	RECT startPos;

	finalPos.x = -88;
	finalPos.y = 18;
	finalPos.w = 176;
	finalPos.h = BTL_END_BOX_TEXTBUFFER[0] ? 66 : 31;

	getEntityScreenPos(ENTITY_TABLE[0], 1, screenPos);

	startPos.x = screenPos[0] - 5;
	startPos.y = screenPos[1] - 5;
	startPos.w = 10;
	startPos.h = 10;
	createAnimatedUIBox(1, 0, 2, &finalPos, &startPos, tickBitBox, (RenderFunction)renderBitBox);

	drawString(BITS_LABEL, 0, 72);
}

void handleBattleEndBox(void)
{
	uint8_t droppedItems[3];
	RECT boxPosition;
	int32_t i;
	int32_t slot;
	int32_t done;

	initBitBox();

	MAIN_D_80134E90 = 0;
	for (i = 1; i <= ENEMY_COUNT; i++) {
		MAIN_D_80134E90 += NPC_ENTITIES[COMBAT_DATA_PTR->player.entityIds[i] - 2].bits;
	}

	battleStatsGainsAndDrops(droppedItems);
	boxPosition.x = -78;
	boxPosition.y = 54;
	boxPosition.w = 156;
	boxPosition.h = 24;
	BTL_initializeBattleEndText(0x60, 2, &boxPosition);

	for (i = 0; i < 3; i++) {
		slot = i;

		if (droppedItems[i] == 0xff) {
			continue;
		}

		BTL_appendItemDroppedText(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[slot + 1]]);
	}

	if (!(PARTNER_PARA.condition & 0x20)) {
		if (MAIN_D_80134D70 == 1) {
			handleBattleInjury();
		}

		if (PARTNER_PARA.condition & 0x20) {
			BTL_appendInjuredText(DIGIMON_DATA[ENTITY_TABLE[1]->type].name);
		}
	}

	BTL_appendCommandLearnedText();
	BTL_appendMPBonusText();
	battleMoveLearning();
	GAME_STATE = 2;
	createPostBattleStatsBox();
	createBitBox();

	while (1) {
		if ((UI_BOX_DATA[0].state == 1) &&
		    (UI_BOX_DATA[1].state == 1)) {
			break;
		}

		BTL_battleTickFrame();
	}

	if (BTL_END_BOX_TEXTBUFFER[0]) {
		for (i = 0; i < 2; i++) {
			BTL_drawBattleEndText(1);
			BTL_battleTickFrame();
		}
	}

	done = 0;
	while (!done) {
		for (i = 0; i < 6; i++) {
			if (STATS_GAINS[i] != 0) {
				break;
			}
		}

		if ((i == 6) && (MAIN_D_80134EA0 == 0)) {
			done = 1;
		}

		BTL_battleTickFrame();
	}

	done = 0;
	createFinalBalanceBox();

	while (!done) {
		if (UI_BOX_DATA[2].state == 1) {
			done = 1;
		}

		BTL_battleTickFrame();
	}

	while (1) {
		if (MAIN_D_80134E90 == 0) {
			break;
		}

		BTL_battleTickFrame();
	}

	MAIN_D_80134E9C = 0;

	while (1) {
		if (BTL_isEndBoxTextFinished()) {
			break;
		}

		BTL_battleTickFrame();
	}

	for (i = 0; i < ENEMY_COUNT; i++) {
		slot = i;

		if (droppedItems[i] == 0xff) {
			continue;
		}

		spawnDroppedItems(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[slot + 1]],
				  droppedItems[i]);
	}

	resetStatsAfterCombat();
	MAIN_func_800EE7B4(0);
	MAIN_func_800EE7B4(1);
	MAIN_func_800EE7B4(2);
}

void tickBitBox(int32_t instanceId)
{
	int32_t bits;

	if (UI_BOX_DATA[2].state != 1) {
		return;
	}

	if ((bits = MAIN_D_80134E90) == 0) {
		BTL_tickBattleEndText(instanceId, bits);
		return;
	}

	if ((POLLED_INPUT == 0x40) || (POLLED_INPUT == 0x10)) {
		if (!(POLLED_INPUT_PREVIOUS & 0x40) &&
		    !(POLLED_INPUT_PREVIOUS & 0x10)) {
			MAIN_D_80134E9C = 1;
		}
	}

	if (MAIN_D_80134E9C == 1) {
		MAIN_D_80134E90 = 0;
		MONEY += bits;
	} else {
		playSound(0, 0x16);
		--MAIN_D_80134E90;
		MONEY += 1;
	}

	if (MONEY >= 1000000) {
		MONEY = 999999;
	}

	if (MAIN_D_80134E90 == 0) {
		playSound(0, 0x17);
	}
}

void renderBitBox(uint8_t layer)
{
	renderNumber(2, -18, 28, 5, MAIN_D_80134E90, 6 - layer);

	BIT_BOX.x0 = 52;
	BIT_BOX.y0 = 28;
	BIT_BOX.x1 = 76;
	BIT_BOX.y1 = 28;
	BIT_BOX.x2 = 52;
	BIT_BOX.y2 = 40;
	BIT_BOX.x3 = 76;
	BIT_BOX.y3 = 40;
	GsSortPoly(&BIT_BOX, ACTIVE_ORDERING_TABLE, 6 - layer);

	renderString(4, -78, 28, 48, 12, 0, 72, 6 - layer, 0);

	if (BTL_END_BOX_TEXTBUFFER[0]) {
		renderLinePrimitive(0x8e8e8e, -86, 50, 84, 50, 6 - layer, 0);
		renderLinePrimitive(0x121212, -85, 51, 85, 51, 6 - layer, 0);
		BTL_renderBattleEndText(layer);
	}
}

void renderFinalBalance(int32_t layer)
{
	renderNumber(0,
		     UI_BOX_DATA[2].finalPos.x + 58,
		     UI_BOX_DATA[2].finalPos.y + 10,
		     6, MONEY, 6 - layer);

	BIT_BOX.x0 = UI_BOX_DATA[2].finalPos.x + 140;
	BIT_BOX.y0 = UI_BOX_DATA[2].finalPos.y + 10;
	BIT_BOX.x1 = UI_BOX_DATA[2].finalPos.x + 164;
	BIT_BOX.y1 = UI_BOX_DATA[2].finalPos.y + 10;
	BIT_BOX.x2 = UI_BOX_DATA[2].finalPos.x + 140;
	BIT_BOX.y2 = UI_BOX_DATA[2].finalPos.y + 22;
	BIT_BOX.x3 = UI_BOX_DATA[2].finalPos.x + 164;
	BIT_BOX.y3 = UI_BOX_DATA[2].finalPos.y + 22;
	GsSortPoly(&BIT_BOX, ACTIVE_ORDERING_TABLE, 6 - layer);

	renderString(0,
		     UI_BOX_DATA[2].finalPos.x + 10,
		     UI_BOX_DATA[2].finalPos.y + 10,
		     36, 12, 0, 72, 6 - layer, 0);
}

void resetStatsAfterCombat(void)
{
	PARTNER_ENTITY.digimonEntity.stats.base.hp = INITIAL_COMBAT_STATS[0][0];
	PARTNER_ENTITY.digimonEntity.stats.base.mp = INITIAL_COMBAT_STATS[0][1];
	PARTNER_ENTITY.digimonEntity.stats.base.off = INITIAL_COMBAT_STATS[0][2];
	PARTNER_ENTITY.digimonEntity.stats.base.def = INITIAL_COMBAT_STATS[0][3];
	PARTNER_ENTITY.digimonEntity.stats.base.speed = INITIAL_COMBAT_STATS[0][4];
	PARTNER_ENTITY.digimonEntity.stats.base.brain = INITIAL_COMBAT_STATS[0][5];
}

void createPostBattleStatsBox(void)
{
	int32_t i;
	int32_t row;
	int32_t y;
	int16_t screenPos[2];
	RECT finalPos;
	RECT startPos;

	MAIN_D_80134E9C = 0;
	clearTextArea();

	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] == 0) {
			HAS_STAT_GAIN[i] = 0;
		} else {
			HAS_STAT_GAIN[i] = 1;
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
			drawString(MAIN_D_80124C54, 0, 240);
		}

		DrawSync(0);
	}

	MAIN_D_80134EA0 = 100;

	finalPos.x = -88;
	finalPos.y = -78;
	finalPos.w = 176;
	finalPos.h = 96;

	getEntityScreenPos(ENTITY_TABLE[1], 1, screenPos);

	startPos.x = screenPos[0] - 5;
	startPos.y = screenPos[1] - 5;
	startPos.w = 10;
	startPos.h = 10;
	createAnimatedUIBox(0, 0, 2, &finalPos, &startPos, MAIN_func_800EE088,
			    (RenderFunction)MAIN_func_800EE1FC);
}

void MAIN_func_800EE088(void)
{
	int32_t i;
	int16_t gain;

	if (MAIN_D_80134EA0 > 0) {
		MAIN_D_80134EA0--;
	}

	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] != 0) {
			break;
		}
	}

	if ((POLLED_INPUT == 0x40) || (POLLED_INPUT == 0x10)) {
		if (!(POLLED_INPUT_PREVIOUS & 0x40) &&
		    !(POLLED_INPUT_PREVIOUS & 0x10)) {
			MAIN_D_80134E9C = 1;
		}
	}

	if (MAIN_D_80134E9C != 1) {
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

	MAIN_D_80134EA0 = 0;
}

void MAIN_func_800EE1FC(uint8_t depth)
{
	RECT *box = &UI_BOX_DATA[0].finalPos;
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

		if ((MAIN_D_80134EA0 == 0) &&
		    (STATS_GAINS[i] > 0) &&
		    (first == 1)) {
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
			prim->r0 = 0x80;
			prim->g0 = 0x80;
			prim->b0 = 0x80;
			setUVDataPolyFT4(prim, 96, 180, 12, 12);
			setPosDataPolyFT4(prim, box->x + 130,
					  (box->y + 9) + (i * 13), 12, 12);
			AddPrim((ACTIVE_ORDERING_TABLE->org + 6) - depth,
				prim++);
			GsSetWorkBase((PACKET *)prim);
		}

		renderNumber(5,
			     box->x + 142, (box->y + 9) + (i * 13),
			     2, STATS_GAINS[i], 6 - depth);
	}

	for (i = 0; i < 6; i++) {
		renderNumber(0,
			     box->x + 68, (box->y + 9) + (i * 13),
			     4, INITIAL_COMBAT_STATS[0][i], 6 - depth);
	}

	renderLinePrimitive(0xfad990, box->x + 122,
			    box->y + 2, box->x + 122,
			    (box->y + box->h) - 3, 6 - depth, 0);
	renderLinePrimitive(0x20202, box->x + 123,
			    box->y + 2, box->x + 123,
			    (box->y + box->h) - 3, 6 - depth, 0);

	for (i = 0; i < 6; i++) {
		renderString(4,
			     box->x + 10, (box->y + 9) + (i * 13),
			     48, 12, 0, i * 12, 6 - depth, 0);
	}

	rect.attribute = 0;
	rect.x = -22;
	rect.h = 2;

	for (i = 0; i < 6; i++) {
		y = (box->y + 20) + (i * 13);
		rect.y = y - 2;

		if (i < 2) {
			rect.w = (INITIAL_COMBAT_STATS[0][i] * 50) / 9999;
		} else {
			rect.w = INITIAL_COMBAT_STATS[0][i] * 50 / 999;
		}

		hasStatGain = HAS_STAT_GAIN[i];
		if (hasStatGain != 1) {
			if (hasStatGain == 0) {
				rect.r = rect.g = rect.b = 0x78;
				GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE,
					      (uint16_t)(6 - depth));
				rect.r = rect.g = rect.b = 0x28;
			}
		} else {
			rect.r = 0x69;
			rect.g = 0xc2;
			rect.b = 0xff;
			GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE,
				      (uint16_t)(6 - depth));
			rect.r = 0;
			rect.g = 0x5a;
			rect.b = 0x96;
		}

		rect.w = 50;

		renderTrianglePrimitive(0x20202,
					box->x + 64, y,
					box->x + 64, y - 3,
					box->x + 117, y - 3,
					6 - depth, 0);
		renderTrianglePrimitive(0x666666,
					box->x + 117, y - 2,
					box->x + 117, y,
					box->x + 65, y,
					6 - depth, 0);
		GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE,
			      (uint16_t)(6 - depth));
	}
}

void MAIN_func_800EE7B4(int32_t id)
{
	if (UI_BOX_DATA[id].state != 0) {
		removeAnimatedUIBox(id, NULL);
	}
}

void initBitBox(void)
{
	initStringFT4(&BIT_BOX);

	BIT_BOX.r0 = 0x80;
	BIT_BOX.g0 = 0x80;
	BIT_BOX.b0 = 0x80;
	setUVDataPolyFT4(&BIT_BOX, 156, 240, 24, 12);
}

void createFinalBalanceBox(void)
{
	RECT finalPos;
	RECT startPos;

	finalPos.x = -88;
	finalPos.y = -13;
	finalPos.w = 176;
	finalPos.h = 31;
	startPos = UI_BOX_DATA[1].startPos;
	createAnimatedUIBox(2, 1, 0, &finalPos, &startPos, NULL,
			    renderFinalBalance);
}
