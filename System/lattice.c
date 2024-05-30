// lattice.c
#include "lattice.h"
#include <stdlib.h>

Lattice* createLattice(int x, int y) {
    Lattice* newLattice = (Lattice*)malloc(sizeof(Lattice));
    newLattice->x = x;
    newLattice->y = y;
    newLattice->value = 0;
    for(int i = 0;i<4;i++){
        newLattice->children[i] = NULL;
    }
    return newLattice;
}