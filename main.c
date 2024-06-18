/*
	Программа нахождения местоположения спутников ГНСС
	Sat
	© Затолокин Д.А., 2015-2023
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
//#include <alloc.h>
#include <dir.h>
#include <time.h>

#include "Common.h"
#include "Constants.h"
#include "DataFiles.h"
#include "RINEX.h"
#include "SP3.h"
#include "Ephemeris.h"

int main(int argc, char **argv)
{
	char Month, Day, Hours, Minutes,
		 *Pch, CurrentString[BYTES_OF_STRING], OutputFileName[MAXPATH],
		 OutputFileString[BYTES_OF_STRING];
	short Year;
	int i, j, k;
	float Seconds;
	double deltat, tau = 0, theta, NullTime, CurrentTime;
	FILE *outputfile = NULL;
	size_t SizeOfChar = sizeof(char);
	size_t SizeOfCharPtr = sizeof(char*);
	size_t SizeOfRINEXNav = sizeof(struct RINEXNav);
	size_t SizeOfSP3 = sizeof(struct SP3);
    size_t SizeOfSattelite = sizeof(struct Sattelite);
	size_t SizeOfInterpolationPoints = sizeof(struct InterpolationPoints);
	struct Settings Settings;
	struct RINEXNav *RINEXNav = NULL;
	struct SP3 *SP3 = NULL;
	struct Sattelite *Sattelites = NULL;
	struct InterpolationPoints *InterpolationPoints = NULL;
    enum Ephemeris Ephemeris;
	RINEXNav = (struct RINEXNav*)calloc(1, SizeOfRINEXNav);
	SP3 = (struct SP3*)calloc(1, SizeOfSP3);
	SetDefaultSettings(&Settings);
	if(!ReadSettings("sat.ini", &Settings))
	{
		WriteSettings("sat.ini", &Settings);
	}

	if(argc == 2)
	{
		for(i = 1; i < argc; i++)
		{
			strcpy(Settings.EphemerisFile, "");
			if(argv[i][strlen(argv[i]) - 1] == 'n' ||
			   argv[i][strlen(argv[i]) - 1] == 'g' ||
			   argv[i][strlen(argv[i]) - 1] == 'l' ||
			   argv[i][strlen(argv[i]) - 1] == 'p' ||
			   strstr(argv[i], "N.rnx") ||
			   strstr(argv[i], ".sp3"))
			{
				strcpy(Settings.EphemerisFile, argv[i]);
			}
        }
	}

	Ephemeris = BOARD;
	if(strstr(Settings.EphemerisFile, ".sp3"))
	{
		Ephemeris = PRECISE;
	}

    if(Ephemeris == BOARD)
	{
		if(strcmp(Settings.EphemerisFile, "") &&
		   !strstr(Settings.EphemerisFile, ".sp3"))
		{
			if(!ReadRINEXEphemeris(Settings.EphemerisFile, RINEXNav))
			{
				printf("Unable to open ephemeris file %s", Settings.EphemerisFile);
				if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
				{
					getch();
				}
				return 1;
			}
		}

		if(RINEXNav->NOfGPSEphemeris +
		   RINEXNav->NOfGLOEphemeris +
		   RINEXNav->NOfGALEphemeris +
		   RINEXNav->NOfBDSEphemeris == 0)
		{
			printf("Broadcast ephemeris not loaded");
			if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
			{
				getch();
			}
			return 1;
		}

        if(RINEXNav->NOfGPSEphemeris)
		{
			NullTime =
			DateTimeToUNIXTime(RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris / 2].Year,
							   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris / 2].Month,
							   RINEXNav->GPSEphemeris[RINEXNav->NOfGPSEphemeris / 2].Day,
							   0, 0, 0.0);
		}

		if(RINEXNav->NOfGLOEphemeris)
		{
			NullTime =
			DateTimeToUNIXTime(RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris / 2].Year,
							   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris / 2].Month,
							   RINEXNav->GLOEphemeris[RINEXNav->NOfGLOEphemeris / 2].Day,
							   0, 0, 0.0);
		}

		if(RINEXNav->NOfGALEphemeris)
		{
			NullTime =
			DateTimeToUNIXTime(RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris / 2].Year,
							   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris / 2].Month,
							   RINEXNav->GALEphemeris[RINEXNav->NOfGALEphemeris / 2].Day,
							   0, 0, 0.0);
		}

		if(RINEXNav->NOfBDSEphemeris)
		{
			NullTime =
			DateTimeToUNIXTime(RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris / 2].Year,
							   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris / 2].Month,
							   RINEXNav->BDSEphemeris[RINEXNav->NOfBDSEphemeris / 2].Day,
							   0, 0, 0.0);
		}
	}

	if(Ephemeris == PRECISE)
	{
		if(strstr(Settings.EphemerisFile, ".sp3"))
		{
			if(!ReadSP3Ephemeris(Settings.EphemerisFile, SP3))
			{
				printf("Unable to open ephemeris file %s", Settings.EphemerisFile);
				if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
				{
					getch();
				}
				return 1;
			}
		}

		if(SP3->NOfGPSEphemeris +
		   SP3->NOfGLOEphemeris +
		   SP3->NOfGALEphemeris +
		   SP3->NOfBDSEphemeris == 0)
		{
			printf("Precise ephemeris not loaded");
			if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
			{
				getch();
			}
			return 1;
		}

        if(SP3->NOfGPSEphemeris)
		{
			NullTime =
			DateTimeToUNIXTime(SP3->GPSEphemeris[SP3->NOfGPSEphemeris / 2].Year,
							   SP3->GPSEphemeris[SP3->NOfGPSEphemeris / 2].Month,
							   SP3->GPSEphemeris[SP3->NOfGPSEphemeris / 2].Day,
							   0, 0, 0.0);
		}

		if(SP3->NOfGLOEphemeris)
		{
			NullTime =
			DateTimeToUNIXTime(SP3->GLOEphemeris[SP3->NOfGLOEphemeris / 2].Year,
							   SP3->GLOEphemeris[SP3->NOfGLOEphemeris / 2].Month,
							   SP3->GLOEphemeris[SP3->NOfGLOEphemeris / 2].Day,
							   0, 0, 0.0);
		}

		if(SP3->NOfGALEphemeris)
		{
			NullTime =
			DateTimeToUNIXTime(SP3->GALEphemeris[SP3->NOfGALEphemeris / 2].Year,
							   SP3->GALEphemeris[SP3->NOfGALEphemeris / 2].Month,
							   SP3->GALEphemeris[SP3->NOfGALEphemeris / 2].Day,
							   0, 0, 0.0);
		}

		if(SP3->NOfBDSEphemeris)
		{
			NullTime =
			DateTimeToUNIXTime(SP3->BDSEphemeris[SP3->NOfBDSEphemeris / 2].Year,
							   SP3->BDSEphemeris[SP3->NOfBDSEphemeris / 2].Month,
							   SP3->BDSEphemeris[SP3->NOfBDSEphemeris / 2].Day,
							   0, 0, 0.0);
		}
	}

	if(!strcmp(Settings.SattelitesString, ""))
	{
		printf("Enter Numbers of sattelites (G2 R7 E13 C8... Xn) ");
		gets(Settings.SattelitesString);
	}
	Settings.Sattelites = (char**)calloc(1, SizeOfCharPtr);
	i = 0;
	strcpy(CurrentString, Settings.SattelitesString);
	Pch = strpbrk(CurrentString, "GRESC");
	while(Pch)
	{
		Settings.Sattelites =
		(char**)realloc(Settings.Sattelites, (i + 1) * SizeOfCharPtr);
		Settings.Sattelites[i] = (char*)calloc(2, SizeOfChar);
		Settings.Sattelites[i][0] = Pch[0];
		Pch = strpbrk(Pch + 1, "GRESC");
		i++;
	}
	Settings.NumberOfSattelites = i;
	i = 0;
	Pch = strtok(CurrentString, " GRESC");
	while(Pch)
	{
		Settings.Sattelites[i][1] = atoi(Pch);
		Pch = strtok(NULL, " GRESC");
		i++;
	}

	if(!Settings.InitialHours &&
	   !Settings.InitialMinutes &&
	   !Settings.InitialSeconds &&
	   !Settings.FinalHours &&
	   !Settings.FinalMinutes &&
	   !Settings.FinalSeconds)
	{
		printf("Enter initial time (h m s) ");
		scanf("%hhd %hhd %f", &Settings.InitialHours,
							  &Settings.InitialMinutes,
							  &Settings.InitialSeconds);
		printf("Enter final time (h m s) ");
		scanf("%hhd %hhd %f", &Settings.FinalHours,
							  &Settings.FinalMinutes,
							  &Settings.FinalSeconds);
	}

	if(!Settings.Step)
	{
		printf("Enter Step (s) ");
		scanf("%f", &Settings.Step);
	}

	if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
	{
		printf(" EphemerisFilename = %s\n", Settings.EphemerisFile);
		printf(" Sattelites = %s\n", Settings.SattelitesString);
		printf(" InitialTime = %hhd %hhd %.1f\n", Settings.InitialHours, Settings.InitialMinutes, Settings.InitialSeconds);
		printf(" FinalTime = %hhd %hhd %.1f\n", Settings.FinalHours, Settings.FinalMinutes, Settings.FinalSeconds);
		printf(" Step = %.1f\n", Settings.Step);
		printf(" SimpleVelocity = %d\n", Settings.SimpleVelocity);
        printf(" GalileoDataType = %c\n", Settings.GalileoDataType);
		if(Settings.x || Settings.y || Settings.z)
		{
			DecartToGeo(Settings.x, Settings.y, Settings.z, &Settings.B, &Settings.L, &Settings.H);
            printf("\n Receiver position:");
			printf("\n x0 = %12.3lf m  B0 = %13.8lf \xF8", Settings.x, Settings.B);
			printf("\n y0 = %12.3lf m  L0 = %13.8lf \xF8", Settings.y, Settings.L);
			printf("\n z0 = %12.3lf m  H0 = %13.3lf m\n", Settings.z, Settings.H);
		}
		printf("\n ------------------------------------------------------------------------------");
		printf("\n    date       time     S       x(m)         y(m)         z(m)          dt(us)");
		if(Settings.Geodethic)
		{
			printf("\n                                B(\xF8)         L(\xF8)         H(\xF8)");
        }
		printf("\n                                 vx(m/s)      vy(m/s)      vz(m/s)");
		if(Settings.x || Settings.y || Settings.z)
		{
			printf("\n                                  A(\xF8)         E(\xF8)         r(m)");
		}
		printf("\n ------------------------------------------------------------------------------");
	}

	if(Settings.OutputType == TO_FILE || Settings.OutputType == TO_SCREEN_FILE)
	{
		if(Settings.OutputType == TO_FILE)
		{
			printf("\nProcessing...\n\n");
		}
		mkdir(Settings.OutputPath);
		chdir(Settings.OutputPath);
		strcpy(CurrentString, "");
		strcpy(CurrentString, Settings.EphemerisFile);
		Pch = strtok(CurrentString, "\\");
		while(Pch && !strstr(Pch, "."))
		{
			Pch = strtok(NULL, "\\");
		}

		for(i = 0; i < (int)strlen(Pch); i++)
		{
			if(Pch[i] == '.')
			{
				Pch[i] = '_';
			}
		}
		strcpy(OutputFileString, Pch);
		sprintf(OutputFileName, "%s.txt", OutputFileString);
		outputfile = fopen(OutputFileName, "w");
		if(!outputfile)
		{
			printf("Unable to create output file %s!", OutputFileName);
			if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
			{
				getch();
			}
			return 1;
		}
		WriteOutputFileHeader(outputfile, &Settings);
	}
    Sattelites =
	(struct Sattelite*)calloc(Settings.NumberOfSattelites, SizeOfSattelite);
	if(Ephemeris == PRECISE)
	{
		InterpolationPoints = (struct InterpolationPoints*)
		calloc(Settings.NumberOfSattelites, SizeOfInterpolationPoints);
	}
	Settings.InitialTime = NullTime + Settings.InitialHours * 3600 +
									  Settings.InitialMinutes * 60 +
									  Settings.InitialSeconds;
	Settings.FinalTime = NullTime + Settings.FinalHours * 3600 +
									Settings.FinalMinutes * 60 +
									Settings.FinalSeconds;
	CurrentTime = Settings.InitialTime;
	while(CurrentTime <= Settings.FinalTime)
	{
		if(Ephemeris == BOARD)
		{
			FindRINEXEphemeris(&Settings, RINEXNav,
							   Sattelites, CurrentTime);
		}

		if(Ephemeris == PRECISE)
		{
			FindSP3Ephemeris(&Settings, SP3,
							 Sattelites, InterpolationPoints, CurrentTime, NullTime);
		}

		for(i = 0; i < Settings.NumberOfSattelites; i++)
		{
			if(Sattelites[i].Valid)
			{
				for(j = 0; j < 2; j++)
				{
					if(Settings.SimpleVelocity)
					{
						Sattelites[i].vx = 0.0;
						Sattelites[i].vy = 0.0;
						Sattelites[i].vz = 0.0;
					}

					if(Ephemeris == BOARD)
					{
						if(Settings.Sattelites[i][0] == 'G' ||
						   Settings.Sattelites[i][0] == 'E' ||
						   Settings.Sattelites[i][0] == 'C')
						{
							GPSSatteliteXV(&Sattelites[i], !Settings.SimpleVelocity);
							Sattelites[i].dt = Sattelites[i].a0 +
										   Sattelites[i].a1 * Sattelites[i].tk +
										   Sattelites[i].a2 * sqr(Sattelites[i].tk)
										   + C * Sattelites[i].e * Sattelites[i].sqrta * sin(Sattelites[i].E);
										   //- 2.0 * (Sattelites[i].x * Sattelites[i].vx + Sattelites[i].y * Sattelites[i].vy + Sattelites[i].z * Sattelites[i].vz) / sqr(c);
						   //	printf("\n%d %lf %lf %lf", Sattelites[i].Number[1], Sattelites[i].vx, Sattelites[i].vy, Sattelites[i].vz);

						}

						if(Settings.Sattelites[i][0] == 'R')
						{
							WGS84ToPZ90_02(Sattelites[i].x0, Sattelites[i].y0, Sattelites[i].z0,
											  &Sattelites[i].x, &Sattelites[i].y, &Sattelites[i].z);
							GLOSatteliteXV(&Sattelites[i]);
							PZ90_02ToWGS84(Sattelites[i].xi, Sattelites[i].yi, Sattelites[i].zi,
										  &Sattelites[i].x, &Sattelites[i].y, &Sattelites[i].z);
							Sattelites[i].dt = -Sattelites[i].TauN + Sattelites[i].GammaN * Sattelites[i].tk;
						}

					}

					if(Ephemeris == PRECISE)
					{

						Sattelites[i].x = Neville(InterpolationPoints[i].x,
												  InterpolationPoints[i].toc,
												  Sattelites[i].tk,
												  INTERPOLATION_ORDER + 1);
						Sattelites[i].y = Neville(InterpolationPoints[i].y,
												  InterpolationPoints[i].toc,
												  Sattelites[i].tk,
												  INTERPOLATION_ORDER + 1);
						Sattelites[i].z = Neville(InterpolationPoints[i].z,
												  InterpolationPoints[i].toc,
												  Sattelites[i].tk,
												  INTERPOLATION_ORDER + 1);
						Sattelites[i].dt = Linear(InterpolationPoints[i].dt,
												   InterpolationPoints[i].toc,
												   Sattelites[i].tk,
												   INTERPOLATION_ORDER + 1);
						if(!Settings.SimpleVelocity)
						{
							Sattelites[i].vx = Neville(InterpolationPoints[i].vx,
													  InterpolationPoints[i].toc,
													  Sattelites[i].tk,
													  INTERPOLATION_ORDER + 1);
							Sattelites[i].vy = Neville(InterpolationPoints[i].vy,
													  InterpolationPoints[i].toc,
													  Sattelites[i].tk,
													  INTERPOLATION_ORDER + 1);
							Sattelites[i].vz = Neville(InterpolationPoints[i].vz,
													  InterpolationPoints[i].toc,
													  Sattelites[i].tk,
													  INTERPOLATION_ORDER + 1);
						}
					}

					if(Settings.SimpleVelocity)
					{
						if(Ephemeris == BOARD)
						{
							//Фиксация текущих координат
							Sattelites[i].vx0 = Sattelites[i].x;
							Sattelites[i].vy0 = Sattelites[i].y;
							Sattelites[i].vz0 = Sattelites[i].z;
							//Расчёт предыдущихкоординат
							Sattelites[i].tk -= Settings.Step;
							if(Settings.Sattelites[i][0] == 'G' ||
							   Settings.Sattelites[i][0] == 'E' ||
							   Settings.Sattelites[i][0] == 'C')
							{
								GPSSatteliteXV(&Sattelites[i], 0);
							}

							if(Settings.Sattelites[i][0] == 'R')
							{
								WGS84ToPZ90_02(Sattelites[i].x0, Sattelites[i].y0, Sattelites[i].z0,
												  &Sattelites[i].x, &Sattelites[i].y, &Sattelites[i].z);
								GLOSatteliteXV(&Sattelites[i]);
								PZ90_02ToWGS84(Sattelites[i].xi, Sattelites[i].yi, Sattelites[i].zi,
											  &Sattelites[i].x, &Sattelites[i].y, &Sattelites[i].z);
							}
							Sattelites[i].x0 = Sattelites[i].x;
							Sattelites[i].y0 = Sattelites[i].y;
							Sattelites[i].z0 = Sattelites[i].z;
							//Расчёт следующих координат
							Sattelites[i].tk += 2.0 * Settings.Step;
							if(Settings.Sattelites[i][0] == 'G' ||
							   Settings.Sattelites[i][0] == 'E' ||
							   Settings.Sattelites[i][0] == 'C')
							{
								GPSSatteliteXV(&Sattelites[i], 0);
							}

							if(Settings.Sattelites[i][0] == 'R')
							{
								WGS84ToPZ90_02(Sattelites[i].x0, Sattelites[i].y0, Sattelites[i].z0,
												  &Sattelites[i].x, &Sattelites[i].y, &Sattelites[i].z);
								GLOSatteliteXV(&Sattelites[i]);
								PZ90_02ToWGS84(Sattelites[i].xi, Sattelites[i].yi, Sattelites[i].zi,
											  &Sattelites[i].x, &Sattelites[i].y, &Sattelites[i].z);
							}
							Sattelites[i].xi = Sattelites[i].x;
							Sattelites[i].yi = Sattelites[i].y;
							Sattelites[i].zi = Sattelites[i].z;
							Sattelites[i].x = Sattelites[i].vx0;
							Sattelites[i].y = Sattelites[i].vy0;
							Sattelites[i].z = Sattelites[i].vz0;
							Sattelites[i].tk -= Settings.Step;
						}

						if(Ephemeris == PRECISE)
						{
							Sattelites[i].tk -= Settings.Step;
							Sattelites[i].x0 =
							Neville(InterpolationPoints[i].x,
									InterpolationPoints[i].toc,
									Sattelites[i].tk,
									INTERPOLATION_ORDER + 1);
							Sattelites[i].y0 =
							Neville(InterpolationPoints[i].y,
									InterpolationPoints[i].toc,
									Sattelites[i].tk,
									INTERPOLATION_ORDER + 1);
							Sattelites[i].z0 =
							Neville(InterpolationPoints[i].z,
									InterpolationPoints[i].toc,
									Sattelites[i].tk,
									INTERPOLATION_ORDER + 1);
							Sattelites[i].tk += 2.0 * Settings.Step;
							Sattelites[i].xi =
							Neville(InterpolationPoints[i].x,
									InterpolationPoints[i].toc,
									Sattelites[i].tk,
									INTERPOLATION_ORDER + 1);
							Sattelites[i].yi =
							Neville(InterpolationPoints[i].y,
									InterpolationPoints[i].toc,
									Sattelites[i].tk,
									INTERPOLATION_ORDER + 1);
							Sattelites[i].zi =
							Neville(InterpolationPoints[i].z,
									InterpolationPoints[i].toc,
									Sattelites[i].tk,
									INTERPOLATION_ORDER + 1);
							Sattelites[i].tk -= Settings.Step;
						}
						Sattelites[i].vx = (Sattelites[i].xi - Sattelites[i].x0) / (2.0 * Settings.Step);
						Sattelites[i].vy = (Sattelites[i].yi - Sattelites[i].y0) / (2.0 * Settings.Step);
						Sattelites[i].vz = (Sattelites[i].zi - Sattelites[i].z0) / (2.0 * Settings.Step);
					}

					if(Settings.x || Settings.y || Settings.z)
					{
						for(k = 0; k < 2; k++)
						{
							Sattelites[i].rho = sqrt(sqr(Settings.x - Sattelites[i].x) + sqr(Settings.y - Sattelites[i].y) + sqr(Settings.z - Sattelites[i].z));
							Sattelites[i].S = (Sattelites[i].x * Settings.y - Sattelites[i].y * Settings.x) * OMEGAi_WGS84 / c;
							tau = (Sattelites[i].rho + Sattelites[i].S) / c;
							theta = -OMEGAi_WGS84 * tau;
							Sattelites[i].xi = Sattelites[i].x * cos(theta) - Sattelites[i].y * sin(theta);
							Sattelites[i].yi = Sattelites[i].x * sin(theta) + Sattelites[i].y * cos(theta);
							Sattelites[i].x = Sattelites[i].xi;
							Sattelites[i].y = Sattelites[i].yi;
							if(Sattelites[i].vx || Sattelites[i].vy || Sattelites[i].vz)
							{
								Sattelites[i].xi = Sattelites[i].vx * cos(theta) - Sattelites[i].vy * sin(theta) + OMEGAi_WGS84 * Sattelites[i].y;
								Sattelites[i].yi = Sattelites[i].vx * sin(theta) + Sattelites[i].vy * cos(theta) - OMEGAi_WGS84 * Sattelites[i].x;
								Sattelites[i].vx = Sattelites[i].xi;
								Sattelites[i].vy = Sattelites[i].yi;
							}
						}
					}

					if(Ephemeris == BOARD)
					{
						Sattelites[i].tk = CurrentTime - tau - Sattelites[i].dt - Sattelites[i].toc;
					}

					if(Ephemeris == PRECISE)
					{
						Sattelites[i].tk = CurrentTime - tau - Sattelites[i].dt;
					}
				}
				UNIXTimeToDateTime(CurrentTime, &Year, &Month, &Day,
								   &Hours, &Minutes, &Seconds);
				if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
				{
					printf("\n\n %02d.%02d.%4d %02d:%02d:%04.1f %c%-2d", Day, Month, Year, Hours, Minutes, Seconds, Sattelites[i].Number[0], Sattelites[i].Number[1]);
					printf(" %13.3lf %13.3lf %13.3lf %10.5lf", Sattelites[i].x, Sattelites[i].y, Sattelites[i].z, Sattelites[i].dt * 1.0E+6);
					if(Settings.Geodethic)
					{
						DecartToGeo(Sattelites[i].x, Sattelites[i].y, Sattelites[i].z,
									&Sattelites[i].B, &Sattelites[i].L, &Sattelites[i].H);
						printf("\n                           %13.8lf %13.8lf %13.3lf", Sattelites[i].B, Sattelites[i].L, Sattelites[i].H);
					}
					printf("\n                           %13.3lf %13.3lf %13.3lf", Sattelites[i].vx, Sattelites[i].vy, Sattelites[i].vz);
				}

				if(Settings.OutputType == TO_FILE || Settings.OutputType == TO_SCREEN_FILE)
				{
					fprintf(outputfile, "\n %02d.%02d.%4d %02d:%02d:%04.1f %c%-2d", Day, Month, Year, Hours, Minutes, Seconds, Sattelites[i].Number[0], Sattelites[i].Number[1]);
					fprintf(outputfile, " %13.3lf %13.3lf %13.3lf %10.5lf", Sattelites[i].x, Sattelites[i].y, Sattelites[i].z, Sattelites[i].dt * 1.0E+6);
					fprintf(outputfile, " %13.3lf %13.3lf %13.3lf", Sattelites[i].vx, Sattelites[i].vy, Sattelites[i].vz);
					if(Settings.Geodethic)
					{
						DecartToGeo(Sattelites[i].x, Sattelites[i].y, Sattelites[i].z,
									&Sattelites[i].B, &Sattelites[i].L, &Sattelites[i].H);
						fprintf(outputfile, " %13.8lf %13.8lf %12.3lf", Sattelites[i].B, Sattelites[i].L, Sattelites[i].H);
					}
				}

				if(Settings.x || Settings.y || Settings.z)
				{
					Sattelites[i].Az = Azimuth(Settings.x, Settings.y, Settings.z, Sattelites[i].x, Sattelites[i].y, Sattelites[i].z);
					Sattelites[i].El = Elevation(Settings.x, Settings.y, Settings.z, Sattelites[i].x, Sattelites[i].y, Sattelites[i].z);
					if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
					{
						printf("\n                            %12.3lf  %12.3lf  %12.3lf", Sattelites[i].Az * RadToDeg, Sattelites[i].El * RadToDeg, Sattelites[i].rho);
					}

					if(Settings.OutputType == TO_FILE || Settings.OutputType == TO_SCREEN_FILE)
					{
						fprintf(outputfile, " %12.3lf  %12.3lf  %12.3lf", Sattelites[i].Az * RadToDeg, Sattelites[i].El * RadToDeg, Sattelites[i].rho);
					}
				}
			}
		}
		CurrentTime += Settings.Step;
		if(kbhit() && getch() == 27)
		{
			CurrentTime = Settings.FinalTime + 1;
		}
	}
	fclose(outputfile);
	free(Sattelites);
	free(InterpolationPoints);
	free(RINEXNav->GPSEphemeris);
	free(RINEXNav->GLOEphemeris);
	free(RINEXNav->GALEphemeris);
	free(RINEXNav->BDSEphemeris);
	free(RINEXNav);
	free(SP3->GPSEphemeris);
	free(SP3->GLOEphemeris);
	free(SP3->GALEphemeris);
	free(SP3->BDSEphemeris);
	free(SP3);
    for(i = 0; i < Settings.NumberOfSattelites; i++)
	{
		free(Settings.Sattelites[i]);
	}
	free(Settings.Sattelites);
	if(Settings.OutputType == TO_SCREEN || Settings.OutputType == TO_SCREEN_FILE)
	{
		printf("\nPress any key to exit...");
		getch();
	}
    return 0;
}


