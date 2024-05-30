#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Node structure for the queue
typedef struct QueueNode {
    struct Lattice* data;
    int level;
    struct QueueNode* next;
} QueueNode;

// Queue structure
typedef struct {
    QueueNode* front; // Pointer to the front of the queue
    QueueNode* rear;  // Pointer to the rear of the queue
    int size;         // Current number of elements in the queue
} Queue;

// Function to create an empty queue
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    queue->size = 0;
    return queue;
}

// Function to check if the queue is empty
bool is_empty(Queue* queue) {
    return queue->front == NULL;
}

// Function to enqueue (add) an element to the rear of the queue
void enqueue(Queue* queue, struct Lattice* data) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->data = data;
    newNode->next = NULL;

    if (is_empty(queue)) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    queue->size++;
}

// Function to dequeue (remove) an element from the front of the queue
QueueNode* dequeue(Queue* queue) {
    if (is_empty(queue)) {
        printf("Error: Queue is empty\n");
        return NULL; // Or handle the error differently
    }

    QueueNode* temp = queue->front;
    struct Lattice* data = temp->data;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL; // Queue becomes empty
    }
    free(temp);
    queue->size--;
    return temp;
}

// Lattice structure
typedef struct Lattice {
    int x, y;
    int value;
    bool north;
    bool south;
    bool east;
    bool west;
} Lattice;

// Matrix structure
typedef struct {
    int num_points;
    Lattice** all_lattice_points;
    Lattice* root[4];
} Matrix;

#define GRID_SIZE 5 // Example grid size

Lattice* createLattice(int x, int y) {
    Lattice* newLattice = (Lattice*)malloc(sizeof(Lattice));
    newLattice->x = x;
    newLattice->y = y;
    newLattice->value = 0;
    newLattice->north = (y == GRID_SIZE-1) ? false : true;
    newLattice->south = (y == 0) ? false : true;
    newLattice->east = (x == GRID_SIZE-1) ? false : true;
    newLattice->west = (x == 0) ? false : true;
    return newLattice;
}

int findLattice(Matrix* matrix, int x, int y) {
    for (int i = 0; i < matrix->num_points; i++) {
        if (matrix->all_lattice_points[i]->x == x && matrix->all_lattice_points[i]->y == y) {
            return matrix->all_lattice_points[i];
        }
    }
    return NULL;
}

int grid_is_even() {
    return GRID_SIZE % 2 == 0;
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

void display_queue(Queue* queue){
        // Displaying the nodes
        QueueNode* curr_node = queue->front;  // Initialize curr_node to the front of the queue

        if (curr_node == NULL) {
            printf("Queue is empty.\n\n");
        } else {
            while (curr_node != queue->rear) {
                printf("x: %d, y: %d, val: %d\n", curr_node->data->x, curr_node->data->y, curr_node->data->value);
                curr_node = curr_node->next;
            }
            // Print the rear node
            if (curr_node == queue->rear) {
                printf("x: %d, y: %d, val: %d\n", curr_node->data->x, curr_node->data->y, curr_node->data->value);
            }
            printf("\n\n");
        }
}

void flood_fill(Matrix* matrix){

    // Creating a Queue
    Queue* queue = createQueue();
    
    // Initializing the Queue
    if (grid_is_even() == 1) {
        enqueue(queue,matrix->root[0]);
        enqueue(queue,matrix->root[1]);
        enqueue(queue,matrix->root[2]);
        enqueue(queue,matrix->root[3]);
    }
    else
    {
        enqueue(queue,matrix->root[0]);
    }
    
    QueueNode* curr_queue_node = queue->front;
    int level = curr_queue_node->data->value+1;
    Lattice* temp_Lattice;
    while(is_empty(queue) != true){
        curr_queue_node = dequeue(queue);
        
        if(curr_queue_node->data->north == true)
        {   
            temp_Lattice = findLattice(matrix,curr_queue_node->data->x,curr_queue_node->data->y + 1);
            if(temp_Lattice != NULL)
            {
                if(curr_queue_node->data->value == 0)  //Checking if node is visited or not
                {
                    enqueue(queue,temp_Lattice);
                    temp_Lattice->value = curr_queue_node->data->value + 1;
                }
            }
        }
        
        display_queue(queue);
        
    }

}

int main() {
    // Create the matrix
    Matrix* myMatrix = createMatrix();
    flood_fill(myMatrix);
    showMatrix(myMatrix);
    return 0;
}