#include <file_loading.h>


long fileSize(FILE * fp) {
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    return size; 
}

char * loadFile(const char * filePath, int * error) {
    if(filePath == NULL) {
        *error = PATH_IS_NULL;
        return NULL;
    } else {
        FILE * fp = fopen(filePath, "r");
        if(fp == NULL) {
            *error = FILE_OPEN_FAIL;
            return NULL;
        }
        // Get the file size
        long size = fileSize(fp);
        // Allocate enough memory for the file
        char * buffer = (char*) calloc(size, sizeof(char));

        if(buffer==NULL) {
            *error = BUFFER_ALLOC_FAIL;
            return NULL;
        }
        // Read the contents of the file 
        fread(buffer, 1, size, fp);
        // Close the file pointer
        fclose(fp);
        *error = SUCCESS;
        // Return the file's contents:
        return buffer;
    }
}