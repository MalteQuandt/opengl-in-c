#pragma once

#include <stdio.h>
#include <stdlib.h>

// Error codes:
#define SUCCESS 0
#define BUFFER_ALLOC_FAIL 1
#define PATH_IS_NULL 2
#define FILE_OPEN_FAIL 3

long fileSize(FILE * );
char * loadFile(const char * , int*);