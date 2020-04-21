#include "CNTR_UTIL.h"

void integrate(double signal[], double intSig[], double dt)
{
	// Trapazoidal Integrator
	// signal is an array of length 2
	//		indx 0 = current time data
	//		indx 1 = delayed 1 time data

	intSig[0] = intSig[1] + ((0.5)*(signal[0] + signal[1]))*dt;

}

void fifo2(double arr[], double newValue)
{
	// (F)irst (I)n (F)irst (O)ut buffer of length (2)
	// To be used with len2 arrays to shift the old data back and replace 
	// with new data.
	//		indx 0 = current time data
	//		indx 1 = delayed 1 time data

	arr[1] = arr[0];
	arr[0] = newValue;

	// Also Valid, but stlightly less clear...
	/*
	// move one index past the array pointer (index 1)
	arr++;

	// assign value of index 1 to the value of index 0
	*arr = *(arr - 1);

	// move back to index 0
	arr--;

	// assing value of index 0 to the newvalue
	*arr = newValue;
	*/
}


void fifo(double *arr, double newValue, int size)
{
	// (F)irst (I)n (F)irst (O)ut buffer

	for (int ii = (size - 1); ii != 0; ii--)
	{
		arr[ii] = arr[ii-1];
	}

	arr[0] = newValue;
}


double differentiate(double signal[], double dt)
{
	// Simple derivative...

	double derivative = (signal[0] - signal[1]) / dt;

	return derivative;
}


double saturate(double signal, double lb, double ub)
{   // Simple saturations function
	if (signal > ub)
	{
		return ub;
	}
	else if (signal < lb)
	{
		return lb;
	}
	else
	{
		return signal;
	}
}

double cvFriction(double velocity, double coul, double visc)
{
    // This is the basic coulomb + viscous friction model. Coulomb friction
    // is direction dependent and magnitude independent while viscous friction
    // depends on both the sign and magnitude of the velocity.
    //
    // All units must be compatible (rad/s, Nm, Nm/rad/s) are recommended...
    return (sign(velocity)*coul) + (velocity*visc);
}

double sign(double val)
{
    if (val > 0.0)
    {
        return 1.0;
    }
    else if (val < 0.0)
    {
        return -1.0;
    }
    else
    {
        return 0.0;
    }
}


double secOrdTF(double in, double *mem, struct TFParams *params)
{
	// Discrete 2nd Order Transfer Function
	//
	// This algorithm was adapted from 2018b Simulink Discrete Transfer function block through Autocoding...

	double temp = ((in - params->den[1] * mem[0]) - (params->den[2] * mem[1])) / params->den[0];
	
	double out = (params->num[0] * temp + params->num[1] * mem[0]) + (params->num[2] * mem[1]);

	fifo(mem, temp, 2);

	return out;
}

void secOrdSS_mat(double *out, double in, double *state, struct SSParams *params)
{
	// Discrete 2nd Order State Space Model
	//
	// This algorithm was adapted from 2018b Simulink Discrete State Space block through Autocoding...


	out[0] = params->C[0][0] * state[0] + params->C[0][1] * state[1] + params->D[0][0] * in;
	out[1] = params->C[1][0] * state[0] + params->C[1][1] * state[1] + params->D[0][1] * in;

	double temp[2];

	temp[0] = params->A[0][0] * state[0] + params->A[0][1] * state[1] + params->B[0][0] * in;
	temp[1] = params->A[1][0] * state[0] + params->A[1][1] * state[1] + params->B[0][1] * in;

	state[0] = temp[0];
	state[1] = temp[1];

}
