#include "crypt.h"

#define KNUM 10
#define ER_M "Usage: %s [-o open key path] [-c close key path] [-l key length]\n"

void key_parse(int argc, char **argv, FILE** ifp, FILE** ofp, int* key);

int main(int argc, char *argv[])
{
	FILE *public, *private;
	int key;

	key_parse(argc, argv, &public, &private, &key);				//Определении начальных параметров программы при помощи ф-й getopt

	keys(public, private, key);

	close(public);
	close(private);

	return 0;
}

void key_parse(int argc, char **argv, FILE** ifp, FILE** ofp, int* key){
	int flag;

	if (argc < 6){
		printf(ER_M, argv[0]);	
		exit(EXIT_FAILURE);
	}
	else {
		while ((flag = getopt(argc, argv, "o:c:l:")) != -1){
			switch (flag) {
			case 'o':
				open(*ifp, optarg, "wb");
				break;				
			case 'c':
				open(*ofp, optarg, "wb");
				break;	
			case 'l':{
				int l = strlen(optarg);
				for(int i = 0; i < l; i++)
					if (optarg[i] < '0' && optarg[i] > '9')
						exit(EXIT_FAILURE);
				*key = strtol(optarg, NULL, KNUM);
			}				
				break;	
			default:						//Случай '?'
				fprintf(stderr, ER_M, argv[0]);
				exit(EXIT_FAILURE);
			}
		}
		for (int i = optind; i < argc; i++)				//Если в строке были лишние аргументы, то они выводятся в stdout
			printf ("Non-option argument %s\n", argv[i]);
	}
}

