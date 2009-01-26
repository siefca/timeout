/* timeout: execute a command with specified timeout
 *
 * PLD Linux program,
 * Copyright (C) 1999 Pawel Wilk <siefca@pld.org.pl>,
 *
 * This is free software; see the GNU General Public License version 2
 * or later for copying conditions.  There is NO warranty.
 *
 */

#include "variables.h"
#include "timeout.h"

/******************************************************		MAIN FUNCTION */

int main (int argc, char *argv[])
    {
    int x;
    char *p;
    
    initlocale ();    
    if (!argv[1] || !(*argv[1])) { usage (argv[0]); exit (1); }
    else
    while ((x=getopt(argc, argv, "Vhlvmxzier:s:c:k:A:B:C:D:E:F:G:H:b:")) != EOF)
	{
		switch (x)
		{
			case 'V':
				print_version ();
				exit (0);
				break;
			case 'h':
				print_version ();
				usage (argv[0]);
				exit (0);
				break;
			case 'l':
				list_signals ();
				exit (0);
				break;
			case 'v':
				ST.verbose_mode = 1;
				break;
			case 'c': 
				ST.kill_code = preprocess_code (optarg);
				if (ST.kill_code == -1) exit (1);
				break;	    
			case 'r': 
				ST.exit_code = preprocess_code (optarg);
				if (ST.exit_code == -1) exit (1);
				break;
			case 's':
				ST.sig_to_send = preprocess_signal (optarg);
				if (ST.sig_to_send == -1) exit (1);
				break;
			case 'k':
				ST.kill_after = preprocess_time (optarg);
				if (ST.kill_after == -1) 
				    {
				    ST.sleep_before = 0;
				    exit (1);
				    }
				break;
			case 'm':
				ST.always_zero = 1;
				break;
			case 'x':
				ST.return_signal = 1;
				break;
			case 'z':
				ST.to_group = 0;
				break;
			case 'b':
				ST.sleep_before = preprocess_time (optarg);
				break;
			case 'i':
				ST.dont_hup = 1;
				break;
			case 'e':
				ST.wait_for_cmds = 1;
				break;
			case 'A':
				timeout_msg = strdup (optarg);
				break;
			case 'B':
				kill_msg = strdup (optarg);
				break;
			case 'C':
				success_msg = strdup (optarg);
				break;
			case 'D':
				timeout_cmd = strdup (optarg);
				break;
			case 'E':
				kill_cmd = strdup (optarg);
				break;
			case 'H':
				success_cmd = strdup (optarg);
				break;
			default :
				ST.sleep_before = 0;
				exit (1);
				break;
		}
	
	}

    VERB fprintf (stderr, "Timeout by <siefca@pld.org.pl>\n");
	
    if (argv[optind] == NULL || argv[optind] == '\0' || 
        argv[optind+1] == NULL || argv[optind+1] == '\0')
	{
	fprintf (stderr, _ ("timeout: you have to specify timeout value and a command to execute!\n"));
	ST.sleep_before = 0;
	exit (1);
	}
    
    ST.timeout_value = preprocess_time (argv[optind]);
    if (ST.timeout_value == -1 || !ST.timeout_value) ST.sleep_before = 0;
    if (ST.timeout_value == -1) exit (1);
    if (!ST.timeout_value) exit (0);

    VERB {
	    fprintf (stderr, _("timeout:  command to invoke is: %s\n"
			       "timeout:  timeout is set to %d seconds\n"
			       "timeout:  exit code after timeout is set to %d\n"
			       "timeout:  signal is set to %d (%s)\n"),
		     argv[optind+1], ST.timeout_value, ST.exit_code, ST.sig_to_send, strsignal (ST.sig_to_send));
		     
	    if (ST.kill_after)
		fprintf (stderr, _("timeout:  KILL signal will be delivered %d seconds after timeout.\n"
				   "timeout:  return code after KILL is set to %d\n"),
			ST.kill_after, ST.kill_code);
	    if (ST.always_zero)
		fprintf (stderr, _("timeout:  return code of successfuly finished proccess will be always 0\n"));
	 }

    command_name[sizeof(command_name)-1] = '\0';
    strncpy (command_name, argv[optind+1], sizeof(command_name)-2);
    if ((p = strchr (command_name, ' '))) *p = '\0';
    
    /* Let's play ;-)) */

    init_signals ();
    signals_waiting_invocation (argv[optind+1]);
        
    return (0);
    }

/******************************************************		DEFINITIONS   */

void print_version ()
    {
    fprintf (stderr, "trifle-tools, version %s -- 'timeout' by Pawel Wilk <siefca@pld.org.pl>\n"
		       "Copyright (C) 2000 Polish(ed) GNU/Linux Distribution\n"
		       "This is free software; see the GNU General Public License version 2\n"
                       "or later for copying conditions.  There is NO warranty.\n\n",
	     VERSION);
    }

void usage (char *progname)
    {
    fprintf (stderr, _("Usage: %s [options] timeout command\n\n"
		       "-b time   tells timeout to sleep the given number of seconds just before exit\n"
		       "-c code   sets exit code after KILL signal (only in combination with -k)\n"
		       "-e        tells timeout to wait for any of -D -E -F commands before exit\n"
		       "-i        tells timeout to not handle Hangup signal (to avoid timer refresh)\n"
		       "-k time   sends the KILL signal after the given number of seconds after timeout\n"
		       "-m        returns always 0 if process had exited before timeout\n"
		       "-r code   sets return code after timeout\n"
		       "-s signal sets name or number of signal to send on timeout\n"
		       "-v        enables verbose mode\n"
		       "-x        enables signal code returning instead of exit code\n"
		       "-z        disables sending signals to all processes in a group\n"
		       "-A msg    this message will be displayed when timeout arrive\n"
		       "-B msg    this message will be displayed when (if) KILL arrive\n"
		       "-C msg    this message will be displayed if process has exited before timeout\n"
		       "-D -E -F is the same as -A -B -C but invokes a COMMAND\n\n"
		       "Additional options:\n"
		       "-V        prints version and exits\n"
		       "-l        lists available signal names and exits\n"
		       "-h        this help message\n\n"), progname, (int)MICRO_WAIT);
    }

/******************************************************************************/
    
int preprocess_code (char *code_name)
    {
    char *b = code_name;
    int ret;
    
    if (code_name == NULL) 
	{
	fprintf (stderr, _ ("timeout: something is wrong.. null pointer at code_name argument!\n"));
	return (-1);
	}
	
    while (*b)
	{
	if (!isdigit((int)*b)) 
	    {
	    fprintf (stderr, _ ("timeout: invaild return code (digit expected).\n"));
	    return (-1);
	    }
	b++;
	}
    
    ret = atoi (code_name);
    
    if (ret < 0 || ret > 255)
	{
	fprintf (stderr, _ ("timeout: invaild return code value (try 0 to 255).\n"));
	return (-1);
	}
    
    return (ret);
    }

/******************************************************************************/
    
int preprocess_time (char *time_value)
    {
    char *p = time_value;
    unsigned int timeout_va;
    
    if (time_value == NULL)
	{
	fprintf (stderr, _("timeout: something is wrong.. null pointer at time_valuee argument!\n"));
	return (-1);
	}

    while (*p)
	{
	if (!isdigit((int)*p))
	    {
	    fprintf (stderr, _("timeout: invaild time value - only digits are supported.\n"));
	    return (-1);
	    }
	p++;
	}
	
    timeout_va = (unsigned int) atoi (time_value);
    
    if (timeout_va < 0 || timeout_va > 65536)
	{
	fprintf (stderr, _("timeout: invaild time value - (try 0 to 65536)\n"));
        return (-1);
	}
	
    return (timeout_va);
    }

/******************************************************************************/