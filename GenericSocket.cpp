//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : GenericSocket.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2000/04/04 11:22:36 $
//  $Revision: 1.9 $
//  $RCSfile: GenericSocket.cpp,v $
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

#include "TraceLog.h"
#include "GenericSocket.h"
#include "StatisticsKeeper.h"
#include <iostream.h>
#include <iomanip.h>

//-------------------------------------------------------------------------

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------------

#ifndef INADDR_NONE
#define INADDR_NONE -1
#endif

//------------------------------------------------------------------------

#ifdef WIN32
/* remove all alarm() calls neatly */
#   define alarm(x)

#   define SocketRead(x,y,z) recv(x,y,z,0)
#   define SocketWrite(x,y,z) send(x,y,z,0)
#   define strcasecmp(x,y) stricmp(x,y)
#else
#   define closesocket(x) close(x)
#   define SocketRead(x,y,z) read(x,y,z)
#   define SocketWrite(x,y,z) write(x,y,z)
/* this is for "-ansi -Wall -pedantic" to stop complaining! */
extern int strcasecmp(const char *s1, const char *s2);
#endif

MultiStream Lsocketcalls (LOGS_CALLS,"SOCKET",SHOW_TYPE|TO_COUT|TO_FILE);

//------------------------------------------------------------------------
// Construction
//------------------------------------------------------------------------

GenericSocket::GenericSocket()
{
    socketCreateTime = time(NULL);
    sendBytes = 0;
    receivedBytes = 0;
    connected = false;
}

//------------------------------------------------------------------------

GenericSocket::GenericSocket(
        const string hostName, 
        const unsigned short portNumber,
        const int socketNr)
{
    fSocketNr = socketNr;
    socketCreateTime = time(NULL);
    sendBytes = 0;
    receivedBytes = 0;
    connected = false;
    Connect(hostName,portNumber);
}

//------------------------------------------------------------------------
// Destruction
//------------------------------------------------------------------------

GenericSocket::~GenericSocket()
{
    PrintStatistics(Linfo);
    Linfo << endl << flush;

    closesocket(socketHandle);

#ifdef WIN32
    if (connected)
        WSACleanup();
#endif
}

//------------------------------------------------------------------------

bool
GenericSocket::SetConnectionParams(
        const string hostName, 
        const unsigned short portNumber)
{
    /* this is for the lame win32 socket crap */
#ifdef WIN32
    WORD    wVersionRequested;  
    WSADATA wsaData; 
    int     err; 
    wVersionRequested = MAKEWORD(1, 1); 
    
    err = WSAStartup(wVersionRequested, &wsaData); 
    
    if (err != 0) 
        /* Tell the user that we couldn't find a useable */ 
        /* winsock.dll.     */ 
        return false; 
    
    /* Confirm that the Windows Sockets DLL supports 1.1.*/ 
    /* Note that if the DLL supports versions greater */ 
    /* than 1.1 in addition to 1.1, it will still return */ 
    /* 1.1 in wVersion since that is the version we */ 
    /* requested. */ 
    
    if (LOBYTE( wsaData.wVersion ) != 1 || 
        HIBYTE( wsaData.wVersion ) != 1 ) 
    { 
        /* Tell the user that we couldn't find a useable */ 
        /* winsock.dll. */ 
        WSACleanup(); 
        return false; 
    }
#endif
    
    port = portNumber;
    alarm(60*30); /* 30 minutes */
    
    hostEntry = GetHostEntry(hostName);
    if(hostEntry == NULL) 
    {
        Lerror << "Couldn't resolv '" << hostName.c_str() << "', exiting."
               << endl << flush;
        return false;
    }

    return true;
}

//------------------------------------------------------------------------

bool
GenericSocket::Connect(const string hostName, const unsigned short portNumber)
{
    if (SetConnectionParams(hostName,portNumber))
        Connect();
    return connected;
}

//------------------------------------------------------------------------

struct hostent * 
GenericSocket::GetHostEntry(const string hostname)
{
    struct hostent *hostEnt = NULL;
    
    unsigned long inetAddress = inet_addr(hostname.c_str());
    
    if ( inetAddress != INADDR_NONE ) 
    {                                                              
        hostEnt = gethostbyaddr((char *)&inetAddress, sizeof(inetAddress), AF_INET);
        
        if ( hostEnt == NULL )
        {
            Lerror << "gethostbyaddr(1) failed for " 
                   << hostname.c_str() << endl << flush;
        }
    } 
    else 
    {
        hostEnt=gethostbyname(hostname.c_str());

        if ( hostEnt == NULL ) 
        {
            Lerror << "gethostbyaddr(2) failed for " 
                   << hostname.c_str()  << endl << flush;
        }   
    }
    return (hostEnt);
}

//------------------------------------------------------------------------

bool
GenericSocket::Connect()
{
    socketHandle = socket(AF_INET, SOCK_STREAM, 0);
    
    memset((char *) &serv_addr, '\0', sizeof(serv_addr));
    memcpy((char *)&(serv_addr.sin_addr), hostEntry->h_addr, hostEntry->h_length);
    serv_addr.sin_family = hostEntry->h_addrtype;
    serv_addr.sin_port = htons(port);
    
    if (connect(socketHandle, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
#ifdef WIN32
        Lerror << "Can't connect to server." << endl << flush;
#else
        switch(errno) 
        {
            /* this should be made nicer */
        case ECONNREFUSED:
            Lerror << "Connection refused." << endl << flush;
            break;
        default:
            Lerror << "Can't connect to server." << endl << flush;
            break;
        }
#endif        
        return false;
    }
    connected = true;
    
    return true;
}

//------------------------------------------------------------------------
bool
GenericSocket::IsConnected()
{
    return (connected);
}

//------------------------------------------------------------------------
// Send 0x00 terminated ACSII string pointed to by 'buffer' 
// Returns number of bytes sent or -1 in case of error.

int 
GenericSocket::Send(const char *buffer)
{
    if (buffer==NULL)
    {
        Lfatal << "CANNOT SEND: Provided buffer == NULL !!!" << endl << flush;
        return -1;
    }
    
    return Send(buffer,strlen(buffer));
}

//------------------------------------------------------------------------
// Send 'len' bytes pointed to by 'buffer' 
// Returns number of bytes sent or -1 in case of error.

int 
GenericSocket::Send(const char *buffer, const unsigned int len)
{
    if (buffer==NULL)
    {
        Lfatal << "CANNOT SEND: Provided buffer == NULL !!!" << endl << flush;
        return -1;
    }
        
    if (!connected)
    {
        Lfatal << "CANNOT SEND: NOT CONNECTED !!!" << endl << flush;
        return -1;
    }

    STAT_AddValue("Socket Send Bytes",len);
    sendBytes += len;

    Lsocketcalls << "Send " << len << " bytes (total " 
                 << sendBytes << "): \""<<buffer<<"\"." << endl << flush;

    return(SocketWrite(socketHandle,buffer,len));
}

//------------------------------------------------------------------------
// Receive maximum of 'maxlen' bytes and store them in the preallocated 'buffer'.
// Returns Number Of Bytes stored in 'buffer' or -1 in case of error
// Stop reading at the end of the stream. 
// When 'usestopChar' is true stop reading when 'stopChar' is found.
// When 'usestopChar' is true then 'storestopCharInBuffer' determined if the 'stopChar' 
// is stored in 'buffer'.

int 
GenericSocket::__Receive(char *buffer, int maxlen, char stopChar, 
        bool usestopChar, bool storeStopCharInBuffer)
{
    char * realBuffer  = buffer;
    int  byteCounter   = 0;
    bool stop          = !KeepRunning();
    char receivedChar  = 0x00;

    if (buffer==NULL)
    {
        Lfatal << "CANNOT RECEIVE: Provided buffer == NULL !!!" << endl << flush;
        return -1;
    }

    if (!connected)
    {
        Lfatal << "CANNOT RECEIVE: NOT CONNECTED !!!" << endl << flush;
        return -1;
    }

    // Try to read as many bytes as specified in maxlen
    while (!stop && byteCounter < maxlen && KeepRunning()) 
    {
        // Try to get 1 byte
        switch(SocketRead(socketHandle, &receivedChar, 1))
        {
            case 1: // We got 1 byte.
                    // Is this the byte where we should stop ? 
                    if (usestopChar && receivedChar == stopChar)
                    {
                        stop = true; // We stop reading bytes NOW.

                        // Store th stop char ?
                        if (storeStopCharInBuffer)
                        {
                            // We got 1 byte, now store it.
                            *buffer++ = receivedChar;

                            // We read and stored one byte
                            byteCounter ++; 
                        }
                    }
                    else
                    {
                        // We got 1 byte, now store it.
                        *buffer++ = receivedChar;

                        // We read and stored one byte
                        byteCounter ++; 
                    }

                    break;
            //------------
            case 0: // We got 0 bytes --> End of stream
                    connected = false;
                    Lerror << "Connection " << fSocketNr << " ended." << endl << flush;
                    return byteCounter; 

            //------------
            default: // We got some kind of error condition.
                    connected = false;
                    Lerror << "Connection " << fSocketNr << " failed." << endl << flush;
                    return -1;
        } // switch
    } // while

    // In case someone descides to interpret this as an ASCII string we
    // put a NULL terminator at the end.
    *buffer = 0x00;

    STAT_AddValue("Socket Received Bytes",byteCounter);
    receivedBytes += byteCounter;

//    PrintStatistics(cout);
    
    Lsocketcalls << "Received " << byteCounter << " bytes (total " 
                 << receivedBytes << "): \""<<realBuffer<<"\"." << endl << flush;

    return byteCounter;
}

//------------------------------------------------------------------------

// Receive maximum of 'maxlen' bytes and store them in the preallocated 'buffer'.
// Stop reading at the end of the stream. 
// Returns Number Of Bytes stored in 'buffer' or -1 in case of error
int 
GenericSocket::Receive (char *buffer, int maxlen)
{
    return __Receive(buffer, maxlen, ' ', false, false);
}

//------------------------------------------------------------------------
// Receive maximum of 'maxlen' bytes and store them in the preallocated 'buffer'.
// Stop reading at the end of the stream. 
// Stop reading when 'stopChar' is found.
// 'storestopCharInBuffer' determines if the 'stopChar' is stored in 'buffer'.
// Returns Number Of Bytes stored in 'buffer' or -1 in case of error

int 
GenericSocket::Receive (char *buffer, int maxlen, 
        char stopChar, bool storestopCharInBuffer)
{
    return __Receive(buffer, maxlen, stopChar, true, storestopCharInBuffer);
}

//------------------------------------------------------------------------

void
GenericSocket::PrintStatistics(ostream &os)
{
    time_t usedTime = time(NULL) - socketCreateTime;

    if (fSocketNr == 0)
        os << "Socket : ";
    else
        os << "Socket " << fSocketNr << ": ";
        
    os << "OUT: " << setw(7) << sendBytes << " bytes, " 
       << "IN: " << setw(9) <<  receivedBytes << " bytes, AVG: " << setw(7);
    if (usedTime==0)
        os << (receivedBytes) << " bytes/second"; // We assume 1 second
    else
        os << (receivedBytes)/(usedTime) << " bytes/second";
}

//-------------------------------------------------------------------------

// End of the file GenericSocket.cpp
//=========================================================================
