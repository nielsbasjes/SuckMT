//=========================================================================
//                   Copyright (C) 1999 by Niels Basjes
//                  Suck MT Website: http://go.to/suckmt
//                        Author: SuckMT@Basjes.nl
//-------------------------------------------------------------------------
//  Filename  : NNTPProxy.cpp
//  Sub-system: SuckMT, a multithreaded suck replacement
//  Language  : C++
//  $Date: 1999/09/29 20:12:39 $
//  $Revision: 1.3 $
//  $RCSfile: NNTPProxy.cpp,v $
//  $Author: niels $
//=========================================================================

#ifdef WIN32
#pragma warning( disable : 4786 ) 
#endif

//--------------------------------------------------------------------

#include "TraceLog.h"
#include "NNTPProxy.h"
#include "NNTPGetArticleCommand.h"
#include "StatisticsKeeper.h"

//--------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------

NNTPProxy::NNTPProxy (string serverName)
{
    // Create socket connection to server
    nntp = new AsciiLineSocket(serverName,119);
    
    if (nntp == NULL)
    {
        lprintf(LOG_ERROR,"Unable to connect to NNTP server on %s", serverName.c_str());
        return; // abort
    }

    currentGroup = ""; // No current group yet

    // Get the server ID message
    string serverIdentification = "An unknown fatal error occurred.";
    
    if (nntp->GetResponse(serverIdentification) != 200)
    {
        cout << "Some error occurred connecting." << endl;
        delete (nntp);
        nntp = NULL;
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
    if (nntp == NULL)
    {
        lprintf(LOG_ERROR,"Not connected.");
        return false; // Not connected
    }
        
    nntp->Send("LIST\n");

    string completeResponseLine;

    switch(nntp->GetResponse(completeResponseLine))
    {
        case 215: // Ok, proceed
                break;
        default:
            cout << "Error: " << completeResponseLine << endl;
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
    if (nntp == NULL)
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
    if (nntp == NULL)
    {
        lprintf(LOG_ERROR,"Not connected.");
        return 0; // Not connected
    }
	
    GroupInfo groupInfo;
    if (!SetCurrentGroup(groupName,groupInfo))
    {
        cout << "ERROR" << endl;
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
//		cout << endl << "Finishing XOVER command cleanly." << endl << flush;
//		while (nntp->GetLine(line));
//		cout << endl << "FINISHED XOVER command cleanly." << endl << flush;
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
//		cout << endl << "Finishing XOVER command cleanly." << endl << flush;
//		while (nntp->GetLine(line));
//		cout << endl << "FINISHED XOVER command cleanly." << endl << flush;
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
    if (nntp == NULL)
    {
        lprintf(LOG_ERROR,"Not connected.");
        return false; // Not connected
    }

    nntp->Sendf("HEAD %s\n",articleId.c_str());

    string completeResponseLine;

    switch(nntp->GetResponse(completeResponseLine))
    {
        case 221: // Ok, proceed
            break;
        case 411: // Group doesn't exist
            cout << "Error: " << completeResponseLine << endl;
            return false;
        default:
            cout << "Error: " << completeResponseLine << endl;
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
    if (nntp == NULL)
    {
        lprintf(LOG_ERROR,"Not connected.");
        return false; // Not connected
    }

    nntp->Sendf("BODY %s\n",articleId.c_str());

    string completeResponseLine;

    switch(nntp->GetResponse(completeResponseLine))
    {
        case 222: // Ok, proceed
            break;
        case 411: // Group doesn't exist
            cout << "Error: " << completeResponseLine << endl;
            return false;
        default:
            cout << "Error: " << completeResponseLine << endl;
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

// End of the file NNTPProxy.cpp
//=========================================================================
