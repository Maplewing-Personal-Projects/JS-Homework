__kernel void preprocess(__global float* line_vector, __global float* vector, const unsigned int MAX_VECTOR, const int i){
	int j = get_global_id(0) * 3;
	
	int k;
	float sma = 0.0f;
	for(k = 0; k <= j; k++){
		sma += line_vector[k];
	}
	sma /= j/3+1;
	
	float smv = sqrt(line_vector[j]*line_vector[j]+line_vector[j+1]*line_vector[j+1]+line_vector[j+2]*line_vector[j+2]); // (x^2 + y^2 + z^2)^0.5
	float ta = asin(line_vector[j+1]/smv) * 180.0f / 3.14159265;
	
	vector[i*MAX_VECTOR+j] = sma;
	vector[i*MAX_VECTOR+j+1] = smv;
	vector[i*MAX_VECTOR+j+2] = ta;
}

