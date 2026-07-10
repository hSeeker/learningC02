#include "parse.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int validate_db_header(int fd, struct db_header_t **db_header) {
  printf("validating header\n");
  if (fd < 0) {
    printf("Got a wrong filedescriptor\n");
    return -1;
  }
  *db_header = malloc(sizeof(struct db_header_t));
  if (read(fd, *db_header, sizeof(struct db_header_t)) !=
      sizeof(struct db_header_t)) {
    perror("Failed to read header");
    return -1;
  }

  (*db_header)->version = ntohl((*db_header)->version);
  (*db_header)->magic_number = ntohl((*db_header)->magic_number);
  (*db_header)->count = ntohl((*db_header)->count);
  (*db_header)->filesize = ntohl((*db_header)->filesize);

  printf("%lu   %lu   %lu   %lu\n", (*db_header)->version,
         (*db_header)->magic_number, (*db_header)->filesize,
         (*db_header)->count);

  if ((*db_header)->version != VERSION_NUMBER) {
    printf("Wrong db version\n");
    return -1;
  }
  if ((*db_header)->magic_number != MAGIC_NUMBER) {
    printf("Improper db magic");
    return -1;
  }
  struct stat dbstat = {0};
  fstat(fd, &dbstat);
  if ((*db_header)->filesize != dbstat.st_size) {
    printf("Corrupted db (Wrong size)");
    return -1;
  }
  printf("header valid\n");
  return 0;
}

int write_to_file(int fd, struct db_header_t *db_header,
                  struct employee_t *employees) {
  if (fd < 0) {
    printf("Got a wrong filedescriptor\n");
    return -1;
  }
  printf("writing to file \n");
  int count = db_header->count;
  db_header->count = htonl(db_header->count);
  db_header->filesize =
      htonl(sizeof(struct db_header_t) + count * sizeof(struct employee_t));
  db_header->magic_number = htonl(db_header->magic_number);
  db_header->version = htonl(db_header->version);

  printf("%lu   %lu   %lu   %lu\n", db_header->version, db_header->magic_number,
         db_header->filesize, db_header->count);

  lseek(fd, 0, SEEK_SET);
  write(fd, db_header, sizeof(struct db_header_t));
  printf("writing to file, count = %d \n", count);

  for (int i = 0; i < count; i++) {
    employees[i].hours = htonl(employees[i].hours);
  }
  write(fd, employees, count * sizeof(struct employee_t));
  return 0;
}

int read_employees(int fd, struct db_header_t *db_header,
                   struct employee_t **employees) {
  printf("read_employees\n");
  if (fd < 0) {
    printf("Wrong filedescriptor\n");
    return -1;
  }
  int count = db_header->count;
  *employees = calloc(count, sizeof(struct employee_t));
  if (employees == NULL) {
    printf("Failed to allocate memmory\n");
    return -1;
  }
  int read_ammount = read(fd, *employees, count * sizeof(struct employee_t));
  printf("read ammount = %d\n", read_ammount);
  for (int i = 0; i < count; i++) {
    (*employees)[i].hours = ntohl((*employees)[i].hours);
  }
  printf("read_employees finished\n");
  return 0;
}

int add_employee(struct db_header_t *db_header, struct employee_t **employees,
                 char *addstring) {
  printf("adding employee\n");
  if (*employees == NULL) {
    printf("malloc\n");
    *employees = calloc(1, sizeof(struct employee_t));
    if (*employees == NULL) {
      printf("Failed to allocate memmory\n");
      return -1;
    }
    db_header->count++;
  } else {
    db_header->count++;
    if ((*employees = realloc(*employees, (db_header->count) *
                                              sizeof(struct employee_t))) ==
        NULL) {
      printf("Failed to allocate memmory\n");
      return -1;
    }
  }
  char *name = strtok(addstring, ",");
  char *address = strtok(NULL, ",");
  char *hours = strtok(NULL, ",");

  strncpy((*employees)[db_header->count - 1].name, name,
          sizeof((*employees)[db_header->count - 1].name));
  strncpy((*employees)[db_header->count - 1].address, address,
          sizeof((*employees)[db_header->count - 1].address));
  (*employees)[db_header->count - 1].hours = atoi(hours);
  printf("employee added\n");
  return 0;
}

void list_employees(struct db_header_t *db_header,
                    struct employee_t *employees) {
  printf("List employees:\n");
  printf("count: %lu\n", db_header->count);
  for (int i = 0; i < db_header->count; i++)
    printf("%d: %s; %s; %u\n", i, employees[i].name, employees[i].address,
           employees[i].hours);
}
