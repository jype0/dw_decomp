#include <dw/font.h>
#include <dw/graphics.h>
#include <dw/params.h>
#include <dw/ui.h>

extern int8_t MENU_SUB_STATE;
extern char FULLWIDTH_DIGITS[];
extern int16_t MAIN_D_80134D42;
extern int16_t MAIN_D_80134D44;
extern RECT EVO_CHART_TEXT_AREA;
extern RECT EVO_CHART_NAME_AREA;
extern char *CARD_CHART_LABELS[];

int32_t drawEvoChartStrings(int32_t arg);

int32_t drawEvoChartStrings(int32_t arg)
{
	RECT rect1;
	RECT rect2;
	int32_t result;

	rect1 = EVO_CHART_TEXT_AREA;
	rect2 = EVO_CHART_NAME_AREA;
	switch (MENU_SUB_STATE) {
	case 0:
		clearTextSubArea(&rect1);
		drawString(FULLWIDTH_DIGITS, 0, 0xf0);
		drawString(CARD_CHART_LABELS[1], 0, 0xc);
		MAIN_D_80134D44 = 0;
		MAIN_D_80134D42 = 0;
		MENU_SUB_STATE = 1;
		/* fall through */
	case 1:
		drawString(CARD_CHART_LABELS[2], 0, 0x18);
		drawString(CARD_CHART_LABELS[3], 0, 0x24);
		drawString(CARD_CHART_LABELS[4], 0, 0x3c);
		result = 1;
		break;
	case 2:
		clearTextSubArea(&rect2);
		drawString((char *)(DIGIMON_DATA + arg), 0, 0x30);
		/* fall through */
	default:
		result = 0;
		break;
	}
	return result;
}
