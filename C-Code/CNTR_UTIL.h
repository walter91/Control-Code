#ifndef CNTR_UTIL_H
#define CNTR_UTIL_H

#include <math.h>

struct TFParams
{
	// Numerator and denominator must be discrete.
	// Use c2d() command in matlab and specify transform method and time step.
    double num[3];
    double den[3];
};


struct SSParams
{
	// These matrices must be discrete.
	// Use c2d() command in matlab and specify transform method and time step.
	double A[2][2];
	double B[2][1];
	double C[2][2];
	double D[2][1];
};


void integrate(double *signal, double *intSig, double dt);
void fifo2(double *arr, double newValue);
void fifo(double *arr, double newValue, int size);
double differentiate(double *signal, double dt);
double saturate(double signal, double lb, double ub);
double cvFriction(double velocity, double coul, double visc);
double sign(double val);
double secOrdTF(double in, double *mem, struct TFParams *params);
void secOrdSS(double *out, double in, double *state, struct SSParams *params);

#endif