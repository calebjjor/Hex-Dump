/*
 * Project 1 of COMP389: Cryptography
 * Function: hexdump, enc-base64 and dec-base64 handling
 * File Name: fns.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>


// Base64 table used for encoding and decoding
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


/**
 * Helper function for b64 encoding
 * Encode a block of 3 bytes into 4 base64 characters.
 * This function takes an array of 3 bytes and encodes it into 4 base64 characters
 * using the standard base64 encoding scheme. The output is stored in an array of
 * 4 characters. Encoding is performed by taking each 6 bits of the input
 * data and mapping them to a base64 character, resulting in 4 output characters
 * for every 3 input bytes.
 * @param block - an array of 3 bytes to be encoded
 * @param output  - an array of 4 characters to store the output encoding
 * 
 * Both encoding and decoding helpers inspired from: http://web.mit.edu/freebsd/head/contrib/wpa/src/utils/base64.c
*/
static void encode_block(uint8_t block[3], char output[4]) {
    output[0] = base64_table[block[0] >> 2];
    output[1] = base64_table[((block[0] & 0x03) << 4) | (block[1] >> 4)];
    output[2] = base64_table[((block[1] & 0x0f) << 2) | (block[2] >> 6)];
    output[3] = base64_table[block[2] & 0x3f];
}


/**
 * Helper function for b64 encoding
 * Decode a block of 4 base64 characters into 3 bytes.
 * This function takes an array of 4 base64 characters and decodes them into 3 bytes
 * using the standard base64 decoding scheme. The output is stored in an array of
 * 3 bytes. The decoding is performed by taking each character and mapping it to
 * its corresponding 6-bit value, and then combining those 6-bit values into 3
 * output bytes.
 * @param input - a string of 4 base64 characters to be decoded with a null-terminator at the end
 * @param output - an array of 3 bytes to store the output decoding
 * @throws an error and exits the program with status code -1 if an invalid Base64
 * 
*/

static void decode_block(const char* input, uint8_t* output) {
    // Store the 6-bit values corresponding to the input characters
    uint8_t indices[4] = {0};
    // Loop through the 4 input characters
    for (int i = 0; i < 4; i++) {
      // Find the position of the current character in the base64 table
      const char* pos = strchr(base64_table, input[i]);
      // Throw error for invalid characters passed through input
      if (pos == NULL) {
          printf("Invalid Base64 character '%c'\n", input[i]);
          exit(-1);
      }
      // Store the index of the character in the indices array
      indices[i] = pos - base64_table;
    }
    output[0] = (indices[0] << 2) | (indices[1] >> 4);
    output[1] = (indices[1] << 4) | (indices[2] >> 2);
    output[2] = (indices[2] << 6) | indices[3];
}


/*
 * read from the specified file or stdin and do hexdump
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * Create a .hex file with the hex dump
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



/*
 * read from the specified file or stdin and do enc-base64
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * Creates a .b64 file with standard base64 encoding
 * on success, return 0, else return -1
*/
int enc_base64(FILE* fp) {
    const int BUFFER_SIZE = 1024;
    // Ensure the file being read from is not empty
    if (fp == NULL) {
        printf("Could not open file\n");
        return -1;
    }

    // Create new output file for writing encoded data
    FILE* out_fp = fopen("enc.b64", "w");
    if (out_fp == NULL) {
        printf("Could not create output file\n");
        return -1;
    }

    // Allocate buffer for file contents
    uint8_t buffer[BUFFER_SIZE];
    size_t bytes_read = 0;

    // Read file contents into buffer and encode
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
        size_t bytes_written = 0;
        uint8_t block[3] = {0};
        char output[4] = {0};

        // Encode file contents in 3-byte blocks
        for (size_t i = 0; i < bytes_read; i++) {
            block[i % 3] = buffer[i];
            if ((i + 1) % 3 == 0) {
                encode_block(block, output); // Call helper function to encode block
                fwrite(output, 1, 4, out_fp); // Write encoded block to output file
                bytes_written += 4;
                memset(block, 0, sizeof(block)); // Reset block buffer to 0
            }
        }

        // Handle any remaining bytes that did not form a complete block
        if (bytes_read % 3 != 0) {
            size_t padding = 3 - (bytes_read % 3);
            for (size_t i = 0; i < padding; i++) {
                block[(bytes_read + i) % 3] = '\0'; // Pad block with null characters
            }
            encode_block(block, output); // Encode padded block
            fwrite(output, 1, padding + 1, out_fp); // Write encoded padded block to output file
            bytes_written += padding + 1;
        }

        // Ensure output is padded to a multiple of 4 characters
        while (bytes_written % 4 != 0) {
            fputc('=', out_fp); // Append padding character '=' to output file
            bytes_written++;
        }
    }
}


/*
 * read from the specified file or stdin and do dec-base64
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * Creates a .txt file with the decoded b64 information
 * on success, return 0, else return -1
*/

int dec_base64(FILE* fp) {
    const int BUFFER_SIZE = 1024;
    // Ensure the file being read from is not empty
    if (fp == NULL) {
        printf("Could not open file\n");
        return -1;
    }

    // Create new output file for writing decoded data
    FILE* out_fp = fopen("dec.txt", "wb");
    if (out_fp == NULL) {
        printf("Could not create output file\n");
        return -1;
    }

    // Allocate buffer for file contents
    char buffer[BUFFER_SIZE];
    size_t bytes_read = 0;
    size_t bytes_written = 0;
    bool done = false;

    // Read file contents into buffer and decode
    while (!done) {
        char block[4] = {0};
        uint8_t output[3] = {0};

        // Read a block of data from the input file
        size_t bytes_in_block = 0;
        for (size_t i = 0; i < 4; i++) {
            bytes_read = fread(&buffer[i], 1, 1, fp);
            if (bytes_read == 0) {
                done = true;
                break;
            }
            if (buffer[i] == '\n' || buffer[i] == '\r') {
                i--;
                continue;
            }
            block[bytes_in_block] = buffer[i];
            bytes_in_block++;
        }

        // Decode the block of data
        if (bytes_in_block > 0) {
            // Call decode_block function to decode the block
            decode_block(block, output);
            
            // Determine how many bytes to write to the output file
            size_t bytes_to_write = 0;
            if (bytes_in_block == 4) {
                // If the block is complete, write 3 bytes to the output file
                bytes_to_write = 3;
            } else {
                // If the block is incomplete, write (bytes_in_block - 1) bytes to the output file
                bytes_to_write = bytes_in_block - 1;
            }
            
            // Write the decoded data to the output file and update bytes_written counter
            bytes_written += fwrite(output, 1, bytes_to_write, out_fp);
        }

    }

    // Free buffer and close file pointers
    fclose(fp);
    fclose(out_fp);

    return 0;
}
