#include <dw/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <dw/entity.h>
#include "common.h"

INCLUDE_ASM("asm/main/nonmatchings/anim", calculatePosMatrix);
INCLUDE_ASM("asm/main/nonmatchings/anim", resetMomentumData);
INCLUDE_ASM("asm/main/nonmatchings/anim", animateEntityTexture);
INCLUDE_ASM("asm/main/nonmatchings/anim", setupModelMatrix);
INCLUDE_ASM("asm/main/nonmatchings/anim", startAnimation);
INCLUDE_ASM("asm/main/nonmatchings/anim", tickAnimation);
INCLUDE_ASM("asm/main/nonmatchings/anim", tickMomentum);
INCLUDE_ASM("asm/main/nonmatchings/anim", readMomentumInstructions);
INCLUDE_ASM("asm/main/nonmatchings/anim", readMomentumInstruction);
INCLUDE_ASM("asm/main/nonmatchings/anim", applyMomentum);
INCLUDE_ASM("asm/main/nonmatchings/anim", applyRootMomentum);
