#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <cmath>
#include <cfloat>

//computes integral by sampling method
double simpleSampling(int numberOfSamples){
	//lower and upper boundary of interval
	int a=0;
	int b=1;

	//sum over all f(x); computed to <f(x)> in return statement
	double sum_f_x = 0;
	for(int i=0; i<numberOfSamples; i++){
		//get random number in x e [0,1] and compute f(x)
		double x = double(rand())/RAND_MAX;
		sum_f_x += pow(cos(1./x),2);
	}

	//return integral
	return (b-a)*sum_f_x/numberOfSamples;
}

//computes integral by hit-or-miss-method
double hitOrMiss(int numberOfSamples){
	//lower and upper boundary of interval
	int a=0;
	int b=1;

	//maximum value of function in known integral; for better results equal to maximum value of f(x)
	double max = 1.;

	//value of known integral
	double I_known = (b-a)*max;

	//number of hits
	int hit = 0;
	for(int i=0; i<numberOfSamples; i++){
		//random number x e [0,1] for computing f(x)
		double x = double(rand())/RAND_MAX;

		//random number y e [0,max] for comparisuon
		double y = double(rand())*max/RAND_MAX;

		//compute f(x)
		double f_x = pow(cos(1./x),2.);

		//compare
		if(y<= f_x){
			hit++;
		}
	}

	//return integral as fraction of known integral
	return I_known*hit/numberOfSamples;
}

int main(){
	srand(time(NULL));

	//new file
	std::ofstream file_sample;
	file_sample.open("values_sample.txt");

	//same for hit-or-miss
	std::ofstream file_hOm;
	file_hOm.open("values_hOm.txt");
	int runsForVariance = 100;

	//loop over numberOfSamples
	for(int i=0; i<6; i++){
		int numberOfSamples = pow(10,i);

		//initilaize mean, min and max value for both methods
		double samplingMethod_mean = 0;
		double samplingMethod_min = DBL_MAX;
		double samplingMethod_max = -DBL_MAX;

		double hitOrMiss_mean = 0;
		double hitOrMiss_min = DBL_MAX;
		double hitOrMiss_max = -DBL_MAX;

		//loop over number of runs for variance
		for(int j=0; j<runsForVariance; j++){
			//compute integral by sampling method
			double samp = simpleSampling(numberOfSamples);

			//add to mean of sampling method (divided by runsForVariance for right result)
			samplingMethod_mean += samp;

			//compare with min and max value of hit-or-miss
			if(samp<samplingMethod_min){
				samplingMethod_min = samp;
			}
			if(samp>samplingMethod_max){
				samplingMethod_max = samp;
			}

			//compute integral by hir-or-miss-method
			double hOm = hitOrMiss(numberOfSamples);

			//add to mean of hit-or-miss-method (divided by runsForVariance for right result)
			hitOrMiss_mean += hOm;

			//compare with min and max value of hit-or-miss
			if(hOm<hitOrMiss_min){
				hitOrMiss_min = hOm;
			}
			if(hOm>hitOrMiss_max){
				hitOrMiss_max = hOm;
			}
		}

		//print and write into file
		//in order for sample: numberOfSamples - integral sampling method - sampling method max - sampling method min
		//in order for hit-or-miss: numberOfSamples - integral hit-or-miss - hit or miss min - hit or miss max
		std::cout << numberOfSamples << "\t" << samplingMethod_mean/runsForVariance << "\t" << samplingMethod_max-samplingMethod_min << "\t";
		file_sample << numberOfSamples << "\t" << samplingMethod_mean/runsForVariance << "\t" << samplingMethod_min << "\t" << samplingMethod_max << std::endl;
		std::cout << hitOrMiss_mean/runsForVariance << "\t" << hitOrMiss_max-hitOrMiss_min << "\t" << "0.326543" << std::endl;
		file_hOm << numberOfSamples << "\t" << hitOrMiss_mean/runsForVariance << "\t" << hitOrMiss_min << "\t" << hitOrMiss_max << std::endl;
	}
	file_sample.close();
}
