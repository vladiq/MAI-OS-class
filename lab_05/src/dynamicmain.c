#include <dlfcn.h>

#include "queue.h" 

int main(void)
{
    Node* (*Initialize)(double val);
    void (*PushFront)(Node* head, double data);
    void (*PopBack)(Node** head);
    void (*Destroy)(Node** head);
    void (*QueuePrint)(Node* head);

    void* libHandle;
    libHandle = dlopen("./libqueue.so", RTLD_LAZY);
    if (!libHandle) {
        perror("dlopen error");
        exit(-1);
    }

    Initialize = (Node (*)(void))dlsym(libHandle, "Initialize");
    PushFront = (void (*)(void))dlsym(libHandle, "PushFront");
    PopBack = (void (*)(void))dlsym(libHandle, "PopBack");
    Destroy = (void (*)(void))dlsym(libHandle, "Destroy");
    QueuePrint = (void (*)(void))dlsym(libHandle, "QueuePrint");

    int act = 0;
    double key = 0;
    Node* queue = NULL;

    printf("Choose an operation:\n");
    printf("1) Initialize the queue\n");
    printf("2) Push front\n");
    printf("3) Pop back\n");
    printf("4) Print queue\n");
    printf("0) Exit\n");

    while (scanf("%d", &act) && act) {
        switch(act) {
        case 1:
            printf("Enter key to initialize the queue: ");
            scanf("%lf", &key);
            queue = (*Initialize)(key);
            break;

        case 2:
            printf("Enter key: ");
            scanf("%lf", &key);
            (*PushFront)(queue, key);
            break;

        case 3:
            (*PopBack)(&queue);
            break;

        case 4:
            (*QueuePrint)(queue);
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
    (*Destroy)(&queue);

    if (dlclose(libHandle) != 0) {
        perror("dlclose error");
        exit(-1);
    }

    return 0;
}
