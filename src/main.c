#include "file.h"
#include "parse.h"
#include <bits/getopt_core.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_usage() {
  printf("usage: \n \
                  -n            -create new database file \n \
                  -f filename   -specifies path to database file \n \
                  -r            -reads database istead of writing (writing is the default)\n");
}

bool DEBUG = false;

int main(int argc, char *argv[]) {
  bool newfile = false;
  char *filename = NULL;
  bool read = false;
  char *addstring = NULL;

  int c;
  while ((c = getopt(argc, argv, "nf:rd")) != -1) {
    switch (c) {
    case 'n':
      newfile = true;
      break;
    case 'f':
      filename = strdup(optarg);
      break;
    case 'r':
      read = true;
      break;
    case 'a':
      addstring = strdup(optarg);
      break;
    case 'd':
      DEBUG = true;
      break;
    case '?':
      print_usage();
      break;
    default:
      print_usage();
      return -1;
    }
  }
  if (newfile && read) {
    printf("Cannot crete newfile and read at the same time!\n");
    return -1;
  }

  if (filename == NULL) {
    printf("Filename argument required!\n");
    return -1;
  }
  printf("%d\n", read);
  printf("%d\n", newfile);
  int filedescriptor = 0;
  printf("test0\n");
  struct db_header_t *db_header = NULL;
  struct employee_t *employees = NULL;
  printf("test1\n");
  if (read) {
    filedescriptor = open_db_file_to_read(filename);
    if (validate_db_header(filedescriptor, &db_header) != 0) {
      printf("Failed to open valid header\n");
      close(filedescriptor);
      return -1;
    }
    if (read_employees(filedescriptor, db_header, &employees) < 0) {
      printf("Failed to read data\n");
      close(filedescriptor);
      return -1;
    }
  } else {
    if (newfile) {
      filedescriptor = create_db_file(filename);
      if (filedescriptor == -1) {
        perror("Cannot create database file");
        return -1;
      }
    } else {
      printf("test 2\n");
      filedescriptor = open_db_file_to_write(filename);
    }
    printf("test 3\n");
    if (create_db_header(&db_header) < 0) {
      printf("Failed to create db header\n");
      free(db_header);
      return -1;
    }
    if (addstring)
      if (add_employee(db_header, &employees, addstring) < 0) {
        printf("Failed to add employee\n");
        return -1;
      }
    write_to_file(filedescriptor, db_header);
  }
  free(db_header);
  free(filename);
  free(addstring);
  close(filedescriptor);
  return 0;
}
