#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#define MAX_LEN 2048								//Длина(по умолч.) ключа в битах						
#define MAX 7									//Длина буфера, содержащего часть открытого ключа
#define BLOCK_SIZE  (KEY_LEN / 8 - 1)						//Размер шифруемого блока в байтах

#define PRIME 2003								//Число, до которого будет проверяться делимость
#define REP 5									//Количество повторений теста Миллера-Рабина
#define NUM 16 									//Основание системы, в которой представлены, ключ и зашифрованные блоки
#define E 65537									//Часть открытого ключа(выгодно записывается в двоичном виде и ускоряет вычисления)

#define MEMORY(pointer, size, type)\
	pointer = (type*)malloc(size * sizeof(type));\
	if (pointer == NULL){\
		puts("Memory allocate error!");\
		exit(EXIT_FAILURE);\
	}

typedef struct header{								
	unsigned unit_num;							//Количество блоков для шифрования
	unsigned len_of_last;							//Длина последнего блока
}Header;



void decrypt(FILE * encr, FILE* decr, FILE* priv);
void encrypt(FILE * source, FILE* encr, FILE* pub);

void powm(mpz_t res, const mpz_t base, const mpz_t exp, const mpz_t mod);
void invertm_prime(mpz_t res, const mpz_t s_a, const mpz_t s_b);

void key_fr_file(FILE* fp, char* e, char* n, int* key);
void pr_key_fr_file(FILE* fp, char* d, char* p, char* q, int* key);

void keys(FILE* public, FILE* private, int key);
