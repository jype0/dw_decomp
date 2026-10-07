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

extern uint8_t PARTNER_MODEL_BUFFER[];
extern uint8_t TAMER_MODEL_BUFFER[];

extern GsOT *ACTIVE_ORDERING_TABLE;
void renderDropShadow(Entity *entity);
void setRotTransMatrix(MATRIX *m);
int32_t addScreenPolyFT4(POLY_FT4 *poly, SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
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

// clang-format off
SkeletonBone SKELETON_BOTAMON[3] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
};

int32_t PARTNER_WIREFRAME_TOTAL = 0x00000010;

int32_t ENTITY1_WIREFRAME_TOTAL = 0x00000010;

int32_t PLAYER_SHADOW_ENABLED = 0x00000001;

int16_t WIREFRAME_COLOR_MIN = 0x0037;

int16_t WIREFRAME_COLOR_MAX = 0x00ff;

char STR_DIGIMON_FILE_NAME_BOYS[] = "BOYS";

char STR_DIGIMON_FILE_NAME_BOTA[] = "BOTA";

char STR_DIGIMON_FILE_NAME_KORO[] = "KORO";

char STR_DIGIMON_FILE_NAME_AGUM[] = "AGUM";

char STR_DIGIMON_FILE_NAME_BETA[] = "BETA";

char STR_DIGIMON_FILE_NAME_GREY[] = "GREY";

char STR_DIGIMON_FILE_NAME_DEVI[] = "DEVI";

char STR_DIGIMON_FILE_NAME_AIRD[] = "AIRD";

char STR_DIGIMON_FILE_NAME_TYRA[] = "TYRA";

char STR_DIGIMON_FILE_NAME_MERA[] = "MERA";

char STR_DIGIMON_FILE_NAME_SEAD[] = "SEAD";

char STR_DIGIMON_FILE_NAME_NUME[] = "NUME";

char STR_DIGIMON_FILE_NAME_MTGR[] = "MTGR";

char STR_DIGIMON_FILE_NAME_MAME[] = "MAME";

char STR_DIGIMON_FILE_NAME_MONZ[] = "MONZ";

char STR_DIGIMON_FILE_NAME_PUNI[] = "PUNI";

char STR_DIGIMON_FILE_NAME_TUNO[] = "TUNO";

char STR_DIGIMON_FILE_NAME_GABU[] = "GABU";

char STR_DIGIMON_FILE_NAME_ELEC[] = "ELEC";

char STR_DIGIMON_FILE_NAME_KABU[] = "KABU";

char STR_DIGIMON_FILE_NAME_ANGE[] = "ANGE";

char STR_DIGIMON_FILE_NAME_BIRD[] = "BIRD";

char STR_DIGIMON_FILE_NAME_GARU[] = "GARU";

char STR_DIGIMON_FILE_NAME_YUKI[] = "YUKI";

char STR_DIGIMON_FILE_NAME_HOEE[] = "HOEE";

char STR_DIGIMON_FILE_NAME_VEGI[] = "VEGI";

char STR_DIGIMON_FILE_NAME_SKUL[] = "SKUL";

char STR_DIGIMON_FILE_NAME_MTMA[] = "MTMA";

char STR_DIGIMON_FILE_NAME_VEDA[] = "VEDA";

char STR_DIGIMON_FILE_NAME_POYO[] = "POYO";

char STR_DIGIMON_FILE_NAME_TOKO[] = "TOKO";

char STR_DIGIMON_FILE_NAME_PATA[] = "PATA";

char STR_DIGIMON_FILE_NAME_KUNE[] = "KUNE";

char STR_DIGIMON_FILE_NAME_UNIM[] = "UNIM";

char STR_DIGIMON_FILE_NAME_OGRE[] = "OGRE";

char STR_DIGIMON_FILE_NAME_SHEL[] = "SHEL";

char STR_DIGIMON_FILE_NAME_CENT[] = "CENT";

char STR_DIGIMON_FILE_NAME_BAKE[] = "BAKE";

char STR_DIGIMON_FILE_NAME_DORI[] = "DORI";

char STR_DIGIMON_FILE_NAME_SCUM[] = "SCUM";

char STR_DIGIMON_FILE_NAME_ANDR[] = "ANDR";

char STR_DIGIMON_FILE_NAME_GIRO[] = "GIRO";

char STR_DIGIMON_FILE_NAME_ETEM[] = "ETEM";

char STR_DIGIMON_FILE_NAME_YURA[] = "YURA";

char STR_DIGIMON_FILE_NAME_TANE[] = "TANE";

char STR_DIGIMON_FILE_NAME_PIYO[] = "PIYO";

char STR_DIGIMON_FILE_NAME_PALM[] = "PALM";

char STR_DIGIMON_FILE_NAME_MONO[] = "MONO";

char STR_DIGIMON_FILE_NAME_LEOM[] = "LEOM";

char STR_DIGIMON_FILE_NAME_SIRA[] = "SIRA";

char STR_DIGIMON_FILE_NAME_COCA[] = "COCA";

char STR_DIGIMON_FILE_NAME_KUWA[] = "KUWA";

char STR_DIGIMON_FILE_NAME_MOJA[] = "MOJA";

char STR_DIGIMON_FILE_NAME_NANI[] = "NANI";

char STR_DIGIMON_FILE_NAME_MGDR[] = "MGDR";

char STR_DIGIMON_FILE_NAME_PICC[] = "PICC";

char STR_DIGIMON_FILE_NAME_DIGI[] = "DIGI";

char STR_DIGIMON_FILE_NAME_PENM[] = "PENM";

char STR_DIGIMON_FILE_NAME_IGAM[] = "IGAM";

char STR_DIGIMON_FILE_NAME_HOUO[] = "HOUO";

char STR_DIGIMON_FILE_NAME_HKAB[] = "HKAB";

char STR_DIGIMON_FILE_NAME_MGSD[] = "MGSD";

char STR_DIGIMON_FILE_NAME_wEAG[] = "wEAG";

char STR_DIGIMON_FILE_NAME_PANJ[] = "PANJ";

char STR_DIGIMON_FILE_NAME_GGDR[] = "GGDR";

char STR_DIGIMON_FILE_NAME_MTET[] = "MTET";

char STR_DIGIMON_FILE_NAME_VAND[] = "VAND";

char STR_DIGIMON_FILE_NAME_YANM[] = "YANM";

char STR_DIGIMON_FILE_NAME_GOTU[] = "GOTU";

char STR_DIGIMON_FILE_NAME_FLAR[] = "FLAR";

char STR_DIGIMON_FILE_NAME_WARU[] = "WARU";

char STR_DIGIMON_FILE_NAME_YKAG[] = "YKAG";

char STR_DIGIMON_FILE_NAME_HYOG[] = "HYOG";

char STR_DIGIMON_FILE_NAME_PCSC[] = "PCSC";

char STR_DIGIMON_FILE_NAME_DOKU[] = "DOKU";

char STR_DIGIMON_FILE_NAME_SIMA[] = "SIMA";

char STR_DIGIMON_FILE_NAME_TANK[] = "TANK";

char STR_DIGIMON_FILE_NAME_REDV[] = "REDV";

char STR_DIGIMON_FILE_NAME_JMOJ[] = "JMOJ";

char STR_DIGIMON_FILE_NAME_NISE[] = "NISE";

char STR_DIGIMON_FILE_NAME_GOBR[] = "GOBR";

char STR_DIGIMON_FILE_NAME_TUTI[] = "TUTI";

char STR_DIGIMON_FILE_NAME_PSYC[] = "PSYC";

char STR_DIGIMON_FILE_NAME_MODO[] = "MODO";

char STR_DIGIMON_FILE_NAME_TOYA[] = "TOYA";

char STR_DIGIMON_FILE_NAME_PIDD[] = "PIDD";

char STR_DIGIMON_FILE_NAME_ARUR[] = "ARUR";

char STR_DIGIMON_FILE_NAME_GERE[] = "GERE";

char STR_DIGIMON_FILE_NAME_VARM[] = "VARM";

char STR_DIGIMON_FILE_NAME_FUGA[] = "FUGA";

char STR_DIGIMON_FILE_NAME_TKKA[] = "TKKA";

char STR_DIGIMON_FILE_NAME_MRIS[] = "MRIS";

char STR_DIGIMON_FILE_NAME_GARD[] = "GARD";

char STR_DIGIMON_FILE_NAME_MCHO[] = "MCHO";

char STR_DIGIMON_FILE_NAME_ICEM[] = "ICEM";

char STR_DIGIMON_FILE_NAME_AKAT[] = "AKAT";

char STR_DIGIMON_FILE_NAME_TUKA[] = "TUKA";

char STR_DIGIMON_FILE_NAME_SHAM[] = "SHAM";

char STR_DIGIMON_FILE_NAME_CLEA[] = "CLEA";

char STR_DIGIMON_FILE_NAME_ZASS[] = "ZASS";

char STR_DIGIMON_FILE_NAME_ICDV[] = "ICDV";

char STR_DIGIMON_FILE_NAME_DKRZ[] = "DKRZ";

char STR_DIGIMON_FILE_NAME_SNDY[] = "SNDY";

char STR_DIGIMON_FILE_NAME_SNGB[] = "SNGB";

char STR_DIGIMON_FILE_NAME_BLMR[] = "BLMR";

char STR_DIGIMON_FILE_NAME_GRUR[] = "GRUR";

char STR_DIGIMON_FILE_NAME_SABD[] = "SABD";

char STR_DIGIMON_FILE_NAME_SOUL[] = "SOUL";

char STR_DIGIMON_FILE_NAME_GOLE[] = "GOLE";

char STR_DIGIMON_FILE_NAME_OTAM[] = "OTAM";

char STR_DIGIMON_FILE_NAME_GECO[] = "GECO";

char STR_DIGIMON_FILE_NAME_TENT[] = "TENT";

char STR_DIGIMON_FILE_NAME_WRSE[] = "WRSE";

char STR_DIGIMON_FILE_NAME_INSE[] = "INSE";

char STR_DIGIMON_FILE_NAME_tAKA[] = "tAKA";

char STR_DIGIMON_FILE_NAME_MUGE[] = "MUGE";

char STR_DIGIMON_FILE_NAME_ANLG[] = "ANLG";

char STR_DIGIMON_FILE_NAME_JIJI[] = "JIJI";

char STR_DIGIMON_FILE_NAME_TENS[] = "TENS";

char STR_DIGIMON_FILE_NAME_TONO[] = "TONO";

char STR_DIGIMON_FILE_NAME_SCUD[] = "SCUD";

char STR_DIGIMON_FILE_NAME_JURE[] = "JURE";

char STR_DIGIMON_FILE_NAME_HAGU[] = "HAGU";

char STR_DIGIMON_FILE_NAME_BRIK[] = "BRIK";

char STR_DIGIMON_FILE_NAME_TIRS[] = "TIRS";

char STR_DIGIMON_FILE_NAME_EGOB[] = "EGOB";

char STR_DIGIMON_FILE_NAME_BRAK[] = "BRAK";

char STR_DIGIMON_FILE_NAME_PUTI[] = "PUTI";

char STR_DIGIMON_FILE_NAME_EBET[] = "EBET";

char STR_DIGIMON_FILE_NAME_EGRE[] = "EGRE";

char STR_DIGIMON_FILE_NAME_EDEV[] = "EDEV";

char STR_DIGIMON_FILE_NAME_EAIR[] = "EAIR";

char STR_DIGIMON_FILE_NAME_ETYR[] = "ETYR";

char STR_DIGIMON_FILE_NAME_EMER[] = "EMER";

char STR_DIGIMON_FILE_NAME_ESEA[] = "ESEA";

char STR_DIGIMON_FILE_NAME_ENUM[] = "ENUM";

char STR_DIGIMON_FILE_NAME_EMTG[] = "EMTG";

char STR_DIGIMON_FILE_NAME_EMAM[] = "EMAM";

char STR_DIGIMON_FILE_NAME_EMON[] = "EMON";

char STR_DIGIMON_FILE_NAME_EGAB[] = "EGAB";

char STR_DIGIMON_FILE_NAME_EELE[] = "EELE";

char STR_DIGIMON_FILE_NAME_EKAB[] = "EKAB";

char STR_DIGIMON_FILE_NAME_EANG[] = "EANG";

char STR_DIGIMON_FILE_NAME_EBIR[] = "EBIR";

char STR_DIGIMON_FILE_NAME_EGAR[] = "EGAR";

char STR_DIGIMON_FILE_NAME_EYUK[] = "EYUK";

char STR_DIGIMON_FILE_NAME_EHOE[] = "EHOE";

char STR_DIGIMON_FILE_NAME_EVEG[] = "EVEG";

char STR_DIGIMON_FILE_NAME_ESKU[] = "ESKU";

char STR_DIGIMON_FILE_NAME_EMTM[] = "EMTM";

char STR_DIGIMON_FILE_NAME_EVED[] = "EVED";

char STR_DIGIMON_FILE_NAME_EPAT[] = "EPAT";

char STR_DIGIMON_FILE_NAME_EKUN[] = "EKUN";

char STR_DIGIMON_FILE_NAME_EUNI[] = "EUNI";

char STR_DIGIMON_FILE_NAME_EOGR[] = "EOGR";

char STR_DIGIMON_FILE_NAME_ESHE[] = "ESHE";

char STR_DIGIMON_FILE_NAME_ECEN[] = "ECEN";

char STR_DIGIMON_FILE_NAME_EBAK[] = "EBAK";

char STR_DIGIMON_FILE_NAME_EDOR[] = "EDOR";

char STR_DIGIMON_FILE_NAME_ESCU[] = "ESCU";

char STR_DIGIMON_FILE_NAME_EAND[] = "EAND";

char STR_DIGIMON_FILE_NAME_EGIR[] = "EGIR";

char STR_DIGIMON_FILE_NAME_EETE[] = "EETE";

char STR_DIGIMON_FILE_NAME_EPIY[] = "EPIY";

char STR_DIGIMON_FILE_NAME_EPAL[] = "EPAL";

char STR_DIGIMON_FILE_NAME_EMNO[] = "EMNO";

char STR_DIGIMON_FILE_NAME_ELEO[] = "ELEO";

char STR_DIGIMON_FILE_NAME_ESIR[] = "ESIR";

char STR_DIGIMON_FILE_NAME_ECOC[] = "ECOC";

char STR_DIGIMON_FILE_NAME_EKUW[] = "EKUW";

char STR_DIGIMON_FILE_NAME_EMOJ[] = "EMOJ";

char STR_DIGIMON_FILE_NAME_ENAN[] = "ENAN";

char STR_DIGIMON_FILE_NAME_EMGD[] = "EMGD";

char STR_DIGIMON_FILE_NAME_EPIC[] = "EPIC";

char STR_DIGIMON_FILE_NAME_EDIG[] = "EDIG";

char STR_DIGIMON_FILE_NAME_EIGA[] = "EIGA";

char STR_DIGIMON_FILE_NAME_EPEN[] = "EPEN";

char STR_DIGIMON_FILE_NAME_EVAN[] = "EVAN";

char STR_DIGIMON_FILE_NAME_CEGR[] = "CEGR";

char STR_DIGIMON_FILE_NAME_CEMG[] = "CEMG";

char MAIN_D_801340E4[] = ".MMD";

char MAIN_D_801340EC[] = ".TMD";

char MAIN_D_801340F4[] = ".MTN";

SkeletonBone SKELETON_HIRO[17] = {
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

SkeletonBone SKELETON_GABUMON[22] = {
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

SkeletonBone SKELETON_BETAMON[24] = {
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

SkeletonBone SKELETON_GREYMON[26] = {
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

SkeletonBone SKELETON_DEVIMON[28] = {
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

SkeletonBone SKELETON_TYRANNOMON[27] = {
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

SkeletonBone SKELETON_MERAMON[22] = {
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

SkeletonBone SKELETON_METALGREYMON[30] = {
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

SkeletonBone SKELETON_MONZAEMON[14] = {
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

SkeletonBone SKELETON_GOBURIMON[23] = {
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

SkeletonBone SKELETON_SUKAMON[21] = {
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

SkeletonBone SKELETON_ETEMON[27] = {
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

SkeletonBone SKELETON_FRIGIMON[19] = {
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

SkeletonBone SKELETON_ANDROMON[21] = {
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

SkeletonBone SKELETON_GIROMON[11] = {
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

SkeletonBone SKELETON_ANGEMON[26] = {
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

SkeletonBone SKELETON_PALMON[31] = {
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

SkeletonBone SKELETON_UNIMON[27] = {
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

SkeletonBone SKELETON_VEGIEMON[20] = {
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

SkeletonBone SKELETON_BIYOMON[22] = {
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

SkeletonBone SKELETON_KUNEMON[28] = {
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

SkeletonBone SKELETON_TOKOMON[11] = {
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

SkeletonBone SKELETON_CENTARUMON[27] = {
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

SkeletonBone SKELETON_KOROMON[7] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, 0x02 },
	{ 0x04, 0x05 },
};

SkeletonBone SKELETON_NANIMON[20] = {
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

SkeletonBone SKELETON_SKULLGREYMON[26] = {
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

SkeletonBone SKELETON_NUMEMON[22] = {
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

SkeletonBone SKELETON_BAKEMON[10] = {
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

SkeletonBone SKELETON_BIRDRAMON[24] = {
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

SkeletonBone SKELETON_YURAMON[9] = {
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

SkeletonBone SKELETON_PIXIMON[26] = {
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

SkeletonBone SKELETON_DRIMOGEMON[28] = {
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

SkeletonBone SKELETON_WHAMON[12] = {
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

SkeletonBone SKELETON_YANMAMON[30] = {
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

SkeletonBone SKELETON_GOTSUMON[22] = {
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

SkeletonBone SKELETON_JIJIMON[17] = {
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

SkeletonBone SKELETON_PENGUINMON[25] = {
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

SkeletonBone SKELETON_MOJYAMON[18] = {
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

SkeletonBone SKELETON_TANEMON[10] = {
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

SkeletonBone SKELETON_COELAMON[20] = {
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

SkeletonBone SKELETON_VADEMON[16] = {
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

SkeletonBone SKELETON_SEADRAMON[17] = {
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

SkeletonBone SKELETON_MEGASEADRAMON[15] = {
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

SkeletonBone SKELETON_OGREMON[18] = {
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

SkeletonBone SKELETON_ELECMON[25] = {
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

SkeletonBone SKELETON_KABUTERIMON[28] = {
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

SkeletonBone SKELETON_BRACHIOMON[21] = {
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

SkeletonBone SKELETON_LEOMON[23] = {
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

SkeletonBone SKELETON_MONOCHROMON[24] = {
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

SkeletonBone SKELETON_KUWAGAMON[30] = {
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

SkeletonBone SKELETON_HERCULESKABUTERIMON[26] = {
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

SkeletonBone SKELETON_WARUMONZAEMON[12] = {
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

SkeletonBone SKELETON_PHOENIXMON[31] = {
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

SkeletonBone SKELETON_DIGITAMAMON[12] = {
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

SkeletonBone SKELETON_KOKATORIMON[21] = {
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

SkeletonBone SKELETON_SHELLMON[15] = {
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

SkeletonBone SKELETON_PATAMON[23] = {
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

SkeletonBone SKELETON_FLAREIZAMON[22] = {
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

SkeletonBone SKELETON_HAGURUMON[5] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x01 },
	{ 0x02, 0x01 },
};

SkeletonBone SKELETON_MAMEMON[19] = {
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

SkeletonBone SKELETON_METALMAMEMON[19] = {
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

SkeletonBone SKELETON_GUARDROMON[19] = {
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

SkeletonBone SKELETON_MACHINEDRAMON[32] = {
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

SkeletonBone SKELETON_MARKET_MANAGER[16] = {
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

SkeletonBone SKELETON_KING_SUKAMON[21] = {
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

SkeletonBone SKELETON_MEGADRAMON[29] = {
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

SkeletonBone SKELETON_MYOTISMON[25] = {
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

SkeletonBone SKELETON_GARURUMON[26] = {
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

SkeletonBone SKELETON_TANKMON[17] = {
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

SkeletonBone SKELETON_SHOGUNGEKOMON[28] = {
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

SkeletonBone SKELETON_CHERRYMON[26] = {
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

SkeletonBone SKELETON_TINMON[7] = {
	{ 0xff, 0xff },
	{ 0xff, 0x00 },
	{ 0x00, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, 0x02 },
	{ 0x03, 0x02 },
	{ 0x04, 0x01 },
};

SkeletonBone SKELETON_AIRDRAMON[20] = {
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

SkeletonBone SKELETON_TENTOMON[27] = {
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

SkeletonBone SKELETON_GEKOMON[22] = {
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

SkeletonBone SKELETON_OTAMAMON[15] = {
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

SkeletonBone SKELETON_ANALOGMAN[18] = {
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
	SKELETON_HIRO,
	SKELETON_BOTAMON,
	SKELETON_KOROMON,
	SKELETON_GABUMON,
	SKELETON_BETAMON,
	SKELETON_GREYMON,
	SKELETON_DEVIMON,
	SKELETON_AIRDRAMON,
	SKELETON_TYRANNOMON,
	SKELETON_MERAMON,
	SKELETON_SEADRAMON,
	SKELETON_NUMEMON,
	SKELETON_METALGREYMON,
	SKELETON_MAMEMON,
	SKELETON_MONZAEMON,
	SKELETON_BOTAMON,
	SKELETON_BOTAMON,
	SKELETON_GABUMON,
	SKELETON_ELECMON,
	SKELETON_KABUTERIMON,
	SKELETON_ANGEMON,
	SKELETON_BIRDRAMON,
	SKELETON_GARURUMON,
	SKELETON_FRIGIMON,
	SKELETON_WHAMON,
	SKELETON_VEGIEMON,
	SKELETON_SKULLGREYMON,
	SKELETON_METALMAMEMON,
	SKELETON_VADEMON,
	SKELETON_BOTAMON,
	SKELETON_TOKOMON,
	SKELETON_PATAMON,
	SKELETON_KUNEMON,
	SKELETON_UNIMON,
	SKELETON_OGREMON,
	SKELETON_SHELLMON,
	SKELETON_CENTARUMON,
	SKELETON_BAKEMON,
	SKELETON_DRIMOGEMON,
	SKELETON_SUKAMON,
	SKELETON_ANDROMON,
	SKELETON_GIROMON,
	SKELETON_ETEMON,
	SKELETON_YURAMON,
	SKELETON_TANEMON,
	SKELETON_BIYOMON,
	SKELETON_PALMON,
	SKELETON_MONOCHROMON,
	SKELETON_LEOMON,
	SKELETON_COELAMON,
	SKELETON_KOKATORIMON,
	SKELETON_KUWAGAMON,
	SKELETON_MOJYAMON,
	SKELETON_NANIMON,
	SKELETON_MEGADRAMON,
	SKELETON_PIXIMON,
	SKELETON_DIGITAMAMON,
	SKELETON_PENGUINMON,
	SKELETON_MAMEMON,
	SKELETON_PHOENIXMON,
	SKELETON_HERCULESKABUTERIMON,
	SKELETON_MEGASEADRAMON,
	NULL,
	SKELETON_LEOMON,
	SKELETON_MEGADRAMON,
	SKELETON_ETEMON,
	SKELETON_MYOTISMON,
	SKELETON_YANMAMON,
	SKELETON_GOTSUMON,
	SKELETON_FLAREIZAMON,
	SKELETON_WARUMONZAEMON,
	SKELETON_GABUMON,
	SKELETON_OGREMON,
	SKELETON_SUKAMON,
	SKELETON_KUNEMON,
	SKELETON_UNIMON,
	SKELETON_TANKMON,
	SKELETON_VEGIEMON,
	SKELETON_MOJYAMON,
	SKELETON_DRIMOGEMON,
	SKELETON_GOBURIMON,
	SKELETON_FRIGIMON,
	SKELETON_GABUMON,
	SKELETON_BETAMON,
	SKELETON_GABUMON,
	SKELETON_ANGEMON,
	SKELETON_PALMON,
	SKELETON_NUMEMON,
	SKELETON_MONOCHROMON,
	SKELETON_OGREMON,
	SKELETON_GIROMON,
	SKELETON_SHELLMON,
	SKELETON_GUARDROMON,
	SKELETON_PENGUINMON,
	SKELETON_GOTSUMON,
	SKELETON_KOKATORIMON,
	SKELETON_PATAMON,
	SKELETON_GOBURIMON,
	SKELETON_GABUMON,
	SKELETON_VEGIEMON,
	SKELETON_DEVIMON,
	SKELETON_FLAREIZAMON,
	SKELETON_YANMAMON,
	SKELETON_GOBURIMON,
	SKELETON_MERAMON,
	SKELETON_GARURUMON,
	SKELETON_BIRDRAMON,
	SKELETON_BAKEMON,
	SKELETON_FRIGIMON,
	SKELETON_OTAMAMON,
	SKELETON_GEKOMON,
	SKELETON_TENTOMON,
	SKELETON_MEGASEADRAMON,
	SKELETON_GOTSUMON,
	NULL,
	SKELETON_MACHINEDRAMON,
	SKELETON_ANALOGMAN,
	SKELETON_JIJIMON,
	SKELETON_MARKET_MANAGER,
	SKELETON_SHOGUNGEKOMON,
	SKELETON_KING_SUKAMON,
	SKELETON_CHERRYMON,
	SKELETON_HAGURUMON,
	SKELETON_TINMON,
	SKELETON_TYRANNOMON,
	SKELETON_GOBURIMON,
	SKELETON_BRACHIOMON,
	SKELETON_BOTAMON,
	SKELETON_BETAMON,
	SKELETON_GREYMON,
	SKELETON_DEVIMON,
	SKELETON_AIRDRAMON,
	SKELETON_TYRANNOMON,
	SKELETON_MERAMON,
	SKELETON_SEADRAMON,
	SKELETON_NUMEMON,
	SKELETON_METALGREYMON,
	SKELETON_MAMEMON,
	SKELETON_MONZAEMON,
	SKELETON_GABUMON,
	SKELETON_ELECMON,
	SKELETON_KABUTERIMON,
	SKELETON_ANGEMON,
	SKELETON_BIRDRAMON,
	SKELETON_GARURUMON,
	SKELETON_FRIGIMON,
	SKELETON_WHAMON,
	SKELETON_VEGIEMON,
	SKELETON_SKULLGREYMON,
	SKELETON_METALMAMEMON,
	SKELETON_VADEMON,
	SKELETON_PATAMON,
	SKELETON_KUNEMON,
	SKELETON_UNIMON,
	SKELETON_OGREMON,
	SKELETON_SHELLMON,
	SKELETON_CENTARUMON,
	SKELETON_BAKEMON,
	SKELETON_DRIMOGEMON,
	SKELETON_SUKAMON,
	SKELETON_ANDROMON,
	SKELETON_GIROMON,
	SKELETON_ETEMON,
	SKELETON_BIYOMON,
	SKELETON_PALMON,
	SKELETON_MONOCHROMON,
	SKELETON_LEOMON,
	SKELETON_COELAMON,
	SKELETON_KOKATORIMON,
	SKELETON_KUWAGAMON,
	SKELETON_MOJYAMON,
	SKELETON_NANIMON,
	SKELETON_MEGADRAMON,
	SKELETON_PIXIMON,
	SKELETON_DIGITAMAMON,
	SKELETON_MAMEMON,
	SKELETON_PENGUINMON,
	SKELETON_MYOTISMON,
	SKELETON_GREYMON,
	SKELETON_METALGREYMON,
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

char MAIN_D_8011D190[] = "ALLTIM.TIM";

char *PTR_DIGIMON_FILE_NAMES[180] = {
	STR_DIGIMON_FILE_NAME_BOYS,
	STR_DIGIMON_FILE_NAME_BOTA,
	STR_DIGIMON_FILE_NAME_KORO,
	STR_DIGIMON_FILE_NAME_AGUM,
	STR_DIGIMON_FILE_NAME_BETA,
	STR_DIGIMON_FILE_NAME_GREY,
	STR_DIGIMON_FILE_NAME_DEVI,
	STR_DIGIMON_FILE_NAME_AIRD,
	STR_DIGIMON_FILE_NAME_TYRA,
	STR_DIGIMON_FILE_NAME_MERA,
	STR_DIGIMON_FILE_NAME_SEAD,
	STR_DIGIMON_FILE_NAME_NUME,
	STR_DIGIMON_FILE_NAME_MTGR,
	STR_DIGIMON_FILE_NAME_MAME,
	STR_DIGIMON_FILE_NAME_MONZ,
	STR_DIGIMON_FILE_NAME_PUNI,
	STR_DIGIMON_FILE_NAME_TUNO,
	STR_DIGIMON_FILE_NAME_GABU,
	STR_DIGIMON_FILE_NAME_ELEC,
	STR_DIGIMON_FILE_NAME_KABU,
	STR_DIGIMON_FILE_NAME_ANGE,
	STR_DIGIMON_FILE_NAME_BIRD,
	STR_DIGIMON_FILE_NAME_GARU,
	STR_DIGIMON_FILE_NAME_YUKI,
	STR_DIGIMON_FILE_NAME_HOEE,
	STR_DIGIMON_FILE_NAME_VEGI,
	STR_DIGIMON_FILE_NAME_SKUL,
	STR_DIGIMON_FILE_NAME_MTMA,
	STR_DIGIMON_FILE_NAME_VEDA,
	STR_DIGIMON_FILE_NAME_POYO,
	STR_DIGIMON_FILE_NAME_TOKO,
	STR_DIGIMON_FILE_NAME_PATA,
	STR_DIGIMON_FILE_NAME_KUNE,
	STR_DIGIMON_FILE_NAME_UNIM,
	STR_DIGIMON_FILE_NAME_OGRE,
	STR_DIGIMON_FILE_NAME_SHEL,
	STR_DIGIMON_FILE_NAME_CENT,
	STR_DIGIMON_FILE_NAME_BAKE,
	STR_DIGIMON_FILE_NAME_DORI,
	STR_DIGIMON_FILE_NAME_SCUM,
	STR_DIGIMON_FILE_NAME_ANDR,
	STR_DIGIMON_FILE_NAME_GIRO,
	STR_DIGIMON_FILE_NAME_ETEM,
	STR_DIGIMON_FILE_NAME_YURA,
	STR_DIGIMON_FILE_NAME_TANE,
	STR_DIGIMON_FILE_NAME_PIYO,
	STR_DIGIMON_FILE_NAME_PALM,
	STR_DIGIMON_FILE_NAME_MONO,
	STR_DIGIMON_FILE_NAME_LEOM,
	STR_DIGIMON_FILE_NAME_SIRA,
	STR_DIGIMON_FILE_NAME_COCA,
	STR_DIGIMON_FILE_NAME_KUWA,
	STR_DIGIMON_FILE_NAME_MOJA,
	STR_DIGIMON_FILE_NAME_NANI,
	STR_DIGIMON_FILE_NAME_MGDR,
	STR_DIGIMON_FILE_NAME_PICC,
	STR_DIGIMON_FILE_NAME_DIGI,
	STR_DIGIMON_FILE_NAME_PENM,
	STR_DIGIMON_FILE_NAME_IGAM,
	STR_DIGIMON_FILE_NAME_HOUO,
	STR_DIGIMON_FILE_NAME_HKAB,
	STR_DIGIMON_FILE_NAME_MGSD,
	STR_DIGIMON_FILE_NAME_wEAG,
	STR_DIGIMON_FILE_NAME_PANJ,
	STR_DIGIMON_FILE_NAME_GGDR,
	STR_DIGIMON_FILE_NAME_MTET,
	STR_DIGIMON_FILE_NAME_VAND,
	STR_DIGIMON_FILE_NAME_YANM,
	STR_DIGIMON_FILE_NAME_GOTU,
	STR_DIGIMON_FILE_NAME_FLAR,
	STR_DIGIMON_FILE_NAME_WARU,
	STR_DIGIMON_FILE_NAME_YKAG,
	STR_DIGIMON_FILE_NAME_HYOG,
	STR_DIGIMON_FILE_NAME_PCSC,
	STR_DIGIMON_FILE_NAME_DOKU,
	STR_DIGIMON_FILE_NAME_SIMA,
	STR_DIGIMON_FILE_NAME_TANK,
	STR_DIGIMON_FILE_NAME_REDV,
	STR_DIGIMON_FILE_NAME_JMOJ,
	STR_DIGIMON_FILE_NAME_NISE,
	STR_DIGIMON_FILE_NAME_GOBR,
	STR_DIGIMON_FILE_NAME_TUTI,
	STR_DIGIMON_FILE_NAME_PSYC,
	STR_DIGIMON_FILE_NAME_MODO,
	STR_DIGIMON_FILE_NAME_TOYA,
	STR_DIGIMON_FILE_NAME_PIDD,
	STR_DIGIMON_FILE_NAME_ARUR,
	STR_DIGIMON_FILE_NAME_GERE,
	STR_DIGIMON_FILE_NAME_VARM,
	STR_DIGIMON_FILE_NAME_FUGA,
	STR_DIGIMON_FILE_NAME_TKKA,
	STR_DIGIMON_FILE_NAME_MRIS,
	STR_DIGIMON_FILE_NAME_GARD,
	STR_DIGIMON_FILE_NAME_MCHO,
	STR_DIGIMON_FILE_NAME_ICEM,
	STR_DIGIMON_FILE_NAME_AKAT,
	STR_DIGIMON_FILE_NAME_TUKA,
	STR_DIGIMON_FILE_NAME_SHAM,
	STR_DIGIMON_FILE_NAME_CLEA,
	STR_DIGIMON_FILE_NAME_ZASS,
	STR_DIGIMON_FILE_NAME_ICDV,
	STR_DIGIMON_FILE_NAME_DKRZ,
	STR_DIGIMON_FILE_NAME_SNDY,
	STR_DIGIMON_FILE_NAME_SNGB,
	STR_DIGIMON_FILE_NAME_BLMR,
	STR_DIGIMON_FILE_NAME_GRUR,
	STR_DIGIMON_FILE_NAME_SABD,
	STR_DIGIMON_FILE_NAME_SOUL,
	STR_DIGIMON_FILE_NAME_GOLE,
	STR_DIGIMON_FILE_NAME_OTAM,
	STR_DIGIMON_FILE_NAME_GECO,
	STR_DIGIMON_FILE_NAME_TENT,
	STR_DIGIMON_FILE_NAME_WRSE,
	STR_DIGIMON_FILE_NAME_INSE,
	STR_DIGIMON_FILE_NAME_tAKA,
	STR_DIGIMON_FILE_NAME_MUGE,
	STR_DIGIMON_FILE_NAME_ANLG,
	STR_DIGIMON_FILE_NAME_JIJI,
	STR_DIGIMON_FILE_NAME_TENS,
	STR_DIGIMON_FILE_NAME_TONO,
	STR_DIGIMON_FILE_NAME_SCUD,
	STR_DIGIMON_FILE_NAME_JURE,
	STR_DIGIMON_FILE_NAME_HAGU,
	STR_DIGIMON_FILE_NAME_BRIK,
	STR_DIGIMON_FILE_NAME_TIRS,
	STR_DIGIMON_FILE_NAME_EGOB,
	STR_DIGIMON_FILE_NAME_BRAK,
	STR_DIGIMON_FILE_NAME_PUTI,
	STR_DIGIMON_FILE_NAME_EBET,
	STR_DIGIMON_FILE_NAME_EGRE,
	STR_DIGIMON_FILE_NAME_EDEV,
	STR_DIGIMON_FILE_NAME_EAIR,
	STR_DIGIMON_FILE_NAME_ETYR,
	STR_DIGIMON_FILE_NAME_EMER,
	STR_DIGIMON_FILE_NAME_ESEA,
	STR_DIGIMON_FILE_NAME_ENUM,
	STR_DIGIMON_FILE_NAME_EMTG,
	STR_DIGIMON_FILE_NAME_EMAM,
	STR_DIGIMON_FILE_NAME_EMON,
	STR_DIGIMON_FILE_NAME_EGAB,
	STR_DIGIMON_FILE_NAME_EELE,
	STR_DIGIMON_FILE_NAME_EKAB,
	STR_DIGIMON_FILE_NAME_EANG,
	STR_DIGIMON_FILE_NAME_EBIR,
	STR_DIGIMON_FILE_NAME_EGAR,
	STR_DIGIMON_FILE_NAME_EYUK,
	STR_DIGIMON_FILE_NAME_EHOE,
	STR_DIGIMON_FILE_NAME_EVEG,
	STR_DIGIMON_FILE_NAME_ESKU,
	STR_DIGIMON_FILE_NAME_EMTM,
	STR_DIGIMON_FILE_NAME_EVED,
	STR_DIGIMON_FILE_NAME_EPAT,
	STR_DIGIMON_FILE_NAME_EKUN,
	STR_DIGIMON_FILE_NAME_EUNI,
	STR_DIGIMON_FILE_NAME_EOGR,
	STR_DIGIMON_FILE_NAME_ESHE,
	STR_DIGIMON_FILE_NAME_ECEN,
	STR_DIGIMON_FILE_NAME_EBAK,
	STR_DIGIMON_FILE_NAME_EDOR,
	STR_DIGIMON_FILE_NAME_ESCU,
	STR_DIGIMON_FILE_NAME_EAND,
	STR_DIGIMON_FILE_NAME_EGIR,
	STR_DIGIMON_FILE_NAME_EETE,
	STR_DIGIMON_FILE_NAME_EPIY,
	STR_DIGIMON_FILE_NAME_EPAL,
	STR_DIGIMON_FILE_NAME_EMNO,
	STR_DIGIMON_FILE_NAME_ELEO,
	STR_DIGIMON_FILE_NAME_ESIR,
	STR_DIGIMON_FILE_NAME_ECOC,
	STR_DIGIMON_FILE_NAME_EKUW,
	STR_DIGIMON_FILE_NAME_EMOJ,
	STR_DIGIMON_FILE_NAME_ENAN,
	STR_DIGIMON_FILE_NAME_EMGD,
	STR_DIGIMON_FILE_NAME_EPIC,
	STR_DIGIMON_FILE_NAME_EDIG,
	STR_DIGIMON_FILE_NAME_EIGA,
	STR_DIGIMON_FILE_NAME_EPEN,
	STR_DIGIMON_FILE_NAME_EVAN,
	STR_DIGIMON_FILE_NAME_CEGR,
	STR_DIGIMON_FILE_NAME_CEMG,
};

char MAIN_D_8011D46C[12] = "CHDAT\\TMD0\\";

char MAIN_D_8011D478[12] = "CHDAT\\MTN0\\";

char MAIN_D_8011D484[12] = "CHDAT\\MMD0\\";
// clang-format on

ModelComponent NPC_MODELS[5];
int32_t NPC_MODEL_TAKEN[5];
ModelComponent TAMER_MODEL;
ModelComponent PARTNER_MODEL;
ModelComponent UNKNOWN_MODEL[16];
int32_t UNKNOWN_MODEL_TAKEN[16];

static void *model_bss_order[] = {
	UNKNOWN_MODEL_TAKEN,
	UNKNOWN_MODEL,
	&PARTNER_MODEL,
	&TAMER_MODEL,
	NPC_MODEL_TAKEN,
	NPC_MODELS,
};

static inline int8_t *model_s8ptr(uint8_t *arg0)
{
	return (int8_t *)arg0;
}

void initializePosData(PositionData *posData)
{
	RotMatrix(&posData->rotation, &posData->posMatrix.coord);
	ScaleMatrix(&posData->posMatrix.coord, &posData->scale);
	TransMatrix(&posData->posMatrix.coord, &posData->location);
	posData->posMatrix.flg = 0;
}

void renderFlatDigimon(Entity *entity)
{
	int32_t half;
	int32_t i;
	VECTOR *loc;
	MATRIX m;
	SVECTOR in[4];
	SVECTOR out[4];
	ModelComponent *model;
	POLY_FT4 *prim;
	int16_t height;

	model = getEntityModelComponent(entity->type, getEntityType(entity));
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
	height = DIGIMON_DATA[entity->type].height;
	half = height / 2;
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
	addScreenPolyFT4(prim, &out[0], &out[1], &out[2], &out[3]);
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
	PositionData *pos;
	int16_t *anim;
	SkeletonBone *bone;
	ModelComponent *model;
	int32_t i;
	int32_t boneCount;
	int32_t entityType;

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
		if (bone->parentIndex != -1) {
			GsInitCoordinate2(&entity->posData[bone->parentIndex].posMatrix, &pos->posMatrix);
		} else {
			GsInitCoordinate2(NULL, &pos->posMatrix);
		}
		pos->obj.attribute = 0;
		pos->obj.coord2 = &pos->posMatrix;
	}
	anim = (int16_t *)((char *)entity->animPtr + *entity->animPtr);
	++anim;
	pos = entity->posData;
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
	PositionData *pos;
	MATRIX lw;
	MATRIX ls;
	int32_t boneCount;
	int32_t i;

	if (ENTITY_TABLE[instanceId]->isOnMap != 2) {
		if (ENTITY_TABLE[instanceId]->isOnMap == 0) {
			return;
		}
		if (ENTITY_TABLE[instanceId]->isOnScreen == 0) {
			return;
		}
	}
	pos = ENTITY_TABLE[instanceId]->posData;
	boneCount = DIGIMON_DATA[ENTITY_TABLE[instanceId]->type].boneCount;
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
	PositionData *posData;

	if ((entityId >= 0) && (entityId < ENTITY_MAX)) {
		posData = ENTITY_TABLE[entityId]->posData;
		RotMatrix(&posData->rotation, &posData->posMatrix.coord);
		ScaleMatrix(&posData->posMatrix.coord, &posData->scale);
		TransMatrix(&posData->posMatrix.coord, &posData->location);
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

#if defined(VERSION_JP)
void renderWireframed(GsDOBJ2 *obj, int32_t wireFrameShare)
{
	LINE_F4 *lf3;
	TMD_P_TG4 *quad;
	TMD_P_TG3 *tri;
	struct TMD_STRUCT *tmd;
	int32_t i;
	u_char *prim;
	int32_t primn;
	SVECTOR *vert;
	SVECTOR *normal;
	u_char *pk;
	CVECTOR col;
	long p;
	long flag;
	long otz;
	MATRIX m;
	GsCOORDINATE2 *coord;
	POLY_GT3 *gt3;
	POLY_GT4 *gt4;
	LINE_F2 *lf2;
	int8_t color;

	color = WIREFRAME_COLOR_MIN + rand() % (WIREFRAME_COLOR_MAX - WIREFRAME_COLOR_MIN);
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
			tri = (TMD_P_TG3 *)prim;
			if (WIREFRAME_RNG_TABLE[i & 0xF] < wireFrameShare) {
				gt3 = (POLY_GT3 *)pk;
				if (0 < RotNclip3(&vert[tri->v0], &vert[tri->v1],
				                  &vert[tri->v2], (long *)&gt3->x0, (long *)&gt3->x1,
				                  (long *)&gt3->x2, &p, &otz, &flag)) {
					NormalColorCol3(&normal[tri->n0],
					                &normal[tri->n1],
					                &normal[tri->n2], &col, (CVECTOR *)&gt3->r0,
					                (CVECTOR *)&gt3->r1, (CVECTOR *)&gt3->r2);
					setUV3(gt3, tri->tu0, tri->tv0, tri->tu1, tri->tv1, tri->tu2, tri->tv2);
					gt3->clut = tri->clut;
					gt3->tpage = tri->tpage;
					setPolyGT3(gt3);
					otz = otz >> 2;
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, gt3);
					pk = (u_char *)++gt3;
				}
			} else {
				lf3 = (LINE_F4 *)pk;
				if (0 < RotNclip3(&vert[tri->v0], &vert[tri->v1],
				                  &vert[tri->v2], (long *)&lf3->x0, (long *)&lf3->x1,
				                  (long *)&lf3->x2, &p, &otz, &flag)) {
					lf3->x3 = lf3->x0;
					lf3->y3 = lf3->y0;
					setlen(lf3, 6);
					setcode(lf3, 0x4C);
					lf3->pad = 0x55555555;
					lf3->r0 = lf3->g0 = lf3->b0 = color;
					otz = otz >> 2;
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, lf3);
					pk = (u_char *)(lf3 + 1);
				}
			}
			prim = (u_char *)(tri + 1);
		} else if ((prim[3] & 0xFC) == 0x3C) {
			quad = (TMD_P_TG4 *)prim;
			if (WIREFRAME_RNG_TABLE[i & 0xF] < wireFrameShare) {
				gt4 = (POLY_GT4 *)pk;
				if (0 < RotNclip4(&vert[quad->v0], &vert[quad->v1],
				                  &vert[quad->v2], &vert[quad->v3],
				                  (long *)&gt4->x0, (long *)&gt4->x1, (long *)&gt4->x2, (long *)&gt4->x3, &p,
				                  &otz, &flag)) {
					NormalColorCol3(&normal[quad->n0],
					                &normal[quad->n1],
					                &normal[quad->n2], &col, (CVECTOR *)&gt4->r0,
					                (CVECTOR *)&gt4->r1, (CVECTOR *)&gt4->r2);
					NormalColorCol(&normal[quad->n3], &col, (CVECTOR *)&gt4->r3);
					setUV4(gt4, quad->tu0, quad->tv0, quad->tu1, quad->tv1, quad->tu2, quad->tv2, quad->tu3, quad->tv3);
					gt4->clut = quad->clut;
					gt4->tpage = quad->tpage;
					setPolyGT4(gt4);
					otz = otz >> 2;
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, gt4);
					pk = (u_char *)++gt4;
				}
			} else {
				lf3 = (LINE_F4 *)pk;
				if (0 < RotNclip4(&vert[quad->v0], &vert[quad->v1],
				                  &vert[quad->v2], &vert[quad->v3],
				                  (long *)&lf3->x0, (long *)&lf3->x1, (long *)&lf3->x3, (long *)&lf3->x2, &p,
				                  &otz, &flag)) {
					setlen(lf3, 6);
					setcode(lf3, 0x4C);
					lf3->pad = 0x55555555;
					lf3->r0 = lf3->g0 = lf3->b0 = color;
					otz = otz >> 2;
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, lf3);
					lf2 = (LINE_F2 *)(lf3 + 1);
					setLineF2(lf2);
					lf2->r0 = lf2->g0 = lf2->b0 = color;
					setXY2(lf2, lf3->x3, lf3->y3, lf3->x0, lf3->y0);
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, lf2);
					pk = (u_char *)++lf2;
				}
			}
			prim = prim + 0x24;
		} else {
			break;
		}
	}
	GsSetWorkBase((PACKET *)pk);
}
#else
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
	TMD_P_TG3 *tri;
	TMD_P_TG4 *quad;
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
			tri = (TMD_P_TG3 *)prim;
			cur = (uint32_t)prim;
			if (WIREFRAME_RNG_TABLE[i & 0xF] < wireFrameShare) {
				gt3 = (POLY_GT3 *)pk;
				start = (int32_t)pk;
				if (0 < RotNclip3(&vert[tri->v0], &vert[tri->v1],
				                  &vert[tri->v2], (long *)&gt3->x0, (long *)&gt3->x1,
				                  (long *)&gt3->x2, &p, &otz, &flag)) {
					NormalColorCol3(&normal[tri->n0],
					                &normal[tri->n1],
					                &normal[tri->n2], &col, (CVECTOR *)&gt3->r0,
					                (CVECTOR *)&gt3->r1, (CVECTOR *)&gt3->r2);
					setUV3(gt3, tri->tu0, tri->tv0, tri->tu1, tri->tv1, tri->tu2, tri->tv2);
					gt3->clut = tri->clut;
					gt3->tpage = tri->tpage;
					setPolyGT3(gt3);
					otz = otz >> 2;
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, gt3);
					pk = (u_char *)(start + sizeof(POLY_GT3));
				}
			} else {
				lf3 = (LINE_F4 *)pk;
				start = (int32_t)pk;
				if (0 < RotNclip3(&vert[tri->v0], &vert[tri->v1],
				                  &vert[tri->v2], (long *)&lf3->x0, (long *)&lf3->x1,
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
			quad = (TMD_P_TG4 *)prim;
			if (WIREFRAME_RNG_TABLE[i & 0xF] < wireFrameShare) {
				gt4 = (POLY_GT4 *)pk;
				start = (int32_t)pk;
				if (0 < RotNclip4(&vert[quad->v0], &vert[quad->v1],
				                  &vert[quad->v2], &vert[quad->v3],
				                  (long *)&gt4->x0, (long *)&gt4->x1, (long *)&gt4->x2, (long *)&gt4->x3, &p,
				                  &otz, &flag)) {
					NormalColorCol3(&normal[quad->n0],
					                &normal[quad->n1],
					                &normal[quad->n2], &col, (CVECTOR *)&gt4->r0,
					                (CVECTOR *)&gt4->r1, (CVECTOR *)&gt4->r2);
					NormalColorCol(&normal[quad->n3], &col, (CVECTOR *)&gt4->r3);
					setUV4(gt4, quad->tu0, quad->tv0, quad->tu1, quad->tv1, quad->tu2, quad->tv2, quad->tu3, quad->tv3);
					gt4->clut = quad->clut;
					gt4->tpage = quad->tpage;
					setPolyGT4(gt4);
					otz = otz >> 2;
					AddPrim(ACTIVE_ORDERING_TABLE->org + otz, gt4);
					pk = (u_char *)(start + sizeof(POLY_GT4));
				}
			} else {
				lf3 = (LINE_F4 *)pk;
				if (0 < RotNclip4(&vert[quad->v0], &vert[quad->v1],
				                  &vert[quad->v2], &vert[quad->v3],
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
#endif

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
	char *p;
	char fileName[32];

	if (NULL == (p = strrchr(path, '\\'))) {
		p = path;
	} else {
		p++;
	}
	strcpy(fileName, MAIN_D_8011D190);

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
	t.pixelOffsetX = t.pixelOffsetY = 0;
	t.modelId = -1;
	t.digiType = -1;

	for (i = 0; i < 5; ++i) {
		NPC_MODELS[i] = t;
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
	char path[32];
	char *name;
	int32_t i;
	int32_t slot;
	int32_t k;

	m = NULL;
	if (modelType == 1) {
		return m;
	}
	if (digiType < 0 || digiType >= 0xB4) {
		return 0;
	}
	if (modelType == 0) {
		slot = -1;
		for (m = NPC_MODELS, i = 0; i < 5; m++, i++) {
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
			m = &NPC_MODELS[slot];
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
		concatStrings(path, MAIN_D_8011D484, name);
		concatStrings(path, path, MAIN_D_801340E4);
		path[9] = digiType / 30 + '0';
		m->mmdPtr = malloc3(((int32_t)lookupFileSize(path) + 0x7FF) & ~0x7FF);
		if (m->mmdPtr == NULL) {
			handleNullModel();
		}
		readFile(path, m->mmdPtr);
		m->modelPtr = (TMDModel *)((char *)m->mmdPtr + ((long *)m->mmdPtr)[0]);
		m->animTablePtr = (long *)((char *)m->mmdPtr + ((long *)m->mmdPtr)[1]);
		GsMapModelingData((u_long *)&m->modelPtr->flags);
		updateTMDTextureData((char *)m->modelPtr, m->pixelPage, m->pixelOffsetX, m->pixelOffsetY,
		                     m->clutPage - 0x7A00);
		return m;
	}
	if (modelType == 2) {
		m = &TAMER_MODEL;
		m->pixelPage = 0x15;
		m->clutPage = 0x7A00;
		m->pixelOffsetX = 0;
		m->pixelOffsetY = 0;
		m->modelId = 0;
		m->mmdPtr = TAMER_MODEL_BUFFER;
	} else if (modelType == 3) {
		m = &PARTNER_MODEL;
		m->pixelPage = 0x15;
		m->clutPage = 0x7A01;
		m->pixelOffsetX = 0;
		m->pixelOffsetY = 0x80;
		m->modelId = 0;
		m->mmdPtr = PARTNER_MODEL_BUFFER;
	} else {
		return 0;
	}
	name = PTR_DIGIMON_FILE_NAMES[digiType];
	loadDigimonTexture(digiType, name, m);
	concatStrings(path, MAIN_D_8011D484, name);
	concatStrings(path, path, MAIN_D_801340E4);
	path[9] = digiType / 30 + '0';
	readFile(path, m->mmdPtr);
	m->modelPtr = (TMDModel *)((char *)m->mmdPtr + ((long *)m->mmdPtr)[0]);
	m->animTablePtr = (long *)((char *)m->mmdPtr + ((long *)m->mmdPtr)[1]);
	GsMapModelingData((u_long *)&m->modelPtr->flags);
	updateTMDTextureData((char *)m->modelPtr, m->pixelPage, m->pixelOffsetX, m->pixelOffsetY,
	                     m->clutPage - 0x7A00);
	return m;
}

void unloadModel(int32_t digiType, int32_t modelType)
{
	int32_t i;
	int32_t j;
	ModelComponent *m;

	if (modelType == 0) {
		for (m = NPC_MODELS, i = 0; i < 5; ++m, ++i) {
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
		for (m = UNKNOWN_MODEL, j = 0; j < 16; ++m, ++j) {
			if (m->useCount == digiType) {
				break;
			}
		}
		if (j != 16) {
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

	if (type == 2) {
		p = &TAMER_MODEL;
	} else if (type == 3) {
		p = &PARTNER_MODEL;
	} else if (type == 0) {
		if ((instance < 0) || (instance >= 0xb4)) {
			return NULL;
		}
		for (p = NPC_MODELS, i = 0; i < 5; p++, i++) {
			if (p->digiType == instance) {
				break;
			}
		}
		if (i == 5) {
			return NULL;
		}
		if (p->useCount == 0) {
			return NULL;
		}
	} else if (type == 1) {
		if ((instance < 0) || (instance >= 0x96)) {
			return NULL;
		}
		p = &UNKNOWN_MODEL[instance];
		if (p->useCount == 0) {
			return NULL;
		}
	}

	return p;
}

int32_t getEntityType(Entity *entity)
{
	int32_t i;

	for (i = 0; i < 10; i++) {
		if (ENTITY_TABLE[i] == entity) {
			break;
		}
	}
	switch (i) {
	case 0:
		return 2;
	case 1:
		return 3;
	case 10:
		return -1;
	default:
		return 0;
	}
}

void uploadModelTexture(void *textureData, ModelComponent *component)
{
	GsIMAGE img;
	RECT rect;

	GsGetTimInfo((unsigned long *)textureData + 1, &img);

	img.px = ((component->pixelPage % 16) << 6) + component->pixelOffsetX;
	img.py = ((component->pixelPage / 16) << 8) + component->pixelOffsetY;
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

int32_t loadMMDAsync(int32_t digimonType, int32_t entityType, int32_t buffer,
                     EvoModelData *modelData, uint8_t *readComplete)
{
	ModelComponent *m;
	char path[32];
	char *name;
	int32_t i;
	int32_t slot;
	char tim[32];

	if (digimonType < 0 || digimonType >= 0xB4) {
		return 0;
	}
	if (entityType == 0) {
		slot = -1;
		for (m = NPC_MODELS, i = 0; i < 5; m++, i++) {
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
			m = &NPC_MODELS[slot];
			m->digiType = digimonType;
		}
		++m->useCount;
		if (m->useCount != 1) {
			return 0;
		}
	} else if (entityType == 2) {
		m = &TAMER_MODEL;
	} else if (entityType == 3) {
		m = &PARTNER_MODEL;
	} else {
		return 0;
	}
	name = PTR_DIGIMON_FILE_NAMES[digimonType];
	strcpy(tim, MAIN_D_8011D190);
	if ((buffer & 3L) != 0) {
		buffer += 4 - (buffer & 3);
	}
	readFileSectors(tim, (void *)buffer, digimonType * 9, 9);
	modelData->imagePtr = (uint8_t *)buffer;
	modelData->imageSize = 0x4800;
	buffer += modelData->imageSize;
	concatStrings(path, MAIN_D_8011D484, name);
	concatStrings(path, path, MAIN_D_801340E4);
	path[9] = digimonType / 30 + '0';
	if ((buffer & 3L) != 0) {
		buffer += 4 - (buffer & 3);
	}
	addFileReadRequestPath(path, (uint8_t *)buffer, readComplete, 0, 0);
	modelData->modelPtr = (uint8_t *)buffer;
	modelData->modelSize = lookupFileSize(path);
	buffer += modelData->modelSize;
	return buffer;
}

ModelComponent *applyMMD(int32_t digimonType, int32_t entityType,
                         EvoModelData *modelData)
{
	ModelComponent *m;
	char path[32];
	char *name;
	int32_t i;
	GsIMAGE img;
	RECT rect;
	int32_t k;

	if (digimonType < 0 || digimonType >= 0xB4) {
		return 0;
	}
	if (entityType == 0) {
		for (m = NPC_MODELS, i = 0; i < 5; m++, i++) {
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
		concatStrings(path, MAIN_D_8011D46C, name);
		concatStrings(path, path, MAIN_D_801340EC);
		path[9] = digimonType / 30 + '0';
		m->modelPtr = malloc3(((int32_t)lookupFileSize(path) + 0x7FF) & ~0x7FF);
		readFile(path, m->modelPtr);
		GsMapModelingData((u_long *)&m->modelPtr->flags);
		updateTMDTextureData((char *)m->modelPtr, m->pixelPage, m->pixelOffsetX, m->pixelOffsetY,
		                     m->clutPage - 0x7A00);
		concatStrings(path, MAIN_D_8011D478, name);
		concatStrings(path, path, MAIN_D_801340F4);
		path[9] = digimonType / 30 + '0';
		m->animTablePtr = malloc3(((int32_t)lookupFileSize(path) + 0x7FF) & ~0x7FF);
		readFile(path, m->animTablePtr);
		return m;
	}
	if (entityType == 2) {
	} else if (entityType == 3) {
		m = &PARTNER_MODEL;
		m->pixelPage = 0x15;
		m->clutPage = 0x7A01;
		m->pixelOffsetX = 0;
		m->pixelOffsetY = 0x80;
		m->modelId = 0;
		m->mmdPtr = PARTNER_MODEL_BUFFER;
		m->useCount = 0;
	} else {
		return 0;
	}
	name = PTR_DIGIMON_FILE_NAMES[digimonType];
	++m->useCount;
	if (m->useCount != 1) {
		return m;
	}
	GsGetTimInfo((unsigned long *)modelData->imagePtr + 1, &img);
	img.px = ((m->pixelPage % 16) << 6) + m->pixelOffsetX;
	img.py = ((m->pixelPage / 16) << 8) + m->pixelOffsetY;
	img.cx = (m->clutPage & 0x3f) << 4;
	img.cy = m->clutPage >> 6;
	setRECT(&rect, img.px, img.py, img.pw, img.ph);
	LoadImage(&rect, img.pixel);
	if ((img.pmode >> 3) & 1) {
		setRECT(&rect, img.cx, img.cy, img.cw, img.ch);
		LoadImage(&rect, img.clut);
	}
	memcpy(m->mmdPtr, modelData->modelPtr, modelData->modelSize);
	m->modelPtr = (TMDModel *)((char *)m->mmdPtr + ((long *)m->mmdPtr)[0]);
	m->animTablePtr = (long *)((char *)m->mmdPtr + ((long *)m->mmdPtr)[1]);
	GsMapModelingData((u_long *)&m->modelPtr->flags);
	updateTMDTextureData((char *)m->modelPtr, m->pixelPage, m->pixelOffsetX, m->pixelOffsetY,
	                     m->clutPage - 0x7A00);
	return m;
}
