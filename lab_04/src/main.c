#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>


const int ARG_MAX = 2097152 + 1;

int main(int argc, char** argv) {
    pid_t pid;
    int rv, fd;

    char* filename = "memfile";

    if ((fd = shm_open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1) {
        perror("shm_open error");
        exit(-1);
    }

    if (ftruncate(fd, ARG_MAX * 2) == -1) {
        perror("failed to truncate");
        exit(-1);
    }

    char* memory = mmap(NULL, ARG_MAX * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (memory == MAP_FAILED) {
        perror("mapping error");
        fprintf(stderr, "%p", memory);
        exit(-1);
    }

    if ((pid = fork()) < 0) {
        perror("fork error");
        exit(-1);

    } else if (pid == 0) {

        dup2(fd, STDOUT_FILENO);

        rv = execvp(argv[1], argv + 1);
        if (rv) {
            perror("exec error");
        }
        exit(rv);

    } else if (pid > 0) {
        waitpid(pid, &rv, 0);

        for (int i = 0; memory[i] != '\0'; ++i) {
            if (islower(memory[i])) {
                putchar(toupper(memory[i]));
            } else {
                putchar(memory[i]);
            }
        }

        exit(WEXITSTATUS(rv));
    }

    if (munmap(memory, ARG_MAX * 2)) {
        perror("munmap error");
        exit(-1);
    }

    return 0;
}
