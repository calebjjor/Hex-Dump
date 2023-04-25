/*
 * Project 1 of COMP389: Cryptography
 * Function: Header File of fns.c
 * File Name: fns.h
 */

#ifndef FNS_H
#define FNS_H

/*
 * read from the specified file or stdin and do hexdump
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
*/
int hexdump(FILE *fp);

/*
 * read from the specified file or stdin and do enc-base64
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
*/
int enc_base64(FILE *fp);

/*
 * read from the specified file or stdin and do dec-base64
 * Parameter:
 * fp: file pointer pointing to the specified file or stdin
 *
 * Return value:
 * on success, return 0, else return -1
*/
int dec_base64(FILE *fp);

#endif
