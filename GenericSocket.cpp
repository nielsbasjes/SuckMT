//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : GenericSocket.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/18 21:27:24 $
//  $Revision: 1.2 $
//  $RCSfile: GenericSocket.cpp,v $
//  $Author: niels $
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//-------------------------------------------------------------------------

#include "TraceLog.h"
#include "GenericSocket.h"
#include "StatisticsKeeper.h"

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

GenericSocket::GenericSocket(string hostName, unsigned short portNumber)
{
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
    cout << "Socket statistics: ";
    PrintStatistics(cout);
    cout << endl << flush;
#ifdef WIN32
    if (connected)
        WSACleanup();
#endif
}

//------------------------------------------------------------------------

bool
GenericSocket::SetConnectionParams(string hostName, unsigned short portNumber)
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
    if(hostEntry == 0) 
    {
        lprintf(LOG_ERROR,"Couldn't resolv '%s', exiting.\n", 
                hostName.c_str());
        return false;
    }

    return true;
}

//------------------------------------------------------------------------

bool
GenericSocket::Connect(string hostName, unsigned short portNumber)
{
    if (SetConnectionParams(hostName,portNumber))
        Connect();
    return connected;
}

//------------------------------------------------------------------------

struct hostent * 
GenericSocket::GetHostEntry(string hostname)
{
    struct hostent *hostEnt = NULL;
    
    unsigned long inetAddress = inet_addr(hostname.c_str());
    
    if ( inetAddress != INADDR_NONE ) 
    {                                                              
        hostEnt = gethostbyaddr((char *)&inetAddress, sizeof(inetAddress), AF_INET);
        
        if ( hostEnt == NULL )
        {
            lprintf(LOG_ERROR,"gethostbyaddr(1) failed for %s\n", 
                    hostname.c_str());
        }
    } 
    else 
    {
        hostEnt=gethostbyname(hostname.c_str());

        if ( hostEnt == NULL ) 
        {
            lprintf(LOG_ERROR,"gethostbyname(2) failed for %s\n", 
                    hostname.c_str());
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
        lprintf(LOG_ERROR,"Can't connect to server.\n");
#else
        switch(errno) 
        {
            /* this should be made nicer */
        case ECONNREFUSED:
            lprintf(LOG_ERROR,"Connection refused.\n");
            break;
        default:
            lprintf(LOG_ERROR,"Can't connect to server.\n");
            break;
        }
#endif        
        return false;
    }
    connected = true;
    
    return true;
}

//------------------------------------------------------------------------
// Send 0x00 terminated ACSII string pointed to by 'buffer' 
// Returns number of bytes sent or -1 in case of error.

int 
GenericSocket::Send(char *buffer)
{
    if (buffer==NULL)
    {
        lprintf(LOG_FATAL,"CANNOT SEND: Provided buffer == NULL !!!");
        return -1;
    }
    
    return Send(buffer,strlen(buffer));
}

//------------------------------------------------------------------------
// Send 'len' bytes pointed to by 'buffer' 
// Returns number of bytes sent or -1 in case of error.

int 
GenericSocket::Send(char *buffer, unsigned int len)
{
    if (buffer==NULL)
    {
        lprintf(LOG_FATAL,"CANNOT SEND: Provided buffer == NULL !!!");
        return -1;
    }
        
    if (!connected)
    {
        lprintf(LOG_FATAL,"CANNOT SEND: NOT CONNECTED !!!");
        return -1;
    }

//    cout << "SENDING: \"" << buffer <<  "\"" << endl << flush;

    STAT_AddValue("Socket Send Bytes",len);
    sendBytes += len;

    lprintf(LOG_SOCKET,"Send %d bytes (total %d): \"%s\".",len,sendBytes,buffer);

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
        lprintf(LOG_FATAL,"CANNOT RECEIVE: Provided buffer == NULL !!!");
        return -1;
    }

    if (!connected)
    {
        lprintf(LOG_FATAL,"CANNOT RECEIVE: NOT CONNECTED !!!");
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
                    return byteCounter; 

            //------------
            default: // We got some kind of error condition.
                    return -1;
        } // switch
    } // while

    // In case someone descides to interpret this as an ASCII string we
    // put a NULL terminator at the end.
    *buffer = 0x00;

    STAT_AddValue("Socket Received Bytes",byteCounter);
    receivedBytes += byteCounter;

//    cout << "\r";
//    PrintStatistics(cout);
    
    lprintf(LOG_SOCKET,"Received %d bytes (total %d)",
                       byteCounter,receivedBytes,realBuffer);

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
    os << "Out " << sendBytes << " bytes, in " << receivedBytes << " bytes" 
       << " in " << usedTime << " seconds : "; 
    if (usedTime==0)
        os << (receivedBytes) << " bytes/second"; // We assume 1 second
    else
        os << (receivedBytes)/(usedTime) << " bytes/second";
}

//-------------------------------------------------------------------------

// End of the file GenericSocket.cpp
//=========================================================================
