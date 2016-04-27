// preprocess.cpp : �w�q�D���x���ε{�����i�J�I�C
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_DATA	3000
#define MAX_VECTOR	30000
#define MAX_LINE	3000000
#define N_FEATURE	1004
#define TEMP_FILE	"temp.txt"
#define T			10
#define PI			3.14159265

double vector[MAX_DATA][MAX_VECTOR];
int item_num[MAX_DATA];
double line_vector[MAX_VECTOR];
char line[MAX_LINE];
char str[MAX_LINE];
int n_data;
double X[MAX_VECTOR], Y[MAX_VECTOR], Z[MAX_VECTOR];
int gmax =-99999, gmin = 99999;
int label;

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

double SMA(double *vec, int j)
{
	int i;
	double sma = 0.0;
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

double SMV(double *vec)
{
	double smv = sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]); // (x^2 + y^2 + z^2)^0.5
	return smv;
}

double TA(double *vec)
{
	double ta = asin(vec[1]/SMV(vec)) * 180.0 / PI; // arcsin(y/((x^2 + y^2 + z^2)^0.5))
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

void ComputeFeatures(int i, int n)
{
	#pragma omp parallel for
	for(int j = 0; j < n; j += 3){
		AddFeatures(i, j);
	}
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
		sprintf(outfile, "features_origin/fall%02d.txt", i+1);
		if(n = FeatureExtract(infile)){
			CopyFile(TEMP_FILE, outfile);
			OutputLog(outfile, n);
		}
		n_data = 0;
		sprintf(infile, "data/adl%02d.txt", i+1);
		sprintf(outfile, "features_origin/adl%02d.txt", i+1);
		if(n = FeatureExtract(infile)){
			CopyFile(TEMP_FILE, outfile);
			OutputLog(outfile, n);
		}
	}
	OutputMaxMin("log.txt");

	return 0;
}

