#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <signal.h>

#define BUFFERSIZE 4096

pid_t child_pid;


void ParentRoutine(std::string &filepath) {
    int fd;
    char* mapped;
    
    fd = open(filepath.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Ошибка open");
        exit(1);
    }
    
    if (ftruncate(fd, BUFFERSIZE) == -1) {
        perror("Ошибка ftruncate");
        close(fd);
        exit(1);
    }
    
    mapped = (char*)mmap(NULL, BUFFERSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("Ошибка mmap");
        close(fd);
        exit(1);
    }
    close(fd);
    
    child_pid = fork();
    if (child_pid == -1) {
        perror("Ошибка fork()");
        munmap(mapped, BUFFERSIZE);
        exit(1);
    } else if (child_pid == 0) {        
        signal(SIGUSR1, [](int){});
        pause(); 

        char filesize_str[10];
        sprintf(filesize_str, "%d", BUFFERSIZE);
        const char* args[] = {"./child", filepath.c_str(), filesize_str, NULL};

        execve("./child", (char* const*)args, NULL);
        perror("Ошибка execve");
        exit(1);
    } else {
        char input_data[BUFFERSIZE];
        printf("Введите числа через пробел: ");
        fgets(input_data, sizeof(input_data), stdin);
        memcpy(mapped, input_data, BUFFERSIZE);        
        kill(child_pid, SIGUSR1);
        wait(NULL);
        munmap(mapped, BUFFERSIZE);
    }
}
