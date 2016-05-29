#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <gmp.h>

#define MAX_LEN 2048								//Длина(по умолч.) ключа в битах						
#define MAX 7									//Длина буфера, содержащего часть открытого ключа
#define BLOCK_SIZE  (KEY_LEN / 8 - 1)						//Размер шифруемого блока в байтах
#define NUM 16 									//Основание системы, в которой представлены, ключ и зашифрованные блоки

#define memory(pointer, size, type)\
	pointer = (type*)malloc(size * sizeof(type));\
	if (pointer == NULL){\
		puts("Memory allocate error!");\
		exit(EXIT_FAILURE);\
	}

#define open(pointer, path, mode)\
	if (!(pointer = fopen(path, mode))){\
		puts("Open file error!");\
		exit(EXIT_FAILURE);\
	}

#define close(pointer)\
	if (fclose(pointer) == EOF){\
		puts("Close file error!");\
		exit(EXIT_FAILURE);\
	}

typedef struct header{								
	unsigned unit_num;							//Количество блоков для шифрования
	unsigned len_of_last;							//Длина последнего блока
}Header;

void parse(int argc, char **argv, FILE** ifp, FILE** ofp, FILE** kfp);

void encrypt(FILE * source, FILE* encr, FILE* pub);
void decrypt(FILE * encr, FILE* decr, FILE* priv);

void keys(FILE* public, FILE* private, int key);				//Генерирует ключи
void key_fr_file(FILE* fp, char* e, char* n, int* key);				//Читает ключи из файла

void mod(char* s_cell, const char* s_exp, const char* s_mod);			//Возведение в степень по модулю
