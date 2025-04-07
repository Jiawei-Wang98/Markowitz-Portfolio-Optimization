#ifndef adjoint_matrix_h
#define adjoint_matrix_h
#include <iostream>
using namespace std;


//this is for low-dimensional matrix
class adjoint{
public:
    void getCfactor(double M[N][N], double t[N][N], int p, int q, int n) {
        int i = 0, j = 0;
        for (int r= 0; r< n; r++) {
            for (int c = 0; c< n; c++){ //Copy only those elements which are not in given row r and column c: 
                if (r != p && c != q) { t[i][j++] = M[r][c]; //If row is filled increase r index and reset c index
                    if (j == n - 1) {
                    j = 0; i++;
                    }
                }
            }
        }
    }

    double DET(double M[N][N], double n){ //to find determinant 
        double D = 0;
        if (n == 1)
            return M[0][0];
        double t[N][N]; //store cofactors
        int s = 1; //store sign multiplier //To Iterate each element of first row
        for (int f = 0; f < n; f++) {
            //For Getting Cofactor of M[0][f] do 
            getCfactor(M, t, 0, f, n); 
            D += s * M[0][f] * DET(t, n - 1);

            s = -s;
        }
        return D;
    }

    void ADJ(double M[N][N],double adj[N][N]){
            //to find adjoint matrix 
        if (N == 1) {
            adj[0][0] = 1; return;
        }
        int s = 1;
        double t[N][N];
        for (int i=0; i<N; i++) {
            for (int j=0; j<N; j++) {
                //To get cofactor of M[i][j]
                getCfactor(M, t, i, j, N);
                s = ((i+j)%2==0)? 1: -1; //sign of adj[j][i] positive if sum of row and column indexes is even.
                adj[j][i] = (s)*(DET(t, N-1)); //Interchange rows and columns to get the transpose of the cofactor matrix
            }
        }
    }

    bool INV(double M[N][N], double inv[N][N]) {
        double det = DET(M, N);
        if (det == 0) {
            cout << "can't find its inverse";
            return false;
        }
        double adj[N][N]; 
        ADJ(M, adj);
        for (int i=0; i<N; i++) for (int j=0; j<N; j++) inv[i][j] = adj[i][j]/det;
        return true;
    }

};

# endif