//
// Created by kkyse on 12/19/2017.
//

#include "sigaction.h"
#include "utils.h"

#include <stdlib.h>
#include <err.h>

void add_sigaction(int signal, const struct sigaction *sig_action) {
    if (sigaction(signal, sig_action, NULL) != 0) {
        perror("sigaction(signal, sig_action, NULL)");
        err(EXIT_FAILURE, "sigaction");
    }
}

void add_signal_handler(int signal, const SignalHandler signal_handler) {
    struct sigaction sig_action = {.sa_handler = signal_handler, .sa_flags = 0};
    sigemptyset(&sig_action.sa_mask);
    add_sigaction(signal, &sig_action);
}

void register_for_all_signals(const SignalHandler signal_handler) {
    struct sigaction sig_action = {.sa_handler = signal_handler, .sa_flags = 0};
    sigemptyset(&sig_action.sa_mask);
    for (int signal = 0; signal < NSIG; ++signal) {
        if (signal != SIGKILL) {
            add_sigaction(signal, &sig_action);
        }
    }
}

void always_run_on_any_exit(const Runnable on_exit, const SignalHandler signal_handler) {
    if (atexit(on_exit) != 0) {
        perror("atexit(on_exit)");
        err(EXIT_FAILURE, "atexit");
    }
    register_for_all_signals(signal_handler);
}