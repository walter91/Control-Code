#ifndef TGFF_H
#define TGFF_H

#include "CNTR_UTIL.h"

// TGFF Parameters
struct ParamsTGFF 
{
	double K;
	double B;
	double J;
	double MomArm;
	double Kf;
	double Gain;
	double CmdLim_ub;
	double CmdLim_lb;
    double Coul_Nm;
    double Visc_Nmsprad;
};

// TGFF Signals
struct SigsTGFF 
{
    double cmd;
	double pos[2];
	double rate[2];
	double accel[2];
	double curr;
    double delta;
    double posEff;
    double velEff;
    double effTot;
};

void TGFF(struct ParamsTGFF *params, struct SigsTGFF *sigs, double dt);

#endif