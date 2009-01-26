/* timeout: execute a command with specified timeout
 *
 * PLD Linux program, 
 * Copyright (C) 1999 Pawel Wilk <siefca@pld.org.pl>,
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */

#ifndef	INVOKES__H
#define	INVOKES__H

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "defaults.h"
#include "variables.h"
#include "localestuff.h"

void exec_here (char *cmd);
void invoke_here (char *cmd);

#endif