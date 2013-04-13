// Sample demo for QuickLZ 1.5.x

// Remember to define QLZ_COMPRESSION_LEVEL and QLZ_STREAMING_MODE to the same values for the compressor and decompressor

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "quicklz.h"

bool parseArgv(int argc, char * argv[]);

// OPTIONS HERE
bool compress = true; // default is compressing.
bool decompress = false; // -d
bool show_help = false; // -h
FILE* input_file; // -f
char* input_file_name; // -f
FILE* output_file; // -o
char* output_file_name; // -o

int main(int argc, char* argv[])
{
    input_file = stdin;
    output_file = stdout;
    char *src = 0;
    char *dst = 0;
    size_t input_file_size = 0;
    size_t output_file_size = 0;
    char inputBuffer[1];
    size_t bytesRead = 0;

    parseArgv(argc, argv);

    if(show_help)
    {
        printf("Quicklz implementation version 1:\n");
        printf("\n");
        printf("usage: quicklz [options]:\n");
        printf("\n");
        printf("   -f file - input file that will be compress or decompress. This can be stdin.\n");
        printf("   -h      - show this help message.\n");
        printf("   -o file - output file. This can be stdout.\n");
        printf("   -d      - will decompress the file given via stdin or via -f option.\n");
        printf("\n");
        printf("Examples:.\n");
        printf("\n");
        printf("   cat file.qlz | quicklz -d > file\n");
        printf("   cat file | quicklz > file.qlz\n");
        printf("   quicklz -i file -o file.qlz\n");
        printf("   quicklz -d -i file.qlz -o file\n");

        exit(0);
    }

    do
    {
        bytesRead = fread(
            inputBuffer,
            1,
            1,
            input_file
        );
        char *tmp = (char*)realloc(src, input_file_size + bytesRead);
        if (tmp)
        {
            src = tmp;
            memmove(&src[input_file_size], inputBuffer, bytesRead);
            input_file_size += bytesRead;
        }
        else
        {
            free(src);
            exit(1);
        }
    } while (!feof(input_file));

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
            else if(argv[i][1] == 'h')
            {
                show_help = true;
            }
            else if(i + 1 < argc)
            {
                if(argv[i][1] == 'o')
                {
                    output_file_name = argv[i+1];
                    output_file = fopen(argv[i+1], "wb");
                }
                else if(argv[i][1] == 'f')
                {
                    input_file_name = argv[i+1];
                    input_file = fopen(argv[i+1], "rb");
                }
            }
        }
    }

    return true;
}
