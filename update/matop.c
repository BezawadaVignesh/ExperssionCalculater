#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include"matrix.h"
#include"matrix.c"

void getCofactor(double** a, double** temp, int p, int q, int n)
{
    int i = 0, j = 0;
 
    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those element
            //  which are not in given row and column
            if (row != p && col != q)
            {
                temp[i][j++] = a[row][col];
 
                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

double determinant(double** a, double k){
  	double s = 1, det = 0, **b;
  	
  	b = (double**)malloc(k * sizeof(double*));
  	for(int i = 0; i < k; i++) {
		b[i] = malloc(k * sizeof(double));
	}
	
	int i, j, m, n, c;
	if (k == 1){
	    return (a[0][0]);
	    }
  else{
	 det = 0;
	 for (c = 0; c < k; c++){
	    m = 0;
	    n = 0;
	    for (i = 0;i < k; i++){
	        for (j = 0 ;j < k; j++){
	            b[i][j] = 0;
	            if (i != 0 && j != c){
	               	b[m][n] = a[i][j];
	               	if (n < (k - 2))
	            		n++;
	               	else{
	                 n = 0;
	                 m++;
	                 }
	               }
	           }
	         }
	      det = det + s * (a[0][c] * determinant(b, k - 1));
	      s = -1 * s;
	      }
    }
 
    return (det);
}



Matrix* adjoint(Matrix* a){
	
    if (a->rows!=a->cols){
        printf("Matrix is not square.");
        return NULL;
    }
 	Matrix* adj = matrix_create(a->rows, a->cols);
 	
 	int sign = 1, N=a->rows;
    double **temp;
    
    temp = (double**)malloc(N * sizeof(double*));
  	for(int i = 0; i < N; i++) {
		temp[i] = malloc(N * sizeof(double));
	}
 
    for (int i=0; i<N; i++)
    {
        for (int j=0; j<N; j++)
        {
            // Get cofactor of A[i][j]
            getCofactor(a->entries, temp, i, j, N);
 
            // sign of adj[j][i] positive if sum of row
            // and column indexes is even.
            sign = ((i+j)%2==0)? 1: -1;
 
            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            adj->entries[j][i] = (sign)*(determinant(temp, N-1));
        }
    }
    return adj;
}

Matrix* mat_inverse(Matrix* a)
{
	if (a->rows!=a->cols){
        printf("Matrix is not square.");
        return NULL;
    }
    // Find determinant of A[][]
    
    double det = determinant(a->entries, a->rows);
    if (det == 0){
        printf("Singular matrix, can't find its inverse");
        return NULL;
    }
 
    Matrix* adj = adjoint(a);
    Matrix* inv = matrix_create(a->rows, a->cols);
 	int N=a->rows;
    for (int i=0; i<N; i++)
        for (int j=0; j<N; j++)
            inv->entries[i][j] = adj->entries[i][j]/det;
    return inv;
}

Matrix* activate(Matrix* a){
	Matrix* mat = matrix_create(a->rows, a->cols);
	matrix_print(mat);
	printf("You can do row(eg: r1=r1+r2) operations or coloum(eg: r1=r1+r2) operations.\nEnter deactivate to close.");
	
}

void row_swap(Matrix* a, int r1, int r2){
	double tmp;
	for(int i=0;i<a->cols;i++){
		tmp = a->entries[r1][i];
		a->entries[r1][i] = a->entries[r2][i];
		a->entries[2][i] = tmp;
	}
}

Matrix* echelon_form_of(Matrix* a){
	Matrix* cpy = matrix_copy(a);
	double** mat = cpy->entries;
	double temp;
	int i;
	for(int k=0;k<cpy->rows;k++)
	{
		if(mat[k][k]==0){
			for(i=k;i<cpy->rows;i++){
				if(mat[i][k]!=0){
					printf("\nSwap R%d <-> R%d", k+1, i+1);
					row_swap(cpy, k, i);
					break;
				}
			}
			if(i==cpy->rows)
				continue;
		}
		if( (mat[k][k]) != 1)
		{
			temp = mat[k][k];
			if(temp == 0)
				continue; // Avoiding division by zero
			printf("\nR%d <- R%d / %.3lf", k+1,k+1,mat[k][k]);
			for(int j=0;j<cpy->cols;j++)
			{
				mat[k][j] = ( (mat[k][j]) / temp );
			}
		}
		
		for(int i=k+1;i<cpy->rows;i++)
		{
			temp = mat[i][k];
			printf("\nR%d <- R%d - (%.3lf * R%d)", i+1, i+1, temp, k+1);
			for(int j=k;j<cpy->cols;j++){	
				mat[i][j] = mat[i][j] - ( mat[k][j] * temp);
			}
		
		}

		// Printing Each Step
		printf("\n**************************\n");
		printf("Step %d\n\n",k+1);
		for(int a=0;a<cpy->rows;a++)
		{
			for(int b=0;b<cpy->cols;b++)
			{
				if(mat[a][b] == -0)
					mat[a][b] = 0; // Simply converting'-0' into '0'
				printf("%.3lf\t",mat[a][b]);
			}
			printf("\n");
		}
	}
	return cpy;
	
}
/*
Matrix* normal_form_of(Matrix* a){
	
}

int main(){
	Matrix* m = matrix_create(3,3);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			scanf("%lf", &(m->entries[i][j]));
		}
	}
	matrix_print(m);
	matrix_print(echelon(m));
	return 0;
}
*/
