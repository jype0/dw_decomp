#include <libetc.h>
#include <libgs.h>
#include <mwinline_n.h>

#include <dw/anim.h>
#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/graphics.h>
#include <dw/item.h>
#include <dw/main.h>
#include <dw/math.h>
#include <dw/params.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/sound_async.h>
#include <dw/types.h>
#include <dw/utils.h>
#include <dw/world_object.h>

#define BTL_FINISHER_TIM	((char *)0x80052ae0)
#define BTL_FINISHER_MODEL	((char *)0x80053800)
#define BTL_CONFUSION_MODEL	((char *)0x80054838)
#define BTL_STUN_MODEL		((char *)0x80054d00)
#define BTL_BUFF_MODEL		((char *)0x80055328)

#define NUM_CONCAVE_SCREENS	18

typedef struct {
	int16_t frame;
	int16_t type;
	uint8_t unused[8];
} FleeBubbleState;

extern uint8_t CURRENT_SCREEN;
extern int32_t VIEWPORT_DISTANCE;
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT GS_ORDERING_TABLE[];
extern PACKET GS_WORK_BASES[];
extern char DRAW_OFFSETS[];
extern uint8_t FLEE_BUBBLE_SCALE[];
extern uint8_t BATTLE_MUSIC[];
extern uint8_t FLEE_CHANCE_TABLE[];
extern uint8_t CONCAVE_SCREENS[NUM_CONCAVE_SCREENS];
extern uint8_t IS_PREDEFINED_BATTLE;
extern int8_t LOAD_EFE_STATE;
extern FleeBubbleState FLEE_BUBBLE_DATA[];

int32_t isInvisible(Entity *entity);
void getEntityTileFromModel(Entity *entity, int8_t *outTileX, int8_t *outTileY);
int32_t isLinearPathBlocked(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
int32_t worldPosToScreenPos(SVECTOR *pos, DVECTOR *out);
void tickPartnerWaypoints(void);
void tickPartnerCollision(void);
int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
void initializeBuffModel(void *model);
void initializeBuffModelObject(void);
void BTL_initializeBattleItemParticles(void);
void BTL_initializeUnk2(void);
void BTL_initializeUnk3(void);
void BTL_initializeFinisherAuraModel(char *tim, char *base);
void BTL_initializePoisonBubble(void);
void BTL_initializeConfusionEffect(char *base);
void BTL_initializeStunEffect(char *base);
char *BTL_initializeEFEEngine(char *base);
void BTL_loadMoveEFE(int16_t *moves, int16_t *effectIds, int8_t *isLoaded);
int32_t isScreenConcave(void);
void loadBattleData(int32_t entityId, int32_t count);
void loadCombatDataTick();
void handleBattleIdle(DigimonEntity *entity, Stats *stats, int32_t flags);
void setFleeBubble(int16_t id, int16_t v);
void renderFleeBubble(int32_t instanceId);
void removeFleeBubble(int32_t id);
int32_t handleBattleStart(int32_t id);

static void *btl_functions[] = {
	startBattleIdleAnimation,
	handleBattleStart,
	removeFleeBubble,
	renderFleeBubble,
	setFleeBubble,
	handleBattleIdle,
	loadCombatDataTick,
	loadBattleData,
	isScreenConcave,
};

int32_t isScreenConcave(void)
{
	int32_t i;

	for (i = 0; i < NUM_CONCAVE_SCREENS; ++i) {
		if (CURRENT_SCREEN == CONCAVE_SCREENS[i]) {
			return 1;
		}
	}

	return 0;
}

void loadCombatDataTick(void)
{
	POLLED_INPUT = PadRead(1);
	ACTIVE_FRAMEBUFFER = GsGetActiveBuff();
	ACTIVE_ORDERING_TABLE = &GS_ORDERING_TABLE[ACTIVE_FRAMEBUFFER];
	GsSetWorkBase(&GS_WORK_BASES[ACTIVE_FRAMEBUFFER * 0x14000]);
	GsClearOt(0, 0, ACTIVE_ORDERING_TABLE);
	tickObjects();
	renderObjects();
	AddPrim((char *)ACTIVE_ORDERING_TABLE->org + 0x80,
		&DRAW_OFFSETS[ACTIVE_FRAMEBUFFER * 0xc]);
	DrawSync(0);
	VSync(3);
	POLLED_INPUT_PREVIOUS = POLLED_INPUT;
	GsSetOrign(DRAWING_OFFSET_X, DRAWING_OFFSET_Y);
	GsSwapDispBuff();
	GsSortClear(0, 0, 0, ACTIVE_ORDERING_TABLE);
	GsDrawOt(ACTIVE_ORDERING_TABLE);
}

void handleBattleIdle(DigimonEntity *entity, Stats *stats, int32_t flags)
{
	DigimonEntity *p;

	p = entity;
	if ((MAIN_D_80134D74 == 0) || (&p->entity != MAIN_D_80134D60)) {
		if ((p->entity.anim.animId != 0x21) &&
		    (p->entity.anim.animId != 0x22)) {
			startBattleIdleAnimation(p, &p->stats, flags);
		}
	}
}

static void setFleeBubble__garbage__(void)
{
	int32_t t0;
	int32_t t1;
	int32_t t2;
	int32_t t3;

	t0 = ((int16_t *)FLEE_BUBBLE_DATA)[0];
	t1 = ((int16_t *)FLEE_BUBBLE_DATA)[1];
	t2 = ((int16_t *)FLEE_BUBBLE_DATA)[2];
	t3 = ((int16_t *)FLEE_BUBBLE_DATA)[3];
	removeFleeBubble(t0);
	removeFleeBubble(t1);
	((int16_t *)FLEE_BUBBLE_DATA)[0] = t0 + 1;
	((int16_t *)FLEE_BUBBLE_DATA)[1] = t1 + 2;
	((int16_t *)FLEE_BUBBLE_DATA)[2] = t2 + 3;
	((int16_t *)FLEE_BUBBLE_DATA)[3] = t3 + 4;
}

void setFleeBubble(int16_t id, int16_t v)
{
	int32_t i;

	i = id - 2;
	FLEE_BUBBLE_DATA[i].frame = 0;
	FLEE_BUBBLE_DATA[i].type = v;
	addObject(0x196, id, NULL, renderFleeBubble);
}

void removeFleeBubble(int32_t id)
{
	removeObject(0x196, id);
}

void startBattleIdleAnimation(DigimonEntity *entity, Stats *stats,
			      int32_t flags)
{
	int32_t f;
	int32_t anim;

	f = 1;
	if (!(flags & 1)) {
		if (stats->current.currentHP > (stats->base.hp / 5)) {
			f = 0;
		}
	}
	if (f != 0) {
		anim = 0x22;
	} else {
		anim = 0x21;
	}
	startAnimation(&entity->entity, (uint8_t)anim);
}

int32_t handleBattleStart(int32_t id)
{
	uint8_t slots[4];
	int8_t tx0;
	int8_t ty0;
	int8_t tx1;
	int8_t ty1;
	int32_t count;
	int32_t i;
	int32_t j;
	int32_t concave;
	int16_t flag;
	int32_t pt;
	int32_t row;
	int32_t chance;
	int32_t hasA;
	int32_t hasB;
	uint8_t partnerType;

	COMBAT_DATA_PTR->player.entityIds[0] = 1;
	COMBAT_DATA_PTR->player.unk4 = 0;
	count = 0;
	MAIN_D_80134D7C[0] = isTriggerSet(1);
	IS_PREDEFINED_BATTLE = readPStat(0xfa);
	if (IS_PREDEFINED_BATTLE == 1) {
		for (i = 0; i < 3; i++) {
			slots[i] = readPStat((i + 0xfb) & 0xff);
			if (slots[i] != 0xff) {
				setFleeBubble(slots[i], 0);
				COMBAT_DATA_PTR->player.entityIds[++count] = slots[i];
			}
		}

		for (i = 2; i < 10; i++) {
			for (j = 0; j < 3; j++) {
				if (slots[j] == i) {
					break;
				}
			}
			if (j != 3) {
				continue;
			}
			if (ENTITY_TABLE[i]->isOnScreen == 0) {
				ENTITY_TABLE[i]->isOnMap = 0;
			}
			if (isInvisible(ENTITY_TABLE[i])) {
				continue;
			}
			setFleeBubble((int16_t)i, 1);
		}

		return count;
	}

	COMBAT_DATA_PTR->player.entityIds[++count] = id;
	concave = isScreenConcave();
	for (i = 2; i < 10; i++) {
		if (ENTITY_TABLE[i] == NULL) {
			continue;
		}
		if (ENTITY_TABLE[i]->isOnMap == 0) {
			continue;
		}
		if (i == id) {
			setFleeBubble((int16_t)i, 0);
			continue;
		}
		if ((NPC_ENTITIES - 2)[i].digimonEntity.entity.isOnScreen == 0) {
			(NPC_ENTITIES - 2)[i].digimonEntity.entity.isOnMap = 0;
			continue;
		}
		if (count == 3) {
			setFleeBubble((int16_t)i, 1);
			continue;
		}
		if (concave != 0) {
			getEntityTileFromModel(ENTITY_TABLE[0], &tx0, &ty0);
			getEntityTileFromModel(ENTITY_TABLE[i], &tx1, &ty1);
			if (isLinearPathBlocked(tx0, ty0, tx1, ty1)) {
				setFleeBubble((int16_t)i, 1);
				continue;
			}
			getEntityTileFromModel(ENTITY_TABLE[1], &tx0, &ty0);
			if (isLinearPathBlocked(tx0, ty0, tx1, ty1)) {
				setFleeBubble((int16_t)i, 1);
				continue;
			}
		}

		chance = random(100);
		hasB = 0;
		hasA = 0;
		for (j = 0; j < INVENTORY.size; j++) {
			if (INVENTORY.types.array[j] == 0x23) {
				hasA = 1;
			}
			if (INVENTORY.types.array[j] == 0x24) {
				hasB = 1;
			}
		}
		if ((hasA == 1) || (hasB == 1)) {
			if (hasB == 1) {
				chance -= 0x32;
			} else {
				chance += 0x14;
			}
		}
		if (MAIN_D_80134D7C[0] != 0) {
			chance = 0;
		}

		flag = 1;
		partnerType = DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].type;
		pt = partnerType;
		if (partnerType != 0xff) {
			j = DIGIMON_DATA[(NPC_ENTITIES - 2)[i].digimonEntity.entity.type].type;
			row = pt;
			row = row - 1;
			if (chance < (&FLEE_CHANCE_TABLE[row * 3])[j - 1]) {
				COMBAT_DATA_PTR->player.entityIds[++count] = i;
				flag = 0;
			}
		} else {
			if (random(100) < 0x46) {
				COMBAT_DATA_PTR->player.entityIds[++count] = i;
				flag = 0;
			}
		}
		setFleeBubble((int16_t)i, flag);
	}

	return count;
}

void loadBattleData(int32_t entityId, int32_t count)
{
	int16_t moveList[18];
	int16_t efeList[18];
	int8_t loadState;
	int32_t i;
	int32_t j;
	int32_t n;
	int32_t slot;
	int32_t vabId;
	int32_t music;
	int32_t type;
	int32_t move;
	DigimonEntity *e;

	ENEMY_COUNT = count;
	if (ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[1]]->type == 0x73) {
		playMusic(0x21, 3);
		music = -1;
	} else if ((entityId >= 2) && (entityId < 10)) {
		if (IS_PREDEFINED_BATTLE == 1) {
			playMusic(0x21, 0);
			music = -1;
		} else {
			type = ENTITY_TABLE[entityId]->type;
			if ((type >= 0x43) && (type < 0x70)) {
				playMusic(0x21,
					  BATTLE_MUSIC[ENTITY_TABLE[entityId]
					  ->type -
					  0x43]);
				music = -1;
			} else {
				playMusic(0x21, 1);
				music = -1;
			}
		}
	} else if (IS_PREDEFINED_BATTLE == 1) {
		playMusic(0x21, 0);
		music = -1;
	} else {
		playMusic(0x21, 1);
		music = -1;
	}
	LOAD_EFE_STATE = music;
	loadDynamicLibrary(BTL_REL, (uint8_t *)&loadState, DW_TRUE, NULL, NULL);
	tickPartnerWaypoints();
	PARTNER_ENTITY.digimonEntity.entity.isOnScreen = 1;
	ENEMY_COUNT = count;
	COMBAT_DATA_PTR->player.currentCommand[0] = 3;
	startAnimation(&TAMER_ENTITY.entity, 1);
	for (i = 2; i < 10; i++) {
		e = (DigimonEntity *)ENTITY_TABLE[i];
		if (isInvisible(&e->entity) == 0) {
			startAnimation(&e->entity, 0x21);
		}
	}
	GAME_STATE = 3;
	if (IS_PREDEFINED_BATTLE != 1) {
		startAnimation(ENTITY_TABLE[1], 0x24);
	}
	while (loadState > 0) {
		if (IS_PREDEFINED_BATTLE != 1) {
			for (i = 1; i <= ENEMY_COUNT; i++) {
				entityLookAtLocation(
						     ENTITY_TABLE[COMBAT_DATA_PTR->player
						     .entityIds[i]],
						     &ENTITY_TABLE[1]->posData->location);
			}
			tickPartnerCollision();
		}
		loadCombatDataTick();
	}
	loadSB();
	for (i = 0, vabId = 4; i <= ENEMY_COUNT; i++, vabId++) {
		e = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		if (i != 0) {
			e->stats.current.vabId = vabId;
			slot = readVBALLSection(vabId, e->entity.type);
		}
	}
	while (isSoundBufferLoading(slot) != 0) {
		if (IS_PREDEFINED_BATTLE != 1) {
			for (i = 1; i <= ENEMY_COUNT; i++) {
				entityLookAtLocation(
						     ENTITY_TABLE[COMBAT_DATA_PTR->player
						     .entityIds[i]],
						     &ENTITY_TABLE[1]->posData->location);
			}
			tickPartnerCollision();
		}
		loadCombatDataTick();
	}
	BTL_initializeBattleItemParticles();
	BTL_initializeUnk3();
	BTL_initializeUnk2();
	BTL_initializeFinisherAuraModel(BTL_FINISHER_TIM,
					BTL_FINISHER_MODEL);
	if (IS_PREDEFINED_BATTLE != 1) {
		tickPartnerCollision();
	}
	loadCombatDataTick();
	BTL_initializePoisonBubble();
	BTL_initializeConfusionEffect(BTL_CONFUSION_MODEL);
	if (IS_PREDEFINED_BATTLE != 1) {
		tickPartnerCollision();
	}
	loadCombatDataTick();
	BTL_initializeStunEffect(BTL_STUN_MODEL);
	if (IS_PREDEFINED_BATTLE != 1) {
		tickPartnerCollision();
	}
	loadCombatDataTick();
	initializeBuffModel(BTL_BUFF_MODEL);
	if (IS_PREDEFINED_BATTLE != 1) {
		tickPartnerCollision();
	}
	loadCombatDataTick();
	initializeBuffModelObject();
	n = 0;
	BTL_initializeEFEEngine((char *)GENERAL_BUFFER_PTR);
	if (IS_PREDEFINED_BATTLE != 1) {
		tickPartnerCollision();
	}
	loadCombatDataTick();
	for (i = 0; i <= ENEMY_COUNT; i++) {
		e = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		e->stats.current.unk1 = -1;
		e->stats.current.efeSubEffect = -1;
		for (j = 0; j < 4; j++) {
			move = e->stats.base.moves[j];
			if (move != 0xff) {
				if (entityGetTechFromAnim(&e->entity, move) == 0xff) {
					e->stats.base.moves[j] = 0xff;
				} else {
					moveList[n++] = DIGIMON_DATA[e->entity.type].moves[e->stats.base.moves[j] - 0x2e] + 0x100;
				}
			}
		}
	}
	moveList[n] = -1;
	BTL_loadMoveEFE(moveList, efeList, &LOAD_EFE_STATE);
	while (LOAD_EFE_STATE > 0) {
		for (i = 2; i < 10; i++) {
			isInvisible(ENTITY_TABLE[i]);
		}
		if (IS_PREDEFINED_BATTLE != 1) {
			for (i = 1; i <= ENEMY_COUNT; i++) {
				entityLookAtLocation(ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]],
						     &ENTITY_TABLE[1]->posData->location);
			}
			tickPartnerCollision();
		}
		loadCombatDataTick();
	}
	n = 0;
	for (i = 0; i <= ENEMY_COUNT; i++) {
		e = (DigimonEntity *)ENTITY_TABLE[COMBAT_DATA_PTR->player.entityIds[i]];
		for (j = 0; j < 4; j++) {
			if (e->stats.base.moves[j] == 0xff) {
				COMBAT_DATA_PTR->fighter[i].effectSlot[j] = -1;
			} else {
				COMBAT_DATA_PTR->fighter[i].effectSlot[j] = efeList[n++];
			}
		}
	}
	handleBattleIdle(&PARTNER_ENTITY.digimonEntity,
			 &PARTNER_ENTITY.digimonEntity.stats, 0);
	GAME_STATE = 2;
}

static void renderFleeBubble__garbage(void)
{
	int32_t t0;
	int32_t t1;
	int32_t t2;
	int32_t t3;
	int32_t t4;
	int32_t t5;
	int32_t t6;
	int32_t t7;
	int32_t t8;

	t0 = ((int16_t *)FLEE_BUBBLE_DATA)[0];
	t1 = ((int16_t *)FLEE_BUBBLE_DATA)[1];
	t2 = ((int16_t *)FLEE_BUBBLE_DATA)[2];
	t3 = ((int16_t *)FLEE_BUBBLE_DATA)[3];
	t4 = ((int16_t *)FLEE_BUBBLE_DATA)[4];
	t5 = ((int16_t *)FLEE_BUBBLE_DATA)[5];
	t6 = ((int16_t *)FLEE_BUBBLE_DATA)[6];
	t7 = ((int16_t *)FLEE_BUBBLE_DATA)[7];
	t8 = ((int16_t *)FLEE_BUBBLE_DATA)[8];
	((int16_t *)FLEE_BUBBLE_DATA)[0] = t0 + 1;
	((int16_t *)FLEE_BUBBLE_DATA)[1] = t1 + 2;
	((int16_t *)FLEE_BUBBLE_DATA)[2] = t2 + 3;
	((int16_t *)FLEE_BUBBLE_DATA)[3] = t3 + 4;
	((int16_t *)FLEE_BUBBLE_DATA)[4] = t4 + 5;
	((int16_t *)FLEE_BUBBLE_DATA)[5] = t5 + 6;
	((int16_t *)FLEE_BUBBLE_DATA)[6] = t6 + 7;
	((int16_t *)FLEE_BUBBLE_DATA)[7] = t7 + 8;
	((int16_t *)FLEE_BUBBLE_DATA)[8] = t8 + 9;
}

void renderFleeBubble(int32_t instanceId)
{
	SVECTOR pos;
	DVECTOR screen;
	GsSPRITE sprite;
	Entity *e;
	VECTOR *loc;
	POLY_FT4 *prim;
	int16_t scale;
	int16_t sc;
	int16_t sx;
	int16_t sy;
	int32_t depth;

	e = ENTITY_TABLE[instanceId];
	loc = &e->posData->location;
	if (FLEE_BUBBLE_DATA[instanceId - 2].frame < 5) {
		pos.vx = loc->vx;
		pos.vy = -DIGIMON_DATA[e->type].height - 0xc8 - ((FLEE_BUBBLE_DATA[instanceId - 2].frame + 1) * 50 / 5);
		pos.vz = loc->vz;
	} else {
		pos.vx = loc->vx;
		pos.vy = -DIGIMON_DATA[e->type].height - 0xfa;
		pos.vz = loc->vz;
	}

	depth = worldPosToScreenPos(&pos, &screen);
	sc = (uint32_t)(VIEWPORT_DISTANCE * 144) / (uint32_t)depth;
	if (FLEE_BUBBLE_DATA[instanceId - 2].frame < 5) {
		sc = sc * (FLEE_BUBBLE_DATA[instanceId - 2].frame + 1) / 5;
	} else {
		sc = sc * FLEE_BUBBLE_SCALE[(FLEE_BUBBLE_DATA[instanceId - 2].frame - 5) % 10] / 100;
	}

	sx = (sy = screen.vx) - (0xa0 - DRAWING_OFFSET_X);
	sy = screen.vy - (0x78 - DRAWING_OFFSET_Y);
	if (sx < -0x8c) {
		sx = -0x8c;
	}
	if (sx >= 0x8d) {
		sx = 0x8c;
	}
	if (sy < -0x64) {
		sy = -0x64;
	}
	if (sy >= 0x65) {
		sy = 0x64;
	}

	sprite.attribute = 0;
	setWH(&sprite, (sc < 0x30) ? 0x18 : 0x17, (sc < 0x30) ? 0x18 : 0x17);
	scale = sc;
	sprite.mx = 0xb;
	sprite.my = 0xc;
	sprite.r = 0x80;
	sprite.g = 0x80;
	sprite.b = 0x80;
	sprite.cx = 0x100;
	sprite.cy = 0x1e7;
	sprite.u = FLEE_BUBBLE_DATA[instanceId - 2].type * 24 + 0xb8;
	sprite.v = 0x80;
	sprite.tpage = getTPage(0, 0, 896, 256);
	sprite.scaley = sprite.scalex = (scale << 12) / sprite.w;
	sprite.x = sx;
	sprite.y = sy;

	copyVector(&pos, loc);
	gte_ldv0(&pos);
	gte_rtps();
	gte_stsxy(&screen);
	screen.vx = (depth = screen.vx) - (0xa0 - DRAWING_OFFSET_X);
	screen.vy = screen.vy - (0x78 - DRAWING_OFFSET_Y);
	sprite.rotate = _atan(-(screen.vy - sprite.y), screen.vx - sprite.x) * 15 * 3 * 8;
	scale = scale * 2 / 3;

	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->tpage = getTPage(0, 0, 896, 256);
	setClut(prim, 0x100, 0x1e7);
	SetSemiTrans(prim, 1);
	setUVWH(prim,
		FLEE_BUBBLE_DATA[instanceId - 2].type * 16 + 0xc8, 0x98,
		(sprite.w == 0x18) ? 0x10 : 0xf,
		(sprite.w == 0x18) ? 0x10 : 0xf);

	setXYWH(prim, sx - scale / 2, sy - scale / 2, scale, scale);

	AddPrim(&ACTIVE_ORDERING_TABLE->org[8], prim);
	prim++;
	GsSetWorkBase((PACKET *)prim);
	GsSortSprite(&sprite, ACTIVE_ORDERING_TABLE, 8);
	FLEE_BUBBLE_DATA[instanceId - 2].frame++;
	if ((LOAD_EFE_STATE == 0) || (FLEE_BUBBLE_DATA[instanceId - 2].frame == 0xc8)) {
		removeFleeBubble(instanceId);
	}
}
