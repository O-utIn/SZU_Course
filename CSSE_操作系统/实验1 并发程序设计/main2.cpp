#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;

#define NUM_CHILDREN 4

int main() {
    pid_t pid;
    int i;

    // 显示父进程信息
    cout << "Parent process ID: " << getpid() << endl;
    cout << "Parent is creating child processes..." << endl;

    for (i = 0; i < NUM_CHILDREN; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            return EXIT_FAILURE;
        } else if (pid == 0) {
            // 子进程
            cout << "Child process " << i + 1 << " ID: " << getpid()
                 << ", Parent ID: " << getppid() << endl;
            cout << "Child process " << i + 1 << " is doing some work..." << endl;
            return EXIT_SUCCESS;
        }
    }

    // 父进程等待所有子进程结束
    cout << "Parent is waiting for all child processes to finish..." << endl;
    for (i = 0; i < NUM_CHILDREN; i++) {
        waitpid(-1, NULL, 0);
    }
    cout << "All child processes have finished. Parent is exiting." << endl;

    return 0;
}
