//
// Created by kkyse on 11/8/2017.
//

#ifndef SYSTEMS_SIGACTION_H
#define SYSTEMS_SIGACTION_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 201711L
#endif

#ifndef XOPEN_SOURCE
#define XOPEN_SOURCE 700
#endif

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 1
#endif

//#ifndef _BSD_SOURCE
//#define _BSD_SOURCE 201711L
//#endif

#include <signal.h>
#include <sys/signal.h>

#ifndef SA_ONSTACK
// Call signal handler on alternate signal stack provided by sigaltstack(2).
    #define SA_ONSTACK   0x20000000
#endif

//int sigaction(int signum, const struct sigaction *action, struct sigaction *old_action);

//int sigemptyset(sigset_t *set);

//typedef struct {
//    void  *ss_sp;     /* Base address of stack */
//    int    ss_flags;  /* Flags */
//    size_t ss_size;   /* Number of bytes in stack */
//} stack_t;
//

//int sigaltstack(const stack_t *ss, stack_t *old_ss);

typedef void (*Runnable)();

typedef void (*SignalHandler)(const int);

void add_sigaction(int signal, const struct sigaction *sigaction);

void add_signal_handler(int signal, SignalHandler signal_handler);

void register_for_all_signals(SignalHandler signal_handler);

void always_run_on_any_exit(Runnable on_exit, SignalHandler signal_handler);

#endif //SYSTEMS_SIGACTION_H
