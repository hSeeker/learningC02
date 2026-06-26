#include "file.h"
#include "parse.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void test() { printf("file test\n"); }

int create_db_file(char *filename) {
  int fd = open(filename, O_RDONLY);
  if (fd != -1) {
    close(fd);
    printf("File already exists!");
    return -1;
  }
  fd = open(filename, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("Failed to create db file");
    return -1;
  }
  return fd;
}

int open_db_file_to_write(char *filename) {
  int fd = open(filename, O_RDWR);
  if (fd == -1) {
    perror("Failed to open db - write");
    return -1;
  }
  return fd;
}

int open_db_file_to_read(char *filename) {
  int fd = open(filename, O_RDONLY);
  if (fd == -1) {
    perror("Failed to open db - read");
    return -1;
  }
  return fd;
}
