#include <cmath>
#include <random>
#include <cfloat>
#include <fstream>
#include <iostream>

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
		sum_f_x += exp(-pow(x,2));
	}

	//return integral
	return (b-a)*sum_f_x/numberOfSamples;
}

double importanceSampling(int numberOfSamples){
	//f(x)=e^(-x²)
	//g(x) = a*e^(-x)
	//normalization: a=1/(1-1/e)
	//G(x)=(1-1/e^x)/(1-1/e)
	//G(0)=0; G(1)=1 -> ye[0,1]
	//x=G^(-1)(y)=-ln[1-y(1-1/e)]
	//I=1/N*sum(f(x)/g(x))
	double sum_fg = 0;
	for(int i=0; i<numberOfSamples; i++){
		double y = double(rand())/RAND_MAX;
		double x = -log(1-y*(1-exp(-1)));
		sum_fg += exp(-pow(x,2))/(exp(-x)*1/(1-exp(-1)));
	}
	return sum_fg/numberOfSamples;
}

int main(){
	srand(time(NULL));

	//new file
	std::ofstream file_simple;
	file_simple.open("values_simple.txt");

	//same for hit-or-miss
	std::ofstream file_importance;
	file_importance.open("values_importance.txt");
	int runsForVariance = 100;

	//loop over numberOfSamples
	for(int i=0; i<6; i++){
		int numberOfSamples = pow(10,i);

		//initilaize mean, min and max value for both methods
		double simpleSampling_mean = 0;
		double simpleSampling_min = DBL_MAX;
		double simpleSampling_max = -DBL_MAX;

		double importanceSampling_mean = 0;
		double importanceSampling_min = DBL_MAX;
		double importanceSampling_max = -DBL_MAX;

		//loop over number of runs for variance
		for(int j=0; j<runsForVariance; j++){
			//compute integral by sampling method
			double simple = simpleSampling(numberOfSamples);

			//add to mean of sampling method (divided by runsForVariance for right result)
			simpleSampling_mean += simple;

			//compare with min and max value of hit-or-miss
			if(simple<simpleSampling_min){
				simpleSampling_min = simple;
			}
			if(simple>simpleSampling_max){
				simpleSampling_max = simple;
			}

			//compute integral by hir-or-miss-method
			double importance = importanceSampling(numberOfSamples);

			//add to mean of hit-or-miss-method (divided by runsForVariance for right result)
			importanceSampling_mean += importance;

			//compare with min and max value of hit-or-miss
			if(importance<importanceSampling_min){
				importanceSampling_min = importance;
			}
			if(importance>importanceSampling_max){
				importanceSampling_max = importance;
			}
		}

		//print and write into file
		//in order for sinple sampling: numberOfSamples - integral simple sampling method - ssimple ampling method max - simple sampling method min
		//in order for importance sampling: numberOfSamples - integral importance sampling - importance sampling min - importance sampling max
		std::cout << numberOfSamples << "\t" << simpleSampling_mean/runsForVariance << "\t" << simpleSampling_max-simpleSampling_min << "\t";
		file_simple << numberOfSamples << "\t" << simpleSampling_mean/runsForVariance << "\t" << simpleSampling_min << "\t" << simpleSampling_max << std::endl;
		std::cout << importanceSampling_mean/runsForVariance << "\t" << importanceSampling_max-importanceSampling_min << "\t" << "0.746824" << std::endl;
		file_importance << numberOfSamples << "\t" << importanceSampling_mean/runsForVariance << "\t" << importanceSampling_min << "\t" << importanceSampling_max << std::endl;
	}
	file_simple.close();
}
