#include <gmp.h>
#include "crypt.h"

void mod(char* s_cell, const char* s_exp, const char* s_mod){
	mpz_t base, exp, mod;

	mpz_init_set_str(base, s_cell, NUM);			//Инициализация перем-х типа mpz_t, знач-е которых получаем из соотв. строк в NUM-сист. счисления
	mpz_init_set_str(exp, s_exp, NUM);
	mpz_init_set_str(mod, s_mod, NUM);

	mpz_powm(base, base, exp, mod);				//Возведение в степень по модулю

	mpz_get_str(s_cell, NUM, base);

	mpz_clear(base);					//Очистка памяти, заним-й переменными типа mpz_t
	mpz_clear(exp);
	mpz_clear(mod);
}

static unsigned fsize(FILE* fp){				//Получение размера файла, содерж-го ключи(т.е. в начале ф-ла указана длина ключа, а затем его знач-е)
	unsigned size = 0;

	size = ftell(fp);
	fseek(fp, 0, SEEK_END);
	size = ftell(fp) - size;
	fseek(fp, ftell(fp) - size, SEEK_SET);
	return size / sizeof(char);
}

static void read_file(FILE* fp, char** buff, unsigned* size){	//Читаем весь ф-л в буфер

	*size = fsize(fp);
	MEMORY(*buff, *size, char);
	fread(*buff, sizeof(char), *size, fp);
}

void key_fr_file(FILE* fp, char* e, char* n, int* key){
	char* buff;
	int i = 0;
	unsigned size;

	fread(key, sizeof(int), 1, fp);				//Считываем размер ключа
	read_file(fp, &buff, &size);
	while (buff[i] != '\n'){				//Разбиваем ключ на две части
		i++;
	}
	strncpy(e, buff, i);
	e[i] = 0;
	i++;
	strncpy(n, buff + i, size - i);
	n[size - i] = 0;
	free(buff);
}
