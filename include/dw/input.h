#ifndef DW_INPUT_H
#define DW_INPUT_H

#include <dw/version.h>

#if VERSION_REGION_IS(NTSCJ)
#define CONFIRM_BUTTON 0x20
#define CANCEL_BUTTON 0x40
#define ALT_BUTTON 0x10
#else
#define CONFIRM_BUTTON 0x40
#define CANCEL_BUTTON 0x10
#define ALT_BUTTON 0x20
#endif

#endif
