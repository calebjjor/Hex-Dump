/*
 * Project 1 of COMP389: Cryptography
 * Function: commandline parsing and file reading
 * File Name: proj1.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fns.h"

void usage(){
  printf("The commandline syntax for proj 1 is as follows:\n");
  printf("    proj1 {hexdump|enc-base64|dec-base64} [file]\n");
  exit(1);
}

int main(int argc, char *argv[]){
  usage();
  return 0;
}

