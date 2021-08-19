#include "queue.h"

int main(void)
{
    int act = 0;
    double key = 0;
    Node* queue = NULL;
    printf("This is compile-time linking\n\n");
    printf("Choose an operation:\n");
    printf("1) Initialize the queue\n");
    printf("2) Push front\n");
    printf("3) Pop back\n");
    printf("4) Print queue\n");
    printf("0) Exit\n");

    while (scanf("%d", &act) && act != 0) {
        switch(act) {
        case 1:
            printf("Enter key to initialize the queue: ");
            scanf("%lf", &key);
            queue = Initialize(key);
            break;
        case 2:
            printf("Enter key: ");
            scanf("%lf", &key);
            PushFront(queue, key);
            break;
        case 3:
            PopBack(&queue);
            break;
        case 4:
            QueuePrint(queue);
            break;
        default:
            printf("Incorrect command\n");
            break;
        }
        
        printf("Choose an operation:\n");
        printf("1) Initialize the queue\n");
        printf("2) Push front\n");
        printf("3) Pop back\n");
        printf("4) Print queue\n");
        printf("0) Exit\n");
    }

    Destroy(&queue);
    return 0;
}