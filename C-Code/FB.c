#include "FB.h"

void FB(struct ParamsFB *params, struct SigsFB *sigs, double dt)
{
	// Update error array
	fifo2(sigs->error, sigs->cmd - sigs->fb);

	// Integrate error
	integrate(sigs->error, sigs->intError, dt);

	// Differentiate error
	sigs->difError = differentiate(sigs->error, dt);

	// Sum up the effort... and add the FeedForward...
	sigs->eff = (params->Kp * sigs->error[0]) + (params->Kd * sigs->difError) + (params->Ki * sigs->intError[0]) + sigs->ff;

	// Update Memory
	fifo2(sigs->intError, sigs->intError[0]);
	fifo2(sigs->error, sigs->error[0]);
}