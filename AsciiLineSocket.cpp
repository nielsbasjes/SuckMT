/***************************************************************************
                          AsciiLineSocket.cpp  -  description                              
                             -------------------                                         
    begin                : Sat Jul 17 1999                                           
    copyright            : (C) 1999 by Niels Basjes                         
    email                : Niels@Basjes.nl                                     
 ***************************************************************************/

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif


#include "AsciiLineSocket.h"

//-------------------------------------------------------------------------

FUNCTION_START(AsciiLineSocket::AsciiLineSocket())
{
    init_line_buffer();
}
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(AsciiLineSocket::AsciiLineSocket(string hostName, unsigned short portNumber) : GenericSocket(hostName,portNumber))
{
    init_line_buffer();
}
FUNCTION_END

//-------------------------------------------------------------------------

FUNCTION_START(AsciiLineSocket::~AsciiLineSocket())
{
    if (line_buffer != NULL)
        free(line_buffer);
}
FUNCTION_END

//------------------------------------------------------------------------

int 
FUNCTION_START(AsciiLineSocket::Sendf(char *fmt, ...))
{
    va_list ap;
    va_start(ap, fmt);
    vsprintf(line_buffer, fmt, ap);
    va_end(ap);
    return(Send(line_buffer));
}
FUNCTION_END

//-------------------------------------------------------------------------
// The first line the NNTP or HTTP server sends back always contains a
// status code. We want to extract this status code.

int 
FUNCTION_START(AsciiLineSocket::GetResponse (string &completeResponseLine, bool keepEOL))
{
    if (Receive(line_buffer,line_buffer_size,'\n',keepEOL) == -1)
        return -1;
    completeResponseLine = line_buffer;
    
    unsigned int statusCode;
    sscanf(line_buffer,"%ud",&statusCode);
    return statusCode;
}
FUNCTION_END

//--------------------------------------------------------------------
// The NNTP and HTTP protocols send information until there is a singleline
// containing just ".\r". This function returns true if this line is 
// NOT ".\r". This can be used in: while(GetLine(x)){ ... }

bool 
FUNCTION_START(AsciiLineSocket::GetLine (string &completeResponseLine,bool keepEOL))
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
FUNCTION_END

//------------------------------------------------------------------------

void
FUNCTION_START(AsciiLineSocket::init_line_buffer())
{
    line_buffer_size  = 10240; // We just allocate a big enough buffer

    // The +10 is a safety buffer
    line_buffer = (char*) malloc ((line_buffer_size+10)*sizeof(char));
}
FUNCTION_END

//------------------------------------------------------------------------

