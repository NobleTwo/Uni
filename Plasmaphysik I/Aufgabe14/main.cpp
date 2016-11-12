#include <iostream>
#include <math.h>
#include <stdlib.h>

double c1(){
    double nint = 0.99;
    double nt = pow(10,22);
    double me = 9.1*pow(10,-31);
    double k = 1.4*pow(10,-23);
    double hbar = 1.1*pow(10,-34);
    return (1-nint)/nt*pow(me*k/(2*M_PI*hbar*hbar), 3./2.);
}

double c2(){
    double chi = 13.6*1.6*pow(10,-19);
    double k = 1.4*pow(10,-23);
    return chi/k;
}

double f(double T){
    return c1()*pow(T, 3./2.)-exp(c2()/T);
}

double fprime(double T){
    return 3./2.*c1()*pow(T, 1./2.)+exp(c2()/T)*c2()/(T*T);
}

int main(){
    double T = pow(10,4);
    int n = 0;
    while(abs(f(T))>pow(10,-15) && n<100){
        T = T-f(T)/fprime(T);
        std::cout << "T = " << T << ", n = " << n << std::endl;
        n++;
    }
}

