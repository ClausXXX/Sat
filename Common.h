#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

#include "Constants.h"

enum GALNavMsgType
{
	F_NAV,
	I_NAV
};

struct Sattelite
{
	char Valid,
		 Number[2];                  // ����������� ��������� ������� ��������,
									 // ������������� ��������
    enum GALNavMsgType GALNavMsgType;
	double toc,                      // ����� �������� UNIX, �
		   a0, a1, a2,               // ������ �������� GPS/Galileo/BeiDou
		   Crs, dn, M0,
		   Cuc, e, Cus, sqrta,
		   toe, Cic, OMEGA0, Cis,
		   i0, Crc, omega, OMEGAi,
		   ii, Ds,
		   Health, TGD, BGD_E1_E5a, BGD_E1_E5b, TGD1_B1_B3, TGD2_B2_B3,
		   TauN, GammaN,             // ������ �������� �������
		   x0, vx0, ax, Bn,
		   y0, vy0, ay, k,
		   z0, vz0, az,
		   x, y, z,                // ��������� ���������� ��������
		   B, L, H,                // ������������� ���������� ��������
		   vx, vy, vz,             // ���������� ������� �������� ��������
		   P[2], P1[2], P2[2], PC5[2], PC7[2],    // ��������������� (���������� ��������� �� ���������)
								   //��� ������ ����� ��������
		   rho, E, dt, S, tk, Az, El, I, T;
		   // rho - �������������� (������������) ��������� �� �������� �� ��������
		   // dt - �������� � ����� ��������
           // S - �������� ������� �������
		   // tk - �������� ������� ���������� ������� ��������� � �������� ������� �������� (t - toc)
		   // Az - ������ �� �������
		   // El - ���� �����
		   // I - ����������� ��������, �
		   // T - ������������ ��������, �
};

struct InterpolationPoints
{
	double x[INTERPOLATION_ORDER + 1],
		   y[INTERPOLATION_ORDER + 1],
		   z[INTERPOLATION_ORDER + 1],
		   vx[INTERPOLATION_ORDER + 1],
		   vy[INTERPOLATION_ORDER + 1],
		   vz[INTERPOLATION_ORDER + 1],
		   dt[INTERPOLATION_ORDER + 1],
		   toc[INTERPOLATION_ORDER + 1];
};

char* FindStringWithData(FILE*, const char*);
double DateTimeToUNIXTime(short, char, char, char, char, float);
void UNIXTimeToDateTime(double, short*, char*, char*, char*, char*, float*);
int GetIndex(char*, char);
double sqr(double);
void WGS84ToPZ90_02(double, double, double, double*, double*, double*);
void PZ90_02ToWGS84(double, double, double, double*, double*, double*);
double Linear(double*, double*, double, int);
double Lagrange(double*, double*, double, int);
double Neville(double*, double*, double, int);
double Azimuth(double, double, double, double, double, double);
double Elevation(double, double, double, double, double, double);
void DecartToGeo(double, double, double, double*, double*, double*);
void ECEFToLocalENU(double, double, double, double*, double*, double*);

#endif
