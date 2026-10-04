#include <libgpu.h>
#include <libgs.h>

#include <dw/line.h>
#include <dw/types.h>

extern GsOT *ACTIVE_ORDERING_TABLE;

void setLineBlendingMode(int32_t mode, int32_t order);

void setLineBlendingMode(int32_t mode, int32_t order)
{
	DR_TPAGE *prim;

	if (mode == 0) {
		return;
	}

	prim = (DR_TPAGE *)GsGetWorkBase();
	setDrawTPage(prim, 1, 1, getTPage(0, mode & 3, 0, 0));

	addPrim(&ACTIVE_ORDERING_TABLE->org[order], prim);
	GsSetWorkBase((PACKET *)&prim[1]);
}

void drawLine3P(uint32_t color, int32_t x0, int32_t y0,
                int32_t x1, int32_t y1, int32_t x2, int32_t y2,
                int32_t order, uint32_t mode)
{
	LINE_F3 *prim;

	prim = (LINE_F3 *)GsGetWorkBase();
	*(uint32_t *)&prim->r0 = color;
	setXY3(prim, x0, y0, x1, y1, x2, y2);
	setLineF3(prim);
	setSemiTrans(prim, mode >> 2);

	addPrim(&ACTIVE_ORDERING_TABLE->org[order], prim);
	GsSetWorkBase((PACKET *)&prim[1]);

	setLineBlendingMode(mode, order);
}

void drawLine2P(uint32_t color, int32_t x0, int32_t y0, int32_t x1,
                int32_t y1, int32_t order, uint32_t mode)
{
	LINE_F2 *prim;

	prim = (LINE_F2 *)GsGetWorkBase();

	*(uint32_t *)&prim->r0 = color;
	setXY2(prim, x0, y0, x1, y1);
	setLineF2(prim);
	setSemiTrans(prim, mode >> 2);

	addPrim(&ACTIVE_ORDERING_TABLE->org[order], prim);
	GsSetWorkBase((PACKET *)&prim[1]);

	setLineBlendingMode(mode, order);
}
