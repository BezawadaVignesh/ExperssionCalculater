#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
	double** entries;
	int rows;
	int cols;
} Matrix;

Matrix* matrix_load(char* str);
Matrix* matrix_create(int row, int col);
void matrix_free(Matrix *m);
void matrix_print(Matrix* m);
Matrix* matrix_copy(Matrix* m);
Matrix* matrix_unit(int r, int  c, double n);
int check_dimensions(Matrix *m1, Matrix *m2);
Matrix* multiply(Matrix *m1, Matrix *m2);
Matrix* add(Matrix *m1, Matrix *m2);
Matrix* subtract(Matrix *m1, Matrix *m2);
Matrix* apply(double (*func)(double), Matrix* m);
Matrix* dot(Matrix *m1, Matrix *m2);
Matrix* transpose(Matrix* m);



#endif
