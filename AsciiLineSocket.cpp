//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : AsciiLineSocket.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/11/18 22:46:15 $
//  $Revision: 1.4 $
//  $RCSfile: AsciiLineSocket.cpp,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include "AsciiLineSocket.h"

//-------------------------------------------------------------------------

AsciiLineSocket::AsciiLineSocket()
{
    init_line_buffer();
}

//-------------------------------------------------------------------------

AsciiLineSocket::AsciiLineSocket
    (string hostName, unsigned short portNumber) 
    : GenericSocket(hostName,portNumber)
{
    init_line_buffer();
}

//-------------------------------------------------------------------------

AsciiLineSocket::~AsciiLineSocket()
{
    if (line_buffer != NULL)
        free(line_buffer);
}

//------------------------------------------------------------------------

int 
AsciiLineSocket::Sendf(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsprintf(line_buffer, fmt, ap);
    va_end(ap);
    return(Send(line_buffer));
}

//-------------------------------------------------------------------------
// The first line the NNTP or HTTP server sends back always contains a
// status code. We want to extract this status code.

int 
AsciiLineSocket::GetResponse (string &completeResponseLine, bool keepEOL)
{
    if (Receive(line_buffer,line_buffer_size,'\n',keepEOL) == -1)
        return -1;
    completeResponseLine = line_buffer;
    
    unsigned int statusCode;
    sscanf(line_buffer,"%ud",&statusCode);
    return statusCode;
}

//--------------------------------------------------------------------
// The NNTP and HTTP protocols send information until there is a singleline
// containing just ".\r". This function returns true if this line is 
// NOT ".\r". This can be used in: while(GetLine(x)){ ... }

bool 
AsciiLineSocket::GetLine (string &completeResponseLine,bool keepEOL)
{
    Receive(line_buffer,line_buffer_size,'\n',keepEOL);
    completeResponseLine = line_buffer;

    if (keepEOL)
    {
        if (strcmp(line_buffer,".\r\n"))
            return true;
    }
    else
    {
        if (strcmp(line_buffer,".\r"))
            return true;
    }

    return false;
}

//------------------------------------------------------------------------

void
AsciiLineSocket::init_line_buffer()
{
    line_buffer_size  = 10240; // We just allocate a big enough buffer

    // The +10 is a safety buffer
    line_buffer = (char*) malloc ((line_buffer_size+10)*sizeof(char));
}


//------------------------------------------------------------------------

// End of the file AsciiLineSocket.cpp
//=========================================================================
