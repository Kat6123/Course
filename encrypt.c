#include <stdio.h>
#include <string.h>

static int OpenFiles(/*@out@*/ FILE **i, /*@out@*/ FILE **o, /*@out@*/ char orfile[], int argc, char *argv[]);
static int CloseFiles(FILE **i, FILE **o, char* in);

int main(int argc, char *argv[])
{
	FILE *in, *out; char orfile[200];
	if (OpenFiles(&in, &out, orfile, argc, argv) == EOF) return 1;
	if (CloseFiles(&in, &out, orfile) == EOF) return 1;
	return 0;
}

static int OpenFiles(FILE ** i, FILE **o, char orfile[], int argc, char *argv[]){
	char fn[200]; char* in; *i = *o = NULL;

	fn[0] = orfile[0] = '\0';
	if (argc == 1) {
		(void)puts("Enter address of original file !");
		(void)scanf("%s", fn);
		strcpy(orfile, fn);
		in = fn;
		argc++;
	}
	else {
		in = argv[1];
		strcpy(orfile, argv[1]);
	}
	if ((*i = fopen(in, "rb")) == NULL){
		(void)puts("Unable to open original file!");
		return EOF;
	}
	if (argc == 2) {
		(void)puts("Enter name for encrypted file!");
		(void)scanf("%s", fn);
		in = fn;
	}
	else in = argv[2];
	if ((*o = fopen(in, "wb")) == NULL){
		(void)puts("Unable to create encrypted file!");
		return EOF;
	}
	return 0;
}
static int CloseFiles(FILE **i, FILE **o, char in[]){
	char ans = '\0';

	if (fclose(*i) == EOF) {
		(void)puts("Error during closing original file");
		return EOF;
	}
	if (fclose(*o) == EOF) {
		(void)puts("Error during closing encrypted file");
		return EOF;
	}
	(void)puts("Delete original file? (Y/N): ");
	(void)getchar();
	while (1 > 0) {
		ans = (char)getchar();
		if (getchar() != (int)'\n') {
			(void)puts("Enter (Y/N)!");
			while (getchar() != (int)'\n');
		}
		else {
			if (ans == 'Y' || ans == 'y'){
				if (remove(in) != 0) {
					(void)puts("Unable to delete original file!");
					return EOF;
				}
				break;
			}
			if (ans == 'N' || ans == 'n') break;
			else (void)puts("Enter (Y/N)!");
		}
	}
	return 0;
}
