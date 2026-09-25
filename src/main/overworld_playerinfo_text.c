#include <stdio.h>

#include <dw/clock.h>
#include <dw/font.h>
#include <dw/params.h>
#include <dw/ui.h>

extern int8_t MENU_SUB_STATE;
extern char FULLWIDTH_DIGITS[];
extern RECT PLAYER_INFO_TEXT_AREA;
extern char PLAYTIME_FORMAT[8];
extern char *PLAYER_VIEW_LABELS[];
extern char *TAMER_LEVEL_NAMES[];

int32_t drawPlayerInfoStrings(void);

int32_t drawPlayerInfoStrings(void)
{
	RECT rect;
	char buf[8];
	int32_t i;

	rect = PLAYER_INFO_TEXT_AREA;
	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect);
		drawString(FULLWIDTH_DIGITS, 0, 0xf0);
		MENU_SUB_STATE = 1;
		break;
	case 1:
		sprintf(buf, PLAYTIME_FORMAT, (int)PLAYTIME_HOURS, (int)PLAYTIME_MINUTES);
		drawString(buf, 0, 0xe4);
	case 2:
	case 3:
		drawString(PLAYER_VIEW_LABELS[MENU_SUB_STATE + 1], 0,
		    (MENU_SUB_STATE - 1) * 0xc + 0xc);
		MENU_SUB_STATE = MENU_SUB_STATE + 1;
		break;
	case 4:
		drawString((char *)DIGIMON_DATA, 0, 0x30);
		for (i = 0; i < 4; i++) {
			drawString(TAMER_LEVEL_NAMES[i], 0, i * 0xc + 0x40);
		}
		MENU_SUB_STATE = 5;
		break;
	case 5:
		for (i = 4; i < 0xb; i++) {
			drawString(TAMER_LEVEL_NAMES[i], 0, i * 0xc + 0x40);
		}
		return 1;
	}
	return 0;
}
