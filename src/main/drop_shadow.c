#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>

#include <dw/aabb.h>
#include <dw/entity.h>
#include <dw/garbage.h>
#include <dw/params.h>
#include <dw/types.h>

extern GsOT *ACTIVE_ORDERING_TABLE;

void setRotTransMatrix(MATRIX *m);
int32_t getTileTrigger(VECTOR *loc);
void renderDropShadow(Entity *entity);

GARBAGE(renderDropShadow, 19);

void renderDropShadow(Entity *entity)
{
	SVECTOR p0;
	SVECTOR p1;
	SVECTOR p2;
	SVECTOR p3;
	long depth;
	long flag;
	GsOT_TAG *ot;
	AABB box;
	SVECTOR c;
	POLY_FT4 *prim;
	int16_t x;
	int16_t y;
	int16_t z;
	int16_t radiusX;
	int16_t radiusZ;

	if (getTileTrigger(&entity->posData->location) == -1) {
		return;
	}
	radiusX = DIGIMON_DATA[entity->type].radius;
	radiusZ = DIGIMON_DATA[entity->type].radius;
	x = entity->posData->location.vx;
	y = entity->posData->location.vy;
	z = entity->posData->location.vz;
	p0.vx = x - radiusX;
	p0.vy = y;
	p0.vz = z - radiusZ;
	p1.vx = x + radiusX;
	p1.vy = y;
	p1.vz = z - radiusZ;
	p2.vx = x - radiusX;
	p2.vy = y;
	p2.vz = z + radiusZ;
	p3.vx = x + radiusX;
	p3.vy = y;
	p3.vz = z + radiusZ;
	prim = (POLY_FT4 *)GsGetWorkBase();
	SetPolyFT4(prim);
	SetSemiTrans(prim, 1);
	prim->tpage = getTPage(1, 2, 832, 256);
	setClut(prim, 0, 0x1E7);
	setUVWH(prim, 0x40, 0x80, 63, 63);
	setRotTransMatrix(&GsWSMATRIX);
	RotTransPers4(&p0, &p1, &p2, &p3, (long *)&prim->x0, (long *)&prim->x1, (long *)&prim->x2,
	              (long *)&prim->x3, &depth, &flag);
	prim->r0 = prim->g0 = prim->b0 = 0x30;
	ot = ACTIVE_ORDERING_TABLE->org;
	AddPrim(ot + 0xFFD, prim);
	prim++;
	GsSetWorkBase((PACKET *)prim);
	c.vx = entity->posData->location.vx;
	c.vy = -(DIGIMON_DATA[entity->type].height >> 1);
	c.vz = entity->posData->location.vz;
	box.center = &c;
	box.extent.vx = DIGIMON_DATA[entity->type].radius;
	box.extent.vy = DIGIMON_DATA[entity->type].height >> 1;
	box.extent.vz = DIGIMON_DATA[entity->type].radius;
	renderAABB(&box);
}
