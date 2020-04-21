#define S_FUNCTION_NAME cfun_secOrdSS
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"

#include "CNTR_UTIL.h"

#define NPARAMS 13

// Define Parameters
struct SSParams params;

double SSout[2];
double SSstate[2];

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
    
    params.A[0][0]  = ( mxGetPr(ssGetSFcnParam(S,1)) )[0];
    params.A[0][1]  = ( mxGetPr(ssGetSFcnParam(S,2)) )[0];
    params.A[1][0]  = ( mxGetPr(ssGetSFcnParam(S,3)) )[0];
    params.A[1][1]  = ( mxGetPr(ssGetSFcnParam(S,4)) )[0];
    
    params.B[0][0]  = ( mxGetPr(ssGetSFcnParam(S,5)) )[0];
    params.B[1][0]  = ( mxGetPr(ssGetSFcnParam(S,6)) )[0];
    
    params.C[0][0]  = ( mxGetPr(ssGetSFcnParam(S,7)) )[0];
    params.C[0][1]  = ( mxGetPr(ssGetSFcnParam(S,8)) )[0];
    params.C[1][0]  = ( mxGetPr(ssGetSFcnParam(S,9)) )[0];
    params.C[1][1]  = ( mxGetPr(ssGetSFcnParam(S,10)) )[0];
    
    params.D[0][0]  = ( mxGetPr(ssGetSFcnParam(S,11)) )[0];
    params.D[1][0]  = ( mxGetPr(ssGetSFcnParam(S,12)) )[0];
    
    SSout[0] = 0.0;
    SSout[1] = 0.0;
    
    SSstate[0] = 0.0;
    SSstate[1] = 0.0;

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
    InputRealPtrsType in = ssGetInputPortRealSignalPtrs(S,0);
    
    real_T            *out0    = ssGetOutputPortRealSignal(S,0);
    real_T            *out1    = ssGetOutputPortRealSignal(S,1);
        
    // Run Algorithm
    //secOrdSS(SSout, *in[0], SSstate_d1, SSstateDot_d1, &params, SIM_dt);
	secOrdSS_mat(SSout, *in[0], SSstate, &params);
    
    // Update the output
    *out0 = SSout[0];
    *out1 = SSout[1];

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
