#include "crypt.h"

#define REP 20							//Количество повторений (для проверки числа на простоту)

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

void random(mpz_t num, int key){
	gmp_randstate_t state;

	gmp_randinit_default(state);				//Задаёт способ генерации чисел = по умолчанию
	gmp_randseed_ui(state, time(NULL));			//Значение seed зависит от времени, поэтому при каждом запуске прогр. генерир-е числа буду разными
	mpz_urandomb(num, state, key / 2);			//Генерация случайного числа
	gmp_randclear(state);
}

void get_prime(mpz_t num, int key){
	char flag = 0;

	random(num, key);					//Генерация числа и прибавление к нему 1, пока не получится простое
	while (flag == 0){
		mpz_add_ui(num, num, 1);
		flag = mpz_probab_prime_p(num, REP);
	}
}

void key_gener(mpz_t e, mpz_t d, mpz_t n, int key){
	mpz_t p, q, f;
	
	mpz_init(p); 
	mpz_init(q); 
	mpz_init(f);

	get_prime(p, key); 					//Генерация простого числа
	mpz_nextprime(q, p);					//Получаем следующее за P простое число

	mpz_mul(n, p, q);					//Находим N - произведение простых P и Q
	mpz_sub_ui(p, p, 1);
	mpz_sub_ui(q, q, 1);
	mpz_mul(f, p, q);					//Считаем функцию Эйлера для N

	mpz_clear(p); 
	mpz_clear(q);

	if (mpz_gcd_ui(NULL, f, 65537) == 1)			//Устанавливаем E = 65537 - простое число, проверяем, чтобы было взаимно простым с ф. Эйлера
				mpz_set_ui(e, 65537);
	else{
		puts("Try again!");
		exit(EXIT_FAILURE);
	}
	mpz_invert(d, e, f);					//Находим D = E ^ (-1) mod F, гду F - ф. Эйлера

	mpz_clear(f);
}

void keys(FILE* public, FILE* private, int key){
	mpz_t e, d, n;
	char * str;
	
	memory(str, key / 4 + 2, char);

	mpz_init(e); 
	mpz_init(d);
	mpz_init(n);

	key_gener(e, d, n, key);				//Генерируем ключи длиной key

	mpz_get_str(str, NUM, e);				//Возвращаем представление ключа e в системе NUM в строку str
	fwrite(&key, sizeof(int), 1, public);			//Записываем размер ключа в битах
	fwrite(str, strlen(str), 1, public);			//Записываем сами ключи, разделённые символом '\n'
	fputc('\n', public);

	mpz_get_str(str, NUM, d);
	fwrite(&key, sizeof(int), 1, private);
	fwrite(str, strlen(str), 1, private);
	fputc('\n', private);

	mpz_get_str(str, NUM, n);
	fwrite(str, strlen(str), 1, public);
	fwrite(str, strlen(str), 1, private);

	mpz_clear(e); 
	mpz_clear(d); 
	mpz_clear(n);

	free(str);
}

unsigned fsize(FILE* fp){					//Получение размера файла, содерж-го ключи(т.е. в начале ф-ла указана длина ключа, а затем его знач-е)
	unsigned size = 0;

	size = ftell(fp);
	fseek(fp, 0, SEEK_END);
	size = ftell(fp) - size;
	fseek(fp, ftell(fp) - size, SEEK_SET);
	return size / sizeof(char);
}

void read_file(FILE* fp, char** buff, unsigned* size){		//Читаем весь ф-л в буфер

	*size = fsize(fp);
	memory(*buff, *size, char);
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
