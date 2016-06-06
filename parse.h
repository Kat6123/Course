#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#define ER_M "Usage: %s [-i input file path] [-o output file path] [-k key path]\n"

#define OPEN(pointer, path, mode)\
	if (!(pointer = fopen(path, mode))){\
		puts("Open file error!");\
		exit(EXIT_FAILURE);\
	}

#define CLOSE(pointer)\
	if (fclose(pointer) == EOF){\
		puts("Close file error!");\
		exit(EXIT_FAILURE);\
	}

void parse(int argc, char **argv, FILE** ifp, FILE** ofp, FILE** kfp);
