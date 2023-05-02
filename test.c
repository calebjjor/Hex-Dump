#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * read from the specified file or stdin and do hexdump
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
 */
int hexdump(FILE *fp) {
  // create a 1kb buffer size
  size_t const BUFFER_SIZE = 1024;

  // ensure the file being read from is not empty
  if (fp == NULL) {
    printf("Could not open file\n");
    return -1;
  }

  // create new hexdump file
  FILE* out_fp = fopen("output.hex", "w");

  if (out_fp == NULL) {
    printf("Could not create output file\n");
    return -1;
  }

  // allocate buffer for file contents
  unsigned char* buffer = malloc(BUFFER_SIZE);
  if (buffer == NULL) {
    printf("Could not allocate memory for file buffer\n");
    fclose(out_fp);
    return -1;
  }

  // keep track of the current address being printed in hex dump
  unsigned int address = 0;

  // loop over file and output hex dump for each buffer-sized chunk
  while (1) {
    // read a chunk of data from the file into the buffer
    size_t readBytes = fread(buffer, 1, BUFFER_SIZE, fp);

    if (readBytes == 0) {
      // end of file reached
      break;
    } 
    
    else if (readBytes < BUFFER_SIZE && feof(fp) == 0) {
      // error reading file
      printf("Error reading file into buffer\n");
      fclose(out_fp);
      free(buffer);
      return -1;
    }

    // loop over buffer and output hex dump
    for (size_t i = 0; i < readBytes; i += 16) {
      //initialize char arrays to 0
      char hexOutput[50] = {0};
      char asciiOutput[17] = {0};
      char addressOutput[10] = {0};

      // store formatted address in the address array and output it to the hex file
      snprintf(addressOutput, 8, "%06x: ", address);
      fprintf(out_fp, "%s ", addressOutput);

      // output hex values for current line
      for (size_t j = 0; j < 16; j++) {
        int buf_pos = (j * 3);
        if (i + j < readBytes) {
          // Format the hex values to 2 characters proceeded by a space
          snprintf(hexOutput + buf_pos, 50 - buf_pos, "%02x ", buffer[i + j]);
        } 
        // format empty hex positions with '--'
        else {
          strncpy(hexOutput + buf_pos, "-- ", 3);
        }
      }
      // Print formatted chars from the buffer to the output text file
      fprintf(out_fp, "%s ", hexOutput);

      // output ascii values for current line
      for (size_t j = 0; j < 16; j++) {

        if (i + j < readBytes) {
          unsigned char c = buffer[i + j];
          // Output ASCII character representation
          if (c >= 32 && c <= 126) {
            asciiOutput[j] = c;
          } 
          // keyboard functions ASCII 1-31
          else if(c < 32) {
            asciiOutput[j] = '.';
          }
          // Account for hex values not in ASCII table
          else{
            asciiOutput[j] = '~';
          }
        } 
      }
      // Print new line to match formatting
      fprintf(out_fp, "%s\n", asciiOutput);
      
      // Increment the address by 16 to be represented in next line
      address += 16;
    }
  }

  // Close buffer and free memory
  fclose(out_fp);
  free(buffer);
  return 0;
}


int main(int argc, char const *argv[])
{
    // check if filename argument is provided
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }

    // open file for reading
    FILE* fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("Could not open file: %s\n", argv[1]);
        return -1;
    }

    // call hexdump function with file pointer
    hexdump(fp);

    // close file and exit
    fclose(fp);
    return 0;
}