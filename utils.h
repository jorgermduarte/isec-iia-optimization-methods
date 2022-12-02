//
// Created by joaok on 12/2/2022.
//

#ifndef ISEC_IIA_OPTIMIZATION_METHODS_UTILS_H
#define ISEC_IIA_OPTIMIZATION_METHODS_UTILS_H

int* data_initialize(char *nome, int *n, int *iter);
void initial_solution(int *sol, int v);
void write_solution(int *sol, int vert);
void substitute(int a[], int b[], int n);
void init_rand();
int random_l_h(int min, int max);
float rand_01();

int  evaluate_solution(int a[], int *mat, int vert);




#endif //ISEC_IIA_OPTIMIZATION_METHODS_UTILS_H
