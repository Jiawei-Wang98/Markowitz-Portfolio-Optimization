#ifndef constructMatrix_h
#define constructMatrix_h
#include <vector>
#include <iostream>
using namespace std;

class constructMatrix{
public:
    // output return Matrix
    void read_returnMatrix(double mar[][numberReturns]){
        for(int i=0;i<numberAssets;i++){
            for (int j=0;j<numberReturns;j++){
                cout << mar[i][j] << ' ';
            }
            cout << endl;
        }
    }
    // output variance matrix
    void read_var(double mar[][numberAssets]){
        for(int i=0;i<numberAssets;i++){
            for (int j=0;j<numberAssets;j++){
                cout << mar[i][j] << ' ';
            }
            cout << endl;
        }
    }
    // output merge matrix
    void read_merge(double mar[][numberAssets+2]){
        for(int i=0;i<numberAssets+2;i++){
            for (int j=0;j<numberAssets+2;j++){
                cout << mar[i][j] << ' ';
            }
            cout << endl;
        }
    }

    /* function for exchanging two rows of
    a matrix */
    void swap(double mat[][numberAssets], int row1, int row2,
            int col)
    {
        for (int i = 0; i < col; i++)
        {
            int temp = mat[row1][i];
            mat[row1][i] = mat[row2][i];
            mat[row2][i] = temp;
        }
    }

    // copy matrix
    void copy_matrix(double mat[][numberAssets],double secondmat[][numberAssets]){
        for(int a=0;a<numberAssets;a++){
            for(int b=0;b<numberAssets;b++){
                secondmat[a][b]=mat[a][b];
            }
        }
    }

    /* function for finding rank of matrix */
    int rankOfMatrix(double orig_mat[][numberAssets])
    {
        int rank = numberAssets;
        double mat[numberAssets][numberAssets];
        copy_matrix(orig_mat,mat);
        for (int row = 0; row < rank; row++)
        {
            if (mat[row][row])
            {
            for (int col = 0; col < numberAssets; col++)
            {
                if (col != row)
                {
                    double mult = (double)mat[col][row] /
                                        mat[row][row];
                    for (int i = 0; i < rank; i++)
                    mat[col][i] -= mult * mat[row][i];
                }
            }
            }
            else
            {
                bool reduce = true;
                for (int i = row + 1; i < numberAssets;  i++)
                {
                    if (mat[i][row])
                    {
                        swap(mat, row, i, rank);
                        reduce = false;
                        break ;
                    }
                }

                if (reduce)
                {
                    rank--;
                    for (int i = 0; i < numberAssets; i ++)
                        mat[i][row] = mat[i][rank];
                }
                row--;
            }
        }
        return rank;
    }
    // merge matrix
    void createmergematrix(double cov_mar[][numberAssets],double r[numberAssets], double e[numberAssets],
    double merge_matrix[][numberAssets+2]){
        for(int i=0;i<numberAssets+2;i++){
            for (int j=0;j<numberAssets+2;j++){
                if (i<numberAssets&j<numberAssets){
                    merge_matrix[i][j]=cov_mar[i][j];
                }else if(i<numberAssets&j==numberAssets){
                    merge_matrix[i][j]=-r[i];
                }else if(i==numberAssets&j<numberAssets){
                    merge_matrix[i][j]=-r[j];
                }else if(i<numberAssets&j==numberAssets+1){
                    merge_matrix[i][j]=-e[i];
                }else if(i==numberAssets+1&j<numberAssets){
                    merge_matrix[i][j]=-e[j];
                }else{
                    merge_matrix[i][j]=0;
                }
            } 
        }
    }
    // mutiply
    void mutiply(double AB[numberAssets+2],double A[][numberAssets+2],double B[numberAssets+2]){
        for (int i = 0; i < numberAssets+2; i++){
            for(int j=0; j< numberAssets+2; j++){
                AB[i] += A[i][j]*B[j];
            }
        }
    }

};

#endif