#include <stdlib.h>

#ifndef PARSE_H
#define PARSE_H

#define MAGIC_NUMBER 0x1111888
#define VERSION_NUMBER 0x1

struct db_header_t {
  unsigned long version, magic_number, filesize, count;
};

int create_db_header(struct db_header_t **db_header);
int validate_db_header(int fd);
int write_to_file(int fd, struct db_header_t *db_header);

#endif // !PARSE_H
