#ifndef SIM_RAM_H
#define SIM_RAM_H

#define RAM_SIZE 100            //We will have 99 addressable cells, (100 inc null pointer)

typedef struct {
    int memory_cell [RAM_SIZE];
}RAM;


void init_RAM (RAM *ram);
int read_memory_cell(const RAM *ram, int index);
int write_to_memory_cell(RAM *ram, int value);
void print_memory_cell(const RAM* ram, int cell);
void print_RAM(const RAM *ram);
int load_program(RAM *ram, const int program[], int size);

#endif SIM_RAM_H