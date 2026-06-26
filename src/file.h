#ifndef FILE_H
#define FILE_H

void test();

int create_db_file(char *filename);
int open_db_file_to_write(char *filename);
int open_db_file_to_read(char *filename);

#endif
