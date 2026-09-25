#include <string.h>

#include <libgs.h>

#include <dw/anim.h>
#include <dw/bubble.h>
#include <dw/butterfly.h>
#include <dw/clock.h>
#include <dw/dooa.h>
#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/evolution.h>
#include <dw/file.h>
#include <dw/font.h>
#include <dw/item.h>
#include <dw/map_object.h>
#include <dw/math.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/sound.h>
#include <dw/tamer.h>
#include <dw/types.h>
#include <dw/world_object.h>

extern int8_t PARTNER_ANIMATION;
extern int32_t NANIMON_TRIGGER;
extern uint8_t POOP_TO_EAT;

extern int32_t HAS_IMMORTAL_HOUR;
extern int8_t IMMORTAL_HOUR;
extern int32_t HAS_BUTTERFLY;
extern int16_t EVOLUTION_TARGET;
extern uint8_t CURRENT_POOP_ID;
extern uint8_t HAS_USED_EVOITEM;
extern int32_t IS_NATURAL_DEATH;
extern int8_t ITEM_SCOLD_FLAG;
extern int16_t STATUS_UI_OFFSET_X;

char PARTNER_TEXT_IS_SICK[] = "is Sick!";
char PATH_ETCNA_UNTI_TMD[16] = "\\ETCNA\\UNTI.TMD";
RaiseData RAISE_DATA[66] = {
	{ { -1, -1, -1, -1, -1, -1, -1, -1 },
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 670, 200, -2270 },
	{ { -1, -1, -1, -1, -1, -1, -1, -1 },
	  25, 6, 3, 18, 1200, 6, 0, 6, 8, 0, 5, 710, 140, -2210 },
	{ { -1, -1, -1, -1, -1, -1, -1, -1 },
	  25, 10, 3, 18, 1200, 7, 0, 7, 8, 0, 10, 460, 210, -1770 },
	{ { 1, 5, 9, 13, 17, 21, -1, -1 },
	  40, 20, 5, 24, 1200, 8, 38, 1, 0, 0, 15, 550, 230, -1870 },
	{ { 0, 5, 9, 14, 19, -1, -1, -1 },
	  40, 22, 5, 24, 1200, 8, 63, 2, 4, 3, 15, 370, 230, -1570 },
	{ { 2, 6, 11, 16, 21, -1, -1, -1 },
	  80, 45, 9, 36, 1200, 12, 40, 1, 0, 0, 30, 30, 390, -880 },
	{ { 4, 9, 14, 18, 23, -1, -1, -1 },
	  80, 30, 6, 36, 1200, 10, 46, 4, 6, 0, 20, 130, 350, -1120 },
	{ { 5, 11, 17, 23, -1, -1, -1, -1 },
	  80, 35, 6, 36, 1200, 10, 40, 1, 2, 2, 30, 260, 380, -1450 },
	{ { 1, 5, 9, 13, 17, 21, -1, -1 },
	  80, 55, 14, 36, 1200, 13, 51, 3, 0, 0, 30, 50, 340, -940 },
	{ { 4, 9, 14, 18, 23, -1, -1, -1 },
	  80, 35, 8, 36, 1200, 10, 41, 4, 0, 1, 20, 130, 360, -1090 },
	{ { 0, 4, 8, 12, 16, 20, -1, -1 },
	  80, 42, 11, 36, 1200, 11, 65, 2, 4, 3, 30, 230, 280, -1400 },
	{ { 0, 3, 6, 9, 12, 15, 18, 21 },
	  80, 34, 11, 36, 1200, 13, 68, 2, 6, 4, 20, 270, 280, -1340 },
	{ { 0, 4, 8, 12, 16, 20, -1, -1 },
	  80, 45, 11, 36, 1200, 15, 53, 2, 3, 0, 50, -30, 390, -780 },
	{ { 2, 6, 11, 16, 21, -1, -1, -1 },
	  80, 24, 5, 36, 1200, 8, 52, 1, 3, 0, 5, 570, 170, -1900 },
	{ { 2, 6, 11, 16, 21, -1, -1, -1 },
	  80, 40, 8, 36, 1200, 10, 47, 1, 2, 3, 40, 340, 300, -1460 },
	{ { -1, -1, -1, -1, -1, -1, -1, -1 },
	  25, 6, 3, 18, 1200, 6, 0, 6, 8, 3, 5, 660, 160, -2090 },
	{ { -1, -1, -1, -1, -1, -1, -1, -1 },
	  25, 10, 3, 18, 1200, 7, 0, 7, 8, 3, 10, 460, 220, -1720 },
	{ { 2, 5, 8, 11, 14, 17, 20, 23 },
	  40, 25, 8, 24, 1200, 8, 38, 2, 5, 3, 15, 420, 230, -1610 },
	{ { 1, 5, 9, 13, 17, 21, -1, -1 },
	  40, 23, 6, 24, 1200, 8, 62, 1, 5, 1, 15, 250, 240, -1410 },
	{ { 2, 7, 12, 17, 21, -1, -1, -1 },
	  80, 45, 9, 36, 1200, 12, 57, 5, 7, 1, 30, 210, 360, -1290 },
	{ { 7, 12, 17, -1, -1, -1, -1, -1 },
	  80, 24, 4, 36, 1200, 9, 59, 1, 2, 2, 20, 50, 350, -1030 },
	{ { 2, 6, 11, 16, 21, -1, -1, -1 },
	  80, 35, 8, 36, 1200, 10, 49, 1, 0, 1, 20, 250, 400, -1320 },
	{ { 2, 6, 11, 16, 21, -1, -1, -1 },
	  80, 35, 8, 36, 1200, 10, 39, 1, 1, 0, 30, 190, 290, -1200 },
	{ { 1, 5, 9, 13, 17, 21, -1, -1 },
	  80, 42, 11, 36, 1200, 11, 45, 3, 1, 2, 30, 80, 340, -1020 },
	{ { 2, 6, 10, 14, 18, 21, -1, -1 },
	  80, 50, 13, 36, 1200, 15, 48, 1, 4, 3, 40, 200, 360, -1360 },
	{ { 3, 7, 11, 14, 18, 23, -1, -1 },
	  80, 40, 10, 36, 1200, 11, 42, 0, 5, 2, 10, 280, 270, -1450 },
	{ { 4, 12, 20, -1, -1, -1, -1, -1 },
	  80, 30, 4, 36, 1200, 14, 67, 4, 6, 1, 30, 30, 390, -940 },
	{ { 2, 6, 11, 16, 21, -1, -1, -1 },
	  80, 24, 5, 36, 1200, 8, 52, 1, 3, 3, 5, 550, 170, -1900 },
	{ { 4, 9, 14, 18, 23, -1, -1, -1 },
	  80, 35, 7, 36, 1200, 10, 53, 4, 3, 2, 15, 440, 350, -1730 },
	{ { -1, -1, -1, -1, -1, -1, -1, -1 },
	  25, 6, 3, 18, 1200, 6, 0, 6, 8, 1, 5, 680, 270, -2140 },
	{ { -1, -1, -1, -1, -1, -1, -1, -1 },
	  25, 10, 3, 18, 1200, 7, 0, 7, 8, 1, 10, 420, 220, -1630 },
	{ { 1, 5, 9, 13, 17, 21, -1, -1 },
	  40, 20, 5, 24, 1200, 8, 63, 1, 5, 0, 15, 470, 180, -1720 },
	{ { 2, 6, 10, 14, 18, 22, -1, -1 },
	  40, 20, 5, 24, 1200, 8, 69, 4, 7, 1, 15, 290, 280, -1380 },
	{ { 2, 6, 11, 16, 21, -1, -1, -1 },
	  80, 35, 7, 36, 1200, 11, 39, 1, 2, 0, 30, 240, 370, -1360 },
	{ { 1, 5, 9, 13, 17, 21, -1, -1 },
	  80, 48, 12, 36, 1200, 13, 50, 3, 5, 1, 30, 280, 340, -1390 },
	{ { 0, 4, 8, 12, 16, 20, -1, -1 },
	  80, 56, 14, 36, 1200, 15, 54, 2, 4, 3, 40, 0, 320, -880 },
	{ { 2, 6, 11, 16, 21, -1, -1, -1 },
	  80, 40, 8, 36, 1200, 11, 58, 1, 0, 2, 30, 310, 370, -1370 },
	{ { 2, 6, 10, 14, 18, 22, -1, -1 },
	  80, 33, 8, 36, 1200, 9, 69, 4, 6, 2, 20, 490, 340, -1810 },
	{ { 2, 6, 10, 14, 18, 22, -1, -1 },
	  80, 60, 12, 36, 1200, 12, 39, 4, 5, 1, 40, 210, 340, -1220 },
	{ { 0, 3, 6, 9, 12, 15, 18, 21 },
	  80, 35, 11, 36, 1200, 13, 68, 1, 6, 4, 30, 300, 260, -1420 },
	{ { 2, 6, 11, 16, 21, -1, -1, -1 },
	  80, 32, 6, 36, 1200, 10, 46, 1, 3, 2, 40, 260, 360, -1400 },
	{ { 4, 9, 13, 18, 23, -1, -1, -1 },
	  80, 30, 6, 36, 1200, 8, 50, 3, 3, 1, 5, 370, 270, -1610 },
	{ { 2, 6, 10, 14, 18, 22, -1, -1 },
	  80, 40, 10, 36, 1200, 10, 55, 1, 5, 1, 20, 500, 230, -1870 },
	{ { -1, -1, -1, -1, -1, -1, -1, -1 },
	  25, 6, 3, 18, 1200, 6, 0, 6, 8, 2, 5, 670, 230, -2100 },
	{ { -1, -1, -1, -1, -1, -1, -1, -1 },
	  25, 10, 3, 18, 1200, 7, 0, 7, 8, 2, 10, 330, 230, -1480 },
	{ { 1, 5, 9, 13, 17, 21, -1, -1 },
	  40, 20, 5, 24, 1200, 8, 44, 1, 5, 2, 15, 540, 210, -1880 },
	{ { 2, 6, 10, 14, 18, 22, -1, -1 },
	  40, 23, 6, 24, 1200, 8, 44, 0, 5, 2, 15, 480, 260, -1750 },
	{ { 2, 6, 10, 14, 18, 22, -1, -1 },
	  80, 53, 12, 36, 1200, 13, 66, 1, 0, 3, 40, 0, 350, -870 },
	{ { 2, 6, 11, 16, 21, -1, -1, -1 },
	  80, 33, 7, 36, 1200, 10, 40, 1, 5, 0, 20, 200, 330, -1320 },
	{ { 4, 8, 13, 18, 23, -1, -1, -1 },
	  80, 35, 7, 36, 1200, 10, 64, 3, 4, 1, 30, 270, 380, -1400 },
	{ { 3, 8, 13, 18, 23, -1, -1, -1 },
	  80, 40, 8, 36, 1200, 11, 65, 0, 5, 3, 30, 310, 330, -1520 },
	{ { 4, 9, 14, 18, 23, -1, -1, -1 },
	  80, 43, 9, 36, 1200, 12, 56, 4, 7, 3, 30, 60, 333, -1030 },
	{ { 3, 8, 13, 18, 23, -1, -1, -1 },
	  80, 42, 8, 36, 1200, 11, 63, 0, 1, 0, 20, 360, 270, -1620 },
	{ { 1, 5, 9, 13, 17, 21, -1, -1 },
	  80, 35, 9, 36, 1200, 10, 40, 3, 6, 4, 20, 530, 230, -1890 },
	{ { 2, 6, 10, 14, 18, 22, -1, -1 },
	  80, 45, 11, 36, 1200, 14, 48, 4, 3, 0, 50, 0, 460, -940 },
	{ { 3, 8, 13, 18, 23, -1, -1, -1 },
	  80, 24, 5, 36, 1200, 8, 52, 0, 2, 2, 5, 470, 330, -1800 },
	{ { 0, 3, 6, 9, 12, 15, 18, 21 },
	  80, 50, 14, 36, 1200, 9, 70, 1, 6, 0, 10, 550, 200, -1960 },
	{ { 1, 5, 9, 13, 17, 21, -1, -1 },
	  40, 22, 6, 24, 1200, 8, 62, 1, 1, 0, 15, 450, 240, -1760 },
	{ { 3, 8, 13, 18, 23, -1, -1, -1 },
	  80, 24, 6, 36, 1200, 8, 52, 0, 5, 1, 10, 550, 180, -1920 },
	{ { 2, 6, 11, 16, 21, -1, -1, -1 },
	  80, 35, 7, 36, 1200, 12, 61, 1, 0, 2, 30, 120, 430, -1230 },
	{ { 2, 7, 12, 17, 21, -1, -1, -1 },
	  80, 45, 9, 36, 1200, 13, 60, 5, 7, 3, 40, 0, 360, -950 },
	{ { 0, 5, 9, 14, 19, -1, -1, -1 },
	  80, 42, 8, 36, 1200, 12, 67, 2, 4, 1, 30, 110, 320, -1180 },
	{ { 0, 0, 0, 0, 0, -1, -1, -1 },
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ { 2, 6, 11, 16, 21, -1, -1, -1 },
	  80, 33, 7, 36, 1200, 10, 40, 1, 2, 0, 20, 200, 330, -1320 },
	{ { 2, 6, 10, 14, 18, 22, -1, -1 },
	  80, 45, 11, 36, 1200, 14, 48, 4, 3, 0, 50, 0, 460, -940 },
	{ { 2, 6, 10, 14, 18, 22, -1, -1 },
	  80, 40, 10, 36, 1200, 10, 55, 1, 3, 1, 20, 500, 230, -1870 },
};

typedef struct {
	uint8_t sleepyHour;
	uint8_t sleepyMinute;
	uint8_t wakeupHour;
	uint8_t wakeupMinute;
	uint8_t awakeHours;
	uint8_t sleepyHours;
} SleepPattern;

SleepPattern SLEEP_PATTERN[8] = {
	{16, 0, 1, 0, 15, 9},
	{19, 0, 4, 0, 15, 9},
	{22, 0, 7, 0, 15, 9},
	{2, 0, 11, 0, 15, 9},
	{7, 0, 16, 0, 15, 9},
	{10, 0, 19, 0, 15, 9},
	{0, 0, 0, 0, 4, 1},
	{0, 0, 0, 0, 8, 3},
};

int8_t ITEM_TAKE_DISTANCE[66] = {
	10, 10, 10, 10, 15, 10, 10, 15, 10, 15, 10, 15, 10, 10, 10, 10, 10,
	10, 15, 10, 10, 15, 10, 15, 10, 25, 10, 10, 10, 10, 10, 10, 10, 10,
	15, 15, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 15,
	10, 10, 10, 10, 10, 10, 10, 10, 15, 15, 10, 10, 10, 10, 10,
};

extern uint8_t MAIN_D_80137C00[2048];
extern uint8_t MAIN_D_80127BDC[];
extern GsDOBJ2 POOP_OBJECT;
extern GsCOORDINATE2 POOP_COORDINATES;

extern int8_t GAME_STATE;
extern int32_t FADE_PROTECTION;
extern int8_t PARTNER_STATE;
extern int8_t PARTNER_SUB_STATE;


extern int8_t STOP_DISTANCE_TIMER;
extern uint8_t CURRENT_SCREEN;
extern uint8_t MAP_LAYER_ENABLED;
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT GS_ORDERING_TABLE[2];
SVECTOR POOP_ROTATION = { 0, 0, 0, 0 };

extern int8_t PARTNER_AREA_RESPONSE[];
extern uint16_t CURRENT_FRAME;
extern uint16_t LAST_HANDLED_FRAME;
extern int32_t MAIN_D_80134C6C;
extern int32_t MAIN_D_80134C70;
extern int32_t MAIN_D_80134C74;
extern int32_t BUTTERFLY_ID;
extern int32_t IS_SCRIPT_PAUSED;
extern Stats DEATH_STATS;

void writePStat(int32_t id, uint8_t value);
void callScriptSection(int32_t a, int32_t b, int32_t c);
ModelComponent *thunkLoadMMD(int32_t digiType, int32_t modelType);
void learnMove(int32_t move);
void addTamerLevel(int32_t chance, int32_t amount);
void setSleepDisabled(int32_t disabled);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void setTrigger(uint16_t flag);
void createCloudFX(int16_t *pos);
void updateTimeOfDay(void);
int32_t isInDaytimeTransition(void);
void renderRectPolyFT4(int32_t posX, int32_t posY, int32_t width,
		       int32_t height, int32_t texX, int32_t texY,
		       int32_t texturePage, int32_t clut, int32_t zIndex,
		       int32_t flag);

void initializeStatusObjects(void);
void initializePoop(void);
void initializePartner(int32_t type, int32_t posX, int32_t posY,
		       int32_t posZ, int32_t rotX, int32_t rotY,
		       int32_t rotZ);
void setSleepTimes(PartnerPara *para, int32_t type);
void setFoodTimer(int16_t type);
void renderPoop(int32_t instanceId);
void initializeEvolvedPartner(int32_t type, int32_t posX, int32_t posY,
			      int32_t posZ, int32_t rotX, int32_t rotY,
			      int32_t rotZ);
void resetPartnerPara(PartnerPara *para, int32_t type);
void initializeReincarnatedPartner(int32_t type, int32_t posX, int32_t posY,
				    int32_t posZ, int32_t rotX, int32_t rotY,
				    int32_t rotZ);
void setReincarnateStats(int16_t hp, int16_t mp, int16_t offense,
			 int16_t defense, int16_t speed, int16_t brains);
void setupPartnerOnWarp(int32_t posX, int32_t posY, int32_t posZ,
			int32_t rotation);
void tickConditionBoundaries(void);
void tickSleepMechanics(void);
void tickUnhappinessMechanics(void);
void handlePraiseScold(int32_t state);
void handleSleeping(void);
int32_t createPoopPile(int16_t tileX, int16_t tileY);
void sleepRegen(void);
void tickTirednessMechanics(void);
int32_t partnerWillRefuseItem(void);
void tickHungerMechanics(void);
void handleConditionBubble(void);
void partnerHandleFoodFeed(int32_t itemType);
void tickPartnerPoopingMechanic(void);
void detectEdiblePoop(void);
void handleToilet(void);
void handlePoopWeightLoss(int32_t type);
void handleWildPoop(void);
void handleEatingPoop(void);
void tickSicknessMechanics(void);
void tickDeathCondition(void);
void skipHours(int32_t hours);
void handlePostBattleTiredness(void);
void tickPartnerNormal(void);
void renderStatusBars(int32_t state);
void setImmortalHour(void);

static void *partner_impl_functions[] = {
	setImmortalHour,
	renderStatusBars,
	tickPartnerNormal,
	handlePostBattleTiredness,
	skipHours,
	tickDeathCondition,
	tickSicknessMechanics,
	handleEatingPoop,
	handleWildPoop,
	handlePoopWeightLoss,
	handleToilet,
	detectEdiblePoop,
	tickPartnerPoopingMechanic,
	partnerHandleFoodFeed,
	handleConditionBubble,
	tickHungerMechanics,
	partnerWillRefuseItem,
	tickTirednessMechanics,
	sleepRegen,
	createPoopPile,
	handleSleeping,
	handlePraiseScold,
	tickUnhappinessMechanics,
	tickSleepMechanics,
	tickConditionBoundaries,
	setupPartnerOnWarp,
	setReincarnateStats,
	initializeReincarnatedPartner,
	resetPartnerPara,
	initializeEvolvedPartner,
	renderPoop,
	setFoodTimer,
	setSleepTimes,
	initializePartner,
	initializePoop,
	initializeStatusObjects,
};

void initializeStatusObjects(void)
{
	initializeConditionBubbles();

	initializeButterfly();
	HAS_BUTTERFLY = -1;

	initializePoop();

	EVOLUTION_TARGET = -1;
	CURRENT_POOP_ID = 0;
	HAS_USED_EVOITEM = 0;
	IS_NATURAL_DEATH = 0;
	ITEM_SCOLD_FLAG = 0;
	STATUS_UI_OFFSET_X = 75;
}

void initializePoop(void)
{
	char *buf;

	buf = (char *)MAIN_D_80137C00;
	readFile(PATH_ETCNA_UNTI_TMD, buf);

	GsMapModelingData((unsigned long *)&buf[4]);
	GsLinkObject4((unsigned long)&buf[0xc], &POOP_OBJECT, 0);

	GsInitCoordinate2(NULL, &POOP_COORDINATES);
	POOP_OBJECT.attribute = 0;
	POOP_OBJECT.coord2 = &POOP_COORDINATES;
}

static uint32_t primeInitializePartnerScheduler(uint32_t a, uint32_t b,
						uint32_t c, uint32_t d)
{
	uint32_t value = a;

	value += RAISE_DATA[value & 7].defaultWeight;
	value += (uint32_t)(uint16_t)RAISE_DATA[value & 7].poopTimer;

	return value + a + b + c + d;
}

void initializePartner(int32_t type, int32_t posX, int32_t posY,
		       int32_t posZ, int32_t rotX, int32_t rotY,
		       int32_t rotZ)
{
	int32_t i;

	thunkLoadMMD(type, 3);
	ENTITY_TABLE[1] = (Entity *)&PARTNER_ENTITY;
	initializeDigimonObject(type, 1, tickPartner);
	setEntityPosition(1, posX, posY, posZ);
	setEntityRotation(1, rotX, rotY, rotZ);
	setupEntityMatrix(1);
	startAnimation(ENTITY_TABLE[1], 0);

	STOP_DISTANCE_TIMER = 0;
	PARTNER_ENTITY.digimonEntity.stats.current.vabId = 4;
	loadPartnerSounds(type);
	PARTNER_ENTITY.digimonEntity.entity.isOnMap = 1;
	PARTNER_ENTITY.digimonEntity.entity.isOnScreen = 1;
	PARTNER_PARA.condition = 0;
	setSleepTimes(&PARTNER_PARA, type);

	PARTNER_PARA.missedSleepHours = 0;
	PARTNER_PARA.poopLevel = RAISE_DATA[type].poopTimer;
	PARTNER_PARA.poopingTimer = -1;
	PARTNER_PARA.virusBar = 0;
	PARTNER_PARA.tiredness = 0;
	PARTNER_PARA.discipline = 50;
	PARTNER_PARA.happiness = 50;
	PARTNER_PARA.unused3 = 50;
	setFoodTimer(type);

	PARTNER_PARA.energyLevel = RAISE_DATA[type].energyThreshold;
	PARTNER_PARA.remainingLifetime = 360;
	PARTNER_PARA.weight = RAISE_DATA[type].defaultWeight;
	PARTNER_PARA.trainBoostTimer = 0;
	PARTNER_PARA.trainBoostValue = 0;
	PARTNER_PARA.trainBoostFlag = 0;
	PARTNER_PARA.evoTimer = 0;
	PARTNER_PARA.battles = 0;
	PARTNER_PARA.careMistakes = 0;
	PARTNER_PARA.injuryTimer = 0;
	PARTNER_PARA.sicknessTimer = 0;
	PARTNER_PARA.areaEffectTimer = 0;
	PARTNER_PARA.timesBeingSick = 0;

	PARTNER_ENTITY.lives = 3;
	if (type == 3) {
		PARTNER_ENTITY.digimonEntity.stats.base.moves[0] = 0x2e;
		learnMove(2);
	}
	if (type == 0x11) {
		PARTNER_ENTITY.digimonEntity.stats.base.moves[0] = 0x30;
		learnMove(0x2b);
	}

	PARTNER_ENTITY.digimonEntity.stats.base.moves[1] = 0xff;
	PARTNER_ENTITY.digimonEntity.stats.base.moves[2] = 0xff;
	for (i = 15; i >= 0; i--) {
		if (DIGIMON_DATA[type].moves[i] == 0xff)
			continue;
		if (DIGIMON_DATA[type].moves[i] < 0x3a)
			continue;
		if (DIGIMON_DATA[type].moves[i] >= 0x71)
			continue;

		PARTNER_ENTITY.digimonEntity.stats.base.moves[3] = i + 0x2e;
		break;
	}
	if (i < 0)
		PARTNER_ENTITY.digimonEntity.stats.base.moves[3] = 0xff;

	addObject(0xfa8, 0, NULL, renderPoop);
	HAS_IMMORTAL_HOUR = 0;
	IMMORTAL_HOUR = -1;
}

void renderPoop(int32_t instanceId)
{
	VECTOR pos;
	VECTOR scale;
	SVECTOR rotation = POOP_ROTATION;
	int32_t i;
	int16_t scaleValue;
	int16_t height;
	int8_t size;

	if (!MAP_LAYER_ENABLED)
		return;

	SetRotMatrix(&GsWSMATRIX);
	SetTransMatrix(&GsWSMATRIX);
	for (i = 0; i < 100; i++) {
		if (((size = WORLD_POOP[i].size) != 0) &&
		    (CURRENT_SCREEN == WORLD_POOP[i].map)) {
			height = PARTNER_ENTITY.digimonEntity.entity.posData->location.vy;
			scaleValue = size * 4096 / 10;
			pos.vx = (int16_t)((WORLD_POOP[i].x - 50) * 100 + 50);
			pos.vy = height;
			pos.vz = (int16_t)((50 - WORLD_POOP[i].y) * 100 - 50);
			scale.vx = scaleValue;
			scale.vy = scaleValue;
			scale.vz = scaleValue;
			projectPosition(&POOP_COORDINATES, &pos, &rotation, &scale);
			drawObject(&POOP_OBJECT, GS_ORDERING_TABLE + ACTIVE_FRAMEBUFFER, 2);
		}
	}
}

void initializeEvolvedPartner(int32_t type, int32_t posX, int32_t posY,
			      int32_t posZ, int32_t rotX, int32_t rotY,
			      int32_t rotZ)
{
	removeObject((int16_t)type, 1);
	applyMMD(type, 3, &EVO_SEQUENCE_DATA.modelData);

	ENTITY_TABLE[1] = (Entity *)&PARTNER_ENTITY;

	initializeDigimonObject(type, 1, tickPartner);

	setEntityPosition(1, posX, posY, posZ);
	setEntityRotation(1, rotX, rotY, rotZ);
	setupEntityMatrix(1);

	startAnimation(ENTITY_TABLE[1], 0);

	PARTNER_ENTITY.digimonEntity.stats.current.vabId = 4;
	STOP_DISTANCE_TIMER = 0;
	PARTNER_PARA.condition = 0;

	setSleepTimes(&PARTNER_PARA, type);
	resetPartnerPara(&PARTNER_PARA, type);

	HAS_IMMORTAL_HOUR = 0;
	IMMORTAL_HOUR = -1;
}

/* CodeWarrior retains scheduler state between functions. This unused function
 * reproduces the state required by resetPartnerPara and setSleepTimes and is
 * linker-GC'd. */
static uint32_t primeResetPartnerParaScheduler(uint32_t a, uint32_t b,
					       uint32_t c, uint32_t d)
{
	uint32_t value = a;

	value ^= a;
	value = value * 5 + d;
	value += a;
	value = value * 3 + c;
	value = (value << 1) + a;
	value += (uint32_t)(uint16_t)RAISE_DATA[value & 7].poopTimer;
	value += RAISE_DATA[value & 7].defaultWeight;
	value ^= d;
	value += a;
	value += (uint32_t)(uint16_t)RAISE_DATA[value & 7].poopTimer;

	return value + a + b + c + d;
}

void resetPartnerPara(PartnerPara *para, int32_t type)
{
	PARTNER_PARA.poopLevel = RAISE_DATA[type].poopTimer;
	PARTNER_PARA.unused2 = 0;
	PARTNER_PARA.unused1 = 0;
	PARTNER_PARA.poopingTimer = -1;
	PARTNER_PARA.tiredness = 0;
	PARTNER_PARA.subTiredness = 0;
	PARTNER_PARA.tirednessHungerTimer = 0;
	PARTNER_PARA.timesBeingSick = 0;
	PARTNER_PARA.areaEffectTimer = 0;
	PARTNER_PARA.sicknessTimer = 0;
	PARTNER_PARA.injuryTimer = 0;
	PARTNER_PARA.sicknessTries = 0;
	PARTNER_PARA.unused4 = 0;

	setFoodTimer(type);

	PARTNER_PARA.starvationTimer = -1;
	PARTNER_PARA.emptyStomachTimer = 0;
	PARTNER_PARA.weight = RAISE_DATA[type].defaultWeight;
	PARTNER_PARA.careMistakes = 0;
	PARTNER_PARA.battles = 0;
}

void setSleepTimes(PartnerPara *para, int32_t type)
{
	int16_t level;
	int16_t awakeTimeBase;
	int32_t hoursAwake;

	level = DIGIMON_DATA[type].level;

	if (level < 3) {
		if (level == 1) {
			awakeTimeBase = 3;
		} else {
			awakeTimeBase = 7;
		}

		para->sleepyHour =
			(HOUR +
			 (hoursAwake = awakeTimeBase + (int16_t)random(1))) % 24;
		para->sleepyMinute = 0;
		para->wakeupHour =
			(para->sleepyHour +
			 SLEEP_PATTERN[RAISE_DATA[type].sleepCycle].sleepyHours);
		para->wakeupMinute = para->sleepyMinute;
		para->wakeupHour %= 24;
		para->hoursAwakeDefault = hoursAwake;
		para->hoursAsleepDefault =
			SLEEP_PATTERN[RAISE_DATA[type].sleepCycle].sleepyHours;
	} else {
		para->sleepyHour =
			SLEEP_PATTERN[RAISE_DATA[type].sleepCycle].sleepyHour;
		para->sleepyMinute =
			SLEEP_PATTERN[RAISE_DATA[type].sleepCycle].sleepyMinute;
		para->wakeupHour =
			SLEEP_PATTERN[RAISE_DATA[type].sleepCycle].wakeupHour;
		para->wakeupMinute =
			SLEEP_PATTERN[RAISE_DATA[type].sleepCycle].wakeupMinute;
		para->hoursAwakeDefault =
			SLEEP_PATTERN[RAISE_DATA[type].sleepCycle].awakeHours;
		para->hoursAsleepDefault =
			SLEEP_PATTERN[RAISE_DATA[type].sleepCycle].sleepyHours;
	}

	PARTNER_PARA.timeAwakeToday = PARTNER_PARA.hoursAwakeDefault * 6;
	PARTNER_PARA.sicknessCounter = 0;
	PARTNER_PARA.tirednessSleepTimer = 0;
}

void setFoodTimer(int16_t type)
{
	int32_t i;
	int32_t nextIndex;
	uint8_t level;

	level = DIGIMON_DATA[type].level;

	if (level == 1) {
		PARTNER_PARA.nextHungerHour = HOUR / 2 * 2 + 2;
		if (PARTNER_PARA.nextHungerHour >= 24) {
			PARTNER_PARA.nextHungerHour -= 24;
		}
	} else if (level == 2) {
		PARTNER_PARA.nextHungerHour = HOUR / 3 * 3 + 3;
		if (PARTNER_PARA.nextHungerHour >= 24) {
			PARTNER_PARA.nextHungerHour -= 24;
		}
	} else {
		for (i = 0, nextIndex = 1; i < 8; i++, nextIndex++) {
			if (HOUR <= PARTNER_PARA.nextHungerHour) {
				if (PARTNER_PARA.nextHungerHour <
				    RAISE_DATA[type].hungerTimes[i]) {
					PARTNER_PARA.nextHungerHour =
						RAISE_DATA[type].hungerTimes[i];
					goto calculateTimer;
				}
			} else if (HOUR < RAISE_DATA[type].hungerTimes[i]) {
				PARTNER_PARA.nextHungerHour =
					RAISE_DATA[type].hungerTimes[i];
				goto calculateTimer;
			}

			if (RAISE_DATA[type].hungerTimes[nextIndex] == -1 ||
			    i == 7) {
				int32_t j;
				int32_t nextJ;

				for (j = 0, nextJ = 1; j < 8; j++, nextJ++) {
					if (HOUR < RAISE_DATA[type].hungerTimes[j]) {
						PARTNER_PARA.nextHungerHour =
							RAISE_DATA[type]
								.hungerTimes[j];
						goto calculateTimer;
					}

					if (RAISE_DATA[type].hungerTimes[nextJ] == -1 ||
					    j == 7) {
						PARTNER_PARA.nextHungerHour =
							RAISE_DATA[type]
								.hungerTimes[0];
						goto calculateTimer;
					}
				}

				goto calculateTimer;
			}
		}
	}

calculateTimer:
	if (HOUR <= PARTNER_PARA.nextHungerHour) {
		PARTNER_PARA.foodLevel =
			(PARTNER_PARA.nextHungerHour - HOUR) * 60;
		if (MINUTE != 0) {
			PARTNER_PARA.foodLevel -= MINUTE;
		}
	} else {
		PARTNER_PARA.foodLevel = (24 - HOUR) * 60;
		PARTNER_PARA.foodLevel += PARTNER_PARA.nextHungerHour * 60;
		if (MINUTE != 0) {
			PARTNER_PARA.foodLevel -= MINUTE;
		}
	}
}

void initializeReincarnatedPartner(int32_t type, int32_t posX, int32_t posY,
				    int32_t posZ, int32_t rotX, int32_t rotY,
				    int32_t rotZ)
{
	removeObject((int16_t)type, 1);
	applyMMD(type, 3,
		 (EvoModelData *)&DOOA_REINCARNATION_SEQ.modelData);

	ENTITY_TABLE[1] = (Entity *)&PARTNER_ENTITY;
	initializeDigimonObject(type, 1, tickPartner);
	setEntityPosition(1, posX, posY, posZ);
	setEntityRotation(1, rotX, rotY, rotZ);
	setupEntityMatrix(1);
	startAnimation(ENTITY_TABLE[1], 0);

	PARTNER_ENTITY.digimonEntity.stats.current.vabId = 4;
	STOP_DISTANCE_TIMER = 0;

	if (type == 1)
		setReincarnateStats(90, 110, 10, 11, 9, 10);
	if (type == 15)
		setReincarnateStats(120, 100, 16, 8, 8, 6);
	if (type == 29)
		setReincarnateStats(100, 140, 12, 6, 6, 8);
	if (type == 43)
		setReincarnateStats(100, 120, 8, 8, 8, 8);

	PARTNER_PARA.condition = 0;
	setSleepTimes(&PARTNER_PARA, type);
	PARTNER_PARA.missedSleepHours = 0;
	PARTNER_PARA.tirednessSleepTimer = 0;
	resetPartnerPara(&PARTNER_PARA, type);

	PARTNER_PARA.discipline = 50;
	PARTNER_PARA.happiness = 50;
	PARTNER_PARA.unused3 = 50;
	PARTNER_PARA.timesBeingSick = 0;
	PARTNER_PARA.remainingLifetime = 360;
	PARTNER_PARA.age = 0;
	EVOLUTION_TARGET = -1;
	PARTNER_PARA.evoTimer = 0;
	PARTNER_PARA.careMistakes = 0;
	PARTNER_PARA.battles = 0;
	PARTNER_PARA.weight = RAISE_DATA[type].defaultWeight;

	TAMER_ENTITY.raisedCount++;
	if (TAMER_ENTITY.raisedCount >= 100)
		TAMER_ENTITY.raisedCount = 99;

	HAS_IMMORTAL_HOUR = 0;
	IMMORTAL_HOUR = -1;
}

void setReincarnateStats(int16_t hp, int16_t mp, int16_t offense,
			 int16_t defense, int16_t speed, int16_t brains)
{
	int16_t percentage;
	Stats *stats;
	int16_t inheritedMp;
	int16_t inheritedOffense;
	int16_t inheritedDefense;
	int16_t inheritedSpeed;
	int16_t inheritedBrains;
	int16_t inheritedHp;

	if (IS_NATURAL_DEATH == 1) {
		percentage = TAMER_ENTITY.tamerLevel;
	} else {
		percentage = 0;
	}

	inheritedMp = DEATH_STATS.base.mp * percentage / 100;
	inheritedOffense = DEATH_STATS.base.off * percentage / 100;
	inheritedDefense = DEATH_STATS.base.def * percentage / 100;
	inheritedSpeed = DEATH_STATS.base.speed * percentage / 100;
	inheritedBrains = DEATH_STATS.base.brain * percentage / 100;
	inheritedHp = DEATH_STATS.base.hp * percentage / 100;
	stats = &PARTNER_ENTITY.digimonEntity.stats;

	stats->current.currentHP = stats->base.hp = hp + inheritedHp;
	stats->current.currentMP = stats->base.mp = mp + inheritedMp;
	stats->base.off = offense + inheritedOffense;
	stats->base.def = defense + inheritedDefense;
	stats->base.speed = speed + inheritedSpeed;
	stats->base.brain = brains + inheritedBrains;

	IS_NATURAL_DEATH = 0;
}

void setupPartnerOnWarp(int32_t posX, int32_t posY, int32_t posZ,
			int32_t rotation)
{
	setEntityPosition(1, posX, posY, posZ);
	setEntityRotation(1, PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vx,
			  rotation,
			  PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vz);
	setupEntityMatrix(1);

	startAnimation(ENTITY_TABLE[1], 0);

	PARTNER_STATE = 1;
	PARTNER_SUB_STATE = 0;
	PARTNER_PARA.areaEffectTimer = 0;
}

void tickConditionBoundaries(void)
{
	if (PARTNER_PARA.happiness > 100) {
		PARTNER_PARA.happiness = 100;
	}

	if (PARTNER_PARA.happiness < -100) {
		PARTNER_PARA.happiness = -100;
	}

	if (PARTNER_PARA.discipline > 100) {
		PARTNER_PARA.discipline = 100;
	}

	if (PARTNER_PARA.discipline < 0) {
		PARTNER_PARA.discipline = 0;
	}

	if (PARTNER_PARA.tiredness > 100) {
		PARTNER_PARA.tiredness = 100;
	}

	if (PARTNER_PARA.tiredness < 0) {
		PARTNER_PARA.tiredness = 0;
	}

	if (PARTNER_PARA.energyLevel >
	    RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyCap) {
		PARTNER_PARA.energyLevel =
			RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyCap;
	}

	if (PARTNER_PARA.energyLevel < 0) {
		PARTNER_PARA.energyLevel = 0;
	}

	if (PARTNER_PARA.weight > 99) {
		PARTNER_PARA.weight = 99;
	}

	if (PARTNER_PARA.weight < 1) {
		PARTNER_PARA.weight = 1;
	}
}

void tickSleepMechanics(void)
{
	int32_t wasSleepy;
	int32_t delta;
	int32_t type;
	int16_t level;
	int8_t hoursLate;
	int8_t minutePart;
	int32_t sleepCycle;

	wasSleepy = PARTNER_PARA.condition & 1;

	if (((CURRENT_FRAME % 20) == 0) &&
	    (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
		PARTNER_PARA.tirednessSleepTimer +=
			(int16_t)(PARTNER_PARA.tiredness * 3 / 10);
	}

	if ((wasSleepy == 0) &&
	    (PARTNER_PARA.tirednessSleepTimer >= 180) &&
	    ((CURRENT_FRAME % 200) == 0) &&
	    (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
		PARTNER_PARA.timeAwakeToday--;
		PARTNER_PARA.tirednessSleepTimer = 0;

		hoursLate =
			(delta = PARTNER_PARA.hoursAwakeDefault * 6 -
				 PARTNER_PARA.timeAwakeToday) / 6;
		if ((delta % 6) != 0)
			hoursLate++;

		minutePart = PARTNER_PARA.timeAwakeToday % 6;
		PARTNER_PARA.sleepyHour =
			PARTNER_PARA.wakeupHour -
			PARTNER_PARA.hoursAsleepDefault - hoursLate;
		if (PARTNER_PARA.sleepyHour < 0)
			PARTNER_PARA.sleepyHour += 24;
		PARTNER_PARA.sleepyMinute = minutePart * 10;
	}

	type = PARTNER_ENTITY.digimonEntity.entity.type;
	level = DIGIMON_DATA[type].level;
	if (PARTNER_PARA.condition & 1) {
		if (level == 1) {
			if (((CURRENT_FRAME % 200) == 0) &&
			    (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
				PARTNER_PARA.happiness--;
				PARTNER_PARA.discipline--;
			}
		} else if (level == 2) {
			if (((CURRENT_FRAME % 300) == 0) &&
			    (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
				PARTNER_PARA.happiness--;
				PARTNER_PARA.discipline--;
			}
		} else if (((CURRENT_FRAME % 1200) == 0) &&
			   (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
			PARTNER_PARA.happiness -= 2;
			PARTNER_PARA.discipline -= 4;
		}

		if (((CURRENT_FRAME % 1200) == 0) &&
		    (CURRENT_FRAME != LAST_HANDLED_FRAME) &&
		    (HOUR != PARTNER_PARA.sleepyHour)) {
			PARTNER_PARA.sicknessCounter++;
			PARTNER_PARA.missedSleepHours++;
		}
	}

	if ((PARTNER_PARA.condition & 1) &&
	    (((PARTNER_PARA.sleepyHour < PARTNER_PARA.wakeupHour) &&
	      (PARTNER_PARA.sleepyHour < HOUR) &&
	      (HOUR >= PARTNER_PARA.wakeupHour)) ||
	     ((PARTNER_PARA.wakeupHour < PARTNER_PARA.sleepyHour) &&
	      (HOUR < PARTNER_PARA.sleepyHour) &&
	      (PARTNER_PARA.wakeupHour <= HOUR)))) {
		if (level < 3) {
			PARTNER_PARA.sleepyHour =
				PARTNER_PARA.wakeupHour +
				PARTNER_PARA.hoursAwakeDefault;
			if (PARTNER_PARA.sleepyHour >= 24)
				PARTNER_PARA.sleepyHour -= 24;
			PARTNER_PARA.sleepyMinute = 0;

			PARTNER_PARA.wakeupHour =
				PARTNER_PARA.sleepyHour +
				PARTNER_PARA.hoursAsleepDefault;
			if (PARTNER_PARA.wakeupHour >= 24)
				PARTNER_PARA.wakeupHour -= 24;
			PARTNER_PARA.wakeupMinute = 0;
		} else {
			sleepCycle = RAISE_DATA[type].sleepCycle;
			PARTNER_PARA.sleepyHour =
				SLEEP_PATTERN[sleepCycle].sleepyHour;
			PARTNER_PARA.sleepyMinute =
				SLEEP_PATTERN[sleepCycle].sleepyMinute;
			PARTNER_PARA.wakeupHour =
				SLEEP_PATTERN[sleepCycle].wakeupHour;
			PARTNER_PARA.wakeupMinute =
				SLEEP_PATTERN[sleepCycle].wakeupMinute;
		}

		PARTNER_PARA.timeAwakeToday =
			PARTNER_PARA.hoursAwakeDefault * 6;
		PARTNER_PARA.tirednessSleepTimer = 0;
		PARTNER_PARA.condition &= ~1;
		PARTNER_PARA.careMistakes++;
		updateConditionAnimation();
		addTamerLevel(1, -1);
	}

	if (wasSleepy == 0) {
		if (PARTNER_PARA.wakeupHour < PARTNER_PARA.sleepyHour) {
			if ((HOUR >= PARTNER_PARA.sleepyHour) ||
			    (HOUR < PARTNER_PARA.wakeupHour)) {
				PARTNER_PARA.condition |= 1;
			}
		} else if (PARTNER_PARA.sleepyHour <
			   PARTNER_PARA.wakeupHour) {
			if ((HOUR >= PARTNER_PARA.sleepyHour) &&
			    (HOUR < PARTNER_PARA.wakeupHour)) {
				PARTNER_PARA.condition |= 1;
			}
		}
	}

	if (PARTNER_PARA.condition & 1)
		setSleepDisabled(0);
	else
		setSleepDisabled(1);
}

void tickUnhappinessMechanics(void)
{
	int16_t happiness;
	int16_t randomValue;
	Stats *stats;

	if ((PARTNER_PARA.condition == 0) &&
	    (PARTNER_PARA.happiness < 0) &&
	    ((CURRENT_FRAME % 200) == 0) &&
	    (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
		happiness = PARTNER_PARA.happiness;
		if (happiness < 0)
			happiness = -happiness;

		randomValue = random(100);
		if (randomValue < happiness - PARTNER_PARA.discipline)
			PARTNER_PARA.condition |= 0x10;
	}

	if (PARTNER_PARA.condition & 0x10) {
		if (((CURRENT_FRAME % 15) == 0) &&
		    (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
			PARTNER_PARA.happiness++;
		}

		if (((CURRENT_FRAME % 300) == 0) &&
		    (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
			stats = &PARTNER_ENTITY.digimonEntity.stats;
			stats->base.hp -=
				(int16_t)(stats->base.hp * 2 / 100);
			stats->base.mp -=
				(int16_t)(stats->base.mp * 2 / 100);
			stats->base.off -=
				(int16_t)(stats->base.off * 2 / 100);
			stats->base.def -=
				(int16_t)(stats->base.def * 2 / 100);
			stats->base.speed -=
				(int16_t)(stats->base.speed * 2 / 100);
			stats->base.brain -=
				(int16_t)(stats->base.brain * 2 / 100);

			if (stats->base.hp <= 0)
				stats->base.hp = 1;
			if (stats->base.mp <= 0)
				stats->base.mp = 1;
			if (stats->base.off <= 0)
				stats->base.off = 1;
			if (stats->base.def <= 0)
				stats->base.def = 1;
			if (stats->base.speed <= 0)
				stats->base.speed = 1;
			if (stats->base.brain <= 0)
				stats->base.brain = 1;

			if (stats->current.currentHP >= stats->base.hp)
				stats->current.currentHP = stats->base.hp;
			if (stats->current.currentMP >= stats->base.mp)
				stats->current.currentMP = stats->base.mp;
		}
	}

	if ((PARTNER_PARA.condition & 0x10) &&
	    (PARTNER_PARA.happiness >= 0)) {
		PARTNER_PARA.condition &= ~0x10;
		PARTNER_ENTITY.digimonEntity.entity.anim.loopCount = 1;
		unsetButterfly(BUTTERFLY_ID);
		HAS_BUTTERFLY = -1;
	}
}

void handlePraiseScold(int32_t state)
{
	int16_t oldDiscipline;
	int32_t oldDiscipline2;
	int16_t oldHappiness;
	int16_t disciplineDelta;
	int16_t happinessDelta;
	uint8_t level;

	oldDiscipline = PARTNER_PARA.discipline;
	oldHappiness = PARTNER_PARA.happiness;
	oldDiscipline2 = oldDiscipline;

	if (state == 4) {
		if (ITEM_SCOLD_FLAG == 1) {
			disciplineDelta = 8;
			ITEM_SCOLD_FLAG = 2;
		} else {
			disciplineDelta = 2;
			happinessDelta = -10;
		}

		PARTNER_PARA.refusedFavFood = 0;
		PARTNER_PARA.condition &= ~0x10;

		if (HAS_BUTTERFLY == 0) {
			unsetButterfly(BUTTERFLY_ID);
			HAS_BUTTERFLY = -1;
		}
	} else {
		disciplineDelta = -5;
		happinessDelta = (oldDiscipline / 10) + 2;
	}

	PARTNER_PARA.discipline += disciplineDelta;
	PARTNER_PARA.happiness += happinessDelta;

	if (state == 4) {
		level = DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level;
		NANIMON_TRIGGER = 0;
		if ((level == 3) &&
		    (oldDiscipline2 == 0) &&
		    (oldHappiness == -100)) {
			NANIMON_TRIGGER = 1;
		}
	}
}

void handleSleeping(void)
{
	int16_t tileX;
	int16_t tileY;
	int16_t hoursSlept;
	int32_t type;
	int32_t sleepCycle;

	if (HOUR < PARTNER_PARA.wakeupHour)
		hoursSlept = PARTNER_PARA.wakeupHour - HOUR;
	else
		hoursSlept = PARTNER_PARA.wakeupHour + (24 - HOUR);

	type = PARTNER_ENTITY.digimonEntity.entity.type;
	advanceToTime(PARTNER_PARA.wakeupHour, PARTNER_PARA.wakeupMinute);
	PARTNER_PARA.evoTimer += PARTNER_PARA.hoursAsleepDefault;
	PARTNER_PARA.remainingLifetime -= hoursSlept;
	if (PARTNER_PARA.remainingLifetime < 0)
		PARTNER_PARA.remainingLifetime = 0;

	if (DIGIMON_DATA[type].level < 3) {
		PARTNER_PARA.sleepyHour =
			PARTNER_PARA.wakeupHour +
			PARTNER_PARA.hoursAwakeDefault;
		if (PARTNER_PARA.sleepyHour >= 24)
			PARTNER_PARA.sleepyHour -= 24;
		PARTNER_PARA.sleepyMinute = 0;

		PARTNER_PARA.wakeupHour =
			PARTNER_PARA.sleepyHour +
			PARTNER_PARA.hoursAsleepDefault;
		if (PARTNER_PARA.wakeupHour >= 24)
			PARTNER_PARA.wakeupHour -= 24;
		PARTNER_PARA.wakeupMinute = 0;
	} else {
		sleepCycle = RAISE_DATA[type].sleepCycle;
		PARTNER_PARA.sleepyHour =
			SLEEP_PATTERN[sleepCycle].sleepyHour;
		PARTNER_PARA.sleepyMinute =
			SLEEP_PATTERN[sleepCycle].sleepyMinute;
		PARTNER_PARA.wakeupHour =
			SLEEP_PATTERN[sleepCycle].wakeupHour;
		PARTNER_PARA.wakeupMinute =
			SLEEP_PATTERN[sleepCycle].wakeupMinute;
	}

	setFoodTimer((int16_t)type);
	PARTNER_PARA.starvationTimer = 0;
	PARTNER_PARA.condition &= ~4;
	PARTNER_PARA.timeAwakeToday = PARTNER_PARA.hoursAwakeDefault * 6;
	PARTNER_PARA.tirednessSleepTimer = 0;
	PARTNER_PARA.sicknessCounter = 0;
	PARTNER_PARA.condition &= ~1;
	setSleepDisabled(1);
	HAS_IMMORTAL_HOUR = 1;
	IMMORTAL_HOUR = (int8_t)HOUR;

	if (PARTNER_PARA.condition & 8) {
		getModelTile(&PARTNER_ENTITY.digimonEntity.entity.posData->location,
			     &tileX, &tileY);
		createPoopPile(tileX, tileY);
		PARTNER_PARA.condition &= ~8;
		PARTNER_PARA.poopLevel =
			RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type]
				.poopTimer;
		PARTNER_PARA.poopingTimer = -1;
	}

	if (PARTNER_PARA.condition & 4)
		PARTNER_PARA.weight--;

	if (PARTNER_PARA.condition & 0x40) {
		PARTNER_PARA.sicknessTimer += hoursSlept;
		PARTNER_PARA.happiness -= 20;
		PARTNER_PARA.discipline -= 10;
		PARTNER_PARA.tiredness += 10;
	}

	if (PARTNER_PARA.condition & 0x20) {
		PARTNER_PARA.injuryTimer += hoursSlept;
		PARTNER_PARA.happiness -= 10;
		PARTNER_PARA.discipline -= 5;
		PARTNER_PARA.tiredness += 5;
	}
}

int32_t createPoopPile(int16_t tileX, int16_t tileY)
{
	int16_t rotation;
	int16_t i;
	int16_t count;
	int32_t direction;
	int16_t pos[3];

	rotation = PARTNER_ENTITY.digimonEntity.entity.posData->rotation.vy;
	if (((0 <= rotation) && (rotation <= 0x300)) || (rotation >= 0xD00))
		tileY--;
	if ((rotation >= 0x100) && (rotation < 0x700))
		tileX++;
	if ((rotation >= 0x500) && (rotation <= 0xB00))
		tileY++;
	if ((rotation >= 0x900) && (rotation <= 0xF00))
		tileX--;

	count = 0;
	for (i = 0; i < 100; i++) {
		if ((tileX == WORLD_POOP[i].x) &&
		    (tileY == WORLD_POOP[i].y) &&
		    (CURRENT_SCREEN == WORLD_POOP[i].map)) {
			direction = random(4);
			if (direction == 0)
				tileX--;
			else if (direction == 1)
				tileX++;
			else if (direction == 2)
				tileY--;
			else
				tileY++;
			i = -1;
			count = 0;
		} else {
			if (count >= 16)
				break;
			if (WORLD_POOP[i].size != 0) {
				if (CURRENT_SCREEN == WORLD_POOP[i].map)
					count++;
			} else {
				WORLD_POOP[i].map = CURRENT_SCREEN;
				WORLD_POOP[i].x = tileX;
				WORLD_POOP[i].y = tileY;
				WORLD_POOP[i].size =
					RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].poopSize;
				break;
			}
		}
	}

	if (i >= 100) {
		WORLD_POOP[CURRENT_POOP_ID].map = CURRENT_SCREEN;
		WORLD_POOP[CURRENT_POOP_ID].x = tileX;
		WORLD_POOP[CURRENT_POOP_ID].y = tileY;
		WORLD_POOP[CURRENT_POOP_ID].size =
			RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].poopSize;
		CURRENT_POOP_ID++;
	}
	if (count >= 16) {
		for (i = CURRENT_POOP_ID; i < 100; i++) {
			if (CURRENT_SCREEN == WORLD_POOP[i].map) {
				WORLD_POOP[i].map = CURRENT_SCREEN;
				WORLD_POOP[i].x = tileX;
				WORLD_POOP[i].y = tileY;
				WORLD_POOP[i].size =
					RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].poopSize;
				CURRENT_POOP_ID++;
				break;
			}
			if (i == 99) {
				i = -1;
				CURRENT_POOP_ID = 0;
			}
		}
	}

	if (PARTNER_ENTITY.digimonEntity.entity.type != 0x27)
		PARTNER_PARA.virusBar++;
	if (CURRENT_POOP_ID >= 100)
		CURRENT_POOP_ID = 0;

	pos[0] = (tileX - 50) * 100 + 50;
	pos[1] = PARTNER_ENTITY.digimonEntity.entity.posData->location.vy;
	pos[2] = (50 - tileY) * 100 - 50;
	createCloudFX(pos);
	return i;
}

void sleepRegen(void)
{
	int16_t hoursSlept;
	int16_t hoursAsleepDefault;
	int16_t sleepFactor;
	int16_t maxHP;
	int16_t maxMP;
	int16_t healRoll;
	int32_t tiredPercent;

	hoursAsleepDefault = PARTNER_PARA.hoursAsleepDefault;
	if (HOUR < PARTNER_PARA.wakeupHour)
		hoursSlept = PARTNER_PARA.wakeupHour - HOUR;
	else
		hoursSlept = PARTNER_PARA.wakeupHour + (24 - HOUR);

	if ((MINUTE > 0) && (HOUR != PARTNER_PARA.sleepyHour))
		hoursSlept++;

	sleepFactor =
		(int16_t)(hoursSlept * 100 / hoursAsleepDefault);
	if (getItemCount(0x22) != 0)
		sleepFactor = (int16_t)(sleepFactor * 12 / 10);

	if (PARTNER_AREA_RESPONSE[0] == 1)
		sleepFactor = (int16_t)(sleepFactor * 12 / 10);
	else if (PARTNER_AREA_RESPONSE[0] == 2)
		sleepFactor = (int16_t)(sleepFactor * 8 / 10);

	maxHP = PARTNER_ENTITY.digimonEntity.stats.base.hp;
	maxMP = PARTNER_ENTITY.digimonEntity.stats.base.mp;
	healRoll = random(10);
	PARTNER_ENTITY.digimonEntity.stats.current.currentHP +=
		(int16_t)(sleepFactor * (maxHP * (healRoll + 70) / 100) /
			  100);
	PARTNER_ENTITY.digimonEntity.stats.current.currentMP +=
		(int16_t)(sleepFactor * (maxMP * (healRoll + 70) / 100) /
			  100);

	tiredPercent = (int16_t)random(20) + 80;
	PARTNER_PARA.tiredness -=
		(int16_t)(sleepFactor *
			  (PARTNER_PARA.tiredness * tiredPercent / 100) /
			  100);

	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP >= maxHP)
		PARTNER_ENTITY.digimonEntity.stats.current.currentHP = maxHP;
	if (PARTNER_ENTITY.digimonEntity.stats.current.currentMP >= maxMP)
		PARTNER_ENTITY.digimonEntity.stats.current.currentMP = maxMP;
	if (PARTNER_PARA.tiredness < 0)
		PARTNER_PARA.tiredness = 0;

	PARTNER_PARA.weight -=
		(int16_t)(RAISE_DATA[ENTITY_TABLE[1]->type].defaultWeight / 10);
	if (PARTNER_PARA.weight <= 0)
		PARTNER_PARA.weight = 1;
}

void tickTirednessMechanics(void)
{
	if (((PARTNER_PARA.areaEffectTimer % 1200) == 0) && (PARTNER_PARA.areaEffectTimer > 0)) {
		if (PARTNER_AREA_RESPONSE[0] == 1) {
			PARTNER_PARA.happiness = PARTNER_PARA.happiness + 1;
			PARTNER_PARA.tiredness = PARTNER_PARA.tiredness - 2;
		}
		if (PARTNER_AREA_RESPONSE[0] == 2) {
			PARTNER_PARA.happiness = PARTNER_PARA.happiness - 1;
			PARTNER_PARA.tiredness = PARTNER_PARA.tiredness + 1;
		}
	}
	if (PARTNER_PARA.subTiredness >= 0x3C) {
		PARTNER_PARA.tiredness = PARTNER_PARA.tiredness + 1;
		if (PARTNER_PARA.tiredness >= 0x64) {
			PARTNER_PARA.tiredness = 0x64;
		}
		PARTNER_PARA.subTiredness = 0;
	}
	if (PARTNER_PARA.tiredness >= 0x32) {
		PARTNER_PARA.tirednessHungerTimer = PARTNER_PARA.tirednessHungerTimer + 1;
	} else {
		PARTNER_PARA.tirednessHungerTimer = 0;
	}
	if (PARTNER_PARA.tiredness >= 0x50) {
		PARTNER_PARA.condition |= 2;
		if (((CURRENT_FRAME % 100) == 0) && (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
			PARTNER_PARA.happiness = PARTNER_PARA.happiness - 2;
		}
	} else {
		PARTNER_PARA.condition &= ~2;
	}
}

int32_t partnerWillRefuseItem(void)
{
	int32_t type;
	uint32_t condition;
	int16_t chance;
	int32_t item;
	int16_t disciplineChance;
	uint8_t level;
	uint8_t targetLevel;

	PARTNER_PARA.refusedFavFood = 0;
	type = PARTNER_ENTITY.digimonEntity.entity.type;
	condition = PARTNER_PARA.condition;
	chance = random(10);
	item = TAMER_ITEM.worldItem.type;

	if ((item >= 0x73) && (item != 0x79) && (item != 0x7A) &&
	    (item < 0x7D))
		return 1;
	if ((item >= 0x21) && (item < 0x26))
		return 1;

	if (((item >= 0x47) && (item < 0x73)) ||
	    ((item >= 0x7D) && (item < 0x80))) {
		if (item == 0x7E) {
			if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level != 3)
				return 1;
		} else if ((item == 0x7D) || (item == 0x7F)) {
			if (DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level != 4)
				return 1;
		} else {
			if (((level = DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level) ==
			     (targetLevel = DIGIMON_DATA[(&MAIN_D_80127BDC[0x39])[item]].level)) ||
			    (level + 2 == targetLevel) ||
			    (level - 1 == targetLevel))
				return 1;
		}
	}

	if (ITEM_SCOLD_FLAG == 2) {
		ITEM_SCOLD_FLAG = 0;
		return 0;
	}
	if ((0 <= item) && (item < 0x26)) {
		disciplineChance = random(100);
		if (disciplineChance < 110 -
		    (PARTNER_PARA.discipline + (random(10) + 10))) {
			ITEM_SCOLD_FLAG = 1;
			return 1;
		}
	}
	if (((condition & 4) == 0) &&
	    (item == RAISE_DATA[type].favoriteFood) && (chance < 2))
		PARTNER_PARA.refusedFavFood = 2;

	if (PARTNER_PARA.refusedFavFood != 0) {
		if ((PARTNER_PARA.condition & 4) != 0)
			ITEM_SCOLD_FLAG = 1;
		return 1;
	}
	return 0;
}

void tickHungerMechanics(void)
{
	int32_t type = PARTNER_ENTITY.digimonEntity.entity.type;

	if ((PARTNER_PARA.condition & 4) != 0) {
		if (((CURRENT_FRAME % 10) == 0) &&
		    (CURRENT_FRAME != LAST_HANDLED_FRAME))
			PARTNER_PARA.starvationTimer--;
	} else if (((CURRENT_FRAME % 20) == 0) &&
		   (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
		PARTNER_PARA.foodLevel--;
	}

	if (((CURRENT_FRAME % 1200) == 0) &&
	    (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
		PARTNER_PARA.energyLevel -= RAISE_DATA[type].energyUsage;
		if (PARTNER_PARA.energyLevel < 0)
			PARTNER_PARA.energyLevel = 0;
	}

	if (((PARTNER_PARA.condition & 4) == 0) &&
	    (PARTNER_PARA.foodLevel <= 0)) {
		PARTNER_PARA.starvationTimer = 180;
		PARTNER_PARA.condition |= 4;
		handleConditionBubble();
	}
	if (((PARTNER_PARA.condition & 4) != 0) &&
	    (PARTNER_PARA.starvationTimer <= 0)) {
		setFoodTimer(type);
		PARTNER_PARA.starvationTimer = 0;
		PARTNER_PARA.condition &= ~4;
		if (PARTNER_PARA.energyLevel <
		    RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyThreshold)
			PARTNER_PARA.careMistakes++;
	}

	if (PARTNER_PARA.energyLevel <= 0) {
		if ((CURRENT_FRAME % 200) == 0) {
			PARTNER_PARA.weight--;
			if (PARTNER_PARA.weight <= 0)
				PARTNER_PARA.weight = 1;
		}
		if (((CURRENT_FRAME % 20) == 0) &&
		    (CURRENT_FRAME != LAST_HANDLED_FRAME))
			PARTNER_PARA.emptyStomachTimer++;
	}
	if ((PARTNER_PARA.tirednessHungerTimer >= 50) &&
	    ((CURRENT_FRAME % 1200) == 0) &&
	    (CURRENT_FRAME != LAST_HANDLED_FRAME))
		PARTNER_PARA.foodLevel -= 3;

	if (RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyThreshold <=
	    PARTNER_PARA.energyLevel)
		setTrigger(0x280);
}

void handleConditionBubble(void)
{
	int32_t newBubble;
	uint32_t cond;

	newBubble = -1;

	if ((PARTNER_STATE != 1) && (PARTNER_STATE != 0xa)) {
		return;
	}

	cond = PARTNER_PARA.condition;

	if ((cond & 0x40) && (MAIN_D_80134C6C != 2)) {
		newBubble = 2;
	}

	if ((cond & 0x20) && (MAIN_D_80134C6C != 6)) {
		newBubble = 6;
	}

	if ((cond & 0x08) && (MAIN_D_80134C6C != 1)) {
		newBubble = 1;
	}

	if ((cond & 0x04) && (MAIN_D_80134C6C != 0)) {
		newBubble = 0;
	}

	if ((cond & 0x02) && (MAIN_D_80134C6C != 4)) {
		newBubble = 4;
	}

	if ((cond & 0x01) && (MAIN_D_80134C6C != 3)) {
		newBubble = 3;
	}

	if ((cond & 0x10) && (newBubble == -1) &&
	    (HAS_BUTTERFLY != 0) && (cond == 0x10)) {
		unsetBubble(MAIN_D_80134C70);
		BUTTERFLY_ID = setButterfly(ENTITY_TABLE[1]);
		HAS_BUTTERFLY = 0;
		return;
	} else if ((newBubble != MAIN_D_80134C6C) && (newBubble != -1) &&
		   (MAIN_D_80134C74 >= 50)) {
		if (HAS_BUTTERFLY == 0) {
			unsetButterfly(BUTTERFLY_ID);
			HAS_BUTTERFLY = -1;
			PARTNER_PARA.condition &= ~0x10;
			PARTNER_ENTITY.digimonEntity.entity.anim.loopCount = 1;
		}

		unsetBubble(MAIN_D_80134C70);
		MAIN_D_80134C70 = addConditionBubble(newBubble, ENTITY_TABLE[1]);
		MAIN_D_80134C74 = 0;
		MAIN_D_80134C6C = newBubble;
	}

	MAIN_D_80134C74 += 1;
	if (MAIN_D_80134C74 >= 60) {
		MAIN_D_80134C6C = -1;
	}
}

void partnerHandleFoodFeed(int32_t itemType)
{
	int32_t type;

	if (PARTNER_PARA.condition & 4) {
		if (((itemType < 0x26) || (itemType >= 0x47)) &&
		    (itemType != 0x79) && (itemType != 0x7a)) {
			return;
		}

		type = PARTNER_ENTITY.digimonEntity.entity.type;

		if (!(RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyThreshold >
		      PARTNER_PARA.energyLevel)) {
			PARTNER_PARA.happiness += 5;
			PARTNER_PARA.discipline += 1;
			PARTNER_PARA.condition &= ~4;
			setFoodTimer(PARTNER_ENTITY.digimonEntity.entity.type);
			PARTNER_PARA.starvationTimer = 0;
		}

		if ((TAMER_ITEM.worldItem.type ==
		     RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].favoriteFood) ||
		    !(PARTNER_PARA.energyLevel <
		      RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyThreshold)) {
			PARTNER_ANIMATION = 0xb;
			startAnimation(&PARTNER_ENTITY.digimonEntity.entity, 0xb);
		}

		return;
	}

	if (((itemType < 0x26) || (itemType >= 0x47)) &&
	    (itemType != 0x79) && (itemType != 0x7a)) {
		return;
	}

	PARTNER_PARA.happiness -= 3;
	PARTNER_PARA.discipline -= 2;
}

void tickPartnerPoopingMechanic(void)
{
	uint32_t isPoopy;

	if (getTamerState() != 0) {
		return;
	}

	isPoopy = PARTNER_PARA.condition & 8;

	if (!isPoopy && ((CURRENT_FRAME % 200) == 0) &&
	    (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
		PARTNER_PARA.poopLevel -= 1;
	}

	if (!isPoopy && (PARTNER_PARA.poopLevel <= 0)) {
		PARTNER_PARA.condition |= 8;
		PARTNER_PARA.poopingTimer =
			((PARTNER_PARA.discipline + 20) * 20 * 60) / 100;
	}

	if (isPoopy && (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
		PARTNER_PARA.poopingTimer -= 1;
	}

	if (isPoopy && (PARTNER_PARA.poopingTimer <= 0)) {
		setPartnerState(7);
		PARTNER_PARA.poopingTimer = -1;
		ITEM_SCOLD_FLAG = 1;
	}
}

void detectEdiblePoop(void)
{
	int16_t radius;
	int16_t tileX;
	int16_t tileZ;
	int16_t posX;
	int16_t posZ;
	int32_t i;

	if ((PARTNER_ENTITY.digimonEntity.entity.type != 0xb) &&
	    (PARTNER_ENTITY.digimonEntity.entity.type != 0x27)) {
		return;
	}

	for (i = 0; i < 100; ++i) {
		if ((WORLD_POOP[i].x == -1) ||
		    (WORLD_POOP[i].map != CURRENT_SCREEN)) {
			continue;
		}

		if (WORLD_POOP[i].size < 11) {
			radius = 200;
		} else {
			radius = 300;
		}

		tileX = ((WORLD_POOP[i].x - 50) * 100) + 50;
		tileZ = ((50 - WORLD_POOP[i].y) * 100) - 50;

		posX = TAMER_ENTITY.entity.posData->location.vx;
		posZ = TAMER_ENTITY.entity.posData->location.vz;

		if (((tileX - radius) < posX) && ((tileX + radius) > posX) &&
		    ((tileZ - radius) < posZ) && ((tileZ + radius) > posZ)) {
			POOP_TO_EAT = i;
			PARTNER_STATE = 9;
			PARTNER_SUB_STATE = 0;
			return;
		}
	}
}

void handleToilet(void)
{
	PARTNER_PARA.happiness += 2;
	PARTNER_PARA.discipline += 2;
	PARTNER_PARA.poopLevel =
		(RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].poopTimer +
		 RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].poopTimer);
	PARTNER_PARA.condition &= ~8;
	handlePoopWeightLoss(ENTITY_TABLE[1]->type);
}

void handlePoopWeightLoss(int32_t type)
{
	int32_t r;

	r = random(4);

	PARTNER_PARA.weight -= (r + RAISE_DATA[type].poopSize) / 4;
	if (PARTNER_PARA.weight < 1) {
		PARTNER_PARA.weight = 1;
	}
}

void handleWildPoop(void)
{
	PARTNER_PARA.condition &= ~8;
	PARTNER_PARA.careMistakes += 1;
	PARTNER_PARA.happiness -= 10;
	PARTNER_PARA.discipline -= 5;
	PARTNER_PARA.poopLevel =
		(RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].poopTimer +
		 RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].poopTimer);

	handlePoopWeightLoss(ENTITY_TABLE[1]->type);
}

void handleEatingPoop(void)
{
	Stats *stats;
	int16_t healingChance;
	int16_t stat;
	int16_t r;

	stats = &PARTNER_ENTITY.digimonEntity.stats;
	healingChance = 0;

	if (WORLD_POOP[POOP_TO_EAT].size < 11) {
		stats->current.currentHP += (int16_t)(stats->base.hp / 20);
		stats->current.currentMP += (int16_t)(stats->base.mp / 50);
		PARTNER_PARA.weight += 1;
		healingChance = 2;
	} else if (WORLD_POOP[POOP_TO_EAT].size < 14) {
		stats->current.currentHP += (int16_t)(stats->base.hp / 10);
		stats->current.currentMP += (int16_t)(stats->base.mp / 20);
		PARTNER_PARA.weight += 3;
		healingChance = 7;
	} else {
		stats->current.currentHP += (int16_t)(stats->base.hp / 2);
		stats->current.currentMP += (int16_t)(stats->base.mp / 10);
		PARTNER_PARA.weight += 10;
		healingChance = 20;
	}

	stat = stats->current.currentHP;
	if (stats->base.hp < stat) {
		stats->current.currentHP = stats->base.hp;
	}

	stat = stats->current.currentMP;
	if (stats->base.mp < stat) {
		stats->current.currentMP = stats->base.mp;
	}

	stat = PARTNER_PARA.weight;
	if (stat > 99) {
		PARTNER_PARA.weight = 99;
	}

	r = random(100);
	if (r < healingChance) {
		if ((PARTNER_PARA.condition & 0x20) != 0) {
			PARTNER_PARA.condition &= ~0x20;
			PARTNER_PARA.injuryTimer = 0;
		}

		if ((PARTNER_PARA.condition & 0x40) != 0) {
			PARTNER_PARA.condition &= ~0x40;
			PARTNER_PARA.sicknessTimer = 0;
			PARTNER_PARA.areaEffectTimer = 0;
		}
	}

	WORLD_POOP[POOP_TO_EAT].map = 0xff;
	WORLD_POOP[POOP_TO_EAT].x = -1;
	WORLD_POOP[POOP_TO_EAT].y = -1;
	WORLD_POOP[POOP_TO_EAT].size = 0;
}

void tickSicknessMechanics(void)
{
	int32_t wasSick;
	uint8_t chance;
	int16_t roll;
	int32_t nameLength;

	wasSick = PARTNER_PARA.condition & 0x40;
	if ((PARTNER_PARA.sicknessCounter >= 10) &&
	    ((CURRENT_FRAME % 1200) == 0) &&
	    (CURRENT_FRAME != LAST_HANDLED_FRAME) && !wasSick) {
		PARTNER_PARA.sicknessTries++;
		chance = (PARTNER_PARA.sicknessTries - 10) * 5 +
			 (PARTNER_PARA.tiredness - 50);
		roll = random(100);
		if (roll < chance) {
			PARTNER_PARA.condition |= 0x40;
			PARTNER_PARA.timesBeingSick++;
			PARTNER_PARA.sicknessTimer = 1;
			PARTNER_PARA.happiness -= 20;
			PARTNER_PARA.sicknessCounter = 0;
		}
	}

	if (!wasSick && (PARTNER_PARA.areaEffectTimer > 12000) &&
	    ((CURRENT_FRAME % 1200) == 0) &&
	    (CURRENT_FRAME != LAST_HANDLED_FRAME) &&
	    (PARTNER_AREA_RESPONSE[0] == 2)) {
		PARTNER_PARA.condition |= 0x40;
		PARTNER_PARA.timesBeingSick++;
		PARTNER_PARA.sicknessTimer = 1;
		PARTNER_PARA.areaEffectTimer = 0;
	}
	if (((CURRENT_FRAME % 1200) == 0) &&
	    (CURRENT_FRAME != LAST_HANDLED_FRAME) &&
	    ((PARTNER_PARA.condition & 0x40) != 0)) {
		PARTNER_PARA.happiness -= 10;
		PARTNER_PARA.discipline -= 5;
		PARTNER_PARA.tiredness += 5;
	}
	if (!wasSick && (PARTNER_PARA.sicknessTimer > 0))
		PARTNER_PARA.sicknessTimer = 0;

	if (((CURRENT_FRAME % 1200) == 0) &&
	    (CURRENT_FRAME != LAST_HANDLED_FRAME)) {
		if ((PARTNER_PARA.condition & 0x40) != 0)
			PARTNER_PARA.sicknessTimer++;
		if ((PARTNER_PARA.condition & 0x20) != 0)
			PARTNER_PARA.injuryTimer++;
	}
	if ((HAS_IMMORTAL_HOUR == 1) && (IMMORTAL_HOUR == HOUR))
		return;

	if ((PARTNER_PARA.injuryTimer >= 12) && !wasSick &&
	    (getTamerState() == 0)) {
		PARTNER_PARA.condition |= 0x40;
		PARTNER_PARA.timesBeingSick++;
		PARTNER_PARA.sicknessTimer = 1;
		PARTNER_PARA.condition &= ~0x20;
		PARTNER_PARA.injuryTimer = 0;
	}
	if (!wasSick && ((PARTNER_PARA.condition & 0x40) != 0)) {
		setTamerState(0x14);
		clearTextArea();
		setTextColor(10);
		drawString(PARTNER_ENTITY.name, 0, 120);
		nameLength = strlen(PARTNER_ENTITY.name) / 2;
		setTextColor(1);
		drawString(PARTNER_TEXT_IS_SICK, nameLength * 12, 120);
	}

	if ((PARTNER_PARA.sicknessTimer >= 12) && (PARTNER_STATE != 8) &&
	    (getTamerState() == 0) && (PARTNER_PARA.remainingLifetime != 0) &&
	    (IS_SCRIPT_PAUSED == 1)) {
		writePStat(0xFF, 0);
		PARTNER_ENTITY.lives--;
		callScriptSection(0, 0x4DE, 0);
		PARTNER_PARA.sicknessTimer = 0;
	}
}

void tickDeathCondition(void)
{
	if ((HAS_IMMORTAL_HOUR == 1) && (IMMORTAL_HOUR == HOUR)) {
		return;
	}

	if (PARTNER_PARA.remainingLifetime > 0) {
		return;
	}

	if (PARTNER_STATE == 8) {
		return;
	}

	if (getTamerState() != 0) {
		return;
	}

	if (IS_SCRIPT_PAUSED != 1) {
		return;
	}

	IS_NATURAL_DEATH = 1;

	writePStat(0xff, 0);
	PARTNER_PARA.remainingLifetime = 0;
	callScriptSection(0, 0x4de, 0);
}

void skipHours(int32_t hours)
{
	int16_t shortHours;
	int32_t rawHours;

	PARTNER_PARA.evoTimer += (int16_t)hours;
	shortHours = hours;
	PARTNER_PARA.remainingLifetime -= shortHours;
	rawHours = hours;
	if (HOUR + hours >= 24) {
		hours = HOUR + hours;
		PARTNER_PARA.age += (int16_t)(hours / 24);
	}
	updateTimeOfDay();

	if ((HOUR + (rawHours % 4) == 0) && (PARTNER_PARA.happiness < 80))
		PARTNER_PARA.remainingLifetime = PARTNER_PARA.remainingLifetime -
			((PARTNER_PARA.happiness - 80) / -50 + 1);
	if (PARTNER_PARA.remainingLifetime < 0)
		PARTNER_PARA.remainingLifetime = 0;

	if ((PARTNER_PARA.condition & 1) != 0) {
		PARTNER_PARA.sicknessCounter += shortHours;
		PARTNER_PARA.missedSleepHours += shortHours;
	}
	if ((PARTNER_PARA.condition & 4) != 0) {
		PARTNER_PARA.starvationTimer -= (int16_t)(rawHours * 120);
		if ((PARTNER_PARA.starvationTimer <= 0) &&
		    (PARTNER_PARA.energyLevel <
		     RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyThreshold))
			PARTNER_PARA.careMistakes++;
	} else {
		PARTNER_PARA.foodLevel -= (int16_t)(rawHours * 60);
	}
	PARTNER_PARA.energyLevel -=
		RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyUsage;
	if ((PARTNER_PARA.condition & 8) != 0)
		PARTNER_PARA.poopingTimer -= 1200;
	else
		PARTNER_PARA.poopLevel -= (int16_t)(rawHours * 6);

	if ((PARTNER_PARA.condition & 0x40) != 0)
		PARTNER_PARA.sicknessTries += shortHours;
	if ((PARTNER_PARA.condition & 0x20) != 0)
		PARTNER_PARA.injuryTimer += shortHours;
	if ((PARTNER_PARA.condition & 0x40) != 0)
		PARTNER_PARA.sicknessTimer += shortHours;
}

void handlePostBattleTiredness(void)
{
	int16_t maxMP;
	int16_t currentMP;
	int16_t diff;
	int16_t diff2;

	PARTNER_PARA.tiredness += 5;

	maxMP = PARTNER_ENTITY.digimonEntity.stats.base.mp;
	diff = (maxMP * 2) / 10;
	currentMP = PARTNER_ENTITY.digimonEntity.stats.current.currentMP;

	diff2 = diff;
	if (currentMP < (maxMP - diff)) {
		diff = maxMP - currentMP;
		/*
		 * NOTE: Vanilla MP factor calculation is broken.
		 * The factor should be ((diff * 10) / maxMP).
		 */
		PARTNER_PARA.tiredness += (diff / maxMP) * 10;
	}

	PARTNER_PARA.foodLevel -= 15;
}

void tickPartnerNormal(void)
{
	int16_t evoTimer;
	uint8_t level;
	int16_t tamerState;

	if (GAME_STATE != 0 || PARTNER_STATE != 1 || !IS_GAMETIME_RUNNING ||
	    CURRENT_FRAME == LAST_HANDLED_FRAME || FADE_PROTECTION == 1)
		return;
	tickDeathCondition();
	tickSleepMechanics();
	tickSicknessMechanics();
	tickTirednessMechanics();
	tickHungerMechanics();
	tickUnhappinessMechanics();
	tickConditionBoundaries();
	PARTNER_PARA.areaEffectTimer++;
	if (PARTNER_PARA.areaEffectTimer > 28800)
		PARTNER_PARA.areaEffectTimer = 0;
	/* Retail accesses trainBoostTimer as a signed halfword. */
	((int16_t *)&PARTNER_PARA)[0x50 / 2]--;
	if (((int16_t *)&PARTNER_PARA)[0x50 / 2] <= 0) {
		PARTNER_PARA.trainBoostFlag = 0;
		PARTNER_PARA.trainBoostTimer = 0;
	}
	if (CURRENT_FRAME % 4800 == 0 && PARTNER_PARA.happiness < 80) {
		PARTNER_PARA.remainingLifetime = PARTNER_PARA.remainingLifetime -
			((PARTNER_PARA.happiness - 80) / -50 + 1);
		if (PARTNER_PARA.remainingLifetime < 0)
			PARTNER_PARA.remainingLifetime = 0;
	}
	if (!isInDaytimeTransition())
		return;
	if (HAS_IMMORTAL_HOUR == 1 && IMMORTAL_HOUR == HOUR)
		return;
	if (HAS_IMMORTAL_HOUR == 1 && IMMORTAL_HOUR != HOUR) {
		HAS_IMMORTAL_HOUR = 0;
		IMMORTAL_HOUR = -1;
	}
	evoTimer = PARTNER_PARA.evoTimer;
	level = DIGIMON_DATA[PARTNER_ENTITY.digimonEntity.entity.type].level;
	tamerState = getTamerState();
	if (CURRENT_FRAME != LAST_HANDLED_FRAME && PARTNER_STATE == 1 &&
	    tamerState == 0) {
		if (level == 1 && evoTimer >= 6)
			EVOLUTION_TARGET = getFreshEvolutionTarget(
				PARTNER_ENTITY.digimonEntity.entity.type);
		else if (level == 2 && evoTimer >= 24)
			EVOLUTION_TARGET = getInTrainingEvolutionTarget(
				PARTNER_ENTITY.digimonEntity.entity.type);
		else if (level == 3 && evoTimer >= 72)
			EVOLUTION_TARGET = getRookieEvolutionTarget(
				PARTNER_ENTITY.digimonEntity.entity.type);
		else if (level == 4 && evoTimer >= 144)
			EVOLUTION_TARGET = getChampionEvolutionTarget(
				PARTNER_ENTITY.digimonEntity.entity.type);
		if (level == 4 && evoTimer == 360)
			EVOLUTION_TARGET = handleSpecialEvolutions(
				3, ENTITY_TABLE[1]);
	}
	if (PARTNER_PARA.virusBar >= 16) {
		/* MWCC requires this qualifier for the retail load/register sequence. */
		int32_t type = *(volatile int32_t *)
			&PARTNER_ENTITY.digimonEntity.entity.type;
		if (type != 0x27) {
			EVOLUTION_TARGET = 0x27;
			writePStat(5, type);
			PARTNER_PARA.sukaBackupHP =
				PARTNER_ENTITY.digimonEntity.stats.base.hp;
			PARTNER_PARA.sukaBackupMP =
				PARTNER_ENTITY.digimonEntity.stats.base.mp;
			PARTNER_PARA.sukaBackupOff =
				PARTNER_ENTITY.digimonEntity.stats.base.off;
			PARTNER_PARA.sukaBackupDef =
				PARTNER_ENTITY.digimonEntity.stats.base.def;
			PARTNER_PARA.sukaBackupSpeed =
				PARTNER_ENTITY.digimonEntity.stats.base.speed;
			PARTNER_PARA.sukaBackupBrain =
				PARTNER_ENTITY.digimonEntity.stats.base.brain;
			PARTNER_PARA.virusBar = 0;
		}
	}
	if (EVOLUTION_TARGET != -1 && PARTNER_STATE != 13) {
		setTamerState(6);
		setPartnerState(13);
	}
}

void renderStatusBars(int32_t state)
{
	uint8_t texX;
	uint8_t texY;
	int32_t count;
	int32_t i;

	if (state == 0)
		STATUS_UI_OFFSET_X += 50;
	else
		STATUS_UI_OFFSET_X -= 50;
	if (STATUS_UI_OFFSET_X > 240)
		STATUS_UI_OFFSET_X = 240;
	if (STATUS_UI_OFFSET_X < 75)
		STATUS_UI_OFFSET_X = 75;
	renderRectPolyFT4(STATUS_UI_OFFSET_X, 85, 75, 8, 0, 236, 5, 0x7D06, 15, 0);
	renderRectPolyFT4(STATUS_UI_OFFSET_X, 97, 75, 8, 0, 236, 5, 0x7D06, 15, 0);
	if (PARTNER_PARA.happiness >= 0)
		texX = 64;
	else
		texX = 97;
	if (PLAYTIME_FRAMES % 10 < 5) {
		if (PARTNER_AREA_RESPONSE[0] == 2)
			texX += 11;
		else if (PARTNER_AREA_RESPONSE[0] == 1)
			texX -= 11;
	}
	renderRectPolyFT4(STATUS_UI_OFFSET_X + 3, 83, 11, 11, texX, 244, 5, GetClut(96, 503), 14, 0);
	if (PARTNER_PARA.discipline >= 50)
		texX = 22;
	else
		texX = 33;
	renderRectPolyFT4(STATUS_UI_OFFSET_X + 3, 95, 11, 11, texX, 244, 5, GetClut(96, 498), 14, 0);
	if (PARTNER_PARA.happiness >= 0) {
		texY = 153;
		count = PARTNER_PARA.happiness / 4;
		for (i = 0; i < 25; i++)
			renderRectPolyFT4(STATUS_UI_OFFSET_X + 18 + i * 2, 88, 2, 2, 232, 151, 5, GetClut(96, 503), 14, 0);
	} else {
		texY = 151;
		count = (PARTNER_PARA.happiness + 100) / 4;
	}
	for (i = 0; i < count; i++)
		renderRectPolyFT4(STATUS_UI_OFFSET_X + 18 + i * 2, 88, 2, 2, 232, texY, 5, GetClut(96, 503), 13, 0);
	if (PARTNER_PARA.discipline >= 50) {
		texY = 153;
		count = (PARTNER_PARA.discipline - 50) / 2;
		for (i = 0; i < 25; i++)
			renderRectPolyFT4(STATUS_UI_OFFSET_X + 18 + i * 2, 100, 2, 2, 232, 151, 5, GetClut(96, 503), 14, 0);
	} else {
		texY = 151;
		count = PARTNER_PARA.discipline / 2;
	}
	for (i = 0; i < count; i++)
		renderRectPolyFT4(STATUS_UI_OFFSET_X + 18 + i * 2, 100, 4, 2, 232, texY, 5, GetClut(96, 503), 13, 0);
}

void setImmortalHour(void)
{
	HAS_IMMORTAL_HOUR = 1;
	IMMORTAL_HOUR = HOUR;
}
