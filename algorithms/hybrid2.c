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
    int tournamentSize;     // Tournament size ?? [NOT USED]
    int generationsToSim;   // Number of generations to simulate ( not defined ? )
    int currentGeneration;  // dont change
    int executions;         // we receive this from the command line [dont change]

    // Probabilities
    float mutationProbability;
    float recombinationProbability;
    Individual BestSolution;

    int iterationsHillClimbing;

}SimulationState;

float executionsMedianFitness = 0;

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
    int pos1, pos2;

    if (rand_01() < simulationState->mutationProbability) {
        pos1 = random_l_h(0, simulationState->genes - 1); // grabs a random position in the solution
        pos2 = random_l_h(0, simulationState->genes - 1); // grabs a random position in the solution

        int currentVal = solution[pos1];
        solution[pos1] = solution[pos2];
        solution[pos2] = currentVal;
    }
}

//Runs the main algorithm
void code_execution(SimulationState *simulationState, int **adjacency_matrix) {
    // Initial Population
    // this initial population does not include the reparation functions and flips
    // this is because all solutions are valid
    Individual *population = generateInitialPopulation(simulationState);

    // Evaluate entire Population (fitness)
    evaluate_population(population, simulationState, adjacency_matrix);

    // define the best solution found so far ( population initialized)
    Individual best = population[0];
    for(int i = 1; i < simulationState->populationSize; i++)
        if (population[i].fitness >= best.fitness)
            best = population[i];

    simulationState->currentGeneration = 0;
    simulationState->BestSolution.fitness = 0;

    do {
        // Malloc for parents
        Individual *parents = (Individual *) malloc(simulationState->populationSize * sizeof(Individual));
        for (int i = 0; i < simulationState->populationSize; i++){
            parents[i].solution = malloc(sizeof(int) * simulationState->genes);
            parents[i].fitness = 0;
            parents[i].isValid = 0;
        }

        // Binary Tournament Selection
        tournament_selection(population, simulationState, parents);

        // Recombination
        recombinacao_uniforme(population, simulationState, parents);

        // Mutation
        for (int i = 0; i < simulationState->populationSize; i++)
            mutacao_por_troca(population[i].solution, simulationState);

        // Evaluate entire Population (fitness)
        evaluate_population(population, simulationState, adjacency_matrix);

        // Update best solution
        for(int i = 0; i < simulationState->populationSize; i++)
            if (population[i].fitness >= best.fitness && population[i].isValid)
                best = population[i];

        // Free parents
        for (int i = 0; i < simulationState->populationSize; i++)
            free(parents[i].solution);

        free(parents);


        // Define the best solution found so far after the current generation
        if (best.fitness >= simulationState->BestSolution.fitness && best.isValid) {
            simulationState->BestSolution = best;
        }

        // hill climbing algorithm (local search) - inside the evolutionary!
        for (int i = 0; i < simulationState->populationSize; i++) {
            if(population[i].isValid){
                //printf("Hill Climbing for individual %d\n", i);
                int current_cost = hill_climbing(population[i].solution, adjacency_matrix, simulationState->genes, simulationState->iterationsHillClimbing);
                if (current_cost > best.fitness) {
                    best = population[i];
                }
            }
        }

        // Define the best solution found so far after the current generation
        if (best.fitness >= simulationState->BestSolution.fitness && best.isValid) {
            simulationState->BestSolution = best;
        }

        // Update current generation
        simulationState->currentGeneration++;

    } while (simulationState->currentGeneration <= simulationState->generationsToSim);

    free(population);
    fflush(stdout);
}


int main(int argc, char *argv[]) {
    // Data
    int **adjacency_matrix;
    SimulationState simulationState = {
            .populationSize = 100, // you can change this value
            .iterationsHillClimbing = 10, // you can change this value
            .tournamentSize = 2, // you can change this value
            .generationsToSim = 1000, // you can change this value
            .mutationProbability = 0.5f, // you can change this value
            .recombinationProbability = 0.9f, // you can change this value
            .genes = 0,
            .k = 0,
            .currentGeneration = 0,
            .executions = 0,
            .BestSolution = {.fitness = 0, .isValid = 0, .solution = NULL},
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
    //displayAdjacencyMatrix(vertices, adjacency_matrix);

    simulationState.genes = vertices;
    simulationState.BestSolution.solution = (int *) malloc(sizeof(int) * vertices);

    printf(" > NumOfGenes: %d\n", simulationState.genes);
    printf(" > SubgroupNumber: %d\n", simulationState.k);

    printf(" > Hill Climbing iterations per individual:  %d\n", simulationState.iterationsHillClimbing);

    for (int i = 0; i < simulationState.executions; i++) {
        printf("=============================== \n");
        printf("Execution : %d\n", i + 1);
        code_execution(&simulationState, adjacency_matrix);
        displaySolution(&simulationState.BestSolution, &simulationState);

        // add to calculate the average later
        executionsMedianFitness += simulationState.BestSolution.fitness;

        // Reset the best solution
        simulationState.BestSolution.fitness = 0;
        simulationState.BestSolution.isValid = 0;
        for (int j = 0; j < simulationState.genes; j++)
            simulationState.BestSolution.solution[j] = 0;



    }
    printf(" =============================== \n");

    printf(" > Average Fitness: %f\n", executionsMedianFitness / simulationState.executions);

    // clean up
    for (int i = 0; i <= vertices; i++) {
        free(adjacency_matrix[i]);
    }

    free(adjacency_matrix);
    free(simulationState.BestSolution.solution);

    return 0;

}