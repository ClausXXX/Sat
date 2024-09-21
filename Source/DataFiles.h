/*
	© Затолокин Д.А., 2015-2020
*/

#ifndef DATAFILES_H
#define DATAFILES_H

#include <stdio.h>
#include <dir.h>

#include "Constants.h"

struct Settings
{
	char EphemerisFile[MAXPATH],
		 SattelitesString[BYTES_OF_STRING],
		 SettingName[64],
		 OutputPath[MAXPATH],
         SimpleVelocity,
		 InitialHours, InitialMinutes, FinalHours, FinalMinutes,
         Geodethic,
		 OutputType,
		 NumberOfSattelites,
         GalileoDataType,
		 **Sattelites;
	float InitialSeconds, FinalSeconds, Step;
	double InitialTime, FinalTime, x, y, z, B, L, H;
};

void SetDefaultSettings(struct Settings*);
int ReadSettings(char*, struct Settings*);
int WriteSettings(char*, struct Settings*);
void WriteOutputFileHeader(FILE*, struct Settings*);

#endif
