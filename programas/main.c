#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registers.h"
#define FILE_PATH "registers.bin"
#define FILE_SIZE 1024  // Same size as used in the first program


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
            onoffdisplay(r0);
        }
           
        if(aux == 2){
            //estatico
            settostatic(r0);
        }
        if(aux == 3){
            //deslizante
          settoslide(r0);
        }
        if(aux == 4){
            //piscante
           settopiscante(r0);
        }
        if(aux == 5){
            //deslizante e piscante
            settopiscanteslide(r0);
        }
        if(aux == 6){
            //velocidade
            printf("Digite a velocidade de exibicao: ");
            scanf("%d",&aux2);
            setvelocidade(r0,aux2);
        }
        if(aux == 7){
            //led de operacao
           onffled(r0);
        }
        if(aux == 8){
            //Liga/Desliga o LED de status e define cor:
            printf("Digite tres numeros (0 ou 1), RGB: ");
            scanf("%d %d %d", &R, &G, &B);
            setRGB(r0,R,G,B);
            
    }
    if(aux == 9){
            //hardreset
            hardreset(r0);
    }
    if (aux ==10)
    {
        printf("Digite tres numeros (0 a 255), RGB: ");
        scanf("%d %d %d", &R, &G, &B);
        setdisplaycolor(r1,r2,R,G,B);

    }
    if (aux ==11)
    {
        printf("0 = crítico,1 = baixo,2 = médio,3 = alto");
        scanf("%d",&aux2);
        
        if(aux2==0){
            setRGB(r0,1,0,0);
            batterycritical(r3);
        }
        if(aux2==1){
            setRGB(r0,1,1,0);
            batterylow(r3);  
        }
        if(aux2==2){
            setRGB(r0,0,1,0);
            batterymedium(r3);
        }
        if(aux2==3){
            setRGB(r0,0,1,0);
            batteryhigh(r3);
        }
       

    }
    if (aux ==12)
    {
        char msg[25]; 
        printf("Digite a mensagem: ");
        fgets(msg, sizeof(msg), stdin); 
        msg[24] = '\0'; 

        for (int i = 0; i < 25; i++) {
            *((char *)r4 + i) = 0; 
            *((char *)r4 + i) = msg[i];
        }

//        for (int i = 0; i < 24; i++) {
//            *((char *)r4 + i) = msg[i]; 
//        }
    }

    }while (1);

    // Release resources
    if (registers_release(map, FILE_SIZE, fd) == -1) {
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}