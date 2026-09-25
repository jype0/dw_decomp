#include <libgpu.h>

#include <dw/font.h>
#include <dw/graphics.h>

extern RECT CARD_VIEW_TEXT_AREA;
extern char *CARD_CHART_LABELS[];
extern char FULLWIDTH_DIGITS[];
extern int8_t MENU_SUB_STATE;
extern int8_t SELECTED_CARD;

int32_t drawCardViewStrings(void);

int32_t drawCardViewStrings(void)
{
	RECT rect;
	int32_t state;

	rect = CARD_VIEW_TEXT_AREA;
	state = MENU_SUB_STATE;
	if (state != 1) {
		if (state != 0) {
			goto ret0;
		}
		clearTextSubArea(&rect);
		drawString(FULLWIDTH_DIGITS, 0, 0xF0);
		MENU_SUB_STATE = 1;
		goto ret0;
	}
	drawString(CARD_CHART_LABELS[0], 0, 0xC);
	SELECTED_CARD = 0;
	return 1;
ret0:
	return 0;
}
