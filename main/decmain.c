#include "crypt.h"

int main(int argc, char *argv[]){

	FILE* encr, *decr, *private;

	parse(argc, argv, &encr, &decr, &private);		//Определении начальных параметров программы при помощи ф-й getopt

	decrypt(encr, decr, private);
	
	close(encr);
	close(decr);
	close(private);

	return 0;
}

