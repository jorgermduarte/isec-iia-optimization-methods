#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

int executions = 0;
int vertices, edges; // number of vertices and edges
int k; // size of the subset
int** adjacency_matrix; // adjacency matrix to store the graph
int* solution; // array to store the current solution
int* best_solution; // array to store the best solution found so far
int best_solution_cost; // value of the best solution found so far

int iterationsHillClimbing = 1000;
bool displaySolutions = false;

// display the best solution found so far
void displayBestSolution(){
    printf("Best solution found: ");
    for(int i = 0; i < k; i++){
        printf("%d ", best_solution[i]);
    }
    printf("\nMax number of edges: %d\n", best_solution_cost);
}

// display the adjacency matrix
void displayAdjacencyMatrix(){
    printf("====================================\n");
    for(int i = 0; i < vertices; i++){
        printf("   ");
        for(int j = 0; j < vertices; j++){
            printf("%d ", adjacency_matrix[i][j]);
        }
        printf("\n");
    }
    printf("====================================\n");
}

// function to read the graph from the input file
void init(char* filename){
    FILE *fp = fopen(filename, "r");

    if(fp == NULL) {
        printf("Error opening the file provided\n");
        exit(1);
    }

    char line[100];
    fgets(line, 100, fp);
    char *token = strtok(line, " ");
    token = strtok(NULL, " ");
    k = atoi(token);

    fgets(line, 100, fp);
    token = strtok(line, " ");
    token = strtok(NULL, " ");
    token = strtok(NULL, " ");
    vertices = atoi(token);
    token = strtok(NULL, " ");
    edges = atoi(token);

    printf(" > k = %d\n",k);
    printf(" > vertices = %d, edges = %d\n", vertices, edges);

    adjacency_matrix = (int**)malloc(vertices * sizeof(int*));
    if(adjacency_matrix == NULL){
        printf("Error allocating memory for the adjacency matrix.\n");
        exit(1);
    }

    for(int i = 0; i < vertices; i++){
        adjacency_matrix[i] = (int*)malloc(vertices * sizeof(int));
        if(adjacency_matrix[i] == NULL){
            printf("Error allocating memory for the adjacency matrix.\n");
            exit(1);
        }
    }

    // read the other lines and set the edges values
    while(fgets(line, 100, fp) != NULL){

        int u, v;
        token = strtok(line, " ");
        token = strtok(NULL, " ");
        u = atoi(token) ;
        token = strtok(NULL, " ");
        v = atoi(token) ;
        //printf("    > e %d %d\n", u, v);
        u--; v--; // the vertices in the input file are 1-indexed, so we need to convert them to 0-indexed
        // set the connection in the vector
        adjacency_matrix[u][v] = adjacency_matrix[v][u] = 1;
    }
    fclose(fp);
}

// function to display the current solution
void displayCurrentSolution(){
    printf("    > Current solution: ");
    for(int i = 0; i < vertices; i++){
        printf("%d ", solution[i]);
    }
    printf("\n");
}

// function to generate the initial solution
void generateInitialSolution(){

    solution = malloc(sizeof(int)*vertices);
    if(solution == NULL){
        printf("Error allocating memory for the solution.\n");
        exit(1);
    }

    for(int i = 0; i < vertices; i++){
        solution[i] = 0;
    }

    //generate k random numbers between 0 and vertices-1 and set them to 1 in the solution
    for(int i = 0; i < k; i++){
        int random_number = rand() % vertices;
        solution[random_number] = 1;
    }

    // verify the initial solution have 4 random numbers
    int count = 0;
    for(int i = 0; i < vertices; i++){
        if(solution[i] == 1){
            count++;
        }
    }

    while(count != k){
        int random_number = rand() % vertices;
        solution[random_number] = 1;
        count = 0;
        for(int i = 0; i < vertices; i++){
            if(solution[i] == 1){
                count++;
            }
        }
    }

    if(count != k){
        printf("    > Error: the initial solution does not have k = %d random numbers.\n", k);
    }

}

int calculate_fit(int a[], int **mat, int vert)
{
    int total = 0;
    int i, j;

    for (i = 0; i < vert; i++)
        if (a[i] == 1)
        {
            for (j = 0; j < vert; j++){
                if (a[j] == 1 && mat[i][j] == 1){
                    total++;
                }
            }
        }
    return total / 2;
}

int random_l_h(int min, int max)
{
    return min + rand() % (max - min + 1);
}

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


void replace(int a[], int b[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        a[i] = b[i];
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

// function to calculate the number of edges in a given subset
void code_execution(){
    generateInitialSolution();

    int current_cost = hill_climbing(solution, adjacency_matrix, vertices, iterationsHillClimbing);

    if(displaySolutions){
        displayCurrentSolution();
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

    init(filename);
   //displayAdjacencyMatrix();

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