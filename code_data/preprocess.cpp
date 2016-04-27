// preprocess.cpp : �w�q�D���x���ε{�����i�J�I�C
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <OpenCL/opencl.h>

#define MAX_DATA	3000
#define MAX_VECTOR	30000
#define MAX_LINE	3000000
#define N_FEATURE	1004
#define TEMP_FILE	"temp.txt"
#define T			10
#define PI			3.14159265

float vector[MAX_DATA][MAX_VECTOR];
int item_num[MAX_DATA];
float line_vector[MAX_VECTOR];
char line[MAX_LINE];
char str[MAX_LINE];
int n_data;
//double X[MAX_VECTOR], Y[MAX_VECTOR], Z[MAX_VECTOR];
int gmax =-99999, gmin = 99999;
int label;


/* OpenCL Zone */
	int err;                            // error code returned from api calls

    unsigned int correct;               // number of correct results returned
 
    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation
 
    cl_device_id device_id;             // compute device id 
    cl_context context;                 // compute context
    cl_command_queue commands;          // compute command queue
    cl_program program;                 // compute program
    cl_kernel kernel;                   // compute kernel

int GetVectors()
{
	int i = 0;
	char * pch;
	pch = strtok (line," ,;");
	while (pch != NULL){
		line_vector[i++] = atof(pch);
		pch = strtok (NULL, " ,;");
		if(i >= MAX_VECTOR - 1)
			break;
	}
	label = line_vector[i-1];
	return i-1;
}

float SMA(float *vec, int j)
{
	int i;
	float sma = 0.0;
	X[j] = vec[j]; // x 
	Y[j] = vec[j+1]; // y
	Z[j] = vec[j+2]; // z
	for(i = 0; i <= j; i++){
		sma += X[i];
	}
	for(i = 0; i <= j; i++){
		sma += Y[i];
	}
	for(i = 0; i <= j; i++){
		sma += Z[i];
	}
	sma /= j+1;
	return sma;
}

float SMV(float *vec)
{
	float smv = sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]); // (x^2 + y^2 + z^2)^0.5
	return smv;
}

float TA(float *vec)
{
	float ta = asin(vec[1]/SMV(vec)) * 180.0 / PI; // arcsin(y/((x^2 + y^2 + z^2)^0.5))
	return ta;
}

void AddFeatures(int i, int j)
{
	double sma, smv, ta;
	
	sma = SMA(&line_vector[j], j/3);
	smv = SMV(&line_vector[j]);
	ta = TA(&line_vector[j]);
	vector[i][j] = sma;
	vector[i][j+1] = smv;
	vector[i][j+2] = ta;
}

void InitOpenCL(){
	// Connect to a compute device
    //
    int gpu = 1;
    err = clGetDeviceIDs(NULL, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to create a device group!\n");
        exit(1);
    }
  
    // Create a compute context 
    //
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context)
    {
        printf("Error: Failed to create a compute context!\n");
        exit(1);
    }
 
    // Create a command commands
    //
    commands = clCreateCommandQueue(context, device_id, 0, &err);
    if (!commands)
    {
        printf("Error: Failed to create a command commands!\n");
        exit(1);
    }
 
    // Create the compute program from the source buffer
    //
	const int MAXK = 10000;
	char KernelSource[MAXK];
	const char *KernelSourcePtr = KernelSource;
	FILE *fp = fopen("preprocess.cl", "r");
	size_t length = fread(KernelSource, 1, MAXK, fp);
	fclose(fp);
	KernelSource[length] = '\0';
	
    program = clCreateProgramWithSource(context, 1, &KernelSourcePtr, &length, &err);
    if (!program)
    {
        printf("Error: Failed to create compute program!\n");
        exit(1);
    }
    // Build the program executable
    //
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];
 
        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }
 
    // Create the compute kernel in the program we wish to run
    //
    kernel = clCreateKernel(program, "preprocess", &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }
}

void ComputeFeatures(int i, int n)
{
    
    cl_mem input;                       // device memory used for the input array
    cl_mem output;                      // device memory used for the output array
 
    // Create the input and output arrays in device memory for our calculation
    //
    input = clCreateBuffer(context,  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  sizeof(float) * MAX_VECTOR, line_vector, NULL);
    output = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float) * MAX_DATA * MAX_VECTOR, vector, NULL);
    if (!input || !output)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }    
   
    // Set the arguments to our compute kernel
    //
	const unsigned int MAXV = MAX_VECTOR; 
    err = 0;
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
    err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &MAXV);
	err |= clSetKernelArg(kernel, 3, sizeof(int), &i);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }
	
	size_t global[] = { (int)ceil((float)(n) / 3) };
	size_t local[] = {1};
	
	// Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    //
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, global, local, 0, NULL, NULL);
    if (err)
    {
        printf("Error: Failed to execute kernel!\n");
        exit(1);
    }
 
    // Wait for the command commands to get serviced before reading back results
    //
    clFinish(commands);
}

void Label(const char *str, int i, int j)
{
	char temp[100];
	strcpy(temp, str);
	temp[strlen(temp) - 6] = '\0';
	if(strcmp(&(temp[strlen(temp) - 4]), "fall") == 0)
		vector[i][j] = 1; // fall activity
	else
		vector[i][j] = 0; // not fall activity
}

void SaveFeature(const char *filename)
{
	int i, j;
	FILE *fp;
	fp = fopen(filename, "wt");
	for(i = 0; i < n_data; i++){
		for(j = 0; j < item_num[i] - 1 && j < N_FEATURE; j++)
			fprintf(fp, "%.6lf ", vector[i][j]);
		fprintf(fp, ";%d\n", (int)vector[i][item_num[i] - 1]);
	}
	fclose(fp);
}

int FeatureExtract(const char *filename)
{
	int n;
	FILE *fp;
	fp = fopen(filename, "rt");
	if(fp == NULL)
		return 0;
	while(1){
		fgets(line, MAX_LINE, fp);
		if(feof(fp))
			break;
		n = GetVectors();
		ComputeFeatures(n_data, n);
		vector[n_data][n] = label;
		//Label(filename, n_data, n);
		item_num[n_data] = n+1;
		n_data++;
	}
	fclose(fp);
	
	SaveFeature(TEMP_FILE);
	return n_data;
}

void CopyFile(const char *src, const char *dest)
{
	char str[1000];
	sprintf(str, "cp %s %s\n", src, dest);
	system(str);
}

void OutputLog(const char *file, int line)
{
	int i;
	double max = -99999.0, min = 99999.0;
	FILE *fp;
	fp = fopen("log.txt", "at");
	printf("%s: %d instances, ", file, line);
	fprintf(fp, "%s: %d instances, ", file, line);
	for(i = 0; i < n_data; i++){
		max = (max < item_num[i]) ? item_num[i] : max;
		min = (min > item_num[i]) ? item_num[i] : min;
		item_num[i] = 0;
	}
	printf("max = %lf, min = %lf\n", max, min);
	fprintf(fp, "max = %lf, min = %lf\n", max, min);
	fclose(fp);
	gmax = (gmax < max) ? max : gmax;
	gmin = (gmin > min) ? min : gmin;
}

void OutputMaxMin(const char *file)
{
	FILE *fp;
	fp = fopen("log.txt", "at");
	printf("global max = %d, global min = %d\n", gmax, gmin);
	fprintf(fp, "global max = %d, global min = %d\n", gmax, gmin);
	fclose(fp);
}

int main(int argc, char* argv[])
{
	
	InitOpenCL();
	/*if(argc <= 1){
		printf("preprocess [data_file_name]\n");
		return 0;
	}*/
	
	//FeatureExtract((const char *)argv[1]);
	char infile[100], outfile[100];
	int i, n;

	/*sprintf(infile, "data\\adl02.txt");
	sprintf(outfile, "features\\adl02.txt");
	if(n = FeatureExtract(infile)){
		CopyFile(TEMP_FILE, outfile);
		OutputLog(outfile, n);
	}
	return 0;*/
	
	
	for(i = 0; i < 11; i++){
		n_data = 0;
		sprintf(infile, "data/fall%02d.txt", i+1);
		sprintf(outfile, "features/fall%02d.txt", i+1);
		
		if(n = FeatureExtract(infile)){
			CopyFile(TEMP_FILE, outfile);
			OutputLog(outfile, n);
		}
		n_data = 0;
		sprintf(infile, "data/adl%02d.txt", i+1);
		sprintf(outfile, "features/adl%02d.txt", i+1);
		if(n = FeatureExtract(infile)){
			CopyFile(TEMP_FILE, outfile);
			OutputLog(outfile, n);
		}
	}
	OutputMaxMin("log.txt");

	return 0;
}

