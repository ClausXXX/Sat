#include "RINEX.h"

#include <stdlib.h>
#undef abs
#include <math.h>
//#include <alloc.h>
#include <string.h>
#include <time.h>

int ReadRINEXEphemeris(char *EphemerisFileName, struct RINEXNav *RINEXNav)
{
	int i, j;
	char *Pch, CurrentString[BYTES_OF_STRING], NavMessage[8][BYTES_OF_STRING];
	size_t SizeOfRINEXGPSEphemeris = sizeof(struct RINEXGPSEphemeris);
	size_t SizeOfRINEXGLOEphemeris = sizeof(struct RINEXGLOEphemeris);
	size_t SizeOfRINEXGALEphemeris = sizeof(struct RINEXGALEphemeris);
	size_t SizeOfRINEXBDSEphemeris = sizeof(struct RINEXBDSEphemeris);
	FILE *EphemerisFile = fopen(EphemerisFileName, "r");
	if(!EphemerisFile)
	{
		return 0;
	}
	Pch = FindStringWithData(EphemerisFile, "RINEX VERSION / TYPE");
	if(Pch)
	{
		strcpy(CurrentString, Pch);
		sscanf(CurrentString, "%f", &RINEXNav->Version);
	}
	Pch = FindStringWithData(EphemerisFile, "CORR TO SYSTEM TIME");
	if(Pch)
	{
		strcpy(CurrentString, Pch);
		for(i = 0; i < BYTES_OF_STRING; i++)
		{
			if(CurrentString[i] == 'D')
			{
				CurrentString[i] = 'E';
			}
		}
		sscanf(CurrentString, "%*d%*d%*d%lf", &RINEXNav->TauC);
	}
	Pch = FindStringWithData(EphemerisFile, "GLUT");
	if(Pch)
	{
		strcpy(CurrentString, Pch);
		for(i = 0; i < BYTES_OF_STRING; i++)
		{
			if(CurrentString[i] == 'D')
			{
				CurrentString[i] = 'E';
			}
		}
		sscanf(CurrentString, "%*s%lf", &RINEXNav->TauC);
	}
	Pch = FindStringWithData(EphemerisFile, "LEAP SECONDS");
	if(Pch)
	{
		strcpy(CurrentString, Pch);
		sscanf(CurrentString, "%f", &RINEXNav->LeapSeconds);
	}
	RINEXNav->TauC = -RINEXNav->TauC;
	FindStringWithData(EphemerisFile, "END OF HEADER");
    if(!RINEXNav->NOfGPSEphemeris)
	{
		RINEXNav->GPSEphemeris =
			(struct RINEXGPSEphemeris*)calloc(1, SizeOfRINEXGPSEphemeris);
	}

	if(!RINEXNav->NOfGLOEphemeris)
	{
		RINEXNav->GLOEphemeris =
			(struct RINEXGLOEphemeris*)calloc(1, SizeOfRINEXGLOEphemeris);
	}

	if(!RINEXNav->NOfGALEphemeris)
	{
		RINEXNav->GALEphemeris =
			(struct RINEXGALEphemeris*)calloc(1, SizeOfRINEXGALEphemeris);
	}

	if(!RINEXNav->NOfBDSEphemeris)
	{
		RINEXNav->BDSEphemeris =
			(struct RINEXBDSEphemeris*)calloc(1, SizeOfRINEXBDSEphemeris);
	}

	if(RINEXNav->Version < 3.00)
	{
		if(EphemerisFileName[strlen(EphemerisFileName) - 1] == 'n')
		{
			while(!feof(EphemerisFile))
			{
				RINEXNav->GPSEphemeris = (struct RINEXGPSEphemeris*)
										 realloc(RINEXNav->GPSEphemeris,
												 (RINEXNav->NOfGPSEphemeris + 1)
												 * SizeOfRINEXGPSEphemeris);
				for(i = 0; i < 8; i++)
				{
					fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
					for(j = 0; j < BYTES_OF_STRING; j++)
					{
						if(CurrentString[j] == 'D')
						{
							CurrentString[j] = 'E';
						}
					}
					strcpy(NavMessage[i], CurrentString);
				}
				RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Number[0] = 'G';
				sscanf(NavMessage[0], "%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Number[1],
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Month,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Day,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Hours,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Minutes,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Seconds,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].a0,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].a1,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].a2);
				sscanf(NavMessage[1], "%*lf%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Crs,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].dn,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].M0);
				sscanf(NavMessage[2], "%lf%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cuc,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].e,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cus,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].sqrta);
				sscanf(NavMessage[3], "%lf%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].toe,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cic,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].OMEGA0,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cis);
				sscanf(NavMessage[4], "%lf%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].i0,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Crc,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].omega,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].OMEGAi);
				sscanf(NavMessage[5], "%lf%*lf%*lf%*lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].ii);
				sscanf(NavMessage[6], "%*lf%lf%lf%*lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Health,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].TGD);
				if(RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year >= 80 &&
				   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year <= 99)
				{
					RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year += 1900;
				}

				if(RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year >= 0 &&
				   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year <= 79)
				{
					RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year += 2000;
				}
				RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].toc =
				DateTimeToUNIXTime(RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Month,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Day,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Hours,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Minutes,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Seconds);
				RINEXNav->NOfGPSEphemeris++;
			}
		}

		if(EphemerisFileName[strlen(EphemerisFileName) - 1] == 'g')
		{
			while(!feof(EphemerisFile))
			{
				RINEXNav->GLOEphemeris = (struct RINEXGLOEphemeris*)
										 realloc(RINEXNav->GLOEphemeris,
												 (RINEXNav->NOfGLOEphemeris + 1)
												 * SizeOfRINEXGLOEphemeris);
				for(i = 0; i < 4; i++)
				{
					fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
					for(j = 0; j < BYTES_OF_STRING; j++)
					{
						if(CurrentString[j] == 'D')
						{
							CurrentString[j] = 'E';
						}
					}
					strcpy(NavMessage[i], CurrentString);
				}
				RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Number[0] = 'R';
				sscanf(NavMessage[0], "%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%*lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Number[1],
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Month,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Day,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Hours,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Minutes,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Seconds,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].TauN,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].GammaN);
				sscanf(NavMessage[1], "%lf%lf%lf%lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].x0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].vx0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].ax,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Bn);
				sscanf(NavMessage[2], "%lf%lf%lf%lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].y0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].vy0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].ay,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].k);
				sscanf(NavMessage[3], "%lf%lf%lf%*lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].z0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].vz0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].az);
			   if(RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year >= 80 &&
				  RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year <= 99)
				{
					RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year += 1900;
				}

				if(RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year >= 0 &&
				   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year <= 79)
				{
					RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year += 2000;
				}
				RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].toc =
				DateTimeToUNIXTime(RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Month,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Day,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Hours,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Minutes,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Seconds);
				RINEXNav->NOfGLOEphemeris++;
			}
		}

		if(EphemerisFileName[strlen(EphemerisFileName) - 1] == 'l')
		{
			while(!feof(EphemerisFile))
			{
				fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
				sscanf(CurrentString, "%c", &RINEXNav->SatteliteMask);
				RINEXNav->GALEphemeris = (struct RINEXGALEphemeris*)
										 realloc(RINEXNav->GALEphemeris,
												 (RINEXNav->NOfGALEphemeris + 1)
												 * SizeOfRINEXGALEphemeris);
				for(i = 0; i < 8; i++)
				{
					if(i)
					{
						fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
					}

					for(j = 0; j < BYTES_OF_STRING; j++)
					{
						if(CurrentString[j] == 'D')
						{
							CurrentString[j] = 'E';
						}
					}
					strcpy(NavMessage[i], CurrentString);
				}

				if(RINEXNav->SatteliteMask == 'E')
				{
					sscanf(NavMessage[0], "%c%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%lf",
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Number[0],
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Number[1],
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Month,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Day,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Hours,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Minutes,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Seconds,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a0,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a1,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a2);
				}
				else
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Number[0] = 'E';
					sscanf(NavMessage[0], "%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%lf",
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Number[1],
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Month,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Day,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Hours,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Minutes,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Seconds,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a0,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a1,
						   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a2);
				}
				sscanf(NavMessage[1], "%*lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Crs,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].dn,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].M0);
				sscanf(NavMessage[2], "%lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cuc,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].e,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cus,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].sqrta);
				sscanf(NavMessage[3], "%lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].toe,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cic,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].OMEGA0,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cis);
				sscanf(NavMessage[4], "%lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].i0,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Crc,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].omega,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].OMEGAi);
				sscanf(NavMessage[5], "%lf%lf%*lf%*lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].ii,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Ds);
				sscanf(NavMessage[6], "%*lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Health,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].BGD_E1_E5a,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].BGD_E1_E5b);
				if(RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year >= 80 &&
				   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year <= 99)
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year += 1900;
				}

				if(RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year >= 0 &&
				   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year <= 79)
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year += 2000;
				}
				RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].toc =
				DateTimeToUNIXTime(RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Month,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Day,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Hours,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Minutes,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Seconds);
				if((int)RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Ds & (1 << 8))
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].GALNavMsgType = F_NAV;
				}

				if((int)RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Ds & (1 << 9))
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].GALNavMsgType = I_NAV;
				}
				RINEXNav->NOfGALEphemeris++;
			}
		}
	}
	else
	{
		while(!feof(EphemerisFile))
		{
			fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
			sscanf(CurrentString, "%c", &RINEXNav->SatteliteMask);
			if(RINEXNav->SatteliteMask == 'G')
			{
				RINEXNav->GPSEphemeris = (struct RINEXGPSEphemeris*)
										 realloc(RINEXNav->GPSEphemeris,
												 (RINEXNav->NOfGPSEphemeris + 1)
												 * SizeOfRINEXGPSEphemeris);
				for(i = 0; i < 8; i++)
				{
					if(i)
					{
						fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
					}

					for(j = 0; j < BYTES_OF_STRING; j++)
					{
						if(CurrentString[j] == 'D')
						{
							CurrentString[j] = 'E';
						}
					}
					strcpy(NavMessage[i], CurrentString);
				}
				sscanf(NavMessage[0], "%c%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Number[0],
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Number[1],
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Month,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Day,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Hours,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Minutes,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Seconds,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].a0,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].a1,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].a2);
				sscanf(NavMessage[1], "%*lf%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Crs,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].dn,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].M0);
				sscanf(NavMessage[2], "%lf%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cuc,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].e,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cus,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].sqrta);
				sscanf(NavMessage[3], "%lf%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].toe,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cic,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].OMEGA0,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Cis);
				sscanf(NavMessage[4], "%lf%lf%lf%lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].i0,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Crc,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].omega,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].OMEGAi);
				sscanf(NavMessage[5], "%lf%*lf%*lf%*lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].ii);
				sscanf(NavMessage[6], "%*lf%lf%lf%*lf",
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Health,
					   &RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].TGD);
				if(RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year >= 80 &&
				   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year <= 99)
				{
					RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year += 1900;
				}

				if(RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year >= 0 &&
				   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year <= 79)
				{
					RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year += 2000;
				}
				RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].toc =
				DateTimeToUNIXTime(RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Year,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Month,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Day,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Hours,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Minutes,
								   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris].Seconds);
				RINEXNav->NOfGPSEphemeris++;
			}

			if(RINEXNav->SatteliteMask == 'R')
			{
				RINEXNav->GLOEphemeris = (struct RINEXGLOEphemeris*)
										 realloc(RINEXNav->GLOEphemeris,
												 (RINEXNav->NOfGLOEphemeris + 1)
												 * SizeOfRINEXGLOEphemeris);
				for(i = 0; i < 4; i++)
				{
					if(i)
					{
						fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
					}

					for(j = 0; j < BYTES_OF_STRING; j++)
					{
						if(CurrentString[j] == 'D')
						{
							CurrentString[j] = 'E';
						}
					}
					strcpy(NavMessage[i], CurrentString);
				}
				sscanf(NavMessage[0], "%c%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%*lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Number[0],
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Number[1],
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Month,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Day,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Hours,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Minutes,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Seconds,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].TauN,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].GammaN);
				sscanf(NavMessage[1], "%lf%lf%lf%lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].x0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].vx0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].ax,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Bn);
				sscanf(NavMessage[2], "%lf%lf%lf%lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].y0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].vy0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].ay,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].k);
				sscanf(NavMessage[3], "%lf%lf%lf%*lf",
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].z0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].vz0,
					   &RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].az);
				if(RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year >= 80 &&
				   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year <= 99)
				{
					RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year += 1900;
				}

				if(RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year >= 0 &&
				   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year <= 79)
				{
					RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year += 2000;
				}
				RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].toc =
				DateTimeToUNIXTime(RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Year,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Month,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Day,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Hours,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Minutes,
								   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris].Seconds);
				RINEXNav->NOfGLOEphemeris++;
			}

			if(RINEXNav->SatteliteMask == 'E')
			{
				RINEXNav->GALEphemeris = (struct RINEXGALEphemeris*)
										 realloc(RINEXNav->GALEphemeris,
												 (RINEXNav->NOfGALEphemeris + 1)
												 * SizeOfRINEXGALEphemeris);
				for(i = 0; i < 8; i++)
				{
					if(i)
					{
						fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
					}

					for(j = 0; j < BYTES_OF_STRING; j++)
					{
						if(CurrentString[j] == 'D')
						{
							CurrentString[j] = 'E';
						}
					}
					strcpy(NavMessage[i], CurrentString);
				}
				sscanf(NavMessage[0], "%c%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Number[0],
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Number[1],
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Month,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Day,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Hours,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Minutes,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Seconds,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a0,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a1,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].a2);
				sscanf(NavMessage[1], "%*lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Crs,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].dn,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].M0);
				sscanf(NavMessage[2], "%lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cuc,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].e,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cus,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].sqrta);
				sscanf(NavMessage[3], "%lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].toe,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cic,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].OMEGA0,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Cis);
				sscanf(NavMessage[4], "%lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].i0,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Crc,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].omega,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].OMEGAi);
				sscanf(NavMessage[5], "%lf%lf%*lf%*lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].ii,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Ds);
				sscanf(NavMessage[6], "%*lf%lf%lf%lf",
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Health,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].BGD_E1_E5a,
					   &RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].BGD_E1_E5b);
				if(RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year >= 80 &&
				   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year <= 99)
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year += 1900;
				}

				if(RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year >= 0 &&
				   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year <= 79)
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year += 2000;
				}
				RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].toc =
				DateTimeToUNIXTime(RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Year,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Month,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Day,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Hours,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Minutes,
								   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Seconds);
				if((int)RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Ds & (1 << 8))
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].GALNavMsgType = F_NAV;
				}

				if((int)RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].Ds & (1 << 9))
				{
					RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris].GALNavMsgType = I_NAV;
				}
				RINEXNav->NOfGALEphemeris++;
			}

			if(RINEXNav->SatteliteMask == 'C')
			{

				RINEXNav->BDSEphemeris = (struct RINEXBDSEphemeris*)
										 realloc(RINEXNav->BDSEphemeris,
												 (RINEXNav->NOfBDSEphemeris + 1)
												 * SizeOfRINEXBDSEphemeris);
				for(i = 0; i < 8; i++)
				{
					if(i)
					{
						fgets(CurrentString, BYTES_OF_STRING, EphemerisFile);
					}

					for(j = 0; j < BYTES_OF_STRING; j++)
					{
						if(CurrentString[j] == 'D')
						{
							CurrentString[j] = 'E';
						}
					}
					strcpy(NavMessage[i], CurrentString);
				}
				sscanf(NavMessage[0], "%c%hhd%hd%hhd%hhd%hhd%hhd%f%lf%lf%lf",
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Number[0],
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Number[1],
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Month,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Day,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Hours,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Minutes,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Seconds,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].a0,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].a1,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].a2);
				sscanf(NavMessage[1], "%*lf%lf%lf%lf",
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Crs,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].dn,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].M0);
				sscanf(NavMessage[2], "%lf%lf%lf%lf",
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Cuc,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].e,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Cus,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].sqrta);
				sscanf(NavMessage[3], "%lf%lf%lf%lf",
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].toe,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Cic,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].OMEGA0,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Cis);
				sscanf(NavMessage[4], "%lf%lf%lf%lf",
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].i0,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Crc,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].omega,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].OMEGAi);
				sscanf(NavMessage[5], "%lf%*lf%*lf%*lf",
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].ii);
				sscanf(NavMessage[6], "%*lf%lf%lf%lf",
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].SatH1,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].TGD1_B1_B3,
					   &RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].TGD2_B2_B3);
				if(RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year >= 80 &&
				   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year <= 99)
				{
					RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year += 1900;
				}

				if(RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year >= 0 &&
				   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year <= 79)
				{
					RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year += 2000;
				}
				RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].toc =
				DateTimeToUNIXTime(RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Year,
								   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Month,
								   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Day,
								   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Hours,
								   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Minutes,
								   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris].Seconds);
				RINEXNav->NOfBDSEphemeris++;
			}
		}
	}
	fclose(EphemerisFile);
	return 1;
}
