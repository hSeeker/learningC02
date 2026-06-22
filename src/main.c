#include "file.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

void print_usage() {
  printf("usage: \n \
                  -n            -create new database file \n \
                  -f filename   -specifies path to database file \n \
                  -r            -reads database istead of writing (writing is the default)\n");
}

int main(int argc, char *argv[]) {
  bool newfile = false;
  char *filename = NULL;
  bool read = false;

  int c = 0;
  while ((c = getopt(argc, argv, "rnf:")) != -1) {
    switch (c) {
    case 'n':
      newfile = true;
      break;
    case 'f':
      filename = optarg;
      break;
    case 'r':
      if (newfile) {
        printf("Cannot crete newfile and read at the same time!\n");
        return -1;
      }
      read = true;
      break;
    default:
      print_usage();
      return -1;
    }
    if (filename == NULL) {
      printf("Filename argument required!\n");
      return -1;
    }
    printf("%s\n", filename);
    int filedescriptor = 0;
    if (newfile) {
      filedescriptor = create_db_file(filename);
      if (filedescriptor == -1) {
        perror("Create database file");
        return -1;
      }
    }
    close(filedescriptor);
  }
  return 0;
}
