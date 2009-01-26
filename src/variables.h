/* timeout: execute a command with specified timeout
 *
 * PLD Linux program,
 * Copyright (C) 1999 Pawel Wilk <siefca@pld.org.pl>,
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */
 
 
#ifndef VARIABLES__H
#define VARIABLES__H

#include "defaults.h"

extern pid_t i_pid[I_PID_SIZE];		/* additional commands PIDs	 */
extern pid_t *ip_pid;			/* internal pointer to last used */

typedef struct settings
	{
	int exit_code;		/* exit code after timeout		*/
	int kill_code;		/* exit code after KILL 		*/
	int sig_to_send;	/* signal to send			*/
	int kill_after;		/* time between timeout and KILL	*/
	int timeout_value; 	/* self-explanatory			*/
	int sleep_before;	/* how much sleep before exit		*/

	unsigned return_signal:1;	/* do we need to return signal ?     */
	unsigned always_zero:1;		/* return always zero on success     */
	unsigned expected_kill:1;	/* internal 			     */
	unsigned verbose_mode:1;	/* AAAAAAAAAAA!			     */
	unsigned to_group:1;		/* should we also use killpg ?	     */
	unsigned dont_hup:1;		/* should we handle HUP ?	     */
	unsigned wait_for_cmds:1;	/* should we wait for -EFGH commands */
	unsigned dummy:1;
	} sts;

extern sts ST;

extern char command_name[25];

extern char *timeout_msg;
extern char *kill_msg;
extern char *success_msg;
extern char *timeout_cmd;
extern char *kill_cmd;
extern char *success_cmd;

#ifndef	VERB
#define VERB if(ST.verbose_mode)
#endif

#endif