//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPProxy.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/12/02 22:34:18 $
//  $Revision: 1.6 $
//  $RCSfile: NNTPProxy.cpp,v $
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

//--------------------------------------------------------------------

#include "TraceLog.h"
#include "SuckDefines.h"
#include "NNTPProxy.h"
#include "NNTPGetArticleCommand.h"
#include "StatisticsKeeper.h"

//--------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------

NNTPProxy::NNTPProxy(IniFile *settings)
{
    fSettings = settings;
    nntp = NULL;       // Not connected yet.
    currentGroup = ""; // No current group yet

    if (fSettings == NULL)
    {
        lprintf(LOG_ERROR,"NNTPProxy called with settings == NULL");
        return;
    }

    string newsServerName;
    
    if (!fSettings->GetValue(SUCK_CONFIG,SUCK_NEWS_SERVER,newsServerName))
        newsServerName = "news"; // Default value if nothing is specified

    long newsServerPort;
    if (!fSettings->GetValue(SUCK_CONFIG,SUCK_NNTP_PORT,newsServerPort))
        newsServerPort = 119; // Default value if nothing is specified

    // Create socket connection to server on the NNTP port
    nntp = new AsciiLineSocket(newsServerName,newsServerPort);
    
    if (nntp == NULL)
    {
        lprintf(LOG_ERROR,
            "Unable to connect to NNTP server on %s", newsServerName.c_str());
        return; // abort
    }

    // Get the server ID message
    string serverIdentification = "An unknown fatal error occurred.";
    
    if (nntp->GetResponse(serverIdentification) != 200)
    {
        cout << "Some error occurred connecting." << endl;
        delete (nntp);
        nntp = NULL;
    }

    bool sendModeReader = false;
    if (fSettings->GetValue(SUCK_CONFIG,SUCK_SEND_MODEREADER,sendModeReader))
    {
        if (sendModeReader)
            Send_MODE_READER();
    }

    string username = "";
    fSettings->GetValue(SUCK_CONFIG,SUCK_NNTP_USERNAME,username);

    if (username != "")
    {   // Only if a username is specified we login to the server.
        string password = "";
        fSettings->GetValue(SUCK_CONFIG,SUCK_NNTP_PASSWORD,password);
        
        Login(username,password);
    }


//    cout << serverIdentification << endl;
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
        lprintf(LOG_ERROR,"Not connected.");
        return false; // Not connected
    }
        
    nntp->Send("LIST\n");

    string responseLine;

    switch(nntp->GetResponse(responseLine))
    {
        case 215: // Ok, proceed
            break;
        case 499: // Server is dead and so is the connection
            delete nntp;
            nntp = NULL;
            cout << "ERROR Connection closed: " << responseLine << endl;
            return false;
        default:
            cout << "Error: " << responseLine << endl;
            return false; // Error
    }

    char groupName[500];
    unsigned long last;
    unsigned long first;
    char flags[500];

    string line;
    while (nntp->GetLine(line) && KeepRunning())
    {
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
        lprintf(LOG_ERROR,"Not connected.");
        return false; // Not connected
    }

//    cout << "Settinggroup :\"" << groupName << "\"." << endl;

    nntp->Sendf("GROUP %s\n",groupName.c_str());

    string responseLine;

    switch(nntp->GetResponse(responseLine))
    {
        case 211: // Ok, proceed
            break;
        case 411: // Group doesn't exist
            cout << "Error: " << responseLine << endl;
            return false;
        case 499: // Server is dead and so is the connection
            delete nntp;
            nntp = NULL;
            cout << "ERROR Connection closed: " << responseLine << endl;
            return false;
        default:
            cout << "Error: " << responseLine << endl;
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
        lprintf(LOG_ERROR,"Not connected.");
        return 0; // Not connected
    }
    
    GroupInfo groupInfo;
    if (!SetCurrentGroup(groupName,groupInfo))
    {
        cout << "ERROR : Unable to get XOVER for " << groupName << endl;
        return 0;
    }
    
    long first = groupInfo.first;
    long last  = groupInfo.last;
    
    if (startAtArticlenr != -1 && startAtArticlenr > first)
        first = startAtArticlenr;
    
    if (first > last)
    {
        cout << endl << "There are no new messages present in " << groupName << endl << flush;
        return 2; // No messages in this group at all.    
    }
    
    if (first == last)
        nntp->Sendf("XOVER %ld\n",first);
    else
        nntp->Sendf("XOVER %ld-%ld\n",first,last);
    
    string responseLine;
    
    switch(nntp->GetResponse(responseLine))
    {
        case 224: // Ok, proceed
            return 1;
        case 411: // Group doesn't exist
            cout << "Error (NO SUCH GROUP): " << responseLine << endl;
            break;
        case 499: // Server is dead and so is the connection
            delete nntp;
            nntp = NULL;
            cout << "ERROR Connection closed: " << responseLine << endl;
            break;
        case 501: // Didn't specify a range ?
            cout << "Error (INVALID/NO RANGE): " << responseLine << endl;
            break;
        default:
            cout << "Error (UNKNOWN): " << responseLine << endl;
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
    while (nntp->GetLine(line) && KeepRunning())
    {
        NEWSArticle * article = new NEWSArticle(groupName,line.c_str());
        newsArticles.push_back(article);
    }

//    if (!KeepRunning())
//    {
//      cout << endl << "Finishing XOVER command cleanly." << endl << flush;
//      while (nntp->GetLine(line));
//      cout << endl << "FINISHED XOVER command cleanly." << endl << flush;
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
        lprintf(LOG_ERROR,"No command handler specified.");
        return false; // Error
    }
    
    switch (COMMON_GetGroupOverview(groupName,startAtArticlenr))
    {
    case 0: return false;  // Error
    case 1: break; // Ok to continue
    case 2: return true; // Didn't start command because there are no messages
    }
    
    string line;
    while (nntp->GetLine(line) && KeepRunning())
    {
        STAT_AddValue("Articles Present",1);
        NEWSArticle * article = new NEWSArticle(groupName,line.c_str());
        commandHandler->AddCommand(new NNTPGetArticleCommand(article));
    }

//    if (!KeepRunning())
//    {
//      cout << endl << "Finishing XOVER command cleanly." << endl << flush;
//      while (nntp->GetLine(line));
//      cout << endl << "FINISHED XOVER command cleanly." << endl << flush;
//    }
    
    return true;
}

//--------------------------------------------------------------------

// Retrieves an article by ID
bool 
NNTPProxy::GetArticle (string /*articleId*/,string &/*article*/)
{
    lprintf(LOG_FATAL,"NOT IMPLEMENTED : NNTPProxy::GetArticle (string articleId,string &article)");
    return true;
}

//--------------------------------------------------------------------

// Retrieves an article by number in the group
bool 
NNTPProxy::GetArticle (int /*articleNumber*/,string &/*article*/)
{
    lprintf(LOG_FATAL,"NOT IMPLEMENTED : NNTPProxy::GetArticle (string articleNumber,string &article)");
    return true;
}

//--------------------------------------------------------------------

// Retrieves the header of an article
bool 
NNTPProxy::GetArticleHead(string articleId,string &articleHead)
{
    if (!IsConnected())
    {
        lprintf(LOG_ERROR,"Not connected.");
        return false; // Not connected
    }

    nntp->Sendf("HEAD %s\n",articleId.c_str());

    string responseLine;

    switch(nntp->GetResponse(responseLine))
    {
        case 221: // Ok, proceed
            break;
        case 411: // Group doesn't exist
            cout << "Error: " << responseLine << endl;
            return false;
        case 499: // Server is dead and so is the connection
            delete nntp;
            nntp = NULL;
            cout << "ERROR Connection closed: " << responseLine << endl;
            return false;
        default:
            cout << "Error: " << responseLine << endl;
            return false; // Error
    }

    articleHead="";

    string line;
    while (nntp->GetLine(line) && KeepRunning())
    {
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
        lprintf(LOG_ERROR,"Not connected.");
        return false; // Not connected
    }

    nntp->Sendf("BODY %s\n",articleId.c_str());

    string responseLine;

    switch(nntp->GetResponse(responseLine))
    {
        case 222: // Ok, proceed
            break;
        case 411: // Group doesn't exist
            cout << "Error: " << responseLine << endl;
            return false;
        case 499: // Server is dead and so is the connection
            delete nntp;
            nntp = NULL;
            cout << "ERROR Connection closed: " << responseLine << endl;
            return false;
        default:
            cout << "Error: " << responseLine << endl;
            return false; // Error
    }

    articleBody="";

    string line;
    while (nntp->GetLine(line) && KeepRunning())
    {
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
        lprintf(LOG_ERROR,"Not connected.");
        return false; // Not connected
    }

    nntp->Send("MODE READER\n");

    string responseLine;

    switch(nntp->GetResponse(responseLine))
    {
        case 200: // 200 Hello, you can post --> Ok, proceed
        case 201: // 201 Hello, you can't post --> Ok, proceed
            return true;

        default:
            delete nntp;
            nntp = NULL;
            cout << "ERROR Connection closed in response to MODE READER: " 
                 << responseLine << endl;
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
        lprintf(LOG_ERROR,"Not connected.");
        return false; // Not connected
    }

    printf("AUTHINFO USER %s\n",user.c_str());
    nntp->Sendf("AUTHINFO USER %s\n",user.c_str());

    string responseLine;
    int responseCode;

    responseCode = nntp->GetResponse(responseLine);
    if (responseCode >= 500)
    {  // Some kind of error
        delete nntp;
        nntp = NULL;
        cout << "ERROR Connection closed in response to AUTHINFO USER : " 
             << responseLine << endl;
        return false;
    }

    printf("AUTHINFO PASS %s\n",pass.c_str());
    nntp->Sendf("AUTHINFO PASS %s\n",pass.c_str());

    responseCode = nntp->GetResponse(responseLine);
    if (responseCode >= 500)
    {  // Some kind of error
        delete nntp;
        nntp = NULL;
        cout << "ERROR Connection closed in response to AUTHINFO PASS : " 
             << responseLine << endl;
        return false;
    }

    return true;
}

// End of the file NNTPProxy.cpp
//=========================================================================
