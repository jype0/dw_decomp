#include <string.h>

#include <inline_n.h>
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>

#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/fade.h>
#include <dw/file.h>
#include <dw/font.h>
#include <dw/graphics.h>
#include <dw/input.h>
#include <dw/item.h>
#include <dw/map.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/tamer.h>
#include <dw/types.h>
#include <dw/utils.h>
#include <dw/vecmath.h>
#include <dw/version.h>

typedef struct {
	int16_t cameraX;
	int16_t cameraY;
	int8_t width;
	int8_t height;
	int8_t pad[26];
} MapState;

long RotTransPers3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, long *sxy0, long *sxy1, long *sxy2, long *p, long *flag);
int abs(int x);
int32_t addObject(int32_t objectId, int16_t instanceId, void (*tick)(int32_t), void (*render)(int32_t));
void checkArenaMap(uint8_t mapId);
void checkCurlingMap(int32_t mapId);
void checkFishingMap(int32_t mapId, int32_t arg1);
int32_t checkMapCollisionX(Entity *entity, int32_t direction);
int32_t checkMapCollisionZ(Entity *entity, int32_t direction);
void checkShopMap(uint8_t mapId);
void clearMapDigimon(void);
void entityLookAtTile(Entity *entity, int32_t tileX, int32_t tileY);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void handleBattleIdle(DigimonEntity *entity, Stats *stats, int32_t flags);
void initializeLoadedNPCModels(void);
int32_t isInvisible(Entity *entity);
void loadDoors(int32_t doorEntryId);
void loadMapCollisionData(uint8_t *data);
void loadMapImage1(uint8_t *tim);
void loadMapImage2(uint8_t *tim, int8_t id);
void loadMapObjects(LocalMapObjectInstance *mapObjects, uint8_t *data, int32_t mapId);
int32_t loadMapSounds(int32_t mapSoundId);
void initializeTrainingPoop(void);
void loadWarpCrystals(int16_t mapId);
int32_t readFile(char *path, void *dest);
void removeMapEntities(void);
int32_t removeObject(int32_t objectId, int16_t instanceId);
void renderPoop(int32_t instanceId);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void runMapHeadScript(int32_t section);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uPos, int32_t vPos, int32_t width, int32_t height);
void startBattleIdleAnimation(DigimonEntity *entity, Stats *stats, int32_t flags);
void tickFileReadQueue(int32_t instanceId);
void tickConditions();
void unloadMapParts(void);

void popPartnerWaypoint(void);
void getClosestTileOffScreen(int8_t *fromX, int8_t *fromY, int8_t *toX, int8_t *toY);
void initializePartnerWaypoint(void);
void initializeTamerWaypoints(void);
void clearTamerWaypoints(void);
void initializeMap(void);
void setupMap(void);
void updateDrawingOffsets(DVECTOR *current, DVECTOR *previous);
void moveCameraByOffset(int32_t diffX, int32_t diffY);
int32_t scriptTickChangeMap(int16_t mapId, int16_t exitId, int32_t showName);
void setDeathMap(int16_t a, int16_t b);
int32_t waitForDeathMapLoading(int32_t flag);
void changeToDeathMap(void);
void reinitializeAfterTournament(void);
void loadTrainingLibrary(int32_t mapId);
void getRViewCopy(GsRVIEW2 *out);
void downloadCLUT1(uint32_t *src);
void fadeoutCLUT1(long level, int16_t *clut);
void downloadCLUT2(u_long *buffer);
void fadeoutCLUT2(long fade, int16_t *clut);
int32_t addPolyFT3Prim(POLY_FT3 *prim, int32_t order);
void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
void renderTMDModel(uint8_t *buffer, int32_t id, GsCOORDINATE2 *coord, GsCOORDINATE2 *super, VECTOR *trans, SVECTOR *rot, VECTOR *scale);
void initializeSomeArenaArrays(int32_t count, int32_t arg1, int32_t arg2, int32_t *out);
int32_t processSomeArenaArrays(int32_t count, int32_t t, int32_t *keys, int32_t *values, int32_t *slopes);
int32_t doSomethingWithSomePoints(int16_t *rect, DVECTOR *line);
int16_t DOOA_storeDigimonY(void);
int16_t DOOA_getStoredDigimonY(void);
int32_t addScreenPolyFT4(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
void addFXPrim(POLY_FT4 *prim, int16_t x, int16_t y, int16_t width, int16_t height, int32_t depth);
void addMapNameObject(int32_t mapId);
void setPartnerWaypoint(int16_t index, int16_t x, int16_t y);
void addTamerWaypoint(int16_t index, int16_t x, int16_t y);
void buildMapPath(char *out, char *name, int8_t *suffix, int32_t mapId);
void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out);
void cameraIsAtEdge(int32_t *atEdgeX, int32_t *atEdgeY);
void changeMap(int16_t a, int16_t b);
int32_t checkCameraMovement(int32_t arg0);
int32_t checkMapCollision(Entity *entity, int32_t diffY, int32_t diffX);
void cleanupGame(void);
void collisionGrace(Entity *a, Entity *entity, int32_t c, int32_t d);
void createCameraMovement(VECTOR *target, int32_t instanceId);
void createMeramonShake(void);
int16_t entityCheckCollision(Entity *a, Entity *entity, int32_t c, int32_t d);
int32_t entityCheckCombatArea(Entity *entity, VECTOR *target, int32_t w, int32_t h);
int32_t entityCheckEntityCollision(Entity *entity, Entity *other, int32_t diffX, int32_t diffZ);
int32_t entityIsInEntity(Entity *a, Entity *b);
int32_t entityIsOffScreen(Entity *entity, int32_t width, int32_t height);
void entityLookAtLocation(Entity *entity, VECTOR *location);
void entityMoveForward(Entity *entity);
void fillTileData(MapTileData *tile, uint8_t *imagePtr, int16_t texU, int16_t texV, int16_t posX, int16_t posY);
void getDrawingOffsetCopy(int32_t *x, int32_t *y);
void getEntityTile(Entity *entity, int8_t *outTileX, int8_t *outTileY);
int32_t getFileCityTopMap(void);
int32_t getMapSoundId(int16_t mapId);
int32_t getOriginalType(int32_t type);
void getViewportDistanceCopy(int32_t *out);
void handleTileUpdate(int32_t input, int32_t force);
int32_t hasMovedOutsideCombatArea(DVECTOR *a, DVECTOR *b, int16_t w, int16_t h);
void initializeDaytimeTransition(int32_t timeOfDay);
int32_t isFiveTileWidePathBlocked(int8_t x1, int8_t y1, int8_t x2, int8_t y2);
int32_t isInDaytimeTransition(void);
int32_t isLinearPathBlocked(int8_t x1, int8_t y1, int8_t x2, int8_t y2);
int32_t isOffScreen(DVECTOR *xy, int16_t w, int16_t h);
int32_t isRectInRect(RECT *rect, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
int32_t isTileOffScreen(int8_t tileX, int8_t tileZ);
int32_t lerp(int32_t start, int32_t end, int32_t t0, int32_t t1, int32_t t);
void loadMap(int16_t mapId);
int32_t loadMapSetup(int32_t *data);
void moveCameraByDiff(VECTOR *from, VECTOR *to);
void readMapTFS(int32_t mapId);
void renderFXParticle(SVECTOR *pos, int32_t size, RGB8 *color);
void renderMap(int32_t arg0);
void renderMapName(int32_t instanceId);
void renderSprite(GsSPRITE *sprite, int16_t x, int16_t y, int32_t distance, int32_t width, int32_t height);
void setCameraFollowPlayer(void);
void setPosDataMapTile(MapTileData *tile, int16_t camX, int16_t camY, POLY_FT4 *prim);
void setInt16WithStride(int16_t *dest, int16_t value, int16_t count, int16_t stride);
void storeEntityLocation(int32_t scriptId, VECTOR *out);
void tickCameraFollowPlayer(void);
void tickCameraMovement(int32_t instanceId);
void tickDaytimeTransition(int32_t instanceId);
void tickMeramonShake(int32_t arg0);
void tickCameraMoveTo(int32_t x, int32_t z, int32_t arg2);
int32_t tickCameraMoveToEntity(int32_t scriptId, int32_t speed);
void partnerTickCollision(void);
void tickPartnerWaypoints(void);
void tickTamerWaypoints(void);
void translateConditionFXToEntity(Entity *entity, SVECTOR *out);
void unloadMap(void);
void unsetCameraFollowPlayer(void);
void updateTileColumn(int32_t right);
void updateTileRow(int32_t bottom);
void updateTimeOfDay(void);
void uploadMapTileImages(MapTileData *tiles, int16_t index);
int32_t worldPosToScreenPos(SVECTOR *pos, DVECTOR *out);

extern GsOT *ACTIVE_ORDERING_TABLE;
extern int32_t COMBAT_AREA_X;
extern int32_t COMBAT_AREA_Y;
extern uint16_t CURRENT_FRAME;
extern long DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_X_COPY;
extern long DRAWING_OFFSET_Y;
extern int32_t DRAWING_OFFSET_Y_COPY;
extern Entity *ENTITY_TABLE[];
extern int16_t FADE_OUT_CURRENT;
extern int8_t GAME_STATE;
extern uint8_t *GENERAL_BUFFER_PTR;
extern GsRVIEW2 GS_VIEWPOINT;
extern GsRVIEW2 GS_VIEWPOINT_COPY;
extern MATRIX GsWSMATRIX;
extern int16_t HOUR;
extern int32_t IS_SCRIPT_PAUSED;
extern int32_t LOADED_DIGIMON_MODELS[];
extern int16_t DOOA_STORED_DIGIMON_Y;
extern int8_t MAP_COLLISION_DATA[];
extern int8_t PARTNER_STATE;
extern int8_t PARTNER_TAMER_PREVIOUS_TILE_X;
extern int8_t PARTNER_TAMER_PREVIOUS_TILE_Y;
extern int8_t PARTNER_WAYPOINT_COUNT;
extern int8_t PARTNER_WAYPOINT_CURRENT;
extern int8_t PARTNER_WAYPOINT_X[];
extern int8_t PARTNER_WAYPOINT_Y[];
extern int32_t POLLED_INPUT;
extern int32_t POLLED_INPUT_PREVIOUS;
extern VECTOR STORED_TAMER_POS;
extern TamerEntity TAMER_ENTITY;
extern int8_t TAMER_PREVIOUS_TILE_X;
extern int8_t TAMER_PREVIOUS_TILE_Y;
extern int8_t TAMER_START_TILE_X;
extern int8_t TAMER_START_TILE_Y;
extern int8_t TAMER_WAYPOINT_ACTIVE;
extern int8_t TAMER_WAYPOINT_COUNT;
extern int8_t TAMER_WAYPOINT_CURRENT;
extern int8_t TAMER_WAYPOINT_X[];
extern int8_t TAMER_WAYPOINT_Y[];
extern int32_t TRAINING_COMPLETE;
extern int32_t VIEWPORT_DISTANCE;
extern int32_t VIEWPORT_DISTANCE_COPY;

static void *map_functions[] = {
	renderFXParticle,
	DOOA_getStoredDigimonY,
	DOOA_storeDigimonY,
	doSomethingWithSomePoints,
	processSomeArenaArrays,
	initializeSomeArenaArrays,
	renderTMDModel,
	setInt16WithStride,
	addFXPrim,
	renderSprite,
	worldPosToScreenPos,
	addScreenPolyFT4,
	addScreenPolyFT3,
	addPolyFT3Prim,
	fadeoutCLUT2,
	downloadCLUT2,
	fadeoutCLUT1,
	downloadCLUT1,
	calculateBoneMatrix,
	getOriginalType,
	translateConditionFXToEntity,
	lerp,
	getFileCityTopMap,
	cleanupGame,
	getDrawingOffsetCopy,
	getViewportDistanceCopy,
	getRViewCopy,
	tickMeramonShake,
	createMeramonShake,
	loadTrainingLibrary,
	reinitializeAfterTournament,
	renderMapName,
	changeToDeathMap,
	waitForDeathMapLoading,
	setDeathMap,
	changeMap,
	addMapNameObject,
	scriptTickChangeMap,
	updateTileColumn,
	updateTileRow,
	setCameraFollowPlayer,
	unsetCameraFollowPlayer,
	moveCameraByOffset,
	moveCameraByDiff,
	tickCameraMoveToEntity,
	tickCameraMoveTo,
	checkCameraMovement,
	storeEntityLocation,
	tickCameraMovement,
	createCameraMovement,
	cameraIsAtEdge,
	handleTileUpdate,
	updateDrawingOffsets,
	isInDaytimeTransition,
	tickDaytimeTransition,
	setPosDataMapTile,
	initializeDaytimeTransition,
	tickCameraFollowPlayer,
	unloadMap,
	uploadMapTileImages,
	initializeDrawingOffsets,
	fillTileData,
	updateTimeOfDay,
	setupMap,
	readMapTFS,
	getMapSoundId,
	loadMapSetup,
	buildMapPath,
	loadMap,
	renderMap,
	initializeMap,
	isOffScreen,
	entityIsOffScreen,
	entityCheckEntityCollision,
	checkMapCollision,
	entityMoveForward,
	tickPartnerWaypoints,
	tickTamerWaypoints,
	collisionGrace,
	entityCheckCollision,
	entityLookAtLocation,
	setPartnerWaypoint,
	partnerTickCollision,
	isLinearPathBlocked,
	isFiveTileWidePathBlocked,
	getEntityTile,
	clearTamerWaypoints,
	initializeTamerWaypoints,
	initializePartnerWaypoint,
	hasMovedOutsideCombatArea,
	isRectInRect,
	entityCheckCombatArea,
	getClosestTileOffScreen,
	addTamerWaypoint,
	entityIsInEntity,
	popPartnerWaypoint,
	isTileOffScreen,
};
// clang-format off
int8_t QUADRANTS[4][2] = {
	{ 0xff, 0x01 },
	{ 0x01, 0x01 },
	{ 0xff, 0xff },
	{ 0x01, 0xff },
};

int16_t COLLISION_GRACE_ROTATION[8][4] = {
	{ 0x0000, 0x0400, 0x0c00, 0x0800 },
	{ 0x0400, 0x0000, 0x0800, 0x0c00 },
	{ 0x0400, 0x0800, 0x0000, 0x0c00 },
	{ 0x0800, 0x0400, 0x0c00, 0x0000 },
	{ 0x0800, 0x0c00, 0x0400, 0x0000 },
	{ 0x0c00, 0x0800, 0x0000, 0x0400 },
	{ 0x0c00, 0x0000, 0x0800, 0x0400 },
	{ 0x0000, 0x0c00, 0x0400, 0x0800 },
};

#if VERSION_IS(EU)
char STR_MAP_NAME_NATIVE_FOREST[] = "Ｎａｔｉｖｅ　Ｆｏｒｅｓｔ";

char STR_MAP_NAME_COELA_POINT[] = "Ｃｏｅｌａ　Ｐｏｉｎｔ";

char STR_MAP_NAME_DRAGON_EYE_LAKE[] = "Ｄｒａｇｏｎ　Ｅｙｅ　Ｌａｋｅ";

char STR_MAP_NAME_DRILL_TUNNEL_ENTRANCE[] = "Ｄｒｉｌｌ　Ｔｕｎｎｅｌ　Ｅｎｔｒａｎｃｅ";

char STR_MAP_NAME_DIGIMON_BRIDGE[] = "Ｄｉｇｉｍｏｎ　Ｂｒｉｄｇｅ　";

char STR_MAP_NAME_TROPICAL_JUNGLE[] = "Ｔｒｏｐｉｃａｌ　Ｊｕｎｇｌｅ";

char STR_MAP_NAME_MANGROVE_REGION[] = "Ｍａｎｇｒｏｖｅ　Ｒｅｇｉｏｎ";

char STR_MAP_NAME_PATH_THRU_MT_PANORAMA[] = "Ｐａｔｈ　Ｔｈｒｕ　Ｍｔ．　Ｐａｎｏｒａｍａ";

char STR_MAP_NAME_ENTRANCE_TO_FILE_CITY[] = "Ｆｉｌｅ　Ｃｉｔｙ";

char STR_MAP_NAME_MT_PANORAMA_PLAINS[] = "Ｍｔ．　Ｐａｎｏｒａｍａ　Ｐｌａｉｎｓ";

char STR_MAP_NAME_FOOT_OF_MT_PANORAMA[] = "Ｆｏｏｔ　ｏｆ　Ｍｔ．　Ｐａｎｏｒａｍａ";

char STR_MAP_NAME_MT_PANORAMA_SPORE_AREA[] = "Ｍｔ．　Ｐａｎｏｒａｍａ　Ｓｐｏｒｅ　Ａｒｅａ";

char STR_MAP_NAME_DRILL_TUNNEL[] = "Ｄｒｉｌｌ　Ｔｕｎｎｅｌ";

char STR_MAP_NAME_DRILL_TUNNEL_2ND_FLOOR[] = "Ｄｒｉｌｌ　Ｔｕｎｎｅｌ　２ｎｄ　ｆｌｏｏｒ";

char STR_MAP_NAME_DRILL_TUNNEL_3RD_FLOOR[] = "Ｄｒｉｌｌ　Ｔｕｎｎｅｌ　３ｒｄ　ｆｌｏｏｒ";

char STR_MAP_NAME_RESIDENTIAL_AREA[] = "Ｒｅｓｉｄｅｎｔｉａｌ　Ａｒｅａ";

char STR_MAP_NAME_UNDERGROUND_POND[] = "Ｕｎｄｅｒｇｒｏｕｎｄ　Ｐｏｎｄ　";

char STR_MAP_NAME_LAVA_CAVE[] = "Ｌａｖａ　Ｃａｖｅ";

char STR_MAP_NAME_OVERDELL[] = "Ｏｖｅｒｄｅｌｌ";

char STR_MAP_NAME_OVERDELL_CEMETERY[] = "Ｏｖｅｒｄｅｌｌ　Ｃｅｍｅｔｅｒｙ";

char STR_MAP_NAME_GREAT_CANYON_ENTRANCE[] = "Ｇｒｅａｔ　Ｃａｎｙｏｎ　Ｅｎｔｒａｎｃｅ";

char STR_MAP_NAME_GREAT_CANYON_TOP_AREA[] = "Ｇ　Ｃａｎｙｏｎ　Ｔｏｐ　Ａｒｅａ";

char STR_MAP_NAME_GREAT_CANYON_BRIDGE[] = "Ｇｒｅａｔ　Ｃａｎｙｏｎ　Ｂｒｉｄｇｅ";

char STR_MAP_NAME_FORTRESS_ENTRANCE[] = "Ｆｏｒｔｒｅｓｓ　Ｅｎｔｒａｎｃｅ";

char STR_MAP_NAME_GREAT_CANYON_BOT_AREA[] = "Ｇｒｅａｔ　Ｃａｎｙｏｎ　Ｂｏｔ．　Ａｒｅａ";

char STR_MAP_NAME_OGRE_FORTRESS[] = "Ｏｇｒｅ　Ｆｏｒｔｒｅｓｓ";

char STR_MAP_NAME_MONOCHROME_SHOP[] = "Ｍｏｎｏｃｈｒｏｍｅ　Ｓｈｏｐ";

char STR_MAP_NAME_GREY_LORDS_MANSION[] = "Ｇｒｅｙ　Ｌｏｒｄ’ｓ　Ｍａｎｓｉｏｎ　";

char STR_MAP_NAME_MANSION_BASEMENT[] = "Ｇｒｅｙ　Ｌｏｒｄ’ｓ　Ｍａｎｓｉｏｎ";

char STR_MAP_NAME_UNDERGROUND_LAB[] = "Ｕｎｄｅｒｇｒｏｕｎｄ　Ｌａｂ";

char STR_MAP_NAME_GEAR_SAVANNA[] = "Ｇｅａｒ　Ｓａｖａｎｎａ";

char STR_MAP_NAME_ANCIENT_DINO_REGION[] = "Ａｎｃｉｅｎｔ　ＤｉｎｏＲｅｇｉｏｎ";

char STR_MAP_NAME_ANCIENT_GLACIAL_REGION[] = "Ａｎｃｉｅｎｔ　Ｇｌａｃｉａｌ　Ｒｅｇｉｏｎ";

char STR_MAP_NAME_ANCIENT_SPEEDY_REGION[] = "Ａｎｃｉｅｎｔ　Ｓｐｅｅｄｙ　Ｒｅｇｉｏｎ";

char STR_MAP_NAME_FREEZELAND[] = "Ｆｒｅｅｚｅｌａｎｄ";

char STR_MAP_NAME_ICE_SANCTUARY[] = "Ｉｃｅ　Ｓａｎｃｔｕａｒｙ";

char STR_MAP_NAME_GREEN_GYM[] = "Ｇｒｅｅｎ　Ｇｙｍ";

char STR_MAP_NAME_LEOMON_ANCESTORS_CAVE[] = "Ｌｅｏｍｏｎ’ｓ　ａｎｃｅｓｔｏｒ’ｓ　ｃａｖｅ";

char STR_MAP_NAME_MISTY_TREES[] = "Ｍｉｓｔｙ　Ｔｒｅｅｓ";

char STR_MAP_NAME_GREAT_CANYON[] = "Ｇｒｅａｔ　Ｃａｎｙｏｎ";

char STR_MAP_NAME_GEKO_SWAMP[] = "Ｇｅｋｏ　Ｓｗａｍｐ";

char STR_MAP_NAME_VOLUME_VILLA[] = "Ｖｏｌｕｍｅ　Ｖｉｌｌａ";

char STR_MAP_NAME_ITEM_KEEPER[] = "Ｉｔｅｍ　Ｋｅｅｐｅｒ";

char STR_MAP_NAME_CENTAR_CLINIC[] = "Ｃｅｎｔａｒ　Ｃｌｉｎｉｃ";

char STR_MAP_NAME_RESTAURANT[] = "Ｒｅｓｔａｕｒａｎｔ";

char STR_MAP_NAME_ITEM_SHOP[] = "Ｉｔｅｍ　Ｓｈｏｐ";

char STR_MAP_NAME_JIJIMONS_HOUSE[] = "Ｊｉｊｉｍｏｎ’ｓ　ｈｏｕｓｅ";

char STR_MAP_NAME_SECRET_ITEM_SHOP[] = "Ｓｅｃｒｅｔ　Ｉｔｅｍ　Ｓｈｏｐ";

char STR_MAP_NAME_TOY_TOWN[] = "Ｔｏｙ　Ｔｏｗｎ";

char STR_MAP_NAME_SECRET_BEACH_CAVE[] = "Ｓｅｃｒｅｔ　Ｂｅａｃｈ　Ｃａｖｅ";

char STR_MAP_NAME_FACTORIAL_TOWN[] = "Ｆａｃｔｏｒｉａｌ　Ｔｏｗｎ";

char STR_MAP_NAME_BIRDRA_TRANSPORT[] = "Ｂｉｒｄｒａ　Ｔｒａｎｓｐｏｒｔ";

char STR_MAP_NAME_ARENA_LOBBY[] = "Ａｒｅｎａ　Ｌｏｂｂｙ";

char STR_MAP_NAME_TREASURE_HUNT[] = "Ｔｒｅａｓｕｒｅ　Ｈｕｎｔ";

char STR_MAP_NAME_TRASH_MOUNTAIN[] = "Ｔｒａｓｈ　Ｍｏｕｎｔａｉｎ";

char STR_MAP_NAME_SEWER[] = "ｓｅｗｅｒ";

char STR_MAP_NAME_BEETLE_LAND[] = "Ｂｅｅｔｌｅ　Ｌａｎｄ";

char STR_MAP_NAME_MT_INFINITY[] = "Ｍｔ．　Ｉｎｆｉｎｉｔｙ";

char STR_MAP_NAME_DIGIMON_CURLING[] = "Ｄｉｇｉｍｏｎ　Ｃｕｒｌｉｎｇ　";

char STR_MAP_NAME_TOY_MANSION[] = "Ｔｏｙ　Ｍａｎｓｉｏｎ";

char STR_MAP_NAME_COSTUME_HOUSE[] = "Ｃｏｓｔｕｍｅ　ｈｏｕｓｅ";

char STR_MAP_NAME_ROBOT_HOUSE[] = "Ｒｏｂｏｔ　Ｈｏｕｓｅ";

char STR_MAP_NAME_MANSION_DOT_BASEMENT[] = "Ｍａｎｓｉｏｎ　Ｂａｓｅｍｅｎｔ";

char STR_MAP_NAME_BACK_DIMENSION[] = "Ｂａｃｋ　Ｄｉｍｅｎｓｉｏｎ";

char STR_MAP_NAME_KUNEMONS_BED[] = "Ｋｕｎｅｍｏｎ’ｓ　Ｂｅｄ";

char STR_MAP_NAME_AMIDA_FOREST[] = "Ａｍｉｄａ　Ｆｏｒｅｓｔ";

#define STR_MAP_NAME_FILE_CITY STR_MAP_NAME_ENTRANCE_TO_FILE_CITY
#define STR_MAP_NAME_MANSION_2ND_FLOOR STR_MAP_NAME_MANSION_BASEMENT
#define STR_MAP_NAME_MANSION_ATTIC STR_MAP_NAME_MANSION_BASEMENT
#elif !VERSION_IS(US)
char STR_MAP_NAME_NATIVE_FOREST[] = "迷わずの森";

char STR_MAP_NAME_COELA_POINT[] = "シーラ岬";

char STR_MAP_NAME_DRAGON_EYE_LAKE[] = "竜の目の湖ほとり";

char STR_MAP_NAME_DRILL_TUNNEL_ENTRANCE[] = "ドリルトンネル入り口";

char STR_MAP_NAME_DIGIMON_BRIDGE[] = "デジブリッジ";

char STR_MAP_NAME_TROPICAL_JUNGLE[] = "トロピカジャングル";

char STR_MAP_NAME_MANGROVE_REGION[] = "マングローブ域";

char STR_MAP_NAME_PATH_THRU_MT_PANORAMA[] = "ミハラシ山道";

char STR_MAP_NAME_ENTRANCE_TO_FILE_CITY[] = "はじまりの街入り口";

char STR_MAP_NAME_MT_PANORAMA_PLAINS[] = "ミハラシ台";

char STR_MAP_NAME_FOOT_OF_MT_PANORAMA[] = "ミハラシすそ野";

char STR_MAP_NAME_MT_PANORAMA_SPORE_AREA[] = "ミハラシ山・ホウシ域";

char STR_MAP_NAME_DRILL_TUNNEL[] = "ドリルトンネル";

char STR_MAP_NAME_DRILL_TUNNEL_2ND_FLOOR[] = "ドリルトンネル地下２階";

char STR_MAP_NAME_DRILL_TUNNEL_3RD_FLOOR[] = "ドリルトンネル地下３階";

char STR_MAP_NAME_RESIDENTIAL_AREA[] = "ドリモゲモン居住区";

char STR_MAP_NAME_UNDERGROUND_POND[] = "地底湖";

char STR_MAP_NAME_LAVA_CAVE[] = "溶岩洞";

char STR_MAP_NAME_OVERDELL[] = "オーバーデル";

char STR_MAP_NAME_OVERDELL_CEMETERY[] = "オーバーデル墓地";

char STR_MAP_NAME_GREAT_CANYON_ENTRANCE[] = "グレートキャニオン入り口";

char STR_MAP_NAME_GREAT_CANYON_TOP_AREA[] = "グレートキャニオン上層";

char STR_MAP_NAME_GREAT_CANYON_BRIDGE[] = "グレ大橋";

char STR_MAP_NAME_FORTRESS_ENTRANCE[] = "トリデ入り口";

char STR_MAP_NAME_GREAT_CANYON_BOT_AREA[] = "グレートキャニオン下層";

char STR_MAP_NAME_OGRE_FORTRESS[] = "オーガトリデ";

char STR_MAP_NAME_MONOCHROME_SHOP[] = "モノクロ店";

char STR_MAP_NAME_GREY_LORDS_MANSION[] = "闇貴族の館";

char STR_MAP_NAME_MANSION_BASEMENT[] = "闇貴族の館地下";

char STR_MAP_NAME_UNDERGROUND_LAB[] = "地下実験場";

char STR_MAP_NAME_GEAR_SAVANNA[] = "ギアサバンナ";

char STR_MAP_NAME_ANCIENT_DINO_REGION[] = "ダイノ古代境入り口";

char STR_MAP_NAME_ANCIENT_GLACIAL_REGION[] = "ダイノ古代境時静域";

char STR_MAP_NAME_ANCIENT_SPEEDY_REGION[] = "ダイノ古代境時急域";

char STR_MAP_NAME_FREEZELAND[] = "フリーズランド";

char STR_MAP_NAME_ICE_SANCTUARY[] = "アイスサンクチュアリ";

char STR_MAP_NAME_GREEN_GYM[] = "グリーンジム";

char STR_MAP_NAME_LEOMON_ANCESTORS_CAVE[] = "レオモン先祖の洞くつ";

char STR_MAP_NAME_MISTY_TREES[] = "ミスティツリーズ";

char STR_MAP_NAME_GREAT_CANYON[] = "グレートキャニオン";

char STR_MAP_NAME_GEKO_SWAMP[] = "ゲッコー湿地";

char STR_MAP_NAME_VOLUME_VILLA[] = "ダイオン郷";

char STR_MAP_NAME_FILE_CITY[] = "はじまりの街";

char STR_MAP_NAME_ITEM_KEEPER[] = "預り屋";

char STR_MAP_NAME_CENTAR_CLINIC[] = "ケンタル医院";

char STR_MAP_NAME_RESTAURANT[] = "レストラン";

char STR_MAP_NAME_ITEM_SHOP[] = "アイテムショップ";

char STR_MAP_NAME_JIJIMONS_HOUSE[] = "ジジモンの家";

char STR_MAP_NAME_SECRET_ITEM_SHOP[] = "ひみつアイテムショップ";

char STR_MAP_NAME_TOY_TOWN[] = "おもちゃのまち";

char STR_MAP_NAME_SECRET_BEACH_CAVE[] = "ひみつの海岸洞";

char STR_MAP_NAME_FACTORIAL_TOWN[] = "ファクトリアルタウン";

char STR_MAP_NAME_BIRDRA_TRANSPORT[] = "バードラ運送";

char STR_MAP_NAME_ARENA_LOBBY[] = "闘技場ロビー";

char STR_MAP_NAME_TREASURE_HUNT[] = "宝探し屋";

char STR_MAP_NAME_TRASH_MOUNTAIN[] = "ゴミの山";

char STR_MAP_NAME_SEWER[] = "下水道";

char STR_MAP_NAME_BEETLE_LAND[] = "ビートランド";

char STR_MAP_NAME_MT_INFINITY[] = "ムゲンマウンテン";

char STR_MAP_NAME_DIGIMON_CURLING[] = "デジモンカーリング場";

char STR_MAP_NAME_TOY_MANSION[] = "おもちゃ館";

char STR_MAP_NAME_COSTUME_HOUSE[] = "ぬいぐるみハウス";

char STR_MAP_NAME_ROBOT_HOUSE[] = "ロボットハウス";

char STR_MAP_NAME_MANSION_2ND_FLOOR[] = "闇貴族の館・２階";

char STR_MAP_NAME_MANSION_ATTIC[] = "闇貴族の館・てんじょう裏";

char STR_MAP_NAME_MANSION_DOT_BASEMENT[] = "闇貴族の館・地下";

char STR_MAP_NAME_BACK_DIMENSION[] = "裏次元";

char STR_MAP_NAME_KUNEMONS_BED[] = "クネモンのねどこ";

char STR_MAP_NAME_AMIDA_FOREST[] = "あみだ森";
#else
char STR_MAP_NAME_NATIVE_FOREST[] = "      Native Forest     ";

char STR_MAP_NAME_COELA_POINT[] = "       Coela Point      ";

char STR_MAP_NAME_DRAGON_EYE_LAKE[] = "     Dragon Eye Lake    ";

char STR_MAP_NAME_DRILL_TUNNEL_ENTRANCE[] = "  Drill Tunnel Entrance ";

char STR_MAP_NAME_DIGIMON_BRIDGE[] = "      Digimon Bridge    ";

char STR_MAP_NAME_TROPICAL_JUNGLE[] = "     Tropical Jungle    ";

char STR_MAP_NAME_MANGROVE_REGION[] = "     Mangrove Region    ";

char STR_MAP_NAME_PATH_THRU_MT_PANORAMA[] = " Path Thru Mt. Panorama ";

char STR_MAP_NAME_ENTRANCE_TO_FILE_CITY[] = "  Entrance to File City ";

char STR_MAP_NAME_MT_PANORAMA_PLAINS[] = "  Mt. Panorama Plains   ";

char STR_MAP_NAME_FOOT_OF_MT_PANORAMA[] = "  Foot of Mt. Panorama  ";

char STR_MAP_NAME_MT_PANORAMA_SPORE_AREA[] = "Mt. Panorama Spore Area ";

char STR_MAP_NAME_DRILL_TUNNEL[] = "      Drill Tunnel      ";

char STR_MAP_NAME_DRILL_TUNNEL_2ND_FLOOR[] = " Drill Tunnel 2nd floor ";

char STR_MAP_NAME_DRILL_TUNNEL_3RD_FLOOR[] = " Drill Tunnel 3rd floor ";

char STR_MAP_NAME_RESIDENTIAL_AREA[] = "    Residential Area    ";

char STR_MAP_NAME_UNDERGROUND_POND[] = "    Underground Pond    ";

char STR_MAP_NAME_LAVA_CAVE[] = "        Lava Cave       ";

char STR_MAP_NAME_OVERDELL[] = "        Overdell        ";

char STR_MAP_NAME_OVERDELL_CEMETERY[] = "    Overdell Cemetery   ";

char STR_MAP_NAME_GREAT_CANYON_ENTRANCE[] = "  Great Canyon Entrance ";

char STR_MAP_NAME_GREAT_CANYON_TOP_AREA[] = "  Great Canyon Top Area ";

char STR_MAP_NAME_GREAT_CANYON_BRIDGE[] = "   Great Canyon Bridge  ";

char STR_MAP_NAME_FORTRESS_ENTRANCE[] = "    Fortress Entrance   ";

char STR_MAP_NAME_GREAT_CANYON_BOT_AREA[] = " Great Canyon Bot. Area ";

char STR_MAP_NAME_OGRE_FORTRESS[] = "      Ogre Fortress     ";

char STR_MAP_NAME_MONOCHROME_SHOP[] = "     Monochrome Shop    ";

char STR_MAP_NAME_GREY_LORDS_MANSION[] = "   Grey Lord's Mansion  ";

char STR_MAP_NAME_MANSION_BASEMENT[] = "    Mansion Basement    ";

char STR_MAP_NAME_UNDERGROUND_LAB[] = "     Underground Lab    ";

char STR_MAP_NAME_GEAR_SAVANNA[] = "      Gear Savanna      ";

char STR_MAP_NAME_ANCIENT_DINO_REGION[] = "   Ancient Dino Region  ";

char STR_MAP_NAME_ANCIENT_GLACIAL_REGION[] = " Ancient Glacial Region ";

char STR_MAP_NAME_ANCIENT_SPEEDY_REGION[] = " Ancient Speedy Region  ";

char STR_MAP_NAME_FREEZELAND[] = "       Freezeland       ";

char STR_MAP_NAME_ICE_SANCTUARY[] = "      Ice Sanctuary     ";

char STR_MAP_NAME_GREEN_GYM[] = "        Green Gym       ";

char STR_MAP_NAME_LEOMON_ANCESTORS_CAVE[] = " Leomon Ancestor's Cave ";

char STR_MAP_NAME_MISTY_TREES[] = "       Misty Trees      ";

char STR_MAP_NAME_GREAT_CANYON[] = "      Great Canyon      ";

char STR_MAP_NAME_GEKO_SWAMP[] = "        Geko Swamp      ";

char STR_MAP_NAME_VOLUME_VILLA[] = "      Volume Villa      ";

char STR_MAP_NAME_FILE_CITY[] = "        File City       ";

char STR_MAP_NAME_ITEM_KEEPER[] = "      Item Keeper       ";

char STR_MAP_NAME_CENTAR_CLINIC[] = "      Centar Clinic     ";

char STR_MAP_NAME_RESTAURANT[] = "       Restaurant       ";

char STR_MAP_NAME_ITEM_SHOP[] = "        Item Shop       ";

char STR_MAP_NAME_JIJIMONS_HOUSE[] = "      Jijimon's house   ";

char STR_MAP_NAME_SECRET_ITEM_SHOP[] = "    Secret Item Shop    ";

char STR_MAP_NAME_TOY_TOWN[] = "        Toy Town        ";

char STR_MAP_NAME_SECRET_BEACH_CAVE[] = "    Secret Beach Cave   ";

char STR_MAP_NAME_FACTORIAL_TOWN[] = "     Factorial Town     ";

char STR_MAP_NAME_BIRDRA_TRANSPORT[] = "     Birdra Transport   ";

char STR_MAP_NAME_ARENA_LOBBY[] = "       Arena Lobby      ";

char STR_MAP_NAME_TREASURE_HUNT[] = "      Treasure Hunt     ";

char STR_MAP_NAME_TRASH_MOUNTAIN[] = "     Trash Mountain     ";

char STR_MAP_NAME_SEWER[] = "          Sewer         ";

char STR_MAP_NAME_BEETLE_LAND[] = "      Beetle Land       ";

char STR_MAP_NAME_MT_INFINITY[] = "      Mt. Infinity      ";

char STR_MAP_NAME_DIGIMON_CURLING[] = "     Digimon Curling    ";

char STR_MAP_NAME_TOY_MANSION[] = "       Toy Mansion      ";

char STR_MAP_NAME_COSTUME_HOUSE[] = "      Costume House     ";

char STR_MAP_NAME_ROBOT_HOUSE[] = "       Robot House      ";

char STR_MAP_NAME_MANSION_2ND_FLOOR[] = "    Mansion 2nd floor   ";

char STR_MAP_NAME_MANSION_ATTIC[] = "      Mansion Attic     ";

char STR_MAP_NAME_BACK_DIMENSION[] = "     Back Dimension     ";

char STR_MAP_NAME_KUNEMONS_BED[] = "      Kunemon's Bed     ";

char STR_MAP_NAME_AMIDA_FOREST[] = "      Amida Forest      ";
#endif

int8_t MAP_FILE_EXT_MAP[] = ".MAP";

int8_t MAP_FILE_EXT_TFS[] = ".TFS";

char MAP_PATH_SEPARATOR[] = "\\";

int16_t DEATH_MAP = 0xffff;

int16_t DEATH_MAP_EXIT = 0xffff;

int8_t MAIN_D_801343B4 = 0x01;

uint8_t MERAMON_SHAKE_GREEN = 0xff;

uint8_t MERAMON_SHAKE_BLUE = 0xff;

int16_t MERAMON_SHAKE_OFFSET = 0x0020;

char MAP_PATH_PREFIX[] = "\\MAP\\MAP";

char MAP_PATH_DIGITS[] = "0123456789";

char *MAP_NAME_PTR[70] = {
	STR_MAP_NAME_NATIVE_FOREST,
	STR_MAP_NAME_COELA_POINT,
	STR_MAP_NAME_DRAGON_EYE_LAKE,
	STR_MAP_NAME_DRILL_TUNNEL_ENTRANCE,
	STR_MAP_NAME_DIGIMON_BRIDGE,
	STR_MAP_NAME_TROPICAL_JUNGLE,
	STR_MAP_NAME_MANGROVE_REGION,
	STR_MAP_NAME_PATH_THRU_MT_PANORAMA,
	STR_MAP_NAME_ENTRANCE_TO_FILE_CITY,
	STR_MAP_NAME_MT_PANORAMA_PLAINS,
	STR_MAP_NAME_FOOT_OF_MT_PANORAMA,
	STR_MAP_NAME_MT_PANORAMA_SPORE_AREA,
	STR_MAP_NAME_DRILL_TUNNEL,
	STR_MAP_NAME_DRILL_TUNNEL_2ND_FLOOR,
	STR_MAP_NAME_DRILL_TUNNEL_3RD_FLOOR,
	STR_MAP_NAME_RESIDENTIAL_AREA,
	STR_MAP_NAME_UNDERGROUND_POND,
	STR_MAP_NAME_LAVA_CAVE,
	STR_MAP_NAME_OVERDELL,
	STR_MAP_NAME_OVERDELL_CEMETERY,
	STR_MAP_NAME_GREAT_CANYON_ENTRANCE,
	STR_MAP_NAME_GREAT_CANYON_TOP_AREA,
	STR_MAP_NAME_GREAT_CANYON_BRIDGE,
	STR_MAP_NAME_FORTRESS_ENTRANCE,
	STR_MAP_NAME_GREAT_CANYON_BOT_AREA,
	STR_MAP_NAME_OGRE_FORTRESS,
	STR_MAP_NAME_MONOCHROME_SHOP,
	STR_MAP_NAME_GREY_LORDS_MANSION,
	STR_MAP_NAME_MANSION_BASEMENT,
	STR_MAP_NAME_UNDERGROUND_LAB,
	STR_MAP_NAME_GEAR_SAVANNA,
	STR_MAP_NAME_ANCIENT_DINO_REGION,
	STR_MAP_NAME_ANCIENT_GLACIAL_REGION,
	STR_MAP_NAME_ANCIENT_SPEEDY_REGION,
	STR_MAP_NAME_FREEZELAND,
	STR_MAP_NAME_ICE_SANCTUARY,
	STR_MAP_NAME_GREEN_GYM,
	STR_MAP_NAME_LEOMON_ANCESTORS_CAVE,
	STR_MAP_NAME_MISTY_TREES,
	STR_MAP_NAME_GREAT_CANYON,
	STR_MAP_NAME_FREEZELAND,
	STR_MAP_NAME_GEKO_SWAMP,
	STR_MAP_NAME_VOLUME_VILLA,
	STR_MAP_NAME_FILE_CITY,
	STR_MAP_NAME_ITEM_KEEPER,
	STR_MAP_NAME_CENTAR_CLINIC,
	STR_MAP_NAME_RESTAURANT,
	STR_MAP_NAME_ITEM_SHOP,
	STR_MAP_NAME_JIJIMONS_HOUSE,
	STR_MAP_NAME_SECRET_ITEM_SHOP,
	STR_MAP_NAME_TOY_TOWN,
	STR_MAP_NAME_SECRET_BEACH_CAVE,
	STR_MAP_NAME_FACTORIAL_TOWN,
	STR_MAP_NAME_BIRDRA_TRANSPORT,
	STR_MAP_NAME_ARENA_LOBBY,
	STR_MAP_NAME_TREASURE_HUNT,
	STR_MAP_NAME_TRASH_MOUNTAIN,
	STR_MAP_NAME_SEWER,
	STR_MAP_NAME_BEETLE_LAND,
	STR_MAP_NAME_MT_INFINITY,
	STR_MAP_NAME_DIGIMON_CURLING,
	STR_MAP_NAME_TOY_MANSION,
	STR_MAP_NAME_COSTUME_HOUSE,
	STR_MAP_NAME_ROBOT_HOUSE,
	STR_MAP_NAME_MANSION_2ND_FLOOR,
	STR_MAP_NAME_MANSION_ATTIC,
#if !VERSION_IS(US)
	STR_MAP_NAME_MANSION_DOT_BASEMENT,
#else
	STR_MAP_NAME_MANSION_BASEMENT,
#endif
	STR_MAP_NAME_BACK_DIMENSION,
	STR_MAP_NAME_KUNEMONS_BED,
	STR_MAP_NAME_AMIDA_FOREST,
};

MapEntry MAP_ENTRIES[255] = {
	{
		"MAYO01",
		0x03,
		0x00,
		0x80,
		0x00,
		0x01,
		0x00,
	},
	{
		"MAYO02",
		0x02,
		0x00,
		0x80,
		0x00,
		0x00,
		0x44,
	},
	{
		"MAYO03",
		0x02,
		0x00,
		0x80,
		0x00,
		0x00,
		0x00,
	},
	{
		"MAYO04A",
		0x03,
		0x00,
		0x80,
		0x10,
		0x00,
		0x00,
	},
	{
		"MAYO04B",
		0x03,
		0x00,
		0x80,
		0x00,
		0x00,
		0x00,
	},
	{
		"MAYO05",
		0x02,
		0x01,
		0x80,
		0x00,
		0x00,
		0x01,
	},
	{
		"MAYO06",
		0x02,
		0x01,
		0x8d,
		0x00,
		0x00,
		0x02,
	},
	{
		"MAYO11",
		0x02,
		0x01,
		0x80,
		0x00,
		0x00,
		0x03,
	},
	{
		"MAYO10",
		0x03,
		0x02,
		0x0d,
		0x00,
		0x00,
		0x02,
	},
	{
		"MAYO08A",
		0x02,
		0x02,
		0x80,
		0x00,
		0x00,
		0x04,
	},
	{
		"MAYO08B",
		0x02,
		0x02,
		0x80,
		0x00,
		0x00,
		0x04,
	},
	{
		"TROP00",
		0x01,
		0x00,
		0x81,
		0x00,
		0x00,
		0x05,
	},
	{
		"TROP01",
		0x01,
		0x00,
		0x81,
		0x00,
		0x00,
		0x05,
	},
	{
		"TROP02",
		0x03,
		0x00,
		0x81,
		0x00,
		0x0a,
		0x05,
	},
	{
		"TROP03",
		0x02,
		0x01,
		0x81,
		0x00,
		0x00,
		0x05,
	},
	{
		"TROP04",
		0x03,
		0x00,
		0x81,
		0x00,
		0x00,
		0x06,
	},
	{
		"TROP05",
		0x02,
		0x00,
		0x81,
		0x00,
		0x00,
		0x06,
	},
	{
		"TROP06",
		0x03,
		0x00,
		0x85,
		0x00,
		0x00,
		0x45,
	},
	{
		"MIHA00",
		0x02,
		0x00,
		0x82,
		0x11,
		0x00,
		0x07,
	},
	{
		"MIHA01",
		0x03,
		0x00,
		0x82,
		0x00,
		0x00,
		0x09,
	},
	{
		"MIHA02",
		0x02,
		0x00,
		0x82,
		0x00,
		0x00,
		0x07,
	},
	{
		"MIHA03",
		0x01,
		0x00,
		0x82,
		0x00,
		0x00,
		0x0b,
	},
	{
		"MIHA04A",
		0x03,
		0x00,
		0x82,
		0x00,
		0x02,
		0x0a,
	},
	{
		"MIHA04B",
		0x02,
		0x00,
		0x82,
		0x00,
		0x02,
		0x0a,
	},
	{
		"TUNN01",
		0x00,
		0x01,
		0xc2,
		0x00,
		0x00,
		0x0c,
	},
	{
		"TUNN02",
		0x01,
		0x00,
		0xc2,
		0x00,
		0x00,
		0x0d,
	},
	{
		"TUNN03",
		0x03,
		0x00,
		0xc2,
		0x00,
		0x00,
		0x0e,
	},
	{
		"TUNN04",
		0x01,
		0x00,
		0xc2,
		0x00,
		0x00,
		0x0f,
	},
	{
		"TUNN05",
		0x01,
		0x00,
		0xc2,
		0x00,
		0x00,
		0x10,
	},
	{
		"TUNN06",
		0x01,
		0x00,
		0xc2,
		0x00,
		0x00,
		0x0d,
	},
	{
		"TUNN07",
		0x03,
		0x00,
		0xc2,
		0x00,
		0x00,
		0x11,
	},
	{
		"TUNN08",
		0x01,
		0x00,
		0xc2,
		0x00,
		0x00,
		0x11,
	},
	{
		"TUNN09",
		0x02,
		0x00,
		0x82,
		0x00,
		0x00,
		0x0c,
	},
	{
		"TUNN10",
		0x01,
		0x00,
		0xc2,
		0x00,
		0x00,
		0x11,
	},
	{
		"DGHA01",
		0x02,
		0x00,
		0x84,
		0x00,
		0x00,
		0x12,
	},
	{
		"DGHA02",
		0x03,
		0x00,
		0xc4,
		0x18,
		0x00,
		0x13,
	},
	{
		"GCAN01",
		0x03,
		0x00,
		0x86,
		0x00,
		0x00,
		0x14,
	},
	{
		"GCAN02",
		0x01,
		0x01,
		0x86,
		0x13,
		0x00,
		0x15,
	},
	{
		"GCAN03",
		0x01,
		0x01,
		0x86,
		0x12,
		0x00,
		0x15,
	},
	{
		"GCAN04",
		0x01,
		0x00,
		0x86,
		0x00,
		0x00,
		0x16,
	},
	{
		"GCAN05",
		0x03,
		0x01,
		0x86,
		0x16,
		0x00,
		0x17,
	},
	{
		"GCAN06",
		0x01,
		0x01,
		0x86,
		0x14,
		0x00,
		0x18,
	},
	{
		"GCAN07",
		0x00,
		0x00,
		0x86,
		0x00,
		0x00,
		0x18,
	},
	{
		"GCAN08_1",
		0x01,
		0x00,
		0x86,
		0x00,
		0x00,
		0x15,
	},
	{
		"GCAN09",
		0x03,
		0x00,
		0x86,
		0x00,
		0x03,
		0x15,
	},
	{
		"OGRE00",
		0x01,
		0x00,
		0xc6,
		0x00,
		0x00,
		0x19,
	},
	{
		"OGRE01",
		0x01,
		0x00,
		0xc6,
		0x00,
		0x00,
		0x19,
	},
	{
		"OGRE02",
		0x03,
		0x00,
		0xc6,
		0x00,
		0x00,
		0x19,
	},
	{
		"OGRE03",
		0x03,
		0x01,
		0xc6,
		0x00,
		0x00,
		0x19,
	},
	{
		"GCAN11",
		0x01,
		0x01,
		0xc6,
		0x00,
		0x00,
		0x1a,
	},
	{
		"YAKA01",
		0x00,
		0x00,
		0x44,
		0x00,
		0x00,
		0x1b,
	},
	{
		"YAKA02",
		0x03,
		0x00,
		0xc4,
		0x00,
		0x00,
		0x1b,
	},
	{
		"YAKA11A",
		0x03,
		0x00,
		0xc4,
		0x01,
		0x00,
		0x1b,
	},
	{
		"YAKA11B",
		0x03,
		0x00,
		0x44,
		0x02,
		0x00,
		0x1b,
	},
	{
		"YAKA12",
		0x02,
		0x00,
		0x44,
		0x03,
		0x00,
		0x1b,
	},
	{
		"YAKA13",
		0x03,
		0x00,
		0xc4,
		0x04,
		0x00,
		0x1b,
	},
	{
		"YAKA14",
		0x03,
		0x00,
		0xc4,
		0x05,
		0x00,
		0x1b,
	},
	{
		"YAKA15",
		0x03,
		0x00,
		0x44,
		0x06,
		0x04,
		0x1b,
	},
	{
		"YAKA16",
		0x03,
		0x00,
		0xc4,
		0x07,
		0x00,
		0x1b,
	},
	{
		"YAKA17",
		0x02,
		0x00,
		0xc4,
		0x08,
		0x00,
		0x1b,
	},
	{
		"YAKA18",
		0x03,
		0x00,
		0x44,
		0x09,
		0x00,
		0x1b,
	},
	{
		"YAKA21",
		0x00,
		0x00,
		0x44,
		0x00,
		0x00,
		0x1b,
	},
	{
		"YAKA22",
		0x03,
		0x00,
		0x44,
		0x0a,
		0x00,
		0x1b,
	},
	{
		"YAKA23",
		0x03,
		0x00,
		0x44,
		0x0b,
		0x00,
		0x1b,
	},
	{
		"YAKA24",
		0x03,
		0x00,
		0x44,
		0x0c,
		0x00,
		0x1b,
	},
	{
		"YAKA25",
		0x00,
		0x00,
		0x44,
		0x0d,
		0x00,
		0x1b,
	},
	{
		"CHKA01",
		0x00,
		0x00,
		0xc4,
		0x00,
		0x00,
		0x1c,
	},
	{
		"SAIB01",
		0x01,
		0x00,
		0xc4,
		0x00,
		0x00,
		0x1d,
	},
	{
		"SAIB02",
		0x02,
		0x00,
		0xc4,
		0x00,
		0x00,
		0x1d,
	},
	{
		"GIAS00",
		0x02,
		0x00,
		0x83,
		0x00,
		0x00,
		0x1e,
	},
	{
		"GIAS01",
		0x03,
		0x00,
		0x83,
		0x00,
		0x00,
		0x1e,
	},
	{
		"GIAS02",
		0x03,
		0x01,
		0x83,
		0x00,
		0x00,
		0x1e,
	},
	{
		"GIAS03",
		0x01,
		0x00,
		0x83,
		0x00,
		0x00,
		0x1e,
	},
	{
		"GIAS04",
		0x03,
		0x00,
		0x83,
		0x00,
		0x00,
		0x1e,
	},
	{
		"GIAS05",
		0x03,
		0x00,
		0x83,
		0x00,
		0x00,
		0x1e,
	},
	{
		"GIAS06A",
		0x03,
		0x00,
		0x83,
		0x00,
		0x00,
		0x1e,
	},
	{
		"GIAS07",
		0x02,
		0x00,
		0x83,
		0x00,
		0x00,
		0x1e,
	},
	{
		"GIAS08",
		0x02,
		0x00,
		0x83,
		0x00,
		0x07,
		0x1e,
	},
	{
		"GIAS09",
		0x03,
		0x01,
		0x8e,
		0x00,
		0x00,
		0x1e,
	},
	{
		"KODA00",
		0x02,
		0x00,
		0x85,
		0x00,
		0x05,
		0x1f,
	},
	{
		"KODA01",
		0x02,
		0x00,
		0x85,
		0x00,
		0x00,
		0x20,
	},
	{
		"KODA02",
		0x03,
		0x00,
		0x85,
		0x00,
		0x00,
		0x20,
	},
	{
		"KODA03",
		0x00,
		0x00,
		0x85,
		0x00,
		0x00,
		0x20,
	},
	{
		"KODA04",
		0x03,
		0x00,
		0x85,
		0x00,
		0x00,
		0x21,
	},
	{
		"KODA05",
		0x02,
		0x00,
		0x05,
		0x00,
		0x00,
		0x21,
	},
	{
		"KODA06",
		0x03,
		0x00,
		0x85,
		0x00,
		0x00,
		0x21,
	},
	{
		"KODA07",
		0x02,
		0x00,
		0x85,
		0x00,
		0x00,
		0x21,
	},
	{
		"KODA08",
		0x02,
		0x00,
		0x85,
		0x00,
		0x00,
		0x21,
	},
	{
		"FRZL01",
		0x01,
		0x01,
		0x8b,
		0x00,
		0x00,
		0x22,
	},
	{
		"FRZL02",
		0x01,
		0x01,
		0x8b,
		0x00,
		0x00,
		0x22,
	},
	{
		"FRZL03",
		0x02,
		0x01,
		0x8b,
		0x00,
		0x06,
		0x22,
	},
	{
		"FRZL04",
		0x01,
		0x01,
		0x8b,
		0x00,
		0x00,
		0x22,
	},
	{
		"FRZL05",
		0x00,
		0x01,
		0x0b,
		0x00,
		0x00,
		0x22,
	},
	{
		"FRZL06",
		0x02,
		0x01,
		0x8b,
		0x00,
		0x00,
		0x22,
	},
	{
		"FRZL07",
		0x01,
		0x01,
		0x8b,
		0x00,
		0x00,
		0x22,
	},
	{
		"FRZL08",
		0x01,
		0x01,
		0x8b,
		0x00,
		0x00,
		0x22,
	},
	{
		"FRZL12",
		0x01,
		0x01,
		0x8b,
		0x00,
		0x00,
		0x22,
	},
	{
		"ICSA01",
		0x01,
		0x00,
		0xcb,
		0x00,
		0x00,
		0x23,
	},
	{
		"ICSA02",
		0x00,
		0x01,
		0xcb,
		0x00,
		0x00,
		0x23,
	},
	{
		"ICSA03",
		0x00,
		0x03,
		0xce,
		0x00,
		0x00,
		0x23,
	},
	{
		"ICSA04",
		0x00,
		0x00,
		0xcb,
		0x00,
		0x00,
		0x23,
	},
	{
		"ICSA05",
		0x00,
		0x01,
		0xcb,
		0x00,
		0x00,
		0x23,
	},
	{
		"ICSA06",
		0x00,
		0x03,
		0xcb,
		0x00,
		0x00,
		0x23,
	},
	{
		"ICSA07",
		0x00,
		0x02,
		0xcb,
		0x00,
		0x00,
		0x23,
	},
	{
		"ICSA08",
		0x00,
		0x01,
		0xcb,
		0x00,
		0x00,
		0x23,
	},
	{
		"BETL01",
		0x03,
		0x00,
		0x80,
		0x00,
		0x00,
		0x3a,
	},
	{
		"BETL02",
		0x03,
		0x00,
		0x80,
		0x00,
		0x00,
		0x3a,
	},
	{
		"BETL03",
		0x02,
		0x02,
		0xce,
		0x00,
		0x00,
		0x3a,
	},
	{
		"BETL04",
		0x03,
		0x01,
		0xce,
		0x00,
		0x00,
		0x3a,
	},
	{
		"MAYO00",
		0x03,
		0x00,
		0x80,
		0x00,
		0x00,
		0x00,
	},
	{
		"MAYO01_2",
		0x03,
		0x00,
		0x80,
		0x00,
		0x00,
		0x00,
	},
	{
		"MAYO02_2",
		0x03,
		0x00,
		0x80,
		0x00,
		0x00,
		0x00,
	},
	{
		"TRAI00",
		0x02,
		0x02,
		0x8e,
		0x00,
		0x00,
		0x24,
	},
	{
		"LEOM01",
		0x02,
		0x00,
		0xc2,
		0x00,
		0x00,
		0x25,
	},
	{
		"LEOM02",
		0x01,
		0x00,
		0xc2,
		0x00,
		0x00,
		0x25,
	},
	{
		"MIST01",
		0x02,
		0x02,
		0x89,
		0x00,
		0x00,
		0x26,
	},
	{
		"MIST02",
		0x02,
		0x02,
		0x89,
		0x00,
		0x00,
		0x26,
	},
	{
		"MIST03",
		0x02,
		0x02,
		0x89,
		0x00,
		0x00,
		0x26,
	},
	{
		"MIST04",
		0x02,
		0x02,
		0x89,
		0x00,
		0x00,
		0x26,
	},
	{
		"MIST05",
		0x02,
		0x03,
		0x8e,
		0x00,
		0x00,
		0x26,
	},
	{
		"MIST06",
		0x03,
		0x02,
		0x89,
		0x00,
		0x00,
		0x26,
	},
	{
		"MIST07",
		0x02,
		0x02,
		0x89,
		0x00,
		0x00,
		0x26,
	},
	{
		"TUNN07_2",
		0x01,
		0x00,
		0x42,
		0x00,
		0x00,
		0x11,
	},
	{
		"TUNN07_3",
		0x01,
		0x00,
		0xc2,
		0x00,
		0x00,
		0x11,
	},
	{
		"TUNN08_2",
		0x00,
		0x00,
		0x42,
		0x00,
		0x00,
		0x11,
	},
	{
		"TUNN08_3",
		0x00,
		0x00,
		0xc2,
		0x00,
		0x00,
		0x11,
	},
	{
		"TUNN03_2",
		0x03,
		0x00,
		0x42,
		0x00,
		0x00,
		0x0c,
	},
	{
		"GCAN04_2",
		0x00,
		0x00,
		0x86,
		0x00,
		0x00,
		0x27,
	},
	{
		"GCAN08_2",
		0x01,
		0x00,
		0x86,
		0x00,
		0x00,
		0x27,
	},
	{
		"GCAN10",
		0x01,
		0x02,
		0x86,
		0x15,
		0x00,
		0x27,
	},
	{
		"OGRE04",
		0x01,
		0x00,
		0x46,
		0x00,
		0x00,
		0x19,
	},
	{
		"GIAS06B",
		0x03,
		0x00,
		0x83,
		0x00,
		0x00,
		0x1e,
	},
	{
		"FRZL13",
		0x01,
		0x01,
		0x8b,
		0x00,
		0x00,
		0x28,
	},
	{
		"FRZL14",
		0x01,
		0x01,
		0x8b,
		0x00,
		0x00,
		0x28,
	},
	{
		"FRZL15",
		0x01,
		0x01,
		0x8b,
		0x00,
		0x00,
		0x28,
	},
	{
		"FRZL16",
		0x00,
		0x02,
		0x8b,
		0x00,
		0x00,
		0x28,
	},
	{
		"FRZL17",
		0x01,
		0x01,
		0xcf,
		0x00,
		0x00,
		0x28,
	},
	{
		"FRZL18",
		0x01,
		0x00,
		0xcb,
		0x00,
		0x00,
		0x28,
	},
	{
		"STIC01",
		0x02,
		0x00,
		0x8a,
		0x00,
		0x00,
		0x29,
	},
	{
		"STIC02",
		0x02,
		0x00,
		0x8a,
		0x00,
		0x08,
		0x29,
	},
	{
		"GKYO01",
		0x03,
		0x00,
		0x8a,
		0x00,
		0x00,
		0x2a,
	},
	{
		"GKYO02",
		0x01,
		0x00,
		0xca,
		0x00,
		0x00,
		0x2a,
	},
	{
		"OGRE10",
		0x01,
		0x00,
		0xcb,
		0x00,
		0x00,
		0x33,
	},
	{
		"OGRE11",
		0x02,
		0x00,
		0xcb,
		0x00,
		0x00,
		0x33,
	},
	{
		"OMOC01",
		0x03,
		0x00,
		0x89,
		0x00,
		0x00,
		0x32,
	},
	{
		"OMOC02",
		0x02,
		0x00,
		0xc9,
		0x00,
		0x00,
		0x3e,
	},
	{
		"OMOC03",
		0x03,
		0x01,
		0xc9,
		0x00,
		0x00,
		0x3f,
	},
	{
		"OMOC04",
		0x01,
		0x01,
		0xc9,
		0x0f,
		0x00,
		0x3d,
	},
	{
		"OMOC05",
		0x01,
		0x01,
		0xc9,
		0x0e,
		0x00,
		0x3d,
	},
	{
		"OMOC06",
		0x02,
		0x01,
		0xc9,
		0x0e,
		0x00,
		0x3d,
	},
	{
		"OMOC07",
		0x01,
		0x01,
		0xc9,
		0x0e,
		0x00,
		0x3d,
	},
	{
		"OMOC08",
		0x01,
		0x02,
		0xc9,
		0x00,
		0x00,
		0x3d,
	},
	{
		"FACT01",
		0x01,
		0x01,
		0x88,
		0x00,
		0x00,
		0x34,
	},
	{
		"FACT02",
		0x01,
		0x01,
		0x88,
		0x00,
		0x00,
		0x34,
	},
	{
		"FACT03",
		0x03,
		0x00,
		0x88,
		0x00,
		0x00,
		0x34,
	},
	{
		"FACT04",
		0x01,
		0x00,
		0x88,
		0x00,
		0x00,
		0x34,
	},
	{
		"FACT05",
		0x01,
		0x00,
		0x08,
		0x00,
		0x00,
		0x34,
	},
	{
		"FACT06",
		0x00,
		0x01,
		0xc8,
		0x00,
		0x00,
		0x34,
	},
	{
		"FACT07",
		0x01,
		0x01,
		0xc8,
		0x17,
		0x00,
		0x34,
	},
	{
		"FACT08A",
		0x01,
		0x00,
		0xc8,
		0x00,
		0x00,
		0x34,
	},
	{
		"FACT08B",
		0x01,
		0x02,
		0xc8,
		0x00,
		0x00,
		0x34,
	},
	{
		"FACT09",
		0x00,
		0x00,
		0xc8,
		0x00,
		0x00,
		0x34,
	},
	{
		"FACT10",
		0x01,
		0x00,
		0xc8,
		0x00,
		0x00,
		0x34,
	},
	{
		"FACT11A",
		0x02,
		0x01,
		0xc8,
		0x00,
		0x00,
		0x39,
	},
	{
		"GOMI01",
		0x02,
		0x00,
		0x87,
		0x00,
		0x00,
		0x38,
	},
	{
		"GOMI02",
		0x03,
		0x01,
		0xce,
		0x00,
		0x00,
		0x38,
	},
	{
		"MGEN01",
		0x00,
		0x00,
		0xd1,
		0x00,
		0x00,
		0x3b,
	},
	{
		"MGEN02",
		0x00,
		0x00,
		0xd1,
		0x00,
		0x00,
		0x3b,
	},
	{
		"TWNA02",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x09,
		0x2b,
	},
	{
		"TWNA03",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x09,
		0x2b,
	},
	{
		"TWNA04",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x09,
		0x2b,
	},
	{
		"TWNA05",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x09,
		0x2b,
	},
	{
		"TWNA06",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x09,
		0x2b,
	},
	{
		"TWNA07",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x09,
		0x2b,
	},
	{
		"TWNA08",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x09,
		0x2b,
	},
	{
		"TWNA09",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x09,
		0x2b,
	},
	{
		"TWNA10",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x09,
		0x2b,
	},
	{
		"TWNA11",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x09,
		0x2b,
	},
	{
		"TWNA12",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x09,
		0x2b,
	},
	{
		"TWNA13",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x09,
		0x2b,
	},
	{
		"TWNB01",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB02",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB03",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB04",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB05",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB06",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB07",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB08",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB09",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB10",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB11",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB12",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB13",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB14",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB15",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB16",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB17",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB18",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB19",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB20",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB21",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB22",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB23",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNB24",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TWNA01",
		0x02,
		0x02,
		0x8c,
		0x00,
		0x09,
		0x2b,
	},
	{
		"ROOM10",
		0x02,
		0x01,
		0xcc,
		0x00,
		0x00,
		0x30,
	},
	{
		"ROOM11",
		0x01,
		0x00,
		0xcc,
		0x00,
		0x00,
		0x30,
	},
	{
		"ROOM12",
		0x01,
		0x01,
		0x8c,
		0x00,
		0x00,
		0x35,
	},
	{
		"ROOM13",
		0x03,
		0x00,
		0xcc,
		0x00,
		0x00,
		0x36,
	},
	{
		"ROOM14",
		0x02,
		0x00,
		0xcc,
		0x00,
		0x00,
		0x37,
	},
	{
		"MGEN03",
		0x00,
		0x00,
		0xd1,
		0x00,
		0x00,
		0x3b,
	},
	{
		"ROOM02",
		0x02,
		0x00,
		0xcc,
		0x00,
		0x00,
		0x2c,
	},
	{
		"MGEN04",
		0x00,
		0x00,
		0xd1,
		0x00,
		0x00,
		0x3b,
	},
	{
		"ROOM04",
		0x02,
		0x00,
		0xcc,
		0x00,
		0x00,
		0x2d,
	},
	{
		"ROOM05A",
		0x02,
		0x00,
		0xcc,
		0x00,
		0x00,
		0x2e,
	},
	{
		"ROOM05B",
		0x03,
		0x00,
		0xcc,
		0x00,
		0x00,
		0x2e,
	},
	{
		"ROOM06",
		0x03,
		0x00,
		0xcc,
		0x00,
		0x00,
		0x2f,
	},
	{
		"ROOM07",
		0x01,
		0x00,
		0xcc,
		0x00,
		0x00,
		0x31,
	},
	{
		"ROOM08",
		0x03,
		0x00,
		0xcc,
		0x00,
		0x00,
		0x30,
	},
	{
		"MGEN05",
		0x00,
		0x00,
		0xd1,
		0x00,
		0x00,
		0x3b,
	},
	{
		"FACT11B",
		0x03,
		0x01,
		0xc8,
		0x00,
		0x00,
		0x39,
	},
	{
		"SAIB03",
		0x00,
		0x00,
		0xc4,
		0x00,
		0x00,
		0x1d,
	},
	{
		"MGEN98",
		0x00,
		0x00,
		0xd1,
		0x00,
		0x00,
		0x3b,
	},
	{
		"ROOM19",
		0x03,
		0x00,
		0xcc,
		0x00,
		0x00,
		0x36,
	},
	{
		"YAKA26",
		0x03,
		0x00,
		0xc4,
		0x06,
		0x00,
		0x1b,
	},
	{
		"MGEN99",
		0x02,
		0x02,
		0xd1,
		0x00,
		0x00,
		0x3b,
	},
	{
		"MGEN11",
		0x00,
		0x00,
		0xd1,
		0x00,
		0x00,
		0x43,
	},
	{
		"MGEN12",
		0x00,
		0x00,
		0xd1,
		0x00,
		0x00,
		0x43,
	},
	{
		"MGEN13",
		0x00,
		0x00,
		0xd1,
		0x00,
		0x00,
		0x43,
	},
	{
		"MGEN14",
		0x00,
		0x00,
		0x51,
		0x00,
		0x00,
		0x3b,
	},
	{
		"MGEN15",
		0x00,
		0x00,
		0x51,
		0x00,
		0x00,
		0x3b,
	},
	{
		"MGEN16",
		0x00,
		0x00,
		0x51,
		0x00,
		0x00,
		0x3b,
	},
	{
		"YAKA25",
		0x02,
		0x00,
		0xc4,
		0x00,
		0x00,
		0x1b,
	},
	{
		"MIHA05",
		0x03,
		0x00,
		0x82,
		0x00,
		0x00,
		0x0b,
	},
	{
		"MIHA06",
		0x02,
		0x00,
		0x82,
		0x00,
		0x00,
		0x0b,
	},
	{
		"ROOM20",
		0x00,
		0x00,
		0xcc,
		0x00,
		0x00,
		0x3c,
	},
	{
		"TEND01",
		0x02,
		0x02,
		0xcc,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TEND02",
		0x02,
		0x02,
		0xcc,
		0x00,
		0x00,
		0x2b,
	},
	{
		"TOPN01",
		0x02,
		0x02,
		0xcc,
		0x00,
		0x00,
		0x2b,
	},
	{
		"",
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
	},
	{
		"",
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
	},
	{
		"",
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
	},
	{
		"",
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
	},
	{
		"",
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
	},
	{
		"",
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
	},
	{
		"",
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
	},
	{
		"",
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
	},
	{
		"MGEN06",
		0x00,
		0x00,
		0xd1,
		0x00,
		0x00,
		0x3b,
	},
	{
		"MGEN07",
		0x00,
		0x00,
		0xd1,
		0x00,
		0x00,
		0x3b,
	},
	{
		"MGEN08",
		0x00,
		0x00,
		0xd1,
		0x00,
		0x00,
		0x3b,
	},
	{
		"",
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
	},
	{
		"",
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
	},
	{
		"",
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
	},
	{
		"MGEN09",
		0x00,
		0x00,
		0xd1,
		0x00,
		0x00,
		0x3b,
	},
	{
		"MGEN10",
		0x00,
		0x00,
		0xd1,
		0x00,
		0x00,
		0x3b,
	},
};

SVECTOR CONDITION_FX_OFFSETS[177] = {
	{ 0x005a, 0x0014, 0x0000, 0x0003 },
	{ 0x0000, 0xffce, 0x0000, 0x0002 },
	{ 0x0000, 0xffbe, 0x0000, 0x0002 },
	{ 0x0064, 0x0041, 0x0000, 0x0003 },
	{ 0x0043, 0x0000, 0x0067, 0x0007 },
	{ 0xffda, 0xffbc, 0x0000, 0x0004 },
	{ 0x0082, 0xffce, 0x0000, 0x0003 },
	{ 0x000a, 0x0000, 0xffce, 0x0002 },
	{ 0x004b, 0x006e, 0x0000, 0x0003 },
	{ 0x0064, 0x001e, 0x0000, 0x0004 },
	{ 0xffba, 0x0000, 0x0023, 0x0003 },
	{ 0x0000, 0xff74, 0xff9c, 0x0003 },
	{ 0x0000, 0x0032, 0x0000, 0x0004 },
	{ 0x000f, 0xffc4, 0x0000, 0x0001 },
	{ 0x005e, 0x0000, 0x0000, 0x0004 },
	{ 0x0000, 0xffe7, 0x0000, 0x0002 },
	{ 0x0000, 0xff56, 0xffb5, 0x0001 },
	{ 0x0050, 0x0032, 0x0000, 0x0003 },
	{ 0x0000, 0xffba, 0xffb0, 0x0003 },
	{ 0x002d, 0x0069, 0x0000, 0x0003 },
	{ 0x0064, 0x0000, 0x0000, 0x0003 },
	{ 0x0000, 0xffba, 0xffec, 0x0004 },
	{ 0xffd2, 0x0000, 0x002a, 0x0004 },
	{ 0x007e, 0x0000, 0x0000, 0x0003 },
	{ 0xffba, 0x0000, 0x00d2, 0x0002 },
	{ 0x001c, 0x0040, 0x0000, 0x0004 },
	{ 0xffd3, 0xff2e, 0x0000, 0x0004 },
	{ 0xffbf, 0xfff6, 0x0000, 0x000a },
	{ 0x0082, 0x002a, 0x0000, 0x0006 },
	{ 0x0000, 0xff92, 0x0000, 0x0002 },
	{ 0x0000, 0xff6e, 0x0000, 0x0002 },
	{ 0x0019, 0x0000, 0x003c, 0x0003 },
	{ 0x003c, 0x0000, 0x005a, 0x0005 },
	{ 0x0000, 0xff71, 0xff94, 0x0005 },
	{ 0xffdb, 0x001b, 0x0000, 0x0003 },
	{ 0x0041, 0x0082, 0x0000, 0x0004 },
	{ 0x0064, 0x0000, 0x0000, 0x0004 },
	{ 0x0000, 0x0000, 0x0000, 0x0002 },
	{ 0x003c, 0x0000, 0x0078, 0x0003 },
	{ 0x007e, 0x0024, 0xfff5, 0x0004 },
	{ 0x0058, 0x0000, 0x0000, 0x0003 },
	{ 0xffbe, 0x0000, 0x0000, 0x0002 },
	{ 0xffd1, 0x0059, 0x0000, 0x0004 },
	{ 0x0000, 0xffce, 0xffe7, 0x0001 },
	{ 0x0000, 0xffd1, 0x0000, 0x0003 },
	{ 0x0000, 0x0050, 0x0000, 0x0003 },
	{ 0x0046, 0x0000, 0x0000, 0x0005 },
	{ 0x0000, 0xffc4, 0xffb0, 0x0004 },
	{ 0x000a, 0xffa9, 0x0000, 0x0003 },
	{ 0x0000, 0xffc4, 0xffce, 0x0003 },
	{ 0x000a, 0x0000, 0x009b, 0x0004 },
	{ 0x000f, 0x005a, 0x0000, 0x0003 },
	{ 0x0082, 0xffec, 0x0000, 0x0002 },
	{ 0x0064, 0x0000, 0x0000, 0x0002 },
	{ 0x0000, 0x005a, 0x0000, 0x0004 },
	{ 0x0050, 0x0000, 0x0000, 0x0002 },
	{ 0x0000, 0xff9c, 0x000a, 0x0002 },
	{ 0x001e, 0x003a, 0x0000, 0x0003 },
	{ 0xffb5, 0x0000, 0x0000, 0x000a },
	{ 0x0000, 0xffd3, 0xffe7, 0x0004 },
	{ 0x0005, 0x0064, 0x0000, 0x0003 },
	{ 0xffba, 0x0000, 0x0023, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0xffa1, 0x0000, 0x0003 },
	{ 0x0000, 0xffe8, 0xffda, 0x0003 },
	{ 0x0064, 0x0023, 0x0000, 0x0003 },
	{ 0xffd3, 0x000f, 0x0000, 0x0005 },
	{ 0x0000, 0xffa1, 0xffdd, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0064, 0x0023, 0x0000, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x003c, 0x0000, 0x005a, 0x0005 },
	{ 0x0000, 0xff71, 0xff94, 0x0005 },
	{ 0x0000, 0xffb0, 0xffec, 0x0005 },
	{ 0x001c, 0x0040, 0x0000, 0x0004 },
	{ 0x0082, 0xffec, 0x0000, 0x0002 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0xffe9, 0x002d, 0x0000, 0x0004 },
	{ 0x007e, 0x0000, 0x0000, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0043, 0x0000, 0x0067, 0x0007 },
	{ 0x0078, 0x0041, 0x0000, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0046, 0x0000, 0x0000, 0x0005 },
	{ 0x0000, 0xff74, 0xff9c, 0x0003 },
	{ 0x0000, 0xffc4, 0xffb0, 0x0004 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0xffbe, 0x0000, 0x0000, 0x0002 },
	{ 0x0041, 0x0082, 0x0000, 0x0004 },
	{ 0x00a0, 0x001e, 0x0000, 0x0002 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0064, 0x0023, 0x0000, 0x0003 },
	{ 0x000a, 0x0000, 0x009b, 0x0004 },
	{ 0x0019, 0x0000, 0x003c, 0x0003 },
	{ 0xffe9, 0x002d, 0x0000, 0x0004 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0xffe8, 0xffda, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0xffc4, 0x0000, 0x001e, 0x0004 },
	{ 0x0000, 0xffba, 0xffec, 0x0004 },
	{ 0x0000, 0xff8d, 0x0000, 0x0002 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x005a, 0x0000, 0x005a, 0x0002 },
	{ 0xff92, 0xffd8, 0x0000, 0x0004 },
	{ 0x0000, 0xffd8, 0xffe2, 0x0008 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0064, 0x0023, 0x0000, 0x0003 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x002d, 0x0064, 0x0000, 0x0006 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0043, 0x0000, 0x0067, 0x0007 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0046, 0x0000, 0x0000, 0x0005 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
};

int16_t ORIGINAL_DIGIMON_TYPES[180] = {
	0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0030,
	0x0036, 0x002a, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0003,
	0x0022, 0x0027, 0x0020, 0x0021, 0xffff, 0x0019, 0x0034, 0x0026,
	0xffff, 0x0017, 0x0011, 0x0004, 0xffff, 0x0014, 0x002e, 0x000b,
	0x002f, 0x0022, 0x0029, 0x0023, 0xffff, 0x0039, 0xffff, 0x0032,
	0x001f, 0x0050, 0x0003, 0x0019, 0x0006, 0x0045, 0xffff, 0x0050,
	0x0009, 0x0016, 0x0015, 0xffff, 0x0017, 0xffff, 0xffff, 0xffff,
	0x003d, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b,
	0x000c, 0x000d, 0x000e, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015,
	0x0016, 0x0017, 0x0018, 0x0019, 0x001a, 0x001b, 0x001c, 0x001f,
	0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
	0x0028, 0x0029, 0x002a, 0x002d, 0x002e, 0x002f, 0x0030, 0x0031,
	0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x003a,
	0x0039, 0x0042, 0x0005, 0x000c,
};
// clang-format on

int8_t DAYTIME_TRANSITION_FRAME;
int8_t CURRENT_TIME_OF_DAY;
int32_t SKIP_MAP_FILE_READ;
int8_t DAYTIME_TRANSITION_ACTIVE;
int16_t DAYTIME_TRANSITION_TARGET;
uint8_t CURRENT_SCREEN;
uint8_t PREVIOUS_SCREEN;
uint8_t CURRENT_EXIT;
uint8_t PREVIOUS_EXIT;
int8_t CAMERA_REACHED_TARGET;
int8_t CAMERA_HAS_TARGET;
int32_t CAMERA_UPDATE_TILES;
int8_t SKIP_DAYTIME_TRANSITION;
int16_t DRAW_OFFSET_LIMIT_X_MAX;
int16_t DRAW_OFFSET_LIMIT_X_MIN;
int16_t DRAW_OFFSET_LIMIT_Y_MAX;
int16_t DRAW_OFFSET_LIMIT_Y_MIN;
int16_t PLAYER_OFFSET_X;
int16_t PLAYER_OFFSET_Y;
int8_t MAP_TILE_X;
int8_t PREV_TILE_X;
int8_t MAP_TILE_Y;
int8_t PREV_TILE_Y;
int32_t CAMERA_FOLLOW_PLAYER;
int16_t CAMERA_X_PREVIOUS;
int16_t CAMERA_Y_PREVIOUS;
int16_t CAMERA_DATA;
int16_t CAMERA_MOVE_DRAW_OFFSET_Y;
int16_t CAMERA_MOVE_DIFF_X;
int16_t CAMERA_MOVE_DIFF_Y;
int16_t CAMERA_MOVE_FINAL_X;
int16_t CAMERA_MOVE_FINAL_Y;
int8_t CAMERA_MOVE_DELTA_X;
int8_t CAMERA_MOVE_DELTA_Y;
int16_t SCRIPT_MAP_CHANGE_STATE;
uint8_t TARGET_MAP;
int8_t TRN_LOADING_COMPLETE;
int32_t MERAMON_SHAKE_DATA;
uint8_t MERAMON_SHAKE_FRAME_COUNT;
int32_t MERAMON_SHAKE_BACKUP_OFFSET_Y;
int16_t MERAMON_SHAKE_HEIGHT;
int16_t MERAMON_SHAKE_WIDTH;
int16_t MERAMON_SHAKE_POS_Y;
int16_t MERAMON_SHAKE_POS_X;
uint8_t MERAMON_SHAKE_RED;
int8_t MAIN_D_80134DF9;

static void *map_sbss_order[] = {
	&MAIN_D_80134DF9,
	&MERAMON_SHAKE_RED,
	&MERAMON_SHAKE_POS_X,
	&MERAMON_SHAKE_POS_Y,
	&MERAMON_SHAKE_WIDTH,
	&MERAMON_SHAKE_HEIGHT,
	&MERAMON_SHAKE_BACKUP_OFFSET_Y,
#if !VERSION_IS(US)
	&MERAMON_SHAKE_DATA,
	&MERAMON_SHAKE_FRAME_COUNT,
#else
	&MERAMON_SHAKE_FRAME_COUNT,
	&MERAMON_SHAKE_DATA,
#endif
	&TRN_LOADING_COMPLETE,
	&TARGET_MAP,
	&SCRIPT_MAP_CHANGE_STATE,
	&CAMERA_MOVE_DELTA_Y,
	&CAMERA_MOVE_DELTA_X,
	&CAMERA_MOVE_FINAL_Y,
	&CAMERA_MOVE_FINAL_X,
	&CAMERA_MOVE_DIFF_Y,
	&CAMERA_MOVE_DIFF_X,
	&CAMERA_MOVE_DRAW_OFFSET_Y,
	&CAMERA_DATA,
	&CAMERA_Y_PREVIOUS,
	&CAMERA_X_PREVIOUS,
	&CAMERA_FOLLOW_PLAYER,
	&PREV_TILE_Y,
	&MAP_TILE_Y,
	&PREV_TILE_X,
	&MAP_TILE_X,
	&PLAYER_OFFSET_Y,
	&PLAYER_OFFSET_X,
	&DRAW_OFFSET_LIMIT_Y_MIN,
	&DRAW_OFFSET_LIMIT_Y_MAX,
	&DRAW_OFFSET_LIMIT_X_MIN,
	&DRAW_OFFSET_LIMIT_X_MAX,
	&SKIP_DAYTIME_TRANSITION,
	&CAMERA_UPDATE_TILES,
	&CAMERA_HAS_TARGET,
	&CAMERA_REACHED_TARGET,
	&PREVIOUS_EXIT,
	&CURRENT_EXIT,
	&PREVIOUS_SCREEN,
	&CURRENT_SCREEN,
	&DAYTIME_TRANSITION_TARGET,
	&DAYTIME_TRANSITION_ACTIVE,
	&SKIP_MAP_FILE_READ,
	&CURRENT_TIME_OF_DAY,
	&DAYTIME_TRANSITION_FRAME,
};

int8_t MAP_TILES[35];
MapTiles MAP_TILE_DATA;
GsF_LIGHT MAP_LIGHTS[3];
u_long *MAP_CLUTS[3];
VECTOR CAMERA_TARGET;

static void *map_bss_order[] = {
	&CAMERA_TARGET,
	MAP_CLUTS,
	MAP_LIGHTS,
	&MAP_TILE_DATA,
	MAP_TILES,
};

void initializePartnerWaypoint(void)
{
	int16_t tileX;
	int16_t tileY;
	int32_t i;

	PARTNER_WAYPOINT_CURRENT = 0;
	PARTNER_WAYPOINT_COUNT = 0;
	getModelTile(&ENTITY_TABLE[0]->posData->location, &tileX, &tileY);
	PARTNER_TAMER_PREVIOUS_TILE_X = tileX;
	PARTNER_TAMER_PREVIOUS_TILE_Y = tileY;
	for (i = 0; i < 30; i++) {
		PARTNER_WAYPOINT_X[i] = PARTNER_WAYPOINT_Y[i] = 0;
	}
}

void popPartnerWaypoint(void)
{
	PARTNER_WAYPOINT_COUNT--;
	PARTNER_WAYPOINT_CURRENT++;
	PARTNER_WAYPOINT_CURRENT = PARTNER_WAYPOINT_CURRENT % 30;
}

void addTamerWaypoint(int16_t index, int16_t x, int16_t y)
{
	TAMER_WAYPOINT_X[index] = x;
	TAMER_WAYPOINT_Y[index] = y;
	TAMER_WAYPOINT_COUNT++;
}

void getClosestTileOffScreen(int8_t *outX, int8_t *outY, int8_t *targetX,
			int8_t *targetY)
{
	int16_t rem;
	int32_t i;
	int16_t quot;
	int16_t dist;
	int16_t acc;
	int16_t sign;
	int16_t dx;
	int16_t dy;
	int8_t x;
	int8_t y;

	dy = *targetY - *outY;
	dx = *targetX - *outX;
	x = *outX;
	y = *outY;

	if (abs(dx) >= abs(dy)) {
		acc = dist = abs(dx);
		quot = dy / dist;
		rem = dy % dist;
		if (rem < 0) {
			sign = -1;
		} else {
			sign = 1;
		}
		rem = abs(rem);
		for (i = 0; i < dist; i++) {
			if (rem != 0) {
				acc -= rem;
				if (acc <= 0) {
					y += sign;
					acc += dist;
				}
			} else {
				y += quot;
			}
			if (dx > 0) {
				x++;
			} else {
				x--;
			}
			if (isTileOffScreen(x, y) != 0) {
				*outX = x;
				*outY = y;
				return;
			}
		}
	} else {
		acc = dist = abs(dy);
		quot = dx / dist;
		rem = dx % dist;
		if (rem < 0) {
			sign = -1;
		} else {
			sign = 1;
		}
		rem = abs(rem);
		for (i = 0; i < dist; i++) {
			if (rem != 0) {
				acc -= rem;
				if (acc <= 0) {
					x += sign;
					acc += dist;
				}
			} else {
				x += quot;
			}
			if (dy > 0) {
				y++;
			} else {
				y--;
			}
			if (isTileOffScreen(x, y) != 0) {
				*outX = x;
				*outY = y;
				return;
			}
		}
	}

	*outX = -1;
}

int32_t entityCheckCombatArea(Entity *entity, VECTOR *target, int32_t w,
			      int32_t h)
{
	int32_t j;
	DVECTOR screen2;
	DVECTOR screen1;
	SVECTOR corner;
	int16_t radius;
	VECTOR *loc;
	int32_t i;

	if (GAME_STATE == 4) {
		return 0;
	}

	GsSetLsMatrix(&GsWSMATRIX);

	loc = &entity->posData->location;
	radius = DIGIMON_DATA[entity->type].radius;

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 4; j++) {
			corner.vx = loc->vx + (radius * QUADRANTS[i][0]);
			corner.vy = loc->vy + (i * -200);
			corner.vz = loc->vz + (radius * QUADRANTS[i][1]);

			gte_ldv0(&corner);
			gte_rtps();
			gte_stsxy((long *)&screen1);

			corner.vx = target->vx + (radius * QUADRANTS[i][0]);
			corner.vy = target->vy + (i * -200);
			corner.vz = target->vz + (radius * QUADRANTS[i][1]);

			gte_ldv0(&corner);
			gte_rtps();
			gte_stsxy((long *)&screen2);

			if (hasMovedOutsideCombatArea(&screen2, &screen1, w, h) != 0) {
				return 1;
			}
		}
	}

	return 0;
}

int32_t hasMovedOutsideCombatArea(DVECTOR *previousv, DVECTOR *currentv,
				  int16_t width, int16_t height)
{
	if (((currentv->vx - COMBAT_AREA_X) < (-width / 2)) &&
	    (currentv->vx < previousv->vx)) {
		return 1;
	}
	if (((currentv->vx - COMBAT_AREA_X) > (width / 2)) &&
	    (currentv->vx > previousv->vx)) {
		return 1;
	}
	if (((currentv->vy - COMBAT_AREA_Y) < (-height / 2)) &&
	    (currentv->vy < previousv->vy)) {
		return 1;
	}
	if (((currentv->vy - COMBAT_AREA_Y) > (height / 2)) &&
	    (currentv->vy > previousv->vy)) {
		return 1;
	}
	return 0;
}

void initializeTamerWaypoints(void)
{
	int16_t tileX;
	int16_t tileY;

	TAMER_WAYPOINT_CURRENT = 0;
	TAMER_WAYPOINT_COUNT = 0;
	getModelTile(&ENTITY_TABLE[0]->posData->location, &tileX, &tileY);
	TAMER_START_TILE_X = tileX;
	TAMER_START_TILE_Y = tileY;
	TAMER_PREVIOUS_TILE_X = tileX;
	TAMER_PREVIOUS_TILE_Y = tileY;
	TAMER_WAYPOINT_ACTIVE = 0;
}

void clearTamerWaypoints(void)
{
	TAMER_WAYPOINT_CURRENT = 0;
	TAMER_WAYPOINT_COUNT = 0;
	TAMER_WAYPOINT_ACTIVE = 0;
}

int32_t isFiveTileWidePathBlocked(int8_t x1, int8_t y1, int8_t x2, int8_t y2)
{
	long i;

	if (abs(x2 - x1) >= abs(y2 - y1)) {
		for (i = -2; i < 3; i++) {
			if (isLinearPathBlocked(x1, y1 + i, x2, y2 + i) != 0) {
				break;
			}
		}
	} else {
		for (i = -2; i < 3; i++) {
			if (isLinearPathBlocked(x1 + i, y1, x2 + i, y2) != 0) {
				break;
			}
		}
	}
	if (i != 3) {
		return 1;
	} else {
		return 0;
	}
}

int32_t isLinearPathBlocked(int8_t x1, int8_t y1, int8_t x2, int8_t y2)
{
	int16_t rem;
	int32_t i;
	int16_t quot;
	int16_t dist;
	int16_t acc;
	int16_t sign;
	int16_t dx;
	int16_t dy;
	int8_t x;
	int8_t y;

	if (x1 < 0) {
		x1 = 0;
	}
	if (x2 < 0) {
		x2 = 0;
	}
	if (y1 < 0) {
		y1 = 0;
	}
	if (y2 < 0) {
		y2 = 0;
	}
	if (x1 >= 100) {
		x1 = 99;
	}
	if (x2 >= 100) {
		x2 = 99;
	}
	if (y1 >= 100) {
		y1 = 99;
	}
	if (y2 >= 100) {
		y2 = 99;
	}

	if ((x1 == x2) && (y1 == y2)) {
		return 0;
	}

	dy = y2 - y1;
	dx = x2 - x1;
	x = x1;
	y = y1;

	if (abs(dx) >= abs(dy)) {
		acc = dist = abs(dx);
		quot = dy / dist;
		rem = dy % dist;
		if (rem < 0) {
			sign = -1;
		} else {
			sign = 1;
		}
		rem = abs(rem);
		for (i = 0; i < dist; i++) {
			if (rem != 0) {
				acc -= rem;
				if (acc <= 0) {
					y += sign;
					acc += dist;
				}
			} else {
				y += quot;
			}
			if (dx > 0) {
				x++;
			} else {
				x--;
			}
			if ((((uint8_t *)MAP_COLLISION_DATA)[x + (y * 100)] & 0x80) != 0) {
				return 1;
			}
		}
	} else {
		acc = dist = abs(dy);
		quot = dx / dist;
		rem = dx % dist;
		if (rem < 0) {
			sign = -1;
		} else {
			sign = 1;
		}
		rem = abs(rem);
		for (i = 0; i < dist; i++) {
			if (rem != 0) {
				acc -= rem;
				if (acc <= 0) {
					x += sign;
					acc += dist;
				}
			} else {
				x += quot;
			}
			if (dy > 0) {
				y++;
			} else {
				y--;
			}
			if ((((uint8_t *)MAP_COLLISION_DATA)[x + (y * 100)] & 0x80) != 0) {
				return 1;
			}
		}
	}

	return 0;
}

void partnerTickCollision(void)
{
#ifdef __MWERKS__
	int32_t isFiveTileWidePathBlocked(int8_t x1, int8_t y1, int8_t x2, int8_t y2);
	int32_t isTileOffScreen(int8_t tileX, int8_t tileZ);
#endif
	int16_t last;
	int16_t modelTileX;
	int16_t modelTileY;
	int8_t tamerTileX;
	int8_t tamerTileY;
	int8_t partnerTileX;
	int8_t partnerTileY;
	int16_t index;
	int32_t i;
	int16_t rotationY;
	int16_t collision;
	int32_t posX;
	int32_t posZ;

	getEntityTile(ENTITY_TABLE[0], &tamerTileX, &tamerTileY);
	if ((tamerTileX != PARTNER_TAMER_PREVIOUS_TILE_X) ||
	    (tamerTileY != PARTNER_TAMER_PREVIOUS_TILE_Y)) {
		getEntityTile(ENTITY_TABLE[1], &partnerTileX, &partnerTileY);
		if (isFiveTileWidePathBlocked(tamerTileX, tamerTileY, partnerTileX, partnerTileY) == 1) {
			if (PARTNER_WAYPOINT_COUNT != 0) {
				last = (PARTNER_WAYPOINT_CURRENT + PARTNER_WAYPOINT_COUNT - 1) % 30;
				if (isFiveTileWidePathBlocked(tamerTileX, tamerTileY, PARTNER_WAYPOINT_X[last], PARTNER_WAYPOINT_Y[last]) == 1) {
					setPartnerWaypoint((int16_t)((PARTNER_WAYPOINT_CURRENT + PARTNER_WAYPOINT_COUNT) % 30),
							   PARTNER_TAMER_PREVIOUS_TILE_X,
							   PARTNER_TAMER_PREVIOUS_TILE_Y);
				}
			} else {
				initializePartnerWaypoint();
				setPartnerWaypoint(PARTNER_WAYPOINT_CURRENT, PARTNER_TAMER_PREVIOUS_TILE_X,
						   PARTNER_TAMER_PREVIOUS_TILE_Y);
			}
		} else {
			initializePartnerWaypoint();
		}
	}

	if (PARTNER_WAYPOINT_COUNT >= 2) {
		for (i = 0; i < 3; i++) {
			index = (PARTNER_WAYPOINT_CURRENT + PARTNER_WAYPOINT_COUNT - 2 - i) % 30;
			if (isFiveTileWidePathBlocked(tamerTileX, tamerTileY, PARTNER_WAYPOINT_X[index], PARTNER_WAYPOINT_Y[index]) == 0) {
				PARTNER_WAYPOINT_COUNT -= (int8_t)(i + 1);
				break;
			}
			if (index == PARTNER_WAYPOINT_CURRENT) {
				break;
			}
		}
	}

	if (PARTNER_WAYPOINT_COUNT != 0) {
		GsSetLsMatrix(&GsWSMATRIX);
		if (isTileOffScreen(PARTNER_WAYPOINT_X[PARTNER_WAYPOINT_CURRENT], PARTNER_WAYPOINT_Y[PARTNER_WAYPOINT_CURRENT]) != 0) {
			if (ENTITY_TABLE[1]->isOnScreen == 0) {
				posX = (PARTNER_WAYPOINT_X[PARTNER_WAYPOINT_CURRENT] - 50) * 100 + 50;
				posZ = (50 - PARTNER_WAYPOINT_Y[PARTNER_WAYPOINT_CURRENT]) * 100 - 50;
				ENTITY_TABLE[1]->posData->location.vx = posX;
				ENTITY_TABLE[1]->posData->location.vz = posZ;
				ENTITY_TABLE[1]->anim.locX = posX << 15;
				ENTITY_TABLE[1]->anim.locZ = posZ << 15;
				popPartnerWaypoint();
			}
		}
	}

	if ((GAME_STATE == 0) || (GAME_STATE == 3)) {
		rotationY = ENTITY_TABLE[1]->posData->rotation.vy;
		if (PARTNER_WAYPOINT_COUNT != 0) {
			getModelTile(&ENTITY_TABLE[1]->posData->location,
				     &modelTileX, &modelTileY);
			entityLookAtTile(ENTITY_TABLE[1],
					 PARTNER_WAYPOINT_X[PARTNER_WAYPOINT_CURRENT],
					 PARTNER_WAYPOINT_Y[PARTNER_WAYPOINT_CURRENT]);
			if ((modelTileX == PARTNER_WAYPOINT_X[PARTNER_WAYPOINT_CURRENT]) &&
			    (modelTileY == PARTNER_WAYPOINT_Y[PARTNER_WAYPOINT_CURRENT])) {
				popPartnerWaypoint();
			}
		} else {
			entityLookAtLocation(ENTITY_TABLE[1],
					     &ENTITY_TABLE[0]->posData->location);
		}
	}

	collision = entityCheckCollision(NULL, ENTITY_TABLE[1], 0, 0);
	if ((0 <= collision) && (collision < 10)) {
		if (GAME_STATE != 3) {
			ENTITY_TABLE[1]->posData->rotation.vy = rotationY;
			collisionGrace(ENTITY_TABLE[0], ENTITY_TABLE[1], 0, 0);
		} else if (collision == 0) {
			startBattleIdleAnimation(&PARTNER_ENTITY.digimonEntity,
						 &PARTNER_ENTITY.digimonEntity.stats, 0);
		}
	} else if ((GAME_STATE == 3) &&
		   (entityIsInEntity(ENTITY_TABLE[0],
				     ENTITY_TABLE[1]) == 1)) {
		handleBattleIdle(&PARTNER_ENTITY.digimonEntity,
				 &PARTNER_ENTITY.digimonEntity.stats, 0);
	}

	PARTNER_TAMER_PREVIOUS_TILE_X = tamerTileX;
	PARTNER_TAMER_PREVIOUS_TILE_Y = tamerTileY;
}

void setPartnerWaypoint(int16_t index, int16_t tileX, int16_t tileY)
{
	PARTNER_WAYPOINT_X[index] = tileX;
	PARTNER_WAYPOINT_Y[index] = tileY;
	PARTNER_WAYPOINT_COUNT++;
}

void entityLookAtLocation(Entity *entity, VECTOR *target)
{
	VECTOR diff;
	VECTOR *loc;
	SVECTOR *rot;
	PositionData *posData;

	if (target != NULL) {
		posData = entity->posData;
		loc = &posData->location;
		rot = &posData->rotation;
		diff.vx = target->vx - loc->vx;
		diff.vy = target->vy - loc->vy;
		diff.vz = target->vz - loc->vz;
		rot->vy = _atan(diff.vz, diff.vx);
	}
}

int16_t entityCheckCollision(Entity *source, Entity *entity, int32_t arg2,
			     int32_t arg3)
{
	int32_t i;
	VECTOR *loc;
	int32_t dx;
	int32_t dz;
	Entity *other;
	VECTOR saved;
	int16_t direction;

	loc = &entity->posData->location;
	saved = *loc;
	entity->anim.animFlag &= 5;
	entityMoveForward(entity);

	dx = loc->vx - saved.vx;
	dz = loc->vz - saved.vz;
	direction = entity->posData->rotation.vy;

	if ((direction == 0x400) || (direction == 0xC00)) {
		dz = 0;
	}

	if ((direction == 0) || (direction == 0x800)) {
		dx = 0;
	}

	if ((arg2 != 0) && (arg3 != 0) &&
	    (entityCheckCombatArea(entity, &saved, arg2, arg3) != 0)) {
		*loc = saved;
		return 11;
	}

	if (checkMapCollision(entity, dx, dz) != 0) {
		*loc = saved;
		return 10;
	}

	for (i = 0; i < 10; i++) {
		other = ENTITY_TABLE[i];

		if ((isInvisible(other) == 0) && (other != entity) &&
		    (other != source)) {
			if (i != 0) {
				if (((DigimonEntity *)other)->stats.current.currentHP == 0) {
					continue;
				}
			} else if ((GAME_STATE == 1) &&
				   ((entity->anim.animId == 0x24) ||
				    (entity->anim.animId == 0x23))) {
				continue;
			}

			if (entityCheckEntityCollision(entity, other, dx, dz) != 0) {
				*loc = saved;
				return (int16_t)i;
			}
		}
	}

	*loc = saved;

	return -1;
}

void collisionGrace(Entity *target, Entity *entity, int32_t dx, int32_t dy)
{
	int16_t *rotY;
	int16_t clockwise;
	int32_t i;
	int16_t dir;
	int16_t diff;
	int16_t startRot;
	int16_t counter;
	uint8_t flags;

	entity->anim.animFlag &= 5;
	flags = 0;
	if (entityCheckCollision(target, entity, dx, dy) == -1) {
		return;
	}

	rotY = &entity->posData->rotation.vy;
	startRot = *rotY;
	dir = *rotY / 512;
	for (i = 0; i < 4; i++) {
		*rotY = COLLISION_GRACE_ROTATION[dir][i];
		if (entityCheckCollision(target, entity, dx, dy) == -1) {
			clockwise = startRot - *rotY;
			if (clockwise < 0) {
				clockwise = (clockwise + 0x1000) & 0xFFF;
			}

			counter = *rotY - startRot;
			if (counter < 0) {
				counter = (counter + 0x1000) & 0xFFF;
			}

			diff = clockwise - counter;
			*rotY = startRot;
			if (diff < 0) {
				if (clockwise >= 0x51) {
					*rotY = (*rotY + 0xfb0) & 0xFFF;
				} else {
					*rotY = (*rotY + 0x1000 - clockwise) &
						0xfff;
				}
			} else {
				if (counter >= 0x51) {
					*rotY = (*rotY + 0x50) & 0xFFF;
				} else {
					*rotY = (*rotY + counter) & 0xfff;
				}
			}

			entity->anim.animFlag |= flags;
			return;
		} else {
			flags |= (uint8_t)(8 << (*rotY / 1024));
		}
	}

	*rotY = (startRot + 32) & 0xfff;
	entity->anim.animFlag |= 2;
}

void tickTamerWaypoints(void)
{
#ifdef __MWERKS__
	int32_t isLinearPathBlocked(int8_t x1, int8_t y1, int8_t x2, int8_t y2);
#endif
	int16_t index;
	int8_t tileX;
	int8_t tileY;

	getEntityTile(ENTITY_TABLE[0], &tileX, &tileY);

	if ((tileX != TAMER_PREVIOUS_TILE_X) || (tileY != TAMER_PREVIOUS_TILE_Y)) {
		if (isLinearPathBlocked(tileX, tileY, TAMER_START_TILE_X, TAMER_START_TILE_Y) != 0) {
			if (TAMER_WAYPOINT_COUNT != 0) {
				index = (TAMER_WAYPOINT_CURRENT + TAMER_WAYPOINT_COUNT - 1)
					% 30;
				if (isLinearPathBlocked(tileX, tileY, TAMER_WAYPOINT_X[index], TAMER_WAYPOINT_Y[index]) != 0) {
					addTamerWaypoint((TAMER_WAYPOINT_CURRENT + TAMER_WAYPOINT_COUNT) % 30,
							 TAMER_PREVIOUS_TILE_X,
							 TAMER_PREVIOUS_TILE_Y);
				}
			} else {
				addTamerWaypoint(0, TAMER_PREVIOUS_TILE_X,
						 TAMER_PREVIOUS_TILE_Y);
			}
		} else {
			clearTamerWaypoints();
		}
	}

	if (TAMER_WAYPOINT_COUNT > 1) {
		if (isLinearPathBlocked(tileX, tileY, TAMER_WAYPOINT_X[TAMER_WAYPOINT_ACTIVE], TAMER_WAYPOINT_Y[TAMER_WAYPOINT_ACTIVE]) == 0) {
			TAMER_WAYPOINT_COUNT = TAMER_WAYPOINT_ACTIVE + 1;
			if (TAMER_WAYPOINT_COUNT > 1) {
				TAMER_WAYPOINT_ACTIVE = TAMER_WAYPOINT_COUNT - 2;
			} else {
				TAMER_WAYPOINT_ACTIVE = TAMER_WAYPOINT_COUNT - 1;
			}
		} else {
			TAMER_WAYPOINT_ACTIVE--;
			if (TAMER_WAYPOINT_ACTIVE < 0) {
				TAMER_WAYPOINT_ACTIVE = TAMER_WAYPOINT_COUNT - 2;
			}
		}
	}

	TAMER_PREVIOUS_TILE_X = tileX;
	TAMER_PREVIOUS_TILE_Y = tileY;
}

void getEntityTile(Entity *entity, int8_t *outTileX,
			    int8_t *outTileY)
{
	int16_t tileX;
	int16_t tileY;

	getModelTile(&entity->posData->location, &tileX, &tileY);
	*outTileX = tileX;
	*outTileY = tileY;
}

void tickPartnerWaypoints(void)
{
#ifdef __MWERKS__
	int32_t isTileOffScreen(int8_t tileX, int8_t tileZ);
#endif
	int16_t tamerTileX;
	int16_t tamerTileY;
	int16_t partnerTileX;
	int16_t partnerTileY;
	int8_t fromX;
	int8_t fromY;
	int8_t toX;
	int8_t toY;
	int32_t count;
	int16_t index;
	int16_t posX;
	int16_t posZ;

	if (ENTITY_TABLE[1]->isOnScreen == 1) {
		return;
	}

	getModelTile(&ENTITY_TABLE[0]->posData->location,
		     &tamerTileX, &tamerTileY);
	fromX = tamerTileX;
	fromY = tamerTileY;
	GsSetLsMatrix(&GsWSMATRIX);

	for (count = PARTNER_WAYPOINT_COUNT; count > 0; count--) {
		index = ((PARTNER_WAYPOINT_CURRENT + count) - 1) % 30;
		if (isTileOffScreen(PARTNER_WAYPOINT_X[index],
					   PARTNER_WAYPOINT_Y[index]) != 0) {
			toX = PARTNER_WAYPOINT_X[index];
			toY = PARTNER_WAYPOINT_Y[index];
			break;
		}
	}

	if (count != 0) {
		while (PARTNER_WAYPOINT_CURRENT != index) {
			popPartnerWaypoint();
		}
		popPartnerWaypoint();
	}

	getModelTile(&ENTITY_TABLE[1]->posData->location,
		     &partnerTileX, &partnerTileY);
	toX = partnerTileX;
	toY = partnerTileY;
	getClosestTileOffScreen(&fromX, &fromY, &toX, &toY);

	if (fromX != -1) {
		posX = ((fromX - 50) * 100) + 50;
		posZ = ((50 - fromY) * 100) - 50;
		ENTITY_TABLE[1]->posData->location.vx = posX;
		ENTITY_TABLE[1]->posData->location.vz = posZ;
		ENTITY_TABLE[1]->anim.locX = posX << 15;
		ENTITY_TABLE[1]->anim.locZ = posZ << 15;
	}
}

int32_t isTileOffScreen(int8_t tileX, int8_t tileZ)
{
	DVECTOR screen;
	SVECTOR world;
	int16_t x;
	int16_t z;

	x = ((tileX - 50) * 100) + 50;
	z = ((50 - tileZ) * 100) - 50;
	world.vx = x;
	world.vy = 0;
	world.vz = z;

	gte_ldv0(&world);
	gte_rtps();
	gte_stsxy((long *)&screen);

	screen.vx -= 160 - DRAWING_OFFSET_X;
	screen.vy -= 120 - DRAWING_OFFSET_Y;

	if ((screen.vx < -200) || (screen.vx > 200) || (screen.vy < -160) ||
	    (screen.vy > 160)) {
		return 1;
	}

	return 0;
}

void entityMoveForward(Entity *entity)
{
	PositionData *posData;
	int16_t *delta;
	EntityAnim *anim;
	int32_t vz;
	VECTOR dir;
	VECTOR out;
	MomentumData momentum;
	MATRIX m;
	int16_t *subDelta;

	momentum = *entity->anim.momentum;

	subDelta = &momentum.subDelta[8];
	delta = &momentum.delta[8];

	if (*subDelta != 0) {
		if ((momentum.subScale[8] - *subDelta) <= 0) {
			vz = (*delta + momentum.subValue[8]) << 15;
		} else {
			vz = *delta << 15;
		}
	} else {
		vz = *delta << 15;
	}

	posData = entity->posData;
	dir.vx = 0;
	dir.vy = 0;
	dir.vz = vz;

	RotMatrix(&posData->rotation, &m);
	ApplyMatrixLV(&m, &dir, &out);

	anim = &entity->anim;

	posData->location.vx = (anim->locX + out.vx) >> 15;
	posData->location.vy = 0;
	posData->location.vz = (anim->locZ + out.vz) >> 15;
}

int32_t checkMapCollision(Entity *entity, int32_t diffY, int32_t diffX)
{
	if ((diffX > 0) && (checkMapCollisionX(entity, 0) != 0)) {
		return 1;
	}

	if ((diffX < 0) && (checkMapCollisionX(entity, 1) != 0)) {
		return 1;
	}

	if ((diffY < 0) && (checkMapCollisionZ(entity, 0) != 0)) {
		return 1;
	}

	if ((diffY > 0) && (checkMapCollisionZ(entity, 1) != 0)) {
		return 1;
	}

	return 0;
}

int32_t entityCheckEntityCollision(Entity *entity, Entity *other,
				   int32_t diffX, int32_t diffZ)
{
	int32_t left;
	int32_t bottom;
	long myRadius;
	long radius;
	int32_t right;
	int32_t top;
	RECT rect;
	VECTOR *myLoc;
	VECTOR *loc;
	int16_t rotation;

	myLoc = &entity->posData->location;
	myRadius = DIGIMON_DATA[entity->type].radius;
	loc = &other->posData->location;
	radius = DIGIMON_DATA[other->type].radius;
	setRECT(&rect, loc->vx - radius, loc->vz + radius, radius * 2, radius * 2);
	left = myLoc->vx - myRadius;
	right = myLoc->vx + myRadius;
	bottom = myLoc->vz + myRadius;
	top = myLoc->vz - myRadius;
	if ((GAME_STATE == 0) && (entity == ENTITY_TABLE[0])) {
		if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & CONFIRM_BUTTON) != 0) &&
		    (entity->anim.animId == 0)) {
			rotation = entity->posData->rotation.vy;
			if ((0 < rotation) && (rotation < 0x800)) {
				left -= 50;
			}
			if ((0x800 < rotation) && (rotation < 0x1000)) {
				right += 50;
			}
			if ((rotation < 0x400) || (0xC00 < rotation)) {
				top -= 50;
			}
			if ((0x400 < rotation) && (rotation < 0xC00)) {
				bottom += 50;
			}
		}
		if (isRectInRect(&rect, left, bottom, left, top) != 0) {
			return 1;
		}
		if (isRectInRect(&rect, right, bottom, right, top) != 0) {
			return 1;
		}
		if (isRectInRect(&rect, left, top, right, top) != 0) {
			return 1;
		}
		if (isRectInRect(&rect, left, bottom, right, bottom) != 0) {
			return 1;
		}
		if (entityIsInEntity(entity, other) != 0) {
			return 1;
		}
	} else {
		rotation = entity->posData->rotation.vy;
		if ((diffX < 0) &&
		    (isRectInRect(&rect, left, bottom, left, top) != 0)) {
			return 1;
		}
		if ((diffX > 0) &&
		    (isRectInRect(&rect, right, bottom, right, top) != 0)) {
			return 1;
		}
		if ((diffZ < 0) &&
		    (isRectInRect(&rect, left, top, right, top) != 0)) {
			return 1;
		}
		if ((diffZ > 0) &&
		    (isRectInRect(&rect, left, bottom, right, bottom) != 0)) {
			return 1;
		}
	}
	return 0;
}

int32_t isRectInRect(RECT *rect, int32_t left, int32_t top, int32_t right,
		     int32_t bottom)
{
	int32_t rectRight;
	int32_t rectBottom;

	rectRight = rect->x + rect->w;
	rectBottom = rect->y - rect->h;
	if (rectRight < left) {
		return 0;
	}
	if (right < rect->x) {
		return 0;
	}
	if (top < rectBottom) {
		return 0;
	}
	if (bottom > rect->y) {
		return 0;
	}
	return 1;
}

int32_t entityIsInEntity(Entity *entityA, Entity *entityB)
{
	VECTOR *locA;
	VECTOR *locB;
	int16_t radA;
	int16_t radB;

	locA = &entityA->posData->location;
	radA = DIGIMON_DATA[entityA->type].radius;
	locB = &entityB->posData->location;
	radB = DIGIMON_DATA[entityB->type].radius;

	if ((locA->vx - radA) > (locB->vx + radB)) {
		return 0;
	}

	if ((locA->vx + radA) < (locB->vx - radB)) {
		return 0;
	}

	if ((locA->vz - radA) > (locB->vz + radB)) {
		return 0;
	}

	if ((locA->vz + radA) < (locB->vz - radB)) {
		return 0;
	}

	return 1;
}

int32_t entityIsOffScreen(Entity *entity, int32_t width, int32_t height)
{
	int32_t i;
	DVECTOR screen;
	SVECTOR corner;
	int16_t radius;
	VECTOR *loc;
	int32_t j;

	if ((entity->type == 0x98) && (entity->anim.animId == 0x1C)) {
		return 0;
	}

	GsSetLsMatrix(&GsWSMATRIX);
	loc = &entity->posData->location;
	radius = DIGIMON_DATA[entity->type].radius;

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 4; j++) {
			corner.vx = loc->vx + (radius * QUADRANTS[j][0]);
			corner.vy = loc->vy + i * -DIGIMON_DATA[entity->type].height;
			corner.vz = loc->vz + (radius * QUADRANTS[j][1]);
			gte_ldv0(&corner);
			gte_rtps();
			gte_stsxy((long *)&screen);
			if (isOffScreen(&screen, width, height) == 0) {
				return 0;
			}
		}
	}

	return 1;
}

int32_t isOffScreen(DVECTOR *pos, int16_t width, int16_t height)
{
	if ((pos->vx - (160 - DRAWING_OFFSET_X)) < (-width / 2)) {
		return 1;
	}
	if ((pos->vx - (160 - DRAWING_OFFSET_X)) > (width / 2)) {
		return 1;
	}
	if ((pos->vy - (120 - DRAWING_OFFSET_Y)) < (-height / 2)) {
		return 1;
	}
	if ((pos->vy - (120 - DRAWING_OFFSET_Y)) > (height / 2)) {
		return 1;
	}
	return 0;
}

void initializeMap(void)
{
	int32_t i;

	for (i = 0; i < 35; i++) {
		MAP_TILES[i] = -1;
	}

	for (i = 0; i < 35; i++) {
		MAP_TILE_DATA.tiles[i].imagePtr = 0;
		MAP_TILE_DATA.tiles[i].posX = MAP_TILE_DATA.tiles[i].posY = 0;
		MAP_TILE_DATA.tiles[i].texU = MAP_TILE_DATA.tiles[i].texV = 0;
	}

	PREVIOUS_SCREEN = CURRENT_SCREEN = 0xcc;
	PREVIOUS_EXIT = CURRENT_EXIT = 9;
	CAMERA_REACHED_TARGET = -1;
	CAMERA_HAS_TARGET = 0;
	DAYTIME_TRANSITION_FRAME = 25;
	CURRENT_TIME_OF_DAY = 0;
	CAMERA_UPDATE_TILES = 0;
	SKIP_MAP_FILE_READ = 0;
	addObject(0xfa0, 0, 0, renderMap);
	DAYTIME_TRANSITION_ACTIVE = 0;
	SKIP_DAYTIME_TRANSITION = 0;
}

void renderMap(int32_t arg0)
{
	POLY_FT4 *prim;
	GsOT_TAG *ot;
	int16_t startTile;
	int16_t stride;
	int16_t columns;
	int8_t skipTimeOfDay;
	int32_t col;
	int32_t row;

	tickCameraFollowPlayer();

	startTile = MAP_TILE_X + ((long)MAP_TILE_Y * MAP_TILE_DATA.width);
	columns = stride = MAP_TILE_DATA.width;
	if (stride > 3) {
		columns = 4;
	}

	skipTimeOfDay = MAP_ENTRIES[CURRENT_SCREEN].flags & 0x40;
	if ((skipTimeOfDay == 0) && ((CURRENT_FRAME % 1200) == 0)) {
		if (HOUR == 16) {
			initializeDaytimeTransition(0);
		}

		if (HOUR == 20) {
			initializeDaytimeTransition(1);
		}

		if (HOUR == 6) {
			initializeDaytimeTransition(2);
		}
	}

	ot = ACTIVE_ORDERING_TABLE->org;

	for (row = 0; row < 3; row++) {
		for (col = 0; col < columns; col++) {
			prim = (POLY_FT4 *)GsGetWorkBase();
			SetPolyFT4(prim);

			if (MAP_TILES[col + (startTile + stride * row)] == -1) {
				setRGB0(prim, 0, 0, 0);
			} else {
				setRGB0(prim, 0x80, 0x80, 0x80);
			}

			if ((MAP_TILE_DATA.tiles[col + (startTile + stride * row)].texV % 256) != 0) {
				setUVDataPolyFT4(prim, 0, MAP_TILE_DATA.tiles[col + (startTile + stride * row)].texV % 256, 0x80, 0x7F);
			} else {
				setUVDataPolyFT4(prim, 0, MAP_TILE_DATA.tiles[col + (startTile + stride * row)].texV % 256, 0x80, 0x80);
			}

			prim->tpage = MAP_TILE_DATA.tiles[col + (startTile + stride * row)].tpage;
			prim->clut = MAP_TILE_DATA.tiles[col + (startTile + stride * row)].clut;
			setPosDataMapTile(&MAP_TILE_DATA.tiles[col + (startTile + stride * row)],
					  MAP_TILE_DATA.cameraX, MAP_TILE_DATA.cameraY, prim);
			AddPrim(&ot[0xfff], prim);
			++prim;

			GsSetWorkBase((PACKET *)prim);
		}
	}

	renderMapOverlays(MAP_TILE_DATA.objects,
			  MAP_TILE_DATA.cameraX, MAP_TILE_DATA.cameraY);

	CAMERA_X_PREVIOUS = MAP_TILE_DATA.cameraX;
	CAMERA_Y_PREVIOUS = MAP_TILE_DATA.cameraY;
}

void loadMap(int16_t mapId)
{
	int32_t *offsets;
	long entityOffset;
	long collisionOffset;
	long objectOffset;
	long image8Offset;
	long image4Offset;
	long setupOffset;
	int32_t i;
	char path[32];
	int32_t result;

	offsets = (int32_t *)GENERAL_BUFFER_PTR;
	if (SKIP_MAP_FILE_READ == 0) {
		buildMapPath(path, MAP_ENTRIES[mapId].filename, MAP_FILE_EXT_MAP, mapId);
		readFile(path, (void *)offsets);
	}

	setupOffset = *offsets++;
	result = loadMapSetup((int32_t *)(GENERAL_BUFFER_PTR + setupOffset));
	clearMapObjects(MAP_TILE_DATA.objects);

	if ((MAP_ENTRIES[mapId].num8bppImages != 0) ||
	    (MAP_ENTRIES[mapId].num4bppImages != 0)) {
		if (MAP_ENTRIES[mapId].num8bppImages != 0) {
			for (i = 0; i < MAP_ENTRIES[mapId].num8bppImages; i++) {
				image8Offset = *offsets++;
				loadMapImage1(GENERAL_BUFFER_PTR + image8Offset);
			}
		}

		if (MAP_ENTRIES[mapId].num4bppImages != 0) {
			for (i = 0; i < MAP_ENTRIES[mapId].num4bppImages; i++) {
				image4Offset = *offsets++;
				loadMapImage2(GENERAL_BUFFER_PTR + image4Offset, i);
			}
		}

		objectOffset = *offsets++;
		loadMapObjects(MAP_TILE_DATA.objects,
			       GENERAL_BUFFER_PTR + objectOffset, mapId);
	}

	entityOffset = *offsets++;

	clearMapDigimon();
	loadMapEntities(GENERAL_BUFFER_PTR + entityOffset, mapId,
			CURRENT_EXIT);

	if (MAP_ENTRIES[mapId].doorsId != 0) {
		loadDoors(MAP_ENTRIES[mapId].doorsId - 1);
	}
	if ((mapId > 100) && (mapId < 104)) {
		loadWarpCrystals(mapId);
	}
	if (mapId == 0xa5) {
		initializeTrainingPoop();
	}

	collisionOffset = *offsets;
	loadMapCollisionData(GENERAL_BUFFER_PTR + collisionOffset);

	if (SKIP_MAP_FILE_READ == 0) {
		result = getMapSoundId(mapId);
		loadMapSounds(result);
	}

	CURRENT_SCREEN = mapId;

	initializePartnerWaypoint();
	initializeTamerWaypoints();
	checkFishingMap(CURRENT_SCREEN, 0);
	checkCurlingMap(CURRENT_SCREEN);
	checkShopMap(CURRENT_SCREEN);
	checkArenaMap(CURRENT_SCREEN);

	SKIP_MAP_FILE_READ = 0;
}

// clang-format off
void buildMapPath(out, name, suffix, mapId)
	char *out;
	char *name;
	int8_t *suffix;
	int16_t mapId;
// clang-format on
{
	char *prefix;
	char *digits;
	char *separator;
	uint8_t index;

	prefix = MAP_PATH_PREFIX;
	digits = MAP_PATH_DIGITS;
	separator = MAP_PATH_SEPARATOR;

	while (*prefix != '\0') {
		*out++ = *prefix++;
	}

	index = (mapId / 15) + 1;
	if (index >= 10) {
		*out++ = digits[index / 10];
	}

	*out++ = digits[index % 10];

	while (*separator != '\0') {
		*out++ = *separator++;
	}

	while (*name != '\0') {
		*out++ = *name++;
	}

	while (*suffix != '\0') {
		*out++ = *suffix++;
	}

	*out = '\0';
}

int32_t loadMapSetup(int32_t *data)
{
	int32_t ambientR;
	int32_t ambientG;
	int32_t ambientB;
	int8_t regionA[4];
	int8_t regionB[4];
	int32_t i;
	int32_t j;
	int16_t favoredRegion;

	GS_VIEWPOINT.vpx = *data++ << 1;
	GS_VIEWPOINT.vpy = *data++ << 1;
	GS_VIEWPOINT.vpz = *data++ << 1;
	GS_VIEWPOINT.vrx = *data++ << 1;
	GS_VIEWPOINT.vry = *data++ << 1;
	GS_VIEWPOINT.vrz = *data++ << 1;
	GsSetRefView2(&GS_VIEWPOINT);

	for (i = 0; i < 3; i++) {
		LIGHT_DATA.light[i].vx = *data++;
		LIGHT_DATA.light[i].vy = *data++;
		LIGHT_DATA.light[i].vz = *data++;
		LIGHT_DATA.light[i].r = *data++;
		LIGHT_DATA.light[i].g = *data++;
		LIGHT_DATA.light[i].b = *data++;
		GsSetFlatLight(i, &LIGHT_DATA.light[i]);
		MAP_LIGHTS[i] = LIGHT_DATA.light[i];
	}

	ambientR = *data++;
	ambientG = *data++;
	ambientB = *data++;
	GsSetAmbient(0x800, 0x800, 0x800);
	GsSetProjection(VIEWPORT_DISTANCE = *data++);

	for (j = 0; j < 4; j++) {
		regionA[j] = *data++;
	}

	for (j = 0; j < 4; j++) {
		regionB[j] = *data++;
	}

	MAP_TILE_DATA.partnerAreaResponse = 0;
	favoredRegion = RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].favoredRegion;

	for (j = 0; j < 4; j++) {
		if (regionA[j] == favoredRegion) {
			MAP_TILE_DATA.partnerAreaResponse = 1;
			break;
		}

		if (regionB[j] == favoredRegion) {
			MAP_TILE_DATA.partnerAreaResponse = 2;
			break;
		}
	}

	MAP_TILE_DATA.width = *data++;
	MAP_TILE_DATA.height = *data++;

	for (i = 0; i < MAP_TILE_DATA.width * MAP_TILE_DATA.height; i++) {
		MAP_TILES[i] = *data++;
	}

	return *data;
}

int32_t getMapSoundId(int16_t mapId)
{
	return MAP_ENTRIES[mapId].flags & 0x1f;
}

void readMapTFS(int32_t mapId)
{
	char path[32];

	buildMapPath(path, MAP_ENTRIES[mapId].filename, MAP_FILE_EXT_TFS, mapId);
	readFile(path, GENERAL_BUFFER_PTR);
}

void setupMap(void)
{
	u_long *data;
	MapTileData *tile;
	int32_t x;
	int32_t y;
	int32_t i;
	int16_t numCluts;

	data = (u_long *)GENERAL_BUFFER_PTR;
	data++;
	numCluts = *data++;

	for (i = 0; i < numCluts; i++) {
		LoadClut(data, 0, 0x1e1 + i);
		MAP_CLUTS[i] = data;
		data += 128;
	}

	updateTimeOfDay();

	tile = MAP_TILE_DATA.tiles;
	for (y = 0; y < MAP_TILE_DATA.height; y++) {
		for (x = 0; x < MAP_TILE_DATA.width; x++) {
			tile->tileId = MAP_TILES[x + (y * MAP_TILE_DATA.width)];
			if (tile->tileId == -1) {
				fillTileData(tile, (uint8_t *)NULL,
					     x % 4 * 64 + 768, y % 3 * 128,
					     x * 128, y * 128);
			} else {
				data++;
				fillTileData(tile, (uint8_t *)data,
					     x % 4 * 64 + 768, y % 3 * 128,
					     x * 128, y * 128);
				data += 4096;
			}
			tile++;
		}
	}

	MAP_TILE_DATA.cameraX = ((MAP_TILE_DATA.width * 128) / 2) - 160;
	MAP_TILE_DATA.cameraY = ((MAP_TILE_DATA.height * 128) / 2) - 120;
	DRAW_OFFSET_LIMIT_X_MAX = MAP_TILE_DATA.cameraX + 160;
	DRAW_OFFSET_LIMIT_X_MIN = -(MAP_TILE_DATA.width * 128 - 320 - DRAW_OFFSET_LIMIT_X_MAX);
	DRAW_OFFSET_LIMIT_Y_MAX = MAP_TILE_DATA.cameraY + 120;
	DRAW_OFFSET_LIMIT_Y_MIN = -(MAP_TILE_DATA.height * 128 - 240 - DRAW_OFFSET_LIMIT_Y_MAX);
	PLAYER_OFFSET_X = DRAWING_OFFSET_X = 160;
	PLAYER_OFFSET_Y = DRAWING_OFFSET_Y = 120;

	initializeDrawingOffsets(MAP_TILE_DATA.tiles);

	MAP_TILE_X = MAP_TILE_DATA.cameraX / 128;
	if (MAP_TILE_DATA.width < 5) {
		MAP_TILE_X = 0;
	} else if ((MAP_TILE_X + 4) > MAP_TILE_DATA.width) {
		MAP_TILE_X -= (MAP_TILE_X + 4) - MAP_TILE_DATA.width;
	}

	PREV_TILE_X = MAP_TILE_X;

	MAP_TILE_Y = MAP_TILE_DATA.cameraY / 128;
	if (MAP_TILE_DATA.height < 4) {
		MAP_TILE_Y = 0;
	} else if ((MAP_TILE_Y + 3) > MAP_TILE_DATA.height) {
		MAP_TILE_Y -= (MAP_TILE_Y + 3) - MAP_TILE_DATA.height;
	}

	PREV_TILE_Y = MAP_TILE_Y;

	uploadMapTileImages(MAP_TILE_DATA.tiles,
			    MAP_TILE_X + ((long)MAP_TILE_Y * MAP_TILE_DATA.width));
	calcMapObjectOrder(MAP_TILE_DATA.objects);

	CAMERA_FOLLOW_PLAYER = 1;
}

void updateTimeOfDay(void)
{
	int32_t i;
	uint8_t red;
	uint8_t green;

	if (DAYTIME_TRANSITION_ACTIVE == 1) {
		removeObject(0xfbe, DAYTIME_TRANSITION_TARGET);
	}

	if ((MAP_ENTRIES[CURRENT_SCREEN].flags & 0x40) != 0) {
		LoadClut(MAP_CLUTS[0], 0, 480);
	} else {
		red = green = 10;

		if ((HOUR >= 16) && (HOUR < 20)) {
			LoadClut(MAP_CLUTS[1], 0, 480);
			green = 7;
			CURRENT_TIME_OF_DAY = 0;
		} else if ((HOUR < 6) || (HOUR >= 20)) {
			LoadClut(MAP_CLUTS[2], 0, 480);
			red = green = 5;
			CURRENT_TIME_OF_DAY = 1;
		} else {
			LoadClut(MAP_CLUTS[0], 0, 480);
			CURRENT_TIME_OF_DAY = 2;
		}

		for (i = 0; i < 3; i++) {
			LIGHT_DATA.light[i].r = (red * MAP_LIGHTS[i].r) / 10;
			LIGHT_DATA.light[i].g = (green * MAP_LIGHTS[i].g) / 10;
			LIGHT_DATA.light[i].b = (green * MAP_LIGHTS[i].b) / 10;
			GsSetFlatLight(i, &LIGHT_DATA.light[i]);
		}
	}

	DAYTIME_TRANSITION_FRAME = 0x19;
	DAYTIME_TRANSITION_ACTIVE = 0;
	SKIP_DAYTIME_TRANSITION = 0;
}

void fillTileData(MapTileData *tile, uint8_t *imagePtr,
		  int16_t texU, int16_t texV,
		  int16_t posX, int16_t posY)
{
	tile->tpage = GetTPage(1, 0, texU, texV);
	tile->clut = GetClut(0, 480);
	tile->posX = posX;
	tile->posY = posY;
	tile->imagePtr = imagePtr;
	tile->texU = texU;
	tile->texV = texV;
}

void initializeDrawingOffsets(MapTileData *tiles)
{
	SVECTOR viewRef;
	SVECTOR tamerPos;
	SVECTOR viewScreen;
	SVECTOR tamerScreen;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	viewRef.vx = GS_VIEWPOINT.vrx;
	viewRef.vy = GS_VIEWPOINT.vry;
	viewRef.vz = GS_VIEWPOINT.vrz;
	gte_ldv0(&viewRef);
	gte_rtps();
	gte_stsxy(&viewScreen);
	copyVector(&tamerPos, &TAMER_ENTITY.entity.posData->location);
	gte_ldv0(&tamerPos);
	gte_rtps();
	gte_stsxy(&tamerScreen);
	MAP_TILE_DATA.cameraX += tamerScreen.vx - viewScreen.vx;
	MAP_TILE_DATA.cameraY += tamerScreen.vy - viewScreen.vy;
	DRAWING_OFFSET_X += viewScreen.vx - tamerScreen.vx;
	DRAWING_OFFSET_Y += viewScreen.vy - tamerScreen.vy;
	PLAYER_OFFSET_X += viewScreen.vx - tamerScreen.vx;
	PLAYER_OFFSET_Y += viewScreen.vy - tamerScreen.vy;

	if (MAP_TILE_DATA.cameraX < 0) {
		MAP_TILE_DATA.cameraX = 0;
		DRAWING_OFFSET_X = DRAW_OFFSET_LIMIT_X_MAX;
	} else if (MAP_TILE_DATA.cameraX > ((MAP_TILE_DATA.width * 128) - 320)) {
		MAP_TILE_DATA.cameraX = (MAP_TILE_DATA.width * 128) - 320;
		DRAWING_OFFSET_X = DRAW_OFFSET_LIMIT_X_MIN;
	}

	if (MAP_TILE_DATA.cameraY < 0) {
		MAP_TILE_DATA.cameraY = 0;
		DRAWING_OFFSET_Y = DRAW_OFFSET_LIMIT_Y_MAX;
	} else if (MAP_TILE_DATA.cameraY > ((MAP_TILE_DATA.height * 128) - 240)) {
		MAP_TILE_DATA.cameraY = (MAP_TILE_DATA.height * 128) - 240;
		DRAWING_OFFSET_Y = DRAW_OFFSET_LIMIT_Y_MIN;
	}
}

void uploadMapTileImages(MapTileData *tiles, int16_t index)
{
	RECT rect;
	int16_t stride;
	int16_t count;
	int32_t i;

	count = stride = *(int8_t *)((int8_t *)tiles + 0x1bb8);
	if (stride > 3) {
		count = 4;
	}

	for (i = 0; i < count; i++) {
		setRECT(&rect, tiles[index + i].texU, tiles[index + i].texV, 64, 128);

		if (tiles[index + i].tileId == -1) {
			ClearImage(&rect, 0, 0, 0);
		} else {
			LoadImage(&rect, (u_long *)tiles[index + i].imagePtr);
		}

		DrawSync(0);

		setRECT(&rect, tiles[index + stride + i].texU, tiles[index + stride + i].texV, 64, 128);

		if (tiles[index + stride + i].tileId == -1) {
			ClearImage(&rect, 0, 0, 0);
		} else {
			LoadImage(&rect, (u_long *)tiles[index + stride + i].imagePtr);
		}

		DrawSync(0);

		if (MAP_TILE_DATA.height > 2) {
			setRECT(&rect, tiles[index + (stride * 2) + i].texU, tiles[index + (stride * 2) + i].texV, 64, 128);

			if (tiles[index + (stride * 2) + i].tileId == -1) {
				ClearImage(&rect, 0, 0, 0);
			} else {
				LoadImage(&rect,
					  (u_long *)tiles[index + stride * 2 + i].imagePtr);
			}

			DrawSync(0);
		}
	}
}

void unloadMap(void)
{
	int32_t i;

	for (i = 0; i < 35; i++) {
		MAP_TILE_DATA.tiles[i].imagePtr = 0;
		MAP_TILE_DATA.tiles[i].posX = MAP_TILE_DATA.tiles[i].posY = 0;
		MAP_TILE_DATA.tiles[i].texU = MAP_TILE_DATA.tiles[i].texV = 0;
	}

	MAP_TILE_DATA.width = MAP_TILE_DATA.height = 0;
	MAP_TILE_DATA.cameraX = MAP_TILE_DATA.cameraY = 0;
	DRAWING_OFFSET_X = 160;
	DRAWING_OFFSET_Y = 120;

	unloadMapParts();
}

void tickCameraFollowPlayer(void)
{
	SVECTOR worldPos;
	SVECTOR screenStart;
	SVECTOR screenEnd;
	int16_t oldCameraX;
	int16_t oldCameraY;

	if ((GAME_STATE == 0) &&
	    (CAMERA_FOLLOW_PLAYER == 1) &&
	    (tamerGetState() == 0) &&
	    (((POLLED_INPUT & 0x1000) != 0) ||
	     ((POLLED_INPUT & 0x4000) != 0) ||
	     ((POLLED_INPUT & 0x8000) != 0) ||
	     ((POLLED_INPUT & 0x2000) != 0))) {
		SetRotMatrix(&GsWSMATRIX);
		SetTransMatrix(&GsWSMATRIX);
		copyVector(&worldPos, &STORED_TAMER_POS);
		gte_ldv0(&worldPos);
		gte_rtps();
		gte_stsxy(&screenStart);
		copyVector(&worldPos, &TAMER_ENTITY.entity.posData->location);
		gte_ldv0(&worldPos);
		gte_rtps();
		gte_stsxy(&screenEnd);
		oldCameraX = MAP_TILE_DATA.cameraX;
		oldCameraY = MAP_TILE_DATA.cameraY;
		MAP_TILE_DATA.cameraX += screenEnd.vx - screenStart.vx;
		MAP_TILE_DATA.cameraY += screenEnd.vy - screenStart.vy;
		updateDrawingOffsets((DVECTOR *)&screenStart,
				   (DVECTOR *)&screenEnd);
		handleTileUpdate(POLLED_INPUT, 0);
	}
}

// clang-format off
void initializeDaytimeTransition(timeOfDay)
	int16_t timeOfDay;
// clang-format on
{
	if ((PARTNER_STATE != 8) && (PARTNER_STATE != 0xd) &&
	    (tamerGetState() != 0x11) && (tamerGetState() != 0x13) &&
	    (CURRENT_TIME_OF_DAY != timeOfDay)) {
		DAYTIME_TRANSITION_FRAME = 0;
		CURRENT_TIME_OF_DAY = timeOfDay;
		addObject(0xfbe, timeOfDay, tickDaytimeTransition, NULL);
	}
}

void setPosDataMapTile(MapTileData *tile, int16_t camX, int16_t camY,
		       POLY_FT4 *prim)
{
	int16_t x;
	int16_t y;

	x = (tile->posX - 160) - (camX - (160 - DRAWING_OFFSET_X));
	y = (tile->posY - 120) - (camY - (120 - DRAWING_OFFSET_Y));
	setPosDataPolyFT4(prim, x, y, 128, 128);
}

// clang-format off
void tickDaytimeTransition(transition)
	int16_t transition;
// clang-format on
{
	int16_t *clutA;
	int16_t *clutB;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t redB;
	uint8_t greenB;
	uint8_t blueB;
	int8_t steps;
	int32_t i;
	int16_t clut[256];
	int32_t j;

	DAYTIME_TRANSITION_TARGET = transition;
	DAYTIME_TRANSITION_ACTIVE = 1;

	if ((GAME_STATE == 0) && (SKIP_DAYTIME_TRANSITION != 1) &&
	    (PARTNER_STATE != 8) && (PARTNER_STATE != 0xd) &&
	    (tamerGetState() != 0x11) && (tamerGetState() != 0x13)) {
		if (transition == 0) {
			clutA = (int16_t *)MAP_CLUTS[0];
			clutB = (int16_t *)MAP_CLUTS[1];
		} else if (transition == 1) {
			clutA = (int16_t *)MAP_CLUTS[1];
			clutB = (int16_t *)MAP_CLUTS[2];
		} else {
			clutA = (int16_t *)MAP_CLUTS[2];
			clutB = (int16_t *)MAP_CLUTS[0];
		}

		if (DAYTIME_TRANSITION_FRAME >= 25) {
			LoadClut((u_long *)clutB, 0, 480);

			for (i = 0; i < 3; i++) {
				if (transition == 0) {
					LIGHT_DATA.light[i].g = MAP_LIGHTS[i].g * 7 / 10;
					LIGHT_DATA.light[i].b = MAP_LIGHTS[i].b * 7 / 10;
				} else if (transition == 1) {
					LIGHT_DATA.light[i].r = MAP_LIGHTS[i].r / 2;
					LIGHT_DATA.light[i].g = MAP_LIGHTS[i].g / 2;
					LIGHT_DATA.light[i].b = MAP_LIGHTS[i].b / 2;
				} else {
					LIGHT_DATA.light[i] = MAP_LIGHTS[i];
				}

				GsSetFlatLight(i, &LIGHT_DATA.light[i]);
			}

			removeObject(0xfbe, transition);
			DAYTIME_TRANSITION_ACTIVE = 0;
			SKIP_DAYTIME_TRANSITION = 0;
		} else {
			steps = 6 - (DAYTIME_TRANSITION_FRAME / 5);

			for (j = 0; j < 256; j++) {
				red = clutA[0] & 0x1f;
				green = (clutA[0] >> 5) & 0x1f;
				blue = (clutA[0] >> 10) & 0x1f;
				redB = clutB[0] & 0x1f;
				greenB = (clutB[0] >> 5) & 0x1f;
				blueB = (clutB[0] >> 10) & 0x1f;

				red -= (red - redB) / steps;
				green -= (green - greenB) / steps;
				blue -= (blue - blueB) / steps;

				clut[j] = (clutA[0] & 0x8000) | red | (green << 5) | (blue << 10);
				clutA++;
				clutB++;
			}

			LoadClut((u_long *)clut, 0, 480);
			DrawSync(0);
			DAYTIME_TRANSITION_FRAME++;
		}
	}
}

int32_t isInDaytimeTransition(void)
{
	if (DAYTIME_TRANSITION_FRAME >= 25) {
		return 1;
	} else {
		return 0;
	}
}

void updateDrawingOffsets(DVECTOR *current, DVECTOR *previous)
{
	int32_t atEdgeX;
	int32_t atEdgeY;

	cameraIsAtEdge(&atEdgeX, &atEdgeY);

	PLAYER_OFFSET_X += (int16_t)(current->vx - previous->vx);
	PLAYER_OFFSET_Y += (int16_t)(current->vy - previous->vy);

	if (atEdgeX == 1) {
		DRAWING_OFFSET_X += current->vx - previous->vx;

		if ((POLLED_INPUT & 0x8000) != 0) {
			if (PLAYER_OFFSET_X < DRAWING_OFFSET_X) {
				DRAWING_OFFSET_X -= current->vx - previous->vx;
				MAP_TILE_DATA.cameraX -= (int16_t)(previous->vx - current->vx);
			}
		} else if ((POLLED_INPUT & 0x2000) != 0) {
			if (PLAYER_OFFSET_X > DRAWING_OFFSET_X) {
				DRAWING_OFFSET_X -= current->vx - previous->vx;
				MAP_TILE_DATA.cameraX -= (int16_t)(previous->vx - current->vx);
			}
		}
	}

	if (atEdgeY == 1) {
		DRAWING_OFFSET_Y += current->vy - previous->vy;

		if ((POLLED_INPUT & 0x1000) != 0) {
			if (PLAYER_OFFSET_Y < DRAWING_OFFSET_Y) {
				DRAWING_OFFSET_Y -= current->vy - previous->vy;
				MAP_TILE_DATA.cameraY -= (int16_t)(previous->vy - current->vy);
			}
		} else if ((POLLED_INPUT & 0x4000) != 0) {
			if (PLAYER_OFFSET_Y > DRAWING_OFFSET_Y) {
				DRAWING_OFFSET_Y -= current->vy - previous->vy;
				MAP_TILE_DATA.cameraY -= (int16_t)(previous->vy - current->vy);
			}
		}
	}
}

void handleTileUpdate(int32_t input, int32_t force)
{
	if ((((input & 0x1000) != 0) ||
	     ((input & 0x8000) != 0) ||
	     ((input & 0x2000) != 0)) &&
	    (MAP_TILE_DATA.cameraY <= CAMERA_Y_PREVIOUS)) {
		if ((MAP_TILE_DATA.height >= 8) &&
		    (MAP_TILE_DATA.cameraY >= 0x201) &&
		    (MAP_TILE_DATA.cameraY < 0x281)) {
			MAP_TILE_Y = 4;
		} else if ((MAP_TILE_DATA.height >= 7) &&
			   (MAP_TILE_DATA.cameraY >= 0x181) &&
			   (MAP_TILE_DATA.cameraY < 0x201)) {
			MAP_TILE_Y = 3;
		} else if ((MAP_TILE_DATA.height >= 6) &&
			   (MAP_TILE_DATA.cameraY >= 0x101) &&
			   (MAP_TILE_DATA.cameraY < 0x181)) {
			MAP_TILE_Y = 2;
		} else if ((MAP_TILE_DATA.height >= 5) &&
			   (MAP_TILE_DATA.cameraY >= 0x81) &&
			   (MAP_TILE_DATA.cameraY < 0x101)) {
			MAP_TILE_Y = 1;
		} else if ((MAP_TILE_DATA.height >= 4) &&
			   (MAP_TILE_DATA.cameraY >= 0) &&
			   (MAP_TILE_DATA.cameraY < 0x81)) {
			MAP_TILE_Y = 0;
		}

		if (force == 0) {
			updateTileRow(0);
		}
	} else if (((input & 0x4000) != 0) ||
		   ((input & 0x8000) != 0) ||
		   ((input & 0x2000) != 0)) {
		if (MAP_TILE_DATA.cameraY >= CAMERA_Y_PREVIOUS) {
			if ((MAP_TILE_DATA.height >= 8) &&
			    (MAP_TILE_DATA.cameraY >= 0x280) &&
			    (MAP_TILE_DATA.cameraY < 0x301)) {
				MAP_TILE_Y = 5;
			} else if ((MAP_TILE_DATA.height >= 7) &&
				   (MAP_TILE_DATA.cameraY >= 0x200) &&
				   (MAP_TILE_DATA.cameraY < 0x280)) {
				MAP_TILE_Y = 4;
			} else if ((MAP_TILE_DATA.height >= 6) &&
				   (MAP_TILE_DATA.cameraY >= 0x180) &&
				   (MAP_TILE_DATA.cameraY < 0x200)) {
				MAP_TILE_Y = 3;
			} else if ((MAP_TILE_DATA.height >= 5) &&
				   (MAP_TILE_DATA.cameraY >= 0x100) &&
				   (MAP_TILE_DATA.cameraY < 0x180)) {
				MAP_TILE_Y = 2;
			} else if ((MAP_TILE_DATA.height >= 4) &&
				   (MAP_TILE_DATA.cameraY >= 0x80)
				   && (MAP_TILE_DATA.cameraY < 0x100)) {
				MAP_TILE_Y = 1;
			}

			if (MAP_TILE_DATA.cameraY >= ((MAP_TILE_DATA.height * 128) - 240)) {
				MAP_TILE_Y = MAP_TILE_DATA.height - 3;
				if (MAP_TILE_Y < 0) {
					MAP_TILE_Y = 0;
				}
			}

			if (force == 0) {
				updateTileRow(1);
			}
		}
	}

	if ((((input & 0x8000) != 0) ||
	     ((input & 0x1000) != 0) ||
	     ((input & 0x4000) != 0)) &&
	    (MAP_TILE_DATA.cameraX <= CAMERA_X_PREVIOUS)) {
		if ((MAP_TILE_DATA.width >= 0xb) &&
		    (MAP_TILE_DATA.cameraX >= 0x30a) &&
		    (MAP_TILE_DATA.cameraX < 0x381)) {
			MAP_TILE_X = 6;
		} else if ((MAP_TILE_DATA.width >= 0xa) &&
			   (MAP_TILE_DATA.cameraX >= 0x28a) &&
			   (MAP_TILE_DATA.cameraX < 0x301)) {
			MAP_TILE_X = 5;
		} else if ((MAP_TILE_DATA.width >= 9) &&
			   (MAP_TILE_DATA.cameraX >= 0x20a) &&
			   (MAP_TILE_DATA.cameraX < 0x281)) {
			MAP_TILE_X = 4;
		} else if ((MAP_TILE_DATA.width >= 8) &&
			   (MAP_TILE_DATA.cameraX >= 0x18a) &&
			   (MAP_TILE_DATA.cameraX < 0x201)) {
			MAP_TILE_X = 3;
		} else if ((MAP_TILE_DATA.width >= 7) &&
			   (MAP_TILE_DATA.cameraX >= 0x10a) &&
			   (MAP_TILE_DATA.cameraX < 0x181)) {
			MAP_TILE_X = 2;
		} else if ((MAP_TILE_DATA.width >= 6) &&
			   (MAP_TILE_DATA.cameraX >= 0x8a) &&
			   (MAP_TILE_DATA.cameraX < 0x101)) {
			MAP_TILE_X = 1;
		} else if ((MAP_TILE_DATA.width >= 5) &&
			   (MAP_TILE_DATA.cameraX >= 0xa) &&
			   (MAP_TILE_DATA.cameraX < 0x81)) {
			MAP_TILE_X = 0;
		}

		if (force == 0) {
			updateTileColumn(0);
		}
	} else if (((input & 0x2000) != 0) ||
		   ((input & 0x1000) != 0) ||
		   ((input & 0x4000) != 0)) {
		if (MAP_TILE_DATA.cameraX >= CAMERA_X_PREVIOUS) {
			if ((MAP_TILE_DATA.width >= 0xb) &&
			    (MAP_TILE_DATA.cameraX >= 0x38a) &&
			    (MAP_TILE_DATA.cameraX < 0x3f7)) {
				MAP_TILE_X = 7;
			} else if ((MAP_TILE_DATA.width >= 0xa) &&
				   (MAP_TILE_DATA.cameraX >= 0x30a) &&
				   (MAP_TILE_DATA.cameraX < 0x377)) {
				MAP_TILE_X = 6;
			} else if ((MAP_TILE_DATA.width >= 9) &&
				   (MAP_TILE_DATA.cameraX >= 0x28a) &&
				   (MAP_TILE_DATA.cameraX < 0x2f7)) {
				MAP_TILE_X = 5;
			} else if ((MAP_TILE_DATA.width >= 8) &&
				   (MAP_TILE_DATA.cameraX >= 0x20a) &&
				   (MAP_TILE_DATA.cameraX < 0x277)) {
				MAP_TILE_X = 4;
			} else if ((MAP_TILE_DATA.width >= 7) &&
				   (MAP_TILE_DATA.cameraX >= 0x18a) &&
				   (MAP_TILE_DATA.cameraX < 0x1f7)) {
				MAP_TILE_X = 3;
			} else if ((MAP_TILE_DATA.width >= 6) &&
				   (MAP_TILE_DATA.cameraX >= 0x10a) &&
				   (MAP_TILE_DATA.cameraX < 0x177)) {
				MAP_TILE_X = 2;
			} else if ((MAP_TILE_DATA.width >= 5) &&
				   (MAP_TILE_DATA.cameraX >= 0x8a) &&
				   (MAP_TILE_DATA.cameraX < 0xf6)) {
				MAP_TILE_X = 1;
			}

			if (MAP_TILE_DATA.cameraX >= ((MAP_TILE_DATA.width * 128) - 320)) {
				MAP_TILE_X = MAP_TILE_DATA.width - 4;
				if (MAP_TILE_X < 0) {
					MAP_TILE_X = 0;
				}
			}

			if (force == 0) {
				updateTileColumn(1);
			}
		}
	}
	if (force == 1) {
		uploadMapTileImages(MAP_TILE_DATA.tiles,
				    MAP_TILE_X + ((long)MAP_TILE_Y * MAP_TILE_DATA.width));
	}
}

void cameraIsAtEdge(int32_t *outX, int32_t *outY)
{
	*outX = *outY = 1;

	if (MAP_TILE_DATA.cameraX < 0) {
		MAP_TILE_DATA.cameraX = 0;
		DRAWING_OFFSET_X = DRAW_OFFSET_LIMIT_X_MAX;
		*outX = 0;
	} else if (MAP_TILE_DATA.cameraX > (MAP_TILE_DATA.width * 128 - 320)) {
		MAP_TILE_DATA.cameraX = MAP_TILE_DATA.width * 128 - 320;
		DRAWING_OFFSET_X = DRAW_OFFSET_LIMIT_X_MIN;
		*outX = 0;
	}

	if (MAP_TILE_DATA.cameraY < 0) {
		MAP_TILE_DATA.cameraY = 0;
		DRAWING_OFFSET_Y = DRAW_OFFSET_LIMIT_Y_MAX;
		*outY = 0;
	} else if (MAP_TILE_DATA.cameraY > (MAP_TILE_DATA.height * 128 - 240)) {
		MAP_TILE_DATA.cameraY = MAP_TILE_DATA.height * 128 - 240;
		DRAWING_OFFSET_Y = DRAW_OFFSET_LIMIT_Y_MIN;
		*outY = 0;
	}
}

// clang-format off
void createCameraMovement(target, instanceId)
	VECTOR *target;
	int16_t instanceId;
// clang-format on
{
	CAMERA_TARGET = *target;
	addObject(0xfb1, instanceId, tickCameraMovement, 0);
	CAMERA_REACHED_TARGET = -1;
}

// clang-format off
void tickCameraMovement(instanceId)
	int16_t instanceId;
// clang-format on
{
	SVECTOR target;
	SVECTOR view;
	SVECTOR screenTarget;
	SVECTOR screenView;
	int32_t atEdgeX;
	int32_t atEdgeY;
	int16_t stepX;
	int16_t qx;
	int16_t qy;
	int16_t stepY;
	int16_t oldCameraX;
	int16_t oldCameraY;
	uint32_t flags;

	if (CAMERA_HAS_TARGET == 0) {
		SetRotMatrix(&GsWSMATRIX);
		SetTransMatrix(&GsWSMATRIX);
		copyVector(&target, &CAMERA_TARGET);
		view.vx = GS_VIEWPOINT.vrx;
		view.vy = GS_VIEWPOINT.vry;
		view.vz = GS_VIEWPOINT.vrz;
		gte_ldv0(&target);
		gte_rtps();
		gte_stsxy((long *)&screenTarget);
		gte_ldv0(&view);
		gte_rtps();
		gte_stsxy((long *)&screenView);
		CAMERA_DATA = DRAWING_OFFSET_X;
		CAMERA_MOVE_DRAW_OFFSET_Y = DRAWING_OFFSET_Y;
		CAMERA_MOVE_DIFF_X = (screenView.vx - screenTarget.vx) + 160;
		CAMERA_MOVE_DIFF_Y = (screenView.vy - screenTarget.vy) + 120;
		PLAYER_OFFSET_X += CAMERA_MOVE_DIFF_X - PLAYER_OFFSET_X;
		PLAYER_OFFSET_Y += CAMERA_MOVE_DIFF_Y - PLAYER_OFFSET_Y;

		if (CAMERA_MOVE_DIFF_X < DRAW_OFFSET_LIMIT_X_MIN) {
			CAMERA_MOVE_DIFF_X = DRAW_OFFSET_LIMIT_X_MIN;
		}

		if (CAMERA_MOVE_DIFF_X > DRAW_OFFSET_LIMIT_X_MAX) {
			CAMERA_MOVE_DIFF_X = DRAW_OFFSET_LIMIT_X_MAX;
		}

		if (CAMERA_MOVE_DIFF_Y < DRAW_OFFSET_LIMIT_Y_MIN) {
			CAMERA_MOVE_DIFF_Y = DRAW_OFFSET_LIMIT_Y_MIN;
		}

		if (CAMERA_MOVE_DIFF_Y > DRAW_OFFSET_LIMIT_Y_MAX) {
			CAMERA_MOVE_DIFF_Y = DRAW_OFFSET_LIMIT_Y_MAX;
		}

		CAMERA_MOVE_FINAL_X = MAP_TILE_DATA.cameraX + (CAMERA_DATA - CAMERA_MOVE_DIFF_X);
		CAMERA_MOVE_FINAL_Y = MAP_TILE_DATA.cameraY + (CAMERA_MOVE_DRAW_OFFSET_Y - CAMERA_MOVE_DIFF_Y);
		CAMERA_MOVE_DELTA_X = (CAMERA_MOVE_DIFF_X - CAMERA_DATA) % instanceId;
		CAMERA_MOVE_DELTA_Y = (CAMERA_MOVE_DIFF_Y - CAMERA_MOVE_DRAW_OFFSET_Y) % instanceId;

		if (CAMERA_MOVE_DELTA_X < 0) {
			CAMERA_MOVE_DELTA_X *= -1;
		}

		if (CAMERA_MOVE_DELTA_Y < 0) {
			CAMERA_MOVE_DELTA_Y *= -1;
		}

		unsetCameraFollowPlayer();
		CAMERA_HAS_TARGET = 1;
	}

	qx = (CAMERA_DATA - CAMERA_MOVE_DIFF_X) / instanceId;
	qy = (CAMERA_MOVE_DRAW_OFFSET_Y - CAMERA_MOVE_DIFF_Y) / instanceId;
	stepX = (CAMERA_MOVE_DIFF_X - CAMERA_DATA) / instanceId;
	stepY = (CAMERA_MOVE_DIFF_Y - CAMERA_MOVE_DRAW_OFFSET_Y) / instanceId;
	oldCameraX = MAP_TILE_DATA.cameraX;
	oldCameraY = MAP_TILE_DATA.cameraY;
	MAP_TILE_DATA.cameraX += qx;
	MAP_TILE_DATA.cameraY += qy;
	DRAWING_OFFSET_X += stepX;
	DRAWING_OFFSET_Y += stepY;

	if (CAMERA_MOVE_DELTA_X >= 0) {
		if ((CAMERA_DATA - CAMERA_MOVE_DIFF_X) > 0) {
			MAP_TILE_DATA.cameraX++;
		} else {
			MAP_TILE_DATA.cameraX--;
		}

		if ((CAMERA_MOVE_DIFF_X - CAMERA_DATA) > 0) {
			DRAWING_OFFSET_X++;
		} else {
			DRAWING_OFFSET_X--;
		}

		CAMERA_MOVE_DELTA_X--;
	}

	if (CAMERA_MOVE_DELTA_Y >= 0) {
		if ((CAMERA_MOVE_DRAW_OFFSET_Y - CAMERA_MOVE_DIFF_Y) > 0) {
			MAP_TILE_DATA.cameraY++;
		} else {
			MAP_TILE_DATA.cameraY--;
		}

		if ((CAMERA_MOVE_DIFF_Y - CAMERA_MOVE_DRAW_OFFSET_Y) > 0) {
			DRAWING_OFFSET_Y++;
		} else {
			DRAWING_OFFSET_Y--;
		}

		CAMERA_MOVE_DELTA_Y--;
	}

	cameraIsAtEdge(&atEdgeX, &atEdgeY);

	if (atEdgeX == 1) {
		if ((CAMERA_MOVE_DIFF_X - CAMERA_DATA) < 0) {
			if (DRAWING_OFFSET_X < CAMERA_MOVE_DIFF_X) {
				DRAWING_OFFSET_X = CAMERA_MOVE_DIFF_X;
			}
		} else {
			if (DRAWING_OFFSET_X > CAMERA_MOVE_DIFF_X) {
				DRAWING_OFFSET_X = CAMERA_MOVE_DIFF_X;
			}

			if (CAMERA_DATA == CAMERA_MOVE_DIFF_X) {
				DRAWING_OFFSET_X = CAMERA_MOVE_DIFF_X;
			}
		}

		if (DRAWING_OFFSET_X == CAMERA_MOVE_DIFF_X) {
			MAP_TILE_DATA.cameraX = CAMERA_MOVE_FINAL_X;
			atEdgeX = 0;
		}
	}

	if (atEdgeY == 1) {
		if ((CAMERA_MOVE_DIFF_Y - CAMERA_MOVE_DRAW_OFFSET_Y) < 0) {
			if (DRAWING_OFFSET_Y < CAMERA_MOVE_DIFF_Y) {
				DRAWING_OFFSET_Y = CAMERA_MOVE_DIFF_Y;
			}
		} else {
			if (DRAWING_OFFSET_Y > CAMERA_MOVE_DIFF_Y) {
				DRAWING_OFFSET_Y = CAMERA_MOVE_DIFF_Y;
			}

			if (CAMERA_MOVE_DRAW_OFFSET_Y == CAMERA_MOVE_DIFF_Y) {
				DRAWING_OFFSET_Y = CAMERA_MOVE_DIFF_Y;
			}
		}

		if (DRAWING_OFFSET_Y == CAMERA_MOVE_DIFF_Y) {
			MAP_TILE_DATA.cameraY = CAMERA_MOVE_FINAL_Y;
			atEdgeY = 0;
		}
	}

	flags = 0;
	if (stepX < 0) {
		flags |= 0x2000;
	} else {
		flags |= 0x8000;
	}
	if (stepY < 0) {
		flags |= 0x4000;
	} else {
		flags |= 0x1000;
	}

	if ((((CAMERA_DATA - CAMERA_MOVE_DIFF_X) / instanceId) >= 0x50) ||
	    (((CAMERA_MOVE_DRAW_OFFSET_Y - CAMERA_MOVE_DIFF_Y) / instanceId) >= 0x50)) {
		handleTileUpdate(flags, 1);
	}

	if ((((CAMERA_DATA - CAMERA_MOVE_DIFF_X) / instanceId) < 0x50) ||
	    (((CAMERA_MOVE_DRAW_OFFSET_Y - CAMERA_MOVE_DIFF_Y) / instanceId) < 0x50)) {
		handleTileUpdate(flags, 0);
	}

	if ((atEdgeX == 0) && (atEdgeY == 0)) {
		CAMERA_HAS_TARGET = 0;
		setCameraFollowPlayer();

		if (CAMERA_REACHED_TARGET == 0) {
			CAMERA_REACHED_TARGET = 1;
		}

		removeObject(0xfb1, instanceId);

		if (CAMERA_UPDATE_TILES == 1) {
			uploadMapTileImages(MAP_TILE_DATA.tiles,
					    MAP_TILE_X + ((long)MAP_TILE_Y * MAP_TILE_DATA.width));
			CAMERA_UPDATE_TILES = 0;
		}
	}
}

// clang-format off
void storeEntityLocation(scriptId, out)
	uint8_t scriptId;
	VECTOR *out;
// clang-format on
{
	NPCEntity *npc;
	int32_t i;

	if (scriptId == 0xfd) {
		*out = TAMER_ENTITY.entity.posData->location;
	} else if (scriptId == 0xfc) {
		*out = PARTNER_ENTITY.digimonEntity.entity.posData->location;
	} else {
		npc = NPC_ENTITIES;
		for (i = 0; i < 8; i++) {
			if (npc->scriptId == scriptId) {
				*out = npc->digimonEntity.entity.posData->location;
				return;
			}

			npc++;
		}
	}
}

// clang-format off
int32_t checkCameraMovement(instanceId)
	int16_t instanceId;
// clang-format on
{
	if (CAMERA_REACHED_TARGET == -1) {
		CAMERA_REACHED_TARGET = 0;
		addObject(0xfb1, instanceId,
			  tickCameraMovement, NULL);
		goto out;
	}

	if (CAMERA_REACHED_TARGET == 1) {
		CAMERA_REACHED_TARGET = -1;
		return 1;
	}
out:
	return 0;
}

// clang-format off
void tickCameraMoveTo(x, z, arg2)
	int16_t x;
	int16_t z;
	int16_t arg2;
// clang-format on
{
	CAMERA_TARGET.vx = x;
	CAMERA_TARGET.vy = TAMER_ENTITY.entity.posData->location.vy;
	CAMERA_TARGET.vz = z;

	checkCameraMovement(arg2);
}

// clang-format off
int32_t tickCameraMoveToEntity(scriptId, speed)
	uint8_t scriptId;
	int16_t speed;
// clang-format on
{
	storeEntityLocation(scriptId, &CAMERA_TARGET);

	return checkCameraMovement(speed);
}

void moveCameraByDiff(VECTOR *from, VECTOR *to)
{
	uint32_t flags;
	SVECTOR fromPos;
	SVECTOR toPos;
	SVECTOR fromScreen;
	SVECTOR toScreen;
	int16_t oldCameraX;
	int16_t oldCameraY;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);

	copyVector(&fromPos, from);
	copyVector(&toPos, to);
	gte_ldv0(&fromPos);
	gte_rtps();
	gte_stsxy(&fromScreen);
	gte_ldv0(&toPos);
	gte_rtps();
	gte_stsxy(&toScreen);
	oldCameraX = MAP_TILE_DATA.cameraX;
	oldCameraY = MAP_TILE_DATA.cameraY;
	MAP_TILE_DATA.cameraX += toScreen.vx - fromScreen.vx;
	MAP_TILE_DATA.cameraY += toScreen.vy - fromScreen.vy;

	flags = 0;
	if (from->vx < to->vx) {
		flags |= 0x2000;
	} else if (from->vx > to->vx) {
		flags |= 0x8000;
	}

	if (from->vz < to->vz) {
		flags |= 0x1000;
	} else if (from->vz > to->vz) {
		flags |= 0x4000;
	}

	updateDrawingOffsets((DVECTOR *)&fromScreen, (DVECTOR *)&toScreen);
	handleTileUpdate(flags, 0);
}

// clang-format off
void moveCameraByOffset(diffX, diffY)
	int16_t diffX;
	int16_t diffY;
// clang-format on
{
	int32_t atEdgeX;
	int32_t atEdgeY;

	MAP_TILE_DATA.cameraX += diffX;
	MAP_TILE_DATA.cameraY += diffY;
	DRAWING_OFFSET_X -= diffX;
	DRAWING_OFFSET_Y -= diffY;
	cameraIsAtEdge(&atEdgeX, &atEdgeY);
	handleTileUpdate(POLLED_INPUT, 1);
}

void unsetCameraFollowPlayer(void)
{
	CAMERA_FOLLOW_PLAYER = 0;
}

void setCameraFollowPlayer(void)
{
	CAMERA_FOLLOW_PLAYER = 1;
}

// clang-format off
void updateTileRow(bottom)
	int8_t bottom;
// clang-format on
{
	RECT rect;
	int16_t base;
	int16_t count;
	int16_t start;
	int32_t i;

	if (MAP_TILE_Y != PREV_TILE_Y) {
		base = MAP_TILE_X + ((long)MAP_TILE_Y * MAP_TILE_DATA.width);

		if (MAP_TILE_DATA.width >= 4) {
			count = 4;
		} else {
			count = MAP_TILE_DATA.width;
		}

		if (bottom == 0) {
			start = 0;
		} else {
			start = (int16_t)(MAP_TILE_DATA.width * 2);
		}

		for (i = 0; i < count; i++) {
			setRECT(&rect, MAP_TILE_DATA.tiles[i + (base + start)].texU, MAP_TILE_DATA.tiles[i + (base + start)].texV, 64, 128);

			if (MAP_TILE_DATA.tiles[base + i].tileId == -1) {
				ClearImage(&rect, 0, 0, 0);
			} else {
				LoadImage(&rect, (u_long *)MAP_TILE_DATA.tiles[i + (base + start)].imagePtr);
			}

			DrawSync(0);
		}

		PREV_TILE_Y = MAP_TILE_Y;
	}
}

// clang-format off
void updateTileColumn(right)
	int8_t right;
// clang-format on
{
	RECT rect;
	int16_t base;
	int16_t stride;
	int16_t start;
	int32_t i;

	if (MAP_TILE_X != PREV_TILE_X) {
		base = MAP_TILE_X + ((long)MAP_TILE_Y * MAP_TILE_DATA.width);
		stride = MAP_TILE_DATA.width;

		if (right == 0) {
			start = 0;
		} else {
			start = 3;
		}

		for (i = 0; i < 3; i++) {
			if ((MAP_TILE_DATA.height < 3) && (i == 2)) {
				break;
			}

			setRECT(&rect, MAP_TILE_DATA.tiles[start + (base + stride * i)].texU, MAP_TILE_DATA.tiles[start + (base + stride * i)].texV, 64, 128);

			if (MAP_TILE_DATA.tiles[start + (base + stride * i)].tileId == -1) {
				ClearImage(&rect, 0, 0, 0);
			} else {
				LoadImage(&rect, (u_long *)MAP_TILE_DATA.tiles[start + (base + stride * i)].imagePtr);
			}

			DrawSync(0);
		}

		PREV_TILE_X = MAP_TILE_X;
	}
}

int32_t scriptTickChangeMap(int16_t mapId, int16_t exitId, int32_t showName)
{
	switch (SCRIPT_MAP_CHANGE_STATE) {
	case 0:
		fadeToBlack(20);
		SCRIPT_MAP_CHANGE_STATE = 1;
		PREVIOUS_EXIT = CURRENT_EXIT;
		break;
	case 1:
		if ((showName == 1) && (FADE_OUT_CURRENT == 10)) {
			addMapNameObject(mapId);
		}

		if (FADE_OUT_CURRENT >= 20) {
			changeMap(mapId, exitId);

			STORED_TAMER_POS = TAMER_ENTITY.entity.posData->location;

			if (showName == 1) {
				removeObject(0xfa1, mapId);
			}

			tamerSetState(6);
			fadeFromBlack(20);

			SCRIPT_MAP_CHANGE_STATE = 0;

			if (IS_SCRIPT_PAUSED == 1) {
				partnerSetState(1);
				tamerSetState(0);
				setCameraFollowPlayer();
			}

			checkMapInteraction();

			return 1;
		}

		break;
	}

	return 0;
}

// clang-format off
void addMapNameObject(mapId)
	int16_t mapId;
// clang-format on
{
	clearTextArea();
	drawString(MAP_NAME_PTR[MAP_ENTRIES[mapId].loadingName], 0, 0);
	addObject(0xfa1, mapId, NULL, renderMapName);
}

void changeMap(int16_t mapId, int16_t exitId)
{
	unloadMap();
	removeMapEntities();
	clearDroppedItems();

	PREVIOUS_SCREEN = CURRENT_SCREEN;

	if (CURRENT_SCREEN == 0xcf) {
		if (mapId == 0x26) {
			PREVIOUS_SCREEN = 0x58;
		}

		if (mapId == 0x46) {
			PREVIOUS_SCREEN = 0x45;
		}

		if (mapId == 0x4f) {
			PREVIOUS_SCREEN = 0x11;
		}

		if (mapId == 0x5d) {
			PREVIOUS_SCREEN = 0x5a;
		}

		if (mapId == 0x77) {
			PREVIOUS_SCREEN = 0x76;
		}

		if (mapId == 0x69) {
			PREVIOUS_SCREEN = 0x6a;
		}

		if ((mapId == 0x26) || (mapId == 0x46)) {
			PREVIOUS_EXIT = 2;
		}

		if ((mapId == 0x4f) || (mapId == 0x77)) {
			PREVIOUS_EXIT = 1;
		}

		if ((mapId == 0x5d) || (mapId == 0x69)) {
			PREVIOUS_EXIT = 0;
		}
	}

	if ((CURRENT_SCREEN == 0x2b) && (mapId == 0x2A)) {
		PREVIOUS_SCREEN = 0x29;
		PREVIOUS_EXIT = 1;
	}

	if ((CURRENT_SCREEN == 6) && (mapId == 0x69)) {
		PREVIOUS_SCREEN = 0x6a;
	}

	if ((CURRENT_SCREEN == 0x8e) && (mapId == 0x87)) {
		PREVIOUS_SCREEN = 0x84;
		PREVIOUS_EXIT = 1;
	}

	if ((CURRENT_SCREEN == 5) && (mapId == 0xc)) {
		PREVIOUS_SCREEN = 0xd;
		PREVIOUS_EXIT = 1;
	}

	CURRENT_EXIT = exitId;
	runMapHeadScript(mapId & 0xff);
}

void setDeathMap(int16_t a, int16_t b)
{
	DEATH_MAP = a;
	DEATH_MAP_EXIT = b;
	MAIN_D_801343B4 = 0;
}

int32_t waitForDeathMapLoading(int32_t flag)
{
	if (flag == 0) {
		while (MAIN_D_801343B4 != 0) {
			tickFileReadQueue(0);
		}

		return 0;
	} else {
		return MAIN_D_801343B4;
	}
}

void changeToDeathMap(void)
{
	changeMap(DEATH_MAP, DEATH_MAP_EXIT);
}

// clang-format off
void renderMapName(mapId)
	int16_t mapId;
// clang-format on
{
	int32_t length;

	length = strlen(MAP_NAME_PTR[MAP_ENTRIES[mapId].loadingName]);
#if !VERSION_IS(US)
	renderString(0, -12 - (length / 2) * 6, -6, (length / 2) * 12, 12, 0,
		     0, 0, 0);
#else
	renderString(0, 12 - (length / 2) * 8, -6, length * 8 + 4, 12, 0, 0,
		     0, 0);
#endif
}

void reinitializeAfterTournament(void)
{
	addObject(0xfa2, 0, tickGameClock, renderGameClock);
	addObject(0xfa0, 0, NULL, renderMap);
#if VERSION_IS(JP)
	addObject(0xfa6, 0, tickConditions, NULL);
#endif
	addObject(0xfa8, 0, NULL, renderPoop);
	tamerSetState(0);
	partnerSetState(1);
}

void loadTrainingLibrary(int32_t mapId)
{
	if ((mapId == 0x70) || (mapId == 0x4E) || (mapId == 0x77)) {
		loadDynamicLibrary(10, (uint8_t *)&TRN_LOADING_COMPLETE,
				   1, 0, 0);
	} else if ((mapId == 0x6b) ||
		   (mapId == 0x6c) ||
		   (mapId == 0xa5) ||
		   (mapId == 0x63)) {
		loadDynamicLibrary(13, (uint8_t *)&TRN_LOADING_COMPLETE,
				   1, 0, 0);
	}
	TRAINING_COMPLETE = 0;
}

void createMeramonShake(void)
{
	MERAMON_SHAKE_FRAME_COUNT = 0;
	MERAMON_SHAKE_DATA = DRAWING_OFFSET_X;
	MERAMON_SHAKE_BACKUP_OFFSET_Y = DRAWING_OFFSET_Y;
	MERAMON_SHAKE_POS_X = MERAMON_SHAKE_POS_Y = MERAMON_SHAKE_WIDTH =
		MERAMON_SHAKE_HEIGHT = 0;
	addObject(0xfb8, 0, tickMeramonShake, 0);
}

void tickMeramonShake(int32_t arg0)
{
	POLY_FT4 *prim;
	int8_t offset;
	GsOT_TAG *org;

	if (MERAMON_SHAKE_FRAME_COUNT < 0x1f) {
		offset = 1;
	} else if (MERAMON_SHAKE_FRAME_COUNT < 0x3d) {
		offset = 2;
	} else if (MERAMON_SHAKE_FRAME_COUNT < 0x79) {
		offset = 3;
	} else if (MERAMON_SHAKE_FRAME_COUNT < 0xb5) {
		offset = 2;
	} else {
		offset = 1;
	}

	if ((MERAMON_SHAKE_FRAME_COUNT % 2) == 0) {
		offset = -offset;
		MERAMON_SHAKE_POS_X -= MERAMON_SHAKE_OFFSET;
		MERAMON_SHAKE_POS_Y -= MERAMON_SHAKE_OFFSET;

		if (MERAMON_SHAKE_POS_X < -200) {
			MERAMON_SHAKE_POS_X = -200;
		}

		if (MERAMON_SHAKE_POS_Y < -200) {
			MERAMON_SHAKE_POS_Y = -200;
		}

		MERAMON_SHAKE_WIDTH = -MERAMON_SHAKE_POS_X * 2;
		MERAMON_SHAKE_HEIGHT = -MERAMON_SHAKE_POS_Y * 2;
	}

	DRAWING_OFFSET_X += offset;

	prim = (POLY_FT4 *)GsGetWorkBase();

	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->tpage = getTPage(1, 2, 832, 256);
	prim->clut = GetClut(0, 0x1e7);
	setUVDataPolyFT4(prim, 64, 128, 63, 63);
	setPosDataPolyFT4(prim, MERAMON_SHAKE_POS_X, MERAMON_SHAKE_POS_Y,
			  MERAMON_SHAKE_WIDTH, MERAMON_SHAKE_HEIGHT);
	setRGB0(prim, MERAMON_SHAKE_RED, MERAMON_SHAKE_GREEN, MERAMON_SHAKE_BLUE);
	org = ACTIVE_ORDERING_TABLE->org;
	AddPrim(&org[9], prim);
	prim++;

	GsSetWorkBase((PACKET *)prim);

	MERAMON_SHAKE_FRAME_COUNT++;

	if (MERAMON_SHAKE_FRAME_COUNT >= 0xd2) {
		DRAWING_OFFSET_X = MERAMON_SHAKE_DATA;
		DRAWING_OFFSET_Y = MERAMON_SHAKE_BACKUP_OFFSET_Y;
		removeObject(0xfb8, 0);
	}
}

void getRViewCopy(GsRVIEW2 *out)
{
	*out = GS_VIEWPOINT_COPY;
}

void getViewportDistanceCopy(int32_t *out)
{
	*out = VIEWPORT_DISTANCE_COPY;
}

void getDrawingOffsetCopy(int32_t *x, int32_t *y)
{
	*x = DRAWING_OFFSET_X_COPY;
	*y = DRAWING_OFFSET_Y_COPY;
}

void cleanupGame(void)
{
	int32_t partnerType;
	int32_t i;

	removeObject(0xfa0, 0);
	removeObject(0xfa8, 0);
	removeObject(0xfa6, 0);
	removeObject(0xfb9, 0);
	removeObject(0xfa2, 0);
	removeObject(0xfb5, 0);

	partnerType = PARTNER_ENTITY.digimonEntity.entity.type;

	for (i = 0; i < ENTITY_MAX; i++) {
		if (ENTITY_TABLE[i] != NULL) {
			removeEntity(ENTITY_TABLE[i]->type, i);
		}
	}

	thunkUnloadModel(0, 2);
	thunkUnloadModel(partnerType, 3);

	for (i = 0; i < 8; i++) {
		if (LOADED_DIGIMON_MODELS[i] != -1L) {
			thunkUnloadModel(LOADED_DIGIMON_MODELS[i], 0);
		}
	}

	initializeLoadedNPCModels();
}

int32_t getFileCityTopMap(void)
{
	if ((isTriggerSet(0xdc) != 0) && (isTriggerSet(0xD6) != 0)) {
		if (isTriggerSet(0xdd) != 0) {
			if (isTriggerSet(0xe1) != 0) {
				return 0xb3;
			}

			if (isTriggerSet(0xf6) != 0) {
				return 0xb2;
			}

			return 0xb1;
		}

		if (isTriggerSet(0xe1) != 0) {
			return 0xb0;
		}

		if (isTriggerSet(0xf6) != 0) {
			return 0xaf;
		}

		return 0xae;
	}

	if (isTriggerSet(0xdd) != 0) {
		if (isTriggerSet(0xe1) != 0) {
			return 0xad;
		}

		if (isTriggerSet(0xf6) != 0) {
			return 0xac;
		}

		return 0xab;
	}

	if (isTriggerSet(0xe1) != 0) {
		return 0xaa;
	}

	if (isTriggerSet(0xf6) != 0) {
		return 0xa9;
	}

	return 0xa8;
}

int32_t lerp(int32_t start, int32_t end, int32_t t0, int32_t t1, int32_t t)
{
	int32_t result;
	int32_t range;
	int32_t duration;

	if (t1 == t0) {
		return 0;
	}

	range = end - start;
	duration = t1 - t0;
	result = (range * (t - t0)) / duration;
	if (range >= 0) {
		result = result % (range + 1);
		if (result < 0) {
			result += range;
		}
	} else {
		result = result % (range - 1);
		if (result > 0) {
			result += range;
		}
	}

	result += start;
	return result;
}

void translateConditionFXToEntity(Entity *entity, SVECTOR *out)
{
	SVECTOR *offset;
	MATRIX *matrix;

	offset = &CONDITION_FX_OFFSETS[getOriginalType(entity->type)];
	matrix = &entity->posData[offset->pad].posMatrix.workm;
	ApplyMatrixSV(matrix, offset, out);
	out->vx += (int16_t)matrix->t[0];
	out->vy += (int16_t)matrix->t[1];
	out->vz += (int16_t)matrix->t[2];
}

int32_t getOriginalType(int32_t type)
{
	int32_t originalType;

	if ((type < 0) || (type > 0xb0)) {
		return -1;
	}

	originalType = ORIGINAL_DIGIMON_TYPES[type];
	if (originalType < 0) {
		return type;
	}

	return originalType;
}

void calculateBoneMatrix(Entity *entity, int32_t boneId, MATRIX *out)
{
	GsCOORDINATE2 *coord;
	GsCOORDINATE2 *bone;

	if (boneId >= DIGIMON_DATA[entity->type].boneCount) {
		boneId = 0;
	}

	coord = &entity->posData->posMatrix;
	bone = &entity->posData[boneId].posMatrix;
	RotMatrix(&entity->posData->rotation, &coord->coord);
	ScaleMatrix(&coord->coord, &entity->posData->scale);
	TransMatrix(&coord->coord, &entity->posData->location);
	calculatePosition(bone, out);
}

void downloadCLUT1(uint32_t *buffer)
{
	RECT rect;

	setRECT(&rect, 0, 480, 256, 7);
	StoreImage(&rect, (u_long *)buffer);
	DrawSync(0);
}

void fadeoutCLUT1(long level, int16_t *clut)
{
	int16_t buffer[1792];
	RECT rect;
	int16_t *src;
	int16_t *dst;
	int32_t i;
	int16_t red;
	int16_t green;
	int16_t blue;
	int16_t mask;

	src = clut;
	dst = buffer;
	for (i = 0; i < 256; i++) {
		red = *src & 0x1f;
		green = (*src >> 5) & 0x1f;
		blue = (*src >> 10) & 0x1f;
		mask = (*src++ >> 15) & 0x1;
		*dst = red * (255 - level) / 255;
		*dst += (green * (255 - level) / 255) << 5;
		*dst += (blue * (255 - level) / 255) << 10;
		*dst++ += mask << 15;
	}

	src += 768;
	dst += 768;
	for (i = 0; i < 768; i++) {
		red = *src & 0x1f;
		green = (*src >> 5) & 0x1f;
		blue = (*src >> 10) & 0x1f;
		mask = (*src++ >> 15) & 0x1;
		*dst = red * (255 - level) / 255;
		*dst += (green * (255 - level) / 255) << 5;
		*dst += (blue * (255 - level) / 255) << 10;
		*dst++ += mask << 15;
	}

	setRECT(&rect, 0, 480, 256, 7);
	LoadImage(&rect, (u_long *)buffer);
	DrawSync(0);
}

void downloadCLUT2(u_long *buffer)
{
	RECT rect;

	setRECT(&rect, 272, 480, 16, 1);
	StoreImage(&rect, buffer);
	setRECT(&rect, 96, 501, 16, 1);
	StoreImage(&rect, buffer + 8);
	setRECT(&rect, 224, 488, 16, 24);
	StoreImage(&rect, buffer + 16);
	DrawSync(0);
}

void fadeoutCLUT2(long fade, int16_t *clut)
{
	int16_t pixels[416];
	RECT rect;
	int16_t *src;
	int16_t *dst;
	int32_t i;
	int16_t red;
	int16_t green;
	int16_t blue;
	int16_t mask;

	src = clut;
	dst = pixels;
	for (i = 0; i < 416; i++) {
		red = *src & 0x1f;
		green = (*src >> 5) & 0x1f;
		blue = (*src >> 10) & 0x1f;
		mask = (*src++ >> 15) & 0x1;
		*dst = red * (255 - fade) / 255;
		*dst += (green * (255 - fade) / 255) << 5;
		*dst += (blue * (255 - fade) / 255) << 10;
		*dst++ += mask << 15;
	}

	setRECT(&rect, 272, 480, 16, 1);
	LoadImage(&rect, (u_long *)&pixels[0]);

	setRECT(&rect, 96, 501, 16, 1);
	LoadImage(&rect, (u_long *)&pixels[16]);

	setRECT(&rect, 224, 488, 16, 24);
	LoadImage(&rect, (u_long *)&pixels[32]);
	DrawSync(0);
}

int32_t addPolyFT3Prim(POLY_FT3 *prim, int32_t order)
{
	if ((order >= 33) && (order < 0x1000)) {
		AddPrim(&ACTIVE_ORDERING_TABLE->org[order], prim);
		prim++;
		GsSetWorkBase((PACKET *)prim);
	}

	return order;
}

void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2)
{
	int32_t otz;
	long p;
	long flag;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	otz = RotTransPers3(v0, v1, v2, (long *)((char *)prim + 8),
			    (long *)((char *)prim + 0x10),
			    (long *)((char *)prim + 0x18),
			    &p, &flag);
	otz = otz >> 2;
	addPolyFT3Prim(prim, otz);
}

int32_t addScreenPolyFT4(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2,
			SVECTOR *v3)
{
	int32_t otz;
	long p;
	long flag;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	otz = RotTransPers4(v0, v1, v2, v3,
			    (long *)&poly->x0, (long *)&poly->x1,
			    (long *)&poly->x2, (long *)&poly->x3,
			    &p, &flag);
	otz = otz >> 2;
	if ((otz > 0x20) && (otz < 0x1000)) {
		AddPrim(ACTIVE_ORDERING_TABLE->org + otz, poly);
		poly++;
		GsSetWorkBase((PACKET *)poly);
	}
}

int32_t worldPosToScreenPos(SVECTOR *pos, DVECTOR *out)
{
	int32_t otz;

	GsSetLsMatrix(&GsWSMATRIX);
	gte_ldv0(pos);
	gte_rtps();
	gte_stsxy(out);
	gte_stszotz(&otz);
	otz = otz << 2;

	return otz;
}

void renderSprite(GsSPRITE *sprite, int16_t x, int16_t y, int32_t distance,
		  int32_t width, int32_t height)
{
	sprite->x = x;
	sprite->y = y;
	sprite->scalex = ((uint32_t)(width * VIEWPORT_DISTANCE) /
			  (uint32_t)distance);
	sprite->scaley = ((uint32_t)(height * VIEWPORT_DISTANCE) /
			  (uint32_t)distance);
#if VERSION_EQUAL_OR_OLDER(JP_TRIAL)
	GsSortSprite(sprite, ACTIVE_ORDERING_TABLE, distance >> 4);
#else
	distance = distance >> 4;
	if ((distance >= 0) && (distance < 0x1000)) {
		GsSortSprite(sprite, ACTIVE_ORDERING_TABLE, distance);
	}
#endif
}

void addFXPrim(POLY_FT4 *prim, int16_t posX, int16_t posY, int16_t width,
	       int16_t height, int32_t distance)
{
	int32_t scaledWidth;
	int32_t scaledHeight;

	scaledWidth = ((uint32_t)(width * VIEWPORT_DISTANCE) /
		       (uint32_t)distance);
	posX -= scaledWidth >> 1;
	scaledHeight = ((uint32_t)(height * VIEWPORT_DISTANCE) /
			(uint32_t)distance);
	posY -= scaledHeight >> 1;
	prim->x0 = posX;
	prim->y0 = posY;
	prim->x1 = posX + scaledWidth;
	prim->y1 = posY;
	prim->x2 = posX;
	prim->y2 = posY + scaledHeight;
	prim->x3 = posX + scaledWidth;
	prim->y3 = posY + scaledHeight;
	distance >>= 4;
	distance -= 0x37;

	if ((distance > 0x20) && (distance < 0x1000)) {
		AddPrim(ACTIVE_ORDERING_TABLE->org + distance, prim);
		prim++;
		GsSetWorkBase((PACKET *)prim);
	}
}

void setInt16WithStride(int16_t *dest, int16_t value, int16_t count,
			int16_t stride)
{
	int16_t i;

	for (i = 0; i < count; i++) {
		*dest = value;
		dest = (int16_t *)((int32_t)dest + stride);
	}
}

// clang-format off
void renderTMDModel(buffer, id, coord, super, trans, rot, scale)
	uint8_t *buffer;
	int16_t id;
	GsCOORDINATE2 *coord;
	GsCOORDINATE2 *super;
	VECTOR *trans;
	SVECTOR *rot;
	VECTOR *scale;
// clang-format on
{
	GsDOBJ2 obj;
	MATRIX m;

	GsLinkObject4((unsigned long)&buffer[12], &obj, id);
	GsInitCoordinate2(super, coord);

	obj.attribute = 0;
	obj.coord2 = coord;

	RotMatrix(rot, &coord->coord);
	ScaleMatrix(&coord->coord, scale);
	TransMatrix(&coord->coord, trans);
	coord->flg = 0;

	GsGetLw(obj.coord2, &m);
	GsSetLightMatrix(&m);

	GsGetLs(obj.coord2, &m);
	GsSetLsMatrix(&m);

	GsSortObject4(&obj, ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0));
}

void initializeSomeArenaArrays(int32_t count, int32_t arg1, int32_t arg2,
			int32_t *out)
{
	int32_t i;

	for (i = 0; i < count; i++) {
		out[i] = 0;
	}
}

int32_t processSomeArenaArrays(int32_t count, int32_t t, int32_t *keys,
			   int32_t *values, int32_t *slopes)
{
	uint32_t off;
	int32_t *key;
	int32_t *value;
	int32_t hi;
	int32_t mid;
	int32_t x0;
	int32_t dt;
	int32_t dx;
	int32_t *slope;
	int32_t lo;
	int32_t m0;
	int32_t m1;

	lo = 0;
	hi = count - 1;
	while (lo < hi) {
		mid = (lo + hi) / 2;
		if (keys[mid] < t) {
			lo = mid + 1;
		} else {
			hi = mid;
		}
	}

	if (lo > 0) {
		lo--;
	}

#if !VERSION_IS(US)
	dx = keys[lo + 1] - keys[lo];
	dt = t - keys[lo];

	return values[lo] +
	       dt * (((values[lo + 1] - values[lo]) / dx -
		      dx * (slopes[lo + 1] + slopes[lo] * 2)) +
		     dt * (slopes[lo] * 3 +
			   (dt * (slopes[lo + 1] - slopes[lo])) / dx));
#else
	off = lo * 4;
	key = (int32_t *)(off + (uint32_t)keys);
	x0 = key[0];
	dx = key[1] - key[0];
	dt = t - x0;
	value = (int32_t *)((uint32_t)values + off);
	slope = (int32_t *)((uint32_t)slopes + off);
	lo = value[0];
	m0 = slope[0];
	m1 = slope[1];

	return lo + dt * (((value[1] - lo) / dx - dx * (m1 + m0 * 2)) +
			  dt * (m0 * 3 + (dt * (m1 - m0)) / dx));
#endif
}

int32_t doSomethingWithSomePoints(int16_t *rect, DVECTOR *line)
{
	int16_t *c;
	DVECTOR *p;
	int16_t code[2];
	int32_t i;
	int32_t dx;
	int32_t dy;
	int32_t cx[4];
	int32_t cy[4];
	int32_t cross;

	for (i = 0; i < 2; i++) {
		p = &line[i];
		c = &code[i];

		if (rect[2] < p->vx) {
			*c = 5;
		} else if (p->vx < rect[0]) {
			*c = 3;
		} else {
			*c = 4;
		}

		if (rect[3] < p->vy) {
			*c += 3;
		} else if (p->vy < rect[1]) {
			*c -= 3;
		}

		if (*c == 4) {
			return -1;
		}
	}

	if ((code[0] + code[1]) == 8) {
		return -1;
	}

	if ((code[0] / 3) == (code[1] / 3)) {
		return 0;
	}

	if ((code[0] % 3) == (code[1] % 3)) {
		return 0;
	}

	dx = line[1].vx - line[0].vx;
	dy = line[1].vy - line[0].vy;

	cx[0] = rect[0] - line[0].vx;
	cy[0] = rect[1] - line[0].vy;
	cx[1] = rect[2] - line[0].vx;
	cy[1] = rect[1] - line[0].vy;
	cx[2] = rect[0] - line[0].vx;
	cy[2] = rect[3] - line[0].vy;
	cx[3] = rect[2] - line[0].vx;
	cy[3] = rect[3] - line[0].vy;

	cross = (dx * cy[0]) - (dy * cx[0]);

	for (i = 1; i < 4; i++) {
		if ((cross * ((dx * cy[i]) - (dy * cx[i]))) < 0) {
			return -1;
		}
	}

	return 0;
}

int16_t DOOA_storeDigimonY(void)
{
	DOOA_STORED_DIGIMON_Y = ENTITY_TABLE[1]->posData->location.vy;

	return DOOA_STORED_DIGIMON_Y;
}

int16_t DOOA_getStoredDigimonY(void)
{
	return DOOA_STORED_DIGIMON_Y;
}

void renderFXParticle(SVECTOR *pos, int32_t size, RGB8 *color)
{
	POLY_FT4 *prim;
	int32_t depth;
	DVECTOR screenPos;

	prim = (POLY_FT4 *)GsGetWorkBase();
	depth = worldPosToScreenPos(pos, &screenPos);
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->code |= 2;
	setRGB0(prim, color->r, color->g, color->b);
	prim->tpage = getTPage(0, 1, 768, 256);
	prim->clut = getClut(192, 489);
	setUVWH(prim, 0, 160, 15, 15);
	addFXPrim(prim, screenPos.vx, screenPos.vy, size, size, depth);
}
