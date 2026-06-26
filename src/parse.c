#include "parse.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int create_db_header(struct db_header_t **db_header) {
  printf("db header create 1\n");
  *db_header = malloc(sizeof(struct db_header_t));
  if (db_header == NULL) {
    printf("Failed to accumulate memmory for header\n");
    return -1;
  }
  printf("db header create 2\n");

  (*db_header)->magic_number = MAGIC_NUMBER;
  (*db_header)->version = VERSION_NUMBER;
  (*db_header)->count = 0;
  (*db_header)->filesize = sizeof(struct db_header_t);
  printf("%lu   %lu   %lu   %lu\n", (*db_header)->version,
         (*db_header)->magic_number, (*db_header)->filesize,
         (*db_header)->count);
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
    perror("Failed to read header");
    return -1;
  }

  db_header->version = ntohl(db_header->version);
  db_header->magic_number = ntohl(db_header->magic_number);
  db_header->count = ntohl(db_header->count);
  db_header->filesize = ntohl(db_header->filesize);

  if (db_header->version != VERSION_NUMBER) {
    printf("Wrong db version\n");
    return -1;
  }
  if (db_header->magic_number != MAGIC_NUMBER) {
    printf("Improper db magic");
    return -1;
  }
  struct stat dbstat = {0};
  fstat(fd, &dbstat);
  if (db_header->filesize != dbstat.st_size) {
    printf("Corrupted db (Wrong size)");
    return -1;
  }
  return 0;
}

int write_to_file(int fd, struct db_header_t *db_header) {
  if (fd < 0) {
    printf("Got a wrong filedescriptor\n");
    return -1;
  }
  printf("writing to file 1\n");
  printf("%lu   %lu   %lu   %lu\n", db_header->version, db_header->magic_number,
         db_header->filesize, db_header->count);
  db_header->count = htonl(db_header->count);
  db_header->filesize = htonl(db_header->filesize);
  db_header->magic_number = htonl(db_header->magic_number);
  db_header->version = htonl(db_header->version);

  printf("%lu   %lu   %lu   %lu\n", db_header->version, db_header->magic_number,
         db_header->filesize, db_header->count);

  printf("writing to file 2\n");
  lseek(fd, 0, SEEK_SET);
  write(fd, db_header, sizeof(struct db_header_t));
  printf("writing to file 3\n");
  return 0;
}
