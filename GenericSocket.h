//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : GenericSocket.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/03/12 21:30:52 $
//  $Revision: 1.8 $
//  $RCSfile: GenericSocket.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __GENERICSOCKET_H__
#define __GENERICSOCKET_H__

//-------------------------------------------------------------------------

#ifdef _MSC_VER
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#endif

/* -- WIN32 approved -- */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

#ifdef WIN32
#include <winsock.h>
#include <time.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#include <iostream.h>
#include <algorithm>
#include <string> 
#include "Abortable.h"

//-------------------------------------------------------------------------

// Warning Dirty hack to skip the std namespace in Visual C++ 6.0
#ifdef __WIN32__
#define string  std::string
#endif

//-------------------------------------------------------------------------

class GenericSocket  : public Abortable
{
//-----------------------------------    
public:
    GenericSocket();
    GenericSocket(const string hostName, const unsigned short portNumber,int socketNr=0);
    virtual ~GenericSocket();

    bool
    SetConnectionParams(const string hostName, const unsigned short portNumber);

    struct hostent *
    GetHostEntry(const string hostname);

    bool
    Connect();

    bool
    Connect(const string hostName, const unsigned short portNumber);

    bool
    IsConnected();

    // Send 0x00 terminated ACSII string pointed to by 'buffer' 
    // Returns number of bytes sent or -1 in case of error.
    int 
    Send (const char *buffer);

    // Send 'len' bytes pointed to by 'buffer' 
    // Returns number of bytes sent or -1 in case of error.
    int 
    Send (const char *buffer, const unsigned int len);

    // Receive maximum of 'maxlen' bytes and store them in the preallocated 'buffer'.
    // Stop reading at the end of the stream. 
    // Returns number of bytes stored in 'buffer' or -1 in case of error
    int 
    Receive (char *buffer, int maxlen);
    
    // Receive maximum of 'maxlen' bytes and store them in the preallocated 'buffer'.
    // Stop reading at the end of the stream. 
    // Stop reading when 'stopChar' is found.
    // 'storestopCharInBuffer' determines if the 'stopChar' is stored in 'buffer'.
    // Returns Number Of Bytes stored in 'buffer' or -1 in case of error
    int 
    Receive (char *buffer, int maxlen, char stopChar, bool storeStopCharInBuffer = false);

    void
    PrintStatistics(ostream &os);

//-----------------------------------    
private:

    // Receive maximum of 'maxlen' bytes and store them in the preallocated 'buffer'.
    // Returns Number Of Bytes stored in 'buffer' or -1 in case of error
    // Stop reading at the end of the stream. 
    // When 'usestopChar' is true stop reading when 'stopChar' is found.
    // When 'usestopChar' is true then 'storestopCharInBuffer' determined if the 'stopChar' 
    // is stored in 'buffer'.
    int
    __Receive(char *buffer, int maxlen, char stopChar, bool usestopChar, bool storestopCharInBuffer);
    
    struct hostent     *hostEntry;
    int                 socketHandle;
    unsigned short      port;
    bool                connected;
    struct sockaddr_in  serv_addr;
    
    int                 fSocketNr;
    
    time_t              socketCreateTime;
    unsigned long       sendBytes;
    unsigned long       receivedBytes;
};

//-------------------------------------------------------------------------

#endif // __GENERICSOCKET_H__

// End of the file GenericSocket.h
//=========================================================================
