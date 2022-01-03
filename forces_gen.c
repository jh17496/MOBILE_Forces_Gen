/*
 * forces_gen.c
 *
 *  Created on: 16 Nov 2021
 *      Author: jonathan
 */

#include "forces_gen.h"

void generateFromRigDataFile(AccelData *accelData)
{
	loadFromDataFile(accelData);
	writeOutForcesFile(accelData);
}

void loadFromDataFile(AccelData *accelData)
{
	FILE *fp;
	fp = fopen("AccelData.bin","rb");

	double dt;
	int triggerIndex, runStartIndex, rc=1;

	long int sizeof_int = sizeof(int);
	long int sizeof_double = sizeof(double);

	if (fp)
	{
		rc *= fread(&dt,sizeof_double,1,fp);
		rc *= fread(&accelData->nPoints,sizeof_int,1,fp);
		rc *= fread(&triggerIndex,sizeof_int,1,fp);

		double *tempArray = (double *)malloc(accelData->nPoints*N_AXES*sizeof(double));

		rc *= fread(tempArray,sizeof_double,accelData->nPoints*N_AXES,fp);


		if (0)
			runStartIndex = (int )(T_DWELL / dt);
		else
			runStartIndex = triggerIndex;

		accelData->nPoints -= runStartIndex;
		triggerIndex -= runStartIndex;

		printf("Timestep: %f, nPoints: %d, triggerIndex: %d \n",dt,accelData->nPoints,triggerIndex);

		if (rc == 0)
			printf("Read error\n");

		double xOffset=0.0, yOffset=0.0, zOffset=0.0;
		int count = 0;

		if (0)
		{
			for (int ii=0; ii<runStartIndex; ++ii)
			{
				if (ii>runStartIndex/10 && ii<(9*runStartIndex)/10)
				{
					xOffset += tempArray[N_AXES*ii];
					yOffset += tempArray[N_AXES*ii + 1];
					zOffset += tempArray[N_AXES*ii + 2];
					count++;
				}
			}

			xOffset /= (double )count;
			yOffset /= (double )count;
			zOffset /= (double )count;
		}

		for (int ii=0; ii<accelData->nPoints; ++ii)
		{
			accelData->t[ii] = ii*dt;
			accelData->x[ii] = xOffset - tempArray[runStartIndex*N_AXES + N_AXES*ii];
			accelData->y[ii] = yOffset - tempArray[runStartIndex*N_AXES + N_AXES*ii + 1];
			accelData->z[ii] = zOffset - tempArray[runStartIndex*N_AXES + N_AXES*ii + 2] + GRAVITY;
		}

		free(tempArray);

		fclose(fp);
	}
}

void generateFromScratch(AccelData *accelData)
{
	double dt = 0.001;
	double period, amplitude, nPeriod, sum = 0.0;

	period = 0.65;
	amplitude = 7.5;
	nPeriod = 10;
	addSinusoidalRigAcceleration(accelData, dt, period, amplitude, nPeriod);
	sum += period*nPeriod;

	period = 1.42;
	amplitude = 7.5;
	addConstantRigAcceleration(accelData, dt, period, amplitude);
	sum += period;

	period = 0.85;
	amplitude = -29.4;
	addConstantRigAcceleration(accelData, dt, period, amplitude);
	sum += period;

	period = 10.0 - sum;
	amplitude = 19.6;
	addConstantRigAcceleration(accelData, dt, period, amplitude);

	writeOutForcesFile(accelData);
}

void addSinusoidalRigAcceleration(AccelData *accelData, double dt, double period, double amplitude, int nPeriod)
{
	int nPointsAdded;

	nPointsAdded = (int )(period / dt)*nPeriod;

	for (int ii=0; ii<nPointsAdded; ++ii)
	{
		accelData->t[accelData->nPoints+ii] = dt*(accelData->nPoints+ii);
		accelData->x[accelData->nPoints+ii] = 0.0;
		accelData->y[accelData->nPoints+ii] = 0.0;
		accelData->z[accelData->nPoints+ii] = GRAVITY - amplitude*cos(ii*dt / period * 2*M_PI);
	}

	accelData->nPoints += nPointsAdded;
}

void addConstantRigAcceleration(AccelData *accelData, double dt, double period, double amplitude)
{
	int nPointsAdded;

	nPointsAdded = (int )(period / dt);

	for (int ii=0; ii<nPointsAdded; ++ii)
	{
		accelData->t[accelData->nPoints+ii] = dt*(accelData->nPoints+ii);
		accelData->x[accelData->nPoints+ii] = 0.0;
		accelData->y[accelData->nPoints+ii] = 0.0;
		accelData->z[accelData->nPoints+ii] = GRAVITY - amplitude;
	}

	accelData->nPoints += nPointsAdded;
}

void writeOutForcesFile(AccelData *accelData)
{
	FILE *fp;

	fp = fopen("input_forces.dat","w");

	if (fp)
	{
		// Header
		fprintf(fp,"nftimes ncols:\n");
		fprintf(fp,"%d 10\n",accelData->nPoints+1);
		fprintf(fp,"time cx cy cz ox oy oz gx gy gz:\n");

		for (int ii=0; ii<accelData->nPoints; ++ii)
			fprintf(fp,"%e %e %e %e %e %e %e %e %e %e\n",accelData->t[ii],0.0,0.0,0.0,0.0,0.0,0.0,accelData->x[ii],accelData->y[ii],accelData->z[ii]);

		fprintf(fp,"%e %e %e %e %e %e %e %e %e %e\n",END_TIME,0.0,0.0,0.0,0.0,0.0,0.0,END_X,END_Y,END_Z);

		fclose(fp);
	}
}
