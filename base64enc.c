#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <string.h>

#ifndef UINT8_MAX
#error "uint8_max not defined."
#endif // uint_8

static char const alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz"
                               "0123456789+/=";

enum {
        IN_SIZE = 3,
        OUT_SIZE = 4
    };

int encode(uint8_t in[], uint8_t out[]){

    // Decodes by bitshifting.  Refer to base64 specs for details.
    // Takes array in, output array, and the len
    out[0] =  in[0] >> 2;
    out[1] =  ((in[0] & 0x03) << 4 | in[1] >> 4);
    out[2] =  ((in[1] & 0x0f) << 2 | in[2] >> 6);
    out[3] =  in[2] & 0x3f;
    return(0);
}

int main(int argc, char *argv[]){

    FILE *input;

    int i;
    int len;
    uint8_t in[IN_SIZE];
    uint8_t out[OUT_SIZE];
    int line_feed = 0;
    int empty_flag = 1;

    // Check for arguments
    // If there isn't any, read from stdin
    if(argc == 1)
    {
        input = stdin;
        printf("Enter text to encode:\n");
    }

    // If there is one assume it is a file to encode.
    else if (argc == 2)
    {
        // Open the file, tell user success or error
        printf("Opening file :%s...", argv[1]);
        input = fopen(argv[1], "rb+");
        if(input){
            printf("success.\n");
        } else {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            exit(1);
        }
    }
    else
    {
        // Too many arguments, or something else wrong with number of args, print error.
        fprintf(stderr, "%s",  "Invalid arguments: b64enc -filepath");
        exit(1);
    }

    do {
        i = 0, len = 0;
        for(int i = 0; i < IN_SIZE; i++)
        {
            in[i] = fgetc(input);
            if(feof(input) != 0 || (input == stdin && in[i] == '\n'))
            {
                break;
            }
            else if (ferror(input) == 0){
                len++;
            }
            else {
                fprintf(stderr, "Error: %s\n", strerror(errno));
            }

        }

        if(len == 0)
        {
            if(input == stdin)
            {
                printf("\nProvide input:");
            }
            else
            {
                if(empty_flag)
                {
                    printf("Empty file: No input to encode.");
                }

                break;
            }

        }

        else
        {
            empty_flag = 0;
            if(len < 3)
            {
                for(int i = len; i < 3; i++){
                    in[i] = 0;
                }
            }

            if(encode(in, out) != 0)
            {
                fprintf(stderr, "Error: Could not decode file");
            }
            else
            {

                if(len < 3)
                {
                    int padding = ceil(4.0 * len / 3.0);
                    for (i = padding; i < OUT_SIZE ; i++)
                        {
                          out[i] = '=';
                        }
                }

                for(i = 0; i < 4; i++)
                {
                    if(line_feed >= 76)
                    {
                        printf("\n");
                        line_feed = 0;
                    }
                    printf("%c", out[i] == '=' ? out[i] : alphabet[out[i]]);
                    line_feed++;
                }
            }
        }
    } while(1);

    return(0);
}



