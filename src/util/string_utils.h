//
// Created by kkyse on 11/26/2017.
//

#ifndef SHELL_STR_UTILS_H
#define SHELL_STR_UTILS_H

#include <sys/types.h>

#define is_newline(c) ((c) == '\n' || (c) == '\r')

#define is_whitespace(c) ((c) == ' ' || (c) == '\t' || is_newline(c))

#define is_quote(c) ((c) == '\'' || (c) == '"')

/**
 * Copy \param length chars of a string \param s into a new dynamically-allocated, null-terminated string.
 *
 * @param [in] s the string to copy
 * @param [in] length the number of chars to copy
 * @return the new dynamically-allocated, null-terminated string
 */
char *str_copy_n(const char *s, size_t length);

/**
 * Copy a null-terminated string \param s into a new dynamically-allocated, null-terminated string.
 * #strlen() is used to find the length of \param s.
 *
 * @param [in] s the string to copy
 * @return the new dynamically-allocated, null-terminated string
 */
char *str_copy(const char *s);

/**
 * Find the number of occurences of \param substring in \param s as specified by #strstr().
 *
 * @param [in] s the string to search for \param substring in
 * @param [in] substring the substring to search for
 * @return the number of occurrences of \param substring in \param s
 */
size_t str_num_occurrences(const char *s, const char *substring);

/**
 * Replace the first \param num_times substring of \param s matching \param to_replace with \param replacement
 * from an offset of \param start and return it in a newly-allocated string.
 *
 * @param [in] s the string to replace parts of
 * @param [in] to_replace the string to replace
 * @param [in] replacement the replacement string
 * @param [in] start the index to start searching for \param to_replace
 * @param [in] num_times the max number of times to replace
 * @return a newly-allocated string with \param to_replace replaced with \param replacement
 */
char *str_replace(const char *s, const char *to_replace, const char *replacement,
                  size_t start, size_t num_times);

/**
 * Join \param num_strings strings, separated/delimited by a \param delimiter
 * and with \param start at the front and \param end at the end,
 * into one newly-allocated string.
 *
 * @param [in] delimiter the delimiter string
 * @param [in] strings the strings to join
 * @param [in] num_strings the number of \param strings
 * @param [in] start the start of the joined string
 * @param [in] end the end of the joined string
 * @return the joined string
 */
char *str_join_with_ends(const char *delimiter, const char *const *strings, size_t num_strings,
               const char *start, const char *end);

/**
 * Join \param num_strings strings, separated/delimited by a \param delimiter,
 * into one newly-allocated string.
 *
 * @param [in] delimiter the delimiter string
 * @param [in] strings the strings to join
 * @param [in] num_strings the number of \param strings
 * @return the joined string
 */
char *str_join(const char *delimiter, const char *const *strings, size_t num_strings);

/**
 * Unquote a string of length \param length
 * by stripping a ' or " at the front and end if present.
 * Modifies the string and returns a different pointer to the same string.
 *
 * @param [in, out] s the string to unquote
 * @param [in] the length of the string
 * @return a pointer to the unquoted string (same string)
 */
char *unquote_n(char *s, size_t length);

/**
 * Unquote a string by stripping a ' or " at the front and end if present.
 * Modifies the string and returns a different pointer to the same string.
 *
 * @param [in, out] s the string to unquote
 * @return a pointer to the unquoted string (same string)
 */
char *unquote(char *s);

/**
 * Strip any extra whitespace around the front and back of
 * the first \param length characters of the string \param s.
 * Modified the string and returns a different pointer to the same string.
 *
 * @param [in, out] s the string to strip
 * @param [in] the length of the string
 * @return a pointer to the stripped string (same string)
 */
char *strip_n(char *s, size_t length);

/**
 * Strip any extra whitespace around the front and back of \param s.
 * Modified the string and returns a different pointer to the same string.
 *
 * @param [in, out] s the string to strip
 * @return a pointer to the stripped string (same string)
 */
char *strip(char *s);

#endif //SHELL_STR_UTILS_H
