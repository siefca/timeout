/* timeout: execute a command with specified timeout
 *
 * PLD Linux program,
 * Copyright (C) 1999 Pawel Wilk <siefca@pld.org.pl>,
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */

#ifndef TIMEOUT__H
#define TIMEOUT__H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <string.h>
#include "signals.h"
#include "variables.h"
#include "localestuff.h"
#include "invokes.h"

void usage (char *progname);
void print_version (void);

int preprocess_code (char *code_name);
int preprocess_time (char *time_value);

#include "initlocale.h"

#endif