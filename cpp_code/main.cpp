#include "read_data.h"
#include "parameter_estimation.h"
#include "constructMatrix.h"
#include "inverse_adjoint_matrix.h"
#include "inverse_LUdecomposition.h"
#include <iostream>

using namespace std;

int  main (int  argc, char  *argv[])
{
    //double rp=0.05; //target returns
    const int window=100;  
    const int test_period=12;
    string fileName="asset_returns.csv";
    std::ofstream out("result.csv"); 
    // the rows represent each window(50),the columns represent returns and variance
    //[return(rp=0),variance(rp=0),return(rp=0.005),variance(rp=0.005)]

    constructMatrix con_Matrix;
    adjoint adjo;
    inverse_LUdecomposition LU;
    parameter_estimation param;

    const int range=20;
    int start = 0;
    double weight[numberAssets]; 
    // a matrix to store the weights of each asset in the portfolio
    cout<<"read data begins"<<endl;
    double returnMatrix[numberAssets][numberReturns]; 
    //a matrix to store the returns
	readData(returnMatrix,fileName);
    cout<<"read data ends"<<endl;

    double cov_mar[numberAssets][numberAssets]; 
    //a matrix to store the covariance
    double r[numberAssets];    
    // create r to store the average return of each asset
    double e[numberAssets];     
    fill_n(e, numberAssets, 1);
    double second_merge_martrix[N]; 
    // create second merge matrix essential for the solution
    
    double average_return[(numberReturns-window)/test_period]; 
    // a matrix to store the average_return in different backtesting 
    double oos_cov[numberAssets][numberAssets]; 
    // out-of-sample cov matrix
    double actual_variance[(numberReturns-window)/test_period]; 
    //out_of_sample actual variance w*oos_cov*w
    double result[(numberReturns-window)/test_period][range*2]; 
    //final result


    vector<double> Rp;
    //construct a vector to store target returns
    double rr=0.005;
    for (int i=0;i<range;i++){
        Rp.push_back(rr);
        rr+=0.005;
    }
    for (int h=0;h<range;h++){
        double rp=Rp[h];
        for(int i=0;i<N;i++){
            if (i<numberAssets){
                second_merge_martrix[i]=0;
            }else if(i==numberAssets){
                second_merge_martrix[i]=-rp;
            }else{
                second_merge_martrix[i]=-1;
            }
        }
        //rolling window begin 
        //the portfolio should be constructed (numberReturns-window)/test_period times
        //and it will bring (numberReturns-window)/test_period times of investment result
        for(int ii=0;ii<(numberReturns-window)/test_period;ii++){
            start=ii*test_period; //start point in sample
            for (int i=0;i<numberAssets;i++){
                r[i]=param.para_mean(i,start,window,returnMatrix); 
                // average return for asset i in sample
            }
            for(int i=0;i<numberAssets;i++){
                for (int j=0;j<numberAssets;j++){
                    cov_mar[i][j]=param.para_cov(i,j,start,window,returnMatrix); 
                    //cov matrix in sample for i and j
                    oos_cov[i][i]=param.para_cov(i,j,start+window,test_period,returnMatrix); 
                    //cov matrix out of sample for i and j
                }
            }
            // solve equation begin
            // first get the inverse of merge matrix
            double merge_matrix[N][N];
            double merge_matrix2[(N)*(N)];
            double inv[N][N];    
            // the inverse of matrix
            double *invOfA = new double[N*N]();
            if (con_Matrix.rankOfMatrix(cov_mar)==numberAssets){
                // if full rank
                con_Matrix.createmergematrix(cov_mar,r,e,merge_matrix);
                //cout << "Input matrix is :\n"; read_merge(merge_matrix);
                //cout << "\nUsing the adjoint matrix, the Inverse is :\n"; if (INV(merge_matrix, inv)) read_merge(inv);
                for(int i=0;i<N;i++){
                    for(int j=0;j<N;j++){
                        merge_matrix2[i*(N)+j]=merge_matrix[i][j];
                    }
                }
                //this is a high-dimensional matrix, here I use LUdecomposition
                invOfA=LU.LUP_solve_inverse(merge_matrix2);
                for(int i=0;i<N;i++){
                    for(int j=0;j<N;j++){
                        inv[i][j]=invOfA[i*N+j];
                    }
                }
            } else {
                    cout <<"The rank of the covariance matrix is not full"<<endl;
                    exit(0);}

            double solution[N]={}; 
            //solution of the equation and first n-2 elements are the weights
            con_Matrix.mutiply(solution,inv,second_merge_martrix);
            for(int i = 0; i<numberAssets; i++){
                weight[i]=solution[i];
            }
            // use weights to back test
            // calculate the average return
            double temp[numberAssets]={}; 
            // average return out of sample for each asset
            for (int i = 0; i<numberAssets; i++){
                temp[i]=param.para_mean(i,start+window,test_period,returnMatrix);
            }
            double temp1num=0;
            // weighted average return out of sample for the portfolio
            for (int i = 0; i<numberAssets; i++){
                temp1num+=weight[i]*temp[i];
            }
            average_return[ii]=temp1num;
            //calculate the actual covariance  w*oos_cov*w
            double temp2[numberAssets]={};
            double temp2num=0;
            for (int i = 0; i<numberAssets; i++){
                for(int j=0;j<numberAssets;j++){
                    temp2num+=weight[j]*oos_cov[i][j];
                }
                temp2[i]=temp2num;
            }
            double temp3num=0;
            for (int i = 0; i<numberAssets; i++){
                temp3num+=weight[i]*temp2[i];
            }
            actual_variance[ii]=temp3num;
        }
        cout<<"------------"<<"average_return for target return "<<rp<<"------------"<<endl;
        for(int i=0;i<(numberReturns-window)/test_period;i++){
            cout<<average_return[i]<<endl;
            result[i][2*h]=average_return[i];
        }
        cout<<"------------"<<"variance for target return "<<rp<<"------------"<<endl;
        for(int i=0;i<(numberReturns-window)/test_period;i++){
            cout<<actual_variance[i]<<endl;
            result[i][2*h+1]=actual_variance[i];
        }
    }
    for (int h=0;h<range;h++){
        if(h==range-1){
            out<<"returns(rp="<<to_string(Rp[h])<<',';
            out<<"variance(rp="<<to_string(Rp[h])<<'\n';
        }else{
            out<<"returns(rp="<<to_string(Rp[h])<<',';
            out<<"variance(rp="<<to_string(Rp[h])<<',';
        }
    }
    for (auto& row : result) {
        for (auto col : row)
            out << col <<',';
        out << '\n';
    }
    
	return 0;
}
