#ifndef REGISTERS_H
#define REGISTERS_H

void setRGB(unsigned short *r, int R, int G, int B);
void onoffdisplay(unsigned short *r);
void settostatic(unsigned short *r);
void settoslide(unsigned short *r);
void settopiscante(unsigned short *r);
void settopiscanteslide(unsigned short *r);
void setvelocidade(unsigned short *r, int velocidade);
void onffled(unsigned short *r);
void hardreset(unsigned short *r);
void setdisplaycolor(unsigned short *r, unsigned short *q, int R, int G, int B);
void batterylow(unsigned short *r);
void batterycritical(unsigned short *r);
void batterymedium(unsigned short *r);
void batteryhigh(unsigned short *r);
char* registers_map(const char* file_path, int file_size, int* fd);
int registers_release(void* map, int file_size, int fd);

#endif /* REGISTERS_H */