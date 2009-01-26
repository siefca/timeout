/* timeout: execute a command with specified timeout
 *
 * PLD Linux program, 
 * Copyright (C) 1999-2003 Pawel Wilk <siefca@pld.org.pl>,
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */
 

#ifndef DEFAULTS__H
#define DEFAULTS__H

#include <sys/types.h>
#include <errno.h>
#include <signal.h>

#ifndef HAVE_SIGBLOCK
#include "../compat/sigblock.h"
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define	EXIT_CODE	2		/* default exit code when timeout */
#define	SIG_TO_SEND	SIGINT		/* default signal to send	  */
#define	MAX_ARGS	1024		/* max. arguments to get	  */
#define	MAX_COMMAND_LEN	256		/* max. length of command name	  */
#define	I_PID_SIZE	10		/* max. number of additional processes */
#define MICRO_WAIT	50

#endif