#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include <libetc.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/anim.h>
#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/file.h>
#include <dw/file_queue.h>
#include <dw/model.h>
#include <dw/params.h>
#include <dw/world_object.h>

extern ModelComponent NPC_MODEL[5];
extern ModelComponent TAMER_MODEL;
extern ModelComponent PARTNER_MODEL;
extern int32_t NPC_MODEL_TAKEN[5];
extern int32_t UNKNOWN_MODEL_TAKEN[16];
extern uint8_t PARTNER_MODEL_BUFFER[];
extern uint8_t TAMER_MODEL_BUFFER[];

extern GsOT *ACTIVE_ORDERING_TABLE;
void renderDropShadow(Entity *entity);
void setRotTransMatrix(MATRIX *m);
int32_t add3DSpritePrim(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
void updateTMDTextureData(char *tmd, int32_t clutX, int32_t x, int32_t y, int32_t tpage);
extern PositionData TAMER_POSITION_DATA[];
extern PositionData PARTNER_POSITION_DATA[];
extern MomentumData TAMER_MOMENTUM_DATA[];
extern MomentumData PARTNER_MOMENTUM_DATA[];

void handleNullModel(void);
void concatStrings(char *dst, char *src1, char *src2);
void initializePosData(PositionData *posData);
void loadDigimonTexture(int32_t digiType, char *path,
                        ModelComponent *component);
ModelComponent *loadMMD(int32_t digiType, int32_t modelType);
void uploadModelTexture(void *textureData, ModelComponent *component);

void renderFlatDigimon(Entity *entity);
void renderDigimon(/* int32_t instanceId */);
void renderWireframed(GsDOBJ2 *obj, int32_t wireFrameShare);
void uploadModelTexture(void *textureData, ModelComponent *component);

static void *model_functions[] = {
	applyMMD,
	loadMMDAsync,
	uploadModelTexture,
	getEntityType,
	getEntityModelComponent,
	unloadModel,
	loadMMD,
	initializeModelComponents,
	handleNullModel,
	concatStrings,
	loadDigimonTexture,
	resetFlattenGlobal,
	renderWireframed,
	setEntityRotation,
	setEntityPosition,
	setupEntityMatrix,
	removeEntity,
	renderDigimon,
	initializeDigimonObject,
	thunkUnloadModel,
	thunkLoadMMD,
	renderFlatDigimon,
	initializePosData,
};

static inline int32_t applyTPageOffset(int32_t tpageOffset,
                                       int32_t pixelOffset)
{
	return tpageOffset + pixelOffset;
}

// clang-format off
SkeletonBone MAIN_D_80133B2C[3] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
};

int32_t PARTNER_WIREFRAME_TOTAL = 0x00000010;

int32_t ENTITY1_WIREFRAME_TOTAL = 0x00000010;

int32_t PLAYER_SHADOW_ENABLED = 0x00000001;

int16_t WIREFRAME_COLOR_MIN = 0x0037;

int16_t WIREFRAME_COLOR_MAX = 0x00ff;

char DIGIMON_FILE_BOYS[] = "BOYS";

char DIGIMON_FILE_BOTA[] = "BOTA";

char DIGIMON_FILE_KORO[] = "KORO";

char DIGIMON_FILE_AGUM[] = "AGUM";

char DIGIMON_FILE_BETA[] = "BETA";

char DIGIMON_FILE_GREY[] = "GREY";

char DIGIMON_FILE_DEVI[] = "DEVI";

char DIGIMON_FILE_AIRD[] = "AIRD";

char DIGIMON_FILE_TYRA[] = "TYRA";

char DIGIMON_FILE_MERA[] = "MERA";

char DIGIMON_FILE_SEAD[] = "SEAD";

char DIGIMON_FILE_NUME[] = "NUME";

char DIGIMON_FILE_MTGR[] = "MTGR";

char DIGIMON_FILE_MAME[] = "MAME";

char DIGIMON_FILE_MONZ[] = "MONZ";

char DIGIMON_FILE_PUNI[] = "PUNI";

char DIGIMON_FILE_TUNO[] = "TUNO";

char DIGIMON_FILE_GABU[] = "GABU";

char DIGIMON_FILE_ELEC[] = "ELEC";

char DIGIMON_FILE_KABU[] = "KABU";

char DIGIMON_FILE_ANGE[] = "ANGE";

char DIGIMON_FILE_BIRD[] = "BIRD";

char DIGIMON_FILE_GARU[] = "GARU";

char DIGIMON_FILE_YUKI[] = "YUKI";

char DIGIMON_FILE_HOEE[] = "HOEE";

char DIGIMON_FILE_VEGI[] = "VEGI";

char DIGIMON_FILE_SKUL[] = "SKUL";

char DIGIMON_FILE_MTMA[] = "MTMA";

char DIGIMON_FILE_VEDA[] = "VEDA";

char DIGIMON_FILE_POYO[] = "POYO";

char DIGIMON_FILE_TOKO[] = "TOKO";

char DIGIMON_FILE_PATA[] = "PATA";

char DIGIMON_FILE_KUNE[] = "KUNE";

char DIGIMON_FILE_UNIM[] = "UNIM";

char DIGIMON_FILE_OGRE[] = "OGRE";

char DIGIMON_FILE_SHEL[] = "SHEL";

char DIGIMON_FILE_CENT[] = "CENT";

char DIGIMON_FILE_BAKE[] = "BAKE";

char DIGIMON_FILE_DORI[] = "DORI";

char DIGIMON_FILE_SCUM[] = "SCUM";

char DIGIMON_FILE_ANDR[] = "ANDR";

char DIGIMON_FILE_GIRO[] = "GIRO";

char DIGIMON_FILE_ETEM[] = "ETEM";

char DIGIMON_FILE_YURA[] = "YURA";

char DIGIMON_FILE_TANE[] = "TANE";

char DIGIMON_FILE_PIYO[] = "PIYO";

char DIGIMON_FILE_PALM[] = "PALM";

char DIGIMON_FILE_MONO[] = "MONO";

char DIGIMON_FILE_LEOM[] = "LEOM";

char DIGIMON_FILE_SIRA[] = "SIRA";

char DIGIMON_FILE_COCA[] = "COCA";

char DIGIMON_FILE_KUWA[] = "KUWA";

char DIGIMON_FILE_MOJA[] = "MOJA";

char DIGIMON_FILE_NANI[] = "NANI";

char DIGIMON_FILE_MGDR[] = "MGDR";

char DIGIMON_FILE_PICC[] = "PICC";

char DIGIMON_FILE_DIGI[] = "DIGI";

char DIGIMON_FILE_PENM[] = "PENM";

char DIGIMON_FILE_IGAM[] = "IGAM";

char DIGIMON_FILE_HOUO[] = "HOUO";

char DIGIMON_FILE_HKAB[] = "HKAB";

char DIGIMON_FILE_MGSD[] = "MGSD";

char DIGIMON_FILE_WEAG[] = "wEAG";

char DIGIMON_FILE_PANJ[] = "PANJ";

char DIGIMON_FILE_GGDR[] = "GGDR";

char DIGIMON_FILE_MTET[] = "MTET";

char DIGIMON_FILE_VAND[] = "VAND";

char DIGIMON_FILE_YANM[] = "YANM";

char DIGIMON_FILE_GOTU[] = "GOTU";

char DIGIMON_FILE_FLAR[] = "FLAR";

char DIGIMON_FILE_WARU[] = "WARU";

char DIGIMON_FILE_YKAG[] = "YKAG";

char DIGIMON_FILE_HYOG[] = "HYOG";

char DIGIMON_FILE_PCSC[] = "PCSC";

char DIGIMON_FILE_DOKU[] = "DOKU";

char DIGIMON_FILE_SIMA[] = "SIMA";

char DIGIMON_FILE_TANK[] = "TANK";

char DIGIMON_FILE_REDV[] = "REDV";

char DIGIMON_FILE_JMOJ[] = "JMOJ";

char DIGIMON_FILE_NISE[] = "NISE";

char DIGIMON_FILE_GOBR[] = "GOBR";

char DIGIMON_FILE_TUTI[] = "TUTI";

char DIGIMON_FILE_PSYC[] = "PSYC";

char DIGIMON_FILE_MODO[] = "MODO";

char DIGIMON_FILE_TOYA[] = "TOYA";

char DIGIMON_FILE_PIDD[] = "PIDD";

char DIGIMON_FILE_ARUR[] = "ARUR";

char DIGIMON_FILE_GERE[] = "GERE";

char DIGIMON_FILE_VARM[] = "VARM";

char DIGIMON_FILE_FUGA[] = "FUGA";

char DIGIMON_FILE_TKKA[] = "TKKA";

char DIGIMON_FILE_MRIS[] = "MRIS";

char DIGIMON_FILE_GARD[] = "GARD";

char DIGIMON_FILE_MCHO[] = "MCHO";

char DIGIMON_FILE_ICEM[] = "ICEM";

char DIGIMON_FILE_AKAT[] = "AKAT";

char DIGIMON_FILE_TUKA[] = "TUKA";

char DIGIMON_FILE_SHAM[] = "SHAM";

char DIGIMON_FILE_CLEA[] = "CLEA";

char DIGIMON_FILE_ZASS[] = "ZASS";

char DIGIMON_FILE_ICDV[] = "ICDV";

char DIGIMON_FILE_DKRZ[] = "DKRZ";

char DIGIMON_FILE_SNDY[] = "SNDY";

char DIGIMON_FILE_SNGB[] = "SNGB";

char DIGIMON_FILE_BLMR[] = "BLMR";

char DIGIMON_FILE_GRUR[] = "GRUR";

char DIGIMON_FILE_SABD[] = "SABD";

char DIGIMON_FILE_SOUL[] = "SOUL";

char DIGIMON_FILE_GOLE[] = "GOLE";

char DIGIMON_FILE_OTAM[] = "OTAM";

char DIGIMON_FILE_GECO[] = "GECO";

char DIGIMON_FILE_TENT[] = "TENT";

char DIGIMON_FILE_WRSE[] = "WRSE";

char DIGIMON_FILE_INSE[] = "INSE";

char DIGIMON_FILE_TAKA[] = "tAKA";

char DIGIMON_FILE_MUGE[] = "MUGE";

char DIGIMON_FILE_ANLG[] = "ANLG";

char DIGIMON_FILE_JIJI[] = "JIJI";

char DIGIMON_FILE_TENS[] = "TENS";

char DIGIMON_FILE_TONO[] = "TONO";

char DIGIMON_FILE_SCUD[] = "SCUD";

char DIGIMON_FILE_JURE[] = "JURE";

char DIGIMON_FILE_HAGU[] = "HAGU";

char DIGIMON_FILE_BRIK[] = "BRIK";

char DIGIMON_FILE_TIRS[] = "TIRS";

char DIGIMON_FILE_EGOB[] = "EGOB";

char DIGIMON_FILE_BRAK[] = "BRAK";

char DIGIMON_FILE_PUTI[] = "PUTI";

char DIGIMON_FILE_EBET[] = "EBET";

char DIGIMON_FILE_EGRE[] = "EGRE";

char DIGIMON_FILE_EDEV[] = "EDEV";

char DIGIMON_FILE_EAIR[] = "EAIR";

char DIGIMON_FILE_ETYR[] = "ETYR";

char DIGIMON_FILE_EMER[] = "EMER";

char DIGIMON_FILE_ESEA[] = "ESEA";

char DIGIMON_FILE_ENUM[] = "ENUM";

char DIGIMON_FILE_EMTG[] = "EMTG";

char DIGIMON_FILE_EMAM[] = "EMAM";

char DIGIMON_FILE_EMON[] = "EMON";

char DIGIMON_FILE_EGAB[] = "EGAB";

char DIGIMON_FILE_EELE[] = "EELE";

char DIGIMON_FILE_EKAB[] = "EKAB";

char DIGIMON_FILE_EANG[] = "EANG";

char DIGIMON_FILE_EBIR[] = "EBIR";

char DIGIMON_FILE_EGAR[] = "EGAR";

char DIGIMON_FILE_EYUK[] = "EYUK";

char DIGIMON_FILE_EHOE[] = "EHOE";

char DIGIMON_FILE_EVEG[] = "EVEG";

char DIGIMON_FILE_ESKU[] = "ESKU";

char DIGIMON_FILE_EMTM[] = "EMTM";

char DIGIMON_FILE_EVED[] = "EVED";

char DIGIMON_FILE_EPAT[] = "EPAT";

char DIGIMON_FILE_EKUN[] = "EKUN";

char DIGIMON_FILE_EUNI[] = "EUNI";

char DIGIMON_FILE_EOGR[] = "EOGR";

char DIGIMON_FILE_ESHE[] = "ESHE";

char DIGIMON_FILE_ECEN[] = "ECEN";

char DIGIMON_FILE_EBAK[] = "EBAK";

char DIGIMON_FILE_EDOR[] = "EDOR";

char DIGIMON_FILE_ESCU[] = "ESCU";

char DIGIMON_FILE_EAND[] = "EAND";

char DIGIMON_FILE_EGIR[] = "EGIR";

char DIGIMON_FILE_EETE[] = "EETE";

char DIGIMON_FILE_EPIY[] = "EPIY";

char DIGIMON_FILE_EPAL[] = "EPAL";

char DIGIMON_FILE_EMNO[] = "EMNO";

char DIGIMON_FILE_ELEO[] = "ELEO";

char DIGIMON_FILE_ESIR[] = "ESIR";

char DIGIMON_FILE_ECOC[] = "ECOC";

char DIGIMON_FILE_EKUW[] = "EKUW";

char DIGIMON_FILE_EMOJ[] = "EMOJ";

char DIGIMON_FILE_ENAN[] = "ENAN";

char DIGIMON_FILE_EMGD[] = "EMGD";

char DIGIMON_FILE_EPIC[] = "EPIC";

char DIGIMON_FILE_EDIG[] = "EDIG";

char DIGIMON_FILE_EIGA[] = "EIGA";

char DIGIMON_FILE_EPEN[] = "EPEN";

char DIGIMON_FILE_EVAN[] = "EVAN";

char DIGIMON_FILE_CEGR[] = "CEGR";

char DIGIMON_FILE_CEMG[] = "CEMG";

char FILE_EXT_MMD[] = ".MMD";

char FILE_EXT_TMD[] = ".TMD";

char FILE_EXT_MTN[] = ".MTN";

SkeletonBone MAIN_D_8011C170[17] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x02 },
	{ 0x03, 0x04 },
	{ 0x04, 0x05 },
	{ 0x05, 0x02 },
	{ 0x06, 0x07 },
	{ 0x07, 0x08 },
	{ 0xff, 0x01 },
	{ 0x08, 0x0a },
	{ 0x09, 0x0b },
	{ 0x0a, 0x0c },
	{ 0x0b, 0x0a },
	{ 0x0c, 0x0e },
	{ 0x0d, 0x0f },
};

SkeletonBone MAIN_D_8011C194[22] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0xff, 0x02 },
	{ 0x04, 0x05 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
	{ 0xff, 0x02 },
	{ 0x07, 0x09 },
	{ 0x08, 0x0a },
	{ 0x09, 0x0b },
	{ 0x03, 0x01 },
	{ 0xff, 0x0d },
	{ 0x0a, 0x0e },
	{ 0x0b, 0x0f },
	{ 0x0c, 0x10 },
	{ 0xff, 0x0d },
	{ 0x0d, 0x12 },
	{ 0x0e, 0x13 },
	{ 0x0f, 0x14 },
};

SkeletonBone MAIN_D_8011C1C0[24] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0xff, 0x01 },
	{ 0x01, 0x02 },
	{ 0xff, 0x03 },
	{ 0x02, 0x04 },
	{ 0x06, 0x02 },
	{ 0x07, 0x06 },
	{ 0xff, 0x02 },
	{ 0x08, 0x08 },
	{ 0x09, 0x09 },
	{ 0xff, 0x02 },
	{ 0x0c, 0x0b },
	{ 0x0d, 0x0c },
	{ 0x00, 0x01 },
	{ 0x03, 0x0e },
	{ 0x04, 0x0f },
	{ 0x05, 0x10 },
	{ 0xff, 0x0e },
	{ 0x0a, 0x12 },
	{ 0x0b, 0x13 },
	{ 0xff, 0x0e },
	{ 0x0e, 0x15 },
	{ 0x0f, 0x16 },
};

SkeletonBone MAIN_D_8011C1F0[26] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0xff, 0x02 },
	{ 0x04, 0x06 },
	{ 0x05, 0x07 },
	{ 0x06, 0x08 },
	{ 0xff, 0x02 },
	{ 0x0a, 0x0a },
	{ 0x0b, 0x0b },
	{ 0x0c, 0x0c },
	{ 0x10, 0x01 },
	{ 0x11, 0x0e },
	{ 0x12, 0x0f },
	{ 0x13, 0x10 },
	{ 0xff, 0x0e },
	{ 0x07, 0x12 },
	{ 0x08, 0x13 },
	{ 0x09, 0x14 },
	{ 0xff, 0x0e },
	{ 0x0d, 0x16 },
	{ 0x0e, 0x17 },
	{ 0x0f, 0x18 },
};

SkeletonBone MAIN_D_8011C224[28] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0xff, 0x02 },
	{ 0x01, 0x03 },
	{ 0xff, 0x02 },
	{ 0x05, 0x05 },
	{ 0x06, 0x06 },
	{ 0x07, 0x07 },
	{ 0x08, 0x08 },
	{ 0xff, 0x02 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x0d },
	{ 0xff, 0x02 },
	{ 0x02, 0x0f },
	{ 0xff, 0x02 },
	{ 0x03, 0x11 },
	{ 0x04, 0x01 },
	{ 0xff, 0x13 },
	{ 0x0d, 0x14 },
	{ 0x0e, 0x15 },
	{ 0x0f, 0x16 },
	{ 0xff, 0x13 },
	{ 0x10, 0x18 },
	{ 0x11, 0x19 },
	{ 0x12, 0x1a },
};

SkeletonBone MAIN_D_8011C25C[27] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x01, 0x01 },
	{ 0x02, 0x02 },
	{ 0x03, 0x03 },
	{ 0x07, 0x02 },
	{ 0x08, 0x05 },
	{ 0x09, 0x06 },
	{ 0x0b, 0x07 },
	{ 0x0a, 0x07 },
	{ 0x0c, 0x07 },
	{ 0x10, 0x02 },
	{ 0x11, 0x0b },
	{ 0x12, 0x0c },
	{ 0x14, 0x0d },
	{ 0x13, 0x0d },
	{ 0x15, 0x0d },
	{ 0x00, 0x01 },
	{ 0x04, 0x11 },
	{ 0x05, 0x12 },
	{ 0x06, 0x13 },
	{ 0x0d, 0x11 },
	{ 0x0e, 0x15 },
	{ 0x0f, 0x16 },
	{ 0x16, 0x11 },
	{ 0x17, 0x18 },
	{ 0x18, 0x19 },
};

SkeletonBone MAIN_D_8011C294[22] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0xff, 0x02 },
	{ 0x01, 0x03 },
	{ 0xff, 0x02 },
	{ 0x02, 0x05 },
	{ 0x03, 0x06 },
	{ 0x04, 0x07 },
	{ 0xff, 0x02 },
	{ 0x05, 0x09 },
	{ 0x06, 0x0a },
	{ 0x07, 0x0b },
	{ 0x08, 0x01 },
	{ 0xff, 0x0d },
	{ 0x09, 0x0e },
	{ 0x0a, 0x0f },
	{ 0x0b, 0x10 },
	{ 0xff, 0x0d },
	{ 0x0c, 0x12 },
	{ 0x0d, 0x13 },
	{ 0x0e, 0x14 },
};

SkeletonBone MAIN_D_8011C2C0[30] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0xff, 0x02 },
	{ 0x04, 0x06 },
	{ 0x05, 0x07 },
	{ 0x06, 0x08 },
	{ 0xff, 0x02 },
	{ 0x07, 0x0a },
	{ 0x08, 0x0b },
	{ 0x09, 0x0c },
	{ 0xff, 0x02 },
	{ 0x0a, 0x0e },
	{ 0xff, 0x02 },
	{ 0x0b, 0x10 },
	{ 0x0c, 0x01 },
	{ 0x0d, 0x12 },
	{ 0x0e, 0x13 },
	{ 0x0f, 0x14 },
	{ 0xff, 0x12 },
	{ 0x10, 0x16 },
	{ 0x11, 0x17 },
	{ 0x12, 0x18 },
	{ 0xff, 0x12 },
	{ 0x13, 0x1a },
	{ 0x14, 0x1b },
	{ 0x15, 0x1c },
};

SkeletonBone MAIN_D_8011C2FC[14] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0xff, 0x02 },
	{ 0x01, 0x03 },
	{ 0xff, 0x02 },
	{ 0x02, 0x05 },
	{ 0xff, 0x02 },
	{ 0x03, 0x07 },
	{ 0xff, 0x01 },
	{ 0xff, 0x09 },
	{ 0x04, 0x0a },
	{ 0xff, 0x09 },
	{ 0x05, 0x0c },
};

SkeletonBone MAIN_D_8011C318[23] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0xff, 0x02 },
	{ 0x04, 0x06 },
	{ 0x05, 0x07 },
	{ 0x06, 0x08 },
	{ 0xff, 0x02 },
	{ 0x07, 0x0a },
	{ 0x08, 0x0b },
	{ 0x09, 0x0c },
	{ 0x0a, 0x01 },
	{ 0xff, 0x0e },
	{ 0x0b, 0x0f },
	{ 0x0c, 0x10 },
	{ 0x0d, 0x11 },
	{ 0xff, 0x0e },
	{ 0x0e, 0x13 },
	{ 0x0f, 0x14 },
	{ 0x10, 0x15 },
};

SkeletonBone MAIN_D_8011C348[21] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0xff, 0x02 },
	{ 0x03, 0x05 },
	{ 0x04, 0x06 },
	{ 0x05, 0x07 },
	{ 0xff, 0x02 },
	{ 0x06, 0x09 },
	{ 0x07, 0x0a },
	{ 0x08, 0x0b },
	{ 0xff, 0x01 },
	{ 0xff, 0x0d },
	{ 0x0c, 0x0e },
	{ 0xff, 0x0e },
	{ 0xff, 0x10 },
	{ 0x09, 0x11 },
	{ 0x0a, 0x12 },
	{ 0x0b, 0x13 },
};

SkeletonBone MAIN_D_8011C374[27] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0xff, 0x02 },
	{ 0x01, 0x03 },
	{ 0x02, 0x04 },
	{ 0xff, 0x02 },
	{ 0x03, 0x06 },
	{ 0x04, 0x07 },
	{ 0x05, 0x08 },
	{ 0xff, 0x02 },
	{ 0x06, 0x0a },
	{ 0x07, 0x0b },
	{ 0x08, 0x0c },
	{ 0x09, 0x01 },
	{ 0xff, 0x0e },
	{ 0x0a, 0x0f },
	{ 0x0b, 0x10 },
	{ 0x0c, 0x11 },
	{ 0xff, 0x0e },
	{ 0x0d, 0x13 },
	{ 0x0e, 0x14 },
	{ 0x0f, 0x15 },
	{ 0xff, 0x0e },
	{ 0x10, 0x17 },
	{ 0x11, 0x18 },
	{ 0x12, 0x19 },
};

SkeletonBone MAIN_D_8011C3AC[19] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0xff, 0x02 },
	{ 0x02, 0x04 },
	{ 0x03, 0x05 },
	{ 0x04, 0x06 },
	{ 0xff, 0x02 },
	{ 0x05, 0x08 },
	{ 0x06, 0x09 },
	{ 0x07, 0x0a },
	{ 0xff, 0x01 },
	{ 0xff, 0x0c },
	{ 0x08, 0x0d },
	{ 0x09, 0x0e },
	{ 0xff, 0x0c },
	{ 0x0a, 0x10 },
	{ 0x0b, 0x11 },
};

SkeletonBone MAIN_D_8011C3D4[21] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0xff, 0x02 },
	{ 0x02, 0x04 },
	{ 0x03, 0x05 },
	{ 0x04, 0x06 },
	{ 0xff, 0x02 },
	{ 0x05, 0x08 },
	{ 0x06, 0x09 },
	{ 0x07, 0x0a },
	{ 0x08, 0x01 },
	{ 0xff, 0x0c },
	{ 0x09, 0x0d },
	{ 0x0a, 0x0e },
	{ 0x0b, 0x0f },
	{ 0xff, 0x0c },
	{ 0x0c, 0x11 },
	{ 0x0d, 0x12 },
	{ 0x0e, 0x13 },
};

SkeletonBone MAIN_D_8011C400[11] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0xff, 0x02 },
	{ 0x01, 0x03 },
	{ 0x02, 0x04 },
	{ 0x03, 0x05 },
	{ 0xff, 0x02 },
	{ 0x04, 0x07 },
	{ 0x05, 0x08 },
	{ 0x06, 0x09 },
};

SkeletonBone MAIN_D_8011C418[26] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x0a, 0x02 },
	{ 0x0b, 0x04 },
	{ 0x0c, 0x05 },
	{ 0x0d, 0x02 },
	{ 0x0e, 0x07 },
	{ 0x0f, 0x08 },
	{ 0x03, 0x02 },
	{ 0x02, 0x0a },
	{ 0x07, 0x02 },
	{ 0x06, 0x0c },
	{ 0x04, 0x02 },
	{ 0x05, 0x02 },
	{ 0x08, 0x02 },
	{ 0x09, 0x02 },
	{ 0x10, 0x01 },
	{ 0x12, 0x12 },
	{ 0x13, 0x13 },
	{ 0x14, 0x14 },
	{ 0x15, 0x12 },
	{ 0x16, 0x16 },
	{ 0x17, 0x17 },
	{ 0x11, 0x12 },
};

SkeletonBone MAIN_D_8011C44C[31] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0xff, 0x03 },
	{ 0x0e, 0x06 },
	{ 0xff, 0x03 },
	{ 0x0a, 0x08 },
	{ 0xff, 0x03 },
	{ 0x0b, 0x0a },
	{ 0xff, 0x03 },
	{ 0x0c, 0x0c },
	{ 0xff, 0x03 },
	{ 0x0d, 0x0e },
	{ 0xff, 0x02 },
	{ 0x04, 0x10 },
	{ 0x05, 0x11 },
	{ 0x06, 0x12 },
	{ 0xff, 0x02 },
	{ 0x07, 0x14 },
	{ 0x08, 0x15 },
	{ 0x09, 0x16 },
	{ 0xff, 0x01 },
	{ 0xff, 0x18 },
	{ 0x0f, 0x19 },
	{ 0x10, 0x1a },
	{ 0xff, 0x18 },
	{ 0x11, 0x1c },
	{ 0x12, 0x1d },
};

SkeletonBone MAIN_D_8011C48C[27] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x02 },
	{ 0x05, 0x02 },
	{ 0x06, 0x02 },
	{ 0x07, 0x08 },
	{ 0x08, 0x09 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x02 },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x0d },
	{ 0x0d, 0x0e },
	{ 0x0e, 0x02 },
	{ 0x0f, 0x10 },
	{ 0x10, 0x11 },
	{ 0x11, 0x12 },
	{ 0x12, 0x13 },
	{ 0x13, 0x10 },
	{ 0x14, 0x15 },
	{ 0x15, 0x16 },
	{ 0x16, 0x17 },
	{ 0x17, 0x10 },
	{ 0x18, 0x19 },
};

SkeletonBone MAIN_D_8011C4C4[20] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0xff, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0xff, 0x03 },
	{ 0x03, 0x05 },
	{ 0xff, 0x02 },
	{ 0x04, 0x07 },
	{ 0x05, 0x08 },
	{ 0x06, 0x09 },
	{ 0x07, 0x0a },
	{ 0x08, 0x0b },
	{ 0xff, 0x02 },
	{ 0x09, 0x0d },
	{ 0x0a, 0x0e },
	{ 0x0b, 0x0f },
	{ 0x0c, 0x10 },
	{ 0x0d, 0x11 },
	{ 0x00, 0x01 },
};

SkeletonBone MAIN_D_8011C4EC[22] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0xff, 0x02 },
	{ 0x03, 0x05 },
	{ 0x04, 0x06 },
	{ 0x05, 0x07 },
	{ 0xff, 0x02 },
	{ 0x06, 0x09 },
	{ 0x07, 0x0a },
	{ 0x08, 0x0b },
	{ 0xff, 0x02 },
	{ 0x09, 0x0d },
	{ 0xff, 0x01 },
	{ 0xff, 0x0f },
	{ 0x0a, 0x10 },
	{ 0x0b, 0x11 },
	{ 0xff, 0x0f },
	{ 0x0c, 0x13 },
	{ 0x0d, 0x14 },
};

SkeletonBone MAIN_D_8011C518[28] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x0b, 0x01 },
	{ 0x08, 0x02 },
	{ 0x05, 0x03 },
	{ 0x00, 0x04 },
	{ 0x01, 0x05 },
	{ 0x02, 0x05 },
	{ 0xff, 0x05 },
	{ 0x03, 0x08 },
	{ 0xff, 0x05 },
	{ 0x04, 0x0a },
	{ 0xff, 0x04 },
	{ 0x06, 0x0c },
	{ 0xff, 0x04 },
	{ 0x07, 0x0e },
	{ 0xff, 0x03 },
	{ 0x09, 0x10 },
	{ 0xff, 0x03 },
	{ 0x0a, 0x12 },
	{ 0xff, 0x02 },
	{ 0x0c, 0x14 },
	{ 0xff, 0x02 },
	{ 0x0d, 0x16 },
	{ 0x0e, 0x01 },
	{ 0x0f, 0x18 },
	{ 0x10, 0x19 },
	{ 0x11, 0x1a },
};

SkeletonBone MAIN_D_8011C550[11] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x02 },
	{ 0x04, 0x05 },
	{ 0x05, 0x02 },
	{ 0x06, 0x02 },
	{ 0x07, 0x02 },
	{ 0x08, 0x02 },
};

SkeletonBone MAIN_D_8011C568[27] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x07, 0x01 },
	{ 0x00, 0x02 },
	{ 0x01, 0x03 },
	{ 0x02, 0x03 },
	{ 0x03, 0x05 },
	{ 0x04, 0x06 },
	{ 0x05, 0x03 },
	{ 0x06, 0x08 },
	{ 0x08, 0x02 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x02 },
	{ 0x0c, 0x0d },
	{ 0x0d, 0x0e },
	{ 0x0e, 0x01 },
	{ 0x17, 0x10 },
	{ 0x18, 0x11 },
	{ 0x0f, 0x10 },
	{ 0x10, 0x13 },
	{ 0x11, 0x14 },
	{ 0x12, 0x15 },
	{ 0x13, 0x10 },
	{ 0x14, 0x17 },
	{ 0x15, 0x18 },
	{ 0x16, 0x19 },
};

SkeletonBone MAIN_D_8011C5A0[7] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x02 },
	{ 0x04, 0x05 },
};

SkeletonBone MAIN_D_8011C5B0[20] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0xff, 0x02 },
	{ 0x01, 0x03 },
	{ 0x02, 0x04 },
	{ 0x03, 0x05 },
	{ 0xff, 0x02 },
	{ 0x04, 0x07 },
	{ 0x05, 0x08 },
	{ 0x06, 0x09 },
	{ 0xff, 0x01 },
	{ 0xff, 0x0b },
	{ 0x07, 0x0c },
	{ 0x08, 0x0d },
	{ 0x09, 0x0e },
	{ 0xff, 0x0b },
	{ 0x0a, 0x10 },
	{ 0x0b, 0x11 },
	{ 0x0c, 0x12 },
};

SkeletonBone MAIN_D_8011C5D8[26] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x02 },
	{ 0xff, 0x02 },
	{ 0x05, 0x07 },
	{ 0x06, 0x08 },
	{ 0x07, 0x09 },
	{ 0xff, 0x02 },
	{ 0x08, 0x0b },
	{ 0x09, 0x0c },
	{ 0x0a, 0x0d },
	{ 0x0b, 0x01 },
	{ 0xff, 0x0f },
	{ 0x0e, 0x10 },
	{ 0x0f, 0x11 },
	{ 0x10, 0x12 },
	{ 0xff, 0x0f },
	{ 0x11, 0x14 },
	{ 0x12, 0x15 },
	{ 0x13, 0x16 },
	{ 0x0c, 0x0f },
	{ 0x0d, 0x18 },
};

SkeletonBone MAIN_D_8011C60C[22] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x05 },
	{ 0x05, 0x03 },
	{ 0x06, 0x07 },
	{ 0x07, 0x08 },
	{ 0x08, 0x01 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x01 },
	{ 0x0d, 0x0e },
	{ 0x0e, 0x0f },
	{ 0x0f, 0x10 },
	{ 0x10, 0x01 },
	{ 0x11, 0x12 },
	{ 0x12, 0x13 },
	{ 0x13, 0x14 },
};

SkeletonBone MAIN_D_8011C638[10] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x02 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
	{ 0x07, 0x02 },
};

SkeletonBone MAIN_D_8011C64C[24] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x02 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
	{ 0x07, 0x02 },
	{ 0x08, 0x09 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x02 },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x0d },
	{ 0x0d, 0x0e },
	{ 0x0e, 0x02 },
	{ 0x0f, 0x10 },
	{ 0x10, 0x11 },
	{ 0x11, 0x12 },
	{ 0x12, 0x02 },
	{ 0x13, 0x14 },
	{ 0x14, 0x15 },
	{ 0x15, 0x16 },
};

SkeletonBone MAIN_D_8011C67C[9] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x02 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
};

SkeletonBone MAIN_D_8011C690[26] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0xff, 0x02 },
	{ 0x01, 0x03 },
	{ 0x02, 0x04 },
	{ 0x03, 0x05 },
	{ 0xff, 0x02 },
	{ 0x04, 0x07 },
	{ 0x05, 0x08 },
	{ 0x06, 0x09 },
	{ 0xff, 0x0a },
	{ 0x07, 0x0b },
	{ 0xff, 0x02 },
	{ 0x08, 0x0d },
	{ 0xff, 0x02 },
	{ 0x09, 0x0f },
	{ 0xff, 0x01 },
	{ 0xff, 0x11 },
	{ 0x0a, 0x12 },
	{ 0x0b, 0x13 },
	{ 0x0c, 0x14 },
	{ 0xff, 0x11 },
	{ 0x0d, 0x16 },
	{ 0x0e, 0x17 },
	{ 0x0f, 0x18 },
};

SkeletonBone MAIN_D_8011C6C4[28] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x02 },
	{ 0xff, 0x02 },
	{ 0x04, 0x06 },
	{ 0x05, 0x07 },
	{ 0x06, 0x08 },
	{ 0x07, 0x08 },
	{ 0x08, 0x08 },
	{ 0xff, 0x02 },
	{ 0x09, 0x0c },
	{ 0x0a, 0x0d },
	{ 0x0b, 0x0e },
	{ 0x0c, 0x0e },
	{ 0x0d, 0x0e },
	{ 0x0e, 0x01 },
	{ 0x0f, 0x12 },
	{ 0xff, 0x12 },
	{ 0x10, 0x14 },
	{ 0x11, 0x15 },
	{ 0x12, 0x16 },
	{ 0xff, 0x12 },
	{ 0x13, 0x18 },
	{ 0x14, 0x19 },
	{ 0x15, 0x1a },
};

SkeletonBone MAIN_D_8011C6FC[12] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x01, 0x01 },
	{ 0x02, 0x02 },
	{ 0x00, 0x01 },
	{ 0x05, 0x04 },
	{ 0x06, 0x05 },
	{ 0x07, 0x06 },
	{ 0xff, 0x04 },
	{ 0x03, 0x08 },
	{ 0xff, 0x04 },
	{ 0x04, 0x0a },
};

SkeletonBone MAIN_D_8011C714[30] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x03 },
	{ 0x04, 0x03 },
	{ 0x05, 0x02 },
	{ 0x06, 0x02 },
	{ 0x07, 0x02 },
	{ 0x08, 0x02 },
	{ 0x09, 0x02 },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x02 },
	{ 0x0d, 0x0e },
	{ 0x0e, 0x0f },
	{ 0x0f, 0x02 },
	{ 0x10, 0x11 },
	{ 0x11, 0x02 },
	{ 0x12, 0x13 },
	{ 0x13, 0x02 },
	{ 0x14, 0x15 },
	{ 0x15, 0x02 },
	{ 0x16, 0x17 },
	{ 0x17, 0x02 },
	{ 0x18, 0x19 },
	{ 0x19, 0x1a },
	{ 0x1a, 0x1b },
	{ 0x1b, 0x1c },
};

SkeletonBone MAIN_D_8011C750[22] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0xff, 0x02 },
	{ 0x03, 0x05 },
	{ 0x04, 0x06 },
	{ 0x05, 0x07 },
	{ 0xff, 0x02 },
	{ 0x06, 0x09 },
	{ 0x07, 0x0a },
	{ 0x08, 0x0b },
	{ 0x09, 0x01 },
	{ 0xff, 0x0d },
	{ 0x0a, 0x0e },
	{ 0x0b, 0x0f },
	{ 0x0c, 0x10 },
	{ 0xff, 0x0d },
	{ 0x0d, 0x12 },
	{ 0x0e, 0x13 },
	{ 0x0f, 0x14 },
};

SkeletonBone MAIN_D_8011C77C[17] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x02 },
	{ 0x03, 0x04 },
	{ 0x04, 0x05 },
	{ 0x05, 0x02 },
	{ 0x06, 0x07 },
	{ 0x07, 0x08 },
	{ 0x08, 0x01 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x0a },
	{ 0x0d, 0x0e },
	{ 0x0e, 0x0f },
};

SkeletonBone MAIN_D_8011C7A0[25] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x03 },
	{ 0x04, 0x03 },
	{ 0x05, 0x06 },
	{ 0x06, 0x03 },
	{ 0x07, 0x08 },
	{ 0x08, 0x02 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x02 },
	{ 0x0c, 0x0d },
	{ 0x0d, 0x0e },
	{ 0xff, 0x01 },
	{ 0xff, 0x10 },
	{ 0xff, 0x11 },
	{ 0x0e, 0x12 },
	{ 0x0f, 0x13 },
	{ 0xff, 0x10 },
	{ 0xff, 0x15 },
	{ 0x10, 0x16 },
	{ 0x11, 0x17 },
};

SkeletonBone MAIN_D_8011C7D4[18] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x03, 0x02 },
	{ 0x04, 0x03 },
	{ 0x05, 0x04 },
	{ 0x06, 0x02 },
	{ 0x07, 0x06 },
	{ 0x08, 0x07 },
	{ 0x02, 0x02 },
	{ 0x01, 0x02 },
	{ 0xff, 0x01 },
	{ 0x09, 0x0b },
	{ 0x0a, 0x0c },
	{ 0x0b, 0x0d },
	{ 0x0c, 0x0b },
	{ 0x0d, 0x0f },
	{ 0x0e, 0x10 },
};

SkeletonBone MAIN_D_8011C7F8[10] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x03 },
	{ 0x04, 0x02 },
	{ 0x05, 0x02 },
	{ 0x06, 0x02 },
	{ 0x07, 0x02 },
};

SkeletonBone MAIN_D_8011C80C[20] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x03 },
	{ 0x05, 0x06 },
	{ 0x06, 0x03 },
	{ 0x07, 0x08 },
	{ 0x08, 0x03 },
	{ 0x09, 0x02 },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x0d },
	{ 0x0d, 0x0d },
	{ 0x0e, 0x0f },
	{ 0x0f, 0x0d },
	{ 0x10, 0x11 },
	{ 0x11, 0x0c },
};

SkeletonBone MAIN_D_8011C834[16] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0xff, 0x01 },
	{ 0xff, 0x02 },
	{ 0x01, 0x03 },
	{ 0x02, 0x04 },
	{ 0x03, 0x05 },
	{ 0xff, 0x04 },
	{ 0x04, 0x07 },
	{ 0x05, 0x08 },
	{ 0x06, 0x09 },
	{ 0xff, 0x04 },
	{ 0x07, 0x0b },
	{ 0x08, 0x0c },
	{ 0x09, 0x0d },
	{ 0x00, 0x01 },
};

SkeletonBone MAIN_D_8011C854[17] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x04, 0x01 },
	{ 0x00, 0x02 },
	{ 0x01, 0x03 },
	{ 0x03, 0x03 },
	{ 0x02, 0x03 },
	{ 0x05, 0x01 },
	{ 0x06, 0x07 },
	{ 0x07, 0x08 },
	{ 0x08, 0x09 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x0d },
	{ 0x0e, 0x07 },
	{ 0x0d, 0x07 },
};

SkeletonBone MAIN_D_8011C878[15] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x02, 0x01 },
	{ 0x00, 0x02 },
	{ 0x01, 0x03 },
	{ 0x03, 0x01 },
	{ 0x04, 0x05 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
	{ 0x07, 0x08 },
	{ 0x08, 0x09 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x05 },
	{ 0x0c, 0x05 },
};

SkeletonBone MAIN_D_8011C898[18] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x02 },
	{ 0x04, 0x05 },
	{ 0x05, 0x06 },
	{ 0x06, 0x02 },
	{ 0x07, 0x08 },
	{ 0x08, 0x09 },
	{ 0x09, 0x01 },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x0d },
	{ 0x0d, 0x0b },
	{ 0x0e, 0x0f },
	{ 0x0f, 0x10 },
};

SkeletonBone MAIN_D_8011C8BC[25] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x03 },
	{ 0x04, 0x03 },
	{ 0x05, 0x02 },
	{ 0x06, 0x07 },
	{ 0x07, 0x02 },
	{ 0x08, 0x09 },
	{ 0x09, 0x02 },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x02 },
	{ 0x0c, 0x0d },
	{ 0xff, 0x02 },
	{ 0x0d, 0x0f },
	{ 0x0e, 0x0f },
	{ 0x0f, 0x0f },
	{ 0x10, 0x0f },
	{ 0x11, 0x0f },
	{ 0x12, 0x0f },
	{ 0x13, 0x0f },
	{ 0x14, 0x0f },
	{ 0x15, 0x0f },
};

SkeletonBone MAIN_D_8011C8F0[28] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x07, 0x02 },
	{ 0x08, 0x05 },
	{ 0x09, 0x06 },
	{ 0x0a, 0x02 },
	{ 0x0b, 0x08 },
	{ 0x0c, 0x09 },
	{ 0x0d, 0x02 },
	{ 0x0e, 0x0b },
	{ 0x0f, 0x0c },
	{ 0x10, 0x02 },
	{ 0x11, 0x0e },
	{ 0x12, 0x0f },
	{ 0x03, 0x02 },
	{ 0x04, 0x02 },
	{ 0x05, 0x02 },
	{ 0x06, 0x02 },
	{ 0x13, 0x01 },
	{ 0x14, 0x15 },
	{ 0x15, 0x16 },
	{ 0x16, 0x17 },
	{ 0x17, 0x15 },
	{ 0x18, 0x19 },
	{ 0x19, 0x1a },
};

SkeletonBone MAIN_D_8011C928[21] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x05 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
	{ 0x07, 0x08 },
	{ 0x08, 0x02 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x02 },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x02 },
	{ 0x0d, 0x0e },
	{ 0x0e, 0x02 },
	{ 0x0f, 0x10 },
	{ 0x10, 0x02 },
	{ 0x11, 0x12 },
	{ 0x12, 0x13 },
};

SkeletonBone MAIN_D_8011C954[23] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x02 },
	{ 0x04, 0x05 },
	{ 0x05, 0x06 },
	{ 0x06, 0x02 },
	{ 0x07, 0x08 },
	{ 0x08, 0x09 },
	{ 0x09, 0x01 },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x0d },
	{ 0x0d, 0x0b },
	{ 0x0e, 0x0f },
	{ 0x0f, 0x10 },
	{ 0x10, 0x11 },
	{ 0x11, 0x0b },
	{ 0x12, 0x13 },
	{ 0x13, 0x14 },
	{ 0x14, 0x15 },
};

SkeletonBone MAIN_D_8011C984[24] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0xff, 0x02 },
	{ 0x04, 0x06 },
	{ 0x05, 0x07 },
	{ 0x06, 0x08 },
	{ 0x07, 0x06 },
	{ 0x08, 0x0a },
	{ 0x09, 0x0b },
	{ 0x0a, 0x01 },
	{ 0x0b, 0x0d },
	{ 0x0c, 0x0e },
	{ 0x0d, 0x0f },
	{ 0xff, 0x0d },
	{ 0x0e, 0x11 },
	{ 0x0f, 0x12 },
	{ 0x10, 0x13 },
	{ 0x11, 0x11 },
	{ 0x12, 0x15 },
	{ 0x13, 0x16 },
};

SkeletonBone MAIN_D_8011C9B4[30] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x03 },
	{ 0x04, 0x03 },
	{ 0x09, 0x02 },
	{ 0x0a, 0x07 },
	{ 0x0b, 0x08 },
	{ 0x0c, 0x02 },
	{ 0x0d, 0x0a },
	{ 0x0e, 0x0b },
	{ 0x0f, 0x02 },
	{ 0x10, 0x0d },
	{ 0x11, 0x0e },
	{ 0x12, 0x02 },
	{ 0x13, 0x10 },
	{ 0x14, 0x11 },
	{ 0x05, 0x02 },
	{ 0x06, 0x02 },
	{ 0x07, 0x02 },
	{ 0x08, 0x02 },
	{ 0x15, 0x01 },
	{ 0x16, 0x17 },
	{ 0x17, 0x18 },
	{ 0x18, 0x19 },
	{ 0x19, 0x17 },
	{ 0x1a, 0x1b },
	{ 0x1b, 0x1c },
};

SkeletonBone MAIN_D_8011C9F0[26] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x07, 0x02 },
	{ 0x08, 0x05 },
	{ 0x09, 0x02 },
	{ 0x0a, 0x07 },
	{ 0x0b, 0x02 },
	{ 0x0c, 0x09 },
	{ 0x0d, 0x0a },
	{ 0x0e, 0x02 },
	{ 0x0f, 0x0c },
	{ 0x10, 0x0d },
	{ 0x03, 0x02 },
	{ 0x04, 0x02 },
	{ 0x05, 0x02 },
	{ 0x06, 0x02 },
	{ 0x11, 0x01 },
	{ 0x12, 0x13 },
	{ 0x13, 0x14 },
	{ 0x14, 0x15 },
	{ 0x15, 0x13 },
	{ 0x16, 0x17 },
	{ 0x17, 0x18 },
};

SkeletonBone MAIN_D_8011CA24[12] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x02 },
	{ 0x03, 0x04 },
	{ 0x04, 0x02 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
	{ 0x07, 0x02 },
	{ 0x08, 0x02 },
	{ 0x09, 0x02 },
};

SkeletonBone MAIN_D_8011CA3C[31] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x02 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
	{ 0x07, 0x02 },
	{ 0x08, 0x09 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x02 },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x0d },
	{ 0x0d, 0x02 },
	{ 0x0e, 0x0f },
	{ 0x0f, 0x10 },
	{ 0x10, 0x02 },
	{ 0x11, 0x12 },
	{ 0x12, 0x02 },
	{ 0x13, 0x14 },
	{ 0x14, 0x02 },
	{ 0x15, 0x02 },
	{ 0x16, 0x17 },
	{ 0x17, 0x02 },
	{ 0x18, 0x19 },
	{ 0x19, 0x02 },
	{ 0x1a, 0x1b },
	{ 0x1b, 0x02 },
	{ 0x1c, 0x1d },
};

SkeletonBone MAIN_D_8011CA7C[12] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x01 },
	{ 0x02, 0x01 },
	{ 0x03, 0x01 },
	{ 0x04, 0x01 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
	{ 0x07, 0x01 },
	{ 0x08, 0x09 },
	{ 0x09, 0x0a },
};

SkeletonBone MAIN_D_8011CA94[21] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x02 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
	{ 0x07, 0x02 },
	{ 0x08, 0x09 },
	{ 0x09, 0x0a },
	{ 0x12, 0x02 },
	{ 0x0a, 0x02 },
	{ 0x0b, 0x0d },
	{ 0x0c, 0x0e },
	{ 0x0d, 0x0f },
	{ 0x0e, 0x02 },
	{ 0x0f, 0x11 },
	{ 0x10, 0x12 },
	{ 0x11, 0x13 },
};

SkeletonBone MAIN_D_8011CAC0[15] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x01, 0x01 },
	{ 0x02, 0x02 },
	{ 0x03, 0x03 },
	{ 0x04, 0x04 },
	{ 0xff, 0x02 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
	{ 0x07, 0x08 },
	{ 0xff, 0x02 },
	{ 0x08, 0x0a },
	{ 0x09, 0x0b },
	{ 0x0a, 0x0c },
	{ 0x00, 0x01 },
};

SkeletonBone MAIN_D_8011CAE0[23] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x05 },
	{ 0x05, 0x03 },
	{ 0x06, 0x07 },
	{ 0x07, 0x08 },
	{ 0x08, 0x02 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x02 },
	{ 0x0c, 0x0d },
	{ 0x0d, 0x0e },
	{ 0x0e, 0x01 },
	{ 0x0f, 0x10 },
	{ 0x10, 0x11 },
	{ 0x11, 0x12 },
	{ 0x12, 0x10 },
	{ 0x13, 0x14 },
	{ 0x14, 0x15 },
};

SkeletonBone MAIN_D_8011CB10[22] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x05 },
	{ 0x05, 0x02 },
	{ 0x06, 0x07 },
	{ 0x07, 0x08 },
	{ 0x08, 0x02 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x01 },
	{ 0x0c, 0x0d },
	{ 0x0d, 0x0e },
	{ 0x0e, 0x0d },
	{ 0x0f, 0x10 },
	{ 0x10, 0x11 },
	{ 0x11, 0x0d },
	{ 0x12, 0x13 },
	{ 0x13, 0x14 },
};

SkeletonBone MAIN_D_8011CB3C[5] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x01 },
	{ 0x02, 0x01 },
};

SkeletonBone MAIN_D_8011CB48[19] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0xff, 0x01 },
	{ 0x04, 0x02 },
	{ 0x05, 0x03 },
	{ 0x06, 0x04 },
	{ 0xff, 0x01 },
	{ 0x01, 0x06 },
	{ 0x02, 0x07 },
	{ 0x03, 0x08 },
	{ 0x00, 0x01 },
	{ 0xff, 0x0a },
	{ 0x07, 0x0b },
	{ 0x08, 0x0c },
	{ 0x09, 0x0d },
	{ 0xff, 0x0a },
	{ 0x0a, 0x0f },
	{ 0x0b, 0x10 },
	{ 0x0c, 0x11 },
};

SkeletonBone MAIN_D_8011CB70[19] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0xff, 0x01 },
	{ 0x03, 0x02 },
	{ 0x04, 0x03 },
	{ 0x05, 0x04 },
	{ 0xff, 0x01 },
	{ 0x01, 0x06 },
	{ 0x02, 0x07 },
	{ 0xff, 0x08 },
	{ 0x00, 0x01 },
	{ 0xff, 0x0a },
	{ 0x06, 0x0b },
	{ 0x07, 0x0c },
	{ 0x08, 0x0d },
	{ 0xff, 0x0a },
	{ 0x09, 0x0f },
	{ 0x0a, 0x10 },
	{ 0x0b, 0x11 },
};

SkeletonBone MAIN_D_8011CB98[19] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x02 },
	{ 0x03, 0x02 },
	{ 0x04, 0x02 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
	{ 0x07, 0x02 },
	{ 0x08, 0x09 },
	{ 0x09, 0x0a },
	{ 0xff, 0x01 },
	{ 0x0a, 0x0c },
	{ 0x0b, 0x0d },
	{ 0x0c, 0x0e },
	{ 0x0d, 0x0c },
	{ 0x0e, 0x10 },
	{ 0x0f, 0x11 },
};

SkeletonBone MAIN_D_8011CBC0[32] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x05 },
	{ 0x05, 0x06 },
	{ 0x06, 0x03 },
	{ 0x07, 0x08 },
	{ 0x08, 0x09 },
	{ 0x09, 0x03 },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x03 },
	{ 0x0d, 0x0e },
	{ 0x0e, 0x0f },
	{ 0x0f, 0x03 },
	{ 0x10, 0x11 },
	{ 0x11, 0x12 },
	{ 0x12, 0x12 },
	{ 0x13, 0x12 },
	{ 0x14, 0x01 },
	{ 0x15, 0x16 },
	{ 0x16, 0x17 },
	{ 0x17, 0x18 },
	{ 0x18, 0x16 },
	{ 0x19, 0x1a },
	{ 0x1a, 0x1b },
	{ 0x1b, 0x16 },
	{ 0x1c, 0x1d },
	{ 0x1d, 0x1e },
};

SkeletonBone MAIN_D_8011CC00[16] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x02 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
	{ 0x07, 0x01 },
	{ 0x08, 0x09 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x09 },
	{ 0x0c, 0x0d },
	{ 0x0d, 0x0e },
};

SkeletonBone MAIN_D_8011CC20[21] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0xff, 0x02 },
	{ 0x03, 0x05 },
	{ 0x04, 0x06 },
	{ 0x05, 0x07 },
	{ 0xff, 0x02 },
	{ 0x06, 0x09 },
	{ 0x07, 0x0a },
	{ 0x08, 0x0b },
	{ 0xff, 0x01 },
	{ 0xff, 0x0d },
	{ 0x0c, 0x0e },
	{ 0xff, 0x0e },
	{ 0xff, 0x10 },
	{ 0x09, 0x11 },
	{ 0x0a, 0x12 },
	{ 0x0b, 0x13 },
};

SkeletonBone MAIN_D_8011CC4C[29] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0xff, 0x02 },
	{ 0x04, 0x06 },
	{ 0x05, 0x07 },
	{ 0x06, 0x08 },
	{ 0x08, 0x08 },
	{ 0x07, 0x08 },
	{ 0xff, 0x02 },
	{ 0x09, 0x0c },
	{ 0x0a, 0x0d },
	{ 0x0b, 0x0e },
	{ 0x0c, 0x0e },
	{ 0x0d, 0x0e },
	{ 0xff, 0x02 },
	{ 0x0e, 0x12 },
	{ 0xff, 0x02 },
	{ 0x0f, 0x14 },
	{ 0x10, 0x01 },
	{ 0x11, 0x16 },
	{ 0x12, 0x17 },
	{ 0x13, 0x18 },
	{ 0x14, 0x19 },
	{ 0x15, 0x1a },
	{ 0x16, 0x1b },
};

SkeletonBone MAIN_D_8011CC88[25] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x02 },
	{ 0x03, 0x02 },
	{ 0x04, 0x02 },
	{ 0x05, 0x02 },
	{ 0x06, 0x02 },
	{ 0x07, 0x08 },
	{ 0x08, 0x02 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x02 },
	{ 0x0d, 0x0e },
	{ 0x0e, 0x0f },
	{ 0x0f, 0x10 },
	{ 0x10, 0x02 },
	{ 0x11, 0x12 },
	{ 0x12, 0x13 },
	{ 0x13, 0x14 },
	{ 0x14, 0x12 },
	{ 0x15, 0x16 },
	{ 0x16, 0x17 },
};

SkeletonBone MAIN_D_8011CCBC[26] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0xff, 0x02 },
	{ 0x04, 0x06 },
	{ 0x05, 0x07 },
	{ 0x06, 0x08 },
	{ 0xff, 0x02 },
	{ 0x07, 0x0a },
	{ 0x08, 0x0b },
	{ 0x09, 0x0c },
	{ 0x0a, 0x01 },
	{ 0x11, 0x0e },
	{ 0x12, 0x0f },
	{ 0x13, 0x10 },
	{ 0xff, 0x0e },
	{ 0x0b, 0x12 },
	{ 0x0c, 0x13 },
	{ 0x0d, 0x14 },
	{ 0xff, 0x0e },
	{ 0x0e, 0x16 },
	{ 0x0f, 0x17 },
	{ 0x10, 0x18 },
};

SkeletonBone MAIN_D_8011CCF0[17] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x02 },
	{ 0x03, 0x04 },
	{ 0x04, 0x05 },
	{ 0x05, 0x04 },
	{ 0x06, 0x07 },
	{ 0x07, 0x04 },
	{ 0x08, 0x09 },
	{ 0xff, 0x02 },
	{ 0x09, 0x0b },
	{ 0x0a, 0x0b },
	{ 0xff, 0x02 },
	{ 0x0b, 0x0e },
	{ 0x0c, 0x0e },
};

SkeletonBone MAIN_D_8011CD14[28] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0xff, 0x01 },
	{ 0x04, 0x06 },
	{ 0x05, 0x06 },
	{ 0xff, 0x01 },
	{ 0x06, 0x09 },
	{ 0x07, 0x0a },
	{ 0x08, 0x0b },
	{ 0x09, 0x0c },
	{ 0x0a, 0x09 },
	{ 0x0b, 0x0e },
	{ 0x0c, 0x0f },
	{ 0x0d, 0x10 },
	{ 0x0e, 0x11 },
	{ 0xff, 0x01 },
	{ 0x0f, 0x13 },
	{ 0x10, 0x14 },
	{ 0x11, 0x15 },
	{ 0x12, 0x16 },
	{ 0x13, 0x13 },
	{ 0x14, 0x18 },
	{ 0x15, 0x19 },
	{ 0x16, 0x1a },
};

SkeletonBone MAIN_D_8011CD4C[26] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x02 },
	{ 0x03, 0x02 },
	{ 0x04, 0x01 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
	{ 0x07, 0x01 },
	{ 0x08, 0x09 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x01 },
	{ 0x0b, 0x0c },
	{ 0x0c, 0x0d },
	{ 0x0d, 0x01 },
	{ 0x0e, 0x0f },
	{ 0x0f, 0x10 },
	{ 0x10, 0x01 },
	{ 0x11, 0x12 },
	{ 0x12, 0x01 },
	{ 0x13, 0x14 },
	{ 0x14, 0x01 },
	{ 0x15, 0x16 },
	{ 0x16, 0x01 },
	{ 0x17, 0x18 },
};

SkeletonBone MAIN_D_8011CD80[7] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x02 },
	{ 0x03, 0x02 },
	{ 0x04, 0x01 },
};

SkeletonBone MAIN_D_8011CD90[20] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x01 },
	{ 0x03, 0x04 },
	{ 0x04, 0x05 },
	{ 0x05, 0x06 },
	{ 0x06, 0x07 },
	{ 0x07, 0x08 },
	{ 0x08, 0x09 },
	{ 0x09, 0x0a },
	{ 0xff, 0x04 },
	{ 0x0a, 0x0c },
	{ 0x0b, 0x0d },
	{ 0x0c, 0x0e },
	{ 0xff, 0x04 },
	{ 0x0d, 0x10 },
	{ 0x0e, 0x11 },
	{ 0x0f, 0x12 },
};

SkeletonBone MAIN_D_8011CDB8[27] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x01 },
	{ 0x02, 0x03 },
	{ 0x03, 0x03 },
	{ 0x04, 0x03 },
	{ 0x05, 0x03 },
	{ 0x06, 0x01 },
	{ 0x07, 0x08 },
	{ 0x08, 0x08 },
	{ 0x09, 0x08 },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x08 },
	{ 0x0c, 0x0d },
	{ 0x0d, 0x01 },
	{ 0x0e, 0x0f },
	{ 0x0f, 0x01 },
	{ 0x10, 0x11 },
	{ 0x11, 0x01 },
	{ 0x12, 0x13 },
	{ 0x13, 0x01 },
	{ 0x14, 0x15 },
	{ 0x15, 0x01 },
	{ 0x16, 0x17 },
	{ 0x17, 0x01 },
	{ 0x18, 0x19 },
};

SkeletonBone MAIN_D_8011CDF0[22] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x04 },
	{ 0x04, 0x02 },
	{ 0x05, 0x02 },
	{ 0x06, 0x07 },
	{ 0x07, 0x08 },
	{ 0x08, 0x02 },
	{ 0x09, 0x0a },
	{ 0x0a, 0x0b },
	{ 0xff, 0x01 },
	{ 0x0b, 0x0d },
	{ 0x0c, 0x0e },
	{ 0x0d, 0x0f },
	{ 0x0e, 0x10 },
	{ 0x0f, 0x0d },
	{ 0x10, 0x12 },
	{ 0x11, 0x13 },
	{ 0x12, 0x14 },
};

SkeletonBone MAIN_D_8011CE1C[15] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x02 },
	{ 0x04, 0x05 },
	{ 0x05, 0x01 },
	{ 0x06, 0x07 },
	{ 0x07, 0x08 },
	{ 0x08, 0x09 },
	{ 0x09, 0x07 },
	{ 0x0a, 0x0b },
	{ 0x0b, 0x07 },
	{ 0x0c, 0x0d },
};

SkeletonBone MAIN_D_8011CE3C[18] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x02 },
	{ 0x03, 0x04 },
	{ 0x04, 0x05 },
	{ 0x05, 0x06 },
	{ 0x06, 0x02 },
	{ 0x07, 0x08 },
	{ 0x08, 0x09 },
	{ 0xff, 0x01 },
	{ 0x09, 0x0b },
	{ 0x0a, 0x0c },
	{ 0x0b, 0x0d },
	{ 0x0c, 0x0b },
	{ 0x0d, 0x0f },
	{ 0x0e, 0x10 },
};

SkeletonBone *DIGIMON_SKELETONS[180] = {
	MAIN_D_8011C170,
	MAIN_D_80133B2C,
	MAIN_D_8011C5A0,
	MAIN_D_8011C194,
	MAIN_D_8011C1C0,
	MAIN_D_8011C1F0,
	MAIN_D_8011C224,
	MAIN_D_8011CD90,
	MAIN_D_8011C25C,
	MAIN_D_8011C294,
	MAIN_D_8011C854,
	MAIN_D_8011C60C,
	MAIN_D_8011C2C0,
	MAIN_D_8011CB48,
	MAIN_D_8011C2FC,
	MAIN_D_80133B2C,
	MAIN_D_80133B2C,
	MAIN_D_8011C194,
	MAIN_D_8011C8BC,
	MAIN_D_8011C8F0,
	MAIN_D_8011C418,
	MAIN_D_8011C64C,
	MAIN_D_8011CCBC,
	MAIN_D_8011C3AC,
	MAIN_D_8011C6FC,
	MAIN_D_8011C4C4,
	MAIN_D_8011C5D8,
	MAIN_D_8011CB70,
	MAIN_D_8011C834,
	MAIN_D_80133B2C,
	MAIN_D_8011C550,
	MAIN_D_8011CAE0,
	MAIN_D_8011C518,
	MAIN_D_8011C48C,
	MAIN_D_8011C898,
	MAIN_D_8011CAC0,
	MAIN_D_8011C568,
	MAIN_D_8011C638,
	MAIN_D_8011C6C4,
	MAIN_D_8011C348,
	MAIN_D_8011C3D4,
	MAIN_D_8011C400,
	MAIN_D_8011C374,
	MAIN_D_8011C67C,
	MAIN_D_8011C7F8,
	MAIN_D_8011C4EC,
	MAIN_D_8011C44C,
	MAIN_D_8011C984,
	MAIN_D_8011C954,
	MAIN_D_8011C80C,
	MAIN_D_8011CA94,
	MAIN_D_8011C9B4,
	MAIN_D_8011C7D4,
	MAIN_D_8011C5B0,
	MAIN_D_8011CC4C,
	MAIN_D_8011C690,
	MAIN_D_8011CA7C,
	MAIN_D_8011C7A0,
	MAIN_D_8011CB48,
	MAIN_D_8011CA3C,
	MAIN_D_8011C9F0,
	MAIN_D_8011C878,
	NULL,
	MAIN_D_8011C954,
	MAIN_D_8011CC4C,
	MAIN_D_8011C374,
	MAIN_D_8011CC88,
	MAIN_D_8011C714,
	MAIN_D_8011C750,
	MAIN_D_8011CB10,
	MAIN_D_8011CA24,
	MAIN_D_8011C194,
	MAIN_D_8011C898,
	MAIN_D_8011C348,
	MAIN_D_8011C518,
	MAIN_D_8011C48C,
	MAIN_D_8011CCF0,
	MAIN_D_8011C4C4,
	MAIN_D_8011C7D4,
	MAIN_D_8011C6C4,
	MAIN_D_8011C318,
	MAIN_D_8011C3AC,
	MAIN_D_8011C194,
	MAIN_D_8011C1C0,
	MAIN_D_8011C194,
	MAIN_D_8011C418,
	MAIN_D_8011C44C,
	MAIN_D_8011C60C,
	MAIN_D_8011C984,
	MAIN_D_8011C898,
	MAIN_D_8011C400,
	MAIN_D_8011CAC0,
	MAIN_D_8011CB98,
	MAIN_D_8011C7A0,
	MAIN_D_8011C750,
	MAIN_D_8011CA94,
	MAIN_D_8011CAE0,
	MAIN_D_8011C318,
	MAIN_D_8011C194,
	MAIN_D_8011C4C4,
	MAIN_D_8011C224,
	MAIN_D_8011CB10,
	MAIN_D_8011C714,
	MAIN_D_8011C318,
	MAIN_D_8011C294,
	MAIN_D_8011CCBC,
	MAIN_D_8011C64C,
	MAIN_D_8011C638,
	MAIN_D_8011C3AC,
	MAIN_D_8011CE1C,
	MAIN_D_8011CDF0,
	MAIN_D_8011CDB8,
	MAIN_D_8011C878,
	MAIN_D_8011C750,
	NULL,
	MAIN_D_8011CBC0,
	MAIN_D_8011CE3C,
	MAIN_D_8011C77C,
	MAIN_D_8011CC00,
	MAIN_D_8011CD14,
	MAIN_D_8011CC20,
	MAIN_D_8011CD4C,
	MAIN_D_8011CB3C,
	MAIN_D_8011CD80,
	MAIN_D_8011C25C,
	MAIN_D_8011C318,
	MAIN_D_8011C928,
	MAIN_D_80133B2C,
	MAIN_D_8011C1C0,
	MAIN_D_8011C1F0,
	MAIN_D_8011C224,
	MAIN_D_8011CD90,
	MAIN_D_8011C25C,
	MAIN_D_8011C294,
	MAIN_D_8011C854,
	MAIN_D_8011C60C,
	MAIN_D_8011C2C0,
	MAIN_D_8011CB48,
	MAIN_D_8011C2FC,
	MAIN_D_8011C194,
	MAIN_D_8011C8BC,
	MAIN_D_8011C8F0,
	MAIN_D_8011C418,
	MAIN_D_8011C64C,
	MAIN_D_8011CCBC,
	MAIN_D_8011C3AC,
	MAIN_D_8011C6FC,
	MAIN_D_8011C4C4,
	MAIN_D_8011C5D8,
	MAIN_D_8011CB70,
	MAIN_D_8011C834,
	MAIN_D_8011CAE0,
	MAIN_D_8011C518,
	MAIN_D_8011C48C,
	MAIN_D_8011C898,
	MAIN_D_8011CAC0,
	MAIN_D_8011C568,
	MAIN_D_8011C638,
	MAIN_D_8011C6C4,
	MAIN_D_8011C348,
	MAIN_D_8011C3D4,
	MAIN_D_8011C400,
	MAIN_D_8011C374,
	MAIN_D_8011C4EC,
	MAIN_D_8011C44C,
	MAIN_D_8011C984,
	MAIN_D_8011C954,
	MAIN_D_8011C80C,
	MAIN_D_8011CA94,
	MAIN_D_8011C9B4,
	MAIN_D_8011C7D4,
	MAIN_D_8011C5B0,
	MAIN_D_8011CC4C,
	MAIN_D_8011C690,
	MAIN_D_8011CA7C,
	MAIN_D_8011CB48,
	MAIN_D_8011C7A0,
	MAIN_D_8011CC88,
	MAIN_D_8011C1F0,
	MAIN_D_8011C2C0,
};

int16_t PARTNER_WIREFRAME_SUB[40] = {
	0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
	0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
	0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
	0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
	0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
};

int8_t WIREFRAME_RNG_TABLE[16] = {
	0x06, 0x03, 0x0b, 0x08, 0x0a, 0x0d, 0x01, 0x04,
	0x0e, 0x07, 0x0c, 0x0f, 0x05, 0x02, 0x09, 0x00,
};

char PATH_ALLTIM_TIM[] = "ALLTIM.TIM";

char *PTR_DIGIMON_FILE_NAMES[180] = {
	DIGIMON_FILE_BOYS,
	DIGIMON_FILE_BOTA,
	DIGIMON_FILE_KORO,
	DIGIMON_FILE_AGUM,
	DIGIMON_FILE_BETA,
	DIGIMON_FILE_GREY,
	DIGIMON_FILE_DEVI,
	DIGIMON_FILE_AIRD,
	DIGIMON_FILE_TYRA,
	DIGIMON_FILE_MERA,
	DIGIMON_FILE_SEAD,
	DIGIMON_FILE_NUME,
	DIGIMON_FILE_MTGR,
	DIGIMON_FILE_MAME,
	DIGIMON_FILE_MONZ,
	DIGIMON_FILE_PUNI,
	DIGIMON_FILE_TUNO,
	DIGIMON_FILE_GABU,
	DIGIMON_FILE_ELEC,
	DIGIMON_FILE_KABU,
	DIGIMON_FILE_ANGE,
	DIGIMON_FILE_BIRD,
	DIGIMON_FILE_GARU,
	DIGIMON_FILE_YUKI,
	DIGIMON_FILE_HOEE,
	DIGIMON_FILE_VEGI,
	DIGIMON_FILE_SKUL,
	DIGIMON_FILE_MTMA,
	DIGIMON_FILE_VEDA,
	DIGIMON_FILE_POYO,
	DIGIMON_FILE_TOKO,
	DIGIMON_FILE_PATA,
	DIGIMON_FILE_KUNE,
	DIGIMON_FILE_UNIM,
	DIGIMON_FILE_OGRE,
	DIGIMON_FILE_SHEL,
	DIGIMON_FILE_CENT,
	DIGIMON_FILE_BAKE,
	DIGIMON_FILE_DORI,
	DIGIMON_FILE_SCUM,
	DIGIMON_FILE_ANDR,
	DIGIMON_FILE_GIRO,
	DIGIMON_FILE_ETEM,
	DIGIMON_FILE_YURA,
	DIGIMON_FILE_TANE,
	DIGIMON_FILE_PIYO,
	DIGIMON_FILE_PALM,
	DIGIMON_FILE_MONO,
	DIGIMON_FILE_LEOM,
	DIGIMON_FILE_SIRA,
	DIGIMON_FILE_COCA,
	DIGIMON_FILE_KUWA,
	DIGIMON_FILE_MOJA,
	DIGIMON_FILE_NANI,
	DIGIMON_FILE_MGDR,
	DIGIMON_FILE_PICC,
	DIGIMON_FILE_DIGI,
	DIGIMON_FILE_PENM,
	DIGIMON_FILE_IGAM,
	DIGIMON_FILE_HOUO,
	DIGIMON_FILE_HKAB,
	DIGIMON_FILE_MGSD,
	DIGIMON_FILE_WEAG,
	DIGIMON_FILE_PANJ,
	DIGIMON_FILE_GGDR,
	DIGIMON_FILE_MTET,
	DIGIMON_FILE_VAND,
	DIGIMON_FILE_YANM,
	DIGIMON_FILE_GOTU,
	DIGIMON_FILE_FLAR,
	DIGIMON_FILE_WARU,
	DIGIMON_FILE_YKAG,
	DIGIMON_FILE_HYOG,
	DIGIMON_FILE_PCSC,
	DIGIMON_FILE_DOKU,
	DIGIMON_FILE_SIMA,
	DIGIMON_FILE_TANK,
	DIGIMON_FILE_REDV,
	DIGIMON_FILE_JMOJ,
	DIGIMON_FILE_NISE,
	DIGIMON_FILE_GOBR,
	DIGIMON_FILE_TUTI,
	DIGIMON_FILE_PSYC,
	DIGIMON_FILE_MODO,
	DIGIMON_FILE_TOYA,
	DIGIMON_FILE_PIDD,
	DIGIMON_FILE_ARUR,
	DIGIMON_FILE_GERE,
	DIGIMON_FILE_VARM,
	DIGIMON_FILE_FUGA,
	DIGIMON_FILE_TKKA,
	DIGIMON_FILE_MRIS,
	DIGIMON_FILE_GARD,
	DIGIMON_FILE_MCHO,
	DIGIMON_FILE_ICEM,
	DIGIMON_FILE_AKAT,
	DIGIMON_FILE_TUKA,
	DIGIMON_FILE_SHAM,
	DIGIMON_FILE_CLEA,
	DIGIMON_FILE_ZASS,
	DIGIMON_FILE_ICDV,
	DIGIMON_FILE_DKRZ,
	DIGIMON_FILE_SNDY,
	DIGIMON_FILE_SNGB,
	DIGIMON_FILE_BLMR,
	DIGIMON_FILE_GRUR,
	DIGIMON_FILE_SABD,
	DIGIMON_FILE_SOUL,
	DIGIMON_FILE_GOLE,
	DIGIMON_FILE_OTAM,
	DIGIMON_FILE_GECO,
	DIGIMON_FILE_TENT,
	DIGIMON_FILE_WRSE,
	DIGIMON_FILE_INSE,
	DIGIMON_FILE_TAKA,
	DIGIMON_FILE_MUGE,
	DIGIMON_FILE_ANLG,
	DIGIMON_FILE_JIJI,
	DIGIMON_FILE_TENS,
	DIGIMON_FILE_TONO,
	DIGIMON_FILE_SCUD,
	DIGIMON_FILE_JURE,
	DIGIMON_FILE_HAGU,
	DIGIMON_FILE_BRIK,
	DIGIMON_FILE_TIRS,
	DIGIMON_FILE_EGOB,
	DIGIMON_FILE_BRAK,
	DIGIMON_FILE_PUTI,
	DIGIMON_FILE_EBET,
	DIGIMON_FILE_EGRE,
	DIGIMON_FILE_EDEV,
	DIGIMON_FILE_EAIR,
	DIGIMON_FILE_ETYR,
	DIGIMON_FILE_EMER,
	DIGIMON_FILE_ESEA,
	DIGIMON_FILE_ENUM,
	DIGIMON_FILE_EMTG,
	DIGIMON_FILE_EMAM,
	DIGIMON_FILE_EMON,
	DIGIMON_FILE_EGAB,
	DIGIMON_FILE_EELE,
	DIGIMON_FILE_EKAB,
	DIGIMON_FILE_EANG,
	DIGIMON_FILE_EBIR,
	DIGIMON_FILE_EGAR,
	DIGIMON_FILE_EYUK,
	DIGIMON_FILE_EHOE,
	DIGIMON_FILE_EVEG,
	DIGIMON_FILE_ESKU,
	DIGIMON_FILE_EMTM,
	DIGIMON_FILE_EVED,
	DIGIMON_FILE_EPAT,
	DIGIMON_FILE_EKUN,
	DIGIMON_FILE_EUNI,
	DIGIMON_FILE_EOGR,
	DIGIMON_FILE_ESHE,
	DIGIMON_FILE_ECEN,
	DIGIMON_FILE_EBAK,
	DIGIMON_FILE_EDOR,
	DIGIMON_FILE_ESCU,
	DIGIMON_FILE_EAND,
	DIGIMON_FILE_EGIR,
	DIGIMON_FILE_EETE,
	DIGIMON_FILE_EPIY,
	DIGIMON_FILE_EPAL,
	DIGIMON_FILE_EMNO,
	DIGIMON_FILE_ELEO,
	DIGIMON_FILE_ESIR,
	DIGIMON_FILE_ECOC,
	DIGIMON_FILE_EKUW,
	DIGIMON_FILE_EMOJ,
	DIGIMON_FILE_ENAN,
	DIGIMON_FILE_EMGD,
	DIGIMON_FILE_EPIC,
	DIGIMON_FILE_EDIG,
	DIGIMON_FILE_EIGA,
	DIGIMON_FILE_EPEN,
	DIGIMON_FILE_EVAN,
	DIGIMON_FILE_CEGR,
	DIGIMON_FILE_CEMG,
};

char PATH_CHDAT_TMD0[12] = "CHDAT\\TMD0\\";

char PATH_CHDAT_MTN0[12] = "CHDAT\\MTN0\\";

char PATH_CHDAT_MMD0[12] = "CHDAT\\MMD0\\";
// clang-format on

static inline int8_t *model_s8ptr(uint8_t *arg0)
{
	return (int8_t *)arg0;
}

void initializePosData(PositionData *posData)
{
	MATRIX *m;

	RotMatrix(&posData->rotation, m = &posData->posMatrix.coord);
	ScaleMatrix(m, &posData->scale);
	TransMatrix(m, &posData->location);
	posData->posMatrix.flg = 0;
}

void renderFlatDigimon(Entity *entity)
{
	MATRIX m;
	SVECTOR in[4];
	SVECTOR out[4];
	ModelComponent *model;
	POLY_FT4 *prim;
	VECTOR *loc;
	int32_t entityType;
	int32_t i;
	int32_t height;
	int32_t half;

	entityType = getEntityType(entity);
	model = getEntityModelComponent(entity->type, entityType);
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	setRGB0(prim, 0x80, 0x80, 0x80);
	prim->clut = model->clutPage + 0x3C0;
	prim->tpage = model->pixelPage;
	if (entity->flatSprite < 2) {
		if ((entity->flatTimer % 10) == 0) {
			entity->flatSprite = (entity->flatSprite + 1) & 1;
		}
		++entity->flatTimer;
	}
	setUVWH(prim,
	        ((entity->flatSprite % 2) == 0) ? 0xE0 : 0xF0,
	        (entity->flatSprite < 2) ? model->pixelOffsetY + 0x60 : model->pixelOffsetY + 0x70,
	        0xF, 0xF);
	half = (height = DIGIMON_DATA[entity->type].height) / 2;
	in[0].vx = 0;
	in[0].vy = -height;
	in[0].vz = -half;
	in[1].vx = 0;
	in[1].vy = -height;
	in[1].vz = half;
	in[2].vx = 0;
	in[2].vy = 0;
	in[2].vz = -half;
	in[3].vx = 0;
	in[3].vy = 0;
	in[3].vz = half;
	loc = &entity->posData->location;
	RotMatrix(&entity->posData->rotation, &m);
	ScaleMatrix(&m, &entity->posData->scale);
	for (i = 0; i < 4; i++) {
		ApplyMatrixSV(&m, &in[i], &out[i]);
		out[i].vx += (int16_t)loc->vx;
		out[i].vy += (int16_t)loc->vy;
		out[i].vz += (int16_t)loc->vz;
	}
	add3DSpritePrim(prim, &out[0], &out[1], &out[2], &out[3]);
}

ModelComponent *thunkLoadMMD(int32_t digiType, int32_t modelType)
{
	return loadMMD(digiType, modelType);
}

void thunkUnloadModel(int32_t digiType, int32_t modelType)
{
	unloadModel(digiType, modelType);
}

void initializeDigimonObject(int32_t type, int32_t instanceId,
                             TickFunction tick)
{
	Entity *entity;
	ModelComponent *model;
	PositionData *pos;
	SkeletonBone *bone;
	int16_t *anim;
	int32_t entityType;
	int32_t boneCount;
	int32_t i;
	int32_t parent;

	if (instanceId < 0 || instanceId >= ENTITY_MAX) {
		return;
	}
	boneCount = DIGIMON_DATA[type].boneCount;
	entity = ENTITY_TABLE[instanceId];
	entityType = getEntityType(entity);
	entity->type = type;
	switch (entityType) {
	case 2:
		entity->posData = TAMER_POSITION_DATA;
		entity->anim.momentum = TAMER_MOMENTUM_DATA;
		break;
	case 3:
		entity->posData = PARTNER_POSITION_DATA;
		entity->anim.momentum = PARTNER_MOMENTUM_DATA;
		break;
	case 0:
		entity->posData = malloc3(boneCount * sizeof(PositionData));
		entity->anim.momentum = malloc3(boneCount * sizeof(MomentumData));
		break;
	}
	model = getEntityModelComponent(type, entityType);
	entity->animPtr = model->animTablePtr;
	pos = entity->posData;
	bone = DIGIMON_SKELETONS[type];
	for (i = 0; i < boneCount; ++pos, ++bone, ++i) {
		if (bone->objIndex != -1) {
			GsLinkObject4((u_long)model->modelPtr->obj, &pos->obj, bone->objIndex);
		} else {
			pos->obj.tmd = NULL;
		}
		if ((parent = bone->parentIndex) != -1) {
			GsInitCoordinate2(&entity->posData[parent].posMatrix, &pos->posMatrix);
		} else {
			GsInitCoordinate2(NULL, &pos->posMatrix);
		}
		pos->obj.attribute = 0;
		pos->obj.coord2 = &pos->posMatrix;
	}
	pos = entity->posData;
	anim = (int16_t *)((char *)entity->animPtr + *entity->animPtr);
	++anim;
	if (type == 0x71) {
		pos->scale.vx = 0x1800, pos->scale.vy = 0x1800, pos->scale.vz = 0x1800;
	} else {
		pos->scale.vx = 0x1000, pos->scale.vy = 0x1000, pos->scale.vz = 0x1000;
	}
	pos->rotation.vx = 0;
	pos->rotation.vy = 0;
	pos->rotation.vz = 0;
	pos->location.vx = 0;
	pos->location.vy = 0;
	pos->location.vz = 0;
	initializePosData(pos);
	for (i = 1; i < boneCount; i++) {
		++pos;
		pos->scale.vx = 0x1000, pos->scale.vy = 0x1000, pos->scale.vz = 0x1000;
		pos->rotation.vx = *anim++;
		pos->rotation.vy = *anim++;
		pos->rotation.vz = *anim++;
		pos->location.vx = *anim++;
		pos->location.vy = *anim++;
		pos->location.vz = *anim++;
		initializePosData(pos);
	}
	startAnimation(entity, 0);
	addObject((int16_t)type, (int16_t)instanceId, tick, renderDigimon);
}

void renderDigimon(instanceId)
	int16_t instanceId;
{
	Entity *entity;
	PositionData *pos;
	MATRIX lw;
	MATRIX ls;
	int32_t boneCount;
	int32_t i;

	entity = ENTITY_TABLE[instanceId];
	if (entity->isOnMap != 2) {
		if (entity->isOnMap == 0) {
			return;
		}
		if (entity->isOnScreen == 0) {
			return;
		}
	}
	boneCount = DIGIMON_DATA[entity->type].boneCount;
	pos = entity->posData;
	for (i = 0; i < boneCount; pos++, i++) {
		if (pos->obj.tmd == NULL) {
			continue;
		}
		GsGetLws(pos->obj.coord2, &lw, &ls);
		GsSetLightMatrix(&lw);
		GsSetLsMatrix(&ls);
		if (ENTITY_TABLE[instanceId]->flatSprite != -1) {
			continue;
		}
		if (instanceId == 1) {
			if (PARTNER_WIREFRAME_TOTAL != 0x10) {
				renderWireframed(&pos->obj, PARTNER_WIREFRAME_TOTAL);
				continue;
			}
			if (PARTNER_WIREFRAME_SUB[i] != 0x10) {
				renderWireframed(&pos->obj, PARTNER_WIREFRAME_SUB[i]);
				continue;
			}
			GsSortObject4(&pos->obj, ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0));
			continue;
		}
		if (instanceId == 2) {
			if (ENTITY1_WIREFRAME_TOTAL != 0x10) {
				renderWireframed(&pos->obj, ENTITY1_WIREFRAME_TOTAL);
				continue;
			}
			GsSortObject4(&pos->obj, ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0));
			continue;
		}
		GsSortObject4(&pos->obj, ACTIVE_ORDERING_TABLE, 2, getScratchAddr(0));
	}
	if (ENTITY_TABLE[instanceId]->flatSprite != -1) {
		renderFlatDigimon(ENTITY_TABLE[instanceId]);
	}
	if (instanceId == 0) {
		if (PLAYER_SHADOW_ENABLED == 0) {
			return;
		}
		renderDropShadow(ENTITY_TABLE[instanceId]);
		return;
	}
	renderDropShadow(ENTITY_TABLE[instanceId]);
}

void removeEntity(int32_t objectId, int32_t entityId)
{
	Entity *entity;

	removeObject((int16_t)objectId, (int16_t)entityId);
	if ((entityId != 1) && (entityId != 0)) {
		entity = ENTITY_TABLE[entityId];
		free3(entity->anim.momentum);
		free3(entity->posData);
		ENTITY_TABLE[entityId] = NULL;
	}
}

void setupEntityMatrix(int32_t entityId)
{
	MATRIX *m;
	PositionData *posData;

	if ((entityId >= 0) && (entityId < ENTITY_MAX)) {
		posData = ENTITY_TABLE[entityId]->posData;
		RotMatrix(&posData->rotation, m = &posData->posMatrix.coord);
		ScaleMatrix(m, &posData->scale);
		TransMatrix(m, &posData->location);
		posData->posMatrix.flg = 0;
	}
}

void setEntityPosition(int32_t entityId, int32_t x, int32_t y, int32_t z)
{
	VECTOR *location;

	if ((entityId >= 0) && (entityId < ENTITY_MAX)) {
		location = &ENTITY_TABLE[entityId]->posData->location;
		location->vx = x;
		location->vy = y;
		location->vz = z;
		ENTITY_TABLE[entityId]->anim.locX = location->vx << 15;
		ENTITY_TABLE[entityId]->anim.locY = location->vy << 15;
		ENTITY_TABLE[entityId]->anim.locZ = location->vz << 15;
	}
}

void setEntityRotation(entityId, x, y, z)
	int32_t entityId;
int16_t x;
int16_t y;
int16_t z;
{
	SVECTOR *rotation;

	if ((entityId >= 0) && (entityId < ENTITY_MAX)) {
		rotation = &ENTITY_TABLE[entityId]->posData->rotation;
		rotation->vx = x;
		rotation->vy = y;
		rotation->vz = z;
	}
}

void renderWireframed(GsDOBJ2 *obj, int32_t wireFrameShare)
{
	int32_t primn;
	CVECTOR col;
	long p;
	long flag;
	long otz;
	MATRIX m;
	struct TMD_STRUCT *tmd;
	SVECTOR *vert;
	SVECTOR *normal;
	u_char *prim;
	GsCOORDINATE2 *coord;
	u_char *pk;
	int32_t start;
	uint32_t cur;
	int32_t i;
	int8_t color;
	int16_t min;
	POLY_GT3 *gt3;
	POLY_GT4 *gt4;
	LINE_F4 *lf3;
	LINE_F2 *lf2;

	min = WIREFRAME_COLOR_MIN;
	color = min + rand() % (WIREFRAME_COLOR_MAX - min);
	tmd = (struct TMD_STRUCT *)obj->tmd;
	vert = (SVECTOR *)tmd->vertop;
	normal = (SVECTOR *)tmd->nortop;
	prim = (u_char *)tmd->primtop;
	primn = tmd->primn;
	pk = (u_char *)GsGetWorkBase();
	col.r = col.g = col.b = 0x80;
	coord = obj->coord2;
	if (coord->flg == 0) {
		coord->flg = 1;
		MulMatrix0(&coord->coord, &coord->super->workm, &coord->workm);
	}
	MulMatrix0(&GsLIGHTWSMATRIX, &coord->workm, &m);
	SetLightMatrix(&m);
	CompMatrix(&GsWSMATRIX, &coord->workm, &m);
	setRotTransMatrix(&m);
	for (i = 0; i < primn; i++) {
		if ((prim[3] & 0xFC) == 0x34) {
			cur = (uint32_t)prim;
			if (WIREFRAME_RNG_TABLE[i & 0xF] < wireFrameShare) {
				gt3 = (POLY_GT3 *)pk;
				start = (int32_t)pk;
				if (0 < RotNclip3(&vert[*(u_short *)(prim + 0x12)], &vert[*(u_short *)(prim + 0x16)],
				                  &vert[*(u_short *)(prim + 0x1A)], (long *)&gt3->x0, (long *)&gt3->x1,
				                  (long *)&gt3->x2, &p, &otz, &flag)) {
					NormalColorCol3(&normal[*(u_short *)(prim + 0x10)],
					                &normal[*(u_short *)(prim + 0x14)],
					                &normal[*(u_short *)(prim + 0x18)], &col, (CVECTOR *)&gt3->r0,
					                (CVECTOR *)&gt3->r1, (CVECTOR *)&gt3->r2);
					setUV3(gt3, prim[4], prim[5], prim[8], prim[9], prim[0xC], prim[0xD]);
					gt3->clut = *(u_short *)(prim + 6);
					gt3->tpage = *(u_short *)(prim + 0xA);
					setPolyGT3(gt3);
					otz = otz >> 2;
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, gt3);
					pk = (u_char *)(start + sizeof(POLY_GT3));
				}
			} else {
				lf3 = (LINE_F4 *)pk;
				start = (int32_t)pk;
				if (0 < RotNclip3(&vert[*(u_short *)(prim + 0x12)], &vert[*(u_short *)(prim + 0x16)],
				                  &vert[*(u_short *)(prim + 0x1A)], (long *)&lf3->x0, (long *)&lf3->x1,
				                  (long *)&lf3->x2, &p, &otz, &flag)) {
					lf3->x3 = lf3->x0;
					lf3->y3 = lf3->y0;
					setlen(lf3, 6);
					setcode(lf3, 0x4C);
					lf3->pad = 0x55555555;
					lf3->b0 = color;
					lf3->g0 = color;
					lf3->r0 = color;
					otz = otz >> 2;
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, lf3);
					pk = (u_char *)(start + sizeof(LINE_F4));
				}
			}
			prim = (u_char *)(cur + 0x1C);
		} else if ((prim[3] & 0xFC) == 0x3C) {
			if (WIREFRAME_RNG_TABLE[i & 0xF] < wireFrameShare) {
				gt4 = (POLY_GT4 *)pk;
				start = (int32_t)pk;
				if (0 < RotNclip4(&vert[*(u_short *)(prim + 0x16)], &vert[*(u_short *)(prim + 0x1A)],
				                  &vert[*(u_short *)(prim + 0x1E)], &vert[*(u_short *)(prim + 0x22)],
				                  (long *)&gt4->x0, (long *)&gt4->x1, (long *)&gt4->x2, (long *)&gt4->x3, &p,
				                  &otz, &flag)) {
					NormalColorCol3(&normal[*(u_short *)(prim + 0x14)],
					                &normal[*(u_short *)(prim + 0x18)],
					                &normal[*(u_short *)(prim + 0x1C)], &col, (CVECTOR *)&gt4->r0,
					                (CVECTOR *)&gt4->r1, (CVECTOR *)&gt4->r2);
					NormalColorCol(&normal[*(u_short *)(prim + 0x20)], &col, (CVECTOR *)&gt4->r3);
					setUV4(gt4, prim[4], prim[5], prim[8], prim[9], prim[0xC], prim[0xD], prim[0x10], prim[0x11]);
					gt4->clut = *(u_short *)(prim + 6);
					gt4->tpage = *(u_short *)(prim + 0xA);
					setPolyGT4(gt4);
					otz = otz >> 2;
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, gt4);
					pk = (u_char *)(start + sizeof(POLY_GT4));
				}
			} else {
				lf3 = (LINE_F4 *)pk;
				if (0 < RotNclip4(&vert[*(u_short *)(prim + 0x16)], &vert[*(u_short *)(prim + 0x1A)],
				                  &vert[*(u_short *)(prim + 0x1E)], &vert[*(u_short *)(prim + 0x22)],
				                  (long *)&lf3->x0, (long *)&lf3->x1, (long *)&lf3->x3, (long *)&lf3->x2, &p,
				                  &otz, &flag)) {
					setlen(lf3, 6);
					setcode(lf3, 0x4C);
					lf3->pad = 0x55555555;
					lf3->b0 = color;
					lf3->g0 = color;
					lf3->r0 = color;
					otz = otz >> 2;
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, lf3);
					lf2 = (LINE_F2 *)(pk + sizeof(LINE_F4));
					setLineF2(lf2);
					lf2->b0 = color;
					lf2->g0 = color;
					lf2->r0 = color;
					setXY2(lf2, lf3->x3, lf3->y3, lf3->x0, lf3->y0);
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, lf2);
					pk = (u_char *)lf2 + sizeof(LINE_F2);
				}
			}
			prim = (u_char *)((uint32_t)prim + 0x24);
		} else {
			break;
		}
	}
	GsSetWorkBase((PACKET *)pk);
}

void resetFlattenGlobal(void)
{
	int32_t i;

	TAMER_ENTITY.entity.flatSprite = -1;

	PARTNER_ENTITY.digimonEntity.entity.flatTimer = 0;
	PARTNER_ENTITY.digimonEntity.entity.flatSprite = -1;

	for (i = 0; i < 8; i++) {
		NPC_ENTITIES[i].digimonEntity.entity.flatTimer = 0;
		NPC_ENTITIES[i].digimonEntity.entity.flatSprite = -1;
	}
}

void loadDigimonTexture(int32_t digiType, char *path,
                        ModelComponent *component)
{
	void *buffer;
	char fileName[32];

	strrchr(path, '\\');
	strcpy(fileName, PATH_ALLTIM_TIM);

	buffer = malloc3(0x4800);

	readFileSectors(fileName, buffer, digiType * 9, 9);
	uploadModelTexture(buffer, component);

	free3(buffer);
}

void concatStrings(char *dst, char *src1, char *src2)
{
	while (*src1 != '\0') {
		*dst++ = *src1++;
	}

	while (*src2 != '\0') {
		*dst++ = *src2++;
	}

	*dst = '\0';
}

void handleNullModel(void)
{
}

void initializeModelComponents(void)
{
	ModelComponent t;
	int32_t i;

	t.useCount = 0;
	t.modelPtr = NULL;
	t.animTablePtr = NULL;
	t.mmdPtr = NULL;
	t.pixelPage = 0;
	t.clutPage = 0;
	t.pixelOffsetY = 0;
	t.pixelOffsetX = 0;
	t.modelId = -1;
	t.digiType = -1;

	for (i = 0; i < 5; ++i) {
		NPC_MODEL[i] = t;
	}

	for (i = 0; i < 5; ++i) {
		NPC_MODEL_TAKEN[i] = 0;
	}

	for (i = 0; i < 16; ++i) {
		UNKNOWN_MODEL[i] = t;
	}

	for (i = 0; i < 16; ++i) {
		UNKNOWN_MODEL_TAKEN[i] = 0;
	}
}

ModelComponent *loadMMD(int32_t digiType, int32_t modelType)
{
	ModelComponent *m;
	char *name;
	char path[32];
	int32_t i;
	int32_t slot;
	int32_t k;
	int32_t size;

	if (modelType == 1) {
		return 0;
	}
	if (digiType < 0 || digiType >= 0xB4) {
		return 0;
	}
	if (modelType == 0) {
		slot = -1;
		for (m = NPC_MODEL, i = 0; i < 5; m++, i++) {
			if (m->useCount == 0) {
				slot = i;
			} else if (m->digiType == digiType) {
				break;
			}
		}
		if (i == 5) {
			if (slot == -1) {
				return 0;
			}
			m = &NPC_MODEL[slot];
			m->digiType = digiType;
		}
		++m->useCount;
		if (m->useCount != 1) {
			return m;
		}
		k = 0;
		while (NPC_MODEL_TAKEN[k] != 0 && k < 5) {
			++k;
		}
		if (k == 5) {
			return 0;
		}
		NPC_MODEL_TAKEN[k] = 1;
		m->pixelPage = k / 2 + 0x16;
		m->clutPage = ((((k << 4) + 0x20) >> 4) & 0x3F) | 0x7A00;
		m->pixelOffsetX = 0;
		m->pixelOffsetY = (k % 2) << 7;
		m->modelId = k;
		name = PTR_DIGIMON_FILE_NAMES[digiType];
		loadDigimonTexture(digiType, name, m);
		concatStrings(path, PATH_CHDAT_MMD0, name);
		concatStrings(path, path, FILE_EXT_MMD);
		path[9] = digiType / 30 + '0';
		size = lookupFileSize(path);
		m->mmdPtr = malloc3((size + 0x7FF) & ~0x7FF);
		if (m->mmdPtr == NULL) {
			handleNullModel();
		}
		readFile(path, m->mmdPtr);
		m->modelPtr = (TMDModel *)((char *)m->mmdPtr + ((int32_t *)m->mmdPtr)[0]);
		m->animTablePtr = (int32_t *)((char *)m->mmdPtr + ((int32_t *)m->mmdPtr)[1]);
		GsMapModelingData((u_long *)&m->modelPtr->flags);
		updateTMDTextureData((char *)m->modelPtr, m->pixelPage, m->pixelOffsetX, m->pixelOffsetY,
		                     m->clutPage - 0x7A00);
		return m;
	}
	if (modelType == 2) {
		TAMER_MODEL.pixelPage = 0x15;
		TAMER_MODEL.clutPage = 0x7A00;
		TAMER_MODEL.pixelOffsetX = 0;
		TAMER_MODEL.pixelOffsetY = 0;
		TAMER_MODEL.modelId = 0;
		m = &TAMER_MODEL;
		TAMER_MODEL.mmdPtr = TAMER_MODEL_BUFFER;
	} else if (modelType == 3) {
		PARTNER_MODEL.pixelPage = 0x15;
		PARTNER_MODEL.clutPage = 0x7A01;
		PARTNER_MODEL.pixelOffsetX = 0;
		PARTNER_MODEL.pixelOffsetY = 0x80;
		PARTNER_MODEL.modelId = 0;
		m = &PARTNER_MODEL;
		PARTNER_MODEL.mmdPtr = PARTNER_MODEL_BUFFER;
	} else {
		return 0;
	}
	name = PTR_DIGIMON_FILE_NAMES[digiType];
	loadDigimonTexture(digiType, name, m);
	concatStrings(path, PATH_CHDAT_MMD0, name);
	concatStrings(path, path, FILE_EXT_MMD);
	path[9] = digiType / 30 + '0';
	readFile(path, m->mmdPtr);
	m->modelPtr = (TMDModel *)((char *)m->mmdPtr + ((int32_t *)m->mmdPtr)[0]);
	m->animTablePtr = (int32_t *)((char *)m->mmdPtr + ((int32_t *)m->mmdPtr)[1]);
	GsMapModelingData((u_long *)m->modelPtr + 1);
	updateTMDTextureData((char *)m->modelPtr, m->pixelPage, m->pixelOffsetX, m->pixelOffsetY,
	                     m->clutPage - 0x7A00);
	return m;
}

void unloadModel(int32_t digiType, int32_t modelType)
{
	int32_t i;
	ModelComponent *m;

	if (modelType == 0) {
		for (m = NPC_MODEL, i = 0; i < 5; ++m, ++i) {
			if (m->digiType == digiType) {
				break;
			}
		}
		if (i != 5) {
			--m->useCount;
			if (m->useCount == 0) {
				m->digiType = -1;
				if (m->mmdPtr != NULL) {
					free3(m->mmdPtr);
				}
				m->modelPtr = NULL;
				m->animTablePtr = NULL;
				m->mmdPtr = NULL;
				if (m->modelId != -1) {
					NPC_MODEL_TAKEN[m->modelId] = 0;
				}
				m->modelId = -1;
			}
		}
	} else if (modelType == 1) {
		for (m = UNKNOWN_MODEL, i = 0; i < 16; ++m, ++i) {
			if (m->useCount == digiType) {
				break;
			}
		}
		if (i != 16) {
			m->useCount = 0;
			if (m->modelId != -1) {
				UNKNOWN_MODEL_TAKEN[m->modelId] = 0;
			}
			m->modelId = -1;
		}
	}
}

ModelComponent *getEntityModelComponent(int32_t instance, int32_t type)
{
	ModelComponent *p;
	int32_t i;

	i = type;
	if (i == 2) {
		return &TAMER_MODEL;
	}
	if (i == 3) {
		return &PARTNER_MODEL;
	}
	if (i == 0) {
		i = instance;
		if ((i < 0) || (instance >= 0xB4)) {
			return 0;
		}
		p = NPC_MODEL;
		for (i = 0; i < 5; p++, i++) {
			if (p->digiType == instance) {
				break;
			}
		}
		if (i == 5) {
			return 0;
		}
		if (p->useCount != 0) {
			goto done;
		}
		return 0;
	}
	if (i == 1) {
		i = instance;
		if ((instance < 0) || (i >= 0x96)) {
			return 0;
		}
		p = &UNKNOWN_MODEL[i];
		if (p->useCount == 0) {
			return 0;
		}
	}
done:
	return p;
}

int32_t getEntityType(Entity *entity)
{
	int32_t i;
	int32_t v;

	for (i = 0; i < 10; i++) {
		if (ENTITY_TABLE[i] == entity) {
			break;
		}
	}
	switch (i) {
	case 0:
		v = 2;
		break;
	case 1:
		v = 3;
		break;
	case 10:
		v = -1;
		break;
	default:
		v = 0;
		break;
	}
	return v;
}

void uploadModelTexture(void *textureData, ModelComponent *component)
{
	GsIMAGE img;
	RECT rect;

	GsGetTimInfo((unsigned long *)textureData + 1, &img);

	img.px = applyTPageOffset((component->pixelPage % 16) * 64,
	                          component->pixelOffsetX);
	img.py = applyTPageOffset((component->pixelPage / 16) * 256,
	                          component->pixelOffsetY);
	img.cx = (component->clutPage & 0x3f) << 4;
	img.cy = component->clutPage >> 6;

	setRECT(&rect, img.px, img.py, img.pw, img.ph);
	LoadImage(&rect, img.pixel);

	if ((img.pmode >> 3) & 1) {
		setRECT(&rect, img.cx, img.cy, img.cw, img.ch);
		LoadImage(&rect, img.clut);
	}

	DrawSync(0);
}

uint8_t *loadMMDAsync(int32_t digimonType, int32_t entityType, uint8_t *buffer,
                      EvoModelData *modelData, uint8_t *readComplete)
{
	ModelComponent *m;
	char path[32];
	char tim[32];
	char *name;
	int32_t i;
	int32_t slot;
	int32_t align;

	if (digimonType < 0 || digimonType >= 0xB4) {
		return 0;
	}
	if (entityType == 0) {
		slot = -1;
		for (m = NPC_MODEL, i = 0; i < 5; m++, i++) {
			if (m->useCount == 0) {
				slot = i;
			} else if (m->digiType == digimonType) {
				break;
			}
		}
		if (i == 5) {
			if (slot == -1) {
				return 0;
			}
			m = &NPC_MODEL[slot];
			m->digiType = digimonType;
		}
		++m->useCount;
		if (m->useCount != 1) {
			return 0;
		}
	} else if (entityType != 2 && entityType != 3) {
		return 0;
	}
	name = PTR_DIGIMON_FILE_NAMES[digimonType];
	strcpy(tim, PATH_ALLTIM_TIM);
	align = (int32_t)buffer & 3;
	if (align != 0) {
		buffer += 4 - align;
	}
	readFileSectors(tim, buffer, digimonType * 9, 9);
	modelData->imagePtr = buffer;
	modelData->imageSize = 0x4800;
	buffer += modelData->imageSize;
	concatStrings(path, PATH_CHDAT_MMD0, name);
	concatStrings(path, path, FILE_EXT_MMD);
	path[9] = digimonType / 30 + '0';
	align = (int32_t)buffer & 3;
	if (align != 0) {
		buffer += 4 - align;
	}
	addFileReadRequestPath(path, buffer, readComplete, 0, 0);
	modelData->modelPtr = buffer;
	modelData->modelSize = lookupFileSize(path);
	buffer += modelData->modelSize;
	return buffer;
}

ModelComponent *applyMMD(int32_t digimonType, int32_t entityType,
                         EvoModelData *modelData)
{
	ModelComponent *m;
	char *name;
	char path[32];
	GsIMAGE img;
	RECT rect;
	int32_t i;
	int32_t k;
	int32_t size;
	int32_t digit;

	if (digimonType < 0 || digimonType >= 0xB4) {
		return 0;
	}
	if (entityType == 0) {
		for (m = NPC_MODEL, i = 0; i < 5; m++, i++) {
			if (m->digiType == digimonType) {
				break;
			}
		}
		if (i == 5) {
			return 0;
		}
		k = 0;
		while (NPC_MODEL_TAKEN[k] != 0 && k < 5) {
			++k;
		}
		if (k == 5) {
			return 0;
		}
		NPC_MODEL_TAKEN[k] = 1;
		m->pixelPage = k / 2 + 0x16;
		m->clutPage = ((((k << 4) + 0x20) >> 4) & 0x3F) | 0x7A00;
		m->pixelOffsetX = 0;
		m->pixelOffsetY = (k % 2) << 7;
		m->modelId = k;
		name = PTR_DIGIMON_FILE_NAMES[digimonType];
		loadDigimonTexture(digimonType, name, m);
		concatStrings(path, PATH_CHDAT_TMD0, name);
		concatStrings(path, path, FILE_EXT_TMD);
		path[9] = digit = digimonType / 30 + '0';
		size = lookupFileSize(path);
		m->modelPtr = malloc3((size + 0x7FF) & ~0x7FF);
		readFile(path, m->modelPtr);
		GsMapModelingData((u_long *)m->modelPtr + 1);
		updateTMDTextureData((char *)m->modelPtr, m->pixelPage, m->pixelOffsetX, m->pixelOffsetY,
		                     m->clutPage - 0x7A00);
		concatStrings(path, PATH_CHDAT_MTN0, name);
		concatStrings(path, path, FILE_EXT_MTN);
		path[9] = digit;
		size = lookupFileSize(path);
		m->animTablePtr = malloc3((size + 0x7FF) & ~0x7FF);
		readFile(path, m->animTablePtr);
		return m;
	}
	if (entityType == 2) {
	} else if (entityType == 3) {
		PARTNER_MODEL.pixelPage = 0x15;
		PARTNER_MODEL.clutPage = 0x7A01;
		PARTNER_MODEL.pixelOffsetX = 0;
		PARTNER_MODEL.pixelOffsetY = 0x80;
		PARTNER_MODEL.modelId = 0;
		PARTNER_MODEL.mmdPtr = PARTNER_MODEL_BUFFER;
		m = &PARTNER_MODEL;
		PARTNER_MODEL.useCount = 0;
	} else {
		return 0;
	}
	++m->useCount;
	if (m->useCount != 1) {
		return m;
	}
	GsGetTimInfo((unsigned long *)modelData->imagePtr + 1, &img);
	img.px = applyTPageOffset((m->pixelPage % 16) * 64, m->pixelOffsetX);
	img.py = applyTPageOffset((m->pixelPage / 16) * 256, m->pixelOffsetY);
	img.cx = (m->clutPage & 0x3f) << 4;
	img.cy = m->clutPage >> 6;
	setRECT(&rect, img.px, img.py, img.pw, img.ph);
	LoadImage(&rect, img.pixel);
	if ((img.pmode >> 3) & 1) {
		setRECT(&rect, img.cx, img.cy, img.cw, img.ch);
		LoadImage(&rect, img.clut);
	}
	memcpy(m->mmdPtr, modelData->modelPtr, modelData->modelSize);
	m->modelPtr = (TMDModel *)((char *)m->mmdPtr + ((int32_t *)m->mmdPtr)[0]);
	m->animTablePtr = (int32_t *)((char *)m->mmdPtr + ((int32_t *)m->mmdPtr)[1]);
	GsMapModelingData((u_long *)m->modelPtr + 1);
	updateTMDTextureData((char *)m->modelPtr, m->pixelPage, m->pixelOffsetX, m->pixelOffsetY,
	                     m->clutPage - 0x7A00);
	return m;
}
