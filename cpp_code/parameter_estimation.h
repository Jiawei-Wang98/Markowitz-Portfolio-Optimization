#ifndef parameter_estimation_h
#define parameter_estimation_h
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <iostream>
using namespace std;

class parameter_estimation{
public:
    //calculate the mean given asset, window and return matrix
    double para_mean(int i,int start, int n,double Matrix[][numberReturns]){
        double mean=0;
        for(int j=0;j<n;j++)
        {
            double temp=Matrix[i][j+start];
            mean=mean+temp/n;
        }
        return mean;
    }
    //calculate the covariance given assets, window and return matrix
    double para_cov(int i, int j, int start, int n, double Matrix[][numberReturns]){
        double cov=0;
        double mean_i=para_mean(i,start,n,Matrix);
        double mean_j=para_mean(j,start,n,Matrix);
        double temp_ij=0;
        for(int k=0;k<n;k++){
            double temp_i=Matrix[i][k+start]-mean_i;
            double temp_j=Matrix[j][k+start]-mean_j;
            temp_ij=temp_ij+temp_i*temp_j;
        }
        cov=temp_ij/(n-1);
        
        return cov;
    }
};


#endif