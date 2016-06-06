#include "crypt.h"
#include "parse.h"

int main(int argc, char *argv[]){

	FILE* srce, *encr, *public;

	parse(argc, argv, &srce, &encr, &public);		//Определении начальных параметров программы при помощи ф-й getopt

	encrypt(srce, encr, public);
	
	CLOSE(srce);
	CLOSE(encr);
	CLOSE(public);

	return 0;
}

