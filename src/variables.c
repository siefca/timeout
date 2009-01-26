/* timeout: execute a command with specified timeout
 *
 * PLD Linux program,
 * Copyright (C) 1999 Pawel Wilk <siefca@pld.org.pl>,
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */

#include <stdio.h> 
#include "variables.h"

pid_t i_pid[I_PID_SIZE];
pid_t *ip_pid = i_pid;

sts ST = {	EXIT_CODE,	/* exit code after timeout		*/
		EXIT_CODE,	/* exit code after KILL 		*/
		SIG_TO_SEND,	/* signal to send			*/
		0,		/* time between timeout and KILL	*/
		0,		/* self-explanatory			*/
		0,		/* how much sleep before exit		*/
		0,		/* do we need to return signal ?     	*/
		0,		/* return always zero on success     	*/
		0,		/* internal			     	*/
		0,		/* AAAAAAAAAAA!			     	*/
		0,		/* should we also use killpg ?	     	*/
		1,		/* should we handle HUP ?	     	*/
		0,		/* should we wait for -EFGH commands 	*/
		0		/* dummy				*/
	};
		
		

char command_name[25];

char *timeout_msg = NULL;
char *kill_msg = NULL;
char *success_msg = NULL;
char *timeout_cmd = NULL;
char *kill_cmd =  NULL;
char *success_cmd = NULL;
