#ifndef DW_TEXT_H
#define DW_TEXT_H

/*
 * Dialogue text format.
 *
 * The text shown by scripts (SCN/DG.SCN and SCN/MAPHEAD.SCN) is a sequence of
 * two-byte units. A unit is either a Shift JIS character (the US release uses
 * full-width letters, so "Ｈｉ" rather than "Hi") or a control code followed
 * by one argument byte. A message ends with TEXT_NEWLINE followed by TEXT_END.
 *
 * showTextbox() lays a message out into rows of TEXT_ROW_SIZE bytes, expanding
 * the codes that insert names and numbers. drawString2() then draws one row
 * into the text area of VRAM and handles colors and tab stops.
 */
#define TEXT_END		0x00
#define TEXT_COLOR		0x01	/* arg: TEXT_COLOR_* */
#define TEXT_SKIP_1		0x02	/* ignored, with one argument byte */
#define TEXT_SKIP_3		0x03	/* ignored, with three argument bytes */
#define TEXT_PSTAT_NUMBER	0x04	/* arg: pstat, printed as a number */
#define TEXT_PLAYER_NAME	0x05
#define TEXT_PARTNER_NAME	0x06
#define TEXT_DIGIMON_NAME	0x07	/* arg: pstat holding a Digimon id */
#define TEXT_MOVE_NAME		0x08	/* arg: pstat holding a move id */
#define TEXT_ITEM_NAME		0x09	/* arg: pstat holding an item id */
#define TEXT_MONEY		0x0A
#define TEXT_MERIT		0x0B
#define TEXT_TAB_8		0x0C	/* next tab stop, every 8 glyphs */
#define TEXT_NEWLINE		0x0D
#define TEXT_TAB_11		0x0E	/* next tab stop, every 11 glyphs */
#define TEXT_HALF_SPACE		0x0F
#define TEXT_PRICE		0x10	/* the price a shop script set */
#define TEXT_BGM_NAME		0x11	/* arg: pstat holding a jukebox track */
#define TEXT_CUP_NAME		0x12	/* arg: pstat holding a tournament cup */
#define TEXT_TOURNAMENTS_WON	0x13
#define TEXT_TOURNAMENT_WINS	0x14
#define TEXT_TOURNAMENTS_LOST	0x15
/*
 * Move the cursor to a fixed x, blanking the pixels in between. The number is
 * the new x. TEXT_COLUMN_105_ALT is the same, but blanks from 6 pixels back.
 */
#define TEXT_COLUMN_105_ALT	0x16
#define TEXT_COLUMN_105		0x17
#define TEXT_COLUMN_96		0x18
#define TEXT_COLUMN_156		0x19
#define TEXT_COLUMN_180		0x1A
#define TEXT_COLUMN_160		0x1B
#define TEXT_COLUMN_100		0x1C

/* Palette indices of FONT_CLUT. */
#define TEXT_COLOR_WHITE	1
#define TEXT_COLOR_BLUE		2
#define TEXT_COLOR_RED		3
#define TEXT_COLOR_PINK		4
#define TEXT_COLOR_GREEN	5
#define TEXT_COLOR_LIGHT_BLUE	6
#define TEXT_COLOR_YELLOW	7
#define TEXT_COLOR_ORANGE	10

/*
 * Who is talking. Values below SPEAKER_SPECIAL are the script ids of the NPCs
 * on the map, and their name is the name of their Digimon. The ones from
 * SPEAKER_SPECIAL up are names of their own ("Sign", "Box", ...).
 */
#define SPEAKER_SPECIAL		0xC8
#define SPEAKER_PARTNER		0xFC
#define SPEAKER_PLAYER		0xFD
#define SPEAKER_NARRATOR	0xFE	/* no name, keeps the current speaker */
#define SPEAKER_NONE		0xFF

/* Laid out text: one row of the text buffer per line of a textbox. */
#define TEXT_ROW_SIZE		0x40
#define TEXT_LINE_HEIGHT	12
#define TEXT_GLYPH_WIDTH	12

/* Shift JIS characters, as read from text with the lead byte on top. */
#define SJIS_SPACE		0x8140	/* "　" */
#define SJIS_DIGIT_ZERO		0x824F	/* "０" */

#endif
