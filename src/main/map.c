#include <string.h>

#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <mwinline_n.h>

#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/fade.h>
#include <dw/file.h>
#include <dw/font.h>
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
#include <dw/world_object.h>

typedef struct {
	uint8_t *imagePtr;
	int16_t tileId;
	int16_t posX;
	int16_t posY;
	int16_t texU;
	int16_t texV;
	int16_t tpage;
	int16_t clut;
} MapTileData;

typedef struct {
	int16_t cameraX;
	int16_t cameraY;
	int8_t width;
	int8_t height;
	int8_t pad[26];
} MapState;

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

long RotTransPers3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, long *sxy0, long *sxy1, long *sxy2, long *p, long *flag);
int abs(int x);
void calcMapObjectOrder(LocalMapObjectInstance *instances);
void calculatePosition(GsCOORDINATE2 *coord, MATRIX *matrix);
void checkArenaMap(uint8_t mapId);
void checkCurlingMap(int32_t mapId);
void checkFishingMap(int32_t mapId, int32_t arg1);
int32_t checkMapCollisionX(Entity *entity, int32_t direction);
int32_t checkMapCollisionY(Entity *entity, int32_t direction);
void checkShopMap(uint8_t mapId);
void clearMapDigimon(void);
void clearMapObjects(uint8_t *instances);
void entityLookAtTile(Entity *entity, int32_t tileX, int32_t tileY);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void handleBattleIdle(DigimonEntity *entity, Stats *stats, int32_t flags);
void initializeLoadedNPCModels(void);
int32_t isInvisible(Entity *entity);
void loadDoors(int32_t doorEntryId);
void loadMapCollisionData(uint8_t *data);
void loadMapImage1(uint8_t *tim);
void loadMapImage2(uint8_t *tim, int8_t id);
void loadMapObjects(uint8_t *mapObjects, uint8_t *data, int32_t mapId);
int32_t loadMapSounds(int32_t mapSoundId);
void loadTrainingPoop(void);
void loadWarpCrystals(int32_t mapId);
int32_t readFile(char *path, void *dest);
void removeMapEntities(void);
void renderMapOverlays(int8_t *instances, int32_t screenX, int16_t screenY);
void renderPoop(int32_t instanceId);
void renderString(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, int32_t h, int32_t i);
void runMapHeadScript(int32_t section);
void setPosDataPolyFT4(POLY_FT4 *prim, int16_t posX, int16_t posY, int16_t width, int16_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uPos, int32_t vPos, int32_t width, int32_t height);
void startBattleIdleAnimation(DigimonEntity *entity, Stats *stats, int32_t flags);
void tickFileReadQueue(int32_t instanceId);
void unloadMapParts(void);

void MAIN_func_800D3174(void);
void MAIN_func_800D32E0(int8_t *fromX, int8_t *fromY, int8_t *toX, int8_t *toY);
void MAIN_func_800D39F8(void);
void MAIN_func_800D3A84(void);
void MAIN_func_800D3ADC(void);
void MAIN_func_800D56E0(void);
void MAIN_func_800D634C(void);
void MAIN_func_800D6A4C(MapTileData *tiles);
void MAIN_func_800D763C(DVECTOR *current, DVECTOR *previous);
void MAIN_func_800D8AC8(int32_t diffX, int32_t diffY);
int32_t MAIN_func_800D8E64(int16_t mapId, int16_t exitId, int32_t showName);
void MAIN_func_800D91EC(int16_t a, int16_t b);
int32_t MAIN_func_800D91FC(int32_t flag);
void MAIN_func_800D9248(void);
void MAIN_func_800D92EC(void);
void MAIN_func_800D9360(int32_t mapId);
void MAIN_func_800D9660(GsRVIEW2 *out);
void MAIN_func_800D9B60(uint32_t *src);
void MAIN_func_800D9BA8(int32_t level, int16_t *src);
void MAIN_func_800D9E68(u_long *buffer);
void MAIN_func_800D9F14(int32_t fade, int16_t *src);
int32_t MAIN_func_800DA0F4(POLY_FT3 *prim, int32_t order);
void addScreenPolyFT3(void *prim, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
void renderTMDModel(uint8_t *buffer, int32_t id, GsCOORDINATE2 *coord, GsCOORDINATE2 *super, VECTOR *trans, SVECTOR *rot, VECTOR *scale);
void MAIN_func_800DA60C(int32_t count, int32_t arg1, int32_t arg2, int32_t *out);
int32_t MAIN_func_800DA63C(int32_t count, int32_t t, int32_t *keys, int32_t *values, int32_t *slopes);
int32_t MAIN_func_800DA740(int16_t *rect, DVECTOR *line);
int16_t MAIN_func_800DA9C8(void);
int16_t MAIN_func_800DA9F4(void);
int32_t add3DSpritePrim(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
void addFXPrim(POLY_FT4 *prim, int32_t x, int32_t y, int16_t width, int16_t height, int32_t depth);
void addMapNameObject(int32_t mapId);
void addPartnerWaypoint(int16_t index, int8_t x, int8_t y);
void addTamerWaypoint(int16_t index, int8_t x, int8_t y);
void buildMapPath(char *out, MapEntry *entry, int8_t *suffix, int32_t mapId);
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
void fillTileData(MapTileData *tile, uint8_t *imagePtr, int32_t texU, int32_t texV, int16_t posX, int16_t posY);
void getDrawingOffsetCopy(int32_t *x, int32_t *y);
void getEntityTileFromModel(Entity *entity, int8_t *outTileX, int8_t *outTileY);
int32_t getFileCityTopMap(void);
int32_t getMapSoundId(int32_t mapId);
int32_t getOriginalType(int32_t type);
void getViewportDistanceCopy(int32_t *out);
void handleTileUpdate(int32_t input, int32_t force);
int32_t hasMovedOutsideCombatArea(DVECTOR *a, DVECTOR *b, int16_t w, int16_t h);
void initializeDaytimeTransition(int32_t timeOfDay);
int32_t isFiveTileWidePathOpen(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
int32_t isInDaytimeTransition(void);
int32_t isLinearPathBlocked(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
int32_t isOffScreen(DVECTOR *xy, int16_t w, int16_t h);
int32_t isRectInRect(RECT *rect, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
int32_t isTileWithinScreenArea(int16_t tileX, int16_t tileZ);
int32_t lerp(int32_t start, int32_t end, int32_t t0, int32_t t1, int32_t t);
void loadMap(int32_t mapId);
int32_t loadMapSetup(int32_t *data);
void moveCameraByDiff(VECTOR *from, VECTOR *to);
void readMapTFS(int32_t mapId);
void renderFXParticle(SVECTOR *pos, int32_t size, uint8_t *color);
void renderMap(int32_t arg0);
void renderMapName(int32_t instanceId);
void renderSprite(GsSPRITE *sprite, int16_t x, int16_t y, int32_t distance, int32_t width, int32_t height);
void setCameraFollowPlayer(void);
void setPosDataMapTile(MapTileData *tile, int32_t camX, int32_t camY, POLY_FT4 *prim);
void setShortWithStride(int16_t *dest, int16_t value, int32_t count, int32_t stride);
void storeEntityLocation(uint32_t scriptId, VECTOR *out);
void tickCameraFollowPlayer(void);
void tickCameraMovement(int16_t instanceId);
void tickDaytimeTransition(int32_t instanceId);
void tickMeramonShake(int32_t arg0);
void tickMoveCameraTo(int32_t x, int32_t z, int32_t arg2);
int32_t tickMoveCameraToEntity(uint32_t scriptId, int32_t speed);
void tickPartnerCollision(void);
void tickPartnerWaypoints(void);
void tickTamerWaypoints(void);
void translateConditionFXToEntity(Entity *entity, SVECTOR *out);
void unloadMap(void);
void unsetCameraFollowPlayer(void);
void updateTileColumn(int32_t arg0);
void updateTileRow(int32_t bottom);
void updateTimeOfDay(void);
void uploadMapTileImages(MapTileData *tiles, int16_t index);
int32_t worldPosToScreenPos(SVECTOR *pos, DVECTOR *out);

extern GsOT *ACTIVE_ORDERING_TABLE;
extern int32_t CAMERA_FOLLOW_PLAYER;
extern int8_t CAMERA_HAS_TARGET;
extern int8_t CAMERA_MOVE_DELTA_X;
extern int8_t CAMERA_MOVE_DELTA_Y;
extern int16_t CAMERA_MOVE_DIFF_X;
extern int16_t CAMERA_MOVE_DIFF_Y;
extern int16_t CAMERA_MOVE_DRAW_OFFSET_X;
extern int16_t CAMERA_MOVE_DRAW_OFFSET_Y;
extern int16_t CAMERA_MOVE_FINAL_X;
extern int16_t CAMERA_MOVE_FINAL_Y;
extern int8_t CAMERA_REACHED_TARGET;
extern VECTOR CAMERA_TARGET;
extern int32_t CAMERA_UPDATE_TILES;
extern int16_t CAMERA_X[];
extern int16_t CAMERA_X_PREVIOUS;
extern int16_t CAMERA_Y[];
extern int16_t CAMERA_Y_PREVIOUS;
extern int32_t COMBAT_AREA_CENTER_X;
extern int32_t COMBAT_AREA_CENTER_Y;
extern uint8_t CURRENT_EXIT;
extern uint16_t CURRENT_FRAME;
extern uint8_t CURRENT_SCREEN;
extern int8_t CURRENT_TIME_OF_DAY;
extern int8_t DAYTIME_TRANSITION_ACTIVE;
extern int8_t DAYTIME_TRANSITION_FRAME;
extern int16_t DAYTIME_TRANSITION_TARGET;
extern int32_t DRAWING_OFFSET_X;
extern int32_t DRAWING_OFFSET_X_COPY;
extern int32_t DRAWING_OFFSET_Y;
extern int32_t DRAWING_OFFSET_Y_COPY;
extern int16_t DRAW_OFFSET_LIMIT_X_MAX;
extern int16_t DRAW_OFFSET_LIMIT_X_MIN;
extern int16_t DRAW_OFFSET_LIMIT_Y_MAX;
extern int16_t DRAW_OFFSET_LIMIT_Y_MIN;
extern Entity *ENTITY_TABLE[];
extern int16_t FADE_OUT_CURRENT;
extern int8_t GAME_STATE;
extern void *GENERAL_BUFFER_PTR;
extern GsRVIEW2 GS_VIEWPOINT;
extern GsRVIEW2 GS_VIEWPOINT_COPY;
extern MATRIX GsWSMATRIX;
extern int16_t HOUR;
extern int32_t IS_SCRIPT_PAUSED;
extern GsF_LIGHT LIGHT_DATA[];
extern int32_t LOADED_DIGIMON_MODELS[];
extern int8_t LOCAL_MAP_OBJECT_INSTANCE[];
extern int16_t MAIN_D_80134DFC;
extern u_long *MAP_CLUTS[];
extern int8_t MAP_COLLISION_DATA[];
extern int8_t MAP_HEIGHT[];
extern GsF_LIGHT MAP_LIGHT[];
extern int8_t MAP_TILES[];
extern MapTileData MAP_TILE_DATA[];
extern int8_t MAP_TILE_X;
extern int8_t MAP_TILE_Y;
extern int8_t MAP_WIDTH[];
extern int32_t MERAMON_SHAKE_BACKUP_OFFSET_X;
extern int32_t MERAMON_SHAKE_BACKUP_OFFSET_Y;
extern uint8_t MERAMON_SHAKE_COLOR_R;
extern uint8_t MERAMON_SHAKE_FRAME_COUNT;
extern int16_t MERAMON_SHAKE_HEIGHT;
extern int16_t MERAMON_SHAKE_POS_X;
extern int16_t MERAMON_SHAKE_POS_Y;
extern int16_t MERAMON_SHAKE_WIDTH;
extern int8_t PARTNER_AREA_RESPONSE[];
extern int8_t PARTNER_STATE;
extern int8_t PARTNER_TAMER_PREVIOUS_TILE_X;
extern int8_t PARTNER_TAMER_PREVIOUS_TILE_Y;
extern int8_t PARTNER_WAYPOINT_COUNT;
extern int8_t PARTNER_WAYPOINT_CURRENT;
extern int8_t PARTNER_WAYPOINT_X[];
extern int8_t PARTNER_WAYPOINT_Y[];
extern int16_t PLAYER_OFFSET_X;
extern int16_t PLAYER_OFFSET_Y;
extern int32_t POLLED_INPUT;
extern int32_t POLLED_INPUT_PREVIOUS;
extern int8_t PREVIOUS_EXIT;
extern uint8_t PREVIOUS_SCREEN;
extern int8_t PREV_TILE_X;
extern int8_t PREV_TILE_Y;
extern int16_t SCRIPT_MAP_CHANGE_STATE;
extern int8_t SKIP_DAYTIME_TRANSITION;
extern int32_t SKIP_MAP_FILE_READ;
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
	MAIN_func_800DA9F4,
	MAIN_func_800DA9C8,
	MAIN_func_800DA740,
	MAIN_func_800DA63C,
	MAIN_func_800DA60C,
	renderTMDModel,
	setShortWithStride,
	addFXPrim,
	renderSprite,
	worldPosToScreenPos,
	add3DSpritePrim,
	addScreenPolyFT3,
	MAIN_func_800DA0F4,
	MAIN_func_800D9F14,
	MAIN_func_800D9E68,
	MAIN_func_800D9BA8,
	MAIN_func_800D9B60,
	calculateBoneMatrix,
	getOriginalType,
	translateConditionFXToEntity,
	lerp,
	getFileCityTopMap,
	cleanupGame,
	getDrawingOffsetCopy,
	getViewportDistanceCopy,
	MAIN_func_800D9660,
	tickMeramonShake,
	createMeramonShake,
	MAIN_func_800D9360,
	MAIN_func_800D92EC,
	renderMapName,
	MAIN_func_800D9248,
	MAIN_func_800D91FC,
	MAIN_func_800D91EC,
	changeMap,
	addMapNameObject,
	MAIN_func_800D8E64,
	updateTileColumn,
	updateTileRow,
	setCameraFollowPlayer,
	unsetCameraFollowPlayer,
	MAIN_func_800D8AC8,
	moveCameraByDiff,
	tickMoveCameraToEntity,
	tickMoveCameraTo,
	checkCameraMovement,
	storeEntityLocation,
	tickCameraMovement,
	createCameraMovement,
	cameraIsAtEdge,
	handleTileUpdate,
	MAIN_func_800D763C,
	isInDaytimeTransition,
	tickDaytimeTransition,
	setPosDataMapTile,
	initializeDaytimeTransition,
	tickCameraFollowPlayer,
	unloadMap,
	uploadMapTileImages,
	MAIN_func_800D6A4C,
	fillTileData,
	updateTimeOfDay,
	MAIN_func_800D634C,
	readMapTFS,
	getMapSoundId,
	loadMapSetup,
	buildMapPath,
	loadMap,
	renderMap,
	MAIN_func_800D56E0,
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
	addPartnerWaypoint,
	tickPartnerCollision,
	isLinearPathBlocked,
	isFiveTileWidePathOpen,
	getEntityTileFromModel,
	MAIN_func_800D3ADC,
	MAIN_func_800D3A84,
	MAIN_func_800D39F8,
	hasMovedOutsideCombatArea,
	isRectInRect,
	entityCheckCombatArea,
	MAIN_func_800D32E0,
	addTamerWaypoint,
	entityIsInEntity,
	MAIN_func_800D3174,
	isTileWithinScreenArea,
};
// clang-format off
int8_t COMBAT_AREA_CORNER_OFFSETS[8] = {
	0xff, 0x01, 0x01, 0x01, 0xff, 0xff, 0x01, 0xff,
};

int8_t MAP_FILE_EXT_MAP[] = ".MAP";

int8_t MAP_FILE_EXT_TFS[] = ".TFS";

// mwcc aligns every array to four, so the two-byte separator can only be a scalar
int16_t MAP_PATH_SEPARATOR = '\\';

int16_t MAIN_D_801343B0 = 0xffff;

int16_t MAIN_D_801343B2 = 0xffff;

int8_t MAIN_D_801343B4 = 0x01;

uint8_t MERAMON_SHAKE_COLOR_G = 0xff;

uint8_t MERAMON_SHAKE_COLOR_B = 0xff;

int16_t MERAMON_SHAKE_STEP = 0x0020;

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

char MAP_NAME_NATIVE_FOREST[] = "      Native Forest     ";

char MAP_NAME_COELA_POINT[] = "       Coela Point      ";

char MAP_NAME_DRAGON_EYE_LAKE[] = "     Dragon Eye Lake    ";

char MAP_NAME_DRILL_TUNNEL_ENTRANCE[] = "  Drill Tunnel Entrance ";

char MAP_NAME_DIGIMON_BRIDGE[] = "      Digimon Bridge    ";

char MAP_NAME_TROPICAL_JUNGLE[] = "     Tropical Jungle    ";

char MAP_NAME_MANGROVE_REGION[] = "     Mangrove Region    ";

char MAP_NAME_PATH_THRU_MT_PANORAMA[] = " Path Thru Mt. Panorama ";

char MAP_NAME_ENTRANCE_TO_FILE_CITY[] = "  Entrance to File City ";

char MAP_NAME_MT_PANORAMA_PLAINS[] = "  Mt. Panorama Plains   ";

char MAP_NAME_FOOT_OF_MT_PANORAMA[] = "  Foot of Mt. Panorama  ";

char MAP_NAME_MT_PANORAMA_SPORE_AREA[] = "Mt. Panorama Spore Area ";

char MAP_NAME_DRILL_TUNNEL[] = "      Drill Tunnel      ";

char MAP_NAME_DRILL_TUNNEL_2ND_FLOOR[] = " Drill Tunnel 2nd floor ";

char MAP_NAME_DRILL_TUNNEL_3RD_FLOOR[] = " Drill Tunnel 3rd floor ";

char MAP_NAME_RESIDENTIAL_AREA[] = "    Residential Area    ";

char MAP_NAME_UNDERGROUND_POND[] = "    Underground Pond    ";

char MAP_NAME_LAVA_CAVE[] = "        Lava Cave       ";

char MAP_NAME_OVERDELL[] = "        Overdell        ";

char MAP_NAME_OVERDELL_CEMETERY[] = "    Overdell Cemetery   ";

char MAP_NAME_GREAT_CANYON_ENTRANCE[] = "  Great Canyon Entrance ";

char MAP_NAME_GREAT_CANYON_TOP_AREA[] = "  Great Canyon Top Area ";

char MAP_NAME_GREAT_CANYON_BRIDGE[] = "   Great Canyon Bridge  ";

char MAP_NAME_FORTRESS_ENTRANCE[] = "    Fortress Entrance   ";

char MAP_NAME_GREAT_CANYON_BOT_AREA[] = " Great Canyon Bot. Area ";

char MAP_NAME_OGRE_FORTRESS[] = "      Ogre Fortress     ";

char MAP_NAME_MONOCHROME_SHOP[] = "     Monochrome Shop    ";

char MAP_NAME_GREY_LORDS_MANSION[] = "   Grey Lord's Mansion  ";

char MAP_NAME_MANSION_BASEMENT[] = "    Mansion Basement    ";

char MAP_NAME_UNDERGROUND_LAB[] = "     Underground Lab    ";

char MAP_NAME_GEAR_SAVANNA[] = "      Gear Savanna      ";

char MAP_NAME_ANCIENT_DINO_REGION[] = "   Ancient Dino Region  ";

char MAP_NAME_ANCIENT_GLACIAL_REGION[] = " Ancient Glacial Region ";

char MAP_NAME_ANCIENT_SPEEDY_REGION[] = " Ancient Speedy Region  ";

char MAP_NAME_FREEZELAND[] = "       Freezeland       ";

char MAP_NAME_ICE_SANCTUARY[] = "      Ice Sanctuary     ";

char MAP_NAME_GREEN_GYM[] = "        Green Gym       ";

char MAP_NAME_LEOMON_ANCESTORS_CAVE[] = " Leomon Ancestor's Cave ";

char MAP_NAME_MISTY_TREES[] = "       Misty Trees      ";

char MAP_NAME_GREAT_CANYON[] = "      Great Canyon      ";

char MAP_NAME_GEKO_SWAMP[] = "        Geko Swamp      ";

char MAP_NAME_VOLUME_VILLA[] = "      Volume Villa      ";

char MAP_NAME_FILE_CITY[] = "        File City       ";

char MAP_NAME_ITEM_KEEPER[] = "      Item Keeper       ";

char MAP_NAME_CENTAR_CLINIC[] = "      Centar Clinic     ";

char MAP_NAME_RESTAURANT[] = "       Restaurant       ";

char MAP_NAME_ITEM_SHOP[] = "        Item Shop       ";

char MAP_NAME_JIJIMONS_HOUSE[] = "      Jijimon's house   ";

char MAP_NAME_SECRET_ITEM_SHOP[] = "    Secret Item Shop    ";

char MAP_NAME_TOY_TOWN[] = "        Toy Town        ";

char MAP_NAME_SECRET_BEACH_CAVE[] = "    Secret Beach Cave   ";

char MAP_NAME_FACTORIAL_TOWN[] = "     Factorial Town     ";

char MAP_NAME_BIRDRA_TRANSPORT[] = "     Birdra Transport   ";

char MAP_NAME_ARENA_LOBBY[] = "       Arena Lobby      ";

char MAP_NAME_TREASURE_HUNT[] = "      Treasure Hunt     ";

char MAP_NAME_TRASH_MOUNTAIN[] = "     Trash Mountain     ";

char MAP_NAME_SEWER[] = "          Sewer         ";

char MAP_NAME_BEETLE_LAND[] = "      Beetle Land       ";

char MAP_NAME_MT_INFINITY[] = "      Mt. Infinity      ";

char MAP_NAME_DIGIMON_CURLING[] = "     Digimon Curling    ";

char MAP_NAME_TOY_MANSION[] = "       Toy Mansion      ";

char MAP_NAME_COSTUME_HOUSE[] = "      Costume House     ";

char MAP_NAME_ROBOT_HOUSE[] = "       Robot House      ";

char MAP_NAME_MANSION_2ND_FLOOR[] = "    Mansion 2nd floor   ";

char MAP_NAME_MANSION_ATTIC[] = "      Mansion Attic     ";

char MAP_NAME_BACK_DIMENSION[] = "     Back Dimension     ";

char MAP_NAME_KUNEMONS_BED[] = "      Kunemon's Bed     ";

char MAP_NAME_AMIDA_FOREST[] = "      Amida Forest      ";

char MAP_PATH_PREFIX[] = "\\MAP\\MAP";

char MAP_PATH_DIGITS[] = "0123456789";

char *MAP_NAME_PTR[70] = {
	MAP_NAME_NATIVE_FOREST,
	MAP_NAME_COELA_POINT,
	MAP_NAME_DRAGON_EYE_LAKE,
	MAP_NAME_DRILL_TUNNEL_ENTRANCE,
	MAP_NAME_DIGIMON_BRIDGE,
	MAP_NAME_TROPICAL_JUNGLE,
	MAP_NAME_MANGROVE_REGION,
	MAP_NAME_PATH_THRU_MT_PANORAMA,
	MAP_NAME_ENTRANCE_TO_FILE_CITY,
	MAP_NAME_MT_PANORAMA_PLAINS,
	MAP_NAME_FOOT_OF_MT_PANORAMA,
	MAP_NAME_MT_PANORAMA_SPORE_AREA,
	MAP_NAME_DRILL_TUNNEL,
	MAP_NAME_DRILL_TUNNEL_2ND_FLOOR,
	MAP_NAME_DRILL_TUNNEL_3RD_FLOOR,
	MAP_NAME_RESIDENTIAL_AREA,
	MAP_NAME_UNDERGROUND_POND,
	MAP_NAME_LAVA_CAVE,
	MAP_NAME_OVERDELL,
	MAP_NAME_OVERDELL_CEMETERY,
	MAP_NAME_GREAT_CANYON_ENTRANCE,
	MAP_NAME_GREAT_CANYON_TOP_AREA,
	MAP_NAME_GREAT_CANYON_BRIDGE,
	MAP_NAME_FORTRESS_ENTRANCE,
	MAP_NAME_GREAT_CANYON_BOT_AREA,
	MAP_NAME_OGRE_FORTRESS,
	MAP_NAME_MONOCHROME_SHOP,
	MAP_NAME_GREY_LORDS_MANSION,
	MAP_NAME_MANSION_BASEMENT,
	MAP_NAME_UNDERGROUND_LAB,
	MAP_NAME_GEAR_SAVANNA,
	MAP_NAME_ANCIENT_DINO_REGION,
	MAP_NAME_ANCIENT_GLACIAL_REGION,
	MAP_NAME_ANCIENT_SPEEDY_REGION,
	MAP_NAME_FREEZELAND,
	MAP_NAME_ICE_SANCTUARY,
	MAP_NAME_GREEN_GYM,
	MAP_NAME_LEOMON_ANCESTORS_CAVE,
	MAP_NAME_MISTY_TREES,
	MAP_NAME_GREAT_CANYON,
	MAP_NAME_FREEZELAND,
	MAP_NAME_GEKO_SWAMP,
	MAP_NAME_VOLUME_VILLA,
	MAP_NAME_FILE_CITY,
	MAP_NAME_ITEM_KEEPER,
	MAP_NAME_CENTAR_CLINIC,
	MAP_NAME_RESTAURANT,
	MAP_NAME_ITEM_SHOP,
	MAP_NAME_JIJIMONS_HOUSE,
	MAP_NAME_SECRET_ITEM_SHOP,
	MAP_NAME_TOY_TOWN,
	MAP_NAME_SECRET_BEACH_CAVE,
	MAP_NAME_FACTORIAL_TOWN,
	MAP_NAME_BIRDRA_TRANSPORT,
	MAP_NAME_ARENA_LOBBY,
	MAP_NAME_TREASURE_HUNT,
	MAP_NAME_TRASH_MOUNTAIN,
	MAP_NAME_SEWER,
	MAP_NAME_BEETLE_LAND,
	MAP_NAME_MT_INFINITY,
	MAP_NAME_DIGIMON_CURLING,
	MAP_NAME_TOY_MANSION,
	MAP_NAME_COSTUME_HOUSE,
	MAP_NAME_ROBOT_HOUSE,
	MAP_NAME_MANSION_2ND_FLOOR,
	MAP_NAME_MANSION_ATTIC,
	MAP_NAME_MANSION_BASEMENT,
	MAP_NAME_BACK_DIMENSION,
	MAP_NAME_KUNEMONS_BED,
	MAP_NAME_AMIDA_FOREST,
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

int16_t ORIGINAL_TYPE[180] = {
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

void MAIN_func_800D39F8(void)
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

void MAIN_func_800D3174(void)
{
	PARTNER_WAYPOINT_COUNT--;
	PARTNER_WAYPOINT_CURRENT++;
	PARTNER_WAYPOINT_CURRENT = PARTNER_WAYPOINT_CURRENT % 30;
}

void addTamerWaypoint(int16_t index, int8_t x, int8_t y)
{
	TAMER_WAYPOINT_X[index] = x;
	TAMER_WAYPOINT_Y[index] = y;
	TAMER_WAYPOINT_COUNT++;
}

void MAIN_func_800D32E0(int8_t *outX, int8_t *outY, int8_t *targetX,
			int8_t *targetY)
{
	int16_t dx;
	int16_t dy;
	int8_t x;
	int8_t y;
	int16_t dist;
	int16_t acc;
	int16_t rem;
	int16_t quot;
	int16_t sign;
	int32_t i;
	int8_t step;

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
					step = sign;
					y += step;
					acc += dist;
				}
			} else {
				step = quot;
				y += step;
			}
			if (dx > 0) {
				x = x + 1;
			} else {
				x = x - 1;
			}
			if (isTileWithinScreenArea(x, y) != 0) {
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
					step = sign;
					x += step;
					acc += dist;
				}
			} else {
				step = quot;
				x += step;
			}
			if (dy > 0) {
				y = y + 1;
			} else {
				y = y - 1;
			}
			if (isTileWithinScreenArea(x, y) != 0) {
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
	DVECTOR screen2;
	DVECTOR screen1;
	SVECTOR corner;
	VECTOR *loc;
	int32_t radius;
	int32_t base;
	int32_t i;
	int32_t j;

	if (GAME_STATE == 4) {
		return 0;
	}

	GsSetLsMatrix(&GsWSMATRIX);

	loc = &entity->posData->location;
	base = DIGIMON_DATA[entity->type].radius;

	for (i = 0; i < 2; i++) {
		radius = base;
		base = radius;

		for (j = 0; j < 4; j++) {
			corner.vx = loc->vx + radius * COMBAT_AREA_CORNER_OFFSETS[i * 2];
			corner.vy = loc->vy + (i * -200);
			corner.vz = loc->vz + radius * (&COMBAT_AREA_CORNER_OFFSETS[1])[i * 2];

			gte_ldv0(&corner);
			gte_rtps();
			gte_stsxy((long *)&screen1);

			corner.vx = target->vx + radius * COMBAT_AREA_CORNER_OFFSETS[i * 2];
			corner.vy = target->vy + (i * -200);
			corner.vz = target->vz + radius * (&COMBAT_AREA_CORNER_OFFSETS[1])[i * 2];

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
	if (((currentv->vx - COMBAT_AREA_CENTER_X) < (-width / 2)) &&
	    (currentv->vx < previousv->vx)) {
		return 1;
	}
	if (((width / 2) < (currentv->vx - COMBAT_AREA_CENTER_X)) &&
	    (previousv->vx < currentv->vx)) {
		return 1;
	}
	if (((currentv->vy - COMBAT_AREA_CENTER_Y) < (-height / 2)) &&
	    (currentv->vy < previousv->vy)) {
		return 1;
	}
	if (((height / 2) < (currentv->vy - COMBAT_AREA_CENTER_Y)) &&
	    (previousv->vy < currentv->vy)) {
		return 1;
	}
	return 0;
}

void MAIN_func_800D3A84(void)
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

void MAIN_func_800D3ADC(void)
{
	TAMER_WAYPOINT_CURRENT = 0;
	TAMER_WAYPOINT_COUNT = 0;
	TAMER_WAYPOINT_ACTIVE = 0;
}

int32_t isFiveTileWidePathOpen(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	int32_t i;
	int32_t start;
	int32_t end;

	if (abs(x2 - x1) >= abs(y2 - y1)) {
		for (i = -2, end = y2 - 2, start = y1 - 2; i < 3;
		     i++, start++, end++) {
			if (isLinearPathBlocked(x1, (int8_t)start, x2, (int8_t)end) != 0) {
				break;
			}
		}
	} else {
		for (i = -2, end = x2 - 2, start = x1 - 2; i < 3;
		     i++, start++, end++) {
			if (isLinearPathBlocked((int8_t)start, y1, (int8_t)end, y2) != 0) {
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

int32_t isLinearPathBlocked(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	int16_t dx;
	int16_t dy;
	int8_t x;
	int8_t y;
	int16_t steps;
	int16_t err;
	int16_t stepMain;
	int16_t dir;
	int16_t rem;
	int32_t t;
	int32_t i;
	int16_t count;

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
		t = abs(dx);
		steps = t;
		err = t;
		stepMain = dy / steps;
		rem = dy % steps;

		if (rem < 0) {
			dir = -1;
		} else {
			dir = 1;
		}

		rem = abs(rem);
		count = steps;

		for (i = 0; i < count; i++) {
			if (rem != 0) {
				err -= rem;
				if (err <= 0) {
					y += (int8_t)dir;
					err += steps;
				}
			} else {
				y += (int8_t)stepMain;
			}

			if (dx > 0) {
				x = x + 1;
			} else {
				x = x - 1;
			}

			if ((((uint8_t *)MAP_COLLISION_DATA)[x + (y * 100)] & 0x80) != 0) {
				return 1;
			}
		}
	} else {
		t = abs(dy);
		steps = t;
		err = t;
		stepMain = dx / steps;
		rem = dx % steps;

		if (rem < 0) {
			dir = -1;
		} else {
			dir = 1;
		}

		rem = abs(rem);
		count = steps;

		for (i = 0; i < count; i++) {
			if (rem != 0) {
				err -= rem;
				if (err <= 0) {
					x += (int8_t)dir;
					err += steps;
				}
			} else {
				x += (int8_t)stepMain;
			}

			if (dy > 0) {
				y = y + 1;
			} else {
				y = y - 1;
			}

			if ((((uint8_t *)MAP_COLLISION_DATA)[x + (y * 100)] & 0x80) != 0) {
				return 1;
			}
		}
	}

	return 0;
}

void tickPartnerCollision(void)
{
	int32_t isFiveTileWidePathOpen(int8_t x1, int8_t y1, int8_t x2, int8_t y2);
	int32_t isTileWithinScreenArea(int8_t tileX, int8_t tileZ);
	int16_t modelTileX;
	int16_t modelTileY;
	int8_t tamerTileX;
	int8_t tamerTileY;
	int8_t partnerTileX;
	int8_t partnerTileY;
	int16_t last;
	int16_t index;
	int32_t i;
	int16_t rotationY;
	int16_t collision;
	int32_t posX;
	int32_t posZ;

	getEntityTileFromModel(ENTITY_TABLE[0], &tamerTileX, &tamerTileY);
	if ((tamerTileX != PARTNER_TAMER_PREVIOUS_TILE_X) ||
	    (tamerTileY != PARTNER_TAMER_PREVIOUS_TILE_Y)) {
		getEntityTileFromModel(ENTITY_TABLE[1], &partnerTileX, &partnerTileY);
		if (isFiveTileWidePathOpen(tamerTileX, tamerTileY, partnerTileX, partnerTileY) == 1) {
			if (PARTNER_WAYPOINT_COUNT != 0) {
				last = (PARTNER_WAYPOINT_CURRENT + PARTNER_WAYPOINT_COUNT - 1) % 30;
				if (isFiveTileWidePathOpen(tamerTileX, tamerTileY, PARTNER_WAYPOINT_X[last], PARTNER_WAYPOINT_Y[last]) == 1) {
					addPartnerWaypoint((int16_t)((PARTNER_WAYPOINT_CURRENT + PARTNER_WAYPOINT_COUNT) % 30),
							   PARTNER_TAMER_PREVIOUS_TILE_X,
							   PARTNER_TAMER_PREVIOUS_TILE_Y);
				}
			} else {
				MAIN_func_800D39F8();
				addPartnerWaypoint(PARTNER_WAYPOINT_CURRENT, PARTNER_TAMER_PREVIOUS_TILE_X,
						   PARTNER_TAMER_PREVIOUS_TILE_Y);
			}
		} else {
			MAIN_func_800D39F8();
		}
	}

	if (PARTNER_WAYPOINT_COUNT >= 2) {
		for (i = 0; i < 3; i++) {
			index = (PARTNER_WAYPOINT_CURRENT + PARTNER_WAYPOINT_COUNT - 2 - i) % 30;
			if (isFiveTileWidePathOpen(tamerTileX, tamerTileY, PARTNER_WAYPOINT_X[index], PARTNER_WAYPOINT_Y[index]) == 0) {
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
		if (isTileWithinScreenArea(PARTNER_WAYPOINT_X[PARTNER_WAYPOINT_CURRENT], PARTNER_WAYPOINT_Y[PARTNER_WAYPOINT_CURRENT]) != 0) {
			if (ENTITY_TABLE[1]->isOnScreen == 0) {
				posX = (PARTNER_WAYPOINT_X[PARTNER_WAYPOINT_CURRENT] - 50) * 100 + 50;
				posZ = (50 - PARTNER_WAYPOINT_Y[PARTNER_WAYPOINT_CURRENT]) * 100 - 50;
				ENTITY_TABLE[1]->posData->location.vx = posX;
				ENTITY_TABLE[1]->posData->location.vz = posZ;
				ENTITY_TABLE[1]->anim.locX = posX << 15;
				ENTITY_TABLE[1]->anim.locZ = posZ << 15;
				MAIN_func_800D3174();
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
				MAIN_func_800D3174();
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

void addPartnerWaypoint(int16_t index, int8_t tileX, int8_t tileY)
{
	PARTNER_WAYPOINT_X[index] = tileX;
	PARTNER_WAYPOINT_Y[index] = tileY;
	PARTNER_WAYPOINT_COUNT++;
}

void entityLookAtLocation(Entity *entity, VECTOR *target)
{
	PositionData *posData;
	VECTOR *loc;
	int32_t tmp;
	int32_t dx;
	int32_t dz;

	if (target != NULL) {
		posData = entity->posData;
		loc = &posData->location;
		tmp = target->vx;
		dx = tmp - loc->vx;
		tmp = target->vz;
		dz = tmp - loc->vz;
		posData->rotation.vy = _atan(dz, dx);
	}
}

int16_t entityCheckCollision(Entity *source, Entity *entity, int32_t arg2,
			     int32_t arg3)
{
	VECTOR *loc;
	Entity *other;
	int32_t dx;
	int32_t dz;
	int16_t direction;
	int32_t i;
	VECTOR saved;

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
	int16_t startRot;
	int16_t dir;
	int16_t clockwise;
	int16_t counter;
	int32_t i;
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

			*rotY = startRot;
			if ((int16_t)(clockwise - counter) < 0) {
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
	int32_t isLinearPathBlocked(int8_t x1, int8_t y1, int8_t x2, int8_t y2);
	int16_t index;
	int8_t tileX;
	int8_t tileY;

	getEntityTileFromModel(ENTITY_TABLE[0], &tileX, &tileY);

	if ((tileX != TAMER_PREVIOUS_TILE_X) || (tileY != TAMER_PREVIOUS_TILE_Y)) {
		if (isLinearPathBlocked(tileX, tileY, TAMER_START_TILE_X, TAMER_START_TILE_Y) != 0) {
			if (TAMER_WAYPOINT_COUNT != 0) {
				index = (TAMER_WAYPOINT_CURRENT + TAMER_WAYPOINT_COUNT - 1)
					% 30;
				if (isLinearPathBlocked(tileX, tileY, TAMER_WAYPOINT_X[index], TAMER_WAYPOINT_Y[index]) != 0) {
					index = (TAMER_WAYPOINT_CURRENT +
						 TAMER_WAYPOINT_COUNT) % 30;
					addTamerWaypoint(index,
							 TAMER_PREVIOUS_TILE_X,
							 TAMER_PREVIOUS_TILE_Y);
				}
			} else {
				addTamerWaypoint(0, TAMER_PREVIOUS_TILE_X,
						 TAMER_PREVIOUS_TILE_Y);
			}
		} else {
			MAIN_func_800D3ADC();
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

void getEntityTileFromModel(Entity *entity, int8_t *outTileX,
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
	int32_t isTileWithinScreenArea(int8_t tileX, int8_t tileZ);
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
	int32_t posX;
	int32_t posZ;

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
		if (isTileWithinScreenArea(PARTNER_WAYPOINT_X[index],
					   PARTNER_WAYPOINT_Y[index]) != 0) {
			toX = PARTNER_WAYPOINT_X[index];
			toY = PARTNER_WAYPOINT_Y[index];
			break;
		}
	}

	if (count != 0) {
		while (PARTNER_WAYPOINT_CURRENT != index) {
			MAIN_func_800D3174();
		}
		MAIN_func_800D3174();
	}

	getModelTile(&ENTITY_TABLE[1]->posData->location,
		     &partnerTileX, &partnerTileY);
	toX = partnerTileX;
	toY = partnerTileY;
	MAIN_func_800D32E0(&fromX, &fromY, &toX, &toY);

	if (fromX != -1) {
		posX = (int16_t)(((fromX - 50) * 100) + 50);
		posZ = (int16_t)(((50 - fromY) * 100) - 50);
		ENTITY_TABLE[1]->posData->location.vx = posX;
		ENTITY_TABLE[1]->posData->location.vz = posZ;
		ENTITY_TABLE[1]->anim.locX = posX << 15;
		ENTITY_TABLE[1]->anim.locZ = posZ << 15;
	}
}

int32_t isTileWithinScreenArea(int16_t tileX, int16_t tileZ)
{
	DVECTOR screen;
	SVECTOR world;

	world.vx = ((tileX - 50) * 100) + 50;
	world.vy = 0;
	world.vz = ((50 - tileZ) * 100) - 50;

	gte_ldv0(&world);
	gte_rtps();
	gte_stsxy((long *)&screen);

	screen.vx = screen.vx - (int16_t)(160 - DRAWING_OFFSET_X);
	screen.vy = screen.vy - (int16_t)(120 - DRAWING_OFFSET_Y);

	if ((screen.vx < -200) || (screen.vx > 200) || (screen.vy < -160) ||
	    (screen.vy > 160)) {
		return 1;
	}

	return 0;
}

void entityMoveForward(Entity *entity)
{
	VECTOR dir;
	VECTOR out;
	MomentumData momentum;
	MATRIX m;
	PositionData *posData;
	int16_t *subDelta;
	int16_t *delta;
	int32_t vz;
	EntityAnim *anim;

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

	if ((diffY < 0) && (checkMapCollisionY(entity, 0) != 0)) {
		return 1;
	}

	if ((diffY > 0) && (checkMapCollisionY(entity, 1) != 0)) {
		return 1;
	}

	return 0;
}

int32_t entityCheckEntityCollision(Entity *entity, Entity *other,
				   int32_t diffX, int32_t diffZ)
{
	RECT rect;
	int32_t radius;
	int32_t myRadius;
	int32_t left;
	int32_t right;
	int32_t bottom;
	int32_t top;
	int16_t rotation;
	VECTOR *myLoc;
	VECTOR *loc;

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
		if (((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS & 0x40) != 0) &&
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
	int32_t rectBottom = rect->y - rect->h;
	int32_t rectRight = rect->x + rect->w;
	int32_t result;

	if (rectRight < left) {
		result = 0;
	} else if (right < rect->x) {
		result = 0;
	} else if (top < rectBottom) {
		result = 0;
	} else if (rect->y < bottom) {
		result = 0;
	} else {
		result = 1;
	}

	return result;
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
	DVECTOR screen;
	SVECTOR corner;
	VECTOR *loc;
	int16_t radius;
	int32_t i;
	int32_t j;
	int32_t index;

	if ((entity->type == 0x98) && (entity->anim.animId == 0x1C)) {
		return 0;
	}

	GsSetLsMatrix(&GsWSMATRIX);
	loc = &entity->posData->location;
	radius = DIGIMON_DATA[entity->type].radius;

	for (i = 0; i < 2; i++) {
		for (j = 0, index = 0; j < 4; j++, index += 2) {
			corner.vx = loc->vx + radius * COMBAT_AREA_CORNER_OFFSETS[index];
			corner.vy = loc->vy + i * -DIGIMON_DATA[entity->type].height;
			corner.vz = loc->vz + radius * (&COMBAT_AREA_CORNER_OFFSETS[1])[index];
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
	int32_t result;

	if ((pos->vx - (160 - DRAWING_OFFSET_X)) < (-width / 2)) {
		result = 1;
	} else if ((width / 2) < (pos->vx - (160 - DRAWING_OFFSET_X))) {
		result = 1;
	} else if ((pos->vy - (120 - DRAWING_OFFSET_Y)) < (-height / 2)) {
		result = 1;
	} else if ((height / 2) < (pos->vy - (120 - DRAWING_OFFSET_Y))) {
		result = 1;
	} else {
		result = 0;
	}

	return result;
}

void MAIN_func_800D56E0(void)
{
	int32_t i;

	for (i = 0; i < 35; i++) {
		MAP_TILES[i] = -1;
	}

	for (i = 0; i < 35; i++) {
		MAP_TILE_DATA[i].imagePtr = 0;
		MAP_TILE_DATA[i].posY = 0;
		MAP_TILE_DATA[i].posX = 0;
		MAP_TILE_DATA[i].texV = 0;
		MAP_TILE_DATA[i].texU = 0;
	}

	CURRENT_SCREEN = 0xcc;
	PREVIOUS_SCREEN = 0xcc;
	CURRENT_EXIT = 9;
	PREVIOUS_EXIT = 9;
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
	int32_t startTile;
	int16_t stride;
	int16_t columns;
	int32_t row;
	int32_t col;

	tickCameraFollowPlayer();

	startTile = (int16_t)(MAP_TILE_X + (MAP_TILE_Y * MAP_WIDTH[0]));
	columns = MAP_WIDTH[0];
	stride = MAP_WIDTH[0];
	if (stride > 3) {
		columns = 4;
	}

	if (((int8_t)(MAP_ENTRIES[CURRENT_SCREEN].flags & 0x40) == 0) &&
	    ((CURRENT_FRAME % 1200) == 0)) {
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

			if (MAP_TILES[startTile + (stride * row) + col] == -1) {
				setRGB0(prim, 0, 0, 0);
			} else {
				setRGB0(prim, 0x80, 0x80, 0x80);
			}

			if ((MAP_TILE_DATA[startTile + (stride * row) + col].texV % 256) != 0) {
				setUVDataPolyFT4(prim, 0, MAP_TILE_DATA[startTile + stride * row + col].texV % 256, 0x80, 0x7F);
			} else {
				setUVDataPolyFT4(prim, 0, MAP_TILE_DATA[startTile + stride * row + col].texV % 256, 0x80, 0x80);
			}

			prim->tpage = MAP_TILE_DATA[startTile + stride * row + col].tpage;
			prim->clut = MAP_TILE_DATA[startTile + stride * row + col].clut;
			setPosDataMapTile(&MAP_TILE_DATA[startTile + stride * row + col],
					  CAMERA_X[0], CAMERA_Y[0], prim);
			AddPrim(&ot[0xfff], prim);
			++prim;

			GsSetWorkBase((PACKET *)prim);
		}
	}

	renderMapOverlays(LOCAL_MAP_OBJECT_INSTANCE,
			  CAMERA_X[0], CAMERA_Y[0]);

	CAMERA_X_PREVIOUS = CAMERA_X[0];
	CAMERA_Y_PREVIOUS = CAMERA_Y[0];
}

void loadMap(int32_t mapId)
{
	int32_t *offsets;
	int32_t offset;
	int32_t i;
	uint32_t idx;
	uint8_t *objects;
	char path[32];

	offsets = (int32_t *)GENERAL_BUFFER_PTR;
	if (SKIP_MAP_FILE_READ == 0) {
		buildMapPath(path, &MAP_ENTRIES[mapId], MAP_FILE_EXT_MAP, mapId);
		readFile(path, (void *)offsets);
	}

	loadMapSetup((int32_t *)((uint8_t *)GENERAL_BUFFER_PTR + *offsets++));
	clearMapObjects(objects = (uint8_t *)LOCAL_MAP_OBJECT_INSTANCE);

	idx = mapId;
	if ((MAP_ENTRIES[mapId].num8bppImages != 0) ||
	    (MAP_ENTRIES[mapId].num4bppImages != 0)) {
		if (MAP_ENTRIES[mapId].num8bppImages != 0) {
			for (i = 0; i < MAP_ENTRIES[idx].num8bppImages; i++) {
				loadMapImage1((uint8_t *)GENERAL_BUFFER_PTR + *offsets++);
			}
		}

		if (MAP_ENTRIES[mapId].num4bppImages != 0) {
			for (i = 0; i < MAP_ENTRIES[idx].num4bppImages; i++) {
				loadMapImage2((uint8_t *)GENERAL_BUFFER_PTR + *offsets++, i);
			}
		}

		loadMapObjects(objects,
			       (uint8_t *)GENERAL_BUFFER_PTR + *offsets++,
			       mapId);
	}

	offset = *offsets++;

	clearMapDigimon();
	loadMapEntities((uint8_t *)GENERAL_BUFFER_PTR + offset, mapId,
			CURRENT_EXIT);

	if (MAP_ENTRIES[mapId].doorsId != 0) {
		loadDoors(MAP_ENTRIES[mapId].doorsId - 1);
	}
	if ((mapId > 100) && (mapId < 104)) {
		loadWarpCrystals(mapId);
	}
	if (mapId == 0xa5) {
		loadTrainingPoop();
	}

	loadMapCollisionData((uint8_t *)GENERAL_BUFFER_PTR + *offsets);

	if (SKIP_MAP_FILE_READ == 0) {
		loadMapSounds(getMapSoundId(mapId));
	}

	CURRENT_SCREEN = mapId;

	MAIN_func_800D39F8();
	MAIN_func_800D3A84();
	checkFishingMap(CURRENT_SCREEN, 0);
	checkCurlingMap(CURRENT_SCREEN);
	checkShopMap(CURRENT_SCREEN);
	checkArenaMap(CURRENT_SCREEN);

	SKIP_MAP_FILE_READ = 0;
}

void buildMapPath(char *out, MapEntry *entry, int8_t *suffix, int32_t mapId)
{
	char *prefix;
	char *digits;
	char *separator;
	char *name;
	uint8_t index;

	prefix = MAP_PATH_PREFIX;
	digits = MAP_PATH_DIGITS;
	separator = (char *)&MAP_PATH_SEPARATOR;

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

	name = entry->filename;
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
	int8_t regionA[4];
	int8_t regionB[4];
	int32_t i;
	int32_t j;
	int16_t favoredRegion;
	int32_t distance;

	GS_VIEWPOINT.vpx = *data++ << 1;
	GS_VIEWPOINT.vpy = *data++ << 1;
	GS_VIEWPOINT.vpz = *data++ << 1;
	GS_VIEWPOINT.vrx = *data++ << 1;
	GS_VIEWPOINT.vry = *data++ << 1;
	GS_VIEWPOINT.vrz = *data++ << 1;
	GsSetRefView2(&GS_VIEWPOINT);

	for (i = 0; i < 3; i++) {
		LIGHT_DATA[i].vx = *data++;
		LIGHT_DATA[i].vy = *data++;
		LIGHT_DATA[i].vz = *data++;
		LIGHT_DATA[i].r = *data++;
		LIGHT_DATA[i].g = *data++;
		LIGHT_DATA[i].b = *data++;
		GsSetFlatLight(i, &LIGHT_DATA[i]);
		MAP_LIGHT[i] = LIGHT_DATA[i];
	}

	data++;
	data++;
	data++;
	GsSetAmbient(0x800, 0x800, 0x800);
	distance = *data++;
	VIEWPORT_DISTANCE = distance;
	GsSetProjection(distance);

	for (j = 0; j < 4; j++) {
		regionA[j] = *data++;
	}

	for (j = 0; j < 4; j++) {
		regionB[j] = *data++;
	}

	PARTNER_AREA_RESPONSE[0] = 0;
	favoredRegion = RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].favoredRegion;

	for (j = 0; j < 4; j++) {
		if (regionA[j] == favoredRegion) {
			PARTNER_AREA_RESPONSE[0] = 1;
			break;
		}

		if (regionB[j] == favoredRegion) {
			PARTNER_AREA_RESPONSE[0] = 2;
			break;
		}
	}

	MAP_WIDTH[0] = *data++;
	MAP_HEIGHT[0] = *data++;

	for (i = 0; i < MAP_WIDTH[0] * MAP_HEIGHT[0]; i++) {
		MAP_TILES[i] = *data++;
	}

	return *data;
}

int32_t getMapSoundId(int32_t mapId)
{
	return MAP_ENTRIES[mapId].flags & 0x1f;
}

void readMapTFS(int32_t mapId)
{
	char path[32];

	buildMapPath(path, &MAP_ENTRIES[mapId], MAP_FILE_EXT_TFS, mapId);
	readFile(path, GENERAL_BUFFER_PTR);
}

void MAIN_func_800D634C(void)
{
	u_long *data;
	int16_t numCluts;
	int32_t i;
	MapTileData *tile;
	int32_t x;
	int32_t y;

	data = (u_long *)GENERAL_BUFFER_PTR;
	data++;
	numCluts = *data++;

	for (i = 0; i < numCluts; i++) {
		LoadClut(data, 0, 0x1e1 + i);
		MAP_CLUTS[i] = data;
		data += 128;
	}

	updateTimeOfDay();

	tile = MAP_TILE_DATA;
	for (y = 0; x = 0, y < MAP_HEIGHT[0]; y++) {
		for (; x < MAP_WIDTH[0]; x++) {
			tile->tileId = MAP_TILES[x + (y * MAP_WIDTH[0])];
			if (tile->tileId == -1) {
				fillTileData(tile, (uint8_t *)NULL,
					     (int16_t)(x % 4 * 64 + 768),
					     (int16_t)(y % 3 * 128), x * 128,
					     y * 128);
			} else {
				data++;
				fillTileData(tile, (uint8_t *)data,
					     (int16_t)(x % 4 * 64 + 768),
					     (int16_t)(y % 3 * 128), x * 128,
					     y * 128);
				data += 4096;
			}
			tile++;
		}
	}

	CAMERA_X[0] = ((MAP_WIDTH[0] * 128) / 2) - 160;
	CAMERA_Y[0] = ((*(int8_t *)MAP_HEIGHT * 128) / 2) - 120;
	DRAW_OFFSET_LIMIT_X_MAX = CAMERA_X[0] + 160;
	DRAW_OFFSET_LIMIT_X_MIN = -(*(int8_t *)MAP_WIDTH * 128 - 320 - DRAW_OFFSET_LIMIT_X_MAX);
	DRAW_OFFSET_LIMIT_Y_MAX = CAMERA_Y[0] + 120;
	DRAW_OFFSET_LIMIT_Y_MIN = -(*(int8_t *)&MAP_HEIGHT[0] * 128 - 240 - DRAW_OFFSET_LIMIT_Y_MAX);
	DRAWING_OFFSET_X = 160;
	PLAYER_OFFSET_X = 160;
	DRAWING_OFFSET_Y = 120;
	PLAYER_OFFSET_Y = 120;

	MAIN_func_800D6A4C(MAP_TILE_DATA);

	MAP_TILE_X = CAMERA_X[0] / 128;
	if (MAP_WIDTH[0] < 5) {
		MAP_TILE_X = 0;
	} else if (MAP_WIDTH[0] < (MAP_TILE_X + 4)) {
		MAP_TILE_X -= (int8_t)((MAP_TILE_X + 4) - MAP_WIDTH[0]);
	}

	PREV_TILE_X = MAP_TILE_X;

	MAP_TILE_Y = CAMERA_Y[0] / 128;
	if (MAP_HEIGHT[0] < 4) {
		MAP_TILE_Y = 0;
	} else if (MAP_HEIGHT[0] < (MAP_TILE_Y + 3)) {
		MAP_TILE_Y -= (int8_t)((MAP_TILE_Y + 3) - MAP_HEIGHT[0]);
	}

	PREV_TILE_Y = MAP_TILE_Y;

	uploadMapTileImages(MAP_TILE_DATA,
			    MAP_TILE_X + MAP_TILE_Y * MAP_WIDTH[0]);
	calcMapObjectOrder((LocalMapObjectInstance *)LOCAL_MAP_OBJECT_INSTANCE);

	CAMERA_FOLLOW_PLAYER = 1;
}

void updateTimeOfDay(void)
{
	int32_t i;
	int32_t red;
	int32_t green;

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
			LIGHT_DATA[i].r = (red * MAP_LIGHT[i].r) / 10;
			LIGHT_DATA[i].g = (green * MAP_LIGHT[i].g) / 10;
			LIGHT_DATA[i].b = (green * MAP_LIGHT[i].b) / 10;
			GsSetFlatLight(i, &LIGHT_DATA[i]);
		}
	}

	DAYTIME_TRANSITION_FRAME = 0x19;
	DAYTIME_TRANSITION_ACTIVE = 0;
	SKIP_DAYTIME_TRANSITION = 0;
}

void fillTileData(MapTileData *tile, uint8_t *imagePtr,
		  int32_t texU, int32_t texV,
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

void MAIN_func_800D6A4C(MapTileData *tiles)
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
	CAMERA_X[0] = CAMERA_X[0] + (int16_t)(tamerScreen.vx - viewScreen.vx);
	CAMERA_Y[0] = CAMERA_Y[0] + (int16_t)(tamerScreen.vy - viewScreen.vy);
	DRAWING_OFFSET_X = DRAWING_OFFSET_X + (viewScreen.vx - tamerScreen.vx);
	DRAWING_OFFSET_Y = DRAWING_OFFSET_Y + (viewScreen.vy - tamerScreen.vy);
	PLAYER_OFFSET_X = PLAYER_OFFSET_X + (int16_t)(viewScreen.vx - tamerScreen.vx);
	PLAYER_OFFSET_Y = PLAYER_OFFSET_Y + (int16_t)(viewScreen.vy - tamerScreen.vy);

	if (CAMERA_X[0] < 0) {
		CAMERA_X[0] = 0;
		DRAWING_OFFSET_X = DRAW_OFFSET_LIMIT_X_MAX;
	} else if (((MAP_WIDTH[0] * 128) - 320) < CAMERA_X[0]) {
		CAMERA_X[0] = (MAP_WIDTH[0] * 128) - 320;
		DRAWING_OFFSET_X = DRAW_OFFSET_LIMIT_X_MIN;
	}

	if (CAMERA_Y[0] < 0) {
		CAMERA_Y[0] = 0;
		DRAWING_OFFSET_Y = DRAW_OFFSET_LIMIT_Y_MAX;
	} else if (((MAP_HEIGHT[0] * 128) - 240) < CAMERA_Y[0]) {
		CAMERA_Y[0] = (MAP_HEIGHT[0] * 128) - 240;
		DRAWING_OFFSET_Y = DRAW_OFFSET_LIMIT_Y_MIN;
	}
}

void uploadMapTileImages(MapTileData *tiles, int16_t index)
{
	RECT rect;
	int16_t count;
	int16_t stride;
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

		if (MAP_HEIGHT[0] > 2) {
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
		MAP_TILE_DATA[i].imagePtr = 0;
		MAP_TILE_DATA[i].posY = 0;
		MAP_TILE_DATA[i].posX = 0;
		MAP_TILE_DATA[i].texV = 0;
		MAP_TILE_DATA[i].texU = 0;
	}

	MAP_HEIGHT[0] = 0;
	MAP_WIDTH[0] = 0;
	CAMERA_Y[0] = 0;
	DRAWING_OFFSET_X = 160;
	CAMERA_X[0] = 0;
	DRAWING_OFFSET_Y = 120;

	unloadMapParts();
}

void tickCameraFollowPlayer(void)
{
	PositionData *posData;
	SVECTOR worldPos;
	SVECTOR screenStart;
	SVECTOR screenEnd;

	if ((GAME_STATE == 0) &&
	    (CAMERA_FOLLOW_PLAYER == 1) &&
	    (getTamerState() == 0) &&
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
		posData = TAMER_ENTITY.entity.posData;
		copyVector(&worldPos, &posData->location);
		gte_ldv0(&worldPos);
		gte_rtps();
		gte_stsxy(&screenEnd);
		CAMERA_X[0] += screenEnd.vx - screenStart.vx;
		CAMERA_Y[0] += screenEnd.vy - screenStart.vy;
		MAIN_func_800D763C((DVECTOR *)&screenStart,
				   (DVECTOR *)&screenEnd);
		handleTileUpdate(POLLED_INPUT, 0);
	}
}

void initializeDaytimeTransition(int32_t timeOfDay)
{
	if ((PARTNER_STATE != 8) && (PARTNER_STATE != 0xd) &&
	    (getTamerState() != 0x11) && (getTamerState() != 0x13) &&
	    (CURRENT_TIME_OF_DAY != timeOfDay)) {
		DAYTIME_TRANSITION_FRAME = 0;
		CURRENT_TIME_OF_DAY = timeOfDay;
		addObject(0xfbe, timeOfDay, tickDaytimeTransition, NULL);
	}
}

void setPosDataMapTile(MapTileData *tile, int32_t camX, int32_t camY,
		       POLY_FT4 *prim)
{
	setPosDataPolyFT4(prim,
			  (tile->posX - 160) - (camX - (160 - DRAWING_OFFSET_X)),
			  (tile->posY - 120) - (camY - (120 - DRAWING_OFFSET_Y)),
			  128, 128);
}

void tickDaytimeTransition(int32_t transition)
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
	int32_t j;
	uint16_t clut[256];

	DAYTIME_TRANSITION_ACTIVE = 1;
	DAYTIME_TRANSITION_TARGET = transition;

	if ((GAME_STATE == 0) && (SKIP_DAYTIME_TRANSITION != 1) &&
	    (PARTNER_STATE != 8) && (PARTNER_STATE != 0xd) &&
	    (getTamerState() != 0x11) && (getTamerState() != 0x13)) {
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
					LIGHT_DATA[i].g = MAP_LIGHT[i].g * 7 / 10;
					LIGHT_DATA[i].b = MAP_LIGHT[i].b * 7 / 10;
				} else if (transition == 1) {
					LIGHT_DATA[i].r = MAP_LIGHT[i].r / 2;
					LIGHT_DATA[i].g = MAP_LIGHT[i].g / 2;
					LIGHT_DATA[i].b = MAP_LIGHT[i].b / 2;
				} else {
					LIGHT_DATA[i] = MAP_LIGHT[i];
				}

				GsSetFlatLight(i, &LIGHT_DATA[i]);
			}

			removeObject(0xfbe, transition);
			DAYTIME_TRANSITION_ACTIVE = 0;
			SKIP_DAYTIME_TRANSITION = 0;
		} else {
			steps = 6 - (DAYTIME_TRANSITION_FRAME / 5);

			for (j = 0; j < 256; j++) {
				red = clutA[0] & 0x1f;
				redB = clutB[0] & 0x1f;
				green = (clutA[0] >> 5) & 0x1f;
				blue = (clutA[0] >> 10) & 0x1f;
				greenB = (clutB[0] >> 5) & 0x1f;
				blueB = (clutB[0] >> 10) & 0x1f;

				red -= (uint8_t)((red - redB) / steps);
				green -= (uint8_t)((green - greenB) / steps);
				blue -= (uint8_t)((blue - blueB) / steps);

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

void MAIN_func_800D763C(DVECTOR *current, DVECTOR *previous)
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
				CAMERA_X[0] -= (int16_t)(previous->vx - current->vx);
			}
		} else if ((POLLED_INPUT & 0x2000) != 0) {
			if (PLAYER_OFFSET_X > DRAWING_OFFSET_X) {
				DRAWING_OFFSET_X -= current->vx - previous->vx;
				CAMERA_X[0] -= (int16_t)(previous->vx - current->vx);
			}
		}
	}

	if (atEdgeY == 1) {
		DRAWING_OFFSET_Y += current->vy - previous->vy;

		if ((POLLED_INPUT & 0x1000) != 0) {
			if (PLAYER_OFFSET_Y < DRAWING_OFFSET_Y) {
				DRAWING_OFFSET_Y -= current->vy - previous->vy;
				CAMERA_Y[0] -= (int16_t)(previous->vy - current->vy);
			}
		} else if ((POLLED_INPUT & 0x4000) != 0) {
			if (PLAYER_OFFSET_Y > DRAWING_OFFSET_Y) {
				DRAWING_OFFSET_Y -= current->vy - previous->vy;
				CAMERA_Y[0] -= (int16_t)(previous->vy - current->vy);
			}
		}
	}
}

void handleTileUpdate(int32_t input, int32_t force)
{
	if ((((input & 0x1000) != 0) ||
	     ((input & 0x8000) != 0) ||
	     ((input & 0x2000) != 0)) &&
	    (CAMERA_Y[0] <= CAMERA_Y_PREVIOUS)) {
		if ((MAP_HEIGHT[0] >= 8) &&
		    (CAMERA_Y[0] >= 0x201) &&
		    (CAMERA_Y[0] < 0x281)) {
			MAP_TILE_Y = 4;
		} else if ((MAP_HEIGHT[0] >= 7) &&
			   (CAMERA_Y[0] >= 0x181) &&
			   (CAMERA_Y[0] < 0x201)) {
			MAP_TILE_Y = 3;
		} else if ((MAP_HEIGHT[0] >= 6) &&
			   (CAMERA_Y[0] >= 0x101) &&
			   (CAMERA_Y[0] < 0x181)) {
			MAP_TILE_Y = 2;
		} else if ((MAP_HEIGHT[0] >= 5) &&
			   (CAMERA_Y[0] >= 0x81) &&
			   (CAMERA_Y[0] < 0x101)) {
			MAP_TILE_Y = 1;
		} else if ((MAP_HEIGHT[0] >= 4) &&
			   (CAMERA_Y[0] >= 0) &&
			   (CAMERA_Y[0] < 0x81)) {
			MAP_TILE_Y = 0;
		}

		if (force == 0) {
			updateTileRow(0);
		}
	} else if (((input & 0x4000) != 0) ||
		   ((input & 0x8000) != 0) ||
		   ((input & 0x2000) != 0)) {
		if (CAMERA_Y[0] >= CAMERA_Y_PREVIOUS) {
			if ((MAP_HEIGHT[0] >= 8) &&
			    (CAMERA_Y[0] >= 0x280) &&
			    (CAMERA_Y[0] < 0x301)) {
				MAP_TILE_Y = 5;
			} else if ((MAP_HEIGHT[0] >= 7) &&
				   (CAMERA_Y[0] >= 0x200) &&
				   (CAMERA_Y[0] < 0x280)) {
				MAP_TILE_Y = 4;
			} else if ((MAP_HEIGHT[0] >= 6) &&
				   (CAMERA_Y[0] >= 0x180) &&
				   (CAMERA_Y[0] < 0x200)) {
				MAP_TILE_Y = 3;
			} else if ((MAP_HEIGHT[0] >= 5) &&
				   (CAMERA_Y[0] >= 0x100) &&
				   (CAMERA_Y[0] < 0x180)) {
				MAP_TILE_Y = 2;
			} else if ((MAP_HEIGHT[0] >= 4) &&
				   (CAMERA_Y[0] >= 0x80)
				   && (CAMERA_Y[0] < 0x100)) {
				MAP_TILE_Y = 1;
			}

			if (((MAP_HEIGHT[0] * 128) - 240) <= CAMERA_Y[0]) {
				MAP_TILE_Y = MAP_HEIGHT[0] - 3;
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
	    (CAMERA_X[0] <= CAMERA_X_PREVIOUS)) {
		if ((MAP_WIDTH[0] >= 0xb) &&
		    (CAMERA_X[0] >= 0x30a) &&
		    (CAMERA_X[0] < 0x381)) {
			MAP_TILE_X = 6;
		} else if ((MAP_WIDTH[0] >= 0xa) &&
			   (CAMERA_X[0] >= 0x28a) &&
			   (CAMERA_X[0] < 0x301)) {
			MAP_TILE_X = 5;
		} else if ((MAP_WIDTH[0] >= 9) &&
			   (CAMERA_X[0] >= 0x20a) &&
			   (CAMERA_X[0] < 0x281)) {
			MAP_TILE_X = 4;
		} else if ((MAP_WIDTH[0] >= 8) &&
			   (CAMERA_X[0] >= 0x18a) &&
			   (CAMERA_X[0] < 0x201)) {
			MAP_TILE_X = 3;
		} else if ((MAP_WIDTH[0] >= 7) &&
			   (CAMERA_X[0] >= 0x10a) &&
			   (CAMERA_X[0] < 0x181)) {
			MAP_TILE_X = 2;
		} else if ((MAP_WIDTH[0] >= 6) &&
			   (CAMERA_X[0] >= 0x8a) &&
			   (CAMERA_X[0] < 0x101)) {
			MAP_TILE_X = 1;
		} else if ((MAP_WIDTH[0] >= 5) &&
			   (CAMERA_X[0] >= 0xa) &&
			   (CAMERA_X[0] < 0x81)) {
			MAP_TILE_X = 0;
		}

		if (force == 0) {
			updateTileColumn(0);
		}
	} else if (((input & 0x2000) != 0) ||
		   ((input & 0x1000) != 0) ||
		   ((input & 0x4000) != 0)) {
		if (CAMERA_X[0] >= CAMERA_X_PREVIOUS) {
			if ((MAP_WIDTH[0] >= 0xb) &&
			    (CAMERA_X[0] >= 0x38a) &&
			    (CAMERA_X[0] < 0x3f7)) {
				MAP_TILE_X = 7;
			} else if ((MAP_WIDTH[0] >= 0xa) &&
				   (CAMERA_X[0] >= 0x30a) &&
				   (CAMERA_X[0] < 0x377)) {
				MAP_TILE_X = 6;
			} else if ((MAP_WIDTH[0] >= 9) &&
				   (CAMERA_X[0] >= 0x28a) &&
				   (CAMERA_X[0] < 0x2f7)) {
				MAP_TILE_X = 5;
			} else if ((MAP_WIDTH[0] >= 8) &&
				   (CAMERA_X[0] >= 0x20a) &&
				   (CAMERA_X[0] < 0x277)) {
				MAP_TILE_X = 4;
			} else if ((MAP_WIDTH[0] >= 7) &&
				   (CAMERA_X[0] >= 0x18a) &&
				   (CAMERA_X[0] < 0x1f7)) {
				MAP_TILE_X = 3;
			} else if ((MAP_WIDTH[0] >= 6) &&
				   (CAMERA_X[0] >= 0x10a) &&
				   (CAMERA_X[0] < 0x177)) {
				MAP_TILE_X = 2;
			} else if ((MAP_WIDTH[0] >= 5) &&
				   (CAMERA_X[0] >= 0x8a) &&
				   (CAMERA_X[0] < 0xf6)) {
				MAP_TILE_X = 1;
			}

			if (((MAP_WIDTH[0] * 128) - 320) <= CAMERA_X[0]) {
				MAP_TILE_X = MAP_WIDTH[0] - 4;
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
		int32_t tmp;
		int32_t width;

		tmp = MAP_WIDTH[0];
		width = tmp;
		tmp = MAP_TILE_Y;
		uploadMapTileImages(MAP_TILE_DATA, MAP_TILE_X + tmp * width);
	}
}

void cameraIsAtEdge(int32_t *outX, int32_t *outY)
{
	int32_t cameraX;
	int32_t limitX;
	int32_t cameraY;
	int32_t limitY;

	*outY = 1;
	*outX = 1;

	if ((cameraX = CAMERA_X[0]) < 0) {
		CAMERA_X[0] = 0;
		DRAWING_OFFSET_X = DRAW_OFFSET_LIMIT_X_MAX;
		*outX = 0;
	} else if ((limitX = MAP_WIDTH[0] * 128 - 320) < cameraX) {
		CAMERA_X[0] = limitX;
		DRAWING_OFFSET_X = DRAW_OFFSET_LIMIT_X_MIN;
		*outX = 0;
	}

	if ((cameraY = CAMERA_Y[0]) < 0) {
		CAMERA_Y[0] = 0;
		DRAWING_OFFSET_Y = DRAW_OFFSET_LIMIT_Y_MAX;
		*outY = 0;
	} else if ((limitY = MAP_HEIGHT[0] * 128 - 240) < cameraY) {
		CAMERA_Y[0] = limitY;
		DRAWING_OFFSET_Y = DRAW_OFFSET_LIMIT_Y_MIN;
		*outY = 0;
	}
}

void createCameraMovement(VECTOR *target, int32_t instanceId)
{
	CAMERA_TARGET = *target;
	addObject(0xfb1, instanceId, (TickFunction)tickCameraMovement, 0);
	CAMERA_REACHED_TARGET = -1;
}

void tickCameraMovement(int16_t instanceId)
{
	SVECTOR target;
	SVECTOR view;
	SVECTOR screenTarget;
	SVECTOR screenView;
	int32_t atEdgeX;
	int32_t atEdgeY;
	int32_t stepX;
	int32_t stepY;
	uint32_t flags;
	int32_t id;
	int32_t qx;
	int32_t qy;
	int32_t remX;
	int32_t remY;

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
		CAMERA_MOVE_DRAW_OFFSET_X = DRAWING_OFFSET_X;
		CAMERA_MOVE_DRAW_OFFSET_Y = DRAWING_OFFSET_Y;
		CAMERA_MOVE_DIFF_X = (screenView.vx - screenTarget.vx) + 160;
		CAMERA_MOVE_DIFF_Y = (screenView.vy - screenTarget.vy) + 120;
		PLAYER_OFFSET_X = PLAYER_OFFSET_X + (int16_t)(CAMERA_MOVE_DIFF_X - PLAYER_OFFSET_X);
		PLAYER_OFFSET_Y = PLAYER_OFFSET_Y + (int16_t)(CAMERA_MOVE_DIFF_Y - PLAYER_OFFSET_Y);

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

		CAMERA_MOVE_FINAL_X = CAMERA_X[0] + (CAMERA_MOVE_DRAW_OFFSET_X - CAMERA_MOVE_DIFF_X);
		CAMERA_MOVE_FINAL_Y = CAMERA_Y[0] + (CAMERA_MOVE_DRAW_OFFSET_Y - CAMERA_MOVE_DIFF_Y);
		CAMERA_MOVE_DELTA_X = (CAMERA_MOVE_DIFF_X - CAMERA_MOVE_DRAW_OFFSET_X) % instanceId;
		CAMERA_MOVE_DELTA_Y = (CAMERA_MOVE_DIFF_Y - CAMERA_MOVE_DRAW_OFFSET_Y) % instanceId;

		remX = CAMERA_MOVE_DELTA_X;
		if (remX < 0) {
			CAMERA_MOVE_DELTA_X = -remX;
		}

		remY = CAMERA_MOVE_DELTA_Y;
		if (remY < 0) {
			CAMERA_MOVE_DELTA_Y = -remY;
		}

		unsetCameraFollowPlayer();
		CAMERA_HAS_TARGET = 1;
	}

	id = instanceId;
	stepX = (int16_t)((CAMERA_MOVE_DIFF_X - CAMERA_MOVE_DRAW_OFFSET_X) / id);
	stepY = (int16_t)((CAMERA_MOVE_DIFF_Y - CAMERA_MOVE_DRAW_OFFSET_Y) / instanceId);
	qx = (int16_t)((CAMERA_MOVE_DRAW_OFFSET_X - CAMERA_MOVE_DIFF_X) / instanceId);
	CAMERA_X[0] = CAMERA_X[0] + qx;
	qy = (int16_t)((CAMERA_MOVE_DRAW_OFFSET_Y - CAMERA_MOVE_DIFF_Y) / instanceId);
	CAMERA_Y[0] = CAMERA_Y[0] + qy;
	DRAWING_OFFSET_X = DRAWING_OFFSET_X + stepX;
	DRAWING_OFFSET_Y = DRAWING_OFFSET_Y + stepY;

	if (CAMERA_MOVE_DELTA_X >= 0) {
		if ((CAMERA_MOVE_DRAW_OFFSET_X - CAMERA_MOVE_DIFF_X) > 0) {
			CAMERA_X[0] = CAMERA_X[0] + 1;
		} else {
			CAMERA_X[0] = CAMERA_X[0] - 1;
		}

		if ((CAMERA_MOVE_DIFF_X - CAMERA_MOVE_DRAW_OFFSET_X) > 0) {
			DRAWING_OFFSET_X = DRAWING_OFFSET_X + 1;
		} else {
			DRAWING_OFFSET_X = DRAWING_OFFSET_X - 1;
		}

		CAMERA_MOVE_DELTA_X--;
	}

	if (CAMERA_MOVE_DELTA_Y >= 0) {
		if ((CAMERA_MOVE_DRAW_OFFSET_Y - CAMERA_MOVE_DIFF_Y) > 0) {
			CAMERA_Y[0] = CAMERA_Y[0] + 1;
		} else {
			CAMERA_Y[0] = CAMERA_Y[0] - 1;
		}

		if ((CAMERA_MOVE_DIFF_Y - CAMERA_MOVE_DRAW_OFFSET_Y) > 0) {
			DRAWING_OFFSET_Y = DRAWING_OFFSET_Y + 1;
		} else {
			DRAWING_OFFSET_Y = DRAWING_OFFSET_Y - 1;
		}

		CAMERA_MOVE_DELTA_Y--;
	}

	cameraIsAtEdge(&atEdgeX, &atEdgeY);

	if (atEdgeX == 1) {
		if ((CAMERA_MOVE_DIFF_X - CAMERA_MOVE_DRAW_OFFSET_X) < 0) {
			if (DRAWING_OFFSET_X < CAMERA_MOVE_DIFF_X) {
				DRAWING_OFFSET_X = (int16_t)CAMERA_MOVE_DIFF_X;
			}
		} else {
			if (CAMERA_MOVE_DIFF_X < DRAWING_OFFSET_X) {
				DRAWING_OFFSET_X = (int16_t)CAMERA_MOVE_DIFF_X;
			}

			if (CAMERA_MOVE_DRAW_OFFSET_X == CAMERA_MOVE_DIFF_X) {
				DRAWING_OFFSET_X = (int16_t)CAMERA_MOVE_DIFF_X;
			}
		}

		if (DRAWING_OFFSET_X == CAMERA_MOVE_DIFF_X) {
			CAMERA_X[0] = CAMERA_MOVE_FINAL_X;
			atEdgeX = 0;
		}
	}

	if (atEdgeY == 1) {
		if ((CAMERA_MOVE_DIFF_Y - CAMERA_MOVE_DRAW_OFFSET_Y) < 0) {
			if (DRAWING_OFFSET_Y < CAMERA_MOVE_DIFF_Y) {
				DRAWING_OFFSET_Y = (int16_t)CAMERA_MOVE_DIFF_Y;
			}
		} else {
			if (CAMERA_MOVE_DIFF_Y < DRAWING_OFFSET_Y) {
				DRAWING_OFFSET_Y = (int16_t)CAMERA_MOVE_DIFF_Y;
			}

			if (CAMERA_MOVE_DRAW_OFFSET_Y == CAMERA_MOVE_DIFF_Y) {
				DRAWING_OFFSET_Y = (int16_t)CAMERA_MOVE_DIFF_Y;
			}
		}

		if (DRAWING_OFFSET_Y == CAMERA_MOVE_DIFF_Y) {
			CAMERA_Y[0] = CAMERA_MOVE_FINAL_Y;
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

	if ((((CAMERA_MOVE_DRAW_OFFSET_X - CAMERA_MOVE_DIFF_X) / instanceId) >= 0x50) ||
	    (((CAMERA_MOVE_DRAW_OFFSET_Y - CAMERA_MOVE_DIFF_Y) / instanceId) >= 0x50)) {
		handleTileUpdate(flags, 1);
	}

	if ((((CAMERA_MOVE_DRAW_OFFSET_X - CAMERA_MOVE_DIFF_X) / instanceId) < 0x50) ||
	    (((CAMERA_MOVE_DRAW_OFFSET_Y - CAMERA_MOVE_DIFF_Y) / instanceId) < 0x50)) {
		handleTileUpdate(flags, 0);
	}

	if ((atEdgeX == 0) && (atEdgeY == 0)) {
		CAMERA_HAS_TARGET = 0;
		setCameraFollowPlayer();

		if (CAMERA_REACHED_TARGET == 0) {
			CAMERA_REACHED_TARGET = 1;
		}

		removeObject(0xfb1, id);

		if (CAMERA_UPDATE_TILES == 1) {
			uploadMapTileImages(MAP_TILE_DATA,
					    MAP_TILE_X + MAP_TILE_Y * ((int8_t *)MAP_WIDTH)[0]);
			CAMERA_UPDATE_TILES = 0;
		}
	}
}

void storeEntityLocation(uint32_t scriptId, VECTOR *out)
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

int32_t checkCameraMovement(int32_t instanceId)
{
	if (CAMERA_REACHED_TARGET == -1) {
		CAMERA_REACHED_TARGET = 0;
		addObject(0xfb1, instanceId,
			  (TickFunction)tickCameraMovement, NULL);
		goto out;
	}

	if (CAMERA_REACHED_TARGET == 1) {
		CAMERA_REACHED_TARGET = -1;
		return 1;
	}
out:
	return 0;
}

void tickMoveCameraTo(int32_t x, int32_t z, int32_t arg2)
{
	CAMERA_TARGET.vx = x;
	CAMERA_TARGET.vy = TAMER_ENTITY.entity.posData->location.vy;
	CAMERA_TARGET.vz = z;

	checkCameraMovement(arg2);
}

int32_t tickMoveCameraToEntity(uint32_t scriptId, int32_t speed)
{
	storeEntityLocation(scriptId, &CAMERA_TARGET);

	return checkCameraMovement(speed);
}

void moveCameraByDiff(VECTOR *from, VECTOR *to)
{
	SVECTOR fromPos;
	SVECTOR toPos;
	SVECTOR fromScreen;
	SVECTOR toScreen;
	uint32_t flags;

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
	CAMERA_X[0] = CAMERA_X[0] + (int16_t)(toScreen.vx - fromScreen.vx);
	CAMERA_Y[0] = CAMERA_Y[0] + (int16_t)(toScreen.vy - fromScreen.vy);

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

	MAIN_func_800D763C((DVECTOR *)&fromScreen, (DVECTOR *)&toScreen);
	handleTileUpdate(flags, 0);
}

void MAIN_func_800D8AC8(int32_t diffX, int32_t diffY)
{
	int32_t atEdgeX;
	int32_t atEdgeY;

	CAMERA_X[0] = CAMERA_X[0] + diffX;
	CAMERA_Y[0] = CAMERA_Y[0] + diffY;
	DRAWING_OFFSET_X = DRAWING_OFFSET_X - diffX;
	DRAWING_OFFSET_Y = DRAWING_OFFSET_Y - diffY;
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

void updateTileRow(int32_t bottom)
{
	RECT rect;
	int16_t base;
	int16_t count;
	int32_t start;
	int32_t i;

	if (MAP_TILE_Y != PREV_TILE_Y) {
		base = MAP_TILE_X + (MAP_TILE_Y * MAP_WIDTH[0]);

		if (MAP_WIDTH[0] >= 4) {
			count = 4;
		} else {
			count = MAP_WIDTH[0];
		}

		if (bottom == 0) {
			start = 0;
		} else {
			start = (int16_t)(MAP_WIDTH[0] * 2);
		}

		start = base + start;

		for (i = 0; i < count; i++) {
			setRECT(&rect, MAP_TILE_DATA[i + start].texU, MAP_TILE_DATA[i + start].texV, 64, 128);

			if (MAP_TILE_DATA[base + i].tileId == -1) {
				ClearImage(&rect, 0, 0, 0);
			} else {
				LoadImage(&rect, (u_long *)MAP_TILE_DATA[i + start].imagePtr);
			}

			DrawSync(0);
		}

		PREV_TILE_Y = MAP_TILE_Y;
	}
}

void updateTileColumn(int32_t arg0)
{
	int32_t width;
	int32_t widthCopy;
	int32_t tileY;
	int32_t stride;
	int16_t tileIndex;
	int16_t colOffset;
	int32_t rowOffset;
	int32_t i;
	int32_t idx;
	RECT rect;

	if (MAP_TILE_X != PREV_TILE_X) {
		width = MAP_WIDTH[0];
		tileY = MAP_TILE_Y;
		tileIndex = MAP_TILE_X + (tileY * width);
		widthCopy = MAP_WIDTH[0];
		stride = (int16_t)widthCopy;

		if (arg0 == 0) {
			colOffset = 0;
		} else {
			colOffset = 3;
		}

		for (i = 0, rowOffset = 0; i < 3; i++, rowOffset += stride) {
			if ((MAP_HEIGHT[0] < 3) && (i == 2)) {
				break;
			}

			idx = colOffset + (tileIndex + rowOffset);
			setRECT(&rect, MAP_TILE_DATA[idx].texU, MAP_TILE_DATA[idx].texV, 64, 128);

			if (MAP_TILE_DATA[idx].tileId == -1) {
				ClearImage(&rect, 0, 0, 0);
			} else {
				LoadImage(&rect, (u_long *)MAP_TILE_DATA[idx].imagePtr);
			}

			DrawSync(0);
		}

		PREV_TILE_X = MAP_TILE_X;
	}
}

int32_t MAIN_func_800D8E64(int16_t mapId, int16_t exitId, int32_t showName)
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

			setTamerState(6);
			fadeFromBlack(20);

			SCRIPT_MAP_CHANGE_STATE = 0;

			if (IS_SCRIPT_PAUSED == 1) {
				setPartnerState(1);
				setTamerState(0);
				setCameraFollowPlayer();
			}

			checkMapInteraction();

			return 1;
		}

		break;
	}

	return 0;
}

void addMapNameObject(int32_t mapId)
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

void MAIN_func_800D91EC(int16_t a, int16_t b)
{
	MAIN_D_801343B0 = a;
	MAIN_D_801343B2 = b;
	MAIN_D_801343B4 = 0;
}

int32_t MAIN_func_800D91FC(int32_t flag)
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

void MAIN_func_800D9248(void)
{
	changeMap(MAIN_D_801343B0, MAIN_D_801343B2);
}

void renderMapName(int32_t mapId)
{
	int32_t length;

	length = strlen(MAP_NAME_PTR[MAP_ENTRIES[mapId].loadingName]);
	renderString(0, 12 - (length / 2) * 8, -6, length * 8 + 4, 12, 0, 0,
		     0, 0);
}

void MAIN_func_800D92EC(void)
{
	addObject(0xfa2, 0, tickGameClock, renderGameClock);
	addObject(0xfa0, 0, NULL, renderMap);
	addObject(0xfa8, 0, NULL, renderPoop);
	setTamerState(0);
	setPartnerState(1);
}

void MAIN_func_800D9360(int32_t mapId)
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
	MERAMON_SHAKE_BACKUP_OFFSET_X = DRAWING_OFFSET_X;
	MERAMON_SHAKE_FRAME_COUNT = 0;
	MERAMON_SHAKE_BACKUP_OFFSET_Y = DRAWING_OFFSET_Y;
	MERAMON_SHAKE_HEIGHT = 0;
	MERAMON_SHAKE_WIDTH = 0;
	MERAMON_SHAKE_POS_Y = 0;
	MERAMON_SHAKE_POS_X = 0;
	addObject(0xfb8, 0, tickMeramonShake, 0);
}

void tickMeramonShake(int32_t arg0)
{
	POLY_FT4 *prim;
	int8_t offset;

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
		MERAMON_SHAKE_POS_X -= MERAMON_SHAKE_STEP;
		MERAMON_SHAKE_POS_Y -= MERAMON_SHAKE_STEP;

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
	setRGB0(prim, MERAMON_SHAKE_COLOR_R, MERAMON_SHAKE_COLOR_G, MERAMON_SHAKE_COLOR_B);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[9], prim);
	prim++;

	GsSetWorkBase((PACKET *)prim);

	MERAMON_SHAKE_FRAME_COUNT++;

	if (MERAMON_SHAKE_FRAME_COUNT >= 0xd2) {
		DRAWING_OFFSET_X = MERAMON_SHAKE_BACKUP_OFFSET_X;
		DRAWING_OFFSET_Y = MERAMON_SHAKE_BACKUP_OFFSET_Y;
		removeObject(0xfb8, 0);
	}
}

void MAIN_func_800D9660(GsRVIEW2 *out)
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
	int32_t *models;
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
		if (LOADED_DIGIMON_MODELS[i] != -1) {
			thunkUnloadModel((models = LOADED_DIGIMON_MODELS)[i], 0);
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

	if (t1 == t0) {
		result = 0;
	} else {
		end -= start;
		result = (end * (t - t0)) / (t1 - t0);
		if (end >= 0) {
			result = result % (end + 1);
			if (result < 0) {
				result += end;
			}
		} else {
			result = result % (end - 1);
			if (result > 0) {
				result += end;
			}
		}

		result += start;
	}

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

	originalType = ORIGINAL_TYPE[type];
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

void MAIN_func_800D9B60(uint32_t *buffer)
{
	RECT rect;

	setRECT(&rect, 0, 480, 256, 7);
	StoreImage(&rect, (u_long *)buffer);
	DrawSync(0);
}

void MAIN_func_800D9BA8(int32_t level, int16_t *src)
{
	int16_t buffer[1792];
	RECT rect;
	int16_t *dst;
	int32_t i;
	int16_t red;
	int16_t green;
	int16_t blue;
	int16_t mask;

	dst = buffer;
	for (i = 0; i < 256; i++) {
		red = *src & 0x1f;
		green = (*src >> 5) & 0x1f;
		blue = (*src >> 10) & 0x1f;
		mask = (*src++ >> 15) & 0x1;
		*dst = red * (255 - level) / 255;
		*dst += (int16_t)((green * (255 - level) / 255) << 5);
		*dst += (int16_t)((blue * (255 - level) / 255) << 10);
		*dst++ += (int16_t)(mask << 15);
	}

	src += 768;
	dst += 768;
	for (i = 0; i < 768; i++) {
		red = *src & 0x1f;
		green = (*src >> 5) & 0x1f;
		blue = (*src >> 10) & 0x1f;
		mask = (*src++ >> 15) & 0x1;
		*dst = red * (255 - level) / 255;
		*dst += (int16_t)((green * (255 - level) / 255) << 5);
		*dst += (int16_t)((blue * (255 - level) / 255) << 10);
		*dst++ += (int16_t)(mask << 15);
	}

	setRECT(&rect, 0, 480, 256, 7);
	LoadImage(&rect, (u_long *)buffer);
	DrawSync(0);
}

void MAIN_func_800D9E68(u_long *buffer)
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

void MAIN_func_800D9F14(int32_t fade, int16_t *src)
{
	int16_t pixels[416];
	RECT rect;
	int16_t *dst;
	int32_t i;
	int16_t red;
	int16_t green;
	int16_t blue;
	int16_t mask;

	dst = pixels;
	for (i = 0; i < 416; i++) {
		red = *src & 0x1f;
		green = (*src >> 5) & 0x1f;
		blue = (*src >> 10) & 0x1f;
		mask = (*src++ >> 15) & 0x1;
		*dst = red * (255 - fade) / 255;
		*dst += (int16_t)((green * (255 - fade) / 255) << 5);
		*dst += (int16_t)((blue * (255 - fade) / 255) << 10);
		*dst++ += (int16_t)(mask << 15);
	}

	setRECT(&rect, 272, 480, 16, 1);
	LoadImage(&rect, (u_long *)&pixels[0]);

	setRECT(&rect, 96, 501, 16, 1);
	LoadImage(&rect, (u_long *)&pixels[16]);

	setRECT(&rect, 224, 488, 16, 24);
	LoadImage(&rect, (u_long *)&pixels[32]);
	DrawSync(0);
}

int32_t MAIN_func_800DA0F4(POLY_FT3 *prim, int32_t order)
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
	MAIN_func_800DA0F4(prim, otz);
}

int32_t add3DSpritePrim(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2,
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
	distance = distance >> 4;
	if ((distance >= 0) && (distance < 0x1000)) {
		GsSortSprite(sprite, ACTIVE_ORDERING_TABLE,
			     (uint16_t)distance);
	}
}

void addFXPrim(POLY_FT4 *prim, int32_t posX, int32_t posY, int16_t width,
	       int16_t height, int32_t distance)
{
	int32_t scaledWidth;
	int16_t x;
	int32_t scaledHeight;
	int16_t y;

	scaledWidth = ((uint32_t)(width * VIEWPORT_DISTANCE) /
		       (uint32_t)distance);
	x = posX - (int16_t)(scaledWidth >> 1);
	scaledHeight = ((uint32_t)(height * VIEWPORT_DISTANCE) /
			(uint32_t)distance);
	prim->x0 = x;
	y = posY - (int16_t)(scaledHeight >> 1);
	prim->y0 = y;
	prim->x1 = x + scaledWidth;
	prim->y1 = y;
	distance >>= 4;
	distance -= 0x37;
	prim->x2 = x;
	prim->y2 = y + scaledHeight;
	prim->x3 = x + scaledWidth;
	prim->y3 = y + scaledHeight;

	if ((distance > 0x20) && (distance < 0x1000)) {
		AddPrim(ACTIVE_ORDERING_TABLE->org + distance, prim);
		prim++;
		GsSetWorkBase((PACKET *)prim);
	}
}

void setShortWithStride(int16_t *dest, int16_t value, int32_t count,
			int32_t stride)
{
	int16_t i;

	for (i = 0; i < count; i++) {
		*dest = value;
		dest = (int16_t *)((int32_t)dest + stride);
	}
}

void renderTMDModel(uint8_t *buffer, int32_t id, GsCOORDINATE2 *coord,
			GsCOORDINATE2 *super, VECTOR *trans, SVECTOR *rot,
			VECTOR *scale)
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

void MAIN_func_800DA60C(int32_t count, int32_t arg1, int32_t arg2,
			int32_t *out)
{
	int32_t i;

	for (i = 0; i < count; i++) {
		out[i] = 0;
	}
}

int32_t MAIN_func_800DA63C(int32_t count, int32_t t, int32_t *keys,
			   int32_t *values, int32_t *slopes)
{
	uint32_t off;
	int32_t *key;
	int32_t *value;
	int32_t hi;
	int32_t mid;
	int32_t x0;
	int32_t dx;
	int32_t *slope;
	int32_t dt;
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
}

int32_t MAIN_func_800DA740(int16_t *rect, DVECTOR *line)
{
	int16_t code[2];
	int32_t cx[4];
	int32_t cy[4];
	int16_t *c;
	DVECTOR *p;
	int32_t i;
	int32_t dx;
	int32_t dy;
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
			*c = *c + 3;
		} else if (p->vy < rect[1]) {
			*c = *c - 3;
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
	cy[3] = rect[3] - line[0].vy;
	cx[3] = rect[2] - line[0].vx;

	cross = (dx * cy[0]) - (dy * cx[0]);

	for (i = 1; i < 4; i++) {
		if ((cross * ((dx * cy[i]) - (dy * cx[i]))) < 0) {
			return -1;
		}
	}

	return 0;
}

int16_t MAIN_func_800DA9C8(void)
{
	MAIN_D_80134DFC = ENTITY_TABLE[1]->posData->location.vy;

	return MAIN_D_80134DFC;
}

int16_t MAIN_func_800DA9F4(void)
{
	return MAIN_D_80134DFC;
}

void renderFXParticle(SVECTOR *pos, int32_t size, uint8_t *color)
{
	POLY_FT4 *prim;
	int32_t depth;
	DVECTOR screenPos;

	prim = (POLY_FT4 *)GsGetWorkBase();
	depth = worldPosToScreenPos(pos, &screenPos);
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	setSemiTrans(prim, 1);
	setRGB0(prim, color[0], color[1], color[2]);
	prim->tpage = getTPage(0, 1, 768, 256);
	prim->clut = getClut(192, 489);
	setUVWH(prim, 0, 160, 15, 15);
	addFXPrim(prim, screenPos.vx, screenPos.vy, size, size, depth);
}
