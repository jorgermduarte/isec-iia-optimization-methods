//
// Created by joaok on 12/2/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "algorithm.h"

#define DEFAULT_RUNS 10

int main(int argc, char *argv[]) {

    char filename[100];
    int vert, num_iter, k, runs, cost, bestCost; //bestCost different best
    int *grafo, *sol, *best
    flot mbf = 0.0; // MEDIA BEST FOUND

    //Reads the input arguments
    if (argc == 3 ) {
        runs = atoi(argv[2]);
        strcpy(filename, argv[1]);
    } else {
        //If the number of process executions is not placed in the input arguments, it is defined with a default value
        if (argc == 2) {
            runs = DEFAULT_RUNS;
            strcpy(filename, argv[1]);
        } else {
            //If the name of the information file is not placed in the input arguments, it is requested again
            runs = DEFAULT_RUNS;
            printf("File name: ");
            gets(filename);
        }
        //If the number of process executions is less or equal to zero , shutdown the program
        if ( runs <= 0)
            return 0;
        //Initialize the generation of random numbers
        init_rand()
        //Fill the adjancency matrix , with the number of vertices to use and the number of iterations to do with the hill climbing
        grafo = init_dados(filename, &vert, &num_iter);
        //Allocates memory space to store a solution
        sol = malloc(sizeof(int)*vert);
        //Allocates memory space to store the best solution
        best = malloc(sizeof(int)*vert);
        //If it is not possible to allocate memory space, shutdown the program
        if (sol == NULL || best == NULL) {
            printf("Memory allocation error");
            exit(1);
        }
        //Makes a cycle with the number of executions defined
        for(k = 0; k < runs; k++){
            //Generates the initial solution
            initial_solution(sol, vert);
            //Shows the initial solution
            write_solution(sol, vert);
            //Executes the hill climbing algorithm
            cost = hill_climbing(grafo, sol, vert, num_iter);
            printf("\nRepeticion %d:", k);
            //Accumulates the cost of the solutions generated
            mbf += cost;
            //Save the best solution found
        if ( k == 0 || bestCost > cost) {
            bestCost = cost;
            substitute(best, sol, vert);
        }
        }
        //Calculate and show the average cost of the solutions generated
        printf("\nMBF: %f", mbf/k);
        //Show the best solution found and its cost
        printf("\nBest solution found: ");
        write_solution(best, vert);
        printf("\nCost: %d", bestCost);
        //Free the memory space allocated
        free(grafo);
        free(sol);
        free(best);
        return 0;
    }

}