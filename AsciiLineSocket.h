//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : AsciiLineSocket.h
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/12/13 20:09:34 $
//  $Revision: 1.5 $
//  $RCSfile: AsciiLineSocket.h,v $
//  $Author: niels $
//=========================================================================
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//=========================================================================

#ifndef __ASCIILINESOCKET_H__
#define __ASCIILINESOCKET_H__

//-------------------------------------------------------------------------

#include "GenericSocket.h"

//-------------------------------------------------------------------------

class AsciiLineSocket : public GenericSocket
{
public:
    AsciiLineSocket();
    AsciiLineSocket(string hostName, unsigned short portNumber);
    ~AsciiLineSocket();

    // Start/Stop showing the socket commands on cout
    void 
    DebugSocketCommands(bool show = true);

    // Send the formatted string to the socket
    int
    Sendf(const char *fmt, ...);

    // Send the formatted command string to the socket
    int
    SendCommandf(const char *fmt, ...);

    // Send the command string to the socket
    int
    SendCommand(const char *buffer);


    // The first line the NNTP or HTTP server sends back always contains a
    // status code. We want to extract this status code.
    // Returns -1 in case of socket error
    int 
    GetResponse(string &completeResponseLine, bool keepEOL = true);

    // The NNTP and HTTP protocols send information until there is a singleline
    // containing just ".\r". This function returns true if this line is 
    // NOT ".\r". This can be used in: while(GetLine(x)){ ... }
    bool
    GetLine(string &completeResponseLine, bool keepEOL = true);

private:
    void   init_line_buffer();
    char * line_buffer; // Internal buffer for GetResponse and GetLine
    int    line_buffer_size;
    bool   fVerbosePrintCommands;
};

//-------------------------------------------------------------------------

#endif // __ASCIILINESOCKET_H__

// End of the file AsciiLineSocket.h
//=========================================================================
