#include "crypt.h"

int main(int argc, char *argv[]){

	FILE* srce, *encr, *public;

	parse(argc, argv, &srce, &encr, &public);		//Определении начальных параметров программы при помощи ф-й getopt

	encrypt(srce, encr, public);
	
	close(srce);
	close(encr);
	close(public);

	return 0;
}

