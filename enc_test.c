#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Base64 encoding table
static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Encode a block of 3 bytes into 4 base64 characters
static void encode_block(uint8_t block[3], char output[4]) {
    output[0] = base64_table[block[0] >> 2];
    output[1] = base64_table[((block[0] & 0x03) << 4) | (block[1] >> 4)];
    output[2] = base64_table[((block[1] & 0x0f) << 2) | (block[2] >> 6)];
    output[3] = base64_table[block[2] & 0x3f];
}


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
    size_t bytes_written = 0;
    bool done = false;

    // Read file contents into buffer and encode
    while (!done) {
        uint8_t block[3] = {0};
        char output[4] = {0};

        // Read a block of data from the input file
        size_t bytes_in_block = 0;
        for (size_t i = 0; i < 3; i++) {
            bytes_read = fread(&buffer[i], 1, 1, fp);
            if (bytes_read == 0) {
                done = true;
                break;
            }
            bytes_in_block++;
        }

        // Encode the block of data
        if (bytes_in_block > 0) {
            if (bytes_in_block == 1) {
                block[0] = buffer[0];
                encode_block(block, output);
                output[2] = '=';
                output[3] = '=';
            } else if (bytes_in_block == 2) {
                block[0] = buffer[0];
                block[1] = buffer[1];
                encode_block(block, output);
                output[3] = '=';
            } else {
                block[0] = buffer[0];
                block[1] = buffer[1];
                block[2] = buffer[2];
                encode_block(block, output);
            }

            // Write the encoded block to the output file
            bytes_written += fwrite(output, 1, 4, out_fp);
            if (bytes_written % 64 == 0) {
                fputc('\n', out_fp);
            }
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

    // call hexdump function with file pointer
    enc_base64(fp);

    // close file and exit
    fclose(fp);
    return 0;
}