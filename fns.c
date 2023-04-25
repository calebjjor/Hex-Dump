/*
 * Project 1 of COMP389: Cryptography
 * Function: hexdump, enc-base64 and dec-base64 handling
 * File Name: fns.c
 */

#include <stdlib.h>
#include <stdio.h>
const unsigned int BUFFER_SIZE = 16;
unsigned char buffer[BUFFER_SIZE] = {0}; // initialize buffer to all zeros

/*
 * read from the specified file or stdin and do hexdump
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
*/
int hexdump(FILE *fp){
  if (fp == NULL){
    printf("Could not open file\n");
    return -1;
  }

  size_t readBytes;
  unsigned int address = 0;
  
  // read from the file while there is still data
  while ((readBytes = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
    //initialize char arrays to 0
    char hexOutput[50] = {0};
    char asciiOutput[20] = {0};
    char addressOutput[10] = {0};
    
    // print address and format to width of minimum 6 characters -- padded with 0s when necessary
    snprintf(addressOutput, 8, "%06x: ", address);
    printf("%s", addressOutput);

  }

  fclose(fp);
  
  return 0; // FIXME
}

/*
 * read from the specified file or stdin and do enc-base64
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
*/
int enc_base64(FILE *fp){
  return 0; // FIXME
}

/*
 * read from the specified file or stdin and do dec-base64
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
*/
int dec_base64(FILE *fp){
  return 0; // FIXME
}
