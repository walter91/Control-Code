/* 
 *FeadBack Controller (PID)
*/

#define S_FUNCTION_NAME cfun_FB
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"

#include "FB.h"

#define NPARAMS 4

// Define Parameters
struct SigsFB ax1_sigsFB;
struct ParamsFB ax1_paramsFB;

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
    if (!ssSetNumInputPorts(S, 3)) return;
    // For Each Port, set the width
    ssSetInputPortWidth(S, 0, 1);
    ssSetInputPortWidth(S, 1, 1);
	ssSetInputPortWidth(S, 2, 1);
    
    // For each port, set direct feedthrough
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
	ssSetInputPortDirectFeedThrough(S, 2, 1);
        
    /* Initialize one output port */
    if (!ssSetNumOutputPorts(S,1)) return;
    ssSetOutputPortWidth(S, 0, 1);

    
    /* Initialize one sample time */
    ssSetNumSampleTimes(S, 1);   
    
    SIM_dt = ( mxGetPr(ssGetSFcnParam(S,0)) )[0];
    
    ax1_paramsFB.Kp  = ( mxGetPr(ssGetSFcnParam(S,1)) )[0];
    ax1_paramsFB.Ki  = ( mxGetPr(ssGetSFcnParam(S,2)) )[0];
    ax1_paramsFB.Kd  = ( mxGetPr(ssGetSFcnParam(S,3)) )[0];    
    
	ax1_sigsFB.cmd = 0.0;
	ax1_sigsFB.eff = 0.0;
	ax1_sigsFB.error[0] = 0.0;
	ax1_sigsFB.error[1] = 0.0;
	ax1_sigsFB.fb = 0.0;
	ax1_sigsFB.intError[0] = 0.0;
	ax1_sigsFB.intError[1] = 0.0;
	ax1_sigsFB.difError = 0.0;
	ax1_sigsFB.ff = 0.0;
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
    InputRealPtrsType cmd = ssGetInputPortRealSignalPtrs(S,0);
	InputRealPtrsType ff = ssGetInputPortRealSignalPtrs(S, 1);
    InputRealPtrsType fb = ssGetInputPortRealSignalPtrs(S,2);
    
    real_T            *curr    = ssGetOutputPortRealSignal(S,0);
        
    // Update FF, FB and CMD
	ax1_sigsFB.ff = *ff[0];
    ax1_sigsFB.fb = *fb[0];
    ax1_sigsFB.cmd = *cmd[0];
    
    // Run FB Algorithm
    FB(&ax1_paramsFB, &ax1_sigsFB, SIM_dt);
    
    // Update the output
    *curr = ax1_sigsFB.eff;

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
