#include <stdio.h>
#include "crypt.h"

int main(){
	FILE* srce, *encr, *decr;
	srce = fopen("C:\\source.txt", "rb");
	encr = fopen("C:\\encr.txt", "wb");
	encrypt(srce, encr);
	fclose(srce, encr);
	encr = fopen("C:\\encr.txt", "rb");
	decr = fopen("C:\\decr.txt", "wb");
	decrypt(encr, decr);
	fclose(encr, decr);
	return 0;
}

