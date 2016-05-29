#include "crypt.h"

void transl_in(char ch, char* str){
	const char alph[] = "0123456789abcdef";		//Один символ заменяется 2-мя следующим образом:
							
	str[0] = alph[(ch >> 4) & 15];			//Его страшие 4 бита = индекс в массиве alph 1-го получ-го символа
	str[1] = alph[ch & 15];				//Его младшие 4 бита = индекс в массиве alph 2-го получ-го символа
}

void encrypt_unit(char* source, int num, char* encr, char* e, char* n, int key){
	int len = num;
	int en_len = key / 4 - 2;

	while ((len--) > 0){				//Перевод в 16-ю СС
		transl_in(source[len], &encr[en_len]);
		en_len -= 2;
	}

	mod(encr, e, n);				//Шифрование блока
}

void encrypt(FILE * source, FILE* encr, FILE* pub){
	Header head;
	char *e, *n, *buff, *encr_buff;
	int num, block, key = MAX_LEN;

	memory(e, MAX, char);				//Чтение ключа и его длины из файла, длина буфера устанавливается как для ключа по умолчанию
	memory(n, key/ 4 + 2, char);			//Длина буфера для числа E = длина числа 65537 в 16-й СС + 2
	key_fr_file(pub, e, n, &key);			

	block = key / 8 - 1;

	memory(buff, block + 1, char);			
	memory(encr_buff, key / 4 + 1, char);
	
	encr_buff[key / 4] = 0;

	fseek(source, 0, SEEK_END);			//Перем-й num присваивается размер исходного файла
	num = ftell(source);
	fseek(source, 0, SEEK_SET);

	head.unit_num = num / block;			//Подсчёт кол-ва блоко длиной block, если длина последнего != 0, то количество блоков увеличивается на 1
	if ((head.len_of_last = num % block) != 0)
		head.unit_num++;
	fwrite(&head, sizeof(Header), 1, encr);

	for (int i = 0; i < head.unit_num; i++){
		memset(buff, 0, block);
		memset(encr_buff, '0', key/ 4);

		fread(buff, block * sizeof(char), 1, source); 
		if (i == head.unit_num - 1 && head.len_of_last != 0)
			encrypt_unit(buff, head.len_of_last, encr_buff, e, n, key);		
		else					//Шифрование блока
			encrypt_unit(buff, block, encr_buff, e, n, key);
		fprintf(encr, "%s\n", encr_buff);	//Блоки в зашифрованном файле разделены символом '\n'
		fflush(encr);
	}

	free(buff);
	free(encr_buff);
	free(e);
	free(n);
}
