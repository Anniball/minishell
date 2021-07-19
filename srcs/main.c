#include <stdio.h>

/* Declare a buffer for user input of size 2048 */
static char input[2048];

int main(int argc, char** argv) {

  while (1) {

    fputs("lispy> ", stdout);

    fgets(input, 2048, stdin);

    printf("%s", input);
  }

  return 0;
}