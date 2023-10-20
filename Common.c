#include "Common.h"

#include <math.h>
#include <time.h>
#include <string.h>

// ����� ������ � ����������� ���������� � �����
// *f - ��������� �� ����
// *String - ���������, ������� ������
// ������������ �������� - ��������� �� ������, � ������� ������� ���������,
// ���� �� ������� - NULL
char* FindStringWithData(FILE *f, const char *String)
{
	char CurrentString[BYTES_OF_STRING], *Pch;
	rewind(f);
    Pch = CurrentString;
	do
	{
		fgets(CurrentString, BYTES_OF_STRING, f);
		if(strstr(CurrentString, String))
		{
			return Pch;
        }
	}
	while(!feof(f));
	return NULL;
}

double DateTimeToUNIXTime(short Year, char Month, char Day,
						  char Hours, char Minutes, float Seconds)
{
	struct tm tm;
	tm.tm_year = Year - 1900;
	tm.tm_mon = Month - 1;
	tm.tm_mday = Day;
	tm.tm_hour = Hours;
	tm.tm_min = Minutes;
	tm.tm_sec = (int)Seconds;
	tm.tm_isdst = 0;
	return (double)mktime(&tm) + (Seconds - floor(Seconds));
}

void UNIXTimeToDateTime(double UNIXTime, short *Year, char *Month, char *Day,
						char *Hours, char *Minutes, float *Seconds)
{
	struct tm *tm;
	time_t t = (time_t)UNIXTime;
	tm = localtime(&t);
	*Year = tm->tm_year + 1900;
	*Month = tm->tm_mon + 1;
	*Day = tm->tm_mday;
	*Hours = tm->tm_hour;
	*Minutes = tm->tm_min;
	*Seconds = tm->tm_sec + (UNIXTime - floor(UNIXTime));
}

int GetIndex(char* String, char c)
{
	char *e = strchr(String, c);
	if (e == NULL)
	{
		return -1;
	}
	return (int)(e - String);
}

double sqr(double x)
{
	return x * x;
}

// ������� ��������� �� ������� WGS-84 � ��-90.02
// x, y, z - ���������� � ������� WGS-84, �
// *xi, *yi, *zi - ��������� �� �������������� ���������� � ������� ��-90.02, �
void WGS84ToPZ90_02(double x, double y, double z,
					double *xi, double *yi, double *zi)
{
	*xi = x + 0.36;
	*yi = y - 0.08;
	*zi = z - 0.18;
}

// ������� ��������� �� ������� ��-90.02 � WGS-84
// x, y, z - ���������� � ������� ��-90.02, �
// *xi, *yi, *zi - ��������� �� �������������� ���������� � ������� WGS-84, �
void PZ90_02ToWGS84(double x, double y, double z,
                    double *xi, double *yi, double *zi)
{
	*xi = x - 0.36;
	*yi = y + 0.08;
	*zi = z + 0.18;
}

// ����� �������������� ������������ �������
// *Y - ������ ��������� �������� ������� � ����� xi
// *x - ������ �������� ����� ��������� ������� xi
// x0 - ��������� ����f x
// n - ����� ����� ������������ (������� ������������ + 1)
// ������������ �������� - �������� ������� � ��������� �����
double Neville(double *Y, double *x, double x0, int n)
{
	int i, j;
	double y[INTERPOLATION_ORDER + 3];
	for(i = 0; i < n; i++)
	{
		y[i] = Y[i];
	}

	for(j = 1; j < n; j++)
	{
		for(i = 0; i < n - j; i++)
		{
			y[i] = ((x0 - x[i + j]) * y[i] - (x0 - x[i]) * y[i + 1]) / (x[i + j] - x[i]);
		}
	}
	return y[0];
}

// ������ ������� ����� ����� ������������ ������
// X, Y, Z - ���������� ����� ����������, �
// x, y, z - ���������� ����������� �����, �
// ������������ �������� - ������, ���
double Azimuth(double X, double Y, double Z, double x, double y, double z)
{
	double xy, xyz, cosl, sinl, sint, xn1, xn2, xn3, xe1, xe2;
	double z1, z2, z3, p1, p2, alpha;
	xy = sqr(X) + sqr(Y);
	xyz = xy + sqr(Z);
	xy = sqrt(xy);
	xyz = sqrt(xyz);
	cosl = X / xy;
	sinl = Y / xy;
	sint = Z / xyz;
	xn1 = -sint * cosl;
	xn2 = -sint * sinl;
	xn3 = xy / xyz;
	xe1 = -sinl;
	xe2 = cosl;
	z1 = x - X;
	z2 = y - Y;
	z3 = z - Z;
	p1 = (xn1 * z1) + (xn2 * z2) + (xn3 * z3);
	p2 = (xe1 * z1) + (xe2 * z2);
	alpha = M_PI_2 - atan2(p1, p2);
	if (alpha < 0.0)
	{
		 return alpha + 2.0 * M_PI;
	}
	else
	{
		return alpha;
	}
}

// ������ ���� ���������� (���� �����) ��� ����������
// X, Y, Z - ���������� ����� ����������, �
// x, y, z - ���������� ����������� �����, �
// ������������ �������� - ���� ����������, ���
double Elevation(double X, double Y, double Z, double x, double y, double z)
{
	double rx, ry, cosEl;
	double r[3];
	r[0] = x - X;
	r[1] = y - Y;
	r[2] = z - Z;
	rx = sqr(r[0]) + sqr(r[1]) + sqr(r[2]);
	ry = sqr(X) + sqr(Y) + sqr(Z);
	cosEl = (r[0] * X + r[1] * Y + r[2] * Z) / sqrt(rx * ry);
	if (fabs(cosEl) > 1.0)
	{
		cosEl = fabs(cosEl) / cosEl;
	}
	return M_PI_2 - acos(cosEl);
}

// ������� ������������� ��������� ECEF XYZ � ������������� BLH
// x, y, z - ��������� ����������, �
// *B, *L, *H - ��������� ��  �������������� ������������� ����������
// �, �, �
void DecartToGeo(double x, double y, double z, double *B, double *L, double *H)
{
	const double e2 = 6.69437999013E-3;
	double B0, D, N;
	D = sqrt(sqr(x) + sqr(y));
	if(D == 0.0)
	{
		*B = M_PI_2;
		*L = 0.0;
        N = ae_WGS84 / sqrt(1.0 - e2 * sqr(sin(*B)));
	}
	else
	{
		*B = 1.0;
		do
		{
			B0 = *B;
			N = ae_WGS84 / sqrt(1.0 - e2 * sqr(sin(*B)));
			*B = atan2((z + N * e2 * sin(*B)), D);
		}
		while (fabs(*B - B0) >= 1.0E-8);
		*L = atan2(y, x);
	}
	*H = z / sin(*B) - N * (1.0 - e2);
	*B *= RadToDeg;
	*L *= RadToDeg;
}

// ������� ������������� ��������� ECEF XYZ � ���������������� ENU
// x, y, z - ��������� ����������, �
// *E, *N, *U - ��������� ��  �������������� ���������������� ����������, �
void ECEFToLocalENU(double x, double y, double z,
                    double *E, double *N, double *U)
{
	double B, L, H;
	DecartToGeo(x, y, z, &B, &L, &H);
	B *= DegToRad;
	L *= DegToRad;
	*E = -x * sin(L) + y * cos(L);
	*N = -x * sin(B) * cos(L) - y * sin(B) * sin(L) + z * cos(B);
	*U = x * cos(B) * cos(L) + y * cos(B) * sin(L) + z * sin(B);
}
