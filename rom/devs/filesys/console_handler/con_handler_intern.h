#ifndef __CON_HANDLER_INTERN_H
#define __CON_HANDLER_INTERN_H
/*
    Copyright (C) 1998 AROS - The Amiga Research OS
    $Id$

    Desc: Internal header-file for emulation-handler.
    Lang: english
*/

/* AROS includes */
#include <exec/libraries.h>
#include <exec/types.h>
#include <dos/dosextens.h>
#include <hidd/hidd.h>

/* POSIX includes */
#include <sys/types.h>
#include <dirent.h>

/*
** stegerg:
**
** if BETTER_WRITE_HANDLING is #defined then writes are sent to
** console.device in smaller parts (max. 256 bytes or upto next
** LINEFEED).
**
** NOTE: Could be problematic with control sequences in case of
**       the 256-Byte-Block write (write size is >256 but no LINE-
**       FEED was found in this first (or better actual) 256 bytes
**       to be written.
**
**/

#define BETTER_WRITE_HANDLING 	1
#define RMB_FREEZES_OUTPUT	1

#define CONTASK_STACKSIZE 	(AROS_STACKSIZE)
#define CONTASK_PRIORITY 	0

#define CONSOLEBUFFER_SIZE 	256
#define INPUTBUFFER_SIZE 	256
#define CMD_HISTORY_SIZE 	32

struct conTaskParams
{
    struct conbase *conbase;
    struct Task	*parentTask;
    struct IOFileSys *iofs;
    ULONG initSignal;
};

struct conbase
{
    struct Device	device;
    struct ExecBase   * sysbase;
    struct DosLibrary * dosbase;
    struct Device     * inputbase;
    struct IntuitionBase *intuibase;

    BPTR seglist;
};

struct filehandle
{
    struct IOStdReq	*conreadio;
    struct IOStdReq	conwriteio;
    struct MsgPort	*conreadmp;
    struct MsgPort	*conwritemp;
    struct Window	*window;
    struct Task 	*contask;
    struct Task		*breaktask;
    struct MsgPort      *contaskmp;
    struct MinList	pendingReads;
    struct MinList	pendingWrites;
    UBYTE		*wintitle;
#if BETTER_WRITE_HANDLING
    LONG		partlywrite_actual;
    LONG		partlywrite_size;
#endif
    WORD		conbufferpos;
    WORD		conbuffersize;
    WORD		inputstart; 	/* usually 0, but needed for multi-lines (CONTROL RETURN) */
    WORD		inputpos; 	/* cursor pos. inside line */
    WORD		inputsize; 	/* length of input string */
    WORD		canreadsize;
    WORD		historysize;
    WORD		historypos;
    WORD		historyviewpos;
    WORD		usecount;
    UWORD		flags;

    UBYTE		consolebuffer[CONSOLEBUFFER_SIZE + 2];
    UBYTE		inputbuffer[INPUTBUFFER_SIZE + 2];
    UBYTE		historybuffer[CMD_HISTORY_SIZE][INPUTBUFFER_SIZE + 1];

};

/* filehandle flags */

#define FHFLG_READPENDING  1
#define FHFLG_WRITEPENDING 2
#define FHFLG_CANREAD      4
#define FHFLG_WAIT	   8 	/* filename contained WAIT */

typedef struct IntuitionBase IntuiBase;

#ifdef SysBase
#   undef SysBase
#endif
#define SysBase conbase->sysbase
#ifdef DOSBase
#   undef DOSBase
#endif
#define DOSBase conbase->dosbase
#ifdef IntuitionBase
#   undef IntuitionBase
#endif
#define IntuitionBase conbase->intuibase
#ifdef InputBase
#   undef InputBase
#endif
#define InputBase conbase->inputbase

VOID conTaskEntry(struct conTaskParams *param);

#endif /* __CON_HANDLER_INTERN_H */
