#ifndef LU_h
#define LU_h
#include <iostream>
#include <cmath>
using namespace std;

class inverse_LUdecomposition{
public:
//mutiply 
double *mul(double A[N*N],double B[N*N]){
    double *C=new double[N*N]{};
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            for(int k=0;k<N;k++){
                C[i*N+j]+= A[i*N+k]*B[k*N+j];
            }
        }
    }
    //if the abosulte value less than 10^-30, let it be 0
    for(int i=0;i<N*N;i++){
        if(abs(C[i])<pow(10,-30)){
            C[i]=0;
        }
    }
    return C;
}

//LUP decomposition
void LUP_Descomposition(double A[N*N],double L[N*N],double U[N*N],int P[N]){
    int row=0;
    for(int i=0;i<N;i++){
        P[i]=i;
    }
    for(int i=0;i<N-1;i++){
        double p=0;
        for(int j=i;j<N;j++){
            if(abs(A[j*N+i])>p){
                p=abs(A[j*N+i]);
                row=j;
            }
        }
        if(0==p){
            cout<<"can not find the inverse"<<endl;
            return;
        }
        // change P[i] and P[row]
        int tmp =P[i];
        P[i]=P[row];
        P[row]=tmp;

        double tmp2=0;
        for(int j=0;j<N;j++)
        {
            //change A[i][j] and A[row][j]
            tmp2=A[i*N+j];
            A[i*N+j]=A[row*N+j];
            A[row*N+j]=tmp2;
        }
        double u=A[i*N+i],l=0;
        for(int j=i+1;j<N;j++){
            l=A[j*N+i]/u;
            A[j*N+i]=l;
            for(int k=i+1;k<N;k++){
                A[j*N+k]=A[j*N+k]-A[i*N+k]*l;
            }
        }
        
    }
    // construct L and U
    for(int i=0;i<N;i++){
        for(int j=0;j<=i;j++){
            if(i!=j){
               L[i*N+j]=A[i*N+j]; 
            }else{
                L[i*N+j]=1;
            }
        }
        for(int k=i;k<N;k++){
            U[i*N+k]=A[i*N+k];
        }
    }
}

// LUP solve
double * LUP_Solve(double L[N*N],double U[N*N],int P[N],double b[N]){
    double *x=new double[N]();
    double *y=new double[N]();
    for(int i = 0;i < N;i++){
        y[i] = b[P[i]];
        for(int j = 0;j < i;j++){
            y[i] = y[i] - L[i*N+j]*y[j];
        }
    }
    for (int i = N-1;i >= 0; i--){
        x[i]=y[i];
        for(int j = N-1;j > i;j--){
            x[i] = x[i] - U[i*N+j]*x[j];
        }
        x[i] /= U[i*N+i];
    }
    return x;
}

/////////////////////////////////
int getNext(int i, int m, int n){
    return (i%n)*m + i/n;
}

int getPre(int i, int m, int n){
    return (i%m)*n + i/m;
}

void movedata(double *mtx, int i, int m, int n){
    double temp = mtx[i];
    int cur = i;  
    int pre = getPre(cur, m, n);
    while(pre != i){
        mtx[cur] = mtx[pre];
        cur = pre;
        pre = getPre(cur, m, n);
    }
    mtx[cur] = temp;
}

void transpose(double *mtx, int m, int n){
    for(int i=0; i<m*n; ++i){
        int next = getNext(i, m, n);
        while(next > i){
            next = getNext(next, m, n);
        }
        if(next == i){
            movedata(mtx, i, m, n);
        }
    }
}
///////////////////////////////
double * LUP_solve_inverse(double A[N*N]){
    double *A_mirror = new double[N*N]();
    double *inv_A=new double[N*N]();
    double *inv_A_each=new double[N]();
    double *b    =new double[N]();
    for(int i=0;i<N;i++){
        double *L=new double[N*N]();
        double *U=new double[N*N]();
        int *P=new int[N]();

        for(int i=0;i<N;i++){
            b[i]=0;
        }
        b[i]=1;

        for(int i=0;i<N*N;i++){
            A_mirror[i]=A[i];
        }
        LUP_Descomposition(A_mirror,L,U,P);
        inv_A_each=LUP_Solve (L,U,P,b);
        memcpy(inv_A+i*N,inv_A_each,N*sizeof(double));
    }
    transpose(inv_A,N,N);
    return inv_A;
}
};

#endif