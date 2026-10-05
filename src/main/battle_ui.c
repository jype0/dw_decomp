#include <libgs.h>

#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/font.h>
#include <dw/graphics.h>
#include <dw/input.h>
#include <dw/item.h>
#include <dw/line.h>
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
void initStringFT4(POLY_FT4 *p);
void setUVDataPolyFT4(POLY_FT4 *p, int32_t u, int32_t v, int32_t w, int32_t h);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t x, int32_t y, int32_t w,
		       int32_t h);
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
void setEntityTextDigit(POLY_FT4 *poly, int32_t x, int32_t y);

void initializeBitText();
void battleStatsGainsAndDrops(uint8_t *droppedItems);
void handleBattleInjury();
void battleMoveLearning();
void createBitsBox();
void createFinalBalanceBox();
void handleBattleEndBox();
void tickBitBox(int32_t instanceId);
void renderBitBox(uint8_t layer);
void renderFinalBalance(int32_t layer);
void resetStatsAfterCombat();
void createPostBattleStatsBox();
void tickPostBattleStatsBox();
void renderPostBattleStatsBox(int16_t depth);
void removeBattleEndBox(int32_t id);

extern uint8_t MOVE_LEARN_CHANCES[58][3];
extern int16_t ENEMY_COUNT;
extern int32_t HAS_TAKEN_DAMAGE;
extern uint16_t BITS_TO_GAIN;
extern int32_t SHOULD_SKIP_BIT_COUNTING;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern POLY_FT4 BIT_TEXT;
extern int16_t INITIAL_COMBAT_STATS[][6];
extern int16_t STATS_GAINS[6];
extern int16_t POST_BATTLE_STATS_TIMER;
extern int8_t BTL_END_BOX_TEXTBUFFER[];
extern int8_t STAT_BOX_HAS_GAIN[6];
extern char MAIN_D_80124C0C[];
extern char MAIN_D_80124C54[];
extern uint8_t GAME_STATE;
extern uint8_t CURRENT_SCREEN;

int8_t STAT_GAIN_FACTORS[4] = { 10, 12, 16, 0 };
#if defined(VERSION_JP)
/* Money obtained */
char BITS_LABEL[] = "取得金";
char STR_BRACES[] = "｛｝";
/* Money held */
char STR_SHOJIKIN[] = "所持金";
#else
char BITS_LABEL[] = "Bits";
#endif

static void *battle_ui_functions[] = {
	removeBattleEndBox,
	renderPostBattleStatsBox,
	tickPostBattleStatsBox,
	createPostBattleStatsBox,
	resetStatsAfterCombat,
	renderFinalBalance,
	renderBitBox,
	tickBitBox,
	handleBattleEndBox,
	createFinalBalanceBox,
	createBitsBox,
	battleMoveLearning,
	handleBattleInjury,
	battleStatsGainsAndDrops,
	initializeBitText,
};

void battleStatsGainsAndDrops(uint8_t *droppedItems)
{
	int32_t den;
	int32_t partnerStat;
	int32_t enemyStat;
	int32_t i;
	int32_t stat;
	int32_t chance;
#if !defined(VERSION_JP)
	int32_t type;
#endif

	for (i = 0; i < 6; i++) {
		STATS_GAINS[i] = 0;
	}

	for (stat = 0; stat < 6; stat++) {
		enemyStat = INITIAL_COMBAT_STATS[1][stat];
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
			if (randomLimit(100) < chance) {
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

		if (randomLimit(100) < chance) {
			STATS_GAINS[i] = 1;
		}
	}

	for (i = 0; i < 3; i++) {
		if (i < ENEMY_COUNT) {
			if (CURRENT_SCREEN == 0x8f) {
				droppedItems[i] = 0xff;
				continue;
			}

#if defined(VERSION_JP)
			if (DIGIMON_DATA[ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i + 1]]->type].dropChance > randomLimit(100)) {
				droppedItems[i] = DIGIMON_DATA[ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i + 1]]->type].dropItem;
				continue;
			}
#else
			type = ENTITY_TABLE[(COMBAT_DATA_PTR->player.entityIds + 1)[i]]->type;

			chance = DIGIMON_DATA[type].dropChance;
			if (randomLimit(100) < chance) {
				droppedItems[i] = DIGIMON_DATA[type].dropItem;
				continue;
			}
#endif
		}
		droppedItems[i] = 0xff;
	}
}

void handleBattleInjury(void)
{
	int16_t roll;
	int16_t hpRatio;
	int16_t chance;

	hpRatio = (100 * PARTNER_ENTITY.digimonEntity.stats.current.currentHP) / PARTNER_ENTITY.digimonEntity.stats.base.hp;
	chance = PARTNER_PARA.tiredness - hpRatio;
	roll = randomLimit(100);
	if (roll < chance) {
		PARTNER_PARA.condition |= 0x20;
	}
}

void battleMoveLearning(void)
{
	int32_t i;
	int32_t count;
	uint8_t learnableMoves[12];
	uint8_t foundIdx;
	int32_t j;
	int16_t moveId;

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

		if (MOVE_LEARN_CHANCES[moveId][foundIdx] > randomLimit(100)) {
			learnableMoves[count++] = moveId;
		}
	}

	if (count == 0) {
		return;
	}

	moveId = learnableMoves[randomLimit(count)];
	learnMove(moveId);
	BTL_appendMoveLearnedText(moveId);
}

void createBitsBox(void)
{
	int16_t screenPos[2];
	RECT finalPos;
	RECT startPos;

	setRECT(&finalPos, -88, 18, 176, BTL_END_BOX_TEXTBUFFER[0] ? 66 : 31);

	getEntityScreenPos(ENTITY_TABLE[0], 1, screenPos);

	setRECT(&startPos, screenPos[0] - 5, screenPos[1] - 5, 10, 10);
	createAnimatedUIBox(1, 0, 2, &finalPos, &startPos, tickBitBox, (RenderFunction)renderBitBox);

	drawString(BITS_LABEL, 0, 72);
#if defined(VERSION_JP)
	drawString(STR_BRACES, 0x9c, 0xf0);
#endif
}

void handleBattleEndBox(void)
{
	uint8_t droppedItems[3];
	RECT boxPosition;
	int32_t i;
#if !defined(VERSION_JP)
	int32_t slot;
#endif
	int32_t done;

	initializeBitText();

	BITS_TO_GAIN = 0;
	for (i = 1; i <= ENEMY_COUNT; i++) {
		BITS_TO_GAIN += NPC_ENTITIES[COMBAT_DATA_PTR->player.entityIds[i] - 2].bits;
	}

	battleStatsGainsAndDrops(droppedItems);
	setRECT(&boxPosition, -78, 54, 156, 24);
	BTL_initializeBattleEndText(0x60, 2, &boxPosition);

#if defined(VERSION_JP)
	for (i = 0; i < 3; i++) {
		if (droppedItems[i] == 0xff) {
			continue;
		}

		BTL_appendItemDroppedText(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i + 1]]);
	}
#else
	for (i = 0; i < 3; i++) {
		slot = i;

		if (droppedItems[i] == 0xff) {
			continue;
		}

		BTL_appendItemDroppedText(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[slot + 1]]);
	}
#endif

	if (!(PARTNER_PARA.condition & 0x20)) {
		if (HAS_TAKEN_DAMAGE == 1) {
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
	createBitsBox();

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

		if ((i == 6) && (POST_BATTLE_STATS_TIMER == 0)) {
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
		if (BITS_TO_GAIN == 0) {
			break;
		}

		BTL_battleTickFrame();
	}

	SHOULD_SKIP_BIT_COUNTING = 0;

	while (1) {
		if (BTL_isEndBoxTextFinished()) {
			break;
		}

		BTL_battleTickFrame();
	}

#if defined(VERSION_JP)
	for (i = 0; i < ENEMY_COUNT; i++) {
		if (droppedItems[i] == 0xff) {
			continue;
		}

		spawnDroppedItems(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i + 1]],
				  droppedItems[i]);
	}
#else
	for (i = 0; i < ENEMY_COUNT; i++) {
		slot = i;

		if (droppedItems[i] == 0xff) {
			continue;
		}

		spawnDroppedItems(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[slot + 1]],
				  droppedItems[i]);
	}
#endif

	resetStatsAfterCombat();
	removeBattleEndBox(0);
	removeBattleEndBox(1);
	removeBattleEndBox(2);
}

void tickBitBox(instanceId)
	int16_t instanceId;
{
	if (UI_BOX_DATA[2].state != 1) {
		return;
	}

	if (BITS_TO_GAIN == 0) {
		BTL_tickBattleEndText(instanceId);
		return;
	}

	if ((POLLED_INPUT == CONFIRM_BUTTON) || (POLLED_INPUT == CANCEL_BUTTON)) {
		if (!(POLLED_INPUT_PREVIOUS & CONFIRM_BUTTON) &&
		    !(POLLED_INPUT_PREVIOUS & CANCEL_BUTTON)) {
			SHOULD_SKIP_BIT_COUNTING = 1;
		}
	}

	if (SHOULD_SKIP_BIT_COUNTING == 1) {
		MONEY += BITS_TO_GAIN;
		BITS_TO_GAIN = 0;
	} else {
		playSound(0, 0x16);
		--BITS_TO_GAIN;
		MONEY += 1;
	}

	if (MONEY >= 1000000) {
		MONEY = 999999;
	}

	if (BITS_TO_GAIN == 0) {
		playSound(0, 0x17);
	}
}

void renderBitBox(uint8_t layer)
{
	renderNumber(2, -18, 28, 5, BITS_TO_GAIN, 6 - layer);

	setXYWH(&BIT_TEXT, 52, 28, 24, 12);
	GsSortPoly(&BIT_TEXT, ACTIVE_ORDERING_TABLE, 6 - layer);

	renderString(4, -78, 28, 48, 12, 0, 72, 6 - layer, 0);

	if (BTL_END_BOX_TEXTBUFFER[0]) {
		drawLine2P(0x8e8e8e, -86, 50, 84, 50, 6 - layer, 0);
		drawLine2P(0x121212, -85, 51, 85, 51, 6 - layer, 0);
		BTL_renderBattleEndText(layer);
	}
}

void renderFinalBalance(int32_t layer)
{
	renderNumber(0,
		     UI_BOX_DATA[2].finalPos.x + 58,
		     UI_BOX_DATA[2].finalPos.y + 10,
		     6, MONEY, 6 - layer);

	setXYWH(&BIT_TEXT, UI_BOX_DATA[2].finalPos.x + 140, UI_BOX_DATA[2].finalPos.y + 10, 24, 12);
	GsSortPoly(&BIT_TEXT, ACTIVE_ORDERING_TABLE, 6 - layer);

	renderString(0,
		     UI_BOX_DATA[2].finalPos.x + 10,
		     UI_BOX_DATA[2].finalPos.y + 10,
#if defined(VERSION_JP)
		     36, 12, 0, 84, 6 - layer, 0);
#else
		     36, 12, 0, 72, 6 - layer, 0);
#endif
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
	int16_t screenPos[2];
	RECT finalPos;
	RECT startPos;
	int32_t i;

	SHOULD_SKIP_BIT_COUNTING = 0;
	clearTextArea();

	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] == 0) {
			STAT_BOX_HAS_GAIN[i] = 0;
		} else {
			STAT_BOX_HAS_GAIN[i] = 1;
		}
	}
	for (i = 0; i < 4; i++) {
		if (i < 3) {
			drawString(&MAIN_D_80124C0C[i * 2 * 12], 0, i * 12 * 2);
			drawString(&MAIN_D_80124C0C[(i * 2 + 1) * 12], 0, (i * 2 + 1) * 12);
		}

		if (i == 3) {
#if defined(VERSION_JP)
			drawString(STR_SHOJIKIN, 0, 84);
#endif
			drawString(MAIN_D_80124C54, 0, 240);
		}

		DrawSync(0);
	}

	POST_BATTLE_STATS_TIMER = 100;

	setRECT(&finalPos, -88, -78, 176, 96);

	getEntityScreenPos(ENTITY_TABLE[1], 1, screenPos);

	setRECT(&startPos, screenPos[0] - 5, screenPos[1] - 5, 10, 10);
	createAnimatedUIBox(0, 0, 2, &finalPos, &startPos, tickPostBattleStatsBox,
			    (RenderFunction)renderPostBattleStatsBox);
}

void tickPostBattleStatsBox(void)
{
	int32_t i;

	if (POST_BATTLE_STATS_TIMER > 0) {
		POST_BATTLE_STATS_TIMER--;
	}

	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] != 0) {
			break;
		}
	}

	if ((POLLED_INPUT == CONFIRM_BUTTON) || (POLLED_INPUT == CANCEL_BUTTON)) {
		if (!(POLLED_INPUT_PREVIOUS & CONFIRM_BUTTON) &&
		    !(POLLED_INPUT_PREVIOUS & CANCEL_BUTTON)) {
			SHOULD_SKIP_BIT_COUNTING = 1;
		}
	}

	if (SHOULD_SKIP_BIT_COUNTING != 1) {
		return;
	}

	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] != 0) {
			INITIAL_COMBAT_STATS[0][i] += STATS_GAINS[i];

			if (STATS_GAINS[i] > 0) {
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

	POST_BATTLE_STATS_TIMER = 0;
}

void renderPostBattleStatsBox(int16_t depth)
{
	RECT *box;
	GsBOXF rect;
	int32_t first;
	int16_t y;
	int32_t i;
	POLY_FT4 *prim;

	box = &UI_BOX_DATA[0].finalPos;
	first = 1;
	for (i = 0; i < 6; i++) {
		if (STATS_GAINS[i] == 0) {
			continue;
		}

		if ((POST_BATTLE_STATS_TIMER == 0) &&
		    (STATS_GAINS[i] > 0) &&
		    (first == 1)) {
			playSound(0, 0x16);
			STATS_GAINS[i]--;
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
			prim = (POLY_FT4 *)GsGetWorkBase();
			setEntityTextDigit(prim, 256, 491);
			setRGB0(prim, 0x80, 0x80, 0x80);
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

	drawLine2P(0xfad990, box->x + 122,
		   box->y + 2, box->x + 122,
		   (box->y + box->h) - 3, 6 - depth, 0);
	drawLine2P(0x20202, box->x + 123,
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

		switch (STAT_BOX_HAS_GAIN[i]) {
		case 0:
			rect.r = rect.g = rect.b = 0x78;
			GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE,
				      (uint16_t)(6 - depth));
			rect.r = rect.g = rect.b = 0x28;
			break;
		case 1:
			rect.r = 0x69;
			rect.g = 0xc2;
			rect.b = 0xff;
			GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE,
				      (uint16_t)(6 - depth));
			rect.r = 0;
			rect.g = 0x5a;
			rect.b = 0x96;
			break;
		}

		rect.w = 50;

		drawLine3P(0x20202,
			   box->x + 64, y,
			   box->x + 64, y - 3,
			   box->x + 117, y - 3,
			   6 - depth, 0);
		drawLine3P(0x666666,
			   box->x + 117, y - 2,
			   box->x + 117, y,
			   box->x + 65, y,
			   6 - depth, 0);
		GsSortBoxFill(&rect, ACTIVE_ORDERING_TABLE,
			      (uint16_t)(6 - depth));
	}
}

void removeBattleEndBox(id)
	int16_t id;
{
	if (UI_BOX_DATA[id].state != 0) {
		removeAnimatedUIBox(id, NULL);
	}
}

void initializeBitText(void)
{
	initStringFT4(&BIT_TEXT);

	setRGB0(&BIT_TEXT, 0x80, 0x80, 0x80);
	setUVDataPolyFT4(&BIT_TEXT, 156, 240, 24, 12);
}

void createFinalBalanceBox(void)
{
	RECT finalPos;
	RECT startPos;

	setRECT(&finalPos, -88, -13, 176, 31);
	startPos = UI_BOX_DATA[1].startPos;
	createAnimatedUIBox(2, 1, 0, &finalPos, &startPos, NULL,
			    renderFinalBalance);
}
