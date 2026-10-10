#ifndef DW_FONT_H
#define DW_FONT_H

#include <libgpu.h>

#include <dw/types.h>
#include <dw/version.h>

#if VERSION_IS(EU)
#define GLYPH_WIDTH 8
#else
#define GLYPH_WIDTH 12
#endif

void initializeFontCLUT(void);
void clearTextArea(void);
void clearTextSubArea(RECT *rect);
void setTextColor(int32_t color);
#if VERSION_IS(EU)
int32_t drawGlyph(uint16_t codepoint, uint16_t x, uint16_t y);
#elif !VERSION_IS(US)
void drawGlyph(uint16_t codepoint, uint16_t x, uint16_t y);
#else
int32_t drawGlyph(/* uint16_t codepoint, uint16_t x, uint16_t y */);
#endif
void drawString(/* char *str, uint16_t x, uint16_t y */);

#endif
