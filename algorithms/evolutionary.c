#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../utils/utils.h"

typedef struct Individual {
    int *solution;
    int fitness;
    int isValid;
}Individual;

typedef struct SimulationState {
    // Sizes
    int populationSize;
    int genes;              // Number of vertices + 1
    int k;                  // Subgroup size
    int tournamentSize;
    int generationsToSim;
    int currentGeneration;
    int executions;

    // Probabilities
    float mutationProbability;
    float recombinationProbability;
    Individual BestSolution;
}SimulationState;


//Generates a float random number 0-1
float rand_01() {
    return (float) rand() / (float) RAND_MAX;
}

//Compares two individuals based on their validity and fitness
int compare_individual(const void *lhs, const void *rhs) {
    const Individual *p1 = lhs;
    const Individual *p2 = rhs;

    if (p1->isValid == 0)
        return -1;

    if (p1->fitness < p2->fitness)
        return -1;

    return 1;
}

//Generates the initial population given the simulation parameters
Individual *generateInitialPopulation(SimulationState *simulationState) {

    Individual *current;

    //allocate memory for the population array
    current = (Individual *) malloc(simulationState->populationSize * sizeof(Individual));
    if (current == NULL) {
        printf("Error allocating memory for the new individual.\n");
        exit(1);
    }

    for (int i = 0; i < simulationState->populationSize; i++) {
        generateInitialSolution(simulationState->genes, simulationState->k, &current[i].solution);
    }
    return current;
}

//Prints the solution and its associated data
void displaySolution(Individual *individual, SimulationState *simulationState) {
    printf("Solution: ");

    for (int i = 0; i < simulationState->genes; i++) {
        printf("%d ", individual->solution[i]);
    }

    printf("\nFitness: %d\n", individual->fitness);
    printf("Valid: %d\n", individual->isValid);
}

//Compares the number of edges with target k value
int evaluate_individual(const int *solution, int num_vertices, int k) {
    int count = 0;
    for (int i = 0; i < num_vertices; i++) {
        if (solution[i] == 1) {
            count++;
        }
    }
    if (count == k)
        return 1;
    return 0;
}

//Evaluates a given population and sorts them in ascending order of fitness
void evaluate_population(Individual *population, SimulationState *simulationState, int **adjacency_matrix) {
    for (int i = 0; i < simulationState->populationSize; i++) {
        population[i].isValid = evaluate_individual(population[i].solution, simulationState->genes, simulationState->k);
        if (population[i].isValid)
            population[i].fitness = calculate_fit(population[i].solution, adjacency_matrix, simulationState->genes);
        else
            population[i].fitness = -1;
    }
    qsort(population, simulationState->populationSize, sizeof(Individual), compare_individual);
}

//Selects the fittest population from the given population for being parents for the next generation
void tournament_selection(Individual *population, SimulationState *simulationState, Individual *parents) {
    int i, x1, x2;

    for (i = 0; i < simulationState->populationSize; i++) {
        x1 = random_l_h(0, simulationState->populationSize - 1);
        do
            x2 = random_l_h((simulationState->populationSize - 1) / 2, simulationState->populationSize - 1);
        while (x1 == x2);
        if (population[x1].fitness > population[x2].fitness)
            replace(parents[i].solution, population[x1].solution, simulationState->genes);
        else
            replace(parents[i].solution, population[x2].solution, simulationState->genes);
    }
}

void reparacao(int *solution, SimulationState *simulationState) {
    int count = 0;

    for (int i = 0; i < simulationState->genes; i++) {
        if (solution[i])
            count++;
    }

    while (count < simulationState->k) {
        int p = random_l_h(0, simulationState->genes - 1);
        if (solution[p] == 0) {
            solution[p] = 1;
            count++;
        }
    }

    while (count > simulationState->k) {
        int p = random_l_h(0, simulationState->genes - 1);
        if (solution[p] == 1) {
            solution[p] = 0;
            count--;
        }
    }
}

//Generates new population from parent population
void crossover(Individual *population, SimulationState *simulationState, Individual *parents) {
    int i, j, point;
    int max = simulationState->populationSize;
    if (max % 2 != 0)
        max--;

    for (i = 0; i < max; i += 2) {
        if (rand_01() < simulationState->recombinationProbability) {
            point = random_l_h(0, simulationState->genes - 1);
            for (j = 0; j < point; j++) {
                population[i].solution[j] = parents[i].solution[j];
                population[i + 1].solution[j] = parents[i + 1].solution[j];
            }
            for (j = point; j < simulationState->genes; j++) {
                population[i].solution[j] = parents[i + 1].solution[j];
                population[i + 1].solution[j] = parents[i].solution[j];
            }
            reparacao(population[i].solution, simulationState);
            reparacao(population[i + 1].solution, simulationState);
        } else {
            replace(population[i].solution, parents[i].solution, simulationState->genes);
            replace(population[i + 1].solution, parents[i + 1].solution, simulationState->genes);
        }
    }
}

//Generates new population from parent population - each bit is randomly chosen from both parents
void recombinacao_uniforme(Individual *population, SimulationState *simulationState, Individual *parents) {
    int i, j;
    int max = simulationState->populationSize;
    if (max % 2 != 0)
        max--;

    for (i = 0; i < max; i += 2) {
        if (rand_01() < simulationState->recombinationProbability) {
            for (j = 0; j < simulationState->genes; j++) {
                if (rand_01() > 0.5) {
                    population[i].solution[j] = parents[i].solution[j];
                    population[i + 1].solution[j] = parents[i + 1].solution[j];
                } else {
                    population[i].solution[j] = parents[i + 1].solution[j];
                    population[i + 1].solution[j] = parents[i].solution[j];
                }
            }
            reparacao(population[i].solution, simulationState);
            reparacao(population[i + 1].solution, simulationState);
        } else {
            replace(population[i].solution, parents[i].solution, simulationState->genes);
            replace(population[i + 1].solution, parents[i + 1].solution, simulationState->genes);
        }
    }
}

//Generates new population from parent population - Recombines using 2 points
void recombinacao_dois_pontos_corte(Individual *population, SimulationState *simulationState, Individual *parents) {
    int i, j, point1, point2;
    int max = simulationState->populationSize;
    if (max % 2 != 0)
        max--;

    for (i = 0; i < max; i += 2) {
        if (rand_01() < simulationState->recombinationProbability) {
            point1 = random_l_h(0, simulationState->genes - 2);
            point2 = random_l_h(point1 + 1, simulationState->genes - 1);
            for (j = 0; j < point1; j++) {
                population[i].solution[j] = parents[i].solution[j];
                population[i + 1].solution[j] = parents[i + 1].solution[j];
            }
            for (j = point1; j < point2; j++) {
                population[i].solution[j] = parents[i + 1].solution[j];
                population[i + 1].solution[j] = parents[i].solution[j];
            }
            for (j = point2; j < simulationState->genes; j++) {
                population[i].solution[j] = parents[i].solution[j];
                population[i + 1].solution[j] = parents[i + 1].solution[j];
            }

            reparacao(population[i].solution, simulationState);
            reparacao(population[i + 1].solution, simulationState);
        } else {
            replace(population[i].solution, parents[i].solution, simulationState->genes);
            replace(population[i + 1].solution, parents[i + 1].solution, simulationState->genes);
        }
    }
}

//Flips the bits based on probability and maintains k validity
void mutation(int *solution, SimulationState *simulationState) {
    for (int i = 0; i < simulationState->genes; i++) {
        float prob = rand_01();
        if (prob < simulationState->mutationProbability)
            solution[i] = !solution[i];
    }

    reparacao(solution, simulationState);
}

//mutation by exchange
void mutacao_por_troca(int *solution, SimulationState *simulationState) {
    int pos1, pos2, aux;

    if (rand_01() < simulationState->mutationProbability) {
        do
            pos1 = random_l_h(0, simulationState->genes - 1);
        while (solution[pos1] == 1);
        do
            pos2 = random_l_h(0, simulationState->genes - 1);
        while (solution[pos2] == 0);
        aux = solution[pos1];
        solution[pos1] = solution[pos2];
        solution[pos2] = aux;
    }
}

//Runs the main algorithm
void code_execution(SimulationState *simulationState, int **adjacency_matrix) {
    // Data
    Individual *population = generateInitialPopulation(simulationState);
    Individual *parents = (Individual *) malloc(simulationState->populationSize * sizeof(Individual));
    for (int i = 0; i < simulationState->populationSize; i++)
        parents[i].solution = malloc(sizeof(int) * simulationState->genes);
    simulationState->currentGeneration = 0;
    simulationState->BestSolution.fitness = -1;
    do {
        evaluate_population(population, simulationState, adjacency_matrix);
        if (population[simulationState->populationSize - 1].isValid &&
            population[simulationState->populationSize - 1].fitness > simulationState->BestSolution.fitness) {
            replace(simulationState->BestSolution.solution, population[simulationState->populationSize - 1].solution,
                    simulationState->genes);
            simulationState->BestSolution.fitness = population[simulationState->populationSize - 1].fitness;
            simulationState->BestSolution.isValid = population[simulationState->populationSize - 1].isValid;
        }
        tournament_selection(population, simulationState, parents);
        recombinacao_uniforme(population, simulationState, parents);
        for (int i = 0; i < simulationState->populationSize; i++)
            mutacao_por_troca(population[i].solution, simulationState);
        simulationState->currentGeneration++;
    } while (simulationState->currentGeneration <= simulationState->generationsToSim);
    //printf("\nGeneration: %d\n", simulationState->currentGeneration);
    displaySolution(&simulationState->BestSolution, simulationState);

    for (int i = 0; i < simulationState->populationSize; i++) {
        free(parents[i].solution);
        free(population[i].solution);
    }
    free(parents);
    free(population);
    fflush(stdout);
}


int main(int argc, char *argv[]) {
    // Data
    int **adjacency_matrix;
    SimulationState simulationState = {
            100, 0, 0, 2, 50, 0, 0, 0.5f, 0.9f, {0, 0, 0}
    };
    int edges;
    int vertices;

    if (argc != 3) {
        printf("Please provide two arguments, example: ./hill-climbing <input_file> <execution_times> \n");
        exit(1);
    }

    // Seeding rand
    srand((unsigned) time(NULL));

    char *filename = argv[1];
    char *ptr_str_tol;
    simulationState.executions = (int) strtol(argv[2], &ptr_str_tol, 10);

    printf(" > Reading input file: %s\n", filename);
    printf(" > Number of executions: %d\n", simulationState.executions);

    init(filename, &simulationState.k, &vertices, &edges, &adjacency_matrix);
    displayAdjacencyMatrix(vertices, adjacency_matrix);

    simulationState.genes = vertices;
    simulationState.BestSolution.solution = (int *) malloc(sizeof(int) * vertices);

    printf(" > NumOfGenes: %d\n", simulationState.genes);
    printf(" > SubgroupNumber: %d\n", simulationState.k);

    for (int i = 0; i < simulationState.executions; i++) {
        printf("\nExecution : %d\n", i + 1);
        code_execution(&simulationState, adjacency_matrix);
    }
    printf(" =============================== \n");

    // clean up
    for (int i = 0; i <= vertices; i++) {
        free(adjacency_matrix[i]);
    }

    free(adjacency_matrix);
    free(simulationState.BestSolution.solution);

    return 0;

}