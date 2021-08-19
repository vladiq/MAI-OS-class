#include <assert.h>
#include "queue.h"

Node* Initialize(double val) {
    Node* res = (Node*)malloc(sizeof(Node));
    res->data = val;
    res->next = NULL;
    return res;
}

void PushFront(Node* head, double data) {
    assert(head != NULL);
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->next = head->next;
    head->next = newNode;
    newNode->data = head->data;
    head->data = data;
}


void PopBack(Node** head) {
    assert(head != NULL);
    if ((*head)->next == NULL) {
        Destroy(head);
        return;
    }
    Node* current = *head;
    while (current->next->next != NULL) {
        current = current->next;
    }
    free(current->next);
    current->next = NULL;
}

void QueuePrint(Node* head) {
    assert (head != NULL);
    Node* current = head;
    while (current) {
        printf("%lf ", current->data);
        current = current->next;
    }
    putchar('\n');
}


void Destroy(Node** head)  { 
    Node* current = *head;
    Node* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}
