/* 
 *Trajectory Generator and FeedForward Module...
*/

#define S_FUNCTION_NAME cfun_TGFF
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"

#include "TGFF.h"

#define NPARAMS 11

// Define TrajectoryGeneratorFeedForward Parameters
struct ParamsTGFF ax1_paramsTGFF;
struct SigsTGFF ax1_sigsTGFF;

double SIM_dt;

/*====================*
 * S-function methods *
 *====================*/
 
/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S){
    ssSetNumSFcnParams(S, NPARAMS);   /* Number of S-function prameters */
    ssSetNumContStates(S, 0);   /* Number of continuous states */
    ssSetNumDiscStates(S, 0);   /* Number of discrete states   */
    
    /* Initialize one input port with direct feedthrough */
    // Define 2 Input Ports
    if (!ssSetNumInputPorts(S, 1)) return;
    // For Each Port, set the width
    ssSetInputPortWidth(S, 0, 1);
    
    // For each port, set direct feedthrough
    ssSetInputPortDirectFeedThrough(S, 0, 1);
        
    /* Initialize one output port */
    if (!ssSetNumOutputPorts(S,2)) return;
    ssSetOutputPortWidth(S, 0, 1);
    ssSetOutputPortWidth(S, 1, 1);

    /* Initialize one sample time */
    ssSetNumSampleTimes(S, 1);   
    
    SIM_dt = ( mxGetPr(ssGetSFcnParam(S,0)) )[0];
      
    ax1_paramsTGFF.K      = ( mxGetPr(ssGetSFcnParam(S,1)) )[0];
    ax1_paramsTGFF.B      = ( mxGetPr(ssGetSFcnParam(S,2)) )[0];
    ax1_paramsTGFF.J      = ( mxGetPr(ssGetSFcnParam(S,3)) )[0];
    ax1_paramsTGFF.MomArm = ( mxGetPr(ssGetSFcnParam(S,4)) )[0];
    ax1_paramsTGFF.Kf     = ( mxGetPr(ssGetSFcnParam(S,5)) )[0];
    ax1_paramsTGFF.Gain   = ( mxGetPr(ssGetSFcnParam(S,6)) )[0];
    ax1_paramsTGFF.CmdLim_ub     = ( mxGetPr(ssGetSFcnParam(S,7)) )[0];
    ax1_paramsTGFF.CmdLim_lb   = ( mxGetPr(ssGetSFcnParam(S,8)) )[0];
    ax1_paramsTGFF.Coul_Nm     = ( mxGetPr(ssGetSFcnParam(S,9)) )[0];
    ax1_paramsTGFF.Visc_Nmsprad   = ( mxGetPr(ssGetSFcnParam(S,10)) )[0];
		
	ax1_sigsTGFF.pos[0] = 0.0;
	ax1_sigsTGFF.pos[1] = 0.0;

    ax1_sigsTGFF.rate[0] = 0.0;
	ax1_sigsTGFF.rate[1] = 0.0;

    ax1_sigsTGFF.accel[0] = 0.0;
	ax1_sigsTGFF.accel[1] = 0.0;
    
	ax1_sigsTGFF.curr = 0.0;
	ax1_sigsTGFF.delta = 0.0;
	ax1_sigsTGFF.posEff = 0.0;
	ax1_sigsTGFF.velEff = 0.0;
	ax1_sigsTGFF.effTot = 0.0;

}

/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S){
    ssSetSampleTime(S, 0, SIM_dt);
    ssSetOffsetTime(S, 0, 0);
}

/* Function: mdlOutputs =======================================================
 * Abstract:
 *    Calls the doubleIt.c function to multiple the input by 2.
 */
static void mdlOutputs(SimStruct *S, int tid){
    InputRealPtrsType cmd       = ssGetInputPortRealSignalPtrs(S,0);
    real_T            *cmdTG    = ssGetOutputPortRealSignal(S,0);
	real_T            *currFF   = ssGetOutputPortRealSignal(S, 1);

    // Update TGFF signals
    ax1_sigsTGFF.cmd = *cmd[0];
    
    // Run TGFF Algorithm
    TGFF(&ax1_paramsTGFF, &ax1_sigsTGFF, SIM_dt);
    
    // Update the output
    *cmdTG = ax1_sigsTGFF.pos[0];
	*currFF = ax1_sigsTGFF.curr; 

}

/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S){
    UNUSED_ARG(S); /* unused input argument */
    
}

#ifdef	MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
