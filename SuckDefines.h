//=========================================================================
//                 Copyright (C)1999-2005 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT
//                       Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Project: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Id: SuckDefines.h 11 2004-12-26 20:06:37Z niels $
//  $Date: 2004-12-26 21:06:37 +0100 (Sun, 26 Dec 2004) $
//  $Revision: 11 $
//  $LastChangedBy: niels $
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

#ifndef __SUCKDEFINES_H__
#define __SUCKDEFINES_H__

//-------------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

//-------------------------------------------------------------------------

#ifdef VERSION
#undef  SUCKMT_VERSION 
#define SUCKMT_VERSION VERSION
#else
#ifndef SUCKMT_VERSION
#define SUCKMT_VERSION "????"
#endif
#endif

//-------------------------------------------------------------------------

#ifndef SUCK_CONFIG_FILE
#define SUCK_CONFIG_FILE       "/etc/suckmt/suckmt.ini"
#endif

#define SUCK_COPYRIGHT         "Suck MT Copyright Information"
#define SUCK_LICENSE           "Suck MT License"
#define SUCK_WEBSITE           "The Suck MT website"
#define SUCK_AUTHOR            "Author"
#define SUCK_EMAIL             "You can email the author at"
#define SUCK_FIRST_VERSION     "This file created by version"
#define SUCK_LATEST_VERSION    "This file modified by version"

#define SUCK_INSTALL           "Suck MT Installation"
#define SUCK_RUN               "Run number"
#define SUCK_INSTALL_DATE      "Date of first run"
#define SUCK_LASTRUN_DATE      "Date of last run"

#define SUCK_CONFIG            "Suck MT Configuration"
#define SUCK_NEWS_SERVER       "News server Hostname"
#define SUCK_NNTP_PORT         "Connect with Port Number"
#define SUCK_DEBUG_SOCKET      "Debug SOCKET commands"
#define SUCK_SEND_MODEREADER   "Send MODE READER to the server"
#define SUCK_NNTP_USERNAME     "Connect with Username"
#define SUCK_NNTP_PASSWORD     "Connect with Password"
#define SUCK_KEEP_CR           "Store message with the CarriageReturn (^M)"
#define SUCK_DIR               "Storage Directory"
#define SUCK_BATCH_FILE        "Batch File"
#define SUCK_THREADS           "Number of threads"
#define SUCK_CONNECT_RETRIES   "Number of retries if connection fails"
#define SUCK_RESTART_FILE      "Restart File"
#define SUCK_LOGLEVEL          "Logging amount [0(nothing)..7(noise)]"

#define SUCK_GLOBAL_KILL_RULES "Global Kill Rules"
#define SUCK_MIN_LINES         "Minimum Lines in a Message"
#define SUCK_MAX_LINES         "Maximum Lines in a Message"
#define SUCK_MAX_BYTES         "Maximum Bytes in a Message"
#define SUCK_MAX_GROUPS        "Maximum Groups in a Message"
#define SUCK_MAX_MSG_THRESHOLD "Maximum messages per newsgroup threshold"
#define SUCK_MAX_MSG_DOWNLOAD  "Only download N messages when over threshold"

#define SUCK_KILL_HEADERS      "Kill Headers"
#define SUCK_KEEP_HEADERS      "Keep Headers"

// This is the name of the "header" field that indicates the article body
#define SUCK_ARTICLEBODY       "ArticleBody"

#define SUCK_KILL_LOGFILE         "Kill Log File"
#define SUCK_KILL_ENABLE_LOGFILE  "Enable Kill Logfile"
#define SUCK_KILL_LOGFILENAME     "Kill Log Filename"
#define SUCK_KILL_LOGFILE_HEADERS "Kill Logfile Headers"

#define SUCK_GROUPS            "News Groups"

//-------------------------------------------------------------------------

#endif // __SUCKDEFINES_H__

// End of the file SuckDefines.h
//=========================================================================
