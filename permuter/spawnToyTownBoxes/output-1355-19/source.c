
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef struct _physadr
{
  int r[1];
} *physadr;
typedef struct label_t
{
  int val[12];
} label_t;
typedef struct _quad
{
  long val[2];
} quad;
typedef long daddr_t;
typedef char *caddr_t;
typedef long *qaddr_t;
typedef u_long ino_t;
typedef long swblk_t;
typedef unsigned int size_t;
typedef long time_t;
typedef short dev_t;
typedef long off_t;
typedef u_short uid_t;
typedef u_short gid_t;
extern int PadIdentifier;
int CheckCallback(void);
void PadInit(int mode);
int ResetCallback(void);
int RestartCallback(void);
int StopCallback(void);
int VSync(int mode);
int VSyncCallback(void (*f)());
long GetVideoMode(void);
long SetVideoMode(long mode);
u_long PadRead(int id);
void PadStop(void);
typedef struct 
{
  short m[3][3];
  long t[3];
} MATRIX;
typedef struct 
{
  long vx;
  long vy;
  long vz;
  long pad;
} VECTOR;
typedef struct 
{
  short vx;
  short vy;
  short vz;
  short pad;
} SVECTOR;
typedef struct 
{
  u_char r;
  u_char g;
  u_char b;
  u_char cd;
} CVECTOR;
typedef struct 
{
  short vx;
  short vy;
} DVECTOR;
typedef struct 
{
  SVECTOR v;
  VECTOR sxyz;
  DVECTOR sxy;
  CVECTOR rgb;
  short txuv;
  short pad;
  long chx;
  long chy;
} EVECTOR;
typedef struct 
{
  SVECTOR v;
  u_char uv[2];
  u_short pad;
  CVECTOR c;
  DVECTOR sxy;
  u_long sz;
} RVECTOR;
typedef struct 
{
  RVECTOR r01;
  RVECTOR r12;
  RVECTOR r20;
  RVECTOR *r0;
  RVECTOR *r1;
  RVECTOR *r2;
  u_long *rtn;
} CRVECTOR3;
typedef struct 
{
  u_long ndiv;
  u_long pih;
  u_long piv;
  u_short clut;
  u_short tpage;
  CVECTOR rgbc;
  u_long *ot;
  RVECTOR r0;
  RVECTOR r1;
  RVECTOR r2;
  CRVECTOR3 cr[5];
} DIVPOLYGON3;
typedef struct 
{
  RVECTOR r01;
  RVECTOR r02;
  RVECTOR r31;
  RVECTOR r32;
  RVECTOR rc;
  RVECTOR *r0;
  RVECTOR *r1;
  RVECTOR *r2;
  RVECTOR *r3;
  u_long *rtn;
} CRVECTOR4;
typedef struct 
{
  u_long ndiv;
  u_long pih;
  u_long piv;
  u_short clut;
  u_short tpage;
  CVECTOR rgbc;
  u_long *ot;
  RVECTOR r0;
  RVECTOR r1;
  RVECTOR r2;
  RVECTOR r3;
  CRVECTOR4 cr[5];
} DIVPOLYGON4;
typedef struct 
{
  short xy[3];
  short uv[2];
  short rgb[3];
} SPOL;
typedef struct 
{
  short sxy[4][2];
  short sz[4][2];
  short uv[4][2];
  short rgb[4][3];
  short code;
} POL4;
typedef struct 
{
  short sxy[3][2];
  short sz[3][2];
  short uv[3][2];
  short rgb[3][3];
  short code;
} POL3;
typedef struct 
{
  SVECTOR *v;
  SVECTOR *n;
  SVECTOR *u;
  CVECTOR *c;
  u_long len;
} TMESH;
typedef struct 
{
  SVECTOR *v;
  SVECTOR *n;
  SVECTOR *u;
  CVECTOR *c;
  u_long lenv;
  u_long lenh;
} QMESH;
extern void InitGeom();
extern MATRIX *MulMatrix0(MATRIX *m0, MATRIX *m1, MATRIX *m2);
extern MATRIX *MulRotMatrix0(MATRIX *m0, MATRIX *m1);
extern MATRIX *MulMatrix(MATRIX *m0, MATRIX *m1);
extern MATRIX *MulMatrix2(MATRIX *m0, MATRIX *m1);
extern MATRIX *MulRotMatrix(MATRIX *m0);
extern MATRIX *SetMulMatrix(MATRIX *m0, MATRIX *m1);
extern MATRIX *SetMulRotMatrix(MATRIX *m0);
extern VECTOR *ApplyMatrix(MATRIX *m, SVECTOR *v0, VECTOR *v1);
extern VECTOR *ApplyRotMatrix(SVECTOR *v0, VECTOR *v1);
extern VECTOR *ApplyRotMatrixLV(VECTOR *v0, VECTOR *v1);
extern VECTOR *ApplyMatrixLV(MATRIX *m, VECTOR *v0, VECTOR *v1);
extern SVECTOR *ApplyMatrixSV(MATRIX *m, SVECTOR *v0, SVECTOR *v1);
extern VECTOR *ApplyTransposeMatrixLV(MATRIX *m, VECTOR *v0, VECTOR *v1);
extern MATRIX *RotMatrix(SVECTOR *r, MATRIX *m);
extern MATRIX *RotMatrixYXZ(SVECTOR *r, MATRIX *m);
extern MATRIX *RotMatrixZYX(SVECTOR *r, MATRIX *m);
extern MATRIX *RotMatrix_gte(SVECTOR *r, MATRIX *m);
extern MATRIX *RotMatrixYXZ_gte(SVECTOR *r, MATRIX *m);
extern MATRIX *RotMatrixZYX_gte(SVECTOR *r, MATRIX *m);
extern MATRIX *RotMatrixX(long r, MATRIX *m);
extern MATRIX *RotMatrixY(long r, MATRIX *m);
extern MATRIX *RotMatrixZ(long r, MATRIX *m);
extern MATRIX *RotMatrixC(SVECTOR *r, MATRIX *m);
extern MATRIX *TransMatrix(MATRIX *m, VECTOR *v);
extern MATRIX *ScaleMatrix(MATRIX *m, VECTOR *v);
extern MATRIX *ScaleMatrixL(MATRIX *m, VECTOR *v);
extern MATRIX *TransposeMatrix(MATRIX *m0, MATRIX *m1);
extern MATRIX *CompMatrix(MATRIX *m0, MATRIX *m1, MATRIX *m2);
extern MATRIX *CompMatrixLV(MATRIX *m0, MATRIX *m1, MATRIX *m2);
extern void MatrixNormal(MATRIX *m, MATRIX *n);
extern void MatrixNormal_0(MATRIX *m, MATRIX *n);
extern void MatrixNormal_1(MATRIX *m, MATRIX *n);
extern void MatrixNormal_2(MATRIX *m, MATRIX *n);
extern void SetRotMatrix(MATRIX *m);
extern void SetLightMatrix(MATRIX *m);
extern void SetColorMatrix(MATRIX *m);
extern void SetTransMatrix(MATRIX *m);
extern void PushMatrix();
extern void PopMatrix();
extern void ReadRotMatrix(MATRIX *m);
extern void ReadLightMatrix(MATRIX *m);
extern void ReadColorMatrix(MATRIX *m);
extern void SetRGBcd(CVECTOR *v);
extern void SetBackColor(long rbk, long gbk, long bbk);
extern void SetFarColor(long rfc, long gfc, long bfc);
extern void SetGeomOffset(long ofx, long ofy);
extern void SetGeomScreen(long h);
extern void ReadSZfifo3(long *sz0, long *sz1, long *sz2);
extern void ReadSZfifo4(long *szx, long *sz0, long *sz1, long *sz2);
extern void ReadSXSYfifo(long *sxy0, long *sxy1, long *sxy2);
extern void ReadRGBfifo(CVECTOR *v0, CVECTOR *v1, CVECTOR *v2);
extern void ReadGeomOffset(long *ofx, long *ofy);
extern long ReadGeomScreen();
extern void TransRot_32(VECTOR *v0, VECTOR *v1, long *flag);
extern long TransRotPers(SVECTOR *v0, long *sxy, long *p, long *flag);
extern long TransRotPers3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, long *sxy0, long *sxy1, long *sxy2, long *p, long *flag);
extern void pers_map(int abuf, SVECTOR **vertex, int tex[4][2], u_short *dtext);
extern void PhongLine(int istart_x, int iend_x, int p, int q, u_short **pixx, int fs, int ft, int i4, int det);
extern long RotTransPers(SVECTOR *v0, long *sxy, long *p, long *flag);
extern long RotTransPers3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, long *sxy0, long *sxy1, long *sxy2, long *p, long *flag);
extern void RotTrans(SVECTOR *v0, VECTOR *v1, long *flag);
extern void RotTransSV(SVECTOR *v0, SVECTOR *v1, long *flag);
extern void LocalLight(SVECTOR *v0, VECTOR *v1);
extern void LightColor(VECTOR *v0, VECTOR *v1);
extern void DpqColorLight(VECTOR *v0, CVECTOR *v1, long p, CVECTOR *v2);
extern void DpqColor(CVECTOR *v0, long p, CVECTOR *v1);
extern void DpqColor3(CVECTOR *v0, CVECTOR *v1, CVECTOR *v2, long p, CVECTOR *v3, CVECTOR *v4, CVECTOR *v5);
extern void Intpl(VECTOR *v0, long p, CVECTOR *v1);
extern VECTOR *Square12(VECTOR *v0, VECTOR *v1);
extern VECTOR *Square0(VECTOR *v0, VECTOR *v1);
extern VECTOR *SquareSL12(SVECTOR *v0, VECTOR *v1);
extern VECTOR *SquareSL0(SVECTOR *v0, VECTOR *v1);
extern SVECTOR *SquareSS12(SVECTOR *v0, SVECTOR *v1);
extern SVECTOR *SquareSS0(SVECTOR *v0, SVECTOR *v1);
extern void NormalColor(SVECTOR *v0, CVECTOR *v1);
extern void NormalColor3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, CVECTOR *v3, CVECTOR *v4, CVECTOR *v5);
extern void NormalColorDpq(SVECTOR *v0, CVECTOR *v1, long p, CVECTOR *v2);
extern void NormalColorDpq3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, CVECTOR *v3, long p, CVECTOR *v4, CVECTOR *v5, CVECTOR *v6);
extern void NormalColorCol(SVECTOR *v0, CVECTOR *v1, CVECTOR *v2);
extern void NormalColorCol3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, CVECTOR *v3, CVECTOR *v4, CVECTOR *v5, CVECTOR *v6);
extern void ColorDpq(VECTOR *v0, CVECTOR *v1, long p, CVECTOR *v2);
extern void ColorCol(VECTOR *v0, CVECTOR *v1, CVECTOR *v2);
extern long NormalClip(long sxy0, long sxy1, long sxy2);
extern long AverageZ3(long sz0, long sz1, long sz2);
extern long AverageZ4(long sz0, long sz1, long sz2, long sz3);
extern void OuterProduct12(VECTOR *v0, VECTOR *v1, VECTOR *v2);
extern void OuterProduct0(VECTOR *v0, VECTOR *v1, VECTOR *v2);
extern long Lzc(long data);
extern long RotTransPers4(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, long *sxy0, long *sxy1, long *sxy2, long *sxy3, long *p, long *flag);
extern void RotTransPersN(SVECTOR *v0, DVECTOR *v1, u_short *sz, u_short *p, u_short *flag, long n);
extern void RotTransPers3N(SVECTOR *v0, DVECTOR *v1, u_short *sz, u_short *flag, long n);
extern void RotMeshH(short *Yheight, DVECTOR *Vo, u_short *sz, u_short *flag, short Xoffset, short Zoffset, short m, short n, DVECTOR *base);
extern long RotAverage3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, long *sxy0, long *sxy1, long *sxy2, long *p, long *flag);
extern long RotAverage4(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, long *sxy0, long *sxy1, long *sxy2, long *sxy3, long *p, long *flag);
extern long RotNclip3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, long *sxy0, long *sxy1, long *sxy2, long *p, long *otz, long *flag);
extern long RotNclip4(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, long *sxy0, long *sxy1, long *sxy2, long *sxy3, long *p, long *otz, long *flag);
extern long RotAverageNclip3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, long *sxy0, long *sxy1, long *sxy2, long *p, long *otz, long *flag);
extern long RotAverageNclip4(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, long *sxy0, long *sxy1, long *sxy2, long *sxy3, long *p, long *otz, long *flag);
extern long RotColorDpq(SVECTOR *v0, SVECTOR *v1, CVECTOR *v2, long *sxy, CVECTOR *v3, long *flag);
extern long RotColorDpq3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, SVECTOR *v4, SVECTOR *v5, CVECTOR *v6, long *sxy0, long *sxy1, long *sxy2, CVECTOR *v7, CVECTOR *v8, CVECTOR *v9, long *flag);
extern long RotAverageNclipColorDpq3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, SVECTOR *v4, SVECTOR *v5, CVECTOR *v6, long *sxy0, long *sxy1, long *sxy2, CVECTOR *v7, CVECTOR *v8, CVECTOR *v9, long *otz, long *flag);
extern long RotAverageNclipColorCol3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, SVECTOR *v4, SVECTOR *v5, CVECTOR *v6, long *sxy0, long *sxy1, long *sxy2, CVECTOR *v7, CVECTOR *v8, CVECTOR *v9, long *otz, long *flag);
extern long RotColorMatDpq(SVECTOR *v0, SVECTOR *v1, CVECTOR *v2, long *sxy, CVECTOR *v3, long matc, long flag);
extern void ColorMatDpq(SVECTOR *v0, CVECTOR *v1, long p, CVECTOR *v2, long matc);
extern void ColorMatCol(SVECTOR *v0, CVECTOR *v1, CVECTOR *v2, long matc);
extern void LoadAverage12(VECTOR *v0, VECTOR *v1, long p0, long p1, VECTOR *v2);
extern void LoadAverageShort12(SVECTOR *v0, SVECTOR *v1, long p0, long p1, SVECTOR *v2);
extern void LoadAverage0(VECTOR *v0, VECTOR *v1, long p0, long p1, VECTOR *v2);
extern void LoadAverageShort0(SVECTOR *v0, SVECTOR *v1, long p0, long p1, SVECTOR *v2);
extern void LoadAverageByte(u_char *v0, u_char *v1, long p0, long p1, u_char *v2);
extern void LoadAverageCol(u_char *v0, u_char *v1, long p0, long p1, u_char *v2);
extern long VectorNormal(VECTOR *v0, VECTOR *v1);
extern long VectorNormalS(VECTOR *v0, SVECTOR *v1);
extern long VectorNormalSS(SVECTOR *v0, SVECTOR *v1);
extern long SquareRoot0(long a);
extern long SquareRoot12(long a);
extern void InvSquareRoot(long a, long *b, long *c);
extern void gteMIMefunc(SVECTOR *otp, SVECTOR *dfp, long n, long p);
extern void SetFogFar(long a, long h);
extern void SetFogNear(long a, long h);
extern void SetFogNearFar(long a, long b, long h);
extern void SubPol4(POL4 *p, SPOL *sp, int ndiv);
extern void SubPol3(POL3 *p, SPOL *sp, int ndiv);
extern int rcos(int a);
extern int rsin(int a);
extern int ccos(int a);
extern int csin(int a);
extern int cln(int a);
extern int csqrt(int a);
extern int catan(int a);
extern long ratan2(long y, long x);
extern void RotPMD_F3(long *pa, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_G3(long *pa, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_FT3(long *pa, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_GT3(long *pa, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_F4(long *pa, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_G4(long *pa, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_FT4(long *pa, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_GT4(long *pa, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_SV_F3(long *pa, long *va, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_SV_G3(long *pa, long *va, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_SV_FT3(long *pa, long *va, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_SV_GT3(long *pa, long *va, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_SV_F4(long *pa, long *va, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_SV_G4(long *pa, long *va, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_SV_FT4(long *pa, long *va, u_long *ot, int otlen, int id, int backc);
extern void RotPMD_SV_GT4(long *pa, long *va, u_long *ot, int otlen, int id, int backc);
extern void InitClip(EVECTOR *evbfad, long hw, long vw, long h, long near, long far);
extern long Clip3F(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, EVECTOR **evmx);
extern long Clip3FP(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, EVECTOR **evmx);
extern long Clip4F(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, EVECTOR **evmx);
extern long Clip4FP(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, EVECTOR **evmx);
extern long Clip3FT(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, short *uv0, short *uv1, short *uv2, EVECTOR **evmx);
extern long Clip3FTP(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, short *uv0, short *uv1, short *uv2, EVECTOR **evmx);
extern long Clip4FT(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, short *uv0, short *uv1, short *uv2, short *uv3, EVECTOR **evmx);
extern long Clip4FTP(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, short *uv0, short *uv1, short *uv2, short *uv3, EVECTOR **evmx);
extern long Clip3G(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, CVECTOR *rgb0, CVECTOR *rgb1, CVECTOR *rgb2, EVECTOR **evmx);
extern long Clip3GP(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, CVECTOR *rgb0, CVECTOR *rgb1, CVECTOR *rgb2, EVECTOR **evmx);
extern long Clip4G(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, CVECTOR *rgb0, CVECTOR *rgb1, CVECTOR *rgb2, CVECTOR *rgb3, EVECTOR **evmx);
extern long Clip4GP(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, CVECTOR *rgb0, CVECTOR *rgb1, CVECTOR *rgb2, CVECTOR *rgb3, EVECTOR **evmx);
extern long Clip3GT(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, short *uv0, short *uv1, short *uv2, CVECTOR *rgb0, CVECTOR *rgb1, CVECTOR *rgb2, EVECTOR **evmx);
extern long Clip3GTP(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, short *uv0, short *uv1, short *uv2, CVECTOR *rgb0, CVECTOR *rgb1, CVECTOR *rgb2, EVECTOR **evmx);
extern long Clip4GT(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, short *uv0, short *uv1, short *uv2, short *uv3, CVECTOR *rgb0, CVECTOR *rgb1, CVECTOR *rgb2, CVECTOR *rgb3, EVECTOR **evmx);
extern long Clip4GTP(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, short *uv0, short *uv1, short *uv2, short *uv3, CVECTOR *rgb0, CVECTOR *rgb1, CVECTOR *rgb2, CVECTOR *rgb3, EVECTOR **evmx);
extern void RotTransPers_nom(SVECTOR *v0);
extern void RotTransPers3_nom(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
extern void RotTransPers4_nom(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3);
extern void RotTrans_nom(SVECTOR *v0);
extern void RotAverage3_nom(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
extern void RotNclip3_nom(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
extern void RotAverageNclip3_nom(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
extern void RotAverageNclipColorDpq3_nom(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, SVECTOR *v4, SVECTOR *v5, CVECTOR *v6);
extern void RotAverageNclipColorCol3_nom(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, SVECTOR *v4, SVECTOR *v5, CVECTOR *v6);
extern void RotColorDpq_nom(SVECTOR *v0, SVECTOR *v1, CVECTOR *v2);
extern long RotColorDpq3_nom(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, SVECTOR *v4, SVECTOR *v5, CVECTOR *v6);
extern void NormalColor_nom(SVECTOR *v0);
extern void NormalColor3_nom(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2);
extern void NormalColorDpq_nom(SVECTOR *v0, CVECTOR *v1, long p);
extern void NormalColorDpq3_nom(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, CVECTOR *v3, long p);
extern void NormalColorCol_nom(SVECTOR *v0, CVECTOR *v1);
extern void NormalColorCol3_nom(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, CVECTOR *v3);
extern void RotSMD_F3(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_G3(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_FT3(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_GT3(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_F4(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_G4(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_FT4(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_GT4(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_SV_F3(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_SV_G3(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_SV_FT3(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_SV_GT3(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_SV_F4(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_SV_G4(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_SV_FT4(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotSMD_SV_GT4(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_F3(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_G3(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_FT3(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_GT3(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_F4(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_G4(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_FT4(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_GT4(long *pa, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_SV_F3(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_SV_G3(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_SV_FT3(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_SV_GT3(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_SV_F4(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_SV_G4(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_SV_FT4(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern void RotRMD_SV_GT4(long *pa, long *va, u_long *ot, int otlen, int id, int sclip, int hclip, int vclip, int nclipmode);
extern long p2otz(long p, long projection);
extern long otz2p(long otz, long projection);
extern int (*GPU_printf)();
typedef struct 
{
  short x;
  short y;
  short w;
  short h;
} RECT;
typedef struct 
{
  int x;
  int y;
  int w;
  int h;
} RECT32;
typedef struct 
{
  u_long tag;
  u_long code[15];
} DR_ENV;
typedef struct 
{
  RECT clip;
  short ofs[2];
  RECT tw;
  u_short tpage;
  u_char dtd;
  u_char dfe;
  u_char isbg;
  u_char r0;
  u_char g0;
  u_char b0;
  DR_ENV dr_env;
} DRAWENV;
typedef struct 
{
  RECT disp;
  RECT screen;
  u_char isinter;
  u_char isrgb24;
  u_char pad0;
  u_char pad1;
} DISPENV;
typedef struct 
{
  unsigned addr : 24;
  unsigned len : 8;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
} P_TAG;
typedef struct 
{
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
} P_CODE;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  short x1;
  short y1;
  short x2;
  short y2;
} POLY_F3;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  short x1;
  short y1;
  short x2;
  short y2;
  short x3;
  short y3;
} POLY_F4;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  u_char u0;
  u_char v0;
  u_short clut;
  short x1;
  short y1;
  u_char u1;
  u_char v1;
  u_short tpage;
  short x2;
  short y2;
  u_char u2;
  u_char v2;
  u_short pad1;
} POLY_FT3;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  u_char u0;
  u_char v0;
  u_short clut;
  short x1;
  short y1;
  u_char u1;
  u_char v1;
  u_short tpage;
  short x2;
  short y2;
  u_char u2;
  u_char v2;
  u_short pad1;
  short x3;
  short y3;
  u_char u3;
  u_char v3;
  u_short pad2;
} POLY_FT4;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char pad1;
  short x1;
  short y1;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char pad2;
  short x2;
  short y2;
} POLY_G3;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char pad1;
  short x1;
  short y1;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char pad2;
  short x2;
  short y2;
  u_char r3;
  u_char g3;
  u_char b3;
  u_char pad3;
  short x3;
  short y3;
} POLY_G4;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  u_char u0;
  u_char v0;
  u_short clut;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char p1;
  short x1;
  short y1;
  u_char u1;
  u_char v1;
  u_short tpage;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char p2;
  short x2;
  short y2;
  u_char u2;
  u_char v2;
  u_short pad2;
} POLY_GT3;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  u_char u0;
  u_char v0;
  u_short clut;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char p1;
  short x1;
  short y1;
  u_char u1;
  u_char v1;
  u_short tpage;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char p2;
  short x2;
  short y2;
  u_char u2;
  u_char v2;
  u_short pad2;
  u_char r3;
  u_char g3;
  u_char b3;
  u_char p3;
  short x3;
  short y3;
  u_char u3;
  u_char v3;
  u_short pad3;
} POLY_GT4;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  short x1;
  short y1;
} LINE_F2;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char p1;
  short x1;
  short y1;
} LINE_G2;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  short x1;
  short y1;
  short x2;
  short y2;
  u_long pad;
} LINE_F3;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char p1;
  short x1;
  short y1;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char p2;
  short x2;
  short y2;
  u_long pad;
} LINE_G3;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  short x1;
  short y1;
  short x2;
  short y2;
  short x3;
  short y3;
  u_long pad;
} LINE_F4;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char p1;
  short x1;
  short y1;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char p2;
  short x2;
  short y2;
  u_char r3;
  u_char g3;
  u_char b3;
  u_char p3;
  short x3;
  short y3;
  u_long pad;
} LINE_G4;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  u_char u0;
  u_char v0;
  u_short clut;
  short w;
  short h;
} SPRT;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  u_char u0;
  u_char v0;
  u_short clut;
} SPRT_16;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  u_char u0;
  u_char v0;
  u_short clut;
} SPRT_8;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
  short w;
  short h;
} TILE;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
} TILE_16;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
} TILE_8;
typedef struct 
{
  u_long tag;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  short x0;
  short y0;
} TILE_1;
typedef struct 
{
  u_long tag;
  u_long code[2];
} DR_MODE;
typedef struct 
{
  u_long tag;
  u_long code[2];
} DR_TWIN;
typedef struct 
{
  u_long tag;
  u_long code[2];
} DR_AREA;
typedef struct 
{
  u_long tag;
  u_long code[2];
} DR_OFFSET;
typedef struct 
{
  u_long tag;
  u_long code[5];
} DR_MOVE;
typedef struct 
{
  u_long tag;
  u_long code[3];
  u_long p[13];
} DR_LOAD;
typedef struct 
{
  u_long tag;
  u_long code[1];
} DR_TPAGE;
typedef struct 
{
  u_long tag;
  u_long code[2];
} DR_STP;
typedef struct 
{
  u_long id;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char p0;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char p1;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char p2;
  u_char r3;
  u_char g3;
  u_char b3;
  u_char p3;
  u_short tpage;
  u_short clut;
  u_char u0;
  u_char v0;
  u_char u1;
  u_char v1;
  u_char u2;
  u_char v2;
  u_char u3;
  u_char v3;
  SVECTOR x0;
  SVECTOR x1;
  SVECTOR x2;
  SVECTOR x3;
  SVECTOR n0;
  SVECTOR n1;
  SVECTOR n2;
  SVECTOR n3;
  SVECTOR *v_ofs;
  SVECTOR *n_ofs;
  u_short vert0;
  u_short vert1;
  u_short vert2;
  u_short vert3;
  u_short norm0;
  u_short norm1;
  u_short norm2;
  u_short norm3;
} TMD_PRIM;
typedef struct 
{
  u_long mode;
  RECT *crect;
  u_long *caddr;
  RECT *prect;
  u_long *paddr;
} TIM_IMAGE;
extern int FntPrint();
extern int KanjiFntPrint();
extern DISPENV *GetDispEnv(DISPENV *env);
extern DISPENV *PutDispEnv(DISPENV *env);
extern DISPENV *SetDefDispEnv(DISPENV *env, int x, int y, int w, int h);
extern DRAWENV *GetDrawEnv(DRAWENV *env);
extern DRAWENV *PutDrawEnv(DRAWENV *env);
extern DRAWENV *SetDefDrawEnv(DRAWENV *env, int x, int y, int w, int h);
extern TIM_IMAGE *ReadTIM(TIM_IMAGE *timimg);
extern TMD_PRIM *ReadTMD(TMD_PRIM *tmdprim);
extern int CheckPrim(char *s, u_long *p);
extern int ClearImage(RECT *rect, u_char r, u_char g, u_char b);
extern int ClearImage2(RECT *rect, u_char r, u_char g, u_char b);
extern int DrawSync(int mode);
extern int FntOpen(int x, int y, int w, int h, int isbg, int n);
extern int GetGraphDebug(void);
extern int GetTimSize(u_char *sjis);
extern int IsEndPrim(void *p);
extern int KanjiFntOpen(int x, int y, int w, int h, int dx, int dy, int cx, int cy, int isbg, int n);
extern void KanjiFntClose(void);
extern int Krom2Tim(u_char *sjis, u_long *taddr, int dx, int dy, int cdx, int cdy, u_int fg, u_int bg);
extern int LoadImage(RECT *rect, u_long *p);
extern int MargePrim(void *p0, void *p1);
extern int MoveImage(RECT *rect, int x, int y);
extern int OpenTIM(u_long *addr);
extern int OpenTMD(u_long *tmd, int obj_no);
extern int ResetGraph(int mode);
extern int SetGraphDebug(int level);
extern int StoreImage(RECT *rect, u_long *p);
extern u_long *ClearOTag(u_long *ot, int n);
extern u_long *ClearOTagR(u_long *ot, int n);
extern u_long *FntFlush(int id);
extern u_long *KanjiFntFlush(int id);
extern u_long DrawSyncCallback(void (*func)());
extern u_short GetClut(int x, int y);
extern u_short GetTPage(int tp, int abr, int x, int y);
extern u_short LoadClut(u_long *clut, int x, int y);
extern u_short LoadClut2(u_long *clut, int x, int y);
extern u_short LoadTPage(u_long *pix, int tp, int abr, int x, int y, int w, int h);
extern void *NextPrim(void *p);
extern void AddPrim(void *ot, void *p);
extern void AddPrims(void *ot, void *p0, void *p1);
extern void CatPrim(void *p0, void *p1);
extern void DrawOTag(u_long *p);
extern void DrawOTagIO(u_long *p);
extern void DrawOTagEnv(u_long *p, DRAWENV *env);
extern void DrawPrim(void *p);
extern void DumpClut(u_short clut);
extern void DumpDispEnv(DISPENV *env);
extern void DumpDrawEnv(DRAWENV *env);
extern void DumpOTag(u_long *p);
extern void DumpTPage(u_short tpage);
extern void FntLoad(int tx, int ty);
extern void SetDispMask(int mask);
extern void SetDrawArea(DR_AREA *p, RECT *r);
extern void SetDrawEnv(DR_ENV *dr_env, DRAWENV *env);
extern void SetDrawLoad(DR_LOAD *p, RECT *rect);
extern void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, RECT *tw);
extern void SetDrawTPage(DR_TPAGE *p, int dfe, int dtd, int tpage);
extern void SetDrawMove(DR_MOVE *p, RECT *rect, int x, int y);
extern void SetDrawOffset(DR_OFFSET *p, u_short *ofs);
extern void SetDrawStp(DR_STP *p, int pbw);
extern void SetDumpFnt(int id);
extern void SetLineF2(LINE_F2 *p);
extern void SetLineF3(LINE_F3 *p);
extern void SetLineF4(LINE_F4 *p);
extern void SetLineG2(LINE_G2 *p);
extern void SetLineG3(LINE_G3 *p);
extern void SetLineG4(LINE_G4 *p);
extern void SetPolyF3(POLY_F3 *p);
extern void SetPolyF4(POLY_F4 *p);
extern void SetPolyFT3(POLY_FT3 *p);
extern void SetPolyFT4(POLY_FT4 *p);
extern void SetPolyG3(POLY_G3 *p);
extern void SetPolyG4(POLY_G4 *p);
extern void SetPolyGT3(POLY_GT3 *p);
extern void SetPolyGT4(POLY_GT4 *p);
extern void SetSemiTrans(void *p, int abe);
extern void SetShadeTex(void *p, int tge);
extern void SetSprt(SPRT *p);
extern void SetSprt16(SPRT_16 *p);
extern void SetSprt8(SPRT_8 *p);
extern void SetTexWindow(DR_TWIN *p, RECT *tw);
extern void SetTile(TILE *p);
extern void SetTile1(TILE_1 *p);
extern void SetTile16(TILE_16 *p);
extern void SetTile8(TILE_8 *p);
extern void TermPrim(void *p);
extern u_long *BreakDraw(void);
extern void ContinueDraw(u_long *insaddr, u_long *contaddr);
extern int IsIdleGPU(int max_count);
extern int GetODE(void);
extern int LoadImage2(RECT *rect, u_long *p);
extern int StoreImage2(RECT *rect, u_long *p);
extern int MoveImage2(RECT *rect, int x, int y);
extern int DrawOTag2(u_long *p);
typedef unsigned char PACKET;
typedef struct 
{
  VECTOR scale;
  SVECTOR rotate;
  VECTOR trans;
} GsCOORD2PARAM;
typedef struct _GsCOORDINATE2
{
  unsigned long flg;
  MATRIX coord;
  MATRIX workm;
  GsCOORD2PARAM *param;
  struct _GsCOORDINATE2 *super;
  struct _GsCOORDINATE2 *sub;
} GsCOORDINATE2;
typedef struct 
{
  MATRIX view;
  GsCOORDINATE2 *super;
} GsVIEW2;
typedef struct 
{
  long vpx;
  long vpy;
  long vpz;
  long vrx;
  long vry;
  long vrz;
  long rz;
  GsCOORDINATE2 *super;
} GsRVIEW2;
typedef struct 
{
  int vx;
  int vy;
  int vz;
  unsigned char r;
  unsigned char g;
  unsigned char b;
} GsF_LIGHT;
typedef struct 
{
  unsigned p : 24;
  unsigned char num : 8;
} GsOT_TAG;
typedef struct 
{
  unsigned long length;
  GsOT_TAG *org;
  unsigned long offset;
  unsigned long point;
  GsOT_TAG *tag;
} GsOT;
typedef struct 
{
  unsigned long attribute;
  GsCOORDINATE2 *coord2;
  unsigned long *tmd;
  unsigned long id;
} GsDOBJ2;
typedef struct 
{
  unsigned long attribute;
  GsCOORDINATE2 *coord2;
  unsigned long *pmd;
  unsigned long *base;
  unsigned long *sv;
  unsigned long id;
} GsDOBJ3;
typedef struct 
{
  unsigned long attribute;
  GsCOORDINATE2 *coord2;
  unsigned long *tmd;
  unsigned long id;
} GsDOBJ4;
typedef struct 
{
  unsigned long attribute;
  GsCOORDINATE2 *coord2;
  unsigned long *tmd;
  unsigned long *packet;
  unsigned long id;
} GsDOBJ5;
typedef struct 
{
  unsigned long attribute;
  short x;
  short y;
  unsigned short w;
  unsigned short h;
  unsigned short tpage;
  unsigned char u;
  unsigned char v;
  short cx;
  short cy;
  unsigned char r;
  unsigned char g;
  unsigned char b;
  short mx;
  short my;
  short scalex;
  short scaley;
  long rotate;
} GsSPRITE;
typedef struct 
{
  unsigned long attribute;
  short x;
  short y;
  DR_MODE mode[2];
  SPRT packet[2];
} GsSPARRAY;
typedef struct 
{
  unsigned char u;
  unsigned char v;
  unsigned short cba;
  unsigned short flag;
  unsigned short tpage;
} GsCELL;
typedef struct 
{
  unsigned char cellw;
  unsigned char cellh;
  unsigned short ncellw;
  unsigned short ncellh;
  GsCELL *base;
  unsigned short *index;
} GsMAP;
typedef struct 
{
  unsigned long attribute;
  short x;
  short y;
  short w;
  short h;
  short scrollx;
  short scrolly;
  unsigned char r;
  unsigned char g;
  unsigned char b;
  GsMAP *map;
  short mx;
  short my;
  short scalex;
  short scaley;
  long rotate;
} GsBG;
typedef struct 
{
  unsigned long attribute;
  short x0;
  short y0;
  short x1;
  short y1;
  unsigned char r;
  unsigned char g;
  unsigned char b;
} GsLINE;
typedef struct 
{
  unsigned long attribute;
  short x0;
  short y0;
  short x1;
  short y1;
  unsigned char r0;
  unsigned char g0;
  unsigned char b0;
  unsigned char r1;
  unsigned char g1;
  unsigned char b1;
} GsGLINE;
typedef struct 
{
  unsigned long attribute;
  short x;
  short y;
  unsigned short w;
  unsigned short h;
  unsigned char r;
  unsigned char g;
  unsigned char b;
} GsBOXF;
typedef struct 
{
  short dqa;
  long dqb;
  unsigned char rfc;
  unsigned char gfc;
  unsigned char bfc;
} GsFOGPARAM;
typedef struct 
{
  unsigned long pmode;
  short px;
  short py;
  unsigned short pw;
  unsigned short ph;
  unsigned long *pixel;
  short cx;
  short cy;
  unsigned short cw;
  unsigned short ch;
  unsigned long *clut;
} GsIMAGE;
typedef struct 
{
  short offx;
  short offy;
} _GsPOSITION;
typedef struct 
{
  GsDOBJ2 *top;
  int nobj;
  int maxobj;
} GsOBJTABLE2;
typedef struct 
{
  PACKET *(*f3[2][3])();
  PACKET *(*nf3[2])();
  PACKET *(*g3[2][3])();
  PACKET *(*ng3[2])();
  PACKET *(*tf3[2][3])();
  PACKET *(*ntf3[2])();
  PACKET *(*tg3[2][3])();
  PACKET *(*ntg3[2])();
  PACKET *(*f4[2][3])();
  PACKET *(*nf4[2])();
  PACKET *(*g4[2][3])();
  PACKET *(*ng4[2])();
  PACKET *(*tf4[2][3])();
  PACKET *(*ntf4[2])();
  PACKET *(*tg4[2][3])();
  PACKET *(*ntg4[2])();
  PACKET *(*f3g[3])();
  PACKET *(*g3g[3])();
  PACKET *(*f4g[3])();
  PACKET *(*g4g[3])();
} _GsFCALL;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  u_short n0;
  u_short v0;
  u_short v1;
  u_short v2;
} TMD_P_F3;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  u_short n0;
  u_short v0;
  u_short n1;
  u_short v1;
  u_short n2;
  u_short v2;
} TMD_P_G3;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char dummy1;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char dummy2;
  u_short n0;
  u_short v0;
  u_short v1;
  u_short v2;
} TMD_P_F3G;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char dummy1;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char dummy2;
  u_short n0;
  u_short v0;
  u_short n1;
  u_short v1;
  u_short n2;
  u_short v2;
} TMD_P_G3G;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  u_short v0;
  u_short v1;
  u_short v2;
  u_short p;
} TMD_P_NF3;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char p1;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char p2;
  u_short v0;
  u_short v1;
  u_short v2;
  u_short p;
} TMD_P_NG3;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  u_short n0;
  u_short v0;
  u_short v1;
  u_short v2;
  u_short v3;
  u_short p;
} TMD_P_F4;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  u_short n0;
  u_short v0;
  u_short n1;
  u_short v1;
  u_short n2;
  u_short v2;
  u_short n3;
  u_short v3;
} TMD_P_G4;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char dummy1;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char dummy2;
  u_char r3;
  u_char g3;
  u_char b3;
  u_char dummy3;
  u_short n0;
  u_short v0;
  u_short v1;
  u_short v2;
  u_short v3;
  u_short dummy4;
} TMD_P_F4G;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char dummy1;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char dummy2;
  u_char r3;
  u_char g3;
  u_char b3;
  u_char dummy3;
  u_short n0;
  u_short v0;
  u_short n1;
  u_short v1;
  u_short n2;
  u_short v2;
  u_short n3;
  u_short v3;
} TMD_P_G4G;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  u_short v0;
  u_short v1;
  u_short v2;
  u_short v3;
} TMD_P_NF4;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char code;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char p1;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char p2;
  u_char r3;
  u_char g3;
  u_char b3;
  u_char p3;
  u_short v0;
  u_short v1;
  u_short v2;
  u_short v3;
} TMD_P_NG4;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char tu0;
  u_char tv0;
  u_short clut;
  u_char tu1;
  u_char tv1;
  u_short tpage;
  u_char tu2;
  u_char tv2;
  u_short p;
  u_short n0;
  u_short v0;
  u_short v1;
  u_short v2;
} TMD_P_TF3;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char tu0;
  u_char tv0;
  u_short clut;
  u_char tu1;
  u_char tv1;
  u_short tpage;
  u_char tu2;
  u_char tv2;
  u_short p;
  u_short n0;
  u_short v0;
  u_short n1;
  u_short v1;
  u_short n2;
  u_short v2;
} TMD_P_TG3;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char tu0;
  u_char tv0;
  u_short clut;
  u_char tu1;
  u_char tv1;
  u_short tpage;
  u_char tu2;
  u_char tv2;
  u_short p0;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char p1;
  u_short v0;
  u_short v1;
  u_short v2;
  u_short p2;
} TMD_P_TNF3;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char tu0;
  u_char tv0;
  u_short clut;
  u_char tu1;
  u_char tv1;
  u_short tpage;
  u_char tu2;
  u_char tv2;
  u_short p0;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char p1;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char p2;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char p3;
  u_short v0;
  u_short v1;
  u_short v2;
  u_short p4;
} TMD_P_TNG3;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char tu0;
  u_char tv0;
  u_short clut;
  u_char tu1;
  u_char tv1;
  u_short tpage;
  u_char tu2;
  u_char tv2;
  u_short p0;
  u_char tu3;
  u_char tv3;
  u_short p1;
  u_short n0;
  u_short v0;
  u_short v1;
  u_short v2;
  u_short v3;
  u_short p2;
} TMD_P_TF4;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char tu0;
  u_char tv0;
  u_short clut;
  u_char tu1;
  u_char tv1;
  u_short tpage;
  u_char tu2;
  u_char tv2;
  u_short p0;
  u_char tu3;
  u_char tv3;
  u_short p1;
  u_short n0;
  u_short v0;
  u_short n1;
  u_short v1;
  u_short n2;
  u_short v2;
  u_short n3;
  u_short v3;
} TMD_P_TG4;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char tu0;
  u_char tv0;
  u_short clut;
  u_char tu1;
  u_char tv1;
  u_short tpage;
  u_char tu2;
  u_char tv2;
  u_short p0;
  u_char tu3;
  u_char tv3;
  u_short p1;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char p2;
  u_short v0;
  u_short v1;
  u_short v2;
  u_short v3;
} TMD_P_TNF4;
typedef struct 
{
  u_char out;
  u_char in;
  u_char dummy;
  u_char cd;
  u_char tu0;
  u_char tv0;
  u_short clut;
  u_char tu1;
  u_char tv1;
  u_short tpage;
  u_char tu2;
  u_char tv2;
  u_short p0;
  u_char tu3;
  u_char tv3;
  u_short p1;
  u_char r0;
  u_char g0;
  u_char b0;
  u_char p2;
  u_char r1;
  u_char g1;
  u_char b1;
  u_char p3;
  u_char r2;
  u_char g2;
  u_char b2;
  u_char p4;
  u_char r3;
  u_char g3;
  u_char b3;
  u_char p5;
  u_short v0;
  u_short v1;
  u_short v2;
  u_short v3;
} TMD_P_TNG4;
struct TMD_STRUCT
{
  u_long *vertop;
  u_long vern;
  u_long *nortop;
  u_long norn;
  u_long *primtop;
  u_long primn;
  u_long scale;
};
typedef struct 
{
  short vx;
  short vy;
  short vz;
  u_char tu;
  u_char tv;
} VERT;
typedef struct 
{
  short vx;
  short vy;
  short vz;
  u_char tu;
  u_char tv;
  CVECTOR col;
} VERTC;
typedef struct 
{
  u_long limit;
  long hwd;
  long vwd;
  int shift;
  u_long *org;
  u_long *pk;
  long otz;
  long adivz;
  short adivw;
  short adivh;
  long flg0;
  long flg;
  short minx;
  short miny;
  short maxx;
  short maxy;
  short hwd0;
  short vwd0;
  u_long *tag;
  POLY_FT4 si;
} GsADIV_FT4;
typedef struct 
{
  VERT vt[4];
} GsADIV_P_FT4;
typedef struct 
{
  u_long limit;
  long hwd;
  long vwd;
  int shift;
  u_long *org;
  u_long *pk;
  long otz;
  long adivz;
  short adivw;
  short adivh;
  long flg0;
  long flg;
  short minx;
  short miny;
  short maxx;
  short maxy;
  short hwd0;
  short vwd0;
  u_long *tag;
  POLY_GT4 si;
} GsADIV_GT4;
typedef struct 
{
  VERTC vt[4];
} GsADIV_P_GT4;
typedef struct 
{
  u_long limit;
  long hwd;
  long vwd;
  int shift;
  u_long *org;
  u_long *pk;
  long otz;
  long adivz;
  short adivw;
  short adivh;
  long flg0;
  long flg;
  short minx;
  short miny;
  short maxx;
  short maxy;
  short hwd0;
  short vwd0;
  u_long *tag;
  POLY_G4 si;
} GsADIV_G4;
typedef struct 
{
  VERTC vt[4];
} GsADIV_P_G4;
typedef struct 
{
  u_long limit;
  long hwd;
  long vwd;
  int shift;
  u_long *org;
  u_long *pk;
  long otz;
  long adivz;
  short adivw;
  short adivh;
  long flg0;
  long flg;
  short minx;
  short miny;
  short maxx;
  short maxy;
  short hwd0;
  short vwd0;
  u_long *tag;
  POLY_F4 si;
} GsADIV_F4;
typedef struct 
{
  VERT vt[4];
} GsADIV_P_F4;
typedef struct 
{
  u_long limit;
  long hwd;
  long vwd;
  int shift;
  u_long *org;
  u_long *pk;
  long otz;
  long adivz;
  short adivw;
  short adivh;
  long flg;
  short minx;
  short miny;
  short maxx;
  short maxy;
  short hwd0;
  short vwd0;
  u_long *tag;
  POLY_FT3 si;
} GsADIV_FT3;
typedef struct 
{
  VERT vt[3];
} GsADIV_P_FT3;
typedef struct 
{
  u_long limit;
  long hwd;
  long vwd;
  int shift;
  u_long *org;
  u_long *pk;
  long otz;
  long adivz;
  short adivw;
  short adivh;
  long flg;
  short minx;
  short miny;
  short maxx;
  short maxy;
  short hwd0;
  short vwd0;
  u_long *tag;
  POLY_GT3 si;
} GsADIV_GT3;
typedef struct 
{
  VERTC vt[3];
} GsADIV_P_GT3;
typedef struct 
{
  u_long limit;
  long hwd;
  long vwd;
  int shift;
  u_long *org;
  u_long *pk;
  long otz;
  long adivz;
  short adivw;
  short adivh;
  long flg;
  short minx;
  short miny;
  short maxx;
  short maxy;
  short hwd0;
  short vwd0;
  u_long *tag;
  POLY_G3 si;
} GsADIV_G3;
typedef struct 
{
  VERTC vt[3];
} GsADIV_P_G3;
typedef struct 
{
  u_long limit;
  long hwd;
  long vwd;
  int shift;
  u_long *org;
  u_long *pk;
  long otz;
  long adivz;
  short adivw;
  short adivh;
  long flg;
  short minx;
  short miny;
  short maxx;
  short maxy;
  short hwd0;
  short vwd0;
  u_long *tag;
  POLY_F3 si;
} GsADIV_F3;
typedef struct 
{
  VERT vt[3];
} GsADIV_P_F3;
typedef struct _GsCOORDUNIT
{
  unsigned long flg;
  MATRIX matrix;
  MATRIX workm;
  SVECTOR rot;
  struct _GsCOORDUNIT *super;
} GsCOORDUNIT;
typedef struct 
{
  MATRIX view;
  GsCOORDUNIT *super;
} GsVIEWUNIT;
typedef struct 
{
  long vpx;
  long vpy;
  long vpz;
  long vrx;
  long vry;
  long vrz;
  long rz;
  GsCOORDUNIT *super;
} GsRVIEWUNIT;
typedef struct 
{
  GsCOORDUNIT *coord;
  unsigned long *primtop;
} GsUNIT;
typedef struct 
{
  unsigned long type;
  unsigned long *ptr;
} GsTYPEUNIT;
typedef struct 
{
  unsigned long *primp;
  GsOT *tagp;
  int shift;
  int offset;
  PACKET *out_packetp;
} GsARGUNIT;
typedef struct 
{
  DVECTOR vec;
  short otz;
  short p;
} GsWORKUNIT;
typedef struct 
{
  unsigned long *primp;
  GsOT *tagp;
  int shift;
  int offset;
  PACKET *out_packetp;
  unsigned long *primtop;
  SVECTOR *vertop;
  SVECTOR *nortop;
} GsARGUNIT_NORMAL;
typedef struct 
{
  unsigned long *primp;
  GsOT *tagp;
  int shift;
  int offset;
  PACKET *out_packetp;
  unsigned long *primtop;
  SVECTOR *vertop;
  GsWORKUNIT *vertop2;
  SVECTOR *nortop;
  SVECTOR *nortop2;
} GsARGUNIT_SHARED;
typedef struct 
{
  unsigned long *primp;
  GsOT *tagp;
  int shift;
  int offset;
  PACKET *out_packetp;
  unsigned long *imagetop;
  unsigned long *cluttop;
} GsARGUNIT_IMAGE;
typedef struct 
{
  unsigned long *primp;
  GsOT *tagp;
  int shift;
  int offset;
  PACKET *out_packetp;
  unsigned long *polytop;
  unsigned long *boxtop;
  unsigned long *pointtop;
  SVECTOR *nortop;
} GsARGUNIT_GND;
typedef struct 
{
  unsigned long *primp;
  GsOT *tagp;
  int shift;
  int offset;
  PACKET *out_packetp;
  unsigned long *polytop;
  unsigned long *boxtop;
  unsigned long *pointtop;
  SVECTOR *nortop;
  unsigned long *uvtop;
} GsARGUNIT_GNDT;
typedef struct 
{
  unsigned long *primp;
  GsOT *tagp;
  int shift;
  int offset;
  PACKET *out_packetp;
  u_long *coord_sect;
  long *mimepr;
  u_long mimenum;
  u_short mimeid;
  u_short reserved;
  u_long *mime_diff_sect;
} GsARGUNIT_JntMIMe;
typedef struct 
{
  unsigned long *primp;
  GsOT *tagp;
  int shift;
  int offset;
  PACKET *out_packetp;
  u_long *coord_sect;
  u_short mimeid;
  u_short reserved;
  u_long *mime_diff_sect;
} GsARGUNIT_RstJntMIMe;
typedef struct 
{
  unsigned long *primp;
  GsOT *tagp;
  int shift;
  int offset;
  PACKET *out_packetp;
  long *mimepr;
  u_long mimenum;
  u_short mimeid;
  u_short reserved;
  u_long *mime_diff_sect;
  SVECTOR *orgs_vn_sect;
  SVECTOR *vert_sect;
  SVECTOR *norm_sect;
} GsARGUNIT_VNMIMe;
typedef struct 
{
  unsigned long *primp;
  GsOT *tagp;
  int shift;
  int offset;
  PACKET *out_packetp;
  u_short mimeid;
  u_short reserved;
  u_long *mime_diff_sect;
  SVECTOR *orgs_vn_sect;
  SVECTOR *vert_sect;
  SVECTOR *norm_sect;
} GsARGUNIT_RstVNMIMe;
typedef struct 
{
  unsigned long *primp;
  GsOT *tagp;
  int shift;
  int offset;
  PACKET *out_packetp;
  long header_size;
  unsigned long *htop;
  unsigned long *ctop;
  unsigned long *ptop;
} GsARGUNIT_ANIM;
typedef struct 
{
  short idx;
  u_char sid;
  u_char pad;
} GsSEH;
typedef struct 
{
  u_long rewrite_idx;
  u_short size;
  u_short num;
  u_short ii;
  u_short aframe;
  u_char sid;
  signed char speed;
  u_short srcii;
  short rframe;
  u_short tframe;
  u_short ci;
  u_short ti;
  u_short start;
  u_char start_sid;
  u_char traveling;
} GsSEQ;
void GsInitGraph(unsigned short x, unsigned short y, unsigned short intmode, unsigned short dith, unsigned short varmmode);
void GsInit3D(void);
void GsMapModelingData(unsigned long *p);
void GsSetProjection(long h);
int GsSetFlatLight(int id, GsF_LIGHT *lt);
void GsSetLightMode(int mode);
void GsSetFogParam(GsFOGPARAM *fogparm);
void GsSetAmbient(long r, long g, long b);
void GsDrawOt(GsOT *ot);
void GsSetWorkBase(PACKET *outpacketp);
void GsSortObject3(GsDOBJ3 *objp, GsOT *ot, int shift);
void GsSortObject4(GsDOBJ2 *objp, GsOT *ot, int shift, u_long *scratch);
void GsSortObject5(GsDOBJ5 *objp, GsOT *ot, int shift, u_long *scratch);
void GsSortObject5J(GsDOBJ5 *objp, GsOT *ot, int shift, u_long *scratch);
void GsSortSprite(GsSPRITE *sp, GsOT *ot, unsigned short pri);
void GsSortSpriteB(GsSPRITE *sp, GsOT *ot, unsigned short pri, unsigned short flip);
void GsSortFastSprite(GsSPRITE *sp, GsOT *ot, unsigned short pri);
void GsSortFastSpriteB(GsSPRITE *sp, GsOT *ot, unsigned short pri, unsigned short flip);
void GsSortFlipSprite(GsSPRITE *sp, GsOT *ot, unsigned short pri);
void GsInitFastSpriteArray(GsSPARRAY *sp, short n);
void GsSetFastSpriteArray(GsSPARRAY *sp, unsigned short w, unsigned short h, unsigned short tpage, unsigned char u, unsigned char v, unsigned short cx, unsigned short cy, unsigned char r, unsigned char g, unsigned char b);
void GsSortFastSpriteArray(GsSPARRAY *sp, unsigned short n, GsOT *ot, unsigned short pri);
void GsSortBg(GsBG *bg, GsOT *ot, unsigned short pri);
void GsSortFastBg(GsBG *bg, GsOT *ot, unsigned short pri);
void GsInitFixBg16(GsBG *bg, u_long *work);
void GsSortFixBg16(GsBG *bg, u_long *work, GsOT *otp, unsigned short pri);
void GsInitFixBg32(GsBG *bg, u_long *work);
void GsSortFixBg32(GsBG *bg, u_long *work, GsOT *otp, unsigned short pri);
void GsSortLine(GsLINE *lp, GsOT *ot, unsigned short pri);
void GsSortGLine(GsGLINE *lp, GsOT *ot, unsigned short pri);
void GsSortBoxFill(GsBOXF *bp, GsOT *ot, unsigned short pri);
void GsSortPoly(void *pp, GsOT *ot, unsigned short pri);
void GsClearOt(unsigned short offset, unsigned short point, GsOT *otp);
GsOT *GsSortOt(GsOT *ot_src, GsOT *ot_dest);
GsOT *GsCutOt(GsOT *ot_src, GsOT *ot_dest);
void GsDefDispBuff(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1);
void GsSortClear(unsigned char, unsigned char, unsigned char, GsOT *);
void GsGetTimInfo(unsigned long *im, GsIMAGE *tim);
void GsSwapDispBuff(void);
int GsGetActiveBuff(void);
void GsSetDrawBuffClip(void);
void GsSetDrawBuffOffset(void);
void GsSetClip(RECT *clip);
DRAWENV *GsSetClip2(RECT *clip);
void GsSetOffset(long x, long y);
void GsSetOrign(long x, long y);
void GsInitCoordinate2(GsCOORDINATE2 *super, GsCOORDINATE2 *base);
void GsMulCoord0(MATRIX *m1, MATRIX *m2, MATRIX *m3);
void GsMulCoord2(MATRIX *m1, MATRIX *m2);
void GsMulCoord3(MATRIX *m1, MATRIX *m2);
void GsGetLw(GsCOORDINATE2 *m, MATRIX *out);
void GsGetLs(GsCOORDINATE2 *m, MATRIX *out);
void GsGetLws(GsCOORDINATE2 *m, MATRIX *outw, MATRIX *outs);
u_long GsLinkObject3(unsigned long pmd_base, GsDOBJ3 *objp);
void GsLinkObject4(unsigned long tmd_base, GsDOBJ2 *objp, int n);
void GsLinkObject5(unsigned long tmd_base, GsDOBJ5 *objp, int n);
void GsSetLightMatrix(MATRIX *mp);
void GsSetLightMatrix2(MATRIX *mp);
int GsSetRefView2(GsRVIEW2 *pv);
int GsSetRefView2L(GsRVIEW2 *pv);
int GsSetView2(GsVIEW2 *pv);
void GsSetLsMatrix(MATRIX *mp);
void GsSetClip2D(RECT *rectp);
void GsInitVcount();
long GsGetVcount();
void GsClearVcount();
void GsDefDispBuff2(u_short x0, u_short y0, u_short x1, u_short y1);
void GsDrawOtIO(GsOT *ot);
PACKET *GsGetWorkBase();
void GsInitGraph2(u_short x, u_short y, u_short intmode, u_short dith, u_short vrammode);
void GsSortObject4J(GsDOBJ2 *objp, GsOT *otp, int shift, u_long *scratch);
void GsInitFastSprite2(GsSPARRAY *sp, u_short pri, short w, short h, u_short tpage, u_short cba, u_char u, u_char v, u_char r, u_char g, u_char b);
void GsSortFastSprite2(GsSPARRAY *sp, u_short n, GsOT *otp, u_short pri);
void GsClearDispArea(unsigned char r, unsigned char g, unsigned char b);
u_long *GsPresetObject(GsDOBJ5 *objp, u_long *base_addr);
void GsScaleScreen(SVECTOR *scale);
PACKET *GsA4divF3L(TMD_P_F3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divF3LFG(TMD_P_F3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divF3NL(TMD_P_F3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divNF3(TMD_P_NF3 *op, VERT *vp, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divF4L(TMD_P_F4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divF4LFG(TMD_P_F4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divF4NL(TMD_P_F4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divNF4(TMD_P_NF4 *op, VERT *vp, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divG3L(TMD_P_G3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divG3LFG(TMD_P_G3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divG3NL(TMD_P_G3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divNG3(TMD_P_NG3 *op, VERT *vp, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divG4L(TMD_P_G4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divG4LFG(TMD_P_G4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divG4NL(TMD_P_G4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divNG4(TMD_P_NG4 *op, VERT *vp, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTF3L(TMD_P_TF3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTF3LFG(TMD_P_TF3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTF3NL(TMD_P_TF3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divNTF3(TMD_P_TNF3 *op, VERT *vp, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTF4L(TMD_P_TF4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTF4LFG(TMD_P_TF4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTF4NL(TMD_P_TF4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divNTF4(TMD_P_TNF4 *op, VERT *vp, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTF4LM(TMD_P_TF4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTF4LFGM(TMD_P_TF4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTF4NLM(TMD_P_TF4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divNTF4M(TMD_P_TNF4 *op, VERT *vp, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTG3L(TMD_P_TG3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTG3LGG(TMD_P_TG3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTG3NL(TMD_P_TG3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divNTG3(TMD_P_TNG3 *op, VERT *vp, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTG4L(TMD_P_TG4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTG4LFG(TMD_P_TG4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTG4NL(TMD_P_TG4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTNG4(TMD_P_TNG4 *op, VERT *vp, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTG4LM(TMD_P_TG4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTG4LFGM(TMD_P_TG4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTG4NLM(TMD_P_TG4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsA4divTNG4M(TMD_P_TNG4 *op, VERT *vp, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastF3GL(TMD_P_F3G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastF3GLFG(TMD_P_F3G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastF3GNL(TMD_P_F3G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastG3GL(TMD_P_G3G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastG3GLFG(TMD_P_G3G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastG3GNL(TMD_P_G3G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsPrstF3GL(TMD_P_F3G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsPrstF3GLFG(TMD_P_F3G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsPrstF3GNL(TMD_P_F3G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsPrstG3GL(TMD_P_G3G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsPrstG3GLFG(TMD_P_G3G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsPrstG3GNL(TMD_P_G3G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastG3M(TMD_P_G3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastG3MFG(TMD_P_G3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastTG3M(TMD_P_TG3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastTG3MFG(TMD_P_TG3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastF4GL(TMD_P_F4G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastF4GLFG(TMD_P_F4G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastF4GNL(TMD_P_F4G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastG4GL(TMD_P_G4G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastG4GLFG(TMD_P_G4G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastG4GNL(TMD_P_G4G *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastG4M(TMD_P_G4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastG4MFG(TMD_P_G4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastTG4M(TMD_P_TG4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastTG4MFG(TMD_P_TG4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastF3M(TMD_P_F3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastF3MFG(TMD_P_F3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastTF3M(TMD_P_TF3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastTF3MFG(TMD_P_TF3 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastF4M(TMD_P_F4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastF4MFG(TMD_P_F4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastTF4M(TMD_P_TF4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
PACKET *GsTMDfastTF4MFG(TMD_P_TF4 *op, VERT *vp, VERT *np, PACKET *pk, int n, int shift, GsOT *ot, u_long *scratch);
extern u_long *GsU_00000008(GsARGUNIT *);
extern u_long *GsU_00000009(GsARGUNIT *);
extern u_long *GsU_0000000c(GsARGUNIT *);
extern u_long *GsU_0000000d(GsARGUNIT *);
extern u_long *GsU_00000010(GsARGUNIT *);
extern u_long *GsU_00000020(GsARGUNIT *);
extern u_long *GsU_00000011(GsARGUNIT *);
extern u_long *GsU_00000014(GsARGUNIT *);
extern u_long *GsU_00000015(GsARGUNIT *);
extern u_long *GsU_00020008(GsARGUNIT *);
extern u_long *GsU_00020009(GsARGUNIT *);
extern u_long *GsU_0002000c(GsARGUNIT *);
extern u_long *GsU_0002000d(GsARGUNIT *);
extern u_long *GsU_00020010(GsARGUNIT *);
extern u_long *GsU_00020011(GsARGUNIT *);
extern u_long *GsU_00020014(GsARGUNIT *);
extern u_long *GsU_00020015(GsARGUNIT *);
extern u_long *GsU_0000000a(GsARGUNIT *);
extern u_long *GsU_0000000b(GsARGUNIT *);
extern u_long *GsU_0000000e(GsARGUNIT *);
extern u_long *GsU_0000000f(GsARGUNIT *);
extern u_long *GsU_00000012(GsARGUNIT *);
extern u_long *GsU_00000013(GsARGUNIT *);
extern u_long *GsU_00000016(GsARGUNIT *);
extern u_long *GsU_00000017(GsARGUNIT *);
extern u_long *GsU_00030008(GsARGUNIT *);
extern u_long *GsU_00030009(GsARGUNIT *);
extern u_long *GsU_0003000c(GsARGUNIT *);
extern u_long *GsU_0003000d(GsARGUNIT *);
extern u_long *GsU_00030010(GsARGUNIT *);
extern u_long *GsU_00030011(GsARGUNIT *);
extern u_long *GsU_00030014(GsARGUNIT *);
extern u_long *GsU_00030015(GsARGUNIT *);
extern u_long *GsU_00040048(GsARGUNIT *);
extern u_long *GsU_00040049(GsARGUNIT *);
extern u_long *GsU_0004004c(GsARGUNIT *);
extern u_long *GsU_0004004d(GsARGUNIT *);
extern u_long *GsU_00040050(GsARGUNIT *);
extern u_long *GsU_00040051(GsARGUNIT *);
extern u_long *GsU_00040054(GsARGUNIT *);
extern u_long *GsU_00040055(GsARGUNIT *);
extern u_long *GsU_00010008(GsARGUNIT *);
extern u_long *GsU_00010009(GsARGUNIT *);
extern u_long *GsU_0001000c(GsARGUNIT *);
extern u_long *GsU_0001000d(GsARGUNIT *);
extern u_long *GsU_00010010(GsARGUNIT *);
extern u_long *GsU_00010011(GsARGUNIT *);
extern u_long *GsU_00010014(GsARGUNIT *);
extern u_long *GsU_00010015(GsARGUNIT *);
extern u_long *GsU_00050048(GsARGUNIT *);
extern u_long *GsU_00050049(GsARGUNIT *);
extern u_long *GsU_0005004c(GsARGUNIT *);
extern u_long *GsU_0005004d(GsARGUNIT *);
extern u_long *GsU_00050050(GsARGUNIT *);
extern u_long *GsU_00050051(GsARGUNIT *);
extern u_long *GsU_00050054(GsARGUNIT *);
extern u_long *GsU_00050055(GsARGUNIT *);
extern u_long *GsU_00040058(GsARGUNIT *);
extern u_long *GsU_00040059(GsARGUNIT *);
extern u_long *GsU_0004005c(GsARGUNIT *);
extern u_long *GsU_0004005d(GsARGUNIT *);
extern u_long *GsU_01000000(GsARGUNIT *);
extern u_long *GsU_0100000c(GsARGUNIT *);
extern u_long *GsU_0100000d(GsARGUNIT *);
extern u_long *GsU_01000014(GsARGUNIT *);
extern u_long *GsU_01000015(GsARGUNIT *);
extern u_long *GsU_0102000c(GsARGUNIT *);
extern u_long *GsU_0102000d(GsARGUNIT *);
extern u_long *GsU_01020014(GsARGUNIT *);
extern u_long *GsU_01020015(GsARGUNIT *);
extern u_long *GsU_01040048(GsARGUNIT *);
extern u_long *GsU_01040049(GsARGUNIT *);
extern u_long *GsU_0104004c(GsARGUNIT *);
extern u_long *GsU_0104004d(GsARGUNIT *);
extern u_long *GsU_01040050(GsARGUNIT *);
extern u_long *GsU_01040051(GsARGUNIT *);
extern u_long *GsU_01040054(GsARGUNIT *);
extern u_long *GsU_01040055(GsARGUNIT *);
extern u_long *GsU_00000018(GsARGUNIT *);
extern u_long *GsU_00000019(GsARGUNIT *);
extern u_long *GsU_0000001c(GsARGUNIT *);
extern u_long *GsU_0000001d(GsARGUNIT *);
extern u_long *GsU_00000209(GsARGUNIT *);
extern u_long *GsU_0000020d(GsARGUNIT *);
extern u_long *GsU_00000211(GsARGUNIT *);
extern u_long *GsU_00000215(GsARGUNIT *);
extern u_long *GsU_02000000(GsARGUNIT *);
extern u_long *GsU_02000001(GsARGUNIT *);
extern u_long *GsU_00040148(GsARGUNIT *);
extern u_long *GsU_00040149(GsARGUNIT *);
extern u_long *GsU_0004014c(GsARGUNIT *);
extern u_long *GsU_0004014d(GsARGUNIT *);
extern u_long *GsU_00040150(GsARGUNIT *);
extern u_long *GsU_00040151(GsARGUNIT *);
extern u_long *GsU_00040154(GsARGUNIT *);
extern u_long *GsU_00040155(GsARGUNIT *);
extern u_long *GsU_00200008(GsARGUNIT *);
extern u_long *GsU_00200009(GsARGUNIT *);
extern u_long *GsU_0020000c(GsARGUNIT *);
extern u_long *GsU_0020000d(GsARGUNIT *);
extern u_long *GsU_00200010(GsARGUNIT *);
extern u_long *GsU_00200011(GsARGUNIT *);
extern u_long *GsU_00200014(GsARGUNIT *);
extern u_long *GsU_00200015(GsARGUNIT *);
extern u_long *GsU_0120000c(GsARGUNIT *);
extern u_long *GsU_0120000d(GsARGUNIT *);
extern u_long *GsU_01200014(GsARGUNIT *);
extern u_long *GsU_01200015(GsARGUNIT *);
extern u_long *GsU_00080008(GsARGUNIT *);
extern u_long *GsU_00080009(GsARGUNIT *);
extern u_long *GsU_0008000c(GsARGUNIT *);
extern u_long *GsU_0008000d(GsARGUNIT *);
extern u_long *GsU_00080010(GsARGUNIT *);
extern u_long *GsU_00080011(GsARGUNIT *);
extern u_long *GsU_00080014(GsARGUNIT *);
extern u_long *GsU_00080015(GsARGUNIT *);
extern u_long *GsU_000a0008(GsARGUNIT *);
extern u_long *GsU_000a0009(GsARGUNIT *);
extern u_long *GsU_000a000c(GsARGUNIT *);
extern u_long *GsU_000a000d(GsARGUNIT *);
extern u_long *GsU_000a0010(GsARGUNIT *);
extern u_long *GsU_000a0011(GsARGUNIT *);
extern u_long *GsU_000a0014(GsARGUNIT *);
extern u_long *GsU_000a0015(GsARGUNIT *);
extern u_long *GsU_000c0048(GsARGUNIT *);
extern u_long *GsU_000c0049(GsARGUNIT *);
extern u_long *GsU_000c004c(GsARGUNIT *);
extern u_long *GsU_000c004d(GsARGUNIT *);
extern u_long *GsU_000c0050(GsARGUNIT *);
extern u_long *GsU_000c0051(GsARGUNIT *);
extern u_long *GsU_000c0054(GsARGUNIT *);
extern u_long *GsU_000c0055(GsARGUNIT *);
extern u_long *GsU_00000000(GsARGUNIT *);
extern u_long *GsU_05000000(GsARGUNIT *);
extern u_long *GsU_05000001(GsARGUNIT *);
extern u_long *GsU_03000000(GsARGUNIT_ANIM *);
extern int GsU_03000001(GsARGUNIT_ANIM *);
extern int GsU_03000002(GsARGUNIT_ANIM *);
extern int GsU_03000003(GsARGUNIT_ANIM *);
extern int GsU_03000009(GsARGUNIT_ANIM *);
extern int GsU_0300000a(GsARGUNIT_ANIM *);
extern int GsU_0300000b(GsARGUNIT_ANIM *);
extern int GsU_03000010(GsARGUNIT_ANIM *);
extern int GsU_03000020(GsARGUNIT_ANIM *);
extern int GsU_03000030(GsARGUNIT_ANIM *);
extern int GsU_03000010(GsARGUNIT_ANIM *);
extern int GsU_03000011(GsARGUNIT_ANIM *);
extern int GsU_03000012(GsARGUNIT_ANIM *);
extern int GsU_03000013(GsARGUNIT_ANIM *);
extern int GsU_03000019(GsARGUNIT_ANIM *);
extern int GsU_0300001a(GsARGUNIT_ANIM *);
extern int GsU_0300001b(GsARGUNIT_ANIM *);
extern int GsU_03000020(GsARGUNIT_ANIM *);
extern int GsU_03000021(GsARGUNIT_ANIM *);
extern int GsU_03000022(GsARGUNIT_ANIM *);
extern int GsU_03000023(GsARGUNIT_ANIM *);
extern int GsU_03000029(GsARGUNIT_ANIM *);
extern int GsU_0300002a(GsARGUNIT_ANIM *);
extern int GsU_0300002b(GsARGUNIT_ANIM *);
extern int GsU_03000030(GsARGUNIT_ANIM *);
extern int GsU_03000031(GsARGUNIT_ANIM *);
extern int GsU_03000032(GsARGUNIT_ANIM *);
extern int GsU_03000033(GsARGUNIT_ANIM *);
extern int GsU_03000039(GsARGUNIT_ANIM *);
extern int GsU_0300003a(GsARGUNIT_ANIM *);
extern int GsU_0300003b(GsARGUNIT_ANIM *);
extern int GsU_03000100(GsARGUNIT_ANIM *);
extern int GsU_03000111(GsARGUNIT_ANIM *);
extern int GsU_03000112(GsARGUNIT_ANIM *);
extern int GsU_03000119(GsARGUNIT_ANIM *);
extern int GsU_0300011a(GsARGUNIT_ANIM *);
extern int GsU_03000901(GsARGUNIT_ANIM *);
extern int GsU_03000902(GsARGUNIT_ANIM *);
extern int GsU_03000909(GsARGUNIT_ANIM *);
extern int GsU_0300090a(GsARGUNIT_ANIM *);
extern int GsU_03000910(GsARGUNIT_ANIM *);
extern int GsU_03000911(GsARGUNIT_ANIM *);
extern int GsU_03000912(GsARGUNIT_ANIM *);
extern int GsU_03000919(GsARGUNIT_ANIM *);
extern int GsU_0300091a(GsARGUNIT_ANIM *);
extern int GsU_03010110(GsARGUNIT_ANIM *);
extern int GsU_03010111(GsARGUNIT_ANIM *);
extern int GsU_03010121(GsARGUNIT_ANIM *);
extern int GsU_03010141(GsARGUNIT_ANIM *);
extern int GsU_03010171(GsARGUNIT_ANIM *);
extern int GsU_03010112(GsARGUNIT_ANIM *);
extern int GsU_03010122(GsARGUNIT_ANIM *);
extern int GsU_03010142(GsARGUNIT_ANIM *);
extern int GsU_03010182(GsARGUNIT_ANIM *);
extern int GsU_03010172(GsARGUNIT_ANIM *);
extern int GsU_03010210(GsARGUNIT_ANIM *);
extern int GsU_03010211(GsARGUNIT_ANIM *);
extern int GsU_03010221(GsARGUNIT_ANIM *);
extern int GsU_03010241(GsARGUNIT_ANIM *);
extern int GsU_03010271(GsARGUNIT_ANIM *);
extern int GsU_03010212(GsARGUNIT_ANIM *);
extern int GsU_03010222(GsARGUNIT_ANIM *);
extern int GsU_03010242(GsARGUNIT_ANIM *);
extern int GsU_03010272(GsARGUNIT_ANIM *);
extern int GsU_03010310(GsARGUNIT_ANIM *);
extern int GsU_03010311(GsARGUNIT_ANIM *);
extern int GsU_03010321(GsARGUNIT_ANIM *);
extern int GsU_03010341(GsARGUNIT_ANIM *);
extern int GsU_03010371(GsARGUNIT_ANIM *);
extern int GsU_03010312(GsARGUNIT_ANIM *);
extern int GsU_03010322(GsARGUNIT_ANIM *);
extern int GsU_03010342(GsARGUNIT_ANIM *);
extern int GsU_03010372(GsARGUNIT_ANIM *);
extern u_long *GsU_04010020(GsARGUNIT *);
extern u_long *GsU_04010021(GsARGUNIT *);
extern u_long *GsU_04010028(GsARGUNIT *);
extern u_long *GsU_04010029(GsARGUNIT *);
extern u_long *GsU_04010010(GsARGUNIT *);
extern u_long *GsU_04010018(GsARGUNIT *);
extern u_long *GsU_04010011(GsARGUNIT *);
extern u_long *GsU_04010019(GsARGUNIT *);
extern GsCOORDUNIT *GsMapCoordUnit(u_long *, u_long *);
extern u_long *GsGetHeadpUnit(void);
extern int GsScanUnit(u_long *, GsTYPEUNIT *, GsOT *, u_long *);
extern void GsMapUnit(u_long *);
extern void GsSortUnit(GsUNIT *, GsOT *, u_long *);
extern void GsGetLwUnit(GsCOORDUNIT *, MATRIX *);
extern void GsGetLsUnit(GsCOORDUNIT *, MATRIX *);
extern void GsGetLwsUnit(GsCOORDUNIT *, MATRIX *, MATRIX *);
extern int GsSetViewUnit(GsVIEWUNIT *);
extern int GsSetRefViewUnit(GsRVIEWUNIT *);
extern int GsSetRefViewLUnit(GsRVIEWUNIT *);
extern u_long *GsScanAnim(u_long *, GsTYPEUNIT *);
extern long GsLinkAnim(GsSEQ **, u_long *);
extern void GsInitRstVtxMIMe(u_long *primtop, u_long *hp);
extern void GsInitRstNrmMIMe(u_long *primtop, u_long *hp);
extern RECT CLIP2;
extern short PSDBASEX[2];
extern short PSDBASEY[2];
extern short PSDIDX;
extern u_long PSDCNT;
extern _GsPOSITION POSITION;
extern DRAWENV GsDRAWENV;
extern DISPENV GsDISPENV;
extern MATRIX GsLSMATRIX;
extern MATRIX GsWSMATRIX;
extern MATRIX GsWSMATRIX_ORG;
extern long HWD0;
extern long VWD0;
extern MATRIX GsLIGHTWSMATRIX;
extern MATRIX GsIDMATRIX;
extern MATRIX GsIDMATRIX2;
extern PACKET *GsOUT_PACKET_P;
extern long GsADIVZ;
extern short GsADIVW;
extern short GsADIVH;
extern int GsLIGHT_MODE;
extern u_long GsMATE_C;
extern u_long GsLMODE;
extern u_long GsLIGNR;
extern u_long GsLIOFF;
extern u_long GsZOVER;
extern u_long GsBACKC;
extern u_long GsNDIV;
extern u_long GsTRATE;
extern u_long GsTON;
extern u_long GsDISPON;
typedef char dw_bool;
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef struct 
{
  GsDOBJ2 obj;
  GsCOORDINATE2 posMatrix;
  VECTOR scale;
  SVECTOR rotation;
  VECTOR location;
} PositionData;
typedef struct 
{
  int16_t scale1[9];
  int16_t subDelta[9];
  int16_t delta[9];
  int16_t subScale[9];
  char subValue[9];
  uint8_t field5_0x51;
} MomentumData;
typedef struct 
{
  MomentumData *momentum;
  int32_t locX;
  int32_t locY;
  int32_t locZ;
  int16_t animFrame;
  int16_t frameCount;
  int16_t *animInstrPtr;
  void *loopStart;
  int16_t loopEndFrame;
  int16_t textureX;
  int16_t textureY;
  uint8_t animId;
  uint8_t loopCount;
  uint8_t animFlag;
  uint8_t field17_0x31;
  int16_t field18_0x32;
} EntityAnim;
typedef struct 
{
  int32_t type;
  PositionData *posData;
  int32_t *animPtr;
  EntityAnim anim;
  int8_t isOnMap;
  int8_t isOnScreen;
  uint8_t flatSprite;
  uint8_t flatTimer;
} Entity;
typedef struct 
{
  int16_t off;
  int16_t def;
  int16_t speed;
  int16_t brain;
  uint8_t movesPrio[4];
  uint8_t moves[4];
  int16_t hp;
  int16_t mp;
} BaseStats;
typedef struct 
{
  int16_t currentHP;
  int16_t currentMP;
  int16_t unk1;
  uint8_t unk2_1;
  uint8_t unk2_2;
  int16_t vabId;
  uint8_t chargeMode;
  uint8_t field7_0x57;
} CurrentStats;
typedef struct 
{
  BaseStats base;
  CurrentStats current;
} Stats;
typedef struct 
{
  int16_t hp;
  int16_t mp;
  int16_t offense;
  int16_t defense;
  int16_t speed;
  int16_t brains;
} StatsGains;
typedef struct 
{
  Entity entity;
  Stats stats;
} DigimonEntity;
typedef struct 
{
  Entity entity;
  int8_t tamerLevel;
  uint8_t raisedCount;
  uint16_t padding;
} TamerEntity;
typedef struct 
{
  DigimonEntity digimonEntity;
  uint32_t learnedMoves[2];
  uint32_t unk1;
  uint32_t unk2;
  uint8_t name[20];
  int8_t lives;
  int8_t pad[3];
} PartnerEntity;
typedef struct 
{
  DigimonEntity digimonEntity;
  SVECTOR flee;
  int16_t bits;
  uint8_t unk1;
  int8_t unk1_2;
  uint8_t unk2;
  uint8_t scriptId;
  uint8_t autotalk;
  uint8_t unk5;
} NPCEntity;
extern Entity *ENTITY_TABLE[10];
extern TamerEntity TAMER_ENTITY;
extern PartnerEntity PARTNER_ENTITY;
extern NPCEntity NPC_ENTITIES[8];
typedef struct 
{
  VECTOR location;
  VECTOR trayLocation;
  SVECTOR rotation;
  SVECTOR trayRotation;
  uint16_t trigger;
  uint8_t item;
  uint8_t isTaken;
  uint8_t tileX;
  uint8_t tileY;
  uint16_t padding;
} Chest;
extern Chest CHEST_ARRAY[8];
extern GsDOBJ2 MEDAL_OBJECT;
extern GsCOORDINATE2 MEDAL_COORDINATES;
void loadDoors(int16_t doorEntryId);
void loadDirtCartModel(void);
void loadDirtPileModel(void);
void initializeChest(void);
void loadWarpCrystals(int32_t mapId);
void spawnBoulder(void);
void projectPosition(GsCOORDINATE2 *coord, VECTOR *trans, SVECTOR *rot, VECTOR *scale);
void initializeMedalModel(void);
void spawnToyTownBoxes(void);
void spawnGearbox(void);
void spawnAngemonPedestal(void);
void loadTrainingPoop(void);
void spawnChest(int32_t posX, int32_t posY, int32_t posZ, int32_t rotation, uint8_t item, uint16_t trigger);
void unloadMapParts(void);
int32_t tickRotateDoor(int32_t instance, int32_t target);
void drawObject(GsDOBJ2 *obj, GsOT *ot, int32_t flag);
int32_t tickOpenChestTray(int32_t chestId);
int32_t tickCloseChestTray(int32_t chestId);
void decreaseDirtPileSize(void);
void moveBoulder(int32_t diffX, int32_t diffZ);
void openToyTownBox(int32_t boxId);
void somethingToyTown(int32_t boxId);
int32_t moveAngemonPedestal(void);
void setDirtCartModel(int32_t model);
void activateMedalTexture(uint8_t medalId);
typedef struct 
{
  char name[20];
  int32_t boneCount;
  int16_t radius;
  int16_t height;
  uint8_t type;
  uint8_t level;
  uint8_t special[3];
  uint8_t dropItem;
  uint8_t dropChance;
  uint8_t moves[16];
  uint8_t padding;
} DigimonPara;
typedef struct 
{
  uint32_t condition;
  int16_t sleepyHour;
  int16_t sleepyMinute;
  int16_t wakeupHour;
  int16_t wakeupMinute;
  int16_t hoursAwakeDefault;
  int16_t hoursAsleepDefault;
  int16_t timeAwakeToday;
  int16_t sicknessCounter;
  int16_t missedSleepHours;
  int16_t tirednessSleepTimer;
  int16_t poopLevel;
  int16_t unused1;
  int16_t unused2;
  int16_t virusBar;
  int16_t poopingTimer;
  int16_t tiredness;
  int16_t subTiredness;
  int16_t tirednessHungerTimer;
  int16_t discipline;
  int16_t happiness;
  int16_t unused3;
  int16_t unused4;
  int16_t timesBeingSick;
  int16_t sicknessTries;
  int16_t areaEffectTimer;
  int16_t sicknessTimer;
  int16_t injuryTimer;
  int16_t nextHungerHour;
  int16_t energyLevel;
  int16_t foodLevel;
  int16_t starvationTimer;
  int16_t weight;
  int16_t refusedFavFood;
  int16_t emptyStomachTimer;
  int16_t remainingLifetime;
  int16_t age;
  uint16_t trainBoostFlag;
  uint16_t trainBoostValue;
  uint16_t trainBoostTimer;
  int16_t careMistakes;
  int16_t battles;
  int16_t evoTimer;
  int16_t unused5;
  int16_t unused6;
  int16_t unused7;
  int16_t fishCaught;
  int16_t unused8;
  int16_t unused9;
  int16_t unused10;
  int16_t upgradeHPcounter;
  int16_t upgradeMPcounter;
  int16_t upgradeOffCounter;
  int16_t buggedUpgradeBrainsCounter;
  int16_t upgradeDefenseCounter;
  int16_t upgradeSpeedCounter;
  int16_t upgradeBrainCounter;
  int16_t unused11;
  int16_t sukaBackupHP;
  int16_t sukaBackupMP;
  int16_t sukaBackupOff;
  int16_t sukaBackupDef;
  int16_t sukaBackupSpeed;
  int16_t sukaBackupBrain;
  int16_t unused12;
} PartnerPara;
extern DigimonPara DIGIMON_DATA[180];
extern PartnerPara PARTNER_PARA;
typedef void (*TickFunction)(int32_t);
typedef void (*RenderFunction)(int32_t);
typedef struct 
{
  int16_t objectId;
  int16_t instanceId;
  TickFunction tick;
  RenderFunction render;
} WorldObject;
extern WorldObject WORLD_OBJECTS[];
void initializeWorldObjects(void);
int32_t addObject(int32_t objectId, int32_t instanceId, TickFunction tick, RenderFunction render);
int32_t removeObject(int32_t objectId, int32_t instanceId);
void tickObjects(void);
void renderObjects(void);
typedef struct 
{
  VECTOR translation;
  SVECTOR rotation;
  int16_t direction;
  int16_t modelId;
} Map3DObject;
typedef struct 
{
  VECTOR pos1;
  VECTOR pos2;
  SVECTOR rotation1;
  SVECTOR rotation2;
  uint8_t pad30;
  uint8_t pad31;
  uint8_t pad32;
  uint8_t isSet;
  int32_t pad34;
} WarpCrystalData;
extern uint8_t CURRENT_SCREEN;
extern uint8_t MAP_LAYER_ENABLED;
extern int32_t ACTIVE_FRAMEBUFFER;
extern GsOT GS_ORDERING_TABLE[2];
int32_t DOORS_DISABLED;
int8_t DOOR_MODEL_IDS[4];
int8_t DOOR_ROTATION_TIMER;
int8_t ACTIVE_DIRT_CART_MODEL;
int16_t map_object_unused_e42;
int16_t BIG_BOX_LID_PROGRESS;
int16_t SMALL_BOX_LID_PROGRESS;
int8_t TOY_TOWN_SELECTED_BOX;
int16_t MAIN_D_80134E4A;
int16_t ANGEMON_PEDESTAL_PROGRESS_X;
int16_t ANGEMON_PEDESTAL_PROGRESS_Z;
char TMD_EXTENSION[8] = ".TMD";
SVECTOR DIRT_PILE_ROTATION = {0, 0, 0, 0};
SVECTOR TRAINING_POOP_ROTATION = {0, 0, 0, 0};
SVECTOR DIRT_PILE_SIZE_ROTATION = {0, 0, 0, 0};
RECT MEDAL_RECT = {582, 432, 6, 32};
int8_t BOX_CORNER_OFFSETS[8] = {-1, 1, 1, 1, -1, -1, 1, -1};
int8_t DIRT_PILE_SIZE_COUNTER = 15;
char DOOR_PATH_PREFIX[12] = "\\DOOR\\DOOR";
char DIGIT_CHARS[12] = "0123456789";
char DIRT_CART_TMD[20] = "\\ETCNA\\DOSYA.TMD";
char DIRT_CART_BOX_TMD[16] = "\\ETCNA\\HAKO.TMD";
VECTOR DIRT_PILE_LOCATION = {2037, 0, 849, 0};
char DIRT_PILE_TMD[20] = "\\ETCNA\\T_YAMA.TMD";
char CHEST_TMD[16] = "\\ETCNA\\TAKA.TMD";
char WARP_CRYSTAL_TMD[16] = "\\ETCNA\\WARP.TMD";
VECTOR BOULDER_SCALE = {4096, 4096, 4096, 0};
char BOULDER_TMD[16] = "\\ETCNA\\IWA.TMD";
char BOULDER_TIM[16] = "\\ETCNA\\IWA1.TIM";
char MEDAL_TMD[20] = "\\ETCNA\\MEDAL.TMD";
VECTOR BIG_BOX_LOCATION = {500, -600, 600, 0};
VECTOR SMALL_BOX_LOCATION = {-400, -400, 600, 0};
char BIG_BOX_TMD[20] = "\\ETCNA\\BIGBOX.TMD";
char SMALL_BOX_TMD[20] = "\\ETCNA\\SMABOX.TMD";
VECTOR GEARBOX_LOCATION = {-100, 0, 1700, 0};
char GEARBOX_TMD[16] = "\\ETCNA\\GAND.TMD";
VECTOR ANGEMON_PEDESTAL_LOCATION = {0, 0, 1500, 0};
char ANGEMON_PEDESTAL_TMD[16] = "\\ETCNA\\ABOX.TMD";
VECTOR TRAINING_POOP_LOCATION = {1922, 0, 503, 0};
char TRAINING_POOP_TMD[16] = "\\ETCNA\\TRY.TMD";
VECTOR DOOR_SCALE = {4096, 4096, 4096, 0};
VECTOR CHEST_SCALE = {4096, 4096, 4096, 0};
VECTOR WARP_CRYSTAL_SCALE = {4096, 4096, 4096, 0};
VECTOR MOVE_BOULDER_SCALE = {4096, 4096, 4096, 0};
Chest CHEST_ARRAY[8];
GsCOORDINATE2 DOOR_COORDS[4];
char GENERAL_MESH_BUFFER[4][0x800];
GsDOBJ2 DOOR_OBJECTS[4];
Map3DObject MAP_3D_OBJECTS[6];
GsDOBJ2 GENERAL_OBJECT[2];
GsCOORDINATE2 GENERAL_COORDS[2];
GsDOBJ2 GENERAL_OBJECT3;
GsCOORDINATE2 GENERAL_COORDS3;
uint8_t CHEST_MESH_BUFFER[0x800];
GsCOORDINATE2 CHEST_COORDS[2];
GsDOBJ2 CHEST_OBJECTS[2];
GsCOORDINATE2 WARP_CRYSTAL_COORDS[2];
GsDOBJ2 WARP_CRYSTAL_OBJECT[2];
WarpCrystalData WARP_CRYSTAL_DATA[5];
char MEDAL_MESH_BUFFER[0x800];
GsDOBJ2 MEDAL_OBJECT;
GsCOORDINATE2 MEDAL_COORDINATES;
int32_t readFile(char *path, void *buffer);
void readMapTFS(int32_t mapId);
int32_t isTriggerSet(uint16_t trigger);
int32_t loadTextureFile(char *path, uint32_t *outTPage, uint32_t *outClut);
void getModelTile(VECTOR *pos, int16_t *outTileX, int16_t *outTileY);
void setRectangleImpassable(int32_t tileX, int32_t tileY, int32_t size);
int32_t isOffScreen(DVECTOR *xy, int32_t w, int32_t h);
void clearChests(void);
void loadStaticTMD(char *path, char *buffer, GsDOBJ2 *obj, GsCOORDINATE2 *coord);
void renderDoors(int32_t instanceId);
void renderDirtCartModel(int32_t instanceId);
void renderDirtPile(int32_t instanceId);
void renderChest(int32_t instanceId);
void renderWarpCrystals(int32_t instanceId);
void addWarpCrystal(int32_t posX, int32_t posZ);
void renderBoulder(int32_t instanceId);
void renderToyTownBoxes(int32_t instanceId);
void renderGearbox(int32_t instanceId);
void renderAngemonPedestal(int32_t instanceId);
void renderTrainingPoop(int32_t instanceId);
void emptyChests(void);
int32_t isBoxOffScreen(VECTOR *position, int32_t width, int32_t height);
static void *map_object_text_order[] = {activateMedalTexture, setDirtCartModel, moveAngemonPedestal, somethingToyTown, openToyTownBox, moveBoulder, decreaseDirtPileSize, tickCloseChestTray, tickOpenChestTray, drawObject, isBoxOffScreen, tickRotateDoor, unloadMapParts, emptyChests, spawnChest, renderTrainingPoop, loadTrainingPoop, renderAngemonPedestal, spawnAngemonPedestal, renderGearbox, spawnGearbox, renderToyTownBoxes, spawnToyTownBoxes, initializeMedalModel, renderBoulder, projectPosition, spawnBoulder, addWarpCrystal, renderWarpCrystals, loadWarpCrystals, renderChest, initializeChest, renderDirtPile, loadDirtPileModel, renderDirtCartModel, loadDirtCartModel, renderDoors, loadDoors, loadStaticTMD, clearChests};
static void *map_object_sbss_order[] = {&ANGEMON_PEDESTAL_PROGRESS_Z, &ANGEMON_PEDESTAL_PROGRESS_X, &MAIN_D_80134E4A, &TOY_TOWN_SELECTED_BOX, &SMALL_BOX_LID_PROGRESS, &BIG_BOX_LID_PROGRESS, &map_object_unused_e42, &ACTIVE_DIRT_CART_MODEL, &DOOR_ROTATION_TIMER, DOOR_MODEL_IDS, &DOORS_DISABLED};
static void *map_object_bss_order[] = {&MEDAL_COORDINATES, &MEDAL_OBJECT, MEDAL_MESH_BUFFER, WARP_CRYSTAL_DATA, WARP_CRYSTAL_OBJECT, &WARP_CRYSTAL_COORDS, CHEST_OBJECTS, CHEST_COORDS, CHEST_MESH_BUFFER, &GENERAL_COORDS3, &GENERAL_OBJECT3, GENERAL_COORDS, GENERAL_OBJECT, MAP_3D_OBJECTS, DOOR_OBJECTS, GENERAL_MESH_BUFFER, DOOR_COORDS, CHEST_ARRAY};
void clearChests(void);
void loadStaticTMD(char *path, char *buffer, GsDOBJ2 *obj, GsCOORDINATE2 *coord);
void renderDoors(int32_t instanceId);
void loadDirtCartModel(void);
void renderDirtPile(int32_t instanceId);
void initializeChest(void);
void renderChest(int32_t instanceId);
void loadWarpCrystals(int32_t mapId);
void renderWarpCrystals(int32_t instanceId);
void addWarpCrystal(int32_t posX, int32_t posZ);
void spawnBoulder(void);
void projectPosition(GsCOORDINATE2 *coord, VECTOR *trans, SVECTOR *rot, VECTOR *scale);
void renderBoulder(int32_t instanceId);
void initializeMedalModel(void);
void spawnToyTownBoxes(void)
{
  VECTOR bigLoc;
  VECTOR smallLoc;
  char *buf2;
  int32_t i;
  int32_t new_var4;
  GsCOORDINATE2 *new_var;
  GsCOORDINATE2 *wc;
  GsDOBJ2 *new_var5;
  GsCOORDINATE2 *gc;
  char *new_var6;
  char *new_var2;
  int new_var3;
  bigLoc.vx = BIG_BOX_LOCATION.vx;
  bigLoc.vy = BIG_BOX_LOCATION.vy;
  smallLoc.vx = SMALL_BOX_LOCATION.vx;
  if (SMALL_BOX_LOCATION.pad == 0U)
  {
  }
  bigLoc.vz = BIG_BOX_LOCATION.vz;
  smallLoc.vy = SMALL_BOX_LOCATION.vy;
  bigLoc.pad = BIG_BOX_LOCATION.pad;
  smallLoc.vz = SMALL_BOX_LOCATION.vz;
  smallLoc.pad = SMALL_BOX_LOCATION.pad;
  readFile(BIG_BOX_TMD, GENERAL_MESH_BUFFER);
  buf2 = ((char *) GENERAL_MESH_BUFFER) + 0x800;
  readFile(SMALL_BOX_TMD, buf2);
  new_var6 = (char *) GENERAL_MESH_BUFFER;
  GsMapModelingData((u_long *) (new_var6 + 4));
  GsMapModelingData((u_long *) (buf2 + 4));
  GsInitCoordinate2(0, &WARP_CRYSTAL_COORDS[0]);
  GsInitCoordinate2(&WARP_CRYSTAL_COORDS[0], &WARP_CRYSTAL_COORDS[1]);
  buf2 = ((char *) GENERAL_MESH_BUFFER) + 0x800;
  new_var = &GENERAL_COORDS[1];
  GsInitCoordinate2(0, &GENERAL_COORDS[0]);
  GsInitCoordinate2(&GENERAL_COORDS[0], new_var);
  wc = WARP_CRYSTAL_COORDS;
  wc++;
  gc = GENERAL_COORDS;
  new_var2 = new_var6 + 0xC;
  for (i = 0; ((i + 1) - 1) <= (2 - 1); i++)
  {
    GsLinkObject4((u_long) new_var2, new_var5 = &WARP_CRYSTAL_OBJECT[i], i);
    new_var4 = i;
    WARP_CRYSTAL_OBJECT[new_var4].attribute = 0;
    WARP_CRYSTAL_OBJECT[new_var4].coord2 = wc;
    GsLinkObject4((u_long) (buf2 + 0xC), &GENERAL_OBJECT[new_var4], new_var4);
    GENERAL_OBJECT[new_var4].attribute = 0;
    GENERAL_OBJECT[new_var4].coord2 = gc;
    gc++;
  }

  TransMatrix(&WARP_CRYSTAL_COORDS[0].coord, &bigLoc);
  WARP_CRYSTAL_COORDS[0].flg = 0;
  TransMatrix(&GENERAL_COORDS[0].coord, &smallLoc);
  do
  {
    new_var3 = 0;
    GENERAL_COORDS[0].flg = new_var3;
    BIG_BOX_LID_PROGRESS = 0;
    TOY_TOWN_SELECTED_BOX = 0;
    SMALL_BOX_LID_PROGRESS = 0;
  }
  while (0);
  addObject(0xFBD, 0, 0, renderToyTownBoxes);
}

void renderToyTownBoxes(int32_t instanceId);
void spawnGearbox(void);
void renderGearbox(int32_t instanceId);
void spawnAngemonPedestal(void);
void renderAngemonPedestal(int32_t instanceId);
void loadTrainingPoop(void);
void renderTrainingPoop(int32_t instanceId);
void spawnChest(int32_t posX, int32_t posY, int32_t posZ, int32_t rotation, uint8_t item, uint16_t trigger);
void emptyChests(void);
void unloadMapParts(void);
int32_t tickRotateDoor(int32_t instance, int32_t target);
int32_t isBoxOffScreen(VECTOR *position, int32_t width, int32_t height);
void drawObject(GsDOBJ2 *obj, GsOT *ot, int32_t flag);
int32_t tickOpenChestTray(int32_t chestId);
int32_t tickCloseChestTray(int32_t chestId);
void decreaseDirtPileSize(void);
void moveBoulder(int32_t diffX, int32_t diffZ);
void openToyTownBox(int32_t boxId);
void somethingToyTown(int32_t boxId);
int32_t moveAngemonPedestal(void);
void setDirtCartModel(int32_t model);
void activateMedalTexture(uint8_t medalId);
void loadDirtPileModel(void);
void renderDirtCartModel(int32_t instanceId);
