/*
	© Затолокин Д.А., 2015-2020
*/

#include "DataFiles.h"
#include "Common.h"

#include <stdlib.h>
#include <math.h>
//#include <alloc.h>
#include <string.h>
#include <time.h>

void SetDefaultSettings(struct Settings *Settings)
{
    strcpy(Settings->EphemerisFile, "");
	strcpy(Settings->SattelitesString, "");
	Settings->InitialHours = 0;
	Settings->InitialMinutes = 0;
	Settings->InitialSeconds = 0.0;
	Settings->FinalHours = 23;
	Settings->FinalMinutes = 59;
	Settings->FinalSeconds = 30.0;
	Settings->Step = 900.0;
	Settings->SimpleVelocity = 0;
	Settings->GalileoDataType = 'I';
	Settings->x = 0.0;
	Settings->y = 0.0;
	Settings->z = 0.0;
    Settings->Geodethic = 1;
	Settings->OutputType = 1;
	strcpy(Settings->OutputPath, "Output");
}

int ReadSettings(char *SettingsFileName, struct Settings *Settings)
{
	char *Pch, CurrentString[BYTES_OF_STRING];
    FILE *SettingsFile = fopen(SettingsFileName, "r");
	if(!SettingsFile)
	{
		return 0;
	}

	while(!feof(SettingsFile))
	{
		fgets(CurrentString, BYTES_OF_STRING, SettingsFile);
		sscanf(CurrentString, "%[^=]=", Settings->SettingName);
		if(!strcmp(Settings->SettingName, "EphemerisFilename"))
		{
			if(!strcmp(CurrentString, "EphemerisFilename=\n"))
			{
				strcpy(Settings->EphemerisFile, "");
			}
			else
			{
				Pch = strtok(CurrentString, "=\n");
				Pch = strtok(NULL, "=\n");
				strcpy(Settings->EphemerisFile, Pch);
			}
		}

		if(!strcmp(Settings->SettingName, "Sattelites"))
		{
			if(!strcmp(CurrentString, "Sattelites=\n"))
			{
				strcpy(Settings->SattelitesString, "");
			}
			else
			{
				Pch = strtok(CurrentString, "=\n");
				Pch = strtok(NULL, "=\n");
				strcpy(Settings->SattelitesString, Pch);
			}
		}

		if(!strcmp(Settings->SettingName, "InitialTime"))
		{
			sscanf(CurrentString, "%*[^=]=%hhd %hhd %f", &Settings->InitialHours,
														 &Settings->InitialMinutes,
														 &Settings->InitialSeconds);
		}

		if(!strcmp(Settings->SettingName, "FinalTime"))
		{
			sscanf(CurrentString, "%*[^=]=%hhd %hhd %f", &Settings->FinalHours,
														 &Settings->FinalMinutes,
														 &Settings->FinalSeconds);
		}

		if(!strcmp(Settings->SettingName, "Step"))
		{
			sscanf(CurrentString, "%*[^=]=%f", &Settings->Step);
		}

		if(!strcmp(Settings->SettingName, "SimpleVelocity"))
		{
			sscanf(CurrentString, "%*[^=]=%hhd", &Settings->SimpleVelocity);
		}

		if(!strcmp(Settings->SettingName, "GalileoDataType"))
		{
			sscanf(CurrentString, "%*[^=]=%c", &Settings->GalileoDataType);
		}

		if(!strcmp(Settings->SettingName, "Position"))
		{
			sscanf(CurrentString, "%*[^=]=%lf %lf %lf", &Settings->x,
														&Settings->y,
														&Settings->z);
		}

		if(!strcmp(Settings->SettingName, "GeodethicCoordinates"))
		{
			sscanf(CurrentString, "%*[^=]=%hhd", &Settings->Geodethic);
		}

		if(!strcmp(Settings->SettingName, "OutputType"))
		{
			sscanf(CurrentString, "%*[^=]=%hhd", &Settings->OutputType);
		}

		if(!strcmp(Settings->SettingName, "OutputPath"))
		{
			if(!strcmp(CurrentString, "OutputPath=\n"))
			{
				strcpy(Settings->OutputPath, "");
			}
			else
			{
				Pch = strtok(CurrentString, "=\n");
				Pch = strtok(NULL, "=\n");
				if(Pch)
				{
					strcpy(Settings->OutputPath, Pch);
				}
			}
		}
	}
    fclose(SettingsFile);
    return 1;
}

int WriteSettings(char *SettingsFileName, struct Settings *Settings)
{
	FILE *SettingsFile = fopen(SettingsFileName, "w");
	if(!SettingsFile)
	{
		return 0;
	}
	fputs("[Settings]\n", SettingsFile);
	fprintf(SettingsFile, "EphemerisFilename=%s\n", Settings->EphemerisFile);
	fprintf(SettingsFile, "Sattelites=%s\n", Settings->SattelitesString);
	fprintf(SettingsFile, "InitialTime=%hhd %hhd %.1f\n", Settings->InitialHours, Settings->InitialMinutes, Settings->InitialSeconds);
	fprintf(SettingsFile, "FinalTime=%hhd %hhd %.1f\n", Settings->FinalHours, Settings->FinalMinutes, Settings->FinalSeconds);
	fprintf(SettingsFile, "Step=%.1f\n", Settings->Step);
    fprintf(SettingsFile, "SimpleVelocity=%hhd\n", Settings->SimpleVelocity);
	fprintf(SettingsFile, "GalileoDataType=%c\n", Settings->GalileoDataType);
	fprintf(SettingsFile, "Position=%.3lf %.3lf %.3lf\n", Settings->x, Settings->y, Settings->z);
	fprintf(SettingsFile, "GeodethicCoordinates=%hhd\n", Settings->Geodethic);
	fprintf(SettingsFile, "OutputType=%hhd\n", Settings->OutputType);
	fprintf(SettingsFile, "OutputPath=%s\n", Settings->OutputPath);
	fclose(SettingsFile);
    return 1;
}

void WriteOutputFileHeader(FILE *outputfile, struct Settings *Settings)
{
	fprintf(outputfile, " EphemerisFilename = %s\n", Settings->EphemerisFile);
	fprintf(outputfile, " Sattelites = %s\n", Settings->SattelitesString);
	fprintf(outputfile, " InitialTime = %hhd %hhd %.1f\n", Settings->InitialHours,
														   Settings->InitialMinutes,
														   Settings->InitialSeconds);
	fprintf(outputfile, " FinalTime = %hhd %hhd %.1f\n", Settings->FinalHours,
														 Settings->FinalMinutes,
														 Settings->FinalSeconds);
	fprintf(outputfile, " Step = %.1f\n", Settings->Step);
	fprintf(outputfile, " SimpleVelocity = %hhd\n", Settings->SimpleVelocity);
	fprintf(outputfile, " GalileoDataType = %c\n", Settings->GalileoDataType);
	if(Settings->x || Settings->y || Settings->z)
	{
		DecartToGeo(Settings->x, Settings->y, Settings->z, &Settings->B, &Settings->L, &Settings->H);
		fprintf(outputfile, "\n Receiver position:");
		fprintf(outputfile, "\n x0 = %12.3lf m  B0 = %13.8lf °", Settings->x, Settings->B);
		fprintf(outputfile, "\n y0 = %12.3lf m  L0 = %13.8lf °", Settings->y, Settings->L);
		fprintf(outputfile, "\n z0 = %12.3lf m  H0 = %13.3lf m\n", Settings->z, Settings->H);
	}
	fprintf(outputfile, "\n ------------------------------------------------------------------------------------------------------------------------");
	if(Settings->x || Settings->y || Settings->z)
	{
		fprintf(outputfile, "-----------------------------------------");
	}

	if(Settings->Geodethic)
	{
		fprintf(outputfile, "-----------------------------------------");
	}
	fprintf(outputfile, "\n    date       time     S       x(m)         y(m)         z(m)          dt(us)");
	fprintf(outputfile, "        vx(m/s)       vy(m/s)       vz(m/s)");
	if(Settings->Geodethic)
	{
		fprintf(outputfile, "        B(°)          L(°)       H(m)    ");
	}
	if(Settings->x || Settings->y || Settings->z)
	{
		fprintf(outputfile, "        A(°)          E(°)       r(m)");
	}
	fprintf(outputfile, "\n ------------------------------------------------------------------------------------------------------------------------");
	if(Settings->x || Settings->y || Settings->z)
	{
		fprintf(outputfile, "-----------------------------------------");
	}

	if(Settings->Geodethic)
	{
		fprintf(outputfile, "-----------------------------------------");
	}
}
