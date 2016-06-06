#include "crypt.h"
#include "parse.h"

int main(int argc, char *argv[]){

	FILE* encr, *decr, *private;

	parse(argc, argv, &encr, &decr, &private);		//Определении начальных параметров программы при помощи ф-й getopt

	decrypt(encr, decr, private);
	
	CLOSE(encr);
	CLOSE(decr);
	CLOSE(private);

	return 0;
}

