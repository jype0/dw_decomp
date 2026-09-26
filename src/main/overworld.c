#include <mwinline_n.h>

#include <dw/anim.h>
#include <dw/clock.h>
#include <dw/evl.h>
#include <dw/fish.h>
#include <dw/font.h>
#include <dw/item.h>
#include <dw/map.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/partner.h>
#include <dw/sound.h>
#include <dw/std.h>
#include <dw/tamer.h>
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
extern int8_t MENU_SUB_STATE;
extern int32_t TRIANGLE_MENU_STATE;
extern int32_t MAIN_D_80134D2C;
extern int8_t MENU_STATE;
extern int8_t MAIN_D_80134D36;
extern int8_t MAIN_D_80134D37;
extern int16_t MAIN_D_80134D38;
extern int16_t MAIN_D_80134D3A;
extern int32_t CHANGED_INPUT;
typedef struct {
  uint8_t digimon;
  uint8_t moves;
} MenuTabPair;

typedef struct {
	int8_t tab[4];
} PlayerTabs;

extern uint8_t INVENTORY_POINTER;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern int32_t MAIN_D_80134D28;

typedef struct {
	uint8_t data[8];
} TriangleCursorUVData;

typedef struct {
	int8_t data[8];
} TriangleCursorOffsetData;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t unknown;
	int8_t disabled;
	int8_t clutY;
	uint8_t width;
	uint8_t height;
	uint8_t texX;
	uint8_t texY;
} GameMenuSprite;

typedef struct {
	int16_t x;
	int16_t y;
	uint8_t width;
	uint8_t height;
	uint8_t texX;
	uint8_t texY;
} GameMenuLabel;

typedef struct {
	int16_t posX;
	int16_t posY;
	uint8_t u;
	uint8_t v;
	uint8_t clut;
	uint8_t pad;
} EvoChartEntry;

extern int8_t SELECTED_MEDAL;
extern int8_t MEDAL_SELECTOR_INDEX;
extern int8_t SELECTED_CARD;
extern int16_t MAIN_D_80134D40;
extern int16_t MAIN_D_80134D42;
extern int16_t MAIN_D_80134D44;
extern int8_t MAIN_D_80134D46;
extern int16_t MAIN_D_80134D48;
extern char MAIN_D_80134338[];
extern char MAIN_D_8013433C[];
extern char MAIN_D_80134344[];
extern char MAIN_D_8013434C[];
extern char MAIN_D_80134354[];

int32_t isTriggerSet(int32_t triggerId);
void callScriptSection(uint16_t scriptId, uint32_t scriptSection,
		       uint32_t param);
void startNPCAnimation(uint8_t scriptId, int32_t animId);
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

void setCameraFollowPlayer(void);
void handleGameMenuSelection(int32_t selection);
int32_t createMenuBox(int32_t id, int16_t x, int16_t y, int16_t width,
		      int16_t height, int8_t features, void (*tick)(void),
		      void (*render)(void));
void closeUIBoxIfOpen(int32_t arg);
void getEntityScreenPos(Entity *entity, int32_t flag, int16_t *outPos);
void initializeInventoryObject(void);
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
void renderSeperatorLines(int16_t *lines, int8_t count, int32_t zIndex);
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
void tickNPCBattle(int32_t instanceId);
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
void removeTriangleMenu(void);
void closeInventoryBoxes(void);
void removeUIBox1(void);
void collisionGrace(int32_t a, Entity *entity, int32_t c, int32_t d);
uint8_t entityIsOffScreen(Entity *entity, int32_t w, int32_t h);
void tickNPC(int32_t instanceId);
void tickNPCOverworld(int32_t instanceId, MapDigimonEntity *mapDigimon);
void scriptUnloadEntity(uint8_t scriptId);
void setLoopCountToOne(uint32_t scriptId);
void loadNPCModel(int32_t digimonId);
void unloadDigimonModel(int32_t digimonType);
void setPartnerIdling(void);
int32_t tickRemoveMist(void);
void setActiveAnim(uint8_t scriptId, uint8_t animId);
void spawnSpriteAtEntity(int32_t scriptId, int32_t nodeId, uint8_t sprite);
void spawnSpriteAtLocation(int16_t x, int16_t y, int16_t z, int16_t sprite,
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
void loadMapObjectPosition(int16_t *xData, int16_t *yData, int16_t startIndex,
			   int32_t count);
void moveMapObjects(uint8_t startIndex, int32_t count, int16_t dx,
		    int16_t dy);
int32_t moveMapObjectsWithLimit(uint8_t startIndex, int32_t count, int16_t dx,
				int16_t dy, int16_t limitX, int16_t limitY);
void setMapObjectsFlag(uint8_t start, int32_t count, int32_t flag);
void resetMapObjectAnimation(uint8_t startIndex, int32_t count);
void clearMapAITable(int32_t index);
void removeMapEntities(void);
void clearMapDigimon(void);
void resetEntityOrigin(int32_t scriptId);
void loadMapDigimon(int16_t *data, int32_t mapId);
void initializeLoadedNPCModels(void);
int32_t scriptSetDigimon(uint8_t type, uint8_t slot, uint8_t autotalk);
int32_t tickMoveObjectTo(uint8_t objectIndex, uint8_t moveIndex,
			 int8_t steps, int16_t targetX, int16_t targetY);
void setMovementEnabled(int32_t id, int32_t enabled);
void addGameMenu(void);
void renderDateDigits(void);
void renderTriangleCursor(int32_t selection, int16_t yOffset);
int32_t isUIBoxAvailable(int32_t id);
void setSleepDisabled(int32_t arg);
void startFeedingItem(int32_t arg);
void removeOneSelectedItem(void);
void renderFeedingItem(int32_t arg);
int32_t getEquippedSlot(void);
void equipMove(void);
int32_t isKeyDown(int32_t mask);
void convertValueToDigits(int32_t digits, int32_t value, int32_t *outCount,
			  int32_t *outDigits);
void renderLinePrimitive(int32_t color, int32_t x0, int32_t y0, int32_t x1,
			 int32_t y1, int32_t zIndex, int32_t flag);
void renderTrianglePrimitive(int32_t color, int32_t x0, int32_t y0,
			     int32_t x1, int32_t y1, int32_t x2,
			     int32_t y2, int32_t zIndex, int32_t flag);
uint8_t entityGetTechFromAnim(Entity *entity, int32_t anim);
int32_t hasDigimonRaised(int32_t digimonId);
int32_t hasMedal(uint16_t medal);
int32_t getCardAmount(int32_t cardId);
void activateMedalTexture(int32_t medalId, int32_t previousMedalId);
int32_t loadCardImage(int32_t id);
void renderEvoChartDetail(void);
void renderCardImage(void);
void renderCardCount(void);
int32_t hasMove(int32_t moveId);

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

// clang-format off
int16_t MIST_X_OFFSETS[1] = {
	0xff60,
};

int16_t MAIN_D_80134226 = 0x00a0;

int16_t MAIN_D_80134228 = 0x00a0;

int16_t MAIN_D_8013422A = 0xff60;

int16_t MIST_Y_OFFSETS[1] = {
	0xff88,
};

int16_t MAIN_D_8013422E = 0x0078;

int16_t MIST_CLUT_Y[2] = {
	0x0050, 0x0010,
};

uint8_t EQUIPPED_MOVES[3] = {
	0xff, 0xff, 0xff,
};

uint8_t MAIN_D_80134237 = 0xff;

SVECTOR MAIN_D_80134238 = { 0x0000, 0x0000, 0x0000, 0x0000 };

char MAIN_D_80134240[8] = "Current";

char MAIN_D_80134248[8] = "Ending";

TriangleCursorUVData MAIN_D_80134250 = { { 0x00, 0x04, 0x00, 0x04, 0x04, 0x04, 0x08, 0x08 } };

TriangleCursorUVData MAIN_D_80134258 = { { 0x04, 0x00, 0x04, 0x00, 0x08, 0x08, 0x0c, 0x0c } };

TriangleCursorUVData MAIN_D_80134260 = { { 0xfb, 0xfb, 0xff, 0xff, 0xfb, 0xfb, 0xfb, 0xfb } };

TriangleCursorUVData MAIN_D_80134268 = { { 0xff, 0xff, 0xfb, 0xfb, 0xff, 0xff, 0xff, 0xff } };

TriangleCursorOffsetData MAIN_D_80134270 = { { 0x00, 0x18, 0x00, 0x18, 0x04, 0x04, 0x00, 0x19 } };

TriangleCursorOffsetData MAIN_D_80134278 = { { 0x00, 0x00, 0x16, 0x16, 0x00, 0x16, 0x00, 0x00 } };

TriangleCursorOffsetData MAIN_D_80134280 = { { 0x04, 0x04, 0x04, 0x04, 0x18, 0x18, 0x04, 0x04 } };

TriangleCursorOffsetData MAIN_D_80134288 = { { 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x16, 0x16 } };

RECT MAIN_D_80134290 = { 0, 232, 24, 12 };

char STR_YEAR_DAY[8] = "YearDay";

uint8_t MAIN_D_801342A0[4] = {
	0x01, 0x01, 0x00, 0x00,
};

PlayerTabs MAIN_D_801342A4 = { { 0x01, 0x01, 0x01, 0x01 } };

char MAIN_D_801342A8[8] = "Amateur";

char MAIN_D_801342B0[] = "Novice";

char MAIN_D_801342B8[8] = "Veteran";

char MAIN_D_801342C0[] = "Super";

char MAIN_D_801342C8[] = "Master";

char MAIN_D_801342D0[] = "Genius";

char MAIN_D_801342D8[] = "Legend";

RECT MAIN_D_801342E0 = { 0, 24, 256, 200 };

RECT MOVES_VIEW_TEXT_AREA = { 0, 24, 256, 200 };

RECT MAIN_D_801342F0 = { 0, 24, 256, 200 };

RECT PLAYER_INFO_TEXT_AREA = { 0, 12, 256, 200 };

char FMT_PLAYTIME[8] = "%d : %d";

RECT EVO_CHART_TEXT_AREA = { 0, 12, 256, 200 };

RECT EVO_CHART_NAME_AREA = { 0, 48, 120, 12 };

RECT MEDAL_VIEW_TEXT_AREA = { 0, 12, 256, 200 };

RECT MEDAL_DETAIL_AREA = { 0, 24, 252, 48 };

RECT CARD_VIEW_TEXT_AREA = { 0, 12, 256, 200 };

GsRVIEW2 MAIN_D_80123860 = {
	1300, 0, -3280, 0, 0, 0, 0, NULL,
};

GsRVIEW2 MAIN_D_80123880 = {
	-1050, 220, -10000, -1050, 220, 0, 0, NULL,
};

char MAIN_D_801238A0[] = "Status  Tech";

char MAIN_D_801238B0[] = "      Disc.      Life  Vir. HPMPnight";

char MAIN_D_801238D8[] = "sunup  day      groggysleepy sunset ";

char MAIN_D_80123900[20] = "TechsetFinal  Techs";

char MAIN_D_80123914[12] = {
	0x68, 0x65, 0x6c, 0x70, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x81, 0xa0, 0x00,
};

char MAIN_D_80123920[] = {
	0x81, 0xa2, 0x20, 0x43, 0x68, 0x61, 0x6e, 0x67,
	0x65, 0x20, 0x54, 0x65, 0x63, 0x68, 0x6e, 0x69,
	0x71, 0x75, 0x65, 0x73, 0x00,
};

char MAIN_D_80123938[] = "poisconfstunflat";

char MAIN_D_8012394C[] = {
	0x81, 0x7e, 0x20, 0x43, 0x61, 0x6e, 0x63, 0x65,
	0x6c, 0x00,
};

char MAIN_D_80123958[] = "S      L      W      A";

char MAIN_D_80123970[] = "Select - choose to select tech";

char MAIN_D_80123990[] = {
	0x43, 0x61, 0x6e, 0x63, 0x65, 0x6c, 0x20, 0x2d,
	0x20, 0x81, 0xa2, 0x20, 0x64, 0x65, 0x73, 0x65,
	0x6c, 0x65, 0x63, 0x74, 0x73, 0x20, 0x74, 0x65,
	0x63, 0x68, 0x00,
};

char MAIN_D_801239AC[] = "Technique Select Help";

char MAIN_D_801239C4[] = "Mastered";

char MAIN_D_801239D0[] = "Unmastered";

char MAIN_D_801239DC[] = "Unusable";

char MAIN_D_801239E8[] = "Selected";

char MAIN_D_801239F4[36] = "BattleRecord Wins Losses Percentage";

char MAIN_D_80123A18[] = "Player   Chart    Med. Card Fish";

char MAIN_D_80123A3C[] = {
	0x4d, 0x65, 0x64, 0x61, 0x6c, 0x73, 0x20, 0x43,
	0x68, 0x61, 0x72, 0x74, 0x20, 0x20, 0x20, 0x81,
	0xa2, 0x81, 0x7e, 0x20, 0x53, 0x65, 0x6c, 0x65,
	0x63, 0x74, 0x20, 0x20, 0x43, 0x61, 0x6e, 0x63,
	0x65, 0x6c, 0x00,
};

char MAIN_D_80123A60[] = "NameLevel           Raised";

char MAIN_D_80123A7C[] = "Bits                Time";

char MAIN_D_80123A98[] = "Items          Medals";

char MAIN_D_80123AB0[] = {
	0x43, 0x61, 0x72, 0x64, 0x20, 0x4c, 0x69, 0x73,
	0x74, 0x81, 0xa2, 0x53, 0x65, 0x6c, 0x65, 0x63,
	0x74, 0x20, 0x20, 0x20, 0x81, 0x7e, 0x43, 0x61,
	0x6e, 0x63, 0x65, 0x6c, 0x00,
};

char MAIN_D_80123AD0[20] = {
	0x81, 0xa2, 0x53, 0x65, 0x6c, 0x65, 0x63, 0x74,
	0x20, 0x20, 0x20, 0x81, 0x7e, 0x43, 0x61, 0x6e,
	0x63, 0x65, 0x6c, 0x00,
};

char MAIN_D_80123AE4[] = {
	0x54, 0x72, 0x65, 0x65, 0x20, 0x46, 0x72, 0x65,
	0x73, 0x68, 0x54, 0x72, 0x61, 0x69, 0x6e, 0x69,
	0x6e, 0x67, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x81, 0xa2, 0x53, 0x65, 0x6c, 0x65, 0x63,
	0x74, 0x00,
};

char MAIN_D_80123B10[] = "RookieChampion";

char MAIN_D_80123B20[] = "Ultimate";

char MAIN_D_80123B2C[] = "Grade Cup";

char MAIN_D_80123B38[12] = "Version Cup";

char MAIN_D_80123B44[] = "Type Cup";

char MAIN_D_80123B50[12] = "Special Cup";

char MAIN_D_80123B5C[] = "100 Times";

char MAIN_D_80123B68[] = "Technique Master";

char MAIN_D_80123B7C[] = "Digimon Master";

char MAIN_D_80123B8C[] = "Max Abilities";

char MAIN_D_80123B9C[16] = "Perfect Curling";

char MAIN_D_80123BAC[] = "100 Fish";

char MAIN_D_80123BB8[] = "Town Flourishing";

char MAIN_D_80123BCC[] = "Card Complete";

char MAIN_D_80123BDC[] = "Bits Maxed";

char MAIN_D_80123BE8[] = "10 Years";

char MAIN_D_80123BF4[] = "Cup D C B A S";

char MAIN_D_80123C04[] = "Win in all";

char MAIN_D_80123C10[] = "Win in all VER 1 2 3 4 0";

char MAIN_D_80123C2C[] = "Fire Grapple Thunder Wind";

char MAIN_D_80123C48[] = "Nature Cool Metallic Filth Cup";

char MAIN_D_80123C68[] = "Dino Wing Animal Human Cup";

char MAIN_D_80123C84[] = "Won Championship 100 times";

char MAIN_D_80123CA0[] = "Mastered 56 switch techniques";

char MAIN_D_80123CC0[] = "Raised all 61 Digimon";

char MAIN_D_80123CD8[] = "Maxed all of the Digimons";

char MAIN_D_80123CF4[] = "parameters";

char MAIN_D_80123D00[] = "Got a perfect score in curling";

char MAIN_D_80123D20[16] = "100 fish caught";

char MAIN_D_80123D30[] = "Finished the game";

char MAIN_D_80123D44[] = "Jijimon said the town";

char MAIN_D_80123D5C[] = "is flourishing";

char MAIN_D_80123D6C[28] = "Collected all Digimon Cards";

char MAIN_D_80123D88[] = "Collected 999999 bits";

char MAIN_D_80123DA0[] = "Survived for 300 days!";

uint8_t MAIN_D_80123DB8[24] = {
	0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x09, 0x09,
	0x09, 0x09, 0x0a, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
	0x0b, 0x04, 0x04, 0x04, 0x0a, 0x00, 0x00, 0x00,
};

int32_t MAIN_D_80123DD0[6] = {
	0x00000040, 0x00000020, 0x00000004, 0x00000001,
	0x00000010, 0x00000002,
};

char MAIN_D_80123DE8[] = "Beginner";

char MAIN_D_80123DF4[] = "Intermediate";

char MAIN_D_80123E04[] = "Advanced";

char MAIN_D_80123E10[] = "Top rate";

int16_t MAIN_D_80123E1C[6] = {
	0x7a07, 0x7a47, 0x7a87, 0x7ac7, 0x7b07, 0x0000,
};

uint8_t MAIN_D_80123E28[68] = {
	0x00, 0x00, 0x04, 0x01, 0x00, 0x03, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x01, 0x04, 0x04, 0x03, 0x03, 0x04,
	0x03, 0x06, 0x01, 0x03, 0x01, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01,
	0x02, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
	0x00, 0x00, 0x00, 0x03, 0x01, 0x01, 0x01, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

int16_t MAIN_D_80123E6C[6] = {
	0x7a07, 0x7a47, 0x7a87, 0x7ac7, 0x7b07, 0x0000,
};

char MAIN_D_80123E78[] = "\\CARD\\CARD.ALL";

int16_t MAIN_D_80123E88[6] = {
	0x0010, 0x001c, 0x0001, 0x0c01, 0x1818, 0x3000,
};

uint8_t GAME_MENU_SPRITES[84] = {
	0x14, 0x00, 0x47, 0x00, 0x01, 0x00, 0x00, 0x0c,
	0x14, 0x14, 0x00, 0x00, 0x38, 0x00, 0x47, 0x00,
	0x02, 0x00, 0x00, 0x0d, 0x14, 0x14, 0x28, 0x00,
	0x5b, 0x00, 0x47, 0x00, 0x03, 0x00, 0x00, 0x0c,
	0x14, 0x14, 0x00, 0x14, 0x14, 0x00, 0x6f, 0x00,
	0x04, 0x00, 0x00, 0x0d, 0x14, 0x14, 0x28, 0x14,
	0x38, 0x00, 0x6f, 0x00, 0x05, 0x00, 0x00, 0x0d,
	0x14, 0x14, 0x00, 0x28, 0x5b, 0x00, 0x6f, 0x00,
	0x06, 0x00, 0x00, 0x0d, 0x14, 0x14, 0x28, 0x28,
	0x14, 0x00, 0x1f, 0x00, 0x07, 0x00, 0x00, 0x0d,
	0x14, 0x13, 0x50, 0x00,
};

uint8_t GAME_MENU_LABELS[64] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x13, 0x00, 0x60, 0x00, 0x18, 0x07, 0x50, 0x14,
	0x35, 0x00, 0x60, 0x00, 0x1c, 0x07, 0x50, 0x1b,
	0x5a, 0x00, 0x60, 0x00, 0x1c, 0x07, 0x50, 0x22,
	0x15, 0x00, 0x88, 0x00, 0x14, 0x07, 0x6c, 0x14,
	0x39, 0x00, 0x88, 0x00, 0x14, 0x07, 0x6c, 0x1b,
	0x60, 0x00, 0x88, 0x00, 0x14, 0x07, 0x6c, 0x22,
	0x17, 0x00, 0x38, 0x00, 0x10, 0x07, 0x50, 0x29,
};

int16_t GAME_MENU_LINES[15] = {
	0xffc2, 0xffc5, 0x003e, 0xffc5, 0x0000, 0xffc1, 0xffc6, 0x003f,
	0xffc6, 0x0001, 0xffc2, 0xffc7, 0x003e, 0xffc7, 0x0000,
};

uint8_t MAIN_D_80123F48[12] = {
	0x02, 0x02, 0x02, 0x90, 0xd9, 0xfa, 0x4a, 0x9f,
	0xc5, 0xb4, 0x96, 0x69,
};

int16_t MAIN_D_80123F54[30] = {
	0xffe5, 0xfff1, 0xffe5, 0x0063, 0x0000, 0xffe6, 0xfff0, 0xffe6,
	0x0064, 0x0001, 0xffe5, 0xfff1, 0xffe7, 0x0063, 0x0000, 0xff6e,
	0xffee, 0x0092, 0xffee, 0x0000, 0xff6d, 0xffef, 0x0093, 0xffef,
	0x0001, 0xff6e, 0xfff0, 0x0092, 0xfff0, 0x0000,
};

int16_t TECH_VIEW_LINES1[30] = {
	0xff6e, 0xffeb, 0x0092, 0xffeb, 0x0000, 0xff6d, 0xffec, 0x0093,
	0xffec, 0x0001, 0xff6e, 0xffed, 0x0092, 0xffed, 0x0000, 0xff6e,
	0x0010, 0x0092, 0x0010, 0x0000, 0xff6d, 0x0011, 0x0093, 0x0011,
	0x0001, 0xff6e, 0x0012, 0x0093, 0x0012, 0x0000,
};

int16_t TECH_VIEW_LINES3[60] = {
	0xff76, 0x001a, 0xffad, 0x001a, 0x0000, 0xff76, 0x001b, 0xffad,
	0x001b, 0x0001, 0xff76, 0x001c, 0xffad, 0x001c, 0x0000, 0xff72,
	0x001e, 0xff72, 0x0059, 0x0000, 0xff73, 0x001e, 0xff73, 0x0059,
	0x0001, 0xff74, 0x001e, 0xff74, 0x0059, 0x0000, 0xffae, 0x001e,
	0xffae, 0x0059, 0x0000, 0xffaf, 0x001e, 0xffaf, 0x0059, 0x0001,
	0xffb0, 0x001e, 0xffb0, 0x0059, 0x0000, 0xff76, 0x005b, 0xffad,
	0x005b, 0x0000, 0xff76, 0x005c, 0xffad, 0x005c, 0x0001, 0xff76,
	0x005d, 0xffad, 0x005d, 0x0000,
};

int32_t MAIN_D_80124044[8] = {
	0x001aff72, 0x10780404, 0x001affad, 0x107c0404,
	0x0059ff72, 0x14780404, 0x0059ffad, 0x147c0404,
};

int16_t MAIN_D_80124064[90] = {
	0xff9a, 0x0001, 0xffef, 0x0001, 0x0000, 0xff9a, 0x0002, 0xffef,
	0x0002, 0x0001, 0xff9a, 0x0003, 0xffef, 0x0003, 0x0000, 0xff6f,
	0x0005, 0xff6f, 0x005c, 0x0000, 0xff70, 0x0005, 0xff70, 0x005c,
	0x0001, 0xff71, 0x0005, 0xff71, 0x005c, 0x0000, 0xfff1, 0x0005,
	0xfff1, 0x0015, 0x0000, 0xfff2, 0x0005, 0xfff2, 0x0015, 0x0001,
	0xfff3, 0x0005, 0xfff3, 0x0015, 0x0000, 0xff72, 0x0016, 0xfff0,
	0x0016, 0x0000, 0xff71, 0x0017, 0xfff0, 0x0017, 0x0001, 0xff72,
	0x0018, 0xfff0, 0x0018, 0x0000, 0xffc3, 0x0019, 0xffc3, 0x005c,
	0x0000, 0xffc4, 0x0018, 0xffc4, 0x005c, 0x0001, 0xffc5, 0x0019,
	0xffc5, 0x005c, 0x0000, 0xff73, 0x005d, 0xffc2, 0x005d, 0x0000,
	0xff73, 0x005e, 0xffc2, 0x005e, 0x0001, 0xff73, 0x005f, 0xffc2,
	0x005f, 0x0000,
};

int16_t MAIN_D_80124118[90] = {
	0x0037, 0x0001, 0x008d, 0x0001, 0x0000, 0x0037, 0x0002, 0x008d,
	0x0002, 0x0001, 0x0037, 0x0003, 0x008d, 0x0003, 0x0000, 0x000c,
	0x0005, 0x000c, 0x0015, 0x0000, 0x000d, 0x0005, 0x000d, 0x0015,
	0x0001, 0x000e, 0x0005, 0x000e, 0x0015, 0x0000, 0x008e, 0x0005,
	0x008e, 0x005c, 0x0000, 0x008f, 0x0005, 0x008f, 0x005c, 0x0001,
	0x0090, 0x0005, 0x0090, 0x005c, 0x0000, 0x0010, 0x0016, 0x008e,
	0x0016, 0x0000, 0x000f, 0x0017, 0x008e, 0x0017, 0x0001, 0x0010,
	0x0018, 0x008e, 0x0018, 0x0000, 0x003a, 0x0019, 0x003a, 0x005c,
	0x0000, 0x003b, 0x0018, 0x003b, 0x005c, 0x0001, 0x003c, 0x0019,
	0x003c, 0x005c, 0x0000, 0x003e, 0x005d, 0x008d, 0x005d, 0x0000,
	0x003e, 0x005e, 0x008d, 0x005e, 0x0001, 0x003e, 0x005f, 0x008d,
	0x005f, 0x0000,
};

int32_t MAIN_D_801241CC[26] = {
	0xffb40016, 0x225c0717, 0xffb40062, 0x294c0713,
	0xffb4007a, 0x30000710, 0xffb40044, 0x2274070b,
	0xffc10035, 0x0c780404, 0xffd00035, 0x0c780404,
	0xffe00035, 0x0c780404, 0xffc1005e, 0x0c780404,
	0xffd0005e, 0x0c780404, 0xffe0005e, 0x0c780404,
	0xffc10075, 0x0c780404, 0xffd00075, 0x0c780404,
	0xffe00075, 0x0c780404,
};

int32_t MAIN_D_80124234[24] = {
	0xffbfff8e, 0x18000073, 0xffbf000f, 0x1871006e,
	0xffcdff8e, 0x18df0000, 0xffdeff8e, 0x2400000c,
	0xffdeff9a, 0x240c00be, 0xffecffa6, 0x30540000,
	0x0002ff9a, 0x3c000064, 0x0012ff9a, 0x48000064,
	0x0022ff9a, 0x54000064, 0x0032ff9a, 0x60000064,
	0x0042ff9a, 0x60000064, 0x0052ff9a, 0x6c000064,
};

int32_t MAIN_D_80124294[14] = {
	0xffbf0005, 0x00240c0c, 0xffde0052, 0x00540c0c,
	0xffecffbe, 0x00240c0c, 0x0002ff88, 0x00540c0c,
	0x0012ff88, 0x00240c0c, 0x0032ff88, 0x006c0c0c,
	0x0052ff88, 0x00b40c0c,
};

RECT MAIN_D_801242CC[13] = {
	{ 50, 52, 98, 14 },
	{ 50, 68, 244, 14 },
	{ 50, 84, 26, 14 },
	{ 122, 84, 26, 14 },
	{ 75, 171, 50, 8 },
	{ 75, 186, 50, 8 },
	{ 75, 201, 50, 8 },
	{ 194, 130, 104, 5 },
	{ 194, 145, 104, 5 },
	{ 194, 160, 54, 5 },
	{ 194, 175, 54, 5 },
	{ 194, 190, 54, 5 },
	{ 194, 205, 54, 5 },
};

int32_t MAIN_D_80124334[18] = {
	0x0022ff76, 0x30480030, 0x0040ff82, 0x30180024,
	0x004fff82, 0x30780024, 0x0004fffc, 0x309c0018,
	0x0013fffc, 0x30b40018, 0x0022fffc, 0x24000024,
	0x0031fffc, 0x24240024, 0xffddffaf, 0x30000000,
	0xffddfff6, 0x30000000,
};

int32_t MAIN_D_8012437C[42] = {
	0xffafff74, 0x4b000b35, 0xfff4ff74, 0x56000b35,
	0xfff4ffee, 0x61000b35, 0xffbeff76, 0x30800a1a,
	0xffceff76, 0x309a0a1a, 0xffdeffbd, 0x30b40a1c,
	0xffdeff7b, 0x3a800a13, 0xffc20034, 0x38000819,
	0xffc20009, 0x38190819, 0xffc20055, 0x38320819,
	0x004fff77, 0x25f00b09, 0x0004ffef, 0x3a930b0b,
	0x0013ffef, 0x3a9e0b0b, 0x0022ffef, 0x3aa90b0b,
	0x0031ffef, 0x3ab40b0b, 0x0040ffef, 0x3abf0b0b,
	0x004fffef, 0x3aca0b0a, 0x0031ff82, 0x3ad40c24,
	0x0040fffc, 0x46d40c24, 0x004ffffc, 0x52d40c24,
	0x004fff76, 0x742c0b09,
};

int16_t MAIN_D_80124424[55] = {
	0xff6e, 0xffbf, 0x0092, 0xffbf, 0x0000, 0xff6d, 0xffc0, 0x0093,
	0xffc0, 0x0001, 0xff6e, 0xffc1, 0x0092, 0xffc1, 0x0000, 0xffd3,
	0xffc1, 0x0092, 0xffc1, 0x0000, 0xffd1, 0xffc1, 0xffd1, 0x0062,
	0x0000, 0xffd2, 0xffc0, 0xffd2, 0x0063, 0x0001, 0xffd3, 0xffc1,
	0xffd3, 0x002b, 0x0000, 0xffd2, 0x002c, 0x0091, 0x002c, 0x0000,
	0xffd3, 0x002d, 0x0093, 0x002d, 0x0001, 0xffd3, 0x002e, 0x0091,
	0x002e, 0x0000, 0xffd3, 0x002f, 0xffd3, 0x0064, 0x0000,
};

RECT MAIN_D_80124494[11] = {
	{ 74, 38, 75, 14 },
	{ 211, 64, 27, 14 },
	{ 211, 101, 27, 14 },
	{ 211, 195, 27, 14 },
	{ 198, 82, 99, 14 },
	{ 199, 120, 75, 14 },
	{ 199, 140, 75, 14 },
	{ 211, 174, 18, 18 },
	{ 231, 174, 18, 18 },
	{ 251, 174, 18, 18 },
	{ 271, 174, 18, 18 },
};

int32_t MAIN_D_801244EC[22] = {
	0xffafff84, 0x0c000024, 0xffcaffdb, 0x0c240054,
	0xffefffdb, 0x0c780054, 0x0002ffdb, 0x18000048,
	0x0016ffdb, 0x18600048, 0x0039ffdb, 0x24000054,
	0x004dffdb, 0x24540048, 0xffef0050, 0x0ccc0018,
	0x00020074, 0x18480018, 0x0016004d, 0x18a8000c,
	0x004d0050, 0x249c000c,
};

EvoChartEntry MAIN_D_80124544[61] = {
	{ 0x001e, 0x002b, 0x40, 0x90, 0x00, 0x00 },
	{ 0x0043, 0x002b, 0x60, 0x90, 0x00, 0x00 },
	{ 0x0068, 0x002b, 0x40, 0x80, 0x00, 0x00 },
	{ 0x0068, 0x00b0, 0x60, 0x80, 0x01, 0x00 },
	{ 0x008d, 0x002b, 0x40, 0x30, 0x01, 0x00 },
	{ 0x00d5, 0x003e, 0x60, 0x30, 0x03, 0x00 },
	{ 0x008d, 0x0064, 0x80, 0x30, 0x01, 0x00 },
	{ 0x00bd, 0x003e, 0xa0, 0x30, 0x00, 0x00 },
	{ 0x00a5, 0x002b, 0xc0, 0x30, 0x00, 0x00 },
	{ 0x00d5, 0x0077, 0xe0, 0x30, 0x01, 0x00 },
	{ 0x00bd, 0x009d, 0xa0, 0x70, 0x01, 0x00 },
	{ 0x00fa, 0x002b, 0xc0, 0x00, 0x00, 0x00 },
	{ 0x0112, 0x0077, 0xe0, 0x00, 0x00, 0x00 },
	{ 0x0112, 0x008a, 0x00, 0x10, 0x00, 0x00 },
	{ 0x001e, 0x0077, 0x80, 0x90, 0x00, 0x00 },
	{ 0x0043, 0x0077, 0xa0, 0x90, 0x00, 0x00 },
	{ 0x0068, 0x003e, 0x80, 0x80, 0x02, 0x00 },
	{ 0x0068, 0x0064, 0xa0, 0x80, 0x00, 0x00 },
	{ 0x00d5, 0x0064, 0x00, 0x40, 0x00, 0x00 },
	{ 0x00bd, 0x0051, 0x20, 0x40, 0x00, 0x00 },
	{ 0x00bd, 0x002b, 0x40, 0x40, 0x00, 0x00 },
	{ 0x00d5, 0x008a, 0x60, 0x40, 0x02, 0x00 },
	{ 0x008d, 0x009d, 0x80, 0x40, 0x00, 0x00 },
	{ 0x008d, 0x008a, 0xa0, 0x40, 0x00, 0x00 },
	{ 0x00a5, 0x0077, 0xc0, 0x70, 0x01, 0x00 },
	{ 0x00fa, 0x003e, 0x20, 0x10, 0x00, 0x00 },
	{ 0x00fa, 0x0077, 0x40, 0x10, 0x00, 0x00 },
	{ 0x00fa, 0x009d, 0x60, 0x10, 0x00, 0x00 },
	{ 0x001e, 0x0051, 0xc0, 0x90, 0x02, 0x00 },
	{ 0x0043, 0x0051, 0xe0, 0x90, 0x00, 0x00 },
	{ 0x0068, 0x0051, 0xc0, 0x80, 0x00, 0x00 },
	{ 0x0068, 0x008a, 0xe0, 0x80, 0x00, 0x00 },
	{ 0x00bd, 0x0064, 0xc0, 0x40, 0x00, 0x00 },
	{ 0x008d, 0x0051, 0xe0, 0x40, 0x01, 0x00 },
	{ 0x00a5, 0x008a, 0x00, 0x50, 0x02, 0x00 },
	{ 0x00d5, 0x002b, 0x20, 0x50, 0x00, 0x00 },
	{ 0x00d5, 0x0051, 0x40, 0x50, 0x00, 0x00 },
	{ 0x00a5, 0x003e, 0x60, 0x50, 0x00, 0x00 },
	{ 0x00d5, 0x009d, 0xe0, 0x70, 0x00, 0x00 },
	{ 0x0112, 0x002b, 0x80, 0x10, 0x03, 0x00 },
	{ 0x00fa, 0x0051, 0xa0, 0x10, 0x00, 0x00 },
	{ 0x00fa, 0x00b0, 0xc0, 0x10, 0x00, 0x00 },
	{ 0x001e, 0x009d, 0x00, 0xa0, 0x00, 0x00 },
	{ 0x0043, 0x009d, 0x20, 0xa0, 0x01, 0x00 },
	{ 0x0068, 0x0077, 0x00, 0x90, 0x02, 0x00 },
	{ 0x0068, 0x009d, 0x20, 0x90, 0x02, 0x00 },
	{ 0x008d, 0x003e, 0x80, 0x50, 0x03, 0x00 },
	{ 0x00a5, 0x0051, 0xa0, 0x50, 0x03, 0x00 },
	{ 0x00bd, 0x008a, 0xc0, 0x50, 0x03, 0x00 },
	{ 0x00a5, 0x0064, 0xe0, 0x50, 0x00, 0x00 },
	{ 0x008d, 0x0077, 0x00, 0x60, 0x00, 0x00 },
	{ 0x00a5, 0x009d, 0x20, 0x60, 0x00, 0x00 },
	{ 0x008d, 0x00b0, 0x00, 0x80, 0x00, 0x00 },
	{ 0x0112, 0x003e, 0xe0, 0x10, 0x00, 0x00 },
	{ 0x0112, 0x0064, 0x00, 0x20, 0x00, 0x00 },
	{ 0x0112, 0x009d, 0x20, 0x20, 0x01, 0x00 },
	{ 0x0068, 0x00c3, 0x60, 0x60, 0x00, 0x00 },
	{ 0x00bd, 0x0077, 0x40, 0x60, 0x00, 0x00 },
	{ 0x0112, 0x0051, 0x20, 0x00, 0x00, 0x00 },
	{ 0x00fa, 0x0064, 0x40, 0x00, 0x04, 0x00 },
	{ 0x00fa, 0x008a, 0x60, 0x00, 0x03, 0x00 },
};

int16_t MAIN_D_8012472C[70] = {
	0xff6e, 0xffbd, 0x0092, 0xffbd, 0x0000, 0xff6d, 0xffbe, 0x0093,
	0xffbe, 0x0001, 0xff6e, 0xffbf, 0x0092, 0xffbf, 0x0000, 0x0047,
	0xffbf, 0x0092, 0xffbf, 0x0000, 0x0045, 0xffc0, 0x0045, 0x000c,
	0x0000, 0x0046, 0xffbf, 0x0046, 0x000f, 0x0001, 0x0047, 0xffc0,
	0x0047, 0x000e, 0x0000, 0xff6e, 0x000e, 0x0046, 0x000e, 0x0000,
	0x0047, 0x000e, 0x0092, 0x000e, 0x0000, 0xff6d, 0x000f, 0x0093,
	0x000f, 0x0001, 0xff6e, 0x0010, 0x0092, 0x0010, 0x0000, 0xff6d,
	0x004f, 0x0091, 0x004f, 0x0000, 0xff6c, 0x0050, 0x0092, 0x0050,
	0x0001, 0xff6d, 0x0051, 0x0091, 0x0051, 0x0000,
};

char *STATUS_VIEW_LABELS[8] = {
	MAIN_D_801238A0,
	MAIN_D_801238B0,
	MAIN_D_801238D8,
	MAIN_D_80123900,
	MAIN_D_80123914,
	MAIN_D_80123920,
	MAIN_D_80123938,
	MAIN_D_8012394C,
};

char *TECH_VIEW_LABELS[10] = {
	MAIN_D_80123958,
	MAIN_D_80123970,
	MAIN_D_80123990,
	MAIN_D_801239AC,
	MAIN_D_80134240,
	MAIN_D_801239C4,
	MAIN_D_801239D0,
	MAIN_D_801239DC,
	MAIN_D_801239E8,
	MAIN_D_801239F4,
};

char *PLAYER_VIEW_LABELS[5] = {
	MAIN_D_80123A18,
	MAIN_D_80123A3C,
	MAIN_D_80123A60,
	MAIN_D_80123A7C,
	MAIN_D_80123A98,
};

char *CARD_CHART_LABELS[5] = {
	MAIN_D_80123AB0,
	MAIN_D_80123AD0,
	MAIN_D_80123AE4,
	MAIN_D_80123B10,
	MAIN_D_80123B20,
};

char *MEDAL_NAMES[15] = {
	MAIN_D_80123B2C,
	MAIN_D_80123B38,
	MAIN_D_80123B44,
	MAIN_D_80123B50,
	MAIN_D_80123B5C,
	MAIN_D_80123B68,
	MAIN_D_80123B7C,
	MAIN_D_80123B8C,
	MAIN_D_80123B9C,
	MAIN_D_80123BAC,
	MAIN_D_80134248,
	MAIN_D_80123BB8,
	MAIN_D_80123BCC,
	MAIN_D_80123BDC,
	MAIN_D_80123BE8,
};

char *MEDAL_DESCRIPTIONS[45] = {
	MAIN_D_80123BF4,
	MAIN_D_80123C04,
	&MAIN_D_80134248[7],
	MAIN_D_80123C10,
	&MAIN_D_80134248[7],
	&MAIN_D_80134248[7],
	MAIN_D_80123C2C,
	MAIN_D_80123C48,
	MAIN_D_80123C04,
	&MAIN_D_80134248[7],
	MAIN_D_80123C68,
	MAIN_D_80123C04,
	&MAIN_D_80134248[7],
	MAIN_D_80123C84,
	&MAIN_D_80134248[7],
	&MAIN_D_80134248[7],
	MAIN_D_80123CA0,
	&MAIN_D_80134248[7],
	&MAIN_D_80134248[7],
	MAIN_D_80123CC0,
	&MAIN_D_80134248[7],
	&MAIN_D_80134248[7],
	MAIN_D_80123CD8,
	MAIN_D_80123CF4,
	&MAIN_D_80134248[7],
	MAIN_D_80123D00,
	&MAIN_D_80134248[7],
	&MAIN_D_80134248[7],
	MAIN_D_80123D20,
	&MAIN_D_80134248[7],
	&MAIN_D_80134248[7],
	MAIN_D_80123D30,
	&MAIN_D_80134248[7],
	&MAIN_D_80134248[7],
	MAIN_D_80123D44,
	MAIN_D_80123D5C,
	&MAIN_D_80134248[7],
	MAIN_D_80123D6C,
	&MAIN_D_80134248[7],
	&MAIN_D_80134248[7],
	MAIN_D_80123D88,
	&MAIN_D_80134248[7],
	&MAIN_D_80134248[7],
	MAIN_D_80123DA0,
	&MAIN_D_80134248[7],
};

char *TAMER_LEVEL_TITLES[11] = {
	MAIN_D_801342A8,
	MAIN_D_801342B0,
	MAIN_D_80123DE8,
	MAIN_D_80123DF4,
	MAIN_D_80123E04,
	MAIN_D_801342B8,
	MAIN_D_80123E10,
	MAIN_D_801342C0,
	MAIN_D_801342C8,
	MAIN_D_801342D0,
	MAIN_D_801342D8,
};

int32_t MAIN_D_80124944[16] = {
	0xffbcffac, 0xfff6ffdb, 0xffd0ffd0, 0xffefffef,
	0xffb6ff9a, 0xfff6ffc0, 0xffe8ffe8, 0xfff2fff2,
	0xffb6ff9a, 0xfff6ffc0, 0xffffffff, 0xfff5fff5,
	0xffbcffac, 0xfff6ffdb, 0x00170017, 0xfff8fff8,
};

int16_t MAIN_D_80124984[40] = {
	0xffbe, 0xffcc, 0xfff6, 0xfff6, 0xffc4, 0xffc4, 0xffee, 0xffee,
	0xffac, 0xffc6, 0xffdb, 0xfff6, 0xffdc, 0xffdc, 0xfff1, 0xfff1,
	0xff9a, 0xfff6, 0xfff6, 0xfff6, 0xfff4, 0xfff4, 0xfff4, 0xfff4,
	0xffac, 0xffc6, 0xffdb, 0xfff6, 0x000c, 0x000c, 0xfff7, 0xfff7,
	0xffbe, 0xffcc, 0xfff6, 0xfff6, 0x0024, 0x0024, 0xfffa, 0xfffa,
};

int32_t MAIN_D_801249D4[24] = {
	0x003d0009, 0x00400040, 0xffb8ffec, 0xffb8ffb8,
	0x00240009, 0x00520043, 0xffefffef, 0xffd0ffd0,
	0x003f0009, 0x00640049, 0xfff2fff2, 0xffe8ffe8,
	0x003f0009, 0x00640049, 0xfff5fff5, 0xffffffff,
	0x00240009, 0x00520043, 0xfff8fff8, 0x00170017,
	0x003d0009, 0x00400040, 0x002ffffb, 0x002f002f,
};

int16_t MAIN_D_80124A34[40] = {
	0x0009, 0x0033, 0x0040, 0x0040, 0xffee, 0xffc4, 0xffc4, 0xffc4,
	0x0009, 0x0024, 0x0039, 0x0052, 0xfff1, 0xfff1, 0xffdc, 0xffdc,
	0x0009, 0x0064, 0x0064, 0x0064, 0xfff4, 0xfff4, 0xfff4, 0xfff4,
	0x0009, 0x0024, 0x0039, 0x0052, 0xfff7, 0xfff7, 0x000c, 0x000c,
	0x0009, 0x0033, 0x0040, 0x0040, 0xfffa, 0x0024, 0x0024, 0x0024,
};

RGB8 MAIN_D_80124A84[12] = {
	{ 0xfb, 0xe8, 0x02 },
	{ 0xec, 0x9a, 0x00 },
	{ 0x00, 0xb6, 0x00 },
	{ 0x00, 0x64, 0x00 },
	{ 0x00, 0xae, 0xb8 },
	{ 0x00, 0x6f, 0x8e },
	{ 0x00, 0x3c, 0xc8 },
	{ 0x00, 0x1e, 0x8c },
	{ 0xbe, 0x00, 0xa0 },
	{ 0x6e, 0x00, 0x6e },
	{ 0xbe, 0x00, 0x1e },
	{ 0x82, 0x1e, 0x1e },
};

int32_t MAIN_D_80124AA8[8] = {
	0xffc7ff9a, 0x00000000, 0xffdfff88, 0x00000000,
	0xfff7ff88, 0x00000000, 0x000fff9a, 0x00000000,
};

int16_t MAIN_D_80124AC8[20] = {
	0xffac, 0xffbb, 0x0000, 0x0000, 0xff9a, 0xffd3, 0x0000, 0x0000,
	0xff88, 0xffeb, 0x0000, 0x0000, 0xff9a, 0x0003, 0x0000, 0x0000,
	0xffac, 0x001b, 0x0000, 0x0000,
};

int32_t MAIN_D_80124AF0[12] = {
	0xffaf0042, 0x00000000, 0xffc70054, 0x00000000,
	0xffdf0066, 0x00000000, 0xfff70066, 0x00000000,
	0x00100054, 0x00000000, 0x00270042, 0x00000000,
};

int16_t MAIN_D_80124B20[20] = {
	0x0042, 0xffbb, 0x0000, 0x0000, 0x0054, 0xffd3, 0x0000, 0x0000,
	0x0066, 0xffeb, 0x0000, 0x0000, 0x0054, 0x0003, 0x0000, 0x0000,
	0x0042, 0x001b, 0x0000, 0x0000,
};

int16_t MAIN_D_80124B48[24] = {
	0x001c, 0x002b, 0x004e, 0x0082, 0x0081, 0x0032, 0x003c, 0x0004,
	0x0088, 0x0055, 0x0030, 0x002e, 0x00d6, 0x002a, 0x004e, 0x0082,
	0x0026, 0x00bb, 0x006c, 0x0004, 0x001a, 0x00ce, 0x0084, 0x0004,
};

RGB8 PARTICLE_COLOR3[6] = {
	{ 0x47, 0x51, 0x00 },
	{ 0x60, 0x6b, 0x19 },
	{ 0x7d, 0x86, 0x3d },
	{ 0xb3, 0xbb, 0x7a },
	{ 0xd7, 0xdc, 0xb7 },
	{ 0xff, 0xff, 0xff },
};

RGB8 PARTICLE_COLOR2[18] = {
	{ 0x04, 0x09, 0x03 },
	{ 0x0a, 0x0e, 0x05 },
	{ 0x10, 0x13, 0x06 },
	{ 0x16, 0x18, 0x07 },
	{ 0x1b, 0x1d, 0x09 },
	{ 0x21, 0x23, 0x0a },
	{ 0x27, 0x28, 0x0b },
	{ 0x2d, 0x2d, 0x0d },
	{ 0x33, 0x32, 0x0e },
	{ 0x39, 0x37, 0x0f },
	{ 0x3f, 0x3d, 0x10 },
	{ 0x44, 0x42, 0x12 },
	{ 0x4a, 0x47, 0x13 },
	{ 0x50, 0x4c, 0x14 },
	{ 0x56, 0x51, 0x16 },
	{ 0x5c, 0x57, 0x17 },
	{ 0x62, 0x5c, 0x18 },
	{ 0x68, 0x61, 0x1a },
};

RGB8 PARTICLE_COLOR1[18] = {
	{ 0x02, 0x04, 0x01 },
	{ 0x04, 0x06, 0x02 },
	{ 0x07, 0x09, 0x03 },
	{ 0x0a, 0x0b, 0x03 },
	{ 0x0c, 0x0d, 0x04 },
	{ 0x0f, 0x10, 0x04 },
	{ 0x12, 0x12, 0x05 },
	{ 0x15, 0x15, 0x06 },
	{ 0x17, 0x17, 0x06 },
	{ 0x1a, 0x19, 0x07 },
	{ 0x1d, 0x1c, 0x07 },
	{ 0x1f, 0x1e, 0x08 },
	{ 0x22, 0x21, 0x09 },
	{ 0x25, 0x23, 0x09 },
	{ 0x27, 0x25, 0x0a },
	{ 0x2a, 0x28, 0x0a },
	{ 0x2d, 0x2a, 0x0b },
	{ 0x30, 0x2d, 0x0c },
};

RGB8 UI_BOX_COLORS[5] = {
	{ 0x00, 0x00, 0x00 },
	{ 0x2d, 0x38, 0x40 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
};

char MAIN_D_80124C0C[2][12] = {
	"HP",
	"MP",
};

char MAIN_D_80124C24[12] = "Off";

char MAIN_D_80124C30[3][12] = {
	"Def",
	"Speed",
	"Brain",
};

char MAIN_D_80124C54[] = {
	0x82, 0x4f, 0x82, 0x50, 0x82, 0x51, 0x82, 0x52,
	0x82, 0x53, 0x82, 0x54, 0x82, 0x55, 0x82, 0x56,
	0x82, 0x57, 0x82, 0x58, 0x00,
};

char MAIN_D_80124C6C[] = "Fire Tower";

char MAIN_D_80124C78[16] = "Prominence Beam";

char MAIN_D_80124C88[] = "Spit Fire";

char MAIN_D_80124C94[12] = "Red Inferno";

char MAIN_D_80124CA0[] = "Magma Bomb";

char MAIN_D_80124CAC[] = "Heat Laser";

char MAIN_D_80124CB8[] = "Inifinity Burn";

char MAIN_D_80124CC8[] = "Meltdown";

char MAIN_D_80124CD4[16] = "Thunder Justice";

char MAIN_D_80124CE4[] = "Spinning Shot";

char MAIN_D_80124CF4[] = "Electric Cloud";

char MAIN_D_80124D04[] = "Megalo Spark";

char MAIN_D_80124D14[] = "Static Elect";

char MAIN_D_80124D24[12] = "Wind Cutter";

char MAIN_D_80124D30[] = "Confused Storm";

char MAIN_D_80124D40[] = "Hurricane";

char MAIN_D_80124D4C[12] = "Giga Freeze";

char MAIN_D_80124D58[] = "Ice Statue";

char MAIN_D_80124D64[] = "Winter Blast";

char MAIN_D_80124D74[] = "Ice Needle";

char MAIN_D_80124D80[] = "Water Blit";

char MAIN_D_80124D8C[] = "Aqua Magic";

char MAIN_D_80124D98[] = "Aurora Freeze";

char MAIN_D_80124DA8[] = "Tear Drop";

char MAIN_D_80124DB4[12] = "Power Crane";

char MAIN_D_80124DC0[] = "All Range Beam";

char MAIN_D_80124DD0[] = "Metal Sprinter";

char MAIN_D_80124DE0[12] = "Pulse Laser";

char MAIN_D_80124DEC[] = "Delete Program";

char MAIN_D_80124DFC[] = "DG Dimension";

char MAIN_D_80124E0C[] = "Full Potential";

char MAIN_D_80124E1C[] = "Reverse Prog";

char MAIN_D_80124E2C[] = "Poison Powder";

char MAIN_D_80124E3C[] = "Mass Morph";

char MAIN_D_80124E48[] = "Insect Plague";

char MAIN_D_80124E58[] = "Charm Perfume";

char MAIN_D_80124E68[12] = "Poison Claw";

char MAIN_D_80124E74[] = "Danger Sting";

char MAIN_D_80124E84[] = "Green Trap";

char MAIN_D_80124E90[] = "Muscle Charge";

char MAIN_D_80124EA0[] = "Sonic Jab";

char MAIN_D_80124EAC[] = "Dynamite Kick";

char MAIN_D_80124EBC[] = "Megaton Punch";

char MAIN_D_80124ECC[12] = "Buster Dive";

char MAIN_D_80124ED8[] = "Odor Spray";

char MAIN_D_80124EE4[] = "Poop Spd Toss";

char MAIN_D_80124EF4[] = "Big Poop Toss";

char MAIN_D_80124F04[] = "Big Rnd Toss";

char MAIN_D_80124F14[] = "Poop Rnd Toss";

char MAIN_D_80124F24[] = "Rnd Spd Toss";

char MAIN_D_80124F34[16] = "Horizontal Kick";

char MAIN_D_80124F44[] = "Ult Poop Hell";

char MAIN_D_80124F54[12] = "Blaze Blast";

char MAIN_D_80124F60[] = "Pepper Breath";

char MAIN_D_80124F70[] = "Lovely Attack";

char MAIN_D_80124F80[] = "Fireball";

char MAIN_D_80124F8C[] = "Death Claw";

char MAIN_D_80124F98[] = "Mega Flame";

char MAIN_D_80124FA4[16] = "Howling Blaster";

char MAIN_D_80124FB4[] = "Party time";

char MAIN_D_80124FC0[] = "Electric Shock";

char MAIN_D_80124FD0[] = "Abduction Beam";

char MAIN_D_80124FE0[12] = "Smiley Bomb";

char MAIN_D_80124FEC[] = "Spnning Needle";

char MAIN_D_80124FFC[] = "Spiral Twister";

char MAIN_D_8012500C[12] = "Boom Bubble";

char MAIN_D_80125018[] = "Sweet Breath";

char MAIN_D_80125028[] = "Bit Bomb";

char MAIN_D_80125034[12] = "Deadly Bomb";

char MAIN_D_80125040[] = "Drill Spin";

char MAIN_D_8012504C[16] = "Electric Thread";

char MAIN_D_8012505C[12] = "Energy Bomb";

char MAIN_D_80125068[16] = "Genoside Attack";

char MAIN_D_80125078[] = "Giga Scissor Claw";

char MAIN_D_8012508C[] = "Dark Shot";

char MAIN_D_80125098[] = "Pummel Whack";

char MAIN_D_801250A8[] = "Hand of Fate";

char MAIN_D_801250B8[] = "Dark Claw";

char MAIN_D_801250C4[] = "Aerial Attack";

char MAIN_D_801250D4[] = "Bone Boomerang";

char MAIN_D_801250E4[] = "Solar Ray";

char MAIN_D_801250F0[] = "Hydro Pressure";

char MAIN_D_80125100[] = "Ice Blast";

char MAIN_D_8012510C[] = "Iga School Knife Throw";

char MAIN_D_80125124[] = "Blasting Spout";

char MAIN_D_80125134[] = "Fist of the Beast King";

char MAIN_D_8012514C[] = "Dark Network & Concert Crush";

char MAIN_D_8012516C[16] = "Electro Shocker";

char MAIN_D_8012517C[12] = "Meteor Wing";

char MAIN_D_80125188[] = "Super Slap";

char MAIN_D_80125194[20] = "Nightmare Syndromer";

char MAIN_D_801251A8[] = "Frozen Fire Shot";

char MAIN_D_801251BC[] = "Poison Ivy";

char MAIN_D_801251C8[] = "Blue Blaster";

char MAIN_D_801251D8[] = "Scissor Claw";

char MAIN_D_801251E8[] = "Super Thunder Strike";

char MAIN_D_80125200[] = "Spiral Sword";

char MAIN_D_80125210[] = "Variable Darts";

char MAIN_D_80125220[16] = "Volcanic Strike";

char MAIN_D_80125230[] = "Subzero Ice Punch";

char MAIN_D_80125244[16] = "Infinity Cannon";

char MAIN_D_80125254[] = "Crimson Flare";

char MAIN_D_80125264[] = "Glacial Blast";

char MAIN_D_80125274[12] = "Mail Strome";

char MAIN_D_80125280[] = "High Electro Shocker";

char MAIN_D_80125298[24] = "Small Recovery: +500 HP";

char MAIN_D_801252B0[] = "Medium Recovery: +1500 HP";

char MAIN_D_801252CC[] = "Large Recovery: +5000 HP";

char MAIN_D_801252E8[24] = "Super Recovery: full HP";

char MAIN_D_80125300[] = "Recover +500 Magic Points";

char MAIN_D_8012531C[] = "Med. MP: recover +1500 MP";

char MAIN_D_80125338[] = "Lrg. MP: recover +5000 MP";

char MAIN_D_80125354[] = "Recovers +1500 MP and HP";

char MAIN_D_80125370[20] = "Cures Status Errors";

char MAIN_D_80125384[] = "Cures errors + rec. HP+MP";

char MAIN_D_801253A0[28] = "Protects yr cond. in battle";

char MAIN_D_801253BC[] = "Cures Coma + rec. half HP";

char MAIN_D_801253D8[28] = "Cures coma, errors +full HP";

char MAIN_D_801253F4[] = "Cures wounds + some sickness";

char MAIN_D_80125414[24] = "Cures wounds + sickness";

char MAIN_D_8012542C[] = "Boost Off. Power in battle";

char MAIN_D_80125448[] = "Boost Def. Power in battle";

char MAIN_D_80125464[] = "Boost Speed in battle";

char MAIN_D_8012547C[] = "Boost all skills in battle";

char MAIN_D_80125498[] = "Super boost off. pwr in bat.";

char MAIN_D_801254B8[] = "Super boost def. pwr in bat.";

char MAIN_D_801254D8[28] = "Super boost Speed in battle";

char MAIN_D_801254F4[] = "Can return to city quickly";

char MAIN_D_80125510[] = "Boost max off. pwr level +50";

char MAIN_D_80125530[] = "Boost max def. pwr level +50";

char MAIN_D_80125550[] = "Boost max Brains level +50";

char MAIN_D_8012556C[] = "Boost max Speed level +50";

char MAIN_D_80125588[24] = "Boost max HP level +500";

char MAIN_D_801255A0[24] = "Boost max MP level +500";

char MAIN_D_801255B8[] = "Boost Off. Pwr+Brains +100";

char MAIN_D_801255D4[] = "Boost Def. Pwr+Speed +100";

char MAIN_D_801255F0[] = "Boost Off. Pwr+Speed +1000";

char MAIN_D_8012560C[] = "Can do potty anywhere";

char MAIN_D_80125624[] = "Train better with this";

char MAIN_D_8012563C[] = "More recovery during rest";

char MAIN_D_80125658[28] = "Repels enemies to stay away";

char MAIN_D_80125674[24] = "Attract enemies to come";

char MAIN_D_8012568C[] = "Walk and HP + MP go up";

char MAIN_D_801256A4[] = "Makes Digimon a bit full";

char MAIN_D_801256C0[] = "Makes Digimon quite full .";

char MAIN_D_801256DC[] = "Makes Digimon very full.";

char MAIN_D_801256F8[] = "Boosts training effect";

char MAIN_D_80125710[] = "Greatly reduces Tiredness";

char MAIN_D_8012572C[24] = "Make Digimon a bit full";

char MAIN_D_80125744[] = "Greatly boosts Discipline";

char MAIN_D_80125760[] = "Boosts all abilities";

char MAIN_D_80125778[20] = "Makes Digimon happy";

char MAIN_D_8012578C[28] = "Gives rest, boost disc.+hap";

char MAIN_D_801257A8[] = "Can be sold for a high price";

char MAIN_D_801257C8[28] = "Makes full + boosts Weight!";

char MAIN_D_801257E4[24] = "Recovers HP completely!";

char MAIN_D_801257FC[24] = "Recovers MP completely!";

char MAIN_D_80125814[] = "Lowers Weight!";

char MAIN_D_80125824[] = "Fully recovers HP and MP";

char MAIN_D_80125840[] = "Boost Offensive Power +20!";

char MAIN_D_8012585C[] = "Boost Defensive Power +20!";

char MAIN_D_80125878[] = "Boost Speed +20!";

char MAIN_D_8012588C[] = "Boost Brains +20!";

char MAIN_D_801258A0[] = "Boost HP by +200!";

char MAIN_D_801258B4[] = "Boost MP by +200!";

char MAIN_D_801258C8[] = "Makes Digimon a bit full.";

char MAIN_D_801258E4[] = "Makes Digimon quite full.";

char MAIN_D_80125900[24] = "Makes Digimon very full";

char MAIN_D_80125918[] = "Full HP and MP + life span++";

char MAIN_D_80125938[28] = "Makes Digimon somewhat full";

char MAIN_D_80125954[] = "Boost Happiness, but risky";

char MAIN_D_80125970[] = "Good for many things";

char MAIN_D_80125988[] = "Digivolve to Greymon!";

char MAIN_D_801259A0[] = "Digivolve to Meramon!";

char MAIN_D_801259B8[24] = "Digivolve to Birdramon!";

char MAIN_D_801259D0[] = "Digivolve to Centarumon!";

char MAIN_D_801259EC[] = "Digivolve to Monochromon!";

char MAIN_D_80125A08[] = "Digivolve to Drimogemon!";

char MAIN_D_80125A24[] = "Digivolve to Tyrannomon!";

char MAIN_D_80125A40[] = "Digivolve to Devimon!";

char MAIN_D_80125A58[] = "Digivolve to Ogremon!";

char MAIN_D_80125A70[] = "Digivolve to Leomon!";

char MAIN_D_80125A88[] = "Digivolve to Angemon!";

char MAIN_D_80125AA0[] = "Digivolve to Bakemon!";

char MAIN_D_80125AB8[] = "Digivolve to Kaminarimon!";

char MAIN_D_80125AD4[24] = "Digivolve to Airdramon!";

char MAIN_D_80125AEC[] = "Digivolve to Kokatorimon";

char MAIN_D_80125B08[] = "Digivolve to Unimon!";

char MAIN_D_80125B20[] = "Digivolve to Kabuterimon!";

char MAIN_D_80125B3C[24] = "Digivolve to Kuwagamon!";

char MAIN_D_80125B54[] = "Digivolve to Vegiemon!";

char MAIN_D_80125B6C[] = "Digivolve to Ninjamon!";

char MAIN_D_80125B84[24] = "Digivolve to Seadramon!";

char MAIN_D_80125B9C[] = "Digivolve to Whamon!";

char MAIN_D_80125BB4[] = "Digivolve to Shellmon!";

char MAIN_D_80125BCC[] = "Digivolve to Coelamon!";

char MAIN_D_80125BE4[24] = "Digivolve to Garurumon!";

char MAIN_D_80125BFC[] = "Digivolve to Frigimon!";

char MAIN_D_80125C14[] = "Digivolve to Mojyamon!";

char MAIN_D_80125C2C[] = "Digivolve to Nanimon!";

char MAIN_D_80125C44[] = "Digivolve to MetalGreymon!";

char MAIN_D_80125C60[] = "Digivolve to SkullGreymon!";

char MAIN_D_80125C7C[] = "Digivolve to Andromon!";

char MAIN_D_80125C94[] = "Digivolve to Megadramon!";

char MAIN_D_80125CB0[] = "Digivolve to Mamemon!";

char MAIN_D_80125CC8[] = "Digivolve to MetalMamemon!";

char MAIN_D_80125CE4[] = "Digivolve to Giromon!";

char MAIN_D_80125CFC[] = "Digivolve to Piximon!";

char MAIN_D_80125D14[24] = "Digivolve to Monzaemon!";

char MAIN_D_80125D2C[] = "Digivolve to Vademon!";

char MAIN_D_80125D44[] = "Digivolve to Etemon!";

char MAIN_D_80125D5C[] = "Digivolve to Digitamamon!";

char MAIN_D_80125D78[] = "Digivolve to Phoenixmon!";

char MAIN_D_80125D94[28] = "Become HerculesKabuterimon!";

char MAIN_D_80125DB0[28] = "Digivolve to MegaSeadramon!";

char MAIN_D_80125DCC[28] = "Digivolve to WereGarurumon!";

char MAIN_D_80125DE8[] = "Seadramon friendship proof";

char MAIN_D_80125E04[28] = "Enables you to fish at lake";

char MAIN_D_80125E20[] = "Gives good fishing at lake";

char MAIN_D_80125E3C[] = "Stone Tablet of Leomon";

char MAIN_D_80125E54[] = "Key to Gray Lord Mansion";

char MAIN_D_80125E70[] = "Mystery Item";

char MAIN_D_80125E80[28] = "Recover 1000 MP +other uses";

char MAIN_D_80125E9C[] = "Key to open Refrigerator";

char MAIN_D_80125EB8[28] = "You can read Ancient Script";

char MAIN_D_80125ED4[] = "Digivolve to Gigadramon!";

char MAIN_D_80125EF0[24] = "Digivolve to Panjyamon!";

char MAIN_D_80125F08[] = "Digivolve to MetalEtemon!";

InventoryTable DEFAULT_ITEM_AMOUNTS = {
	{
		0x28, 0x19, 0x32, 0x1e, 0x14, 0x1e, 0x1e, 0x12,
		0x1e, 0x1e, 0x1e, 0x14, 0x1e, 0x14, 0x1e, 0x32,
		0x32, 0x50, 0x32, 0x32, 0x62, 0x32, 0x32, 0x32,
		0x32, 0x32, 0x32, 0x32, 0x32, 0x00,
	},
};

InventoryTable DEFAULT_ITEM_TYPES = {
	{
		0x02, 0x09, 0x0c, 0x13, 0x14, 0x15, 0x30, 0x2b,
		0x0e, 0x4b, 0x4f, 0x50, 0x55, 0x57, 0x58, 0x5b,
		0x5c, 0x5d, 0x61, 0x64, 0x6a, 0x6c, 0x6e, 0x6f,
		0x70, 0x71, 0x77, 0x20, 0x16, 0xff,
	},
};

char IS_SICK_SUFFIX[] = " is sick!";

uint8_t MAIN_D_80125F70[7][7] = {
	{ 0x0a, 0x0f, 0x05, 0x14, 0x14, 0x0f, 0x14 },
	{ 0x0a, 0x0a, 0x02, 0x0f, 0x0a, 0x0a, 0x05 },
	{ 0x0f, 0x0f, 0x0f, 0x05, 0x0f, 0x0f, 0x05 },
	{ 0x05, 0x0f, 0x14, 0x0a, 0x0a, 0x05, 0x0f },
	{ 0x14, 0x0f, 0x05, 0x0f, 0x0a, 0x0f, 0x14 },
	{ 0x0a, 0x0a, 0x05, 0x0a, 0x0a, 0x0a, 0x05 },
	{ 0x05, 0x0f, 0x05, 0x02, 0x0a, 0x14, 0x02 },
};

uint8_t MOVE_LEARN_CHANCES[58][3] = {
	{ 0x19, 0x10, 0x0b }, { 0x11, 0x0a, 0x05 }, { 0x1e, 0x16, 0x0f }, { 0x14, 0x0c, 0x07 },
	{ 0x16, 0x0e, 0x09 }, { 0x1c, 0x13, 0x0d }, { 0x0f, 0x08, 0x00 }, { 0x0e, 0x06, 0x00 },
	{ 0x0d, 0x09, 0x00 }, { 0x16, 0x0e, 0x0a }, { 0x20, 0x13, 0x0f }, { 0x12, 0x0d, 0x08 },
	{ 0x24, 0x15, 0x11 }, { 0x1a, 0x10, 0x0d }, { 0x0f, 0x0b, 0x07 }, { 0x0c, 0x08, 0x00 },
	{ 0x11, 0x0a, 0x05 }, { 0x0f, 0x08, 0x00 }, { 0x14, 0x0c, 0x07 }, { 0x1e, 0x0f, 0x08 },
	{ 0x14, 0x0a, 0x05 }, { 0x16, 0x0e, 0x09 }, { 0x0e, 0x06, 0x00 }, { 0x1e, 0x16, 0x0f },
	{ 0x28, 0x1e, 0x16 }, { 0x10, 0x0d, 0x00 }, { 0x23, 0x1b, 0x12 }, { 0x1c, 0x15, 0x0d },
	{ 0x14, 0x0e, 0x0a }, { 0x0f, 0x0c, 0x00 }, { 0x19, 0x11, 0x0b }, { 0x20, 0x18, 0x0f },
	{ 0x1a, 0x13, 0x0e }, { 0x0c, 0x08, 0x00 }, { 0x17, 0x0f, 0x0c }, { 0x18, 0x10, 0x0d },
	{ 0x12, 0x0c, 0x09 }, { 0x1c, 0x16, 0x10 }, { 0x1b, 0x14, 0x0f }, { 0x0e, 0x0a, 0x00 },
	{ 0x12, 0x08, 0x00 }, { 0x13, 0x09, 0x08 }, { 0x16, 0x0f, 0x0a }, { 0x1a, 0x13, 0x0e },
	{ 0x18, 0x11, 0x0c }, { 0x14, 0x0b, 0x08 }, { 0x15, 0x0d, 0x09 }, { 0x10, 0x07, 0x00 },
	{ 0x18, 0x11, 0x0c }, { 0x18, 0x0e, 0x09 }, { 0x17, 0x0d, 0x08 }, { 0x0f, 0x0a, 0x05 },
	{ 0x0b, 0x08, 0x00 }, { 0x15, 0x0c, 0x07 }, { 0x14, 0x0b, 0x06 }, { 0x19, 0x10, 0x0a },
	{ 0x09, 0x07, 0x00 }, { 0x19, 0x10, 0x0a },
};

char *MOVE_NAMES[122] = {
	MAIN_D_80124C6C,
	MAIN_D_80124C78,
	MAIN_D_80124C88,
	MAIN_D_80124C94,
	MAIN_D_80124CA0,
	MAIN_D_80124CAC,
	MAIN_D_80124CB8,
	MAIN_D_80124CC8,
	MAIN_D_80124CD4,
	MAIN_D_80124CE4,
	MAIN_D_80124CF4,
	MAIN_D_80124D04,
	MAIN_D_80124D14,
	MAIN_D_80124D24,
	MAIN_D_80124D30,
	MAIN_D_80124D40,
	MAIN_D_80124D4C,
	MAIN_D_80124D58,
	MAIN_D_80124D64,
	MAIN_D_80124D74,
	MAIN_D_80124D80,
	MAIN_D_80124D8C,
	MAIN_D_80124D98,
	MAIN_D_80124DA8,
	MAIN_D_80124DB4,
	MAIN_D_80124DC0,
	MAIN_D_80124DD0,
	MAIN_D_80124DE0,
	MAIN_D_80124DEC,
	MAIN_D_80124DFC,
	MAIN_D_80124E0C,
	MAIN_D_80124E1C,
	MAIN_D_80124E2C,
	MAIN_D_80134338,
	MAIN_D_80124E3C,
	MAIN_D_80124E48,
	MAIN_D_80124E58,
	MAIN_D_80124E68,
	MAIN_D_80124E74,
	MAIN_D_80124E84,
	MAIN_D_8013433C,
	MAIN_D_80124E90,
	MAIN_D_80134344,
	MAIN_D_80124EA0,
	MAIN_D_80124EAC,
	MAIN_D_8013434C,
	MAIN_D_80124EBC,
	MAIN_D_80124ECC,
	MAIN_D_80124EAC,
	MAIN_D_80124ED8,
	MAIN_D_80124EE4,
	MAIN_D_80124EF4,
	MAIN_D_80124F04,
	MAIN_D_80124F14,
	MAIN_D_80124F24,
	MAIN_D_80124F34,
	MAIN_D_80124F44,
	MAIN_D_80124F34,
	MAIN_D_80124F54,
	MAIN_D_80124F60,
	MAIN_D_80124F70,
	MAIN_D_80124F80,
	MAIN_D_80124F8C,
	MAIN_D_80124F98,
	MAIN_D_80124FA4,
	MAIN_D_80124FB4,
	MAIN_D_80124FC0,
	MAIN_D_80124FD0,
	MAIN_D_80124FE0,
	MAIN_D_80124FEC,
	MAIN_D_80124FFC,
	MAIN_D_8012500C,
	MAIN_D_80125018,
	MAIN_D_80125028,
	MAIN_D_80125034,
	MAIN_D_80125040,
	MAIN_D_8012504C,
	MAIN_D_8012505C,
	MAIN_D_80125068,
	MAIN_D_80125078,
	MAIN_D_8012508C,
	MAIN_D_80125098,
	MAIN_D_801250A8,
	MAIN_D_801250B8,
	MAIN_D_801250C4,
	MAIN_D_801250D4,
	MAIN_D_801250E4,
	MAIN_D_801250F0,
	MAIN_D_80125100,
	MAIN_D_8012510C,
	MAIN_D_80125124,
	MAIN_D_80125134,
	MAIN_D_8012514C,
	MAIN_D_8012516C,
	MAIN_D_8012517C,
	MAIN_D_80125188,
	MAIN_D_80125194,
	MAIN_D_801251A8,
	MAIN_D_801251BC,
	MAIN_D_801251C8,
	MAIN_D_801251D8,
	MAIN_D_801251E8,
	MAIN_D_80125200,
	MAIN_D_80125210,
	MAIN_D_80125220,
	MAIN_D_80125230,
	MAIN_D_80125244,
	MAIN_D_80124FB4,
	MAIN_D_80124FB4,
	MAIN_D_80125254,
	MAIN_D_80125264,
	MAIN_D_80125274,
	MAIN_D_80125280,
	MAIN_D_80134354,
	MAIN_D_80134354,
	MAIN_D_80134354,
	MAIN_D_80134354,
	MAIN_D_80134354,
	MAIN_D_80134354,
	MAIN_D_80134354,
	MAIN_D_80134354,
	NULL,
};

Move MOVE_DATA[122] = {
	{
		0x00015f90, 0x009b, 0x1b, 0x1a, 0x02,
		0x00, 0x03, 0x37, 0x08, 0x01,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x01bc, 0x3d, 0x01, 0x02,
		0x00, 0x04, 0x4b, 0x03, 0x00,
		0x00, 0x00,
	},
	{
		0x00027100, 0x0042, 0x0a, 0x01, 0x02,
		0x00, 0x00, 0x2b, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00d2, 0x39, 0x01, 0x03,
		0x00, 0x00, 0x48, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x0117, 0x2c, 0x01, 0x02,
		0x00, 0x02, 0x32, 0x04, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x0054, 0x23, 0x01, 0x03,
		0x00, 0x04, 0x50, 0x20, 0x02,
		0x00, 0x00,
	},
	{
		0x00027100, 0x01e8, 0x58, 0x1a, 0x03,
		0x00, 0x03, 0x4f, 0x0b, 0x03,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x0190, 0x6a, 0x01, 0x03,
		0x00, 0x03, 0x56, 0x18, 0x02,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x024a, 0x6e, 0x01, 0x02,
		0x02, 0x03, 0x55, 0x20, 0x01,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x0185, 0x32, 0x01, 0x02,
		0x02, 0x00, 0x4d, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x000c5c10, 0x0078, 0x17, 0x01, 0x02,
		0x02, 0x03, 0x3c, 0x09, 0x01,
		0x00, 0x00,
	},
	{
		0x00027100, 0x017e, 0x3a, 0x01, 0x02,
		0x02, 0x03, 0x44, 0x17, 0x00,
		0x00, 0x00,
	},
	{
		0x00000000, 0x0055, 0x0f, 0x01, 0x01,
		0x02, 0x03, 0x37, 0x0f, 0x00,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x00b2, 0x1f, 0x01, 0x02,
		0x02, 0x00, 0x3e, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00027100, 0x00e1, 0x48, 0x01, 0x03,
		0x02, 0x02, 0x57, 0x3c, 0x03,
		0x00, 0x00,
	},
	{
		0x00027100, 0x016e, 0x55, 0x01, 0x03,
		0x02, 0x02, 0x49, 0x0d, 0x02,
		0x00, 0x00,
	},
	{
		0x00009c40, 0x0108, 0x28, 0x05, 0x02,
		0x04, 0x03, 0x41, 0x0a, 0x00,
		0x00, 0x00,
	},
	{
		0x00077a10, 0x01a8, 0x3e, 0x11, 0x02,
		0x04, 0x03, 0x4f, 0x26, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x0078, 0x37, 0x01, 0x03,
		0x04, 0x03, 0x5a, 0x0f, 0x02,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x007e, 0x1a, 0x01, 0x02,
		0x04, 0x03, 0x32, 0x05, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00d3, 0x22, 0x01, 0x02,
		0x04, 0x00, 0x4a, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00000000, 0x0000, 0x0c, 0x01, 0x04,
		0x04, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x01ae, 0x56, 0x01, 0x03,
		0x04, 0x04, 0x51, 0x18, 0x02,
		0x00, 0x00,
	},
	{
		0x00027100, 0x003c, 0x0e, 0x01, 0x02,
		0x04, 0x04, 0x2c, 0x2e, 0x01,
		0x00, 0x00,
	},
	{
		0x00077a10, 0x00e2, 0x2a, 0x01, 0x02,
		0x05, 0x00, 0x35, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x023d, 0x6e, 0x01, 0x03,
		0x05, 0x00, 0x51, 0x00, 0x02,
		0x00, 0x00,
	},
	{
		0x0003d090, 0x0096, 0x37, 0x01, 0x03,
		0x05, 0x00, 0x4e, 0x00, 0x02,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x0185, 0x38, 0x01, 0x02,
		0x05, 0x00, 0x48, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x01ae, 0x49, 0x01, 0x02,
		0x05, 0x04, 0x41, 0x32, 0x01,
		0x00, 0x00,
	},
	{
		0x00027100, 0x02d2, 0x8c, 0x01, 0x03,
		0x05, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00000000, 0x0000, 0x21, 0x01, 0x04,
		0x05, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00077a10, 0x0100, 0x63, 0x01, 0x02,
		0x05, 0x04, 0x64, 0x0c, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x0075, 0x39, 0x01, 0x03,
		0x03, 0x01, 0x4d, 0x3e, 0x02,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x01f4, 0x76, 0x01, 0x02,
		0x03, 0x04, 0x50, 0x23, 0x01,
		0x00, 0x00,
	},
	{
		0x00000000, 0x0000, 0x0a, 0x01, 0x04,
		0x03, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x003a, 0x20, 0x0c, 0x02,
		0x03, 0x01, 0x53, 0x5a, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00b4, 0x46, 0x01, 0x03,
		0x03, 0x02, 0x4b, 0x39, 0x02,
		0x00, 0x00,
	},
	{
		0x00000000, 0x003e, 0x11, 0x01, 0x01,
		0x03, 0x01, 0x3c, 0x28, 0x00,
		0x00, 0x00,
	},
	{
		0x00000000, 0x009d, 0x22, 0x01, 0x01,
		0x03, 0x04, 0x46, 0x1a, 0x00,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x0136, 0x31, 0x01, 0x02,
		0x03, 0x03, 0x52, 0x37, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00b2, 0x38, 0x01, 0x03,
		0x01, 0x00, 0x3c, 0x00, 0x02,
		0x00, 0x00,
	},
	{
		0x00000000, 0x0000, 0x16, 0x01, 0x04,
		0x01, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00000000, 0x0000, 0x0e, 0x01, 0x04,
		0x01, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00000000, 0x0034, 0x06, 0x01, 0x01,
		0x01, 0x00, 0x35, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00000000, 0x00c1, 0x21, 0x01, 0x01,
		0x01, 0x03, 0x3e, 0x03, 0x00,
		0x00, 0x00,
	},
	{
		0x00000000, 0x011d, 0x37, 0x01, 0x01,
		0x01, 0x02, 0x64, 0x0a, 0x00,
		0x00, 0x00,
	},
	{
		0x00000000, 0x0140, 0x3e, 0x01, 0x01,
		0x01, 0x03, 0x46, 0x09, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x01f4, 0x56, 0x01, 0x02,
		0x01, 0x02, 0x50, 0x0e, 0x00,
		0x00, 0x00,
	},
	{
		0x00000000, 0x00ff, 0x21, 0x01, 0x01,
		0x01, 0x00, 0x5a, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00002710, 0x0058, 0x19, 0x01, 0x02,
		0x06, 0x03, 0x25, 0x0a, 0x00,
		0x00, 0x00,
	},
	{
		0x00077a10, 0x007a, 0x20, 0x01, 0x02,
		0x06, 0x01, 0x28, 0x0b, 0x00,
		0x00, 0x00,
	},
	{
		0x00027100, 0x00d3, 0x40, 0x01, 0x02,
		0x06, 0x02, 0x31, 0x11, 0x01,
		0x00, 0x00,
	},
	{
		0x00027100, 0x00d3, 0x5e, 0x01, 0x03,
		0x06, 0x02, 0x2f, 0x14, 0x02,
		0x00, 0x00,
	},
	{
		0x00027100, 0x004b, 0x28, 0x01, 0x03,
		0x06, 0x01, 0x21, 0x09, 0x02,
		0x00, 0x00,
	},
	{
		0x0003d090, 0x007a, 0x48, 0x01, 0x03,
		0x06, 0x01, 0x32, 0x0a, 0x02,
		0x00, 0x00,
	},
	{
		0x00000000, 0x0035, 0x08, 0x01, 0x01,
		0x06, 0x00, 0x29, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x014d, 0x6f, 0x01, 0x03,
		0x06, 0x04, 0x64, 0x53, 0x02,
		0x00, 0x00,
	},
	{
		0x00000000, 0x0035, 0x08, 0x01, 0x01,
		0x06, 0x00, 0x29, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00015f90, 0x00ae, 0x28, 0x0f, 0x02,
		0x00, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00027100, 0x0059, 0x28, 0x01, 0x02,
		0x00, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00e6, 0x28, 0x01, 0x02,
		0x01, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x009b, 0x28, 0x01, 0x02,
		0x00, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00000000, 0x00b4, 0x28, 0x01, 0x01,
		0x01, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00015f90, 0x00c4, 0x28, 0x0f, 0x02,
		0x00, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00015f90, 0x00b7, 0x28, 0x0f, 0x02,
		0x00, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x000f4240, 0x0064, 0x28, 0x01, 0x02,
		0x06, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00002710, 0x005c, 0x28, 0x14, 0x02,
		0x02, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00de, 0x28, 0x01, 0x02,
		0x05, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00e1, 0x28, 0x01, 0x02,
		0x05, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x0098, 0x28, 0x01, 0x02,
		0x02, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00015f90, 0x005b, 0x28, 0x01, 0x02,
		0x00, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00015f90, 0x0055, 0x28, 0x01, 0x02,
		0x02, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x0082, 0x28, 0x01, 0x02,
		0x03, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00077a10, 0x00e8, 0x28, 0x01, 0x02,
		0x03, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x0104, 0x28, 0x01, 0x02,
		0x05, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00027100, 0x0096, 0x28, 0x01, 0x02,
		0x01, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00009c40, 0x005e, 0x28, 0x01, 0x02,
		0x02, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00d6, 0x28, 0x01, 0x02,
		0x05, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x00d7, 0x28, 0x01, 0x02,
		0x05, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x00d7, 0x28, 0x01, 0x02,
		0x05, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x00c8, 0x28, 0x01, 0x02,
		0x05, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00015f90, 0x00aa, 0x28, 0x01, 0x02,
		0x01, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00a6, 0x28, 0x01, 0x02,
		0x02, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00000000, 0x008f, 0x28, 0x01, 0x01,
		0x02, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x0099, 0x28, 0x01, 0x02,
		0x02, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x0094, 0x28, 0x01, 0x02,
		0x04, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00a7, 0x28, 0x01, 0x02,
		0x02, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x009b, 0x28, 0x01, 0x02,
		0x04, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00077a10, 0x00a2, 0x28, 0x01, 0x02,
		0x04, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x0096, 0x28, 0x01, 0x02,
		0x01, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x0096, 0x28, 0x01, 0x02,
		0x04, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00015f90, 0x00aa, 0x28, 0x01, 0x02,
		0x01, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00027100, 0x00ca, 0x28, 0x01, 0x03,
		0x01, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00aa, 0x28, 0x01, 0x02,
		0x00, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x009e, 0x28, 0x01, 0x02,
		0x00, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00002710, 0x005b, 0x28, 0x01, 0x02,
		0x04, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00de, 0x28, 0x01, 0x02,
		0x00, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00027100, 0x009f, 0x28, 0x14, 0x02,
		0x00, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00000000, 0x0065, 0x28, 0x04, 0x01,
		0x03, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x005a, 0x28, 0x01, 0x02,
		0x00, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00027100, 0x00ac, 0x28, 0x01, 0x02,
		0x01, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00002710, 0x0064, 0x28, 0x14, 0x02,
		0x02, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00015f90, 0x00d2, 0x28, 0x01, 0x02,
		0x05, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00000000, 0x0099, 0x28, 0x01, 0x01,
		0x04, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00a0, 0x28, 0x01, 0x02,
		0x00, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00000000, 0x009d, 0x28, 0x01, 0x01,
		0x04, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x000f4240, 0x0309, 0x28, 0x26, 0x02,
		0x05, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x000f4240, 0x0064, 0x28, 0x01, 0x02,
		0x06, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x000f4240, 0x0064, 0x28, 0x01, 0x02,
		0x06, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00d5, 0x28, 0x01, 0x02,
		0x00, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00000000, 0x0000, 0x00, 0x01, 0x00,
		0xff, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00d3, 0x28, 0x01, 0x02,
		0x04, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00057e40, 0x00da, 0x28, 0x01, 0x02,
		0x00, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
	{
		0x00027100, 0x0006, 0x00, 0x01, 0x02,
		0x04, 0x00, 0x02, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00027100, 0x0006, 0x00, 0x01, 0x02,
		0x04, 0x00, 0x02, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00027100, 0x0005, 0x00, 0x01, 0x02,
		0x04, 0x00, 0x02, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00027100, 0x0005, 0x00, 0x01, 0x02,
		0x04, 0x00, 0x02, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00027100, 0x000a, 0x00, 0x01, 0x02,
		0x04, 0x00, 0x02, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00027100, 0x000b, 0x00, 0x01, 0x02,
		0x04, 0x00, 0x02, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00027100, 0x000a, 0x00, 0x01, 0x02,
		0x04, 0x00, 0x02, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x00027100, 0x000a, 0x00, 0x01, 0x02,
		0x04, 0x00, 0x02, 0x00, 0x00,
		0x00, 0x00,
	},
	{
		0x0009c400, 0x000a, 0x00, 0x01, 0x02,
		0x04, 0x00, 0x46, 0x00, 0x00,
		0x00, 0x00,
	},
};

Item ITEM_PARA[128] = {
	{
		"sm.recovery",
		0x00000064,
		0x0000,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"med.recovery",
		0x000001f4,
		0x0000,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"lrg.recovery",
		0x000003e8,
		0x0000,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"sup.recovery",
		0x000009c4,
		0x0014,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"MP Floppy",
		0x0000012c,
		0x0000,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"Medium MP",
		0x00000320,
		0x0000,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"Large MP",
		0x000007d0,
		0x0000,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"Double flop",
		0x000005dc,
		0x0000,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"Various",
		0x0000012c,
		0x0000,
		0x0001,
		0x01,
		0x01,
		0x0000,
	},
	{
		"Omnipotent",
		0x000007d0,
		0x0000,
		0x0001,
		0x02,
		0x01,
		0x0000,
	},
	{
		"Protection",
		0x000004b0,
		0x0000,
		0x0001,
		0x01,
		0x01,
		0x0000,
	},
	{
		"Restore",
		0x00000fa0,
		0x0000,
		0x0001,
		0x01,
		0x01,
		0x0000,
	},
	{
		"Sup.restore",
		0x0000251c,
		0x0064,
		0x0001,
		0x02,
		0x01,
		0x0000,
	},
	{
		"Bandage",
		0x000000c8,
		0x0000,
		0x0001,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Medicine",
		0x000003e8,
		0x0000,
		0x0001,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Off. Disk",
		0x000001f4,
		0x0000,
		0x0003,
		0x01,
		0x01,
		0x0000,
	},
	{
		"Def. Disk",
		0x000001f4,
		0x0000,
		0x0003,
		0x01,
		0x01,
		0x0000,
	},
	{
		"Hispeed dsk",
		0x000001f4,
		0x0000,
		0x0003,
		0x01,
		0x01,
		0x0000,
	},
	{
		"Omni Disk",
		0x00000bb8,
		0x0000,
		0x0003,
		0x01,
		0x01,
		0x0000,
	},
	{
		"S.Off.disk",
		0x00000fa0,
		0x0000,
		0x0003,
		0x01,
		0x01,
		0x0000,
	},
	{
		"S.Def.disk",
		0x00000fa0,
		0x0000,
		0x0003,
		0x01,
		0x01,
		0x0000,
	},
	{
		"S.speed.disk",
		0x00000fa0,
		0x0000,
		0x0003,
		0x01,
		0x01,
		0x0000,
	},
	{
		"Auto Pilot",
		0x0000012c,
		0x0000,
		0x0005,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Off. Chip",
		0x0000270f,
		0x0320,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Def. Chip",
		0x0000270f,
		0x0320,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Brain Chip",
		0x0000270f,
		0x0320,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Quick Chip",
		0x0000270f,
		0x0320,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"HP Chip",
		0x0000270f,
		0x0320,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"MP Chip",
		0x0000270f,
		0x0320,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"DV Chip A",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"DV Chip D",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"DV Chip E",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Port. potty",
		0x0000012c,
		0x0000,
		0x0005,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Trn. manual",
		0x0000270f,
		0x0000,
		0x0005,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Rest pillow",
		0x000003e8,
		0x0000,
		0x0005,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Enemy repel",
		0x00001388,
		0x0000,
		0x0005,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Enemy bell",
		0x00001388,
		0x0000,
		0x0005,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Health shoe",
		0x000007d0,
		0x0000,
		0x0005,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Meat",
		0x00000032,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Giant Meat",
		0x000001f4,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Sirloin",
		0x000005dc,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Supercarrot",
		0x000001f4,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Hawk radish",
		0x000001f4,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Spiny green",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Digimushrm",
		0x00000064,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Ice mushrm",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Deluxmushrm",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Digipine",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Blue apple",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Red Berry",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Gold Acorn",
		0x00001f40,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Big Berry",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Sweet Nut",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Super veggy",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Pricklypear",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Orange bana",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Power fruit",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Power Ice",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Speed Leaf",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Sage Fruit",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Muscle Yam",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Calm berry",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Digianchovy",
		0x00000032,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Digisnapper",
		0x00000064,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"DigiTrout",
		0x0000012c,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Black trout",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Digicatfish",
		0x000007d0,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Digiseabass",
		0x00001f40,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Moldy Meat",
		0x00000032,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Happymushrm",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Chain melon",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Grey Claws",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Fireball",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Flamingwing",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Iron Hoof",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Mono Stone",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Steel drill",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"White Fang",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Black Wing",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Spike Club",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Flamingmane",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"White Wing",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Torn tatter",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Electo ring",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Rainbowhorn",
		0x00001388,
		0x01f4,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Rooster",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Unihorn",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Horn helmet",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Scissor jaw",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Fertilizer",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Koga laws",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Waterbottle",
		0x00001388,
		0x01f4,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"North Star",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Red Shell",
		0x00001388,
		0x01f4,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Hard Scale",
		0x00001388,
		0x01f4,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Bluecrystal",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Ice crystal",
		0x00001388,
		0x01f4,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Hair grower",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Sunglasses",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Metal part",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Fatal Bone",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Cyber part",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Mega Hand",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Silver ball",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Metal armor",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Chainsaw",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Small spear",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"X Bandage",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Ray Gun",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Gold banana",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Mysty Egg",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Red Ruby",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Beetlepearl",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Coral charm",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Moon mirror",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Blue Flute",
		0x0000270f,
		0x0000,
		0x0005,
		0x00,
		0x00,
		0x0000,
	},
	{
		"old fishrod",
		0x00000064,
		0x0000,
		0x0005,
		0x00,
		0x00,
		0x0000,
	},
	{
		"Amazing rod",
		0x00000bb8,
		0x012c,
		0x0005,
		0x00,
		0x00,
		0x0000,
	},
	{
		"Leomonstone",
		0x00000064,
		0x0000,
		0x0005,
		0x00,
		0x00,
		0x0000,
	},
	{
		"Mansion key",
		0x00000064,
		0x0000,
		0x0005,
		0x00,
		0x00,
		0x0000,
	},
	{
		"Gear",
		0x00000064,
		0x0000,
		0x0005,
		0x00,
		0x00,
		0x0000,
	},
	{
		"Rain Plant",
		0x000003e8,
		0x0000,
		0x0005,
		0x02,
		0x01,
		0x0000,
	},
	{
		"Steak",
		0x000003e8,
		0x0000,
		0x0000,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Frig Key",
		0x00000064,
		0x0000,
		0x0005,
		0xff,
		0x00,
		0x0000,
	},
	{
		"AS Decoder",
		0x000001f4,
		0x0000,
		0x0005,
		0xff,
		0x00,
		0x0000,
	},
	{
		"Giga Hand",
		0x00000000,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Noble Mane",
		0x00000000,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"Metalbanana",
		0x00000000,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
};

char *ITEM_DESC_PTR[128] = {
	MAIN_D_80125298,
	MAIN_D_801252B0,
	MAIN_D_801252CC,
	MAIN_D_801252E8,
	MAIN_D_80125300,
	MAIN_D_8012531C,
	MAIN_D_80125338,
	MAIN_D_80125354,
	MAIN_D_80125370,
	MAIN_D_80125384,
	MAIN_D_801253A0,
	MAIN_D_801253BC,
	MAIN_D_801253D8,
	MAIN_D_801253F4,
	MAIN_D_80125414,
	MAIN_D_8012542C,
	MAIN_D_80125448,
	MAIN_D_80125464,
	MAIN_D_8012547C,
	MAIN_D_80125498,
	MAIN_D_801254B8,
	MAIN_D_801254D8,
	MAIN_D_801254F4,
	MAIN_D_80125510,
	MAIN_D_80125530,
	MAIN_D_80125550,
	MAIN_D_8012556C,
	MAIN_D_80125588,
	MAIN_D_801255A0,
	MAIN_D_801255B8,
	MAIN_D_801255D4,
	MAIN_D_801255F0,
	MAIN_D_8012560C,
	MAIN_D_80125624,
	MAIN_D_8012563C,
	MAIN_D_80125658,
	MAIN_D_80125674,
	MAIN_D_8012568C,
	MAIN_D_801256A4,
	MAIN_D_801256C0,
	MAIN_D_801256DC,
	MAIN_D_801256F8,
	MAIN_D_801256F8,
	MAIN_D_80125710,
	MAIN_D_8012572C,
	MAIN_D_80125744,
	MAIN_D_80125760,
	MAIN_D_801256F8,
	MAIN_D_80125778,
	MAIN_D_8012578C,
	MAIN_D_801257A8,
	MAIN_D_801257C8,
	MAIN_D_801257E4,
	MAIN_D_801257FC,
	MAIN_D_80125814,
	MAIN_D_80125824,
	MAIN_D_80125840,
	MAIN_D_8012585C,
	MAIN_D_80125878,
	MAIN_D_8012588C,
	MAIN_D_801258A0,
	MAIN_D_801258B4,
	MAIN_D_801258C8,
	MAIN_D_801258E4,
	MAIN_D_801256DC,
	MAIN_D_80125760,
	MAIN_D_80125900,
	MAIN_D_80125918,
	MAIN_D_80125938,
	MAIN_D_80125954,
	MAIN_D_80125970,
	MAIN_D_80125988,
	MAIN_D_801259A0,
	MAIN_D_801259B8,
	MAIN_D_801259D0,
	MAIN_D_801259EC,
	MAIN_D_80125A08,
	MAIN_D_80125A24,
	MAIN_D_80125A40,
	MAIN_D_80125A58,
	MAIN_D_80125A70,
	MAIN_D_80125A88,
	MAIN_D_80125AA0,
	MAIN_D_80125AB8,
	MAIN_D_80125AD4,
	MAIN_D_80125AEC,
	MAIN_D_80125B08,
	MAIN_D_80125B20,
	MAIN_D_80125B3C,
	MAIN_D_80125B54,
	MAIN_D_80125B6C,
	MAIN_D_80125B84,
	MAIN_D_80125B9C,
	MAIN_D_80125BB4,
	MAIN_D_80125BCC,
	MAIN_D_80125BE4,
	MAIN_D_80125BFC,
	MAIN_D_80125C14,
	MAIN_D_80125C2C,
	MAIN_D_80125C44,
	MAIN_D_80125C60,
	MAIN_D_80125C7C,
	MAIN_D_80125C94,
	MAIN_D_80125CB0,
	MAIN_D_80125CC8,
	MAIN_D_80125CE4,
	MAIN_D_80125CFC,
	MAIN_D_80125D14,
	MAIN_D_80125D2C,
	MAIN_D_80125D44,
	MAIN_D_80125D5C,
	MAIN_D_80125D78,
	MAIN_D_80125D94,
	MAIN_D_80125DB0,
	MAIN_D_80125DCC,
	MAIN_D_80125DE8,
	MAIN_D_80125E04,
	MAIN_D_80125E20,
	MAIN_D_80125E3C,
	MAIN_D_80125E54,
	MAIN_D_80125E70,
	MAIN_D_80125E80,
	MAIN_D_80125900,
	MAIN_D_80125E9C,
	MAIN_D_80125EB8,
	MAIN_D_80125ED4,
	MAIN_D_80125EF0,
	MAIN_D_80125F08,
};

uint8_t MAIN_D_80127BDC[128] = {
	0x00, 0x01, 0x12, 0x03, 0x04, 0x08, 0x06, 0x00,
	0x08, 0x06, 0x07, 0x04, 0x08, 0x09, 0x09, 0x0a,
	0x04, 0x0b, 0x08, 0x0c, 0x08, 0x0b, 0x0d, 0x0a,
	0x04, 0x0e, 0x06, 0x00, 0x04, 0x0f, 0x0e, 0x08,
	0x08, 0x0b, 0x0e, 0x08, 0x10, 0x15, 0x11, 0x11,
	0x11, 0x12, 0x13, 0x0b, 0x09, 0x0e, 0x15, 0x02,
	0x03, 0x15, 0x10, 0x12, 0x14, 0x0b, 0x0b, 0x01,
	0x0c, 0x04, 0x0b, 0x07, 0x00, 0x02, 0x0e, 0x14,
	0x0d, 0x0d, 0x0d, 0x0d, 0x0f, 0x08, 0x0b, 0x05,
	0x0c, 0x01, 0x02, 0x04, 0x04, 0x05, 0x04, 0x05,
	0x10, 0x04, 0x05, 0x10, 0x16, 0x15, 0x0c, 0x04,
	0x0c, 0x17, 0x17, 0x05, 0x08, 0x0c, 0x05, 0x03,
	0x0e, 0x11, 0x04, 0x0a, 0x13, 0x0a, 0x04, 0x04,
	0x04, 0x0a, 0x17, 0x05, 0x04, 0x10, 0x05, 0x0a,
	0x04, 0x07, 0x08, 0x0e, 0x14, 0x09, 0x04, 0x10,
	0x04, 0x17, 0x15, 0x04, 0x09, 0x04, 0x04, 0x04,
};

uint8_t EVOLUTION_ITEM_TARGET[44] = {
	0x05, 0x09, 0x15, 0x24, 0x2f, 0x26, 0x08, 0x06,
	0x22, 0x30, 0x14, 0x25, 0x00, 0x07, 0x32, 0x21,
	0x13, 0x33, 0x19, 0x3a, 0x0a, 0x18, 0x23, 0x31,
	0x16, 0x17, 0x34, 0x35, 0x0c, 0x1a, 0x28, 0x36,
	0x0d, 0x1b, 0x29, 0x37, 0x0e, 0x1c, 0x2a, 0x38,
	0x3b, 0x3c, 0x3d, 0x3e,
};

ItemFunction ITEM_FUNCTIONS[128] = {
	(ItemFunction)handleHPHealingItem,
	(ItemFunction)handleHPHealingItem,
	(ItemFunction)handleHPHealingItem,
	(ItemFunction)handleHPHealingItem,
	(ItemFunction)handleMPHealingItem,
	(ItemFunction)handleMPHealingItem,
	(ItemFunction)handleMPHealingItem,
	(ItemFunction)handleDoubleFloppy,
	(ItemFunction)handleStatusItems,
	(ItemFunction)handleStatusItems,
	(ItemFunction)handleStatusItems,
	(ItemFunction)handleRestore,
	(ItemFunction)handleRestore,
	(ItemFunction)handleStatusItems,
	(ItemFunction)handleStatusItems,
	(ItemFunction)handleBuffDisks,
	(ItemFunction)handleBuffDisks,
	(ItemFunction)handleBuffDisks,
	(ItemFunction)handleBuffDisks,
	(ItemFunction)handleBuffDisks,
	(ItemFunction)handleBuffDisks,
	(ItemFunction)handleBuffDisks,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleChips,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	(ItemFunction)handleFood,
	(ItemFunction)handleFood,
	NULL,
	NULL,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
	(ItemFunction)handleEvoItems,
};
// clang-format on

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
		setRECT(&rect, i * 16, 486, 16, 1);
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

void renderMist(void)
{
	POLY_FT4 *prim;
	int16_t cameraDeltaX;
	int16_t cameraDeltaY;
	int32_t i;
	int32_t yIndex;
	int32_t evenFrame;

	if (((CURRENT_SCREEN != 0xa3) && (CURRENT_SCREEN != 0xdc)) ||
	    (isTriggerSet(0x155) != 1)) {
		if ((CURRENT_SCREEN != 0xdc) && (isTriggerSet(0x94) == 1)) {
			MIST_CLUT_Y[0] = 0xc0;
			MIST_CLUT_Y[1] = 0x80;
		}

		cameraDeltaX = CAMERA_X_PREVIOUS;
		cameraDeltaX -= CAMERA_X[0];
		/* Preserve the retail load order with the pinned compiler. */
		cameraDeltaY = CAMERA_Y_PREVIOUS - *(volatile int16_t *)CAMERA_Y;
		MIST_X_OFFSETS[0] += cameraDeltaX;
		if ((evenFrame = PLAYTIME_FRAMES % 2) == 0) {
			MIST_X_OFFSETS[0]--;
		}
		if (MIST_X_OFFSETS[0] >= 0xa0) {
			MIST_X_OFFSETS[0] -= 0x280;
		}
		if (MIST_X_OFFSETS[0] < -0x1df) {
			MIST_X_OFFSETS[0] += 0x280;
		}

		MAIN_D_80134226 = MIST_X_OFFSETS[0] + 0x140;
		if (MAIN_D_80134226 >= 0xa0) {
			MAIN_D_80134226 -= 0x280;
		}
		if (MAIN_D_80134226 < -0x1df) {
			MAIN_D_80134226 += 0x280;
		}

		MAIN_D_80134228 += (int16_t)(cameraDeltaX * 12 / 10);
		if (evenFrame == 0) {
			MAIN_D_80134228++;
		}
		if (MAIN_D_80134228 >= 0x1e0) {
			MAIN_D_80134228 -= 0x280;
		}
		if (MAIN_D_80134228 < -0x9f) {
			MAIN_D_80134228 += 0x280;
		}

		MAIN_D_8013422A = MAIN_D_80134228 + 0x140;
		if (MAIN_D_8013422A >= 0x1e0) {
			MAIN_D_8013422A -= 0x280;
		}
		if (MAIN_D_8013422A < -0x9f) {
			MAIN_D_8013422A += 0x280;
		}
		if (MAIN_D_8013422A >= 0x1e0) {
			MAIN_D_8013422A -= 0x280;
		}
		if (MAIN_D_8013422A < -0x9f) {
			MAIN_D_8013422A += 0x280;
		}

		MIST_Y_OFFSETS[0] += cameraDeltaY;
		if (MIST_Y_OFFSETS[0] >= 0x78) {
			MIST_Y_OFFSETS[0] -= 0x1e0;
		}
		if (MIST_Y_OFFSETS[0] < -0x167) {
			MIST_Y_OFFSETS[0] += 0x1e0;
		}
		MAIN_D_8013422E = MIST_Y_OFFSETS[0] + 0xf0;
		if (MAIN_D_8013422E >= 0x78) {
			MAIN_D_8013422E -= 0x1e0;
		}
		if (MAIN_D_8013422E < -0x167) {
			MAIN_D_8013422E += 0x1e0;
		}

		for (i = 0, yIndex = -4; i < 8; i++, yIndex++) {
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
				prim->x0 = (&MAIN_D_80134228)[i % 2];
				prim->x1 = (&MAIN_D_80134228)[i % 2] - 0x140;
				prim->x2 = (&MAIN_D_80134228)[i % 2];
				prim->x3 = (&MAIN_D_80134228)[i % 2] - 0x140;
				prim->y0 = MIST_Y_OFFSETS[yIndex / 2];
				prim->y1 = MIST_Y_OFFSETS[yIndex / 2];
				prim->y2 = MIST_Y_OFFSETS[yIndex / 2] + 0xf0;
				prim->y3 = MIST_Y_OFFSETS[yIndex / 2] + 0xf0;
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
	int32_t randomRange;
	int32_t fallSpeed;
	int8_t randomValue;
	int8_t chance;
	int8_t horizontalMovement;

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

	randomValue = horizontalMovement = random(randomRange);
	if ((randomValue > (randomRange / 2)) != 0) {
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

	inst->y = inst->y + fallSpeed;
	if ((inst->y >= 0x83) && (inst->animSprites[0] != 2)) {
		chance = random(10);
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

void loadMapObjectPosition(int16_t *xData, int16_t *yData, int16_t startIndex,
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

int32_t tickMoveObjectTo(uint8_t objectIndex, uint8_t moveIndex,
			 int8_t steps, int16_t targetX, int16_t targetY)
{
	int8_t moveX;
	int32_t moveY;
	int32_t currentX;
	int32_t currentY;

	if (MAP_OBJECT_MOVE_TO_DATA[moveIndex] == 0) {
		MAIN_D_8013CB38[moveIndex] =
			(targetX - LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x) / steps;
		MAIN_D_8013CB44[moveIndex] =
			(targetY - LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y) / steps;
		MAP_OBJECT_MOVE_TO_DATA[moveIndex] = 1;
	}

	moveX = MAIN_D_8013CB38[moveIndex];
	LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x +=
		MAIN_D_8013CB38[moveIndex];
	moveY = MAIN_D_8013CB44[moveIndex];
	LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y += moveY;

	if (moveX > 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x >= targetX) {
			LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x = targetX;
		}
	} else if (MAIN_D_8013CB38[moveIndex] < 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x <= targetX) {
			LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x = targetX;
		}
	} else {
		LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x = targetX;
	}

	if (MAIN_D_8013CB44[moveIndex] > 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y >= targetY) {
			LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y = targetY;
		}
	} else if (MAIN_D_8013CB44[moveIndex] < 0) {
		if (LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y <= targetY) {
			LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y = targetY;
		}
	} else {
		LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y = targetY;
	}

	currentX = LOCAL_MAP_OBJECT_INSTANCE[objectIndex].x;
	if (currentX == targetX) {
		currentY = LOCAL_MAP_OBJECT_INSTANCE[objectIndex].y;
		if (currentY == targetY) {
			MAP_OBJECT_MOVE_TO_DATA[moveIndex] = 0;
			return 1;
		}
	}
	return 0;
}

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

void setMapObjectsFlag(uint8_t start, int32_t count, int32_t flag)
{
	long idx;
	int32_t i;

	for (i = 0; i < count; i++) {
		idx = start + i;
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
			if (LOCAL_MAP_OBJECT_INSTANCE[(long)sprite + i]
				    .animSprites[j] != -2) {
				break;
			}
		}
		positionsX[i] =
			screenX -
			LOCAL_MAP_OBJECTS[LOCAL_MAP_OBJECT_INSTANCE[(long)sprite + i]
						  .animSprites[j]]
				.width /
				2;
		positionsY[i] =
			screenY -
			LOCAL_MAP_OBJECTS[LOCAL_MAP_OBJECT_INSTANCE[(long)sprite + i]
						  .animSprites[j]]
				.height /
				2;
	}

	loadMapObjectPosition(positionsX, positionsY, sprite, count);
}

void spawnSpriteAtEntity(int32_t scriptId, int32_t nodeId, uint8_t sprite)
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

void loadMapDigimon(int16_t *data, int32_t mapId)
{
	Stats *stats;
	int16_t count;
	int32_t digimonCount;
	VECTOR *waypoints;
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
			waypoints = MAP_DIGIMON_TABLE[i].waypoints;
			count = *data++;
			for (j = 0; j < 8; j++) {
				*(int16_t *)((uint8_t *)(j * 2) + (uint32_t)waypoints + 0x80) = *data++;
			}
			for (j = 0; j < count; j++) {
				waypoints[j].vx = *data++;
				waypoints[j].vy = *data++;
				waypoints[j].vz = *data++;
			}
			*(int16_t *)((uint8_t *)waypoints + 0x90) = 0;
		}
	}
}

void loadNPCModel(int32_t digimonId)
{
	thunkLoadMMD(digimonId, 0);
}

int32_t scriptSetDigimon(uint8_t type, uint8_t slot, uint8_t autotalk)
{
	Entity *entity;
	int32_t entityId;

	if (type != MAP_DIGIMON_TABLE[slot].typeId) {
		return 0;
	}
	entityId = slot + 2;
	if ((entity = ENTITY_TABLE[entityId]) != NULL) {
		removeEntity(entity->type, slot + 2);
	}
	ENTITY_TABLE[slot + 2] = &NPC_ENTITIES[slot].digimonEntity.entity;
	initializeDigimonObject(type, slot + 2, tickNPC);
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

void removeMapEntities(void)
{
	volatile int32_t unloaded[8];
	Entity *entity;
	int32_t *models;
	int32_t i;
	int32_t j;

	for (j = 0; j < 8; j++) {
		unloaded[j] = -1;
	}
	for (i = 0; i < 8; i++) {
		entity = ENTITY_TABLE[i + 2];
		if (entity != NULL) {
			entity = ENTITY_TABLE[i + 2];
			removeEntity(entity->type, i + 2);
			ENTITY_TABLE[i + 2] = NULL;
		}
	}
	for (i = 0; i < 8; i++) {
		if (LOADED_DIGIMON_MODELS[i] != -1) {
			thunkUnloadModel((models = LOADED_DIGIMON_MODELS)[i], 0);
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

void resetEntityOrigin(int32_t scriptId)
{
	NPCEntity *npc;
	VECTOR *waypoints;
	int16_t deltaX;
	int16_t deltaZ;
	int32_t entityIndex;
	int32_t i;
	int32_t j;

	npc = NPC_ENTITIES;
	for (i = 0; i < 8; i++) {
		entityIndex = i + 2;
		if (ENTITY_TABLE[entityIndex] == NULL) {
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
	waypoints = MAP_DIGIMON_TABLE[i].waypoints;
	for (j = 0; j < 8; j++) {
		if (*(int16_t *)((uint8_t *)(j * 2) +
				   (uint32_t)waypoints + 0x80) != 0) {
			if (*(int16_t *)((uint8_t *)(j * 2) +
					   (uint32_t)waypoints + 0x80) == -1) {
				break;
			}
			waypoints[j].vx += deltaX;
			waypoints[j].vz += deltaZ;
		}
	}
}

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

void addGameMenu(void)
{
	MAIN_D_80123E88[2] = 1;
	MAIN_D_80134D28 = 7;
	GAME_MENU_SPRITES[0x4e] = 0;
	MAIN_D_80134D2C = getFishingAvailability();
	if (MAIN_D_80134D2C != 0) {
		MAIN_D_80134D28++;
		if (MAIN_D_80134D2C == 1) {
			GAME_MENU_SPRITES[0x4e] = 1;
		}
		MAIN_D_80123E88[2] = 7;
	}
	if (PARTNER_PARA.condition & 1) {
		GAME_MENU_SPRITES[0x42] = 0;
	} else {
		GAME_MENU_SPRITES[0x42] = 1;
	}
	TRIANGLE_MENU_STATE = 0;
	addObject(0xfa4, 0, (void (*)(int32_t))tickTriangleMenu, NULL);
}

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
		drawString(STR_YEAR_DAY, 0, 0xe8);
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
			drawString(STATUS_VIEW_LABELS[0], 0, 0);
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
			drawString(PLAYER_VIEW_LABELS[0], 0, 0);
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

void closeUIBoxIfOpen(int32_t id)
{
	int16_t pos[2];

	if (UI_BOX_DATA[id].frame >= 5 && UI_BOX_DATA[id].state == 1) {
		getEntityScreenPos(ENTITY_TABLE[0], 0, pos);
		removeAnimatedUIBox(id, 0);
	}
}

void renderGameMenu(void)
{
	GameMenuSprite *sprite;
	int32_t digitCount;
	int32_t digits[6];
	int8_t disabled;
	int32_t highlight;
	int16_t yOffset;
	int32_t i;

	renderSeperatorLines(GAME_MENU_LINES, 2, 5);
	renderDateDigits();
	yOffset = 0;
	if (MAIN_D_80134D28 == 7) {
		yOffset = -0x28;
	}
	sprite = (GameMenuSprite *)GAME_MENU_SPRITES;
	renderTriangleCursor((int8_t)MAIN_D_80123E88[2], yOffset);
	for (i = 1; i < MAIN_D_80134D28; sprite++, i++) {
		disabled = 0;
		if (sprite->disabled == 1) {
			disabled = 1;
		}
		highlight = 0;
		if ((i == MAIN_D_80123E88[2]) &&
		    ((PLAYTIME_FRAMES % 10) < 5)) {
			highlight = 0x14;
		}
		renderRectPolyFT4(sprite->x - 0x42,
				  sprite->y - 0x50 + yOffset,
				  sprite->width, sprite->height,
				  sprite->texX + highlight,
				  sprite->texY + 0xc0, 0x1e,
				  GetClut(0x100, sprite->clutY + 0x1f0), 6,
				  disabled);
		renderRectPolyFT4(((GameMenuLabel *)GAME_MENU_LABELS)[i].x - 0x42,
				  yOffset + (((GameMenuLabel *)GAME_MENU_LABELS)[i].y - 0x50),
				  ((GameMenuLabel *)GAME_MENU_LABELS)[i].width,
				  ((GameMenuLabel *)GAME_MENU_LABELS)[i].height,
				  ((GameMenuLabel *)GAME_MENU_LABELS)[i].texX,
				  ((GameMenuLabel *)GAME_MENU_LABELS)[i].texY + 0xbf, 0x1e, 0x7f50, 6,
				  disabled);
	}
	renderString(3, -0x3c, -0x49, 0x24, 0xc, 0, 0xe7, 6, 1);
	renderString(3, 6, -0x49, 0x1c, 0xc, 0x24, 0xe7, 6, 1);
	convertValueToDigits(3, DAY, &digitCount, digits);
}

void renderSeperatorLines(int16_t *lines, int8_t count, int32_t zIndex)
{
	uint8_t *color;
	int32_t i;

	for (i = 0; i < count; lines += 5ULL, i++) {
		color = &MAIN_D_80123F48[((uint8_t *)lines)[8] * 3];
		renderLinePrimitive(color[0] | (color[1] << 8) |
				    (color[2] << 16),
				    lines[0], lines[1], lines[2], lines[3],
				    zIndex, 0);
	}
}

void renderDateDigits(void)
{
	int32_t digitCount;
	int32_t digits[6];
	int32_t digit;
	int32_t i;
	int32_t j;
	int16_t texX;
	int16_t texY;

	convertValueToDigits(2, DAY + 1, &digitCount, digits);
	for (i = digitCount - 1, j = 0; i >= 0; i--, j++) {
		texY = 0x34;
		if ((digit = *(int32_t *)&digits[i]) == 0) {
			texX = 0x78;
		} else if (digit < 5) {
			texX = (digits[i] - 1) * 8 + 0x60;
			texY = 0x28;
		} else {
			texX = (digits[i] - 5) * 8 + 0x50;
		}
		renderRectPolyFT4(0x25 + j * 9, -0x4a, 7, 0xc, texX + 1,
				  texY + 0xc0, 0x1e, 0x7f10, 5, 0);
	}

	convertValueToDigits(3, YEAR + 1, &digitCount, digits);
	for (i = digitCount - 1, j = 0; i >= 0; i--, j++) {
		texY = 0x34;
		if ((digit = *(int32_t *)&digits[i]) == 0) {
			texX = 0x78;
		} else if (digit < 5) {
			texX = (digits[i] - 1) * 8 + 0x60;
			texY = 0x28;
		} else {
			texX = (digits[i] - 5) * 8 + 0x50;
		}
		renderRectPolyFT4(-0x16 + j * 9, -0x4a, 7, 0xc, texX + 1,
				  texY + 0xc0, 0x1e, 0x7f10, 5, 0);
	}
}

void renderTriangleCursor(int32_t selection, int16_t yOffset)
{
	TriangleCursorUVData u0;
	TriangleCursorUVData u1;
	TriangleCursorUVData v0;
	TriangleCursorUVData v1;
	TriangleCursorOffsetData xOffset;
	TriangleCursorOffsetData yOffsetData;
	TriangleCursorOffsetData width;
	TriangleCursorOffsetData height;
	POLY_FT4 *prim;
	GsOT_TAG *tag;
	int16_t baseX;
	int16_t baseY;
	int32_t menuYOffset;
	int32_t i;

	u0 = MAIN_D_80134250;
	u1 = MAIN_D_80134258;
	v0 = MAIN_D_80134260;
	v1 = MAIN_D_80134268;
	xOffset = MAIN_D_80134270;
	yOffsetData = MAIN_D_80134278;
	width = MAIN_D_80134280;
	height = MAIN_D_80134288;
	baseX = *(int16_t *)((uint8_t *)MAIN_D_80123E88 + (selection * 12)) -
		0x46;
	baseY = *(int16_t *)((uint8_t *)&MAIN_D_80123E88[1] + (selection * 12)) -
		0x53;
	i = 0;
	menuYOffset = yOffset;
	tag = &ACTIVE_ORDERING_TABLE->org[5];
	for (; i < 8; i++) {
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		setUV4(prim, (uint8_t)u0.data[i], (uint8_t)v0.data[i], (uint8_t)u1.data[i], (uint8_t)v0.data[i], (uint8_t)u0.data[i], (uint8_t)v1.data[i], (uint8_t)u1.data[i], (uint8_t)v1.data[i]);
		setPosDataPolyFT4(prim, baseX + xOffset.data[i],
				   baseY + yOffsetData.data[i] + menuYOffset,
				   width.data[i], height.data[i]);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = GetTPage(0, 0, 0x380, 0x1c0);
		prim->clut = GetClut(0x100, 0x1fc);
		AddPrim(tag, prim);
		prim++;
		GsSetWorkBase((PACKET *)prim);
	}
}

void renderRectPolyFT4(int16_t posX, int16_t posY, int32_t width,
		       int32_t height, uint8_t texX, uint8_t texY,
		       int16_t texturePage, int16_t clut, int32_t zIndex,
		       int8_t flag)
{
	POLY_FT4 *prim;
	GsOT_TAG *tags;
	int32_t doubledWidth;
	uint32_t drawWidth;
	uint32_t drawHeight;
	int32_t x;
	int32_t y;
	int32_t bottom;
	int32_t left;
	int32_t right;
	int32_t top;

	tags = ACTIVE_ORDERING_TABLE->org;
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	if (flag & 0x40) {
		SetSemiTrans(prim, 1);
	}
	if ((texY + height) >= 0x100) {
		height = (height - 1u) & 0xffu;
	}
	if ((texX + width) >= 0x100) {
		width = (width - 1u) & 0xffu;
	}
	setUVDataPolyFT4(prim, texX, texY, drawWidth = width, drawHeight = height);
	if (flag & 2) {
		setPosDataPolyFT4(prim, posX, posY, drawWidth * 2, drawHeight * 2);
	} else if (flag & 4) {
		setPosDataPolyFT4(prim, posX, posY, doubledWidth = drawWidth * 2, drawHeight * 2);
		prim->x0 += (int16_t)doubledWidth;
		prim->x1 -= (int16_t)doubledWidth;
		prim->x2 += (int16_t)doubledWidth;
		prim->x3 -= (int16_t)doubledWidth;
	} else {
		setPosDataPolyFT4(prim, posX, posY, drawWidth, drawHeight);
	}
	if (flag & 1) {
		setRGB0(prim, 0x32, 0x32, 0x32);
	} else {
		setRGB0(prim, 0x80, 0x80, 0x80);
	}
	prim->tpage = texturePage;
	prim->clut = clut;
	AddPrim(&tags[zIndex], prim);
	prim++;
	GsSetWorkBase((PACKET *)prim);
	if (flag & 0x80) {
		x = posX;
		y = posY;
		top = y - 1;
		right = x + 0xd;
		renderTrianglePrimitive(0x20202, left = x - 1, top, right, top, right,
					bottom = y + 0xc, 2, 0);
		renderTrianglePrimitive(0x20202, left, top, left, bottom,
					right, bottom, 2, 0);
	}
}

void tickGameMenu(void)
{
	int32_t selection;
	int32_t previousSelection;
	register int32_t input;
	int32_t pressed;

	if (PARTNER_PARA.condition & 1) {
		GAME_MENU_SPRITES[0x42] = 0;
	}
	selection = MAIN_D_80123E88[2];
	previousSelection = selection;
	pressed = input = POLLED_INPUT & ~POLLED_INPUT_PREVIOUS;
	if (pressed & 0x1000) {
		if ((selection -= 3) <= 0) {
			selection += ((MAIN_D_80134D28 + 1) / 3) * 3;
		}
		if (selection >= MAIN_D_80134D28) {
			selection -= 3;
		}
	} else if (input & 0x4000) {
		if ((selection += 3) >= MAIN_D_80134D28) {
			selection -= ((MAIN_D_80134D28 + 1) / 3) * 3;
		}
		if (selection <= 0) {
			selection += 3;
		}
	} else if (input & 0x8000) {
		if ((selection -= 1) <= 0) {
			selection = MAIN_D_80134D28 - 1;
		}
	} else if (input & 0x2000) {
		if ((selection += 1) >= MAIN_D_80134D28) {
			selection = 1;
		}
	}
	if (selection != previousSelection) {
		MAIN_D_80123E88[2] = selection;
		playSound(0, 2);
	}
	if (TRIANGLE_MENU_STATE == -1) {
		if (isKeyDown(0x40) != 0) {
			if (((int8_t *)&MAIN_D_80123E88[3])[MAIN_D_80123E88[2] * 12] & 1) {
				playSound(0, 4);
			} else {
				playSound(0, 3);
			}
			handleGameMenuSelection(MAIN_D_80123E88[2]);
		}
		if ((isKeyDown(0x10) != 0) &&
		    ((UI_BOX_DATA[0].state == 1) ||
		     (UI_BOX_DATA[0].frame == 0))) {
			playSound(0, 4);
			closeTriangleMenu();
			setTamerState(0);
			setCameraFollowPlayer();
			IS_IN_MENU = 0;
			startGameTime();
		}
	}
}

int32_t createMenuBox(int32_t id, int16_t x, int16_t y, int16_t width,
		      int16_t height, int8_t features, void (*tick)(void),
		      void (*render)(void))
{
	RECT finalPos;
	RECT startPos;
	int16_t entityPos[2];

	if (UI_BOX_DATA[id].state == 1) {
		return 1;
	}
	if (UI_BOX_DATA[id].frame == 0) {
		setRECT(&finalPos, x, y, width, height);
		getEntityScreenPos(ENTITY_TABLE[0], 1, entityPos);
		setRECT(&startPos, entityPos[0] - 5, entityPos[1] - 5, 10, 10);
		createAnimatedUIBox(id, 1, features, &finalPos, &startPos,
				    (TickFunction)tick, (RenderFunction)render);
	}
	return 0;
}

void tickDigimonMenu(void)
{
	int8_t equippedSlot;
	int32_t slotResult;
	int16_t previousX;
	int16_t previousY;
	int32_t i;

	if ((MAIN_D_80134D36 != 1) ||
	    ((MAIN_D_80134D36 == 1) && (MENU_STATE == 1))) {
		if ((CHANGED_INPUT & 0x2000) && (MENU_STATE != 0)) {
			MAIN_D_80134D36++;
			if (MAIN_D_80134D36 >= 2) {
				MAIN_D_80134D36 = 1;
			} else {
				MENU_STATE = 0;
				MENU_SUB_STATE = 0;
				playSound(0, 2);
			}
		}
		if ((CHANGED_INPUT & 0x8000) && (MENU_STATE != 0)) {
			MAIN_D_80134D36--;
			if (MAIN_D_80134D36 < 0) {
				MAIN_D_80134D36 = 0;
			} else {
				MENU_STATE = 0;
				MENU_SUB_STATE = 0;
				playSound(0, 2);
			}
		}
		if (isKeyDown(0x10) != 0) {
			if (MENU_STATE == 1) {
				TRIANGLE_MENU_STATE = 4;
			}
			playSound(0, 4);
		}
		if ((MAIN_D_80134D36 == 1) && (isKeyDown(0x40) != 0)) {
			if (MENU_STATE == 1) {
				MENU_STATE = 2;
			}
			playSound(0, 3);
		}
	} else if (MENU_STATE == 6) {
		if (isKeyDown(0x10) != 0) {
			for (i = 0; i < 3; i++) {
				if ((PARTNER_ENTITY.digimonEntity.stats.base.moves[i] !=
				     0xff) &&
				    (MOVE_DATA[entityGetTechFromAnim(
					     &PARTNER_ENTITY.digimonEntity.entity,
					     PARTNER_ENTITY.digimonEntity.stats.base
						     .moves[i])]
					     .power != 0)) {
					break;
				}
			}
			if (((PARTNER_ENTITY.digimonEntity.stats.base.moves[0] !=
			      0xff) ||
			     (PARTNER_ENTITY.digimonEntity.stats.base.moves[1] !=
			      0xff) ||
			     (PARTNER_ENTITY.digimonEntity.stats.base.moves[2] !=
			      0xff)) &&
			    (i != 3)) {
				MENU_STATE = 4;
			}
			playSound(0, 4);
		} else if (isKeyDown(0x80) != 0) {
			MENU_STATE = 7;
			MENU_SUB_STATE = 0;
			playSound(0, 3);
		} else if (isKeyDown(0x40) != 0) {
			slotResult = getEquippedSlot();
			equippedSlot = (int8_t)slotResult;
			if ((int8_t)slotResult != -1) {
				EQUIPPED_MOVES[equippedSlot] = 0xff;
				PARTNER_ENTITY.digimonEntity.stats.base
					.moves[equippedSlot] = 0xff;
				playSound(0, 3);
			} else {
				equipMove();
			}
		}
		previousY = MAIN_D_80134D38;
		previousX = MAIN_D_80134D3A;
		if (CHANGED_INPUT & 0x1000) {
			MAIN_D_80134D38 -= 0xf;
		}
		if (CHANGED_INPUT & 0x4000) {
			MAIN_D_80134D38 += 0xf;
		}
		if (CHANGED_INPUT & 0x8000) {
			MAIN_D_80134D3A -= 0x12;
		}
		if (CHANGED_INPUT & 0x2000) {
			MAIN_D_80134D3A += 0x12;
		}
		if (MAIN_D_80134D3A < 0x73) {
			MAIN_D_80134D3A = 0x73;
		}
		if (MAIN_D_80134D3A >= 0xf2) {
			MAIN_D_80134D3A = 0xf1;
		}
		if (MAIN_D_80134D38 < 0x6f) {
			MAIN_D_80134D38 = 0x6f;
		}
		if (MAIN_D_80134D38 >= 0xca) {
			MAIN_D_80134D38 = 0xc9;
		}
		if ((previousX != MAIN_D_80134D3A) ||
		    (previousY != MAIN_D_80134D38)) {
			playSound(0, 2);
		}
	} else if ((MENU_STATE == 8) && (isKeyDown(0x10) != 0)) {
		MENU_STATE = 9;
		MENU_SUB_STATE = 0;
		playSound(0, 4);
	}
	TAMER_ENTITY.entity.isOnScreen = 0;
	PARTNER_ENTITY.digimonEntity.entity.isOnScreen = 0;
}

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

int32_t isUIBoxAvailable(int32_t id)
{
	if (UI_BOX_DATA[id].state == 1) {
		return 1;
	}

	if (UI_BOX_DATA[id].frame == 0) {
		return 1;
	}

	return 0;
}

void tickPlayerMenu(void)
{
	RECT finalPos;
	RECT secondFinalPos;
	RECT startPos;
	int16_t previousRow;
	int16_t previousColumn;
	int32_t maxColumn;
	int16_t selectorX;
	int32_t previousCard;
	int32_t i;

	if (MENU_STATE < 2) {
		if (CHANGED_INPUT & 0x2000) {
			MAIN_D_80134D37++;
			if (MAIN_D_80134D37 >= 4) {
				MAIN_D_80134D37 = 3;
			} else {
				MENU_STATE = 0;
				MENU_SUB_STATE = 0;
				playSound(0, 2);
			}
		}
		if (CHANGED_INPUT & 0x8000) {
			MAIN_D_80134D37--;
			if (MAIN_D_80134D37 < 0) {
				MAIN_D_80134D37 = 0;
			} else {
				MENU_STATE = 0;
				MENU_SUB_STATE = 0;
				playSound(0, 2);
			}
		}
		if (isKeyDown(0x10) != 0) {
			TRIANGLE_MENU_STATE = 6;
			playSound(0, 4);
		}
		if ((isKeyDown(0x40) != 0) && (MENU_STATE == 1) &&
		    (MAIN_D_80134D37 != 0) && (MAIN_D_80134D37 != 4)) {
			MENU_STATE = 2;
			playSound(0, 3);
			SELECTED_MEDAL = 0;
			MEDAL_SELECTOR_INDEX = 0;
			SELECTED_CARD = 0;
		}
	} else {
		if ((isKeyDown(0x10) != 0) && (MENU_STATE == 2)) {
			playSound(0, 4);
			MENU_STATE = 1;
			MAIN_D_80134D40 = -1;
		}

		if (MAIN_D_80134D37 == 1) {
			if (MENU_STATE == 2) {
				previousRow = MAIN_D_80134D42;
				previousColumn = MAIN_D_80134D44;
				if (CHANGED_INPUT & 0x8000) {
					MAIN_D_80134D42--;
				}
				if (CHANGED_INPUT & 0x2000) {
					MAIN_D_80134D42++;
				}
				if (MAIN_D_80134D42 < 0) {
					MAIN_D_80134D42 = 0;
				}
				if (MAIN_D_80134D42 >= 9) {
					MAIN_D_80134D42 = 8;
				}
				if ((MAIN_D_80134D42 == 0) ||
				    (MAIN_D_80134D42 == 1)) {
					if (CHANGED_INPUT & 0x1000) {
						MAIN_D_80134D44 -= 2;
					}
					if (CHANGED_INPUT & 0x4000) {
						MAIN_D_80134D44 += 2;
					}
					if (MAIN_D_80134D42 < 2) {
						MAIN_D_80134D44 =
							(MAIN_D_80134D44 / 2) * 2;
					}
					maxColumn = 6;
				} else {
					if (CHANGED_INPUT & 0x1000) {
						MAIN_D_80134D44--;
					}
					if (CHANGED_INPUT & 0x4000) {
						MAIN_D_80134D44++;
					}
					if (MAIN_D_80134D42 == 2) {
						maxColumn = 8;
					} else if (((MAIN_D_80134D42 >= 4) &&
						    (MAIN_D_80134D42 < 7)) ||
						   (MAIN_D_80134D42 == 8)) {
						maxColumn = 6;
					} else {
						maxColumn = 7;
					}
				}
				if (MAIN_D_80134D44 < 0) {
					MAIN_D_80134D44 = 0;
				}
				if (maxColumn < MAIN_D_80134D44) {
					MAIN_D_80134D44 = maxColumn;
				}
				if ((previousRow != MAIN_D_80134D42) ||
				    (previousColumn != MAIN_D_80134D44)) {
					playSound(0, 2);
				}

				if (MAIN_D_80134D42 < 3) {
					selectorX = MAIN_D_80134D42 * 0x25 + 0x1c;
				} else if (MAIN_D_80134D42 < 7) {
					selectorX =
						(MAIN_D_80134D42 - 3) * 0x18 + 0x8b;
				} else {
					selectorX =
						(MAIN_D_80134D42 - 7) * 0x18 + 0xf8;
				}
				MAIN_D_80134D40 = -1;
				for (i = 0; i < 0x3e; i++) {
					if (((selectorX + 2) ==
					     MAIN_D_80124544[i].posX) &&
					    ((MAIN_D_80134D44 * 0x13 + 0x2b) ==
					     MAIN_D_80124544[i].posY)) {
						break;
					}
				}
				MAIN_D_80134D40 = i + 1;
				if (isKeyDown(0x40) != 0) {
					if (hasDigimonRaised(
						    (uint16_t)MAIN_D_80134D40) != 0) {
						if (MAIN_D_80134D42 < 3) {
							selectorX =
								MAIN_D_80134D42 * 0x25 + 0x1c;
						} else if (MAIN_D_80134D42 < 7) {
							selectorX =
								(MAIN_D_80134D42 - 3) * 0x18 + 0x8b;
						} else {
							selectorX =
								(MAIN_D_80134D42 - 7) * 0x18 + 0xf8;
						}
						if (isUIBoxAvailable(2) == 1) {
							setRECT(&finalPos, -0x96, -0x59, 0x12c, 0xbe);
							startPos.x = selectorX - 0x99;
							startPos.y =
								MAIN_D_80134D44 * 0x13 - 0x45;
							setWH(&startPos, 10, 10);
							MAIN_D_80134D46 = 0;
							createAnimatedUIBox(
								2, 1, 0, &finalPos, &startPos,
								NULL,
								(RenderFunction)renderEvoChartDetail);
							playSound(0, 3);
							MENU_STATE = 3;
						}
					} else {
						playSound(0, 4);
					}
				}
			} else if ((MENU_STATE == 3) &&
				   (isKeyDown(0x10) != 0)) {
				if (MENU_STATE == 3) {
					if (MAIN_D_80134D42 < 3) {
						selectorX = MAIN_D_80134D42 * 0x25 + 0x1c;
					} else if (MAIN_D_80134D42 < 7) {
						selectorX =
							(MAIN_D_80134D42 - 3) * 0x18 + 0x8b;
					} else {
						selectorX =
							(MAIN_D_80134D42 - 7) * 0x18 + 0xf8;
					}
					setRECT(&finalPos, selectorX - 0x99, MAIN_D_80134D44 * 0x13 - 0x45, 10, 10);
					removeAnimatedUIBox(2, &finalPos);
					playSound(0, 3);
					MENU_STATE = 2;
				}
			}
		}

		if (MAIN_D_80134D37 == 2) {
			if ((CHANGED_INPUT & 0x8000) &&
			    ((MEDAL_SELECTOR_INDEX % 5) != 0)) {
				MEDAL_SELECTOR_INDEX--;
			}
			if ((CHANGED_INPUT & 0x2000) &&
			    ((MEDAL_SELECTOR_INDEX % 5) != 4)) {
				MEDAL_SELECTOR_INDEX++;
			}
			if ((CHANGED_INPUT & 0x1000) &&
			    (MEDAL_SELECTOR_INDEX >= 5)) {
				MEDAL_SELECTOR_INDEX -= 5;
			}
			if ((CHANGED_INPUT & 0x4000) &&
			    (MEDAL_SELECTOR_INDEX < 10)) {
				MEDAL_SELECTOR_INDEX += 5;
			}
			if (SELECTED_MEDAL != MEDAL_SELECTOR_INDEX) {
				if (hasMedal(MEDAL_SELECTOR_INDEX) != 0) {
					activateMedalTexture(MEDAL_SELECTOR_INDEX,
							     SELECTED_MEDAL);
					MENU_STATE = 3;
				}
				playSound(0, 2);
			}
			SELECTED_MEDAL = MEDAL_SELECTOR_INDEX;
		}

		if (MAIN_D_80134D37 == 3) {
			previousCard = (uint8_t)SELECTED_CARD;
			if (MENU_STATE == 2) {
				if ((isKeyDown(0x40) != 0) &&
				    (getCardAmount((uint8_t)SELECTED_CARD) != 0)) {
					playSound(0, 3);
					MENU_STATE = 3;
					loadCardImage(SELECTED_CARD);
					setRECT(&startPos, (SELECTED_CARD % 11) * 0x18 - 0x79, (SELECTED_CARD / 11) * 0x18 - 0x36, 1, 1);
					setRECT(&finalPos, -0x4b, -0x53, 0x96, 0xb4);
					setRECT(&secondFinalPos, 0x4a, 0x45, 0x36, 0x18);
					createAnimatedUIBox(
						2, 1, 0, &finalPos, &startPos, NULL,
						(RenderFunction)renderCardImage);
					createAnimatedUIBox(
						3, 1, 0, &secondFinalPos, &startPos,
						NULL, (RenderFunction)renderCardCount);
				}
				if (MENU_STATE == 2) {
					if ((CHANGED_INPUT & 0x8000) &&
					    ((SELECTED_CARD % 11) != 0)) {
						SELECTED_CARD--;
					}
					if ((CHANGED_INPUT & 0x2000) &&
					    ((SELECTED_CARD % 11) != 10)) {
						SELECTED_CARD++;
					}
					if ((CHANGED_INPUT & 0x1000) &&
					    (SELECTED_CARD >= 11)) {
						SELECTED_CARD -= 11;
					}
					if ((CHANGED_INPUT & 0x4000) &&
					    (SELECTED_CARD < 0x37)) {
						SELECTED_CARD += 11;
					}
					if (previousCard != SELECTED_CARD) {
						playSound(0, 2);
					}
				}
			}
			if ((isKeyDown(0x10) != 0) && (MENU_STATE == 3)) {
				playSound(0, 4);
				MENU_STATE = 2;
				setRECT(&startPos, (SELECTED_CARD % 11) * 0x18 - 0x79, (SELECTED_CARD / 11) * 0x18 - 0x36, 1, 1);
				removeAnimatedUIBox(2, &startPos);
				removeAnimatedUIBox(3, &startPos);
			}
		}
	}

	if (POLLED_INPUT == 0) {
		MAIN_D_80134D48 = 0;
	} else {
		MAIN_D_80134D48++;
	}
	TAMER_ENTITY.entity.isOnScreen = 0;
	PARTNER_ENTITY.digimonEntity.entity.isOnScreen = 0;
}

void renderPlayerMenu(void);

void renderPlayerMenu(void)
{
	int8_t tabs[4];
	int32_t v;
	int32_t t1;
	int32_t t2;
	int32_t t3;

	*(PlayerTabs *)tabs = MAIN_D_801342A4;
	v = MAIN_D_80134D37;
	if (v != 3) {
		if (v != 2) {
			if (v != 1) {
				if (v == 0) {
					renderPlayerInfoView();
				}
			} else {
				renderEvoChartView();
			}
		} else {
			renderMedalView();
		}
	} else {
		renderCardsView();
	}
	tabs[MAIN_D_80134D37] = 0;
	renderString(tabs[0], -0x89, -0x65, 0x3C, 0xC, 0, 0, 5, 1);
	renderString(t1 = tabs[1], -0x3E, -0x65, 0x3C, 0xC, 0x3C, 0, 5, 1);
	renderString(t2 = tabs[2], 0xD, -0x65, 0x24, 0xC, 0x78, 0, 5, 1);
	renderString(t3 = tabs[3], 0x40, -0x65, 0x24, 0xC, 0x9C, 0, 5, 1);
	renderMenuTab(-0x91, 0x4C, tabs[0]);
	renderMenuTab(-0x46, 0x4C, t1);
	renderMenuTab(5, 0x34, t2);
	renderMenuTab(0x38, 0x34, t3);
}

void handleGameMenuSelection(int32_t selection)
{
	switch (selection) {
	case 1:
		if (((int8_t *)&MAIN_D_80123E88[3])[selection * 12] & 1) {
			return;
		}
		TRIANGLE_MENU_STATE = 2;
		drawInventoryText();
		break;
	case 6:
		if (((int8_t *)&MAIN_D_80123E88[3])[selection * 12] & 1) {
			return;
		}
		closeTriangleMenu();
		setPartnerState(3);
		IS_IN_MENU = 0;
		startGameTime();
		break;
	case 5:
		if (((int8_t *)&MAIN_D_80123E88[3])[selection * 12] & 1) {
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
		if (((int8_t *)&MAIN_D_80123E88[3])[selection * 12] & 1) {
			return;
		}
		closeTriangleMenu();
		setPartnerState(0xf);
		IS_IN_MENU = 0;
		startGameTime();
		break;
	case 7:
		if (((int8_t *)&MAIN_D_80123E88[3])[selection * 12] & 1) {
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
	removeItem(INVENTORY.types.array[INVENTORY_POINTER], 1);
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

	renderOverworldItem(&TAMER_ITEM.worldItem);
}

int32_t getEquippedSlot(void)
{
	uint8_t moveId;
	uint8_t column;
	uint8_t row;
	int32_t slot;

	column = (MAIN_D_80134D3A - 0x73) / 18;
	row = (MAIN_D_80134D38 - 0x6f) / 15;
	if (row == 1) {
		row = 5;
	} else if (row == 2) {
		row = 1;
	} else if (row == 3) {
		row = 4;
	} else if (row == 4) {
		row = 2;
	} else if (row == 5) {
		row = 3;
	}
	moveId = column + row * 8;
	if (moveId >= 0x30) {
		moveId++;
	}
	for (slot = 0; slot < 3; slot++) {
		if ((moveId == 0x2c) && (EQUIPPED_MOVES[slot] == 0x30)) {
			return (int8_t)slot;
		}
		if ((moveId == 0x37) && (EQUIPPED_MOVES[slot] == 0x39)) {
			return (int8_t)slot;
		}
		if (EQUIPPED_MOVES[slot] == moveId) {
			return (int8_t)slot;
		}
	}
	return -1;
}

void equipMove(void)
{
	RECT textArea;
	uint8_t column;
	uint8_t row;
	uint8_t moveId;
	int32_t slot;
	int32_t animation;
	int32_t textY;

	column = (MAIN_D_80134D3A - 0x73) / 18;
	row = (MAIN_D_80134D38 - 0x6f) / 15;
	if (row == 1) {
		row = 5;
	} else if (row == 2) {
		row = 1;
	} else if (row == 3) {
		row = 4;
	} else if (row == 4) {
		row = 2;
	} else if (row == 5) {
		row = 3;
	}
	moveId = column + row * 8;
	if (row == 6) {
		moveId++;
	}
	if (hasMove(moveId) == 0) {
		playSound(0, 4);
		return;
	}

	for (animation = 0; animation < 16; animation++) {
		if (moveId == 0x2c) {
			if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type]
				    .moves[animation] == 0x2c) {
				break;
			}
			if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type]
				    .moves[animation] == 0x30) {
				moveId = 0x30;
				break;
			}
		}
		if (moveId == 0x37) {
			if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type]
				    .moves[animation] == 0x37) {
				break;
			}
			if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type]
				    .moves[animation] == 0x39) {
				moveId = 0x39;
				break;
			}
		}
		if (moveId ==
		    DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type]
			    .moves[animation]) {
			break;
		}
		if (animation == 15) {
			playSound(0, 4);
			return;
		}
	}

	for (slot = 0; slot < 3; slot++) {
		if (EQUIPPED_MOVES[slot] == 0xff) {
			break;
		}
		if (slot == 2) {
			playSound(0, 4);
			return;
		}
	}
	EQUIPPED_MOVES[slot] = moveId;
	PARTNER_ENTITY.digimonEntity.stats.base.moves[slot] = animation + 0x2e;
	setRECT(&textArea, 0, textY = slot * 12 + 0x18, 0x84, 0xc);
	clearTextSubArea(&textArea);
	drawString(MOVE_NAMES[(uint8_t)moveId], 0, textY);
	playSound(0, 3);
}
