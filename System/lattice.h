// lattice.h
#ifndef LATTICE_H
#define LATTICE_H

typedef struct {
    int x;
    int y;
    int value;
    struct Lattice* children[4];  // Pointers to neighbors 
} Lattice;

Lattice* createLattice(int x, int y);

#endif // LATTICE_H
