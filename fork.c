#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

    // Create a new process and save its PID
    pid_t pid;
    pid = fork();
    int status;

 if (pid < 0) {
     // An error has occured while trying to fork
    perror("Fork error.\n");
    return 1;
}   else if (pid == 0) {
    // This is the child process
    printf("Computer Science (PID %d)\n", getpid());

    // Sleep to simulate some work
    sleep(6);
    printf("Child process has completed its work.\n");
} else {
    // This is the parent process
    printf("Chris Dodge (PID %d)\n", getpid());

    // Sleep to simulate some work
    sleep(5);
    printf("Parent process has completed its work.\n");

    // Wait for child to finish running
    waitpid(pid, &status, 0);

    // Print that the child has exited
    printf("Child process has exited the status %d.\n", WEXITSTATUS(status));
   }

   return 0;

}

