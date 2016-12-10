#include <random>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cfloat>

//computes the path length for the particle with angle a
double getPath(double &x, double &y, double a){
	double rand = std::rand();
	//compute F^(-1)(y) as calculated in "Mathematical Preparations"
	double r = -log(1-rand/RAND_MAX);
	//convert a from ° to rad
	a = a*M_PI/180;
	//compute new x- and y-coordinate
	x = cos(a)*r;
	y = sin(a)*r;
	//return path length
	return r;
}

//simulates if the particle becomes absorbed
bool becomesAbsorbed(){
	double random = rand();
	//particle is not absorbed with probability 0.8
	return 0.8<random/RAND_MAX;
}

//computes the angle for the scattering processs
double getAngle(){
	//variable "random" is for calculating the angle, variable "random2" for scattering in positive or negative y-direction
	double random = rand();
	int random2 = (int)(rand()%2);
	//return +/- angle as computed by G^(-1)(y) deviated in "Mathematical Preparations"
	if(random2%2==0){
		return 25*sqrt(-log(1-random/RAND_MAX));
	} else{
		return -25*sqrt(-log(1-random/RAND_MAX));
	}
}

//writes data into file
//data structure: 1st column - counts for this path length; 2nd column - counts for absorption;
//3rd column - counts for this scattering angle; 4th column - y-distribution for x=1-detector;
//5th column - y-distribution for x=5-detector
//file structure similar to data structure with 0th column: integer 0 -> 360
//-> is path length for 1st column [0,360] and scattering angle for 3rd column [-180°,180°]
void write(int N, double data[5][361]){
	std::ofstream file;
	file.open("stat"+std::to_string(N)+".txt");

	for(int i=0; i<361; i++){
		file << i << "\t" << data[0][i] << "\t" << data[1][i] << "\t" << data[2][i] << "\t" << data [3][i] << "\t" << data[4][i] << std::endl;
	}
	file.close();
}

//simulate scattering processes for statistical analyzation
void simulate_stat(int N, double data[5][361]){
	//initialize data-array with 0s
	for(int i=0; i<5; i++){
		for(int j=0; j<361; j++){
			data[i][j] = 0;
		}
	}

	//loop over all particles
	for(int i=0; i<N; i++){
		std::cout << "Simulating path of particle " << i+1 << std::endl;
		//spawn particle at (0/0) with angle 0° for movement in x-direction
		double x=0; //x-position
		double y=0; //y-position
		double a = 0; //angle in degree
		bool absorbed = false;
		//repeat steps as long is particle is not absorbed (and inside box)
		while(!absorbed){
			//calculate path length
			double x_old = x;
			double y_old = y;
			double r = getPath(x, y, a);
			//save in data-array at position r*10 (rounded); factor 10 for spreading of axis
			data[0][(int)round(r*10)] +=1.0;
			//check if particle is still in box
			if(x<-5 || x>100||y<-50||y>50){
				absorbed=true;
			} else if(x_old<1 && x>=1){
				//notice if particle hit detector at x=1
				//y-position is y_old+(y-y_old)/(x-x_old)*(1-x_old)
				//increase by 180 to enable positive and negative values for y
				data[3][(int)((y_old+(y-y_old)/(x-x_old)*(1-x_old))*10+180)] += 1;
			} else if(x_old<5 && x>=5){
				//notice if particle hit detector at x=5
				//y-position is y_old+(y-y_old)/(x-x_old)*(1-x_old)
				//increase by 180 to enable positive and negative values for y
				data[4][(int)((y_old+(y-y_old)/(x-x_old)*(5-x_old))*10+180)] += 1;
			} else{
				//check if particle becomes absorbed
				absorbed = becomesAbsorbed();
				//save process
				data[1][absorbed] += 1.0;
				if(!absorbed){
					//if not absorbed: compute scattering angle
					a = getAngle();
					//save in data-array; add +180 to position to stay in array-bounds
					data[2][(int)round(a)+180] += 1.0;
				}
			}
		}
	}

	//get number of events for each column
	int numberOfEvents[5] = {0,0,0,0,0};
	for(int i=0; i< 5; i++){
		for(int j=0; j<361; j++){
			numberOfEvents[i] += data[i][j];
		}
	}

	//print number of events at detector x=1 and x=5
	std::cout<< std::endl << "Number of events for detector at x=1: "<< numberOfEvents[3] << std::endl;
	std::cout<< "Number of events for detector at x=5: "<< numberOfEvents[4] << std::endl;

	//normalize
	for(int i=0; i< 5; i++){
		for(int j=0; j<361; j++){
			data[i][j] = data[i][j]/numberOfEvents[i];
		}
	}

	//save statistical data
	write(N, data);
	std::cout << "Finished!";
}

//simulate scattering processes for displaying trajectories
void simulate_path(){
	//simulate path for  particles
	for(int i=0; i<5; i++){
		//open file
		std::ofstream file;
		file.open("xy"+std::to_string(i)+".txt");

		std::cout << "Simulating path of particle " << i+1 << std::endl;
		//spawn particle at (0/0) with angle 0° for movement in x-direction
		double x=0; //x-position
		double y=0; //y-position
		//save starting positions in file
		file << x << "\t" << y << std::endl;
		double a = 0; //angle in degree
		bool absorbed = false;
		//repeat steps as long is particle is not absorbed (and inside box)
		while(!absorbed){
			//get path length
			getPath(x, y, a);
			//write new positions into file
			file << x << "\t" << y << std::endl;
			//check if particle is still in box
			if(x<-5 || x>100||y<-50||y>50){
				file << std::endl;
				absorbed=true;
			} else{
				//check if particle becomes absorbed
				absorbed = becomesAbsorbed();
				if(!absorbed){
					//if not absorbed: compute scattering angle
					a = getAngle();
				}
			}
		}
		file.close();
	}
	//close file
	std::cout << "Finished!";

}

int main(){
	srand(time(NULL));
	double data[5][361];
	simulate_stat(100000, data);
}
