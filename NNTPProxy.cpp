//=========================================================================
//                 Copyright (C)1999-2003 by Niels Basjes
//              SuckMT Website : http://oss.basjes.nl/SuckMT/
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPProxy.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 2003/04/13 20:51:55 $
//  $Revision: 1.25 $
//  $RCSfile: NNTPProxy.cpp,v $
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

//--------------------------------------------------------------------

#include "TraceLog.h"
#include "SuckDefines.h"
#include "NNTPProxy.h"
#include "NNTPGetArticleCommand.h"
#include "StatisticsKeeper.h"

//--------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------

int NNTPProxy::Next_ID = 1;

NNTPProxy::NNTPProxy(IniFile *settings)
{
    fSettings = settings;
    fConnectionNr = Next_ID++;
    nntp = NULL;       // Not connected yet.
    currentGroup = ""; // No current group yet

    if (fSettings == NULL)
    {
        Lfatal << "NNTPProxy called with settings == NULL" << endl << flush;
        return;
    }

    //----------
    string newsServerName;
    if (!fSettings->GetValue(SUCK_CONFIG,SUCK_NEWS_SERVER,newsServerName))
        newsServerName = "news"; // Default value if nothing is specified

    //----------
    long newsServerPort;
    if (!fSettings->GetValue(SUCK_CONFIG,SUCK_NNTP_PORT,newsServerPort))
        newsServerPort = 119; // Default value if nothing is specified

    //----------
    // Create socket connection to server on the NNTP port
    nntp = new AsciiLineSocket(newsServerName,newsServerPort,fConnectionNr);
    
    if (nntp == NULL || !nntp->IsConnected())
    {
        Lerror << "Unable to connect to NNTP server on \"" 
               << newsServerName.c_str() << "\"." << endl << flush;
        return; // abort
    }

    //----------
    // En/Disable the socket debug info
    bool showSocketDebug;
    if (!fSettings->GetValue(SUCK_CONFIG,SUCK_DEBUG_SOCKET,showSocketDebug))
        showSocketDebug = false; // Default value if nothing is specified

    nntp->DebugSocketCommands(showSocketDebug);

    //----------
    // Get the server ID message
    string serverIdentification = "An unknown fatal error occurred.";

    switch (nntp->GetResponse(serverIdentification))
    {
    case 200: // Posting Ok
        break;
    case 201: // No Posting ... also Ok
        break;
    default:  // Anything else ... not Ok
        Linfo << "Connection " << fConnectionNr 
              << " FAILED: \"" 
              << serverIdentification << "\"" << endl << flush;
        delete (nntp);
        nntp = NULL;
        break;
    }

    //----------
    bool sendModeReader = false;
    if (fSettings->GetValue(SUCK_CONFIG,SUCK_SEND_MODEREADER,sendModeReader))
    {
        if (sendModeReader)
            Send_MODE_READER();
    }

    //----------
    string username = "";
    fSettings->GetValue(SUCK_CONFIG,SUCK_NNTP_USERNAME,username);

    if (username != "")
    {   // Only if a username is specified we login to the server.
        string password = "";
        fSettings->GetValue(SUCK_CONFIG,SUCK_NNTP_PASSWORD,password);
        
        Login(username,password);
    }

    //----------
    fKeep_CarriageReturn = true; // The default if nothing specified
    fSettings->GetValue(SUCK_CONFIG,SUCK_KEEP_CR,fKeep_CarriageReturn);
    
    //----------
    Linfo << "Connection " << fConnectionNr 
          << " has been established." << endl << flush;
}

//--------------------------------------------------------------------

NNTPProxy::~NNTPProxy ()
{
    if (nntp != NULL)
    {
        delete(nntp);
    }
}

//--------------------------------------------------------------------

bool
NNTPProxy::IsConnected()
{
    return (nntp != NULL && nntp->IsConnected());
}

//--------------------------------------------------------------------

ostream& 
operator<< (ostream &os, const GroupInfo &group)
{
    os << group.name
       << " [" << group.first
       << " ... " << group.last << "] "
       << group.flags;
    return os;
}

//--------------------------------------------------------------------

ostream& 
operator<< (ostream &os, const vector<GroupInfo> &groups)
{
    vector<GroupInfo>::const_iterator itemIter;

    for(itemIter  = groups.begin();
        itemIter != groups.end();
        itemIter ++)
    {
        os << (*itemIter) << endl;
    }

    return os;
}

//--------------------------------------------------------------------

bool 
NNTPProxy::GetGroups(vector<GroupInfo> &groups)
{
    if (!IsConnected())
    {
        Lerror << "Not connected." << endl << flush;
        return false; // Not connected
    }
        
    nntp->SendCommand("LIST");

    string responseLine;

    switch(nntp->GetResponse(responseLine))
    {
        case 215: // Ok, proceed
            break;
        case 499: // Server is dead and so is the connection
            delete nntp;
            nntp = NULL;
            Lfatal << "ERROR Connection closed: " << responseLine << endl << flush;
            return false;
        default:
            delete nntp;
            nntp = NULL;
            Lfatal << "Unknown error Connection closed: " << responseLine << endl << flush;
            return false; // Error
    }

    char groupName[500];
    unsigned long last;
    unsigned long first;
    char flags[500];

    string line;
    while (nntp->GetLine(line,false,fKeep_CarriageReturn) && KeepRunning())
    {
        // Serious error ? 
        if (!IsConnected())
        {
            delete nntp;
            nntp = NULL;
            Lfatal << "ERROR Connection closed during GetGroups." << endl << flush;
            return false;
        }
            
        sscanf(line.c_str(),"%s %ld %ld %s",groupName,&last,&first,flags);
        GroupInfo aGroup;
        aGroup.name  = groupName;
        aGroup.last  = last;
        aGroup.first = first;
        aGroup.flags = flags;
        groups.push_back(aGroup);
    } 

    return true;

}

//--------------------------------------------------------------------

// Sets the current group
bool 
NNTPProxy::SetCurrentGroup(string groupName,GroupInfo &groupInfo)
{
    if (!IsConnected())
    {
        Lerror << "Not connected." << endl << flush;
        return false; // Not connected
    }

    Lcalls << "Settinggroup :\"" << groupName << "\"." << endl << flush;

    nntp->SendCommandf("GROUP %s",groupName.c_str());

    string responseLine;

    switch(nntp->GetResponse(responseLine))
    {
        case 211: // Ok, proceed
            break;
        case 411: // Group doesn't exist
            Lerror << "NO SUCH GROUP: " << responseLine << endl << flush;
            return false;
        case 499: // Server is dead and so is the connection
            delete nntp;
            nntp = NULL;
            Lfatal << "ERROR Connection closed: " << responseLine << endl << flush;
            return false;
        default:
            delete nntp;
            nntp = NULL;
            Lfatal << "Unknown error Connection closed: " << responseLine << endl << flush;
            return false; // Error
    }

    unsigned long responseCode;
    unsigned long estimatedNrOfMessages;
    unsigned long last;
    unsigned long first;
    char flags[500];

    sscanf(responseLine.c_str(),"%ld %ld %ld %ld %s",
           &responseCode,&estimatedNrOfMessages,&first,&last,flags);
    groupInfo.name  = groupName;
    groupInfo.last  = last;
    groupInfo.first = first;
    groupInfo.flags = flags;
    groupInfo.last_we_have = 0; // This means we have no messages

    currentGroup = groupName;
    
    return true;
}

//--------------------------------------------------------------------

// Implements the common part of both versions of GetGroupOverview
// returns 0 if error else returns 1 if OK or 2 if no messages.
int 
NNTPProxy::COMMON_GetGroupOverview(string groupName, long startAtArticlenr)
{
    if (!IsConnected())
    {
        Lerror << "Not connected." << endl << flush;
        return 0; // Not connected
    }
    
    GroupInfo groupInfo;
    if (!SetCurrentGroup(groupName,groupInfo))
    {
        Lerror << "Unable to get XOVER for " << groupName << endl << flush;
        return 0;
    }
    
    long first = groupInfo.first;
    long last  = groupInfo.last;
    
    if (startAtArticlenr != -1 && startAtArticlenr > first)
        first = startAtArticlenr;
    
    if (first > last)
    {
//        Linfo << "There are no new messages present in " << groupName << endl << flush;
        return 2; // No messages in this group at all.    
    }

    long maximumMessages          = -1;
    long maximumMessagesDownload  = -1;
    if (fSettings->GetValue(SUCK_GLOBAL_KILL_RULES,
                            SUCK_MAX_MSG_THRESHOLD,
                            maximumMessages))
    {
        if (maximumMessages > 0 && (last - maximumMessages > first))
        {
            if (!fSettings->GetValue(SUCK_GLOBAL_KILL_RULES,
                                     SUCK_MAX_MSG_DOWNLOAD,
                                     maximumMessagesDownload))
                maximumMessagesDownload = maximumMessages;
            
            Linfo << "Downloading only the last " << maximumMessagesDownload 
                  << " messages of " << last - first << " messages available in " << groupName 
                  << " (threshold is " << maximumMessages << ")." << endl << flush;
            first = last - maximumMessagesDownload;
        }
    }
    
    if (first == last)
        nntp->SendCommandf("XOVER %ld",first);
    else
        nntp->SendCommandf("XOVER %ld-%ld",first,last);
    
    string responseLine;
    
    switch(nntp->GetResponse(responseLine))
    {
        case 224: // Ok, proceed
            return 1;
        case 411: // Group doesn't exist
            Lerror << "NO SUCH GROUP: " << responseLine << endl << flush;
            break;
        case 499: // Server is dead and so is the connection
            delete nntp;
            nntp = NULL;
            Lfatal << "ERROR Connection closed: " << responseLine << endl << flush;
            break;
        case 501: // Didn't specify a range ?
            Lerror << "INVALID/NO RANGE: " << responseLine << endl << flush;
            break;
        default:
            delete nntp;
            nntp = NULL;
            Lfatal << "Unknown error Connection closed: " << responseLine << endl << flush;
            break;
    }
    return 0;
}

//--------------------------------------------------------------------

// Retrieves the complete list of the overview database
bool 
NNTPProxy::GetGroupOverview (string groupName, vector<NEWSArticle*>  &newsArticles, long startAtArticlenr)
{
    switch (COMMON_GetGroupOverview(groupName,startAtArticlenr))
    {
    case 0: return false;  // Error
    case 1: break; // Ok to continue
    case 2: return true; // Didn't start command because there are no messages
    }

    string line;
    while (nntp->GetLine(line,false,fKeep_CarriageReturn) && KeepRunning())
    {
        // Serious error ? 
        if (!IsConnected())
        {
            delete nntp;
            nntp = NULL;
            Lfatal << "ERROR Connection closed during GetGroupOverview." << endl << flush;
            return false;
        }
        
        NEWSArticle * article = new NEWSArticle(groupName,line.c_str());
        newsArticles.push_back(article);
    }

//    if (!KeepRunning())
//    {
//      Linfo << "Finishing XOVER command cleanly." << endl << flush;
//      while (nntp->GetLine(line,false,fKeep_CarriageReturn));
//      Linfo << "FINISHED XOVER command cleanly." << endl << flush;
//    }

    return true;
}

//--------------------------------------------------------------------
// Retrieves the complete list of the overview database
// Instead of storing the articles in a vector they are stored
// with the specified commandhandler
bool 
NNTPProxy::GetGroupOverview(string groupName, NNTPCommandHandler *commandHandler, long startAtArticlenr)
{
    if (commandHandler == NULL)
    {
        Lerror << "No command handler specified." << endl << flush;
        return false; // Error
    }
    
    switch (COMMON_GetGroupOverview(groupName,startAtArticlenr))
    {
    case 0: return false;  // Error
    case 1: break; // Ok to continue
    case 2: return true; // Didn't start command because there are no messages
    }
    
    string line;
    while (nntp->GetLine(line,false,fKeep_CarriageReturn) && KeepRunning())
    {
        // Serious error ? 
        if (!IsConnected())
        {
            delete nntp;
            nntp = NULL;
            Lfatal << "ERROR Connection closed during GetGroupOverview." << endl << flush;
            return false;
        }

        STAT_AddValue("Articles Present",1);
        NEWSArticle * article = new NEWSArticle(groupName,line.c_str());
        commandHandler->AddCommand(new NNTPGetArticleCommand(article));
    }

//    if (!KeepRunning())
//    {
//      Linfo << "Finishing XOVER command cleanly." << endl << flush;
//      while (nntp->GetLine(line,false,fKeep_CarriageReturn));
//      Linfo << "FINISHED XOVER command cleanly." << endl << flush;
//    }
    
    return true;
}

//--------------------------------------------------------------------

// Retrieves an article by ID
bool 
NNTPProxy::GetArticle (string /*articleId*/,string &/*article*/)
{
    Lfatal << "NOT IMPLEMENTED : NNTPProxy::GetArticle (string articleId,string &article)" << endl << flush;
    return true;
}

//--------------------------------------------------------------------

// Retrieves an article by number in the group
bool 
NNTPProxy::GetArticle (int /*articleNumber*/,string &/*article*/)
{
    Lfatal << "NOT IMPLEMENTED : NNTPProxy::GetArticle (string articleNumber,string &article)" << endl << flush;
    return true;
}

//--------------------------------------------------------------------

// Retrieves the header of an article
bool 
NNTPProxy::GetArticleHead(string articleId,string &articleHead)
{
    if (!IsConnected())
    {
        Lerror << "Not connected." << endl << flush;
        return false; // Not connected
    }

    nntp->SendCommandf("HEAD %s",articleId.c_str());

    string responseLine;

    switch(nntp->GetResponse(responseLine))
    {
        case 221: // Ok, proceed
            break;
        case 411: // Group doesn't exist
            Lerror << "NO SUCH GROUP: " << responseLine << endl << flush;
            return false;
        case 423: // Article number doesn't exist in this group (probably bad overview DB)
            Lerror << "NO SUCH ARTICLE NUMBER: " << responseLine << endl << flush;
            return false;
        case 430: // Article doesn't exist (probably bad overview DB)
            Lerror << "NO SUCH ARTICLE: " << responseLine << endl << flush;
            return false;
        case 499: // Server is dead and so is the connection
            delete nntp;
            nntp = NULL;
            Lfatal << "ERROR Connection closed: " << responseLine << endl << flush;
            return false;
        default:
            delete nntp;
            nntp = NULL;
            Lfatal << "Unknown error Connection closed: " << responseLine << endl << flush;
            return false; // Error
    }

    articleHead="";

    string line;
    while (nntp->GetLine(line,true,fKeep_CarriageReturn) && KeepRunning())
    {
        // Serious error ? 
        if (!IsConnected())
        {
            delete nntp;
            nntp = NULL;
            Lfatal << "ERROR Connection closed during GetArticleHead." << endl << flush;
            return false;
        }
        
        articleHead += line;
    } 

    return true;

}

//--------------------------------------------------------------------

// Retrieves the body of an article
bool 
NNTPProxy::GetArticleBody(string articleId,string &articleBody)
{
    if (!IsConnected())
    {
        Lerror << "Not connected." << endl << flush;
        return false; // Not connected
    }

    nntp->SendCommandf("BODY %s",articleId.c_str());

    string responseLine;

    switch(nntp->GetResponse(responseLine))
    {
        case 222: // Ok, proceed
            break;
        case 411: // Group doesn't exist
            Lerror << "NO SUCH GROUP: " << responseLine << endl << flush;
            return false;
        case 423: // Article number doesn't exist in this group (probably bad overview DB)
            Lerror << "NO SUCH ARTICLE NUMBER: " << responseLine << endl << flush;
            return false;
        case 430: // Article doesn't exist (probably bad overview DB)
            Lerror << "NO SUCH ARTICLE: " << responseLine << endl << flush;
            return false;
        case 499: // Server is dead and so is the connection
            delete nntp;
            nntp = NULL;
            Lfatal << "ERROR Connection closed: " << responseLine << endl << flush;
            return false;
        default:
            delete nntp;
            nntp = NULL;
            Lfatal << "Unknown error Connection closed: " << responseLine << endl << flush;
            return false; // Error
    }

    articleBody="";

    string line;
    while (nntp->GetLine(line,true,fKeep_CarriageReturn) && KeepRunning())
    {
        // Serious error ? 
        if (!IsConnected())
        {
            delete nntp;
            nntp = NULL;
            Lfatal << "ERROR Connection closed during GetArticleBody." << endl << flush;
            return false;
        }
        
        articleBody += line;
    } 

    return true;
}

//--------------------------------------------------------------------

// Retrieves the header of an article
bool
NNTPProxy::GetArticleHead(NEWSArticle * article)
{
    if (article == NULL)
        return false;
    string theHeader;
    bool get_status = GetArticleHead(article->fMessageID,theHeader);
    
    if (get_status == false)
        return false;
        
    return article->StoreHeader(theHeader);
}

//--------------------------------------------------------------------

// Retrieves the body of an article
bool
NNTPProxy::GetArticleBody(NEWSArticle * article)
{
    if (article == NULL)
        return false;
    string theBody;
    bool get_status = GetArticleBody(article->fMessageID,theBody);
    
    if (get_status == false)
        return false;
        
    return article->StoreBody(theBody);
}

//--------------------------------------------------------------------
// Sends MODE READER to the server.
// Returns true if success.
// Returns false if the server disconnected.
bool
NNTPProxy::Send_MODE_READER()
{
    if (!IsConnected())
    {
        Lerror << "Not connected." << endl << flush;
        return false; // Not connected
    }

    nntp->SendCommand("MODE READER");

    string responseLine;

    switch(nntp->GetResponse(responseLine))
    {
        case 200: // 200 Hello, you can post --> Ok, proceed
        case 201: // 201 Hello, you can't post --> Ok, proceed
            return true;

        default:
            delete nntp;
            nntp = NULL;
            Lfatal << "Connection closed in response to MODE READER: " 
                   << responseLine << endl << flush;
            return false;
    }

    return true;
}

//--------------------------------------------------------------------

// Tries to login to the server
// Returns true if success
// Returns false if failed ---> also disconnected.
bool
NNTPProxy::Login(string user,string pass)
{
    if (!IsConnected())
    {
        Lerror << "Not connected." << endl << flush;
        return false; // Not connected
    }

    nntp->SendCommandf("AUTHINFO USER %s",user.c_str());

    string responseLine;
    int responseCode;

    responseCode = nntp->GetResponse(responseLine);
    if (responseCode >= 500)
    {  // Some kind of error
        delete nntp;
        nntp = NULL;
        Lfatal << "Connection closed in response to AUTHINFO USER : " 
               << responseLine << endl << flush;
        return false;
    }

    nntp->SendCommandf("AUTHINFO PASS %s",pass.c_str());

    responseCode = nntp->GetResponse(responseLine);
    if (responseCode >= 500)
    {  // Some kind of error
        delete nntp;
        nntp = NULL;
        Lfatal << "Connection closed in response to AUTHINFO PASS : " 
               << responseLine << endl << flush;
        return false;
    }

    return true;
}

// End of the file NNTPProxy.cpp
//=========================================================================
