#include <stdlib.h>

#ifndef PARSE_H
#define PARSE_H

#define MAGIC_NUMBER 0x1111888
#define VERSION_NUMBER 0x1

struct db_header_t {
  unsigned long version, magic_number, filesize, count;
};

struct employee_t {
  char name[256];
  char address[256];
  unsigned int hours;
};

int create_db_header(struct db_header_t **db_header);
int validate_db_header(int fd, struct db_header_t **db_header);
int read_employees(int fd, struct db_header_t *db_header,
                   struct employee_t **employees);
int write_to_file(int fd, struct db_header_t *db_header,
                  struct employee_t *employees);
int add_employee(struct db_header_t *db_header, struct employee_t **employees,
                 char *addstring);

#endif // !PARSE_H
