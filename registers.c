#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

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
    unsigned short *r4 = base_address + 0x04;
    
    int aux2;
    int aux;
    int R, G, B;
    unsigned int mask;

    printf("Current value of R0: 0x%02x\n", *r0);
    printf("Current value of R1: 0x%02x\n", *r1);
    printf("Current value of R2: 0x%02x\n", *r2);
    printf("Current value of R3: 0x%02x\n", *r3);
    printf("Current value of R4: 0x%02x\n", *r4);
    do{
        printf("display on/off = 1\n");
        printf("display exib = 2,3,4,5\n");
        printf("velocidade de exib = 6\n");
        printf("Liga/Desliga o LED de operação = 7\n");
        printf("Led de status RGB = 8\n");
        printf("Define R0 para padrao de fabrica = 9\n");
        printf("Cor do display = 10\n");
        printf("Setar nivel de bateria = 11\n");
        printf("Editar mensagem (apenas 24 caracteres)= 12\n");
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
        if(aux == 6){
            //velocidade
            printf("Digite a velocidade de exibicao: ");
            scanf("%d",&aux2);
            mask = (0xFE07);
            *r0 = *r0 & mask;
            mask = (aux2<<3);
            *r0 = *r0 | mask;
        }
        if(aux == 7){
            //led de operacao
            mask = (0x01<<9);
            *r0 = *r0 ^ mask;
        }
        if(aux == 8){
            //Liga/Desliga o LED de status e define cor:
            printf("Digite tres numeros (0 ou 1), RGB: ");
            scanf("%d %d %d", &R, &G, &B);
            setRGB(r0,R,G,B);
            
    }
     if(aux == 9){
            //led de operacao
            mask = 0x11;
            *r0 = *r0 & mask;
    }
    if (aux ==10)
    {
        printf("Digite tres numeros (0 a 255), RGB: ");
        scanf("%d %d %d", &R, &G, &B);
        *r1 = *r1 & 0x0;
        *r1 = *r1 | R;
        *r1 = *r1 | (G<<8);
        *r2 = *r2 & 0x0;
        *r2 = *r2 | B;

    }
    if (aux ==11)
    {
        printf("0 = crítico,1 = baixo,2 = médio,3 = alto");
        scanf("%d",&aux2);
        if(aux2==0){
            setRGB(r0,1,0,0);
            mask = 0xFC; 
            *r3 = *r3 & mask; 
        }
        if(aux2==1){
            setRGB(r0,1,1,0);
            mask = 0xFC; 
            *r3 = *r3 & mask;
            mask = 0x01;
            *r3 = *r3 | mask;   
        }
        if(aux2==2){
            setRGB(r0,0,1,0);
            mask = 0xFC; 
            *r3 = *r3 & mask;
            mask = 0x01<<1;
            *r3 = *r3 | mask;  
        }
        if(aux2==3){
            setRGB(r0,0,1,0);
            mask = 0xFC; 
            *r3 = *r3 & mask;
            mask = 0x01<<1;
            *r3 = *r3 | mask;
            mask = 0x01;
            *r3 = *r3 | mask;   
        }
       

    }
    if (aux ==12)
    {
    char msg[25]; 
    printf("Digite a mensagem: ");
    fgets(msg, sizeof(msg), stdin); 
    msg[24] = '\0'; 

for (int i = 0; i < 25; i++) {
    *((char *)r4 + i) = 0; // Limpa o byte atual, onde o ponteiro r4 é interpretado como um ponteiro para char
}

for (int i = 0; i < 24; i++) {
    *((char *)r4 + i) = msg[i]; // Adiciona o elemento i de msg ao byte atual, onde o ponteiro r4 é interpretado como um ponteiro para char
}
    }

    }while (1);

    // Release resources
    if (registers_release(map, FILE_SIZE, fd) == -1) {
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}
