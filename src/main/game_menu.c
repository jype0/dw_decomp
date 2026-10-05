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
extern int32_t HAS_FISHING_ROD;
extern int8_t MENU_STATE;
extern int8_t DIGIMON_MENU_STATE;
extern int8_t PLAYER_MENU_STATE;
extern int16_t MOVE_SELECT_BOX_Y;
extern int16_t MOVE_SELECT_BOX_X;
extern int32_t CHANGED_INPUT;
extern uint8_t INVENTORY_POINTER;
extern uint32_t POLLED_INPUT;
extern uint32_t POLLED_INPUT_PREVIOUS;
extern int32_t MENU_OPTION_COUNT;
extern int8_t SELECTED_MEDAL;
extern int8_t MEDAL_SELECTOR_INDEX;
extern int8_t SELECTED_CARD;
extern int16_t CHART_SELECTED_DIGIMON;
extern int16_t CHART_SELECTED_COLUMN;
extern int16_t CHART_SELECTED_ROW;
extern int8_t MAIN_D_80134D46;
extern int16_t MAIN_D_80134D48;
extern int32_t MONEY;
extern uint16_t PLAYTIME_FRAMES;
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT_TAG *FRAMEBUFFER0_ORIGIN;
extern GsOT_TAG *FRAMEBUFFER1_ORIGIN;
extern GsOT *FRAMEBUFFER_OT[2];
extern GsRVIEW2 GS_VIEWPOINT;
extern int32_t VIEWPORT_DISTANCE;
extern GsF_LIGHT LIGHT_DATA[3];
extern char MAIN_D_80124C0C[][12];
extern char MAIN_D_80124C54[];
extern char *MOVE_NAMES[];

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

SVECTOR MEDAL_ROTATION = { 0x0000, 0x0000, 0x0000, 0x0000 };

#if !defined(VERSION_JP)
char MAIN_D_80134240[] = "Current";

char MAIN_D_80134248[] = "Ending";
#endif

char STR_OVERWORLD_EMPTY[] = "";

TriangleCursorUVData SELECTION_CURSOR_U_MIN = { { 0x00, 0x04, 0x00, 0x04, 0x04, 0x04, 0x08, 0x08 } };

TriangleCursorUVData SELECTION_CURSOR_U_MAX = { { 0x04, 0x00, 0x04, 0x00, 0x08, 0x08, 0x0c, 0x0c } };

TriangleCursorUVData SELECTION_CURSOR_V_MIN = { { 0xfb, 0xfb, 0xff, 0xff, 0xfb, 0xfb, 0xfb, 0xfb } };

TriangleCursorUVData SELECTION_CURSOR_V_MAX = { { 0xff, 0xff, 0xfb, 0xfb, 0xff, 0xff, 0xff, 0xff } };

TriangleCursorOffsetData SELECTION_CURSOR_X = { { 0x00, 0x18, 0x00, 0x18, 0x04, 0x04, 0x00, 0x19 } };

TriangleCursorOffsetData SELECTION_CURSOR_Y = { { 0x00, 0x00, 0x16, 0x16, 0x00, 0x16, 0x00, 0x00 } };

TriangleCursorOffsetData SELECTION_CURSOR_WIDTH = { { 0x04, 0x04, 0x04, 0x04, 0x18, 0x18, 0x04, 0x04 } };

TriangleCursorOffsetData SELECTION_CURSOR_HEIGHT = { { 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x16, 0x16 } };

RECT MENU_TEXT_AREA = { 0, 232, 24, 12 };

#if defined(VERSION_JP)
char STR_YEAR_DAY[] = "年日";
#else
char STR_YEAR_DAY[8] = "YearDay";
#endif

DigimonTabs DIGIMON_MENU_VIEWS = { { 0x01, 0x01 } };

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

RECT DIGIMON_STATS_TEXT_AREA = { 0, 24, 256, 200 };

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

GsRVIEW2 DIGIVICE_ENTITY_VIEW = {
	1300, 0, -3280, 0, 0, 0, 0, NULL,
};

GsRVIEW2 MEDAL_VIEW = {
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

StatsIconClutTable STATS_VIEW_ELEMENT_CLUT = { {
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

EvoClutTable EVO_CHART_VIEW_COLORS = { {
	0x7a07, 0x7a47, 0x7a87, 0x7ac7, 0x7b07,
} };

CardSprites CARD_SPRITE_CLUT = { {
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

EvoClutTable EVO_CHART_DETAIL_COLORS = { {
	0x7a07, 0x7a47, 0x7a87, 0x7ac7, 0x7b07,
} };

char MAIN_D_80123E78[] = "\\CARD\\CARD.ALL";

GameMenuSprite MENU_OPTIONS[8] = {
	{ 0x10, 0x1c, 1, 1, 0xc, 0x18, 0x18, 0x0, 0x30 },
	{ 0x14, 0x47, 1, 0, 0xc, 0x14, 0x14, 0x0, 0x0 },
	{ 0x38, 0x47, 2, 0, 0xd, 0x14, 0x14, 0x28, 0x0 },
	{ 0x5b, 0x47, 3, 0, 0xc, 0x14, 0x14, 0x0, 0x14 },
	{ 0x14, 0x6f, 4, 0, 0xd, 0x14, 0x14, 0x28, 0x14 },
	{ 0x38, 0x6f, 5, 0, 0xd, 0x14, 0x14, 0x0, 0x28 },
	{ 0x5b, 0x6f, 6, 0, 0xd, 0x14, 0x14, 0x28, 0x28 },
	{ 0x14, 0x1f, 7, 0, 0xd, 0x14, 0x13, 0x50, 0x0 },
};

uint8_t GAME_MENU_TEXT_SPRITES[64] = {
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

uint8_t UI_LINES_BORDER_COLOR[12] = {
	0x02, 0x02, 0x02, 0x90, 0xd9, 0xfa, 0x4a, 0x9f,
	0xc5, 0xb4, 0x96, 0x69,
};

int16_t STATS_VIEW_LINES[30] = {
	0xffe5, 0xfff1, 0xffe5, 0x0063, 0x0000, 0xffe6, 0xfff0, 0xffe6,
	0x0064, 0x0001, 0xffe5, 0xfff1, 0xffe7, 0x0063, 0x0000, 0xff6e,
	0xffee, 0x0092, 0xffee, 0x0000, 0xff6d, 0xffef, 0x0093, 0xffef,
	0x0001, 0xff6e, 0xfff0, 0x0092, 0xfff0, 0x0000,
};

int16_t MOVES_TECHSET_LINES[30] = {
	0xff6e, 0xffeb, 0x0092, 0xffeb, 0x0000, 0xff6d, 0xffec, 0x0093,
	0xffec, 0x0001, 0xff6e, 0xffed, 0x0092, 0xffed, 0x0000, 0xff6e,
	0x0010, 0x0092, 0x0010, 0x0000, 0xff6d, 0x0011, 0x0093, 0x0011,
	0x0001, 0xff6e, 0x0012, 0x0093, 0x0012, 0x0000,
};

int16_t MOVES_VIEW_HELP_LINES[60] = {
	0xff76, 0x001a, 0xffad, 0x001a, 0x0000, 0xff76, 0x001b, 0xffad,
	0x001b, 0x0001, 0xff76, 0x001c, 0xffad, 0x001c, 0x0000, 0xff72,
	0x001e, 0xff72, 0x0059, 0x0000, 0xff73, 0x001e, 0xff73, 0x0059,
	0x0001, 0xff74, 0x001e, 0xff74, 0x0059, 0x0000, 0xffae, 0x001e,
	0xffae, 0x0059, 0x0000, 0xffaf, 0x001e, 0xffaf, 0x0059, 0x0001,
	0xffb0, 0x001e, 0xffb0, 0x0059, 0x0000, 0xff76, 0x005b, 0xffad,
	0x005b, 0x0000, 0xff76, 0x005c, 0xffad, 0x005c, 0x0001, 0xff76,
	0x005d, 0xffad, 0x005d, 0x0000,
};

IconRect MOVES_VIEW_HELP_CORNERS[4] = {
	{ 0xff72, 0x001a, 0x04, 0x04, 0x78, 0x10 },
	{ 0xffad, 0x001a, 0x04, 0x04, 0x7c, 0x10 },
	{ 0xff72, 0x0059, 0x04, 0x04, 0x78, 0x14 },
	{ 0xffad, 0x0059, 0x04, 0x04, 0x7c, 0x14 },
};

int16_t DIGIMON_MOVE_INFO_LINES_LEFT[90] = {
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

int16_t DIGIMON_MOVE_INFO_LINES_RIGHT[90] = {
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

IconRect MOVES_VIEW_TECHSET_TEXT[13] = {
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
StringRect MOVES_VIEW_STRING_SPRITES[12] = {
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

IconRect MOVES_VIEW_SPRITES[7] = {
	{ 0x0030, 0xffbf, 0x0c, 0x0c, 0x24, 0x00 },
	{ 0xffe8, 0xffde, 0x0c, 0x0c, 0x54, 0x00 },
	{ 0xff94, 0xffec, 0x0c, 0x0c, 0x24, 0x00 },
	{ 0xff88, 0x0002, 0x0c, 0x0c, 0x54, 0x00 },
	{ 0xff88, 0x0012, 0x0c, 0x0c, 0x24, 0x00 },
	{ 0xff88, 0x0032, 0x0c, 0x0c, 0x6c, 0x00 },
	{ 0xff88, 0x0052, 0x0c, 0x0c, 0xb4, 0x00 },
};
#else
StringRect MOVES_VIEW_STRING_SPRITES[12] = {
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

IconRect MOVES_VIEW_SPRITES[7] = {
	{ 0x0005, 0xffbf, 0x0c, 0x0c, 0x24, 0x00 },
	{ 0x0052, 0xffde, 0x0c, 0x0c, 0x54, 0x00 },
	{ 0xffbe, 0xffec, 0x0c, 0x0c, 0x24, 0x00 },
	{ 0xff88, 0x0002, 0x0c, 0x0c, 0x54, 0x00 },
	{ 0xff88, 0x0012, 0x0c, 0x0c, 0x24, 0x00 },
	{ 0xff88, 0x0032, 0x0c, 0x0c, 0x6c, 0x00 },
	{ 0xff88, 0x0052, 0x0c, 0x0c, 0xb4, 0x00 },
};
#endif

RECT STATS_VIEW_INSETS[13] = {
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
StringRect STATS_VIEW_TEXT[9] = {
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
StringRect STATS_VIEW_TEXT[9] = {
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

IconRect STATS_VIEW_ELEMENTS[21] = {
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

int16_t TAMER_VIEW_PLAYER_LINES[55] = {
	0xff6e, 0xffbf, 0x0092, 0xffbf, 0x0000, 0xff6d, 0xffc0, 0x0093,
	0xffc0, 0x0001, 0xff6e, 0xffc1, 0x0092, 0xffc1, 0x0000, 0xffd3,
	0xffc1, 0x0092, 0xffc1, 0x0000, 0xffd1, 0xffc1, 0xffd1, 0x0062,
	0x0000, 0xffd2, 0xffc0, 0xffd2, 0x0063, 0x0001, 0xffd3, 0xffc1,
	0xffd3, 0x002b, 0x0000, 0xffd2, 0x002c, 0x0091, 0x002c, 0x0000,
	0xffd3, 0x002d, 0x0093, 0x002d, 0x0001, 0xffd3, 0x002e, 0x0091,
	0x002e, 0x0000, 0xffd3, 0x002f, 0xffd3, 0x0064, 0x0000,
};

RECT TAMER_WINDOW_BOXES[11] = {
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
StringRect TAMER_WINDOW_NAME_STRING[11] = {
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
StringRect TAMER_WINDOW_NAME_STRING[11] = {
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

EvoChartEntry EVO_CHART_BOXES[61] = {
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

Line4Points CHART_FROM_LINES_EVEN[4] = {
	{ 0xffac, 0xffbc, 0xffdb, 0xfff6, 0xffd0, 0xffd0, 0xffef, 0xffef },
	{ 0xff9a, 0xffb6, 0xffc0, 0xfff6, 0xffe8, 0xffe8, 0xfff2, 0xfff2 },
	{ 0xff9a, 0xffb6, 0xffc0, 0xfff6, 0xffff, 0xffff, 0xfff5, 0xfff5 },
	{ 0xffac, 0xffbc, 0xffdb, 0xfff6, 0x0017, 0x0017, 0xfff8, 0xfff8 },
};

Line4Points CHART_FROM_LINES_ODD[5] = {
	{ 0xffbe, 0xffcc, 0xfff6, 0xfff6, 0xffc4, 0xffc4, 0xffee, 0xffee },
	{ 0xffac, 0xffc6, 0xffdb, 0xfff6, 0xffdc, 0xffdc, 0xfff1, 0xfff1 },
	{ 0xff9a, 0xfff6, 0xfff6, 0xfff6, 0xfff4, 0xfff4, 0xfff4, 0xfff4 },
	{ 0xffac, 0xffc6, 0xffdb, 0xfff6, 0x000c, 0x000c, 0xfff7, 0xfff7 },
	{ 0xffbe, 0xffcc, 0xfff6, 0xfff6, 0x0024, 0x0024, 0xfffa, 0xfffa },
};

Line4Points CHART_TO_LINES_EVEN[6] = {
	{ 0x0009, 0x003d, 0x0040, 0x0040, 0xffec, 0xffb8, 0xffb8, 0xffb8 },
	{ 0x0009, 0x0024, 0x0043, 0x0052, 0xffef, 0xffef, 0xffd0, 0xffd0 },
	{ 0x0009, 0x003f, 0x0049, 0x0064, 0xfff2, 0xfff2, 0xffe8, 0xffe8 },
	{ 0x0009, 0x003f, 0x0049, 0x0064, 0xfff5, 0xfff5, 0xffff, 0xffff },
	{ 0x0009, 0x0024, 0x0043, 0x0052, 0xfff8, 0xfff8, 0x0017, 0x0017 },
	{ 0x0009, 0x003d, 0x0040, 0x0040, 0xfffb, 0x002f, 0x002f, 0x002f },
};

Line4Points CHART_TO_LINES_ODD[5] = {
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

ChartSprite CHART_FROM_SPRITES_EVEN[4] = {
	{ 0xff9a, 0xffc7, 0x0000, 0x0000 },
	{ 0xff88, 0xffdf, 0x0000, 0x0000 },
	{ 0xff88, 0xfff7, 0x0000, 0x0000 },
	{ 0xff9a, 0x000f, 0x0000, 0x0000 },
};

ChartSprite CHART_FROM_SPRITES_ODD[5] = {
	{ 0xffac, 0xffbb, 0x0000, 0x0000 },
	{ 0xff9a, 0xffd3, 0x0000, 0x0000 },
	{ 0xff88, 0xffeb, 0x0000, 0x0000 },
	{ 0xff9a, 0x0003, 0x0000, 0x0000 },
	{ 0xffac, 0x001b, 0x0000, 0x0000 },
};

ChartSprite CHART_TO_SPRITES_EVEN[6] = {
	{ 0x0042, 0xffaf, 0x0000, 0x0000 },
	{ 0x0054, 0xffc7, 0x0000, 0x0000 },
	{ 0x0066, 0xffdf, 0x0000, 0x0000 },
	{ 0x0066, 0xfff7, 0x0000, 0x0000 },
	{ 0x0054, 0x0010, 0x0000, 0x0000 },
	{ 0x0042, 0x0027, 0x0000, 0x0000 },
};

ChartSprite CHART_TO_SPRITES_ODD[5] = {
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
// clang-format on

GARBAGE(addGameMenu, 14);

void addGameMenu(void)
{
	MENU_OPTIONS[0].unknown = 1;
	MENU_OPTION_COUNT = 7;
	MENU_OPTIONS[7].disabled = 0;
	HAS_FISHING_ROD = getFishingAvailability();
	if (HAS_FISHING_ROD != 0) {
		MENU_OPTION_COUNT++;
		if (HAS_FISHING_ROD == 1) {
			MENU_OPTIONS[7].disabled = 1;
		}
		MENU_OPTIONS[0].unknown = 7;
	}
	if (PARTNER_PARA.condition & 1) {
		MENU_OPTIONS[6].disabled = 0;
	} else {
		MENU_OPTIONS[6].disabled = 1;
	}
	TRIANGLE_MENU_STATE = 0;
	addObject(0xfa4, 0, (void (*)(int32_t))tickTriangleMenu, NULL);
}

void tickTriangleMenu(void)
{
	RECT rect;

	rect = MENU_TEXT_AREA;
	switch (TRIANGLE_MENU_STATE) {
	case 0:
		if (HAS_FISHING_ROD != 0) {
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
			DIGIMON_MENU_STATE = 0;
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
			PLAYER_MENU_STATE = 0;
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
	if (MENU_OPTION_COUNT == 7) {
		yOffset = -0x28;
	}
	sprite = &MENU_OPTIONS[1];
	renderTriangleCursor((int8_t)MENU_OPTIONS[0].unknown, yOffset);
	for (i = 1; i < MENU_OPTION_COUNT; sprite++, i++) {
		disabled = 0;
		if (sprite->disabled == 1) {
			disabled = 1;
		}
		highlight = 0;
		if ((i == MENU_OPTIONS[0].unknown) &&
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
		renderRectPolyFT4(((GameMenuLabel *)GAME_MENU_TEXT_SPRITES)[i].x - 0x42,
		                  yOffset + (((GameMenuLabel *)GAME_MENU_TEXT_SPRITES)[i].y - 0x50),
		                  ((GameMenuLabel *)GAME_MENU_TEXT_SPRITES)[i].width,
		                  ((GameMenuLabel *)GAME_MENU_TEXT_SPRITES)[i].height,
		                  ((GameMenuLabel *)GAME_MENU_TEXT_SPRITES)[i].texX,
		                  ((GameMenuLabel *)GAME_MENU_TEXT_SPRITES)[i].texY + 0xbf, 0x1e, 0x7f50, 6,
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

	u0 = SELECTION_CURSOR_U_MIN;
	u1 = SELECTION_CURSOR_U_MAX;
	v0 = SELECTION_CURSOR_V_MIN;
	v1 = SELECTION_CURSOR_V_MAX;
	xOffset = SELECTION_CURSOR_X;
	yOffsetData = SELECTION_CURSOR_Y;
	width = SELECTION_CURSOR_WIDTH;
	height = SELECTION_CURSOR_HEIGHT;
	baseX = MENU_OPTIONS[selection].x -
	        0x46;
	baseY = MENU_OPTIONS[selection].y -
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
		MENU_OPTIONS[6].disabled = 0;
	}
	selection = MENU_OPTIONS[0].unknown;
	if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x1000) {
		if ((selection -= 3) <= 0) {
			selection += ((MENU_OPTION_COUNT + 1) / 3) * 3;
		}
		if (selection >= MENU_OPTION_COUNT) {
			selection -= 3;
		}
	} else if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x4000) {
		if ((selection += 3) >= MENU_OPTION_COUNT) {
			selection -= ((MENU_OPTION_COUNT + 1) / 3) * 3;
		}
		if (selection <= 0) {
			selection += 3;
		}
	} else if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x8000) {
		if ((selection -= 1) <= 0) {
			selection = MENU_OPTION_COUNT - 1;
		}
	} else if ((POLLED_INPUT & ~POLLED_INPUT_PREVIOUS) & 0x2000) {
		if ((selection += 1) >= MENU_OPTION_COUNT) {
			selection = 1;
		}
	}
	if (selection != MENU_OPTIONS[0].unknown) {
		MENU_OPTIONS[0].unknown = selection;
		playSound(0, 2);
	}
	if (TRIANGLE_MENU_STATE == -1) {
		if (isKeyDown(CONFIRM_BUTTON) != 0) {
			if (MENU_OPTIONS[MENU_OPTIONS[0].unknown].disabled & 1) {
				playSound(0, 4);
			} else {
				playSound(0, 3);
			}
			handleGameMenuSelection(MENU_OPTIONS[0].unknown);
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

	if ((DIGIMON_MENU_STATE != 1) ||
	    ((DIGIMON_MENU_STATE == 1) && (MENU_STATE == 1))) {
		if ((CHANGED_INPUT & 0x2000) && (MENU_STATE != 0)) {
			DIGIMON_MENU_STATE++;
			if (DIGIMON_MENU_STATE >= 2) {
				DIGIMON_MENU_STATE = 1;
			} else {
				MENU_STATE = 0;
				MENU_SUB_STATE = 0;
				playSound(0, 2);
			}
		}
		if ((CHANGED_INPUT & 0x8000) && (MENU_STATE != 0)) {
			DIGIMON_MENU_STATE--;
			if (DIGIMON_MENU_STATE < 0) {
				DIGIMON_MENU_STATE = 0;
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
		if ((DIGIMON_MENU_STATE == 1) && (isKeyDown(CONFIRM_BUTTON) != 0)) {
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
		previousX = MOVE_SELECT_BOX_X;
		previousY = MOVE_SELECT_BOX_Y;
		if (CHANGED_INPUT & 0x1000) {
			MOVE_SELECT_BOX_Y -= 0xf;
		}
		if (CHANGED_INPUT & 0x4000) {
			MOVE_SELECT_BOX_Y += 0xf;
		}
		if (CHANGED_INPUT & 0x8000) {
			MOVE_SELECT_BOX_X -= 0x12;
		}
		if (CHANGED_INPUT & 0x2000) {
			MOVE_SELECT_BOX_X += 0x12;
		}
		if (MOVE_SELECT_BOX_X < 0x73) {
			MOVE_SELECT_BOX_X = 0x73;
		}
		if (MOVE_SELECT_BOX_X >= 0xf2) {
			MOVE_SELECT_BOX_X = 0xf1;
		}
		if (MOVE_SELECT_BOX_Y < 0x6f) {
			MOVE_SELECT_BOX_Y = 0x6f;
		}
		if (MOVE_SELECT_BOX_Y >= 0xca) {
			MOVE_SELECT_BOX_Y = 0xc9;
		}
		if ((previousX != MOVE_SELECT_BOX_X) ||
		    (previousY != MOVE_SELECT_BOX_Y)) {
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

	tabs = DIGIMON_MENU_VIEWS;
	switch (DIGIMON_MENU_STATE) {
	case 0:
		renderDigimonStatsView();
		break;
	case 1:
		renderDigimonMovesView();
		break;
	}
	tabs.tab[DIGIMON_MENU_STATE] = 0;
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
			PLAYER_MENU_STATE++;
			if (PLAYER_MENU_STATE >= 4) {
				PLAYER_MENU_STATE = 3;
			} else {
				MENU_STATE = 0;
				MENU_SUB_STATE = 0;
				playSound(0, 2);
			}
		}
		if (CHANGED_INPUT & 0x8000) {
			PLAYER_MENU_STATE--;
			if (PLAYER_MENU_STATE < 0) {
				PLAYER_MENU_STATE = 0;
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
		    (PLAYER_MENU_STATE != 0) && (PLAYER_MENU_STATE != 4)) {
			MENU_STATE = 2;
			playSound(0, 3);
			MEDAL_SELECTOR_INDEX = SELECTED_MEDAL = 0;
			SELECTED_CARD = 0;
		}
	} else {
		if ((isKeyDown(CANCEL_BUTTON) != 0) && (MENU_STATE == 2)) {
			playSound(0, 4);
			MENU_STATE = 1;
			CHART_SELECTED_DIGIMON = -1;
		}

		if (PLAYER_MENU_STATE == 1) {
			if (MENU_STATE == 2) {
				previousRow = CHART_SELECTED_COLUMN;
				previousColumn = CHART_SELECTED_ROW;
				if (CHANGED_INPUT & 0x8000) {
					CHART_SELECTED_COLUMN--;
				}
				if (CHANGED_INPUT & 0x2000) {
					CHART_SELECTED_COLUMN++;
				}
				if (CHART_SELECTED_COLUMN < 0) {
					CHART_SELECTED_COLUMN = 0;
				}
				if (CHART_SELECTED_COLUMN >= 9) {
					CHART_SELECTED_COLUMN = 8;
				}
				if ((CHART_SELECTED_COLUMN == 0) ||
				    (CHART_SELECTED_COLUMN == 1)) {
					if (CHANGED_INPUT & 0x1000) {
						CHART_SELECTED_ROW -= 2;
					}
					if (CHANGED_INPUT & 0x4000) {
						CHART_SELECTED_ROW += 2;
					}
					if (CHART_SELECTED_COLUMN < 2) {
						CHART_SELECTED_ROW =
							(CHART_SELECTED_ROW / 2) * 2;
					}
					maxColumn = 6;
				} else {
					if (CHANGED_INPUT & 0x1000) {
						CHART_SELECTED_ROW--;
					}
					if (CHANGED_INPUT & 0x4000) {
						CHART_SELECTED_ROW++;
					}
					if (CHART_SELECTED_COLUMN == 2) {
						maxColumn = 8;
					} else if (((CHART_SELECTED_COLUMN >= 4) &&
					            (CHART_SELECTED_COLUMN < 7)) ||
					           (CHART_SELECTED_COLUMN == 8)) {
						maxColumn = 6;
					} else {
						maxColumn = 7;
					}
				}
				if (CHART_SELECTED_ROW < 0) {
					CHART_SELECTED_ROW = 0;
				}
				if (maxColumn < CHART_SELECTED_ROW) {
					CHART_SELECTED_ROW = maxColumn;
				}
				if ((previousRow != CHART_SELECTED_COLUMN) ||
				    (previousColumn != CHART_SELECTED_ROW)) {
					playSound(0, 2);
				}

				if (CHART_SELECTED_COLUMN < 3) {
					selectorX = CHART_SELECTED_COLUMN * 0x25 + 0x1c;
				} else if (CHART_SELECTED_COLUMN < 7) {
					selectorX =
						(CHART_SELECTED_COLUMN - 3) * 0x18 + 0x8b;
				} else {
					selectorX =
						(CHART_SELECTED_COLUMN - 7) * 0x18 + 0xf8;
				}
				CHART_SELECTED_DIGIMON = -1;
				for (i = 0; i < 0x3e; i++) {
					if (((selectorX + 2) ==
					     EVO_CHART_BOXES[i].posX) &&
					    ((CHART_SELECTED_ROW * 0x13 + 0x2b) ==
					     EVO_CHART_BOXES[i].posY)) {
						break;
					}
				}
				CHART_SELECTED_DIGIMON = i + 1;
				if (isKeyDown(CONFIRM_BUTTON) != 0) {
					if (hasDigimonRaised(
						    (uint16_t)CHART_SELECTED_DIGIMON) != 0) {
						if (CHART_SELECTED_COLUMN < 3) {
							selectorX =
								CHART_SELECTED_COLUMN * 0x25 + 0x1c;
						} else if (CHART_SELECTED_COLUMN < 7) {
							selectorX =
								(CHART_SELECTED_COLUMN - 3) * 0x18 + 0x8b;
						} else {
							selectorX =
								(CHART_SELECTED_COLUMN - 7) * 0x18 + 0xf8;
						}
						if (isUIBoxAvailable(2) == 1) {
							setRECT(&finalPos, -0x96, -0x59, 0x12c, 0xbe);
							startPos.x = selectorX - 0x99;
							startPos.y =
								CHART_SELECTED_ROW * 0x13 - 0x45;
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
					if (CHART_SELECTED_COLUMN < 3) {
						selectorX = CHART_SELECTED_COLUMN * 0x25 + 0x1c;
					} else if (CHART_SELECTED_COLUMN < 7) {
						selectorX =
							(CHART_SELECTED_COLUMN - 3) * 0x18 + 0x8b;
					} else {
						selectorX =
							(CHART_SELECTED_COLUMN - 7) * 0x18 + 0xf8;
					}
					setRECT(&finalPos, selectorX - 0x99, CHART_SELECTED_ROW * 0x13 - 0x45, 10, 10);
					removeAnimatedUIBox(2, &finalPos);
					playSound(0, 3);
					MENU_STATE = 2;
				}
			}
		}

#if defined(VERSION_JP)
		if (PLAYER_MENU_STATE == 2 && MENU_STATE == 2) {
#else
		if (PLAYER_MENU_STATE == 2) {
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

		if (PLAYER_MENU_STATE == 3) {
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
	switch (PLAYER_MENU_STATE) {
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
	tabs.tab[PLAYER_MENU_STATE] = 0;
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
		if (MENU_OPTIONS[selection].disabled & 1) {
			return;
		}
		TRIANGLE_MENU_STATE = 2;
		drawInventoryText();
		break;
	case 6:
		if (MENU_OPTIONS[selection].disabled & 1) {
			return;
		}
		closeTriangleMenu();
		partnerSetState(3);
		IS_IN_MENU = 0;
		startGameTime();
		break;
	case 5:
		if (MENU_OPTIONS[selection].disabled & 1) {
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
		if (MENU_OPTIONS[selection].disabled & 1) {
			return;
		}
		closeTriangleMenu();
		partnerSetState(0xf);
		IS_IN_MENU = 0;
		startGameTime();
		break;
	case 7:
		if (MENU_OPTIONS[selection].disabled & 1) {
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
	MENU_OPTIONS[6].disabled = arg;
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

	column = (MOVE_SELECT_BOX_X - 0x73) / 18;
	row = (MOVE_SELECT_BOX_Y - 0x6f) / 15;
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

	column = (MOVE_SELECT_BOX_X - 0x73) / 18;
	row = (MOVE_SELECT_BOX_Y - 0x6f) / 15;
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

	cluts = STATS_VIEW_ELEMENT_CLUT;
	switch (MENU_STATE) {
	case 0:
		if (drawDigimonStatsStrings() == 1) {
			MENU_STATE = 1;
		}
		break;
	case 1:
		renderSeparatorLines(STATS_VIEW_LINES, 6, 5);
		for (j = 0; j < 9; j++) {
			sr = &STATS_VIEW_TEXT[j];
			renderString(3, sr->posX, sr->posY + 1, sr->uvWidth, 0xc, sr->uvX, sr->uvY, 5, 1);
		}
		for (j = 0; j < 0x15; j++) {
			icon = &STATS_VIEW_ELEMENTS[j];
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
			r = &STATS_VIEW_INSETS[j];
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
			icon = &MOVES_VIEW_TECHSET_TEXT[i];
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
		renderSeparatorLines(MOVES_TECHSET_LINES, 3, 3);
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
			icon = &MOVES_VIEW_TECHSET_TEXT[i];
			renderRectPolyFT4(icon->posX, -9,
			                  icon->width,
			                  icon->height,
			                  icon->texX,
			                  icon->texY + 0x80, 5, 0x7b06, 5, 0);
		}
		renderRectPolyFT4(0x75, 4, 4, 4, 0x78, 0x8c, 5, 0x7b06, 5, 0);
		renderSeparatorLines(&MOVES_TECHSET_LINES[15], 3, 5);
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
		renderSeparatorLines(MOVES_VIEW_HELP_LINES, 0xc, 5);
		for (i = 0; i < 4; i++) {
			icon = &MOVES_VIEW_HELP_CORNERS[i];
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
		renderRectPolyFT4(MOVE_SELECT_BOX_X - 0xa0,
		                  MOVE_SELECT_BOX_Y - 0x78, 0x12, 0x10, 0xc0, 0x8c, 5, 0x7b06, 5, 0);
		for (row = 0; row < 7; row++) {
			for (j = 0; j < 8; j++) {
				renderBox(j * 0x12 - 0x2a, row * 0xf - 7, 0xc, 0xc,
				          0x4e, 0x60, 0x6e, 0x80, 5);
			}
		}
		if (0xa8 < MOVE_SELECT_BOX_X) {
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
			sr = &MOVES_VIEW_STRING_SPRITES[i];
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
			icon = &MOVES_VIEW_SPRITES[i];
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
		renderSeparatorLines(TAMER_VIEW_PLAYER_LINES, 0xb, 5);

		for (i = 0; i < 0xb; i++) {
			e = &TAMER_WINDOW_NAME_STRING[i];
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
			r = &TAMER_WINDOW_BOXES[k];
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
		renderSeparatorLines(DIGIMON_MOVE_INFO_LINES_LEFT, 0x12, 4);
	} else {
		renderSeparatorLines(DIGIMON_MOVE_INFO_LINES_RIGHT, 0x12, 4);
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

	row = (MOVE_SELECT_BOX_X - 0x73) / 18;
	col = (MOVE_SELECT_BOX_Y - 0x6f) / 15;
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

	cluts = EVO_CHART_VIEW_COLORS;
	switch (MENU_STATE) {
	case 0:
		if (drawEvoChartStrings(0) == 1) {
			MENU_STATE = 1;
		}
		break;
	case 2:
	case 3:
	case 4:
		if (CHART_SELECTED_COLUMN < 3) {
			x = (int16_t)(CHART_SELECTED_COLUMN * 0x25 + 0x1c);
		} else if (CHART_SELECTED_COLUMN < 7) {
			x = (int16_t)((CHART_SELECTED_COLUMN - 3) * 0x18 + 0x8b);
		} else {
			x = (int16_t)((CHART_SELECTED_COLUMN - 7) * 0x18 + 0xf8);
		}
		renderRectPolyFT4((int16_t)(x - 0xa2),
		                  (int16_t)(CHART_SELECTED_ROW * 0x13 - 0x4e), 0x18, 0x14,
		                  0, 0xe8, 0x18, 0x7dc7, 5, 0);
		/* fall through */
	case 1:
		for (i = 0; i < 0x3e; i++) {
			if (hasDigimonRaised((i + 1) & 0xffff)) {
				shift = 0;
				if ((i + 1 == CHART_SELECTED_DIGIMON) && (1 < MENU_STATE) &&
				    ((PLAYTIME_FRAMES % 10) < 5)) {
					shift = 0x10;
				}
				renderRectPolyFT4(
					(int16_t)(EVO_CHART_BOXES[i].posX - 0xa0),
					(int16_t)(EVO_CHART_BOXES[i].posY - 0x78),
					0x10, 0x10,
					(uint8_t)(shift + EVO_CHART_BOXES[i].u),
					EVO_CHART_BOXES[i].v, 0x18,
					cluts.m[EVO_CHART_BOXES[i].clut],
					5, 0);
			}
		}
		for (i = 0; i < 0x3e; i++) {
			renderBorderBox((EVO_CHART_BOXES[i].posX - 1),
			                (EVO_CHART_BOXES[i].posY - 1), 0x12, 0x12,
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

GARBAGE_ARRAY(renderEvoChartDetail, MOVES_TECHSET_LINES, 30, 9);

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

	clut = EVO_CHART_DETAIL_COLORS;
	MENU_SUB_STATE = 2;
	drawEvoChartStrings((int8_t)CHART_SELECTED_DIGIMON);
	count1 = (count2 = 0);
	for (i = 0; i < 5; i++) {
		if (EVOLUTION_PATHS[CHART_SELECTED_DIGIMON - 1].from[i] != -1) {
			count1++;
		}
	}
	for (i = 0; i < 6; i++) {
		if (EVOLUTION_PATHS[CHART_SELECTED_DIGIMON - 1].to[i] != -1) {
			count2++;
		}
	}

	if ((count1 % 2) == 0) {
		fromLines = CHART_FROM_LINES_EVEN;
		fromSprites = CHART_FROM_SPRITES_EVEN;
		fromCount = 4;
	} else {
		fromLines = CHART_FROM_LINES_ODD;
		fromSprites = CHART_FROM_SPRITES_ODD;
		fromCount = 5;
	}
	if ((count2 % 2) == 0) {
		toLines = CHART_TO_LINES_EVEN;
		toSprites = CHART_TO_SPRITES_EVEN;
		toCount = 6;
	} else {
		toLines = CHART_TO_LINES_ODD;
		toSprites = CHART_TO_SPRITES_ODD;
		toCount = 5;
	}

	for (j = 0; j < fromCount; j++) {
		if (EVOLUTION_PATHS[CHART_SELECTED_DIGIMON - 1].from[j] > 0) {
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
		if (EVOLUTION_PATHS[CHART_SELECTED_DIGIMON - 1].to[j] > 0) {
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
	                  EVO_CHART_BOXES[CHART_SELECTED_DIGIMON - 1].u,
	                  EVO_CHART_BOXES[CHART_SELECTED_DIGIMON - 1].v, 0x18,
	                  clut.m[EVO_CHART_BOXES[CHART_SELECTED_DIGIMON - 1].clut],
	                  4, 0);
	renderBorderBox(0x97, 99, 0x12, 0x12, 0xbebebe, 0x3c3c3c, 0x87, 0x87,
	                0x87, 4);

	for (j = 0; j < fromCount; j++) {
		id = EVOLUTION_PATHS[CHART_SELECTED_DIGIMON - 1].from[j];
		if (id > 0) {
			if (hasDigimonRaised(id & 0xffff) == 1) {
				renderRectPolyFT4(fromSprites->posX, fromSprites->posY,
				                  0x10, 0x10,
				                  EVO_CHART_BOXES[id - 1].u,
				                  EVO_CHART_BOXES[id - 1].v, 0x18,
				                  clut.m[EVO_CHART_BOXES[id - 1].clut],
				                  4, 0);
			}
			renderBorderBox(fromSprites->posX + 0x9f,
			                fromSprites->posY + 0x77, 0x12, 0x12,
			                0xbebebe, 0x3c3c3c, 0x87, 0x87, 0x87, 4);
		}
		fromSprites++;
	}

	for (j = 0; j < toCount; j++) {
		id = EVOLUTION_PATHS[CHART_SELECTED_DIGIMON - 1].to[j];
		if (id > 0) {
			if (hasDigimonRaised(id & 0xffff) == 1) {
				renderRectPolyFT4(toSprites->posX, toSprites->posY,
				                  0x10, 0x10,
				                  EVO_CHART_BOXES[id - 1].u,
				                  EVO_CHART_BOXES[id - 1].v, 0x18,
				                  clut.m[EVO_CHART_BOXES[id - 1].clut],
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
	             ((DIGIMON_DATA[CHART_SELECTED_DIGIMON].level - 1) * 36) + 0x30, 0x18,
	             4);
#else
	renderString(3, -0x14, -0x4f, 0x24, 0xc, 0, 0x18, 4);
	switch (DIGIMON_DATA[CHART_SELECTED_DIGIMON].level) {
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
	len = strlen(DIGIMON_DATA[CHART_SELECTED_DIGIMON].name) / 2;
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

	sprites = CARD_SPRITE_CLUT;

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

	rect = DIGIMON_STATS_TEXT_AREA;

	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect);
		drawString(MAIN_D_80124C54, 0, 0xf0);
		drawString(MAIN_D_80124C0C[2], 0, 0x24);
		drawString(MAIN_D_80124C0C[3], 0x24, 0x24);
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
	GsSetRefView2(&DIGIVICE_ENTITY_VIEW);
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
	GsSetRefView2(&MEDAL_VIEW);
	GsClearOt(0, 1, FRAMEBUFFER_OT[ACTIVE_FRAMEBUFFER]);

	MEDAL_ROTATION.vy += 0x64;
	RotMatrix(&MEDAL_ROTATION, &MEDAL_COORDINATES.coord);
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
		MOVE_SELECT_BOX_X = 0x73;
		MOVE_SELECT_BOX_Y = 0x6f;
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
		CHART_SELECTED_COLUMN = CHART_SELECTED_ROW = 0;
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
		color = &UI_LINES_BORDER_COLOR[((uint8_t *)lines)[8] * 3];
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
