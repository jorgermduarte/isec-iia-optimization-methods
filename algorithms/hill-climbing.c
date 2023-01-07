#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "../utils/utils.h"

int executions = 0;
int vertices, edges; // number of vertices and edges
int k; // size of the subset
int** adjacency_matrix; // adjacency matrix to store the graph

int* solution; // array to store the current solution
int* best_solution; // array to store the best solution found so far
int best_solution_cost; // value of the best solution found so far

int iterationsHillClimbing = 1000;
bool displaySolutions = true;

float mbf = 0;

void generate_neighbor(int a[], int b[], int n)
{
    int i, p1, p2;
    for(i = 0; i < n; i++)
        b[i] = a[i];
    do
        p1 = random_l_h(0, n-1);
    while(b[p1] != 0);
    do
        p2 = random_l_h(0, n-1);
    while(b[p2] != 1);
    b[p1] = 1;
    b[p2] = 0;
}

void generate_neighbor2(int a[], int b[], int n)
{
    int i, p1, p2, p3, p4;

    for(i = 0; i < n; i++)
        b[i] = a[i];
    do
        p1 = random_l_h(0, n-1);
    while(b[p1] != 0);
    do
        p2 = random_l_h(0, n-1);
    while(b[p2] != 1);
    b[p1] = 1;
    b[p2] = 0;
    do
        p3 = random_l_h(0, n-1);
    while(b[p3] != 0 || p3 == p2);
    do
        p4 = random_l_h(0, n-1);
    while(b[p4] != 1 || p4 == p1);
    b[p3] = 1;
    b[p4] = 0;
}

int hill_climbing(int sol[], int **mat, int vert, int num_iter)
{
    int *nova_sol, cost, cost_neighbor, i;

    nova_sol = malloc(sizeof(int)*vert);

    if (nova_sol == NULL)
    {
        printf("Error allocating memory for the new solution.\n");
        exit(1);
    }

    cost = calculate_fit(sol, mat, vert);
    for(i = 0; i < num_iter; i++)
    {
        //generate_neighbor(sol, nova_sol, vert);
        generate_neighbor2(sol, nova_sol, vert);


        cost_neighbor = calculate_fit(nova_sol, mat, vert);
        if (cost_neighbor >= cost)
        {
            replace(sol, nova_sol, vert);
            cost = cost_neighbor;
        }
    }
    free(nova_sol);
    return cost;
}

// function to calculate the number of edges in a given subset
void code_execution(){
    generateInitialSolution(vertices, k, &solution);

    int current_cost = hill_climbing(solution, adjacency_matrix, vertices, iterationsHillClimbing);

    if(displaySolutions){
        displayCurrentSolution(vertices, solution);
        printf("         > Cost: %d\n", current_cost);
    }

    if(k==0 || current_cost > best_solution_cost)
    {
        best_solution_cost = current_cost;
        replace(best_solution, solution, vertices);

    }

    //printf("   > Best solution: %d\n" , best_solution_cost);
    //printf("   > Current solution: %d\n" , current_cost);
    //free(solution);
}

int main(int argc, char *argv[]) {
    if(argc != 3){
        printf("Please provide two arguments, example: ./hill-climbing <input_file> <execution_times> \n");
        exit(1);
    }

    // random seed for the random number generator
    srand((unsigned)time(NULL));

    char *filename = argv[1];
    executions = atoi(argv[2]);

    printf(" > Reading input file: %s\n" , filename);
    printf(" > Number of executions: %d\n", executions);

    best_solution = (int*)malloc(k * sizeof(int));

    init(filename, &k, &vertices, &edges, &adjacency_matrix);
    displayAdjacencyMatrix(vertices, adjacency_matrix);

    for(int i = 0; i < executions; i++){
        code_execution();
        mbf += best_solution_cost;
    }

    printf("############################################################################################# \n");

    printf("> Final best solution: ");
    for(int i = 0; i < vertices; i++){
        printf("%d ", best_solution[i]);
    }
    printf("\n");

    printf("> Best solution cost: %d\n", best_solution_cost);

    printf("> Average cost: %f\n", mbf/executions);

    // clean up
    for(int i = 0; i < vertices; i++){
        free(adjacency_matrix[i]);
    }

    free(adjacency_matrix);
    free(best_solution);

    return 0;
}