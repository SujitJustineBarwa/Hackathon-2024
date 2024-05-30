// matrix.c
#include "lattice.h"
#include <stdlib.h>
#include <stdio.h>
#define GRID_SIZE 5

int grid_is_even(void);
int grid_is_even (){
    if (GRID_SIZE %2 == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

typedef struct {
    int num_points;
    Lattice** all_lattice_points;
    Lattice* root[4];
} Matrix;

int findLattice_by_idx(Matrix*, int , int );
int findLattice_by_idx(Matrix* matrix, int x, int y) {
    for (int i = 0; i < matrix->num_points; i++) {
        if (matrix->all_lattice_points[i]->x == x && matrix->all_lattice_points[i]->y == y) {
            return i;
        }
    }
    return -1; 
}

Matrix* createMatrix() {
    Matrix* newMatrix = (Matrix*)malloc(sizeof(Matrix));
    newMatrix->num_points = GRID_SIZE * GRID_SIZE;
    newMatrix->all_lattice_points = (Lattice**)malloc(newMatrix->num_points * sizeof(Lattice*));

    // Generate lattice points
    int index = 0;
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            newMatrix->all_lattice_points[index] = createLattice(x, y);

            if (grid_is_even() == 1) {
                if (x == GRID_SIZE / 2 && y == GRID_SIZE / 2) {
                    newMatrix->root[0] = newMatrix->all_lattice_points[index];
                    newMatrix->root[1] = newMatrix->all_lattice_points[findLattice_by_idx(newMatrix, x - 1, y)];
                    newMatrix->root[2] = newMatrix->all_lattice_points[findLattice_by_idx(newMatrix, x, y - 1)];
                    newMatrix->root[3] = newMatrix->all_lattice_points[findLattice_by_idx(newMatrix, x - 1, y - 1)];
                }
            } else {
                if (x == GRID_SIZE / 2 && y == GRID_SIZE / 2) {
                    newMatrix->root[0] = newMatrix->all_lattice_points[index];
                    newMatrix->root[1] = NULL;
                    newMatrix->root[2] = NULL;
                    newMatrix->root[3] = NULL;
                }
            }
            index++;
        }
    }

    return newMatrix;
}

// Function to show the matrix (updated to include value)
void showMatrix(Matrix* matrix) {
    for (int y = GRID_SIZE - 1; y >= 0; y--) { // Print from top to bottom
        for (int x = 0; x < GRID_SIZE; x++) {
            int idx = findLattice_by_idx(matrix, x, y);
            if (idx != -1) {
                Lattice* current = matrix->all_lattice_points[idx];
                printf("%d ", current->value);
            } else {
                printf("NULL ");
            }
        }
        printf("\n");
    }
}

int main() {
    // Create the matrix
    Matrix* myMatrix = createMatrix();
    //flood_fill(myMatrix);
    showMatrix(myMatrix);
    return 0;
}