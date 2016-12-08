#include <cmath>
#include <random>
#include <cfloat>
#include <fstream>
#include <iostream>

//computes integral by control variates method
double controlVariates(int numberOfSamples){
	//known integral
	double I_g = 0.8;

	//sum over all f(x); computed to <f(x)> in return statement
	double sum = 0;
	for(int i=0; i<numberOfSamples; i++){
		//get random number in x e [0,1], compute f(x)-g(x) and add to sum
		double x = double(rand())/RAND_MAX;
		sum += exp(-pow(x,2))+0.6*pow(x,2)-1;
	}

	//return integral
	return sum/numberOfSamples+I_g;
}

int main(){
	srand(time(NULL));

	//new file
	std::ofstream file;
	file.open("values_controlVariates.txt");

	int runsForVariance = 100;

	//loop over numberOfSamples
	for(int i=0; i<6; i++){
		int numberOfSamples = pow(10,i);

		//initilaize mean, min and max value
		double controlVariates_mean = 0;
		double controlVariates_min = DBL_MAX;
		double controlVariates_max = -DBL_MAX;

		//loop over number of runs for variance
		for(int j=0; j<runsForVariance; j++){
			//compute integral by control variates (cv) method
			double temp = controlVariates(numberOfSamples);

			//add to mean of cv method (divided by runsForVariance for right result)
			controlVariates_mean += temp;

			//compare with min and max value of hit-or-miss
			if(temp<controlVariates_min){
				controlVariates_min = temp;
			}
			if(temp>controlVariates_max){
				controlVariates_max = temp;
			}
		}

		//print and write into file
		//in order: numberOfSamples - computed integral value - min - max - exact value
		std::cout << numberOfSamples << "\t" << controlVariates_mean/runsForVariance << "\t" << controlVariates_max-controlVariates_min << "\t" << "0.746824" << std::endl;
		file << numberOfSamples << "\t" << controlVariates_mean/runsForVariance << "\t" << controlVariates_min << "\t" << controlVariates_max << std::endl;
	}
	file.close();
}
