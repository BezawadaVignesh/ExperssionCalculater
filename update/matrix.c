#ifndef MATRIX_C
#define MATRIX_C

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<assert.h>

#include"matrix.h"



Matrix* matrix_create(int row, int col) {
	Matrix *matrix = malloc(sizeof(Matrix));
	matrix->rows = row;
	matrix->cols = col;
	matrix->entries = malloc(row * sizeof(double*));
	for (int i = 0; i < row; i++) {
		matrix->entries[i] = malloc(col * sizeof(double));
	}
	return matrix;
}

void matrix_fill(Matrix *m, int n) {
	
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			m->entries[i][j] = n;
		}
	}
	
}

Matrix* matrix_load(char* str){
	Matrix* mat = matrix_create(3, 3);
	double temp;
	int i=0;
	for(int a=0;a<3;a++){
		for(int b=0;b<3;b++){
			temp=0;
			if(str[i] == '\0') assert(0);
			while(isspace(str[i])) i++;
			for(;isdigit(str[i]);i++){
				temp = temp*10 + str[i] - '0';	
			}
			if(str[i] == '.'){
				int j=1;
				i+=1;
				for( ;isdigit(str[i]);i++,j*=10){
					temp = temp*10 + str[i] - '0';
				}
				temp /= j;
			}
			
			mat->entries[a][b]=temp;
			while(isspace(str[i])) i++;
		}
		if(str[i] == ',') i++;
		//else assert(0);
	}
	
	return mat;
}

Matrix* matrix_unit(int r, int  c, double n){
	Matrix* mat = matrix_create(r, c);
	for(int i=0;i<r;i++){
		for(int j=0;j<c;j++){
			mat->entries[i][j] = (i==j)?n:0; 
		}
	}
	return mat;
}

void matrix_free(Matrix *m) {
	if(m==NULL) return;
	for (int i = 0; i < m->rows; i++) {
		free(m->entries[i]);
	}
	free(m);
	m = NULL;
}

void matrix_print(Matrix* m) {
	if(m==NULL) return;
	printf("Rows: %d Columns: %d\n", m->rows, m->cols);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			printf("%1.3f ", m->entries[i][j]);
		}
		printf("\n");
	}
}

Matrix* matrix_copy(Matrix* m) {
	Matrix* mat = matrix_create(m->rows, m->cols);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			mat->entries[i][j] = m->entries[i][j];
		}
	}
	return mat;
}

int check_dimensions(Matrix *m1, Matrix *m2) {
	if (m1->rows == m2->rows && m1->cols == m2->cols) return 1;
	return 0;
}

void num_multiply(Matrix *m, double n){
	
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			m->entries[i][j] *= n;
		}
	}
}

Matrix* multiply(Matrix *m1, Matrix *m2) {
	if (check_dimensions(m1, m2)) {
		Matrix *m = matrix_create(m1->rows, m1->cols);
		for (int i = 0; i < m1->rows; i++) {
			for (int j = 0; j < m2->cols; j++) {
				m->entries[i][j] = m1->entries[i][j] * m2->entries[i][j];
			}
		}
		return m;
	} else {
		printf("Dimension mistmatch multiply: %dx%d %dx%d\n", m1->rows, m1->cols, m2->rows, m2->cols);
		return NULL;
	}
}

Matrix* add(Matrix *m1, Matrix *m2) {
	if (check_dimensions(m1, m2)) {
		Matrix *m = matrix_create(m1->rows, m1->cols);
		for (int i = 0; i < m1->rows; i++) {
			for (int j = 0; j < m2->cols; j++) {
				m->entries[i][j] = m1->entries[i][j] + m2->entries[i][j];
			}
		}
		return m;
	} else {
		printf("Dimension mistmatch add: %dx%d %dx%d\n", m1->rows, m1->cols, m2->rows, m2->cols);
		return NULL;
	}
}

Matrix* subtract(Matrix *m1, Matrix *m2) {
	if (check_dimensions(m1, m2)) {
		Matrix *m = matrix_create(m1->rows, m1->cols);
		for (int i = 0; i < m1->rows; i++) {
			for (int j = 0; j < m2->cols; j++) {
				m->entries[i][j] = m1->entries[i][j] - m2->entries[i][j];
			}
		}
		return m;
	} else {
		printf("Dimension mistmatch subtract: %dx%d %dx%d\n", m1->rows, m1->cols, m2->rows, m2->cols);
		return NULL;
	}
}

Matrix* apply(double (*func)(double), Matrix* m) {
	Matrix *mat = matrix_copy(m);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			mat->entries[i][j] = (*func)(m->entries[i][j]);
		}
	}
	return mat;
}

Matrix* dot(Matrix *m1, Matrix *m2) {
	if (m1->cols == m2->rows) {
		Matrix *m = matrix_create(m1->rows, m2->cols);
		for (int i = 0; i < m1->rows; i++) {
			for (int j = 0; j < m2->cols; j++) {
				double sum = 0;
				for (int k = 0; k < m2->rows; k++) {
					sum += m1->entries[i][k] * m2->entries[k][j];
				}
				m->entries[i][j] = sum;
			}
		}
		return m;
	} else {
		printf("Dimension mistmatch dot: %dx%d %dx%d\n", m1->rows, m1->cols, m2->rows, m2->cols);
		return NULL;
	}
}

Matrix* transpose(Matrix* m) {
	Matrix* mat = matrix_create(m->cols, m->rows);
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			mat->entries[j][i] = m->entries[i][j];
		}
	}
	return mat;
}
#endif
/*
func_type find_func(char* func_name){
	if(!strcmp(func_name, "t")){
		return transpose;
	}
	else{
		printf("Else");
		return transpose;
	}
}

/*
int main(){
	//matrix_print(matrix_load("1 2 3, 4 5 6, 7 8 9"));
	func_type func = find_func("t");
	matrix_print(func(matrix_load("1 2 3, 4 5 6, 7 8 9")));
}*/
