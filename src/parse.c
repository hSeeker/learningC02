#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int create_db_header(int fd, struct db_header_t *db_header) {
  db_header = malloc(sizeof(struct db_header_t));
  if (db_header == NULL) {
    printf("Failed to accumulate memmory for header\n");
    return -1;
  }
  db_header->magic_number = MAGIC_NUMBER;
  db_header->version = VERSION_NUMBER;
  db_header->count = 0;
  db_header->filesize = sizeof(struct db_header_t);
  return 0;
}

int validate_db_header(int fd) {
  if (fd < 0) {
    printf("Got a wrong filedescriptor\n");
    return -1;
  }
  struct db_header_t *db_header = NULL;
  if (read(fd, db_header, sizeof(struct db_header_t)) !=
      sizeof(struct db_header_t)) {
    perror("Read header\n");
    return -1;
  }
  if ()
    return 0;
}
