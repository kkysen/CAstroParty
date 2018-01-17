//
// Created by kkyse on 12/19/2017.
//

#include "io.h"

#include <unistd.h>

#include "utils.h"

int unlink_if_exists_and_then(const char *path, void ( *on_success)(const char *const path)) {
    if (unlink(path) == -1) {
        // client may have remove FIFO
        if (errno != ENOENT) {
            perror("unlink(path)");
            return -1;
        }
        errno = 0;
    } else {
//        printf("Removed path: \"%s\"\n", path);
        if (on_success) {
            on_success(path);
        }
    }
    return 0;
}

int unlink_if_exists(const char *const path) {
    return unlink_if_exists_and_then(path, NULL);
}

int read_bytes(const int fd, void *const bytes, const size_t size) {
    const ssize_t bytes_read = read(fd, bytes, size);
    if (bytes_read != (ssize_t) size) {
        perror("read(fd, bytes, size)");
        check(bytes_read);
        fprintf(stderr, "%zd != %zu", bytes_read, size);
    }
    return 0;
}

int write_bytes(const int fd, const void *const bytes, const size_t size) {
    if (write(fd, bytes, size) != (ssize_t) size) {
        perror("write(fd, bytes, size)");
        return -1;
    }
    return 0;
}

int read_size(const int fd, size_t *const size) {
    if (read(fd, size, sizeof(*size)) != sizeof(*size)) {
        perror("read(fd, (size_t *) &size, sizeof(size))");
        return -1;
    }
    return 0;
}

int write_size(const int fd, const size_t size) {
    if (write(fd, &size, sizeof(size)) != sizeof(size)) {
        perror("write(fd, &size, sizeof(size))");
        return -1;
    }
    return 0;
}

int read_int(const int fd, int *const i) {
    if (read(fd, i, sizeof(*i)) != sizeof(*i)) {
        perror("read(fd, (int *) &i, sizeof(i))");
        return -1;
    }
    return 0;
}

int write_int(const int fd, const int i) {
//    printf("Writing int to fd %d: %d\n", fd, i);
    if (write(fd, &i, sizeof(i)) != sizeof(i)) {
        perror("write(fd, &i, sizeof(i))");
        return -1;
    }
    return 0;
}

String read_string(const int fd) {
    size_t length;
    if (read_size(fd, &length) == -1) {
        perror("read_size(fd, &length)");
        return BAD_STRING;
    }
    
    char *chars = (char *) calloc(length + 1, sizeof(char));
    if (!chars) {
        perror("calloc(length + 1, sizeof(char))");
        return BAD_STRING;
    }
    
    const ssize_t signed_actual_length = read(fd, chars, length);
    if (signed_actual_length == -1) {
        perror("read(fd, chars, length)");
        free(chars);
        return BAD_STRING;
    }
    const size_t actual_length = (size_t) signed_actual_length;
    chars[actual_length] = 0;
    if (signed_actual_length != (ssize_t) length) {
        chars = realloc(chars, actual_length + 1);
    }
    return (String) {.length = actual_length, .chars = chars};
}

int write_string(const int fd, const String string) {
    if (!string.chars) {
        perror("Bad string");
        return -1;
    }
    check_perror(write_size(fd, string.length));
    check_perror(write_bytes(fd, string.chars, string.length));
    return 0;
}
