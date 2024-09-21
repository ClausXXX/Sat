#ifndef RINEX_H
#define RINEX_H

#include <stdio.h>
#include <dir.h>
#include <string.h>

#include "Common.h"

struct RINEXGPSEphemeris
{
	short Year;
	char Number[2], Month, Day, Hours, Minutes;
	float Seconds;
	double toc, a0, a1, a2,
		   Crs, dn, M0,
		   Cuc, e, Cus, sqrta,
		   toe, Cic, OMEGA0, Cis,
		   i0, Crc, omega, OMEGAi,
		   ii,
		   Health, TGD;
};

struct RINEXGLOEphemeris
{
	short Year;
	char Number[2], Month, Day, Hours, Minutes;
	float Seconds;
	double toc, TauN, GammaN,
		   x0, vx0, ax, Bn,
		   y0, vy0, ay, k,
		   z0, vz0, az;
};

struct RINEXGALEphemeris
{
    enum GALNavMsgType GALNavMsgType;
	short Year;
	char Number[2], Month, Day, Hours, Minutes;
	float Seconds;
	double toc, a0, a1, a2,
		   Crs, dn, M0,
		   Cuc, e, Cus, sqrta,
		   toe, Cic, OMEGA0, Cis,
		   i0, Crc, omega, OMEGAi,
		   ii, Ds,
		   Health, BGD_E1_E5a, BGD_E1_E5b;
};

struct RINEXBDSEphemeris
{
	short Year;
	char Number[2], Month, Day, Hours, Minutes;
	float Seconds;
	double toc, a0, a1, a2,
		   Crs, dn, M0,
		   Cuc, e, Cus, sqrta,
		   toe, Cic, OMEGA0, Cis,
		   i0, Crc, omega, OMEGAi,
		   ii,
		   SatH1, TGD1_B1_B3, TGD2_B2_B3;
};

struct RINEXNav
{
	char Type, SatteliteMask;
	double AlphaGPS[4], BetaGPS[4], AzGAL[3], AlphaBDS[12][9],
		   a0, A1, T, W,
		   TauC;
	float Version, LeapSeconds;
	int NOfGPSEphemeris, NOfGLOEphemeris, NOfGALEphemeris, NOfBDSEphemeris;
	struct RINEXGPSEphemeris *GPSEphemeris;
	struct RINEXGLOEphemeris *GLOEphemeris;
	struct RINEXGALEphemeris *GALEphemeris;
    struct RINEXBDSEphemeris *BDSEphemeris;
};

int ReadRINEXEphemeris(char*, struct RINEXNav*);

#endif
