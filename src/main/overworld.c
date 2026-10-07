#include <inline_n.h>

#include <dw/anim.h>
#include <dw/clock.h>
#include <dw/map.h>
#include <dw/model.h>
#include <dw/partner.h>
#include <dw/std.h>
#include <dw/tamer.h>
#include <dw/ui.h>

typedef struct {
	VECTOR waypoints[8];
	int16_t aiSections[8];
	int16_t activeSection;
	int16_t padding;
} MapDigimonPath;

typedef struct {
	int16_t typeId;
	int16_t padding0;
	MapDigimonPath path;
	VECTOR targetLocation;
	int16_t posX;
	int16_t posY;
	int16_t posZ;
	int16_t rotX;
	int16_t rotY;
	int16_t rotZ;
	int16_t trackingRange;
	int16_t targetAngle;
	int16_t ccDiff;
	int16_t cwDiff;
	int8_t followMode;
	int8_t waypointWaitTimer;
	int8_t animation;
	int8_t hasWaypointTarget;
	int8_t lookAtTamerState;
	int8_t stopAnim;
	uint8_t pad[2];
} MapDigimonEntity;

typedef struct {
	int16_t texX;
	int16_t texY;
	int16_t someX;
	int16_t someY;
	int16_t someZ;
	uint8_t width;
	uint8_t height;
	int8_t clut;
	int8_t transparency;
} LocalMapObject;

extern int32_t IS_IN_MENU;
extern int32_t IS_SCRIPT_PAUSED;
extern uint16_t CURRENT_SCRIPT_ID;
extern long LOADED_DIGIMON_MODELS[8];
extern int8_t GAME_STATE;
extern MapDigimonEntity MAP_DIGIMON_TABLE[];
extern int16_t NPC_COLLISION_STATE[];
extern int32_t NPC_IS_WALKING_TOWARDS[];
extern int8_t TALKED_TO_ENTITY;
extern int16_t NINJAMON_EFFECT_X[];
extern int16_t NINJAMON_EFFECT_Y[];
extern int8_t NINJAMON_EFFECT_X_OFFSET[];
extern int8_t NINJAMON_EFFECT_Y_OFFSET[];
extern LocalMapObject LOCAL_MAP_OBJECTS[];
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern int32_t MAP_OBJECT_MOVE_TO_DATA[];
extern int8_t MOVE_OBJECT_DELTA_X[];
extern int8_t MOVE_OBJECT_DELTA_Y[];
extern uint8_t MAP_LAYER_ENABLED;
extern uint8_t CURRENT_SCREEN;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern int16_t CAMERA_X_PREVIOUS;
extern int16_t CAMERA_Y_PREVIOUS;

#if defined(VERSION_JP)
extern char STR_MOVE_NAME_PARTY_TIME[];
extern char STR_MOVE_NAME_PUMMEL_WHACK[];
extern char STR_MOVE_NAME_FIST_OF_THE_BEAST_KING[];
extern char STR_ITEM_DESC_MYSTERY_ITEM[];
#else
extern char STR_MOVE_NAME_TREMAR[];
extern char STR_MOVE_NAME_WAR_CRY[];
extern char STR_MOVE_NAME_COUNTER[];
#endif

int32_t isTriggerSet(int32_t triggerId);
void callScriptSection(uint16_t scriptId, uint32_t scriptSection,
                       uint32_t param);
void scriptNPCStartAnimation(uint8_t scriptId, int8_t animId);

void renderPlayerMenu(void);
void renderMist(void);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY,
                       int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uPos, int32_t vPos,
                      int32_t width, int32_t height);
void buildMapOverlayPrim(POLY_FT4 *prim, LocalMapObjectInstance *inst,
                         LocalMapObject *obj, int16_t arg3, int16_t arg4,
                         int8_t mode);
void buildSnowflakePrim(POLY_FT4 *prim, LocalMapObjectInstance *inst,
                        LocalMapObject *obj);
void NPCEntityTickBattle(int32_t instanceId);
#if defined(VERSION_JP)
void NPCEntityTickWaypointAI(MapDigimonEntity *mapDigimon, Entity *entity);
#else
void NPCEntityTickWaypointAI(MapDigimonEntity *mapDigimon, Entity *entity,
                             int32_t instanceId);
#endif
void tickWaypointWait(MapDigimonEntity *mapDigimon, Entity *entity);
#if defined(VERSION_JP)
void NPCEntityTickWaypointWalk(MapDigimonEntity *mapDigimon, Entity *entity,
                               int32_t animation);
#else
void NPCEntityTickWaypointWalk(MapDigimonEntity *mapDigimon, Entity *entity,
                               int32_t animation, int32_t instanceId);
#endif
int32_t NPCEntityIsInTrackingRect(MapDigimonEntity *mapDigimon, VECTOR *location);
void NPCEntityTickLookingAtTamer(MapDigimonEntity *mapDigimon, Entity *entity,
                                 TamerEntity *tamer);
void loadMapObjects(LocalMapObjectInstance *mapObjects, int16_t *data,
                    int16_t mapId);
void getRotationDifference(PositionData *posData, VECTOR *targetLoc,
                           int16_t *outAngle, int16_t *outCcDiff,
                           int16_t *outCwDiff);
int32_t rotateEntity(SVECTOR *rotation, int16_t *targetAngle, int16_t *ccDiff,
                     int16_t *cwDiff, int16_t speed);
void getModelTile(VECTOR *position, int16_t *outTileX, int16_t *outTileY);
void NPCEntityTickTrackingTamer(MapDigimonEntity *mapDigimon, Entity *entity,
                                TamerEntity *tamer, int32_t instanceId);
void NPCEntityTickTrackingTamer3(MapDigimonEntity *mapDigimon, Entity *entity,
                                 int16_t instanceId);
int32_t NPCEntityIsInTrackingRadius(Entity *entity, Entity *otherEntity,
                                    MapDigimonEntity *mapDigimon);
void NPCEntityTickTrackingTamer2(MapDigimonEntity *mapDigimon, Entity *entity,
                                 TamerEntity *tamer, int32_t instanceId, int32_t animId);
void NPCEntityTickTrackingTamer4(MapDigimonEntity *mapDigimon, Entity *entity,
                                 TamerEntity *tamer, int16_t instanceId);
int32_t entityCheckCollision(Entity *a, Entity *entity, int32_t c, int32_t d);
void removeTriangleMenu(void);
void closeInventoryBoxes(void);
void removeUIBox1(void);
void collisionGrace(Entity *a, Entity *entity, int32_t c, int32_t d);
uint8_t entityIsOffScreen(Entity *entity, int32_t w, int32_t h);
void NPCEntityTick(int32_t instanceId);
void NPCEntityTickOverworld(int32_t instanceId, MapDigimonEntity *mapDigimon);
void scriptUnloadEntity(uint8_t scriptId);
void setLoopCountToOne(uint32_t scriptId);
void loadNPCModel(uint8_t digimonId);
void unloadDigimonModel(uint8_t digimonType);
void setPartnerIdling(void);
int32_t tickRemoveMist(void);
void setActiveAnim(uint8_t scriptId, int8_t animId);
void spawnSpriteAtEntity(int32_t scriptId, int8_t nodeId, int16_t sprite);
void spawnSpriteAtLocation(int16_t x, int16_t y, int16_t z, int16_t sprite,
                           int16_t flag);
void loadMapImage1(u_long *tim);
void loadMapImage2(u_long *tim, int8_t id);
void renderNinjamonEffect(int32_t instanceId);
int32_t randomLimit(int32_t max);
int32_t _atan(int32_t dy, int32_t dx);
void createNinjamonEffect(void);
void getDrawPosition(SVECTOR *worldPos, int16_t *outX, int16_t *outY);
void storeMapObjectPosition(int16_t *outX, int16_t *outY, uint8_t a,
                            int16_t count);
void loadMapObjectPosition(int16_t *xData, int16_t *yData, int16_t startIndex,
                           int16_t count);
void moveMapObjects(uint8_t startIndex, int16_t count, int16_t dx, int16_t dy);
int32_t moveMapObjectsWithLimit(uint8_t startIndex, int16_t count, int16_t dx,
                                int16_t dy, int16_t limitX, int16_t limitY);
void setMapObjectsFlag(int16_t start, int16_t count, int8_t flag);
void resetMapObjectAnimation(uint8_t startIndex, int16_t count);
void clearMapAITable(int32_t index);
void removeMapEntities(void);
void clearMapDigimon(void);
void resetEntityOrigin(uint8_t scriptId);
void loadMapDigimon(int16_t *data, int16_t mapId);
void initializeLoadedNPCModels(void);
int32_t scriptSetDigimon(uint8_t type, uint8_t slot, uint8_t autotalk);
int32_t tickMoveObjectTo(uint8_t objectIndex, uint8_t moveIndex,
                         int8_t steps, int16_t targetX, int16_t targetY);
void setMovementEnabled(int32_t id, int32_t enabled);

static void *overworld_functions[] = {
	setPartnerIdling,
	setMovementEnabled,
	resetEntityOrigin,
	setLoopCountToOne,
	scriptNPCStartAnimation,
	setActiveAnim,
	NPCEntityTickWaypointWalk,
	tickWaypointWait,
	NPCEntityTickTrackingTamer4,
	NPCEntityTickTrackingTamer3,
	rotateEntity,
	getRotationDifference,
	NPCEntityTickTrackingTamer2,
	NPCEntityIsInTrackingRadius,
	NPCEntityTickTrackingTamer,
	NPCEntityTickLookingAtTamer,
	NPCEntityIsInTrackingRect,
	NPCEntityTickWaypointAI,
	NPCEntityTickOverworld,
	clearMapAITable,
	removeMapEntities,
	clearMapDigimon,
	unloadDigimonModel,
	scriptUnloadEntity,
	NPCEntityTick,
	scriptSetDigimon,
	loadNPCModel,
	loadMapDigimon,
	tickRemoveMist,
	resetMapObjectAnimation,
	spawnSpriteAtEntity,
	spawnSpriteAtLocation,
	getDrawPosition,
	setMapObjectsFlag,
	moveMapObjects,
	tickMoveObjectTo,
	moveMapObjectsWithLimit,
	loadMapObjectPosition,
	renderNinjamonEffect,
	storeMapObjectPosition,
	createNinjamonEffect,
	buildMapOverlayPrim,
	buildSnowflakePrim,
	renderMist,
	renderMapOverlays,
	calcMapObjectOrder,
	loadMapImage2,
	loadMapImage1,
	loadMapObjects,
	clearMapObjects,
};

// clang-format off
int16_t MIST_X_OFFSETS[2] = {
	0xff60, 0x00a0,
};
int16_t MAIN_D_80134228[2] = {
	0x00a0, 0xff60,
};
int16_t MIST_Y_OFFSETS[2] = {
	0xff88, 0x0078,
};
int16_t MIST_CLUT_Y[2] = {
	0x0050, 0x0010,
};
// clang-format on

int16_t MAP_OBJECT_INSTANCE_COUNT;
int8_t NINJAMON_FX_COUNTER;
int8_t NPC_ACTIVE_ANIM[8];

static void *overworld_sbss_order[] = {
	NPC_ACTIVE_ANIM,
	&NINJAMON_FX_COUNTER,
	&MAP_OBJECT_INSTANCE_COUNT,
};

void clearMapObjects(LocalMapObjectInstance *instances)
{
	int32_t i;
	int32_t j;

	for (i = 0; i < 188; i++) {
		for (j = 0; j < 8; j++) {
			instances->animSprites[j] = -1;
		}
		for (j = 0; j < 8; j++) {
			instances->animTimes[j] = 0;
		}
		instances->orderValue = 0;
		instances->timer = 0;
		instances->currentFrame = -1;
		instances->flag = 1;
		instances++;
	}
}

void loadMapObjects(LocalMapObjectInstance *mapObjects, int16_t *data,
                    int16_t mapId)
{
	LocalMapObjectInstance *obj;
	int32_t i;
	int32_t k;
	int32_t j;
	int16_t count;

	count = *data++;
	for (i = 0; i < count; i++) {
		LOCAL_MAP_OBJECTS[i].texX = *data++;
		LOCAL_MAP_OBJECTS[i].texY = *data++;
		LOCAL_MAP_OBJECTS[i].width = *data++;
		LOCAL_MAP_OBJECTS[i].height = *data++;
		LOCAL_MAP_OBJECTS[i].someX = *data++;
		LOCAL_MAP_OBJECTS[i].someY = *data++;
		LOCAL_MAP_OBJECTS[i].someZ = *data++;
		LOCAL_MAP_OBJECTS[i].clut = *data++;
		LOCAL_MAP_OBJECTS[i].transparency = *data++;
	}
	obj = mapObjects;
	MAP_OBJECT_INSTANCE_COUNT = *data++;
	for (k = 0; k < MAP_OBJECT_INSTANCE_COUNT; k++) {
		for (j = 0; j < 8; j++) {
			mapObjects->animSprites[j] = *data++;
		}
		for (j = 0; j < 8; j++) {
			mapObjects->animTimes[j] = *data++;
		}
		mapObjects->x = *data++;
		mapObjects->y = *data++;
		mapObjects->flag = *data++;
		if (((mapId >= 0x58 && mapId < 0x61) ||
		     (mapId >= 0x84 && mapId < 0x88)) &&
		    k < 0x23) {
			if (k >= 0x14) {
				mapObjects->x = randomLimit(320);
			}
			mapObjects->y = randomLimit(240);
			mapObjects->flag |= 0x80;
		}
		mapObjects++;
	}
	for (k = 0; k < 10; k++) {
		MAP_OBJECT_MOVE_TO_DATA[k] = 0;
	}
}

void loadMapImage1(u_long *tim)
{
	TIM_IMAGE image;

	OpenTIM(tim);
	ReadTIM(&image);
	LoadImage(image.prect, image.paddr);
	DrawSync(0);
	if (image.crect->y != 0x1e0) {
		LoadImage(image.crect, image.caddr);
		DrawSync(0);
	}
}

void loadMapImage2(u_long *tim, int8_t id)
{
	TIM_IMAGE image;
	RECT rect;
	u_long *caddr;
	int32_t i;

	OpenTIM(tim);
	ReadTIM(&image);
	LoadImage(image.prect, image.paddr);
	DrawSync(0);
	if (id != 0) {
		return;
	}

	caddr = image.caddr;
	for (i = 0; i < image.crect->h; i++) {
		setRECT(&rect, i * 16, 486, 16, 1);
		LoadImage(&rect, caddr);
		caddr += 8;
	}
}

void calcMapObjectOrder(LocalMapObjectInstance *instances)
{
	SVECTOR worldPos;
	SVECTOR screen;
	int32_t depth;
	int32_t i;
	int32_t j;
	int16_t val;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	for (i = 0; i < MAP_OBJECT_INSTANCE_COUNT; i++) {
		for (j = 0; j < 8; j++) {
			val = instances->animSprites[j];
			if (val == -1) {
				continue;
			}
			if (val == -2) {
				continue;
			}
			worldPos.vx = LOCAL_MAP_OBJECTS[val].someX;
			worldPos.vy = LOCAL_MAP_OBJECTS[val].someY;
			worldPos.vz = LOCAL_MAP_OBJECTS[val].someZ;
			break;
		}
		gte_ldv0(&worldPos);
		gte_rtps();
		gte_stsxy(&screen);
		gte_stszotz(&depth);
		depth = depth >> 2;
		if (depth < 0x23) {
			depth = 0x23;
		}
		if (depth >= 0x1000) {
			depth = 0xff6;
		}
		if (LOCAL_MAP_OBJECTS[val].someY == 0x2710) {
			depth = 0xffe;
		}
		instances->orderValue = depth;
		if (instances->flag & 0x80) {
			instances->orderValue = 0x1e;
		}
		instances->currentFrame = 0;
		instances++;
	}
}

void renderMapOverlays(LocalMapObjectInstance *instances, int16_t screenX,
                       int16_t screenY)
{
	POLY_FT4 *prim;
	LocalMapObject *obj;
	GsOT_TAG *org;
	int32_t i;
	int16_t startFrame;

	if (!MAP_LAYER_ENABLED) {
		return;
	}

	if ((CURRENT_SCREEN >= 115 && CURRENT_SCREEN < 120) ||
	    CURRENT_SCREEN == 121 || CURRENT_SCREEN == 163 ||
	    CURRENT_SCREEN == 220) {
		renderMist();
	}

	org = ACTIVE_ORDERING_TABLE->org;
	for (i = 0; i < 188; i++) {
		startFrame = instances->currentFrame;
		if (instances->animSprites[startFrame] == -1 || startFrame == -1) {
			instances++;
			continue;
		}

		if (instances->animSprites[startFrame] != -2 &&
		    instances->flag != 1) {
			obj = &LOCAL_MAP_OBJECTS[instances->animSprites[startFrame]];
			if (((screenX - 40 < instances->x + obj->width) &&
			     (instances->x < screenX + 360) &&
			     (screenY - 60 < instances->y + obj->height) &&
			     (instances->y < screenY + 300)) ||
			    (instances->flag & 0x80) ||
			    (instances->orderValue < 20)) {
				prim = (POLY_FT4 *)GsGetWorkBase();
				SetPolyFT4(prim);
				if (obj->transparency == 4) {
					SetSemiTrans(prim, 0);
				} else {
					SetSemiTrans(prim, 1);
				}

				if (instances->flag & 0x80) {
					buildSnowflakePrim(prim, instances, obj);
				} else if (instances->orderValue < 20) {
					buildMapOverlayPrim(prim, instances, obj,
					                    screenX, screenY, 1);
				} else {
					buildMapOverlayPrim(prim, instances, obj,
					                    screenX, screenY, 0);
				}

				AddPrim(&org[instances->orderValue], prim);
				prim++;
				GsSetWorkBase((PACKET *)prim);
			}
			instances->timer++;
			if (instances->timer == instances->animTimes[startFrame]) {
				instances->currentFrame++;
				instances->timer = 0;
				if (instances->animSprites[instances->currentFrame] == -1 ||
				    instances->currentFrame >= 8) {
					instances->currentFrame = 0;
				}
			}
		} else {
			instances->timer++;
			if (instances->timer == instances->animTimes[startFrame]) {
				instances->currentFrame++;
				instances->timer = 0;
				if (instances->animSprites[instances->currentFrame] == -1 ||
				    instances->currentFrame >= 8) {
					instances->currentFrame = 0;
				}
			}
		}
		instances++;
	}
}

void renderMist(void)
{
	POLY_FT4 *prim;
	int16_t cameraDeltaX;
	int16_t cameraDeltaY;
	int32_t i;

	if (((CURRENT_SCREEN != 0xa3) && (CURRENT_SCREEN != 0xdc)) ||
	    (isTriggerSet(0x155) != 1)) {
		if ((CURRENT_SCREEN != 0xdc) && (isTriggerSet(0x94) == 1)) {
			MIST_CLUT_Y[0] = 0xc0;
			MIST_CLUT_Y[1] = 0x80;
		}

		cameraDeltaX = CAMERA_X_PREVIOUS - (int16_t)MAP_TILE_DATA.cameraX;
		cameraDeltaY = CAMERA_Y_PREVIOUS - (int16_t)MAP_TILE_DATA.cameraY;
		MIST_X_OFFSETS[0] += cameraDeltaX;
		if (PLAYTIME_FRAMES % 2 == 0) {
			MIST_X_OFFSETS[0]--;
		}
		if (MIST_X_OFFSETS[0] >= 0xa0) {
			MIST_X_OFFSETS[0] -= 0x280;
		}
		if (MIST_X_OFFSETS[0] < -0x1df) {
			MIST_X_OFFSETS[0] += 0x280;
		}

		MIST_X_OFFSETS[1] = MIST_X_OFFSETS[0] + 0x140;
		if (MIST_X_OFFSETS[1] >= 0xa0) {
			MIST_X_OFFSETS[1] -= 0x280;
		}
		if (MIST_X_OFFSETS[1] < -0x1df) {
			MIST_X_OFFSETS[1] += 0x280;
		}

		MAIN_D_80134228[0] += (int16_t)(cameraDeltaX * 12 / 10);
		if (PLAYTIME_FRAMES % 2 == 0) {
			MAIN_D_80134228[0]++;
		}
		if (MAIN_D_80134228[0] >= 0x1e0) {
			MAIN_D_80134228[0] -= 0x280;
		}
		if (MAIN_D_80134228[0] < -0x9f) {
			MAIN_D_80134228[0] += 0x280;
		}

		MAIN_D_80134228[1] = MAIN_D_80134228[0] + 0x140;
		if (MAIN_D_80134228[1] >= 0x1e0) {
			MAIN_D_80134228[1] -= 0x280;
		}
		if (MAIN_D_80134228[1] < -0x9f) {
			MAIN_D_80134228[1] += 0x280;
		}
		if (MAIN_D_80134228[1] >= 0x1e0) {
			MAIN_D_80134228[1] -= 0x280;
		}
		if (MAIN_D_80134228[1] < -0x9f) {
			MAIN_D_80134228[1] += 0x280;
		}

		MIST_Y_OFFSETS[0] += cameraDeltaY;
		if (MIST_Y_OFFSETS[0] >= 0x78) {
			MIST_Y_OFFSETS[0] -= 0x1e0;
		}
		if (MIST_Y_OFFSETS[0] < -0x167) {
			MIST_Y_OFFSETS[0] += 0x1e0;
		}
		MIST_Y_OFFSETS[1] = MIST_Y_OFFSETS[0] + 0xf0;
		if (MIST_Y_OFFSETS[1] >= 0x78) {
			MIST_Y_OFFSETS[1] -= 0x1e0;
		}
		if (MIST_Y_OFFSETS[1] < -0x167) {
			MIST_Y_OFFSETS[1] += 0x1e0;
		}

		for (i = 0; i < 8; i++) {
			prim = (POLY_FT4 *)GsGetWorkBase();
			SetPolyFT4(prim);
			SetSemiTrans(prim, 1);
			if (i < 4) {
				prim->tpage = GetTPage(0, 3, 0x2c0, 0);
				prim->clut = GetClut(0, 0x1e6);
				setPosDataPolyFT4(prim, MIST_X_OFFSETS[i % 2],
				                  MIST_Y_OFFSETS[i / 2], 0x140,
				                  0xf0);
				setRGB0(prim, 0x96, 0x96, 0x96);
			} else {
				prim->tpage = GetTPage(0, 1, 0x2c0, 0);
				if (CURRENT_SCREEN == 0xa3 || CURRENT_SCREEN == 0xdc) {
					prim->clut = GetClut(0, 0x1e6);
				} else if (CURRENT_SCREEN == 0x77) {
					prim->clut = GetClut(MIST_CLUT_Y[0],
					                     0x1e6);
				} else {
					prim->clut = GetClut(MIST_CLUT_Y[1],
					                     0x1e6);
				}
				prim->x0 = MAIN_D_80134228[i % 2];
				prim->x1 = MAIN_D_80134228[i % 2] - 0x140;
				prim->x2 = MAIN_D_80134228[i % 2];
				prim->x3 = MAIN_D_80134228[i % 2] - 0x140;
				prim->y0 = MIST_Y_OFFSETS[(i - 4) / 2];
				prim->y1 = MIST_Y_OFFSETS[(i - 4) / 2];
				prim->y2 = MIST_Y_OFFSETS[(i - 4) / 2] + 0xf0;
				prim->y3 = MIST_Y_OFFSETS[(i - 4) / 2] + 0xf0;
				setRGB0(prim, 0x50, 0x50, 0x50);
			}
			setUVDataPolyFT4(prim, 0, 0, 0xff, 0xc8);
			AddPrim(&ACTIVE_ORDERING_TABLE->org[20], prim);
			prim++;
			GsSetWorkBase((PACKET *)prim);
		}
	}
}

void buildSnowflakePrim(POLY_FT4 *prim, LocalMapObjectInstance *inst,
                        LocalMapObject *obj)
{
	uint8_t randomRange;
	uint8_t fallSpeed;
	int8_t horizontalMovement;
	int8_t chance;

	if ((inst->animSprites[0] == 0) || (inst->animSprites[0] == 3) ||
	    (inst->animSprites[0] == 4)) {
		randomRange = 6;
		fallSpeed = 3;
	}
	if ((inst->animSprites[0] == 1) || (inst->animSprites[0] == 5) ||
	    (inst->animSprites[0] == 6)) {
		randomRange = 2;
		fallSpeed = 1;
	}
	if (inst->animSprites[0] == 2) {
		randomRange = 4;
		fallSpeed = 10;
	}

	if ((horizontalMovement = randomLimit(randomRange)) > (randomRange / 2)) {
		horizontalMovement = -(horizontalMovement % (randomRange / 2));
	}

	if ((inst->animSprites[0] == 0) || (inst->animSprites[0] == 3) ||
	    (inst->animSprites[0] == 4)) {
		if ((CURRENT_FRAME % 3) == 0) {
			inst->x += horizontalMovement;
		}
		if (inst->x >= 0x141) {
			inst->x = 0;
		}
		if (inst->x < 0) {
			inst->x = 0x140;
		}
	}

	setPosDataPolyFT4(prim, inst->x - 0xa0 + horizontalMovement,
	                  inst->y - 0x78, obj->width, obj->height);
	setRGB0(prim, 0x80, 0x80, 0x80);
	setUVDataPolyFT4(prim, obj->texX % 256, obj->texY % 256,
	                 obj->width - 1, obj->height - 1);
	prim->tpage = GetTPage(0, obj->transparency,
	                       (obj->texX / 256 << 6) + 0x180, 0);
	prim->clut = GetClut(obj->clut * 16, 0x1e6);

	inst->y += fallSpeed;
	if ((inst->y >= 0x83) && (inst->animSprites[0] != 2)) {
		chance = randomLimit(10);
		if ((chance < 2) && (inst->currentFrame == 0)) {
			inst->currentFrame++;
			inst->timer = 0;
		}
		if ((inst->currentFrame >= 2) &&
		    (inst->animSprites[inst->currentFrame + 1] == -1)) {
			inst->currentFrame = 0;
			inst->y = 0;
		}
	}
	if (inst->y >= 0xf1) {
		inst->y = 0;
	}
}

void buildMapOverlayPrim(POLY_FT4 *prim, LocalMapObjectInstance *inst,
                         LocalMapObject *obj, int16_t arg3, int16_t arg4,
                         int8_t mode)
{
	if (mode == 0) {
		setPosDataPolyFT4(prim,
		                  ((inst->x - 160) - ((int16_t)arg3 - (160 - DRAWING_OFFSET_X))),
		                  ((inst->y - 120) - ((int16_t)arg4 - (120 - DRAWING_OFFSET_Y))),
		                  obj->width, obj->height);
	} else {
		setPosDataPolyFT4(prim, inst->x, inst->y, obj->width,
		                  obj->height);
	}

	setRGB0(prim, 128, 128, 128);

	if (((obj->texX % 256) + obj->width) < 256) {
		if (((obj->texY % 256) + obj->height) < 256) {
			goto fit;
		}
	}
	setUVDataPolyFT4(prim, obj->texX % 256, obj->texY % 256,
	                 (obj->width - 1), (obj->height - 1));
	goto clut;

fit:
	setUVDataPolyFT4(prim, obj->texX % 256, obj->texY % 256, obj->width,
	                 obj->height);

clut:
	if (obj->clut == -1) {
		prim->tpage = GetTPage(1, obj->transparency,
		                       ((obj->texX / 256) << 7) + 384, 0);
		prim->clut = GetClut(0, 480);
	} else if (obj->clut < 16) {
		prim->tpage = GetTPage(0, obj->transparency,
		                       ((obj->texX / 256) << 6) + 384, 0);
		prim->clut = GetClut(obj->clut << 4, 486);
	} else {
		prim->tpage = GetTPage(1, obj->transparency,
		                       ((obj->texX / 256) << 7) + 384, 0);
		prim->clut = GetClut(0, obj->clut + 468);
	}
}

void createNinjamonEffect(void)
{
	int32_t i;

	NINJAMON_FX_COUNTER = 0;
	storeMapObjectPosition(NINJAMON_EFFECT_X, NINJAMON_EFFECT_Y, 0, 0x29);
	for (i = 0; i < 0x29; i++) {
		NINJAMON_EFFECT_X_OFFSET[i] = randomLimit(10) + 12;
		NINJAMON_EFFECT_Y_OFFSET[i] = randomLimit(10) + 3;
	}
	addObject(0xfba, 0, NULL, renderNinjamonEffect);
}

void storeMapObjectPosition(int16_t *outX, int16_t *outY, uint8_t startIndex,
                            int16_t count)
{
	int32_t i;

	for (i = 0; i < count; i++) {
		*outX++ = MAP_TILE_DATA.objects[startIndex + i].x;
		*outY++ = MAP_TILE_DATA.objects[startIndex + i].y;
	}
}

void renderNinjamonEffect(int32_t instanceId)
{
	LocalMapObjectInstance *data;
	int32_t i;

	data = MAP_TILE_DATA.objects;
	for (i = 0; i < 0x29; i++) {
		data->x += NINJAMON_EFFECT_X_OFFSET[i];
		data->y += NINJAMON_EFFECT_Y_OFFSET[i];
		if (data->x > 0xa0) {
			data->x = NINJAMON_EFFECT_X[i];
			data->y = NINJAMON_EFFECT_Y[i];
			NINJAMON_EFFECT_X_OFFSET[i] = randomLimit(10) + 12;
			NINJAMON_EFFECT_Y_OFFSET[i] = randomLimit(10) + 3;
		}
		data->orderValue = 10;
		data++;
	}
	NINJAMON_FX_COUNTER++;
	if (NINJAMON_FX_COUNTER >= 0x3c) {
		data = MAP_TILE_DATA.objects;
		i = 0;
		while (i < 0x29) {
			data->flag = 1;
			i++;
			data++;
		}
		removeObject(0xfba, 0);
	}
}

void loadMapObjectPosition(int16_t *xData, int16_t *yData, int16_t startIndex,
                           int16_t count)
{
	int32_t i;

	for (i = 0; i < count; i++) {
		MAP_TILE_DATA.objects[startIndex + i].x = *xData++;
		MAP_TILE_DATA.objects[startIndex + i].y = *yData++;
		MAP_TILE_DATA.objects[startIndex + i].flag &= ~0x10;
	}
}

int32_t moveMapObjectsWithLimit(uint8_t startIndex, int16_t count, int16_t dx,
                                int16_t dy, int16_t limitX, int16_t limitY)
{
	int32_t i;

	for (i = 0; i < count; i++) {
		MAP_TILE_DATA.objects[startIndex + i].x += dx;
		MAP_TILE_DATA.objects[startIndex + i].y += dy;
	}
	if (dx > 0) {
		if (MAP_TILE_DATA.objects[startIndex].x >= limitX) {
			return 1;
		}
	} else if (dx < 0) {
		if (MAP_TILE_DATA.objects[startIndex].x <= limitX) {
			return 1;
		}
	}
	if (dy > 0) {
		if (MAP_TILE_DATA.objects[startIndex].y >= limitY) {
			return 1;
		}
	} else if (dy < 0) {
		if (MAP_TILE_DATA.objects[startIndex].y <= limitY) {
			return 1;
		}
	}
	return 0;
}

int32_t tickMoveObjectTo(uint8_t objectIndex, uint8_t moveIndex,
                         int8_t steps, int16_t targetX, int16_t targetY)
{
	if (MAP_OBJECT_MOVE_TO_DATA[moveIndex] == 0) {
		MOVE_OBJECT_DELTA_X[moveIndex] =
			(targetX - MAP_TILE_DATA.objects[objectIndex].x) / steps;
		MOVE_OBJECT_DELTA_Y[moveIndex] =
			(targetY - MAP_TILE_DATA.objects[objectIndex].y) / steps;
		MAP_OBJECT_MOVE_TO_DATA[moveIndex] = 1;
	}

	MAP_TILE_DATA.objects[objectIndex].x += MOVE_OBJECT_DELTA_X[moveIndex];
	MAP_TILE_DATA.objects[objectIndex].y += MOVE_OBJECT_DELTA_Y[moveIndex];

	if (MOVE_OBJECT_DELTA_X[moveIndex] > 0) {
		if (MAP_TILE_DATA.objects[objectIndex].x >= targetX) {
			MAP_TILE_DATA.objects[objectIndex].x = targetX;
		}
	} else if (MOVE_OBJECT_DELTA_X[moveIndex] < 0) {
		if (MAP_TILE_DATA.objects[objectIndex].x <= targetX) {
			MAP_TILE_DATA.objects[objectIndex].x = targetX;
		}
	} else {
		MAP_TILE_DATA.objects[objectIndex].x = targetX;
	}

	if (MOVE_OBJECT_DELTA_Y[moveIndex] > 0) {
		if (MAP_TILE_DATA.objects[objectIndex].y >= targetY) {
			MAP_TILE_DATA.objects[objectIndex].y = targetY;
		}
	} else if (MOVE_OBJECT_DELTA_Y[moveIndex] < 0) {
		if (MAP_TILE_DATA.objects[objectIndex].y <= targetY) {
			MAP_TILE_DATA.objects[objectIndex].y = targetY;
		}
	} else {
		MAP_TILE_DATA.objects[objectIndex].y = targetY;
	}

	if (MAP_TILE_DATA.objects[objectIndex].x == targetX &&
	    MAP_TILE_DATA.objects[objectIndex].y == targetY) {
		MAP_OBJECT_MOVE_TO_DATA[moveIndex] = 0;
		return 1;
	}
	return 0;
}

void moveMapObjects(uint8_t startIndex, int16_t count, int16_t dx, int16_t dy)
{
	int32_t i;

	for (i = 0; i < count; i++) {
		MAP_TILE_DATA.objects[startIndex + i].x += dx;
		MAP_TILE_DATA.objects[startIndex + i].y += dy;
	}
}

void setMapObjectsFlag(int16_t start, int16_t count, int8_t flag)
{
	int32_t i;

	for (i = 0; i < count; i++) {
		MAP_TILE_DATA.objects[start + i].flag = flag;
	}
}

void getDrawPosition(SVECTOR *worldPos, int16_t *outX, int16_t *outY)
{
	SVECTOR screen;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	gte_ldv0(worldPos);
	gte_rtps();
	gte_stsxy(&screen);
	*outX = DRAWING_OFFSET_X + (screen.vx + (int16_t)MAP_TILE_DATA.cameraX);
	*outY = DRAWING_OFFSET_Y + (screen.vy + MAP_TILE_DATA.cameraY);
}

void spawnSpriteAtLocation(int16_t x, int16_t y, int16_t z, int16_t sprite,
                           int16_t count)
{
	SVECTOR position;
	int16_t positionsX[30];
	int16_t positionsY[30];
	int16_t screenX;
	int16_t screenY;
	int32_t j;
	int32_t i;

	position.vx = x;
	position.vy = y;
	position.vz = z;
	getDrawPosition(&position, &screenX, &screenY);

	for (i = 0; i < count; i++) {
		for (j = 0; j < 8; j++) {
			if (MAP_TILE_DATA.objects[(long)sprite + i]
			            .animSprites[j] != -2) {
				break;
			}
		}
		positionsX[i] =
			screenX -
			LOCAL_MAP_OBJECTS[MAP_TILE_DATA.objects[(long)sprite + i]
		                                  .animSprites[j]]
					.width /
				2;
		positionsY[i] =
			screenY -
			LOCAL_MAP_OBJECTS[MAP_TILE_DATA.objects[(long)sprite + i]
		                                  .animSprites[j]]
					.height /
				2;
	}

	loadMapObjectPosition(positionsX, positionsY, sprite, count);
}

void spawnSpriteAtEntity(int32_t scriptId, int8_t nodeId, int16_t sprite)
{
	Entity *entity;
	MATRIX *m;

	entity = getEntityFromScriptId((uint8_t *)&scriptId);
	m = &entity->posData[nodeId].posMatrix.workm;
	spawnSpriteAtLocation(m->t[0], m->t[1], m->t[2], sprite, 1);
}

void resetMapObjectAnimation(uint8_t startIndex, int16_t count)
{
	int32_t i;

	for (i = 0; i < count; i++) {
		MAP_TILE_DATA.objects[startIndex + i].timer = 0;
		MAP_TILE_DATA.objects[startIndex + i].currentFrame = 0;
	}
}

int32_t tickRemoveMist(void)
{
	MIST_CLUT_Y[0] += 0x10;
	MIST_CLUT_Y[1] += 0x10;

	if (MIST_CLUT_Y[0] >= 0xc0) {
		MIST_CLUT_Y[0] = 0xc0;
		MIST_CLUT_Y[1] = 0x80;
		return 1;
	}

	return 0;
}

void loadMapDigimon(int16_t *data, int16_t mapId)
{
	Stats *stats;
	int16_t digimonCount;
	int16_t count;
	MapDigimonPath *path;
	int32_t i;
	int32_t j;

	digimonCount = *data++;
	if (MAP_ENTRIES[mapId].flags & 0x80) {
		for (i = 0; i < digimonCount; i++) {
			MAP_DIGIMON_TABLE[i].typeId = *data++;
			MAP_DIGIMON_TABLE[i].followMode = *data++;
			MAP_DIGIMON_TABLE[i].posX = *data++;
			MAP_DIGIMON_TABLE[i].posY = *data++;
			MAP_DIGIMON_TABLE[i].posZ = *data++;
			MAP_DIGIMON_TABLE[i].rotX = *data++;
			MAP_DIGIMON_TABLE[i].rotY = *data++;
			MAP_DIGIMON_TABLE[i].rotZ = *data++;
			MAP_DIGIMON_TABLE[i].trackingRange = *data++;
			NPC_ENTITIES[i].unk2 = *data++;
			NPC_ENTITIES[i].scriptId = *data++;
			stats = &NPC_ENTITIES[i].digimonEntity.stats;
			stats->base.hp = *data++;
			stats->base.mp = *data++;
			stats->current.currentHP = *data++;
			stats->current.currentMP = *data++;
			stats->base.off = *data++;
			stats->base.def = *data++;
			stats->base.speed = *data++;
			stats->base.brain = *data++;
			NPC_ENTITIES[i].bits = *data++;
			stats->current.chargeMode = *data++;
			NPC_ENTITIES[i].unk1 = *data++;
			stats->base.moves[0] = *data++;
			stats->base.moves[1] = *data++;
			stats->base.moves[2] = *data++;
			stats->base.moves[3] = *data++;
			stats->base.movesPrio[0] = *data++;
			stats->base.movesPrio[1] = *data++;
			stats->base.movesPrio[2] = *data++;
			stats->base.movesPrio[3] = *data++;
			NPC_ENTITIES[i].flee.vx = *data++;
			NPC_ENTITIES[i].flee.vy = *data++;
			NPC_ENTITIES[i].flee.vz = *data++;
			MAP_DIGIMON_TABLE[i].animation = 0;
			path = &MAP_DIGIMON_TABLE[i].path;
			count = *data++;
			for (j = 0; j < 8; j++) {
				path->aiSections[j] = *data++;
			}
			for (j = 0; j < count; j++) {
				path->waypoints[j].vx = *data++;
				path->waypoints[j].vy = *data++;
				path->waypoints[j].vz = *data++;
			}
			path->activeSection = 0;
		}
	}
}

void loadNPCModel(uint8_t digimonId)
{
	thunkLoadMMD(digimonId, 0);
}

int32_t scriptSetDigimon(uint8_t type, uint8_t slot, uint8_t autotalk)
{
	if (type != MAP_DIGIMON_TABLE[slot].typeId) {
		return 0;
	}
	if (ENTITY_TABLE[slot + 2] != NULL) {
		removeEntity(ENTITY_TABLE[slot + 2]->type, slot + 2);
	}
	ENTITY_TABLE[slot + 2] = &NPC_ENTITIES[slot].digimonEntity.entity;
	initializeDigimonObject(type, slot + 2, NPCEntityTick);
	setEntityPosition(slot + 2, MAP_DIGIMON_TABLE[slot].posX,
	                  MAP_DIGIMON_TABLE[slot].posY,
	                  MAP_DIGIMON_TABLE[slot].posZ);
	setEntityRotation(slot + 2, MAP_DIGIMON_TABLE[slot].rotX,
	                  MAP_DIGIMON_TABLE[slot].rotY,
	                  MAP_DIGIMON_TABLE[slot].rotZ);
	setupEntityMatrix(slot + 2);
	startAnimation(ENTITY_TABLE[slot + 2],
	               MAP_DIGIMON_TABLE[slot].animation);
	NPC_ENTITIES[slot].autotalk = autotalk;
	NPC_IS_WALKING_TOWARDS[slot] = 0;
	ENTITY_TABLE[slot + 2]->isOnMap = 1;
	ENTITY_TABLE[slot + 2]->isOnScreen =
		entityIsOffScreen(ENTITY_TABLE[slot + 2], 0x140, 0xf0) ^ 1;
	return 1;
}

void NPCEntityTick(int32_t instanceId)
{
	if (ENTITY_TABLE[instanceId]->isOnMap == 0) {
		return;
	}

	switch (GAME_STATE) {
	case 0:
		NPCEntityTickOverworld(instanceId, &MAP_DIGIMON_TABLE[instanceId - 2]);
		break;
	case 1:
	case 2:
	case 3:
		NPCEntityTickBattle(instanceId);
		break;
	case 4:
	case 5:
		STD_tickNPCTournament(instanceId);
		break;
	}
}

void scriptUnloadEntity(uint8_t scriptId)
{
	Entity *entity;

	entity = getEntityFromScriptId(&scriptId);
	entity->isOnMap = 0;
	removeEntity(entity->type, scriptId);
}

void unloadDigimonModel(uint8_t digimonType)
{
	thunkUnloadModel(digimonType, 0);
}

void clearMapDigimon(void)
{
	int32_t i;
	int32_t j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			MAP_DIGIMON_TABLE[i].path.aiSections[j] = -1;
			MAP_DIGIMON_TABLE[i].path.waypoints[j].vx = 0;
			MAP_DIGIMON_TABLE[i].path.waypoints[j].vy = 0;
			MAP_DIGIMON_TABLE[i].path.waypoints[j].vz = 0;
		}
		MAP_DIGIMON_TABLE[i].path.activeSection = 0;
		ENTITY_TABLE[i + 2] = NULL;
		ENTITY_TABLE[i + 2]->isOnMap = 0;
		MAP_DIGIMON_TABLE[i].typeId = -1;
		MAP_DIGIMON_TABLE[i].waypointWaitTimer = 0;
		MAP_DIGIMON_TABLE[i].targetAngle = MAP_DIGIMON_TABLE[i].ccDiff =
			MAP_DIGIMON_TABLE[i].cwDiff = 0;
		MAP_DIGIMON_TABLE[i].lookAtTamerState =
			MAP_DIGIMON_TABLE[i].hasWaypointTarget = 0;
		NPC_ACTIVE_ANIM[i] = 0;
	}
}

void removeMapEntities(void)
{
	volatile int32_t unloaded[8];
	Entity *entity;
	int32_t i;
	int32_t j;

	for (j = 0; j < 8; j++) {
		unloaded[j] = -1;
	}
	for (i = 0; i < 8; i++) {
		if (ENTITY_TABLE[i + 2] != NULL) {
			entity = ENTITY_TABLE[i + 2];
			removeEntity(entity->type, i + 2);
			ENTITY_TABLE[i + 2] = NULL;
		}
	}
	for (i = 0; i < 8; i++) {
		if (LOADED_DIGIMON_MODELS[i] != -1) {
			thunkUnloadModel(LOADED_DIGIMON_MODELS[i], 0);
		}
	}
	initializeLoadedNPCModels();
	clearMapAITable(-1);
}

void clearMapAITable(int32_t index)
{
	int32_t i;

	if (index != -1) {
		MAP_DIGIMON_TABLE[index].lookAtTamerState =
			MAP_DIGIMON_TABLE[index].hasWaypointTarget = 0;
		MAP_DIGIMON_TABLE[index].path.activeSection = 0;
	} else {
		for (i = 0; i < 8; i++) {
			MAP_DIGIMON_TABLE[i].lookAtTamerState =
				MAP_DIGIMON_TABLE[i].hasWaypointTarget = 0;
			MAP_DIGIMON_TABLE[i].path.activeSection = 0;
		}
	}
}

void NPCEntityTickOverworld(int32_t instanceId, MapDigimonEntity *mapDigimon)
{
	Entity *entity;
	int32_t inRange;

	if (ENTITY_TABLE[instanceId] == NULL) {
		return;
	}
	if (IS_IN_MENU == 1) {
		return;
	}
	entity = ENTITY_TABLE[instanceId];

	if (NPC_ACTIVE_ANIM[instanceId - 2] == 0) {
		if (mapDigimon->stopAnim == 0) {
			if (mapDigimon->lookAtTamerState == 0) {
#if defined(VERSION_JP)
				NPCEntityTickWaypointAI(mapDigimon, entity);
#else
				NPCEntityTickWaypointAI(mapDigimon, entity, instanceId);
#endif
			}

			switch (mapDigimon->followMode) {
			case 2:
			case 11:
				inRange = NPCEntityIsInTrackingRect(
					mapDigimon,
					&TAMER_ENTITY.entity.posData->location);
				if (inRange == 1 || mapDigimon->lookAtTamerState != 0) {
					NPCEntityTickLookingAtTamer(mapDigimon, entity,
					                            &TAMER_ENTITY);
				}
				break;
			case 3:
			case 4:
			case 5:
			case 12:
			case 13:
			case 14:
				inRange = NPCEntityIsInTrackingRect(
					mapDigimon,
					&TAMER_ENTITY.entity.posData->location);
				if (inRange == 1 || mapDigimon->lookAtTamerState != 0) {
					NPCEntityTickTrackingTamer(mapDigimon, entity,
					                           &TAMER_ENTITY, instanceId);
				}
				break;
			case 6:
			case 15:
				inRange = NPCEntityIsInTrackingRadius(
					&NPC_ENTITIES[instanceId - 2].digimonEntity.entity,
					&TAMER_ENTITY.entity, mapDigimon);
				if (inRange == 1 || mapDigimon->lookAtTamerState != 0) {
					NPCEntityTickTrackingTamer2(mapDigimon, entity,
					                            &TAMER_ENTITY, instanceId, 2);
				}
				break;
			case 7:
			case 16:
				inRange = NPCEntityIsInTrackingRadius(
					&NPC_ENTITIES[instanceId - 2].digimonEntity.entity,
					&TAMER_ENTITY.entity, mapDigimon);
				if (inRange == 1 || mapDigimon->lookAtTamerState != 0) {
					NPCEntityTickTrackingTamer2(mapDigimon, entity,
					                            &TAMER_ENTITY, instanceId, 4);
				}
				break;
			case 8:
			case 9:
			case 17:
			case 18:
				inRange = NPCEntityIsInTrackingRect(
					mapDigimon,
					&TAMER_ENTITY.entity.posData->location);
				if (inRange == 1 || mapDigimon->lookAtTamerState != 0) {
					NPCEntityTickTrackingTamer2(mapDigimon, entity,
					                            &TAMER_ENTITY, instanceId, 2);
				}
				break;
			}

			NPC_COLLISION_STATE[instanceId - 2] =
				entityCheckCollision(NULL, entity, 0, 0);
#if defined(VERSION_JP)
			if (NPC_COLLISION_STATE[instanceId - 2] == 0 &&
			    tamerGetState() == 0) {
				entity->anim.animFlag |= 2;
				if (NPC_ENTITIES[instanceId - 2].autotalk == 1 &&
				    IS_SCRIPT_PAUSED == 1) {
					removeTriangleMenu();
					closeInventoryBoxes();
					removeUIBox1();
					TALKED_TO_ENTITY = instanceId;
					callScriptSection(
						CURRENT_SCRIPT_ID,
						NPC_ENTITIES[instanceId - 2].scriptId, 1);
				}
			}
#else
			if (NPC_COLLISION_STATE[instanceId - 2] == 0 &&
			    tamerGetState() == 0 &&
			    NPC_ENTITIES[instanceId - 2].autotalk == 1) {
				entity->anim.animFlag |= 2;
				if (IS_SCRIPT_PAUSED == 1) {
					removeTriangleMenu();
					closeInventoryBoxes();
					removeUIBox1();
					TALKED_TO_ENTITY = instanceId;
					callScriptSection(
						CURRENT_SCRIPT_ID,
						NPC_ENTITIES[instanceId - 2].scriptId, 1);
				}
			}
#endif
			if (NPC_COLLISION_STATE[instanceId - 2] != -1 &&
			    NPC_IS_WALKING_TOWARDS[instanceId - 2] == 0 &&
			    entity->anim.animId > 1 && entity->anim.animId < 5) {
				collisionGrace(NULL, entity, 0, 0);
			}
		} else {
			if (mapDigimon->animation != 0) {
				mapDigimon->animation = 0;
				startAnimation(entity, mapDigimon->animation);
			}
		}
	}

	entity->isOnScreen = entityIsOffScreen(entity, 0x140, 0xf0) ^ 1;
	tickAnimation(entity);
}

#if defined(VERSION_JP)
void NPCEntityTickWaypointAI(MapDigimonEntity *mapDigimon, Entity *entity)
#else
void NPCEntityTickWaypointAI(MapDigimonEntity *mapDigimon, Entity *entity,
                             int32_t instanceId)
#endif
{
	switch (mapDigimon->path.aiSections[mapDigimon->path.activeSection]) {
	case 0:
		tickWaypointWait(mapDigimon, entity);
		break;
	case 1:
#if defined(VERSION_JP)
		NPCEntityTickWaypointWalk(mapDigimon, entity, 2);
#else
		NPCEntityTickWaypointWalk(mapDigimon, entity, 2, instanceId);
#endif
		break;
	case 2:
#if defined(VERSION_JP)
		NPCEntityTickWaypointWalk(mapDigimon, entity, 4);
#else
		NPCEntityTickWaypointWalk(mapDigimon, entity, 4, instanceId);
#endif
		break;
	}

	if (mapDigimon->path.activeSection >= 8 ||
	    mapDigimon->path.aiSections[mapDigimon->path.activeSection] == -1) {
		mapDigimon->path.activeSection = 0;
	}
}

int32_t NPCEntityIsInTrackingRect(MapDigimonEntity *mapDigimon, VECTOR *location)
{
	if ((mapDigimon->posX + mapDigimon->trackingRange > location->vx) &&
	    (mapDigimon->posX - mapDigimon->trackingRange < location->vx) &&
	    (mapDigimon->posZ + mapDigimon->trackingRange > location->vz) &&
	    (mapDigimon->posZ - mapDigimon->trackingRange < location->vz)) {
		return 1;
	}
	return 0;
}

void NPCEntityTickLookingAtTamer(MapDigimonEntity *mapDigimon, Entity *entity,
                                 TamerEntity *tamer)
{
	if (mapDigimon->lookAtTamerState == 0) {
		mapDigimon->animation = 0;
		startAnimation(entity, mapDigimon->animation);
		mapDigimon->lookAtTamerState = 1;
	} else {
		getRotationDifference(entity->posData,
		                      &tamer->entity.posData->location,
		                      &mapDigimon->targetAngle,
		                      &mapDigimon->ccDiff,
		                      &mapDigimon->cwDiff);
		rotateEntity(&entity->posData->rotation,
		             &mapDigimon->targetAngle, &mapDigimon->ccDiff,
		             &mapDigimon->cwDiff, 0x71);
		if (NPCEntityIsInTrackingRect(mapDigimon, &TAMER_ENTITY.entity.posData->location) == 0) {
			mapDigimon->lookAtTamerState = 0;
			mapDigimon->hasWaypointTarget = 0;
		}
	}
}

// clang-format off
void NPCEntityTickTrackingTamer(mapDigimon, entity, tamer, instanceId)
	MapDigimonEntity *mapDigimon;
	Entity *entity;
	TamerEntity *tamer;
	int16_t instanceId;
// clang-format on
{
	if (mapDigimon->lookAtTamerState == 0) {
		mapDigimon->targetLocation.vx =
			tamer->entity.posData->location.vx;
		mapDigimon->targetLocation.vy =
			tamer->entity.posData->location.vy;
		mapDigimon->targetLocation.vz =
			tamer->entity.posData->location.vz;
		mapDigimon->animation = 2;
		startAnimation(entity, mapDigimon->animation);
		mapDigimon->lookAtTamerState = 1;
	} else {
		NPCEntityTickTrackingTamer3(mapDigimon, entity, instanceId);
	}
}

int32_t NPCEntityIsInTrackingRadius(Entity *entity, Entity *otherEntity,
                                    MapDigimonEntity *mapDigimon)
{
	int32_t dx;
	int32_t dz;
	int32_t dist;
	int32_t radius;

	dx = entity->posData->location.vx - otherEntity->posData->location.vx;
	dz = entity->posData->location.vz - otherEntity->posData->location.vz;
	dist = dx + dz;
	radius = (mapDigimon->trackingRange * 6 / 10) * (mapDigimon->trackingRange * 6 / 10);
	if (dist < radius) {
		return 1;
	}
	return 0;
}

// clang-format off
void NPCEntityTickTrackingTamer2(mapDigimon, entity, tamer, instanceId, animId)
	MapDigimonEntity *mapDigimon;
	Entity *entity;
	TamerEntity *tamer;
	int16_t instanceId;
	uint8_t animId;
// clang-format on
{
	if (mapDigimon->lookAtTamerState == 0) {
		if (mapDigimon->animation != animId) {
			mapDigimon->animation = animId;
			startAnimation(entity, mapDigimon->animation);
		}
		mapDigimon->lookAtTamerState = 1;
	} else {
		NPCEntityTickTrackingTamer4(mapDigimon, entity, tamer, instanceId);
	}
}

void getRotationDifference(PositionData *posData, VECTOR *targetLoc,
                           int16_t *outAngle, int16_t *outCcDiff,
                           int16_t *outCwDiff)
{
	int16_t dx;
	int16_t dz;

	dx = targetLoc->vx - posData->location.vx;
	dz = targetLoc->vz - posData->location.vz;
	*outAngle = _atan(dz, dx);
	if (*outAngle > posData->rotation.vy) {
		*outCwDiff = *outAngle - posData->rotation.vy;
		*outCcDiff = posData->rotation.vy + (4096 - *outAngle);
	} else {
		*outCwDiff = *outAngle + (4096 - posData->rotation.vy);
		*outCcDiff = posData->rotation.vy - *outAngle;
	}
}

int32_t rotateEntity(SVECTOR *rotation, int16_t *targetAngle, int16_t *ccDiff,
                     int16_t *cwDiff, int16_t speed)
{
	int16_t target;
	int16_t cc;
	int16_t cw;

	target = *targetAngle;
	cc = *ccDiff;
	cw = *cwDiff;
	if (rotation->vy < target) {
		if (cc < cw) {
			rotation->vy -= speed;
			if ((target - 4096) > rotation->vy) {
				rotation->vy = target;
				return 1;
			}
		} else if (cw < cc) {
			rotation->vy += speed;
			if (target < rotation->vy) {
				rotation->vy = target;
				return 1;
			}
		}
	} else if (target < rotation->vy) {
		if (cc < cw) {
			rotation->vy -= speed;
			if (rotation->vy < target) {
				rotation->vy = target;
				return 1;
			}
		} else if (cw < cc) {
			rotation->vy += speed;
			if ((target + 4096) < rotation->vy) {
				rotation->vy = target;
				return 1;
			}
		}
	} else {
		rotation->vy = target;
		return 1;
	}
	return 0;
}

void NPCEntityTickTrackingTamer3(MapDigimonEntity *mapDigimon, Entity *entity,
                                 int16_t instanceId)
{
	int16_t currentTileX;
	int16_t currentTileY;
	int16_t targetTileX;
	int16_t targetTileY;

	switch (mapDigimon->lookAtTamerState) {
	case 1:
		mapDigimon->animation = 4;
		startAnimation(entity, mapDigimon->animation);
		mapDigimon->lookAtTamerState = 2;
		break;
	case 2:
		if (NPC_COLLISION_STATE[instanceId - 2] == -1) {
			getRotationDifference(entity->posData,
			                      &mapDigimon->targetLocation,
			                      &mapDigimon->targetAngle,
			                      &mapDigimon->ccDiff,
			                      &mapDigimon->cwDiff);
			rotateEntity(&entity->posData->rotation,
			             &mapDigimon->targetAngle,
			             &mapDigimon->ccDiff, &mapDigimon->cwDiff,
			             0x71);
		}
		getModelTile(&entity->posData->location, &currentTileX,
		             &currentTileY);
		getModelTile(&mapDigimon->targetLocation, &targetTileX,
		             &targetTileY);
		if (((currentTileX == targetTileX) &&
		     (currentTileY == targetTileY)) ||
		    (NPC_COLLISION_STATE[instanceId - 2] == 0)) {
			mapDigimon->animation = 0;
			startAnimation(entity, mapDigimon->animation);
			mapDigimon->waypointWaitTimer = 0;
			mapDigimon->lookAtTamerState = 3;
		}
		break;
	case 3:
		mapDigimon->waypointWaitTimer++;
		if (mapDigimon->waypointWaitTimer >= 40) {
			mapDigimon->animation = 2;
			startAnimation(entity, mapDigimon->animation);
			mapDigimon->targetLocation.vx = mapDigimon->posX;
			mapDigimon->targetLocation.vy = mapDigimon->posY;
			mapDigimon->targetLocation.vz = mapDigimon->posZ;
			mapDigimon->waypointWaitTimer = 0;
			mapDigimon->lookAtTamerState = 4;
		}
		break;
	case 4:
		if (NPC_COLLISION_STATE[instanceId - 2] == -1) {
			getRotationDifference(entity->posData,
			                      &mapDigimon->targetLocation,
			                      &mapDigimon->targetAngle,
			                      &mapDigimon->ccDiff,
			                      &mapDigimon->cwDiff);
			rotateEntity(&entity->posData->rotation,
			             &mapDigimon->targetAngle,
			             &mapDigimon->ccDiff, &mapDigimon->cwDiff,
			             0x71);
		}
		getModelTile(&entity->posData->location, &currentTileX,
		             &currentTileY);
		getModelTile(&mapDigimon->targetLocation, &targetTileX,
		             &targetTileY);
		if ((currentTileX == targetTileX) &&
		    (currentTileY == targetTileY)) {
			mapDigimon->animation = 0;
			startAnimation(entity, mapDigimon->animation);
			mapDigimon->waypointWaitTimer = 0;
			mapDigimon->lookAtTamerState = mapDigimon->hasWaypointTarget = 0;
		}
		break;
	}
}

void NPCEntityTickTrackingTamer4(MapDigimonEntity *mapDigimon, Entity *entity,
                                 TamerEntity *tamer, int16_t instanceId)
{
	int32_t inRadius;
	int16_t currentTileX;
	int16_t currentTileY;
	int16_t targetTileX;
	int16_t targetTileY;

	switch (mapDigimon->lookAtTamerState) {
	case 1:
		inRadius = NPCEntityIsInTrackingRadius(
			&NPC_ENTITIES[instanceId - 2].digimonEntity.entity,
			&tamer->entity, mapDigimon);
		if (inRadius == 1) {
			if (NPC_COLLISION_STATE[instanceId - 2] == -1) {
				getRotationDifference(
					entity->posData,
					&tamer->entity.posData->location,
					&mapDigimon->targetAngle,
					&mapDigimon->ccDiff,
					&mapDigimon->cwDiff);
				rotateEntity(&entity->posData->rotation,
				             &mapDigimon->targetAngle,
				             &mapDigimon->ccDiff,
				             &mapDigimon->cwDiff, 0x71);
			}
		} else {
			mapDigimon->lookAtTamerState = 2;
			mapDigimon->targetLocation.vx = mapDigimon->posX;
			mapDigimon->targetLocation.vy = mapDigimon->posY;
			mapDigimon->targetLocation.vz = mapDigimon->posZ;
		}
		break;
	case 2:
		if (NPC_COLLISION_STATE[instanceId - 2] == -1) {
			getRotationDifference(entity->posData,
			                      &mapDigimon->targetLocation,
			                      &mapDigimon->targetAngle,
			                      &mapDigimon->ccDiff,
			                      &mapDigimon->cwDiff);
			rotateEntity(&entity->posData->rotation,
			             &mapDigimon->targetAngle,
			             &mapDigimon->ccDiff,
			             &mapDigimon->cwDiff, 0x71);
		}
		getModelTile(&entity->posData->location, &currentTileX,
		             &currentTileY);
		getModelTile(&mapDigimon->targetLocation, &targetTileX,
		             &targetTileY);
		if ((currentTileX == targetTileX) &&
		    (currentTileY == targetTileY)) {
			mapDigimon->animation = 0;
			startAnimation(entity, mapDigimon->animation);
			mapDigimon->waypointWaitTimer = 0;
			mapDigimon->lookAtTamerState = 3;
		}
		break;
	case 3:
		mapDigimon->waypointWaitTimer++;
		if (mapDigimon->waypointWaitTimer >= 80) {
			mapDigimon->lookAtTamerState =
				mapDigimon->hasWaypointTarget = 0;
			mapDigimon->waypointWaitTimer = 0;
		}
		break;
	}
}

void tickWaypointWait(MapDigimonEntity *mapDigimon, Entity *entity)
{
	switch (mapDigimon->hasWaypointTarget) {
	case 0:
		if (mapDigimon->animation != 0) {
			mapDigimon->animation = 0;
			startAnimation(entity, mapDigimon->animation);
		}
		mapDigimon->waypointWaitTimer = 0;
		mapDigimon->hasWaypointTarget = 1;
		break;
	case 1:
		mapDigimon->waypointWaitTimer++;
		if (mapDigimon->waypointWaitTimer >=
		    mapDigimon->path.waypoints[mapDigimon->path.activeSection].vx) {
			mapDigimon->hasWaypointTarget = 0;
			mapDigimon->waypointWaitTimer = 0;
			mapDigimon->path.activeSection++;
		}
		break;
	}
}

#if defined(VERSION_JP)
void NPCEntityTickWaypointWalk(MapDigimonEntity *mapDigimon, Entity *entity,
                               int32_t animation)
#else
void NPCEntityTickWaypointWalk(MapDigimonEntity *mapDigimon, Entity *entity,
                               int32_t animation, int32_t instanceId)
#endif
{
	int16_t currentTileX;
	int16_t currentTileY;
	int16_t targetTileX;
	int16_t targetTileY;

	switch (mapDigimon->hasWaypointTarget) {
	case 0:
		mapDigimon->targetLocation.vx =
			mapDigimon->path.waypoints[mapDigimon->path.activeSection].vx;
		mapDigimon->targetLocation.vy =
			mapDigimon->path.waypoints[mapDigimon->path.activeSection].vy;
		mapDigimon->targetLocation.vz =
			mapDigimon->path.waypoints[mapDigimon->path.activeSection].vz;
		if (mapDigimon->animation != animation) {
			mapDigimon->animation = animation;
			startAnimation(entity, mapDigimon->animation);
		}
		mapDigimon->hasWaypointTarget = 1;
		break;
	case 1:
#if defined(VERSION_JP)
		getRotationDifference(entity->posData, &mapDigimon->targetLocation,
		                      &mapDigimon->targetAngle, &mapDigimon->ccDiff,
		                      &mapDigimon->cwDiff);
		rotateEntity(&entity->posData->rotation, &mapDigimon->targetAngle,
		             &mapDigimon->ccDiff, &mapDigimon->cwDiff, 0x71);
#else
		if (NPC_COLLISION_STATE[instanceId - 2] == -1) {
			getRotationDifference(entity->posData,
			                      &mapDigimon->targetLocation,
			                      &mapDigimon->targetAngle,
			                      &mapDigimon->ccDiff,
			                      &mapDigimon->cwDiff);
			rotateEntity(&entity->posData->rotation,
			             &mapDigimon->targetAngle,
			             &mapDigimon->ccDiff, &mapDigimon->cwDiff,
			             0x71);
		}
#endif
		getModelTile(&entity->posData->location, &currentTileX,
		             &currentTileY);
		getModelTile(&mapDigimon->targetLocation, &targetTileX,
		             &targetTileY);
		if ((currentTileX == targetTileX) &&
		    (currentTileY == targetTileY)) {
			mapDigimon->hasWaypointTarget = 0;
			mapDigimon->path.activeSection++;
		}
		break;
	}
}

void setActiveAnim(uint8_t scriptId, int8_t animId)
{
	NPCEntity *npc;
	int32_t i;

	npc = NPC_ENTITIES;
	for (i = 0; i < 8; i++) {
		if (ENTITY_TABLE[i + 2] == NULL) {
			npc++;
			continue;
		}
		if (npc->scriptId == scriptId) {
			NPC_ACTIVE_ANIM[i] = animId;
			return;
		}
		npc++;
	}
}

void scriptNPCStartAnimation(uint8_t scriptId, int8_t animId)
{
	NPCEntity *npc;
	int32_t i;

	npc = NPC_ENTITIES;
	for (i = 0; i < 8; i++) {
		if (ENTITY_TABLE[i + 2] == NULL) {
			npc++;
			continue;
		}
		if (npc->scriptId == scriptId) {
			startAnimation((Entity *)npc, animId);
			return;
		}
		npc++;
	}
}

void setLoopCountToOne(uint32_t scriptId)
{
	Entity *entity;

	entity = getEntityFromScriptId((uint8_t *)&scriptId);
	entity->anim.loopCount = 1;
}

void resetEntityOrigin(uint8_t scriptId)
{
	NPCEntity *npc;
	MapDigimonPath *path;
	int16_t deltaX;
	int16_t deltaZ;
	int32_t i;
	int32_t j;

	npc = NPC_ENTITIES;
	for (i = 0; i < 8; i++) {
		if (ENTITY_TABLE[i + 2] == NULL) {
			npc++;
			continue;
		}
		if (npc->scriptId == scriptId) {
			break;
		}
		npc++;
	}
	deltaX = npc->digimonEntity.entity.posData->location.vx -
	         MAP_DIGIMON_TABLE[i].posX;
	deltaZ = npc->digimonEntity.entity.posData->location.vz -
	         MAP_DIGIMON_TABLE[i].posZ;
	MAP_DIGIMON_TABLE[i].posX =
		npc->digimonEntity.entity.posData->location.vx;
	MAP_DIGIMON_TABLE[i].posZ =
		npc->digimonEntity.entity.posData->location.vz;
	path = &MAP_DIGIMON_TABLE[i].path;
	for (j = 0; j < 8; j++) {
		if (path->aiSections[j] != 0) {
			if (path->aiSections[j] == -1) {
				break;
			}
			path->waypoints[j].vx += deltaX;
			path->waypoints[j].vz += deltaZ;
		}
	}
}

void setMovementEnabled(int32_t id, int32_t enabled)
{
	int32_t i;

	if (id != -1) {
		if (id == 0) {
			if (enabled == 0) {
				tamerSetState(0);
			} else {
				tamerSetState(6);
			}
		} else if (id == 1) {
			if (enabled == 0) {
				partnerSetState(1);
			} else {
				partnerSetState(11);
			}
		} else {
			MAP_DIGIMON_TABLE[id - 2].stopAnim = enabled;
			if (enabled == 1) {
				clearMapAITable((int16_t)(id - 2));
			}
		}
	} else {
		if (enabled == 0) {
			tamerSetState(0);
			partnerSetState(1);
		} else {
			tamerSetState(6);
			partnerSetState(11);
		}
		for (i = 0; i < 8; i++) {
			MAP_DIGIMON_TABLE[i].stopAnim = enabled;
			if (enabled == 1) {
				clearMapAITable(-1);
			}
		}
	}
}

void setPartnerIdling(void)
{
	startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0);
}

void renderPlayerMenu(void);
