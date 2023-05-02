#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Base64 table used for encoding and decoding
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Decodes a block of data from Base64 encoding to binary format
static void decode_block(const char* input, uint8_t* output) {
    uint8_t indices[4] = {0};
    for (int i = 0; i < 4; i++) {
        const char* pos = strchr(base64_table, input[i]);
        if (pos == NULL) {
            printf("Invalid Base64 character '%c'\n", input[i]);
            exit(-1);
        }
        indices[i] = pos - base64_table;
    }
    output[0] = (indices[0] << 2) | (indices[1] >> 4);
    output[1] = (indices[1] << 4) | (indices[2] >> 2);
    output[2] = (indices[2] << 6) | indices[3];
}

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

    // call base64 decoding function with file pointer
    dec_base64(fp);

    // close file and exit
    fclose(fp);
    return 0;
}
