#include <mwinline_n.h>

#include <dw/clock.h>
#include <dw/evl.h>
#include <dw/fish.h>
#include <dw/item.h>
#include <dw/ui.h>

typedef struct {
	int16_t typeId;
	int16_t padding0;
	VECTOR waypoints[8];
	int16_t aiSections[8];
	int16_t activeSection;
	int16_t padding2;
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
	int16_t orderValue;
	int16_t x;
	int16_t y;
	int16_t animSprites[8];
	uint8_t animTimes[8];
	uint8_t timer;
	uint8_t pad;
	int8_t currentFrame;
	int8_t flag;
} LocalMapObjectInstance;

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
extern int32_t LOADED_DIGIMON_MODELS[8];
extern int8_t GAME_STATE;
extern int8_t MAIN_D_80134D20[8];
extern MapDigimonEntity MAP_DIGIMON_TABLE[];
extern int16_t NPC_COLLISION_STATE[];
extern int32_t NPC_IS_WALKING_TOWARDS[];
extern int8_t TALKED_TO_ENTITY;
extern int16_t MIST_CLUT_Y[2];
extern int8_t NPC_ACTIVE_ANIM;
extern LocalMapObjectInstance LOCAL_MAP_OBJECT_INSTANCE[];
extern int16_t MAIN_D_8013CA38[];
extern int16_t MAIN_D_8013CA8C[];
extern int8_t MAIN_D_8013CAE0[];
extern int8_t MAIN_D_8013CB0C[];
extern int16_t MAP_OBJECT_INSTANCE_COUNT;
extern LocalMapObject LOCAL_MAP_OBJECTS[];
extern int16_t CAMERA_X[];
extern int16_t CAMERA_Y[];
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_Y;
extern int32_t MAP_OBJECT_MOVE_TO_DATA[];
extern int8_t MAIN_D_8013CB38[];
extern int8_t MAIN_D_8013CB44[];
extern uint8_t MAP_LAYER_ENABLED;
extern uint8_t CURRENT_SCREEN;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern int16_t CAMERA_X_PREVIOUS;
extern int16_t CAMERA_Y_PREVIOUS;
extern int16_t MIST_X_OFFSETS[4];
extern int16_t MIST_Y_OFFSETS[2];
extern int8_t MENU_SUB_STATE;
extern char *MAIN_D_80124800[];
extern int32_t TRIANGLE_MENU_STATE;
extern int8_t MAIN_D_80123E8E[];
extern RECT MAIN_D_80134290;
extern char MAIN_D_80134298[8];
extern int32_t MAIN_D_80134D2C;
extern int8_t MENU_STATE;
extern int8_t MAIN_D_80134D36;
extern int8_t MAIN_D_80134D37;
typedef struct {
  uint8_t digimon;
  uint8_t moves;
} MenuTabPair;

extern uint8_t MAIN_D_801342A0[2];
extern uint8_t MAIN_D_801342A4[4];
extern char *MAIN_D_801247B8[];
extern uint8_t GAME_MENU_SPRITES[];
extern uint8_t INVENTORY_ITEM_TYPES[];
extern uint8_t INVENTORY_POINTER;
extern TamerItem TAMER_ITEM;

void clearTextSubArea(RECT *area);
int32_t isTriggerSet(int32_t triggerId);
void callScriptSection(uint16_t scriptId, uint32_t scriptSection,
		       uint32_t param);
void startNPCAnimation(uint8_t scriptId, int32_t animId);
void setTamerState(int32_t state);
void drawString(char *text, int32_t color, int32_t pos);
void drawInventoryText(void);
void closeTriangleMenu(void);
void closeInventoryBoxes2();
void renderDigimonMovesView(void);
void renderDigimonStatsView(void);
void renderCardsView(void);
void renderMedalView(void);
void renderEvoChartView(void);
void renderPlayerInfoView(void);
void renderString();
void renderMenuTab(int32_t a, int32_t b, int32_t c);
void renderOverworldItem(TamerItem *item);

void setCameraFollowPlayer(void);
void handleGameMenuSelection(int32_t selection);
void createMenuBox(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
		   int32_t f, void (*tick)(void), void (*render)(void));
void closeUIBoxIfOpen(int32_t arg);
void initializeInventoryObject(void);
void clearTextArea(void);
void tickGameMenu(void);
void renderGameMenu(void);
void tickDigimonMenu(void);
void renderDigimonMenu(void);
void tickPlayerMenu(void);
void renderPlayerMenu(void);
void tickTriangleMenu(void);
void renderRectPolyFT4(int16_t posX, int16_t posY, int32_t width,
		       int32_t height, uint8_t texX, uint8_t texY,
		       int16_t texturePage, int16_t clut, int32_t zIndex,
		       int8_t flag);
void renderSeperatorLines(int16_t *lines, int32_t b, ...);
void renderMist(void);
void renderMapOverlays(LocalMapObjectInstance *instances, int32_t screenX,
		       int16_t screenY);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY,
		       int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uPos, int32_t vPos,
		      int32_t width, int32_t height);
void buildMapOverlayPrim(POLY_FT4 *prim, LocalMapObjectInstance *inst,
			 LocalMapObject *obj, int32_t arg3, int16_t arg4,
			 int8_t mode);
void buildSnowflakePrim(POLY_FT4 *prim, LocalMapObjectInstance *inst,
			LocalMapObject *obj);
Entity *getEntityFromScriptId(uint8_t *scriptId);
void removeEntity(int32_t type, int32_t entityId);
void startAnimation(Entity *entity, int32_t animId);
void thunkLoadMMD(int32_t digiType, int32_t modelType);
void thunkUnloadModel(int32_t digiType, int32_t modelType);
void tickNPCBattle(int32_t instanceId);
void STD_tickNPCTournament(int32_t instanceId);
void tickWaypointAI(MapDigimonEntity *mapDigimon, Entity *entity,
		    int32_t instanceId);
void tickWaypointWait(MapDigimonEntity *mapDigimon, Entity *entity);
void tickWaypointWalk(MapDigimonEntity *mapDigimon, Entity *entity,
		      int32_t animation, int32_t instanceId);
int32_t isInTrackingRect(MapDigimonEntity *mapDigimon, VECTOR *location);
void tickLookingAtTamer(MapDigimonEntity *mapDigimon, Entity *entity,
			TamerEntity *tamer);
void clearMapObjects(LocalMapObjectInstance *instances);
void loadMapObjects(LocalMapObjectInstance *mapObjects, uint8_t *data,
		    int32_t mapId);
void getRotationDifference(PositionData *posData, VECTOR *targetLoc,
			   int16_t *outAngle, int16_t *outCcDiff,
			   int16_t *outCwDiff);
int32_t rotateEntity(SVECTOR *rotation, int16_t *targetAngle, int16_t *ccDiff,
		     int16_t *cwDiff, int16_t speed);
void getModelTile(VECTOR *position, int16_t *outTileX, int16_t *outTileY);
void tickTrackingTamer(MapDigimonEntity *mapDigimon, Entity *entity,
		       TamerEntity *tamer, int32_t instanceId);
void tickTrackingTamer3(MapDigimonEntity *mapDigimon, Entity *entity,
			int32_t instanceId);
int32_t isInTrackingRadius(Entity *entity, Entity *otherEntity,
			   MapDigimonEntity *mapDigimon);
void tickTrackingTamer2(MapDigimonEntity *mapDigimon, Entity *entity,
			TamerEntity *tamer, int32_t instanceId,
			uint8_t animId);
void tickTrackingTamer4(MapDigimonEntity *mapDigimon, Entity *entity,
			TamerEntity *tamer, int32_t instanceId);
int32_t entityCheckCollision(Entity *a, Entity *entity, int32_t c, int32_t d);
int32_t getTamerState(void);
void removeTriangleMenu(void);
void closeInventoryBoxes(void);
void removeUIBox1(void);
void collisionGrace(int32_t a, Entity *entity, int32_t c, int32_t d);
uint8_t entityIsOffScreen(Entity *entity, int32_t w, int32_t h);
void tickAnimation(Entity *entity);
void tickNPC(int32_t instanceId);
void tickNPCOverworld(int32_t instanceId, MapDigimonEntity *mapDigimon);
void scriptUnloadEntity(uint8_t scriptId);
void setLoopCountToOne(uint32_t scriptId);
void loadNPCModel(int32_t digimonId);
void unloadDigimonModel(int32_t digimonType);
void setPartnerIdling(void);
int32_t tickRemoveMist(void);
void setActiveAnim(uint8_t scriptId, uint8_t animId);
void spawnSpriteAtEntity(int32_t scriptId, int32_t nodeId, int32_t sprite);
void spawnSpriteAtLocation(int16_t x, int16_t y, int16_t z, int32_t sprite,
			   int16_t flag);
void loadMapImage1(u_long *tim);
void loadMapImage2(u_long *tim, int32_t id);
void renderNinjamonEffect(int32_t instanceId);
int32_t random(int32_t max);
int32_t _atan(int32_t dy, int32_t dx);
void createNinjamonEffect(void);
void calcMapObjectOrder(LocalMapObjectInstance *instances);
void getDrawPosition(SVECTOR *worldPos, int16_t *outX, int16_t *outY);
void storeMapObjectPosition(int16_t *outX, int16_t *outY, uint8_t a,
			    int32_t count);
void loadMapObjectPosition(int16_t *xData, int16_t *yData, uint8_t startIndex,
			   int32_t count);
void moveMapObjects(uint8_t startIndex, int32_t count, int16_t dx,
		    int16_t dy);
int32_t moveMapObjectsWithLimit(uint8_t startIndex, int32_t count, int16_t dx,
				int16_t dy, int16_t limitX, int16_t limitY);
void setMapObjectsFlag(uint8_t startIndex, int32_t count, int32_t flag);
void resetMapObjectAnimation(uint8_t startIndex, int32_t count);
void clearMapAITable(int32_t index);
void removeMapEntities(void);
void clearMapDigimon(void);
void resetEntityOrigin(int32_t scriptId);
void loadMapDigimon(int16_t *data, int32_t mapId);
void initializeLoadedNPCModels(void);
int32_t scriptSetDigimon(int32_t type, int32_t slot, int32_t autotalk);
void initializeDigimonObject(int32_t type, int32_t instanceId,
			     void (*tick)(int32_t));
void setEntityPosition(int32_t entityId, int32_t x, int32_t y, int32_t z);
void setEntityRotation(int32_t entityId, int16_t x, int16_t y, int16_t z);
void setupEntityMatrix(int32_t entityId);
int32_t tickMoveObjectTo(int32_t objectIndex, int32_t moveIndex,
			 int32_t steps, int32_t targetX, int16_t targetY);
void setMovementEnabled(int32_t id, int32_t enabled);
void setPartnerState(int32_t state);
int32_t addObject(int32_t objectId, int32_t instanceId, void (*tick)(int32_t),
		  void (*render)(int32_t));
void addGameMenu(void);
void renderDateDigits(void);
void renderTriangleCursor(void);
void isUIBoxAvailable(void);
void setSleepDisabled(int32_t arg);
void removeItem(int32_t type, uint32_t amount);
void startFeedingItem(int32_t arg);
void removeOneSelectedItem(void);
void renderFeedingItem(int32_t arg);
void getEquippedSlot(void);
void equipMove(void);

static void *overworld_functions[] = {
	equipMove,
	getEquippedSlot,
	renderFeedingItem,
	removeOneSelectedItem,
	startFeedingItem,
	setSleepDisabled,
	handleGameMenuSelection,
	isUIBoxAvailable,
	renderPlayerMenu,
	tickPlayerMenu,
	renderDigimonMenu,
	tickDigimonMenu,
	createMenuBox,
	tickGameMenu,
	renderRectPolyFT4,
	renderTriangleCursor,
	renderDateDigits,
	renderSeperatorLines,
	renderGameMenu,
	closeUIBoxIfOpen,
	closeTriangleMenu,
	tickTriangleMenu,
	addGameMenu,
	setPartnerIdling,
	setMovementEnabled,
	resetEntityOrigin,
	setLoopCountToOne,
	startNPCAnimation,
	setActiveAnim,
	tickWaypointWalk,
	tickWaypointWait,
	tickTrackingTamer4,
	tickTrackingTamer3,
	rotateEntity,
	getRotationDifference,
	tickTrackingTamer2,
	isInTrackingRadius,
	tickTrackingTamer,
	tickLookingAtTamer,
	isInTrackingRect,
	tickWaypointAI,
	tickNPCOverworld,
	clearMapAITable,
	removeMapEntities,
	clearMapDigimon,
	unloadDigimonModel,
	scriptUnloadEntity,
	tickNPC,
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

void clearMapObjects(LocalMapObjectInstance *instances)
{
	LocalMapObjectInstance *data;
	int32_t i;
	int32_t j;

	data = instances;
	for (i = 0; i < 188; i++) {
		for (j = 0; j < 8; j++) {
			data->animSprites[j] = -1;
		}
		for (j = 0; j < 8; j++) {
			data->animTimes[j] = 0;
		}
		data->orderValue = 0;
		data->timer = 0;
		data->currentFrame = -1;
		data->flag = 1;
		data++;
	}
}

void loadMapObjects(LocalMapObjectInstance *mapObjects, uint8_t *data,
		    int32_t mapId)
{
	LocalMapObjectInstance *obj;
	int16_t *src;
	int32_t i;
	int32_t j;
	int32_t k;
	int16_t count;

	obj = mapObjects;
	src = (int16_t *)data;
	count = *src++;
	for (i = 0; i < count; i++) {
		LOCAL_MAP_OBJECTS[i].texX = *src++;
		LOCAL_MAP_OBJECTS[i].texY = *src++;
		LOCAL_MAP_OBJECTS[i].width = *src++;
		LOCAL_MAP_OBJECTS[i].height = *src++;
		LOCAL_MAP_OBJECTS[i].someX = *src++;
		LOCAL_MAP_OBJECTS[i].someY = *src++;
		LOCAL_MAP_OBJECTS[i].someZ = *src++;
		LOCAL_MAP_OBJECTS[i].clut = *src++;
		LOCAL_MAP_OBJECTS[i].transparency = *src++;
	}
	MAP_OBJECT_INSTANCE_COUNT = *src++;
	for (k = 0; k < MAP_OBJECT_INSTANCE_COUNT; k++) {
		for (j = 0; j < 8; j++) {
			obj->animSprites[j] = *src++;
		}
		for (j = 0; j < 8; j++) {
			obj->animTimes[j] = *src++;
		}
		obj->x = *src++;
		obj->y = *src++;
		obj->flag = *src++;
		if (((mapId >= 0x58 && mapId < 0x61) ||
		     (mapId >= 0x84 && mapId < 0x88)) &&
		    k < 0x23) {
			if (k >= 0x14) {
				obj->x = random(320);
			}
			obj->y = random(240);
			obj->flag |= 0x80;
		}
		obj++;
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

void loadMapImage2(u_long *tim, int32_t id)
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
		rect.x = i * 16;
		rect.y = 486;
		rect.w = 16;
		rect.h = 1;
		LoadImage(&rect, caddr);
		caddr += 8;
	}
}

void calcMapObjectOrder(LocalMapObjectInstance *instances)
{
	LocalMapObjectInstance *data;
	SVECTOR worldPos;
	SVECTOR screen;
	int32_t depth;
	int32_t i;
	int32_t j;
	int16_t val;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	data = instances;
	for (i = 0; i < MAP_OBJECT_INSTANCE_COUNT; i++) {
		for (j = 0; j < 8; j++) {
			val = data->animSprites[j];
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
		data->orderValue = depth;
		if (data->flag & 0x80) {
			data->orderValue = 0x1e;
		}
		data->currentFrame = 0;
		data++;
	}
}

void renderMapOverlays(LocalMapObjectInstance *instances, int32_t screenX,
		       int16_t screenY)
{
	LocalMapObjectInstance *inst;
	LocalMapObject *obj;
	POLY_FT4 *prim;
	GsOT_TAG *org;
	int32_t i;
	int32_t objId;
	int16_t startFrame;
	int32_t sx;
	int32_t xMin;
	int32_t xMax;
	int32_t yMin;
	int32_t yMax;

	if (!MAP_LAYER_ENABLED) {
		return;
	}

	if ((CURRENT_SCREEN >= 115 && CURRENT_SCREEN < 120) ||
	    CURRENT_SCREEN == 121 || CURRENT_SCREEN == 163 ||
	    CURRENT_SCREEN == 220) {
		renderMist();
	}

	sx = screenX;
	org = ACTIVE_ORDERING_TABLE->org;
	screenX = sx;

	xMin = screenX - 40;
	xMax = sx + 360;
	i = 0;
	yMin = screenY - 60;
	yMax = screenY + 300;

	inst = instances;
	for (; i < 188; i++) {
		startFrame = inst->currentFrame;

		if (inst->animSprites[startFrame] == -1 || startFrame == -1) {
			inst++;
			continue;
		}

		objId = inst->animSprites[startFrame];

		if (objId != -2 && inst->flag != 1) {
			obj = &LOCAL_MAP_OBJECTS[objId];

			if (((xMin < (inst->x + obj->width)) &&
			     (inst->x < xMax) &&
			     (yMin < (inst->y + obj->height)) &&
			     (inst->y < yMax)) ||
			    (inst->flag & 0x80) ||
			    (inst->orderValue < 20)) {
				prim = (POLY_FT4 *)GsGetWorkBase();
				SetPolyFT4(prim);
				if (obj->transparency == 4) {
					SetSemiTrans(prim, 0);
				} else {
					SetSemiTrans(prim, 1);
				}

				if (inst->flag & 0x80) {
					buildSnowflakePrim(prim, inst,
							   obj);
				} else if (inst->orderValue < 20) {
					buildMapOverlayPrim(prim, inst,
							    obj, screenX,
							    screenY, 1);
				} else {
					buildMapOverlayPrim(prim, inst,
							    obj, screenX,
							    screenY, 0);
				}

				AddPrim(&org[inst->orderValue], prim);
				prim++;
				GsSetWorkBase((PACKET *)prim);
			}

			inst->timer++;
		if (inst->timer == inst->animTimes[startFrame]) {
			inst->currentFrame++;
			inst->timer = 0;
			if (inst->animSprites[inst->currentFrame] == -1 ||
			    inst->currentFrame >= 8) {
				inst->currentFrame = 0;
			}
		}
		} else {
			inst->timer++;
		if (inst->timer == inst->animTimes[startFrame]) {
			inst->currentFrame++;
			inst->timer = 0;
			if (inst->animSprites[inst->currentFrame] == -1 ||
			    inst->currentFrame >= 8) {
				inst->currentFrame = 0;
			}
		}
		}
		inst++;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/overworld", renderMist);

INCLUDE_ASM("asm/main/nonmatchings/overworld", buildSnowflakePrim);

void buildMapOverlayPrim(POLY_FT4 *prim, LocalMapObjectInstance *inst,
			 LocalMapObject *obj, int32_t arg3, int16_t arg4,
			 int8_t mode)
{

	if (mode == 0) {
		setPosDataPolyFT4(prim,
				  ((inst->x - 160) - (arg3 - (160 - DRAWING_OFFSET_X))),
				  ((inst->y - 120) - (arg4 - (120 - DRAWING_OFFSET_Y))),
				  obj->width, obj->height);
	} else {
		setPosDataPolyFT4(prim, inst->x, inst->y, obj->width,
				  obj->height);
	}

	prim->r0 = 128;
	prim->g0 = 128;
	prim->b0 = 128;

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

	NPC_ACTIVE_ANIM = 0;
	storeMapObjectPosition(MAIN_D_8013CA38, MAIN_D_8013CA8C, 0, 0x29);
	for (i = 0; i < 0x29; i++) {
		MAIN_D_8013CAE0[i] = random(10) + 12;
		MAIN_D_8013CB0C[i] = random(10) + 3;
	}
	addObject(0xfba, 0, NULL, renderNinjamonEffect);
}

void storeMapObjectPosition(int16_t *outX, int16_t *outY, uint8_t startIndex,
			    int32_t count)
{
	long idx;
	int32_t i;

	for (i = 0; i < count; i++) {
		idx = startIndex + i;
		*outX++ = LOCAL_MAP_OBJECT_INSTANCE[idx].x;
		*outY++ = LOCAL_MAP_OBJECT_INSTANCE[idx].y;
	}
}

void renderNinjamonEffect(int32_t instanceId)
{
	LocalMapObjectInstance *data;
	LocalMapObjectInstance *base;
	int32_t i;

	base = LOCAL_MAP_OBJECT_INSTANCE;
	data = base;
	for (i = 0; i < 0x29; i++) {
		data->x += MAIN_D_8013CAE0[i];
		data->y += MAIN_D_8013CB0C[i];
		if (data->x > 0xa0) {
			data->x = MAIN_D_8013CA38[i];
			data->y = MAIN_D_8013CA8C[i];
			MAIN_D_8013CAE0[i] = random(10) + 12;
			MAIN_D_8013CB0C[i] = random(10) + 3;
		}
		data->orderValue = 10;
		data++;
	}
	NPC_ACTIVE_ANIM++;
	if (NPC_ACTIVE_ANIM >= 0x3c) {
		data = base;
		i = 0;
		while (i < 0x29) {
			data->flag = 1;
			i++;
			data++;
		}
		removeObject(0xfba, 0);
	}
}

void loadMapObjectPosition(int16_t *xData, int16_t *yData, uint8_t startIndex,
			   int32_t count)
{
	long idx;
	int32_t i;

	for (i = 0; i < count; i++) {
		idx = startIndex + i;
		LOCAL_MAP_OBJECT_INSTANCE[idx].x = *xData++;
		LOCAL_MAP_OBJECT_INSTANCE[idx].y = *yData++;
		LOCAL_MAP_OBJECT_INSTANCE[idx].flag &= ~0x10;
	}
}

int32_t moveMapObjectsWithLimit(uint8_t startIndex, int32_t count, int16_t dx,
				int16_t dy, int16_t limitX, int16_t limitY)
{
	long idx;
	int32_t i;

	for (i = 0; i < count; i++) {
		idx = startIndex + i;
		LOCAL_MAP_OBJECT_INSTANCE[idx].x += dx;
		LOCAL_MAP_OBJECT_INSTANCE[idx].y += dy;
	}
	if (dx > 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[startIndex].x >= limitX) {
			return 1;
		}
	} else if (dx < 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[startIndex].x <= limitX) {
			return 1;
		}
	}
	if (dy > 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[startIndex].y >= limitY) {
			return 1;
		}
	} else if (dy < 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[startIndex].y <= limitY) {
			return 1;
		}
	}
	return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/overworld", tickMoveObjectTo);

void moveMapObjects(uint8_t startIndex, int32_t count, int16_t dx, int16_t dy)
{
	long idx;
	int32_t i;

	for (i = 0; i < count; i++) {
		idx = startIndex + i;
		LOCAL_MAP_OBJECT_INSTANCE[idx].x += dx;
		LOCAL_MAP_OBJECT_INSTANCE[idx].y += dy;
	}
}

void setMapObjectsFlag(uint8_t startIndex, int32_t count, int32_t flag)
{
	long idx;
	int32_t i;

	for (i = 0; i < count; i++) {
		idx = startIndex + i;
		LOCAL_MAP_OBJECT_INSTANCE[idx].flag = flag;
	}
}

void getDrawPosition(SVECTOR *worldPos, int16_t *outX, int16_t *outY)
{
	int16_t screen[4];
	int16_t camX;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	gte_ldv0(worldPos);
	gte_rtps();
	gte_stsxy(screen);
	*outX = DRAWING_OFFSET_X + (screen[0] + (camX = CAMERA_X[0]));
	*outY = DRAWING_OFFSET_Y + (screen[1] + CAMERA_Y[0]);
}

INCLUDE_ASM("asm/main/nonmatchings/overworld", spawnSpriteAtLocation);

void spawnSpriteAtEntity(int32_t scriptId, int32_t nodeId, int32_t sprite)
{
	Entity *entity;
	MATRIX *m;

	entity = getEntityFromScriptId((uint8_t *)&scriptId);
	m = &entity->posData[nodeId].posMatrix.workm;
	spawnSpriteAtLocation(m->t[0], m->t[1], m->t[2], sprite, 1);
}

void resetMapObjectAnimation(uint8_t startIndex, int32_t count)
{
	long idx;
	int32_t i;

	for (i = 0; i < count; i++) {
		idx = startIndex + i;
		LOCAL_MAP_OBJECT_INSTANCE[idx].timer = 0;
		LOCAL_MAP_OBJECT_INSTANCE[idx].currentFrame = 0;
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

INCLUDE_ASM("asm/main/nonmatchings/overworld", loadMapDigimon);

void loadNPCModel(int32_t digimonId)
{
	thunkLoadMMD(digimonId, 0);
}

INCLUDE_ASM("asm/main/nonmatchings/overworld", scriptSetDigimon);

void tickNPC(int32_t instanceId)
{
	if (ENTITY_TABLE[instanceId]->isOnMap == 0) {
		return;
	}

	switch (GAME_STATE) {
	case 0:
		tickNPCOverworld(instanceId, &MAP_DIGIMON_TABLE[instanceId - 2]);
		break;
	case 1:
	case 2:
	case 3:
		tickNPCBattle(instanceId);
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

void unloadDigimonModel(int32_t digimonType)
{
	thunkUnloadModel(digimonType, 0);
}

void clearMapDigimon(void)
{
	int32_t i;
	int32_t j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			MAP_DIGIMON_TABLE[i].aiSections[j] = -1;
			MAP_DIGIMON_TABLE[i].waypoints[j].vx = 0;
			MAP_DIGIMON_TABLE[i].waypoints[j].vy = 0;
			MAP_DIGIMON_TABLE[i].waypoints[j].vz = 0;
		}
		MAP_DIGIMON_TABLE[i].activeSection = 0;
		ENTITY_TABLE[i + 2] = NULL;
		ENTITY_TABLE[i + 2]->isOnMap = 0;
		MAP_DIGIMON_TABLE[i].typeId = -1;
		MAP_DIGIMON_TABLE[i].waypointWaitTimer = 0;
		MAP_DIGIMON_TABLE[i].cwDiff = 0;
		MAP_DIGIMON_TABLE[i].ccDiff = 0;
		MAP_DIGIMON_TABLE[i].targetAngle = 0;
		MAP_DIGIMON_TABLE[i].hasWaypointTarget = 0;
		MAP_DIGIMON_TABLE[i].lookAtTamerState = 0;
		MAIN_D_80134D20[i] = 0;
	}
}

INCLUDE_ASM("asm/main/nonmatchings/overworld", removeMapEntities);

void clearMapAITable(int32_t index)
{
	int32_t i;

	if (index != -1) {
		MAP_DIGIMON_TABLE[index].lookAtTamerState =
			MAP_DIGIMON_TABLE[index].hasWaypointTarget = 0;
		MAP_DIGIMON_TABLE[index].activeSection = 0;
	} else {
		for (i = 0; i < 8; i++) {
			MAP_DIGIMON_TABLE[i].hasWaypointTarget = 0;
			MAP_DIGIMON_TABLE[i].lookAtTamerState = 0;
			MAP_DIGIMON_TABLE[i].activeSection = 0;
		}
	}
}

void tickNPCOverworld(int32_t instanceId, MapDigimonEntity *mapDigimon)
{
	Entity *entity;

	if ((entity = ENTITY_TABLE[instanceId]) == NULL) {
		return;
	}
	if (IS_IN_MENU == 1) {
		return;
	}

	if (MAIN_D_80134D20[instanceId - 2] != 0) {
	} else if (mapDigimon->stopAnim == 0) {
		if (mapDigimon->lookAtTamerState == 0) {
			tickWaypointAI(mapDigimon, entity, instanceId);
		}

		switch (mapDigimon->followMode) {
		case 2:
		case 11:
			if ((isInTrackingRect(mapDigimon, &TAMER_ENTITY.entity.posData->location) == 1) ||
			    (mapDigimon->lookAtTamerState != 0)) {
				tickLookingAtTamer(mapDigimon, entity,
						   &TAMER_ENTITY);
			}
			break;
		case 3:
		case 4:
		case 5:
		case 12:
		case 13:
		case 14:
			if ((isInTrackingRect(mapDigimon, &TAMER_ENTITY.entity.posData->location) == 1) ||
			    (mapDigimon->lookAtTamerState != 0)) {
				tickTrackingTamer(mapDigimon, entity,
						  &TAMER_ENTITY, instanceId);
			}
			break;
		case 6:
		case 15:
			if ((isInTrackingRadius(&NPC_ENTITIES[instanceId - 2].digimonEntity.entity,
						&TAMER_ENTITY.entity, mapDigimon) == 1) ||
			    (mapDigimon->lookAtTamerState != 0)) {
				tickTrackingTamer2(mapDigimon, entity,
						   &TAMER_ENTITY,
						   instanceId, 2);
			}
			break;
		case 7:
		case 16:
			if ((isInTrackingRadius(&NPC_ENTITIES[instanceId - 2].digimonEntity.entity,
						&TAMER_ENTITY.entity, mapDigimon) == 1) ||
			    (mapDigimon->lookAtTamerState != 0)) {
				tickTrackingTamer2(mapDigimon, entity,
						   &TAMER_ENTITY,
						   instanceId, 4);
			}
			break;
		case 8:
		case 9:
		case 17:
		case 18:
			if (isInTrackingRect(
				    mapDigimon,
				    &TAMER_ENTITY.entity.posData->location) == 1 ||
			    mapDigimon->lookAtTamerState != 0) {
				tickTrackingTamer2(mapDigimon, entity, &TAMER_ENTITY,
						   instanceId, 2);
			}
			break;
		}

		NPC_COLLISION_STATE[instanceId - 2] =
			entityCheckCollision(NULL, entity, 0, 0);
		if ((NPC_COLLISION_STATE[instanceId - 2] == 0) &&
		    (getTamerState() == 0) &&
		    (NPC_ENTITIES[instanceId - 2].autotalk == 1)) {
			entity->anim.animFlag |= 2;
			if (IS_SCRIPT_PAUSED == 1) {
				removeTriangleMenu();
				closeInventoryBoxes();
				removeUIBox1();
				TALKED_TO_ENTITY = instanceId;
				callScriptSection(CURRENT_SCRIPT_ID, NPC_ENTITIES[instanceId - 2].scriptId, 1);
			}
		}
		if ((NPC_COLLISION_STATE[instanceId - 2] != -1) &&
		    (NPC_IS_WALKING_TOWARDS[instanceId - 2] == 0) &&
		    (entity->anim.animId > 1 && entity->anim.animId < 5)) {
			collisionGrace(0, entity, 0, 0);
		}
	} else {
		if (mapDigimon->animation != 0) {
			mapDigimon->animation = 0;
			startAnimation(entity, mapDigimon->animation);
		}
	}

	entity->isOnScreen = entityIsOffScreen(entity, 0x140, 0xf0) ^ 1;
	tickAnimation(entity);
}

void tickWaypointAI(MapDigimonEntity *mapDigimon, Entity *entity,
		    int32_t instanceId)
{
	switch (mapDigimon->aiSections[mapDigimon->activeSection]) {
	case 0:
		tickWaypointWait(mapDigimon, entity);
		break;
	case 1:
		tickWaypointWalk(mapDigimon, entity, 2, instanceId);
		break;
	case 2:
		tickWaypointWalk(mapDigimon, entity, 4, instanceId);
		break;
	}

	if (mapDigimon->activeSection >= 8 ||
	    mapDigimon->aiSections[mapDigimon->activeSection] == -1) {
		mapDigimon->activeSection = 0;
	}
}

int32_t isInTrackingRect(MapDigimonEntity *mapDigimon, VECTOR *location)
{
	if ((mapDigimon->posX + mapDigimon->trackingRange > location->vx) &&
	    (mapDigimon->posX - mapDigimon->trackingRange < location->vx) &&
	    (mapDigimon->posZ + mapDigimon->trackingRange > location->vz) &&
	    (mapDigimon->posZ - mapDigimon->trackingRange < location->vz)) {
		return 1;
	}
	return 0;
}

void tickLookingAtTamer(MapDigimonEntity *mapDigimon, Entity *entity,
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
		if (isInTrackingRect(mapDigimon, &TAMER_ENTITY.entity.posData->location) == 0) {
			mapDigimon->lookAtTamerState = 0;
			mapDigimon->hasWaypointTarget = 0;
		}
	}
}

void tickTrackingTamer(MapDigimonEntity *mapDigimon, Entity *entity,
		       TamerEntity *tamer, int32_t instanceId)
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
		tickTrackingTamer3(mapDigimon, entity, instanceId);
	}
}

int32_t isInTrackingRadius(Entity *entity, Entity *otherEntity,
			   MapDigimonEntity *mapDigimon)
{
	int32_t dx;
	int32_t dz;

	dx = entity->posData->location.vx - otherEntity->posData->location.vx;
	dz = entity->posData->location.vz - otherEntity->posData->location.vz;
	if ((dx + dz) < ((mapDigimon->trackingRange * 6 / 10) * (mapDigimon->trackingRange * 6 / 10))) {
		return 1;
	}
	return 0;
}

void tickTrackingTamer2(MapDigimonEntity *mapDigimon, Entity *entity,
			TamerEntity *tamer, int32_t instanceId,
			uint8_t animId)
{
	if (mapDigimon->lookAtTamerState == 0) {
		if (mapDigimon->animation != animId) {
			mapDigimon->animation = animId;
			startAnimation(entity, mapDigimon->animation);
		}
		mapDigimon->lookAtTamerState = 1;
	} else {
		tickTrackingTamer4(mapDigimon, entity, tamer, instanceId);
	}
}

void getRotationDifference(PositionData *posData, VECTOR *targetLoc,
			   int16_t *outAngle, int16_t *outCcDiff,
			   int16_t *outCwDiff)
{
	*outAngle = _atan((int16_t)(targetLoc->vz - posData->location.vz),
			  (int16_t)(targetLoc->vx - posData->location.vx));
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
	int16_t cw;
	int16_t cc;

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

void tickTrackingTamer3(MapDigimonEntity *mapDigimon, Entity *entity,
			int32_t instanceId)
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
			mapDigimon->hasWaypointTarget = 0;
			mapDigimon->lookAtTamerState = 0;
		}
		break;
	}
}

void tickTrackingTamer4(MapDigimonEntity *mapDigimon, Entity *entity,
			TamerEntity *tamer, int32_t instanceId)
{
	int16_t currentTileX;
	int16_t currentTileY;
	int16_t targetTileX;
	int16_t targetTileY;

	switch (mapDigimon->lookAtTamerState) {
	case 1:
		if (isInTrackingRadius(&NPC_ENTITIES[instanceId - 2].digimonEntity.entity,
				       &tamer->entity, mapDigimon) == 1) {
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
			mapDigimon->hasWaypointTarget = 0;
			mapDigimon->lookAtTamerState = 0;
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
		    mapDigimon->waypoints[mapDigimon->activeSection].vx) {
			mapDigimon->hasWaypointTarget = 0;
			mapDigimon->waypointWaitTimer = 0;
			mapDigimon->activeSection++;
		}
		break;
	}
}

void tickWaypointWalk(MapDigimonEntity *mapDigimon, Entity *entity,
		      int32_t animation, int32_t instanceId)
{
	int16_t currentTileX;
	int16_t currentTileY;
	int16_t targetTileX;
	int16_t targetTileY;

	switch (mapDigimon->hasWaypointTarget) {
	case 0:
		mapDigimon->targetLocation.vx =
			mapDigimon->waypoints[mapDigimon->activeSection].vx;
		mapDigimon->targetLocation.vy =
			mapDigimon->waypoints[mapDigimon->activeSection].vy;
		mapDigimon->targetLocation.vz =
			mapDigimon->waypoints[mapDigimon->activeSection].vz;
		if (mapDigimon->animation != animation) {
			mapDigimon->animation = animation;
			startAnimation(entity, mapDigimon->animation);
		}
		mapDigimon->hasWaypointTarget = 1;
		break;
	case 1:
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
			mapDigimon->hasWaypointTarget = 0;
			mapDigimon->activeSection++;
		}
		break;
	}
}

void setActiveAnim(uint8_t scriptId, uint8_t animId)
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
			MAIN_D_80134D20[i] = animId;
			return;
		}
		npc++;
	}
}

void startNPCAnimation(uint8_t scriptId, int32_t animId)
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

INCLUDE_ASM("asm/main/nonmatchings/overworld", resetEntityOrigin);

void setMovementEnabled(int32_t id, int32_t enabled)
{
	int32_t i;

	if (id != -1) {
		if (id == 0) {
			if (enabled == 0) {
				setTamerState(0);
			} else {
				setTamerState(6);
			}
		} else if (id == 1) {
			if (enabled == 0) {
				setPartnerState(1);
			} else {
				setPartnerState(11);
			}
		} else {
			MAP_DIGIMON_TABLE[id - 2].stopAnim = enabled;
			if (enabled == 1) {
				clearMapAITable((int16_t)(id - 2));
			}
		}
	} else {
		if (enabled == 0) {
			setTamerState(0);
			setPartnerState(1);
		} else {
			setTamerState(6);
			setPartnerState(11);
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

INCLUDE_ASM("asm/main/nonmatchings/overworld", addGameMenu);

void tickTriangleMenu(void)
{
	RECT rect;

	rect = MAIN_D_80134290;
	switch (TRIANGLE_MENU_STATE) {
	case 0:
		if (MAIN_D_80134D2C != 0) {
			createMenuBox(0, -0x42, -0x50, 0x84, 0x95, 2,
				      tickGameMenu, renderGameMenu);
		} else {
			createMenuBox(0, -0x42, -0x50, 0x84, 0x6e, 2,
				      tickGameMenu, renderGameMenu);
		}
		if (UI_BOX_DATA[0].frame == 4) {
			TRIANGLE_MENU_STATE = -1;
		}
		clearTextSubArea(&rect);
		drawString(MAIN_D_80134298, 0, 0xe8);
		break;
	case 1:
		closeUIBoxIfOpen(0);
		removeObject(0xfa4, 0);
		break;
	case 2:
		closeUIBoxIfOpen(0);
		if (UI_BOX_DATA[0].frame == 0) {
			initializeInventoryObject();
			TRIANGLE_MENU_STATE = -1;
		}
		break;
	case 3:
		closeUIBoxIfOpen(0);
		if (UI_BOX_DATA[0].frame == 0) {
			createMenuBox(1, -0x96, -0x59, 300, 0xbe, 0,
				      tickDigimonMenu, renderDigimonMenu);
			clearTextArea();
			drawString(MAIN_D_801247B8[0], 0, 0);
			MENU_STATE = 0;
			MENU_SUB_STATE = 0;
			TRIANGLE_MENU_STATE = -1;
			MAIN_D_80134D36 = 0;
		}
		break;
	case 4:
		TAMER_ENTITY.entity.isOnScreen = 1;
		PARTNER_ENTITY.digimonEntity.entity.isOnScreen = 1;
		closeUIBoxIfOpen(1);
		if (UI_BOX_DATA[1].frame == 0) {
			TRIANGLE_MENU_STATE = 0;
		}
		break;
	case 5:
		closeUIBoxIfOpen(0);
		if (UI_BOX_DATA[0].frame == 0) {
			createMenuBox(1, -0x96, -0x59, 300, 0xbe, 0,
				      tickPlayerMenu, renderPlayerMenu);
			clearTextArea();
			drawString(MAIN_D_80124800[0], 0, 0);
			MENU_STATE = 0;
			MENU_SUB_STATE = 0;
			TRIANGLE_MENU_STATE = -1;
			MAIN_D_80134D37 = 0;
		}
		break;
	case 6:
		TAMER_ENTITY.entity.isOnScreen = 1;
		PARTNER_ENTITY.digimonEntity.entity.isOnScreen = 1;
		closeUIBoxIfOpen(1);
		if (UI_BOX_DATA[1].frame == 0) {
			TRIANGLE_MENU_STATE = 0;
		}
	}
}

void closeTriangleMenu(void)
{
	closeUIBoxIfOpen(1);
	closeInventoryBoxes2();
	closeUIBoxIfOpen(0);
	removeObject(0xfa4, 0);
}

INCLUDE_ASM("asm/main/nonmatchings/overworld", closeUIBoxIfOpen);

INCLUDE_ASM("asm/main/nonmatchings/overworld", renderGameMenu);

INCLUDE_ASM("asm/main/nonmatchings/overworld", renderSeperatorLines);

INCLUDE_ASM("asm/main/nonmatchings/overworld", renderDateDigits);

INCLUDE_ASM("asm/main/nonmatchings/overworld", renderTriangleCursor);

INCLUDE_ASM("asm/main/nonmatchings/overworld", renderRectPolyFT4);

INCLUDE_ASM("asm/main/nonmatchings/overworld", tickGameMenu);

INCLUDE_ASM("asm/main/nonmatchings/overworld", createMenuBox);

INCLUDE_ASM("asm/main/nonmatchings/overworld", tickDigimonMenu);

void renderDigimonMenu(void)
{
  int8_t tabs[2];
  int32_t v;
  MenuTabPair *tp;
  long nv;
  tp = (MenuTabPair *)MAIN_D_801342A0;
  nv = tp->digimon;
  v = tp->moves;
  tabs[0] = nv;
  tabs[1] = v;
  v = MAIN_D_80134D36;
  if (v != 1) {
    if (v == 0) {
      renderDigimonStatsView();
    }
  } else {
    renderDigimonMovesView();
  }
  tabs[MAIN_D_80134D36] = 0;
  renderString(tabs[0], -0x8A, -((short) 0x65), 0x3C, 0xC, 0, 0, 5, 1);
  renderString(tabs[1], -0x3F, -0x65, 0x30, 0xC, 0x3C, 0, 5, 1);
  nv = tabs[0];
  renderMenuTab(-0x91, 0x4C, nv);
  renderMenuTab(-0x46, 0x40, tabs[1]);
}

INCLUDE_ASM("asm/main/nonmatchings/overworld", tickPlayerMenu);

INCLUDE_ASM("asm/main/nonmatchings/overworld", renderPlayerMenu);

INCLUDE_ASM("asm/main/nonmatchings/overworld", isUIBoxAvailable);

void handleGameMenuSelection(int32_t selection)
{
	switch (selection) {
	case 1:
		if (MAIN_D_80123E8E[selection * 12] & 1) {
			return;
		}
		TRIANGLE_MENU_STATE = 2;
		drawInventoryText();
		break;
	case 6:
		if (MAIN_D_80123E8E[selection * 12] & 1) {
			return;
		}
		closeTriangleMenu();
		setPartnerState(3);
		IS_IN_MENU = 0;
		startGameTime();
		break;
	case 5:
		if (MAIN_D_80123E8E[selection * 12] & 1) {
			return;
		}
		closeTriangleMenu();
		setPartnerState(4);
		IS_IN_MENU = 0;
		startGameTime();
		break;
	case 2:
		TRIANGLE_MENU_STATE = 3;
		break;
	case 3:
		TRIANGLE_MENU_STATE = 5;
		break;
	case 4:
		if (MAIN_D_80123E8E[selection * 12] & 1) {
			return;
		}
		closeTriangleMenu();
		setPartnerState(0xf);
		IS_IN_MENU = 0;
		startGameTime();
		break;
	case 7:
		if (MAIN_D_80123E8E[selection * 12] & 1) {
			return;
		}
		closeTriangleMenu();
		setCameraFollowPlayer();
		initializeFishing();
		setTamerState(0xb);
		IS_IN_MENU = 0;
	}
}

void setSleepDisabled(int32_t arg)
{
	GAME_MENU_SPRITES[0x42] = arg;
}

void startFeedingItem(int32_t arg)
{
	if (TAMER_ITEM.worldItem.type == 0xff) {
		TAMER_ITEM.worldItem.type = arg;
		setTamerState(6);
		setPartnerState(5);
		removeObject(0xfa4, 0);
		IS_IN_MENU = 0;
		startGameTime();
	}
}

void removeOneSelectedItem(void)
{
	removeItem(INVENTORY_ITEM_TYPES[INVENTORY_POINTER], 1);
}

void renderFeedingItem(int32_t arg)
{
	MATRIX *m;

	if (arg == 0) {
		m = &TAMER_ENTITY.entity.posData[9].posMatrix.workm;
	}

	TAMER_ITEM.worldItem.spriteLocation.vx = m->t[0];
	TAMER_ITEM.worldItem.spriteLocation.vy = m->t[1];
	TAMER_ITEM.worldItem.spriteLocation.vz = m->t[2];

	renderOverworldItem(&TAMER_ITEM);
}

INCLUDE_ASM("asm/main/nonmatchings/overworld", getEquippedSlot);

INCLUDE_ASM("asm/main/nonmatchings/overworld", equipMove);
