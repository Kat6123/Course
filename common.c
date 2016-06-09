#include "crypt.h"

void invertm_prime(mpz_t res, const mpz_t s_a, const mpz_t s_b){		//Обратное по модулю(расширенный алг-м Евклида)
	mpz_t a, b, q, y_1, y_2;
	int i = 0;

	mpz_init_set(a, s_a);
	mpz_init_set(b, s_b);

	if (mpz_cmp(s_a, s_b) > 0)
		mpz_mod(a, a, b);


	mpz_init(q);
	mpz_init_set_ui(y_1, 0);
	mpz_init_set_ui(y_2, 1);

	while (1){
		if (++i % 2 == 1){
			mpz_fdiv_qr(q, b, b, a);				//Находим целую часть и остатое
			mpz_submul(y_1, q, y_2);				//Вычисляем коэффициент
			if (mpz_cmp_ui(b, 1) == 0){				
				if (mpz_cmp_ui(y_1, 0) < 0)			//Если получаем отрицательный коэффициент, то добавляем модуль
					mpz_add(res, y_1, s_b);
				else
					mpz_set(res, y_1);
				break;
			}
		}
		else{
			mpz_fdiv_qr(q, a, a, b);
			mpz_submul(y_2, q, y_1);
			if (mpz_cmp_ui(a, 1) == 0){
				if (mpz_cmp_ui(y_2, 0) < 0)
					mpz_add(res, y_2, s_b);
				else
					mpz_set(res, y_2);
				break;
			}
		}
	}

	mpz_mod(res, res, s_b);

	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(q);
	mpz_clear(y_1);
	mpz_clear(y_2);
}
										
void powm(mpz_t res, const mpz_t base, const mpz_t exp, const mpz_t mod){	//Возведение в степень по модулю(бинарное возв-е в степень)	
	int i = -1, count;						
	mpz_t temp;

	mpz_init_set(temp, base);
	mpz_set_ui(res, 1);
	count = mpz_popcount(exp);						//Количество 1 в бинарном представлении степени

	while (1){
		if (mpz_tstbit(exp, ++i)){					//Определяем чему равен i-й бит в exp
			mpz_mul(res, res, temp);				//Если равен 1, то умножаем результат на temp, а затем возводим temp в квадрат
			mpz_mod(res, res, mod);					//Иначе просто возводим temp в квадрат
			count--;
		}
		if (count == 0)
			break;
		mpz_mul(temp, temp, temp);
		mpz_mod(temp, temp, mod);
	}

	mpz_clear(temp);
}

static unsigned fsize(FILE* fp){						//Получение размера файла, содерж-го ключи
	unsigned size = 0;							//В начале ф-ла указ. длина ключа, а затем его знач-е)

	size = ftell(fp);
	fseek(fp, 0, SEEK_END);
	size = ftell(fp) - size;
	fseek(fp, ftell(fp) - size, SEEK_SET);
	return size / sizeof(char);
}

static void read_file(FILE* fp, char** buff, unsigned* size){			//Читаем весь ф-л в буфер

	*size = fsize(fp);
	MEMORY(*buff, *size, char);
	fread(*buff, sizeof(char), *size, fp);
}

void key_fr_file(FILE* fp, char* e, char* n, int* key){				//Читаем открытый ключ из файла
	char* buff;
	int i = 0;
	unsigned size;

	fread(key, sizeof(int), 1, fp);						//Считываем размер ключа
	read_file(fp, &buff, &size);
	while (buff[i] != '\n'){						//Разбиваем ключ на две части
		i++;
	}
	strncpy(e, buff, i);
	e[i] = 0;
	i++;
	strncpy(n, buff + i, size - i);
	n[size - i] = 0;
	free(buff);
}

void pr_key_fr_file(FILE* fp, char* d, char* p, char* q, int* key){		//Читаем закрытый ключ из файла
	char* buff;
	int i = 0;
	int j = 1;
	unsigned size;

	fread(key, sizeof(int), 1, fp);				
	read_file(fp, &buff, &size);
	while (buff[i] != '\n'){				
		i++;
	}
	j += i;
	while (buff[j] != '\n'){				
		j++;
	}
	strncpy(d, buff, i);
	d[i] = 0;
	i++;
	strncpy(p, buff + i, j - i);
	p[j - i] = 0;
	j++;
	strncpy(q, buff + j, size - j);
	q[size - j] = 0;

	free(buff);
}

