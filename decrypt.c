#include <stdio.h>

static int OpenFiles(/*@out@*/ FILE **i, /*@out@*/ FILE **o, int argc, char *argv[]);
static int CloseFiles(FILE **i, FILE **o);

int main(int argc, char *argv[])
{
	FILE *in, *out;
	if (OpenFiles(&in, &out, argc, argv) == EOF) return 1;
	if (CloseFiles(&in, &out) == EOF) return 1;
	return 0;
}

static int OpenFiles(FILE ** i, FILE **o, int argc, char *argv[]){
	char fn[200]; char* fname; *i = *o = NULL; 

	fname = fn;
	if (argc == 1) {
		(void)puts("Enter address of encrypted file !");
		(void)scanf("%s", fn);
		argc++;
	}
	else fname = argv[1];
	if ((*i = fopen(fname, "rb")) == NULL){
		(void)puts("Unable to open encrypted file!");
		return EOF;
	}
	if (argc == 2) {
		(void)puts("Enter name for decrypted file!");
		(void)scanf("%s", fn);
	}
	else fname = argv[2];
	if ((*o = fopen(fname, "wb")) == NULL){
		(void)puts("Unable to create decrypted file!");
		return EOF;
	}
	return 0;
}
static int CloseFiles(FILE **i, FILE **o){
	if (fclose(*i) == EOF) {
		(void)puts("Error during closing encrypted file");
		return EOF;
	}
	if (fclose(*o) == EOF) {
		(void)puts("Error during closing decrypted file");
		return EOF;
	}
	return 0;
}
