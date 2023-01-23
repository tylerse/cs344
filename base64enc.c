/* Author: Sean Tyler
 * Date: 01/19/2023
 * Class: CS344
 * Assignment: BASE64
 *
 * base64enc: Encodes a file in base64, or provided no arguments or '-'
 *            encodes from stdin
 *
 * Arguments: filepath
 *
 * Returns: base64 encoded text to stdout from file or stdin
 */

#include <stdio.h>
#include <stdint.h>
#include <err.h>
#include <math.h>

#ifndef UINT8_MAX
#error "uint8_max not defined"
#endif

// Define input/output sizes to handle each pass
#define IN_SIZE   3
#define OUT_SIZE  4

static char const alphabet[] =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                "abcdefghijklmnopqrstuvwxyz"
                                "0123456789+/=";

/*
 * Function:    encode
 * -------------------
 * base64 encodes in[3] array into out[4] array
 *
 * example, with letters standing in for bits
 * input bytes = abcdefgh ijklmnop qrstuvwx
 * first byte -> 00abcdef
 * second     -> 00ghijkl
 * third      -> 00mnopqr
 * fourth     -> 00stuvwx
 *
 * in[3]: three byte array to encode
 * out[4]: four byte array to place encoded data
 *
 * returns: integer 0 upon success.
 */

int encode(uint8_t in[], uint8_t out[]){


  // clear out var from previous cycle
  for(int i = 0; i < OUT_SIZE; i++) {
      out[i] = 0;
  }
    out[0] = in[0] >> 2;
  out[1] = (in[0] & 0x03) << 4 | in[1] >> 4;
  out[2] = (in[1] & 0x0f) << 2 | in[2] >> 6;
  out[3] = in[2] & 0x3f;
  return 0;
}

int main(int argc, char *argv[]){

  int len;
  int eof_flag = 0;
  int line_feed = 0;
  uint8_t in[3];
  uint8_t out[4];

  FILE *input;

  // if no argument or first argument is '-', encode from stdin.
  if(argc < 2 || (argc >= 2 && *argv[1] == '-')) {
    printf("Enter text to encode:\n");
    input = stdin;

  // otherwise try opening and encoding from file at provided path.
  } else {
    input = fopen(argv[1], "rb+");

    // test if file opened properly.
    if(input){
      printf("Successfully opened file\n");
    }
    // return error opening file.
    else {
      err(1, NULL);
    }
  }

  // loop through input IN_SIZE characters at a time until end of file/input
  while(eof_flag == 0) {

    // reset len var, then process IN_SIZE characters from input
    len = 0;
    for(int i = 0; i < IN_SIZE; i++){
      char c = fgetc(input);
      //check end of file, if not increment len var
      if(c != EOF){
        in[i] = c;
        len++;
      }
      // else distinguish between error and eof.  if eof, set eof_flag to true
      else {
        if(ferror(input)){
          err(1, NULL);
        } else {
          eof_flag = 1;
        }
      }
    }

    encode(in, out);

    //if end of input, add appropriate padding to unused output elements
    if(eof_flag == 1){
      if(len < 3){
        int padding = ceil(4.0 * len / 3.0);
        for(int i = padding; i < OUT_SIZE; i++){
          out[i] = 64;
        }
      }
    }

    // print encoded output to stdout
    for(int i = 0; i < OUT_SIZE; i++ ) {
      printf("%c", alphabet[out[i]]);

      // check line length and add line feed if >= 76 chars
      line_feed++;
      if(line_feed >= 76){
        printf("\n");
        line_feed = 0;
      }
    }
  }

  // add a line feed to end of output if one was not already added
  if(line_feed != 0){
    printf("\n");
  }
}


