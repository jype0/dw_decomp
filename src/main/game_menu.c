#include <stdio.h>
#include <string.h>

#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/clock.h>
#include <dw/entity.h>
#include <dw/evl.h>
#include <dw/fish.h>
#include <dw/font.h>
#include <dw/garbage.h>
#include <dw/graphics.h>
#include <dw/input.h>
#include <dw/item.h>
#include <dw/main.h>
#include <dw/model.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/partner.h>
#include <dw/sound.h>
#include <dw/tamer.h>
#include <dw/ui.h>

/* Colors of the two button glyphs in the medal and card view footers */
#if defined(VERSION_JP)
#define FOOTER_GLYPH1_COLOR 0xe
#define FOOTER_GLYPH2_COLOR 0xf
#else
#define FOOTER_GLYPH1_COLOR 0xf
#define FOOTER_GLYPH2_COLOR 7
#endif

typedef struct {
	int8_t tab[2];
} DigimonTabs;

typedef struct {
	int8_t tab[4];
} PlayerTabs;

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

typedef struct {
	int16_t posX;
	int16_t posY;
	uint8_t width;
	uint8_t height;
	uint8_t texX;
	uint8_t texY;
} IconRect;

typedef struct {
	int16_t posX;
	int16_t posY;
	int16_t uvWidth;
	uint8_t uvX;
	uint8_t uvY;
} StringRect;

typedef struct {
	uint8_t v[21];
} StatsIconClutTable;

typedef struct {
	int16_t m[5];
} EvoClutTable;

typedef struct {
	int16_t x1;
	int16_t x2;
	int16_t x3;
	int16_t x4;
	int16_t y1;
	int16_t y2;
	int16_t y3;
	int16_t y4;
} Line4Points;

typedef struct {
	int16_t posX;
	int16_t posY;
	int16_t unk4;
	int16_t unk6;
} ChartSprite;

typedef struct {
	int16_t posX;
	int16_t posY;
	int16_t width;
	int16_t height;
} Inset;

typedef struct {
	int8_t v[66];
} CardSprites;

typedef struct {
	int32_t v[6];
} ConditionMaskTable;

extern GsDOBJ2 MEDAL_OBJECT;
extern GsCOORDINATE2 MEDAL_COORDINATES;
extern int32_t IS_IN_MENU;
extern GsOT *ACTIVE_ORDERING_TABLE;
extern int8_t MENU_SUB_STATE;
extern int32_t TRIANGLE_MENU_STATE;
extern int32_t MAIN_D_80134D2C;
extern int8_t MENU_STATE;
extern int8_t MAIN_D_80134D36;
extern int8_t MAIN_D_80134D37;
extern int16_t MAIN_D_80134D38;
extern int16_t MAIN_D_80134D3A;
extern int32_t CHANGED_INPUT;
extern uint8_t INVENTORY_POINTER;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern int32_t MAIN_D_80134D28;
extern int8_t SELECTED_MEDAL;
extern int8_t MEDAL_SELECTOR_INDEX;
extern int8_t SELECTED_CARD;
extern int16_t MAIN_D_80134D40;
extern int16_t MAIN_D_80134D42;
extern int16_t MAIN_D_80134D44;
extern int8_t MAIN_D_80134D46;
extern int16_t MAIN_D_80134D48;
extern char STR_MOVE_NAME_BUG[];
extern char STR_MOVE_NAME_BUBBLE[];
extern int32_t MONEY;
extern uint16_t PLAYTIME_FRAMES;
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT_TAG *FRAMEBUFFER0_ORIGIN;
extern GsOT_TAG *FRAMEBUFFER1_ORIGIN;
extern GsOT *FRAMEBUFFER_OT[2];
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t VIEWPORT_DISTANCE;
extern GsF_LIGHT LIGHT_DATA[3];

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
void renderMenuTab(int16_t x, int8_t w, int8_t layer);
void setCameraFollowPlayer(void);
void handleGameMenuSelection(int32_t selection);
int32_t createMenuBox(int16_t id, int16_t x, int16_t y, int16_t width,
                      int16_t height, int8_t features, void (*tick)(void),
                      void (*render)(void));
void closeUIBoxIfOpen(int32_t arg);
void getEntityScreenPos(Entity *entity, int32_t flag, int16_t *outPos);
void addInventoryUI(void);
void tickGameMenu(void);
void renderGameMenu(void);
void tickDigimonMenu(void);
void renderDigimonMenu(void);
void tickPlayerMenu(void);
void renderPlayerMenu(void);
void tickTriangleMenu(void);
void renderRectPolyFT4(int16_t posX, int16_t posY, uint8_t width,
                       uint8_t height, uint8_t texX, uint8_t texY,
                       int16_t texturePage, int16_t clut, int32_t zIndex,
                       int8_t flag);
void renderSeparatorLines(int16_t *lines, int8_t count, int32_t zIndex);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY,
                       int32_t width, int32_t height);
void setUVDataPolyFT4(POLY_FT4 *prim, int32_t uPos, int32_t vPos,
                      int32_t width, int32_t height);
void removeTriangleMenu(void);
void removeUIBox1(void);
void addGameMenu(void);
void renderDateDigits(void);
void renderTriangleCursor(int8_t selection, int16_t yOffset);
int32_t isUIBoxAvailable(int32_t id);
void setSleepDisabled(int16_t arg);
void startFeedingItem(uint8_t arg);
void removeOneSelectedItem(void);
void renderFeedingItem(int16_t arg);
int8_t getEquippedSlot(void);
void equipMove(void);
int32_t isKeyDown(int32_t mask);
void convertValueToDigits(int32_t digits, int32_t value, int32_t *outCount,
                          int32_t *outDigits);
void drawLine2P(int32_t color, int32_t x0, int32_t y0, int32_t x1,
                int32_t y1, int32_t zIndex, int32_t flag);
void drawLine3P(int32_t color, int32_t x0, int32_t y0,
                int32_t x1, int32_t y1, int32_t x2,
                int32_t y2, int32_t zIndex, int32_t flag);
int32_t hasDigimonRaised(int32_t digimonId);
int32_t hasMedal(uint16_t medal);
void activateMedalTexture(int32_t medalId, int32_t previousMedalId);
int32_t loadCardImage(int32_t id);
void renderEvoChartDetail(void);
void renderCardImage(void);
void renderCardCount(void);
int32_t hasMove(int32_t moveId);
int32_t drawPlayerInfoStrings(void);
int32_t isTriggerSet(uint16_t trigger);
void renderInsetBox(int16_t a, int16_t b, int16_t c, int16_t d, int32_t otz);
void renderDigiviceEntity(Entity *entity, int32_t entityId);
int32_t drawDigimonMovesText(void);
u_short GetTPage(int32_t tp, int32_t abr, int32_t x, int32_t y);
u_short GetClut(int32_t x, int32_t y);
void renderBox(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t r,
               uint8_t g, uint8_t b, uint8_t flags, int32_t otz);
void renderDigimonMoveBox(void);
int32_t getCardAmount(uint8_t card);
int32_t loadStackedTIMEntry(char *path, u_long buffer, int32_t offset,
                            int32_t sectors);
void renderNumber(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e,
                  int32_t f);
void renderDigimonMovesSelected(int16_t panel);
int32_t drawMoveViewHelpStrings(void);
int32_t drawDigimonStatsStrings(void);
void renderDigimonStatusConditions(int32_t condition);
void clearTextSubArea(RECT *rect);
void sortArray(int16_t *arr, int8_t count);
void removeStaticUIBox(int16_t id);
void drawObject(GsDOBJ2 *obj, GsOT *ot, int32_t flag);
void renderDigiviceMedals(void);
void drawString(char *str, int32_t x, int32_t y);
void renderBorderBox(int16_t x, int16_t y, int16_t w, int16_t h,
                     int32_t c1, int32_t c2, uint8_t r, uint8_t g, uint8_t b,
                     int32_t a10);
int32_t strlen(char *s);
void startAnimation(Entity *entity, uint8_t animId);
void tickAnimation(Entity *entity);
int32_t drawCardViewStrings(void);
int32_t drawMedalViewStrings(void);
void renderDigimonStatsBar(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e);
int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
int32_t drawEvoChartStrings(int8_t arg);

static void *game_menu_functions[] = {
	drawCardViewStrings,
	drawMedalViewStrings,
	drawEvoChartStrings,
	drawPlayerInfoStrings,
	removeUIBox1,
	removeTriangleMenu,
	sortArray,
	renderDigiviceMedals,
	drawMoveViewHelpStrings,
	renderDigimonMoveBox,
	renderDigimonMovesSelected,
	drawDigimonMovesText,
	renderDigiviceEntity,
	renderDigimonStatusConditions,
	renderDigimonStatsBar,
	drawDigimonStatsStrings,
	renderBorderBox,
	renderBox,
	renderInsetBox,
	renderCardsView,
	renderMedalView,
	renderEvoChartView,
	renderPlayerInfoView,
	renderMenuTab,
	renderDigimonMovesView,
	renderDigimonStatsView,
	renderCardCount,
	renderCardImage,
	loadCardImage,
	renderEvoChartDetail,
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
	renderSeparatorLines,
	renderGameMenu,
	closeUIBoxIfOpen,
	closeTriangleMenu,
	tickTriangleMenu,
	addGameMenu,
};

// clang-format off
uint8_t EQUIPPED_MOVES[3] = {
	0xff, 0xff, 0xff,
};

uint8_t MAIN_D_80134237 = 0xff;

SVECTOR MAIN_D_80134238 = { 0x0000, 0x0000, 0x0000, 0x0000 };

#if !defined(VERSION_JP)
char MAIN_D_80134240[] = "Current";

char MAIN_D_80134248[] = "Ending";
#endif

char STR_OVERWORLD_EMPTY[] = "";

TriangleCursorUVData MAIN_D_80134250 = { { 0x00, 0x04, 0x00, 0x04, 0x04, 0x04, 0x08, 0x08 } };

TriangleCursorUVData MAIN_D_80134258 = { { 0x04, 0x00, 0x04, 0x00, 0x08, 0x08, 0x0c, 0x0c } };

TriangleCursorUVData MAIN_D_80134260 = { { 0xfb, 0xfb, 0xff, 0xff, 0xfb, 0xfb, 0xfb, 0xfb } };

TriangleCursorUVData MAIN_D_80134268 = { { 0xff, 0xff, 0xfb, 0xfb, 0xff, 0xff, 0xff, 0xff } };

TriangleCursorOffsetData MAIN_D_80134270 = { { 0x00, 0x18, 0x00, 0x18, 0x04, 0x04, 0x00, 0x19 } };

TriangleCursorOffsetData MAIN_D_80134278 = { { 0x00, 0x00, 0x16, 0x16, 0x00, 0x16, 0x00, 0x00 } };

TriangleCursorOffsetData MAIN_D_80134280 = { { 0x04, 0x04, 0x04, 0x04, 0x18, 0x18, 0x04, 0x04 } };

TriangleCursorOffsetData MAIN_D_80134288 = { { 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x16, 0x16 } };

RECT MAIN_D_80134290 = { 0, 232, 24, 12 };

#if defined(VERSION_JP)
char STR_YEAR_DAY[] = "年日";
#else
char STR_YEAR_DAY[8] = "YearDay";
#endif

DigimonTabs MAIN_D_801342A0 = { { 0x01, 0x01 } };

PlayerTabs MAIN_D_801342A4 = { { 0x01, 0x01, 0x01, 0x01 } };

#if defined(VERSION_JP)
char MAIN_D_80123DE8[] = "半人前";

char MAIN_D_80123DF4[] = "初級";

char MAIN_D_80123E04[] = "中級";

char MAIN_D_80123E10[] = "一流";

char MAIN_D_801342D0[] = "天才";

char MAIN_D_801342D8[] = "伝説";
#else
char MAIN_D_801342A8[8] = "Amateur";

char MAIN_D_801342B0[] = "Novice";

char MAIN_D_801342B8[8] = "Veteran";

char MAIN_D_801342C0[] = "Super";

char MAIN_D_801342C8[] = "Master";

char MAIN_D_801342D0[] = "Genius";

char MAIN_D_801342D8[] = "Legend";
#endif

RECT MAIN_D_801342E0 = { 0, 24, 256, 200 };

RECT MOVES_VIEW_TEXT_AREA = { 0, 24, 256, 200 };

RECT MAIN_D_801342F0 = { 0, 24, 256, 200 };

RECT PLAYER_INFO_TEXT_AREA = { 0, 12, 256, 200 };

#if !defined(VERSION_JP)
char FMT_PLAYTIME[8] = "%d : %d";
#endif

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

#if defined(VERSION_JP)
char MAIN_D_801238A0[] = "ステータス技セット戦績";

char MAIN_D_801238B0[] = "才ｇしつけ／ＬＩＦＥのろいＨＰＭＰゆうよわ";

char MAIN_D_801238D8[] = "あさがたひるがたあさよわゆうがたよるがた";

char MAIN_D_80123900[] = "つけかえ技必殺技おぼえた技";

char MAIN_D_80123914[] = "へルプｃｈｉ近遠全自";

char MAIN_D_80123920[] = "ａｈｉを押すと、技セットモード";

char MAIN_D_80123938[] = "毒混乱マヒ液晶化";

char MAIN_D_8012394C[] = "技セットモードの解除はｂｈｉ";

char MAIN_D_80123958[] = "近距離遠距離全体技補助技";

char MAIN_D_80123970[] = "まだ３つの技をつけてない時はを選んで";

char MAIN_D_80123990[] = "つけかえの時はを　ｈｉでキャンセルして";

char MAIN_D_801239AC[] = "技セットヘルプを選んで　ｈｉで決定ａ";

char MAIN_D_80134240[] = "は、つけかえ技として選ばれている技";

char MAIN_D_801239C4[] = "は、つけかえられる技（技あり）";

char MAIN_D_801239D0[] = "は、まだ覚えていない技（技なし）";

char MAIN_D_801239DC[] = "は、性質でつけかえできない技";

char MAIN_D_801239E8[] = "は、つけかえようとして今選んでいる技";

char MAIN_D_801239F4[] = "合計戦績勝った数負けた数勝率％";

char MAIN_D_80123A18[] = "ステータスデジモン表メダルカードつり";

char MAIN_D_80123A3C[] = "メダルコレクションａｂｈｉｊｋｌｍｎｏ？";

char MAIN_D_80123A60[] = "名前才テイマーレベル育てたデジモンひき";

char MAIN_D_80123A7C[] = "持ってるお金｛｝かかった時間：テイマー";

char MAIN_D_80123A98[] = "特別なアイテム集めたメダル枚";

char MAIN_D_80123AB0[] = "カードリストａｈｉｊｋｌｂｈｉｍｎｏ枚";

char MAIN_D_80123AD0[] = "ａｈｉｊｋｌｂｈｉｍｎｏくわしいデータ";

char MAIN_D_80123AE4[] = "進化系図幼年期幼年期成長期成熟期完全体";

char MAIN_D_80123B10[] = "基本体重ｇ属性活動時間必殺技";

char MAIN_D_80123B20[] = "つりの記録全長";

char STR_MEDAL_NAME_GRADE_CUP[] = "グレード戦制覇";

char STR_MEDAL_NAME_VERSION_CUP[] = "バージョン杯制覇";

char STR_MEDAL_NAME_TYPE_CUP[] = "性質杯制覇";

char STR_MEDAL_NAME_SPECIAL_CUP[] = "特別杯制覇";

char STR_MEDAL_NAME_100_TIMES[] = "合計１００勝";

char STR_MEDAL_NAME_TECHNIQUE_MASTER[] = "全技習得";

char STR_MEDAL_NAME_DIGIMON_MASTER[] = "全デジモン育成";

char STR_MEDAL_NAME_MAX_ABILITIES[] = "全能力ＭＡＸ値";

char STR_MEDAL_NAME_PERFECT_CURLING[] = "カーリング満点";

char STR_MEDAL_NAME_100_FISH[] = "釣り１００ぴき";

char MAIN_D_80134248[] = "エンディング";

char STR_MEDAL_NAME_TOWN_FLOURISHING[] = "街が最大発展";

char STR_MEDAL_NAME_CARD_COMPLETE[] = "カードコンプリート";

char STR_MEDAL_NAME_BITS_MAXED[] = "｛｝最大";

char STR_MEDAL_NAME_10_YEARS[] = "開始１０周年";

char STR_MEDAL_DESCRIPTION_CUP_D_C_B_A_S[] = "グレード戦Ｄ、Ｃ、Ｂ、Ａ、Ｓ、";

char STR_MEDAL_DESCRIPTION_WIN_IN_ALL[] = "全てで優勝";

char STR_MEDAL_DESCRIPTION_WIN_IN_ALL_VER_1_2_3_4_0[] = "ＶＥＲ　１、２、３、４、０、全てで優勝";

char STR_MEDAL_DESCRIPTION_FIRE_GRAPPLE_THUNDER_WIND[] = "ファイアー、グラップル、サンダーウインド、";

char STR_MEDAL_DESCRIPTION_NATURE_COOL_METALLIC_FILTH_CUP[] = "ネイチャー、クール、メタリック、ダーティ杯";

char STR_MEDAL_DESCRIPTION_WIN_IN_ALL_2[] = "の全てで優勝";

char STR_MEDAL_DESCRIPTION_DINO_WING_ANIMAL_HUMAN_CUP[] = "ダイノ、ウイング、アニマル、ヒューマン杯";

char STR_MEDAL_DESCRIPTION_WON_CHAMPIONSHIP_100_TIMES[] = "大会（何でもよい）で１００回優勝する";

char STR_MEDAL_DESCRIPTION_MASTERED_56_SWITCH_TECHNIQUES[] = "全てのつけかえ技５６種を技リストに習得";

char STR_MEDAL_DESCRIPTION_RAISED_ALL_61_DIGIMON[] = "全デジモン６１種を育てる";

char STR_MEDAL_DESCRIPTION_MAXED_ALL_OF_THE_DIGIMONS[] = "デジモン１ぴきのパラメータ６種を";

char STR_MEDAL_DESCRIPTION_PARAMETERS[] = "ＭＡＸにする";

char STR_MEDAL_DESCRIPTION_GOT_A_PERFECT_SCORE_IN_CURLING[] = "カーリングで満点を取る";

char STR_MEDAL_DESCRIPTION_100_FISH_CAUGHT[] = "釣った魚の合計１００ぴき";

char STR_MEDAL_DESCRIPTION_FINISHED_THE_GAME[] = "ラスボスを倒し一度エンディングを見る";

char STR_MEDAL_DESCRIPTION_JIJIMON_SAID_THE_TOWN[] = "ジジモンに最大発展完了のコメントをもらう";

char STR_MEDAL_DESCRIPTION_COLLECTED_ALL_DIGIMON_CARDS[] = "デジモンカードを全種集める";

char STR_MEDAL_DESCRIPTION_COLLECTED_999999_BITS[] = "｛｝を９９９９９９にする";

char STR_MEDAL_DESCRIPTION_SURVIVED_FOR_300_DAYS[] = "ゲームを始めて３００日たった";
#else
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

char STR_MEDAL_NAME_GRADE_CUP[] = "Grade Cup";

char STR_MEDAL_NAME_VERSION_CUP[12] = "Version Cup";

char STR_MEDAL_NAME_TYPE_CUP[] = "Type Cup";

char STR_MEDAL_NAME_SPECIAL_CUP[12] = "Special Cup";

char STR_MEDAL_NAME_100_TIMES[] = "100 Times";

char STR_MEDAL_NAME_TECHNIQUE_MASTER[] = "Technique Master";

char STR_MEDAL_NAME_DIGIMON_MASTER[] = "Digimon Master";

char STR_MEDAL_NAME_MAX_ABILITIES[] = "Max Abilities";

char STR_MEDAL_NAME_PERFECT_CURLING[16] = "Perfect Curling";

char STR_MEDAL_NAME_100_FISH[] = "100 Fish";

char STR_MEDAL_NAME_TOWN_FLOURISHING[] = "Town Flourishing";

char STR_MEDAL_NAME_CARD_COMPLETE[] = "Card Complete";

char STR_MEDAL_NAME_BITS_MAXED[] = "Bits Maxed";

char STR_MEDAL_NAME_10_YEARS[] = "10 Years";

char STR_MEDAL_DESCRIPTION_CUP_D_C_B_A_S[] = "Cup D C B A S";

char STR_MEDAL_DESCRIPTION_WIN_IN_ALL[] = "Win in all";

char STR_MEDAL_DESCRIPTION_WIN_IN_ALL_VER_1_2_3_4_0[] = "Win in all VER 1 2 3 4 0";

char STR_MEDAL_DESCRIPTION_FIRE_GRAPPLE_THUNDER_WIND[] = "Fire Grapple Thunder Wind";

char STR_MEDAL_DESCRIPTION_NATURE_COOL_METALLIC_FILTH_CUP[] = "Nature Cool Metallic Filth Cup";

char STR_MEDAL_DESCRIPTION_DINO_WING_ANIMAL_HUMAN_CUP[] = "Dino Wing Animal Human Cup";

char STR_MEDAL_DESCRIPTION_WON_CHAMPIONSHIP_100_TIMES[] = "Won Championship 100 times";

char STR_MEDAL_DESCRIPTION_MASTERED_56_SWITCH_TECHNIQUES[] = "Mastered 56 switch techniques";

char STR_MEDAL_DESCRIPTION_RAISED_ALL_61_DIGIMON[] = "Raised all 61 Digimon";

char STR_MEDAL_DESCRIPTION_MAXED_ALL_OF_THE_DIGIMONS[] = "Maxed all of the Digimons";

char STR_MEDAL_DESCRIPTION_PARAMETERS[] = "parameters";

char STR_MEDAL_DESCRIPTION_GOT_A_PERFECT_SCORE_IN_CURLING[] = "Got a perfect score in curling";

char STR_MEDAL_DESCRIPTION_100_FISH_CAUGHT[16] = "100 fish caught";

char STR_MEDAL_DESCRIPTION_FINISHED_THE_GAME[] = "Finished the game";

char STR_MEDAL_DESCRIPTION_JIJIMON_SAID_THE_TOWN[] = "Jijimon said the town";

char STR_MEDAL_DESCRIPTION_IS_FLOURISHING[] = "is flourishing";

char STR_MEDAL_DESCRIPTION_COLLECTED_ALL_DIGIMON_CARDS[28] = "Collected all Digimon Cards";

char STR_MEDAL_DESCRIPTION_COLLECTED_999999_BITS[] = "Collected 999999 bits";

char STR_MEDAL_DESCRIPTION_SURVIVED_FOR_300_DAYS[] = "Survived for 300 days!";
#endif

StatsIconClutTable MAIN_D_80123DB8 = { {
	0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x09, 0x09,
	0x09, 0x09, 0x0a, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
	0x0b, 0x04, 0x04, 0x04, 0x0a,
} };

ConditionMaskTable MAIN_D_80123DD0 = { {
	0x00000040, 0x00000020, 0x00000004, 0x00000001,
	0x00000010, 0x00000002,
} };

#if defined(VERSION_JP)
char MAIN_D_801342A8[] = "しろうと";

char MAIN_D_801342B0[] = "かけだし";

char MAIN_D_801342B8[] = "ベテラン";

char MAIN_D_801342C0[] = "スーパー";

char MAIN_D_801342C8[] = "マスター";
#else
char MAIN_D_80123DE8[] = "Beginner";

char MAIN_D_80123DF4[] = "Intermediate";

char MAIN_D_80123E04[] = "Advanced";

char MAIN_D_80123E10[] = "Top rate";
#endif

EvoClutTable MAIN_D_80123E1C = { {
	0x7a07, 0x7a47, 0x7a87, 0x7ac7, 0x7b07,
} };

CardSprites MAIN_D_80123E28 = { {
	0x00, 0x00, 0x04, 0x01, 0x00, 0x03, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x01, 0x04, 0x04, 0x03, 0x03, 0x04,
	0x03, 0x06, 0x01, 0x03, 0x01, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01,
	0x02, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
	0x00, 0x00, 0x00, 0x03, 0x01, 0x01, 0x01, 0x00,
	0x00, 0x00,
} };

EvoClutTable MAIN_D_80123E6C = { {
	0x7a07, 0x7a47, 0x7a87, 0x7ac7, 0x7b07,
} };

char MAIN_D_80123E78[] = "\\CARD\\CARD.ALL";

GameMenuSprite GAME_MENU_SPRITES[8] = {
	{ 0x10, 0x1c, 1, 1, 0xc, 0x18, 0x18, 0x0, 0x30 },
	{ 0x14, 0x47, 1, 0, 0xc, 0x14, 0x14, 0x0, 0x0 },
	{ 0x38, 0x47, 2, 0, 0xd, 0x14, 0x14, 0x28, 0x0 },
	{ 0x5b, 0x47, 3, 0, 0xc, 0x14, 0x14, 0x0, 0x14 },
	{ 0x14, 0x6f, 4, 0, 0xd, 0x14, 0x14, 0x28, 0x14 },
	{ 0x38, 0x6f, 5, 0, 0xd, 0x14, 0x14, 0x0, 0x28 },
	{ 0x5b, 0x6f, 6, 0, 0xd, 0x14, 0x14, 0x28, 0x28 },
	{ 0x14, 0x1f, 7, 0, 0xd, 0x14, 0x13, 0x50, 0x0 },
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

IconRect MAIN_D_80124044[4] = {
	{ 0xff72, 0x001a, 0x04, 0x04, 0x78, 0x10 },
	{ 0xffad, 0x001a, 0x04, 0x04, 0x7c, 0x10 },
	{ 0xff72, 0x0059, 0x04, 0x04, 0x78, 0x14 },
	{ 0xffad, 0x0059, 0x04, 0x04, 0x7c, 0x14 },
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

IconRect MAIN_D_801241CC[13] = {
	{ 0x0016, 0xffb4, 0x17, 0x07, 0x5c, 0x22 },
	{ 0x0062, 0xffb4, 0x13, 0x07, 0x4c, 0x29 },
	{ 0x007a, 0xffb4, 0x10, 0x07, 0x00, 0x30 },
	{ 0x0044, 0xffb4, 0x0b, 0x07, 0x74, 0x22 },
	{ 0x0035, 0xffc1, 0x04, 0x04, 0x78, 0x0c },
	{ 0x0035, 0xffd0, 0x04, 0x04, 0x78, 0x0c },
	{ 0x0035, 0xffe0, 0x04, 0x04, 0x78, 0x0c },
	{ 0x005e, 0xffc1, 0x04, 0x04, 0x78, 0x0c },
	{ 0x005e, 0xffd0, 0x04, 0x04, 0x78, 0x0c },
	{ 0x005e, 0xffe0, 0x04, 0x04, 0x78, 0x0c },
	{ 0x0075, 0xffc1, 0x04, 0x04, 0x78, 0x0c },
	{ 0x0075, 0xffd0, 0x04, 0x04, 0x78, 0x0c },
	{ 0x0075, 0xffe0, 0x04, 0x04, 0x78, 0x0c },
};

#if defined(VERSION_JP)
StringRect MAIN_D_80124234[12] = {
	{ 0xff8e, 0xffbf, 0x009c, 0x00, 0x18 },
	{ 0x0042, 0xffbf, 0x0030, 0xa8, 0x18 },
	{ 0xff8e, 0xffcd, 0x0048, 0x84, 0x30 },
	{ 0xff8e, 0xffde, 0x0054, 0x00, 0x24 },
	{ 0xfffa, 0xffde, 0x0084, 0x54, 0x24 },
	{ 0xffa6, 0xffec, 0x0078, 0x54, 0x30 },
	{ 0xff9a, 0x0002, 0x00cc, 0x00, 0x3c },
	{ 0xff9a, 0x0012, 0x0078, 0x00, 0x48 },
	{ 0xff9a, 0x0022, 0x0084, 0x00, 0x54 },
	{ 0xff9a, 0x0032, 0x00a8, 0x00, 0x60 },
	{ 0xff9a, 0x0042, 0x00a8, 0x00, 0x60 },
	{ 0xff9a, 0x0052, 0x00d8, 0x00, 0x6c },
};

IconRect MAIN_D_80124294[7] = {
	{ 0x0030, 0xffbf, 0x0c, 0x0c, 0x24, 0x00 },
	{ 0xffe8, 0xffde, 0x0c, 0x0c, 0x54, 0x00 },
	{ 0xff94, 0xffec, 0x0c, 0x0c, 0x24, 0x00 },
	{ 0xff88, 0x0002, 0x0c, 0x0c, 0x54, 0x00 },
	{ 0xff88, 0x0012, 0x0c, 0x0c, 0x24, 0x00 },
	{ 0xff88, 0x0032, 0x0c, 0x0c, 0x6c, 0x00 },
	{ 0xff88, 0x0052, 0x0c, 0x0c, 0xb4, 0x00 },
};
#else
StringRect MAIN_D_80124234[12] = {
	{ 0xff8e, 0xffbf, 0x0073, 0x00, 0x18 },
	{ 0x000f, 0xffbf, 0x006e, 0x71, 0x18 },
	{ 0xff8e, 0xffcd, 0x0000, 0xdf, 0x18 },
	{ 0xff8e, 0xffde, 0x000c, 0x00, 0x24 },
	{ 0xff9a, 0xffde, 0x00be, 0x0c, 0x24 },
	{ 0xffa6, 0xffec, 0x0000, 0x54, 0x30 },
	{ 0xff9a, 0x0002, 0x0064, 0x00, 0x3c },
	{ 0xff9a, 0x0012, 0x0064, 0x00, 0x48 },
	{ 0xff9a, 0x0022, 0x0064, 0x00, 0x54 },
	{ 0xff9a, 0x0032, 0x0064, 0x00, 0x60 },
	{ 0xff9a, 0x0042, 0x0064, 0x00, 0x60 },
	{ 0xff9a, 0x0052, 0x0064, 0x00, 0x6c },
};

IconRect MAIN_D_80124294[7] = {
	{ 0x0005, 0xffbf, 0x0c, 0x0c, 0x24, 0x00 },
	{ 0x0052, 0xffde, 0x0c, 0x0c, 0x54, 0x00 },
	{ 0xffbe, 0xffec, 0x0c, 0x0c, 0x24, 0x00 },
	{ 0xff88, 0x0002, 0x0c, 0x0c, 0x54, 0x00 },
	{ 0xff88, 0x0012, 0x0c, 0x0c, 0x24, 0x00 },
	{ 0xff88, 0x0032, 0x0c, 0x0c, 0x6c, 0x00 },
	{ 0xff88, 0x0052, 0x0c, 0x0c, 0xb4, 0x00 },
};
#endif

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

#if defined(VERSION_JP)
StringRect MAIN_D_80124334[9] = {
	{ 0xff76, 0x0022, 0x0030, 0x48, 0x30 },
	{ 0xff82, 0x0040, 0x0024, 0x18, 0x30 },
	{ 0xff82, 0x004f, 0x0024, 0x78, 0x30 },
	{ 0xfffc, 0x0004, 0x0018, 0x9c, 0x30 },
	{ 0xfffc, 0x0013, 0x0018, 0xb4, 0x30 },
	{ 0xfffc, 0x0022, 0x0024, 0x00, 0x24 },
	{ 0xfffc, 0x0031, 0x0024, 0x24, 0x24 },
	{ 0xffaf, 0xffdd, 0x000c, 0x00, 0x30 },
	{ 0xfff6, 0xffdd, 0x000c, 0x0c, 0x30 },
};
#else
StringRect MAIN_D_80124334[9] = {
	{ 0xff76, 0x0022, 0x0030, 0x48, 0x30 },
	{ 0xff82, 0x0040, 0x0024, 0x18, 0x30 },
	{ 0xff82, 0x004f, 0x0024, 0x78, 0x30 },
	{ 0xfffc, 0x0004, 0x0018, 0x9c, 0x30 },
	{ 0xfffc, 0x0013, 0x0018, 0xb4, 0x30 },
	{ 0xfffc, 0x0022, 0x0024, 0x00, 0x24 },
	{ 0xfffc, 0x0031, 0x0024, 0x24, 0x24 },
	{ 0xffaf, 0xffdd, 0x0000, 0x00, 0x30 },
	{ 0xfff6, 0xffdd, 0x0000, 0x00, 0x30 },
};
#endif

IconRect MAIN_D_8012437C[21] = {
	{ 0xff74, 0xffaf, 0x35, 0x0b, 0x00, 0x4b },
	{ 0xff74, 0xfff4, 0x35, 0x0b, 0x00, 0x56 },
	{ 0xffee, 0xfff4, 0x35, 0x0b, 0x00, 0x61 },
	{ 0xff76, 0xffbe, 0x1a, 0x0a, 0x80, 0x30 },
	{ 0xff76, 0xffce, 0x1a, 0x0a, 0x9a, 0x30 },
	{ 0xffbd, 0xffde, 0x1c, 0x0a, 0xb4, 0x30 },
	{ 0xff7b, 0xffde, 0x13, 0x0a, 0x80, 0x3a },
	{ 0x0034, 0xffc2, 0x19, 0x08, 0x00, 0x38 },
	{ 0x0009, 0xffc2, 0x19, 0x08, 0x19, 0x38 },
	{ 0x0055, 0xffc2, 0x19, 0x08, 0x32, 0x38 },
	{ 0xff77, 0x004f, 0x09, 0x0b, 0xf0, 0x25 },
	{ 0xffef, 0x0004, 0x0b, 0x0b, 0x93, 0x3a },
	{ 0xffef, 0x0013, 0x0b, 0x0b, 0x9e, 0x3a },
	{ 0xffef, 0x0022, 0x0b, 0x0b, 0xa9, 0x3a },
	{ 0xffef, 0x0031, 0x0b, 0x0b, 0xb4, 0x3a },
	{ 0xffef, 0x0040, 0x0b, 0x0b, 0xbf, 0x3a },
	{ 0xffef, 0x004f, 0x0a, 0x0b, 0xca, 0x3a },
	{ 0xff82, 0x0031, 0x24, 0x0c, 0xd4, 0x3a },
	{ 0xfffc, 0x0040, 0x24, 0x0c, 0xd4, 0x46 },
	{ 0xfffc, 0x004f, 0x24, 0x0c, 0xd4, 0x52 },
	{ 0xff76, 0x004f, 0x09, 0x0b, 0x2c, 0x74 },
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

#if defined(VERSION_JP)
StringRect MAIN_D_801244EC[11] = {
	{ 0xff88, 0xffaf, 0x0018, 0x00, 0x0c },
	{ 0xffdb, 0xffca, 0x0054, 0x24, 0x0c },
	{ 0xffdb, 0xffef, 0x0054, 0x78, 0x0c },
	{ 0xffdb, 0x0002, 0x0048, 0x00, 0x18 },
	{ 0xffdb, 0x0016, 0x0048, 0x60, 0x18 },
	{ 0xffdb, 0x0039, 0x0054, 0x00, 0x24 },
	{ 0xffdb, 0x004d, 0x0048, 0x54, 0x24 },
	{ 0x0050, 0xffef, 0x0018, 0xcc, 0x0c },
	{ 0x0074, 0x0002, 0x0018, 0x48, 0x18 },
	{ 0x004d, 0x0016, 0x000c, 0xa8, 0x18 },
	{ 0x0050, 0x004d, 0x000c, 0x9c, 0x24 },
};
#else
StringRect MAIN_D_801244EC[11] = {
	{ 0xff84, 0xffaf, 0x0024, 0x00, 0x0c },
	{ 0xffdb, 0xffca, 0x0054, 0x24, 0x0c },
	{ 0xffdb, 0xffef, 0x0054, 0x78, 0x0c },
	{ 0xffdb, 0x0002, 0x0048, 0x00, 0x18 },
	{ 0xffdb, 0x0016, 0x0048, 0x60, 0x18 },
	{ 0xffdb, 0x0039, 0x0054, 0x00, 0x24 },
	{ 0xffdb, 0x004d, 0x0048, 0x54, 0x24 },
	{ 0x0050, 0xffef, 0x0018, 0xcc, 0x0c },
	{ 0x0074, 0x0002, 0x0018, 0x48, 0x18 },
	{ 0x004d, 0x0016, 0x000c, 0xa8, 0x18 },
	{ 0x0050, 0x004d, 0x000c, 0x9c, 0x24 },
};
#endif

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
	STR_MEDAL_NAME_GRADE_CUP,
	STR_MEDAL_NAME_VERSION_CUP,
	STR_MEDAL_NAME_TYPE_CUP,
	STR_MEDAL_NAME_SPECIAL_CUP,
	STR_MEDAL_NAME_100_TIMES,
	STR_MEDAL_NAME_TECHNIQUE_MASTER,
	STR_MEDAL_NAME_DIGIMON_MASTER,
	STR_MEDAL_NAME_MAX_ABILITIES,
	STR_MEDAL_NAME_PERFECT_CURLING,
	STR_MEDAL_NAME_100_FISH,
	MAIN_D_80134248,
	STR_MEDAL_NAME_TOWN_FLOURISHING,
	STR_MEDAL_NAME_CARD_COMPLETE,
	STR_MEDAL_NAME_BITS_MAXED,
	STR_MEDAL_NAME_10_YEARS,
};

#if defined(VERSION_JP)
char *MEDAL_DESCRIPTIONS[45] = {
	STR_MEDAL_DESCRIPTION_CUP_D_C_B_A_S,
	STR_MEDAL_DESCRIPTION_WIN_IN_ALL,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_WIN_IN_ALL_VER_1_2_3_4_0,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_FIRE_GRAPPLE_THUNDER_WIND,
	STR_MEDAL_DESCRIPTION_NATURE_COOL_METALLIC_FILTH_CUP,
	STR_MEDAL_DESCRIPTION_WIN_IN_ALL_2,
	STR_MEDAL_DESCRIPTION_DINO_WING_ANIMAL_HUMAN_CUP,
	STR_MEDAL_DESCRIPTION_WIN_IN_ALL_2,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_WON_CHAMPIONSHIP_100_TIMES,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_MASTERED_56_SWITCH_TECHNIQUES,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_RAISED_ALL_61_DIGIMON,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_MAXED_ALL_OF_THE_DIGIMONS,
	STR_MEDAL_DESCRIPTION_PARAMETERS,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_GOT_A_PERFECT_SCORE_IN_CURLING,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_100_FISH_CAUGHT,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_FINISHED_THE_GAME,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_JIJIMON_SAID_THE_TOWN,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_COLLECTED_ALL_DIGIMON_CARDS,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_COLLECTED_999999_BITS,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_SURVIVED_FOR_300_DAYS,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
};
#else
char *MEDAL_DESCRIPTIONS[45] = {
	STR_MEDAL_DESCRIPTION_CUP_D_C_B_A_S,
	STR_MEDAL_DESCRIPTION_WIN_IN_ALL,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_WIN_IN_ALL_VER_1_2_3_4_0,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_FIRE_GRAPPLE_THUNDER_WIND,
	STR_MEDAL_DESCRIPTION_NATURE_COOL_METALLIC_FILTH_CUP,
	STR_MEDAL_DESCRIPTION_WIN_IN_ALL,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_DINO_WING_ANIMAL_HUMAN_CUP,
	STR_MEDAL_DESCRIPTION_WIN_IN_ALL,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_WON_CHAMPIONSHIP_100_TIMES,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_MASTERED_56_SWITCH_TECHNIQUES,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_RAISED_ALL_61_DIGIMON,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_MAXED_ALL_OF_THE_DIGIMONS,
	STR_MEDAL_DESCRIPTION_PARAMETERS,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_GOT_A_PERFECT_SCORE_IN_CURLING,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_100_FISH_CAUGHT,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_FINISHED_THE_GAME,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_JIJIMON_SAID_THE_TOWN,
	STR_MEDAL_DESCRIPTION_IS_FLOURISHING,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_COLLECTED_ALL_DIGIMON_CARDS,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_COLLECTED_999999_BITS,
	STR_OVERWORLD_EMPTY,
	STR_OVERWORLD_EMPTY,
	STR_MEDAL_DESCRIPTION_SURVIVED_FOR_300_DAYS,
	STR_OVERWORLD_EMPTY,
};
#endif

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

Line4Points MAIN_D_80124944[4] = {
	{ 0xffac, 0xffbc, 0xffdb, 0xfff6, 0xffd0, 0xffd0, 0xffef, 0xffef },
	{ 0xff9a, 0xffb6, 0xffc0, 0xfff6, 0xffe8, 0xffe8, 0xfff2, 0xfff2 },
	{ 0xff9a, 0xffb6, 0xffc0, 0xfff6, 0xffff, 0xffff, 0xfff5, 0xfff5 },
	{ 0xffac, 0xffbc, 0xffdb, 0xfff6, 0x0017, 0x0017, 0xfff8, 0xfff8 },
};

Line4Points MAIN_D_80124984[5] = {
	{ 0xffbe, 0xffcc, 0xfff6, 0xfff6, 0xffc4, 0xffc4, 0xffee, 0xffee },
	{ 0xffac, 0xffc6, 0xffdb, 0xfff6, 0xffdc, 0xffdc, 0xfff1, 0xfff1 },
	{ 0xff9a, 0xfff6, 0xfff6, 0xfff6, 0xfff4, 0xfff4, 0xfff4, 0xfff4 },
	{ 0xffac, 0xffc6, 0xffdb, 0xfff6, 0x000c, 0x000c, 0xfff7, 0xfff7 },
	{ 0xffbe, 0xffcc, 0xfff6, 0xfff6, 0x0024, 0x0024, 0xfffa, 0xfffa },
};

Line4Points MAIN_D_801249D4[6] = {
	{ 0x0009, 0x003d, 0x0040, 0x0040, 0xffec, 0xffb8, 0xffb8, 0xffb8 },
	{ 0x0009, 0x0024, 0x0043, 0x0052, 0xffef, 0xffef, 0xffd0, 0xffd0 },
	{ 0x0009, 0x003f, 0x0049, 0x0064, 0xfff2, 0xfff2, 0xffe8, 0xffe8 },
	{ 0x0009, 0x003f, 0x0049, 0x0064, 0xfff5, 0xfff5, 0xffff, 0xffff },
	{ 0x0009, 0x0024, 0x0043, 0x0052, 0xfff8, 0xfff8, 0x0017, 0x0017 },
	{ 0x0009, 0x003d, 0x0040, 0x0040, 0xfffb, 0x002f, 0x002f, 0x002f },
};

Line4Points MAIN_D_80124A34[5] = {
	{ 0x0009, 0x0033, 0x0040, 0x0040, 0xffee, 0xffc4, 0xffc4, 0xffc4 },
	{ 0x0009, 0x0024, 0x0039, 0x0052, 0xfff1, 0xfff1, 0xffdc, 0xffdc },
	{ 0x0009, 0x0064, 0x0064, 0x0064, 0xfff4, 0xfff4, 0xfff4, 0xfff4 },
	{ 0x0009, 0x0024, 0x0039, 0x0052, 0xfff7, 0xfff7, 0x000c, 0x000c },
	{ 0x0009, 0x0033, 0x0040, 0x0040, 0xfffa, 0x0024, 0x0024, 0x0024 },
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

ChartSprite MAIN_D_80124AA8[4] = {
	{ 0xff9a, 0xffc7, 0x0000, 0x0000 },
	{ 0xff88, 0xffdf, 0x0000, 0x0000 },
	{ 0xff88, 0xfff7, 0x0000, 0x0000 },
	{ 0xff9a, 0x000f, 0x0000, 0x0000 },
};

ChartSprite MAIN_D_80124AC8[5] = {
	{ 0xffac, 0xffbb, 0x0000, 0x0000 },
	{ 0xff9a, 0xffd3, 0x0000, 0x0000 },
	{ 0xff88, 0xffeb, 0x0000, 0x0000 },
	{ 0xff9a, 0x0003, 0x0000, 0x0000 },
	{ 0xffac, 0x001b, 0x0000, 0x0000 },
};

ChartSprite MAIN_D_80124AF0[6] = {
	{ 0x0042, 0xffaf, 0x0000, 0x0000 },
	{ 0x0054, 0xffc7, 0x0000, 0x0000 },
	{ 0x0066, 0xffdf, 0x0000, 0x0000 },
	{ 0x0066, 0xfff7, 0x0000, 0x0000 },
	{ 0x0054, 0x0010, 0x0000, 0x0000 },
	{ 0x0042, 0x0027, 0x0000, 0x0000 },
};

ChartSprite MAIN_D_80124B20[5] = {
	{ 0x0042, 0xffbb, 0x0000, 0x0000 },
	{ 0x0054, 0xffd3, 0x0000, 0x0000 },
	{ 0x0066, 0xffeb, 0x0000, 0x0000 },
	{ 0x0054, 0x0003, 0x0000, 0x0000 },
	{ 0x0042, 0x001b, 0x0000, 0x0000 },
};

#if defined(VERSION_JP)
Inset MAIN_D_80124B48[6] = {
	{ 0x001c, 0x002b, 0x004e, 0x0082 },
	{ 0x0081, 0x0032, 0x003c, 0x0004 },
	{ 0x0088, 0x0055, 0x0030, 0x002e },
	{ 0x00d6, 0x002a, 0x004e, 0x0082 },
	{ 0x0044, 0x00bb, 0x0030, 0x0004 },
	{ 0x001a, 0x00ce, 0x0084, 0x0004 },
};
#else
Inset MAIN_D_80124B48[6] = {
	{ 0x001c, 0x002b, 0x004e, 0x0082 },
	{ 0x0081, 0x0032, 0x003c, 0x0004 },
	{ 0x0088, 0x0055, 0x0030, 0x002e },
	{ 0x00d6, 0x002a, 0x004e, 0x0082 },
	{ 0x0026, 0x00bb, 0x006c, 0x0004 },
	{ 0x001a, 0x00ce, 0x0084, 0x0004 },
};
#endif

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

#if defined(VERSION_JP)
char MAIN_D_80124C0C[2][12] = {
	"最大ＨＰ",
	"最大ＭＰ",
};

char MAIN_D_80124C24[12] = "攻撃力";

char MAIN_D_80124C30[3][12] = {
	"防御力",
	"すばやさ",
	"かしこさ",
};
#else
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
#endif

char MAIN_D_80124C54[] = {
	0x82, 0x4f, 0x82, 0x50, 0x82, 0x51, 0x82, 0x52,
	0x82, 0x53, 0x82, 0x54, 0x82, 0x55, 0x82, 0x56,
	0x82, 0x57, 0x82, 0x58, 0x00,
};

#if defined(VERSION_JP)
char STR_MOVE_NAME_FIRE_TOWER[] = "ファイアータワー";

char STR_MOVE_NAME_PROMINENCE_BEAM[] = "プロミネンスビーム";

char STR_MOVE_NAME_SPIT_FIRE[] = "スピットファイアー";

char STR_MOVE_NAME_RED_INFERNO[] = "レッドインフェルノ";

char STR_MOVE_NAME_MAGMA_BOMB[] = "マグマボム";

char STR_MOVE_NAME_HEAT_LASER[] = "ヒートウェーブ";

char STR_MOVE_NAME_INIFINITY_BURN[] = "インフィニティバーン";

char STR_MOVE_NAME_MELTDOWN[] = "メルトダウン";

char STR_MOVE_NAME_THUNDER_JUSTICE[] = "サンダージャスティス";

char STR_MOVE_NAME_SPINNING_SHOT[] = "スピニングショット";

char STR_MOVE_NAME_ELECTRIC_CLOUD[] = "エレキクラウド";

char STR_MOVE_NAME_MEGALO_SPARK[] = "メガロスパーク";

char STR_MOVE_NAME_STATIC_ELECT[] = "スタティックエレクト";

char STR_MOVE_NAME_WIND_CUTTER[] = "ウインドカッター";

char STR_MOVE_NAME_CONFUSED_STORM[] = "コンフューズストーム";

char STR_MOVE_NAME_HURRICANE[] = "ハリケーン";

char STR_MOVE_NAME_GIGA_FREEZE[] = "ギガフリーズ";

char STR_MOVE_NAME_ICE_STATUE[] = "アイススタチュー";

char STR_MOVE_NAME_WINTER_BLAST[] = "ウィンターブラスト";

char STR_MOVE_NAME_ICE_NEEDLE[] = "アイスニードル";

char STR_MOVE_NAME_WATER_BLIT[] = "ウォーターブリット";

char STR_MOVE_NAME_AQUA_MAGIC[] = "アクアマジック";

char STR_MOVE_NAME_AURORA_FREEZE[] = "オーロラフリーズ";

char STR_MOVE_NAME_TEAR_DROP[] = "ティアドロップ";

char STR_MOVE_NAME_POWER_CRANE[] = "パワークレーン";

char STR_MOVE_NAME_ALL_RANGE_BEAM[] = "オールレンジビーム";

char STR_MOVE_NAME_METAL_SPRINTER[] = "メタルスプリンター";

char STR_MOVE_NAME_PULSE_LASER[] = "パルスレーザー";

char STR_MOVE_NAME_DELETE_PROGRAM[] = "デリートプログラム";

char STR_MOVE_NAME_DG_DIMENSION[] = "ＤＧディメンジョン";

char STR_MOVE_NAME_FULL_POTENTIAL[] = "フルポテンシャル";

char STR_MOVE_NAME_REVERSE_PROG[] = "リバースプログラム";

char STR_MOVE_NAME_POISON_POWDER[] = "ポイズンパウダー";

char STR_MOVE_NAME_MASS_MORPH[] = "マスモーフ";

char STR_MOVE_NAME_INSECT_PLAGUE[] = "インセクトプレーグ";

char STR_MOVE_NAME_CHARM_PERFUME[] = "チャームパヒューム";

char STR_MOVE_NAME_POISON_CLAW[] = "ポイズンクロー";

char STR_MOVE_NAME_DANGER_STING[] = "デンジャースティング";

char STR_MOVE_NAME_GREEN_TRAP[] = "グリーントラップ";

char STR_MOVE_NAME_TREMAR[] = "トレマー";

char STR_MOVE_NAME_MUSCLE_CHARGE[] = "マッスルチャージ";

char STR_MOVE_NAME_WAR_CRY[] = "ウォークライ";

char STR_MOVE_NAME_SONIC_JAB[] = "マッハジャブ";

char STR_MOVE_NAME_DYNAMITE_KICK[] = "ダイナマイトキック";

char STR_MOVE_NAME_COUNTER[] = "カウンター";

char STR_MOVE_NAME_MEGATON_PUNCH[] = "メガトンパンチ";

char STR_MOVE_NAME_BUSTER_DIVE[] = "バスターダイブ";

char STR_MOVE_NAME_ODOR_SPRAY[] = "超悪臭噴射";

char STR_MOVE_NAME_POOP_SPD_TOSS[] = "高速ウンチ投げ";

char STR_MOVE_NAME_BIG_POOP_TOSS[] = "巨大ウンチ投げ";

char STR_MOVE_NAME_BIG_RND_TOSS[] = "巨大ウンチ乱れ投げ";

char STR_MOVE_NAME_POOP_RND_TOSS[] = "ウンチ乱れ投げ";

char STR_MOVE_NAME_RND_SPD_TOSS[] = "高速ウンチ乱れ投げ";

char STR_MOVE_NAME_HORIZONTAL_KICK[] = "エンガチョキック";

char STR_MOVE_NAME_ULT_POOP_HELL[] = "究極ウンチ地獄絵図";

char STR_MOVE_NAME_BLAZE_BLAST[] = "ファイアーブレス";

char STR_MOVE_NAME_PEPPER_BREATH[] = "ベビーフレイム";

char STR_MOVE_NAME_LOVELY_ATTACK[] = "ラブリーアタック";

char STR_MOVE_NAME_FIREBALL[] = "バーニングフィスト";

char STR_MOVE_NAME_DEATH_CLAW[] = "デスクロウ";

char STR_MOVE_NAME_MEGA_FLAME[] = "メガフレイム";

char STR_MOVE_NAME_HOWLING_BLASTER[] = "フォックスファイアー";

char STR_MOVE_NAME_ELECTRIC_SHOCK[] = "電撃ビリリン";

char STR_MOVE_NAME_ABDUCTION_BEAM[] = "アブダクション光線";

char STR_MOVE_NAME_SMILEY_BOMB[] = "スマイリーボム";

char STR_MOVE_NAME_SPNNING_NEEDLE[] = "スピニングニードル";

char STR_MOVE_NAME_SPIRAL_TWISTER[] = "マジカルファイアー";

char STR_MOVE_NAME_BOOM_BUBBLE[] = "エアショット";

char STR_MOVE_NAME_SWEET_BREATH[] = "甘い吐息";

char STR_MOVE_NAME_BIT_BOMB[] = "ビットボム";

char STR_MOVE_NAME_DEADLY_BOMB[] = "デッドリーボム";

char STR_MOVE_NAME_DRILL_SPIN[] = "ドリルスピン";

char STR_MOVE_NAME_ELECTRIC_THREAD[] = "エレクトリックスレッド";

char STR_MOVE_NAME_ENERGY_BOMB[] = "エネルギーボム";

char STR_MOVE_NAME_GENOSIDE_ATTACK[] = "ジェノサイドアタック";

char STR_MOVE_NAME_GIGA_SCISSOR_CLAW[] = "ギガデストロイヤー";

char STR_MOVE_NAME_DARK_SHOT[] = "グラウンドゼロ";

char STR_MOVE_NAME_HAND_OF_FATE[] = "ヘブンズナックル";

char STR_MOVE_NAME_DARK_CLAW[] = "ヘルズハンド";

char STR_MOVE_NAME_AERIAL_ATTACK[] = "ホーリーショット";

char STR_MOVE_NAME_BONE_BOOMERANG[] = "骨骨ブーメラン";

char STR_MOVE_NAME_SOLAR_RAY[] = "ハンティングキャノン";

char STR_MOVE_NAME_HYDRO_PRESSURE[] = "ハイドロプレッシャー";

char STR_MOVE_NAME_ICE_BLAST[] = "アイスアロー";

char STR_MOVE_NAME_IGA_SCHOOL_KNIFE_THROW[] = "イガ流手裏剣投げ";

char STR_MOVE_NAME_BLASTING_SPOUT[] = "ジェットアロー";

char STR_MOVE_NAME_DARK_NETWORK_CONCERT_CRUSH[] = "ラブ・セレナーデ";

char STR_MOVE_NAME_ELECTRO_SHOCKER[] = "メガブラスター";

char STR_MOVE_NAME_METEOR_WING[] = "メテオウイング";

char STR_MOVE_NAME_SUPER_SLAP[] = "無限ビンタ";

char STR_MOVE_NAME_NIGHTMARE_SYNDROMER[] = "ナイトメアシンドローム";

char STR_MOVE_NAME_FROZEN_FIRE_SHOT[] = "ペトラファイアー";

char STR_MOVE_NAME_POISON_IVY[] = "ポイズンアイビー";

char STR_MOVE_NAME_BLUE_BLASTER[] = "プチファイアー";

char STR_MOVE_NAME_SCISSOR_CLAW[] = "シザーアームズ";

char STR_MOVE_NAME_SUPER_THUNDER_STRIKE[] = "スパークリングサンダー";

char STR_MOVE_NAME_SPIRAL_SWORD[] = "スパイラルソード";

char STR_MOVE_NAME_VARIABLE_DARTS[] = "ヴァリアブルダーツ";

char STR_MOVE_NAME_VOLCANIC_STRIKE[] = "ヴォルケーノストライク";

char STR_MOVE_NAME_SUBZERO_ICE_PUNCH[] = "絶対零度パンチ";

char STR_MOVE_NAME_INFINITY_CANNON[] = "無限キャノン";

char STR_MOVE_NAME_CRIMSON_FLARE[] = "クリムゾンフレア";

char STR_MOVE_NAME_GLACIAL_BLAST[] = "グレイシャルブラスト";

char STR_MOVE_NAME_MAIL_STROME[] = "メイルストローム";

char STR_MOVE_NAME_HIGH_ELECTRO_SHOCKER[] = "ハイメガブラスター";

char STR_ITEM_DESC_SMALL_RECOVERY_500_HP[] = "ＨＰを５００回復する。";

char STR_ITEM_DESC_MEDIUM_RECOVERY_1500_HP[] = "ＨＰを１５００回復する。";

char STR_ITEM_DESC_LARGE_RECOVERY_5000_HP[] = "ＨＰを５０００回復する。";

char STR_ITEM_DESC_SUPER_RECOVERY_FULL_HP[] = "ＨＰをフル回復するぞ！";

char STR_ITEM_DESC_RECOVER_500_MAGIC_POINTS[] = "ＭＰを５００回復する。";

char STR_ITEM_DESC_MED_MP_RECOVER_1500_MP[] = "ＭＰを１５００回復する。";

char STR_ITEM_DESC_LRG_MP_RECOVER_5000_MP[] = "ＭＰを５０００回復する。";

char STR_ITEM_DESC_RECOVERS_1500_MP_AND_HP[] = "ＨＰとＭＰを１５００ずつ回復する。";

char STR_ITEM_DESC_CURES_STATUS_ERRORS[] = "毒・マヒ・混乱・液晶化を治す。";

char STR_ITEM_DESC_CURES_ERRORS_REC_HP_MP[] = "状態異常を治し、ＨＰ、ＭＰも回復するぞ！";

char STR_ITEM_DESC_PROTECTS_YR_COND_IN_BATTLE[] = "戦闘中の全ての状態変化を防ぐぞ！";

char STR_ITEM_DESC_CURES_COMA_REC_HALF_HP[] = "仮死を治し、ＨＰは半分回復するぞ！";

char STR_ITEM_DESC_CURES_COMA_ERRORS_FULL_HP[] = "状態異常、仮死を治し、ＨＰも全快するぞ！";

char STR_ITEM_DESC_CURES_WOUNDS_SOME_SICKNESS[] = "「ケガ」を治し、「病気」にも効くかも？";

char STR_ITEM_DESC_CURES_WOUNDS_SICKNESS[] = "「ケガ」、「病気」を治す。";

char STR_ITEM_DESC_BOOST_OFF_POWER_IN_BATTLE[] = "使った戦闘中、攻撃力がアップする。";

char STR_ITEM_DESC_BOOST_DEF_POWER_IN_BATTLE[] = "使った戦闘中、防御力がアップする。";

char STR_ITEM_DESC_BOOST_SPEED_IN_BATTLE[] = "使った戦闘中、すばやさがアップする。";

char STR_ITEM_DESC_BOOST_ALL_SKILLS_IN_BATTLE[] = "使った戦闘中、全能力がアップする。";

char STR_ITEM_DESC_SUPER_BOOST_OFF_PWR_IN_BAT[] = "使った戦闘中、攻撃力が大きくアップする。";

char STR_ITEM_DESC_SUPER_BOOST_DEF_PWR_IN_BAT[] = "使った戦闘中、防御力が大きくアップする。";

char STR_ITEM_DESC_SUPER_BOOST_SPEED_IN_BATTLE[] = "使った戦闘中、すばやさが大きくアップする。";

char STR_ITEM_DESC_CAN_RETURN_TO_CITY_QUICKLY[] = "街に一瞬で戻ることができるぞ！";

char STR_ITEM_DESC_BOOST_MAX_OFF_PWR_LEVEL_50[] = "攻撃力を永久に＋５０するぞ！";

char STR_ITEM_DESC_BOOST_MAX_DEF_PWR_LEVEL_50[] = "防御力を永久に＋５０するぞ！";

char STR_ITEM_DESC_BOOST_MAX_BRAINS_LEVEL_50[] = "かしこさを永久に＋５０するぞ！";

char STR_ITEM_DESC_BOOST_MAX_SPEED_LEVEL_50[] = "すばやさを永久に＋５０するぞ！";

char STR_ITEM_DESC_BOOST_MAX_HP_LEVEL_500[] = "ＨＰ上限を永久に＋５００するぞ！";

char STR_ITEM_DESC_BOOST_MAX_MP_LEVEL_500[] = "ＭＰ上限を永久に＋５００するぞ！";

char STR_ITEM_DESC_BOOST_OFF_PWR_BRAINS_100[] = "攻撃力、かしこさを永久に＋１００。しかし…";

char STR_ITEM_DESC_BOOST_DEF_PWR_SPEED_100[] = "防御力、すばやさを永久に＋１００。しかし…";

char STR_ITEM_DESC_BOOST_OFF_PWR_SPEED_1000[] = "ＨＰ、ＭＰ上限を永久に＋１０００。しかし…";

char STR_ITEM_DESC_CAN_DO_POTTY_ANYWHERE[] = "使い捨てのトイレ。トイレが無い場所も安心！";

char STR_ITEM_DESC_TRAIN_BETTER_WITH_THIS[] = "持っているだけで、トレーニング効果アップ！";

char STR_ITEM_DESC_MORE_RECOVERY_DURING_REST[] = "持っているだけで、睡眠時の回復力アップ！";

char STR_ITEM_DESC_REPELS_ENEMIES_TO_STAY_AWAY[] = "持っていると敵が参加しにくくなるぞ！";

char STR_ITEM_DESC_ATTRACT_ENEMIES_TO_COME[] = "持っていると敵が参加しやすくなるぞ！";

char STR_ITEM_DESC_WALK_AND_HP_MP_GO_UP[] = "歩くとＨＰとＭＰが少し回復。走るとダメ！";

char STR_ITEM_DESC_MAKES_DIGIMON_A_BIT_FULL[] = "食べ物。そこそこおなかがふくれる。";

char STR_ITEM_DESC_MAKES_DIGIMON_QUITE_FULL[] = "食べ物。かなりおなかがふくれる。";

char STR_ITEM_DESC_MAKES_DIGIMON_VERY_FULL[] = "食べ物。すごくおなかがふくれる。";

char STR_ITEM_DESC_BOOSTS_TRAINING_EFFECT[] = "食べ物。しばらくトレーニング効果アップ。";

char STR_ITEM_DESC_GREATLY_REDUCES_TIREDNESS[] = "食べ物。大幅に疲れがとれるぞ！";

char STR_ITEM_DESC_MAKE_DIGIMON_A_BIT_FULL[] = "食べ物。少しおなかがふくれるぞ！";

char STR_ITEM_DESC_GREATLY_BOOSTS_DISCIPLINE[] = "食べ物。しつけが大幅アップ！";

char STR_ITEM_DESC_BOOSTS_ALL_ABILITIES[] = "食べ物。全能力アップ！";

char STR_ITEM_DESC_BOOSTS_TRAINING_EFFECT_2[] = "食べ物。しばらくトレーニング効果アップ！";

char STR_ITEM_DESC_MAKES_DIGIMON_HAPPY[] = "食べ物。ごきげんが大幅アップ！";

char STR_ITEM_DESC_GIVES_REST_BOOST_DISC_HAP[] = "食べ物。疲れ回復、しつけとごきげんアップ！";

char STR_ITEM_DESC_CAN_BE_SOLD_FOR_A_HIGH_PRICE[] = "食べ物。高く売れるぞ！";

char STR_ITEM_DESC_MAKES_FULL_BOOSTS_WEIGHT[] = "食べ物。満腹になるが体重が大きくアップ！";

char STR_ITEM_DESC_RECOVERS_HP_COMPLETELY[] = "食べ物。ＨＰが全快するぞ！";

char STR_ITEM_DESC_RECOVERS_MP_COMPLETELY[] = "食べ物。ＭＰが全快するぞ！";

char STR_ITEM_DESC_LOWERS_WEIGHT[] = "食べ物。体重が減るぞ！";

char STR_ITEM_DESC_FULLY_RECOVERS_HP_AND_MP[] = "食べ物。ＨＰとＭＰが回復するぞ！";

char STR_ITEM_DESC_BOOST_OFFENSIVE_POWER_20[] = "食べ物。攻撃力が２０アップ！";

char STR_ITEM_DESC_BOOST_DEFENSIVE_POWER_20[] = "食べ物。防御力が２０アップ！";

char STR_ITEM_DESC_BOOST_SPEED_20[] = "食べ物。すばやさが２０アップ！";

char STR_ITEM_DESC_BOOST_BRAINS_20[] = "食べ物。かしこさが２０アップ！";

char STR_ITEM_DESC_BOOST_HP_BY_200[] = "食べ物。ＨＰが２００アップ！";

char STR_ITEM_DESC_BOOST_MP_BY_200[] = "食べ物。ＭＰが２００アップ！";

char STR_ITEM_DESC_MAKES_DIGIMON_A_BIT_FULL_2[] = "食べ物。少しおなかがふくれる。";

char STR_ITEM_DESC_FULL_HP_AND_MP_LIFE_SPAN[] = "食べ物。ＨＰ＆ＭＰ全快、寿命ものびるぞ！";

char STR_ITEM_DESC_MAKES_DIGIMON_SOMEWHAT_FULL[] = "食べ物。一応おなかはふくれるぞ！";

char STR_ITEM_DESC_BOOST_HAPPINESS_BUT_RISKY[] = "食べ物。ごきげんアップ！でも運が悪いと…";

char STR_ITEM_DESC_GOOD_FOR_MANY_THINGS[] = "食べ物。様々なすばらしい効果がえられるぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_GREYMON[] = "グレイモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_MERAMON[] = "メラモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_BIRDRAMON[] = "バードラモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_CENTARUMON[] = "ケンタルモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_MONOCHROMON[] = "モノクロモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_DRIMOGEMON[] = "ドリモゲモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_TYRANNOMON[] = "ティラノモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_DEVIMON[] = "デビモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_OGREMON[] = "オーガモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_LEOMON[] = "レオモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_ANGEMON[] = "エンジェモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_BAKEMON[] = "バケモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_KAMINARIMON[] = "カミナリモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_AIRDRAMON[] = "エアドラモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_KOKATORIMON[] = "コカトリモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_UNIMON[] = "ユニモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_KABUTERIMON[] = "カブテリモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_KUWAGAMON[] = "クワガーモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_VEGIEMON[] = "ベジーモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_NINJAMON[] = "イガモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_SEADRAMON[] = "シードラモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_WHAMON[] = "ホエーモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_SHELLMON[] = "シェルモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_COELAMON[] = "シーラモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_GARURUMON[] = "ガルルモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_FRIGIMON[] = "ユキダルモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_MOJYAMON[] = "モジャモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_NANIMON[] = "ナニモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_METALGREYMON[] = "メタルグレイモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_SKULLGREYMON[] = "スカルグレイモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_ANDROMON[] = "アンドロモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_MEGADRAMON[] = "メガドラモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_MAMEMON[] = "マメモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_METALMAMEMON[] = "メタルマメモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_GIROMON[] = "ギロモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_PIXIMON[] = "ピッコロモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_MONZAEMON[] = "もんざえモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_VADEMON[] = "ベーダモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_ETEMON[] = "エテモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_DIGITAMAMON[] = "デジタマモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_PHOENIXMON[] = "ホウオウモンに進化するぞ！";

char STR_ITEM_DESC_BECOME_HERCULESKABUTERIMON[] = "ヘラクルカブテリモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_MEGASEADRAMON[] = "メガシードラモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_WEREGARURUMON[] = "ウェアガルルモンに進化するぞ！";

char STR_ITEM_DESC_SEADRAMON_FRIENDSHIP_PROOF[] = "シードラモンとの友情のあかし。";

char STR_ITEM_DESC_ENABLES_YOU_TO_FISH_AT_LAKE[] = "湖で釣りができるぞ！";

char STR_ITEM_DESC_GIVES_GOOD_FISHING_AT_LAKE[] = "湖で釣りができ、よく釣れるぞ！";

char STR_ITEM_DESC_STONE_TABLET_OF_LEOMON[] = "レオモンの先祖がのこした石板。";

char STR_ITEM_DESC_KEY_TO_GRAY_LORD_MANSION[] = "闇貴族の館のカギ。";

char STR_ITEM_DESC_RECOVER_1000_MP_OTHER_USES[] = "ＭＰが１０００回復。他にも使い道が…";

char STR_ITEM_DESC_KEY_TO_OPEN_REFRIGERATOR[] = "れいぞうこをあけるカギ。";

char STR_ITEM_DESC_YOU_CAN_READ_ANCIENT_SCRIPT[] = "古代文字が読めるようになるぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_GIGADRAMON[] = "ギガドラモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_PANJYAMON[] = "パンジャモンに進化するぞ！";

char STR_ITEM_DESC_DIGIVOLVE_TO_METALETEMON[] = "メタルエテモンに進化するぞ！";
#else
char STR_MOVE_NAME_FIRE_TOWER[] = "Fire Tower";

char STR_MOVE_NAME_PROMINENCE_BEAM[16] = "Prominence Beam";

char STR_MOVE_NAME_SPIT_FIRE[] = "Spit Fire";

char STR_MOVE_NAME_RED_INFERNO[12] = "Red Inferno";

char STR_MOVE_NAME_MAGMA_BOMB[] = "Magma Bomb";

char STR_MOVE_NAME_HEAT_LASER[] = "Heat Laser";

char STR_MOVE_NAME_INIFINITY_BURN[] = "Inifinity Burn";

char STR_MOVE_NAME_MELTDOWN[] = "Meltdown";

char STR_MOVE_NAME_THUNDER_JUSTICE[16] = "Thunder Justice";

char STR_MOVE_NAME_SPINNING_SHOT[] = "Spinning Shot";

char STR_MOVE_NAME_ELECTRIC_CLOUD[] = "Electric Cloud";

char STR_MOVE_NAME_MEGALO_SPARK[] = "Megalo Spark";

char STR_MOVE_NAME_STATIC_ELECT[] = "Static Elect";

char STR_MOVE_NAME_WIND_CUTTER[12] = "Wind Cutter";

char STR_MOVE_NAME_CONFUSED_STORM[] = "Confused Storm";

char STR_MOVE_NAME_HURRICANE[] = "Hurricane";

char STR_MOVE_NAME_GIGA_FREEZE[12] = "Giga Freeze";

char STR_MOVE_NAME_ICE_STATUE[] = "Ice Statue";

char STR_MOVE_NAME_WINTER_BLAST[] = "Winter Blast";

char STR_MOVE_NAME_ICE_NEEDLE[] = "Ice Needle";

char STR_MOVE_NAME_WATER_BLIT[] = "Water Blit";

char STR_MOVE_NAME_AQUA_MAGIC[] = "Aqua Magic";

char STR_MOVE_NAME_AURORA_FREEZE[] = "Aurora Freeze";

char STR_MOVE_NAME_TEAR_DROP[] = "Tear Drop";

char STR_MOVE_NAME_POWER_CRANE[12] = "Power Crane";

char STR_MOVE_NAME_ALL_RANGE_BEAM[] = "All Range Beam";

char STR_MOVE_NAME_METAL_SPRINTER[] = "Metal Sprinter";

char STR_MOVE_NAME_PULSE_LASER[12] = "Pulse Laser";

char STR_MOVE_NAME_DELETE_PROGRAM[] = "Delete Program";

char STR_MOVE_NAME_DG_DIMENSION[] = "DG Dimension";

char STR_MOVE_NAME_FULL_POTENTIAL[] = "Full Potential";

char STR_MOVE_NAME_REVERSE_PROG[] = "Reverse Prog";

char STR_MOVE_NAME_POISON_POWDER[] = "Poison Powder";

char STR_MOVE_NAME_MASS_MORPH[] = "Mass Morph";

char STR_MOVE_NAME_INSECT_PLAGUE[] = "Insect Plague";

char STR_MOVE_NAME_CHARM_PERFUME[] = "Charm Perfume";

char STR_MOVE_NAME_POISON_CLAW[12] = "Poison Claw";

char STR_MOVE_NAME_DANGER_STING[] = "Danger Sting";

char STR_MOVE_NAME_GREEN_TRAP[] = "Green Trap";

char STR_MOVE_NAME_MUSCLE_CHARGE[] = "Muscle Charge";

char STR_MOVE_NAME_SONIC_JAB[] = "Sonic Jab";

char STR_MOVE_NAME_DYNAMITE_KICK[] = "Dynamite Kick";

char STR_MOVE_NAME_MEGATON_PUNCH[] = "Megaton Punch";

char STR_MOVE_NAME_BUSTER_DIVE[12] = "Buster Dive";

char STR_MOVE_NAME_ODOR_SPRAY[] = "Odor Spray";

char STR_MOVE_NAME_POOP_SPD_TOSS[] = "Poop Spd Toss";

char STR_MOVE_NAME_BIG_POOP_TOSS[] = "Big Poop Toss";

char STR_MOVE_NAME_BIG_RND_TOSS[] = "Big Rnd Toss";

char STR_MOVE_NAME_POOP_RND_TOSS[] = "Poop Rnd Toss";

char STR_MOVE_NAME_RND_SPD_TOSS[] = "Rnd Spd Toss";

char STR_MOVE_NAME_HORIZONTAL_KICK[16] = "Horizontal Kick";

char STR_MOVE_NAME_ULT_POOP_HELL[] = "Ult Poop Hell";

char STR_MOVE_NAME_BLAZE_BLAST[12] = "Blaze Blast";

char STR_MOVE_NAME_PEPPER_BREATH[] = "Pepper Breath";

char STR_MOVE_NAME_LOVELY_ATTACK[] = "Lovely Attack";

char STR_MOVE_NAME_FIREBALL[] = "Fireball";

char STR_MOVE_NAME_DEATH_CLAW[] = "Death Claw";

char STR_MOVE_NAME_MEGA_FLAME[] = "Mega Flame";

char STR_MOVE_NAME_HOWLING_BLASTER[16] = "Howling Blaster";

char STR_MOVE_NAME_PARTY_TIME[] = "Party time";

char STR_MOVE_NAME_ELECTRIC_SHOCK[] = "Electric Shock";

char STR_MOVE_NAME_ABDUCTION_BEAM[] = "Abduction Beam";

char STR_MOVE_NAME_SMILEY_BOMB[12] = "Smiley Bomb";

char STR_MOVE_NAME_SPNNING_NEEDLE[] = "Spnning Needle";

char STR_MOVE_NAME_SPIRAL_TWISTER[] = "Spiral Twister";

char STR_MOVE_NAME_BOOM_BUBBLE[12] = "Boom Bubble";

char STR_MOVE_NAME_SWEET_BREATH[] = "Sweet Breath";

char STR_MOVE_NAME_BIT_BOMB[] = "Bit Bomb";

char STR_MOVE_NAME_DEADLY_BOMB[12] = "Deadly Bomb";

char STR_MOVE_NAME_DRILL_SPIN[] = "Drill Spin";

char STR_MOVE_NAME_ELECTRIC_THREAD[16] = "Electric Thread";

char STR_MOVE_NAME_ENERGY_BOMB[12] = "Energy Bomb";

char STR_MOVE_NAME_GENOSIDE_ATTACK[16] = "Genoside Attack";

char STR_MOVE_NAME_GIGA_SCISSOR_CLAW[] = "Giga Scissor Claw";

char STR_MOVE_NAME_DARK_SHOT[] = "Dark Shot";

char STR_MOVE_NAME_PUMMEL_WHACK[] = "Pummel Whack";

char STR_MOVE_NAME_HAND_OF_FATE[] = "Hand of Fate";

char STR_MOVE_NAME_DARK_CLAW[] = "Dark Claw";

char STR_MOVE_NAME_AERIAL_ATTACK[] = "Aerial Attack";

char STR_MOVE_NAME_BONE_BOOMERANG[] = "Bone Boomerang";

char STR_MOVE_NAME_SOLAR_RAY[] = "Solar Ray";

char STR_MOVE_NAME_HYDRO_PRESSURE[] = "Hydro Pressure";

char STR_MOVE_NAME_ICE_BLAST[] = "Ice Blast";

char STR_MOVE_NAME_IGA_SCHOOL_KNIFE_THROW[] = "Iga School Knife Throw";

char STR_MOVE_NAME_BLASTING_SPOUT[] = "Blasting Spout";

char STR_MOVE_NAME_FIST_OF_THE_BEAST_KING[] = "Fist of the Beast King";

char STR_MOVE_NAME_DARK_NETWORK_CONCERT_CRUSH[] = "Dark Network & Concert Crush";

char STR_MOVE_NAME_ELECTRO_SHOCKER[16] = "Electro Shocker";

char STR_MOVE_NAME_METEOR_WING[12] = "Meteor Wing";

char STR_MOVE_NAME_SUPER_SLAP[] = "Super Slap";

char STR_MOVE_NAME_NIGHTMARE_SYNDROMER[20] = "Nightmare Syndromer";

char STR_MOVE_NAME_FROZEN_FIRE_SHOT[] = "Frozen Fire Shot";

char STR_MOVE_NAME_POISON_IVY[] = "Poison Ivy";

char STR_MOVE_NAME_BLUE_BLASTER[] = "Blue Blaster";

char STR_MOVE_NAME_SCISSOR_CLAW[] = "Scissor Claw";

char STR_MOVE_NAME_SUPER_THUNDER_STRIKE[] = "Super Thunder Strike";

char STR_MOVE_NAME_SPIRAL_SWORD[] = "Spiral Sword";

char STR_MOVE_NAME_VARIABLE_DARTS[] = "Variable Darts";

char STR_MOVE_NAME_VOLCANIC_STRIKE[16] = "Volcanic Strike";

char STR_MOVE_NAME_SUBZERO_ICE_PUNCH[] = "Subzero Ice Punch";

char STR_MOVE_NAME_INFINITY_CANNON[16] = "Infinity Cannon";

char STR_MOVE_NAME_CRIMSON_FLARE[] = "Crimson Flare";

char STR_MOVE_NAME_GLACIAL_BLAST[] = "Glacial Blast";

char STR_MOVE_NAME_MAIL_STROME[12] = "Mail Strome";

char STR_MOVE_NAME_HIGH_ELECTRO_SHOCKER[] = "High Electro Shocker";

char STR_ITEM_DESC_SMALL_RECOVERY_500_HP[24] = "Small Recovery: +500 HP";

char STR_ITEM_DESC_MEDIUM_RECOVERY_1500_HP[] = "Medium Recovery: +1500 HP";

char STR_ITEM_DESC_LARGE_RECOVERY_5000_HP[] = "Large Recovery: +5000 HP";

char STR_ITEM_DESC_SUPER_RECOVERY_FULL_HP[24] = "Super Recovery: full HP";

char STR_ITEM_DESC_RECOVER_500_MAGIC_POINTS[] = "Recover +500 Magic Points";

char STR_ITEM_DESC_MED_MP_RECOVER_1500_MP[] = "Med. MP: recover +1500 MP";

char STR_ITEM_DESC_LRG_MP_RECOVER_5000_MP[] = "Lrg. MP: recover +5000 MP";

char STR_ITEM_DESC_RECOVERS_1500_MP_AND_HP[] = "Recovers +1500 MP and HP";

char STR_ITEM_DESC_CURES_STATUS_ERRORS[20] = "Cures Status Errors";

char STR_ITEM_DESC_CURES_ERRORS_REC_HP_MP[] = "Cures errors + rec. HP+MP";

char STR_ITEM_DESC_PROTECTS_YR_COND_IN_BATTLE[28] = "Protects yr cond. in battle";

char STR_ITEM_DESC_CURES_COMA_REC_HALF_HP[] = "Cures Coma + rec. half HP";

char STR_ITEM_DESC_CURES_COMA_ERRORS_FULL_HP[28] = "Cures coma, errors +full HP";

char STR_ITEM_DESC_CURES_WOUNDS_SOME_SICKNESS[] = "Cures wounds + some sickness";

char STR_ITEM_DESC_CURES_WOUNDS_SICKNESS[24] = "Cures wounds + sickness";

char STR_ITEM_DESC_BOOST_OFF_POWER_IN_BATTLE[] = "Boost Off. Power in battle";

char STR_ITEM_DESC_BOOST_DEF_POWER_IN_BATTLE[] = "Boost Def. Power in battle";

char STR_ITEM_DESC_BOOST_SPEED_IN_BATTLE[] = "Boost Speed in battle";

char STR_ITEM_DESC_BOOST_ALL_SKILLS_IN_BATTLE[] = "Boost all skills in battle";

char STR_ITEM_DESC_SUPER_BOOST_OFF_PWR_IN_BAT[] = "Super boost off. pwr in bat.";

char STR_ITEM_DESC_SUPER_BOOST_DEF_PWR_IN_BAT[] = "Super boost def. pwr in bat.";

char STR_ITEM_DESC_SUPER_BOOST_SPEED_IN_BATTLE[28] = "Super boost Speed in battle";

char STR_ITEM_DESC_CAN_RETURN_TO_CITY_QUICKLY[] = "Can return to city quickly";

char STR_ITEM_DESC_BOOST_MAX_OFF_PWR_LEVEL_50[] = "Boost max off. pwr level +50";

char STR_ITEM_DESC_BOOST_MAX_DEF_PWR_LEVEL_50[] = "Boost max def. pwr level +50";

char STR_ITEM_DESC_BOOST_MAX_BRAINS_LEVEL_50[] = "Boost max Brains level +50";

char STR_ITEM_DESC_BOOST_MAX_SPEED_LEVEL_50[] = "Boost max Speed level +50";

char STR_ITEM_DESC_BOOST_MAX_HP_LEVEL_500[24] = "Boost max HP level +500";

char STR_ITEM_DESC_BOOST_MAX_MP_LEVEL_500[24] = "Boost max MP level +500";

char STR_ITEM_DESC_BOOST_OFF_PWR_BRAINS_100[] = "Boost Off. Pwr+Brains +100";

char STR_ITEM_DESC_BOOST_DEF_PWR_SPEED_100[] = "Boost Def. Pwr+Speed +100";

char STR_ITEM_DESC_BOOST_OFF_PWR_SPEED_1000[] = "Boost Off. Pwr+Speed +1000";

char STR_ITEM_DESC_CAN_DO_POTTY_ANYWHERE[] = "Can do potty anywhere";

char STR_ITEM_DESC_TRAIN_BETTER_WITH_THIS[] = "Train better with this";

char STR_ITEM_DESC_MORE_RECOVERY_DURING_REST[] = "More recovery during rest";

char STR_ITEM_DESC_REPELS_ENEMIES_TO_STAY_AWAY[28] = "Repels enemies to stay away";

char STR_ITEM_DESC_ATTRACT_ENEMIES_TO_COME[24] = "Attract enemies to come";

char STR_ITEM_DESC_WALK_AND_HP_MP_GO_UP[] = "Walk and HP + MP go up";

char STR_ITEM_DESC_MAKES_DIGIMON_A_BIT_FULL[] = "Makes Digimon a bit full";

char STR_ITEM_DESC_MAKES_DIGIMON_QUITE_FULL[] = "Makes Digimon quite full .";

char STR_ITEM_DESC_MAKES_DIGIMON_VERY_FULL[] = "Makes Digimon very full.";

char STR_ITEM_DESC_BOOSTS_TRAINING_EFFECT[] = "Boosts training effect";

char STR_ITEM_DESC_GREATLY_REDUCES_TIREDNESS[] = "Greatly reduces Tiredness";

char STR_ITEM_DESC_MAKE_DIGIMON_A_BIT_FULL[24] = "Make Digimon a bit full";

char STR_ITEM_DESC_GREATLY_BOOSTS_DISCIPLINE[] = "Greatly boosts Discipline";

char STR_ITEM_DESC_BOOSTS_ALL_ABILITIES[] = "Boosts all abilities";

char STR_ITEM_DESC_MAKES_DIGIMON_HAPPY[20] = "Makes Digimon happy";

char STR_ITEM_DESC_GIVES_REST_BOOST_DISC_HAP[28] = "Gives rest, boost disc.+hap";

char STR_ITEM_DESC_CAN_BE_SOLD_FOR_A_HIGH_PRICE[] = "Can be sold for a high price";

char STR_ITEM_DESC_MAKES_FULL_BOOSTS_WEIGHT[28] = "Makes full + boosts Weight!";

char STR_ITEM_DESC_RECOVERS_HP_COMPLETELY[24] = "Recovers HP completely!";

char STR_ITEM_DESC_RECOVERS_MP_COMPLETELY[24] = "Recovers MP completely!";

char STR_ITEM_DESC_LOWERS_WEIGHT[] = "Lowers Weight!";

char STR_ITEM_DESC_FULLY_RECOVERS_HP_AND_MP[] = "Fully recovers HP and MP";

char STR_ITEM_DESC_BOOST_OFFENSIVE_POWER_20[] = "Boost Offensive Power +20!";

char STR_ITEM_DESC_BOOST_DEFENSIVE_POWER_20[] = "Boost Defensive Power +20!";

char STR_ITEM_DESC_BOOST_SPEED_20[] = "Boost Speed +20!";

char STR_ITEM_DESC_BOOST_BRAINS_20[] = "Boost Brains +20!";

char STR_ITEM_DESC_BOOST_HP_BY_200[] = "Boost HP by +200!";

char STR_ITEM_DESC_BOOST_MP_BY_200[] = "Boost MP by +200!";

char STR_ITEM_DESC_MAKES_DIGIMON_A_BIT_FULL_2[] = "Makes Digimon a bit full.";

char STR_ITEM_DESC_MAKES_DIGIMON_QUITE_FULL_2[] = "Makes Digimon quite full.";

char STR_ITEM_DESC_MAKES_DIGIMON_VERY_FULL_2[24] = "Makes Digimon very full";

char STR_ITEM_DESC_FULL_HP_AND_MP_LIFE_SPAN[] = "Full HP and MP + life span++";

char STR_ITEM_DESC_MAKES_DIGIMON_SOMEWHAT_FULL[28] = "Makes Digimon somewhat full";

char STR_ITEM_DESC_BOOST_HAPPINESS_BUT_RISKY[] = "Boost Happiness, but risky";

char STR_ITEM_DESC_GOOD_FOR_MANY_THINGS[] = "Good for many things";

char STR_ITEM_DESC_DIGIVOLVE_TO_GREYMON[] = "Digivolve to Greymon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_MERAMON[] = "Digivolve to Meramon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_BIRDRAMON[24] = "Digivolve to Birdramon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_CENTARUMON[] = "Digivolve to Centarumon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_MONOCHROMON[] = "Digivolve to Monochromon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_DRIMOGEMON[] = "Digivolve to Drimogemon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_TYRANNOMON[] = "Digivolve to Tyrannomon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_DEVIMON[] = "Digivolve to Devimon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_OGREMON[] = "Digivolve to Ogremon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_LEOMON[] = "Digivolve to Leomon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_ANGEMON[] = "Digivolve to Angemon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_BAKEMON[] = "Digivolve to Bakemon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_KAMINARIMON[] = "Digivolve to Kaminarimon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_AIRDRAMON[24] = "Digivolve to Airdramon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_KOKATORIMON[] = "Digivolve to Kokatorimon";

char STR_ITEM_DESC_DIGIVOLVE_TO_UNIMON[] = "Digivolve to Unimon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_KABUTERIMON[] = "Digivolve to Kabuterimon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_KUWAGAMON[24] = "Digivolve to Kuwagamon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_VEGIEMON[] = "Digivolve to Vegiemon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_NINJAMON[] = "Digivolve to Ninjamon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_SEADRAMON[24] = "Digivolve to Seadramon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_WHAMON[] = "Digivolve to Whamon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_SHELLMON[] = "Digivolve to Shellmon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_COELAMON[] = "Digivolve to Coelamon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_GARURUMON[24] = "Digivolve to Garurumon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_FRIGIMON[] = "Digivolve to Frigimon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_MOJYAMON[] = "Digivolve to Mojyamon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_NANIMON[] = "Digivolve to Nanimon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_METALGREYMON[] = "Digivolve to MetalGreymon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_SKULLGREYMON[] = "Digivolve to SkullGreymon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_ANDROMON[] = "Digivolve to Andromon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_MEGADRAMON[] = "Digivolve to Megadramon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_MAMEMON[] = "Digivolve to Mamemon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_METALMAMEMON[] = "Digivolve to MetalMamemon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_GIROMON[] = "Digivolve to Giromon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_PIXIMON[] = "Digivolve to Piximon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_MONZAEMON[24] = "Digivolve to Monzaemon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_VADEMON[] = "Digivolve to Vademon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_ETEMON[] = "Digivolve to Etemon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_DIGITAMAMON[] = "Digivolve to Digitamamon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_PHOENIXMON[] = "Digivolve to Phoenixmon!";

char STR_ITEM_DESC_BECOME_HERCULESKABUTERIMON[28] = "Become HerculesKabuterimon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_MEGASEADRAMON[28] = "Digivolve to MegaSeadramon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_WEREGARURUMON[28] = "Digivolve to WereGarurumon!";

char STR_ITEM_DESC_SEADRAMON_FRIENDSHIP_PROOF[] = "Seadramon friendship proof";

char STR_ITEM_DESC_ENABLES_YOU_TO_FISH_AT_LAKE[28] = "Enables you to fish at lake";

char STR_ITEM_DESC_GIVES_GOOD_FISHING_AT_LAKE[] = "Gives good fishing at lake";

char STR_ITEM_DESC_STONE_TABLET_OF_LEOMON[] = "Stone Tablet of Leomon";

char STR_ITEM_DESC_KEY_TO_GRAY_LORD_MANSION[] = "Key to Gray Lord Mansion";

char STR_ITEM_DESC_MYSTERY_ITEM[] = "Mystery Item";

char STR_ITEM_DESC_RECOVER_1000_MP_OTHER_USES[28] = "Recover 1000 MP +other uses";

char STR_ITEM_DESC_KEY_TO_OPEN_REFRIGERATOR[] = "Key to open Refrigerator";

char STR_ITEM_DESC_YOU_CAN_READ_ANCIENT_SCRIPT[28] = "You can read Ancient Script";

char STR_ITEM_DESC_DIGIVOLVE_TO_GIGADRAMON[] = "Digivolve to Gigadramon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_PANJYAMON[24] = "Digivolve to Panjyamon!";

char STR_ITEM_DESC_DIGIVOLVE_TO_METALETEMON[] = "Digivolve to MetalEtemon!";
#endif

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

#if defined(VERSION_JP)
char IS_SICK_SUFFIX[] = "は病気になってしまった！";
#else
char IS_SICK_SUFFIX[] = " is sick!";
#endif

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
	STR_MOVE_NAME_FIRE_TOWER,
	STR_MOVE_NAME_PROMINENCE_BEAM,
	STR_MOVE_NAME_SPIT_FIRE,
	STR_MOVE_NAME_RED_INFERNO,
	STR_MOVE_NAME_MAGMA_BOMB,
	STR_MOVE_NAME_HEAT_LASER,
	STR_MOVE_NAME_INIFINITY_BURN,
	STR_MOVE_NAME_MELTDOWN,
	STR_MOVE_NAME_THUNDER_JUSTICE,
	STR_MOVE_NAME_SPINNING_SHOT,
	STR_MOVE_NAME_ELECTRIC_CLOUD,
	STR_MOVE_NAME_MEGALO_SPARK,
	STR_MOVE_NAME_STATIC_ELECT,
	STR_MOVE_NAME_WIND_CUTTER,
	STR_MOVE_NAME_CONFUSED_STORM,
	STR_MOVE_NAME_HURRICANE,
	STR_MOVE_NAME_GIGA_FREEZE,
	STR_MOVE_NAME_ICE_STATUE,
	STR_MOVE_NAME_WINTER_BLAST,
	STR_MOVE_NAME_ICE_NEEDLE,
	STR_MOVE_NAME_WATER_BLIT,
	STR_MOVE_NAME_AQUA_MAGIC,
	STR_MOVE_NAME_AURORA_FREEZE,
	STR_MOVE_NAME_TEAR_DROP,
	STR_MOVE_NAME_POWER_CRANE,
	STR_MOVE_NAME_ALL_RANGE_BEAM,
	STR_MOVE_NAME_METAL_SPRINTER,
	STR_MOVE_NAME_PULSE_LASER,
	STR_MOVE_NAME_DELETE_PROGRAM,
	STR_MOVE_NAME_DG_DIMENSION,
	STR_MOVE_NAME_FULL_POTENTIAL,
	STR_MOVE_NAME_REVERSE_PROG,
	STR_MOVE_NAME_POISON_POWDER,
	STR_MOVE_NAME_BUG,
	STR_MOVE_NAME_MASS_MORPH,
	STR_MOVE_NAME_INSECT_PLAGUE,
	STR_MOVE_NAME_CHARM_PERFUME,
	STR_MOVE_NAME_POISON_CLAW,
	STR_MOVE_NAME_DANGER_STING,
	STR_MOVE_NAME_GREEN_TRAP,
	STR_MOVE_NAME_TREMAR,
	STR_MOVE_NAME_MUSCLE_CHARGE,
	STR_MOVE_NAME_WAR_CRY,
	STR_MOVE_NAME_SONIC_JAB,
	STR_MOVE_NAME_DYNAMITE_KICK,
	STR_MOVE_NAME_COUNTER,
	STR_MOVE_NAME_MEGATON_PUNCH,
	STR_MOVE_NAME_BUSTER_DIVE,
	STR_MOVE_NAME_DYNAMITE_KICK,
	STR_MOVE_NAME_ODOR_SPRAY,
	STR_MOVE_NAME_POOP_SPD_TOSS,
	STR_MOVE_NAME_BIG_POOP_TOSS,
	STR_MOVE_NAME_BIG_RND_TOSS,
	STR_MOVE_NAME_POOP_RND_TOSS,
	STR_MOVE_NAME_RND_SPD_TOSS,
	STR_MOVE_NAME_HORIZONTAL_KICK,
	STR_MOVE_NAME_ULT_POOP_HELL,
	STR_MOVE_NAME_HORIZONTAL_KICK,
	STR_MOVE_NAME_BLAZE_BLAST,
	STR_MOVE_NAME_PEPPER_BREATH,
	STR_MOVE_NAME_LOVELY_ATTACK,
	STR_MOVE_NAME_FIREBALL,
	STR_MOVE_NAME_DEATH_CLAW,
	STR_MOVE_NAME_MEGA_FLAME,
	STR_MOVE_NAME_HOWLING_BLASTER,
	STR_MOVE_NAME_PARTY_TIME,
	STR_MOVE_NAME_ELECTRIC_SHOCK,
	STR_MOVE_NAME_ABDUCTION_BEAM,
	STR_MOVE_NAME_SMILEY_BOMB,
	STR_MOVE_NAME_SPNNING_NEEDLE,
	STR_MOVE_NAME_SPIRAL_TWISTER,
	STR_MOVE_NAME_BOOM_BUBBLE,
	STR_MOVE_NAME_SWEET_BREATH,
	STR_MOVE_NAME_BIT_BOMB,
	STR_MOVE_NAME_DEADLY_BOMB,
	STR_MOVE_NAME_DRILL_SPIN,
	STR_MOVE_NAME_ELECTRIC_THREAD,
	STR_MOVE_NAME_ENERGY_BOMB,
	STR_MOVE_NAME_GENOSIDE_ATTACK,
	STR_MOVE_NAME_GIGA_SCISSOR_CLAW,
	STR_MOVE_NAME_DARK_SHOT,
	STR_MOVE_NAME_PUMMEL_WHACK,
	STR_MOVE_NAME_HAND_OF_FATE,
	STR_MOVE_NAME_DARK_CLAW,
	STR_MOVE_NAME_AERIAL_ATTACK,
	STR_MOVE_NAME_BONE_BOOMERANG,
	STR_MOVE_NAME_SOLAR_RAY,
	STR_MOVE_NAME_HYDRO_PRESSURE,
	STR_MOVE_NAME_ICE_BLAST,
	STR_MOVE_NAME_IGA_SCHOOL_KNIFE_THROW,
	STR_MOVE_NAME_BLASTING_SPOUT,
	STR_MOVE_NAME_FIST_OF_THE_BEAST_KING,
	STR_MOVE_NAME_DARK_NETWORK_CONCERT_CRUSH,
	STR_MOVE_NAME_ELECTRO_SHOCKER,
	STR_MOVE_NAME_METEOR_WING,
	STR_MOVE_NAME_SUPER_SLAP,
	STR_MOVE_NAME_NIGHTMARE_SYNDROMER,
	STR_MOVE_NAME_FROZEN_FIRE_SHOT,
	STR_MOVE_NAME_POISON_IVY,
	STR_MOVE_NAME_BLUE_BLASTER,
	STR_MOVE_NAME_SCISSOR_CLAW,
	STR_MOVE_NAME_SUPER_THUNDER_STRIKE,
	STR_MOVE_NAME_SPIRAL_SWORD,
	STR_MOVE_NAME_VARIABLE_DARTS,
	STR_MOVE_NAME_VOLCANIC_STRIKE,
	STR_MOVE_NAME_SUBZERO_ICE_PUNCH,
	STR_MOVE_NAME_INFINITY_CANNON,
	STR_MOVE_NAME_PARTY_TIME,
	STR_MOVE_NAME_PARTY_TIME,
	STR_MOVE_NAME_CRIMSON_FLARE,
	STR_MOVE_NAME_GLACIAL_BLAST,
	STR_MOVE_NAME_MAIL_STROME,
	STR_MOVE_NAME_HIGH_ELECTRO_SHOCKER,
	STR_MOVE_NAME_BUBBLE,
	STR_MOVE_NAME_BUBBLE,
	STR_MOVE_NAME_BUBBLE,
	STR_MOVE_NAME_BUBBLE,
	STR_MOVE_NAME_BUBBLE,
	STR_MOVE_NAME_BUBBLE,
	STR_MOVE_NAME_BUBBLE,
	STR_MOVE_NAME_BUBBLE,
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
#if defined(VERSION_JP)
	{
		0x00000000, 0x0000, 0x21, 0x01, 0x04,
		0x05, 0x00, 0x64, 0x00, 0x00,
		0x00, 0x00,
	},
#else
	{
		0x00000000, 0x0000, 0x21, 0x01, 0x04,
		0x05, 0x00, 0x64, 0x00, 0x01,
		0x00, 0x00,
	},
#endif
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

#if defined(VERSION_JP)
Item ITEM_PARA[128] = {
	{
		"回復フロッピー",
		0x00000064,
		0x0000,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"中回復フロッピー",
		0x000001f4,
		0x0000,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"大回復フロッピー",
		0x000003e8,
		0x0000,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"超回復フロッピー",
		0x000009c4,
		0x0014,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"ＭＰフロッピー",
		0x0000012c,
		0x0000,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"中ＭＰフロッピー",
		0x00000320,
		0x0000,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"大ＭＰフロッピー",
		0x000007d0,
		0x0000,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"ダブルフロッピー",
		0x000005dc,
		0x0000,
		0x0000,
		0x02,
		0x01,
		0x0000,
	},
	{
		"色々フロッピー",
		0x0000012c,
		0x0000,
		0x0001,
		0x01,
		0x01,
		0x0000,
	},
	{
		"万能フロッピー",
		0x000007d0,
		0x0000,
		0x0001,
		0x02,
		0x01,
		0x0000,
	},
	{
		"対変化フロッピー",
		0x000004b0,
		0x0000,
		0x0001,
		0x01,
		0x01,
		0x0000,
	},
	{
		"再生フロッピー",
		0x00000fa0,
		0x0000,
		0x0001,
		0x01,
		0x01,
		0x0000,
	},
	{
		"超再生フロッピー",
		0x0000251c,
		0x0064,
		0x0001,
		0x02,
		0x01,
		0x0000,
	},
	{
		"ばんそうこう",
		0x000000c8,
		0x0000,
		0x0001,
		0x00,
		0x01,
		0x0000,
	},
	{
		"薬",
		0x000003e8,
		0x0000,
		0x0001,
		0x00,
		0x01,
		0x0000,
	},
	{
		"攻撃プラグイン",
		0x000001f4,
		0x0000,
		0x0003,
		0x01,
		0x01,
		0x0000,
	},
	{
		"防御プラグイン",
		0x000001f4,
		0x0000,
		0x0003,
		0x01,
		0x01,
		0x0000,
	},
	{
		"高速プラグイン",
		0x000001f4,
		0x0000,
		0x0003,
		0x01,
		0x01,
		0x0000,
	},
	{
		"万能プラグイン",
		0x00000bb8,
		0x0000,
		0x0003,
		0x01,
		0x01,
		0x0000,
	},
	{
		"攻撃プラグインＳ",
		0x00000fa0,
		0x0000,
		0x0003,
		0x01,
		0x01,
		0x0000,
	},
	{
		"防御プラグインＳ",
		0x00000fa0,
		0x0000,
		0x0003,
		0x01,
		0x01,
		0x0000,
	},
	{
		"高速プラグインＳ",
		0x00000fa0,
		0x0000,
		0x0003,
		0x01,
		0x01,
		0x0000,
	},
	{
		"オートパイロット",
		0x0000012c,
		0x0000,
		0x0005,
		0x00,
		0x01,
		0x0000,
	},
	{
		"攻撃チップ",
		0x0000270f,
		0x0320,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"防御チップ",
		0x0000270f,
		0x0320,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"かしこさチップ",
		0x0000270f,
		0x0320,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"すばやさチップ",
		0x0000270f,
		0x0320,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"ＨＰチップ",
		0x0000270f,
		0x0320,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"ＭＰチップ",
		0x0000270f,
		0x0320,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"デビルチップＡ",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"デビルチップＤ",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"デビルチップＥ",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"けいたいトイレ",
		0x0000012c,
		0x0000,
		0x0005,
		0x00,
		0x01,
		0x0000,
	},
	{
		"トレーマニュアル",
		0x0000270f,
		0x0000,
		0x0005,
		0x00,
		0x01,
		0x0000,
	},
	{
		"安眠まくら",
		0x000003e8,
		0x0000,
		0x0005,
		0x00,
		0x01,
		0x0000,
	},
	{
		"敵よけお守り",
		0x00001388,
		0x0000,
		0x0005,
		0x00,
		0x01,
		0x0000,
	},
	{
		"敵よせシグナル",
		0x00001388,
		0x0000,
		0x0005,
		0x00,
		0x01,
		0x0000,
	},
	{
		"健康サンダル",
		0x000007d0,
		0x0000,
		0x0005,
		0x00,
		0x01,
		0x0000,
	},
	{
		"肉",
		0x00000032,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"巨大肉",
		0x000001f4,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"極上肉",
		0x000005dc,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"挑戦ニンジン",
		0x000001f4,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"サクラ鳥大根",
		0x000001f4,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"ペンペンペン草",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"デジタケ",
		0x00000064,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"雪割りキノコ",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"デラックスキノコ",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"デジぼっくり",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"ブルーリンゴ",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"アセラロ",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"黄金ドングリ",
		0x00001f40,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"ヘビーいちご",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"天然あまぐり",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"モンドレイク",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"食用サボテン",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"オレンジバナナ",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"ごうりき草の実",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"じょうぶ草の実",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"はやあし草の実",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"ものしり草の実",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"もりもり草の実",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"やすらぎ草の実",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"デジジャコ",
		0x00000032,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"デジブナ",
		0x00000064,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"デジマス",
		0x0000012c,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"ブラックデジマス",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"デジナマズ",
		0x000007d0,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"デジカムル",
		0x00001f40,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"くさった肉",
		0x00000032,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"運だめしキノコ",
		0x000003e8,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"クサリカケメロン",
		0x00001388,
		0x0000,
		0x0002,
		0x00,
		0x01,
		0x0000,
	},
	{
		"灰色の爪",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"炎のかけら",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"燃える羽",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"鉄のヒヅメ",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"モノクロストーン",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"鋼のドリル",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"牙の化石",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"黒い羽",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"トゲこん棒",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"輝くたてがみ",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"白い羽",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"ボロ布",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"電気リング",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"虹色の笛",
		0x00001388,
		0x01f4,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"風見鶏",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"聖なる角",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"重い兜",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"ギザギザはさみ",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"有機肥料",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"イガ流の秘伝書",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"お供え酒",
		0x00001388,
		0x01f4,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"北極星の印",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"赤い貝がら",
		0x00001388,
		0x01f4,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"硬いウロコ",
		0x00001388,
		0x01f4,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"ミスリル",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"氷水晶",
		0x00001388,
		0x01f4,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"けはえ薬",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"サングラス",
		0x00001388,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"メタルパーツ",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"死を呼ぶ骨",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"サイバーパーツ",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"メガハンド",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"銀玉",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"メタルアーマー",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"チェンソー",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"小さな槍",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"愛のばんそうこう",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"ビームガン",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"金のバナナ",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"謎のタマゴ",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"奇跡のルビー",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"ビートルダイヤ",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"サンゴのお守り",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"月光の鏡",
		0x0000270f,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"湖のぬしの笛",
		0x0000270f,
		0x0000,
		0x0005,
		0x00,
		0x00,
		0x0000,
	},
	{
		"ぼろい釣ざお",
		0x00000064,
		0x0000,
		0x0005,
		0x00,
		0x00,
		0x0000,
	},
	{
		"すごい釣ざお",
		0x00000bb8,
		0x012c,
		0x0005,
		0x00,
		0x00,
		0x0000,
	},
	{
		"レオモン族の石板",
		0x00000064,
		0x0000,
		0x0005,
		0x00,
		0x00,
		0x0000,
	},
	{
		"館のカギ",
		0x00000064,
		0x0000,
		0x0005,
		0x00,
		0x00,
		0x0000,
	},
	{
		"はぐるま",
		0x00000064,
		0x0000,
		0x0005,
		0x00,
		0x00,
		0x0000,
	},
	{
		"雨ふり草の実",
		0x000003e8,
		0x0000,
		0x0005,
		0x02,
		0x01,
		0x0000,
	},
	{
		"血のしたたる肉",
		0x000003e8,
		0x0000,
		0x0000,
		0x00,
		0x01,
		0x0000,
	},
	{
		"れいぞうこのカギ",
		0x00000064,
		0x0000,
		0x0005,
		0xff,
		0x00,
		0x0000,
	},
	{
		"古代文字解読機",
		0x000001f4,
		0x0000,
		0x0005,
		0xff,
		0x00,
		0x0000,
	},
	{
		"ギガハンド",
		0x00000000,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"高貴なたてがみ",
		0x00000000,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
	{
		"メタルバナナ",
		0x00000000,
		0x0000,
		0x0004,
		0x00,
		0x01,
		0x0000,
	},
};
#else
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
#endif

#if defined(VERSION_JP)
char *ITEM_DESC_PTR[128] = {
	STR_ITEM_DESC_SMALL_RECOVERY_500_HP,
	STR_ITEM_DESC_MEDIUM_RECOVERY_1500_HP,
	STR_ITEM_DESC_LARGE_RECOVERY_5000_HP,
	STR_ITEM_DESC_SUPER_RECOVERY_FULL_HP,
	STR_ITEM_DESC_RECOVER_500_MAGIC_POINTS,
	STR_ITEM_DESC_MED_MP_RECOVER_1500_MP,
	STR_ITEM_DESC_LRG_MP_RECOVER_5000_MP,
	STR_ITEM_DESC_RECOVERS_1500_MP_AND_HP,
	STR_ITEM_DESC_CURES_STATUS_ERRORS,
	STR_ITEM_DESC_CURES_ERRORS_REC_HP_MP,
	STR_ITEM_DESC_PROTECTS_YR_COND_IN_BATTLE,
	STR_ITEM_DESC_CURES_COMA_REC_HALF_HP,
	STR_ITEM_DESC_CURES_COMA_ERRORS_FULL_HP,
	STR_ITEM_DESC_CURES_WOUNDS_SOME_SICKNESS,
	STR_ITEM_DESC_CURES_WOUNDS_SICKNESS,
	STR_ITEM_DESC_BOOST_OFF_POWER_IN_BATTLE,
	STR_ITEM_DESC_BOOST_DEF_POWER_IN_BATTLE,
	STR_ITEM_DESC_BOOST_SPEED_IN_BATTLE,
	STR_ITEM_DESC_BOOST_ALL_SKILLS_IN_BATTLE,
	STR_ITEM_DESC_SUPER_BOOST_OFF_PWR_IN_BAT,
	STR_ITEM_DESC_SUPER_BOOST_DEF_PWR_IN_BAT,
	STR_ITEM_DESC_SUPER_BOOST_SPEED_IN_BATTLE,
	STR_ITEM_DESC_CAN_RETURN_TO_CITY_QUICKLY,
	STR_ITEM_DESC_BOOST_MAX_OFF_PWR_LEVEL_50,
	STR_ITEM_DESC_BOOST_MAX_DEF_PWR_LEVEL_50,
	STR_ITEM_DESC_BOOST_MAX_BRAINS_LEVEL_50,
	STR_ITEM_DESC_BOOST_MAX_SPEED_LEVEL_50,
	STR_ITEM_DESC_BOOST_MAX_HP_LEVEL_500,
	STR_ITEM_DESC_BOOST_MAX_MP_LEVEL_500,
	STR_ITEM_DESC_BOOST_OFF_PWR_BRAINS_100,
	STR_ITEM_DESC_BOOST_DEF_PWR_SPEED_100,
	STR_ITEM_DESC_BOOST_OFF_PWR_SPEED_1000,
	STR_ITEM_DESC_CAN_DO_POTTY_ANYWHERE,
	STR_ITEM_DESC_TRAIN_BETTER_WITH_THIS,
	STR_ITEM_DESC_MORE_RECOVERY_DURING_REST,
	STR_ITEM_DESC_REPELS_ENEMIES_TO_STAY_AWAY,
	STR_ITEM_DESC_ATTRACT_ENEMIES_TO_COME,
	STR_ITEM_DESC_WALK_AND_HP_MP_GO_UP,
	STR_ITEM_DESC_MAKES_DIGIMON_A_BIT_FULL,
	STR_ITEM_DESC_MAKES_DIGIMON_QUITE_FULL,
	STR_ITEM_DESC_MAKES_DIGIMON_VERY_FULL,
	STR_ITEM_DESC_BOOSTS_TRAINING_EFFECT,
	STR_ITEM_DESC_BOOSTS_TRAINING_EFFECT,
	STR_ITEM_DESC_GREATLY_REDUCES_TIREDNESS,
	STR_ITEM_DESC_MAKE_DIGIMON_A_BIT_FULL,
	STR_ITEM_DESC_GREATLY_BOOSTS_DISCIPLINE,
	STR_ITEM_DESC_BOOSTS_ALL_ABILITIES,
	STR_ITEM_DESC_BOOSTS_TRAINING_EFFECT_2,
	STR_ITEM_DESC_MAKES_DIGIMON_HAPPY,
	STR_ITEM_DESC_GIVES_REST_BOOST_DISC_HAP,
	STR_ITEM_DESC_CAN_BE_SOLD_FOR_A_HIGH_PRICE,
	STR_ITEM_DESC_MAKES_FULL_BOOSTS_WEIGHT,
	STR_ITEM_DESC_RECOVERS_HP_COMPLETELY,
	STR_ITEM_DESC_RECOVERS_MP_COMPLETELY,
	STR_ITEM_DESC_LOWERS_WEIGHT,
	STR_ITEM_DESC_FULLY_RECOVERS_HP_AND_MP,
	STR_ITEM_DESC_BOOST_OFFENSIVE_POWER_20,
	STR_ITEM_DESC_BOOST_DEFENSIVE_POWER_20,
	STR_ITEM_DESC_BOOST_SPEED_20,
	STR_ITEM_DESC_BOOST_BRAINS_20,
	STR_ITEM_DESC_BOOST_HP_BY_200,
	STR_ITEM_DESC_BOOST_MP_BY_200,
	STR_ITEM_DESC_MAKES_DIGIMON_A_BIT_FULL_2,
	STR_ITEM_DESC_MAKES_DIGIMON_A_BIT_FULL,
	STR_ITEM_DESC_MAKES_DIGIMON_QUITE_FULL,
	STR_ITEM_DESC_BOOSTS_ALL_ABILITIES,
	STR_ITEM_DESC_MAKES_DIGIMON_VERY_FULL,
	STR_ITEM_DESC_FULL_HP_AND_MP_LIFE_SPAN,
	STR_ITEM_DESC_MAKES_DIGIMON_SOMEWHAT_FULL,
	STR_ITEM_DESC_BOOST_HAPPINESS_BUT_RISKY,
	STR_ITEM_DESC_GOOD_FOR_MANY_THINGS,
	STR_ITEM_DESC_DIGIVOLVE_TO_GREYMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_MERAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_BIRDRAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_CENTARUMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_MONOCHROMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_DRIMOGEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_TYRANNOMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_DEVIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_OGREMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_LEOMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_ANGEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_BAKEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_KAMINARIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_AIRDRAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_KOKATORIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_UNIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_KABUTERIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_KUWAGAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_VEGIEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_NINJAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_SEADRAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_WHAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_SHELLMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_COELAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_GARURUMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_FRIGIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_MOJYAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_NANIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_METALGREYMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_SKULLGREYMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_ANDROMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_MEGADRAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_MAMEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_METALMAMEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_GIROMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_PIXIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_MONZAEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_VADEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_ETEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_DIGITAMAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_PHOENIXMON,
	STR_ITEM_DESC_BECOME_HERCULESKABUTERIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_MEGASEADRAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_WEREGARURUMON,
	STR_ITEM_DESC_SEADRAMON_FRIENDSHIP_PROOF,
	STR_ITEM_DESC_ENABLES_YOU_TO_FISH_AT_LAKE,
	STR_ITEM_DESC_GIVES_GOOD_FISHING_AT_LAKE,
	STR_ITEM_DESC_STONE_TABLET_OF_LEOMON,
	STR_ITEM_DESC_KEY_TO_GRAY_LORD_MANSION,
	STR_ITEM_DESC_MYSTERY_ITEM,
	STR_ITEM_DESC_RECOVER_1000_MP_OTHER_USES,
	STR_ITEM_DESC_MAKES_DIGIMON_QUITE_FULL,
	STR_ITEM_DESC_KEY_TO_OPEN_REFRIGERATOR,
	STR_ITEM_DESC_YOU_CAN_READ_ANCIENT_SCRIPT,
	STR_ITEM_DESC_DIGIVOLVE_TO_GIGADRAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_PANJYAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_METALETEMON,
};
#else
char *ITEM_DESC_PTR[128] = {
	STR_ITEM_DESC_SMALL_RECOVERY_500_HP,
	STR_ITEM_DESC_MEDIUM_RECOVERY_1500_HP,
	STR_ITEM_DESC_LARGE_RECOVERY_5000_HP,
	STR_ITEM_DESC_SUPER_RECOVERY_FULL_HP,
	STR_ITEM_DESC_RECOVER_500_MAGIC_POINTS,
	STR_ITEM_DESC_MED_MP_RECOVER_1500_MP,
	STR_ITEM_DESC_LRG_MP_RECOVER_5000_MP,
	STR_ITEM_DESC_RECOVERS_1500_MP_AND_HP,
	STR_ITEM_DESC_CURES_STATUS_ERRORS,
	STR_ITEM_DESC_CURES_ERRORS_REC_HP_MP,
	STR_ITEM_DESC_PROTECTS_YR_COND_IN_BATTLE,
	STR_ITEM_DESC_CURES_COMA_REC_HALF_HP,
	STR_ITEM_DESC_CURES_COMA_ERRORS_FULL_HP,
	STR_ITEM_DESC_CURES_WOUNDS_SOME_SICKNESS,
	STR_ITEM_DESC_CURES_WOUNDS_SICKNESS,
	STR_ITEM_DESC_BOOST_OFF_POWER_IN_BATTLE,
	STR_ITEM_DESC_BOOST_DEF_POWER_IN_BATTLE,
	STR_ITEM_DESC_BOOST_SPEED_IN_BATTLE,
	STR_ITEM_DESC_BOOST_ALL_SKILLS_IN_BATTLE,
	STR_ITEM_DESC_SUPER_BOOST_OFF_PWR_IN_BAT,
	STR_ITEM_DESC_SUPER_BOOST_DEF_PWR_IN_BAT,
	STR_ITEM_DESC_SUPER_BOOST_SPEED_IN_BATTLE,
	STR_ITEM_DESC_CAN_RETURN_TO_CITY_QUICKLY,
	STR_ITEM_DESC_BOOST_MAX_OFF_PWR_LEVEL_50,
	STR_ITEM_DESC_BOOST_MAX_DEF_PWR_LEVEL_50,
	STR_ITEM_DESC_BOOST_MAX_BRAINS_LEVEL_50,
	STR_ITEM_DESC_BOOST_MAX_SPEED_LEVEL_50,
	STR_ITEM_DESC_BOOST_MAX_HP_LEVEL_500,
	STR_ITEM_DESC_BOOST_MAX_MP_LEVEL_500,
	STR_ITEM_DESC_BOOST_OFF_PWR_BRAINS_100,
	STR_ITEM_DESC_BOOST_DEF_PWR_SPEED_100,
	STR_ITEM_DESC_BOOST_OFF_PWR_SPEED_1000,
	STR_ITEM_DESC_CAN_DO_POTTY_ANYWHERE,
	STR_ITEM_DESC_TRAIN_BETTER_WITH_THIS,
	STR_ITEM_DESC_MORE_RECOVERY_DURING_REST,
	STR_ITEM_DESC_REPELS_ENEMIES_TO_STAY_AWAY,
	STR_ITEM_DESC_ATTRACT_ENEMIES_TO_COME,
	STR_ITEM_DESC_WALK_AND_HP_MP_GO_UP,
	STR_ITEM_DESC_MAKES_DIGIMON_A_BIT_FULL,
	STR_ITEM_DESC_MAKES_DIGIMON_QUITE_FULL,
	STR_ITEM_DESC_MAKES_DIGIMON_VERY_FULL,
	STR_ITEM_DESC_BOOSTS_TRAINING_EFFECT,
	STR_ITEM_DESC_BOOSTS_TRAINING_EFFECT,
	STR_ITEM_DESC_GREATLY_REDUCES_TIREDNESS,
	STR_ITEM_DESC_MAKE_DIGIMON_A_BIT_FULL,
	STR_ITEM_DESC_GREATLY_BOOSTS_DISCIPLINE,
	STR_ITEM_DESC_BOOSTS_ALL_ABILITIES,
	STR_ITEM_DESC_BOOSTS_TRAINING_EFFECT,
	STR_ITEM_DESC_MAKES_DIGIMON_HAPPY,
	STR_ITEM_DESC_GIVES_REST_BOOST_DISC_HAP,
	STR_ITEM_DESC_CAN_BE_SOLD_FOR_A_HIGH_PRICE,
	STR_ITEM_DESC_MAKES_FULL_BOOSTS_WEIGHT,
	STR_ITEM_DESC_RECOVERS_HP_COMPLETELY,
	STR_ITEM_DESC_RECOVERS_MP_COMPLETELY,
	STR_ITEM_DESC_LOWERS_WEIGHT,
	STR_ITEM_DESC_FULLY_RECOVERS_HP_AND_MP,
	STR_ITEM_DESC_BOOST_OFFENSIVE_POWER_20,
	STR_ITEM_DESC_BOOST_DEFENSIVE_POWER_20,
	STR_ITEM_DESC_BOOST_SPEED_20,
	STR_ITEM_DESC_BOOST_BRAINS_20,
	STR_ITEM_DESC_BOOST_HP_BY_200,
	STR_ITEM_DESC_BOOST_MP_BY_200,
	STR_ITEM_DESC_MAKES_DIGIMON_A_BIT_FULL_2,
	STR_ITEM_DESC_MAKES_DIGIMON_QUITE_FULL_2,
	STR_ITEM_DESC_MAKES_DIGIMON_VERY_FULL,
	STR_ITEM_DESC_BOOSTS_ALL_ABILITIES,
	STR_ITEM_DESC_MAKES_DIGIMON_VERY_FULL_2,
	STR_ITEM_DESC_FULL_HP_AND_MP_LIFE_SPAN,
	STR_ITEM_DESC_MAKES_DIGIMON_SOMEWHAT_FULL,
	STR_ITEM_DESC_BOOST_HAPPINESS_BUT_RISKY,
	STR_ITEM_DESC_GOOD_FOR_MANY_THINGS,
	STR_ITEM_DESC_DIGIVOLVE_TO_GREYMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_MERAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_BIRDRAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_CENTARUMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_MONOCHROMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_DRIMOGEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_TYRANNOMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_DEVIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_OGREMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_LEOMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_ANGEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_BAKEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_KAMINARIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_AIRDRAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_KOKATORIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_UNIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_KABUTERIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_KUWAGAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_VEGIEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_NINJAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_SEADRAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_WHAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_SHELLMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_COELAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_GARURUMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_FRIGIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_MOJYAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_NANIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_METALGREYMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_SKULLGREYMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_ANDROMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_MEGADRAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_MAMEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_METALMAMEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_GIROMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_PIXIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_MONZAEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_VADEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_ETEMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_DIGITAMAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_PHOENIXMON,
	STR_ITEM_DESC_BECOME_HERCULESKABUTERIMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_MEGASEADRAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_WEREGARURUMON,
	STR_ITEM_DESC_SEADRAMON_FRIENDSHIP_PROOF,
	STR_ITEM_DESC_ENABLES_YOU_TO_FISH_AT_LAKE,
	STR_ITEM_DESC_GIVES_GOOD_FISHING_AT_LAKE,
	STR_ITEM_DESC_STONE_TABLET_OF_LEOMON,
	STR_ITEM_DESC_KEY_TO_GRAY_LORD_MANSION,
	STR_ITEM_DESC_MYSTERY_ITEM,
	STR_ITEM_DESC_RECOVER_1000_MP_OTHER_USES,
	STR_ITEM_DESC_MAKES_DIGIMON_VERY_FULL_2,
	STR_ITEM_DESC_KEY_TO_OPEN_REFRIGERATOR,
	STR_ITEM_DESC_YOU_CAN_READ_ANCIENT_SCRIPT,
	STR_ITEM_DESC_DIGIVOLVE_TO_GIGADRAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_PANJYAMON,
	STR_ITEM_DESC_DIGIVOLVE_TO_METALETEMON,
};
#endif

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

GARBAGE(addGameMenu, 14);

void addGameMenu(void)
{
	GAME_MENU_SPRITES[0].unknown = 1;
	MAIN_D_80134D28 = 7;
	GAME_MENU_SPRITES[7].disabled = 0;
	MAIN_D_80134D2C = getFishingAvailability();
	if (MAIN_D_80134D2C != 0) {
		MAIN_D_80134D28++;
		if (MAIN_D_80134D2C == 1) {
			GAME_MENU_SPRITES[7].disabled = 1;
		}
		GAME_MENU_SPRITES[0].unknown = 7;
	}
	if (PARTNER_PARA.condition & 1) {
		GAME_MENU_SPRITES[6].disabled = 0;
	} else {
		GAME_MENU_SPRITES[6].disabled = 1;
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
			addInventoryUI();
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

void closeUIBoxIfOpen(id)
	int16_t id;
{
	RECT rect;
	int16_t pos[2];

	if (UI_BOX_DATA[id].frame >= 5 && UI_BOX_DATA[id].state == 1) {
		getEntityScreenPos(ENTITY_TABLE[0], 0, pos);
		setRECT(&rect, pos[0] - 5, pos[1] - 5, 10, 10);
		removeAnimatedUIBox(id, 0);
	}
}

void renderGameMenu(void)
{
	GameMenuSprite *sprite;
	int32_t digitCount;
	int32_t digits[6];
	int8_t disabled;
	int8_t highlight;
	int16_t yOffset;
	int32_t i;

	renderSeparatorLines(GAME_MENU_LINES, 2, 5);
	renderDateDigits();
	yOffset = 0;
	if (MAIN_D_80134D28 == 7) {
		yOffset = -0x28;
	}
	sprite = &GAME_MENU_SPRITES[1];
	renderTriangleCursor((int8_t)GAME_MENU_SPRITES[0].unknown, yOffset);
	for (i = 1; i < MAIN_D_80134D28; sprite++, i++) {
		disabled = 0;
		if (sprite->disabled == 1) {
			disabled = 1;
		}
		highlight = 0;
		if ((i == GAME_MENU_SPRITES[0].unknown) &&
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
#if defined(VERSION_JP)
	renderString(3, 7, -0x49, 0xc, 0xc, 0, 0xe8, 6, 1);
	renderString(3, 0x2c, -0x49, 0xc, 0xc, 0xc, 0xe8, 6, 1);
#else
	renderString(3, -0x3c, -0x49, 0x24, 0xc, 0, 0xe7, 6, 1);
	renderString(3, 6, -0x49, 0x1c, 0xc, 0x24, 0xe7, 6, 1);
#endif
	convertValueToDigits(3, DAY, &digitCount, digits);
}

#if defined(VERSION_JP)
void renderDateDigits(void)
{
	int32_t digitCount;
	int32_t digits[6];
	int32_t i;
	int16_t texX;
	int16_t texY;

	convertValueToDigits(2, DAY + 1, &digitCount, digits);
	for (i = digitCount - 1; i >= 0; i--) {
		texY = 0x34;
		if (digits[i] == 0) {
			texX = 0x78;
		} else if (digits[i] < 5) {
			texX = (digits[i] - 1) * 8 + 0x60;
			texY = 0x28;
		} else {
			texX = (digits[i] - 5) * 8 + 0x50;
		}
		renderRectPolyFT4((2 - i) * 9 + 0xf, -0x4a, 8, 0xc, texX,
		                  texY + 0xc0, 0x1e, 0x7f10, 5, 0);
	}

	convertValueToDigits(3, YEAR + 1, &digitCount, digits);
	for (i = digitCount - 1; i >= 0; i--) {
		texY = 0x34;
		if (digits[i] == 0) {
			texX = 0x78;
		} else if (digits[i] < 5) {
			texX = (digits[i] - 1) * 8 + 0x60;
			texY = 0x28;
		} else {
			texX = (digits[i] - 5) * 8 + 0x50;
		}
		renderRectPolyFT4((2 - i) * 9 - 0x16, -0x4a, 8, 0xc, texX,
		                  texY + 0xc0, 0x1e, 0x7f10, 5, 0);
	}
}
#else
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
#endif

void renderTriangleCursor(int8_t selection, int16_t yOffset)
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
	GsOT_TAG *tags;
	int16_t baseX;
	int16_t baseY;
	int32_t i;

	u0 = MAIN_D_80134250;
	u1 = MAIN_D_80134258;
	v0 = MAIN_D_80134260;
	v1 = MAIN_D_80134268;
	xOffset = MAIN_D_80134270;
	yOffsetData = MAIN_D_80134278;
	width = MAIN_D_80134280;
	height = MAIN_D_80134288;
	baseX = GAME_MENU_SPRITES[selection].x -
	        0x46;
	baseY = GAME_MENU_SPRITES[selection].y -
	        0x53;
	tags = ACTIVE_ORDERING_TABLE->org;
	for (i = 0; i < 8; i++) {
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		setUV4(prim, (uint8_t)u0.data[i], (uint8_t)v0.data[i], (uint8_t)u1.data[i], (uint8_t)v0.data[i], (uint8_t)u0.data[i], (uint8_t)v1.data[i], (uint8_t)u1.data[i], (uint8_t)v1.data[i]);
		setPosDataPolyFT4(prim, baseX + xOffset.data[i],
		                  yOffset + (baseY + yOffsetData.data[i]),
		                  width.data[i], height.data[i]);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = GetTPage(0, 0, 0x380, 0x1c0);
		prim->clut = GetClut(0x100, 0x1fc);
		AddPrim(&tags[5], prim);
		prim++;
		GsSetWorkBase((PACKET *)prim);
	}
}

void tickGameMenu(void)
{
	int32_t selection;

	if (PARTNER_PARA.condition & 1) {
		GAME_MENU_SPRITES[6].disabled = 0;
	}
	selection = GAME_MENU_SPRITES[0].unknown;
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x1000) {
		if ((selection -= 3) <= 0) {
			selection += ((MAIN_D_80134D28 + 1) / 3) * 3;
		}
		if (selection >= MAIN_D_80134D28) {
			selection -= 3;
		}
	} else if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x4000) {
		if ((selection += 3) >= MAIN_D_80134D28) {
			selection -= ((MAIN_D_80134D28 + 1) / 3) * 3;
		}
		if (selection <= 0) {
			selection += 3;
		}
	} else if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x8000) {
		if ((selection -= 1) <= 0) {
			selection = MAIN_D_80134D28 - 1;
		}
	} else if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x2000) {
		if ((selection += 1) >= MAIN_D_80134D28) {
			selection = 1;
		}
	}
	if (selection != GAME_MENU_SPRITES[0].unknown) {
		GAME_MENU_SPRITES[0].unknown = selection;
		playSound(0, 2);
	}
	if (TRIANGLE_MENU_STATE == -1) {
		if (isKeyDown(CONFIRM_BUTTON) != 0) {
			if (GAME_MENU_SPRITES[GAME_MENU_SPRITES[0].unknown].disabled & 1) {
				playSound(0, 4);
			} else {
				playSound(0, 3);
			}
			handleGameMenuSelection(GAME_MENU_SPRITES[0].unknown);
		}
		if ((isKeyDown(CANCEL_BUTTON) != 0) &&
		    ((UI_BOX_DATA[0].state == 1) ||
		     (UI_BOX_DATA[0].frame == 0))) {
			playSound(0, 4);
			closeTriangleMenu();
			tamerSetState(0);
			setCameraFollowPlayer();
			IS_IN_MENU = 0;
			startGameTime();
		}
	}
}

int32_t createMenuBox(int16_t id, int16_t x, int16_t y, int16_t width,
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
		if (isKeyDown(CANCEL_BUTTON) != 0) {
			if (MENU_STATE == 1) {
				TRIANGLE_MENU_STATE = 4;
			}
			playSound(0, 4);
		}
		if ((MAIN_D_80134D36 == 1) && (isKeyDown(CONFIRM_BUTTON) != 0)) {
			if (MENU_STATE == 1) {
				MENU_STATE = 2;
			}
			playSound(0, 3);
		}
	} else if (MENU_STATE == 6) {
		if (isKeyDown(CANCEL_BUTTON) != 0) {
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
		} else if (isKeyDown(CONFIRM_BUTTON) != 0) {
			if ((equippedSlot = getEquippedSlot()) != -1) {
				EQUIPPED_MOVES[equippedSlot] = 0xff;
				PARTNER_ENTITY.digimonEntity.stats.base
					.moves[equippedSlot] = 0xff;
				playSound(0, 3);
			} else {
				equipMove();
			}
		}
		previousX = MAIN_D_80134D3A;
		previousY = MAIN_D_80134D38;
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
	} else if ((MENU_STATE == 8) && (isKeyDown(CANCEL_BUTTON) != 0)) {
		MENU_STATE = 9;
		MENU_SUB_STATE = 0;
		playSound(0, 4);
	}
	TAMER_ENTITY.entity.isOnScreen = 0;
	PARTNER_ENTITY.digimonEntity.entity.isOnScreen = 0;
}

void renderDigimonMenu(void)
{
	DigimonTabs tabs;

	tabs = MAIN_D_801342A0;
	switch (MAIN_D_80134D36) {
	case 0:
		renderDigimonStatsView();
		break;
	case 1:
		renderDigimonMovesView();
		break;
	}
	tabs.tab[MAIN_D_80134D36] = 0;
	renderString(tabs.tab[0], -0x8a, -0x65, 0x3c, 0xc, 0, 0, 5, 1);
	renderString(tabs.tab[1], -0x3f, -0x65, 0x30, 0xc, 0x3c, 0, 5, 1);
	renderMenuTab(-0x91, 0x4c, tabs.tab[0]);
	renderMenuTab(-0x46, 0x40, tabs.tab[1]);
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
	int16_t cardX;
	uint8_t maxColumn;
	int16_t selectorX;
	uint8_t previousCard;
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
		if (isKeyDown(CANCEL_BUTTON) != 0) {
			TRIANGLE_MENU_STATE = 6;
			playSound(0, 4);
		}
		if ((isKeyDown(CONFIRM_BUTTON) != 0) && (MENU_STATE == 1) &&
		    (MAIN_D_80134D37 != 0) && (MAIN_D_80134D37 != 4)) {
			MENU_STATE = 2;
			playSound(0, 3);
			MEDAL_SELECTOR_INDEX = SELECTED_MEDAL = 0;
			SELECTED_CARD = 0;
		}
	} else {
		if ((isKeyDown(CANCEL_BUTTON) != 0) && (MENU_STATE == 2)) {
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
				if (isKeyDown(CONFIRM_BUTTON) != 0) {
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
			           (isKeyDown(CANCEL_BUTTON) != 0)) {
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

#if defined(VERSION_JP)
		if (MAIN_D_80134D37 == 2 && MENU_STATE == 2) {
#else
		if (MAIN_D_80134D37 == 2) {
#endif
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
			previousCard = SELECTED_CARD;
			if (MENU_STATE == 2) {
				if ((isKeyDown(CONFIRM_BUTTON) != 0) &&
				    (getCardAmount((uint8_t)SELECTED_CARD) != 0)) {
					playSound(0, 3);
					MENU_STATE = 3;
					loadCardImage(SELECTED_CARD);
					cardX = (SELECTED_CARD % 11) < 6 ? 0xa6 : -0xcc;
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
			if ((isKeyDown(CANCEL_BUTTON) != 0) && (MENU_STATE == 3)) {
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

void renderPlayerMenu(void)
{
	PlayerTabs tabs;

	tabs = MAIN_D_801342A4;
	switch (MAIN_D_80134D37) {
	case 0:
		renderPlayerInfoView();
		break;
	case 1:
		renderEvoChartView();
		break;
	case 2:
		renderMedalView();
		break;
	case 3:
		renderCardsView();
		break;
	}
	tabs.tab[MAIN_D_80134D37] = 0;
	renderString(tabs.tab[0], -0x89, -0x65, 0x3c, 0xc, 0, 0, 5, 1);
	renderString(tabs.tab[1], -0x3e, -0x65, 0x3c, 0xc, 0x3c, 0, 5, 1);
	renderString(tabs.tab[2], 0xd, -0x65, 0x24, 0xc, 0x78, 0, 5, 1);
	renderString(tabs.tab[3], 0x40, -0x65, 0x24, 0xc, 0x9c, 0, 5, 1);
	renderMenuTab(-0x91, 0x4c, tabs.tab[0]);
	renderMenuTab(-0x46, 0x4c, tabs.tab[1]);
	renderMenuTab(5, 0x34, tabs.tab[2]);
	renderMenuTab(0x38, 0x34, tabs.tab[3]);
}

void handleGameMenuSelection(int32_t selection)
{
	switch (selection) {
	case 1:
		if (GAME_MENU_SPRITES[selection].disabled & 1) {
			return;
		}
		TRIANGLE_MENU_STATE = 2;
		drawInventoryText();
		break;
	case 6:
		if (GAME_MENU_SPRITES[selection].disabled & 1) {
			return;
		}
		closeTriangleMenu();
		partnerSetState(3);
		IS_IN_MENU = 0;
		startGameTime();
		break;
	case 5:
		if (GAME_MENU_SPRITES[selection].disabled & 1) {
			return;
		}
		closeTriangleMenu();
		partnerSetState(4);
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
		if (GAME_MENU_SPRITES[selection].disabled & 1) {
			return;
		}
		closeTriangleMenu();
		partnerSetState(0xf);
		IS_IN_MENU = 0;
		startGameTime();
		break;
	case 7:
		if (GAME_MENU_SPRITES[selection].disabled & 1) {
			return;
		}
		closeTriangleMenu();
		setCameraFollowPlayer();
		initializeFishing();
		tamerSetState(0xb);
		IS_IN_MENU = 0;
	}
}

void setSleepDisabled(int16_t arg)
{
	GAME_MENU_SPRITES[6].disabled = arg;
}

void startFeedingItem(uint8_t arg)
{
	if (TAMER_ITEM.worldItem.type == 0xff) {
		TAMER_ITEM.worldItem.type = arg;
		tamerSetState(6);
		partnerSetState(5);
		removeObject(0xfa4, 0);
		IS_IN_MENU = 0;
		startGameTime();
	}
}

void removeOneSelectedItem(void)
{
	removeItem(INVENTORY.types.array[INVENTORY_POINTER], 1);
}

void renderFeedingItem(int16_t arg)
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

int8_t getEquippedSlot(void)
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
			return slot;
		}
		if ((moveId == 0x37) && (EQUIPPED_MOVES[slot] == 0x39)) {
			return slot;
		}
		if (EQUIPPED_MOVES[slot] == moveId) {
			return slot;
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
	setRECT(&textArea, 0, slot * 12 + 0x18, 0x84, 0xc);
	clearTextSubArea(&textArea);
	drawString(MOVE_NAMES[moveId], 0, slot * 12 + 0x18);
	playSound(0, 3);
}

int32_t loadCardImage(id)
int8_t id;
{
	return loadStackedTIMEntry(MAIN_D_80123E78, (u_long)TEXTURE_BUFFER,
	                           id * 0xe, 0xe);
}

void renderCardImage(void)
{
	renderRectPolyFT4(-0x4b, -0x54, 0x96, 0xb4, 0, 0, GetTPage(1, 0, 576, 256), GetClut(448, 511), 4, 0);
}

void renderCardCount(void)
{
	renderNumber(0, 0x59, 0x4b, 1, getCardAmount(SELECTED_CARD), 3);
	renderString(0, 0x6b, 0x4c, 0xc, 0xc, 0xd8, 0xc, 0);
}

void renderDigimonStatsView(void)
{
	RECT *r;
	StringRect *sr;
	StatsIconClutTable cluts;
	IconRect *icon;
	int32_t i;
	int32_t j;
	int16_t w;
	int16_t k;
	int16_t special;
	int16_t clut;
	uint8_t frame;

	cluts = MAIN_D_80123DB8;
	switch (MENU_STATE) {
	case 0:
		if (drawDigimonStatsStrings() == 1) {
			MENU_STATE = 1;
		}
		break;
	case 1:
		renderSeparatorLines(MAIN_D_80123F54, 6, 5);
		for (j = 0; j < 9; j++) {
			sr = &MAIN_D_80124334[j];
			renderString(3, sr->posX, sr->posY + 1, sr->uvWidth, 0xc, sr->uvX, sr->uvY, 5, 1);
		}
		for (j = 0; j < 0x15; j++) {
			icon = &MAIN_D_8012437C[j];
			renderRectPolyFT4(icon->posX, icon->posY, icon->width, icon->height, icon->texX,
			                  icon->texY + 0x80, 5, GetClut(0x60, cluts.v[j] + 0x1e8), 5, 0);
		}
		renderString(0, -0x6d, -0x42, 0x48, 0xc, 0, 0x3c, 5, 0);
		w = strlen(DIGIMON_DATA[ENTITY_TABLE[1]->type].name);
#if defined(VERSION_JP)
		renderString(0, -0x6d, -0x33, (w / 2) * 12, 0xc, 0x9c, 0x24, 5, 0);
#else
		w = w * 10;
		if (w >= 0x79) {
			w = 0x78;
		}
		renderString(0, -0x6c, -0x32, w, 0xc, 0, 0x48, 5, 0);
#endif
		renderNumber(0, -0x6d, -0x22, 2, PARTNER_PARA.age, 5);
#if defined(VERSION_JP)
		renderNumber(0, -0x26, -0x22, 2, PARTNER_PARA.weight, 5);
		renderNumber(0, 0x23, 3, 4, PARTNER_ENTITY.digimonEntity.stats.current.currentHP, 5);
		renderString(0, 0x53, 4, 0xc, 0xc, 0x3c, 0x30, 5, 0);
		renderNumber(0, 0x5f, 3, 4, PARTNER_ENTITY.digimonEntity.stats.base.hp, 5);
		renderNumber(0, 0x23, 0x12, 4, PARTNER_ENTITY.digimonEntity.stats.current.currentMP, 5);
		renderString(0, 0x53, 0x13, 0xc, 0xc, 0x3c, 0x30, 5, 0);
		renderNumber(0, 0x5f, 0x12, 4, PARTNER_ENTITY.digimonEntity.stats.base.mp, 5);
		renderNumber(0, 0x23, 0x20, 4, PARTNER_ENTITY.digimonEntity.stats.base.off, 5);
		renderNumber(0, 0x23, 0x30, 4, PARTNER_ENTITY.digimonEntity.stats.base.def, 5);
		renderNumber(0, 0x23, 0x3f, 4, PARTNER_ENTITY.digimonEntity.stats.base.speed, 5);
		renderNumber(0, 0x23, 0x4e, 4, PARTNER_ENTITY.digimonEntity.stats.base.brain, 5);
#else
		renderNumber(0, -0x23, -0x22, 2, PARTNER_PARA.weight, 5);
		renderNumber(0, 0x23, 1, 4, PARTNER_ENTITY.digimonEntity.stats.current.currentHP, 5);
		renderNumber(0, 0x5f, 1, 4, PARTNER_ENTITY.digimonEntity.stats.base.hp, 5);
		renderNumber(0, 0x23, 0x10, 4, PARTNER_ENTITY.digimonEntity.stats.current.currentMP, 5);
		renderNumber(0, 0x5f, 0x10, 4, PARTNER_ENTITY.digimonEntity.stats.base.mp, 5);
		renderNumber(0, 0x23, 0x1f, 4, PARTNER_ENTITY.digimonEntity.stats.base.off, 5);
		renderNumber(0, 0x23, 0x2e, 4, PARTNER_ENTITY.digimonEntity.stats.base.def, 5);
		renderNumber(0, 0x23, 0x3d, 4, PARTNER_ENTITY.digimonEntity.stats.base.speed, 5);
		renderNumber(0, 0x23, 0x4c, 4, PARTNER_ENTITY.digimonEntity.stats.base.brain, 5);
#endif
		renderDigimonStatsBar(PARTNER_ENTITY.digimonEntity.stats.base.hp, 0x270f, 0x64, 0x24, 0xc);
		renderDigimonStatsBar(PARTNER_ENTITY.digimonEntity.stats.base.mp, 0x270f, 0x64, 0x24, 0x1b);
		renderDigimonStatsBar(PARTNER_ENTITY.digimonEntity.stats.base.off, 0x3e7, 0x32, 0x24, 0x2a);
		renderDigimonStatsBar(PARTNER_ENTITY.digimonEntity.stats.base.def, 0x3e7, 0x32, 0x24, 0x39);
		renderDigimonStatsBar(PARTNER_ENTITY.digimonEntity.stats.base.speed, 0x3e7, 0x32, 0x24, 0x48);
		renderDigimonStatsBar(PARTNER_ENTITY.digimonEntity.stats.base.brain, 0x3e7, 0x32, 0x24, 0x57);
		for (i = 0, k = 0; i < 3; i++) {
			special = DIGIMON_DATA[ENTITY_TABLE[1]->type].special[i];
			if (special != 0xff) {
				clut = 0x7a06;
				if ((special == 2) || (special == 3) || (special == 4)) {
					clut = 0x7a46;
				} else if (special == 5) {
					clut = 0x7a86;
				}
				if (PLAYTIME_FRAMES % 10 < 5) {
					frame = 0;
				} else {
					frame = 0xc;
				}
				renderRectPolyFT4(k + 9, -0x33, 0xc, 0xc, frame + (special * 0x18 + 0x24), 0x80, 5, clut, 5, 0);
				k += 0xc;
			}
		}
		if (DIGIMON_DATA[ENTITY_TABLE[1]->type].type != 0) {
			renderRectPolyFT4(0x3b, -0x33, 0xc, 0xc, (DIGIMON_DATA[ENTITY_TABLE[1]->type].type - 1) * 0xc, 0x80, 5, 0x7a06, 5, 0);
		}
		if (RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].sleepCycle == 5) {
			renderString(0, 0x55, -0x33, 0x30, 0xc, 0xcc, 0x30, 5, 0);
		}
		if (RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].sleepCycle < 5) {
			renderString(0, 0x55, -0x33, 0x30, 0xc, RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].sleepCycle * 0x30, 0x18, 5, 0);
		}
		for (i = 0; i < 3; i++) {
			if (i < PARTNER_ENTITY.lives) {
				renderRectPolyFT4(i * 0xe - 0x54, 0x22, 0xc, 0xb, 0xe8, 0x8c, 5, 0x7b86, 5, 0);
			}
			renderRectPolyFT4(i * 0xe - 0x54, 0x22, 0xc, 0xb, 0xf4, 0x8c, 5, 0x7b86, 5, 0);
		}
		renderDigimonStatusConditions(PARTNER_PARA.condition);
		if (PARTNER_PARA.happiness >= 0) {
			renderRectPolyFT4(-0x8a, 0x31, 0xb, 0xb, 0, 0xf4, 5, GetClut(0x60, 0x1f2), 5, 0);
		} else {
			renderRectPolyFT4(-0x8a, 0x31, 0xb, 0xb, 0xb, 0xf4, 5, GetClut(0x60, 0x1f2), 5, 0);
		}
		if (PARTNER_PARA.happiness >= 0) {
			renderRectPolyFT4(-0x54, 0x35, (uint8_t)(0x32 - (0x32 - PARTNER_PARA.happiness / 2)), 6, 0x18, 0xf0, 0x18, GetClut(0x70, 0x1f6), 5, 0);
			renderRectPolyFT4(-0x54, 0x35, 0x32, 6, 0x4a, 0xf0, 0x18, GetClut(0x70, 0x1f5), 5, 0);
		} else {
			renderRectPolyFT4(-0x54, 0x35, (uint8_t)(0x32 - (0x32 - (PARTNER_PARA.happiness + 0x64) / 2)), 6, 0x4a, 0xf0, 0x18, GetClut(0x70, 0x1f5), 5, 0);
		}
		if (PARTNER_PARA.discipline >= 0x32) {
			renderRectPolyFT4(-0x8a, 0x40, 0xb, 0xb, 0x16, 0xf4, 5, GetClut(0x60, 0x1f2), 5, 0);
		} else {
			renderRectPolyFT4(-0x8a, 0x40, 0xb, 0xb, 0x21, 0xf4, 5, GetClut(0x60, 0x1f2), 5, 0);
		}
		if (PARTNER_PARA.discipline >= 0x32) {
			renderRectPolyFT4(-0x54, 0x44, (uint8_t)(PARTNER_PARA.discipline - 0x32), 6, 0x18, 0xf0, 0x18, GetClut(0x70, 0x1f6), 5, 0);
			renderRectPolyFT4(-0x54, 0x44, 0x32, 6, 0x4a, 0xf0, 0x18, GetClut(0x70, 0x1f5), 5, 0);
		} else {
			renderRectPolyFT4(-0x54, 0x44, (uint8_t)PARTNER_PARA.discipline, 6, 0x4a, 0xf0, 0x18, GetClut(0x70, 0x1f5), 5, 0);
		}
		renderBox(-0x54, 0x52, PARTNER_PARA.virusBar * 3, 6, 0xc8, 0xc8, 0x3c, 0, 5);
		for (j = 0; j < 0xd; j++) {
			r = &MAIN_D_801242CC[j];
			renderInsetBox(r->x, r->y, r->w, r->h, 5);
		}
		renderDigiviceEntity(ENTITY_TABLE[1], 1);
		break;
	}
}

void renderDigimonMovesView(void)
{
	extern uint8_t MAIN_D_80134237;
	Move *mv;
	StringRect *sr;
	int32_t row;
	IconRect *icon;
	int32_t i;
	int32_t j;
	if (MENU_STATE > 0 && MENU_STATE < 7) {
		for (i = 0; i < 0xd; i++) {
			icon = &MAIN_D_801241CC[i];
			renderRectPolyFT4(icon->posX,
			                  icon->posY,
			                  icon->width,
			                  icon->height,
			                  icon->texX,
			                  icon->texY + 0x80, 5, 0x7b06, 5, 0);
		}
		renderString(3, -0x8e, -0x57, 0x3c, 0xc, 0, 0x48, 5, 1);
		j = 0;
		while (j < 3) {
			if (EQUIPPED_MOVES[j] != 0xff) {
				mv = &MOVE_DATA[EQUIPPED_MOVES[j]];
				renderString(0, -0x7c, j * 0xf - 0x42, 0x78, 0xc, 0, j * 0xc + 0x18, 5, 1);
				renderNumber(0, 0x10, j * 0xf - 0x43, 3, mv->power, 5);
				renderNumber(0, 0x39, j * 0xf - 0x43, 3, mv->mpCost * 3, 5);
				if (mv->range != 0) {
#if defined(VERSION_JP)
					renderString(0, 0x65, j * 0xf - 0x42, 0xc, 0xc,
					             (mv->range - 1) * 0xc + 0x48, 0x6c, 5, 1);
#else
					renderString(0, 0x65, j * 0xf - 0x42, 0xc, 0xc,
					             (mv->range - 1) * 0x24, 0x78, 5, 1);
#endif
				}
				if (mv->status != 0) {
					renderRectPolyFT4(0x7d, j * 0xf - 0x43, 0xc, 0xc,
					                  (mv->status - 1) * 0xc, 0x8c, 5, 0x7a06, 5, 0);
				}
			}
			j++;
		}
		renderSeparatorLines(TECH_VIEW_LINES1, 3, 3);
	}
	switch (MENU_STATE) {
	case 0:
		if (drawDigimonMovesText() == 1) {
			MENU_STATE = 1;
		}
		break;
	case 1:
		renderString(3, -0x8e, -0xf, 0x24, 0xc, 0x3c, 0x48, 5, 1);
		for (i = 0; i < 3; i++) {
			icon = &MAIN_D_801241CC[i];
			renderRectPolyFT4(icon->posX, -9,
			                  icon->width,
			                  icon->height,
			                  icon->texX,
			                  icon->texY + 0x80, 5, 0x7b06, 5, 0);
		}
		renderRectPolyFT4(0x75, 4, 4, 4, 0x78, 0x8c, 5, 0x7b06, 5, 0);
		renderSeparatorLines(&TECH_VIEW_LINES1[15], 3, 5);
		if (MAIN_D_80134237 != 0xff) {
			renderString(3, -0x8e, -0xf, 0x24, 0xc, 0x3c, 0x48, 5, 1);
			renderString(0, -0x7c, 1, 0x84, 0xc, 0, 0x3c, 5, 1);
			renderNumber(0, 0x10, 0, 3, MOVE_DATA[MAIN_D_80134237].power, 5);
			renderString(0, 0x65, 1, 0xc, 0xc,
			             (MOVE_DATA[MAIN_D_80134237].range - 1) * 0xc + 0x48, 0x6c, 5, 1);
			if (MOVE_DATA[MAIN_D_80134237].status != 0) {
				renderRectPolyFT4(0x7d, 0, 0xc, 0xc,
				                  (MOVE_DATA[MAIN_D_80134237].status - 1) * 0xc, 0x8c, 5,
				                  0x7a06, 5, 0);
			}
		}
#if defined(VERSION_JP)
		renderString(0xe, -0x82, 0x19, 0xc, 0xc, 0, 0x54, 5, 1);
		renderString(0, -0x76, 0x19, 0xa8, 0xc, 0xc, 0x54, 5, 1);
		renderString(0, -0x82, 0x27, 0x84, 0xc, 0, 0x60, 5, 1);
		renderString(0xf, 2, 0x27, 0xc, 0xc, 0x84, 0x60, 5, 1);
#else
		renderString(0xf, -0x82, 0x19, 0xc, 0xc, 0, 0x54, 5, 1);
		renderString(0, -0x76, 0x19, 0xa8, 0xc, 0xc, 0x54, 5, 1);
		renderString(0, -0x76, 0x27, 0x84, 0xc, 0xc, 0x60, 5, 1);
		renderString(7, -0x82, 0x27, 0xc, 0xc, 0, 0x60, 5, 1);
#endif
		renderString(0, 0xe, 0x27, 0x18, 0xc, 0x90, 0x60, 5, 1);
		break;
	case 2:
	case 4:
		UI_BOX_DATA[1].finalPos.h -= 0x27;
		if (UI_BOX_DATA[1].finalPos.h < 0x4a) {
			++MENU_STATE;
		}
		break;
	case 3:
	case 5:
		UI_BOX_DATA[1].finalPos.h += 0x27;
		if (0xbd < UI_BOX_DATA[1].finalPos.h) {
			MENU_STATE = (MENU_STATE == 3) ? 6 : 1;
		}
		break;
	case 6:
		renderString(3, -0x8e, -0xf, 0x3c, 0xc, 0x60, 0x48, 5, 1);
		renderSeparatorLines(TECH_VIEW_LINES3, 0xc, 5);
		for (i = 0; i < 4; i++) {
			icon = &MAIN_D_80124044[i];
			renderRectPolyFT4(icon->posX,
			                  icon->posY,
			                  icon->width,
			                  icon->height,
			                  icon->texX,
			                  icon->texY + 0x80, 5, 0x7b06, 5, 0);
		}
		renderString(0, -0x87, 0x20, 0x30, 0xc, 0x3c, 0, 5, 1);
		renderString(0, -0x81, 0x2e, 0x24, 0xc, 0, 0x6c, 5, 1);
#if defined(VERSION_JP)
		renderString(0x10, -0x80, 0x4c, 0xc, 0xc, 0x24, 0x6c, 5, 1);
		renderString(0, -0x74, 0x4c, 0x18, 0xc, 0x30, 0x6c, 5, 1);
#else
		renderString(0xc, -0x74, 0x4c, 0x18, 0xc, 0x30, 0x6c, 5, 1);
#endif
		renderRectPolyFT4(-0x74, 0x3c, 10, 10, 0x80, 0x8c, 5, 0x7b06, 5, 0);
		j = 0;
		while (j < 7) {
			renderRectPolyFT4(-0x43, j * 0xf - 8, 0x14, 10, j * 0x14, 0x98, 5, 0x7b06, 5, 0);
			renderRectPolyFT4(100, j * 0xf - 8, 0x14, 10, j * 0x14, 0x98, 5, 0x7b06, 5, 0);
			j++;
		}
		j = 0;
		while (j < 8) {
			renderRectPolyFT4(j * 0x12 - 0x26, -0xf, 4, 5, j * 4 + 0x94, 0x8c, 5, 0x7b06, 5, 0);
			j++;
		}
		renderRectPolyFT4(MAIN_D_80134D3A - 0xa0,
		                  MAIN_D_80134D38 - 0x78, 0x12, 0x10, 0xc0, 0x8c, 5, 0x7b06, 5, 0);
		for (row = 0; row < 7; row++) {
			for (j = 0; j < 8; j++) {
				renderBox(j * 0x12 - 0x2a, row * 0xf - 7, 0xc, 0xc,
				          0x4e, 0x60, 0x6e, 0x80, 5);
			}
		}
		if (0xa8 < MAIN_D_80134D3A) {
			renderDigimonMovesSelected(0);
		} else {
			renderDigimonMovesSelected(1);
		}
		renderDigimonMoveBox();
		break;
	case 7:
		if (drawMoveViewHelpStrings() == 1) {
			MENU_STATE = 8;
		}
		break;
	case 8:
		drawLine2P(0x20202, -0x92, -4, 0x92, -4, 5, 0);
		drawLine2P(0xfad990, -0x93, -3, 0x93, -3, 5, 0);
		drawLine2P(0x20202, -0x92, -2, 0x92, -2, 5, 0);
#if defined(VERSION_JP)
		renderString(3, -0x8e, -0x52, 0x54, 0xc, 0, 0x30, 5, 1);
#else
		renderString(3, -0x8e, -0x52, 0xa0, 0xc, 0, 0x30, 5, 1);
#endif
		renderString(0xe, -0x72, -0x32, 0xc, 0xc, 0xcc, 0x30, 5, 1);
		renderString(0xe, 6, -0x21, 0xc, 0xc, 0xcc, 0x30, 5, 1);
		renderString(0xe, -0x2a, -0x13, 0xc, 0xc, 0xcc, 0x30, 5, 1);
		for (i = 0; i < 0xc; i++) {
			sr = &MAIN_D_80124234[i];
			renderString(0, sr->posX,
			             sr->posY + 1,
			             sr->uvWidth, 0xc,
			             sr->uvX,
			             sr->uvY, 5, 1);
		}
		renderString(0, 0x42, 0x33, 0x3c, 0xc, 0x78, 0x48, 5, 1);
		renderString(0, 0x42, 0x43, 0x3c, 0xc, 0x84, 0x54, 5, 1);
		renderRectPolyFT4(-0x81, -0x41, 0xc, 0xc, 0xb4, 0x8c, 5, 0x7b06, 5, 0);
		renderRectPolyFT4(-0x81, -0x22, 0xc, 0xc, 0xb4, 0x8c, 5, 0x7b06, 5, 0);
		renderRectPolyFT4(-0x7b, 0x50, 0x12, 0x10, 0xc0, 0x8c, 5, 0x7b06, 5, 0);
		for (i = 0; i < 7; i++) {
			icon = &MAIN_D_80124294[i];
#if !defined(VERSION_JP)
			if (i != 2) {
#endif
				renderRectPolyFT4(icon->posX, icon->posY, icon->width, icon->height,
				                  icon->texX, icon->texY + 0x80, 5,
				                  (i == 5) ? getClut(96, 489) : getClut(96, 488), 5, 0x80);
#if !defined(VERSION_JP)
			}
#endif
		}
#if defined(VERSION_JP)
		renderBox(-0x18, -0x22, 0xc, 0xc, 200, 0, 0x28, 0x81, 4);
#else
		renderBox(0x52, -0x22, 0xc, 0xc, 200, 0, 0x28, 0x81, 4);
#endif
		renderBox(-0x78, 2, 0xc, 0xc, 200, 0, 0x28, 0x81, 4);
		renderBox(-0x78, 0x22, 0xc, 0xc, 0x4e, 0x60, 0x6e, 0x80, 4);
		renderBox(-0x78, 0x32, 0xc, 0xc, 0x68, 0x68, 0x68, 0x83, 4);
		renderBox(-0x78, 0x42, 0xc, 0xc, 0x68, 0x68, 0x68, 0x83, 4);
		break;
	case 9:
		if (drawDigimonMovesText() == 1) {
			MENU_STATE = 6;
		}
	}
}

void renderMenuTab(int16_t x, int8_t w, int8_t layer)
{
	int32_t i;
	int8_t h;

	h = 0x10;
	if (layer == 1) {
		h--;
	}
	renderRectPolyFT4(x, -0x68, 7, h, 0xd4, 0x8c, 5, GetClut(0x60, 0x1ec), 5, layer);
	for (i = 0; i < (w - 0xe) / 4; i++) {
		renderRectPolyFT4(x + 7 + i * 4, -0x68, 4, h, 0xe2, 0x8c, 5, GetClut(0x60, 0x1ec), 5, layer);
	}
	renderRectPolyFT4(x + w - 9, -0x68, 7, h, 0xdb, 0x8c, 5, GetClut(0x60, 0x1ec), 5, layer);
}

void renderPlayerInfoView(void)
{
	int32_t i;
	StringRect *e;
	RECT *r;
	int32_t j;
	int32_t k;
	int32_t n;
	int8_t count;

	switch (MENU_STATE) {
	case 0:
		if (drawPlayerInfoStrings() == 1) {
			MENU_STATE = 1;
		}
		break;
	case 1:
		renderSeparatorLines(MAIN_D_80124424, 0xb, 5);

		for (i = 0; i < 0xb; i++) {
			e = &MAIN_D_801244EC[i];
			renderString(3 - ((i / 7) * 3), e->posX, e->posY + 1, e->uvWidth, 0xc, e->uvX,
			             e->uvY, 5, 1);
		}

		renderString(0, -0x54, -0x50, 0x48, 0xc, 0, 0x30, 5, 1);
		renderNumber(0, 0x35, -0x36, 2, TAMER_ENTITY.tamerLevel, 5);
#if defined(VERSION_JP)
		renderString(0, 0x28, -0x25, 0x30, 0xc, 0, (TAMER_ENTITY.tamerLevel * 12) + 0x40, 5, 1);
#else
		renderString(0, 0x28, -0x24, 0x64, 0xc, 0, (TAMER_ENTITY.tamerLevel * 12) + 0x40, 5, 1);
#endif
		n = strlen(TAMER_LEVEL_TITLES[TAMER_ENTITY.tamerLevel]) / 2;
		renderString(0, (n * 12) + 0x28, -0x25, 0x30, 0xc, 0xb4, 0x18, 5, 1);
		renderNumber(0, 0x35, -0x11, 2, TAMER_ENTITY.raisedCount, 5);
		renderNumber(0, 0x29, 2, 6, MONEY, 5);
#if defined(VERSION_JP)
		renderNumber(0, 0x29, 0x16, 3, PLAYTIME_HOURS, 5);
		renderNumber(0, 0x59, 0x16, 2, PLAYTIME_MINUTES, 5);
#else
		renderString(0, 0x32, 0x16, 0x4c, 0xc, 0, 0xe4, 5, 1);
#endif

		for (i = 0; i < 2; i++) {
			if (isTriggerSet(i + 0x2d) == 1) {
				renderRectPolyFT4(i * 0x15 + 0x34, 0x37, 0x10, 0x10, i * 0x10 + 0xb0, 0xa0, 5, 0x7bc6, 5, 0);
			}
		}

		if (isTriggerSet(0x2f) == 1) {
			renderRectPolyFT4(0x5e, 0x37, 0x10, 0x10, 0xe0, 0xa0, 5, 0x7bc6, 5, 0);
		}

		if (isTriggerSet(0x30) == 1) {
			renderRectPolyFT4(0x73, 0x37, 0x10, 0x10, 0xd0, 0xa0, 5, 0x7bc6, 5, 0);
		}

		count = 0;
		for (j = 0; j < 0x12; j++) {
			if (hasMedal(j) != 0) {
				count++;
			}
		}

		renderNumber(0, 0x35, 0x4d, 2, count, 5);
		renderDigiviceEntity(ENTITY_TABLE[0], 0);

		for (k = 0; k < 0xb; k++) {
			r = &MAIN_D_80124494[k];
			renderInsetBox(r->x, r->y, r->w, r->h, 5);
		}
		break;
	}
}

void renderDigimonMovesSelected(int16_t panel)
{
	RECT rect;
	Move *move;
	int32_t i;
	uint8_t row;
	uint8_t col;
#if defined(VERSION_JP)
	uint8_t status;
#endif
	uint8_t moveId;

	if (panel == 0) {
		renderSeparatorLines(MAIN_D_80124064, 0x12, 4);
	} else {
		renderSeparatorLines(MAIN_D_80124118, 0x12, 4);
	}

	renderRectPolyFT4((panel * 0x9d) - 0x91, 1, 4, 4, 0x78, 0x90, 5, 0x7b06, 4, 0);
	renderRectPolyFT4((panel * 0x9d) - 0x10, 1, 4, 4, 0x7c, 0x90, 5, 0x7b06, 4, 0);
	renderRectPolyFT4((panel * 0xcb) - 0x91, 0x5c, 4, 4, 0x78, 0x94, 5, 0x7b06, 4, 0);
	renderRectPolyFT4((panel * 0xcb) - 0x3e, 0x5c, 4, 4, 0x7c, 0x94, 5, 0x7b06, 4, 0);
	if (panel == 0) {
		renderRectPolyFT4(-0x10, 0x15, 4, 4, 0x7c, 0x94, 5, 0x7b06, 4, 0);
	} else {
		renderRectPolyFT4(0xc, 0x15, 4, 4, 0x78, 0x94, 5, 0x7b06, 4, 0);
	}
	renderBox((panel * 0x9d) - 0x8f, 3, 0x80, 0x13, 0x32, 0x32, 0x80, 0, 4);
	renderBox((panel * 0xcb) - 0x8f, 0x18, 0x57, 0x45, 0x32, 0x32, 0x80, 0, 4);
	renderRectPolyFT4((panel * 0x9d) - 0x8c, -2, 0x25, 7, 0x11, 0xb0, 5, 0x7b06, 3, 0);
	renderRectPolyFT4((panel * 0xcb) - 0x8c, 0x1d, 0x17, 7, 0x5c, 0xa2, 5, 0x7b06, 3, 0);
	renderRectPolyFT4((panel * 0xcb) - 0x8c, 0x2e, 0xb, 7, 0x74, 0xa2, 5, 0x7b06, 3, 0);
	renderRectPolyFT4((panel * 0xcb) - 0x8c, 0x3f, 0x13, 7, 0x4c, 0xa9, 5, 0x7b06, 3, 0);
	renderRectPolyFT4((panel * 0xcb) - 0x8c, 0x50, 0x10, 7, 0, 0xb0, 5, 0x7b06, 3, 0);

	for (i = 0; i < 4; i++) {
		renderRectPolyFT4((panel * 0xcb) - 0x71, i * 0x11 + 0x1b, 0xa, 0xa, 0x8a, 0x8c, 5, 0x7b06, 3, 0);
	}

	row = (MAIN_D_80134D3A - 0x73) / 18;
	col = (MAIN_D_80134D38 - 0x6f) / 15;
	if (col == 1) {
		col = 5;
	} else if (col == 2) {
		col = 1;
	} else if (col == 3) {
		col = 4;
	} else if (col == 4) {
		col = 2;
	} else if (col == 5) {
		col = 3;
	}
	moveId = col * 8 + row;
	if (col == 6) {
		moveId++;
	}

	if (hasMove(moveId)) {
		move = &MOVE_DATA[moveId];
		setRECT(&rect, 0, 0x84, 0x78, 0xc);
		clearTextSubArea(&rect);
		drawString(MOVE_NAMES[moveId], 0, 0x84);
		renderString(0, (panel * 0x9d) - 0x8a, 9, 0x78, 0xc, 0, 0x84, 3, 1);
		renderNumber(0, (panel * 0xcb) - 0x64, 0x1b, 3, move->power, 3);
		renderNumber(0, (panel * 0xcb) - 0x64, 0x2c, 3, move->mpCost * 3, 3);
		renderString(0, (panel * 0xcb) - 0x64, 0x3e, 0x24, 0xc, (move->range - 1) * 0x24, 0x78, 3, 1);
#if defined(VERSION_JP)
		status = move->status;
		if (status == 1) {
			renderString(0, (panel * 0xcb) - 0x64, 0x4f, 0xc, 0xc, 0x84, 0x6c, 3, 1);
		} else if (status == 4) {
			renderString(0, (panel * 0xcb) - 0x64, 0x4f, 0x24, 0xc, 0xc0, 0x6c, 3, 1);
		} else if (status != 0) {
			renderString(0, (panel * 0xcb) - 0x64, 0x4f, 0x18, 0xc, (status - 2) * 0x18 + 0x90, 0x6c, 3, 1);
		}
#else
		switch (move->status) {
		case 1:
			renderString(0, (panel * 0xcb) - 0x64, 0x4f, 0x1a, 0xc, 0x84, 0x6c, 3, 1);
			break;
		case 2:
			renderString(0, (panel * 0xcb) - 0x64, 0x4f, 0x1c, 0xc, 0xa0, 0x6c, 3, 1);
			break;
		case 3:
			renderString(0, (panel * 0xcb) - 0x64, 0x4f, 0x20, 0xc, 0xbc, 0x6c, 3, 1);
			break;
		case 4:
			renderString(0, (panel * 0xcb) - 0x64, 0x4f, 0x1e, 0xc, 0xdb, 0x6c, 3, 1);
			break;
		}
#endif
	}
}

void renderDigimonMoveBox(void)
{
	int16_t moveId;
	int16_t moves[16];
	int32_t col;
	int32_t row;
	int32_t j;
	int16_t clut;
	uint8_t x;
	uint8_t special;
	int8_t blink;
	int32_t i;
	uint8_t y;

	for (i = 0; i < 3; i++) {
		if (EQUIPPED_MOVES[i] < 0x3a) {
			x = EQUIPPED_MOVES[i] % 8;
			if (EQUIPPED_MOVES[i] < 8) {
				y = 0;
			} else if (EQUIPPED_MOVES[i] < 0x10) {
				y = 2;
			} else if (EQUIPPED_MOVES[i] < 0x18) {
				y = 4;
			} else if (EQUIPPED_MOVES[i] < 0x20) {
				y = 5;
			} else if (EQUIPPED_MOVES[i] < 0x28) {
				y = 3;
			} else if (EQUIPPED_MOVES[i] < 0x30) {
				y = 1;
			} else {
				x = (EQUIPPED_MOVES[i] - 1) % 8;
				y = 6;
			}
			if (EQUIPPED_MOVES[i] == 0x30) {
				x = 4;
				y = 1;
			}
			if (EQUIPPED_MOVES[i] == 0x39) {
				x = 6;
				y = 6;
			}
			renderBox(x * 0x12 - 0x2a, y * 0xf - 7, 0xc, 0xc, 200, 0, 0x28, 1, 4);
		}
	}

	for (i = 0; i < 0x10; i++) {
		moves[i] = DIGIMON_DATA[ENTITY_TABLE[1]->type].moves[i];
	}
	sortArray(moves, 0x10);

	i = 0;
	for (row = 0; row < 7; row++) {
		for (col = 0; col < 8; col++) {
			moveId = row * 8 + col;
			if (moveId >= 0x30) {
				moveId++;
			}
			j = 0;
			if (moveId == 0x2c) {
				for (j = 0; j < 0x10; j++) {
					if (moves[j] == 0x2c || moves[j] == 0x30) {
						break;
					}
					if (j == 0xf) {
						renderBox(0x1e, 8, 0xc, 0xc, 0x68, 0x68, 0x68, 3, 4);
					}
				}
			}
			if (moveId == 0x37) {
				for (j = 0; j < 0x10; j++) {
					if (moves[j] == 0x37 || moves[j] == 0x39) {
						break;
					}
					if (j == 0xf) {
						renderBox(0x42, 0x53, 0xc, 0xc, 0x68, 0x68, 0x68, 3, 4);
					}
				}
			}
			if (moveId == moves[i]) {
				i++;
			} else if (moveId != 0x2c && moveId != 0x37 && moveId != 0x30 &&
			           moveId != 0x39) {
				if (moveId < 8) {
					y = 0;
				} else if (moveId < 0x10) {
					y = 2;
				} else if (moveId < 0x18) {
					y = 4;
				} else if (moveId < 0x20) {
					y = 5;
				} else if (moveId < 0x28) {
					y = 3;
				} else if (moveId < 0x30) {
					y = 1;
				} else {
					y = 6;
				}
				renderBox(col * 0x12 - 0x2a, y * 0xf - 7, 0xc, 0xc, 0x68, 0x68, 0x68, 3, 4);
			}
			if (hasMove(moveId) == 1) {
				special = MOVE_DATA[moveId].special;
				switch (special) {
				case 0:
					clut = 0x7a06;
					break;
				case 1:
					clut = 0x7a06;
					break;
				case 2:
					clut = 0x7a46;
					break;
				case 3:
					clut = 0x7a46;
					break;
				case 4:
					clut = 0x7a46;
					break;
				case 5:
					clut = 0x7a86;
					break;
				case 6:
					clut = 0x7a06;
				}
				if (PLAYTIME_FRAMES % 0x14 < 10) {
					blink = 0;
				} else {
					blink = 0xc;
				}
				renderRectPolyFT4(col * 0x12 - 0x2a, special * 0xf - 7, 0xc, 0xc,
				                  blink + (special * 0x18 + 0x24), 0x80, 5, clut, 4, 0);
			}
		}
	}
}

int32_t drawMoveViewHelpStrings(void)
{
	RECT rect;

	rect = MAIN_D_801342F0;
	if (MENU_SUB_STATE == 0) {
		clearTextSubArea(&rect);
	}

	drawString(STATUS_VIEW_LABELS[MENU_SUB_STATE + 9], 0, MENU_SUB_STATE * 0xc + 0x18);
	MENU_SUB_STATE++;

	if (MENU_SUB_STATE == 8) {
		return 1;
	}

	return 0;
}

void sortArray(int16_t *arr, int8_t count)
{
	int32_t i;
	int32_t j;
	int16_t tmp;

	for (i = 0; i < count - 1; ++i) {
		for (j = i + 1; j < count; ++j) {
			if (arr[i] > arr[j]) {
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

void removeTriangleMenu(void)
{
	removeStaticUIBox(0);
	removeObject(0xfa4, 0);
}

void removeUIBox1(void)
{
	removeStaticUIBox(1);
}

void renderEvoChartView(void)
{
	int32_t i;
	int16_t x;
	int8_t shift;
	EvoClutTable cluts;

	cluts = MAIN_D_80123E1C;
	switch (MENU_STATE) {
	case 0:
		if (drawEvoChartStrings(0) == 1) {
			MENU_STATE = 1;
		}
		break;
	case 2:
	case 3:
	case 4:
		if (MAIN_D_80134D42 < 3) {
			x = (int16_t)(MAIN_D_80134D42 * 0x25 + 0x1c);
		} else if (MAIN_D_80134D42 < 7) {
			x = (int16_t)((MAIN_D_80134D42 - 3) * 0x18 + 0x8b);
		} else {
			x = (int16_t)((MAIN_D_80134D42 - 7) * 0x18 + 0xf8);
		}
		renderRectPolyFT4((int16_t)(x - 0xa2),
		                  (int16_t)(MAIN_D_80134D44 * 0x13 - 0x4e), 0x18, 0x14,
		                  0, 0xe8, 0x18, 0x7dc7, 5, 0);
		/* fall through */
	case 1:
		for (i = 0; i < 0x3e; i++) {
			if (hasDigimonRaised((i + 1) & 0xffff)) {
				shift = 0;
				if ((i + 1 == MAIN_D_80134D40) && (1 < MENU_STATE) &&
				    ((PLAYTIME_FRAMES % 10) < 5)) {
					shift = 0x10;
				}
				renderRectPolyFT4(
					(int16_t)(MAIN_D_80124544[i].posX - 0xa0),
					(int16_t)(MAIN_D_80124544[i].posY - 0x78),
					0x10, 0x10,
					(uint8_t)(shift + MAIN_D_80124544[i].u),
					MAIN_D_80124544[i].v, 0x18,
					cluts.m[MAIN_D_80124544[i].clut],
					5, 0);
			}
		}
		for (i = 0; i < 0x3e; i++) {
			renderBorderBox((MAIN_D_80124544[i].posX - 1),
			                (MAIN_D_80124544[i].posY - 1), 0x12, 0x12,
			                0xbebebe, 0x3c3c3c, 0x87, 0x87, 0x87, 5);
		}
		renderBorderBox(0x15, 0x27, 0x22, 0x9f, 0xaaa0c8, 0x5a3c8c,
		                0xa5, 0x5a, 0x73, 5);
		renderBorderBox(0x3a, 0x27, 0x22, 0x9f, 0xa078a5, 0x46144b,
		                0x6e, 0x2d, 0x5a, 5);
		renderBorderBox(0x5f, 0x27, 0x22, 0xb0, 0xc88250, 0x5f370a,
		                0xf, 0x50, 0x82, 5);
		renderBorderBox(0x84, 0x27, 0x6a, 0x9f, 0x6eaf5a, 0x285a00,
		                0, 0x87, 0x41, 5);
		renderBorderBox(0xf1, 0x27, 0x38, 0x9f, 0x6996d2, 0x1e4178,
		                0xaf, 0x64, 0x2d, 5);
#if defined(VERSION_JP)
		renderString(0xe, -0x1a, 0x54, 0xc, 0xc, 0, 0xc, 5, 1);
#else
		renderString(0xf, -0x1a, 0x54, 0xc, 0xc, 0, 0xc, 5, 1);
#endif
		renderString(0, -0xe, 0x54, 0x3c, 0xc, 0xc, 0xc, 5, 1);
#if defined(VERSION_JP)
		renderString(0xf, 0x3a, 0x54, 0xc, 0xc, 0x48, 0xc, 5, 1);
#else
		renderString(7, 0x3a, 0x54, 0xc, 0xc, 0x48, 0xc, 5, 1);
#endif
		renderString(0, 0x46, 0x54, 0x3c, 0xc, 0x54, 0xc, 5, 1);
	}
}

GARBAGE_ARRAY(renderEvoChartDetail, TECH_VIEW_LINES1, 30, 9);

void renderEvoChartDetail(void)
{
	Line4Points *fromLines;
	Line4Points *toLines;
	int32_t j;
	ChartSprite *fromSprites;
	ChartSprite *toSprites;
	uint32_t color1;
	uint32_t color2;
	int32_t id;
	int32_t len;
	EvoClutTable clut;
	int32_t i;
	int8_t count1;
	int8_t count2;
	int8_t fromCount;
	int8_t toCount;

	clut = MAIN_D_80123E6C;
	MENU_SUB_STATE = 2;
	drawEvoChartStrings((int8_t)MAIN_D_80134D40);
	count1 = (count2 = 0);
	for (i = 0; i < 5; i++) {
		if (EVO_PATHS_DATA[MAIN_D_80134D40 - 1].from[i] != -1) {
			count1++;
		}
	}
	for (i = 0; i < 6; i++) {
		if (EVO_PATHS_DATA[MAIN_D_80134D40 - 1].to[i] != -1) {
			count2++;
		}
	}

	if ((count1 % 2) == 0) {
		fromLines = MAIN_D_80124944;
		fromSprites = MAIN_D_80124AA8;
		fromCount = 4;
	} else {
		fromLines = MAIN_D_80124984;
		fromSprites = MAIN_D_80124AC8;
		fromCount = 5;
	}
	if ((count2 % 2) == 0) {
		toLines = MAIN_D_801249D4;
		toSprites = MAIN_D_80124AF0;
		toCount = 6;
	} else {
		toLines = MAIN_D_80124A34;
		toSprites = MAIN_D_80124B20;
		toCount = 5;
	}

	for (j = 0; j < fromCount; j++) {
		if (EVO_PATHS_DATA[MAIN_D_80134D40 - 1].from[j] > 0) {
			drawLine3P(0x65db,
			           fromLines->x1, fromLines->y1 - 1,
			           fromLines->x2, fromLines->y2 - 1,
			           fromLines->x3, fromLines->y3 - 1,
			           4, 0);
			drawLine2P(0x65db,
			           fromLines->x3, fromLines->y3 - 1,
			           fromLines->x4, fromLines->y4 - 1,
			           4, 0);
			drawLine3P(0x794e3,
			           fromLines->x1, fromLines->y1,
			           fromLines->x2, fromLines->y2,
			           fromLines->x3, fromLines->y3,
			           4, 0);
			drawLine2P(0x794e3,
			           fromLines->x3, fromLines->y3,
			           fromLines->x4, fromLines->y4,
			           4, 0);
			drawLine3P(0x65db,
			           fromLines->x1, fromLines->y1 + 1,
			           fromLines->x2, fromLines->y2 + 1,
			           fromLines->x3, fromLines->y3 + 1,
			           4, 0);
			drawLine2P(0x65db,
			           fromLines->x3, fromLines->y3 + 1,
			           fromLines->x4, fromLines->y4 + 1,
			           4, 0);
		}
		fromLines++;
	}

	for (j = 0; j < toCount; j++) {
		if (EVO_PATHS_DATA[MAIN_D_80134D40 - 1].to[j] > 0) {
			color1 = (MAIN_D_80124A84[j * 2].r & 0xff) |
			         ((MAIN_D_80124A84[j * 2].g & 0xff) << 8) |
			         ((MAIN_D_80124A84[j * 2].b & 0xff) << 16);
			color2 = (MAIN_D_80124A84[(j * 2) + 1].r & 0xff) |
			         ((MAIN_D_80124A84[(j * 2) + 1].g & 0xff) << 8) |
			         ((MAIN_D_80124A84[(j * 2) + 1].b & 0xff) << 16);
			drawLine3P(color2,
			           toLines->x1, toLines->y1 - 1,
			           toLines->x2, toLines->y2 - 1,
			           toLines->x3, toLines->y3 - 1,
			           4, 0);
			drawLine2P(color2,
			           toLines->x3, toLines->y3 - 1,
			           toLines->x4, toLines->y4 - 1,
			           4, 0);
			drawLine3P(color1,
			           toLines->x1, toLines->y1,
			           toLines->x2, toLines->y2,
			           toLines->x3, toLines->y3,
			           4, 0);
			drawLine2P(color1,
			           toLines->x3, toLines->y3,
			           toLines->x4, toLines->y4,
			           4, 0);
			drawLine3P(color2,
			           toLines->x1, toLines->y1 + 1,
			           toLines->x2, toLines->y2 + 1,
			           toLines->x3, toLines->y3 + 1,
			           4, 0);
			drawLine2P(color2,
			           toLines->x3, toLines->y3 + 1,
			           toLines->x4, toLines->y4 + 1,
			           4, 0);
		}
		toLines++;
	}

	renderRectPolyFT4(-8, -0x14, 0x10, 0x10,
	                  MAIN_D_80124544[MAIN_D_80134D40 - 1].u,
	                  MAIN_D_80124544[MAIN_D_80134D40 - 1].v, 0x18,
	                  clut.m[MAIN_D_80124544[MAIN_D_80134D40 - 1].clut],
	                  4, 0);
	renderBorderBox(0x97, 99, 0x12, 0x12, 0xbebebe, 0x3c3c3c, 0x87, 0x87,
	                0x87, 4);

	for (j = 0; j < fromCount; j++) {
		id = EVO_PATHS_DATA[MAIN_D_80134D40 - 1].from[j];
		if (id > 0) {
			if (hasDigimonRaised(id & 0xffff) == 1) {
				renderRectPolyFT4(fromSprites->posX, fromSprites->posY,
				                  0x10, 0x10,
				                  MAIN_D_80124544[id - 1].u,
				                  MAIN_D_80124544[id - 1].v, 0x18,
				                  clut.m[MAIN_D_80124544[id - 1].clut],
				                  4, 0);
			}
			renderBorderBox(fromSprites->posX + 0x9f,
			                fromSprites->posY + 0x77, 0x12, 0x12,
			                0xbebebe, 0x3c3c3c, 0x87, 0x87, 0x87, 4);
		}
		fromSprites++;
	}

	for (j = 0; j < toCount; j++) {
		id = EVO_PATHS_DATA[MAIN_D_80134D40 - 1].to[j];
		if (id > 0) {
			if (hasDigimonRaised(id & 0xffff) == 1) {
				renderRectPolyFT4(toSprites->posX, toSprites->posY,
				                  0x10, 0x10,
				                  MAIN_D_80124544[id - 1].u,
				                  MAIN_D_80124544[id - 1].v, 0x18,
				                  clut.m[MAIN_D_80124544[id - 1].clut],
				                  4, 0);
			}
			renderBorderBox(toSprites->posX + 0x9f,
			                toSprites->posY + 0x77, 0x12, 0x12,
			                0xbebebe, 0x3c3c3c, 0x87, 0x87, 0x87, 4);
		}
		toSprites++;
	}

#if defined(VERSION_JP)
	renderString(3, -0x19, -0x4f, 0x30, 0xc, 0, 0x18, 4);
	renderString(0, -0x56, 0x3a, 0x24, 0xc,
	             ((DIGIMON_DATA[MAIN_D_80134D40].level - 1) * 36) + 0x30, 0x18,
	             4);
#else
	renderString(3, -0x14, -0x4f, 0x24, 0xc, 0, 0x18, 4);
	switch (DIGIMON_DATA[MAIN_D_80134D40].level) {
	case 1:
		renderString(0, -0x5b, 0x3a, 0x2e, 0xc, 0x24, 0x18, 4);
		break;
	case 2:
		renderString(0, -0x63, 0x3a, 0x3e, 0xc, 0x52, 0x18, 4);
		break;
	case 3:
		renderString(0, -0x5b, 0x3a, 0x2f, 0xc, 0, 0x24, 4);
		break;
	case 4:
		renderString(0, -0x64, 0x3a, 0x41, 0xc, 0x2f, 0x24, 4);
		break;
	case 5:
		renderString(0, -0x64, 0x3a, 0x41, 0xc, 0, 0x3c, 4);
	}
#endif
	len = strlen(DIGIMON_DATA[MAIN_D_80134D40].name) / 2;
	renderString(0, -0x5c - ((len - 4) * 6), 0x4d, 0x78, 0xc, 0, 0x30, 4);
	for (i = 0; i < 6; i++) {
		renderInsetBox(MAIN_D_80124B48[i].posX, MAIN_D_80124B48[i].posY,
		               MAIN_D_80124B48[i].width,
		               MAIN_D_80124B48[i].height, 4);
	}
}

void renderMedalView(void)
{
	int32_t i;
	int32_t j;

	switch (MENU_STATE) {
	case 0:
		if (drawMedalViewStrings() == 1) {
			MENU_STATE = 1;
		}
		return;
	case 3:
		if (drawMedalViewStrings() == 1) {
			MENU_STATE = 2;
		}
	case 2:
		renderRectPolyFT4(((MEDAL_SELECTOR_INDEX % 5) * 38) - 0x7e,
		                  ((MEDAL_SELECTOR_INDEX / 5) * 24) - 0x3d, 0x18, 0x18, 0, 0xd0,
		                  0x18, 0x7dc7, 1, 0);
	case 1:
		renderSeparatorLines(MAIN_D_8012472C, 0xe, 5);
		renderString(3, -0x36, -0x51, 0x6c, 0xc, 0, 0xc, 5, 1);

		for (i = 0; i < 0xf; i++) {
			if (hasMedal(i) != 0) {
				if (MEDAL_SELECTOR_INDEX == i) {
					if (MENU_STATE != 3) {
#if defined(VERSION_JP)
						renderString(0, -0x42, 0x17, 0x84, 0xc, 0, 0x18, 5, 1);
#else
						renderString(0, -0x5d, 0x17, 0xac, 0xc, 0, 0x18, 5, 1);
#endif
						for (j = 0; j < 3; j++) {
							renderString(0, -0x7e, j * 13 + 0x27, 0xfc, 0xc, 0,
							             j * 12 + 0x24, 5, 1);
						}
					}
					renderDigiviceMedals();
				}
			} else {
#if defined(VERSION_JP)
				if (MEDAL_SELECTOR_INDEX == i) {
					renderString(0, -0x42, 0x17, 0xc, 0xc, 0xe4, 0xc, 5, 1);
					renderString(0, -0x7e, 0x27, 0xc, 0xc, 0xe4, 0xc, 5, 1);
				}
#endif
				renderRectPolyFT4(((i % 5) * 38) - 0x7c, ((i / 5) * 24) - 0x3d, 0xf,
				                  0x18, 0xf0, 0xc8, 0x18, 0x7dc7, 5, 0);
			}

			renderRectPolyFT4(((i % 5) * 38) - 0x7a, ((i / 5) * 24) - 0x3d, 0xf, 0x18,
			                  0xf0, 0xb0, 0x18, 0x7dc7, 5, 0);
		}

#if defined(VERSION_JP)
		renderInsetBox(0x5c, 0x8d, 0x88, 0xe, 5);
#else
		renderInsetBox(0x3e, 0x8d, 0xb0, 0xe, 5);
#endif
		renderString(FOOTER_GLYPH1_COLOR, -0x23, 0x55, 0xc, 0xc, 0x6c, 0xc, 5, 1);
		renderString(0, -0x17, 0x55, 0x18, 0xc, 0x84, 0xc, 5, 1);
		renderString(0, 1, 0x55, 0x24, 0xc, 0x9c, 0xc, 5, 1);
		renderString(FOOTER_GLYPH2_COLOR, 0x2b, 0x55, 0xc, 0xc, 0x78, 0xc, 5, 1);
#if defined(VERSION_JP)
		renderString(0, 0x37, 0x55, 0x18, 0xc, 0x84, 0xc, 5, 1);
		renderString(0, 0x4f, 0x55, 0x24, 0xc, 0xc0, 0xc, 5, 1);
#else
		renderString(0, 0x37, 0x55, 0x30, 0xc, 0xc0, 0xc, 5, 1);
#endif
	}
}

void renderCardsView(void)
{
	CardSprites sprites;
	int32_t j;
	int32_t i;
	int8_t card;
	int8_t amount;

	sprites = MAIN_D_80123E28;

	switch (MENU_STATE) {
	case 0:
		if (drawCardViewStrings() == 1) {
			MENU_STATE = 1;
		}
		return;
	case 2:
	case 3:
		renderRectPolyFT4(((SELECTED_CARD % 11) * 24) - 0x84,
		                  ((SELECTED_CARD / 11) * 24) - 0x40, 0x18, 0x14, 0, 0xe8,
		                  0x18, 0x7dc7, 5, 0);
	case 1:
		for (i = 0; i < 6; i++) {
			for (j = 0; j < 11; j++) {
				card = j + i * 11;
				amount = getCardAmount(card);
				if (amount > 0) {
					renderRectPolyFT4(j * 24 - 0x81, i * 24 - 0x3e, 0x10, 0x10,
					                  (card % 8) * 32, (card / 8) * 16, 0x18,
					                  sprites.v[card] * 64 + 0x7a07, 5, 0);
				}
				renderBorderBox(j * 24 + 0x1e, i * 24 + 0x39, 0x12, 0x12,
				                0xbebebe, 0x3c3c3c, 0x69, 0x69, 0x69, 5);
			}
		}

		renderString(3, -0x24, -0x50, 0x48, 0xc, 0, 0xc, 5, 1);
		renderString(FOOTER_GLYPH1_COLOR, -0x23, 0x53, 0xc, 0xc, 0x48, 0xc, 5, 1);
		renderString(0, -0x17, 0x53, 0x3c, 0xc, 0x54, 0xc, 5, 1);
		renderString(FOOTER_GLYPH2_COLOR, 0x2b, 0x53, 0xc, 0xc, 0x90, 0xc, 5, 1);
		renderString(0, 0x37, 0x53, 0x3c, 0xc, 0x9c, 0xc, 5, 1);
	}
}

void renderInsetBox(int16_t x, int16_t y, int16_t w, int16_t h, int32_t otz)
{
	x -= 0xa0;
	y -= 0x78;
	drawLine3P(0x20202, x, y + h, x, y, x + w, y, otz, 0);
	drawLine3P(0xa08769, x, y + h, x + w, y + h, x + w, y, otz, 0);
	renderBox(x, y, w, h, 0x35, 0x4b, 0x5c, 0, otz);
}

void renderBox(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t r,
               uint8_t g, uint8_t b, uint8_t flags, int32_t otz)
{
	GsBOXF box;

	if ((flags & 1) != 0) {
		if ((flags & 2) != 0) {
			box.attribute = 0x60000000;
		} else {
			box.attribute = 0x40000000;
		}
	} else {
		box.attribute = 0;
	}

	box.x = x;
	box.y = y;
	setWH(&box, w, h);
	box.r = r;
	box.g = g;
	box.b = b;

	GsSortBoxFill(&box, ACTIVE_ORDERING_TABLE, otz);

	if ((flags & 0x80) != 0) {
		drawLine3P(0x20202,
		           x - 1, y - 1,
		           x + 0xd, y - 1,
		           x + 0xd, y + 0xc,
		           otz, 0);
		drawLine3P(0x20202,
		           x - 1, y - 1,
		           x - 1, y + 0xc,
		           x + 0xd,
		           y + 0xc,
		           otz, 0);
	}
}

void renderBorderBox(int16_t x, int16_t y, int16_t w, int16_t h, int32_t c1,
                     int32_t c2, uint8_t r, uint8_t g, uint8_t b, int32_t a10)
{
	x -= 0xa0;
	y -= 0x78;
	drawLine3P(c1, x, y + h, x, y, x + w, y, a10, 0);
	drawLine3P(c2, x, y + h, x + w, y + h, x + w, y, a10, 0);
	renderBox(x, y, w, h, r, g, b, 0, a10);
}

int32_t drawDigimonStatsStrings(void)
{
	RECT rect;

	rect = MAIN_D_801342E0;

	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect);
		drawString(MAIN_D_80124C54, 0, 0xf0);
		drawString(MAIN_D_80124C24, 0, 0x24);
		drawString(MAIN_D_80124C30[0], 0x24, 0x24);
		MENU_SUB_STATE = 1;
		break;
	case 1:
#if defined(VERSION_JP)
		drawString(DIGIMON_DATA[ENTITY_TABLE[1]->type].name, 0x9c, 0x24);
#else
		drawString(DIGIMON_DATA[ENTITY_TABLE[1]->type].name, 0, 0x48);
#endif
		drawString(PARTNER_ENTITY.name, 0, 0x3c);
		MENU_SUB_STATE = 2;
		break;
	case 2:
		drawString(STATUS_VIEW_LABELS[1], 0, 0x30);
		MENU_SUB_STATE = 3;
		break;
	case 3:
		drawString(STATUS_VIEW_LABELS[2], 0, 0x18);

		return 1;
	}

	return 0;
}

void renderDigimonStatsBar(a, b, c, d, e)
	int16_t a;
int16_t b;
uint8_t c;
int16_t d;
int16_t e;
{
	uint8_t w;

	w = c * a / b;
	renderBox(d, e, w, 2, 0x32, 0xc8, 0xc8, 0, 5);
}

void renderDigimonStatusConditions(int32_t condition)
{
	ConditionMaskTable masks;
	int32_t i;
	uint8_t bobY;
	int16_t clut;

	masks = MAIN_D_80123DD0;
	if (PLAYTIME_FRAMES % 16 < 3) {
		bobY = 0x82;
	} else if (PLAYTIME_FRAMES % 16 < 5) {
		bobY = 0x80;
	} else if (PLAYTIME_FRAMES % 16 < 7) {
		bobY = 0x7d;
	} else if (PLAYTIME_FRAMES % 16 < 9) {
		bobY = 0x79;
	} else if (PLAYTIME_FRAMES % 16 < 11) {
		bobY = 0x7d;
	} else if (PLAYTIME_FRAMES % 16 < 13) {
		bobY = 0x80;
	} else {
		bobY = 0x82;
	}
	for (i = 0; i < 6; i++) {
		if (condition & masks.v[i]) {
			clut = 0x7a06;
			if ((i == 0) || (i == 4)) {
				clut = 0x7a86;
			}
			if (i == 5) {
				clut = 0x7a46;
			}
			renderRectPolyFT4(i * 0xf - 0x8a, bobY - 0x78, 0xc, 0xc, i * 0xc + 0x30, 0x8c, 5, clut, 5, 0);
		}
	}

	if (condition & 8) {
		renderRectPolyFT4(-0x30, bobY - 0x78, 0xc, 0xc, 0x60, 0xc8, 5, 0x7a06, 5, 0);
	}
}

void renderDigiviceEntity(entity, entityId)
	Entity *entity;
int8_t entityId;
{
	MATRIX m;
	VECTOR savedPos;
	SVECTOR savedRot;
	GsF_LIGHT lights[3];
	PositionData *pos;
	int32_t type;
	int32_t bone;
	int32_t count;
	int32_t i;
	uint8_t anim;

	FRAMEBUFFER_OT[0]->length = 9;
	FRAMEBUFFER_OT[0]->org = FRAMEBUFFER0_ORIGIN;
	FRAMEBUFFER_OT[1]->length = 9;
	FRAMEBUFFER_OT[1]->org = FRAMEBUFFER1_ORIGIN;

	GsSetProjection(0x200);
	GsSetRefView2(&MAIN_D_80123860);
	GsClearOt(0, 5, FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER]);

	for (i = 0; i < 3; i++) {
		lights[i].vx = lights[i].vy = lights[i].vz = (i == 0) ? 100 : 0;
		lights[i].r = lights[i].g = lights[i].b = 0x80;
		GsSetFlatLight(i, &lights[i]);
	}

	type = entity->type;
	anim = entity->anim.animId;
	pos = entity->posData;
	count = DIGIMON_DATA[type].boneCount;
	savedPos = pos->location;
	savedRot = pos->rotation;
	setEntityPosition(entityId, RAISE_DATA[type].viewX,
	                  RAISE_DATA[type].viewY,
	                  RAISE_DATA[type].viewZ);
	setEntityRotation(entityId, 0, 0, 0);
	setupEntityMatrix(entityId);
	startAnimation(entity, 0);
	tickAnimation(entity);
	m = GsWSMATRIX;

	for (bone = 0; bone < count; pos++, bone++) {
		if (pos->obj.tmd != NULL) {
			GsGetLw(pos->obj.coord2, &m);
			GsSetLightMatrix(&m);
			GsGetLs(pos->obj.coord2, &m);
			GsSetLsMatrix(&m);
			GsSortObject4(&pos->obj, FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER], 5,
			              getScratchAddr(0));
		}
	}

	GsSortOt(FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);
	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);

	for (i = 0; i < 3; i++) {
		GsSetFlatLight(i, &LIGHT_DATA[i]);
	}

	setEntityPosition(entityId, savedPos.vx, savedPos.vy, savedPos.vz);
	setEntityRotation(entityId, savedRot.vx, savedRot.vy, savedRot.vz);
	setupEntityMatrix(entityId);
	startAnimation(entity, anim);
	tickAnimation(entity);
}

void renderDigiviceMedals(void)
{
	MATRIX m;

	m = GsWSMATRIX;
	FRAMEBUFFER_OT[0]->length = 9;
	FRAMEBUFFER_OT[0]->org = FRAMEBUFFER0_ORIGIN;
	FRAMEBUFFER_OT[1]->length = 9;
	FRAMEBUFFER_OT[1]->org = FRAMEBUFFER1_ORIGIN;

	GsSetProjection(0x400);
	GsSetRefView2(&MAIN_D_80123880);
	GsClearOt(0, 1, FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER]);

	MAIN_D_80134238.vy += 0x64;
	RotMatrix(&MAIN_D_80134238, &MEDAL_COORDINATES.coord);
	MEDAL_COORDINATES.flg = 0;
	drawObject(&MEDAL_OBJECT, FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER], 5);
	GsSortOt(FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER], ACTIVE_ORDERING_TABLE);

	GsSetProjection(VIEWPORT_DISTANCE);
	GsSetRefView2(&GS_VIEWPOINT);
}

int32_t drawDigimonMovesText(void)
{
	RECT rect;

	rect = MOVES_VIEW_TEXT_AREA;
	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect);
		drawString(MAIN_D_80124C54, 0, 0xf0);
		if (PARTNER_ENTITY.digimonEntity.stats.base.moves[0] != 0xff) {
			EQUIPPED_MOVES[0] = entityGetTechFromAnim(ENTITY_TABLE[1],
			                                          PARTNER_ENTITY.digimonEntity.stats.base.moves[0]);
			drawString(MOVE_NAMES[EQUIPPED_MOVES[0]], 0, 0x18);
		} else {
			EQUIPPED_MOVES[0] = 0xff;
		}
		MENU_SUB_STATE = 1;
		break;
	case 1:
		drawString(STATUS_VIEW_LABELS[3], 0, 0x48);
		if (PARTNER_ENTITY.digimonEntity.stats.base.moves[1] != 0xff) {
			EQUIPPED_MOVES[1] = entityGetTechFromAnim(ENTITY_TABLE[1],
			                                          PARTNER_ENTITY.digimonEntity.stats.base.moves[1]);
			drawString(MOVE_NAMES[EQUIPPED_MOVES[1]], 0, 0x24);
		} else {
			EQUIPPED_MOVES[1] = 0xff;
		}
		MENU_SUB_STATE = 2;
#if !defined(VERSION_JP)
		DrawSync(0);
#endif
		break;
	case 2:
		drawString(STATUS_VIEW_LABELS[6], 0x84, 0x6c);
		if (PARTNER_ENTITY.digimonEntity.stats.base.moves[2] != 0xff) {
			EQUIPPED_MOVES[2] = entityGetTechFromAnim(ENTITY_TABLE[1],
			                                          PARTNER_ENTITY.digimonEntity.stats.base.moves[2]);
			drawString(MOVE_NAMES[EQUIPPED_MOVES[2]], 0, 0x30);
		} else {
			EQUIPPED_MOVES[2] = 0xff;
		}
		MENU_SUB_STATE = 3;
#if !defined(VERSION_JP)
		DrawSync(0);
#endif
		break;
	case 3:
		if (PARTNER_ENTITY.digimonEntity.stats.base.moves[3] != 0xff) {
			EQUIPPED_MOVES[3] = entityGetTechFromAnim(ENTITY_TABLE[1],
			                                          PARTNER_ENTITY.digimonEntity.stats.base.moves[3]);
			drawString(MOVE_NAMES[EQUIPPED_MOVES[3]], 0, 0x3c);
		} else {
			EQUIPPED_MOVES[3] = 0xff;
		}
		drawString(STATUS_VIEW_LABELS[4], 0, 0x6c);
		MENU_SUB_STATE = 4;
#if !defined(VERSION_JP)
		DrawSync(0);
#endif
		break;
	case 4:
		drawString(STATUS_VIEW_LABELS[5], 0, 0x54);
		MENU_SUB_STATE = 5;
		break;
	case 5:
		drawString(STATUS_VIEW_LABELS[7], 0, 0x60);
		drawString(TECH_VIEW_LABELS[0], 0, 0x78);
		MAIN_D_80134D3A = 0x73;
		MAIN_D_80134D38 = 0x6f;
		return 1;
	}
	return 0;
}

int32_t drawPlayerInfoStrings(void)
{
	RECT rect;
#if !defined(VERSION_JP)
	char buf[8];
#endif
	int32_t i;

	rect = PLAYER_INFO_TEXT_AREA;
	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect);
		drawString(MAIN_D_80124C54, 0, 0xf0);
		MENU_SUB_STATE = 1;
		break;
	case 1:
#if !defined(VERSION_JP)
		sprintf(buf, FMT_PLAYTIME, (int)PLAYTIME_HOURS, (int)PLAYTIME_MINUTES);
		drawString(buf, 0, 0xe4);
#endif
	case 2:
	case 3:
		drawString(PLAYER_VIEW_LABELS[MENU_SUB_STATE + 1], 0,
		           (MENU_SUB_STATE - 1) * 0xc + 0xc);
		MENU_SUB_STATE++;
		break;
	case 4:
		drawString((char *)DIGIMON_DATA, 0, 0x30);
		for (i = 0; i < 4; i++) {
			drawString(TAMER_LEVEL_TITLES[i], 0, i * 0xc + 0x40);
		}
		MENU_SUB_STATE = 5;
		break;
	case 5:
		for (i = 4; i < 0xb; i++) {
			drawString(TAMER_LEVEL_TITLES[i], 0, i * 0xc + 0x40);
		}
		return 1;
	}
	return 0;
}

int32_t drawEvoChartStrings(int8_t arg)
{
	RECT rect1;
	RECT rect2;

	rect1 = EVO_CHART_TEXT_AREA;
	rect2 = EVO_CHART_NAME_AREA;
	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect1);
		drawString(MAIN_D_80124C54, 0, 0xf0);
		drawString(CARD_CHART_LABELS[1], 0, 0xc);
		MAIN_D_80134D42 = MAIN_D_80134D44 = 0;
		MENU_SUB_STATE = 1;
		/* fall through */
	case 1:
		drawString(CARD_CHART_LABELS[2], 0, 0x18);
#if !defined(VERSION_JP)
		drawString(CARD_CHART_LABELS[3], 0, 0x24);
		drawString(CARD_CHART_LABELS[4], 0, 0x3c);
#endif
		return 1;
	case 2:
		clearTextSubArea(&rect2);
		drawString((char *)(DIGIMON_DATA + arg), 0, 0x30);
	}
	return 0;
}

int32_t drawMedalViewStrings(void)
{
	RECT rect1;
	RECT rect2;

	rect1 = MEDAL_VIEW_TEXT_AREA;
	rect2 = MEDAL_DETAIL_AREA;
	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect1);
		drawString(MAIN_D_80124C54, 0, 0xf0);
		MENU_SUB_STATE = 1;
		break;
	case 1:
		drawString(PLAYER_VIEW_LABELS[1], 0, 0xc);
		MENU_SUB_STATE = 2;
		break;
	case 2:
		drawString(MEDAL_NAMES[MEDAL_SELECTOR_INDEX], 0, 0x18);
		MENU_SUB_STATE = 3;
		break;
	case 3:
	case 4:
	case 5:
		drawString(MEDAL_DESCRIPTIONS[(MENU_SUB_STATE - 3) + MEDAL_SELECTOR_INDEX * 3], 0,
		           (MENU_SUB_STATE - 3) * 0xc + 0x24);
		MENU_SUB_STATE++;
		if (MENU_SUB_STATE == 6) {
			return 1;
		}
		break;
	case 6:
		clearTextSubArea(&rect2);
		MENU_SUB_STATE = 2;
	}
	return 0;
}

int32_t drawCardViewStrings(void)
{
	RECT rect;

	rect = CARD_VIEW_TEXT_AREA;
	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect);
		drawString(MAIN_D_80124C54, 0, 0xF0);
		MENU_SUB_STATE = 1;
		break;
	case 1:
		drawString(CARD_CHART_LABELS[0], 0, 0xC);
		SELECTED_CARD = 0;
		return 1;
	}
	return 0;
}

void renderSeparatorLines(int16_t *lines, int8_t count, int32_t zIndex)
{
	uint8_t *color;
	int32_t i;

	for (i = 0; i < count; lines += 5ULL, i++) {
		color = &MAIN_D_80123F48[((uint8_t *)lines)[8] * 3];
		drawLine2P((color[0] & 0xff) | ((color[1] & 0xff) << 8) |
		                            ((color[2] & 0xff) << 16),
		           lines[0], lines[1], lines[2], lines[3],
		           zIndex, 0);
	}
}

void renderRectPolyFT4(int16_t posX, int16_t posY, uint8_t width,
                       uint8_t height, uint8_t texX, uint8_t texY,
                       int16_t texturePage, int16_t clut, int32_t zIndex,
                       int8_t flag)
{
	POLY_FT4 *prim;
	GsOT_TAG *tags;

	tags = ACTIVE_ORDERING_TABLE->org;
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	if (flag & 0x40) {
		SetSemiTrans(prim, 1);
	}
	if ((texY + height) >= 0x100) {
		height--;
	}
	if ((texX + width) >= 0x100) {
		width--;
	}
	setUVDataPolyFT4(prim, texX, texY, width, height);
	if (flag & 2) {
		setPosDataPolyFT4(prim, posX, posY, width * 2, height * 2);
	} else if (flag & 4) {
		setPosDataPolyFT4(prim, posX, posY, width * 2, height * 2);
		prim->x0 += width * 2;
		prim->x1 -= width * 2;
		prim->x2 += width * 2;
		prim->x3 -= width * 2;
	} else {
		setPosDataPolyFT4(prim, posX, posY, width, height);
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
		drawLine3P(0x20202, posX - 1, posY - 1, posX + 0xd,
		           posY - 1, posX + 0xd, posY + 0xc, 2, 0);
		drawLine3P(0x20202, posX - 1, posY - 1, posX - 1,
		           posY + 0xc, posX + 0xd, posY + 0xc, 2, 0);
	}
}
