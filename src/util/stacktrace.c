//
// Created by kkyse on 11/8/2017.
//

#include "stacktrace.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "utils.h"
#include "string_builder.h"

#ifdef __linux__
#include <err.h>
    #include <execinfo.h>
//#include <bits/sigstack.h>
#include <signal.h>

#endif

#ifdef __APPLE__
    #define addr2line_base "atos -o "
#else
    #define addr2line_base "addr2line -f -s -p -e "
#endif

int stacktrace_level = 0;

static char addr2line_cmd[arraylen(addr2line_base) + PATH_MAX + arraylen(" ") + PTR_MAX_STRLEN + 1] = {0};
static char *addr_start = NULL;

static StringBuilder *stacktrace_sb = NULL;

Runnable pre_stack_trace = NULL;
Runnable post_stack_trace = NULL;

/**
 * Use the program addr2line to convert the backtraced symbol address to
 * the file, function, and line number in the stacktrace,
 * appending the output to \param sb.
 *
 * @param addr the address in the executable to find the line number of
 * @param sb the StringBuilder to append the line number, etc. message to
 * @return -1 if error, else the exit status of the popen() call to the addr2line program
 */
static ssize_t addr2line(const void *const addr, StringBuilder *const sb) {
    if (!addr_start) {
        memcpy(addr2line_cmd, addr2line_base, sizeof(addr2line_base));
        if (readlink("/proc/self/exe", addr2line_cmd + arraylen(addr2line_base) - 1, PATH_MAX) == -1) {
            perror("readlink");
        }
        addr_start = addr2line_cmd + strlen(addr2line_cmd);
        *addr_start++ = ' ';
    }
    sprintf(addr_start, "%p", addr);
    FILE *const output = popen(addr2line_cmd, "r");
    if (!output) {
        perror("popen");
        return -1;
    }
    StringBuilder_append_string(sb, "    ");
    const size_t streamed_bytes = StringBuilder_append_stream(sb, output);
    if (streamed_bytes == 0) {
        // if popen() doesn't work for some reason, at least use system
        printf("    ");
        if (system(addr2line_cmd) == -1) {
            perror("system");
        }
    }
    const int ret_val = pclose(output);
    if (ret_val == -1) {
        return -1;
    }
    if (WEXITSTATUS(ret_val) != 0) {
        return -1;
    }
    return streamed_bytes;
}

// header for builtin func
int backtrace(void **buffer, int size);

// header for builtin func
char **backtrace_symbols(void *const *buffer, int size);

#define MAX_STACK_FRAMES SIGSTKSZ

static void *stack_traces[MAX_STACK_FRAMES];

#define AVG_ADDR_2_LINE_LENGTH 40

/**
 * Print the stacktrace of a thread to stderr, assuming a POSIX system
 * (i.e. backtrace() and backtrace_symbols() exists).
 *
 * Print the process IDs in the front as [pid : ppid].
 *
 * @param sb the StringBuilder to append the stacktrace to before printing to stderr
 */
static void posix_print_stack_trace(StringBuilder *const sb) {
    if (!stacktrace_sb) {
        stacktrace_sb = StringBuilder_new(0);
    }
    const int trace_size = backtrace(stack_traces, MAX_STACK_FRAMES);
    if (trace_size < 0) {
        perror("backtrace");
        return;
    }
//    pd(trace_size);
//    debug();
    StringBuilder_ensure_more_capacity(sb, (size_t) (trace_size + 1) * AVG_ADDR_2_LINE_LENGTH);
    StringBuilder_append_string(sb, "Stacktrace:\n");
//    debug();
    const char **const messages = (const char **) backtrace_symbols(stack_traces, trace_size);
//    debug();
    if (!messages) {
        perror("backtrace_symbols(stack_traces, trace_size)");
        StringBuilder_clear(sb);
        return;
    }
    size_t total_streamed_bytes = 0;
    for (uint32_t i = 0; i < trace_size; ++i) {
        const ssize_t streamed_bytes = addr2line(stack_traces[i], sb);
        if (streamed_bytes == -1) {
            fprintf(stderr, "\terror determining line # for: %s\n", messages[i]);
        } else {
            total_streamed_bytes += streamed_bytes;
        }
//        ps(sb->chars);
    }
//    debug();
//    ps(sb->chars);
//    debug();
    
    // fprintf(stderr) hangs and raises SIGSEGV sometimes, so I'm using printf
//    fprintf(stderr, "\n[%d : %d]\n%s\n", getpid(), getppid(), sb->chars);
//    fflush(stderr);
    if (total_streamed_bytes > 0) {
        printf("\n[%d : %d]\n%s\n", getpid(), getppid(), sb->chars);
    }
//    debug();
    StringBuilder_clear(sb);
    free(messages);
//    debug();
}

void print_stack_trace() {
    posix_print_stack_trace(stacktrace_sb);
}

#define SIGNAL_MSG_SIZE_OVERESTIMATE 50

#define print_signal(signal, message) \
    { \
        StringBuilder_append_string(sb, signal); \
        StringBuilder_append_string(sb, ": "); \
        StringBuilder_append_string(sb, message); \
        break; \
    }

#define catch_signal(signal, message) case signal: print_signal(#signal, message)

#define catch_sub(signal, subsignal, message) \
    case subsignal: { \
        StringBuilder_append_string(sb, #signal); \
        StringBuilder_append_string(sb, " ("); \
        StringBuilder_append_string(sb, #subsignal); \
        StringBuilder_append_string(sb, "): "); \
        StringBuilder_append_string(sb, message); \
        break; \
    }

#define catch_FPE(subsignal, message) catch_sub(SIGFPE, subsignal, message)
#define catch_ILL(subsignal, message) catch_sub(SIGILL, subsignal, message)

/**
 * Catch the actual signal and append a message to the StringBuilder \param sb.
 *
 * @param signal the signal caught
 * @param code the siginfo->si_code of the signal caught
 * @param sb the StringBuilder to append the message to
 */
static void catch_signal_and_print_msg(int signal, int code, StringBuilder *const sb) {
    switch (signal) {
        catch_signal(SIGSEGV, "Segmentation Fault");
        catch_signal(SIGINT, "Interrupt: Interactive attention signal, usually Ctrl+C");
        catch_signal(SIGTERM, "Termination: a termination request was sent to the program");
        catch_signal(SIGABRT, "Abort: usually caused by an abort() or assert()");
        catch_signal(SIGALRM, "Alarm");
//        catch_signal(SIGQUIT, "Quit");
        catch_signal(SIGTSTP, "Terminal stop");
        case SIGFPE:
            switch (code) {
                catch_FPE(FPE_INTDIV, "integer divide by zero");
                catch_FPE(FPE_INTOVF, "integer overflow");
                catch_FPE(FPE_FLTDIV, "floating-point divide by zero");
                catch_FPE(FPE_FLTUND, "floating-point overflow");
                catch_FPE(FPE_FLTRES, "floating-point underflow");
                catch_FPE(FPE_FLTINV, "floating-point inexact result");
                catch_FPE(FPE_FLTSUB, "subscript out of range");
                default: print_signal("SIGFPE", "Arithmetic Exception");
            }
            break;
        case SIGILL:
            switch (code) {
                catch_ILL(ILL_ILLOPC, "illegal opcode");
                catch_ILL(ILL_ILLOPN, "illegal operand");
                catch_ILL(ILL_ILLADR, "illegal addressing mode");
                catch_ILL(ILL_ILLTRP, "illegal trap");
                catch_ILL(ILL_PRVOPC, "privileged opcode");
                catch_ILL(ILL_PRVREG, "privileged register");
                catch_ILL(ILL_COPROC, "coprocessor error");
                catch_ILL(ILL_BADSTK, "internal stack error");
                default: print_signal("SIGILL", "Illegal Instruction");
            }
            break;
        default: print_signal("Unknown Signal", "Unknown Cause");
    }
}

/**
 * A signal handler that prints the current stacktrace to stderr.
 *
 * @param signal the signal caught
 * @param siginfo extra info about the signal caught
 * @param context extra context about the signal caught
 */
static void stack_trace_signal_handler_posix(int signal, siginfo_t *siginfo, void *context) {
    if (stacktrace_level != 0) {
        perror("Recursive signal raised in stacktrace handler");
        _Exit(1);
    }
    stacktrace_level++;
    if (pre_stack_trace) {
        pre_stack_trace();
    }
    if (!stacktrace_sb) {
        stacktrace_sb = StringBuilder_new(0);
    }
    StringBuilder *const sb = stacktrace_sb;
    StringBuilder_ensure_more_capacity(sb, SIGNAL_MSG_SIZE_OVERESTIMATE);
    StringBuilder_append_string(sb, "Caught ");
    catch_signal_and_print_msg(signal, siginfo->si_code, sb);
    StringBuilder_append_string(sb, "\n");
//    debug();
    posix_print_stack_trace(sb);
//    debug();
    if (post_stack_trace) {
        post_stack_trace();
    }
    stacktrace_level--;
    exit(EXIT_FAILURE);
}

#undef catch_ILL
#undef catch_FPE
#undef catch_sub
#undef catch_signal
#undef print_signal

static uint8_t alternate_stack[SIGSTKSZ];

#define add_action(signal) add_sigaction(signal, &sig_action)

void set_stack_trace_signal_handler() {
    {
        // setup alternate stack
        stack_t ss = {.ss_sp = alternate_stack, .ss_size = SIGSTKSZ, .ss_flags = 0};
        if (sigaltstack(&ss, NULL) != 0) {
            err(EXIT_FAILURE, "sigaltstack");
        }
    }
    {
        // register stack trace signal handlers
        int flags = SA_SIGINFO;
        #ifndef __APPLE__
        // for some reason backtrace() doesn't work on macOS when using alternate stack
        flags |= SA_ONSTACK;
        #endif
        struct sigaction sig_action = {.sa_sigaction = stack_trace_signal_handler_posix, .sa_flags = flags};
        sigemptyset(&sig_action.sa_mask);
        
        add_action(SIGSEGV);
        add_action(SIGFPE);
        add_action(SIGINT);
        add_action(SIGILL);
        add_action(SIGILL);
        add_action(SIGTERM);
        add_action(SIGABRT);
//        add_action(SIGQUIT);
        add_action(SIGTSTP);

//        p("set stacktrace signal handler");
    }
}

#undef add_action
