#include <gmp.h>
#include <time.h>
#include "crypt.h"

static void prime_num(char* arr){					//Используя "Решето Эратосфена" для нечётных чисел получим массив из 0 и 1
	memset(arr, '1', PRIME / 2);					//Индексы которого будут задавать простые числа до PRIME
	arr[PRIME / 2] = 0;

	for (int i = 3; i * i <= PRIME; i += 2){			//Берём простое число i, и начиная с i^i вычёркиваем каждое 2*i - е число
		if (arr[(i - 3) / 2] == '1')				//Т.к. начиная с 2-ки, все i-простые числа
		for (int j = i * i; j <= PRIME; j += 2 * i)
			arr[(j - 3) / 2] = '0';
	}
}

static char start_check(mpz_t num, char * arr){				//Начальная проверка, которая отсекает большинство составных чисел 

	for (int i = 0; i < PRIME / 2; i++)				//Проверяет делится ли сгенерированное число на простые числа до PRIME
		if (arr[i] == '1')
			if (mpz_fdiv_ui(num, 2 * i + 3) == 0)
				return 0;

	return 1;
}

static void random(mpz_t num, int length){
	gmp_randstate_t state;

	gmp_randinit_default(state);					//Задаёт способ генерации чисел = по умолчанию		
	gmp_randseed_ui(state, time(NULL));				//Значение seed зависит от времени, 
									//Поэтому при каждом запуске прогр. генерир-е числа будут разными
	mpz_urandomb(num, state, length);				//Генерация случайного числа
	gmp_randclear(state);
}

static void transform(mpz_t p, unsigned* s, mpz_t d){			//Представляет число p в виде p = 2 ^ s * d

	*s = mpz_scan1(p, 0);						//Определяет номер первого бита равного 1(начиная с нулевого, в сторону старших бит)
	mpz_cdiv_q_2exp(d, p, *s);
}

static char millerrabin(mpz_t num, char rep, int key){
	char flag = 1;
	int i, j;
	mpz_t p, d, a;
	unsigned s = 0;

	mpz_init(p);
	mpz_init(d);
	mpz_init(a);
	mpz_sub_ui(p, num, 1);				

	transform(p, &s, d);						//Представляем p = num - 1 в виде 2 ^ s * d 

	for (i = 0; i < rep; i++){
		random(a, key);						//Генерируем число a, длина которого в 2 раза меньше длины числа num
		mpz_powm(a, a, d, num);					//Вычисляем a = a ^ d (mod num)

		if (mpz_cmp_ui(a, 1) == 0 || mpz_cmp(a, p) == 0)	//Если a == 1 (mod num) или a == - 1 (mod num), то a - свидетель простоты числа num
			continue;

		for (j = 0; j < s - 1; j++){
			mpz_powm_ui(a, a, 2, num);			//Вычисляем a = a ^ 2 (mod num)

			if (mpz_cmp_ui(a, 1) == 0){			//Если a == 1 (mod num), то a - свидетель составного числа num
				flag = 0;
				break;
			}
			if (mpz_cmp(a, p) == 0)				//Если a == - 1 (mod num), то a - свидетель простоты числа num
				break;
		}

		if (j == s - 1)						//Если после цикла с итер.j не случая a == - 1 (mod num), то a - свидетель составного числа num
			flag = 0;

		if (flag == 0)
			break;
	}

	mpz_clear(p);
	mpz_clear(d);
	mpz_clear(a);

	if (flag == 1)
		return 1;
	else
		return 0;
}

static char probab_prime(mpz_t num, char* prime, int key){

	if (start_check(num, prime) == 0)				//Если проходит нач. проверку(делимость на все простые числа до PRIME)	
		return 0;
	if (millerrabin(num, REP, key) == 0)				//То делаем REP вероятностых тестов на простоту Миллера-Рабина
		return 0;

	return 1;
}

static void get_prime(mpz_t num, int key){
	char prime[PRIME / 2 + 1];

	prime_num(prime);						//Получаем массив, содерж. информацию о всех простых числах до PRIME

	random(num, key / 2);					
	mpz_setbit(num, key / 2 - 1);					//Устанавливаем старший бит = 1, чтобы длина числа была key / 2 - 1
	mpz_setbit(num, 0);						//Устанавливаем младший бит = 1, чтобы было нечётное число
	while (probab_prime(num, prime, key / 4) != 1)			//Пока не пройдёт проверку на простоту, прибавляем 2
		mpz_add_ui(num, num, 2);
}

static void key_gener(mpz_t e, mpz_t d, mpz_t n, int key){
	mpz_t p, q, f;						

	mpz_init(p);
	mpz_init(q);
	mpz_init(f);

	get_prime(p, key);						//Генерация простого числа 					
	mpz_nextprime(q, p);						//Получаем следующее за P простое число

	mpz_mul(n, p, q);						//Находим N - произведение простых P и Q		
	mpz_sub_ui(p, p, 1);
	mpz_sub_ui(q, q, 1);
	mpz_mul(f, p, q);						//Считаем функцию Эйлера для N			

	mpz_clear(p);
	mpz_clear(q);

	if (mpz_gcd_ui(NULL, f, E) == 1)				//Устанавл. E = 65537 -простое число, проверяем, чтобы было взаимно простым с ф. Эйлера	
		mpz_set_ui(e, E);
	else{
		puts("Try again!");
		exit(EXIT_FAILURE);
	}
	mpz_invert(d, e, f);						//Находим D = E ^ (-1) mod F, гду F - ф. Эйлера	

	mpz_clear(f);
}			

void keys(FILE* public, FILE* private, int key){
	mpz_t e, d, n;
	char * str;

	MEMORY(str, key / 4 + 2, char);

	mpz_init(e);
	mpz_init(d);
	mpz_init(n);

	key_gener(e, d, n, key);					//Генерируем ключи длиной key				

	mpz_get_str(str, NUM, e);					//Возвращаем представление ключа e в системе NUM в строку str		
	fwrite(&key, sizeof(int), 1, public);				//Записываем размер ключа в битах
	fwrite(str, strlen(str), 1, public);				//Записываем сами ключи, разделённые символом '\n'
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
