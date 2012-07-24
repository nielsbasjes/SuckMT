//=========================================================================
//                 Copyright (C)1999-2012 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//   If you reuse code from SuckMT you are required to put a notice of 
//   this fact in both your manual and about box.
//
//=========================================================================
//
// This file contains code from sendmail 8.9.3 which has been modified
// Originating files are conf.h and conf.c
// A _LOT_ of code/defines has been removed because it was unneeded.
//
//=========================================================================

#ifdef _MSC_VER
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include "SetProcTitle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <omnithread.h>

//-------------------------------------------------------------------------

/*
 * Copyright (c) 1998 Sendmail, Inc.  All rights reserved.
 * Copyright (c) 1983, 1995-1997 Eric P. Allman.  All rights reserved.
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the sendmail distribution.
 *
 */

#ifndef WIN32
# include <sys/ioctl.h>
# include <sys/param.h>
# include <limits.h>
# include <sys/param.h>
#endif
# include <sys/types.h>
# include <sys/stat.h>
#ifndef WIN32
#ifndef __QNX__
/* in QNX this grabs bogus LOCK_* manifests */
# include <sys/file.h>
#endif
# include <sys/wait.h>
#endif
# include <limits.h>
# include <fcntl.h>
# include <signal.h>
#ifndef WIN32
# include <netdb.h>
# include <pwd.h>
#endif

//-------------------------------------------------------------------------
# define MAXLINE 2048            /* max line length */

/* make a copy of a string */
#define newstr(s)	strcpy(xalloc(strlen(s) + 1), s)

#define SPACELEFT(buf, ptr)  (sizeof buf - ((ptr) - buf))
//-------------------------------------------------------------------------


/**********************************************************************
**  Operating system configuration.
**
**	Unless you are porting to a new OS, you shouldn't have to
**	change these.
**********************************************************************/

/*
**  Linux 0.99pl10 and above...
*/

#ifdef __linux__
# define SPT_PADCHAR	'\0'		/* pad process title with nulls */
#endif

/**********************************************************************
**  End of Per-Operating System defines
**********************************************************************/

/*
**  Arrange to use either varargs or stdargs
*/

# ifdef __STDC__

# include <stdarg.h>

# define VA_LOCAL_DECL	va_list ap;
# define VA_START(f)	va_start(ap, f)
# define VA_END		va_end(ap)

# else

# include <varargs.h>

# define VA_LOCAL_DECL	va_list ap;
# define VA_START(f)	va_start(ap)
# define VA_END		va_end(ap)

# endif

/*
**  SETPROCTITLE -- set process title for ps
**
**	Parameters:
**		fmt -- a printf style format string.
**		a, b, c -- possible parameters to fmt.
**
**	Returns:
**		none.
**
**	Side Effects:
**		Clobbers argv of our main procedure so ps(1) will
**		display the title.
*/

#define SPT_NONE	0	/* don't use it at all */
#define SPT_REUSEARGV	1	/* cover argv with title information */
#define SPT_BUILTIN	2	/* use libc builtin */
#define SPT_PSTAT	3	/* use pstat(PSTAT_SETCMD, ...) */
#define SPT_PSSTRINGS	4	/* use PS_STRINGS->... */
#define SPT_SYSMIPS	5	/* use sysmips() supported by NEWS-OS 6 */
#define SPT_SCO		6	/* write kernel u. area */
#define SPT_CHANGEARGV	7	/* write our own strings into argv[] */

#ifndef SPT_TYPE
# define SPT_TYPE	SPT_REUSEARGV
#endif

#if SPT_TYPE != SPT_NONE && SPT_TYPE != SPT_BUILTIN

# if SPT_TYPE == SPT_PSTAT
#  include <sys/pstat.h>
# endif
# if SPT_TYPE == SPT_PSSTRINGS
#  include <machine/vmparam.h>
#  include <sys/exec.h>
#  ifndef PS_STRINGS	/* hmmmm....  apparently not available after all */
#   undef SPT_TYPE
#   define SPT_TYPE	SPT_REUSEARGV
#  else
#   ifndef NKPDE			/* FreeBSD 2.0 */
#    define NKPDE 63
typedef unsigned int	*pt_entry_t;
#   endif
#  endif
# endif

# if SPT_TYPE == SPT_PSSTRINGS || SPT_TYPE == SPT_CHANGEARGV
#  define SETPROC_STATIC	static
# else
#  define SETPROC_STATIC
# endif

# if SPT_TYPE == SPT_SYSMIPS
#  include <sys/sysmips.h>
#  include <sys/sysnews.h>
# endif

# if SPT_TYPE == SPT_SCO
#  include <sys/immu.h>
#  include <sys/dir.h>
#  include <sys/user.h>
#  include <sys/fs/s5param.h>
#  if PSARGSZ > MAXLINE
#   define SPT_BUFSIZE	PSARGSZ
#  endif
# endif

# ifndef SPT_PADCHAR
#  define SPT_PADCHAR	' '
# endif

#endif /* SPT_TYPE != SPT_NONE && SPT_TYPE != SPT_BUILTIN */

# ifndef SPT_BUFSIZE
#  define SPT_BUFSIZE	MAXLINE
# endif

/*
**  Pointers for setproctitle.
**	This allows "ps" listings to give more useful information.
*/

static char  **Argv = NULL;		/* pointer to argument vector */
static char  *LastArgv = NULL;	/* end of argv */
static char  *ApplicationPrefix;

//-------------------------------------------------------------------------

/*
**  XALLOC -- Allocate memory and bitch wildly on failure.
**
**	THIS IS A CLUDGE.  This should be made to give a proper
**	error -- but after all, what can we do?
**
**	Parameters:
**		sz -- size of area to allocate.
**
**	Returns:
**		pointer to data region.
**
**	Side Effects:
**		Memory is allocated.
*/

char *
xalloc(register int sz)
{
	register char *p;

	/* some systems can't handle size zero mallocs */
	if (sz <= 0)
		sz = 1;

	p = (char*)malloc((unsigned) sz);
	return (p);
}

//-------------------------------------------------------------------------

void
InitSetProcTitle(int argc,char **argv,char **envp,
                 const char * applicationPrefix)
{
	register int i, envpsize = 0;
	extern char **environ;

	/*
	**  Move the environment so setproctitle can use the space at
	**  the top of memory.
	*/

	for (i = 0; envp[i] != NULL; i++)
		envpsize += strlen(envp[i]) + 1;
	environ = (char **) xalloc(sizeof (char *) * (i + 1));
	for (i = 0; envp[i] != NULL; i++)
		environ[i] = newstr(envp[i]);
	environ[i] = NULL;

	/*
	**  Save start and extent of argv for setproctitle.
	*/

	Argv = argv;
    
    // This is definately a memory leak.
    ApplicationPrefix = strdup(applicationPrefix); // MLK !!

	/*
	**  Determine how much space we can use for setproctitle.  
	**  Use all contiguous argv and envp pointers starting at argv[0]
 	*/
	for (i = 0; i < argc; i++)
	{
		if (i==0 || LastArgv + 1 == argv[i])
			LastArgv = argv[i] + strlen(argv[i]);
		else
			continue;
	}
	for (i=0; envp[i] != NULL; i++)
	{
		if (LastArgv + 1 == envp[i])
			LastArgv = envp[i] + strlen(envp[i]);
		else
			continue;
	}
}

//-------------------------------------------------------------------------

#if SPT_TYPE != SPT_BUILTIN

#ifdef WIN32
void
setproctitle(const char * /*fmt*/, ...)
{
    // Not implemented on Windows
}
#else
/*VARARGS1*/
void
setproctitle(const char *fmt, ...)
{
# if SPT_TYPE != SPT_NONE
	register char *p;
	register int i;
	SETPROC_STATIC char buf[SPT_BUFSIZE];
	VA_LOCAL_DECL
#  if SPT_TYPE == SPT_PSTAT
	union pstun pst;
#  endif
#  if SPT_TYPE == SPT_SCO
	off_t seek_off;
	static int kmem = -1;
	static int kmempid = -1;
	struct user u;
#  endif

	p = buf;

	/* print application name heading for grep */
//	(void) strcpy(p, ApplicationPrefix);
	(void) strcpy(p, "SuckMT : ");
	p += strlen(p);

	/* print the argument string */
	VA_START(fmt);
	(void) vsnprintf(p, SPACELEFT(buf, p), fmt, ap);
	VA_END;

	i = strlen(buf);

#  if SPT_TYPE == SPT_PSTAT
	pst.pst_command = buf;
	pstat(PSTAT_SETCMD, pst, i, 0, 0);
#  endif
#  if SPT_TYPE == SPT_PSSTRINGS
	PS_STRINGS->ps_nargvstr = 1;
	PS_STRINGS->ps_argvstr = buf;
#  endif
#  if SPT_TYPE == SPT_SYSMIPS
	sysmips(SONY_SYSNEWS, NEWS_SETPSARGS, buf);
#  endif
#  if SPT_TYPE == SPT_SCO
	if (kmem < 0 || kmempid != getpid())
	{
		if (kmem >= 0)
			close(kmem);
		kmem = open(_PATH_KMEM, O_RDWR, 0);
		if (kmem < 0)
			return;
		(void) fcntl(kmem, F_SETFD, 1);
		kmempid = getpid();
	}
	buf[PSARGSZ - 1] = '\0';
	seek_off = UVUBLK + (off_t) u.u_psargs - (off_t) &u;
	if (lseek(kmem, (off_t) seek_off, SEEK_SET) == seek_off)
		(void) write(kmem, buf, PSARGSZ);
#  endif
#  if SPT_TYPE == SPT_REUSEARGV
	if (i > LastArgv - Argv[0] - 2)
	{
		i = LastArgv - Argv[0] - 2;
		buf[i] = '\0';
	}
	(void) strcpy(Argv[0], buf);
	p = &Argv[0][i];
	while (p < LastArgv)
		*p++ = SPT_PADCHAR;
	Argv[1] = NULL;
#  endif
#  if SPT_TYPE == SPT_CHANGEARGV
	Argv[0] = buf;
	Argv[1] = 0;
#  endif
# endif /* SPT_TYPE != SPT_NONE */
}
# endif // WIN32

#endif /* SPT_TYPE != SPT_BUILTIN */

/*
**  SM_SETPROCTITLE -- set process task and set process title for ps
**
**	Possibly set process status and call setproctitle() to
**	change the ps display.
**
**	Parameters:
**		status -- whether or not to store as process status
**		fmt -- a printf style format string.
**		a, b, c -- possible parameters to fmt.
**
**	Returns:
**		none.
*/

#ifdef WIN32
void
SetProcTitle(const char */*fmt*/, ...)
{
}
#else
void
SetProcTitle(const char *fmt, ...)
{
	char buf[SPT_BUFSIZE];

	VA_LOCAL_DECL
	/* print the argument string */
	VA_START(fmt);
	(void) vsnprintf(buf, SPT_BUFSIZE, fmt, ap);
	VA_END;

	setproctitle("%s", buf);
}
#endif
