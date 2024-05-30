#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lattice.h"

// Node structure for the queue
typedef struct QueueNode {
    Lattice* data;
    int level;
    struct QueueNode* next;
} QueueNode;

// Queue structure
typedef struct {
    QueueNode* front; // Pointer to the front of the queue
    QueueNode* rear;  // Pointer to the rear of the queue
    int size;        // Current number of elements in the queue
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
void enqueue(Queue* queue, Lattice* data, int level) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->data = data;
    newNode->level = level;
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
    Lattice* data = temp->data;
    int level = temp->level;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL; // Queue becomes empty
    }
    free(temp);
    queue->size--;
    return temp;
}