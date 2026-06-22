#include <dw/clock.h>
#include <dw/fade.h>
#include <dw/file_queue.h>
#include <dw/map_object.h>
#include <dw/params.h>
#include <dw/script.h>
#include <dw/sound.h>
#include <dw/sound_async.h>
#include <dw/tournament.h>
#include <dw/ui.h>
#include <dw/utils.h>

#include "common.h"

void resetMapObjectAnimation(int32_t a0, int32_t a1);
void createMeramonShake(void);
void createNinjamonEffect(void);
void openSaveMachine(void);
void gameClearSave(void);
void spawnItem(int32_t type, int16_t x, int16_t y);
void spawnSpriteAtLocation(int16_t x, int16_t y, int16_t z, int16_t w,
			   int32_t type);
void spawnSpriteAtEntity(int32_t entId, int32_t sprite, int32_t param);
void setRectImpassible(int16_t x, int16_t y, int8_t w, int8_t h);
void addEntityText(int32_t a0, int32_t a1, int32_t a2, int32_t a3,
		   int32_t a4);
int32_t startBattle(int16_t instanceId);
void setLoopCountToOne(int32_t a0);
void MAIN_func_800D9360(int32_t a0);
void DOO2_func_8007189C(void);
void TRN2_func_80089134(int32_t mapId);
void TRN2_func_80089A6C(int32_t mapId);
void TRN2_func_8008A654(int32_t mapId);
void TRN2_func_8008A00C(int32_t mapId);
void TRN2_func_800896B4(int32_t mapId);
void TRN_func_8008AAF0(int32_t mapId);
void TRN_func_8008B5E8(int32_t mapId);
void TRN_func_8008CA04(int32_t mapId);
void TRN_func_8008C068(int32_t mapId);
void TRN_func_8008B0D0(int32_t mapId);
void TRN_func_8008CE90(int32_t mapId);
void EAB_func_8006148C(Entity *e);
int32_t loadTextureFile(char *path, uint32_t *outTPage, uint32_t *outClut);

void setMapHeadActive(void);

extern char MAIN_D_80130394[];
extern char MAIN_D_801303A8[];
extern uint8_t PREVIOUS_SCREEN;
extern uint8_t PREVIOUS_EXIT;
extern uint8_t CURRENT_EXIT;
extern uint8_t MAIN_D_801BE738[];

extern uint8_t MAIN_D_801BE6B4[];

static void *script_instr64_functions[] = {
	returnFromScriptFile,
	setMapHeadActive,
	scriptInstruction64to7E,
};

void scriptInstruction64to7E(int32_t op)
{
	StackEntry entry;
	int16_t posX;
	int16_t posY;
	int16_t posZ;
	int16_t posW;
	uint16_t triggerId;
	uint8_t byteArg1;
	uint8_t byteArg2;
	uint8_t byteArg3;
	uint8_t entityId;
	uint8_t padByte;
	uint8_t *b;

	switch (op) {
	case 0x64:
		pollNextScriptUByte(&byteArg1);
		MAIN_D_80134FF8 = byteArg1;
		switch (byteArg1) {
		case 0x16:
			if (checkTournamentMedalConditions() == -1) break;
			/* fall through */
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x06:
		case 0x08:
		case 0x09:
		case 0x0a:
		case 0x0b:
		case 0x0e:
		case 0x12:
		case 0x2f:
			ACTIVE_INSTRUCTION = 0x64;
			SELECTION_MENU_STATE = 0;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x07:
			byteArg1 = readPStat(0xfe);
			if ((MAIN_D_80134FFE == 0x6b) ||
			    (MAIN_D_80134FFE == 0x6c) ||
			    (MAIN_D_80134FFE == 0xa5) ||
			    (MAIN_D_80134FFE == 0x63)) {
				switch (byteArg1) {
				case 0:
					TRN2_func_80089134(MAIN_D_80134FFE);
					break;
				case 1:
					TRN2_func_80089A6C(MAIN_D_80134FFE);
					break;
				case 2:
					TRN2_func_8008A654(MAIN_D_80134FFE);
					break;
				case 3:
					TRN2_func_8008A00C(MAIN_D_80134FFE);
					break;
				case 4:
					TRN2_func_800896B4(MAIN_D_80134FFE);
					break;
				}
			} else {
				switch (byteArg1) {
				case 0:
					TRN_func_8008AAF0(MAIN_D_80134FFE);
					break;
				case 1:
					TRN_func_8008B5E8(MAIN_D_80134FFE);
					break;
				case 2:
					TRN_func_8008CA04(MAIN_D_80134FFE);
					break;
				case 3:
					TRN_func_8008C068(MAIN_D_80134FFE);
					break;
				case 4:
					TRN_func_8008B0D0(MAIN_D_80134FFE);
					break;
				case 5:
					TRN_func_8008CE90(MAIN_D_80134FFE);
					break;
				}
			}
			ACTIVE_INSTRUCTION = 0x64;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
			break;
		case 0x0c:
			byteArg1 = readPStat(0xfe);
			if (byteArg1 != 0xff) removeItem(byteArg1, 1);
			break;
		case 0x0d:
			byteArg1 = readPStat(0xfe);
			if (byteArg1 != 0xff) removeItem(byteArg1, 0x63);
			break;
		case 0x0f:
			MAIN_func_800FC968(1);
			break;
		case 0x10:
			triggerBoxCloseFlag(2);
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x13:
			byteArg1 = readPStat(0xfe);
			setDirtCartModel(byteArg1);
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x14:
			decreaseDirtPileSize();
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x15:
			byteArg1 = readPStat(0xf7);
			byteArg2 = readPStat(0xf8);
			resetMapObjectAnimation(byteArg1, byteArg2);
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x1d:
			DOO2_func_8007189C();
			/* fall through */
		case 0x17: case 0x18: case 0x32:
			ACTIVE_INSTRUCTION = 0x64;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x19:
			MAIN_D_80134FE7 = readPStat(0xfe);
			break;
		case 0x1a:
			loadDirtCartModel();
			break;
		case 0x1b:
			loadDirtPileModel();
			break;
		case 0x1c:
			createMonochromonMoodBubble();
			break;
		case 0x1e:
			MAIN_D_8013500C = 0;
			break;
		case 0x1f:
			{
				int32_t hi = readPStat(0xf3) << 8;
				int32_t lo = readPStat(0xf4);
				posX = lo + hi;
				MAIN_D_8013500C += posX;
			}
			break;
		case 0x38:
			writePStat(0xf3, (MAIN_D_8013500C / 256) & 0xff);
			writePStat(0xf4, MAIN_D_8013500C & 0xff);
			break;
		case 0x20:
			initializeNamingBuffer(readPStat(0xfe));
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x21:
			setTrigger(0x25);
			setTrigger(0x26);
			setTrigger(0x27);
			if (DAY != 0x16) {
				byteArg1 = 0x95;
			} else {
				byteArg1 = 0x16;
			}
			writePStat(2, byteArg1);
			writePStat(3, 0x16);
			writePStat(4,
				   PARTNER_ENTITY.digimonEntity.entity.type);
			break;
		case 0x22:
			createMeramonShake();
			break;
		case 0x23:
			MAIN_func_800FF9AC();
			break;
		case 0x24:
			readMapTFS(MAIN_D_80134FFE);
			break;
		case 0x27:
			MAIN_func_800D9360(MAIN_D_80134FFE);
			break;
		case 0x30:
			openSaveMachine();
			/* fall through */
		case 0x25:
			ACTIVE_INSTRUCTION = 0x64;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x36:
			gameClearSave();
			ACTIVE_INSTRUCTION = 0x64;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
		case 0x26:
			createNinjamonEffect();
			break;
		case 0x33:
			addFileReadRequestPath(MAIN_D_80130394,
					       (uint8_t *)0x80058000, 0, 0,
					       0);
			loadDynamicLibrary(EAB_REL, 0, 0, 0, 0);
			readVBALLSection(5, 0x73);
			loadMapSounds2(0x15);
			break;
		case 0x34:
			loadTextureFile(MAIN_D_801303A8, 0, 0);
			loadDynamicLibrary(ENDI_REL, 0, 1, 0, 0);
			loadMapSounds2(0x14);
			break;
		case 0x28:
			isSoundLoaded(0, 8);
			EAB_func_8006148C(
				ENTITY_TABLE[scriptIdToEntityId(5)]);
			break;
		case 0x35:
			isSoundLoaded(0, 8);
			setTamerState(0x10);
			SOME_SCRIPT_SYNC_BIT = 0;
			ACTIVE_INSTRUCTION = 0x64;
			SCRIPT_STATE_3 = 0;
			longjmp(SCRIPT_JMP_BUF, 2);
			break;
		case 0x37:
			setLoopCountToOne(readPStat(0xfe));
			longjmp(SCRIPT_JMP_BUF, 2);
			break;
		case 0x29:
			spawnGearbox();
			break;
		case 0x2a:
			{
				int8_t pstat = readPStat(0xfe);
				somethingToyTown(pstat);
			}
			break;
		case 0x2b:
			spawnToyTownBoxes();
			break;
		case 0x2c:
			{
				int8_t value = readPStat(0xfe);
				openToyTownBox(value);
			}
			break;
		case 0x2d:
			{
				int16_t value = readPStat(0xfe);
				fadeToWhite(value);
			}
			break;
		case 0x2e:
			{
				int16_t value = readPStat(0xfe);
				fadeFromWhite(value);
			}
			break;
		case 0x31:
			spawnAngemonPedestal();
			break;
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x11:
			goto script_end;
		}
		break;
	case 0x65:
		pollNextScriptUByte(&byteArg1);
		PARTNER_PARA.condition &= ~byteArg1;
		break;
	case 0x66:
		pollNextScriptUByte(&byteArg1);
		if (MAIN_D_80134FC8 < 0x270f) {
			MAIN_D_80134FC8++;
		}
		byteArg1 = readPStat(0xfa);
		if (byteArg1 != 0) {
			for (byteArg1 = 0xfb; byteArg1 < 0xfe; byteArg1++) {
				byteArg2 = readPStat(byteArg1);
				if (byteArg2 != 0xff) {
					byteArg2 =
						scriptIdToEntityId(byteArg2);
					writePStat(byteArg1, byteArg2);
				}
			}
		}
		stopBGM();
		{
			int16_t outcome;

			outcome = startBattle(MAIN_D_80134F9C);
			writePStat(0xff, outcome);
			if (outcome == -1) {
				handleItemLoss();
				PARTNER_ENTITY.lives -= 1;
				if (PARTNER_ENTITY.lives == 0) {
					PARTNER_PARA.remainingLifetime = 0;
				}
				CURRENT_SCRIPT_PTR = (int32_t)getScript(0);
				MAIN_D_80134FDC = getScriptSection((uint8_t *)CURRENT_SCRIPT_PTR, 0x4de);
				break;
			}
			if (outcome == 0) {
				if (MAIN_D_80134FCA < 0x270f) {
					MAIN_D_80134FCA++;
				}
				MAIN_D_80134FA0 = 0;
				MAIN_D_80134FF8 = PREVIOUS_SCREEN;
				SELECTION_MENU_STATE = PREVIOUS_EXIT;
				PREVIOUS_EXIT = CURRENT_EXIT;
				entry.smth[0] = 4;
				entry.smth[1] = 0xff;
				pushScriptStack(&entry);
				longjmp(SCRIPT_JMP_BUF, 3);
			}
		}
		if (isTriggerSet(1) != 0) {
			setMovementEnabled(-1, 1);
			MAIN_func_8010020C();
			break;
		}

		b = (uint8_t *)&MAIN_D_801BE72C;
		b[0] = 7;
		b[1] = 0xfd;
		b[3] = 0xa;

		ACTIVE_INSTRUCTION = 0x4a;
		MAIN_D_80134FA4 = 0xa;
		MAIN_func_8010020C();
		longjmp(SCRIPT_JMP_BUF, 2);
	case 0x67:
		skipOneReadOneUShort(&MAIN_D_80134FFC);
		ACTIVE_INSTRUCTION = 0x67;
		longjmp(SCRIPT_JMP_BUF, 2);
	case 0x68:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		MAIN_D_80134FE5 = byteArg1;
		if (byteArg1 != 2) break;
		MAIN_D_80135010 = byteArg2;
		break;
	case 0x69:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		{
			int32_t damage;
			damage = (PARTNER_ENTITY.digimonEntity.stats.base.hp *
				  readPStat(byteArg1)) / 100;
			if (PARTNER_ENTITY.digimonEntity.stats.current
					.currentHP - (int16_t)damage < 1) {
				damage = PARTNER_ENTITY.digimonEntity.stats
						.current.currentHP - 1;
			}
			PARTNER_ENTITY.digimonEntity.stats.current
					.currentHP -= damage;
			addEntityText((int32_t)ENTITY_TABLE[1], 0, 0, damage,
				      0);
		}
		break;
	case 0x6a:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		byteArg1 = scriptIdToEntityId(byteArg1);
		if (byteArg1 == 0xff || byteArg1 < 2) {
			break;
		}
		NPC_ENTITIES[byteArg1 - 2].autotalk = byteArg2;
		break;
	case 0x6b:
		break;
	case 0x6c:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = MAIN_D_801BE6B4 + entityId * 0xc;
			b[0] = 8;
			b[1] = byteArg1;
			b[3] = byteArg2;
			*(int16_t *)(b + 4) = posX;
			*(int16_t *)(b + 6) = posY;
		}
		break;
	case 0x6d:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = MAIN_D_801BE6B4 + entityId * 0xc;
			b[0] = 9;
			b[1] = byteArg1;
			b[2] = byteArg2;
			b[3] = byteArg3;
		}
		break;
	case 0x6e:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = MAIN_D_801BE6B4 + entityId * 0xc;
			b[0] = 0xb;
			b[1] = byteArg1;
			b[3] = byteArg2;
			*(int16_t *)(b + 4) = posX;
			*(int16_t *)(b + 6) = posY;
		}
		break;
	case 0x6f:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = MAIN_D_801BE6B4 + entityId * 0xc;
			b[0] = 0xb;
			b[1] = byteArg1;
			b[2] = byteArg2;
			b[3] = byteArg3;
		}
		break;
	case 0x70:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		MAIN_D_801BE738[0] = 0xc;
		MAIN_D_801BE738[1] = byteArg1;
		MAIN_D_801BE738[2] = byteArg3;
		break;
	case 0x71:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		pollNextTwoScriptBytes(&entityId, &padByte);
		pollNextTwoScriptShorts(&posX, &posY);
		byteArg1 += 0xc;
		b = MAIN_D_801BE6B4 + byteArg1 * 0xc;
		b[0] = 0xd;
		b[1] = byteArg2;
		b[3] = byteArg3;
		b[2] = entityId;
		*(int16_t *)(b + 8) = posX;
		*(int16_t *)(b + 0xa) = posY;
		break;
	case 0x72:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextScriptShort(&posX);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = MAIN_D_801BE6B4 + entityId * 0xc;
			b[0] = 0xe;
			b[1] = byteArg1;
			b[2] = byteArg2;
			*(int16_t *)(b + 4) = posX;
			b[3] = byteArg3;
		}
		break;
	case 0x73:
		MAIN_func_801062F8(0xff);
		pollNextScriptUByte(&byteArg1);
		pollNextScriptShort(&posX);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		entityId = scriptIdToEntityId(byteArg1);
		&entityId;
		if (entityId != 0xff) {
			b = MAIN_D_801BE6B4 + entityId * 0xc;
			b[0] = 0xf;
			b[1] = byteArg1;
			b[2] = byteArg2;
			*(int16_t *)(b + 4) = posX;
			b[3] = byteArg3;
		}
		break;
	case 0x74:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		spawnItem(byteArg1, posX, posY);
		break;
	case 0x75:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptShorts(&posZ, &posW);
		pollNextScriptUShort(&triggerId);
		spawnChest(posX, posY, posZ, posW, byteArg1, triggerId);
		break;
	case 0x76:
		pollNextScriptUByte(&byteArg1);
		spawnBoulder();
		break;
	case 0x77:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		moveBoulder(posX, posY);
		longjmp(SCRIPT_JMP_BUF, 2);
	case 0x78:
		pollNextScriptUByte(&byteArg1);
		removeObject(0xfb6, 0);
		break;
	case 0x79:
		pollNextScriptUByte(&byteArg1);
		scriptUnloadModel(byteArg1);
		break;
	case 0x7a:
		skipOnePollTwoScriptBytes(&byteArg1, &byteArg2);
		byteArg1 = readPStat(byteArg1);
		writePStat(byteArg2, byteArg1);
		break;
	case 0x7b:
		pollNextScriptUByte(&byteArg1);
		entry.smth[0] = 4;
		entry.smth[1] = byteArg1;
		pushScriptStack(&entry);
		writePStat(0, MAIN_D_80134FE7);
		break;
	case 0x7c:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		setRectImpassible(posX, posY, byteArg2,
				  byteArg3);
		break;
	case 0x7d:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptShorts(&posX, &posY);
		pollNextTwoScriptShorts(&posZ, &posW);
		spawnSpriteAtLocation(posX, posY, posZ, posW, byteArg1);
		break;
	case 0x7e:
		pollNextScriptUByte(&byteArg1);
		pollNextTwoScriptBytes(&byteArg2, &byteArg3);
		spawnSpriteAtEntity(byteArg1, byteArg2, byteArg3);
		break;
	}
script_end:
	longjmp(SCRIPT_JMP_BUF, 1);
}

INCLUDE_ASM("asm/main/nonmatchings/script_instr64", returnFromScriptFile);

void setMapHeadActive(void)
{
	uint8_t *scriptPtr;
	int32_t off;

	scriptPtr = MAPHEAD_DATA_PTR;
	off = *(int16_t *)scriptPtr;
	CURRENT_SCRIPT_PTR = (int32_t)scriptPtr;
	MAIN_D_80134FDC = scriptPtr + (off & 0xffff) - 2;
}
