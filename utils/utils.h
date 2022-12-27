
// display the best solution found so far
void displayBestSolution(int k, int* best_solution, int best_solution_cost);
// display the adjacency matrix
void displayAdjacencyMatrix(int vertices, int** adjacency_matrix);

// function to read the graph from the input file
void init(char* filename, int* k, int* vertices, int* edges, int*** adjacency_matrix);

// function to display the current solution
void displayCurrentSolution(int vertices, int* solution);

// function to generate the initial solution
void generateInitialSolution(int vertices, int k, int** solution);

int calculate_fit(int a[], int **mat, int vert);

int random_l_h(int min, int max);

void replace(int a[], int b[], int n);