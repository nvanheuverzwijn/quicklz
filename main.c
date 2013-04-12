// Sample demo for QuickLZ 1.5.x

// Remember to define QLZ_COMPRESSION_LEVEL and QLZ_STREAMING_MODE to the same values for the compressor and decompressor

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "quicklz.h"

bool parseArgv(int argc, char * argv[]);

bool compress = true;
bool decompress = false;

FILE* input_file;
FILE* output_file;

int main(int argc, char* argv[])
{
    freopen(NULL, "rb", stdin);
    freopen(NULL, "rb", stdout);

    input_file = stdin;
    output_file = stdout;
    if(!parseArgv(argc, argv))
    {
        printf("FAIL TO PARSE ARGUMENT");
        exit(1);
    }
    char *src = 0;
    char *dst = 0;
    size_t input_file_size = 0;
    size_t output_file_size = 0;

    // allocate source buffer and read file
    if(input_file != stdin)
    {
        fseek(input_file, 0, SEEK_END);
        input_file_size = ftell(input_file);
        fseek(input_file, 0, SEEK_SET);
        src = (char*) malloc(input_file_size);
        fread(src, 1, input_file_size, input_file);
    }
    else
    {
        unsigned char inputBuffer[1024];
        size_t bytesRead = 0;

        while ((bytesRead = fread(
            inputBuffer,            // target buffer
            sizeof inputBuffer,     // number of bytes in buffer
            1,                      // number of buffer-sized elements to read
            input_file)) > 0)
        {
            char *tmp = (char*)realloc(src, input_file_size + bytesRead);
            if (tmp)
            {
                src = tmp;
                memmove(&src[input_file_size], inputBuffer, bytesRead);
                input_file_size += bytesRead;
            }
            else
            {
                printf("Ran out of memory\n");
                free(src);
                exit(1);
            }
        }
    }


    if(compress)
    {
        qlz_state_compress *state_compress = (qlz_state_compress *)malloc(sizeof(qlz_state_compress));

        dst = (char*) malloc(input_file_size + 400);
        output_file_size = qlz_compress(src, dst, input_file_size, state_compress);
    }
    else if(decompress)
    {
        qlz_state_decompress *state_decompress = (qlz_state_decompress *)malloc(sizeof(qlz_state_decompress));

        output_file_size = qlz_size_decompressed(src);
        dst = (char*) malloc(output_file_size);

        output_file_size = qlz_decompress(src, dst, state_decompress);
    }

    fwrite(dst, output_file_size, 1, output_file);

    fclose(input_file);
    fclose(output_file);
    return 0;
}

bool parseArgv(int argc, char * argv[])
{

    for(int i = 0; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            if(argv[i][1] == 'd')
            {
                decompress = true;
                compress = false;
            }
            ȩlse if(i + 1 < argc)
            {
                if(argv[i][1] == 'o')
                {
                    output_file = fopen(argv[i+1], "wb");
                }
                else if(argv[i][1] == 'i')
                {
                    input_file = fopen(argv[i+1], "rb");
                }
            }
        }
    }

    return true;
}
