#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "registers.h"

#define FILE_PATH "registers.bin"
#define FILE_SIZE 1024  // Same size as used in the first program

void setRGB(unsigned short *r,int R,int G,int B){    
            unsigned short mask = 0xE3FF;
            *r = *r & mask;

            if(B == 1){
                mask = 0x01<<10;
                *r = *r | mask;
            }
            if(G == 1){
                mask = 0x01<<11;
                *r = *r | mask;
            }
            if(R == 1){
                mask = 0x01<<12;
                *r = *r | mask;
            }
}

void onoffdisplay(unsigned short *r){*r = *r ^ 0x01;}


void settostatic(unsigned short *r){
    unsigned short mask;
         //estatico
            mask = ~(0x01<<1);
            *r = *r & mask;
            mask = ~(0x01<<2);
            *r = *r & mask; 
}

void settoslide(unsigned short *r){
    unsigned short mask;
            //deslizante
            mask = (0x01<<1);
            *r = *r | mask;
            mask = ~(0x01<<2);
            *r = *r & mask;
}

void settopiscante(unsigned short *r){
    unsigned short mask;
            //piscante
             mask = (0x01<<2);
            *r = *r | mask;
            mask = ~(0x01<<1);
            *r = *r & mask;
}

void settopiscanteslide(unsigned short *r){
    unsigned short mask;
            //deslizante e piscante
            mask = (0x01<<1);
            *r = *r | mask;
            mask = (0x01<<2);
            *r = *r | mask;
}

void setvelocidade(unsigned short *r,int velocidade){
      unsigned short mask;
            mask = (0xFE07);
            *r = *r & mask;
            mask = (velocidade<<3);
            *r = *r | mask;
}

void onffled(unsigned short *r){
    unsigned short mask;        
        mask = (0x01<<9);
        *r = *r ^ mask;
}

void hardreset(unsigned short *r){
    unsigned short mask;
    mask = 0x11;
    *r = *r & mask;
}

void setdisplaycolor(unsigned short *r,unsigned short *q,int R,int G,int B){
    unsigned short mask;
        *r = *r & 0x0;
        *r = *r | R;
        *r = *r | (G<<8);
        *q = *q& 0x0;
        *q = *q | B;
 
}

void batterylow(unsigned short *r){
    unsigned short mask;
    
            mask = 0xFC; 
            *r = *r & mask;
            mask = 0x01;
            *r = *r | mask; 
}
void batterycritical(unsigned short *r){
    unsigned short mask;
    
            mask = 0xFC; 
            *r = *r & mask; 
}
void batterymedium(unsigned short *r){
    unsigned short mask;
            
            mask = 0xFC; 
            *r = *r & mask;
            mask = 0x01<<1;
            *r = *r | mask;  
}
void batteryhigh(unsigned short *r){
    unsigned short mask;
            
            mask = 0xFC; 
            *r = *r & mask;
            mask = 0x01<<1;
            *r = *r | mask;
            mask = 0x01;
            *r = *r | mask;   
}



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


