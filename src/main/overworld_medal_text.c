#include <dw/font.h>
#include <dw/graphics.h>
#include <dw/ui.h>

extern int8_t MENU_SUB_STATE;
extern int8_t MEDAL_SELECTOR_INDEX;
extern RECT MEDAL_VIEW_TEXT_AREA;
extern RECT MEDAL_DETAIL_AREA;
extern char MAIN_D_80124C54[];
extern char *PLAYER_VIEW_LABELS[];
extern char *MEDAL_NAMES[];
extern char *MEDAL_DESCRIPTIONS[];

int32_t drawMedalViewStrings(void);

int32_t drawMedalViewStrings(void)
{
	RECT rect1;
	RECT rect2;
	int32_t i;

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
		i = MENU_SUB_STATE - 3;
		drawString(MEDAL_DESCRIPTIONS[i + MEDAL_SELECTOR_INDEX * 3], 0,
			   i * 0xc + 0x24);
		MENU_SUB_STATE = MENU_SUB_STATE + 1;
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
