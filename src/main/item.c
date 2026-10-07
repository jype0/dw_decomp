#include <stdio.h>
#include <string.h>

#include <inline_n.h>
#include <libgs.h>

#include <dw/anim.h>
#include <dw/btl.h>
#include <dw/combat.h>
#include <dw/entity.h>
#include <dw/font.h>
#include <dw/garbage.h>
#include <dw/item.h>
#include <dw/math.h>
#include <dw/move.h>
#include <dw/params.h>
#include <dw/particle.h>
#include <dw/partner.h>
#include <dw/script.h>
#include <dw/tamer.h>
#include <dw/types.h>
#include <dw/world_object.h>

#include "common.h"

extern uint8_t MAP_LAYER_ENABLED;
extern int32_t VIEWPORT_DISTANCE;
extern int16_t EVOLUTION_TARGET;
extern uint8_t HAS_USED_EVOITEM;

void deleteDroppedItem(int16_t itemId);
void setUVDataPolyFT4(POLY_FT4 *p, int32_t u, int32_t v, int32_t w, int32_t h);
void setPosDataPolyFT4(POLY_FT4 *prim, int32_t posX, int32_t posY, int32_t width,
                       int32_t height);
void setItemTexture(POLY_FT4 *prim, int32_t type);
void decreasePoopLevel(void);
void modifyLifetime(int16_t delta);
void reduceTiredness(int16_t amount);
void setTrainingBoost(int32_t flag, int32_t value, int32_t duration);
void addEnergy(int16_t amount);
void addHappiness(int16_t amount);
void addDiscipline(int16_t amount);
void addWeight(int16_t amount);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void renderDroppedItem(int32_t instanceId);
void renderDroppedItemShadow(WorldItem *item);
void handlePoopWeightLoss(int32_t type);
void closeInventoryBoxes(void);
void BTL_healStatusEffect(int32_t arg);
void addEntityText(Entity *entity, int16_t slotId, int16_t color, int32_t value, uint8_t icon);
void addWithLimit(int16_t *value, int16_t amount, int16_t limit);
int32_t handleMedicineHealing(int16_t injuryChance, int16_t sicknessChance);
void handlePortaPotty(void);
void handleItemSickness(int16_t arg);
void addTamerLevel(int32_t chance, int32_t amount);

Inventory INVENTORY;
TamerItem TAMER_ITEM;
DroppedItem DROPPED_ITEMS[11];

// clang-format off
#if defined(VERSION_JP)
char STR_MOVE_NAME_BUG[] = "バグ";

char STR_MOVE_NAME_PARTY_TIME[] = "ウンチ";

char STR_MOVE_NAME_PUMMEL_WHACK[] = "覇王拳";

char STR_MOVE_NAME_FIST_OF_THE_BEAST_KING[] = "獣王拳";

char STR_MOVE_NAME_BUBBLE[] = "あわ";

char STR_ITEM_DESC_MYSTERY_ITEM[] = "？？？";
#else
char STR_MOVE_NAME_BUG[4] = "Bug";

char STR_MOVE_NAME_TREMAR[] = "Tremar";

char STR_MOVE_NAME_WAR_CRY[8] = "War Cry";

char STR_MOVE_NAME_COUNTER[8] = "Counter";

char STR_MOVE_NAME_BUBBLE[] = "Bubble";
#endif

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

InventoryTable INITIAL_INVENTORY_AMOUNTS = {
	{
		0x28, 0x19, 0x32, 0x1e, 0x14, 0x1e, 0x1e, 0x12,
		0x1e, 0x1e, 0x1e, 0x14, 0x1e, 0x14, 0x1e, 0x32,
		0x32, 0x50, 0x32, 0x32, 0x62, 0x32, 0x32, 0x32,
		0x32, 0x32, 0x32, 0x32, 0x32, 0x00,
	},
};

InventoryTable INITIAL_INVENTORY_TYPES = {
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

uint8_t TYPE_FACTORS[7][7] = {
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

uint8_t ITEM_CLUT_DATA[128] = {
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

int16_t HEALING_VALUES[4] = { 500, 1500, 5000, 9999 };
uint8_t HEAL_EFFECT_VARIANT[4] = { 0, 0, 1, 1 };
#if !defined(VERSION_JP)
char NAME_FORMAT[] = "%s";
#endif

CombatData *COMBAT_DATA_PTR;
int8_t TAMER_WAYPOINT_ACTIVE;
int8_t TAMER_START_TILE_Y;
int8_t TAMER_START_TILE_X;
int8_t TAMER_WAYPOINT_COUNT;
int8_t TAMER_WAYPOINT_CURRENT;
int8_t TAMER_PREVIOUS_TILE_Y;
int8_t TAMER_PREVIOUS_TILE_X;
int8_t PARTNER_PREVIOUS_TILE_Y;
int8_t PARTNER_PREVIOUS_TILE_X;
int8_t PARTNER_WAYPOINT_COUNT;
int8_t PARTNER_WAYPOINT_CURRENT;
int8_t PARTNER_TAMER_PREVIOUS_TILE_Y;
int8_t PARTNER_TAMER_PREVIOUS_TILE_X;
Entity *FINISHING_ENTITY;
uint8_t BATTLE_TOGGLE_LIFEBAR;
int16_t BATTLE_FRAME_COUNT;
int16_t FLEE_TIMER;
int16_t DEATH_COUNTDOWN;
int16_t ENEMY_COUNT;
int32_t HAS_TAKEN_DAMAGE;
int32_t NO_AI_FLAG;
int32_t IS_TAMERLESS_BATTLE;
int32_t FLEE_DISABLED[2];
int32_t P2_AOE_TIMER;
int32_t COMBAT_AREA_Y;
int32_t COMBAT_AREA_X;

static void *item_sbss_order[] = {
	&COMBAT_AREA_X,
	&COMBAT_AREA_Y,
	&P2_AOE_TIMER,
	FLEE_DISABLED,
	&IS_TAMERLESS_BATTLE,
	&NO_AI_FLAG,
	&HAS_TAKEN_DAMAGE,
	&ENEMY_COUNT,
	&DEATH_COUNTDOWN,
	&FLEE_TIMER,
	&BATTLE_FRAME_COUNT,
	&BATTLE_TOGGLE_LIFEBAR,
	&FINISHING_ENTITY,
	&PARTNER_TAMER_PREVIOUS_TILE_X,
	&PARTNER_TAMER_PREVIOUS_TILE_Y,
	&PARTNER_WAYPOINT_CURRENT,
	&PARTNER_WAYPOINT_COUNT,
	&PARTNER_PREVIOUS_TILE_X,
	&PARTNER_PREVIOUS_TILE_Y,
	&TAMER_PREVIOUS_TILE_X,
	&TAMER_PREVIOUS_TILE_Y,
	&TAMER_WAYPOINT_CURRENT,
	&TAMER_WAYPOINT_COUNT,
	&TAMER_START_TILE_X,
	&TAMER_START_TILE_Y,
	&TAMER_WAYPOINT_ACTIVE,
	&COMBAT_DATA_PTR,
};

void *item_text_order[] = {
	handleItemSickness,
	setTrainingBoost,
	decreasePoopLevel,
	addWeight,
	addDiscipline,
	addHappiness,
	reduceTiredness,
	addEnergy,
	modifyLifetime,
	handlePortaPotty,
	handleMedicineHealing,
	addWithLimit,
	removeTamerItem,
	initializeInventory,
	pickupItem,
	removeItem,
	giveItem,
	getItemCount,
	renderDroppedItemShadow,
	renderOverworldItem,
	clearDroppedItems,
	deleteDroppedItem,
	spawnItem,
	renderDroppedItem,
	spawnDroppedItems,
	initializeDroppedItems,
	setInventorySize,
	handleHPHealingItem,
	handleMPHealingItem,
	handleDoubleFloppy,
	handleRestore,
	handleStatusItems,
	handleChips,
	handleFood,
	handleEvoItems,
};

static void *item_bss_order[] = {
	DROPPED_ITEMS,
	&TAMER_ITEM,
	&INVENTORY,
};

GARBAGE(handleEvoItems, 15);

void handleEvoItems(int16_t item)
{
	int16_t level;

	if (item >= 0x7d) {
		if (item == 0x7d) {
			EVOLUTION_TARGET = 0x40;
		}
		if (item == 0x7e) {
			EVOLUTION_TARGET = 0x3f;
		}
		if (item == 0x7f) {
			EVOLUTION_TARGET = 0x41;
		}
	} else {
		level = DIGIMON_DATA[EVOLUTION_ITEM_TARGET[item - 0x47]].level - 1;
		if (level != DIGIMON_DATA[ENTITY_TABLE[1]->type].level) {
			return;
		}
		EVOLUTION_TARGET = EVOLUTION_ITEM_TARGET[item - 0x47];
	}
	HAS_USED_EVOITEM = 1;
	removeTamerItem();
	closeInventoryBoxes();
	tamerSetState(6);
	partnerSetState(0xd);
}

void handleStatusItems(int32_t itemId)
{
	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0) {
		switch (itemId) {
		case 8:
			BTL_healStatusEffect(1);
			return;
		case 9:
			if (GAME_STATE == 1) {
				BTL_healStatusEffect(0);
			}
			handleDoubleFloppy(itemId);
			return;
		case 10:
			COMBAT_DATA_PTR->fighter[0].flags |= FIGHTER_FLAG_PROTECTED;
			break;
		case 0xd:
			if (handleMedicineHealing(3, 2) == 1) {
				addHealingParticleEffect(ENTITY_TABLE[1], 0);
				return;
			}
			break;
		case 0xe:
			if (handleMedicineHealing(3, 10) == 1) {
				addHealingParticleEffect(ENTITY_TABLE[1], 0);
			}
		}
	}
}

void handleChips(int16_t chipId)
{
	int16_t lifetime;
	int16_t off;
	int16_t def;
	int16_t speed;
	int16_t brain;
	int16_t hp;
	int16_t mp;

	off = def = speed = brain = hp = mp = lifetime = 0;
	switch (chipId) {
	case 0x16:
		if (IS_SCRIPT_PAUSED == 1) {
			removeTamerItem();
			callScriptSection(0, 0x4dd, 0);
		}
		break;
	case 0x17:
		off = 0x32;
		break;
	case 0x18:
		def = 0x32;
		break;
	case 0x19:
		brain = 0x32;
		break;
	case 0x1a:
		speed = 0x32;
		break;
	case 0x1b:
		hp = 500;
		break;
	case 0x1c:
		mp = 500;
		break;
	case 0x1d:
		off = 100;
		brain = 100;
		lifetime = -24;
		break;
	case 0x1e:
		def = 100;
		speed = 100;
		lifetime = -24;
		break;
	case 0x1f:
		hp = 1000;
		mp = 1000;
		lifetime = -24;
		break;
	case 0x20:
		handlePortaPotty();
		return;
	}
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.hp, hp, 9999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.mp, mp, 9999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.off, off, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.def, def, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.speed, speed, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.brain, brain, 999);
	modifyLifetime(lifetime);
	if ((0x1c < chipId) && (chipId < 0x20)) {
		addTamerLevel(10, -1);
	}
}

void handleRestore(int16_t type)
{
	CombatData *combat = COMBAT_DATA_PTR;
	uint16_t *flags = &combat->fighter[0].flags;
	int16_t amount;

	if (GAME_STATE == 1) {
		BTL_removeDeathCountdown();
	}

	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP == 0) {
		startAnimation(ENTITY_TABLE[1], 0x2c);
	}

	switch (type) {
	case 0xb:
		amount = PARTNER_ENTITY.digimonEntity.stats.base.hp / 2;
		break;
	case 0xc:
		if (GAME_STATE == 1) {
			BTL_healStatusEffect(0);
		}

		amount = 0x270f;
		break;
	}

	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP,
	             amount, PARTNER_ENTITY.digimonEntity.stats.base.hp);
	if (GAME_STATE == 1) {
		addEntityText(ENTITY_TABLE[1], 0, 0xb, amount, 1);
	}

	addHealingParticleEffect(ENTITY_TABLE[1], 1);
}

void handleDoubleFloppy(int32_t itemId)
{
	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0) {
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP,
		             0x5dc, PARTNER_ENTITY.digimonEntity.stats.base.hp);
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentMP,
		             0x5dc, PARTNER_ENTITY.digimonEntity.stats.base.mp);
		if (GAME_STATE == 1) {
			addEntityText(ENTITY_TABLE[1], 0, 0xb, 0x5dc, 1);
			addEntityText(ENTITY_TABLE[1], 0, 0xb, 0x5dc, 2);
		}
		addHealingParticleEffect(ENTITY_TABLE[1], 0);
	}
}

void handleMPHealingItem(uint8_t idx)
{
	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0) {
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentMP, HEALING_VALUES[idx - 4], PARTNER_ENTITY.digimonEntity.stats.base.mp);
		if (GAME_STATE == 1) {
			addEntityText(ENTITY_TABLE[1], 0, 0xb, HEALING_VALUES[idx - 4], 2);
		}
		addHealingParticleEffect(ENTITY_TABLE[1], HEAL_EFFECT_VARIANT[idx - 4]);
	}
}

void handleHPHealingItem(uint8_t idx)
{
	if (PARTNER_ENTITY.digimonEntity.stats.current.currentHP != 0) {
		addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP, HEALING_VALUES[idx], PARTNER_ENTITY.digimonEntity.stats.base.hp);
		if (GAME_STATE == 1) {
			addEntityText(ENTITY_TABLE[1], 0, 0xb, HEALING_VALUES[idx], 1);
		}
		addHealingParticleEffect(ENTITY_TABLE[1], HEAL_EFFECT_VARIANT[idx]);
	}
}

void setInventorySize(uint8_t size)
{
	INVENTORY.size = size;
}

void initializeDroppedItems(void)
{
	int32_t i;

	TAMER_ITEM.worldItem.type = 0xff;
	for (i = 0; i < 11; i++) {
		DROPPED_ITEMS[i].worldItem.type = 0xff;
	}
}

void spawnDroppedItems(Entity *e, uint8_t type)
{
	int32_t i;
	DroppedItem *it;
	VECTOR *loc;

	loc = &e->posData->location;
	for (i = 0; i < 0xb; i++) {
		it = &DROPPED_ITEMS[i];
		if (it->worldItem.type == 0xff) {
			it->worldItem.type = type;
			break;
		}
	}
	if (i != 0xb) {
		it->worldItem.spriteLocation.vx = loc->vx;
		it->worldItem.spriteLocation.vy = 0;
		it->worldItem.spriteLocation.vz = loc->vz;
		getModelTile(loc, (int16_t *)((char *)it + 0xc),
		             (int16_t *)((char *)it + 0xe));
		addObject(0x195, i, 0, renderDroppedItem);
	}
}

void renderDroppedItem(int32_t instanceId)
{
	if (MAP_LAYER_ENABLED != 0) {
		renderOverworldItem(&DROPPED_ITEMS[instanceId].worldItem);
		renderDroppedItemShadow(&DROPPED_ITEMS[instanceId].worldItem);
	}
}

GARBAGE(spawnItem, 24);

void spawnItem(uint8_t itemId, int16_t tileX, int16_t tileY)
{
	int32_t i;

	for (i = 0; i < 0xb; i++) {
		if (DROPPED_ITEMS[i].worldItem.type == 0xff) {
			DROPPED_ITEMS[i].worldItem.type = itemId;
			DROPPED_ITEMS[i].tileX = tileX;
			DROPPED_ITEMS[i].tileY = tileY;
			DROPPED_ITEMS[i].worldItem.spriteLocation.vx =
				(tileX - 0x32) * 100 + 0x32;
			DROPPED_ITEMS[i].worldItem.spriteLocation.vy =
				ENTITY_TABLE[0]->posData->location.vy;
			DROPPED_ITEMS[i].worldItem.spriteLocation.vz =
				(0x32 - tileY) * 100 - 0x32;
			addObject(0x195, i, 0, renderDroppedItem);
			return;
		}
	}
}

void deleteDroppedItem(int16_t itemId)
{
	removeObject(0x195, itemId);
	DROPPED_ITEMS[itemId].worldItem.type = 0xff;
}

void clearDroppedItems(void)
{
	int32_t i;

	for (i = 0; i < 11; i++) {
		if (DROPPED_ITEMS[i].worldItem.type != 0xff) {
			deleteDroppedItem(i);
		}
	}
}

void renderOverworldItem(WorldItem *item)
{
	DVECTOR screen;
	int32_t otz;
	POLY_FT4 *prim;
	int16_t width;

	GsSetLsMatrix(&GsWSMATRIX);
	gte_ldv0(&item->spriteLocation);
	gte_rtps();
	gte_stsxy(&screen);
	gte_stszotz(&otz);
	width = (uint32_t)(VIEWPORT_DISTANCE << 7) / (uint32_t)(int32_t)(otz * 4);
	otz = otz >> 2;
	if ((0 < otz) && (otz < 0x1000)) {
		prim = (POLY_FT4 *)GsGetWorkBase();
		SetPolyFT4(prim);
		setRGB0(prim, 0x80, 0x80, 0x80);
		prim->tpage = getTPage(0, 0, 320, 0);
		setItemTexture(prim, item->type);
		if (width >= 0x20) {
			setUVWH(prim, prim->u0, prim->v0, 0xf, 0xf);
		}
		setPosDataPolyFT4(prim, screen.vx - (width >> 1),
		                  screen.vy - (width >> 1), width, width);
		AddPrim(&ACTIVE_ORDERING_TABLE->org[otz], prim++);
		GsSetWorkBase((PACKET *)prim);
	}
}

void renderDroppedItemShadow(WorldItem *item)
{
	SVECTOR p0;
	SVECTOR p1;
	SVECTOR p2;
	SVECTOR p3;
	int32_t otz;
	POLY_FT4 *prim;
	int16_t px;
	int16_t pz;

	GsSetLsMatrix(&GsWSMATRIX);
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->tpage = getTPage(1, 2, 832, 256);
	setClut(prim, 0, 0x1e7);
	setUVDataPolyFT4(prim, 0x40, 0x80, 0x3f, 0x3f);
	setRGB0(prim, 0x30, 0x30, 0x30);
	px = item->spriteLocation.vx;
	pz = item->spriteLocation.vz;
	p0.vx = px - 100;
	p0.vy = 0;
	p0.vz = pz - 100;
	p1.vx = px + 100;
	p1.vy = 0;
	p1.vz = pz - 100;
	p2.vx = px - 100;
	p2.vy = 0;
	p2.vz = pz + 100;
	p3.vx = px + 100;
	p3.vy = 0;
	p3.vz = pz + 100;
	gte_ldv3(&p0, &p1, &p2);
	gte_rtpt();
	gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
	gte_stszotz(&otz);
	gte_ldv0(&p3);
	gte_rtps();
	gte_stsxy(&prim->x3);
	AddPrim(&ACTIVE_ORDERING_TABLE->org[0xffd], prim++);
	GsSetWorkBase((PACKET *)prim);
}

uint8_t getItemCount(uint8_t type)
{
	int32_t i;

	for (i = 0; i < INVENTORY.size; i++) {
		if (INVENTORY.types.array[i] == type) {
			return INVENTORY.amounts.array[i];
		}
	}

	return 0;
}

int32_t giveItem(uint8_t item, uint8_t amount)
{
	int16_t used[30];
	int32_t i;
	int32_t j;
	uint8_t *count;
	int32_t result;

	result = 0;
	for (i = 0; i < INVENTORY.size; i++) {
		if (INVENTORY.types.array[i] == item) {
			count = &INVENTORY.amounts.array[i];
			if (*count != 99) {
				*count += amount;
				if (*count >= 100) {
					*count = 99;
				}
				return 1;
			}
			return 0;
			/* unreachable */
			INVENTORY.types.array[i] = 0xff;
			INVENTORY.amounts.array[i] = 0;
			INVENTORY.names.array[i] = 0xff;
		}
	}

	if (result == 0) {
		for (i = 0; i < INVENTORY.size; i++) {
			if (INVENTORY.types.array[i] == 0xff) {
				INVENTORY.types.array[i] = item;
				INVENTORY.amounts.array[i] = amount;
				for (j = 0; j < INVENTORY.size; j++) {
					used[j] = 0;
				}
				for (j = 0; j < INVENTORY.size; j++) {
					if (INVENTORY.names.array[j] != 0xff) {
						used[INVENTORY.names.array[j]] = 1;
					}
				}
				for (j = 0; j < INVENTORY.size; j++) {
					if (used[j] == 0) {
						INVENTORY.names.array[i] = j;
						break;
					}
				}
				return 1;
			}
		}
	}

	return result;
}

void removeItem(uint8_t type, uint8_t amount)
{
	int32_t i;
	uint8_t *count;

	if (type == 0xff) {
		return;
	}

	for (i = 0; i < INVENTORY.size; i++) {
		if (INVENTORY.types.array[i] == type) {
			count = &INVENTORY.amounts.array[i];
			if (amount < *count) {
				*count -= amount;
			} else {
				*count = 0;
				INVENTORY.types.array[i] = 0xff;
				INVENTORY.names.array[i] = 0xff;
			}
		}
	}
}

int32_t pickupItem(int16_t itemId)
{
	uint8_t type;
	int32_t got;

	type = DROPPED_ITEMS[itemId].worldItem.type;
	got = giveItem(type, 1);
	if (got != 0) {
		deleteDroppedItem(itemId);
	}
	return got;
}

void initializeInventory(void)
{
	InventoryTable amounts;
	InventoryTable types;
	int32_t i;

	for (i = 0; i < 30; ++i) {
		INVENTORY.types.array[i] = 0xff;
		INVENTORY.amounts.array[i] = 0;
		INVENTORY.names.array[i] = 0xff;
	}

	INVENTORY.size = 10;
	amounts = INITIAL_INVENTORY_AMOUNTS;
	types = INITIAL_INVENTORY_TYPES;

	for (i = 0; i < 30; ++i) {
		INVENTORY.types.array[i] = types.array[i];
		INVENTORY.amounts.array[i] = amounts.array[i];
		INVENTORY.names.array[i] = i;
	}

	INVENTORY.size = 30;
}

void removeTamerItem(void)
{
	if (TAMER_ITEM.worldItem.type != 0xff) {
		removeObject(0x194, 0);
		TAMER_ITEM.worldItem.type = 0xff;
	}
}

void handleFood(int16_t itemId)
{
	int16_t energy;
	int16_t happiness;
	int16_t weight;
	int16_t tiredness;
	int16_t discipline;
	int16_t lifetime;
	int16_t addedHP;
	int16_t addedMP;
	int16_t healedHP;
	int16_t healedMP;
	int16_t addedOffense;
	int16_t addedDefense;
	int16_t addedSpeed;
	int16_t addedBrain;
	int16_t trainFlag;
	int16_t trainValue;
	int16_t trainDuration;
	int16_t sicknessChance;

	energy = tiredness = happiness = discipline = weight = lifetime = 0;
	addedHP = addedMP = healedHP = healedMP = addedOffense = addedDefense = addedSpeed = addedBrain = 0;
	trainFlag = trainValue = trainDuration = sicknessChance = 0;

	switch (itemId) {
	case 0x26:
		energy = 12;
		weight = 1;
		break;
	case 0x27:
		energy = 24;
		weight = 2;
		break;
	case 0x28:
		energy = 35;
		tiredness = 5;
		happiness = 3;
		weight = 3;
		break;
	case 0x29:
		energy = 10;
		trainFlag = 0x29;
		trainValue = 12;
		trainDuration = 6;
		weight = -2;
		break;
	case 0x2a:
		energy = 15;
		trainFlag = 0x16;
		trainValue = 12;
		trainDuration = 6;
		weight = 3;
		break;
	case 0x2b:
		energy = 9;
		tiredness = 50;
		weight = 1;
		break;
	case 0x2c:
		energy = 12;
		weight = 1;
		break;
	case 0x2d:
		energy = 19;
		discipline = 50;
		weight = 2;
		break;
	case 0x2e:
		energy = 38;
		addedOffense = 10;
		addedDefense = 10;
		addedSpeed = 10;
		addedBrain = 10;
		addedHP = 100;
		addedMP = 100;
		weight = 4;
		break;
	case 0x2f:
		energy = 22;
		trainFlag = 0x3f;
		trainValue = 15;
		trainDuration = 6;
		weight = 2;
		break;
	case 0x30:
		energy = 30;
		happiness = 50;
		weight = 3;
		break;
	case 0x31:
		energy = 25;
		tiredness = 20;
		happiness = 20;
		discipline = 20;
		weight = 2;
		break;
	case 0x32:
		energy = 40;
		weight = 4;
		break;
	case 0x33:
		energy = 100;
		weight = 10;
		break;
	case 0x34:
		energy = 20;
		healedHP = 9999;
		weight = 2;
		break;
	case 0x35:
		energy = 16;
		healedMP = 9999;
		weight = 2;
		break;
	case 0x36:
		energy = 33;
		weight = -5;
		break;
	case 0x37:
		energy = 24;
		healedHP = 1000;
		healedMP = 1000;
		weight = 2;
		break;
	case 0x38:
		energy = 20;
		addedOffense = 20;
		weight = 2;
		break;
	case 0x39:
		energy = 20;
		addedDefense = 20;
		weight = 2;
		break;
	case 0x3a:
		energy = 20;
		addedSpeed = 20;
		weight = 2;
		break;
	case 0x3b:
		energy = 20;
		addedBrain = 20;
		weight = 2;
		break;
	case 0x3c:
		energy = 20;
		addedHP = 200;
		weight = 2;
		break;
	case 0x3d:
		energy = 20;
		addedMP = 200;
		weight = 2;
		break;
	case 0x3e:
		energy = 8;
		weight = 1;
		break;
	case 0x3f:
		energy = 12;
		weight = 1;
		break;
	case 0x40:
		energy = 22;
		weight = 2;
		break;
	case 0x41:
		energy = 27;
		addedOffense = 1;
		addedDefense = 1;
		addedSpeed = 1;
		addedBrain = 1;
		addedHP = 10;
		addedMP = 10;
		weight = -2;
		break;
	case 0x42:
		energy = 49;
		weight = 5;
		break;
	case 0x43:
		energy = 35;
		healedHP = 9999;
		healedMP = 9999;
		lifetime = 3;
		sicknessChance = 20;
		weight = 4;
		break;
	case 0x44:
		energy = 30;
		sicknessChance = 100;
		weight = 2;
		break;
	case 0x45:
		energy = 15;
		happiness = 30;
		tiredness = 30;
		sicknessChance = 30;
		weight = 1;
		break;
	case 0x46:
		energy = 50;
		happiness = 50;
		tiredness = 50;
		lifetime = 20;
		sicknessChance = 5;
		weight = 3;
		break;
	case 0x79:
		healedMP = 1000;
		break;
	case 0x7a:
		energy = 32;
		healedHP = 1000;
		sicknessChance = 20;
		break;
	}

	if (itemId == RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].favoriteFood) {
		energy = energy * 14 / 10;
		happiness += 2;
	}
	addEnergy(energy);
	reduceTiredness(tiredness);
	addHappiness(happiness);
	addDiscipline(discipline);
	addWeight(weight);
	decreasePoopLevel();
	setTrainingBoost(trainFlag, trainValue, trainDuration);
	handleItemSickness(sicknessChance);
	PARTNER_PARA.remainingLifetime += lifetime;
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.hp, addedHP, 9999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.mp, addedMP, 9999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentHP, healedHP,
	             PARTNER_ENTITY.digimonEntity.stats.base.hp);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.current.currentMP, healedMP,
	             PARTNER_ENTITY.digimonEntity.stats.base.mp);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.off, addedOffense, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.def, addedDefense, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.speed, addedSpeed, 999);
	addWithLimit(&PARTNER_ENTITY.digimonEntity.stats.base.brain, addedBrain, 999);
}

void addWithLimit(int16_t *value, int16_t amount, int16_t limit)
{
	*value += amount;
	if (*value > limit) {
		*value = limit;
	}
}

int32_t handleMedicineHealing(int16_t injuryChance, int16_t sicknessChance)
{
	int16_t roll;

	if (((PARTNER_PARA.condition & CONDITION_INJURED) != 0) && (roll = randomLimit(3), roll < injuryChance)) {
		PARTNER_PARA.condition &= ~CONDITION_INJURED;
		PARTNER_PARA.injuryTimer = 0;
	}
	if (((PARTNER_PARA.condition & CONDITION_SICK) != 0) && (roll = randomLimit(10), roll < sicknessChance)) {
		PARTNER_PARA.condition &= ~CONDITION_SICK;
		PARTNER_PARA.sicknessTimer = 0;
		PARTNER_PARA.areaEffectTimer = 0;
		return 1;
	}
	return 0;
}

void handlePortaPotty(void)
{
	if (PARTNER_PARA.condition & CONDITION_POOPY) {
		PARTNER_PARA.poopLevel =
			RAISE_DATA[ENTITY_TABLE[1]->type].poopTimer;
		PARTNER_PARA.condition &= ~CONDITION_POOPY;
		handlePoopWeightLoss(ENTITY_TABLE[1]->type);
	}
}

void modifyLifetime(int16_t delta)
{
	int16_t *lifetime = &PARTNER_PARA.remainingLifetime;

	*lifetime += delta;
	if (*lifetime < 0) {
		*lifetime = 0;
	}
}

void addEnergy(int16_t amount)
{
	PARTNER_PARA.energyLevel += amount;
	if (PARTNER_PARA.energyLevel >
	    RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyCap) {
		PARTNER_PARA.energyLevel =
			RAISE_DATA[PARTNER_ENTITY.digimonEntity.entity.type].energyCap;
	}
}

void reduceTiredness(int16_t amount)
{
	PARTNER_PARA.tiredness -= amount;
	if (PARTNER_PARA.tiredness <= 0) {
		PARTNER_PARA.tiredness = 0;
	}
}

void addHappiness(int16_t amount)
{
	PARTNER_PARA.happiness += amount;
	if (PARTNER_PARA.happiness >= 0x64) {
		PARTNER_PARA.happiness = 0x64;
	}
}

void addDiscipline(int16_t amount)
{
	PARTNER_PARA.discipline += amount;
	if (PARTNER_PARA.discipline >= 0x64) {
		PARTNER_PARA.discipline = 0x64;
	}
}

void addWeight(int16_t amount)
{
	PARTNER_PARA.weight += amount;
	if (PARTNER_PARA.weight >= 0x64) {
		PARTNER_PARA.weight = 0x63;
	}

	if (PARTNER_PARA.weight <= 0) {
		PARTNER_PARA.weight = 1;
	}
}

void decreasePoopLevel(void)
{
	PARTNER_PARA.poopLevel--;
}

void setTrainingBoost(flag, value, duration)
int16_t flag;
int16_t value;
int16_t duration;
{
	PARTNER_PARA.trainBoostFlag = flag;
	PARTNER_PARA.trainBoostValue = value;
	PARTNER_PARA.trainBoostTimer = duration * 1200;
}

void handleItemSickness(int16_t chance)
{
	int32_t isSick;
	int16_t r;
#if defined(VERSION_JP)
	int32_t halfLen;
#else
	char buf[0x18];
#endif

	r = randomLimit(0x64);
	isSick = PARTNER_PARA.condition & CONDITION_SICK;
	if ((r < chance) && (!isSick)) {
		PARTNER_PARA.condition |= CONDITION_SICK;
		PARTNER_PARA.timesBeingSick++;
		PARTNER_PARA.sicknessTimer = 1;
		if (PARTNER_PARA.condition & CONDITION_INJURED) {
			PARTNER_PARA.condition &= ~CONDITION_INJURED;
			PARTNER_PARA.injuryTimer = 0;
		}
		tamerSetState(0x14);
		clearTextArea();
		setTextColor(0xa);
#if defined(VERSION_JP)
		drawString(PARTNER_ENTITY.name, 0, 0x78);
		halfLen = strlen(PARTNER_ENTITY.name) / 2;
		setTextColor(1);
		drawString(IS_SICK_SUFFIX, halfLen * 12, 0x78);
#else
		sprintf(buf, NAME_FORMAT, PARTNER_ENTITY.name);
		strcat(buf, IS_SICK_SUFFIX);
		drawString(buf, 0, 0x78);
#endif
	}
}
