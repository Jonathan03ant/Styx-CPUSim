#ifndef SIM_RAM_H
#define SIM_RAM_H

#define RAM_SIZE 100            //We will have 99 addressable cells, (100 inc null pointer)

typedef struct {
    int value; //stores instruction/data opr + opc
    bool free; // 1 if cell is in use and 0 if not
}MemoryCell;

typedef struct {
    MemoryCell cell[RAM_SIZE];
}RAM;

/*
*FUTURE: Ideally RAM should be an entity where it gives our CPU unlimited amount of RAM from the OS

    * RAM cells are where instructions and datas. 
    * They are loaded by the compiler
        * ISA instructions will be compiled in to numeric form
            * int program[] = [1910, 1015, 1216...] #this is an exmple of a program
            * First two digits = OPCODE (should match ISA enum so the cpu knows what to do)
            * Last two digits = OPERAND (this is the address of the cell in the ram)      
                * 1910-> input a number and store it at address cell 10
                * 1015-> Load value from memory cell 15 in to ACC  
*/
void init_RAM (RAM *ram);
int read_memory_cell(const RAM *ram, int index);
int write_to_memory_cell(RAM *ram, int index, int value);
void print_RAM(const RAM *ram);
int load_program(RAM *ram, const int program[], int size);

#endif SIM_RAM_H