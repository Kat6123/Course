#include "parse.h"

void parse(int argc, char **argv, FILE** ifp, FILE** ofp, FILE** kfp){
	int flag;

	if (argc < 6){
		printf(ER_M, argv[0]);	
		exit(EXIT_FAILURE);
	}
	else {
		while ((flag = getopt(argc, argv, "i:o:k:")) != -1){
			switch (flag) {
			case 'i':						//Открывает файл-источник с адресом optarg для чтения
				OPEN(*ifp, optarg, "rb");
				break;				
			case 'o':
				OPEN(*ofp, optarg, "wb");			//Открывает файл для записи с адресом optarg
				break;	
			case 'k':
				OPEN(*kfp, optarg, "rb");			//Открывает файл с ключом и адресом optarg
				break;	
			default:						//Срабатывает в случае '?'
				fprintf(stderr, ER_M, argv[0]);
				exit(EXIT_FAILURE);
			}
		}

		for (int i = optind; i < argc; i++)				//Если были лишние аргументы, то выводятся в stdout
			printf ("Non-option argument %s\n", argv[i]);
	}
}

