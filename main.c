#include<stdio.h>
#include "scanner.c"

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    printf("SyntaxError: Require 1 argument of file");
    return 0;
  }
  lex(argv[1]);
  return 0;
}
