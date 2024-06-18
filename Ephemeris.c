#include "Ephemeris.h"

#include <math.h>

void FindRINEXEphemeris(struct Settings *Settings, struct RINEXNav *RINEXNav,
						struct Sattelite *Sattelites, double CurrentTime)
{
	int i, j;
	double deltat, tau;
	enum GALNavMsgType WantedMsgType;
	if(Settings->GalileoDataType == 'F')
	{
		WantedMsgType = F_NAV;
	}

	if(Settings->GalileoDataType == 'I')
	{
		WantedMsgType = I_NAV;
	}

	for(i = 0; i < Settings->NumberOfSattelites; i++)
	{
		if(Settings->Sattelites[i])
		{
			Sattelites[i].Valid = 0;
			j = 0;
			if(Settings->Sattelites[i][0] == 'G')
			{
				while(!Sattelites[i].Valid && j < RINEXNav->NOfGPSEphemeris)
				{
					deltat = fabs(CurrentTime - RINEXNav->GPSEphemeris[j].toc);
					if(RINEXNav->GPSEphemeris[j].Number[0] == Settings->Sattelites[i][0] &&
					   RINEXNav->GPSEphemeris[j].Number[1] == Settings->Sattelites[i][1] &&
					   deltat <= 7200.0 && !RINEXNav->GPSEphemeris[j].Health)
					{
						Sattelites[i].Valid = 1;
						Sattelites[i].Number[0] = RINEXNav->GPSEphemeris[j].Number[0];
						Sattelites[i].Number[1] = RINEXNav->GPSEphemeris[j].Number[1];
						Sattelites[i].a0 = RINEXNav->GPSEphemeris[j].a0;
						Sattelites[i].a1 = RINEXNav->GPSEphemeris[j].a1;
						Sattelites[i].a2 = RINEXNav->GPSEphemeris[j].a2;
						Sattelites[i].Crs = RINEXNav->GPSEphemeris[j].Crs;
						Sattelites[i].dn = RINEXNav->GPSEphemeris[j].dn;
						Sattelites[i].M0 = RINEXNav->GPSEphemeris[j].M0;
						Sattelites[i].Cuc = RINEXNav->GPSEphemeris[j].Cuc;
						Sattelites[i].e = RINEXNav->GPSEphemeris[j].e;
						Sattelites[i].Cus = RINEXNav->GPSEphemeris[j].Cus;
						Sattelites[i].sqrta = RINEXNav->GPSEphemeris[j].sqrta;
						Sattelites[i].toe = RINEXNav->GPSEphemeris[j].toe;
						Sattelites[i].Cic = RINEXNav->GPSEphemeris[j].Cic;
						Sattelites[i].OMEGA0 = RINEXNav->GPSEphemeris[j].OMEGA0;
						Sattelites[i].Cis = RINEXNav->GPSEphemeris[j].Cis;
						Sattelites[i].i0  = RINEXNav->GPSEphemeris[j].i0;
						Sattelites[i].Crc = RINEXNav->GPSEphemeris[j].Crc;
						Sattelites[i].omega = RINEXNav->GPSEphemeris[j].omega;
						Sattelites[i].OMEGAi = RINEXNav->GPSEphemeris[j].OMEGAi;
						Sattelites[i].ii = RINEXNav->GPSEphemeris[j].ii;
						Sattelites[i].Health = RINEXNav->GPSEphemeris[j].Health;
						Sattelites[i].TGD = RINEXNav->GPSEphemeris[j].TGD;
						Sattelites[i].toc = RINEXNav->GPSEphemeris[j].toc;
						Sattelites[i].tk = CurrentTime - Sattelites[i].toc;
						//printf("\n%c%d %lf", Sattelites[i].Number[0],  Sattelites[i].Number[1], Sattelites[i].SecondsFromEpoch);
					}
					j++;
				}
			}

			if(Settings->Sattelites[i][0] == 'R')
			{
				while(!Sattelites[i].Valid && j < RINEXNav->NOfGLOEphemeris)
				{
					deltat = fabs(CurrentTime - RINEXNav->GLOEphemeris[j].toc - RINEXNav->LeapSeconds);
					if(RINEXNav->GLOEphemeris[j].Number[0] == Settings->Sattelites[i][0] &&
					   RINEXNav->GLOEphemeris[j].Number[1] == Settings->Sattelites[i][1] &&
					   deltat <= (900.0 + RINEXNav->LeapSeconds) && !RINEXNav->GLOEphemeris[j].Bn)
					{
						Sattelites[i].Valid = 1;
						Sattelites[i].Number[0] = RINEXNav->GLOEphemeris[j].Number[0];
						Sattelites[i].Number[1] = RINEXNav->GLOEphemeris[j].Number[1];
						Sattelites[i].TauN = -RINEXNav->GLOEphemeris[j].TauN;
						Sattelites[i].GammaN = RINEXNav->GLOEphemeris[j].GammaN;
						Sattelites[i].x0 = RINEXNav->GLOEphemeris[j].x0 * 1000.0;
						Sattelites[i].vx0 = RINEXNav->GLOEphemeris[j].vx0 * 1000.0;
						Sattelites[i].ax = RINEXNav->GLOEphemeris[j].ax * 1000.0;
						Sattelites[i].Bn = RINEXNav->GLOEphemeris[j].Bn;
						Sattelites[i].y0 = RINEXNav->GLOEphemeris[j].y0 * 1000.0;
						Sattelites[i].vy0 = RINEXNav->GLOEphemeris[j].vy0 * 1000.0;
						Sattelites[i].ay = RINEXNav->GLOEphemeris[j].ay * 1000.0;
						Sattelites[i].k = RINEXNav->GLOEphemeris[j].k;
						Sattelites[i].z0 = RINEXNav->GLOEphemeris[j].z0 * 1000.0;
						Sattelites[i].vz0 = RINEXNav->GLOEphemeris[j].vz0 * 1000.0;
						Sattelites[i].az = RINEXNav->GLOEphemeris[j].az * 1000.0;
						Sattelites[i].toc = RINEXNav->GLOEphemeris[j].toc + RINEXNav->LeapSeconds;
						Sattelites[i].tk = CurrentTime - Sattelites[i].toc;
					}
					j++;
				}
			}

			if(Settings->Sattelites[i][0] == 'E')
			{
				while(!Sattelites[i].Valid && j < RINEXNav->NOfGALEphemeris)
				{
					deltat = fabs(CurrentTime - RINEXNav->GALEphemeris[j].toc);
					if(RINEXNav->GALEphemeris[j].Number[0] == Settings->Sattelites[i][0] &&
					   RINEXNav->GALEphemeris[j].Number[1] == Settings->Sattelites[i][1] &&
					   deltat <= 7200.0 && !RINEXNav->GALEphemeris[j].Health &&
					   RINEXNav->GALEphemeris[j].GALNavMsgType == WantedMsgType)
					{
						Sattelites[i].Valid = 1;
                        Sattelites[i].GALNavMsgType = RINEXNav->GALEphemeris[j].GALNavMsgType;
						Sattelites[i].Number[0] = RINEXNav->GALEphemeris[j].Number[0];
						Sattelites[i].Number[1] = RINEXNav->GALEphemeris[j].Number[1];
						Sattelites[i].a0 = RINEXNav->GALEphemeris[j].a0;
						Sattelites[i].a1 = RINEXNav->GALEphemeris[j].a1;
						Sattelites[i].a2 = RINEXNav->GALEphemeris[j].a2;
						Sattelites[i].Crs = RINEXNav->GALEphemeris[j].Crs;
						Sattelites[i].dn = RINEXNav->GALEphemeris[j].dn;
						Sattelites[i].M0 = RINEXNav->GALEphemeris[j].M0;
						Sattelites[i].Cuc = RINEXNav->GALEphemeris[j].Cuc;
						Sattelites[i].e = RINEXNav->GALEphemeris[j].e;
						Sattelites[i].Cus = RINEXNav->GALEphemeris[j].Cus;
						Sattelites[i].sqrta = RINEXNav->GALEphemeris[j].sqrta;
						Sattelites[i].toe = RINEXNav->GALEphemeris[j].toe;
						Sattelites[i].Cic = RINEXNav->GALEphemeris[j].Cic;
						Sattelites[i].OMEGA0 = RINEXNav->GALEphemeris[j].OMEGA0;
						Sattelites[i].Cis = RINEXNav->GALEphemeris[j].Cis;
						Sattelites[i].i0  = RINEXNav->GALEphemeris[j].i0;
						Sattelites[i].Crc = RINEXNav->GALEphemeris[j].Crc;
						Sattelites[i].omega = RINEXNav->GALEphemeris[j].omega;
						Sattelites[i].OMEGAi = RINEXNav->GALEphemeris[j].OMEGAi;
						Sattelites[i].ii = RINEXNav->GALEphemeris[j].ii;
						Sattelites[i].Ds = RINEXNav->GALEphemeris[j].Ds;
						Sattelites[i].Health = RINEXNav->GALEphemeris[j].Health;
						Sattelites[i].BGD_E1_E5a = RINEXNav->GALEphemeris[j].BGD_E1_E5a;
						Sattelites[i].BGD_E1_E5b = RINEXNav->GALEphemeris[j].BGD_E1_E5b;
						Sattelites[i].toc = RINEXNav->GALEphemeris[j].toc;
						Sattelites[i].tk = CurrentTime - Sattelites[i].toc;
					}
					j++;
				}
			}

			if(Settings->Sattelites[i][0] == 'C')
			{
				while(!Sattelites[i].Valid && j < RINEXNav->NOfBDSEphemeris)
				{
					deltat = fabs(CurrentTime - RINEXNav->BDSEphemeris[j].toc - 14.0);
					if(RINEXNav->BDSEphemeris[j].Number[0] == Settings->Sattelites[i][0] &&
					   RINEXNav->BDSEphemeris[j].Number[1] == Settings->Sattelites[i][1] &&
					   deltat <= (3600.0 + 14.0) && !RINEXNav->BDSEphemeris[j].SatH1)
					{
						Sattelites[i].Valid = 1;
						Sattelites[i].Number[0] = RINEXNav->BDSEphemeris[j].Number[0];
						Sattelites[i].Number[1] = RINEXNav->BDSEphemeris[j].Number[1];
						Sattelites[i].a0 = RINEXNav->BDSEphemeris[j].a0;
						Sattelites[i].a1 = RINEXNav->BDSEphemeris[j].a1;
						Sattelites[i].a2 = RINEXNav->BDSEphemeris[j].a2;
						Sattelites[i].Crs = RINEXNav->BDSEphemeris[j].Crs;
						Sattelites[i].dn = RINEXNav->BDSEphemeris[j].dn;
						Sattelites[i].M0 = RINEXNav->BDSEphemeris[j].M0;
						Sattelites[i].Cuc = RINEXNav->BDSEphemeris[j].Cuc;
						Sattelites[i].e = RINEXNav->BDSEphemeris[j].e;
						Sattelites[i].Cus = RINEXNav->BDSEphemeris[j].Cus;
						Sattelites[i].sqrta = RINEXNav->BDSEphemeris[j].sqrta;
						Sattelites[i].toe = RINEXNav->BDSEphemeris[j].toe;
						Sattelites[i].Cic = RINEXNav->BDSEphemeris[j].Cic;
						Sattelites[i].OMEGA0 = RINEXNav->BDSEphemeris[j].OMEGA0;
						Sattelites[i].Cis = RINEXNav->BDSEphemeris[j].Cis;
						Sattelites[i].i0  = RINEXNav->BDSEphemeris[j].i0;
						Sattelites[i].Crc = RINEXNav->BDSEphemeris[j].Crc;
						Sattelites[i].omega = RINEXNav->BDSEphemeris[j].omega;
						Sattelites[i].OMEGAi = RINEXNav->BDSEphemeris[j].OMEGAi;
						Sattelites[i].ii = RINEXNav->BDSEphemeris[j].ii;
						Sattelites[i].TGD1_B1_B3 = RINEXNav->BDSEphemeris[j].TGD1_B1_B3;
						Sattelites[i].TGD2_B2_B3 = RINEXNav->BDSEphemeris[j].TGD2_B2_B3;
						Sattelites[i].toc = RINEXNav->BDSEphemeris[j].toc + 14.0;
						Sattelites[i].tk = CurrentTime - Sattelites[i].toc;
					}
					j++;
				}
			}

		}
	}
}

void FindSP3Ephemeris(struct Settings *Settings, struct SP3 *SP3,
					  struct Sattelite *Sattelites,
					  struct InterpolationPoints *InterpolationPoints,
					  double CurrentTime, double NullTime)
{
	const int GLOFreqLiters[26] = { 1, -4, 5,  6, 1, -4, 5, 6, -2, -7, 0, -1,
								   -2, -7, 0, -1, 4, -3, 3, 2,  4, -3, 3,  2
								   -8,  6};
	int InterpolationOrder = INTERPOLATION_ORDER, Index1, Index2, i, j, k, l, m;
	double deltat, tau;
	for(i = 0; i < Settings->NumberOfSattelites; i++)
	{

		if(InterpolationOrder % 2 == 0)
		{
			Index1 = -InterpolationOrder / 2;
			Index2 = InterpolationOrder / 2;
		}
		else
		{
			Index1 = -InterpolationOrder / 2;
			Index2 = InterpolationOrder / 2 + 1;
		}

		while((CurrentTime + SP3->Step * Index1) <= NullTime)
		{
			Index1++;
			Index2++;
		}

		while((CurrentTime + SP3->Step * Index2) >= NullTime + 86400.0)
		{
			Index1--;
			Index2--;
		}
		m = 0;
		for(k = Index1; k <= Index2; k++)
		{
			l = 0;
			if(Settings->Sattelites[i][0] == 'G')
			{
				while(l < SP3->NOfGPSEphemeris)
				{
					deltat = fabs(CurrentTime + SP3->Step * k - SP3->GPSEphemeris[l].toc);
					if(SP3->GPSEphemeris[l].Number[0] == Settings->Sattelites[i][0] &&
					   SP3->GPSEphemeris[l].Number[1] == Settings->Sattelites[i][1] &&
					   deltat <= SP3->Step)
					{
						if(SP3->GPSEphemeris[l].x == 0.0 ||
						   SP3->GPSEphemeris[l].y == 0.0 ||
						   SP3->GPSEphemeris[l].z == 0.0 ||
						   SP3->GPSEphemeris[l].dt == 999999.9999)
						{
							Sattelites[i].Valid = 0;
							k = Index2 + 1;
							l = SP3->NOfGPSEphemeris;
						}
						else
						{
							if(!Sattelites[i].Valid)
							{
								Sattelites[i].Valid = 1;
								Sattelites[i].Number[0] = SP3->GPSEphemeris[l].Number[0];
								Sattelites[i].Number[1] = SP3->GPSEphemeris[l].Number[1];
								Sattelites[i].tk = CurrentTime;
							}
							InterpolationPoints[i].x[m] = SP3->GPSEphemeris[l].x * 1000.0;
							InterpolationPoints[i].y[m] = SP3->GPSEphemeris[l].y * 1000.0;
							InterpolationPoints[i].z[m] = SP3->GPSEphemeris[l].z * 1000.0;
							InterpolationPoints[i].dt[m] = SP3->GPSEphemeris[l].dt * 1E-6;
							InterpolationPoints[i].toc[m] = SP3->GPSEphemeris[l].toc;
							m++;
							l = SP3->NOfGPSEphemeris;
						}
					}
                    else
					{
						Sattelites[i].Valid = 0;
					}
					l++;
				}
			}

			if(Settings->Sattelites[i][0] == 'R')
			{
				while(l < SP3->NOfGLOEphemeris)
				{
					deltat = fabs(CurrentTime + SP3->Step * k - SP3->GLOEphemeris[l].toc);
					if(SP3->GLOEphemeris[l].Number[0] == Settings->Sattelites[i][0] &&
					   SP3->GLOEphemeris[l].Number[1] == Settings->Sattelites[i][1] &&
					   deltat <= SP3->Step)
					{
						if(SP3->GLOEphemeris[l].x == 0.0 ||
						   SP3->GLOEphemeris[l].y == 0.0 ||
						   SP3->GLOEphemeris[l].z == 0.0 ||
						   SP3->GLOEphemeris[l].dt == 999999.9999)
						{
							Sattelites[i].Valid = 0;
							k = Index2 + 1;
							l = SP3->NOfGLOEphemeris;
						}
						else
						{
							if(!Sattelites[i].Valid)
							{
								Sattelites[i].Valid = 1;
								Sattelites[i].Number[0] = SP3->GLOEphemeris[l].Number[0];
								Sattelites[i].Number[1] = SP3->GLOEphemeris[l].Number[1];
								Sattelites[i].k = GLOFreqLiters[Sattelites[i].Number[1] - 1];
								Sattelites[i].tk = CurrentTime;
							}
							InterpolationPoints[i].x[m] = SP3->GLOEphemeris[l].x * 1000.0;
							InterpolationPoints[i].y[m] = SP3->GLOEphemeris[l].y * 1000.0;
							InterpolationPoints[i].z[m] = SP3->GLOEphemeris[l].z * 1000.0;
							InterpolationPoints[i].dt[m] = SP3->GLOEphemeris[l].dt * 1E-6;
							InterpolationPoints[i].toc[m] = SP3->GLOEphemeris[l].toc;
							m++;
							l = SP3->NOfGLOEphemeris;
						}
					}
                    else
					{
						Sattelites[i].Valid = 0;
					}
					l++;
				}
			}

			if(Settings->Sattelites[i][0] == 'E')
			{
				while(l < SP3->NOfGALEphemeris)
				{
					deltat = fabs(CurrentTime + SP3->Step * k - SP3->GALEphemeris[l].toc);
					if(SP3->GALEphemeris[l].Number[0] == Settings->Sattelites[i][0] &&
					   SP3->GALEphemeris[l].Number[1] == Settings->Sattelites[i][1] &&
					   deltat <= SP3->Step)
					{
						if(SP3->GALEphemeris[l].x == 0.0 ||
						   SP3->GALEphemeris[l].y == 0.0 ||
						   SP3->GALEphemeris[l].z == 0.0 ||
						   SP3->GALEphemeris[l].dt == 999999.9999)
						{
							Sattelites[i].Valid = 0;
							k = Index2 + 1;
							l = SP3->NOfGALEphemeris;
						}
						else
						{
							if(!Sattelites[i].Valid)
							{
								Sattelites[i].Valid = 1;
								Sattelites[i].Number[0] = SP3->GALEphemeris[l].Number[0];
								Sattelites[i].Number[1] = SP3->GALEphemeris[l].Number[1];
								Sattelites[i].tk = CurrentTime;
							}
							InterpolationPoints[i].x[m] = SP3->GALEphemeris[l].x * 1000.0;
							InterpolationPoints[i].y[m] = SP3->GALEphemeris[l].y * 1000.0;
							InterpolationPoints[i].z[m] = SP3->GALEphemeris[l].z * 1000.0;
							InterpolationPoints[i].dt[m] = SP3->GALEphemeris[l].dt * 1E-6;
							InterpolationPoints[i].toc[m] = SP3->GALEphemeris[l].toc;
							//InterpolationPoints[i].t[m] = RINEXObs->Epochs[RINEXObs->CurrentEpoch].t - tau - InterpolationPoints[i].toc[m];
							m++;
							l = SP3->NOfGALEphemeris;
						}
					}
					else
					{
						Sattelites[i].Valid = 0;
					}
					l++;
				}
			}

			if(Settings->Sattelites[i][0] == 'C')
			{
				while(l < SP3->NOfBDSEphemeris)
				{
					deltat = fabs(CurrentTime + SP3->Step * k - SP3->BDSEphemeris[l].toc);
					if(SP3->BDSEphemeris[l].Number[0] == Settings->Sattelites[i][0] &&
					   SP3->BDSEphemeris[l].Number[1] == Settings->Sattelites[i][1] &&
					   deltat <= SP3->Step)
					{
						if(SP3->BDSEphemeris[l].x == 0.0 ||
						   SP3->BDSEphemeris[l].y == 0.0 ||
						   SP3->BDSEphemeris[l].z == 0.0 ||
						   SP3->BDSEphemeris[l].dt == 999999.9999)
						{
							Sattelites[i].Valid = 0;
							k = Index2 + 1;
							l = SP3->NOfBDSEphemeris;
						}
						else
						{
							if(!Sattelites[i].Valid)
							{
								Sattelites[i].Valid = 1;
								Sattelites[i].Number[0] = SP3->BDSEphemeris[l].Number[0];
								Sattelites[i].Number[1] = SP3->BDSEphemeris[l].Number[1];
                                Sattelites[i].tk = CurrentTime;
							}
							InterpolationPoints[i].x[m] = SP3->BDSEphemeris[l].x * 1000.0;
							InterpolationPoints[i].y[m] = SP3->BDSEphemeris[l].y * 1000.0;
							InterpolationPoints[i].z[m] = SP3->BDSEphemeris[l].z * 1000.0;
							InterpolationPoints[i].dt[m] = SP3->BDSEphemeris[l].dt * 1E-6;
							InterpolationPoints[i].toc[m] = SP3->BDSEphemeris[l].toc;
							m++;
							l = SP3->NOfBDSEphemeris;
						}
					}
                    else
					{
						Sattelites[i].Valid = 0;
					}
					l++;
				}
			}
		}
	}
}

// ������ ��������� � ��������� �������� GPS/Galileo/BeiDou
// *Sattelite - ��������� �� ���������, ���������� ��������� ��������:
void GPSSatteliteXV(struct Sattelite *Sattelite, char Velocity)
{
	double a, n0, n, M, E0, nu, F, du, dr, di, u, r, i, x, y, xg, yg, zg, OMEGA,
		   Mi, Ei, nui, Fi, dui, dri, dii, ui, ri, ii, xi, yi, xgi, ygi, zgi, OMEGAi,
		   cos2F, sin2F, cosOMEGAi_WGS84tk, sinOMEGAi_WGS84tk;
	a = sqr(Sattelite->sqrta);
	n0 = sqrt(mu_WGS84 / (a * sqr(a)));
	n = n0 + Sattelite->dn;
	M = Sattelite->M0 + n * Sattelite->tk;
	Sattelite->E = M;
	do
	{
		E0 = Sattelite->E;
		Sattelite->E -= (M + Sattelite->e * sin(Sattelite->E) - Sattelite->E) /
						(Sattelite->e * cos(Sattelite->E) - 1.0);
	}
	while(fabs(Sattelite->E - E0) >= 1.0E-8);
	nu = atan2((sqrt(1.0 - sqr(Sattelite->e)) * sin(Sattelite->E)),
			   (cos(Sattelite->E) - Sattelite->e));
	F = nu + Sattelite->omega;
	sin2F = sin(2.0 * F);
	cos2F = cos(2.0 * F);
	du = Sattelite->Cus * sin2F + Sattelite->Cuc * cos2F;
	dr = Sattelite->Crs * sin2F + Sattelite->Crc * cos2F;
	di = Sattelite->Cis * sin2F + Sattelite->Cic * cos2F;
	u = F + du;
	r = a * (1.0 - Sattelite->e * cos(Sattelite->E)) + dr;
	i = Sattelite->i0 + di + Sattelite->ii * Sattelite->tk;
	x = r * cos(u);
	y = r * sin(u);
	if(Sattelite->Number[0] == 'C' &&
	   (Sattelite->Number[1] <= 5 || Sattelite->Number[1] >= 59))
	{
        OMEGA = Sattelite->OMEGA0 + Sattelite->OMEGAi * Sattelite->tk -
				OMEGAi_WGS84 * Sattelite->toe;
		xg = x * cos(OMEGA) - y * cos(i) * sin(OMEGA);
		yg = x * sin(OMEGA) + y * cos(i) * cos(OMEGA);
		zg = y * sin(i);
		cosOMEGAi_WGS84tk = cos(OMEGAi_WGS84 * Sattelite->tk);
		sinOMEGAi_WGS84tk = sin(OMEGAi_WGS84 * Sattelite->tk) ;
		Sattelite->x = xg * cosOMEGAi_WGS84tk +
					   yg * sinOMEGAi_WGS84tk * COS_MINUS_5_DEG +
					   zg * sinOMEGAi_WGS84tk * SIN_MINUS_5_DEG;
		Sattelite->y = -xg * sinOMEGAi_WGS84tk +
						yg * cosOMEGAi_WGS84tk * COS_MINUS_5_DEG +
						zg * cosOMEGAi_WGS84tk * SIN_MINUS_5_DEG;
		Sattelite->z = -yg * SIN_MINUS_5_DEG + zg * COS_MINUS_5_DEG;
	}
	else
	{
        OMEGA = Sattelite->OMEGA0 + (Sattelite->OMEGAi - OMEGAi_WGS84) *
				Sattelite->tk - OMEGAi_WGS84 * Sattelite->toe;
		Sattelite->x = x * cos(OMEGA) - y * cos(i) * sin(OMEGA);
		Sattelite->y = x * sin(OMEGA) + y * cos(i) * cos(OMEGA);
		Sattelite->z = y * sin(i);
	}

	if(Velocity)
	{
		Mi = n;
		Ei = Mi / (1.0 - Sattelite->e * cos(Sattelite->E));
		nui = Ei * sin(Sattelite->E) * (1.0 + Sattelite->e * cos(nu)) /
			  (sin(nu) * (1.0 - Sattelite->e * cos(Sattelite->E)));
		Fi =  nui;
		dui = 2.0 * Fi * (Sattelite->Cus * cos2F - Sattelite->Cuc * sin2F);
		dri = 2.0 * Fi * (Sattelite->Crs * cos2F - Sattelite->Crc * sin2F);
		dii = 2.0 * Fi * (Sattelite->Cis * cos2F - Sattelite->Cic * sin2F);
		ui = Fi + dui;
		ri = a * Sattelite->e * Ei * sin(Sattelite->E) + dri;
		ii = Sattelite->ii + dii;
		if(Sattelite->Number[0] == 'C' &&
		   (Sattelite->Number[1] <= 5 || Sattelite->Number[1] >= 59))
		{
			OMEGAi = Sattelite->OMEGAi;
			xi = ri * cos(u) - y * ui;
			yi = ri * sin(u) + x * ui;
			xgi = xi * cos(OMEGA) - yi * cos(i) * sin(OMEGA) +
				  y * ii * sin(i) * sin(OMEGA) - Sattelite->y * OMEGAi;
			ygi = xi * sin(OMEGA) + yi * cos(i) * cos(OMEGA) -
				  y * ii * sin(i) * cos(OMEGA) + Sattelite->x * OMEGAi;
			zgi = yi * sin(i) + y * ii * cos(i);
			Sattelite->vx = xgi * cosOMEGAi_WGS84tk -
							 OMEGAi_WGS84 * xg * sinOMEGAi_WGS84tk +
							 ygi * sinOMEGAi_WGS84tk * COS_MINUS_5_DEG +
							 OMEGAi_WGS84 * yg * cosOMEGAi_WGS84tk * COS_MINUS_5_DEG +
							 zgi * sinOMEGAi_WGS84tk * SIN_MINUS_5_DEG +
							 OMEGAi_WGS84 * zg * cosOMEGAi_WGS84tk * SIN_MINUS_5_DEG;
			Sattelite->vy = -xgi * sinOMEGAi_WGS84tk -
							 OMEGAi_WGS84 * xg * cosOMEGAi_WGS84tk +
							 ygi * cos(OMEGAi_WGS84 * Sattelite->tk) * COS_MINUS_5_DEG -
							 OMEGAi_WGS84 * yg * sinOMEGAi_WGS84tk * COS_MINUS_5_DEG +
							 zgi * cos(OMEGAi_WGS84 * Sattelite->tk) * SIN_MINUS_5_DEG -
							 OMEGAi_WGS84 * zg * sinOMEGAi_WGS84tk * SIN_MINUS_5_DEG;
			Sattelite->vz = -ygi * SIN_MINUS_5_DEG + zgi * COS_MINUS_5_DEG;
		}
		else
		{
			OMEGAi = Sattelite->OMEGAi - OMEGAi_WGS84;
			xi = ri * cos(u) - y * ui;
			yi = ri * sin(u) + x * ui;
			Sattelite->vx = xi * cos(OMEGA) - yi * cos(i) * sin(OMEGA) +
							y * ii * sin(i) * sin(OMEGA) - Sattelite->y * OMEGAi;
			Sattelite->vy = xi * sin(OMEGA) + yi * cos(i) * cos(OMEGA) -
							y * ii * sin(i) * cos(OMEGA) + Sattelite->x * OMEGAi;
			Sattelite->vz = yi * sin(i) + y * ii * cos(i);
		}
	}
}

// ������ ������������� ��� ������� ������� ��������� ��������
// �������� ������� ������� �����-����� 4 �������
// *K - ������ �������������� �������������
// *X - ������ ��������� � ��������� ���������, �
// *Xii - ������ ��������� �� ���������� ���� � ������, �/�^2
// dt - ��� ��������������, �
void GLODiffEqCoefficients(double *K, double *X, double *Xii, double dt)
{
	double r, r2, r3, r5, C[5];
	r = sqrt(sqr(X[0]) + sqr(X[1]) + sqr(X[2]));
	r2 = sqr(r);
	r3 = r * r2;
	r5 = r2 * r3;
	C[0] = mu_PZ90 / r3;
	C[1] = 1.5 * J20_PZ90 * mu_PZ90 * sqr(ae_PZ90) / r5;
	C[2] = 5.0 * sqr(X[2]) / r2;
	C[3] = sqr(omega_PZ90);
	C[4] = 2.0 * omega_PZ90;
	K[0] = X[3] * dt;
	K[1] = X[4] * dt;
	K[2] = X[5] * dt;
	K[3] = (-C[0] * X[0] - C[1] * X[0] * (1.0 - C[2]) + C[3] * X[0] +
		   C[4] * X[4] + Xii[0]) * dt;
	K[4] = (-C[0] * X[1] - C[1] * X[1] * (1.0 - C[2]) + C[3] * X[1] -
		   C[4] * X[3] + Xii[1]) * dt;
	K[5] = (-C[0] * X[2] - C[1] * X[2] * (3.0 - C[2]) + Xii[2]) * dt;
}

// ������ ��������� � ��������� �������� �������
// *Sattelite - ��������� �� ���������, ���������� ��������� ��������:
void GLOSatteliteXV(struct Sattelite *Sattelite)
{
	int i;
	double t, dt, X[6], Xk[6], Xii[3], K1[6], K2[6], K3[6], K4[6];
	X[0] = Sattelite->x;
	X[1] = Sattelite->y;
	X[2] = Sattelite->z;
	X[3] = Sattelite->vx0;
	X[4] = Sattelite->vy0;
	X[5] = Sattelite->vz0;
	Xii[0] = Sattelite->ax;
	Xii[1] = Sattelite->ay;
	Xii[2] = Sattelite->az;
	t = Sattelite->tk;
	if(t > 0)
	{
		dt = GLOStep;
	}
	else
	{
		dt = -GLOStep;
	}

	while(fabs(t) > 0)
	{
		if(fabs(t) < GLOStep)
		{
			dt = t;
		}
		GLODiffEqCoefficients(K1, X, Xii, dt);
		for(i = 0; i < 6; i++)
		{
			Xk[i] = X[i] + K1[i] / 2.0;
		}
		GLODiffEqCoefficients(K2, Xk, Xii, dt);
		for(i = 0; i < 6; i++)
		{
			Xk[i] = X[i] + K2[i] / 2.0;
		}
		GLODiffEqCoefficients(K3, Xk, Xii, dt);
		for(i = 0; i < 6; i++)
		{
			Xk[i] = X[i] + K3[i];
		}
		GLODiffEqCoefficients(K4, Xk, Xii, dt);
		for(i = 0; i < 6; i++)
		{
			X[i] += (K1[i] + 2.0 * (K2[i] + K3[i]) + K4[i]) / 6.0;
		}
		t -= dt;
	}
	Sattelite->xi = X[0];
	Sattelite->yi = X[1];
	Sattelite->zi = X[2];
	Sattelite->vx = X[3];
	Sattelite->vy = X[4];
	Sattelite->vz = X[5];
}
