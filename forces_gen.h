/*
 * forces_gen.h
 *
 *  Created on: 16 Nov 2021
 *      Author: jonathan
 */

#ifndef FORCES_GEN_H_
#define FORCES_GEN_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_DATA_POINTS 1E6

#define GRAVITY		-9.81

#define END_TIME	60.0
#define END_X		0.0
#define END_Y		0.0
#define END_Z		GRAVITY

#define N_AXES		3

#define T_DWELL		5.0

struct AccelDataStruct
{
	double *t;
	double *x;
	double *y;
	double *z;
	int nPoints;
};

typedef struct AccelDataStruct AccelData;

void generateFromRigDataFile(AccelData *accelData);
void loadFromDataFile(AccelData *accelData);

void generateFromScratch(AccelData *accelData);
void addSinusoidalRigAcceleration(AccelData *accelData, double dt, double period, double amplitude, int nPeriod);
void addConstantRigAcceleration(AccelData *accelData, double dt, double period, double amplitude);

void writeOutForcesFile(AccelData *accelData);

#endif /* FORCES_GEN_H_ */
