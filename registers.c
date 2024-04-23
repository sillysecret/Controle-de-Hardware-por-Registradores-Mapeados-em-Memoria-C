#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>

#define FILE_PATH "registers.bin"
#define FILE_SIZE 1024  // Same size as used in the first program

// Function to open or create the file and map it into memory
char* registers_map(const char* file_path, int file_size, int* fd) {
    *fd = open(file_path, O_RDWR | O_CREAT, 0666);
    if (*fd == -1) {
        perror("Error opening or creating file");
        return NULL;
    }

    // Ensure the file is of the correct size
    if (ftruncate(*fd, file_size) == -1) {
        perror("Error setting file size");
        close(*fd);
        return NULL;
    }

    // Map the file into memory
    char *map = mmap(0, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);
    if (map == MAP_FAILED) {
        perror("Error mapping file");
        close(*fd);
        return NULL;
    }

    return map;
}

// Function to release mapped memory and file descriptor
int registers_release(void* map, int file_size, int fd) {
    if (munmap(map, file_size) == -1) {
        perror("Error unmapping the file");
        close(fd);
        return -1;
    }

    if (close(fd) == -1) {
        perror("Error closing file");
        return -1;
    }

    return 0;
}

int main() {
    int fd;
    // Open the file and map it into memory
    char* map = registers_map(FILE_PATH, FILE_SIZE, &fd);
    if (map == NULL) {
        return EXIT_FAILURE;
    }

    unsigned short *base_address = (unsigned short *)map;

    unsigned short *r0 = base_address + 0x00;
    unsigned short *r1 = base_address + 0x01;
    unsigned short *r2 = base_address + 0x02;
    unsigned short *r3 = base_address + 0x03;

    int aux;
    unsigned int mask;

    printf("Current value of R0: 0x%02x\n", *r0);
    printf("Current value of R1: 0x%02x\n", *r1);
    do{
        printf("display on/off = 1\n");
        printf("display exib = 2,3,4,5\n");
        scanf("%d",&aux);
        if(aux == 1){
            *r0 = *r0 ^ 0x01;
        }

        
        if(aux == 2){
            //estatico
            mask = ~(0x01<<1);
            *r0 = *r0 & mask;
            mask = ~(0x01<<2);
            *r0 = *r0 & mask;
            
        }

        if(aux == 3){
            //deslizante
            mask = (0x01<<1);
            *r0 = *r0 | mask;
            mask = ~(0x01<<2);
            *r0 = *r0 & mask;
        }
        
        if(aux == 4){
            //piscante
            mask = (0x01<<2);
            *r0 = *r0 | mask;
            mask = ~(0x01<<1);
            *r0 = *r0 & mask;
        }
        
        if(aux == 5){
            //deslizante e piscante
            mask = (0x01<<1);
            *r0 = *r0 | mask;
            mask = (0x01<<2);
            *r0 = *r0 | mask;
        }

    }while (1);

    // Release resources
    if (registers_release(map, FILE_SIZE, fd) == -1) {
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}
