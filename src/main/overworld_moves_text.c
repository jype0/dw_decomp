#include <dw/evl.h>
#include <dw/font.h>

extern int8_t MENU_SUB_STATE;
extern char MAIN_D_80124C54[];
extern RECT MOVES_VIEW_TEXT_AREA;
extern uint8_t EQUIPPED_MOVES[4];
extern int16_t MAIN_D_80134D38;
extern int16_t MAIN_D_80134D3A;
extern char *STATUS_VIEW_LABELS[];
extern char *TECH_VIEW_LABELS[];
extern char *MOVE_NAMES[];

int32_t entityGetTechFromAnim(Entity *entity, int32_t anim);
int32_t drawDigimonMovesText(void);

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
		DrawSync(0);
		return 0;
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
		DrawSync(0);
		return 0;
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
		DrawSync(0);
		return 0;
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
