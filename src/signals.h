/* timeout: execute a command with specified timeout
 *
 * PLD Linux program,
 * Copyright (C) 1999 Pawel Wilk <siefca@pld.org.pl>,
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */
 

#ifndef SIGNALS__H
#define SIGNALS__H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "variables.h"
#include "invokes.h"

/* FIRESTARTER */

void start_da_signal_game (void);

/* HANDLERS */

void signals_handler (int signum);
void sigalrm_handler (void);
void sigchld_handler (void);
void sighup_handler (void);
void dangerous_handler (void);
void nothing (int x);

/* INTERFACE OPERATIONS */

void list_signals (void);
int preprocess_signal (char *signal_name);
void handle_exit (void);
int is_waiting (void);
int is_one_of_waiting (pid_t p);

/* GURU OPERATIONS */

void signals_waiting_invocation (char* cmd);
void init_signals (void);

/* talk to me... :) */

extern const char * const sys_siglist[];
extern char *strsignal (int);

#endif