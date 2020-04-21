#ifndef FB_H
#define FB_H

#include "CNTR_UTIL.h"

// FB Parameters
struct ParamsFB
{
	double Kp;
	double Ki;
	double Kd;
};

// FB Signals
struct SigsFB
{
	double cmd;
	double fb;
	double ff;
	double eff;
	double error[2];
	double intError[2];
	double difError;
};

void FB(struct ParamsFB *params, struct SigsFB *sigs, double dt);

#endif