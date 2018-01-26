//
// Created by kkyse on 12/14/2017.
//

#ifndef SYSTEMS_IO_H
#define SYSTEMS_IO_H

#include "sized_string.h"

int unlink_if_exists(const char *path);

int unlink_if_exists_and_then(const char *path, void (*on_success)(const char *const path));

int read_bytes(int fd, void *bytes, size_t size);

int write_bytes(int fd, const void *bytes, size_t size);

int read_size(int fd, size_t *size);

int write_size(int fd, size_t size);

int read_int(int fd, int *i);

int write_int(int fd, int i);

String read_string(int fd);

int write_string(int fd, String string);

#endif //SYSTEMS_IO_H
