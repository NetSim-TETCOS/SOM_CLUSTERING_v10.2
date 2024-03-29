/*
 *
 *	This is a simple program that illustrates how to call the MATLAB
 *	Engine functions from NetSim C Code.
 *
 */
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"
#include "mat.h"
#include "mex.h"
#include "direct.h"

char buf[100];
Engine *ep;
int status;
mxArray *x=NULL,*IDX=NULL, *C=NULL, *A=NULL, *B=NULL, *p=NULL;
mxArray *out;
double *result[3];

double fn_netsim_matlab_init()
{
	/*
	 * Start the MATLAB engine 
	 */
	if (!(ep = engOpen(NULL))) {
		MessageBox ((HWND)NULL, (LPCWSTR)"Can't start MATLAB engine", 
			(LPCWSTR) "MATLAB_Interface.c", MB_OK);
		exit(-1);
	}
	engEvalString(ep,"desktop");
//	sprintf(buf, "cd 'C:\\Users\\tetcos\\Desktop\\Final_Project\\SOM CLUSTERING\\64bit\\Code\\DSR'");
	sprintf(buf,"fid = fopen('log.txt','w+'); fclose(fid);");
	status=engEvalString(ep,buf);
	return 0;
}

double **fn_netsim_matlab_run(char *positions,int s_count,int n_cls,char *power)
{
	
	//specify the path of the DSR folder where the clustering.m file is present
	engPutVariable(ep,"x",x);
	status=engEvalString(ep,positions);

	engPutVariable(ep,"p",p);
	status=engEvalString(ep,power);

	sprintf(buf,"[A,B,C]=clustering(x,%d,%d,p)",s_count,n_cls);
	status=engEvalString(ep,buf);

	out=engGetVariable(ep,"A");//contains the cluster head id's of each cluster
	result[0]=mxGetPr(out);

	out=engGetVariable(ep,"B");//contains the cluster id's of each sensor
	result[1]=mxGetPr(out);

	out=engGetVariable(ep,"C");//contains the size of each cluster
	result[2]=mxGetPr(out);
	
	return result;
}

double fn_netsim_matlab_finish()
{
	//Close the MATLAB Engine

	status=engEvalString(ep,"exit");
	return 0;
}

