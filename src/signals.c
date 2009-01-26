/* timeout: execute a command with specified timeout
 *
 * PLD Linux program,
 * Copyright (C) 1999 Pawel Wilk <siefca@pld.org.pl>,
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */
 
#include <stdlib.h>
#include "signals.h"
#include "localestuff.h"

static pid_t cpid;				/* PID of first child */

static int signal_operation;

/* adapted from 'killall' code */

typedef struct {
    int number;
    const char *name;
} SIGNAME;

static SIGNAME signals[] = {
#include "signames.h"
  { 0,NULL }};

/* EOF overtake :) */

/******************************************************************************/

void signals_handler (int signum)
    {
    signal_operation = signum;
    }

/******************************************************************************/

void sigalrm_handler ()
    {
    alarm (0);
    if (ST.expected_kill)	/* killing */
	{
	VERB fprintf (stderr, _("timeout: killing process %s [%d]\n"), 
			      command_name, cpid);
	if (kill_msg) fprintf (stderr, "%s\n", kill_msg);
	(void) kill (cpid, SIGKILL);
	if (ST.to_group) (void) killpg (cpid, SIGKILL);
	cpid = 0;
	if (kill_cmd) invoke_here (kill_cmd);
	exit (ST.return_signal?SIGKILL:ST.kill_code);
	}
			
	/* signaling */
	
        VERB fprintf (stderr, _("timeout: timeout for process %s [%d]\n"
				"timeout: sending signal: %s (%d).\n"),
		      command_name, cpid, strsignal (ST.sig_to_send), 
		      ST.sig_to_send);
	if (timeout_msg) fprintf (stderr, "%s\n", timeout_msg);
	(void) kill (cpid, ST.sig_to_send);
	if (ST.to_group) (void) killpg (cpid, ST.sig_to_send);
	if (timeout_cmd) invoke_here (timeout_cmd);
	if (ST.kill_after)
	    {
	    ST.expected_kill = 1;
	    usleep ((unsigned long)MICRO_WAIT);
	    alarm (ST.kill_after);
	    return;
	    }
	exit (ST.return_signal?ST.sig_to_send:ST.exit_code);
    }

/******************************************************************************/

void sigchld_handler ()
    {
    int status;
    pid_t hp;

    hp = waitpid (-1, &status, WNOHANG);
    
    if (cpid && hp == cpid)    /* our first child */
	{
	    cpid = 0;
	    signal (SIGALRM, SIG_IGN);		     /* fuck out da timer */
	    alarm (0);
	    if (!WIFSIGNALED(status) || WTERMSIG(status) != ST.sig_to_send)
		{
		VERB fprintf (stderr, _("timeout: execution of child process finished before %s"),
		              ST.expected_kill ? _("kill.\n") : _("timeout.\n"));
		if (success_msg) fprintf (stderr, "%s\n", success_msg);
		if (success_cmd) invoke_here (success_cmd);
		}
	    else /* automagically killed by KILL signal */
		{
		if (ST.expected_kill) /* really? */
		    {
    		    if (ST.return_signal) 
			exit ((WIFSIGNALED(status))?WTERMSIG(status):0);
    		    else
			exit (ST.exit_code);
    		    }
		//else
		//    exit (ST.return_signal?ST.sig_to_send:ST.exit_code);
		}
	}
	
    if (ST.wait_for_cmds && (is_one_of_waiting(hp)))	/* some command */
	return;	/* wait for next command to finish */
    
    /* if we're here that means we don't have to wait for any additional */
    /* commands, and there was no KILL signall after timeout arrival	 */
    
    if (ST.expected_kill) /* if this is set that means there was some command */
	return;		  /* just finished but we have to wait for KILL       */

    if (ST.always_zero) exit (0);
    if (ST.return_signal) exit ((WIFSIGNALED(status))?WTERMSIG(status):0);
    exit (WIFEXITED(status)?WEXITSTATUS(status):0);
    }

/******************************************************************************/

void sighup_handler ()
    {
    if (ST.dont_hup) return;	/* already done... i know, i'm paranoid :> */
    VERB fprintf (stderr, _("timeout: refresh requested... "));
    if (!ST.expected_kill)		/* you cannot stop me from KILL ;-> */
	{
	alarm (ST.timeout_value);
	VERB fprintf (stderr, _("reset to %d seconds\n"), ST.timeout_value);
	}
    else VERB fprintf (stderr, _("but cannot be preproccessed (killing state)\n"));
    }

/******************************************************************************/

void dangerous_handler (void)
    {
    sigalrm_handler ();
    }

/******************************************************************************/

void init_signals ()
    {
    int x;
    struct sigaction sa;
    
    sigaction (SIGALRM,0,&sa);			/* old action */
    sa.sa_handler = signals_handler;		/* little update */
    sa.sa_flags &= ~SA_RESTART;			/* do NOT restart system call */
    sa.sa_flags &= ~SA_ONESHOT;			/* do NOT reset to DFL */
    sigaction (SIGALRM,&sa,0);			/* brand new action :-) */
    sigaction (SIGCHLD,0,&sa);			
    sa.sa_handler = signals_handler;		
    sa.sa_flags &= ~SA_RESTART;
    sa.sa_flags &= ~SA_ONESHOT;
    sigaction (SIGCHLD,&sa,0);			
    if (!ST.dont_hup)
	{
	sigaction (SIGHUP,0,&sa);			
	sa.sa_handler = signals_handler;		
	sa.sa_flags |= SA_RESTART;		/* restart system call */
	sigaction (SIGHUP,&sa,0);
	}
    else signal (SIGHUP, SIG_IGN);
    signal (SIGINT, signals_handler);
    signal (SIGTERM, signals_handler);
    signal (SIGSEGV, signals_handler);
    
    if ((atexit(handle_exit)) == -1)
        {
        perror ("timeout");
        exit (1);
        }
    
    for (x = 0; x <= I_PID_SIZE; x++) i_pid[x] = 0;
    }	  

/******************************************************************************/

void list_signals ()
    {
    SIGNAME *sgnl;
    
    for (sgnl = signals; sgnl->name; sgnl++)
	fprintf (stdout, "%d\t%s\t%s\n", 
		 sgnl->number, sgnl->name, strsignal (sgnl->number));
    }

/******************************************************************************/

int preprocess_signal (char *signal_name)
    {
    int i = 0;
    char *b = signal_name;
    size_t litery = 0;
    SIGNAME *sgnl;

    if (signal_name == NULL) 
	{
	fprintf (stderr, _("timeout: something is wrong.. null pointer at signal_name argument!\n"));
	return (-1);
	}
    
    while (*b)
	{
	if (isalpha((int)*b))
	    {
	    if (!islower((int)*b)) litery++;
	    else 
		{
		fprintf (stderr, _ ("timeout: invaild signal specifier (SIGNAME or value requested)\n"));
		return (-1);
		}
	    }
	b++;
	}

    b = signal_name;
    
    if (litery)
	{
        for (sgnl = signals; sgnl->name; sgnl++)
	    if (!strcmp(sgnl->name, signal_name)) break;
	if (sgnl->name && sgnl->number) 
	    return (sgnl->number);
	fprintf (stderr, _ ("timeout: invaild signal name, type -l to get list.\n"));
	return (-1);
	}
    else
	{
	if (!isdigit((int)*b)) 
	    {
	    fprintf (stderr, _ ("timeout: invaild signal specifier (digit expected).\n"));
	    return (-1);
	    }
	}

    i = atoi (signal_name);
    return (i);
    }

/******************************************************************************/

void start_da_signal_game ()
    {
    alarm (ST.timeout_value);				/* set alarm */
    while (1) 
	{
	sigblock (sigmask (ST.dont_hup?0:SIGHUP) | sigmask (SIGCHLD) | sigmask (SIGALRM));
	while (!signal_operation)
	    sigpause (0);		     /* wait for some signals */
	
	switch (signal_operation)
	    {
	    case SIGALRM: sigalrm_handler (); break; /* TIMEOUT HAS ARRIVED */
	    case SIGCHLD: sigchld_handler (); break; /* PROCESS HAS EXITED  */
	    case SIGHUP: sighup_handler (); break;   /* REFRESH REQUESTED   */
	    case SIGINT: 
	    case SIGTERM:
	    case SIGSEGV: dangerous_handler (); break; /* FALSE TIMEOUT */
	    default: break;
	    }
	signal_operation = 0;
	}
    }

/******************************************************************************/

void handle_exit (void)
    {
    int x;
    
    if (ST.wait_for_cmds && (is_waiting()))
	VERB fprintf (stderr, _("timeout: waiting for additional processes to finish...\n"));

    signal (SIGCHLD, SIG_IGN);
    if (ST.wait_for_cmds)
	{
	while ((x = is_waiting())) 
	    {
	    (void) waitpid (i_pid[x], NULL, 0);
	    VERB fprintf (stderr, _("timeout: [%d] - finished\n"), i_pid[x]);
	    i_pid[x] = 0;
	    }
	}

    VERB fprintf (stderr, _("timeout: exiting...\n"));
    sleep (ST.sleep_before);
    }

/******************************************************************************/

void signals_waiting_invocation (char *cmd)
    {
    cpid = fork ();
    if (cpid == -1)
	{
	perror ("timeout");
	ST.sleep_before = 0;
	exit (1);
	}
	
    if (!cpid) 	/* CHILD CODE  */
	exec_here (cmd);
    else    	/* PARENT CODE */
        start_da_signal_game ();
    }

/******************************************************************************/

int is_waiting ()
    {
    int x;
    
    for (x = 0; x <= I_PID_SIZE; x++)
	if (i_pid[x] != 0) return (x);
    return (0);
    }
    
/******************************************************************************/

int is_one_of_waiting (pid_t p)
    {
    int x;
    
    for (x = 0; x <= I_PID_SIZE; x++)
	if (i_pid[x] != 0 && i_pid[x] == (int) p) 
	    {
	    i_pid[x] = 0;	/* WARNING */
	    return (1);
	    }
    return (0);
    }
    
/******************************************************************************/

void nothing (int x)
    {
    x = 0;
    }

/******************************************************************************/