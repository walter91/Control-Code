#include "TGFF.h"

void TGFF(struct ParamsTGFF *params, struct SigsTGFF *sigs, double dt)
{

	sigs->cmd = saturate(sigs->cmd, params->CmdLim_lb, params->CmdLim_ub);

	sigs->delta = sigs->cmd - sigs->pos[1];
	sigs->posEff = params->K * sigs->delta;
	sigs->velEff = params->B * sigs->rate[1];
	sigs->effTot = sigs->posEff - sigs->velEff;

	// Update acceleration
	sigs->accel[0] = sigs->effTot / params->J;

	// Update rate
	//integrate(sig, intSig, dt)
    integrate(sigs->accel, sigs->rate, dt);

	// Update position (output 1)
	//integrate(sig, intSig, dt)
	integrate(sigs->rate, sigs->pos, dt);
	
		
    // Update FF Current (output 2)
	double fricTorq = cvFriction(sigs->rate[0], params->Coul_Nm, params->Visc_Nmsprad);
	sigs->curr = params->Gain * ((sigs->effTot + fricTorq) / params->MomArm / params->Kf / 2.0);

	// Update Memory
	fifo2(sigs->accel, sigs->accel[0]);
	fifo2(sigs->rate, sigs->rate[0]);
	fifo2(sigs->pos, sigs->pos[0]);

}