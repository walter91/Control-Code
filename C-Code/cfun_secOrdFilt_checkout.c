/* 
 *FeadBack Controller (PID)
*/

#define S_FUNCTION_NAME cfun_secOrdFilt_checkout
#define S_FUNCTION_LEVEL 2
#include "simstruc.h"

#include "CNTR_UTIL.h"

#define NPARAMS 7

// Define Parameters
struct TFParams params;

double SIM_dt;
double signal[3];
double output[2];
double memory[2];

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
    if (!ssSetNumOutputPorts(S,1)) return;
    ssSetOutputPortWidth(S, 0, 1);

    
    /* Initialize one sample time */
    ssSetNumSampleTimes(S, 1);   
    
    SIM_dt = ( mxGetPr(ssGetSFcnParam(S,0)) )[0];
    
    params.num[0] = ( mxGetPr(ssGetSFcnParam(S,1)) )[0];
    params.num[1] = ( mxGetPr(ssGetSFcnParam(S,2)) )[0];
    params.num[2] = ( mxGetPr(ssGetSFcnParam(S,3)) )[0];
    
    params.den[0] = ( mxGetPr(ssGetSFcnParam(S,4)) )[0];
    params.den[1] = ( mxGetPr(ssGetSFcnParam(S,5)) )[0];
    params.den[2] = ( mxGetPr(ssGetSFcnParam(S,6)) )[0];
    
    //params.K = ( mxGetPr(ssGetSFcnParam(S,7)) )[0];
    
    signal[0] = 0.0;
    signal[1] = 0.0;
    signal[2] = 0.0;
    
    output[0] = 0.0;
    output[1] = 0.0;

	memory[0] = 0.0;
	memory[1] = 0.0;
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
 //   InputRealPtrsType in = ssGetInputPortRealSignalPtrs(S,0);
 //   
 //   real_T            *out    = ssGetOutputPortRealSignal(S,0);
 //   
 //   // Update Values
 //   fifo(signal, *in[0], 3);
 //   
 //   // Run Algorithms
	//*out = secOrdTF(signal, output, &params);

	//fifo(output, *out, 2);

	InputRealPtrsType in = ssGetInputPortRealSignalPtrs(S, 0);

	real_T            *out = ssGetOutputPortRealSignal(S, 0);

	// Run Algorithms
	*out = secOrdTF(*in[0], memory, &params);
    
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
