/* timeout: execute a command with specified timeout
 *
 * PLD Linux program, 
 * Copyright (C) 1999 Pawel Wilk <siefca@pld.org.pl>,
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */

#include "invokes.h"
#include "variables.h"

/******************************************************************************/

void invoke_here (char *cmd)
    {
    if (cmd == NULL || *cmd == '\0') return;
    ip_pid++;
    *ip_pid = fork ();
    if (*ip_pid == -1)
	{
	perror ("timeout");
	ST.sleep_before = 0;
	exit (1);
	}
    if (!(*ip_pid)) exec_here (cmd);
    VERB fprintf (stderr, _("timeout: calling command: %s\n"), cmd);
    *cmd = '\0';
    }
    
/******************************************************************************/

void exec_here (char *cmd)
    {
    int argcount = 0;
    char *p, *q, *cm_args[MAX_ARGS];
    char cm_name[MAX_COMMAND_LEN];
    
    strncpy (cm_name, cmd, sizeof (cm_name) - 1);
    cm_name[sizeof(cm_name)-1] = '\0';
    q = p = cm_name;
    cm_args[MAX_ARGS-1] = NULL;
    while ((p = strchr (q, ' ')) && argcount < MAX_ARGS)
        {
        *p = '\0';
        cm_args[argcount] = strdup(q);
        q = p+1;
        argcount++;
        }
    cm_args[argcount] = q;
    cm_args[++argcount] = NULL;
    execvp (cm_name, cm_args);
    perror ("timeout");
    ST.sleep_before = 0;
    exit (1);
    }
    
/******************************************************************************/