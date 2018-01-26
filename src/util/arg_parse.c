//
// Created by kkyse on 1/24/2018.
//

#include "arg_parse.h"
#include "../serialize/buffer.h"

#include <string.h>
#include <stdio.h>

static void Option_set(Option *option, ParsedArg arg);

static bool Option_print_error_message(Option option);

ParsedArgv parse_args(const int argc, const char *const *const argv) {
    ParsedArgv parsed_argv = {
            .num_args = 0,
            .args = (ParsedArg *) malloc((argc - 1) * sizeof(ParsedArg)),
    };
    
    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];
        if (arg[0] == '-') {
            arg++;
        } else {
            continue;
        }
        if (arg[0] == '-') {
            arg++;
        }
        const char *const option_arg = arg;
        const char *const option_arg_end_nullable = strchr(option_arg, '=');
        const char *const option_arg_end = option_arg_end_nullable ? option_arg_end_nullable : option_arg + strlen(option_arg);
        
        const String option = {
                .chars = (char *) option_arg,
                .length = option_arg_end - option_arg
        };
        
        const char *chars = NULL;
        if (option_arg_end_nullable) {
            // use after '=' as value
            chars = option_arg_end_nullable + 1;
        }
        if (i + 1 < argc && argv[i + 1][0] != '-') {
            // use next arg as value
            chars = argv[i++];
        }
        const String value = {
                .chars = (char *) chars,
                .length = chars ? strlen(chars) : 0,
        };
        
        const ParsedArg parsed_arg = {
                .option = option,
                .value = value,
        };
        memcpy(parsed_argv.args + parsed_argv.num_args++, &parsed_arg, sizeof(parsed_arg));
    }
    
    // free extra memory
    parsed_argv.args = (ParsedArg *) realloc(parsed_argv.args, parsed_argv.num_args * sizeof(ParsedArg));
    return parsed_argv;
}

static void Option_set(Option *const option, const ParsedArg arg) {
    option->found = true;
    if (option->has_arg != NOT_REQUIRED) {
        set_field(option->value, arg.value);
//        option->value = arg.value;
    }
}

static bool Option_print_error_message(const Option option) {
    if (option.found || option.required != REQUIRED) {
        return false;
    }
    
    if (!option.name && !option.letter) {
        return false;
    }
    
    printf("Missing Argument: ");
    if (option.name && option.letter) {
        // failed on both
        printf("--%s, or -%c", option.name, option.letter);
    } else if (option.name) {
        printf("--%s", option.name);
        // failed on name
    } else if (option.letter) {
        printf("-%c", option.letter);
        // failed on letter
    }
    printf("\n");
    
    return true;
}

int fill_options(const ParsedArgv argv, const Options options) {
    for (size_t i = 0; i < options.num_options; ++i) {
        Option *const option = options.options + i;
        
        if (option->name) {
            const String option_name = String_new(option->name);
            for (size_t j = 0; j < argv.num_args; ++j) {
                const ParsedArg arg = argv.args[j];
                if (String_equals(option_name, arg.option)) {
                    Option_set(option, arg);
                }
            }
        }
        const char letter = option->letter;
        if (!letter) {
            if (Option_print_error_message(*option)) {
                return -1;
            }
        }
        
        if (letter) {
            for (size_t j = 0; j < argv.num_args; ++j) {
                const ParsedArg arg = argv.args[j];
                if (arg.option.length != 1) {
                    continue;
                }
                if (letter == arg.option.chars[0]) {
                    Option_set(option, arg);
                }
            }
        }
        
        if (Option_print_error_message(*option)) {
            return -1;
        }
    }
    return 0;
}

int parse_options(const int argc, const char *const *const argv, const Options options) {
    const ParsedArgv parsed_argv = parse_args(argc, argv);
    const int ret_val = fill_options(parsed_argv, options);
    free(parsed_argv.args);
    return ret_val;
}