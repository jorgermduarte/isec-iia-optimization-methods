//
// Created by joaok on 12/2/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include "algorithm.h"

// Switch a vertice from one group to another
// Receives actual solution, neighbor solution, number of vertices
void neighborOne (int a[], int b[], int n)
{
    int i, p1, p2;
    // copy actual solution to neighbor solution
    for(i = 0; i < n; i++)
        b[i] = a[i];
    // Found randomly a position of a vertice with value 0
    do
        p1 = random_l_h(0, n-1);
    while(b[p1] != 0);
    // Found randomly a position of a vertice with value 1
    do
        p2 = random_l_h(0, n-1);
    while(b[p2] != 1);
    // Switch the values of the vertices of the positions found
    b[p1] = 1;
    b[p2] = 0;
}

// Switch two vertices from one group to another
// Receives actual solution, neighbor solution, number of vertices
void NeighborTwo (int a[], int b[], int n)
{
    int i, p1, p2, p3, p4;
    // Copy the actual solution to the neighbor solution
    for(i = 0; i < n; i++)
        b[i] = a[i];
    // Found randomly a position of a vertice with value 0
    do
        p1 = random_l_h(0, n-1);
    while(b[p1] != 0);
    // Found randomly a position of a vertice with value 1
    do
        p2 = random_l_h(0, n-1);
    while(b[p2] != 1);
    // Switch the values of the vertices of the positions found
    b[p1] = 1;
    b[p2] = 0;
    // Found randomly a position of a vertice, that is not equal to p2, with value 0
    do
        p3 = random_l_h(0, n-1);
    while(b[p3] != 0 || p3 == p2);
    // Found randomly a position of a vertice, that is not equal to p1, with value 1
    do
        p4 = random_l_h(0, n-1);
    while(b[p4] != 1 || p4 == p1);
    // Switch the values of the vertices of the positions found
    b[p3] = 1;
    b[p4] = 0;
}


// Mountain climbing algorithm : First Choice
// Returns the best solution found
int mountain_climbing(int sol[], int *mat, int vert, int num_iter){
    int *new_sol, cost, cost_neighbor, i;
    new_sol = malloc(sizeof(int)*vert); // Allocate memory for the new solution
    if (new_sol == NULL) {
        printf("Error while allocating memory\n");
        exit(1);
    }
    //Evaluate the initial solution
    cost = evaluate_solution(sol, mat, vert);
    for (i = 0; i < num_iter; i++) {
        // generate the neighbor solution
        neighborOne(sol, new_sol, vert);
        cost_neighbor = evaluate_solution(new_sol, mat, vert);
        // if the neighbor solution is better than the actual solution
        if (cost_neighbor < cost) {
            // substitute the actual solution by the neighbor solution
            substitute(sol, new_sol, vert);
            cost = cost_neighbor;
        }
    }
    // Free memory used by the new solution
    free(new_sol);
    return cost;
}