#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int random_l_h(int min, int max)
{
    return min + rand() % (max - min + 1);
}

// display the best solution found so far
void displayBestSolution(int k, int* best_solution, int best_solution_cost){
    printf("Best solution found: ");
    for(int i = 0; i < k; i++){
        printf("%d ", best_solution[i]);
    }
    printf("\nMax number of edges: %d\n", best_solution_cost);
}

// display the adjacency matrix
void displayAdjacencyMatrix(int vertices, int** adjacency_matrix){
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
void init(char* filename, int* k, int* vertices, int* edges, int*** adjacency_matrix){
    FILE *fp = fopen(filename, "r");

    if(fp == NULL) {
        printf("Error opening the file provided\n");
        exit(1);
    }

    char line[100];
    fgets(line, 100, fp);
    char *token = strtok(line, " ");
    token = strtok(NULL, " ");
    int new_k = atoi(token);

    fgets(line, 100, fp);
    token = strtok(line, " ");
    token = strtok(NULL, " ");
    token = strtok(NULL, " ");
    int target_vertices = atoi(token);
    token = strtok(NULL, " ");
    int target_edges = atoi(token);

    printf(" > k = %d\n",new_k);
    printf(" > vertices = %d, edges = %d\n", target_vertices, target_edges);

    int** new_adjacency_matrix;
    new_adjacency_matrix = (int**)malloc(target_vertices * sizeof(int*));
    if(new_adjacency_matrix == NULL){
        printf("Error allocating memory for the adjacency matrix.\n");
        exit(1);
    }

    for(int i = 0; i < target_vertices; i++){
        new_adjacency_matrix[i] = (int*)malloc(target_vertices * sizeof(int));
        if(new_adjacency_matrix[i] == NULL){
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
        new_adjacency_matrix[u][v] = new_adjacency_matrix[v][u] = 1;
    }
    fclose(fp);

    *k = new_k;
    *vertices = target_vertices;
    *edges = target_edges;
    *adjacency_matrix = new_adjacency_matrix;
}

// function to display the current solution
void displayCurrentSolution(int vertices, int* solution){
    printf("    > Current solution: ");
    for(int i = 0; i < vertices; i++){
        printf("%d ", solution[i]);
    }
    printf("\n");
}

// function to generate the initial solution
void generateInitialSolution(int vertices, int k, int** solution){

    int* new_solution = malloc(sizeof(int)*vertices);
    if(new_solution == NULL){
        printf("Error allocating memory for the solution.\n");
        exit(1);
    }

    for(int i = 0; i < vertices; i++){
        new_solution[i] = 0;
    }

    //generate k random numbers between 0 and vertices-1 and set them to 1 in the solution
    for(int i = 0; i < k; i++){
        int random_number = rand() % vertices;
        new_solution[random_number] = 1;
    }

    // verify the initial solution have 4 random numbers
    int count = 0;
    for(int i = 0; i < vertices; i++){
        if(new_solution[i] == 1){
            count++;
        }
    }

    while(count != k){
        int random_number = rand() % vertices;
        new_solution[random_number] = 1;
        count = 0;
        for(int i = 0; i < vertices; i++){
            if(new_solution[i] == 1){
                count++;
            }
        }
    }

    if(count != k){
        printf("    > Error: the initial solution does not have k = %d random numbers.\n", k);
    }

    *solution = new_solution;

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


void replace(int a[], int b[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        a[i] = b[i];
}