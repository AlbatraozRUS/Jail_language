#include "Language.h"

long CounterOfSymbols(FILE * readfile)
{

    assert(readfile != nullptr);

    fseek(readfile , 0 , SEEK_END);
    long size = ftell(readfile);
    fseek(readfile , 0 , SEEK_SET );

    return size;
}


char * Reading()
{

    FILE *readfile = fopen(Input_File , "rb" );

    long size = 0;

    size = CounterOfSymbols(readfile);

    char *buffer = (char*) calloc((size_t)size + 5, sizeof(char));
    assert(buffer != nullptr);

    fread(buffer, 1, (size_t)size, readfile);

    fclose (readfile);

    return buffer;
}

char *Reading_Back()
{
    FILE *readfile = fopen(Output , "rb" );

    long size = 0;

    size = CounterOfSymbols(readfile);

    char *buffer = (char*) calloc((size_t)size + 5, sizeof(char));
    assert(buffer != nullptr);

    fread(buffer, 1, (size_t)size, readfile);

    fclose (readfile);

    return buffer;
}
