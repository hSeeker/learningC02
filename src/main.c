#include "file.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

void print_usage() { printf("wrong arguments"); }

int main(int argc, char *argv[]) {
  printf("main\n");
  test();
  int c = 0;
  bool newfile = false;
  char *filename = NULL;
  while ((c = getopt(argc, argv, "nf:")) != -1) {
    switch (c) {
    case 'n':
      newfile = true;
      break;
    case 'f':
      filename = optarg;
      break;
    default:
      print_usage();
      return -1;
    }
  }
  return 0;
}
