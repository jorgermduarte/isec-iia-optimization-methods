#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../utils/utils.h"

struct populationSettings{
    int size;
    float mutationProbability;
    float recombinationProbability;
    int tournamentSize;
    int penalization;
    int subgroupNumber;
    int capacity;
    int generationsNumber;
};

struct iterationSolution{
    int* solution;
    int fitness;
    int valid;
};

typedef struct iterationSolution* population; // population is an array of iterationSolution

int executions = 0;
int vertices, edges; // number of vertices and edges
int k; // size of the subset
int** adjacency_matrix; // adjacency matrix to store the graph
int* solution; // array to store the current solution

struct iterationSolution current_best_round, best_final_round;
int invalid_solutions = 0; // total number of invalid solutions;
int current_generation = 0;



int iterationsEvolutionary = 100;

struct populationSettings parameters = {
        100,
        0.0,
        0.3,
        2,
        0,
        0,
        0,
        2500
};

int verifyAux(int *array_k_size, int k,int elements){
    for(int i= 0; i < k; i++){
        if(array_k_size[i] != elements){
            return 0;
        }
    }
    return 1;
}

void displaySolution(struct iterationSolution solution){
    printf("Solution: ");

    for(int i = 0; i < vertices; i++){
        printf("%d ", solution.solution[i]);
    }

    printf("Fitness: %d\n", solution.fitness);
    printf("Valid: %d\n", solution.valid);
}

struct iterationSolution* generateInitialPopulation(struct populationSettings *settings){
    struct iterationSolution* current;
    int elements = settings->capacity / settings->subgroupNumber;
    int sub_conj = settings->subgroupNumber - 1;

    int aux[settings->subgroupNumber];
    int n_random;

    for(int i = 0; i < settings->subgroupNumber; i++){
        aux[i] = 0;
    }

    //allocate memory for the population array
    current = (struct iterationSolution*) malloc(settings->size * sizeof(struct iterationSolution));
    if(current == NULL){
        printf("Error allocating memory for the new individual.\n");
        exit(1);
    }

    int j,x;
    for(x = 0; x < settings->size; x++){
        for(j = 0; j < settings->capacity; j++){
            // TODO: generate the initial solution for each individual in the population
        }


        for(int i = 0; i < settings->subgroupNumber; i++){
            aux[i] = 0;
        }

    }
    return current;
}

// function to calculate the number of edges in a given subset
void code_execution(){

    current_generation++; // starts at 0
    printf("Generation: %d\n", current_generation);

    //generate initial population ( population is an array of iterationSolution )
    population currentPopulation = generateInitialPopulation(&parameters); // generate initial population ( list of iterationSolutions )
    printf("Initial population generated.\n");
    displaySolution(currentPopulation[0]);

    //evaluate population ( fitness)

    while(current_generation <= parameters.generationsNumber){
        //binary tournament

        //apply genetic operators

        //crossover

        //mutation

        //evaluate the new population

        //update the best solution found so far

        //increment the generation counter
        current_generation++;
    }

    // count the invalid solutions found

    // print the best solution found so far

    // print the median of the invalid solutions found

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

    init(filename, &k, &vertices, &edges, &adjacency_matrix);
    //displayAdjacencyMatrix(vertices, adjacency_matrix);

    parameters.capacity = vertices;
    parameters.subgroupNumber = k;

    printf(" > Capacity: %d\n", parameters.capacity);
    printf(" > SubgroupNumber: %d\n", parameters.subgroupNumber);

    for(int i = 0; i < executions; i++){
        code_execution();
    }

    printf(" =============================== \n");
   // printf("> Best solution with the best cost: %d\n", best_solution_cost);
    //printf("> Best solution: ");
    //for(int i = 0; i < vertices; i++){
    //    printf("%d ", best_solution[i]);
    //}
    //printf("\n");

    // clean up
    for(int i = 0; i < vertices; i++){
        free(adjacency_matrix[i]);
    }

    free(adjacency_matrix);

    return 0;
}