#ifndef DW_FONT_H
#define DW_FONT_H

#include <libgpu.h>

#include <dw/types.h>
#include <dw/version.h>

void initializeFontCLUT(void);
void clearTextArea(void);
void clearTextSubArea(RECT *rect);
void setTextColor(int32_t color);
#if VERSION_REGION_IS(NTSCJ)
void drawGlyph(uint16_t codepoint, uint16_t x, uint16_t y);
#else
int32_t drawGlyph(/* uint16_t codepoint, uint16_t x, uint16_t y */);
#endif
void drawString(/* char *str, uint16_t x, uint16_t y */);

#endif
