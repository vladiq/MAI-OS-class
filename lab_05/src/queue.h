#ifndef D_QUEUE_H
#define D_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node {
    double data;
    struct node* next;
} Node;

Node* Initialize(double val);
void PushFront(Node* head, double data);
void PopBack(Node** head);
void Destroy(Node** head);
void QueuePrint(Node* head);

#endif // D_QUEUE_H