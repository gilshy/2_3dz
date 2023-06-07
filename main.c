#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int64_t numOfNodes;
    char** matrix;
} Graph;

int64_t countNodes(){
    FILE *file = fopen("../forgraph.txt", "r");
    int64_t numOfNodes = 0;
    while (! feof(file)) {
        if (fgetc(file) == '\n') numOfNodes++;
    }
    numOfNodes++;
    fclose(file);
    return numOfNodes;
}

Graph* initializeGraph(int64_t nodes) {
    Graph *n_g = malloc(nodes * sizeof(Graph));
    n_g -> matrix = (char**)malloc( nodes * sizeof(char*));
    n_g -> numOfNodes = nodes;
    for(int64_t i = 0; i < nodes; i++) {
        n_g -> matrix[i] = (char*)malloc( nodes * sizeof(char));
        for(int64_t j = 0; j < nodes; j++) {
            n_g->matrix[i][j] = 0;
        }
    }
    return n_g;
}

void createGraph(Graph* graph) {
    FILE *file = fopen("../forgraph.txt", "r");
    char *line = malloc(4 * sizeof(char));
    for (uint32_t i = 0; i < graph->numOfNodes; ++i){
        fgets(line,graph->numOfNodes * 4,file);
        for (uint32_t j = 0; j < graph->numOfNodes; j++) graph->matrix[i][j] = line[j*2];
    }
    fclose(file);
}

void printGraph(Graph* graph) {
    for(int64_t i = 0; i < graph -> numOfNodes; i++) {
        for(int64_t j = 0; j < graph -> numOfNodes; j++) {
            printf("%c ", graph -> matrix[i][j]);
        }
        printf("\n");
    }
}

void saveAsDot(Graph* graph) {
    FILE* file2 = fopen("../forgraph.dot", "w");
    fprintf(file2, "graph{\n");
    for(int64_t i = 0; i < graph -> numOfNodes; i++) {
        for(int64_t j = i; j < graph -> numOfNodes; j++) {
            if(graph -> matrix[i][j] == 1) {
                fprintf(file2, "%ld -- %ld;\n", i + 1, j + 1);
            }
        }
    }
    fprintf(file2, "}\n");
    fclose(file2);
    system("dot -Tpng -O ../forgraph.dot");
}

void DFS(int64_t i, Graph* graph, bool* visited) {
    visited[i] = true;
    for (int64_t j = 0; j < graph -> numOfNodes; j++) {
        if (graph -> matrix[i][j] != 0 && !visited[j]) {
            DFS(j, graph, visited);
        }
    }
}

bool isConnected(Graph *graph, int64_t node) {
    bool* visited = malloc(node * sizeof(bool));
    for (int64_t i = 0; i < graph -> numOfNodes; i++) {
        visited[i] = false;
    }
    DFS(0, graph, visited);
    for (int64_t i = 0; i < graph -> numOfNodes; i++) {
        if (!visited[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    int64_t nodes = countNodes();
    Graph *graph = initializeGraph(nodes);
    createGraph(graph);
    if (isConnected(graph, nodes)) {
        printf("The graph is connected\n");
    } else {
        printf("The graph is not connected\n");
    }
    printGraph(graph);
    saveAsDot(graph);
    return 0;
}
