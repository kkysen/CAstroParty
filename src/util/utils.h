//
// Created by kkyse on 11/9/2017.
//

#ifndef CASTROPARTY_UTILS_H
#define CASTROPARTY_UTILS_H

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef unsigned int uint;

#define arraylen(array) (sizeof((array)) / sizeof(*(array)))

#define PTR_MAX_STRLEN (((sizeof(void *) * 8) / 4) + 1)

#ifndef PATH_MAX
    #define PATH_MAX 4096
#endif

#define extreme(cmp, a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a cmp _b ? _a : _b; })

#define min(a,b) extreme(<, a, b)

#define max(a, b) extreme(>, a, b)

#define memcpy_add(dst, src, size) \
    memcpy(dst, src, size); \
    (dst) += (size);

extern const char *HOME;

/**
 * Initialize the HOME global the environment variable HOME.
 */
void init_home();

#define einval_ret(assertion, message, ret_val) \
    if (assertion) { \
        fprintf(stderr, "Invalid Argument: %s\n", message); \
        return ret_val; \
    }

#define einval(assertion, message) einval_ret(assertion, message, EXIT_FAILURE)

#define check(expr) if ((expr) == -1) return -1
#define check_perror(expr) if ((expr) == -1) return perror(#expr), -1
#define check_msg(expr, msg) if ((expr) == -1) return perror(msg), -1

#define check_null(expr) if ((expr) == NULL) return -1;
#define check_null_perror(expr) if ((expr) == NULL) return perror(#expr), -1
#define check_null_msg(expr, msg) if ((expr) == NULL) return perror(msg), -1

#define p(s) printf(s"\n")
#define pn() p("")
#define ps(var) printf(#var": '%s' (%zu)\n", (char *) (var), strlen(var))
#define pz(var) printf(#var": %zu\n", (size_t) (var))
#define pd(var) printf(#var": %zd\n", (ssize_t) (var))
#define pp(var) printf(#var": %p\n", var)
#define pb(var) printf(#var": %s\n", (var) ? "true" : "false")
#define pc(var) printf(#var": %c\n", (char) (var))

#define _perror(strerror_call, s) \
    fprintf(stderr, \
        "[%d] %s (%s at %s:%d): \n\t%s\n\n", \
        getpid(), strerror_call, __func__, __FILE__, __LINE__, s)

#define perror(s) _perror(strerror(errno), s)

#define perror_assert_expr(expr) do {if (!(expr)) perror(#expr); _Static_assert(false, #expr);} while (false)

#define perror_assert(expr, s) do {if (!(expr)) perror(s); _Static_assert(false, #expr);} while (false)

#define debug() printf("[%d] %s at %s:%d\n", getpid(), __func__, __FILE__, __LINE__)

#endif // CASTROPARTY_UTILS_H
