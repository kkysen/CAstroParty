//
// Created by kkyse on 1/24/2018.
//

#ifndef CASTROPARTY_ARG_PARSE_H
#define CASTROPARTY_ARG_PARSE_H

#include <stdlib.h>
#include <stdint.h>

#include "sized_string.h"

typedef enum {
    NOT_REQUIRED = 0,
    OPTIONAL = 2,
    REQUIRED = 1,
} Requirement;

typedef struct {
    const uint32_t type;
    const char *const name;
    const char letter;
    const Requirement required;
    const Requirement has_arg;
    bool found;
    String value;
} Option;

typedef struct {
    const size_t num_options;
    Option *const options;
} Options;

typedef struct {
    const String option;
    const String value;
} ParsedArg;

typedef struct {
    size_t num_args;
    ParsedArg *args;
} ParsedArgv;

ParsedArgv parse_args(int argc, const char *const *argv);

int fill_options(ParsedArgv argv, Options options);

int parse_options(int argc, const char *const *argv, Options options);

#endif // CASTROPARTY_ARG_PARSE_H
