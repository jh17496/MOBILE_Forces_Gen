/*
 * main.c
 *
 *  Created on: 16 Nov 2021
 *      Author: jonathan
 */

#include "forces_gen.h"

int main()
{
	AccelData *accelData;

	accelData = (AccelData *)malloc(1*sizeof(AccelData));
	accelData->t = (double *)malloc(MAX_DATA_POINTS*sizeof(double));
	accelData->x = (double *)malloc(MAX_DATA_POINTS*sizeof(double));
	accelData->y = (double *)malloc(MAX_DATA_POINTS*sizeof(double));
	accelData->z = (double *)malloc(MAX_DATA_POINTS*sizeof(double));

	accelData->nPoints = 0;

	generateFromRigDataFile(accelData);
	//generateFromScratch(accelData);

	free(accelData->t);
	free(accelData->x);
	free(accelData->y);
	free(accelData->z);
	free(accelData);

	return 0;
}
