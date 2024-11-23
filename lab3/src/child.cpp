#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Использование: %s <pipe_read_fd> <output_file>\n", argv[0]);
        return 1;
    }

    char* filepath = argv[1];
    int filesize = atoi(argv[2]);

    int fd = open(filepath, O_RDWR);
    if (fd == -1) {
        perror("Ошибка open (child)");
        exit(1);
    }
    
    char *mapped = (char*)mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("Ошибка mmap в child");
        close(fd);
        exit(1);
    }
    close(fd);

    float sum = 0;
    char *strNum = strtok(mapped, " ");
    while (strNum != NULL) {
        sum += atof(strNum);
        strNum = strtok(NULL, " ");
    }

    fd = open(filepath, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        perror("Ошибка open файла под запись (child)");
        exit(1);
    }

    dprintf(fd, "%f\n", sum);
    close(fd);

    return 0;
}
