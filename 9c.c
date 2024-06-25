#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pfds[2];
    char buffer[100];
    pid_t pid;

    if (pipe(pfds) == -1) {
        perror("Pipe is not created");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) { // Child process
        close(pfds[1]); // Close the write end of the pipe

        printf("Child printing received data\n");
        int n = read(pfds[0], buffer, 100);
        write(1, buffer, n); // Write to stdout

        close(pfds[0]); // Close the read end of the pipe in the child
    } else { // Parent process
        close(pfds[0]); // Close the read end of the pipe

        printf("Parent passing value to child\n");
        write(pfds[1], "Hello\n", 6); // Write to the pipe

        close(pfds[1]); // Close the write end of the pipe in the parent

        wait(NULL); // Wait for the child process to finish
    }

    return 0;
}
