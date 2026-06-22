#include <stdlib.h>

#ifndef PARSE_H
#define PARSE_H

#define MAGIC_NUMBER 0x1111888
#define VERSION_NUMBER 0x1

struct db_header_t {
  unsigned int version, magic_number, filesize, count;
};

#endif // !PARSE_H
