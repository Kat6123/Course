#include "crypt.h"

static void dec_mod(char* buff, const mpz_t p, const mpz_t q, const mpz_t d_p, const mpz_t d_q, const mpz_t q_p){//Используя китайскую теорему об остатках
	mpz_t m, a, b, temp;									//Упрощаем возведение в степень при расшифровывании
												//Т.к. заранее известны:     q_p = q ^ -1 mod p			
	mpz_init(a);										//d_p = d mod (p - 1);     d_q = d mod (q - 1); 
	mpz_init(b);
	mpz_init(temp);
	mpz_init_set_str(m, buff, NUM);
	
	powm(a, m, d_p, p);
	powm(b, m, d_q, q);
	
	mpz_mod(temp, b, p);									//Определяем b mod p

	if (mpz_cmp(a, temp) < 0)
		mpz_add(a, a, p);

	mpz_sub(temp, a, temp);
	mpz_mul(temp, temp, q_p);
	mpz_mod(temp, temp, p);
	mpz_mul(temp, temp, q);
	mpz_add(m, temp, b);
	mpz_get_str(buff, NUM, m);								//Найдём m = (((a - b) * q_p) mod p) * q + b, если a < b, то a = a + p
	
	mpz_clear(m);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(temp);
}

static void transl_out(char* str, char* ch, char flag){		
	const char alph[] = "0123456789abcdef";

	str[0] = strchr(alph, str[0]) - alph;
	if (flag != 1){										//Если flag == 1, то на вход подаётся 1 символ
		str[1] = strchr(alph, str[1]) - alph; 						//2 символа заменяются индексами в строке alph, затем склеиваются: 
		*ch = (str[0] << 4 & 240) | str[1];						//Младшие 4 бита 1-го символа = старшим 4-мя битам итогового 
	}											//А младшие 2-го = младшим итогового
	else											
		*ch = str[0];
}

static void decrypt_unit(char* buff, char* decr_buff, mpz_t p, mpz_t q, mpz_t d_p, mpz_t d_q, mpz_t q_p, int key){
	char flag = 0;
	int buff_len;
	int decr_len = key / 8 - 2;

	dec_mod(buff, p, q, d_p, d_q, q_p);							//Расшифровываем блок
	buff_len = strlen(buff) - 2;

	while (buff_len >= 0){									//Переводим блок из 16-й СС, каждые 2 символа заменятся 1-и
		transl_out(&buff[buff_len], &decr_buff[decr_len], flag);		
		if ((buff_len -= 2) < 0){							//Если длина получ-го блока < длины исх-го блока,то оно дополняется 0
			buff_len++;								//Если его длина - нечётное число, то блок также дополн. 0
			flag = 1;								//Причём 1 оставшийся символ = символу,получившемуся в итоге склеивания
		}
		decr_len--;
	}
}

void decrypt(FILE * encr, FILE* decr, FILE* priv){
	Header head;
	char* s_d, *s_p, *s_q, *buff, *decr_buff;
	int block, key = MAX_LEN;
	mpz_t d, p, q, d_p, d_q, q_p;


	MEMORY(s_d, key/ 4 + 2, char);
	MEMORY(s_p, key/ 8 + 2, char);
	MEMORY(s_q, key/ 8 + 2, char);
	pr_key_fr_file(priv, s_d, s_p, s_q, &key);						//Считываение ключа и его длины из файла
												//Длина буфера устанавливается как для ключа по умолчанию
	block = key / 8 - 1;

	MEMORY(buff, key/ 4 + 1, char);								//Выделение памяти для обычного и расшифрованного блока
	MEMORY(decr_buff, block + 1, char);

	buff[key / 4] = 0;

	mpz_init_set_str(d, s_d, NUM);
	mpz_init_set_str(q, s_q, NUM);
	mpz_init_set_str(p, s_p, NUM);
	mpz_init(d_p);
	mpz_init(d_q);
	mpz_init(q_p);

	invertm_prime(q_p, q, p);								//Вычисляем q ^ (-1) mod p;  d mod (p - 1);  d mod (q - 1)
	mpz_sub_ui(p, p, 1);
	mpz_sub_ui(q, q, 1);
	mpz_mod(d_p, d, p);
	mpz_mod(d_q, d, q);
	mpz_add_ui(p, p, 1);
	mpz_add_ui(q, q, 1);

	fread(&head, sizeof(Header), 1, encr);							//Считывание размера исходного файла
	
	for (int i = 0; i < head.unit_num; i++){
		memset(buff, '0', key / 4);
		memset(decr_buff, 0, block);
															
		fscanf(encr, "%s\n", buff);
		decrypt_unit(buff, decr_buff, p, q, d_p, d_q, q_p, key);
		if (i == head.unit_num - 1 && head.len_of_last != 0)
			fwrite(&decr_buff[block - head.len_of_last], head.len_of_last * sizeof(char), 1, decr);	//Запись стандартного расшифрованного блока
		else						
			fwrite(decr_buff, block * sizeof(char), 1, decr);			//Запись последнего блока,т.е. размера head.len_of_last
		fflush(decr);					
	}

	
	free(buff);
	free(decr_buff);
	free(s_d);
	free(s_p);
	free(s_q);

	mpz_clear(d_p);
	mpz_clear(d_q);
	mpz_clear(q_p);
	mpz_clear(q);
	mpz_clear(p);
	mpz_clear(d);
}
