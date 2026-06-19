#include "file.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

void print_usage() {
  printf("usage: \n \
                  -n            -create new database file \n \
                  -f filename   -specifies path to database file \n \
                  -r            -reads database istead of writing (writing is the default)");
}

int main(int argc, char *argv[]) {
  bool newfile = false;
  char *filename = NULL;
  bool read = false;

  int c = 0;
  while ((c = getopt(argc, argv, "nf:r")) != -1) {
    switch (c) {
    case 'n':
      newfile = true;
      break;
    case 'f':
      filename = optarg;
      break;
    case 'r':
      if (newfile) {
        printf("Cannot newfile and read at the same time!");
        return -1;
      }
      read = true;
      break;
    default:
      print_usage();
      return -1;
    }
  }
  return 0;
}
