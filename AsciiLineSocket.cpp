//=========================================================================
//                 Copyright (C)1999-2003 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT/
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : AsciiLineSocket.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2003/04/13 20:51:54 $
//  $Revision: 1.12 $
//  $RCSfile: AsciiLineSocket.cpp,v $
//  $Author: niels $
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

#ifdef _MSC_VER
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include "TraceLog.h"
#include "AsciiLineSocket.h"

// We set this stream to LOGS_ALWAYS because we use a different
// flag to enable or disable messaging.
MultiStream Lsocketdebug (LOGS_ALWAYS,"SOCKET",SHOW_TYPE|TO_COUT|TO_FILE);

//-------------------------------------------------------------------------

AsciiLineSocket::AsciiLineSocket()
{
    fVerbosePrintCommands = false;
    init_line_buffer();
}

//-------------------------------------------------------------------------

AsciiLineSocket::AsciiLineSocket
    (string hostName, unsigned short portNumber,int socketNr) 
    : GenericSocket(hostName,portNumber,socketNr)
{
    fVerbosePrintCommands = false;
    init_line_buffer();
}

//-------------------------------------------------------------------------

AsciiLineSocket::~AsciiLineSocket()
{
    if (line_buffer != NULL)
        free(line_buffer);
}

//------------------------------------------------------------------------
// Start/Stop showing the socket commands on Lsocketdebug
void 
AsciiLineSocket::DebugSocketCommands(bool show)
{
    fVerbosePrintCommands = show;
    if (show)
    {
        Lsocketdebug << "DEBUG mode for Ascii Socket commands enabled" << endl << flush;
    }
}

//------------------------------------------------------------------------

int 
AsciiLineSocket::Sendf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsprintf(line_buffer, fmt, ap);
    va_end(ap);
    return(Send(line_buffer));
}

//-------------------------------------------------------------------------
// Send the formatted command string to the socket
int
AsciiLineSocket::SendCommandf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsprintf(line_buffer, fmt, ap);
    va_end(ap);
    return(SendCommand(line_buffer));
}

//-------------------------------------------------------------------------
// Send the command string to the socket
int
AsciiLineSocket::SendCommand(const char *buffer)
{
    string localBuffer = buffer ;
    if (fVerbosePrintCommands)
        Lsocketdebug << ">> " << buffer << endl << flush;
    localBuffer += "\r\n";
    return(Send(localBuffer.c_str()));
}

//-------------------------------------------------------------------------
// Helper function to cleanup the incoming data
static void 
RemoveTrailingCRLF(string &str)
{
    if (str == "")
        return;
        
    while(str.begin() != str.end() && 
          (*(str.end()-1) == '\n'  ||
           *(str.end()-1) == '\r' ))
        str.erase(str.end()-1);
}

//-------------------------------------------------------------------------
// The first line the NNTP or HTTP server sends back always contains a
// status code. We want to extract this status code.

int 
AsciiLineSocket::GetResponse (string &completeResponseLine, 
                              bool keepEOL, 
                              bool keepCR)
{
    if (Receive(line_buffer,line_buffer_size,'\n',keepEOL) == -1 )
    {
        if (fVerbosePrintCommands)
            Lsocketdebug << "<< SOCKET ERROR" << endl << flush;
        return -1;
    }
    completeResponseLine = line_buffer;

    if (!keepCR)
    {
        RemoveTrailingCRLF(completeResponseLine);
        if (keepEOL)
            completeResponseLine += '\n';
    }

    if (fVerbosePrintCommands)
        Lsocketdebug << "<< " << line_buffer << endl << flush;
    
    unsigned int statusCode;
    sscanf(line_buffer,"%ud",&statusCode);
    return statusCode;
}

//--------------------------------------------------------------------
// The NNTP and HTTP protocols send information until there is a singleline
// containing just ".\r". This function returns true if this line is 
// NOT ".\r". This can be used in: while(GetLine(x)){ ... }

bool 
AsciiLineSocket::GetLine (string &completeResponseLine,
                          bool keepEOL, 
                          bool keepCR)
{
    if (Receive(line_buffer,line_buffer_size,'\n',keepEOL) == -1 )
    {
        if (fVerbosePrintCommands)
            Lsocketdebug << "<< SOCKET ERROR" << endl << flush;
        return false;
    }

    completeResponseLine = line_buffer;

    if (!keepCR)
    {
        RemoveTrailingCRLF(completeResponseLine);
        if (keepEOL)
            completeResponseLine += '\n';
    }

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
