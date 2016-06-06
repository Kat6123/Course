#include "crypt.h"

static void transl_out(char* str, char* ch, char flag){		
	const char alph[] = "0123456789abcdef";

	str[0] = strchr(alph, str[0]) - alph;
	if (flag != 1){						//Если flag == 1, то на вход подаётся 1 символ
		str[1] = strchr(alph, str[1]) - alph; 		//2 символа заменяются индексами в строке alph, затем склеиваются: 
		*ch = (str[0] << 4 & 240) | str[1];		//младшие 4 бита 1-го символа = старшим 4-мя битам итог. , а младшие 2-го = младшим итог.
	}
	else
		*ch = str[0];
}

static void decrypt_unit(char* buff, char* decr_buff, char* d, char* n, int key){
	char flag = 0;
	int buff_len;
	int decr_len = key / 8 - 2;

	mod(buff, d, n);					//Расшифровываем блок
	buff_len = strlen(buff) - 2;

	while (buff_len >= 0){					//Переводим блок из 16-й СС, каждые 2 символа заменятся 1-и
		transl_out(&buff[buff_len], &decr_buff[decr_len], flag);		
		if ((buff_len -= 2) < 0){			//Если длина полученного блока < длины исходного блока, то оно дополняется 0
			buff_len++;				//Если его длина - нечётное число, то блок также дополн. 0
			flag = 1;				//Ппричём 1 оставшийся символ = символу, кот. получится в итоге склеивания
		}
		decr_len--;
	}
}

void decrypt(FILE * encr, FILE* decr, FILE* priv){
	Header head;
	char* d, *n, *buff, *decr_buff;
	int block, key = MAX_LEN;

	MEMORY(d, key/ 4 + 2, char);
	MEMORY(n, key/ 4 + 2, char);
	key_fr_file(priv, d, n, &key);				//Считываение ключа и его длины из файла, длина буфера устанавливается как для ключа по умолчанию

	block = key / 8 - 1;

	MEMORY(buff, key/ 4 + 1, char);				//Выделение памяти для обычного и расшифрованного блока
	MEMORY(decr_buff, block + 1, char);

	buff[key / 4] = 0;
	

	fread(&head, sizeof(Header), 1, encr);			//Считывание размера исходного файла

	for (int i = 0; i < head.unit_num; i++){
		memset(buff, '0', key / 4);
		memset(decr_buff, 0, block);

		fscanf(encr, "%s\n", buff);
		decrypt_unit(buff, decr_buff, d, n, key);
		if (i == head.unit_num - 1 && head.len_of_last != 0)
			fwrite(&decr_buff[block - head.len_of_last], head.len_of_last * sizeof(char), 1, decr);
		else						//Запись расшифрованного блока, в зависимости от того является ли он стандартным или последним
			fwrite(decr_buff, block * sizeof(char), 1, decr);			
		fflush(decr);					//Т.е. с размером head.len_of_last
	}

	free(buff);
	free(decr_buff);
	free(d);
	free(n);
}
