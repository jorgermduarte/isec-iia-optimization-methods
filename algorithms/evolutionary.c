#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../utils/utils.h"

struct population{
    int size;
    float mutationProbability;
    float recombinationProbability;
    int tournamentSize;
    int penalization;
    int subgroupNumber;
    int capacity;
    int generationsNumber;
};

int evolutionary(int sol[], int **mat, int vert, int num_iter){
    return 0;
}


int executions = 0;
int vertices, edges; // number of vertices and edges
int k; // size of the subset
int** adjacency_matrix; // adjacency matrix to store the graph

int* solution; // array to store the current solution
int* best_solution; // array to store the best solution found so far
int best_solution_cost; // value of the best solution found so far

int iterationsEvolutionary = 100;
bool displaySolutions = false;

// function to calculate the number of edges in a given subset
void code_execution(){
    generateInitialSolution(vertices, k, &solution);



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
    }

    printf(" =============================== \n");
    printf("> Best solution with the best cost: %d\n", best_solution_cost);
    printf("> Best solution: ");
    for(int i = 0; i < vertices; i++){
        printf("%d ", best_solution[i]);
    }
    printf("\n");

    // clean up
    for(int i = 0; i < vertices; i++){
        free(adjacency_matrix[i]);
    }

    free(adjacency_matrix);
    free(best_solution);

    return 0;
}