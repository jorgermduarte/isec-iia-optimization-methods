//
// Created by joaok on 12/2/2022.
//


// Read the file and return the adjancency matrix
int* data_initialize(char *nome, int *n, int *iter) { // num_iter != iter ???
    FILE    *f;
    int     *p, *q;
    int     i, j;

    f=fopen(nome, "r");
    if (!f)
    {
        printf("Erro no acesso ao ficheiro dos dados\n");
        exit(1);
    }
    // Reads the number of iterations
    fscanf(f, " %d", iter);
    // Reads the number of vertices
    fscanf(f, " %d", n);
    // Allocates memory to store the adjacency matrix
    p = malloc(sizeof(int)*(*n)*(*n));
    if (!p)
    {
        printf("Error while allocating memory\n");
        exit(1);
    }
    q = p;
    // Fill the adjacency matrix
    for (i = 0; i < *n; i++)
        for (j=0; j < *n; j++)
            fscanf(f, " %d", q++);
    fclose(f);
    // Returns the adjacency matrix
    return p;
}

// Now the function returns totalCost ,
// which is the number os links between vertices from the 2 groups
// Teacher told us to use this function to insert only ?!?!
int evaluate_solution(int a[], int *mat, int vert) {
    int i, j, totalCost = 0;

    for (i = 0 , i < vert; i++)
        if (a[i] == 0){
            for (j = 0; j < vert; j++)
                if (a[j] == 1 && *(mat + i*vert + j) == 1)
                    totalCost++;
        }
    return totalCost;
}

void substitute(int a[], int b[], int vert) {
    int i;
    for (i = 0; i < vert; i++)
        a[i] = b[i];
}