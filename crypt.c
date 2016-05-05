#include <stdio.h>
#include <gmp.h>

#define E "3"
#define N "9173503"
#define D "6111579"
#define LKEY 7
#define SIZE  LKEY - 1
#define LNUM 10
#define NUM 10

void mod(char* s_cell, const char* s_exp, const char* s_mod, int* num){				 //Возведение в степень по модулю
	mpz_t base, exp, mod, res;  //Избавиться от res
	if (mpz_init_set_str(base, s_cell, NUM) != 0) { puts("Error"); exit(1); }
	if (mpz_init_set_str(exp, s_exp, LNUM) != 0) { puts("Error"); exit(1); }
	if (mpz_init_set_str(mod, s_mod, LNUM) != 0) { puts("Error"); exit(1); }
	mpz_init(res);
	mpz_powm(res, base, exp, mod);
	mpz_get_str(s_cell, NUM, res);   //Длина строки
	int i = 0;
	while (s_cell[i] != NULL) i++;
	*num = i;
	mpz_clear(base);
	mpz_clear(exp);
	mpz_clear(mod);
	mpz_clear(res);
}

void encrypt(FILE * srce, FILE* encr){
	char buff[LKEY + 1]; int num;
	while (!feof(srce)){
		num = fread(buff, sizeof(char), SIZE, srce);
		buff[num] = 0;
		mod(buff, E, N, &num);
		fwrite(buff, sizeof(char), LKEY, encr);
		fflush(encr);
	}
}

void decrypt(FILE * encr, FILE* decr){
	char buff[LKEY + 1]; int num;
	while (!feof(encr)){
		num = fread(buff, sizeof(char), LKEY, encr);
		buff[num] = 0;
		mod(buff, D, N, &num);
		fwrite(buff, sizeof(char), num, decr);
		fflush(decr);
	}
}
